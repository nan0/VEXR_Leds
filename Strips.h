#ifndef Strips_h
#define Strips_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

class Strips {
private:

public:
  Strips();
  int colorFront;  //The color of the fornt strip
  int colorBack;   // The color of the back strip
  void animateAccordion();
  void animateLinear();
  void fixed();
};

#endif
