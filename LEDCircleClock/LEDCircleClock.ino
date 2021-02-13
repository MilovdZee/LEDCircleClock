#include <NeoPixelBus.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <EEPROM.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "defines.h"

// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, TIME_OFFSET);

int previousEffectSecond = -1;

int previousClockSecond = -1;
int millisOffset = 0; // Offset compared to millis() to get partial seconds in sync with the NTP seconds

char ssid[60];
char wifiPassword[60];

// Number of rings
#define RINGS 9

// Number of LEDs per ring
int ringSizes[] = {1, 8, 12, 16, 24, 32, 40, 48, 60};

// The higher the power the narrower the region that lights up
int ringPowers[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

// First LED number of a ring
int startLEDs[RINGS];
int totalLEDs;

const int PIXEL_COUNT = 241; // make sure to set this to the number of pixels in your strip
int brightness; // Brightness between 0 and 255

// For Esp8266, the Pin is omitted and it uses GPIO3(Rx) due to DMA hardware use.
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXEL_COUNT);

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void clearStrip() {
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
}

void handlingDelay(int delayMillis) {
  timeClient.update();
  ArduinoOTA.handle();
  server.handleClient();

  // Make sure the software watchdog does not trigger
  ESP.wdtFeed();

  if (delayMillis > 0) delay(delayMillis);
}

void setPixel(int pixelNumber, RgbColor color) {
  int red = min((int)color.R, MAX_BRIGHTNESS);
  int green = min((int)color.G, MAX_BRIGHTNESS);
  int blue = min((int)color.B, MAX_BRIGHTNESS);
  RgbColor limitedColor = RgbColor(red, green, blue);
  strip.SetPixelColor(pixelNumber, limitedColor);
}

void setRandomSeed() {
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  // Serial.println(seed);
  randomSeed(seed);
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial attach

  // For the ESP the flash has to be read to a buffer
  EEPROM.begin(512);

  strip.Begin();
  strip.Show();

  // Setup the wifi
  EEPROM.get(SSID_ADDR, ssid);
  EEPROM.get(WIFI_PASSWORD_ADDR, wifiPassword);
  Serial.print("\r\nConnecting to WIFI '");
  Serial.print(String(ssid));
  Serial.print("'...\r\n");
  strip.SetPixelColor(0, RgbColor(0, 0, 50));
  strip.Show();
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(String(ssid), String(wifiPassword));
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Continuing...");
    strip.SetPixelColor(0, RgbColor(50, 0, 0));
    strip.Show();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(HOSTNAME);
  } else {
    Serial.println("Connected to WIFI...");
    strip.SetPixelColor(0, RgbColor(0, 50, 0));
    strip.Show();
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

  setupOTA();

  EEPROM.get(BRIGHTNESS_ADDR, brightness);

  // Setup the web server
  httpUpdater.setup(&server);
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.begin();

  clearStrip();

  // Set the start LED numbers for every ring
  int startLED = 0;
  for (int ring = 0; ring < RINGS; ring++) {
    startLEDs[ring] = startLED;
    startLED += ringSizes[ring];
  }
  totalLEDs = startLED + ringSizes[RINGS - 1];

  timeClient.begin();

  setRandomSeed();

  Serial.println();
  Serial.println("Running...");
}

void loop() {
  if (previousEffectSecond != timeClient.getSeconds()) {
    previousEffectSecond = timeClient.getSeconds();
    if (random(10) == 0) {
      int effectChoice = random(3);
      switch (effectChoice) {
        case 0:
          sparkle();
          break;
        case 1:
          pacman();
          break;
        case 2:
          scan();
          break;
      }
    }
  }

  timeClient.update();
  ArduinoOTA.handle();
  server.handleClient();

  updateClockHands();
}
