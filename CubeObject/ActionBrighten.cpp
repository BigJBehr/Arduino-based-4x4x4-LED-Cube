// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-09-17
//******************************************************************************
// The ActionBrighten class fills the cube with a single color and then
// brightens the color until MAXBRIGHTNESS is achieved. Then the cube dims
// until all LEDs are blanked. Color is changed and the process continues.
//******************************************************************************

// Action class implementation
#include <avr/pgmspace.h>
#include "ActionBrighten.h"     // ActionBrighten class definition

#define RANDOMDELAY     1000

#define NUMREVOLUTIONS  1

#define STEP            2

#define MAXBRIGHTNESS   64    // limit the brightness to 25%

// ***** Sprite initializers *****
const Sprite_t Sprite[] PROGMEM = {{{.rgb = {0,  0,  0}}, 1, NUMLEDS, 0, FWD, 50}};

byte mode = 0;
//******************************************************************************
// Start the action.
//******************************************************************************
void ActionBrighten::start(Sprite_t *ps)
{
  pSprites = ps;
  memcpy_P(pSprites, Sprite, sizeof(Sprite));
  numSprites = 1;
  count = NUMREVOLUTIONS;

  // seed the random number geneerator
  randomSeed(analogRead(0));
} //  start

//******************************************************************************
// Fill the cube with one color
//******************************************************************************
void ActionBrighten::addSprites(void)
{
  for (int i = 0; i < NUMLEDS; i++)
  {
    xorSprite(pSprites, i);
  } //  for
} // addSprites

//******************************************************************************
// Nothing moves. This will be invoked every RANDOMDELAY milliseconds.
//******************************************************************************
void ActionBrighten::moveSprite(void)
{
  int i;
  switch (mode)
  {
    case 0:   // red brighten
      if (pSprites->led.rgb.red)
      {
        i = pSprites->led.rgb.red;
        i += STEP;
        if (i < MAXBRIGHTNESS)
        {
          pSprites->led.rgb.red = i;
        }
        else
        {
          pSprites->led.rgb.red = MAXBRIGHTNESS;
          mode++;
        }
      }
      else
        pSprites->led.rgb.red = STEP - 1;
      break;
    case 1:   // red dim
      if (pSprites->led.rgb.red)
      {
        i = pSprites->led.rgb.red;
        i -= STEP;
        if (i > 0)
        {
          pSprites->led.rgb.red = i;
        }
        else
        {
          pSprites->led.rgb.red = 0;
          mode++;
        }
      }
      else
        pSprites->led.rgb.red = STEP - 1;
      break;
    case 2:   // green brighten
      if (pSprites->led.rgb.green)
      {
        i = pSprites->led.rgb.green;
        i += STEP;
        if (i < MAXBRIGHTNESS)
        {
          pSprites->led.rgb.green = i;
        }
        else
        {
          pSprites->led.rgb.green = MAXBRIGHTNESS;
          mode++;
        }
      }
      else
        pSprites->led.rgb.green = STEP - 1;
      break;
    case 3:   // green dim
      if (pSprites->led.rgb.green)
      {
        i = pSprites->led.rgb.green;
        i -= STEP;
        if (i > 0)
        {
          pSprites->led.rgb.green = i;
        }
        else
        {
          pSprites->led.rgb.green = 0;
          mode++;
        }
      }
      else
        pSprites->led.rgb.green = STEP - 1;
      break;
    case 4:   // blue brighten
      if (pSprites->led.rgb.blue)
      {
        i = pSprites->led.rgb.blue;
        i += STEP;
        if (i < MAXBRIGHTNESS)
        {
          pSprites->led.rgb.blue = i;
        }
        else
        {
          pSprites->led.rgb.blue = MAXBRIGHTNESS;
          mode++;
        }
      }
      else
        pSprites->led.rgb.blue = STEP - 1;
      break;
    case 5:   // blue dim
      if (pSprites->led.rgb.blue)
      {
        i = pSprites->led.rgb.blue;
        i -= STEP;
        if (i > 0)
        {
          pSprites->led.rgb.blue = i;
        }
        else
        {
          pSprites->led.rgb.blue = 0;
          mode = 0;
          count--;
        }
      }
      else
        pSprites->led.rgb.blue = STEP - 1;
      break;
  } //  switch mode
} //  moveSprite


