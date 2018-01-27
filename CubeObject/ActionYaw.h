// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17

// ActionYaw class definition
#ifndef ACTIONYAW_H
#define ACTIONYAW_H

#include "Action.h"

enum 
{
  MODE_OUTTER, MODE_INNER, MODE_BOTH, MODE_SPLIT, MODE_COLOR,
  NUM_MODES
};

// ActionYaw is drived from Action
class ActionYaw : public Action
{
  // class variables
  protected:
    Sprite_t  *pAllSprites;
    byte      mode;
  // class methods
  public:
    void start(Sprite_t *ps);   // start the action
    void endOfRevolution(void);
    void addSprites(void);      // add all sprites to the cube
    void moveSprite(void);      // move the sprite within the cube
}; //  ActionYaw class
#endif  //  ACTIONYAW_H
