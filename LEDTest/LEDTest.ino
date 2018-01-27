// Project:  4x4x4 Cube of APA106 addressable LEDs.
// Author:   James R. Behrens
// Compiler: Arduino 1.8.5
// Target:   Arduino Mini, AtMega 328P

//******************************************************************************
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

#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>

#define LEDPIN  6
#define NUMLEDS 16

#define BRIGHTNESS  0x40

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUMLEDS, LEDPIN, NEO_RGB + NEO_KHZ800);


//******************************************************************************
//******************************************************************************
void setup() 
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  Serial.begin(115200);

  leds.begin();

  // turn off all the LEDs
  showCube(0, 0, 0);

  delay(200);
} //  setup

//******************************************************************************
//******************************************************************************
void showCube(byte red, byte green, byte blue)
{
  for (int i = 0; i < NUMLEDS; i++)
    leds.setPixelColor(i, red, green, blue);

  leds.show();
} //  showCube

void fill(byte red, byte green, byte blue)
{
  for (int i = 0; i < NUMLEDS; i++)
  {
    leds.setPixelColor(i, red, green, blue);
    leds.show();
    delay(500);  
  }
} //  fill

//******************************************************************************
//******************************************************************************
void loop()
{
  showCube(BRIGHTNESS, 0, 0);
  delay(500);
  showCube(0, BRIGHTNESS, 0);
  delay(500);
  showCube(0, 0, BRIGHTNESS);
  delay(500);
  showCube(0, 0, 0);
  delay(500);
  fill(BRIGHTNESS, 0, 0);
  delay (500);
} //  loop
