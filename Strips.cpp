#include "Strips.h"

#define PIN_FRONT D1  // The pin on which the front data cable is connected
#define PIN_BACK D2   // The pin on which the back data cable is connected
#define NB_PIXELS 9   // The number of pixels per strip

Adafruit_NeoPixel stripFront(NB_PIXELS, PIN_FRONT, NEO_GRB + NEO_KHZ800);  // The front strip
Adafruit_NeoPixel stripBack(NB_PIXELS, PIN_BACK, NEO_GRB + NEO_KHZ800);    // The back strip

Strips::Strips() {
  colorFront = stripFront.Color(255, 100, 0);
  colorBack = stripFront.Color(255, 100, 0);

  stripFront.begin();
  stripBack.begin();
}

/**
  Triggers one turn of accordion animation for both the strips
**/
void Strips::animateAccordion() {
  const int brightness = 180;
  const int turnDelay = 50;

  stripFront.setBrightness(brightness);
  stripBack.setBrightness(brightness);

  for (int i = 0; i < NB_PIXELS; i++) {
    int offset = 0;
    int middlePos = (int)(NB_PIXELS / 2);
    if (i < middlePos) {
      offset = 0;
    } else if (i > middlePos) {
      offset = NB_PIXELS - 2 * i;
    }
    const int ledLeft = i + offset;
    const int ledRight = NB_PIXELS - i - offset - 1;

    stripFront.clear();
    stripFront.setPixelColor(ledLeft, colorFront);
    stripFront.setPixelColor(ledRight, colorFront);
    stripFront.show();

    stripBack.clear();
    stripBack.setPixelColor(ledLeft, colorBack);
    stripBack.setPixelColor(ledRight, colorBack);
    stripBack.show();

    delay(turnDelay);

    stripFront.clear();
    stripFront.show();
    stripBack.clear();
    stripBack.show();
  }
}

/**
  Triggers one turn of linear animation for both the strips
**/
void Strips::animateLinear() {
  const int brightness = 255;
  const int turnDelay = 35;

  stripFront.setBrightness(brightness);
  stripBack.setBrightness(brightness);

  for (int i = 1; i < NB_PIXELS * 2 - 1; i++) {
    int offset = 0;
    int middlePos = (int)(NB_PIXELS);
    if (i < middlePos) {
      offset = 0;
    } else if (i >= middlePos) {
      offset = -2 * i + NB_PIXELS * 2;
    }
    const int led = i + offset - 1;

    stripFront.clear();

    stripFront.setPixelColor(led, colorFront);
    stripFront.show();

    stripBack.clear();
    stripBack.setPixelColor(led, colorBack);
    stripBack.show();

    delay(turnDelay);

    stripFront.clear();
    stripFront.show();
    stripBack.clear();
    stripBack.show();
  }
}
/**
  Turns on the lights without any animation
**/
void Strips::fixed() {
  const int brightness = 40;
  stripFront.setBrightness(brightness);
  stripBack.setBrightness(brightness);

  for (int i = 0; i < NB_PIXELS; i++) {
    stripFront.setPixelColor(i, colorFront);
    stripFront.show();

    stripBack.setPixelColor(i, colorBack);
    stripBack.show();
  }
}
