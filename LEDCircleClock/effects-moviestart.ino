#include "animation.h"

// MovieStart Countdown Animation.
//
// This effect is a combination of computed images, and some hand drawn digits which are rendered (single still image) using the animation function.
//
// Thijs Kaper (15/5/2022).


#define FILLER_COLOR_1 RgbColor(1, 0, 0)
#define FILLER_COLOR_2 RgbColor(0, 1, 1)
#define LINE_COLOR RgbColor(20, 20, 20)
#define CROSSHAIR_COLOR RgbColor(0, 0, 30)
#define DIGIT_COLOR RgbColor(50,0,0)

// Digit color
const RgbColor movie_downcount_colors[] = {
  DIGIT_COLOR,  // COLOR_00FFFF (originally drawn as 00FFFF, but re-mapped here)
};

// Animation color index to DIGIT_COLOR
#define COLOR_00FFFF 0

// Animation array, abused to store digits as single frame (still) animations.
// Each frame ends with the END command, so animation stops there.
// We calculate start offsets (by searching for END markers) to start at the required digit.
const PROGMEM uint8_t movie_downcount_digits[] = {
  /* load: nr_1 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 11, /* pixels: */ 0, 1, 5, 9, 15, 21, 29, 37, 49, 61, 77,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_2 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 23, /* pixels: */ 0, 2, 6, 11, 17, 23, 31, 40, 52, 61, 62, 63, 64, 74, 75, 76, 77, 78, 79, 80, 90, 91, 92,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_3 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 22, /* pixels: */ 0, 3, 11, 13, 23, 27, 40, 46, 61, 62, 63, 64, 74, 75, 76, 77, 78, 79, 80, 90, 91, 92,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_4 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 17, /* pixels: */ 0, 3, 7, 11, 12, 13, 18, 19, 23, 27, 35, 39, 47, 59, 63, 75, 91,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_5 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 20, /* pixels: */ 0, 3, 7, 13, 18, 19, 27, 35, 47, 59, 61, 62, 63, 75, 76, 77, 78, 79, 91, 92,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_6 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 23, /* pixels: */ 0, 3, 7, 13, 17, 18, 19, 27, 31, 35, 47, 51, 59, 61, 62, 63, 75, 76, 77, 78, 79, 91, 92,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_7 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 17, /* pixels: */ 0, 2, 6, 11, 16, 23, 30, 40, 50, 61, 62, 63, 64, 78, 90, 91, 92,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_8 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 27, /* pixels: */ 0, 2, 4, 6, 8, 11, 13, 17, 19, 23, 27, 31, 35, 39, 47, 51, 59, 61, 62, 63, 75, 76, 77, 78, 79, 91, 92,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,

  /* load: nr_9 */
  /* command: */ SET_PIXELS | COLOR_00FFFF, /* length: */ 23, /* pixels: */ 0, 3, 7, 11, 12, 13, 19, 23, 27, 35, 39, 47, 59, 61, 62, 63, 75, 76, 77, 78, 79, 91, 92,
  /* command: */ END | ANIMATION_DATA_BIT_MASK,
};

// Helper function to compute offsets to each digit frame in the "animation" array.
void determineDigitOffsets(int movie_downcount_digits_offsets[]) {
  movie_downcount_digits_offsets[0]=0;
  int offset=0;
  int digit=2;
  while(digit<=9) {
    // Find "END" indicator (it's value is chosen that it does not exist in the rest of the animation data; (END | ANIMATION_DATA_BIT_MASK)=255).
    while(pgm_read_byte_near(movie_downcount_digits + (offset++))!=(END | ANIMATION_DATA_BIT_MASK)) { }
    // The offset is now PAST the END, so record it as start fo next digit
    movie_downcount_digits_offsets[(digit++)-1]=offset;
  }
}

// The main movie animation start,
void moviestart() {
  // Start by finding the starts of each digit in the animation data array
  int movie_downcount_digits_offsets[9];
  determineDigitOffsets(movie_downcount_digits_offsets);

  // Background filling is alternating between two colors, this boolean is the selector
  boolean toggleFiller = false;
    
  for (int digit=9;digit>=0;digit--) {
    for (int deg=0;deg<=360;deg+=6) {
      strip.ClearTo(RgbColor(0, 0, 0));
      
      // Fill the background with alternating colors, depening in where the fast rotating line is
      for (int filler=0;filler<=360;filler+=6) {
        drawSimpleAngle(filler, RINGS, filler<=deg?(toggleFiller ? FILLER_COLOR_1 : FILLER_COLOR_2):(toggleFiller ? FILLER_COLOR_2 : FILLER_COLOR_1));
      }

      // Draw the outer circle of the "crosshair" image
      for (int r=0;r<ringSizes[7];r++) {
        setPixel(startLEDs[7]+r, CROSSHAIR_COLOR);
      }
      // Draw the cross of the "crosshair" image
      for (int c=0;c<360;c+=90) {
        drawSimpleAngle(c, RINGS, CROSSHAIR_COLOR);
      }

      // Draw the fast rotating line...
      drawSimpleAngle(deg, RINGS, LINE_COLOR);

      if (digit>0) {
        // Draw digit, for 0 skip digit (so we show an empty circle animation once)
        showAnimation(movie_downcount_digits + movie_downcount_digits_offsets[digit-1], movie_downcount_colors, /* delays not used: */ 0);
      }

      // Show end-result
      strip.Show();
      // We do 60 steps per turn, 1000ms/60 is approximately a delay of 16ms per step.
      handlingDelay(16);
    }

    toggleFiller=!toggleFiller;
  }

  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
  handlingDelay(500);
}
