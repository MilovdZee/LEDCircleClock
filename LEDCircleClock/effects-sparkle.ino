
// Sparkle/glitter effect.
//
// Milo.

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
