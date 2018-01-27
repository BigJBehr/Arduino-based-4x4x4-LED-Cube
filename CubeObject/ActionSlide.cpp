// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-12-17
//******************************************************************************
//******************************************************************************

// Sprite class implementation
#include <avr/pgmspace.h>
#include "ActionSlide.h"        // ActionSlide class definition

//***** Flash Constants - Maps *****
//                                0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
const byte SlideLeft[] PROGMEM = {0,  7,  8, 15,  1,  6,  9, 14,  2,  5, 10, 13,  3,  4, 11, 12,
//                               16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
                                 16, 23, 24, 31, 17, 22, 25, 30, 18, 21, 26, 29, 19, 20, 27, 28, 
//                               32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47
                                 32, 39, 40, 47, 33, 38, 41, 46, 34, 37, 42, 45, 35, 36, 43, 44, 
//                               48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63
                                 48, 55, 56, 63, 49, 54, 57, 62, 50, 53, 58, 61, 51, 52, 59, 60};
                                 
// ***** Sprite initializers *****
const Sprite_t SpSlide[] PROGMEM = {{{.rgb = {32, 0, 0}}, 1, 16,  0, UP,   50},  // slide up,  1 sprite
                                    {{.rgb = {32, 0, 0}}, 1,  4, 60, FWD,  50},  // slide fwd, 4 sprites
                                    {{.rgb = {32, 0, 0}}, 1,  4, 32, FWD,  50},
                                    {{.rgb = {32, 0, 0}}, 1,  4, 28, FWD,  50},
                                    {{.rgb = {32, 0, 0}}, 1,  4,  0, FWD,  50},
                                    {{.rgb = {32, 0, 0}}, 1,  4, 48, LEFT, 50},  // slide left, 4 sprites
                                    {{.rgb = {32, 0, 0}}, 1,  4, 32, LEFT, 50},
                                    {{.rgb = {32, 0, 0}}, 1,  4, 16, LEFT, 50},
                                    {{.rgb = {32, 0, 0}}, 1,  4,  0, LEFT, 50}};

enum 
{
  MODE_UP, MODE_FWD, MODE_LEFT, NUMMODES
};

#define NUMREVOLUTIONS  4

//******************************************************************************
// Start the action.
//******************************************************************************
void ActionSlide::start(Sprite_t *ps)
{
  pAllSprites = pSprites = ps;
  memcpy_P(pSprites, SpSlide, sizeof(SpSlide));
  pSprites = pAllSprites;
  numSprites = 1;
  mode  = MODE_UP;
  count = NUMREVOLUTIONS;

  // set all sprites to the current brightness
  for(int i = 0; i < numSprites; i++)
  {
    changeBrightness(pSprites + i);
  } //  for
} //  start

//******************************************************************************
// Add all sprites to the cube.
//******************************************************************************
void ActionSlide::addSprites(void)
{
  Sprite_t *ps;

  // for each sprite
  for (int i = 0; i < numSprites; i++)
  {
    ps = pSprites + i;
    // one sprite fills the level
    for (int j = 0; j < ps->len; j++)
    {
      if (MODE_LEFT == mode)
      {
        // follow the map
        xorSprite(ps, pgm_read_byte(SlideLeft + ps->pos + j));
      }
      else
      {
        // MODE_UP and MODE_FWD
        xorSprite(ps, ps->pos + j);
      }
    } // for each LED in the sprite
  } //  for each sprite
} // addSprites

//******************************************************************************
// The detects the end of one sequence then chnges the direction of every other
// sprite and repeats the sequence.
//******************************************************************************
void ActionSlide::endOfRevolution(void)
{
  Sprite_t *ps;

  // finished one revolution
  count--;
  if (!count)
  {
    // finished n revolutions
    switch(mode)
    {
      case MODE_UP:
        // finished up/down, setup for fwd/rev
        // fwd/rev uses four sprites, starting with the second sprite
        pSprites = pAllSprites + 1;
        numSprites = 4;
      
        // set all sprites to the current brightness
        for(int i = 0; i < numSprites; i++)
        {
          changeBrightness(pSprites + i);
        } //  for
        break;
      case MODE_FWD:
        // finished fwd/rev, setup for right/left
        // left/right uses four sprites, starting with the fifth sprite
        pSprites = pAllSprites + 5;
        numSprites = 4;

        // set all sprites to the current brightness
        for(int i = 0; i < numSprites; i++)
        {
          changeBrightness(pSprites + i);
        } //  for
        break;
      case MODE_LEFT:
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
void ActionSlide::moveSprite(void)
{
  Sprite_t *ps;
  byte     level;
  bool     colorChanged = false;
  bool     flag = false;    // forces changes to be made outside of the for loop.
  
  for(int i = 0; i < numSprites; i++)
  {
    ps = pSprites + i;
    // every other level moves in the opposite direction to make all levels
    // move in the same direction.
    level = ps->pos / 16;

    changeBrightness(ps);
    
    switch(ps->dir)
    {
      case FWD:
        if (level & 0x01)
        {
          // odd levels move opposite of even levels
          ps->pos -= 4;
          if (16 == ps->pos || 48 == ps->pos)
          {
            // end of the level reached, change direction
            ps->dir = REV;
          }
        }
        else
        {
          // even level
          ps->pos += 4;
          if (12 == ps->pos || 44 == ps->pos)
          {
            // end of the level reached, change direction
            ps->dir = REV;
          }
        }
        break;
      case REV:
        if (level & 0x01)
        {
          // odd levels move opposite of even levels
          if (28 == ps->pos || 60 == ps->pos)
          {
            // end of the level reached, change direction
            ps->dir = FWD;
            changeColor(ps);
          }
          else
          {
            ps->pos += 4;
          }
        }
        else
        {
          // even level
          if (0 == ps->pos || 32 == ps->pos)
          {
            // end of the level reached, change direction
            ps->dir = FWD;
            colorChanged = changeColor(ps);
          }
          else
          {
            ps->pos -= 4;
          }
          
          if (0 == ps->pos && colorChanged)
          {
            flag = true;
          }
        }
        break;
          
      case UP:
        // move up
        ps->pos += 16;
        if (48 == ps->pos)
        {
          ps->dir = DOWN;
        }
        break;
      case DOWN:
        if (0 == ps->pos)
        {
          ps->dir = UP;
          if (changeColor(ps))
          {
            flag = true;
          } 
        }
        else
        {
          ps->pos -= 16;
        }
        break;
        
      case LEFT:
        ps->pos += 4;
        if (12 == ps->pos || 28 == ps->pos || 44 == ps->pos || 60 == ps->pos)
        {
          ps->dir = RIGHT;
        }
        break;
      case RIGHT:
        if (0 == ps->pos || 16 == ps->pos || 32 == ps->pos || 48 == ps->pos)
        {
          ps->dir = LEFT;
          colorChanged = changeColor(ps);
        }
        else
          ps->pos -= 4;
  
        if (colorChanged && 0 == ps->pos)
        {
          // end of the color sequence reached
          flag = true;
        }
        break;
    } //  switch mode
  } //  for each sprite

  if (flag)
    endOfRevolution();
} //  moveSprite


