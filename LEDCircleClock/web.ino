
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

  String effectLinks = "";
  for (int i=0; i< getNrOfEffects(); i++) {
    effectLinks += "<form method='post' action='/effect'><input type='hidden' name='effect' value='" + String(i) + "'><input type='submit' value='" + String(i) + "'></form>";
  }

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

  String settingsForm = "<html><head>" + String(CSS) + "</head>\
  <body>\
    <div class=\"container\">\
      <h1>Settings</h1>\
      <form method=\"POST\" action=\"/\">\
        Brightness:<br>\
        <input type=\"range\" name=\"brightness\" min=\"2\" max=\"255\" value=\"" + String(brightness) + "\"><br><br>\
        <input type=\"submit\" value=\"Submit\" style=\"position: inherit;\">\
      </form>\
      Show Effect: (mode = " + String(statusMessage)+ ")<div class='effects'>" + effectLinks + "\
        <form method='post' action='/demo'><input type='submit' value='ALL'></form>\
        <form method='post' action='/toggle-pause'><input type='submit' value='" + String(pauseLabel) + "'></form>\
      </div>\
      <div class='wifimenudiv'><a href=\"/wifi\">network</a></div>\
      <div class='colormenudiv'><a href=\"/color\">color-picker</a></div>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", settingsForm);
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

  String settingsForm = "<html><head>" + String(CSS) + "</head>\
  <body>\
    <div class=\"container\">\
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
      </form>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", settingsForm);
}
