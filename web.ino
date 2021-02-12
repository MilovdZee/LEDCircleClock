
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

  String settingsForm = String(CSS) + "<html>\
  <body>\
    <div class=\"container\">\
      <h1>Settings</h1>\
      <form method=\"POST\" action=\"/\">\
        Brightness:</br>\
        <input type=\"range\" name=\"brightness\" min=\"2\" max=\"255\" value=\"" + String(brightness) + "\"></br></br>\
        <input type=\"submit\" value=\"Submit\">\
      </form>\
      <div><a href=\"/wifi\">wifi</a></div>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", settingsForm);
}

void handleWifi() {
  if (server.method() == HTTP_POST) {
    for (uint16_t pixel = 0; pixel < PIXEL_COUNT; pixel++) {
      strip.SetPixelColor(pixel, RgbColor(50, 50, 0));
      strip.Show();
    }
    delay(500);

    snprintf(ssid, sizeof(ssid), server.arg("ssid").c_str());
    snprintf(wifiPassword, sizeof(wifiPassword), server.arg("password").c_str());

    // Store values in EEProm
    EEPROM.put(SSID_ADDR, ssid);
    if (String(wifiPassword).length() > 0) EEPROM.put(WIFI_PASSWORD_ADDR, wifiPassword);
    EEPROM.commit();

    clearStrip();
  }

  // Read back to check if the values are stored correctly
  EEPROM.get(SSID_ADDR, ssid);
  EEPROM.get(WIFI_PASSWORD_ADDR, wifiPassword);

  String settingsForm = String(CSS) + "<html>\
  <body>\
    <div class=\"container\">\
    <h1>WiFi Settings</h1>\
      <form method=\"POST\" action=\"/wifi\">\
        SSID:</br>\
        <input type=\"text\" name=\"ssid\" value=\"" + String(ssid) + "\"></br></br>\
        Password:</br>\
        <input type=\"text\" name=\"password\" value=\"" + String(wifiPassword) + "\">\
        <input type=\"submit\" value=\"Submit\">\
      </form>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", settingsForm);
}
