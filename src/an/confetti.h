#ifndef __AN_CONFETTI_H__
#define __AN_CONFETTI_H__

#include "animations.h"


class AnConfetti : public AnIfc {

public:

  AnConfetti(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() const {return "Confetti"; }

  void paint() {

    int totalPixels = numLeds();

    // calculate how many pixels should start at this phase
    int pixelsToShine = (totalPixels * m_density) / (densityScaleFactor);
    int reminderPixels = (totalPixels * m_density) % (densityScaleFactor);
    if(random16(densityScaleFactor) < reminderPixels) {
      pixelsToShine++;
    }


    for(int i=0; i < numLeds(); i++) {
      ledsArray[i].val = max(0, ledsArray[i].val - 5);
    }

    for(int i=0; i<pixelsToShine; i++) {
      int pos = random16(numLeds());
      CHSV colorForNewPixel = m_anGlobalParams.m_leadingColor;
      colorForNewPixel.hue += random8(32) - 16;
      ledsArray[pos] = colorForNewPixel;
    }
  }

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    this->m_density = root["density"].as<uint8_t>();
    Serial.println(String("Confetti: density = ") + this->m_density);

    return true;
  }

  bool validateJson(const JsonObject &root) {

    const JsonVariant currAnName = root["density"];
    if(!currAnName.is<uint8_t>()) {
      Serial.println("AnConfetti: density field in the json is not uint8_t");
      return false;
    }

    return true;
  }

  const int densityScaleFactor = 256 * 32;
  int m_density = 64;

};


#endif // __AN_RAINBOW_H__
