// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17

// ActionSlide class definition
#ifndef ACTIONSLIDE_H
#define ACTIONSLIDE_H

#include "ActionYaw.h"

// ActionSlide is drived from ActionYaw
class ActionSlide : public ActionYaw
{
  // class variables
  protected:
  // class methods
  public:
    void start(Sprite_t *ps);   // start the action
    void endOfRevolution(void);
    void addSprites(void);      // add all sprites to the cube
    void moveSprite(void);      // move the sprite within the cube
}; //  ActionSlide class
#endif  //  ACTIONSLIDE_H
