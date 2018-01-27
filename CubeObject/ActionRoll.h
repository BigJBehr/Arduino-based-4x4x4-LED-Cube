// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17

// ActionRoll class definition
#ifndef ACTIONROLL_H
#define ACTIONROLL_H

#include "ActionYaw.h"

// ActionRoll is drived from Action
class ActionRoll : public ActionYaw
{
  // class variables
  private:
  // class methods
  public:
    void start(Sprite_t *ps);   // start the action
    void moveSprite(void);      // move the sprite within the cube
}; //  ActionRoll class
#endif  //  ACTIONROLL_H
