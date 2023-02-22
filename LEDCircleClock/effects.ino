
#define NR_OF_EFFECTS 18

const char* effectNames[] {
  "Sparkle", "PacMan", "Scan", "Fire", "Snake", "Maze", "DigiClock", "Bomb",
  "TicTacToe", "Drain", "Vault", "HourGlass", "Snail", "WordClock", "MovieStart",
  "WarpTunnel", "Throbber", "MeteorShower", 
};

int getNrOfEffects() {
  return NR_OF_EFFECTS;
}

#define EFFECT_BACKOFF_COUNT 4

// Remember last shown effects, the randmizer will not show one of these next.
int lastEffects[EFFECT_BACKOFF_COUNT];

void executeRandomEffect() {
  int effectChoice = lastEffects[0];
  while (isInLastEffects(effectChoice)) {
    effectChoice = random(NR_OF_EFFECTS);  
  }
  executeEffect(effectChoice);
}

boolean isInLastEffects(int nr) {
  for (int i=0; i<EFFECT_BACKOFF_COUNT; i++) {
    if (lastEffects[i] == nr) {
      return true;
    }
  }
  return false;
}

void registerLastEffect(int nr) {
  // Move all one field UP.
  for (int i=(EFFECT_BACKOFF_COUNT-1); i>0; i--) {
    lastEffects[i] = lastEffects[i-1];
  }
  lastEffects[0] = nr;
}

void executeEffect(int choice) {
  registerLastEffect(choice);
  
  switch (choice) {
    case 0: sparkle(); break;
    case 1: pacman(); break;
    case 2: scan(); break;
    case 3: fire(); break;
    case 4: snake(); break;
    case 5: maze(); break;
    case 6: digiclock(); break;
    case 7: bomb(); break;
    case 8: ticTacToe(); break;
    case 9: drain(); break;
    case 10: vault(); break;
    case 11: hourglass(); break;
    case 12: snail(); break;
    case 13: wordclock(); break;
    case 14: moviestart(); break;
    case 15: warpTunnel(); break;
    case 16: throbber(); break;
    case 17: meteorShower(); break;
    case 100: colorPicker(); break; // 100 = hidden developer option (not shown as effect), use/trigger using http://ip-address/color
    default: break;
  }
}
