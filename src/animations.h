#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

#include "FastLED.h"
#include "an/gParams.h"
#include "jsonCommon.h"

class AnIfc : public ObjectFromJsonIfc {

public:

  AnIfc(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : m_anGlobalParams(anGlobalParams)
  {
    this->ledsArray = ledsArray;
  }

  inline uint16_t numLeds() const {
    return m_anGlobalParams.m_numOfPixels;
  }

  inline uint16_t bpm() const {
    return m_anGlobalParams.m_bpm;
  }

  virtual String getName() const = 0;

  virtual void paint() = 0;

  virtual bool setFromJson(const JsonObject &root) {
    return true;
  }

  virtual bool validateJson(const JsonObject &root) {
    return true;
  }

protected:

  CHSV *ledsArray;

  const AnimationsGlobalParams &m_anGlobalParams;
};


#endif // __ANIMATIONS_H__
