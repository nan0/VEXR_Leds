#ifndef Strips_h
#define Strips_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

class Strips {

public:
  Strips();
  int colorFront;  //The color of the fornt strip
  int colorBack;   // The color of the back strip
  int brightnessFront;
  int brightnessBack;

  /**
    Powers On/off the led strips
    param on : true if on, false if off
  **/
  void powerOn(bool on);

  /**
    Gets the current power status
    returns : true if on, false, if off
  **/
  bool isPoweredOn();

  /**
    Triggers one turn of accordion animation for both the strips
  **/
  void animateAccordion();

  /**
    Triggers one turn of linear animation for both the strips
  **/
  void animateLinear();

  /**
    Turns on the lights without any animation
  **/
  void fixed();
};

#endif
