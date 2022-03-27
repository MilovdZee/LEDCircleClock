void setupOTA() {
  // Setup OTA updates
  ArduinoOTA.setPort(8266); // default 8266
  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%s_%d", HOSTNAME, random(9999));
  ArduinoOTA.setHostname(buffer);

  EEPROM.get(OTA_PASSWORD_ADDR, otaPassword);
  if (String(otaPassword).length() == 0 || String(otaPassword).length() >= (sizeof(otaPassword)-1)) {
    ArduinoOTA.setPassword(PASSWORD);
    strcpy(otaPassword, PASSWORD);
  } else {
    ArduinoOTA.setPassword(otaPassword);
  }

  ArduinoOTA.onStart([]() {
    otaActive = true;
    Serial.println("OTA update start");
    strip.SetPixelColor(0, RgbColor(0, 0, 50));
    strip.Show();
  });
  ArduinoOTA.onEnd([]() {
    otaActive = false;
    Serial.println("OTA update end");
    strip.SetPixelColor(0, RgbColor(0, 50, 0));
    strip.Show();
  });
  ArduinoOTA.onProgress([](int progress, int total) {
    otaActive = true;
    Serial.printf("OTA update progress: %u\r\n", progress * 100 / total);
    // Start 5 LEDs before the percentage. This is needed because the jump could be more then one LED.
    // 5 is just an arbitrary number and works up to a couple of hundred LEDs.
    int startLED = max(1, PIXEL_COUNT * progress / total - 5);
    int endLED = PIXEL_COUNT * progress / total;
    for (int led = startLED; led <= endLED; led++) {
      strip.ClearTo(RgbColor(0, 0, 0));
      strip.SetPixelColor(led, RgbColor(0, 50, 0));
    }
    strip.Show();
  });
  ArduinoOTA.onError([](ota_error_t error) {
    otaActive = false;
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      adminOtaCredentialsFailCount++;
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    for (int led = 0; led < ringSizes[0] + ringSizes[1]; led++) {
      strip.SetPixelColor(led, RgbColor(50, 0, 0));
    }
    strip.Show();
    delay(1000);
  });
  ArduinoOTA.begin();
}

void handleOta() {
  // Only handle OTA, if not too many failed login attempts have been done.
  if (adminOtaCredentialsFailCount<=MAX_CREDENTIAL_FAILS) {
    ArduinoOTA.handle();
  }
}
