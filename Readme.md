# Arduino based 4x4x4 LED Cube
This project is for a 4x4x4 cube of APA106 tri-color, addressable LEDs. The code was developed using the Arduino IDE. The Adafruit NeoPixel Arduino library and the Arduino Encoder library are required. You can install both libraries using the Library Manager in the Arduino IDE.

The code is for a project that was developed for the Arduino contest on the Instructables web site. There you can find detailed instuctions on how to assemble one for yourself. Follow this link; https://www.instructables.com/id/Cubic_Art/.

## The CubeObject folder has all of the Arduino source files. The project was written in C++. Each class used has its own files (.cpp & .h).

## The LEDTest folder has the LEDTest source code used for testing the LED layers as they are assembled. It is easier to correct wiring mistakes as the layers are assembled than it is to replace an LED later.

## The 3D Parts folder has the OpenSCAD source files and .STL files of all of the 3D printed parts of the project. The files are;
    LedCubeJig.stl file is used to 3D print a bending/soldering jig for bending the leads of the LEDs and soldering them together to made the layers of the LED cube.
    spacer.stl file is used to 3D print the spacers that are used to hold the LED layers together while you solder the buses.
    All.stl file is used to 3d print all of the parts for the LED cube.
    Bottom.stl is for 3D printing just the base or bottom of the LED cube.
    Top.stl is for 3D printing the top of the LED cube and the four LED cube holders.
    Knob.stl will 3D print a knob for the rotary encoder in the shape of an orb.

The LedCub.scad file has selections that can be set to true or false to determine what type of Arduino you are using (R3 compatible, ProMini or Nano), what type of USB socket is used and wheather you require a power switch or not. The file usses the software decorator patttern to create the top and base of the LED cube.

## The Diagrams folder has PDF wiring diagrams and a drawing you can print to help make a jig for soldering the LEDs into the layers that make up the cube. The preffered mthod is to use the 3D printed jig, but not everyone has access to a 3D printer. The wiring diagram to use is based on the Arduino type and how you choose to use it.
    Nano Wiring .pdf
    R3 Wiring.pdf
    Wiring.pdf
    SpacingJig.pdf is for making a paper diagram for building a soldering jig out of a block of wood.


Enjoy
