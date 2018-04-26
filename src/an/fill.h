#ifndef __AN_FILL_H__
#define __AN_FILL_H__

#include "animations.h"

class AnFill : public AnIfc {

public:

  AnFill(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() const {return "Fill"; }

  void paint() {
    uint16_t beatPos = beat16(bpm());
    uint16_t changePos = beatPos / (65535 / (numLeds() * 2) ) % (numLeds() * 2);

    CHSV c1, c2;
    if(changePos < numLeds()) {
      c1 = m_anGlobalParams.m_leadingColor;
      c2 = m_anGlobalParams.m_secondaryColor;
    }
    else {
      c1 = m_anGlobalParams.m_secondaryColor;
      c2 = m_anGlobalParams.m_leadingColor;
      changePos -= numLeds();
    }

    if(!m_directionStartToEnd) {
      changePos = numLeds() - changePos - 1;
    }


    for(int i=0; i<changePos; i++) {
      ledsArray[i] = c1;
    }
    for(int i=changePos; i<numLeds(); i++) {
      ledsArray[i] = c2;
    }

  }

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }
    readJsonParameter<>("directionStartToEnd", root, &this->m_directionStartToEnd);
    return true;
  }

  bool validateJson(const JsonObject &root) {
    bool success = true;
    success &= validateParameter<bool>("directionStartToEnd", root);
    return success;
  }

  bool m_directionStartToEnd = true;

};

#endif // __AN_FILL_H__
