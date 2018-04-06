#ifndef __AN_BLINK_H__
#define __AN_BLINK_H__

#include "animations.h"


class AnBlink : public AnIfc {

public:

  AnBlink(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() const {return "Blink"; }

  void paint() {

    CHSV colorToApply = m_anGlobalParams.m_leadingColor;
    colorToApply.val = beatsin8(bpm(), m_minBrightness, m_maxBrightness);
    for(int i=0; i<numLeds(); i++) {
      ledsArray[i] = colorToApply;
    }
  }

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    readJsonParameter<uint8_t>("minBrightness", root, &this->m_minBrightness);
    readJsonParameter<uint8_t>("maxBrightness", root, &this->m_maxBrightness);

    return true;
  }

  bool validateJson(const JsonObject &root) {
    bool success = true;
    success &= validateParameter<uint8_t>("minBrightness", root);
    success &= validateParameter<uint8_t>("maxBrightness", root);
    if(root["minBrightness"].as<uint8_t>() >= root["maxBrightness"].as<uint8_t>()) {
      Serial.println(String("blink animation min brightness [") +
          root["minBrightness"].as<uint8_t>() + "] should be smaller than max brightness [" +
          root["maxBrightness"].as<uint8_t>() + "]");
      return false;
    }
    return success;
  }

  uint8_t m_minBrightness = 32;
  uint8_t m_maxBrightness = 255;

};



#endif // __AN_BLINK_H__
