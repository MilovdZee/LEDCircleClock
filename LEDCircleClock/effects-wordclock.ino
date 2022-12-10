#include <math.h>

// Word Clock & HAL9001 (2001: A Space Odyssey) Quotes.
//
// In this effect, I found space at the top and bottom of the clock to show a text scroller,
// using a custom 3x5 pixel font. In the middle you see a slowly fading red eye (=HAL9001).
// At the sides, there are some blue fillers (metal shields), which remind a bit of portal game elements.
// 
// The time is converted to a text, and scrolled twice (as the first time you probably miss what it reads).
// Then afterwards, a random 2001 quote will be shown.
//
// Note: I did try showing two different texts (at bottom and top), but doing that will make sure that
// the viewer will be able to read only one of them ;-) It seems to be too confusing to show different ones.
//
// Thijs Kaper (7/5/2022).

// The side filler color
#define FILLER RgbColor(0,0,8)

// The scroll-text characters use alternating colors, to help the viewer following the scrolling properly.
// If you use the same color for all, some texts behave odd, and become unreadable (as your brain does not
// see it scrolling to the left in that case, but it just seems to keep alternating between some patterns).
#define TXT1 RgbColor(6,10,14) /* light-cyan */
#define TXT2 RgbColor(10,10,10) /* white */
//#define TXT1 RgbColor(0,10,0) /* green */
//#define TXT2 RgbColor(10,10,0) /* yellow */

// The red eye fade configuration
#define START_FADE 4.0
#define MAX_FADE 20.0
#define EYE_RING_0_RED 50
#define EYE_RING_1_RED 35
#define EYE_RING_2_RED 18
#define EYE_RING_3_RED 5

#define membersof(x) (sizeof(x) / sizeof(x[0]))

// The max length of a quote or time as text info (time text is roughly 48 chars).
// So keep this a bit longer than the longest quote, and longer than 48.
#define MAX_TEXT_LENGTH 80

// Some HAL9001 quotes, if changing the set, keep the count to match the array string count.
#define HAL9001_QUOTE_COUNT 7
const PROGMEM char* hal9001_quotes[] = {
  "I'm sorry Dave, I'm afraid I can't do that ",
  "Daisy, Daisy, give me your answer do ",
  "It can only be attributable to human error ",
  "Will you stop Dave? Stop, Dave",
  "Just what do you think you're doing, Dave? ",
  "The 9000 series is the most reliable computer ever made ",
  "", /* empty string, so sometimes you do not see a quote ;-) */
};

// Number-to-text data: [0]=zero..[20]=twenty, [21]=thirty, [22]=fourty, [23]=fifty.
const PROGMEM char* numbers[] = {
  "zero", "one","two","three","four","five","six","seven","eight","nine","ten",
  "eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen","twenty",
  "thirty","fourty","fifty"
};

// Font 3x5, bits left to right = rows top to bottom, columns left to right (last bit always zero).
// Partial ascii map, starting at " " (0x20), running consecutively up to "Z" (0x5a).
const PROGMEM uint16_t charset_3x5[] = {
  /*   = 0000000000000000 */ 0x0000,
  /* ! = 0000000000111010 */ 0x003A,
  /* " = 1100000000110000 */ 0xC030,
  /* # = 1111101010111110 */ 0xFABE,
  /* $ = 1110111111101110 */ 0xEFEE,
  /* % = 1001100100110010 */ 0x9932,
  /* & = 1111110101001010 */ 0xFD4A,
  /* ' = 0000000000110000 */ 0x0030,
  /* ( = 0000001110100010 */ 0x03A2,
  /* ) = 0000010001011100 */ 0x045C,
  /* * = 0000000000000010 */ 0x0002,
  /* + = 0010001110001000 */ 0x2388,
  /* , = 0000000001000110 */ 0x0046,
  /* - = 0010000100001000 */ 0x2108,
  /* . = 0000000000000010 */ 0x0002,
  /* / = 0001100100110000 */ 0x1930,
  /* 0 = 0111010001011100 */ 0x745C,
  /* 1 = 0100111111000010 */ 0x4FC2,
  /* 2 = 1001110101010010 */ 0x9D52,
  /* 3 = 1000110101111110 */ 0x8D7E,
  /* 4 = 1110000100111110 */ 0xE13E,
  /* 5 = 1110110101101100 */ 0xED6C,
  /* 6 = 0111110101101110 */ 0x7D6E,
  /* 7 = 1001110100110000 */ 0x9D30,
  /* 8 = 1111110101111110 */ 0xFD7E,
  /* 9 = 1110110101111100 */ 0xED7C,
  /* : = 0000000000010100 */ 0x0014,
  /* ; = 0000000010010110 */ 0x0096,
  /* < = 0010001010100010 */ 0x22A2,
  /* = = 0101001010010100 */ 0x5294,
  /* > = 1000101010001000 */ 0x8A88,
  /* ? = 1000010101111000 */ 0x8578,
  /* @ = 1111110001111010 */ 0xFC7A,
  /* A = 1111110100111110 */ 0xFD3E,
  /* B = 1111110101010100 */ 0xFD54,
  /* C = 0111010001100010 */ 0x7462,
  /* D = 1111110001011100 */ 0xFC5C,
  /* E = 1111110101101010 */ 0xFD6A,
  /* F = 1111110100101000 */ 0xFD28,
  /* G = 0111010001101110 */ 0x746E,
  /* H = 1111100100111110 */ 0xF93E,
  /* I = 1000111111100010 */ 0x8FE2,
  /* J = 0001100001111110 */ 0x187E,
  /* K = 1111100100110110 */ 0xF936,
  /* L = 1111100001000010 */ 0xF842,
  /* M = 1111101000111110 */ 0xFA3E,
  /* N = 1111110000111110 */ 0xFC3E,
  /* O = 1111110001111110 */ 0xFC7E,
  /* P = 1111110100111000 */ 0xFD38,
  /* Q = 1111010010111110 */ 0xF4BE,
  /* R = 1111110100010110 */ 0xFD16,
  /* S = 0100110101100100 */ 0x4D64,
  /* T = 1000011111100000 */ 0x87E0,
  /* U = 1111100001111110 */ 0xF87E,
  /* V = 1111100001111100 */ 0xF87C,
  /* W = 1111100010111110 */ 0xF8BE,
  /* X = 1101100100110110 */ 0xD936,
  /* Y = 1110000111111000 */ 0xE1F8,
  /* Z = 1001110101110010 */ 0x9D72,
};

// top led lane for text display: pixels top to bottom, left to right, 5 rows x 9 bars (bar by bar).
const PROGMEM uint8_t top_bar[] = {
  /* bar-top-00: */ 237, 177, 129, 89, 57,
  /* bar-top-01: */ 238, 178, 130, 90, 58,
  /* bar-top-02: */ 239, 179, 131, 91, 59,
  /* bar-top-03: */ 240, 180, 132, 92, 60,
  /* bar-top-04: */ 181, 133, 93, 61, 37,
  /* bar-top-05: */ 182, 134, 94, 62, 38,
  /* bar-top-06: */ 183, 135, 95, 63, 39,
  /* bar-top-07: */ 184, 136, 96, 64, 40,
  /* bar-top-08: */ 185, 137, 97, 65, 41,
};

// bottom led lane for text display: pixels top to bottom, right to left, 5 rows x 9 bars (bar by bar).
const PROGMEM uint8_t bottom_bar[] = {
  /* bar-bottom-00: */ 53, 81, 117, 161, 215,
  /* bar-bottom-01: */ 52, 80, 116, 160, 214,
  /* bar-bottom-02: */ 51, 79, 115, 159, 213,
  /* bar-bottom-03: */ 50, 78, 114, 158, 212,
  /* bar-bottom-04: */ 49, 77, 113, 157, 211,
  /* bar-bottom-05: */ 48, 76, 112, 156, 210,
  /* bar-bottom-06: */ 47, 75, 111, 155, 209,
  /* bar-bottom-07: */ 46, 74, 110, 154, 208,
  /* bar-bottom-08: */ 45, 73, 109, 153, 207,
};

// info for drawing the side "shields" / fillers.
const PROGMEM uint8_t fillers[] {
  // startled,endled
  227,235, // ordered low/high.
  170,175,
  124,127,
  86,87,
  
  68, 67,  // swapped high/low (for fade-in effect).
  102, 99,
  144, 139,
  195, 187,

  225, 217, // swapped high/low (for fade-in effect).
  168, 163,
  122, 119,
  84, 83,

  70,71, // ordered low/high.
  104,107,
  146,151,
  197,205,
};

// Draw a single character on a given column position.
void drawChar(char ch, int col, boolean topRow, RgbColor color) {
  // Allow drawing at columns -2 up to and including 8.
  // Having col=-2 will draw the rightmost column of a 3x5 character.
  if (col<-2 || col>8) return;

  // Map lowercase to uppercase, and map unknown characters to dot.
  if (ch>='a' && ch<='z') { ch=ch-'a'+'A'; }
  if (ch < ' ') ch = '.';
  if (ch > 'Z') ch = '.';

  // Get pixel-data by shifting the ascii-code index down by the space value (which is first char in table).
  uint16_t pixels = pgm_read_word_near(charset_3x5 + ch - ' ');

  // Compute start pixel (may be negative, as chars scroll out of the left side).
  int pixelIndex = 5*col;
  // Loop through all 15 charset pixel data bits.
  for(int i=0;i<15;i++) {
    if (pixels & 0x8000) {
      // Only draw inside visible range.
      if ((pixelIndex>=0) && (pixelIndex<(5*9))) {
        strip.SetPixelColor(topRow ? pgm_read_byte_near(top_bar + pixelIndex) : pgm_read_byte_near(bottom_bar + pixelIndex), color);
      }
    }
    pixelIndex++;
    pixels<<=1;
  }
}

// Draw a text starting at a given column. Using alternating font colors.
void drawText(char *ch, int col, boolean topRow, RgbColor color1, RgbColor color2) {
  boolean charColorToggle = topRow;
  while(*ch && col<21) {
    drawChar(*(ch++), col, topRow, charColorToggle ? color1 : color2);
    if (*ch != ' ') {
      // Only toggle color if not rendering a space.
      charColorToggle = !charColorToggle;
    }
    col+=4;
  }
}

// Draw the "shields" / fillers. The percentage is used for fade in/out.
void fillFillers(RgbColor color, int percentage) {
  for(int i=0;i<membersof(fillers)/2;i++) {
    int start = pgm_read_byte_near(fillers + (i*2));
    int end = pgm_read_byte_near(fillers + (i*2+1));
    int maxSteps = round((1+abs(start-end))*percentage/100);
    if (start<end) {
      for (int x=start;x<=end && (maxSteps--)>0;x++) {
        strip.SetPixelColor(x, color);
      }
    } else {
      for (int x=start;x>=end && (maxSteps--)>0;x--) {
        strip.SetPixelColor(x, color);
      }
    }
  }
}


float f=START_FADE;
int fDir=1;

// Draw the red eye. Percentage is used to fade in/out the eye at start/end of the effect.
// Every call to this function will redraw the eye, AND change it's FADING STATE "f".
void fillEye(int percentage) {
  strip.SetPixelColor(0, RgbColor(round((EYE_RING_0_RED*f/MAX_FADE)*percentage/100),0,0));
  for (int i=1;i<=8;i++) {
    strip.SetPixelColor(i, RgbColor(round((EYE_RING_1_RED*f/MAX_FADE)*percentage/100),0,0));
  }
  for (int i=9;i<=20;i++) {
    strip.SetPixelColor(i, RgbColor(round((EYE_RING_2_RED*f/MAX_FADE)*percentage/100),0,0));
  }
  for (int i=21;i<=36;i++) {
    strip.SetPixelColor(i, RgbColor(round((EYE_RING_3_RED*f/MAX_FADE)*percentage/100),0,0));
  }
  f+=fDir;
  if (f>MAX_FADE) {
    f=MAX_FADE;
    fDir=-1;
  }
  if (f<START_FADE) {
    f=START_FADE;
    fDir=1;
  }
}


char nr_text[15];

// Convert number to word (nr between 0 and 59), used for hours and minutes.
char * number2word(int nr) {
  if(nr<0 || nr>59) {
    return "?";
  }
  if(nr>19 && nr%10==0) {
    strcpy_P(nr_text, numbers[18+nr/10]);
    return nr_text;
  }
  if(nr>20) {
    char tmp_copy_1[15], tmp_copy_2[15];
    strcpy_P(tmp_copy_1, numbers[18+int(nr/10)]);
    strcpy_P(tmp_copy_2, numbers[nr%10]);
    sprintf(nr_text, "%s %s", tmp_copy_1, tmp_copy_2);
    return nr_text;
  }
  strcpy_P(nr_text, numbers[nr]);
  return nr_text;
}

char time_result[48]; // longest: "twenty three minutes past twelve" = 32 + "the time is " = 12

// Convert time (h, m) to text version.
char * the_time(int h, int m) {
  if (h>12) { h-=12; }
  int nexth = (h<12)?(h+1):1;
  if (m == 0) {
    sprintf(time_result, "the time is %s o'clock ", number2word(h));
    return time_result;
  }
  if (m == 15) {
    sprintf(time_result, "the time is a quarter past %s ", number2word(h));
    return time_result;
  }
  if (m == 30) {
    sprintf(time_result, "the time is half past %s ", number2word(h));
    return time_result;
  }
  if (m == 45) {
    sprintf(time_result, "the time is a quarter to %s ", number2word(nexth));
    return time_result;
  }
  if (m < 30) {
    char minutes_txt[15];
    strcpy(minutes_txt, number2word(m));
    sprintf(time_result, "the time is %s %s past %s ", minutes_txt, getMinuteSingleOrPlural(m), number2word(h));
    return time_result;
  }
  char minutes_txt[15];
  strcpy(minutes_txt, number2word(60-m));
  sprintf(time_result, "the time is %s %s to %s ", minutes_txt, getMinuteSingleOrPlural(60-m), number2word(nexth));
  return time_result;
}

char* getMinuteSingleOrPlural(int minute) {
  if (minute==1) {
    return "minute";
  }
  return "minutes";
}

// The main effect routine.
void wordclock() {
  // Fade in the eye and the "shields"/fillers.
  for (int percentage = 0; percentage <=100;percentage+=10) {
    strip.ClearTo(RgbColor(0,0,0));
    fillFillers(FILLER, percentage);
    fillEye(percentage);
    strip.Show();
    handlingDelay(120);
  }

  // Display 3 texts. The time twice, and as third a HAL9001 quote.
  int r=3;
  while(r--) {

    // Determine what text to display.
    char text[MAX_TEXT_LENGTH];
    if (r>0) {
      // Get us the current time.
      struct tm * digiTimeInfo;
      time_t digiTime = time(nullptr); // time_t = seconds since epoch
      digiTimeInfo = localtime (&digiTime); // setup digiTimeInfo -> tm_hour, digiTimeInfo -> tm_min, digiTimeInfo -> tm_sec
      strcpy(text, the_time(digiTimeInfo -> tm_hour, digiTimeInfo -> tm_min));
    } else {
      // Find a random quote.
      int q = random(HAL9001_QUOTE_COUNT);
      strcpy_P(text, hal9001_quotes[q]);
    }
    int txtcols = strlen(text)*4;

    // Scroll the text (and keep fading the eye, and keep the fillers in place).
    for (int colNr=9;colNr>=-txtcols;colNr--) {
      strip.ClearTo(RgbColor(0,0,0));
      fillEye(100);
      fillFillers(FILLER, 100);
      drawText(text, colNr, true, TXT1, TXT2);
      drawText(text, colNr, false, TXT1, TXT2);
      strip.Show();
      handlingDelay(75);
      fillEye(100);
      strip.Show();
      handlingDelay(75);
    }
  }

  // Fade out the eye and the "shields"/fillers.
  for (int percentage = 100; percentage >=0;percentage-=10) {
    strip.ClearTo(RgbColor(0,0,0));
    fillFillers(FILLER, percentage);
    fillEye(percentage);
    strip.Show();
    handlingDelay(120);
  }

}
