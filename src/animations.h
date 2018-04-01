#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

#include "FastLED.h"
#include "an/gParams.h"

class AnIfc {

public:

  AnIfc(CHSV *ledsArray, uint16_t numLeds, const AnimationsGlobalParams &anGlobalParams)
    : m_anGlobalParams(anGlobalParams)
  {
    this->ledsArray = ledsArray;
    this->numLeds = numLeds;
  }

  virtual String getName() = 0;

  virtual void paint() = 0;

protected:

  CHSV *ledsArray;
  uint16_t numLeds;

  const AnimationsGlobalParams &m_anGlobalParams;
};


#endif // __ANIMATIONS_H__
