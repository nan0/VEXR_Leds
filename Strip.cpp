#include <cstdio>
#include "HardwareSerial.h"
#include <EEPROM.h>
#include "Strip.h"
#include "EEPROMData.h"

#define NB_PIXELS 10  // The number of pixels per strip

Strip::Strip(int pin, int slotNumber) {

  if (slotNumber < 1 || slotNumber > 50) {
    Serial.println("slotNumber must be between 1 and 50");
    return;
  }

  this->slotNumber = slotNumber;

  EEPROM.begin(512);

  leds = Adafruit_NeoPixel(NB_PIXELS, pin, NEO_GRB + NEO_KHZ800);
  leds.begin();
}

void Strip::powerOn(bool on) {
  for (int i = 0; i < NB_PIXELS; i++) {
    if (!on) {
      leds.setPixelColor(i, leds.Color(0, 0, 0));
    } else {
      leds.setPixelColor(i, color);
    }
    leds.show();
  }
  const int address = this->getSlot(EEPROMData::POWER);
  EEPROM.write(address, (char)on);
  EEPROM.commit();
}

bool Strip::isPoweredOn() {
  const int address = Strip::getSlot(EEPROMData::POWER);
  return (bool)EEPROM.read(address);
}

void Strip::setBrightness(int level) {
  const int address = Strip::getSlot(EEPROMData::BRIGHTNESS);
  EEPROM.write(address, (char)level);
  EEPROM.commit();
  this->leds.setBrightness(level);
}

int Strip::getBrightness() {
  const int address = Strip::getSlot(EEPROMData::BRIGHTNESS);
  return (int)EEPROM.read(address);
}

void Strip::setAnimation(AnimationType animation) {
  const int address = Strip::getSlot(EEPROMData::ANIMATION);
  EEPROM.write(address, (char)animation);
  EEPROM.commit();
  switch (animation) {
    case AnimationType::LINEAR:
      this->animateLinear();
      break;
    case AnimationType::ACCORDION:
      this->animateAccordion();
      break;
    case AnimationType::FIXED:
      this->fixed();
      break;
  }
}

AnimationType Strip::getAnimation() {
  const int address = Strip::getSlot(EEPROMData::ANIMATION);
  const int animation = EEPROM.read(address);
  return static_cast<AnimationType>(animation);
}

void Strip::animateAccordion() {
  int offset = 0;
  if (led < 10) {
    offset = 0;
  } else {
    offset = 2 * (NB_PIXELS - led) - 2;
  }
  const int ledLeft = led + offset;
  const int ledRight = NB_PIXELS - led - offset - 1;

  leds.clear();
  leds.setPixelColor(ledLeft, color);
  leds.setPixelColor(ledRight, color);
  leds.show();

  if (led == 17) {
    led = 0;
  } else {
    led++;
  }

  delay(20);
}

void Strip::animateLinear() {
  int offset = 0;

  if (led > 9) {
    offset = 2 * (NB_PIXELS - led) - 2;
  }

  leds.clear();
  leds.setPixelColor(led + offset, color);
  leds.show();

  if (led == 17) {
    led = 0;
  } else {
    led++;
  }

  delay(10);
}

void Strip::fixed() {
  leds.clear();
  for (int i = 0; i < NB_PIXELS; i++) {
    leds.setPixelColor(i, color);
    leds.show();
  }
}

char Strip::getSlot(int EEPROMData) {
  return this->slotNumber * 10 + EEPROMData;
}

void Strip::animate() {
  switch (this->getAnimation()) {
    case AnimationType::LINEAR:
      if (this->isPoweredOn()) {
        this->animateLinear();
      }
      break;
    case AnimationType::ACCORDION:
      if (this->isPoweredOn()) {
        this->animateAccordion();
      }
      break;
    case AnimationType::FIXED:
      if (this->isPoweredOn()) {
        this->fixed();
      }
      break;
  }
}

void Strip::setColor(int r, int g, int b) {
  this->color = this->leds.Color(g, r, b);
}
