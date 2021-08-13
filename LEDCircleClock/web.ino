
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

  String settingsForm = "<html><head>" + String(CSS) + "</head>\
  <body>\
    <div class=\"container\">\
      <h1>Settings</h1>\
      <form method=\"POST\" action=\"/\">\
        Brightness:<br>\
        <input type=\"range\" name=\"brightness\" min=\"2\" max=\"255\" value=\"" + String(brightness) + "\"><br><br>\
        <input type=\"submit\" value=\"Submit\" style=\"position: inherit;\">\
      </form>\
      Show Effect: <div class='effects'>" + effectLinks + "\
        <form method='post' action='/demo'><input type='submit' value='ALL'></form>\
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
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

void handleDemoMode() {
  // Enable demo mode, this will cycle through all effects in a row.
  demoMode = 0;
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

void handleWifi() {
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
        OTA Password:<br>\
        <input type=\"text\" name=\"otaPassword\" value=\"" + String(otaPassword) + "\"><br><br>\
        (restart device to activate updates)<br>\
        <input type=\"submit\" value=\"Submit\" style=\"position: inherit;\">\
      </form>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", settingsForm);
}
