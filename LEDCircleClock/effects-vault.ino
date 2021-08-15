
// The Vault: open the code combination lock, and take the gold...
//
// The (inside workings) animation is actually really opening a combination lock.
// A code knob is attached to the inner disc, and that one can rotate clockwise or counter clockwise.
// There are randomly placed notches on each code disc, which determine
// when an inner disc locks to it's next outer disc for the current rotation direction.
// It starts with rotating clockwise until all discs engage, and then it starts
// solving the code combination, and in the end we take the gold.
//
// Thijs Kaper (15/8/2021).

#define VAULT_BACKGROUND_COLOR RgbColor(0, 0, 0)
#define VAULT_GOLD_COLOR RgbColor(10, 10, 0)
#define VAULT_DISC_COLOR RgbColor(0, 6, 14)
#define VAULT_LEFT_GAP_COLOR RgbColor(20, 0, 0)
#define VAULT_RIGHT_GAP_COLOR RgbColor(0, 20, 0)
#define VAULT_NOTCH_COLOR RgbColor(20, 0, 20)

#define membersof(x) (sizeof(x) / sizeof(x[0]))

// This dataset indicates what movements need to be done to animate the "gaps" to open.
const uint8_t vaultOpenAnimationData[][4] {
  // dest, source, count, dest-ring
  { 193, 142, 7, 8 },
  { 142, 100, 7, 7 },
  { 101, 67, 5, 6 },
  { 67, 41, 5, 5 },
  { 42, 24, 3, 4 },
  { 24, 11, 3, 3 }
};

// Main routine, show a locked vault, solve the code combination, and empty the vault.
void vault() {
  strip.ClearTo(VAULT_BACKGROUND_COLOR);

  // Draw some "gold" in the safe.
  for(int i=1;i<9;i++) {
    strip.SetPixelColor(i, VAULT_GOLD_COLOR);
  }

  // Determine start rotation of all discs
  int rotate[3];
  rotate[0] = random(16);
  rotate[1] = random(16);
  rotate[2] = random(16);

  // Determine where the "code" notches will be on the discs.
  // The code notches indicate the vault combination to use to open it.
  int notch[3];
  notch[0] = vaultGetRandomNotchPos(-1);
  notch[1] = vaultGetRandomNotchPos(notch[0]);
  notch[2] = vaultGetRandomNotchPos(notch[1]);

  // Show the discs
  for(int i=0;i<3;i++) {
    vaultDrawDisc(i, rotate[i], notch[i]);
  }
  strip.Show();

  // Wait before starting the safe cracking.
  handlingDelay(1200);

  // The lockdir is a helper variable, which determines if two rings can lock together using their notch.
  // Locking depends on rotation direction.
  int lockdir[2];
  lockdir[0] = 0;
  lockdir[1] = 0;

  // We start turning clockwise.
  int dir = 1;
  
  // Start spinning 3 rounds in one direction to get all discs to engage in the notch locking.
  for(int i=0;i<3*16;i++) {
    vaultTurnKnob(dir, rotate, notch, lockdir);
    handlingDelay(100);
  }

  // Solve discs from outside to inside.
  for(int solveDisc=2;solveDisc>=0;solveDisc--) {
    while (rotate[solveDisc] != 0) {
      vaultTurnKnob(dir, rotate, notch, lockdir);
      handlingDelay(250);
    }
    // Reverse rotation direction between each solved disc.
    dir = -dir;
    handlingDelay(500);
  }

  // Now for the prize... Open the vault, and get the gold.
  vaultOpenVault();
  vaultTakeTheGold();

  handlingDelay(2000);
}

// Animate, move gaps to the outside.
void vaultOpenVault() {
  // 6 frames/steps.
  for(int steps=0;steps<6;steps++) {
    // loop through the dataset which has the data on what to move where.
    for(int i=0;i<membersof(vaultOpenAnimationData);i++) {
      int srcPos = vaultOpenAnimationData[i][1];
      int destPos = vaultOpenAnimationData[i][0];
      int srcOffset = ringSizes[vaultOpenAnimationData[i][3]-1] / 2;
      int destOffset = ringSizes[vaultOpenAnimationData[i][3]] / 2;
        
      // move all pixels as noted in data set, do this for both set of gaps (left and right).
      for(int j=0;j<vaultOpenAnimationData[i][2];j++) {
        // copy source to dest pixel, for right gap
        strip.SetPixelColor(destPos + j, strip.GetPixelColor(srcPos + j));
        // copy source to dest pixel, for left gap
        strip.SetPixelColor(destPos + j + destOffset, strip.GetPixelColor(srcPos + j + srcOffset));
      }
    }
    strip.Show();
    handlingDelay(150);
  }
}

// Animate taking the gold. Rotate gold counter clockwise, and shift it out horizontally via the opened gaps.
void vaultTakeTheGold() {
  for(int i=0;i<10;i++) {
    // Shift out gold horizontally.
    vaultShifGoldOutHorizontally();
    strip.Show();
    handlingDelay(150);
    vaultShifGoldOutHorizontally();
    strip.Show();
    handlingDelay(150);

    // Rotate center gold, counter clockwise.
    RgbColor color = strip.GetPixelColor(1);
    for(int j=1;j<8;j++) {
      strip.SetPixelColor(j, strip.GetPixelColor(j+1));
    }
    strip.SetPixelColor(8, color);
    strip.Show();
  }
}

// Move out the gold horzontally from center to left and right.
void vaultShifGoldOutHorizontally() {
  for(int ring=8;ring>0;ring--) {
    // determine destination as quarter into ring (e.g. 90 degrees).
    int destPos = (ringSizes[ring]/4) + startLEDs[ring];
    int srcPos = (ringSizes[ring-1]/4) + startLEDs[ring-1];
    // copy source to dest pixel, for right half
    strip.SetPixelColor(destPos, strip.GetPixelColor(srcPos));
    // determine the next (left) half at 270 degrees (or just add half a ring size).
    destPos += ringSizes[ring]/2;
    srcPos += ringSizes[ring-1]/2;
    // copy source to dest pixel, for left half
    strip.SetPixelColor(destPos, strip.GetPixelColor(srcPos));
  }
}

// This routine will turn the inner ring in the given direction for 1 spot.
// If a notch is lining up with a notch of a ring outside, the rings will be locked and rotate together,
// until direction of rotation is reversed.
// dir: left = -1, right = 1.
// rotate: the state of the discs.
// notch: the spots on the discs where they lock together, an inner disc locks with the one outside it.
// lockdir: direction in which the notch lock should work. When reversing disc rotation, the movement unlocks.
void vaultTurnKnob(int dir, int rotate[], int notch[], int lockdir[]) {
  int notch0 = vaultGetEffectiveNotchPos(notch[0], rotate[0]);
  int notch1 = vaultGetEffectiveNotchPos(notch[1], rotate[1]);
  int notch2 = vaultGetEffectiveNotchPos(notch[2], rotate[2]);

  // Determine if the middle and/or outer disc should also rotate.
  boolean rotate1 = (lockdir[0] == dir) && (notch0 == notch1);
  boolean rotate2 = (lockdir[1] == dir) && rotate1 && (notch1 == notch2);

  // We always rotate the inner disc.
  vaultRotateDisc(0, dir, rotate, notch);
  lockdir[0] = dir;

  if(rotate1) {
    vaultRotateDisc(1, dir, rotate, notch);
    lockdir[1] = dir;
  }

  if(rotate2) {
    vaultRotateDisc(2, dir, rotate, notch);
  }

  strip.Show();
}

// We store the notch position for a solved/base-position of a disc.
// This routine calculates where the notch is in rotated state.
int vaultGetEffectiveNotchPos(int notch, int rotation) {
  notch = notch + rotation;
  if (notch >= 16) {
    notch -= 16;
  }
  return notch;
}

// Rotate a single disc, and draw it on screen.
void vaultRotateDisc(int discNr, int dir, int rotate[], int notch[]) {
  rotate[discNr] = rotate[discNr] + dir;
  if (rotate[discNr] < 0) {
    rotate[discNr] = 15;
  } else if (rotate[discNr] > 15) {
    rotate[discNr] = 0;
  }
  vaultDrawDisc(discNr, rotate[discNr], notch[discNr]);
}

// Generate a random notch position, not in the left and right gaps,
// and not the same as previous disc nothc position.
int vaultGetRandomNotchPos(int previousDiscNotch) {
  int pos;
  do {
    pos = random(16);
  } while((pos >=3 && pos <= 5) || (pos >=11 && pos <=13) || (pos == previousDiscNotch));
  return pos;
}

// Draw a disc, with given rotation, and notch spot.
// discNr: inner = 0, middle = 1, outer = 2.
// rotation: 0..15 for disc rotation (middle and outer disc use same # of steps, just using bigger steps).
// notchPos: 0..15 indicating where the "code" notch is (all discs same # of spots, just different step size).
void vaultDrawDisc(int discNr, int rotation, int notchPos) {
  rotation = rotation * (discNr+1);
  notchPos = notchPos * (discNr+1);
  
  int ring = (discNr*2)+3;
  int ringSize = ringSizes[ring];

  int gapStart1 = 3 * (discNr+1);
  int gapStart2 = 11 * (discNr+1);
  int gapSize = 3 + (discNr*2);
  
  for(int i=0;i<ringSize;i++) {
    RgbColor color;
    if (i == notchPos) {
      color = VAULT_NOTCH_COLOR;
    } else if (i >= gapStart1 && i < (gapStart1 + gapSize)) {
      color = VAULT_RIGHT_GAP_COLOR;
    } else if (i >= gapStart2 && i < (gapStart2 + gapSize)) {
      color = VAULT_LEFT_GAP_COLOR;
    } else {
      color = VAULT_DISC_COLOR;
    }
    int ledPos = vaultGetRotatedRingLedPos(ring, i, rotation);
    strip.SetPixelColor(ledPos, color);
  }
}

// Translate ring number, ring position, and rotation into the proper LED position.
// ring: 0..8 (3 = inner disc, 5 = middle disc, 7 = outer disc)
// ringPos: 0..(ringSize-1)
// rotation: 0..any value
int vaultGetRotatedRingLedPos(int ring, int ringPos, int rotation) {
  int newPos = ringPos + rotation;
  while (newPos >= ringSizes[ring]) {
    newPos-=ringSizes[ring];
  }
  return startLEDs[ring] + newPos;
}
