
// Throbber.
//
// Thijs Kaper (15/5/2022).

#define THROBBER_BACKGROUND RgbColor(0, 0, 0)

void throbber() {
  strip.ClearTo(THROBBER_BACKGROUND);
  strip.Show();

  int numSnakes = 4;
  int snakeLen[numSnakes];
  int snakePos[numSnakes];
  boolean snakeDir[numSnakes];
  RgbColor snakeColor[numSnakes];

  for (int s=0;s<numSnakes;s++) {
    snakeLen[s]=random(6)+5-s;
    snakePos[s]=random(60);
    snakeDir[s]=(random(2)==0);
    snakeColor[s]=getRandomColor();
  }
  
  for(int i=0;i<=300;i++) {
    for (int s=0;s<numSnakes;s++) {
      throbberSnake(RINGS-1-(s*2), snakeLen[s], snakePos[s], snakeDir[s], snakeColor[s]);
      if (s==0 || (i%s)==0) {
        snakePos[s]+=(snakeDir[s]?1:-1);
      }
    }
    strip.Show();
    handlingDelay(20);
  }
}

RgbColor getRandomColor() {
  int r=random(25);
  int g=random(25);
  int b=random(25);
  int x=random(3);
  if(x==0) r=0;
  if(x==1) g=0;
  if(x==2) b=0;
  return RgbColor(r,g,b);
}


void throbberSnake(int ring, int length, int start, boolean direction, RgbColor color) {
  int ringSize = ringSizes[ring];
  int ringStartLed = startLEDs[ring];
  int ringEndLed = startLEDs[ring+1]-1;
  int pos = ringStartLed + (start % ringSize);
  
  for (int i=0;i<length;i++) {
    if (pos > ringEndLed) { pos-=ringSize; }
    if (pos < ringStartLed) { pos+=ringSize; }
    int percentage = 100-round(100*i/length);
    if (percentage<10) percentage = 0;
    setPixel(pos, fadedColor(color, THROBBER_BACKGROUND, percentage));
    if (direction) { pos++; } else { pos--; }
  }
  for (int i=length;i<ringSize;i++) {
    if (pos > ringEndLed) { pos-=ringSize; }
    if (pos < ringStartLed) { pos+=ringSize; }
    setPixel(pos, THROBBER_BACKGROUND);
    if (direction) { pos++; } else { pos--; }
  }
}
