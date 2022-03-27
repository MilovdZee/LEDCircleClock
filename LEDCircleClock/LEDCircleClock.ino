#include <NeoPixelBus.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <TZ.h>
#include <coredecls.h> // required for settimeofday_cb() (NTP sync callback)

#include "defines.h"

// To start running/showing ALL effects in a row, set demoMode to 0.
int demoMode = -1;

boolean timeIsSet = false;
time_t lastNtpSet = 0;
time_t currentTime = time(nullptr); // time_t = seconds since epoch
struct tm * timeinfo;

boolean otaActive = false;

// The ota update and wifi settings pages are protected using the ota password (wifi page uses user "admin").
// If someone keeps trying with the wrong password, we will lock up updates until device restart.
#define MAX_CREDENTIAL_FAILS 6
int adminOtaCredentialsFailCount = 0;

// Used for web-page to set which effect will play next in the main loop (to manually trigger an effect)
int triggerEffect = -1;

time_t previousEffectTime = time(nullptr);

int previousClockSecond = -1;
int millisOffset = 0; // Offset compared to millis() to get partial seconds in sync with the NTP seconds

char ssid[60];
char wifiPassword[60];
char otaPassword[12];

// Number of rings
#define RINGS 9

// Number of LEDs per ring
int ringSizes[] = {1, 8, 12, 16, 24, 32, 40, 48, 60};

// The higher the power the narrower the region that lights up
int ringPowers[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

// First LED number of a ring (one ring more than exists, to be able to check last led nr)
int startLEDs[RINGS+1];
int totalLEDs;

const int PIXEL_COUNT = 241; // make sure to set this to the number of pixels in your strip
int brightness; // Brightness between 0 and 255

// For Esp8266, the Pin is omitted and it uses GPIO3(Rx) due to DMA hardware use.
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXEL_COUNT);

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

// Record the NPT set time
void timeUpdated() {
  timeIsSet = true;
  lastNtpSet = time(nullptr);
  Serial.print("NTP Updated: "); Serial.println(ctime(&lastNtpSet));
}

// Check if NTPD sync was more than ONE hour and 20 SECONDS ago. If so, we show it on screen to indicate we are not sure about the time yet or anymore.
boolean isNtpOlderThanOneHour() {
  // time(nullptr) = time in seconds
  return (!timeIsSet) || (time(nullptr) - lastNtpSet) > 3620;
}

void clearStrip() {
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
}

void handlingDelay(int delayMillis) {
  handleOta();
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
  if (String(wifiPassword).length() > 0) {
    WiFi.begin(String(ssid), String(wifiPassword));
  } else {
    WiFi.begin(String(ssid));
  }
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


  // implement NTP update of timekeeping (with automatic hourly updates)
  configTime(MY_TZ, NTP_SERVERS);

  // callback, when NTP changes the time
  settimeofday_cb(timeUpdated);


  setupOTA();

  EEPROM.get(BRIGHTNESS_ADDR, brightness);

  // Setup the web server
  httpUpdater.setup(&server);
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/effect", handleEffect);
  server.on("/color", handleColorPicker);
  server.on("/demo", handleDemoMode);
  server.on("/toggle-pause", handleTogglePause);
  server.begin();

  clearStrip();

  // Set the start LED numbers for every ring, add one extra for startLed of non-existent next ring.
  int startLED = 0;
  for (int ring = 0; ring <= RINGS; ring++) {
    startLEDs[ring] = startLED;
    startLED += ringSizes[ring];
  }
  totalLEDs = startLED + ringSizes[RINGS - 1];

  setRandomSeed();

  Serial.println();
  Serial.println("Running...");
}

int count = 0;
void loop() {
  if (otaActive) {
    server.handleClient();
    handleOta();
    // Skip normal drawing routines, to keep ota update more stable
    return;    
  }
  
  currentTime = time(nullptr); // time_t = seconds since epoch
  timeinfo = localtime (&currentTime); // setup timeinfo -> tm_hour, timeinfo -> tm_min, timeinfo -> tm_sec

  // Check if web interface scheduled an effect to be played.
  if (triggerEffect != -1) {
    executeEffect(triggerEffect);
    triggerEffect = -1;
  }
  
  // suppress effects in the night between 22.00 and 8:00
  if (demoMode == -1 && (timeinfo -> tm_hour) >= 8 && (timeinfo -> tm_hour) <= 21) {
    if (previousEffectTime != currentTime) {
      previousEffectTime = currentTime;
      if (random(30) == 0) {
        executeRandomEffect();
      }
    }
  }

  if (demoMode >= 0 && currentTime > previousEffectTime) {
    executeEffect(demoMode);
    previousEffectTime = time(nullptr) + 2;
    if (demoMode >= 0) {
      // This "if" check >=0 is there, as another (web) thread can set the value back to a negative value.
      demoMode++;
    }
    if (demoMode >= getNrOfEffects()) {
      demoMode = -1;
    }
  }

  server.handleClient();
  handleOta();
  updateClockHands();
}
