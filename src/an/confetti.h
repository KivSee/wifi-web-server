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
    // TODO: use the time since last paint option!
    int pixelsToShine = (totalPixels * m_density) / (densityScaleFactor);
    int reminderPixels = (totalPixels * m_density) % (densityScaleFactor);
    if(random16(densityScaleFactor) < reminderPixels) {
      pixelsToShine++;
    }

    unsigned long currentTime = millis();
    unsigned long diffMilisSinceLastPaint = currentTime - m_lastPaintTime;
    float diffSecondsSinceLastPaint = diffMilisSinceLastPaint / 1000.0f;
    m_lastPaintTime = currentTime;

    double relAmountToDecrease = 255 * (diffSecondsSinceLastPaint / m_fadeTimeSeconds);
    int intAmountToDecrease = (int) relAmountToDecrease;
    if( ((int)(relAmountToDecrease * 256) % 256) > random8()) {
      intAmountToDecrease++;
    }

    for(int i=0; i < numLeds(); i++) {
      ledsArray[i].val = max(0, ledsArray[i].val - intAmountToDecrease);
    }

    for(int i=0; i<pixelsToShine; i++) {
      int pos = random16(numLeds());
      CHSV colorForNewPixel = m_anGlobalParams.m_leadingColor;
      colorForNewPixel.hue += random8(m_hueSpan) - m_hueSpan / 2;
      ledsArray[pos] = colorForNewPixel;
    }
  }

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    readJsonParameter<uint8_t>("density", root, &this->m_density);
    readJsonParameter<uint8_t>("hueSpan", root, &this->m_hueSpan);
    readJsonParameter<double>("fadeTimeSeconds", root, &this->m_fadeTimeSeconds);

    return true;
  }

  bool validateJson(const JsonObject &root) {
    bool success = true;
    success &= validateParameter<uint8_t>("density", root);
    success &= validateParameter<uint8_t>("hueSpan", root);
    success &= validateParameter<double>("fadeTimeSeconds", root);
    return success;
  }

  unsigned long m_lastPaintTime = 0;

  const int densityScaleFactor = 256 * 32;
  uint8_t m_density = 64;
  uint8_t m_hueSpan = 32;
  double m_fadeTimeSeconds = 1.0;

};



#endif // __AN_RAINBOW_H__
