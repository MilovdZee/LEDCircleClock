
// Warp Tunnel Effect.
//
// Thijs Kaper (1/12/2022)

#define WARP_TUNNEL_BACKGROUND RgbColor(0, 0, 0)
#define WARP_TUNNEL_RING_EDGE RgbColor(0, 0, 8)

void drawCircle(int ring, RgbColor color) {
  for (int r=0;r<ringSizes[ring];r++) {
    setPixel(startLEDs[ring]+r, color);
  }
}

RgbColor warpColors[] = { 
  WARP_TUNNEL_BACKGROUND,WARP_TUNNEL_BACKGROUND,WARP_TUNNEL_BACKGROUND,
  WARP_TUNNEL_BACKGROUND,WARP_TUNNEL_BACKGROUND,WARP_TUNNEL_BACKGROUND,
  WARP_TUNNEL_BACKGROUND,WARP_TUNNEL_BACKGROUND,WARP_TUNNEL_BACKGROUND,
};

RgbColor lastWarpColor = WARP_TUNNEL_RING_EDGE;

void drawAndShiftWarpTunnelRings() {
  for (int i=RINGS-1; i>=0; i--) {
      if (i == RINGS-1 && (warpColors[i].R != 0 || warpColors[i].G != 0 || warpColors[i].B != 0)) {
        lastWarpColor = warpColors[i];
      }
      if (i == RINGS-1 && warpColors[i].R == 0 && warpColors[i].G == 0 && warpColors[i].B == 0) {
        drawCircle(i, lastWarpColor);
      } else {
        drawCircle(i, warpColors[i]);
      }
      if (i>0) {
        warpColors[i] = warpColors[i-1];
      } else {
        warpColors[i] = WARP_TUNNEL_BACKGROUND;
      }
  }
  strip.Show();
}

void warpOuterRingFade(int percentage) {
  drawCircle(RINGS-1, fadedColor(RgbColor(0, 0, 0), lastWarpColor, percentage));
  strip.Show();
  handlingDelay(130);
}

void warpTunnel() {
  int maxSteps = 130;
  int minDelay = 30;
  int maxDelay = maxSteps+minDelay;

  strip.ClearTo(WARP_TUNNEL_BACKGROUND);
  strip.Show();
  
  for (int i=0;i<=100;i+=10) {
    warpOuterRingFade(i);
  }

  for (int i=0;i<maxSteps;i++) {
    if (i%4==0 && i<(maxSteps-10)) {
      warpColors[0] = getRandomColor();
    }
    drawAndShiftWarpTunnelRings();
    if (i<=(maxSteps/2)) {
      handlingDelay(maxDelay - (i*2));
    } else {
      handlingDelay(maxDelay - ((maxSteps-i)*2));
    }
  }

  for (int i=100;i>=0;i-=10) {
    warpOuterRingFade(i);
  }
}
