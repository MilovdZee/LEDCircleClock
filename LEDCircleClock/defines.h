#define PASSWORD "KlokPassword"
#define HOSTNAME "LED-Klok"

#define SSID_ADDR 0 // String[60]
#define WIFI_PASSWORD_ADDR (SSID_ADDR + sizeof(ssid)) // String[60]
#define BRIGHTNESS_ADDR (WIFI_PASSWORD_ADDR + sizeof(wifiPassword)) // int
#define OTA_PASSWORD_ADDR (BRIGHTNESS_ADDR + sizeof(int)) // String[12]

// Limit the power consumption. A full lit display can take up to 4 Amps.
// A value of 255 disabled the limit and allows all LEDs to be full brightness.
#define MAX_BRIGHTNESS 100

// The timezone to sync the date/time to, using NTP. For timezone to use, see TZ.h.
#define MY_TZ TZ_Europe_Amsterdam

// NTP server list to use for syncing time.
#define NTP_SERVERS "0.nl.pool.ntp.org", "1.nl.pool.ntp.org", "2.nl.pool.ntp.org"
