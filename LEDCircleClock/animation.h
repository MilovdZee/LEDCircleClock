
// "command" list, upper 3 bits are command, lower 5 bits are data - by combining the two in one byte, we save some space in the animations.

#define SET_PIXELS (uint8_t) 0b00000000 // lower 5 bits are for color, next byte = length, next bytes is pixel-nrs.
#define SET_RANGE  (uint8_t) 0b00100000 // lower 5 bits are for color, next 2 bytes indicate start and end.
#define CLEAR_ALL  (uint8_t) 0b01000000 // lower 5 bits are for color, no following data. Wipes all pixels.
#define WAIT       (uint8_t) 0b01100000 // lower 5 bits indicate which delay to use, no following data.
#define END        (uint8_t) 0b11100000 // lower 5 bits ignored, no following data.

#define ANIMATION_COMMAND_BIT_MASK (uint8_t) 0b11100000
#define ANIMATION_DATA_BIT_MASK (uint8_t) 0b00011111
