// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-09-17
//******************************************************************************
// The ActionContinuous class uses one sprite and moves the sprite sequentially
// through the cube in either direction. Color changes after each pass through
// the cube. Direction can be FWD or REV. 
//******************************************************************************

// Action class implementation
#include <avr/pgmspace.h>
#include "ActionContinuous.h"     // ActionContinuous class definition

#define NUMREVOLUTIONS  4

// ***** Sprite initializers *****
const Sprite_t Sprite[] PROGMEM = {{{.rgb = {32,  0,  0}}, 1, 6, 0, FWD, 50},
                                   {{.rgb = {32,  0,  0}}, 2, 4, 0, REV, 50}};

//******************************************************************************
// Start the action.
//******************************************************************************
void ActionContinuous::start(Sprite_t *ps)
{
  pSprites = ps;
  memcpy_P(pSprites, Sprite, sizeof(Sprite));
  numSprites = 2;
  count = NUMREVOLUTIONS;

  // set the brightness of all sprites
  for(int i = 0; i < numSprites; i++)
  {
    changeBrightness(pSprites + i);
  } //  for
} //  start

//******************************************************************************
// Add all sprites to the cube.
//******************************************************************************
void ActionContinuous::addSprites(void)
{
  Sprite_t  *ps;
  int8_t    pos;
  
  for(int s = 0; s < numSprites; s++)
  {
    ps  = pSprites + s;
    pos = ps->pos;
    
    // add all the LEDs in the sprite
    for (int i = 0; i < ps->len; i++)
    {
      if ((pos >= 0) && (pos < NUMLEDS))
      {
        xorSprite(ps, pos);
      }
      else
      {
        switch(pos)
        {
          case -2:
          case NUMLEDS:
            xorSprite(ps, 32);
            break;
          case -1:
          case NUMLEDS +1:
            xorSprite(ps, 31);
            break;
          default:
            if (pos > NUMLEDS)
            {
              xorSprite(ps, pos - (NUMLEDS + 2));
            }
            if (pos < 0)
            {
              xorSprite(ps, pos + (NUMLEDS + 2));
            }
            break;  
        }
      }
  
      if (FWD == ps->dir & ~FLG_MASK)
        pos--;
      else
        pos++;
    } //  for each LED in the sprite
  } //  for each sprite
} // addSprites

//******************************************************************************
// Move the sprite within the cube 
//******************************************************************************
void ActionContinuous::moveSprite(void)
{
  Sprite_t  *ps;
  
  for(int s = 0; s < numSprites; s++)
  {
    ps = pSprites + s;
    changeBrightness(ps);
    if (FWD == ps->dir & ~FLG_MASK)
    {
      if (ps->pos < NUMLEDS + 1)
        ps->pos++;
      else
      {
        // starting over
        ps->pos = 0;
      }
    }
    else
    {
      if (ps->pos > -2)
        ps->pos--;
      else
        ps->pos = NUMLEDS - 1;
    }
  
    if (0 == ps->pos)
    {
      if (changeColor(ps))
      {
        ps->dir ^= REV;
        if (FWD == ps->dir)
        {
          if (count)
            count--;
        }
      }
    }
  } //  for each sprite
} //  moveSprite


