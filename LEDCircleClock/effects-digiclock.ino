
// DigiClock effect.
// Showing a digital clock, instead of the analog one.
// To keep it a bit analog, it shows a pendulum.
// Disclaimer: a ring display does not lead to high grade 7-segment digits ;-)
//
// Thijs Kaper (17/7/2021)

// Red-Blue-Neon
//#define DIGICLOCK_BACKGROUND RgbColor(0, 0, 0)
//#define DIGICLOCK_DIGITS     RgbColor(60, 0, 0)
//#define DIGICLOCK_IMAGE      RgbColor(0, 0, 60)
//#define DIGICLOCK_COLON      RgbColor(60, 60, 0)
//#define DIGICLOCK_COLON2     RgbColor(0, 0, 60)
//#define DIGICLOCK_PENDULUM   RgbColor(60, 0, 60)

// New-Reto-Wave
#define DIGICLOCK_BACKGROUND RgbColor(0, 0, 0)
#define DIGICLOCK_DIGITS     RgbColor(50, 15, 0)
#define DIGICLOCK_IMAGE      RgbColor(23, 0, 12)
#define DIGICLOCK_COLON      RgbColor(28, 3, 7)
#define DIGICLOCK_COLON2     RgbColor(0, 14, 30)
#define DIGICLOCK_PENDULUM   RgbColor(0, 14, 30)

// 7 segment definitions for numbers 0..9
// The segments are numbered like this:
// 
//  AAAAA
// F     B
// F     B
// F     B
//  GGGGG
// E     C
// E     C
// E     C
//  DDDDD
//
// To save some space, the letters are mapped to bits in a byte.
// Byte: 0ABCDEFG
// Bit#: 76543210
//
const byte nrToSegments[] = {
  /*           -ABCDEFG     ABCDEFG */
  /* nr 0 */ 0b01111110, // ABCDEF
  /* nr 1 */ 0b00110000, // -BC----
  /* nr 2 */ 0b01101101, // AB-DE-G
  /* nr 3 */ 0b01111001, // ABCD--G
  /* nr 4 */ 0b00110011, // -BC--FG
  /* nr 5 */ 0b01011011, // A-CD-FG
  /* nr 6 */ 0b01011111, // A-CDEFG
  /* nr 7 */ 0b01110000, // ABC----
  /* nr 8 */ 0b01111111, // ABCDEFG
  /* nr 9 */ 0b01111011, // ABCD-FG
};

// Mapping of segments to PIXELS
// Inices:
// - first  - 4 digit positions, 0 to 3 from left to right.
// - second - 7 segments per digit. 01234567 = GFEDCBA.
// - third  - 3 series per segment (we need between 1 and 3 descriptions).
// - fourth - 2 pieces of info per description. Info[0] = start-pixel, and Info[1] = number of pixels (clockwise).
// Info set to zero if not needed.
const uint8_t segments[4][7][3][2] = {
  { 
    // Leftmost digit (0):
    { { 226, 1}, { 169, 1}, { 123, 1}, }, // G
    { { 226, 6}, {   0, 0}, {   0, 0}, }, // F
    { { 221, 6}, {   0, 0}, {   0, 0}, }, // E
    { { 221, 1}, { 165, 1}, { 120, 1}, }, // D
    { { 120, 4}, {   0, 0}, {   0, 0}, }, // C
    { { 123, 4}, {   0, 0}, {   0, 0}, }, // B
    { { 231, 1}, { 173, 1}, { 126, 1}, }, // A
  },
  { 
    // Second digit from the left (1):
    { {  55, 1}, {  33, 1}, {  18, 1}, }, // G
    { {  55, 4}, {   0, 0}, {   0, 0}, }, // F
    { {  52, 4}, {   0, 0}, {   0, 0}, }, // E
    { {  52, 1}, {  31, 1}, {  17, 0}, }, // D // disabled dot in D (fixes rendering of 2,5,6)
    { {  17, 2}, {   0, 0}, {   0, 0}, }, // C
    { {  18, 2}, {   0, 0}, {   0, 0}, }, // B
    { {  58, 1}, {  35, 1}, {  19, 0}, }, // A // disabled dot in A (fixes rendering of 2,5,6)
  },
  { 
    // Third digit from the left (2):
    { {  12, 1}, {  25, 1}, {  43, 1}, }, // G
    { {  11, 2}, {   0, 0}, {   0, 0}, }, // F
    { {  12, 2}, {   0, 0}, {   0, 0}, }, // E
    { {  13, 0}, {  27, 1}, {  46, 1}, }, // D // disabled dot in D (fixes rendering of 2,3,5,9)
    { {  43, 4}, {   0, 0}, {   0, 0}, }, // C
    { {  40, 4}, {   0, 0}, {   0, 0}, }, // B
    { {  11, 0}, {  23, 1}, {  40, 1}, }, // A // disabled dot in A (fixes rendering of 2,3,5,9)
  },
  { 
    // Fourth digit from the left (3):
    { { 103, 1}, { 145, 1}, { 196, 1}, }, // G
    { { 100, 4}, {   0, 0}, {   0, 0}, }, // F
    { { 103, 4}, {   0, 0}, {   0, 0}, }, // E
    { { 106, 1}, { 149, 1}, { 201, 1}, }, // D
    { { 196, 6}, {   0, 0}, {   0, 0}, }, // C
    { { 191, 6}, {   0, 0}, {   0, 0}, }, // B
    { { 100, 1}, { 141, 1}, { 191, 1}, }, // A
  },
};

// static bacground image, using pairs with start and segment length.
// Info[0] = start-pixel, and Info[1] = number of pixels (clockwise).
const uint8_t image[][2] = {
  { 234, 1}, { 237, 4}, { 181, 5}, {188, 1}, // top row arc and dots
  { 176, 1}, { 138, 1},                      // second row dots
  { 129, 4}, { 93, 5},                       // third row arc
  { 109, 9},                                 // third from bottom arc
  { 152, 1}, { 162, 1},                      // second from bottom dots
  { 204, 1}, { 207, 9}, { 218, 1},           // bottom row arc and dots
};

#define membersof(x) (sizeof(x) / sizeof(x[0]))

// Draw clock, background image, and pendulum.
void digiclock() {
  time_t digiTime = time(nullptr);
  struct tm * digiTimeInfo;
  
  // Together with delay 50, runing 400 times shows the clock for 20 seconds.
  for (int loop=0; loop<400;loop++) {
    // clear
    strip.ClearTo(DIGICLOCK_BACKGROUND);

    // Show background image
    digiclockDrawArcs(image, membersof(image), DIGICLOCK_IMAGE);

    // Get us the current time
    digiTime = time(nullptr); // time_t = seconds since epoch
    digiTimeInfo = localtime (&digiTime); // setup digiTimeInfo -> tm_hour, digiTimeInfo -> tm_min, digiTimeInfo -> tm_sec

    // split in 4 separate digits
    int splitDigits[] = { 
      (digiTimeInfo -> tm_hour / 10),
      (digiTimeInfo -> tm_hour % 10),
      (digiTimeInfo -> tm_min / 10),
      (digiTimeInfo -> tm_min % 10),
    };

    // show the digits
    for (int d=0; d<4;d++) {
      digiclockDrawDigit(d, splitDigits[d]);
    }

    // show pendulum (and flashing colon)
    digiclockDrawPendulum();
    
    strip.Show();
    handlingDelay(50);
  }
}

// Draw 7-segment digit
void digiclockDrawDigit(int pos, int value) {
  byte pattern = nrToSegments[value]; // binary bit values: 0ABCDEFG
  for (int bitpos=0; bitpos<7;bitpos++) {
    int bitval = pattern & 1;
    pattern = pattern >> 1;
    if (bitval) {
      digiclockDrawArcs(segments[pos][bitpos], 3, DIGICLOCK_DIGITS);
    }
  }
  
}

// Draw pendulum (and flashing colon)
void digiclockDrawPendulum() {
  int pendulum;
  // calculate and display pendulum
  int millisOfSecond = millis() % 1000L;
  if ((millis() % 2000L)>=1000) {
    strip.SetPixelColor(1, DIGICLOCK_COLON);
    strip.SetPixelColor(5, DIGICLOCK_COLON);
    pendulum = (9 * millisOfSecond + 0.5) / 1000;
  } else {
    strip.SetPixelColor(1, DIGICLOCK_COLON2);
    strip.SetPixelColor(5, DIGICLOCK_COLON2);
    pendulum = 9 - (9 * millisOfSecond + 0.5) / 1000;
  }
  strip.SetPixelColor(153 + pendulum, DIGICLOCK_PENDULUM);
  if (pendulum < 4) {
    strip.SetPixelColor(177 + pendulum, DIGICLOCK_PENDULUM);
  } else {
    strip.SetPixelColor(133 + pendulum - 4, DIGICLOCK_PENDULUM);
  }
}

// Draw one or more arcs
void digiclockDrawArcs(const uint8_t arcs[][2], int nrOfArcs, RgbColor color) {
  for(int i=0;i<nrOfArcs;i++) {
    int start = arcs[i][0];
    int len = arcs[i][1];
    if (len != 0) {
      for (int j=0;j<len;j++) {
        strip.SetPixelColor(start+j, color);
      }
    }
  }
}
