
// Radar scanner effect.
//
// Milo.

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
