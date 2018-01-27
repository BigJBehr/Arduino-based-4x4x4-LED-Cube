// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-09-17

// ActionContinuous base class definition
#ifndef ACTIONCONTINUOUS_H
#define ACTIONCONTINUOUS_H

#include "Action.h"     // Action base class definitions

class ActionContinuous : public Action
{
  // class variables
  protected:
    
  // class methods
  public:
    void start(Sprite_t *ps);         // start the action
    void addSprites(void);            // add all sprites to the cube
    void moveSprite(void);            // move the sprite within the cube
}; //  ActionContinuous class

#endif  //  ACTIONCONTINUOUS_H

