// Snail Animation - In it's little round house, getting out, and then hiding again. Drawn by (my daughter) Elly.
//
// Note: to draw/design this animation, I first created a web based "drawing tool / program" with which you can
// make drawings/animations. The data from that tool can be exported as the big animation array.
//
// Implemented by: Thijs Kaper (30/4/2022).

#define membersof(x) (sizeof(x) / sizeof(x[0]))

#include "animation.h"

// Color indexes
#define COLOR_000000 0
#define COLOR_00FF00 1
#define COLOR_AA4400 2
#define COLOR_FF8800 3

// Color list
const RgbColor snail_colors[] = {
  RgbColor(0, 0, 0),   // COLOR_000000 background
  RgbColor(0, 5, 0),   // COLOR_00FF00 snail-body (green)
  RgbColor(9, 3, 0),   // COLOR_AA4400 snail-house dark orangy-yellow
  RgbColor(25, 13, 0), // COLOR_FF8800 snail-house light orangy-yellow
};

// Delay indexes
#define DELAY_200MS 0
#define DELAY_300MS 1
#define DELAY_900MS 2

// Delay list
const int snail_delays[] = {
  200,
  300,
  900,
};

// The animation data (commands / data) - see animation.ino and animation.h for an explanation of this array construction.
const PROGMEM uint8_t snail_animation[] = {
  /* animation: elly's snail */
  
  /* load: snail_12 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AA4400, /* length: */ 10, /* pixels: */ 0, 3, 9, 10, 11, 16, 17, 18, 19, 20,
  /* command: */ SET_PIXELS | COLOR_FF8800, /* length: */ 25, /* pixels: */ 1, 2, 4, 5, 6, 7, 8, 12, 13, 14, 15, 21, 22, 23, 24, 25, 26, 27, 28, 31, 32, 33, 34, 35, 36,
  
  /* delay: 900ms */
  /* command: */ WAIT | DELAY_900MS,
  
  /* load: snail_11 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 2, /* pixels: */ 29, 30,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_10 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 2, /* pixels: */ 51, 52,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_09 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 3, /* pixels: */ 49, 50, 79,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_08 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 3, /* pixels: */ 48, 80, 117,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_07 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 5, /* pixels: */ 28, 53, 77, 81, 119,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 117,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_06 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 4, /* pixels: */ 54, 82, 83, 121,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 79, 119,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_05 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 54, 77, 80,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 5, /* pixels: */ 76, 84, 85, 120, 167,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_04 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 5, /* pixels: */ 47, 55, 75, 122, 169,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 76, 85, 167,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_03 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 55, 75, 169,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 5, /* pixels: */ 56, 74, 85, 123, 170,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_02 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 6, /* pixels: */ 46, 73, 86, 87, 124, 171,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 56, 74, 170,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_01 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 4, /* pixels: */ 72, 74, 88, 229,

  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* load: snail_00e */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 228,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 229,
  
  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* load: snail_00d */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 88,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 126,
  
  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* load: snail_00c */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 88,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 126,
  
  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* load: snail_00b */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 228,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 229,
  
  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* load: snail_00a */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 57,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 88,
  
  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* load: snail_00 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 172,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 229,
  
  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* load: snail_0 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 57,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 88,
  
  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,

  /* load: snail_01 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 172,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 229,

  /* delay: 200ms */
  /* command: */ WAIT | DELAY_200MS,

  /* load: snail_02 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 72, 74, 88, 229,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_03 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 6, /* pixels: */ 46, 73, 86, 87, 124, 171,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 3, /* pixels: */ 56, 74, 170,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_04 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 3, /* pixels: */ 55, 75, 169,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 5, /* pixels: */ 56, 74, 85, 123, 170,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_05 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 5, /* pixels: */ 47, 55, 75, 122, 169,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 3, /* pixels: */ 76, 85, 167,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_06 */
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 3, /* pixels: */ 54, 77, 80,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 5, /* pixels: */ 76, 84, 85, 120, 167,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_07 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 54, 82, 83, 121,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 2, /* pixels: */ 79, 119,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_08 */
  /* command: */ SET_PIXELS | COLOR_FF8800, /* length: */ 1, /* pixels: */ 28,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 53, 77, 81, 119,
  /* command: */ SET_PIXELS | COLOR_00FF00, /* length: */ 1, /* pixels: */ 117,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_09 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 48, 80, 117,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_10 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 49, 50, 79,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_11 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 51, 52,
  
  /* delay: 250ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: snail_12 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 29, 30,

  /* delay: 250ms */
  /* command: */ WAIT | DELAY_900MS,

  /* wipe ring */
  /* command: */ SET_RANGE | COLOR_000000, /* start, end: */ 21, 36,

  /* delay: 250ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* wipe ring */
  /* command: */ SET_RANGE | COLOR_000000, /* start, end: */ 9, 20,

  /* delay: 250ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* wipe ring */
  /* command: */ SET_RANGE | COLOR_000000, /* start, end: */ 1, 8,

  /* delay: 250ms */
  /* command: */ WAIT | DELAY_200MS,

  /* wipe last dot */
  /* command: */ CLEAR_ALL | COLOR_000000,

  /* delay: 250ms */
  /* command: */ WAIT | DELAY_200MS,
  
  /* command: */ END,
};

void snail() {
  showAnimation(snail_animation, snail_colors, snail_delays);
}
