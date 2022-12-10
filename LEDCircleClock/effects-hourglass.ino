// HourGlass Animation - Turning, and ticking away it's sandy time.
// Initial standing shape drawn by (my daughter) Elly, rotation and sand animation drawn by me (Thijs).
//
// Note: to draw/design this animation, I first created a web based "drawing tool / program" with which you can
// make drawings/animations. The data from that tool can be exported as the big animation array.
//
// Implemented by: Thijs Kaper (30/4/2022).

#include "animation.h"

// Color indexes
#define COLOR_000000 0
#define COLOR_AAAAAA 1
#define COLOR_FFAA00 2

// Color list
const RgbColor hourglass_colors[] = {
  RgbColor(0, 0, 0),  // COLOR_000000 background
  RgbColor(20,20,20), // COLOR_AAAAAA glass
  RgbColor(42,20,0),  // COLOR_FFAA00 sand
};

// Delay indexes
#define DELAY_100MS 0
#define DELAY_300MS 1
#define DELAY_800MS 2
#define DELAY_900MS 3

// Delay list
const int hourglass_delays[] = {
  100, // DELAY_100MS
  300, // DELAY_300MS
  800, // DELAY_800MS
  900, // DELAY_900MS
};

// The animation data (commands / data) - see animation.ino and animation.h for an explanation of this array construction.
const PROGMEM uint8_t hourglass_animation[] = {
  /* animation: elly's hourglass */
  
  /* load: hourglass */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 2, 3, 4, 6, 7, 8, 10, 14, 16, 20, 23, 27, 31, 35, 40, 46, 52, 58, 65, 73, 81, 89, 98, 108, 118, 128, 138, 152, 162, 176, 186, 206, 216, 236,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 26, /* pixels: */ 77, 110, 111, 112, 113, 114, 115, 116, 153, 154, 155, 156, 157, 158, 159, 160, 161, 207, 208, 209, 210, 211, 212, 213, 214, 215,
  
  /* delay: 900ms */
  /* command: */ WAIT | DELAY_900MS,
  
  /* load: hourglass-rotate-033 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 1, 3, 4, 6, 7, 8, 11, 14, 17, 20, 24, 28, 32, 36, 41, 47, 54, 59, 67, 75, 83, 91, 100, 111, 120, 131, 141, 155, 165, 179, 191, 209, 221, 239,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 26, /* pixels: */ 79, 112, 113, 114, 115, 116, 117, 118, 156, 157, 158, 159, 160, 161, 162, 163, 164, 210, 211, 212, 213, 214, 215, 216, 217, 218,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass-rotate-045 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 1, 3, 4, 5, 7, 8, 9, 12, 15, 18, 21, 25, 29, 33, 37, 43, 49, 55, 61, 69, 77, 85, 93, 103, 113, 123, 134, 144, 158, 168, 183, 194, 213, 224,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 24, /* pixels: */ 50, 78, 79, 80, 114, 115, 116, 117, 118, 159, 160, 161, 162, 163, 164, 165, 214, 215, 216, 217, 218, 219, 220, 221,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass-rotate-078 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 1, 3, 4, 5, 6, 8, 10, 12, 16, 19, 22, 26, 30, 34, 39, 45, 51, 57, 63, 71, 80, 87, 96, 106, 117, 126, 138, 148, 162, 172, 188, 199, 218, 229,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 26, /* pixels: */ 52, 53, 81, 82, 83, 84, 118, 119, 120, 121, 122, 163, 164, 165, 166, 167, 168, 169, 219, 220, 221, 222, 223, 224, 225, 226,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass-rotate-090 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 1, 2, 4, 5, 6, 8, 11, 13, 17, 19, 23, 27, 31, 35, 40, 46, 52, 58, 65, 73, 81, 89, 98, 108, 118, 128, 140, 150, 164, 174, 191, 201, 221, 231,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 24, /* pixels: */ 32, 53, 54, 82, 83, 84, 85, 119, 120, 121, 122, 123, 165, 166, 167, 168, 169, 170, 222, 223, 224, 225, 226, 227,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass-rotate-113 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 1, 2, 4, 5, 6, 7, 11, 14, 17, 20, 24, 28, 32, 36, 41, 47, 53, 59, 67, 75, 83, 91, 101, 110, 121, 130, 143, 153, 167, 177, 194, 205, 224, 235,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 24, /* pixels: */ 18, 33, 54, 55, 56, 84, 85, 86, 87, 122, 123, 124, 125, 168, 169, 170, 171, 172, 225, 226, 227, 228, 229, 230,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass-rotate-135 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 1, 2, 3, 5, 6, 7, 9, 12, 15, 18, 21, 25, 29, 33, 37, 43, 49, 55, 61, 69, 77, 85, 93, 103, 113, 123, 146, 156, 170, 180, 198, 209, 228, 239,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 24, /* pixels: */ 19, 20, 34, 35, 56, 57, 58, 86, 87, 88, 89, 124, 125, 126, 127, 128, 171, 172, 173, 174, 229, 230, 231, 232,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass-rotate-158 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 2, 3, 5, 6, 7, 8, 10, 13, 15, 19, 22, 26, 30, 34, 38, 45, 50, 56, 63, 71, 79, 87, 95, 106, 115, 126, 135, 149, 159, 173, 183, 201, 213, 231,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 24, /* pixels: */ 9, 20, 21, 35, 36, 57, 58, 59, 60, 88, 89, 90, 91, 127, 128, 129, 130, 174, 175, 176, 177, 232, 233, 234,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass-rotate-180 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 34, /* pixels: */ 2, 3, 4, 6, 7, 8, 10, 14, 16, 20, 23, 27, 31, 35, 40, 46, 52, 58, 65, 73, 81, 89, 98, 108, 118, 128, 138, 152, 162, 176, 186, 206, 216, 236,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 24, /* pixels: */ 0, 1, 9, 21, 22, 36, 37, 38, 39, 59, 60, 61, 62, 63, 90, 91, 92, 129, 130, 131, 132, 177, 178, 237,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-01 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 0, 237,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 2, /* pixels: */ 5, 64,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-02 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 0, 15, 97,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 1, 5, 178,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-03 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 0, 15, 61, 132,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 1, 5, 29, 96,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-04 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 0, 15, 49,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 5, 29, 37,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-05 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 5, 29, 37, 77,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 15, 49, 131, 177,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-06 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 15, 49, 113,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 29, 37, 77,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-07 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 15, 49, 62, 113,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 29, 37, 77, 157,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-08 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 15, 49, 113, 211,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 29, 77, 92, 157,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-09 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 5, 49, 113,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 29, 77, 157,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-10 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 5, 49, 113, 212,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 15, 37, 77, 157,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-11 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 15, 77, 157,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 29, 38, 113,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-12 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 29, 38, 113, 210,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 49, 60, 63, 157,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-13 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 49, 60, 157,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 77, 130,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-14 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 29, 96, 113,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 63, 77, 158,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-15 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 5, 49, 63, 158,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 29, 37, 113, 213,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-16 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 5, 49, 158,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 15, 37, 77,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-17 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 1, 29, 113, 129,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 15, 37, 77, 156,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-18 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 0, 37, 49, 97,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 1, 29, 63, 113,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-19 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 0, 49, 112,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 5, 63, 77,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-20 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 5, 77, 209,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 15, 91, 112,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-21 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 15, 112, 114,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 29, 38, 49,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-22 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 21, 77, 114,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 49, 159, 208,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-23 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 21, 38, 77, 155,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 49, 64, 90, 112,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-24 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 15, 21, 77, 159,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 29, 49, 114, 214,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-25 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 15, 21, 77, 159,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 29, 49, 60,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-26 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 15, 38, 77,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 29, 49, 112,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-27 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 15, 77, 215,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 21, 29, 49,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-28 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 15, 39,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 2, /* pixels: */ 29, 49,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-29 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 49, 59,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 2, /* pixels: */ 76, 111,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-30 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 15, 78, 154, 207,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 22, 29, 76, 111,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-31 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 5, 36, 78,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 29, 49, 160,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-32 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 3, /* pixels: */ 5, 78, 115,
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 3, /* pixels: */ 9, 15, 49,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-33 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 1, 29,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 2, /* pixels: */ 15, 111,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-34 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 0, 5,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 2, /* pixels: */ 29, 49,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-35 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 15, 29,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 2, /* pixels: */ 48, 76,
  
  /* delay: 300ms */
  /* command: */ WAIT | DELAY_300MS,
  
  /* load: hourglass-drop-36 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 4, /* pixels: */ 48, 49, 76, 78,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 4, /* pixels: */ 110, 116, 153, 161,
  
  /* delay: 800ms */
  /* command: */ WAIT | DELAY_800MS,
  
  /* command: */ END,
};

const PROGMEM uint8_t hourglass_fade_in_animation[] = {
  /* load: hourglass_fade_19 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_18 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 206, 216,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 9, /* pixels: */ 207, 208, 209, 210, 211, 212, 213, 214, 215,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_17 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 152, 162,
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 9, /* pixels: */ 153, 154, 155, 156, 157, 158, 159, 160, 161,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_16 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 7, /* pixels: */ 110, 111, 112, 113, 114, 115, 116,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_15 */
  /* command: */ SET_PIXELS | COLOR_FFAA00, /* length: */ 1, /* pixels: */ 77,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_14 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 108, 118,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_13 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 73, 81,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_12 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 46, 52,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_11 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 27, 31,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_10 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 14, 16,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_9 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 4, 6,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_8 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 3, 7,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_7 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 2, 8,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_6 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 10, 20,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_5 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 23, 35,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_4 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 40, 58,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_3 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 65, 89,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_2 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 98, 128,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_1 */
  /* command: */ SET_PIXELS | COLOR_AAAAAA, /* length: */ 2, /* pixels: */ 138, 176,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* command: */ END,
};

const PROGMEM uint8_t hourglass_fade_out_animation[] = {
  /* load: hourglass_fade_1 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 186, 236,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_2 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 138, 176,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_3 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 98, 128,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_4 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 65, 89,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_5 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 40, 58,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_6 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 23, 35,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_7 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 10, 20,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_8 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 2, 8,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_9 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 3, 7,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_10 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 4, 6,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_11 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 14, 16,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_12 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 27, 31,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_13 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 46, 52,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_14 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 73, 81,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_15 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 2, /* pixels: */ 108, 118,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_16 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 1, /* pixels: */ 77,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_17 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 7, /* pixels: */ 110, 111, 112, 113, 114, 115, 116,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_18 */
  /* command: */ SET_PIXELS | COLOR_000000, /* length: */ 11, /* pixels: */ 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* load: hourglass_fade_19 */
  /* command: */ CLEAR_ALL | COLOR_000000,
  
  /* delay: 100ms */
  /* command: */ WAIT | DELAY_100MS,
  
  /* command: */ END,
};

void hourglass() {
  // Build hourglass from the ground up (fade-in)
  showAnimation(hourglass_fade_in_animation, hourglass_colors, hourglass_delays);
  
  // Show hourglass animation twice
  for(int x=0;x<2;x++) {
    showAnimation(hourglass_animation, hourglass_colors, hourglass_delays);
  }
  // Fade-out
  showAnimation(hourglass_fade_out_animation, hourglass_colors, hourglass_delays);
}
