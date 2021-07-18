
#define NR_OF_EFFECTS 7

int getNrOfEffects() {
  return NR_OF_EFFECTS;
}

// Remember last shown effect, the randmizer will not show the same one twice this way.
int lastEffect = -1;

void executeRandomEffect() {
  int effectChoice;
  while ((effectChoice = random(NR_OF_EFFECTS)) == lastEffect) { }
  executeEffect(effectChoice);
}

void executeEffect(int choice) {
  lastEffect = choice;
  switch (choice) {
    case 0: sparkle(); break;
    case 1: pacman(); break;
    case 2: scan(); break;
    case 3: fire(); break;
    case 4: snake(); break;
    case 5: maze(); break;
    case 6: digiclock(); break;
    default: break;
  }
}
