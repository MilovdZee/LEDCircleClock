void sparkle() {
  int sparkleRounds = 50;
  int sparklesPerRound = 5;
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
  RgbColor color = RgbColor(brightness, brightness, 0);
  int bites = 2;
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
    for (int angle = 60; angle >= 2 * stepSize; angle -= stepSize) {
      drawAngle(270 + angle, RINGS, color);
      drawAngle(270 - angle, RINGS, color);
      strip.Show();
      handlingDelay(biteDelay);
    }

    handlingDelay(200);
    
    // Open beak
    for (int angle = 2 * stepSize; angle <= 60; angle += stepSize) {
      drawAngle(270 + angle, RINGS, RgbColor(0, 0, 0), true);
      drawAngle(270 - angle, RINGS, RgbColor(0, 0, 0), true);
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
  int maxAngle = 4 * 360;

  int red = random(brightness);
  int green = random(brightness);
  int blue = random(brightness);
  int offsetAngle = random(360);
  
  strip.ClearTo(RgbColor(0, 0, 0));
  for(int angle = 0; angle < maxAngle; angle += stepSize) {
    // Set the intensity (fade in and fade out)
    if(intensityStep > 0.0 && intensityStep * fadeOutSpeedup * (maxAngle - angle - 45) <= 1.0) {
      intensityStep = -intensityStep * fadeOutSpeedup;
    }
    intensity = intensity + intensityStep * stepSize;
    if(intensity > 1.0) intensity = 1.0;
    if(intensity < 0.0) intensity = 0.0;
    
    RgbColor color = RgbColor(red * intensity, green * intensity, blue * intensity);

    drawAngle((angle + offsetAngle) % 360, RINGS, color, true);
    drawAngle((angle - 90 + offsetAngle) % 360, RINGS, RgbColor(0,0,0), true);
    strip.Show();
    
    handlingDelay(0);
  }
  
  handlingDelay(100);
}
