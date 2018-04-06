#ifndef __AN_ALTERNATE_H__
#define __AN_ALTERNATE_H__

#include "animations.h"


class AnAlternate : public AnIfc {

public:

  AnAlternate(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() const {return "Alternate"; }

  void paint() {

    bool phase = beatsin8(bpm(), 0, 1);

    CHSV c1 = m_anGlobalParams.m_leadingColor;
    CHSV c2 = m_anGlobalParams.m_secondaryColor;
    for(int i=0; i<numLeds(); i++) {
      bool currPixelPhase = phase ^ ( (i / m_pixelsPerSegment) % 2 == 0);
      ledsArray[i] = currPixelPhase ? c1 : c2;
    }
  }

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    readJsonParameter<>("pixelsPerSegment", root, &this->m_pixelsPerSegment);

    return true;
  }

  bool validateJson(const JsonObject &root) {
    bool success = true;
    success &= validateParameter<uint8_t>("pixelsPerSegment", root);
    return success;
  }

  uint8_t m_pixelsPerSegment = 1;

};



#endif // __AN_ALTERNATE_H__
