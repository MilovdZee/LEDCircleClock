
// Snake Effect.
//
// A bunch of food pallets are randomly dropped on the disk.
// A small snake starts going around the disk (zig-zagging), eating the food,
// growing at each bite, and dropping poops after each one.
// Note: we do not use the middle 3 rings. Didn't look right.
//
// Thijs Kaper (16/7/2021)

#define SNAKE_FOOD_COUNT 20
#define SNAKE_FOOD_DELAY 45
#define SNAKE_FOOD_COLOR RgbColor(80, 0, 0)
#define SNAKE_POOP_COLOR RgbColor(18, 0, 20)
#define SNAKE_BODY_COLOR RgbColor(0, 70, 80)
#define SNAKE_HEAD_COLOR RgbColor(0, 100, 0)
#define SNAKE_BACKGROUND RgbColor(0, 0, 0)

void snake() {
  strip.ClearTo(SNAKE_BACKGROUND);
  strip.Show();
  handlingDelay(SNAKE_FOOD_DELAY);
  
  // Skip middle 3 rings.
  int startpixel = ringSizes[0] + ringSizes[1] + ringSizes[2];

  for (int i=0;i<SNAKE_FOOD_COUNT;i++) {
    strip.SetPixelColor(random(totalLEDs-startpixel)+startpixel, SNAKE_FOOD_COLOR);
    strip.Show();
    handlingDelay(SNAKE_FOOD_DELAY);
  }

  int snakeLength = 2;

  for (int pixel = startpixel+snakeLength; pixel < PIXEL_COUNT+SNAKE_FOOD_COUNT; pixel++) {
      boolean eaten = false;
      RgbColor foodCheck = strip.GetPixelColor(zigzag(pixel));
      if(foodCheck.R > 0 || foodCheck.G > 0 || foodCheck.B > 0) {
         snakeLength++;
         eaten = true;
      }
      for (int i=0;i<=snakeLength;i++) {
        strip.SetPixelColor(zigzag(pixel-i), (i==0) ? SNAKE_HEAD_COLOR : SNAKE_BODY_COLOR);
      }
      strip.Show();
      // wipe tail pixel
      strip.SetPixelColor(zigzag(pixel-snakeLength), SNAKE_BACKGROUND);

      // Draw snake poop when something eaten (only 50% chance on pooping)
      if (eaten && random(2) == 0) {
        strip.SetPixelColor(zigzag(pixel-snakeLength), SNAKE_POOP_COLOR);
      }

      handlingDelay(45);
  }
}

// Revert the direction of 3 of the rings, to make the snake zig-zag
int zigzag(int pos) {
  pos = zigzagRing(pos, 4);
  pos = zigzagRing(pos, 6);
  pos = zigzagRing(pos, 8);
  return pos;
}

int zigzagRing(int pos, int ring) {
  if (pos >= startLEDs[ring] && pos < startLEDs[ring+1]) {
    return startLEDs[ring]-pos+startLEDs[ring+1]-1;
  }
  return pos;  
}
