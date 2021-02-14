void sparkle() {
  int sparkleRounds = 75;
  int sparklesPerRound = 4;
  int sparkleDelay = 50;

  for (int sparkleRound = 0; sparkleRound < sparkleRounds; sparkleRound++) {
    strip.ClearTo(RgbColor(0, 0, 0));
    for (int sparkle = 0; sparkle < sparklesPerRound; sparkle++) {
      int ledNumber = random(totalLEDs);
      RgbColor color = RgbColor(random(brightness), random(brightness), random(brightness));
      setPixel(ledNumber, color);
    }
    strip.Show();
    handlingDelay(sparkleDelay);
  }
}

void pacman() {
  // Yellow beak
  RgbColor color = RgbColor(brightness / 2 + 1, brightness / 2 + 1, 0);
  int bites = 4;
  int biteDelay = 1;
  int stepSize = 3;

  strip.ClearTo(RgbColor(0, 0, 0));

  // 45 degrees is always shown so just fill that always
  for (int ring = 1; ring < RINGS; ring++) {
    // Upper part of beak
    int startLED = startLEDs[ring] + 315 * ringSizes[ring] / 360;
    int endLED = startLEDs[ring] + ringSizes[ring] - 1;
    for (int led = startLED; led <= endLED; led++) {
      setPixel(led, color);
    }

    // Lower part of beak
    startLED = startLEDs[ring];
    endLED = startLEDs[ring] + 225 * ringSizes[ring] / 360;
    for (int led = startLED; led <= endLED; led++) {
      setPixel(led, color);
    }
  }

  while (bites-- > 0) {
    // Close beak
    for (int angle = 80; angle >= stepSize; angle -= stepSize) {
      drawAngle(270 + angle, RINGS, color);
      drawAngle(270 - angle, RINGS, color);
      strip.Show();
      handlingDelay(biteDelay);
    }

    handlingDelay(200);

    // Open beak
    for (int angle = 0; angle <= 60; angle += stepSize) {
      drawAngle(270 + angle, RINGS, RgbColor(0, 0, 0), true, 0.2);
      drawAngle(270 - angle, RINGS, RgbColor(0, 0, 0), true, 0.2);
      strip.Show();
      handlingDelay(biteDelay);
    }

    handlingDelay(200);
  }
}

void scan() {
  int stepSize = 5;
  double intensity = 0.0;
  double intensityStep = 0.001;
  double fadeOutSpeedup = 5.0;
  int maxAngle = 5 * 360;

  int red = random(brightness);
  int green = random(brightness);
  int blue = random(brightness);
  int offsetAngle = random(360);

  strip.ClearTo(RgbColor(0, 0, 0));
  for (int angle = 0; angle < maxAngle; angle += stepSize) {
    // Set the intensity (fade in and fade out)
    if (intensityStep > 0.0 && intensityStep * fadeOutSpeedup * (maxAngle - angle - 45) <= 1.0) {
      intensityStep = -intensityStep * fadeOutSpeedup;
    }
    intensity = intensity + intensityStep * stepSize;
    if (intensity > 1.0) intensity = 1.0;
    if (intensity < 0.0) intensity = 0.0;

    RgbColor color = RgbColor(red * intensity, green * intensity, blue * intensity);

    drawAngle((angle + offsetAngle) % 360, RINGS, color, true);
    drawAngle((angle - 90 + offsetAngle) % 360, RINGS, RgbColor(0, 0, 0), true);
    strip.Show();

    handlingDelay(0);
  }

  handlingDelay(100);
}

void fire() {
  // Translated version of a FastLed example
  int rounds = 150;

  int fireWidth = ringSizes[RINGS - 1];
  int fireHeight = RINGS;
  uint8_t heat[fireWidth][fireHeight];

  for(int i = 0; i < fireWidth; i++) {
    for(int j = 0; j < fireHeight; j++) {
      heat[i][j] = 0;
    }
  }

  for (int round = 0; round < rounds; round++) {
    strip.ClearTo(RgbColor(0, 0, 0));
    int activity = random(100) + 156;
    for (uint8_t h = 0; h < fireWidth; h++) {
      // Step 1.  Cool down every cell a little
      for ( uint8_t i = 0; i < fireHeight; i++) {
        int newHeat = heat[h][i] - random(80);
        heat[h][i] = newHeat > 0 ? newHeat : 0;
      }

      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for ( uint8_t k = fireHeight - 1; k >= 1; k--) {
        uint8_t hleft = (h + fireWidth - 1) % fireWidth;
        uint8_t hright = (h + 1) % fireWidth;
        heat[h][k] = (heat[h][k]
                      + heat[hleft][k - 1]
                      + heat[h][k - 1]
                      + heat[hright][k - 1] ) / 4;
      }

      if ( random(256) < activity ) {
        int newHeat = heat[h][0]  + random(100);
        heat[h][0] = newHeat < 255 ? newHeat : 255;
      }
    }

    // map the modified fire onto the rings. Rings 0 is the outer ring and ring 8 is the inner ring.
    uint8_t ring = 0;
    do {
      int led = startLEDs[RINGS - ring - 1];
      uint8_t count = ringSizes[RINGS - ring - 1];
      uint16_t td = fireWidth * 255 / count;
      uint16_t t = 0;
      for (uint8_t i = 0; i < count ; i++) {
        uint8_t h = heat[t >> 8][fireHeight - 1 - ring];
        if (ring >= RINGS - 2) h = h | 128;

        // Convert heat to color
        RgbColor color;
        int t192 = h * 191 / 255;
        int heatramp = t192 & 0x3F; // 0..63
        heatramp <<= 2; // scale up to 0..252
        if(t192 & 0x80) {
          // very hot
          color = RgbColor(brightness, brightness, heatramp * brightness / 255);
        } else if(t192 & 0x40) {
          // warm
          color = RgbColor(brightness, heatramp * brightness / 255, 0);
        } else {
          // cool
          color = RgbColor(heatramp * brightness / 255, 0, 0);
        }

        // Set the pixel
        setPixel(led++, color);
        t += td;
      }
    } while (++ring < RINGS);
    strip.Show();

    handlingDelay(100);
  }
}
