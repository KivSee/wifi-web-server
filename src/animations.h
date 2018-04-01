#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

#include "FastLED.h"
#include "an/gParams.h"

class AnIfc {

public:

  AnIfc(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : m_anGlobalParams(anGlobalParams)
  {
    this->ledsArray = ledsArray;
  }

  inline uint16_t numLeds() const {
    return m_anGlobalParams.m_numOfPixels;
  }

  virtual String getName() = 0;

  virtual void paint() = 0;

protected:

  CHSV *ledsArray;

  const AnimationsGlobalParams &m_anGlobalParams;
};


#endif // __ANIMATIONS_H__
