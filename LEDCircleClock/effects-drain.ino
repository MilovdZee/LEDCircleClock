
// Drain (pulling the plug / vortex) effect.
// This will normally swallow the analog clock image.
//
// Thijs Kaper (14/8/2021)

void drain() {

  // wipe screen using vortex (pulling the plug / drain)
  strip.SetPixelColor(240, RgbColor(0,0,0));
  for(int i=0; i<=PIXEL_COUNT;i++) {
    strip.ShiftLeft(1);
    strip.Show();
    handlingDelay(25);
  }

}
