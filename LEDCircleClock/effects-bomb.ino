
// Exploding (Cartoon) Bomb effect.
//
// Thijs Kaper (23/7/2021)

#define BOMB_BACKGROUND RgbColor(0, 0, 0)
#define BOMB_COLOR RgbColor(0, 0, 25)
#define BOMB_SHINE RgbColor(0, 0, 60)
#define FUSE_COLOR RgbColor(20, 20, 20)
#define FIRE_COLOR_1 RgbColor(30, 0, 0)
#define FIRE_COLOR_2 RgbColor(25, 13, 0)
#define EXPLOSION_DOT RgbColor(27, 6, 0)

// fuse pixels, from outside to bomb, both used to draw fuse, and let fire follow it
const uint8_t fuse[] = { 197, 145, 102, 101, 142, 141, 99, 65 };

void bomb() {
  strip.ClearTo(BOMB_BACKGROUND);

  // Bomb (ball)
  for (int i=0;i<=36;i++) {
    strip.SetPixelColor(i, BOMB_COLOR);
  }
  
  // Bomb (flat top)
  strip.SetPixelColor(39, BOMB_COLOR);
  strip.SetPixelColor(40, BOMB_COLOR);
  strip.SetPixelColor(41, BOMB_COLOR);
  
  // Bomb (shine spot /  light reflection)
  strip.SetPixelColor(18, BOMB_SHINE);
  strip.SetPixelColor(19, BOMB_SHINE);
  for(int i=32;i<=35;i++) {
    strip.SetPixelColor(i, BOMB_SHINE);
  }
  
  // Bomb (fuse)
  for (int i=0; i<sizeof(fuse);i++) {
    strip.SetPixelColor(fuse[i], FUSE_COLOR);
  }

  strip.Show();
  handlingDelay(500);

  // Burn the fuse
  int fader=100;
  int fadeDir=20;
  for (int i=0; i<(sizeof(fuse)+2);i++) {
    for (int j=0;j<20;j++) {
      if (i<sizeof(fuse)) {
        strip.SetPixelColor(fuse[i], getBrightnessPercentage(FIRE_COLOR_1, fader));
      }
      if (i > 0 && i < sizeof(fuse)+1) {
        strip.SetPixelColor(fuse[i-1], getBrightnessPercentage(FIRE_COLOR_2, (130 - fader + 20)));
      }
      if (i > 1) {
        strip.SetPixelColor(fuse[i-2], BOMB_BACKGROUND);
      }
      strip.Show();
      handlingDelay(20);

      fader+=fadeDir;
      if (fader>120 || fader <30)  {
        fadeDir=-fadeDir;
      }
    }
  }
  strip.Show();

  // Drop some fire sparks (only in the inner 4 rings, as we want to see movement from inside to outside)
  for (int i=0;i<30;i++) {
    strip.SetPixelColor(random(37), getBrightnessPercentage(FIRE_COLOR_1, random(90) + 20));
    strip.SetPixelColor(random(37), getBrightnessPercentage(FIRE_COLOR_2, random(90) + 20));
    strip.SetPixelColor(random(37), getBrightnessPercentage(EXPLOSION_DOT, random(90) + 20));
  }

  // Blow everything up
  for(int wipeSteps=0;wipeSteps<9;wipeSteps++) {
    // Go trough all rings, from outside to inside
    for(int r=8;r>0;r--) {
      // Shuffle the position in the ring, to get better movement distribution
      int shuffled[ringSizes[r]];
      getRandomIndexOrder(shuffled, ringSizes[r]);
      // Go through all pixels of outer ring
      for(int i=0;i<ringSizes[r];i++) {
        int outerPos = shuffled[i];
        int outerLed = startLEDs[r] + outerPos;
        // Convert position to degrees
        int outerDegrees = outerPos * 360 / ringSizes[r];
        // Convert degrees to inner ring led position
        int innerLed = startLEDs[r-1] + (ringSizes[r-1] * (outerDegrees + 5) / 360);
        // Move pixel from inner to outer ring
        strip.SetPixelColor(outerLed, strip.GetPixelColor(innerLed));
        strip.SetPixelColor(innerLed, BOMB_BACKGROUND);
      }
    }
    // After all rings handled once, show result
    strip.Show();
    handlingDelay(100);
  }

  // Due to a rounding error, sometimes one pixel remains on the edge, just clear all
  strip.ClearTo(BOMB_BACKGROUND);
  strip.Show();

  handlingDelay(1000);
}

// Return input color, changed for brightness by given percentage
RgbColor getBrightnessPercentage(RgbColor color, int percentage) {
  return RgbColor(color.R*percentage/100, color.G*percentage/100, color.B*percentage/100);
}

// Fill an int array of given length, where elements are indexes 0..(size-1).
// And these are randomly shuffeled a bit. This is to make the explosion not always
// go clockwise, as that results in just some separate big blocks moving, not a nice distribution.
void getRandomIndexOrder(int result[], int len) {
  for (int i=0;i<len;i++) {
    result[i]=i;
  }
  // do a randmom set of position swaps
  for (int i=0; i<len*4;i++) {
    int a = random(len);
    int b = random(len);
    if (a!=b) {
      int h = result[a];
      result[a]=result[b];
      result[b]=h;
    }
  }
}
