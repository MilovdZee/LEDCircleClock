
// Meteor Shower - Note: they also look quite like guppies (fish)
// First half of the animation time, the colors are "meteor-like", the second half just random fading colors.
//
// Thijs Kaper (22/12/2022)

const PROGMEM uint8_t meteor_path_1[] = { 185, 186, 187, 188, 189, 190, 191, 192, 255};
const PROGMEM uint8_t meteor_path_2[] = { 181, 134, 95, 96, 97, 98, 99, 100, 101, 144, 196, 255};
const PROGMEM uint8_t meteor_path_3[] = { 238, 179, 132, 61, 38, 39, 40, 41, 42, 69, 104, 147, 199, 255 };
const PROGMEM uint8_t meteor_path_4[] = { 235, 176, 129, 90, 59, 36, 9, 10, 11, 12, 26, 45, 72, 107, 150, 202, 255 };
const PROGMEM uint8_t meteor_path_5[] = { 232, 174, 127, 88, 57, 34, 18, 17, 16, 15, 28, 47, 74, 109, 152, 205, 255 };
const PROGMEM uint8_t meteor_path_6[] = { 229, 171, 124, 85, 54, 53, 52, 51, 50, 77, 112, 155, 208, 255 };
const PROGMEM uint8_t meteor_path_7[] = { 226, 168, 121, 120, 119, 118, 117, 116, 115, 158, 211, 255 };
const PROGMEM uint8_t meteor_path_8[] = { 222, 221, 220, 219, 218, 217, 216, 215, 255 };

const PROGMEM uint8_t* meteor_paths[] = { meteor_path_1, meteor_path_2, meteor_path_3, meteor_path_4, meteor_path_5, meteor_path_6, meteor_path_7, meteor_path_8 };

RgbColor meteor_fire_colors[] = { RgbColor(50, 0, 0), RgbColor(8, 4, 0), RgbColor(2, 2, 2), RgbColor(0, 0, 0) };

void meteorShower() {
  uint8_t meteor_index[] = { 255, 255, 255, 255, 255, 255, 255, 255 };
  RgbColor meteor_color[8];

  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();

  for (int j=0; j<300;j++) {
    if (j<(300-20)) {
      scheduleMeteor(meteor_index, meteor_color);
    }

    for (int i=0; i<8;i++) {
      uint8_t index = meteor_index[i];
      if (index == 255) {
        continue;
      }
      meteor_index[i] = drawMeteor(meteor_paths[i], index, meteor_color[i], (j>150));
    }
    strip.Show();
    handlingDelay(40);
  }
}

// return new index value (e.g. index+1, or 255 on end)
uint8_t drawMeteor(const uint8_t* meteor_path, uint8_t index, RgbColor color, boolean useRandomColors) {
  int maxIndex = 0;
  while (pgm_read_byte_near(meteor_path + maxIndex)!=255) {
    maxIndex++;
  }
  int percentage[] = { 0, 50, 90, 100 };
  for (int i=0;i<4;i++) {
    uint8_t dot = (index>=i && (index-i)<maxIndex) ? pgm_read_byte_near(meteor_path + index - i) : 255;
    if (useRandomColors) {
      setPixel(dot, fadedColor(color, RgbColor(0, 0, 0), percentage[i]));
    } else {
      setPixel(dot, meteor_fire_colors[i]);
    }
  }
  return ((index-4)>maxIndex) ? 255 : (index+1);
}

void scheduleMeteor(uint8_t meteor_index[], RgbColor meteor_color[]) {
  int inUseCount = 0;
  for (int i=0; i<8;i++) {
    if (meteor_index[i] != 255) {
      inUseCount++;
    }
  }
  for (int i=0; i<8;i++) {
    if (meteor_index[i] == 255 && random(8)==0 && inUseCount<3) {
      inUseCount++;
      meteor_color[i] = getRandomColor();
      meteor_index[i] = 0;
    }
  }
}
