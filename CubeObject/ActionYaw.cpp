// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17
//******************************************************************************
//******************************************************************************

// Sprite class implementation
#include <avr/pgmspace.h>
#include "ActionYaw.h"        // ActionYaw class definition

//***** Flash Constants - Maps *****
const byte YawFwd[] PROGMEM = {  1,  2,  3,  4, 11, 10,  5,  0,  7,  6,  9, 12, 13, 14, 15,  8,
                                23, 16, 17, 18, 19, 22, 25, 24, 31, 26, 21, 20, 27, 28, 29, 30,
                                33, 34, 35, 36, 43, 42, 37, 32, 39, 38, 41, 44, 45, 46, 47, 40,
                                55, 48, 49, 50, 51, 54, 57, 56, 63, 58, 53, 52, 59, 60, 61, 62};
                                
const byte YawRev[] PROGMEM = {  7,  0,  1,  2,  3,  6,  9,  8, 15, 10,  5,  4, 11, 12, 13, 14,
                                17, 18, 19, 20, 27, 26, 21, 16, 23, 22, 25, 28, 29, 30, 31, 24,
                                39, 32, 33, 34, 35, 38, 41, 40, 47, 42, 37, 36, 43, 44, 45, 46,
                                49, 50, 51, 52, 59, 58, 53, 48, 55, 54, 57, 60, 61, 62, 63, 56};
                        
// ***** Sprite initializers *****
// Yaw - has to use four sprites of length one
const Sprite_t SpYaw[] PROGMEM = {{{.rgb = {32, 0, 0}}, 1, 1, 63, FWD, 50},
                                  {{.rgb = {32, 0, 0}}, 1, 1, 32, FWD, 50},
                                  {{.rgb = {32, 0, 0}}, 1, 1, 31, FWD, 50},
                                  {{.rgb = {32, 0, 0}}, 1, 1,  0, FWD, 50},
                                  {{.rgb = {32, 0, 0}}, 1, 1, 57, FWD, 50},
                                  {{.rgb = {32, 0, 0}}, 1, 1, 38, FWD, 50},
                                  {{.rgb = {32, 0, 0}}, 1, 1, 25, FWD, 50},
                                  {{.rgb = {32, 0, 0}}, 1, 1,  6, FWD, 50}};

#define NUMREVOLUTIONS  4

//******************************************************************************
// Start the action.
//******************************************************************************
void ActionYaw::start(Sprite_t *ps)
{
  pAllSprites = pSprites = ps;
  memcpy_P(pSprites, SpYaw, sizeof(SpYaw));
  numSprites = 4;
  mode = MODE_OUTTER;
  count = NUMREVOLUTIONS;
  timer = 0;
  
  // set all sprites to the current brightness
  for(int i = 0; i < numSprites; i++)
  {
    changeBrightness(pSprites + i);
  } //  for
} //  start

//******************************************************************************
// Add all sprites to the cube.
//******************************************************************************
void ActionYaw::addSprites(void)
{
  Sprite_t *ps;
    
  for(int i = 0; i < numSprites; i++)
  {
    ps = pSprites + i;

    // every sprite has only one LED
    if ((ps->pos >= 0) && (ps->pos < NUMLEDS))
    {
      xorSprite(ps, ps->pos);
    }
  } //  for each sprite
} // addSprites

//******************************************************************************
// The detects the end of one sequence then chnges the direction of every other
// sprite and repeats the sequence.
//******************************************************************************
void ActionYaw::endOfRevolution(void)
{
  Sprite_t *ps;

  // finished one revolution
  count--;
  if (!count)
  {
    // finished n revolutions
    switch(mode)
    {
      case MODE_OUTTER:
        // finished outter, setup for inner
        pSprites = pAllSprites + 4;
        
        // set all sprites to the current brightness
        for(int i = 0; i < numSprites; i++)
        {
          changeBrightness(pSprites + i);
        } //  for
        break;
      case MODE_INNER:
        // finished inner, setup for both
        numSprites = 8;
        pSprites = pAllSprites;
        break;
      case MODE_BOTH:
        // finished both, setup for alternating
        for (int i = 0; i < numSprites; i += 2)
        {
          ps = pAllSprites + i;
          ps->dir ^= REV;
        }
        break;
      case MODE_SPLIT:
        // finished split, setup for different colors
        for (int i = 0; i < numSprites; i++)
        {
          ps = pAllSprites + i;
          ps->hue = random(6) + 1;
          ps->dir = FWD;
        }
        break;
      case MODE_COLOR:
        // finished all variations, do next action
        return;
    } // switch

    count = NUMREVOLUTIONS;
    mode++;
  }
} //  endOfRevolution

//******************************************************************************
// Move the sprite within the cube 
//******************************************************************************
void ActionYaw::moveSprite(void)
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
        ps->pos = pgm_read_byte(YawFwd + ps->pos);
        break;
      case REV:
        ps->pos = pgm_read_byte(YawRev + ps->pos);
        break;
    } //  switch direction
    
    if (0 == ps->pos || 31 == ps->pos || 32 == ps->pos || 63 == ps->pos ||
        6 == ps->pos || 25 == ps->pos || 38 == ps->pos || 57 == ps->pos)
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
        if (4 == numSprites && 6 == ps->pos)
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


