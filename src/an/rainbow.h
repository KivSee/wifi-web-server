#ifndef __AN_RAINBOW_H__
#define __AN_RAINBOW_H__

#include "animations.h"


class AnRainbow : public AnIfc {

public:

  AnRainbow(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() const {return "Rainbow"; }

  void paint() {
    uint8_t beatOffset = beat8(bpm());
    for(int i=0; i < numLeds(); i++) {
      uint8_t currHue = beatOffset + (i * 255 * m_hueLoops) / numLeds();
      ledsArray[i] = CHSV(currHue, 255, 255);
    }
  }

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }
    readJsonParameter<>("hueLoops", root, &this->m_hueLoops);
    return true;
  }

  bool validateJson(const JsonObject &root) {
    bool success = true;
    success &= validateParameter<uint16_t>("hueLoops", root);
    return success;
  }

private:
  uint16_t m_hueLoops = 1;

};


#endif // __AN_RAINBOW_H__
