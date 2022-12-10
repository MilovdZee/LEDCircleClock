// General purpose animation routine.
//
// This routine accepts 3 arrays, and an optional fade percentage.
// The animation array consists of commands and data for rendering/executing the animation.
// The other two arrays deterime the colors and delays to use.
// 
// The showAnimation routine is in use by the snail and hourglass animations (and maybe later some more).
//
// Implemented by: Thijs Kaper (30/4/2022).

#include "animation.h"

// The animation works by going through the animation array.
//
// The first byte read, is a "command". It will indicate if we want to clear the screen, draw a range of dots,
// or draw a bunch of individual dots, wait a while, or end the animation.
//
// Depending on the command, zero or more bytes will follow, as needed for the command:
//
// - SET_PIXELS, has a next byte indicating the number of led-positions which will follow afterwards.
// - SET_RANGE, has two bytes followin it; the start-led, and end-led. All leds from start to end will be drawn.
// - CLEAR_ALL, has no extra bytes following it. It will clear ALL dots to the specified color.
// - WAIT, has no extra bytes following it. It will DISPLAY the previous drawing actions, and wait for a given time.
// - END, has no extra bytes following it. It indicates the end of the animation.
//
// The command byte is using the three upper bits of a byte only for the command. The lower 5 bits in that same byte
// are used as an index into the color array, or into the delays array to use the data from there for the command.
// See also include file animation.h for the command defines / bits.
//
// See effects-snail.ino and effects-hourglass.ino for animation usage examples.
// Note: to draw/design the animations, I first created a web based "drawing tool / program" with which you can
// make drawings/animations. The data from that tool can be exported as the big animation array.
// The drawing tool will try to determine when rendering a full frame, or when rendering only differences is better
// for used storage space. So some frames of the animation start with a clear screen, and some don't (and depend on
// the current screen state). Having these "diff's" makes it impossible to render the animation backwards. You can
// only run from start to end.

// The full animation is shown ONCE using this routine (at fade = 100%).
void showAnimation(const uint8_t the_animation[], const RgbColor the_colors[], const int the_delays[]) {
  showAnimation(the_animation, the_colors, the_delays, 100);
}

// Fade in the first frame, percentage 0..95, with fadeInDelayMs sleep between steps.
void fadeInAnimation(const uint8_t the_animation[], const RgbColor the_colors[], int fadeInDelayMs) {
  for (int p=0;p<=95;p++) {
    showAnimation(the_animation, the_colors, 0, p);
    handlingDelay(fadeInDelayMs);
  }
}

// The full animation is shown ONCE using this routine. This version has a fadePercent parameter to fade in the colors.
// If a FADE percentage below 100 (0..99) is used, we ONLY show the FIRST FRAME.
void showAnimation(const  uint8_t the_animation[], const RgbColor the_colors[], const int the_delays[], int fadePercent) {
  // pointer in the animation data array
  const uint8_t *ptr = the_animation;

  boolean keepRunning = true;
  while(keepRunning) {
    // Determine command and data from the current byte.
    uint8_t animation_command = pgm_read_byte_near(ptr++);
    uint8_t animation_data = animation_command & ANIMATION_DATA_BIT_MASK;
    animation_command = animation_command & ANIMATION_COMMAND_BIT_MASK;

    // Handle the command.
    switch(animation_command) {
      case SET_PIXELS: {
        // animation_data is index in color map, next byte = length, next bytes is pixel-nrs.
        uint8_t numberOfPixels = pgm_read_byte_near(ptr++);
        RgbColor color = animationColor(animation_data, the_colors, fadePercent);
        for (int p=0;p<numberOfPixels;p++) {
          uint8_t ledNr = pgm_read_byte_near(ptr++);
          strip.SetPixelColor(ledNr, color);
          // Note/Reminder: SetPixelColor will not actually show anything. You need to execute a "WAIT"
          // command to render the end-result.
        }
        break;
      }
      case SET_RANGE: {
        // animation_data is index in color map, next 2 bytes indicate start and end.
        uint8_t fromPixel = pgm_read_byte_near(ptr++);
        uint8_t toPixel = pgm_read_byte_near(ptr++);
        RgbColor color = animationColor(animation_data, the_colors, fadePercent);
        for (int ledNr=fromPixel;ledNr<=toPixel;ledNr++) {
          strip.SetPixelColor(ledNr, color);
          // Note/Reminder: SetPixelColor will not actually show anything. You need to execute a "WAIT"
          // command to render the end-result.
        }
        break;
      }
      case CLEAR_ALL: {
        // animation_data is index in color map, no following data. Wipes all pixels.
        RgbColor color = animationColor(animation_data, the_colors, fadePercent);
        strip.ClearTo(color);
        // Note/Reminder: ClearTo will not actually show (wipe) anything. You need to execute a "WAIT"
        // command to render the end-result.
        break;
      }
      case WAIT: {
        // animation_data is index in delay map, no following data.
        // Render/Show the result of any changes done using the other commands.
        strip.Show();
        if (fadePercent<100) {
          // If we are in fade mode (0..99%), we only want the first frame to be shown.
          keepRunning = false;
        } else {
          // And wait a bit before going to the next frame(s).
          handlingDelay(the_delays[animation_data]);
        }
        break;
      }
      case END: {
        // End of animation, exit routine.
        keepRunning = false;
        break;
      }
      default:
        // ignore? not possible for now.
        break;
    }
  }
}

RgbColor animationColor(uint8_t index, const RgbColor the_colors[], int fadePercent) {
  if (fadePercent>=100) {
    return the_colors[index];
  }
  return fadedColor(RgbColor(0,0,0), the_colors[index], fadePercent);
}
