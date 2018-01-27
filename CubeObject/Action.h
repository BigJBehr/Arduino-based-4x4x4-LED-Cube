// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.0
// Target:   Arduino Mini, AtMega 328P
// 10-07-17

// Sprite basec class definition
#ifndef ACTION_H
#define ACTION_H

#include "CubeObject.h"   // common definitions


class Action
{
  // class variables
  protected:
    Sprite_t  *pSprites;  // pointer to the sprites array
    byte      numSprites; // number of sprites in use by the action
    byte      brightness; // maximum brightness allowed
    int       timer;      // timer count
    int       count;      // number of times the action runs
    
  // class methods
  public:
    void xorSprite(Sprite_t *ps, int pos);  // add an LED to the Cube by xoring its colors
    void changeBrightness(Sprite_t *ps);    // change the brightness of the sprite
    bool changeColor(Sprite_t *ps);         // step to next color, return true when a color cycle is complete
    void setBrightness(byte b);             // set a new brightness level
    void setTimer(int rate);                // load the timer with a new rate
    bool runTimer(void);                    // run the Sprite's timer, return true when timer is expired
    bool isDone(void);                      // true if the action has finished
    virtual void start(Sprite_t *ps);       // start the action
    virtual void addSprites(void);           // add a sprite to the cube
    virtual void moveSprite(void);          // move the sprite within the cube
}; //  Action class

#endif  //  ACTION_H

