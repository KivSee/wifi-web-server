#ifndef __AN_SOLID_COLOR_H__
#define __AN_SOLID_COLOR_H__

#include "animations.h"

class AnSolidColor : public AnIfc {

public:

  AnSolidColor(CRGB *ledsArray, uint16_t numLeds, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, numLeds, anGlobalParams)
  { }

  String getName() {return "SolidColor"; }

  void paint() {
    fill_solid( ledsArray, numLeds, m_anGlobalParams.m_leadingColor);
  }

};

#endif // __AN_SOLID_COLOR_H__
