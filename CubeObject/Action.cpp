// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17
//******************************************************************************
// This is the base class for all Actions. Derive new Action classes from this
// class.
//******************************************************************************

// Action class implementation
#include <avr/pgmspace.h>
#include "Action.h"     // Sprite class definition

//******************************************************************************
// Add an LED by xoring all of its color values with the color values of the
// cube at that position.
//******************************************************************************
void Action::xorSprite(Sprite_t *ps, int pos)
{
  if (pos >= 0 && pos < NUMLEDS)
    Cube[pos].color ^= ps->led.color;
} //  xorSprite

//******************************************************************************
// Add all sprites to the cube.
//******************************************************************************
void Action::addSprites(void)
{
} // addSprites

//******************************************************************************
// Move the sprite within the cube 
//******************************************************************************
void Action::moveSprite(void)
{
} //  moveSprite

//******************************************************************************
// Change the brightness of the sprite
//******************************************************************************
void Action::changeBrightness(Sprite_t *ps)
{
  uint32_t  color = brightness & 0x000000FF;
  switch (ps->hue)
  {
    case 1:     // red
      break;
    case 2:     // green
      color <<= 8;
      break;
    case 3:     // red + green
      color <<= 8;
      color |= brightness & 0x000000FF;
      break;
    case 4:     // blue
      color <<= 16;
      break;
    case 5:     // red + blue
      color <<= 16;
      color |= brightness & 0x000000FF;
      break;
    case 6:     // green + blue
      color <<= 8;
      color |= brightness & 0x000000FF;
      color <<= 8;
      break;
    case 7:     // red + green + blue
      color <<= 8;
      color |= brightness & 0x000000FF;
      color <<= 8;
      color |= brightness & 0x000000FF;
      break;
  }

  ps->led.color = color;
} //  changeBrightness

//******************************************************************************
// Step to the next color. Return true if one color cycle is finished.
//******************************************************************************
bool Action::changeColor(Sprite_t *ps)
{
  ps->hue++;
  if (ps->hue > 7)
  {
    ps->hue = 1;
  }

  return ps->hue == 1;
} //  changeColor

//******************************************************************************
// Return true if the action has finished.
//******************************************************************************
bool Action::isDone(void)
{
  return !count;
} //  isDone

//******************************************************************************
// Load the sprite's timer with a new rate value (change speed);
//******************************************************************************
void Action::setTimer(int rate)
{
  timer = rate;
} //  runTimer

//******************************************************************************
// Load the sprite's brightness with a new value (change brighness);
//******************************************************************************
void Action::setBrightness(byte b)
{
  brightness = b;
} //  setBrightness

//******************************************************************************
// Run the sprite's timer. Return true when timer is zero
//******************************************************************************
bool Action::runTimer()
{
  if (timer)
    timer--;
    
  return !timer;  
} //  runTimer


