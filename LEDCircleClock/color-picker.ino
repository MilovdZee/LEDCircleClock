
// Color picker tool (hidden developer option, to find proper colors to use in effects).
//
// Shows 8 lines from center to edges.
//
// The center value can be chosen on what r/g/b values to start.
// The edges will be fading to:
// - R
// - G
// - B
// - RG
// - RB
// - GB
// - RGB
// - ??? (some r/g/b value to choose)
// The corners will use a configurable max value for r/g/b.
//
// Trigger this using http://ip-nr/color
//
// Thijs Kaper (23/7/2021)


// Note: these two are defined in the "web.ino" file, which is compiled last, so define them here as "extern" references;
extern const PROGMEM char page_start[];
extern const PROGMEM char page_end[];


int colorPickEdgeMax = 50;

int colorPickCenterR = 0;
int colorPickCenterG = 0;
int colorPickCenterB = 0;

int colorPickChoiceR = 20;
int colorPickChoiceG = 30;
int colorPickChoiceB = 40;

int showColorPickerSeconds = 60;

// main color picker routine.
void colorPicker() {
  showColorPickerSeconds = 60;
  
  while(showColorPickerSeconds-- > 0) {
    strip.ClearTo(RgbColor(0, 0, 0));
  
    RgbColor fromColor = RgbColor(colorPickCenterR, colorPickCenterG, colorPickCenterB);
    colorPickDrawAngleFade(0, 9, fromColor, RgbColor(colorPickEdgeMax, 0, 0)); // R
    colorPickDrawAngleFade(45, 9, fromColor, RgbColor(0, colorPickEdgeMax, 0)); // G
    colorPickDrawAngleFade(90, 9, fromColor, RgbColor(0, 0, colorPickEdgeMax)); // B
    colorPickDrawAngleFade(135, 9, fromColor, RgbColor(colorPickEdgeMax, colorPickEdgeMax, 0)); // RG
    colorPickDrawAngleFade(180, 9, fromColor, RgbColor(colorPickEdgeMax, 0, colorPickEdgeMax)); // RB
    colorPickDrawAngleFade(225, 9, fromColor, RgbColor(0, colorPickEdgeMax, colorPickEdgeMax)); // GB
    colorPickDrawAngleFade(270, 9, fromColor, RgbColor(colorPickEdgeMax, colorPickEdgeMax, colorPickEdgeMax)); // RGB
    colorPickDrawAngleFade(315, 9, fromColor, RgbColor(colorPickChoiceR, colorPickChoiceG, colorPickChoiceB)); // ?
  
    strip.Show();
    
    handlingDelay(1000);
  }
}

// line drawing routine, fading colors.
void colorPickDrawAngleFade(int angle, int rings, RgbColor fromColor, RgbColor toColor) {
  for (int ring = 0; ring < rings; ring++) {
    int ringSize = ringSizes[ring];
    int startLed = startLEDs[ring];
    int led = startLed + int((ringSize * (angle + 5) / 360) + 0.5);
    strip.SetPixelColor(led, fadedColor(fromColor, toColor, (ring * 100 / (rings-1))));
  }
}

// Percentage 0 will return fromColor, percentage 100 will return toColor.
// Anything in between will return between color.
RgbColor fadedColor(RgbColor fromColor, RgbColor toColor, int percentage) {
  if (percentage <= 0) {
    return fromColor;
  }
  if (percentage >= 100) {
    return toColor;
  }
  float factor = percentage/100.0;
  int r = fromColor.R + int(((toColor.R - fromColor.R) * factor) + 0.5);
  int g = fromColor.G + int(((toColor.G - fromColor.G) * factor) + 0.5);
  int b = fromColor.B + int(((toColor.B - fromColor.B) * factor) + 0.5);
  return RgbColor(r,g,b);
}


// HTTP SERVER METHOD
void handleColorPicker() {
  if (server.method() == HTTP_POST) {
    // let main loop know which effect we want to see, but return web response immediately.
    triggerEffect = 100; // color picker has effect nr 100
    showColorPickerSeconds = 60;
  
    if (server.arg("exit") != "") { showColorPickerSeconds = 0; }
    
    if (server.arg("max") != "") { colorPickEdgeMax = server.arg("max").toInt();  }
  
    if (server.arg("cr") != "") { colorPickCenterR = server.arg("cr").toInt();  }
    if (server.arg("cg") != "") { colorPickCenterG = server.arg("cg").toInt();  }
    if (server.arg("cb") != "") { colorPickCenterB = server.arg("cb").toInt();  }
    
    if (server.arg("er") != "") { colorPickChoiceR = server.arg("er").toInt();  }
    if (server.arg("eg") != "") { colorPickChoiceG = server.arg("eg").toInt();  }
    if (server.arg("eb") != "") { colorPickChoiceB = server.arg("eb").toInt();  }
  
    server.sendHeader("Location", showColorPickerSeconds > 0 ? "/color" : "/", true);
    server.send ( 302, "text/plain", "");
  } else {

    server.chunkedResponseModeStart(200, "text/html");
    server.sendContent_P(page_start);
  
    server.sendContent("\
      <style>\nbody { font-size: 1.2em; }\ninput { width: 200px; }\ninput[type=\"submit\"] { position: inherit; }\n</style>\
      <h3>Color Picker</h3>\
      <form method=\"post\">\
        max: <input type=\"text\" name=\"max\" value=\"" + String(colorPickEdgeMax) + "\"><br/><br/>\
        cr: <input type=\"text\" name=\"cr\" value=\"" + String(colorPickCenterR) + "\"> \
        cg: <input type=\"text\" name=\"cg\" value=\"" + String(colorPickCenterG) + "\"> \
        cb: <input type=\"text\" name=\"cb\" value=\"" + String(colorPickCenterB) + "\"><br/><br/>\
        er: <input type=\"text\" name=\"er\" value=\"" + String(colorPickChoiceR) + "\"> \
        eg: <input type=\"text\" name=\"eg\" value=\"" + String(colorPickChoiceG) + "\"> \
        eb: <input type=\"text\" name=\"eb\" value=\"" + String(colorPickChoiceB) + "\"><br/><br/>\
        <input type=\"submit\" value=\"submit\"> <input type=\"submit\" value=\"exit\" name=\"exit\">\
      </form>");

    server.sendContent_P(page_end);
    server.chunkedResponseFinalize();
  }
}
