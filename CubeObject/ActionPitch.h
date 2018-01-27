// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17

// ActionPitch class definition
#ifndef ACTIONPITCH_H
#define ACTIONPITCH_H

#include "ActionYaw.h"

// ActionPitch is drived from ActionYaw
class ActionPitch : public ActionYaw
{
  // class variables
  private:
  // class methods
  public:
    void start(Sprite_t *ps);   // start the action
    void moveSprite(void);      // move the sprite within the cube
}; //  ActionPitch class
#endif  //  ACTIONPITCH_H
