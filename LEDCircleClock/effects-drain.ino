
// Drain (pulling the plug / vortex) effect.
// This will normally swallow the analog clock image.
//
// Thijs Kaper (14/8/2021)
//
// Thijs, 1/12/2022 - added ShiftRight option...

void drain() {
  boolean drainRight = (random(3) == 0);
  // wipe screen using vortex (pulling the plug / drain)
  strip.SetPixelColor(240, RgbColor(0,0,0));
  strip.SetPixelColor(0, RgbColor(0,0,0));
  for(int i=0; i<=PIXEL_COUNT;i++) {
    if (drainRight) {
      strip.ShiftRight(1);
    } else {
      strip.ShiftLeft(1);
    }
    strip.Show();
    handlingDelay(25);
  }
}
