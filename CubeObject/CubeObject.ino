// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.5
// Target:   Arduino Mini, AtMega 328P
// 10-07-17
//==============================================================================
// The LEDs are wired as a continuous strip of 64 LEDs. The LEDs are arranged
// into four layers of 16 LEDs each. Every other layer is reversed. Every other
// row is reversed.

// The number represents the LED position in the strip, The layers are;
//---- Layer 0 ---- + ---- Layer 1 ---- + ---- Layer 2 ---- + ---- Layer 3 ----
//  3,  4, 11, 12   |   28, 27, 20, 19  |   35, 36, 43, 44  |   60, 59, 52, 51
//  2,  5, 10, 13   |   29, 26, 21, 18  |   34, 37, 42, 45  |   61, 59, 52, 51
//  1,  6,  9, 14   |   30, 25, 22, 17  |   33, 38, 41, 46  |   62, 57, 54, 49
//  0,  7,  8, 15   |   31, 24, 23, 16  |   32, 39, 40, 47  |   63, 56, 55, 48

// The AtMega328P only has 2KB of RAM. To avoid using up all the RAM, all of
// maps and initializers have been moved to Flash memory. Special functions
// are used to move the data from Flash to RAM.

// The maps are indexed by the current position of an LED. The value returned
// is the new position of the LED.

// A rotary quadrature encoder is used to set the speed of the sprites. The
// encoder is wired such that a clockwise rotation increases the speed of the
// sprites (decrease Speed to increase sprite speed).
// CLK -> pin 5, DT -> pin 4, SW -> pin 3

// This project uses object oriented coding.
//==============================================================================
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>
#include <avr/pgmspace.h>

//#include "CubeObject.h"     // common definitions
#include "ActionContinuous.h" // ActionCotinuous class definitions
#include "ActionYaw.h"        // ActionYaw class definitions
#include "ActionPitch.h"      // ActionPitch class definitions
#include "ActionRoll.h"       // ActionRoll class definitions
#include "ActionSlide.h"      // ActionSlide class definitions
#include "ActionRandom.h"     // ActionRandom class definitions
#include "ActionBrighten.h"   // ActionBrighten class definitions


#define NUMACTIONS    7

#define SWPIN       3           // rotary encoder pins
#define DTPIN       4
#define CLKPIN      5
#define LEDPIN      6           // I/O pin the Cube is wired to
#define MODELEDPIN  7           // mode indicator pin

#define DBGLED      13    // debug LED

#define RANDOMTIME    16
#define RANDOMDELAY   1000

#define SWMASK        0x00000FFF
enum
{
  NOMODE = 0,
  SPEEDMODE,
  BRIGHTMODE
};

//***** Ram Variables *****
int       LoopDelay = 1;
int       LoopTimer = 10;
int       Index = 0;
int       Speed = 100;
Action    *pAction;
Action    *pActions[NUMACTIONS];
uint32_t  debounce = 0;
bool      flag = false;

// This configuration works for APA106 LEDs.
// LED cube object
Adafruit_NeoPixel Leds = Adafruit_NeoPixel(NUMLEDS, LEDPIN, NEO_RGB + NEO_KHZ800);
// mode LED object
Adafruit_NeoPixel ModeLed = Adafruit_NeoPixel(1, MODELEDPIN, NEO_RGB + NEO_KHZ800);
// decrease means the sprites move faster, increase moves the sprites slower
Encoder SpeedEnc(CLKPIN, DTPIN);

// The cube as an array of APA106 LEDs
Apa106_t  Cube[NUMLEDS];

Sprite_t sprites[MAXSPEITES];

int32_t oldPosition  = -999;
byte    swmode = NOMODE;
int     Brightness = 32;

//******************************************************************************
// Pin 13 has has the built-in LED.
// Pin 3 is the rotary encoder switch input
// Pin 4 is the rotary encoder DT input
// Pin 5 is the rotary encoder CLK input
// Pin 6 is the APA106 LED cube output pin 
// Pin 7 is the APA106 Mode LED output pin
//******************************************************************************
void setup()
{
  pinMode(DBGLED, OUTPUT);
  digitalWrite(DBGLED, LOW);
  
  pinMode(SWPIN, INPUT_PULLUP);

  Serial.begin(115200);

  randomSeed(analogRead(0));
  
  Leds.begin();
  ModeLed.begin();

  // turn off all LEDs
  ModeLed.setPixelColor(0, 0, 0, 0);
  fillCube(0);
  showCube();

  // allocate an array of pointers to each instansiated Action
  pActions[0] = new ActionContinuous();
  pActions[1] = new ActionYaw();
  pActions[2] = new ActionPitch();
  pActions[3] = new ActionRoll();
  pActions[4] = new ActionSlide();
  pActions[5] = new ActionRandom();
  pActions[6] = new ActionBrighten();

  // setup first action
  selectAction();

  delay(100);
} //  Setup

//******************************************************************************
// Fill the cube with a single color.
//******************************************************************************
void fillCube(uint32_t color)
{
  for (int i = 0; i < NUMLEDS; i++)
    Cube[i].color = color;
} //  fillCube

//******************************************************************************
// Transfer the contents of the Cube to the NeoPixel buffer and display it.
//******************************************************************************
void showCube()
{
  for (int i = 0; i < NUMLEDS; i++)
    Leds.setPixelColor(i, Cube[i].rgb.red, Cube[i].rgb.green, Cube[i].rgb.blue);

  Leds.show();
} //  showCube

//******************************************************************************
// Select the next action from the array of actions.
//******************************************************************************
void selectAction()
{
  pAction = pActions[Index];
  pAction->setTimer(Speed);
  pAction->setBrightness(Brightness);
  pAction->start(sprites);
  Index++;
  Index %= NUMACTIONS;
} //  selectAction

//******************************************************************************
//******************************************************************************
void loop()
{
  int32_t newPosition, change;
  
  if (pAction->runTimer())
  {
    pAction->setTimer(Speed);
    pAction->setBrightness(Brightness);

    // blank the cube
    fillCube(0);
  
    // add all sprites to the cube
    pAction->addSprites();

    // show the cube
    showCube();

    // move all sprites
    pAction->moveSprite();

    if (pAction->isDone())
        selectAction();
  }

  delay(LoopDelay);

  // check the rotary encoder
  newPosition = SpeedEnc.read();
  if (newPosition != oldPosition)
  {
    change = newPosition - oldPosition;
    oldPosition = newPosition;

    switch(swmode)
    {
      case NOMODE:
        break;
      case SPEEDMODE:
        Speed += change;
        if (Speed < 1)
          Speed = 1;
        break;
      case BRIGHTMODE:
        Brightness -= change;
        if (Brightness > 255)
          Brightness == 255;
  
        if (Brightness < 1)
          Brightness = 1;
          break;
    } //  switch
  }

  debounce <<= 1;
  debounce |= digitalRead(SWPIN) & 0x00000001;
  if ((0 == debounce & SWMASK))
  {
    if (flag)
    {
      digitalWrite(DBGLED, HIGH);
      flag = false;
      swmode++;
      if (swmode > BRIGHTMODE)
        swmode = NOMODE;
    }
  }
  else if (SWMASK == debounce & SWMASK)
  {
    flag = true;
    digitalWrite(DBGLED, LOW);
  }

  switch(swmode)
  {
    case NOMODE:
      ModeLed.setPixelColor(0, 0, 0, 0);
      ModeLed.show();
      break;
    case SPEEDMODE:
      // red for SPEEDMODE
      ModeLed.setPixelColor(0, 32, 0, 0);
      ModeLed.show();
      break;
    case BRIGHTMODE:
      // green for BRIGHTNESSMODE
      ModeLed.setPixelColor(0, 0, 32, 0);
      ModeLed.show();
      break;
  } //  switch

} //  Loop

