# LEDCircleClock
A clock with 241 RGB LEDs.

The clock shows the hands in diferent colors on the grid of LEDs. To make it more beautiful hands are dealiased. The brightness of the LED is calculated from the distance to the virtual pixel.

<img src="https://raw.githubusercontent.com/MilovdZee/LEDCircleClock/main/images/clock.jpg" height=300/> <img src="https://raw.githubusercontent.com/MilovdZee/LEDCircleClock/main/images/LayoutVideo.gif" height=300/>

Filming this does not show the colors I see. It looks a lot warmer and more saturated in real life.

# Hardware
- esp8266 / NodeMCU
- DC-DC converter if you like to accept multiple input voltages. The LED ring contains 241 LEDs and can consume up to 3.5 Amps
- LED ring. Search aliexpress for "241 led ring". The diameter is 170mm.

## Schematics
<img src="https://raw.githubusercontent.com/MilovdZee/LEDCircleClock/main/images/schematics.png" width=300/>

## Case
I designed the case in Blender and the files can be found in this project. Together with the STL files.
The STL for the front with dividers is in one STL file. Make sure to split it in two materials. One semi transparent first 2mm and the rest an opaque color.

The cable hole was a bit of an afterthought. Originally it was intended to have an USB-C power supply but that ended up being more complicated than benneficial. Still, if you like to use USB-C then you could use the design and expose the USB-C connector.

## 3D printed case
<img src="https://raw.githubusercontent.com/MilovdZee/LEDCircleClock/main/images/LEDRingGrid.png" width=300/>

## LED ring
<img src="https://raw.githubusercontent.com/MilovdZee/LEDCircleClock/main/images/241LEDring.jpg" width=300/>

# Software
The software is written for the Arduino IDE. 
- NTP to synchronize the time
- OTA for wireless code updates 
- WEB interface to configure brightness and WIFI settings
- If WIFI is not found the esp8266 becomes an open access point. Pointing a browser to http://192.168.4.1/wifi opens the WIFI configuration page.
