
// Changed web server code to use chunked responses (in some spots), so we do not have to do BIG String concatenations (to conserve memory).
// Another option would be to use https://github.com/me-no-dev/ESPAsyncWebServer templating.

// The page_start contains the head, styles, body-start-tag, and content-div-start-tag
const PROGMEM char page_start[] = R"rawliteral(<!DOCTYPE html>
<html>
<head>
<title>LEDCircleClock</title>
<style>
  body {
    background: #fad7a0;
    color: #154360;
    padding: 20px;
    font-size: 2em;
    text-align: center;
  }
  .effects input { position: initial !important; width: 200px !important; float: left; margin: 4px; }
  .effects { width: 100%; left: auto; font-size: 30px; }
  div.container {
    display: inline-block;
    width: 90%;
    height: 90%;
    background: #f8c471;
    box-shadow: 15px 20px 20px #88888888;
    border-radius: 20px;
    padding: 2%;
    text-align: left;
  }
  h1 {
    margin-top: 0;
  }
  input {
    width: 100%;
    border: 0;
    border-bottom: 2px solid grey;
    background: none;
    color: #154360;
    font-size: 1.2em;
  }
  input[type="range"] {
    width: 100%;
  }
  input[type="submit"] {
    background: #154360;
    color: #fad7a0;
    border: 0;
    border-radius: 5px;
    width: 40%;
    height: 2em;
    cursor: pointer;
    font-size: 0.8em;
    position: absolute;
    left: 30%;
    bottom: 20%;
  }
  .nextline {
    clear: both;
    padding-top: 1em;
  }
</style>
</head>
<body>
    <div class="container">
)rawliteral";

// The page_start contains the content-div-end-tag, and body-end-tag
const PROGMEM char page_end[] = R"rawliteral(
    </div>
</body>
</html>
)rawliteral";

void handleRoot() {
  if (server.method() == HTTP_POST) {
    for (uint16_t pixel = 0; pixel < PIXEL_COUNT; pixel++) {
      strip.SetPixelColor(pixel, RgbColor(0, 50, 50));
      strip.Show();
    }
    delay(500);

    brightness = server.arg("brightness").toInt();

    // Store values in EEProm
    EEPROM.put(BRIGHTNESS_ADDR, brightness);
    EEPROM.commit();

    clearStrip();
  }

  // Read back to check if the values are stored correctly
  EEPROM.get(BRIGHTNESS_ADDR, brightness);

  char pauseLabel[8];
  char statusMessage[20];
  if (demoMode==-2) {
    strcpy(pauseLabel, "&#x25B6;&#xFE0F;"); // play
    strcpy(statusMessage, "effect-pause");
  } else if (demoMode==-1) {
    strcpy(pauseLabel, "&#x23F8;"); // pause
    strcpy(statusMessage, "random-effects");
  } else {
    strcpy(pauseLabel, "&#x23F9;"); // stop
    strcpy(statusMessage, "demo-run");
  }

  server.chunkedResponseModeStart(200, "text/html");
  server.sendContent_P(page_start);

  server.sendContent("\
      <h1>Settings</h1>\
      <form method=\"POST\" action=\"/\">\
        Brightness:<br>\
        <input type=\"range\" name=\"brightness\" min=\"2\" max=\"255\" value=\"" + String(brightness) + "\"><br><br>\
        <input type=\"submit\" value=\"Submit\" style=\"position: inherit;\">\
      </form>\
      <div style='margin-top: 1em;'>Show Effect: (mode = " + String(statusMessage)+ ")</div>\
      <div class='effects'>\n");

  for (int i=0; i< getNrOfEffects(); i++) {
    server.sendContent(
      "<form method='post' action='/effect'><input type='hidden' name='effect' value='" + String(i) + "'><input type='submit' value='" + String(effectNames[i]) + "'></form>\n"
    );
  }

  server.sendContent("\
        <form method='post' action='/demo' class='nextline'><input type='submit' value='Demo all'></form>\
        <form method='post' action='/toggle-pause'><input type='submit' value='" + String(pauseLabel) + "'></form>\
        <form method='get' action='/color'><input type='submit' value='color-picker'></form>\
        <form method='get' action='/wifi'><input type='submit' value='network'></form>\
      </div>");

  server.sendContent_P(page_end);
  server.chunkedResponseFinalize();
}

void handleEffect() {
  // let main loop know which effect we want to see, but return web response immediately.
  triggerEffect = server.arg("effect").toInt();
  // disable demo mode, if it was active
  demoMode = -1;
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

void handleDemoMode() {
  // Enable demo mode, this will cycle through all effects in a row.
  demoMode = 0;
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

void handleTogglePause() {
  if (demoMode==-2) {
    // Re-enable random animation selection.
    demoMode = -1;
  } else if (demoMode==-1) {
    // Disable random animation selection.
    demoMode = -2;
  } else {
    // End demo-mode.
    demoMode = -1;
  }
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

void handleWifi() {
  // First check if the user has been authenticated.
  // You need to use "admin" as username, and the otaPassword as password.
  if (adminOtaCredentialsFailCount>MAX_CREDENTIAL_FAILS) {
    server.send(200, "text/html", "Too many login failures, reboot clock to try again...");
  }
  if (!server.authenticate("admin", otaPassword)) {
    adminOtaCredentialsFailCount++;
    return server.requestAuthentication(BASIC_AUTH);
  }
  adminOtaCredentialsFailCount=0;
  
  if (server.method() == HTTP_POST) {
    for (uint16_t pixel = 1; pixel < PIXEL_COUNT; pixel++) {
      strip.SetPixelColor(pixel-1, RgbColor(0, 0, 0));
      strip.SetPixelColor(pixel, RgbColor(50, 50, 0));
      strip.Show();
    }
    delay(500);

    snprintf(ssid, sizeof(ssid), server.arg("ssid").c_str());
    snprintf(wifiPassword, sizeof(wifiPassword), server.arg("password").c_str());
    snprintf(otaPassword, sizeof(otaPassword), server.arg("otaPassword").c_str());

    // Store values in EEProm
    EEPROM.put(SSID_ADDR, ssid);
    EEPROM.put(WIFI_PASSWORD_ADDR, wifiPassword);
    EEPROM.put(OTA_PASSWORD_ADDR, otaPassword);
    EEPROM.commit();

    clearStrip();
  }

  // Read back to check if the values are stored correctly
  EEPROM.get(SSID_ADDR, ssid);
  EEPROM.get(WIFI_PASSWORD_ADDR, wifiPassword);
  EEPROM.get(OTA_PASSWORD_ADDR, otaPassword);

  server.chunkedResponseModeStart(200, "text/html");
  server.sendContent_P(page_start);

  server.sendContent("\
    <h1>Network Settings</h1>\
      <form method=\"POST\" action=\"/wifi\">\
        WiFi SSID:<br>\
        <input type=\"text\" name=\"ssid\" value=\"" + String(ssid) + "\"><br><br>\
        WiFi Password:<br>\
        <input type=\"text\" name=\"password\" value=\"" + String(wifiPassword) + "\"><br><br>\
        OTA/admin Password:<br>\
        <input type=\"text\" name=\"otaPassword\" value=\"" + String(otaPassword) + "\"><br><br>\
        (restart device to activate updates)<br>\
        <input type=\"submit\" value=\"Submit\" style=\"position: inherit;\">\
        <input type=\"submit\" value=\"Cancel\" style=\"position: inherit;\" onclick=\"document.location.href='/';return false;\">\
      </form>");

  server.sendContent_P(page_end);
  server.chunkedResponseFinalize();
}
