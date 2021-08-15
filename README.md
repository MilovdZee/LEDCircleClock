# LEDCircleClock

A clock with 241 RGB LEDs.


This https://github.com/atkaper/LEDCircleClock is a fork of the awesome project from Milo: https://github.com/MilovdZee/LEDCircleClock

The original project had 4 effects, next to the analog clock. I (Thijs) have added some more. Here's the total effect/animation list:

- Base animation: Analog clock (by Milo).
- Sparkle (by Milo).
- PacMan (original by Milo, updated by Thijs [March 2021?] to be smaller, and eat some moving food).
- Scan (by Milo).
- Fire (by Milo, or he found it somewhere on the internet I think).
- Snake (by Thijs, July 2021).
- Maze (by Thijs, July 2021).
- Digital Clock (by Thijs, July 2021).
- Cartoon Bomb Explosion (by Thijs, July 2021).
- Tic Tac Toe (by Thijs, August 2021).
- Down the Drain (by Thijs, August 2021).
- Hidden feature: color picker (by Thijs, July 2021).

And I also did add automatic following of summer/winter time (in March 2021). Possibly some or all of this will be merged into the original github project.
Oh, and I did some refactoring to put the effects each in a separate source file. And just now added a demo mode to play all effects, and a setting for ota password.

Back to Milo's descriptions:


## Description

The clock shows the hands in diferent colors on the grid of LEDs. To make it more beautiful hands are antialiased. The brightness of the LED is calculated from the distance to the virtual pixel. I also use the milliseconds clock of the Arduino combined with the NTP clock to have a seconds hand that rotates fluently instead of ticking the seconds.

<img src="images/clock.jpg?raw=true" height=300/> <img src="images/LayoutVideo.gif?raw=true" height=300/>

Filming this does not show the colors I see. It looks a lot warmer and more saturated in real life.

## Introduction

As a sucker for clocks I'm always looking for cool projects surrounding clocks. At our favorite Chinese shop I found a 241 LED ring that demanded to become a clock. So I ordered one and waited the standard couple of weeks for it to arrive. Nice, It looked perfect and very easy to use. Data-in and Data-out were next to each other and could simply be daisy-chained with a short piece of wire. Effectively the ring became a strand of 241 LED's all addressable from a serial bus.

Now I needed to design a enclosure. Each LED needs to be separate from the neighboring LEDs. Some calculations were needed but after a couple of hours I had my 3D design in Blender. 26 hours of printing resulted in an enclosure that perfectly fits the LED ring.

I started by testing with an example program to see if all functions as expected and it did. The current it takes at full brightness is a bit high but that is no problem while the clock in normal operation is not at full brightness and not all LEDs are lit.

After the testing I wrote an Arduino program to show time based on NTP and once in a while shows an effect.

Some trickery was needed. I wanted the seconds hand to move smoothly and so I used the milliseconds as an offset to the seconds. 

Calculating the hands uses the 'pow()' function and that is a very CPU intensive operation. That is why I created a function that caches the previous values of the 'pow()' function to make it a lot faster.

After all I'm quite happy with the result :)

## Effects

At random moments the clock is replaced by an effect. Just for a couple of seconds.

<img src="images/sparkle.gif?raw=true" height=300/> <img src="images/scan.gif?raw=true" height=300/> 

<img src="images/pacman.gif?raw=true" height=300/> <img src="images/fire.gif?raw=true" height=300/>

# Hardware

- esp8266 / NodeMCU
- DC-DC converter if you like to accept multiple input voltages. The LED ring contains 241 LEDs and can consume up to 3.5 Amps. An XL4015 version sound like capable of handling this.
- LED ring. Search aliexpress for "241 led ring". The diameter is 170mm.

## Schematics

Not really schematics but more like a wiring diagram. The schematics are not even worth drawing out :)

<img src="images/schematics.png?raw=true" width=300/>

## 3D printed Case

I designed the case in Blender and the files can be found in this project. Together with the STL files.
The STL for the front with dividers is in one STL file. Make sure to split it in two materials. One semi transparent first 2mm and the rest an opaque color. Also make sure to print the front first layer in a spiral. This removed the step of the print head to the next ring.

The cable hole was a bit of an afterthought. Originally it was intended to have an USB-C power supply but that ended up being more complicated than benneficial. Still, if you like to use USB-C then you could use the design and expose the USB-C connector.

I printed the case in PETG because this seems to be the only type that has transaparent filament. It took about 26 hours for the grid and front panel and another 5 hours for the back. Electronics holder and cable hole were much quicker.
I like to print in .15 mm resolution so this is also a reason why it took that long.

<img src="images/LEDRingGrid.png?raw=true" height=300/> <img src="images/LEDRingBack.png?raw=true" height=300/>
<img src="images/SlicedGrid.png?raw=true" height=300/> <img src="images/printing.jpg?raw=true" height=300/>
<img src="images/ledring_fits.jpg?raw=true" height=300/>

## LED ring

As mentioned, just search for "241 LED ring". The prices varies a lot so make sure to compare them. Mine was about €17,-.

<img src="images/241LEDring.jpg?raw=true" height=300/>

# Software

The software is written for the Arduino IDE. 

- NTP to synchronize the time
- OTA for wireless code updates. The OTA password as set in this repo is 'KlokPassword'.
- WEB interface to configure brightness and WIFI settings. The ip address is received through DHCP and so should be looked up in the router. Or use the Arduino IDE to find it. It will show up as a port after restarting the IDE.
- If WIFI is not found the esp8266 becomes an open access point. Pointing a browser to http://192.168.4.1/wifi opens the WIFI configuration page.
- At random moments an effect is shown (sparkle, pacman, scan, fire)

## Libraries

- NeoPixelBus by Makuna
- NTPClient

The libraries can be installed under "Sketch -> Include Library -> Manage Libraries..."

And additional board manager for the ESP8266: http://arduino.esp8266.com/stable/package_esp8266com_index.json. This must be set under "File->Preferences->Additional Boards Manager URLs".


## Change Log

- March 21, 2021: Use native ESP NTP timekeeping, instead of external NTP library, and make it use automatic summer/winter time switch (Thijs Kaper).
- July/August 2021: Add some more effects, demo mode, and ota-password config (see top of readme) (Thijs Kaper).


