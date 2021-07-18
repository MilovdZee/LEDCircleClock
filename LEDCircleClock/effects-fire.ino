
// Fire Effect.
//
// Glowing red/yellow fire lights.
//
// Milo.

void fire() {
  // Translated version of a FastLed example
  int rounds = 75;

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
