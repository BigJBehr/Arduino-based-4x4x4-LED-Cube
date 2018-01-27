// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17
//******************************************************************************
#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include <Arduino.h>

// Each APA106 LED consists of three LEDs, red, green & blue.
// The intensity of each color is set with the eight bit valued stored
// in the color.
typedef union {
  uint32_t  color;
  struct {
    byte  red;
    byte  green;
    byte  blue;
  } rgb;
} Apa106_t;

typedef struct {
  Apa106_t  led;      // all LEDs in the sprite are the same color
  byte      hue;      // color index of the sprite
  byte      len;      // number of LEDs in the sprite
  int8_t    pos;      // current position
  byte      dir;      // direction, FWD, REV, UP, DOWN
  int       rate;     // ticks between moves
} Sprite_t;

typedef struct {
  byte    numSprites;
  Sprite_t  *sprites;
} Display_t;

// Directions
enum {
  FWD,      // forward through sequence
  REV,      // reverse through sequence
  UP,       // bottom to top
  DOWN,     // top to bottom
  LEFT,     // left to right
  RIGHT,    // right to left
};

// Modifier flags
#define FLG_REVERSE     0x80    // reverse direction when at end
#define FLG_LEVEL       0x40    // stay on level
#define FLG_MASK        (FLG_REVERSE | FLG_LEVEL)

#define MAXSPEITES    9     // No action uses more than 9 sprites

#define NUMTIMES      6     // Number of times to repeat one display

#define NUMLEDS      64     // number of APA106 LEDs in the cube
extern Apa106_t Cube[NUMLEDS];

#endif  //  CUBEOBJECT_H

