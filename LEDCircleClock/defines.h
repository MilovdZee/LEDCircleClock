#define PASSWORD "KlokPassword"
#define HOSTNAME "LED-Klok"

#define SSID_ADDR 0 // String[60]
#define WIFI_PASSWORD_ADDR (SSID_ADDR + sizeof(ssid)) // String[60]
#define BRIGHTNESS_ADDR (WIFI_PASSWORD_ADDR + sizeof(wifiPassword)) // int

// Limit the power consumption. A full lit display can take up to 4 Amps.
// A value of 255 disabled the limit and allows all LEDs to be full brightness.
#define MAX_BRIGHTNESS 100

// The timezone to sync the date/time to, using NTP. For timezone to use, see TZ.h.
#define MY_TZ TZ_Europe_Amsterdam

// NTP server list to use for syncing time.
#define NTP_SERVERS "0.nl.pool.ntp.org", "1.nl.pool.ntp.org", "2.nl.pool.ntp.org"



#define CSS "<style>\
  body {\
    background: #fad7a0;\
    color: #154360;\
    padding: 20px;\
    font-size: 3em;\
    text-align: center;\
  }\
  .effects input { position: initial !important; width: 200px !important; float: left; margin: 4px; } \
  .effects { width: 900px; left: auto; }\
  div.container {\
    display: inline-block;\
    width: 90%;\
    height: 90%;\
    background: #f8c471;\
    box-shadow: 15px 20px 20px #88888888;\
    border-radius: 20px;\
    padding: 2%;\
    text-align: left;\
  }\
  h1 {\
    margin-top: 0;\
  }\
  input {\
    width: 100%;\
    border: 0;\
    border-bottom: 2px solid grey;\
    background: none;\
    color: #154360;\
    font-size: 1.2em;\
  }\
  input[type=\"range\"] {\
    width: 100%;\
  }\
  input[type=\"submit\"] {\
    background: #154360;\
    color: #fad7a0;\
    border: 0;\
    border-radius: 5px;\
    width: 40%;\
    height: 10%;\
    cursor: pointer;\
    font-size: 1em;\
    position: absolute;\
    left: 30%;\
    bottom: 20%;\
  }\
  .wifimenudiv {\
    position: absolute;\
    right: 2%;\
    bottom: 2%;\
    font-size: .6em;\
  }\
</style>"
