// 4x4x4 LED Cube 3D printed parts
// All of the parts are made by this file.
// The Base is a rectangle with rounded corners that houses an Arduino
// The Top fits into the base.
// The knob is for the rotary encoder shaft

// #2 x 1/4" pan head wood screws are used to hold the rotary encode and
// the Arduino R3 boards in place.

// Double sticky tape 1/16" (1.5mm) thick is used to hold the Arduino ProMini and
// nano in place.

// Baseball display case top is press fit.

// Case top is press fit into case base.

// select your Arduino by setting it to true
// only one selection can be true.
ArduinoR3 = false;     // Uno or Leonardo
ProMini = true;
Nano = false;

// make this true if your Arduino board uses a micro USB connector
MicroUSB = true;

// Select what parts to 3D print. Set to true to print a part.
// You can print all three at once or each one at a time.
PrintTop = true;
PrintBase = true;
PrintKnob = true;

// Make this false to remove the power switch.
PowerSwitch = true;

//==================================================================================

X = 125;
Y = 97;
Z = 20;
Radius = 10;
Wall = 3;

RotaryEncoderHeight = 5;

SHD_2_56 = 1.75;         // correct size for #2 pan tapping screw
SHD_M2_5 = 2.25;         // correct size for M2.5 pan tapping screw or tap
SHD_M3 = 2.75;           // correct size for M3 pan tapping screw or tap

// Baseball display case cover
BBLip2Lip = 85;
BBLipWidth = 3;
BBLipLength = 84.5;
BBLipDepth = 6;
BBTopX = 89;
BBTopY = BBLip2Lip + BBLipWidth;

ArduinoBossH = 5;

// Arduino R3 mounts 19mm from the inside back wall
R3XOffset = 19;


// Arduino R3 boards have a power jack. The circuitry on the Arduino board
// cannot handle the load of the LED cube (4A). If you wish to use the onboard
// power jack then you must solder wires to the power jack on the bottom of the
// board to run power to the LED cube. If you do this then you do not need the 
// power switch on the top.

ArduinoXOffset = R3XOffset;
// uncomment this offset to hide the Arduino's power jack
//ArduinoXOffset = 19 + Wall;


// Arduino Pro Mini Dimensions
ProMiniX = 44;
ProMiniY = 18.5;
ProMiniZ = 5;

// Arduino Nano Dimensions
NanoX = 18.75;
NanoY = 34.5;
NanoZ = 3;

//===== main =======================================================================
$fn = 50;

if (PrintTop)
{
    DecoratedTop();
    
    for(x = [0:15:45])
        translate([x, -10, 0]) LedCubeMount(0);
}

if (PrintBase)
{
    if (PrintTop)
    {
        translate([0, Y + 3, 0]) DecoratedBase();
    }
    else
    {
        DecoratedBase();
    }
}

if (PrintKnob)
{
    translate([-20, 20, 0]) Knob();
}

//===== Top Level Parts ============================================================

// Top with cutouts for the power switch, mode LED, rotary encoder,
// baseball case and LED cube mounts
module DecoratedTop()
{
    yoff1 = Y / 4;
    yoff2 = Y / 2;
    yoff3 = yoff1 + yoff2;

    // center the baseball display case cover
    bby = (Y - BBTopY) / 2;
    bbx = X - (BBTopX + bby);

    difference()
    {
        union()
        {
            translate([Radius / 2, Radius / 2, 0]) Top();
            
            // add mode LED holder
            translate([Wall + 12, yoff2, 0]) cylinder(d = 9, h = 2 * Wall);
        }   //  union
        
        // add rotaty encoder on top (shaft hole)
        translate([Wall + 12, yoff3, 0]) RotaryEncoder();
        
        // add mode LED top
        translate([Wall + 12, yoff2, 0]) cylinder(d = 5, h = 2 * Wall);
        
        if (PowerSwitch)
        {
            // add the power switch cutout on top
            translate([Wall + 12, yoff1, 0]) RockerSw();
        }
        
        // center the baseball display case cover
        translate([bbx, bby, 0]) BBTopSlots();
        
        // LED Cube wires
        LedCubeWires();
        
        // center the LED cube
        translate([56, (Y - 66) / 2, 4]) rotate([180, 0, 0]) LedCubeMounts(1);
    }   //  difference
}   //  DecoratedTop


// features are added to the basic base
module DecoratedBase()
{
    yoff1 = Y / 4;
    yoff2 = Y / 2;
    yoff3 = yoff1 + yoff2;

    if (ArduinoR3)
    {
        // Arduino R3 (Uno or Leonardo)
        yuno = Y - ((Y - 53) / 2);
        extraZ = 10;
        
        difference()
        {
            union()
            {
                // align base with axis
                translate([Radius / 2, Radius / 2, 0]) Base(Z + extraZ);
        
                // standard Arduino Uno R3 hole pattern
                translate([X - ArduinoXOffset, yuno, 1]) rotate([0, 0, 180]) R3Bosses(6);
            }   //  union
            
            // cutouts in the back wall for the Uno powerjack and USB-B connector
            // Uno mounting bosses
            translate([X - ArduinoXOffset, yuno, 1]) rotate([0, 0, 180])
            {
                UnoBosses(SHD_M3);
                if (ArduinoXOffset == R3XOffset)
                {
                    R3PowerJack();
                }

                if (MicroUSB)
                {
                    MicroUsb();
                }
                else
                {
                    R3Usb();
                }
            }
            
            if (ArduinoXOffset != R3XOffset)
            {
                // add power jack to back panel
                PowerJack(X - 12, 15, (Z + extraZ)/ 2);
            }
                

        }   //  difference
    
        // add rotaty encoder bosses
        translate([Wall + 12, yoff1, Wall]) RotaryEncoderBosses(RotaryEncoderHeight + extraZ);
    }
    else
    {
        // not an Arduino R3
        difference()
        {
            // align base with axis
            translate([Radius / 2, Radius / 2, 0]) Base(Z);
            
            // add power jack to back panel
            PowerJack(X - 12, 15, Z / 2);
            
            // Arduino Pro Mini has a Mini USB connector
            if (ProMini)
            {
                translate([X - ProMiniX - 5, 50, Wall]) ProMiniUsb();
            }
        }   //  difference
        
        // add rotaty encoder bosses
        translate([Wall + 12, yoff1, Wall]) RotaryEncoderBosses(RotaryEncoderHeight);
        
        if (ProMini)
        {
            translate([X - ProMiniX - Wall - 2, 50, Wall]) ProMiniMount();
        }

        if (Nano)
        {
            translate([60, (Y - (NanoY + 4)) / 2, Z - NanoZ - Wall]) NanoMount();
        }
    }
}   //  DecoratedBase

// Rotary Encoder Ball Knob
module Knob()
{
    od = 10;
    z = 15;

    $fn = 75;
    difference()
    {
        union()
        {
            translate([0, 0, od]) sphere(od);
            cylinder(d = od, h = 5);
        }
        DHole(z);
    }
}   //  Knob

//===== Various Building Blocks ====================================================

// basic base, a hollow block with rounded corners
module Base(z)
{
    difference()
    {
        minkowski()
        {
            cube([X - Radius, Y - Radius, z / 2]);
            cylinder(d = Radius, h = z / 2);
        }   // minkowski
        
        // hollow it out
        translate([Wall, Wall, Wall]) minkowski()
        {
            cube([X - Radius - Wall - Wall, Y - Radius - Wall - Wall, (z / 2) - Wall]);
            cylinder(d = Radius, h = (z / 2));
        }   // minkowski
    }   // difference
}   //Base

// Top fits into the base
module Top()
{
    z = Wall;
    translate([0, 0, 0]) minkowski()
    {
        cube([X - Radius, Y - Radius, z / 2]);
        cylinder(d = Radius, h = z / 2);
    }   // minkowski
        
    translate([Wall, Wall, Wall]) difference()
    {
        minkowski()
        {
            cube([X - Radius - (2 * Wall), Y - Radius - (2 * Wall), (z / 2)]);
            cylinder(d = Radius, h = z / 2);
        }   // minkowski
        
        // hollow it out
        translate([Wall, Wall, 0]) minkowski()
        {
            cube([X - Radius - (4 * Wall), Y - Radius - (4 * Wall), (z / 2)]);
            cylinder(d = Radius, h = z / 2);
        }   // minkowski
    }   // difference
}   //  Top

//===== Arduino mounts =====

// uses double stick tape to hold it in place
module ProMiniMount()
{
    difference()
    {
        cube([ProMiniX + 2, ProMiniY + 4, ProMiniZ]);
        translate([2, 2, 0]) cube([ProMiniX, ProMiniY, ProMiniZ]);
    }
       
}   //  ProMiniMount

module ProMiniUsb()
{
    usbx = Wall + 5;
    usby = 9;
    usbz = 6;
    
    translate([ProMiniX + 2, (ProMiniY + 4 - usby) / 2, ProMiniZ]) cube([usbx, usby, usbz]);
}   //  ProMiniUsb

// uses double stick tape to hold it in place
module NanoMount()
{
    difference()
    {
        cube([NanoX + 4, NanoY + 4, NanoZ]);
        translate([2, 2, 0]) cube([NanoX, NanoY, NanoZ]);
    }
}   //  NanoMount

// Arduino Uno R3 & Leonardo
// uses #2x1/4" pan head, wood screws
module R3Bosses(d)
{
    x = 52.25;
    y = 48.75;
    z = 5;

    cylinder(d = d, h = ArduinoBossH); 
    translate([1.5, y, 0]) cylinder(d = d, h = ArduinoBossH); 

    translate([x, 5, 0]) cylinder(d = d, h = ArduinoBossH);
    translate([x, 33, 0]) cylinder(d = d, h = ArduinoBossH);
}   //  R3Bosses

// Arduino R3 power jack
module R3PowerJack()
{
    x = 10;
    y = 10;
    z = 12;
    
    translate([-ArduinoXOffset - Wall, 1.5, ArduinoBossH + 1.5]) cube([x, y, z]);
}   //  R3PowerJack

// Arduino R3 USB type B receptacle
module R3Usb()
{
    x = 10;
    y = 13;
    z = 12;
    
    translate([-ArduinoXOffset - Wall, 29.5, ArduinoBossH + 1.5]) cube([x, y, z]);
}   //  R3Usb

// Micro USB cutout.
module MicroUsb()
{
    x = 10;
    y = 13;
    z = 12;
    
    translate([-ArduinoXOffset - Wall, 29.5, ArduinoBossH + 1.5]) cube([x, y, z]);
}   //  UnoUsb

// LED Cube Wire holes
module LedCubeWires()
{
    // wire holes
    
    translate([45, 22, 0]) cylinder(d = 3, h = 2 * Wall);  // Signal
    translate([74, 31, 0]) cylinder(d = 3, h = 2 * Wall);  // Gnd
    translate([74, 16, 0]) cylinder(d = 3, h = 2 * Wall);  // Power
}   //  LedCubeWires

// LED cube mounts
module LedCubeMounts(a)
{
    xp2p = 66;
    yp2p = 34;
    
    LedCubeMount(a);
    translate([yp2p, 0, 0]) LedCubeMount(a);

    translate([0,    -xp2p, 0]) LedCubeMount(a);
    translate([yp2p, -xp2p, 0]) LedCubeMount(a);
}   //  LedCubeMounts

module LedCubeMount(a)
{
    x = 5;
    z = 7;
    wire = 1;           // sized for 22awg bare wire

    translate([0, 0, Wall / 2]) difference()
    {
        cylinder(d = x, h = z);
        if (a == 0)
        {
            translate([-x / 2, -wire / 2, z - 2]) cube([x, wire, z]);
        }
    }
    
    translate([-x, -x / 2, 0]) cube([2 * x, x, Wall]); 
}   //  LedCubeMount

// slots for the baseball display case cover
module BBTopSlots()
{
    translate([0, 0, 0]) cube([BBLipLength, BBLipWidth, BBLipDepth]);
    translate([0, BBLip2Lip, 0]) cube([BBLipLength, BBLipWidth, BBLipDepth]);
}   //  TopSlot

// rectangular rocker switch
module RockerSw()
{
    swy = 13.5;
    swx = 8.5;
    swz = Wall;

    translate([-swx / 2, -swy / 2, 0]) cube([swx, swy, swz]);
}   //  RockerSw

// power jack with the retaining nut on the outside
module PowerJack(x, y, z)
{
    od = 11;
    id = 8;
    
    translate([x, y, z]) rotate([0, 90, 0]) 
    {
        cylinder(d = od, h = 10);
        cylinder(d = id, h = 15);
    }
}       //  PowerJack

module RotaryEncoder()
{
    shaftD = 8;
    
    cylinder(d = shaftD, h = 2 * Wall);
}   //  RotaryEncoder

module RotaryEncoderBosses(z)
{
    xoffset = -5;
    xh2h = 14;
    yoffset = -8.25;
    
    translate([xoffset, yoffset, 0])
    {
        RotaryEncoderBoss(z);
        translate([xh2h, 0, 0]) RotaryEncoderBoss(z);
        
        // support
        translate([0, 14, 0]) cube([xh2h, 2, z]) ;
    }
}   //  RotaryEncoderBosses
    
module RotaryEncoderBoss(z)
{
    bossD = 4;

    difference()
    {
        cylinder(d = bossD, h = z);
        cylinder(d = SHD_2_56, h = z);
    }
}   //  RotaryEncoderBoss

// D-Hole to match the rotary encoder shaft
module DHole(z)
{
    id = 6.25;

    difference()
    {
        cylinder(d = id, h = z - 3);
        translate([(id / 2) - 1.5, -id / 2, 0]) cube([1.5, id, z - 3]);
    }
}   //  DHole

//======

// Bosses for mounting of bottom plate
module BaseBosses()
{
    BaseBoss(3, 3);
    BaseBoss(3, Y - Wall - Radius);
    BaseBoss(X - Wall - Radius, 3);
    BaseBoss(X - Wall - Radius, Y - Wall - Radius);
}   //  BaseBosses

// definition of a base boss
module BaseBoss(x, y)
{
    z = 5;
    translate([x, y, z]) difference()
    {
        cylinder(d = 8, h = Z - z);
        cylinder(d = 3, h = Z - z);
    }
}   //  BaseBoss




