#ifndef __AN_SOLID_COLOR_H__
#define __AN_SOLID_COLOR_H__

#include "animations.h"

class AnSolidColor : public AnIfc {

public:

  AnSolidColor(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() {return "SolidColor"; }

  void paint() {
    for(int i=0; i<numLeds(); i++) {
      ledsArray[i] = m_anGlobalParams.m_leadingColor;
    }
  }

};

#endif // __AN_SOLID_COLOR_H__
