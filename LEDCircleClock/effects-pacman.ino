
// Pacman, eating pallets/dots/pills.
//
// Revision 1, initial version by Milo - big pacman, covering full screen.
//
// Revision 2 by Thijs Kaper (March 2021)
// - added moving pallets/dots/pills to eat
// - made pacman smaller
// - use a simpler line drawing routines to get dot eating working.
//
// Revision 3 by Thijs Kaper (December 22, 2022)
// - refactored drawing of pacman
// - open mouth less wide, and munch faster to look more like real pacman
// - added shadow food trail
// - added power-food pill, which makes pacman turn around
// - added pacman turn around

void pacman() {
  // Yellow pacman
  RgbColor color = RgbColor(brightness / 3 + 1, brightness / 3 + 1, 0);
  int bites = 26;
  int flipAtBites = bites/2;
  int biteDelay = 10;
  int stepSize = 4;
  int pacmanFoodState = 0;
  #define SPEED_DIVIDER_INIT 10
  #define POWER_FOOD_PIXEL_NR 9

  int speedDivider = SPEED_DIVIDER_INIT;
  while (bites-- > 0) {
    boolean flip = bites < flipAtBites;
    
    // Close mouth
    for (int angle = 54; angle >= stepSize; angle -= stepSize) {
      strip.ClearTo(RgbColor(0, 0, 0));
      drawPacmanFood(pacmanFoodState, speedDivider, flip);
      drawPacman(angle, stepSize, color, flip);
      // turn center pixel on
      setPixel(0, color);
      strip.Show();
      handlingDelay(biteDelay);
    }
    
    handlingDelay(20);

    // Open mouth
    for (int angle = 0; angle <= 54; angle += stepSize) {
      strip.ClearTo(RgbColor(0, 0, 0));
      drawPacmanFood(pacmanFoodState, speedDivider, flip);
      drawPacman(angle, stepSize, color, flip);
      // turn center pixel on
      setPixel(0, color);
      strip.Show();
      handlingDelay(biteDelay);
    }

    handlingDelay(20);
  }
}

void drawPacman(int angle, int stepSize, RgbColor color, boolean flip) {
  // degrees of horizontal line, the +10 is to correct for some rounding errors to get mouth more in center
  int baseDegrees = (flip ? 90 : 270) + 10;
  int degrees = 360-(2*angle);
  for (int i=0; i<=degrees; i+=stepSize) {
    drawSimpleAngle((baseDegrees + angle + i)%360, (RINGS * 2 / 3), color);
  }
}

void drawPacmanFood(int &pacmanFoodState, int &speedDivider, boolean flip) {
  RgbColor foodShadowColor = RgbColor(brightness / 5 + 1, brightness / 5 + 1, brightness / 5 + 1);
  RgbColor foodColor = RgbColor(brightness / 3 + 1, 0, 0);
  RgbColor powerColor = RgbColor(0, 0, brightness / 1.5 + 1);

  int baseQarter = flip ? 1 : 3;
  int shadowQarter = flip ? 3 : 1;
  
  // draw dots as pacman food, they scroll to the right for each render to simulate pacman movement.
  for (int i=RINGS; i>3; i--) {
    int size = ringSizes[i-1];
    int start = startLEDs[i-1];
    // Get 270 (or 90) degree led nr for ring "i".
    int led = start + (size * baseQarter / 4);
    int shadowLed = start + (size * shadowQarter / 4);
    RgbColor originalColor = strip.GetPixelColor(led);
      if ((pacmanFoodState + i) % 4 == 0) {
        int foodNr = int((pacmanFoodState+i)/4);
        setPixel(led, flip ? foodShadowColor : (foodNr == POWER_FOOD_PIXEL_NR ? powerColor : (foodNr > POWER_FOOD_PIXEL_NR ? RgbColor(0,0,0) : foodColor)));
      }
      if ((pacmanFoodState - i) % 4 == 0) {
        setPixel(shadowLed, foodShadowColor);
      }
  }
  // Toggle state to move pixels
  if (speedDivider == 0) {
    ++pacmanFoodState;
  }
  speedDivider--;
  if (speedDivider<0) {
    speedDivider = SPEED_DIVIDER_INIT;
  }
}
