// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17
//******************************************************************************
//******************************************************************************

// ActionPitch class implementation
#include <avr/pgmspace.h>
#include "ActionPitch.h"        // ActionPitch class definition

//***** Flash Constants - Maps *****
const byte PitchFwd[] PROGMEM = {31, 30, 29, 28,  3,  2,  1,  0,  7,  6,  5,  4, 11, 10,  9,  8,
                                 15, 14, 13, 12, 27, 26, 25, 24, 39, 38, 37, 36, 35, 34, 33, 32,
                                 63, 62, 61, 60, 43, 42, 41, 40, 23, 22, 21, 20, 19, 18, 17, 16,
                                 47, 46, 45, 44, 51, 50, 49, 48, 55, 54, 53, 52, 59, 58, 57, 56};

const byte PitchRev[] PROGMEM = { 7,  6,  5,  4, 11, 10,  9,  8, 15, 14, 13, 12, 19, 18, 17, 16,
                                 47, 46, 45, 44, 43, 42, 41, 40, 23, 22, 21, 20,  3,  2,  1,  0,
                                 31, 30, 29, 28, 27, 26, 25, 24, 39, 38, 37, 36, 51, 50, 49, 48,
                                 55, 54, 53, 52, 59, 58, 57, 56, 63, 62, 61, 60, 35, 34, 33, 32};
                        
                        
// ***** Sprite initializers *****
// Pitch - uses four sprites of length one
const Sprite_t SpPitch[] PROGMEM = {{{.rgb = {32, 0, 0}}, 1, 1,  3, FWD, 50},
                                    {{.rgb = {32, 0, 0}}, 1, 1,  2, FWD, 50},
                                    {{.rgb = {32, 0, 0}}, 1, 1,  1, FWD, 50},
                                    {{.rgb = {32, 0, 0}}, 1, 1,  0, FWD, 50},
                                    {{.rgb = {32, 0, 0}}, 1, 1, 27, FWD, 50},
                                    {{.rgb = {32, 0, 0}}, 1, 1, 26, FWD, 50},
                                    {{.rgb = {32, 0, 0}}, 1, 1, 25, FWD, 50},
                                    {{.rgb = {32, 0, 0}}, 1, 1, 24, FWD, 50}};
                                
#define NUMREVOLUTIONS  4

//******************************************************************************
// Start the action.
//******************************************************************************
void ActionPitch::start(Sprite_t *ps)
{
  pAllSprites = pSprites = ps;
  memcpy_P(pSprites, SpPitch, sizeof(SpPitch));
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
// Move the sprite within the cube. Follow the map. 
//******************************************************************************
void ActionPitch::moveSprite(void)
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
        ps->pos = pgm_read_byte(PitchFwd + ps->pos);
        break;
      case REV:
        ps->pos = pgm_read_byte(PitchRev + ps->pos);
        break;
    } //  switch direction
    
    if ( 0 == ps->pos ||  1 == ps->pos ||  2 == ps->pos ||  3 == ps->pos ||
        24 == ps->pos || 25 == ps->pos || 26 == ps->pos || 27 == ps->pos)
    {
      // finished one revolution
      if (changeColor(ps))
      {
        ps->dir ^= REV;         // change direction

        // to detect end of an outer sequence
        if (3 == ps->pos)
        {
          // finished one revolution
          flag = true;
        }
        
        // to detect end of an inner sequence
        if (4 == numSprites && 24 == ps->pos)
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


