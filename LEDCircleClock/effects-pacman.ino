
// Pacman, eating pallets/dots.
//
// Initial version by Milo - big pacman, covering full screen.
//
// Revision by Thijs Kaper (March 2021?) - added moving pallets/dots to eat, and made pacman
// smaller, and using simpler line drawing routines to get dot eating working.

void pacman() {
  // Yellow beak
  RgbColor color = RgbColor(brightness / 2 + 1, brightness / 2 + 1, 0);
  int bites = 6;
  int biteDelay = 16;
  int stepSize = 3;
  int pacmanFoodState = 0;
  int speedDivider = 6;

  strip.ClearTo(RgbColor(0, 0, 0));

  // 45 degrees is always shown so just fill that always
  // Use 2/3 of the rings to make a smaller packman, to give space to eat floating pills
  for (int ring = 1; ring < (RINGS * 2 / 3); ring++) {
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
    for (int angle = 78; angle >= stepSize; angle -= stepSize) {
      drawPacmanFood(pacmanFoodState, speedDivider);
      drawSimpleAngle(270 + angle, (RINGS * 2 / 3), color);
      drawSimpleAngle(285 - angle, (RINGS * 2 / 3), color);
      // turn center pixel on
      setPixel(0, color);
      strip.Show();
      handlingDelay(biteDelay);
    }
    
    handlingDelay(20);

    // Open beak
    for (int angle = 0; angle <= 60; angle += stepSize) {
      drawSimpleAngle(270 + angle, (RINGS * 2 / 3), RgbColor(0, 0, 0));
      drawSimpleAngle(285 - angle, (RINGS * 2 / 3), RgbColor(0, 0, 0));
      drawPacmanFood(pacmanFoodState, speedDivider);
      // turn center pixel on
      setPixel(0, color);
      strip.Show();
      handlingDelay(biteDelay);
    }

    handlingDelay(20);
  }
}

void drawPacmanFood(int &pacmanFoodState, int &speedDivider) {
  RgbColor foodColor = RgbColor(brightness / 2 + 1, 0, 0);
  
  // draw dots as pacman food, they scroll to the right for each render to simulate pacman movement.
  for (int i=RINGS; i>0; i--) {
    int size = ringSizes[i-1];
    int start = startLEDs[i-1];
    // Get 270 degree led nr for ring "i".
    int led = start + (size * 3 / 4);
    RgbColor originalColor = strip.GetPixelColor(led);
      if ((pacmanFoodState + i) % 4 == 0) {
        setPixel(led, foodColor);
      } else {
        setPixel(led, RgbColor(0, 0, 0));
      }
  }
  // Toggle state to move pixels
  if (speedDivider == 0) {
    ++pacmanFoodState;
  }
  speedDivider--;
  if (speedDivider<0) {
    speedDivider=6;
  }
}
