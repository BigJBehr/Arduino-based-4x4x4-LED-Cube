// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17
//******************************************************************************
//******************************************************************************

// Action class implementation
#include <avr/pgmspace.h>
#include "ActionRoll.h"        // ActionRoll class definition

//***** Flash Constants - Maps *****
const byte RollFwd[] PROGMEM = { 1,  2,  3, 28, 27,  4,  5,  6,  9, 10, 11, 20, 19, 12, 13, 14,
                                15, 18, 45, 44, 43, 42, 21,  8,  7, 26, 37, 36, 35, 34, 29,  0,
                                31, 30, 33, 60, 59, 38, 25, 24, 23, 22, 41, 52, 51, 46, 17, 16,
                                47, 48, 49, 50, 53, 54, 55, 40, 39, 56, 57, 58, 61, 62, 63, 32};

const byte RollRev[] PROGMEM = {31,  0,  1,  2,  5,  6,  7, 24, 23,  8,  9, 10, 13, 14, 15, 16,
                                47, 46, 17, 12, 11, 22, 41, 40, 39, 38, 25,  4,  3, 30, 33, 32,
                                63, 34, 29, 28, 27, 26, 37, 56, 55, 42, 21, 20, 19, 18, 45, 48,
                                49, 50, 51, 44, 43, 52, 53, 54, 57, 58, 59, 36, 35, 60, 61, 62};
                        
// ***** Sprite initializers *****
// Roll - uses four sprites of length one
const Sprite_t SpRoll[] PROGMEM = {{{.rgb = {32, 0, 0}}, 1, 1, 15, FWD, 50},
                                   {{.rgb = {32, 0, 0}}, 1, 1,  8, FWD, 50},
                                   {{.rgb = {32, 0, 0}}, 1, 1,  7, FWD, 50},
                                   {{.rgb = {32, 0, 0}}, 1, 1,  0, FWD, 50},
                                   {{.rgb = {32, 0, 0}}, 1, 1, 17, FWD, 50},
                                   {{.rgb = {32, 0, 0}}, 1, 1, 22, FWD, 50},
                                   {{.rgb = {32, 0, 0}}, 1, 1, 25, FWD, 50},
                                   {{.rgb = {32, 0, 0}}, 1, 1, 30, FWD, 50}};
                                
#define NUMREVOLUTIONS  4

//******************************************************************************
// Start the action
//******************************************************************************
void ActionRoll::start(Sprite_t *ps)
{
  pAllSprites = pSprites = ps;
  memcpy_P(pSprites, SpRoll, sizeof(SpRoll));
  numSprites = 4;
  mode = MODE_OUTTER;
  count = NUMREVOLUTIONS;

  // set all sprites to the current brightness
  for(int i = 0; i < numSprites; i++)
  {
    changeBrightness(pSprites + i);
  } //  for
} //  start

//******************************************************************************
// Move the sprite within the cube. Follow the map 
//******************************************************************************
void ActionRoll::moveSprite(void)
{
  Sprite_t *ps;
  bool     flag = false;    // forces changes to be made outside of the for loop.
  
  for(int i = 0; i < numSprites; i++)
  {
    ps = pSprites + i;
    changeBrightness(ps);
  
    // move each sprite, follow the map
    switch(ps->dir & ~FLG_MASK)
    {
      case FWD:
        ps->pos = pgm_read_byte(RollFwd + ps->pos);
        break;
      case REV:
        ps->pos = pgm_read_byte(RollRev + ps->pos);
        break;
    } //  switch direction
    
    if ( 0 == ps->pos ||  7 == ps->pos ||  8 == ps->pos || 15 == ps->pos ||
        30 == ps->pos || 25 == ps->pos || 22 == ps->pos || 17 == ps->pos)
    {
      // finished one revolution
      if (changeColor(ps))
      {
        ps->dir ^= REV;         // change direction

        // to detect end of an outer sequence
        if (0 == ps->pos)
        {
          // finished one revolution
          flag = true;
        }
        
        // to detect end of an inner sequence
        if (4 == numSprites && 30 == ps->pos)
        {
          // finished one revolution
          flag = true;
        }
      } // color changed
    } // end of one revolution
  }  // for each sprite

  if (flag)
    endOfRevolution();
} //  moveSprite


