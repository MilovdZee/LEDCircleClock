
// Maze Runner.
//
// A maze is drawn, and a snake will find it's way out.
// To get some variation in display, the maze can be randomly rotated in 4 steps
// of 90 degress, and randomly mirrored along the vertical axis.
// And there are two (pre determined) mazes which we choose from each time.
//
// Thijs Kaper (16/7/2021)

// How big is this snake?
#define MAZE_SNAKE_LEN 5

// Some colors
#define MAZE_SNAKE_HEAD_COLOR RgbColor(10, 2, 0)
#define MAZE_SNAKE_BODY_COLOR RgbColor(10, 10, 0)
#define MAZE_WALL_COLOR RgbColor(2, 2, 10)
#define MAZE_BACKGROUND_COLOR RgbColor(0, 0, 0)

// A bit of timing
#define MAZE_WALL_BUILD_DELAY 500
#define MAZE_SNAKE_WALK_DELAY 120

// When stepping a ring in or out, due to rounding down you might end up in the wrong spot.
// With this define, we add a bit of extra rouding up. Only need to change it if your snake walks into a wall.
#define MAZE_RING_HOP_DEGREES_ROUND_UP 5

// Maze and walking route definition (mazeData char array), each ring, clockwise, start at top middle.
//
// Note: IF you want to design your own maze, make sure the way out starts from center with a step straight up.
// This is because the snake starts in the center, and does not know what it's direction is other than poiting up.
// Also make sure to cross between walls at points in the led matrix where leds are not too much shifted sideways from each other.
// Two reasons for that: It looks weird, and we re-calculate position by going to degrees and back, and if the cells do not
// line up nicely, you might end up in rounding errors.
// And when making your own design, be carefull to lay the breadcrumb path properly to lead the snake to the exit.
//
// Character meaning in the maze data:
//
// '#' = wall
// ' ' = path, not used by snake (not the way to the exit)
// '+' = path, breadcrumb exit route to next +1 pos in the ring
// '-' = path, breadcrumb exit route to prev -1 pos in the ring
// 'o' = path, breadcrumb exit route to one ring to the outside (ring+1)
// 'i' = path, breadcrumb exit route to one ring to the inside (ring-1)

const PROGMEM char mazeDataOne[] = {
  /* ring 0 =  1 */
  'o',
  /* ring 1 =  8 */
  'o',' ',' ',' ',' ',' ',' ',' ',
  /* ring 2 = 12 */
  'o','#','#',' ','#','#','#','#','#','#','#','#',
  /* ring 3 = 16 */
  '-','#',' ',' ',' ','#','o','-','-','#','o','-','-','#','o','-',
  /* ring 4 = 24 */
  '#','#','#','#','#','#','#','#','#','o','#','#','i','#','#','o','#','#','i','#','#','o','#','#',
  /* ring 5 = 32 */
  '+','+','+','+','o',' ',' ','#','o','-','-','-','-','#',' ','#','i','-','-','-','-',' ',' ','#','i',' ',' ','#','+','+','+','+',
  /* ring 6 = 40 */
  '#','#','#','#','#','o','#','#','#','#','o','#','#','#','#','#','#','#',' ','#',
  '#','#',' ','#','#','#',' ','#','#','#','i','#','#','#','#','#','#','#','#','#',
  /* ring 7 = 48 */
  '-','-','-','-','-','-','-',' ',' ',' ',' ','#','+','+','+','+','+','+','+','+','+','+','+','+',
  'o','#',' ',' ',' ','#',' ',' ',' ',' ','#',' ','i','-','-','-','-','-','-','-','-','-','-','-',
  /* ring 8 = 60 */
  '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
  'o','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
};

const PROGMEM char mazeDataTwo[] = {
  /* ring 0 =  1 */
  'o',
  /* ring 1 =  8 */
  'o',' ',' ',' ',' ',' ',' ',' ',
  /* ring 2 = 12 */
  'o','#','#','#','#','#','#','#','#','#','#','#',
  /* ring 3 = 16 */
  '+','+','+','+','o',' ',' ','#',' ',' ','+','+','+','+','o','#',
  /* ring 4 = 24 */
  '#','#','#','#','#','#','o','#','#','#','#','#','#','#','#','i','#','#','#','#','#','o','#','#',
  /* ring 5 = 32 */
  '+','+','+','+','o',' ',' ','#','o','#',' ','+','+','+','+','o','#',' ',' ',' ','i','-','-','-','-',' ',' ','#','o',' ',' ','#',
  /* ring 6 = 40 */
  'i','#','#','#','#','o','#','#','#','#','o','#','#','#','i','#','#','#','#','o',
  '#','#','#','#','#','#','#','#','#','#','i','#','#','#','#','o','#','#','#','#',
  /* ring 7 = 48 */
  'i',' ',' ','#','o','-','-',' ',' ','#',' ',' ','+','+','+','+','+','i',' ',' ',' ',' ','#','+',
  '+','+','+','+','+','+','+','+','+','+','+','+','i','#',' ',' ',' ',' ','+','+','+','+','+','+',
  /* ring 8 = 60 */
  '#','#','#','#','#','o','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
  '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
};


// Draw The Maze, And Walk It...
void maze() {
  // Maze Snake Head Position (used in some sub functions), and snake body array
  int mazeRing = 0;
  int mazeRingPos = 0;
  int mazeSnakeSegments[MAZE_SNAKE_LEN+1];
  
  // We can randomly rotate the maze in 4 steps, using 0,1,2,3 (each 90 degrees).
  int mazeRotate = random(4);

  // And we can mirror the maze along the y (vertical) axis
  boolean mazeMirror = (random(2) == 1);

  // Select randmonly from maze 1 or 2
  int mazeNumber = random(2) + 1;

  // Build some walls
  drawMazeWalls(mazeRotate, mazeMirror, mazeNumber);

  // walk the maze, until the head is free outside outer ring (#9)
  while(mazeRing<RINGS) {
    // Calculate snake head location, and draw snake
    drawSnake(startLEDs[mazeRing] + mazeRingPos, mazeSnakeSegments);

    // Determine next position for the head, follow the breadcrumbs in the maze data
    switch(getMazeData(mazeRotate, mazeMirror, mazeNumber, mazeSnakeSegments[0])) {
      case 'o': mazeStepOut(mazeRing, mazeRingPos, mazeRotate);  break;
      case 'i': mazeStepIn(mazeRing, mazeRingPos);               break;
      case '+': mazeStepClockwise(mazeRing, mazeRingPos);        break;
      case '-': mazeStepCounterClockwise(mazeRing, mazeRingPos); break;
      default : mazeRing=RINGS;                                  break;
    }
  }

  // let the snake out of the maze
  for (int i=0; i<=MAZE_SNAKE_LEN; i++) {
    drawSnake(startLEDs[RINGS], mazeSnakeSegments);
  }

  // clean up
  tearDownTheWall();
}

// Draw The Maze Walls
void drawMazeWalls(int mazeRotate, boolean mazeMirror, int mazeNumber) {
  strip.ClearTo(MAZE_BACKGROUND_COLOR);
  strip.Show();
  handlingDelay(MAZE_WALL_BUILD_DELAY);

  // We display two walls at a time (for dramatic effect), so ring 8+7, 6+5, etc...
  int displayAtRing = 7;
  
  for (int i=PIXEL_COUNT-1; i>=0;i--) {
    if (getMazeData(mazeRotate, mazeMirror, mazeNumber, i) == '#') {
      strip.SetPixelColor(i, MAZE_WALL_COLOR);
    }
    // Check if it's time to display two walls
    if (i == startLEDs[displayAtRing]) {
      strip.Show();
      handlingDelay(MAZE_WALL_BUILD_DELAY);
      displayAtRing-=2;
    }
  }
  strip.Show();
  handlingDelay(MAZE_WALL_BUILD_DELAY * 2);
}

// Tear Down The Wall (Hey, teacher, leave them snakes alone...)
void tearDownTheWall() {
  int displayAtRing = 2;
  for (int i=0; i<PIXEL_COUNT;i++) {
    // Check if it's time to display two walls
    if (i == startLEDs[displayAtRing]) {
      strip.Show();
      handlingDelay(MAZE_WALL_BUILD_DELAY);
      displayAtRing+=2;
    }
    strip.SetPixelColor(i, MAZE_BACKGROUND_COLOR);
  }
  strip.Show();
  handlingDelay(MAZE_WALL_BUILD_DELAY);
}

// Draw snake head, body, and wipe out after tail, and remember all segments properly
void drawSnake(int headPos, int mazeSnakeSegments[]) {
  mazeSnakeSegments[0] = headPos;
  // draw snake tail
  for (int i=MAZE_SNAKE_LEN; i>0; i--) {
    RgbColor color = (i == MAZE_SNAKE_LEN) ? MAZE_BACKGROUND_COLOR : MAZE_SNAKE_BODY_COLOR;
    strip.SetPixelColor(mazeSnakeSegments[i], color);
    mazeSnakeSegments[i] = mazeSnakeSegments[i-1];
  }
  // draw snake head
  strip.SetPixelColor(mazeSnakeSegments[0], MAZE_SNAKE_HEAD_COLOR);
  strip.Show();
  handlingDelay(MAZE_SNAKE_WALK_DELAY);
}

// Wrapper around the mazeData array, to make it possible to rotate the
// maze by (mazeRotate * 90) degrees, and to be able to mirror it vertically.
char getMazeData(int mazeRotate, boolean mazeMirror, int mazeNumber, int pos) {
  int ring = findRingForPos(pos);
  
  int firstPos = startLEDs[ring];
  int lastPos = startLEDs[ring+1]-1;
  int ringSize = ringSizes[ring];
  int shift = ringSize / 4 * mazeRotate;

  // Handle rotation
  pos = pos + shift;
  if (pos > lastPos) {
    pos = pos - ringSize;
  }

  // Handle mirroring
  if (mazeMirror && pos > 0) {
    // x = position withing active ring
    int x = pos - firstPos;
    if (x != 0) {
      // flip/mirror
      x = ringSize - x;
      pos = firstPos + x;
    }
  }

  // Choose between maze one and two
  char result;
  switch (mazeNumber) {
    case 1: result = pgm_read_word_near(mazeDataOne+pos); break;
    case 2: result = pgm_read_word_near(mazeDataTwo+pos); break;
  }
  // If mirroring, we also need to correct the breadcrumbs (they need mirroring too)
  if (mazeMirror) {
    if (result == '-') {
      // minus indicated you need to walk counter clockwise, change to clockwise
      result = '+';
    } else if (result == '+') {
      // plus indicated you need to walk clockwise, change to counter clockwise
      result = '-';
    }
  }
  return result;
}

// Find in which ring the given position is
int findRingForPos(int pos) {
  for (int i=0; i<RINGS; i++) {
    if (pos >= startLEDs[i] && pos < startLEDs[i+1]) {
      return i;
    }
  }
  return RINGS;
}

// Move head one ring outside
void mazeStepOut(int &mazeRing, int &mazeRingPos, int mazeRotate) {
  // When changing ring, we need to recalculate the position in the ring (using degrees), as there are different nr of leds per ring.
  int posDegrees = mazeRingPos * 360 / ringSizes[mazeRing];
  if (mazeRing == 0 && mazeRotate != 0) {
    // for our first baby step, we do need to know which direction to take (if maze is rotated).
    posDegrees = 90 * (4-mazeRotate);
  }
  mazeRing++;
  mazeRingPos = ringSizes[mazeRing] * (posDegrees + MAZE_RING_HOP_DEGREES_ROUND_UP) / 360;
}

// Move head one ring inside
void mazeStepIn(int &mazeRing, int &mazeRingPos) {
  // When changing ring, we need to recalculate the position in the ring (using degrees), as there are different nr of leds per ring.
  int posDegrees = mazeRingPos * 360 / ringSizes[mazeRing];
  mazeRing--;
  mazeRingPos = ringSizes[mazeRing] * (posDegrees + MAZE_RING_HOP_DEGREES_ROUND_UP) / 360;
}

// Move head clockwise
void mazeStepClockwise(int &mazeRing, int &mazeRingPos) {
  mazeRingPos++; if (mazeRingPos>=ringSizes[mazeRing]) { mazeRingPos=0; };
}

// Move head counter clockwise
void mazeStepCounterClockwise(int &mazeRing, int &mazeRingPos) {
  mazeRingPos--; if (mazeRingPos<0) { mazeRingPos=ringSizes[mazeRing]-1; };
}
