// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-09-17
//******************************************************************************
// The ActionRandom class fills the cube with randomly selected colors. 
// Brightness and color are changed every second. The display lasts for ten
// seconds. No sprites are used.
//******************************************************************************

// Action class implementation
#include <avr/pgmspace.h>
#include "ActionRandom.h"     // ActionRandom class definition

#define RANDOMDELAY     1000

#define NUMREVOLUTIONS  10

#define MAXBRIGHTNESS   64    // limit the brightness to 25%

// ***** Sprite initializers *****
const Sprite_t Sprite[] PROGMEM = {{{.rgb = {0,  0,  0}}, 1, NUMLEDS, 0, FWD, 50}};

//******************************************************************************
// Start the action.
//******************************************************************************
void ActionRandom::start(Sprite_t *ps)
{
  pSprites = ps;
  memcpy_P(pSprites, Sprite, sizeof(Sprite));
  numSprites = 1;
  count = NUMREVOLUTIONS;

  // seed the random number geneerator
  randomSeed(analogRead(0));
} //  start

//******************************************************************************
// Fill the cube with randomly selected colors.
//******************************************************************************
void ActionRandom::addSprites(void)
{
  for (int i = 0; i < NUMLEDS; i++)
  {
    do
    {
      pSprites->led.rgb.red   = random(MAXBRIGHTNESS);
      pSprites->led.rgb.green = random(MAXBRIGHTNESS);
      pSprites->led.rgb.blue  = random(MAXBRIGHTNESS);
    }
    while (!pSprites->led.color);

    xorSprite(pSprites, i);
  } //  for
} // addSprites

//******************************************************************************
// Nothing moves. This will be invoked every RANDOMDELAY milliseconds.
//******************************************************************************
void ActionRandom::moveSprite(void)
{
  // change the timer to one second
  setTimer(RANDOMDELAY);

  // count down ten seconds
  if (count)
    count--;
} //  moveSprite


