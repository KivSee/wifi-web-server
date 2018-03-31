#ifndef __AN_RAINBOW_H__
#define __AN_RAINBOW_H__

#include "animations.h"


class AnRainbow : public AnIfc {

public:

  AnRainbow(CRGB *ledsArray, uint16_t numLeds, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, numLeds, anGlobalParams)
  { }

  String getName() {return "Rainbow"; }

  void paint() {
    for(int i=0; i < numLeds; i++) {
      hsv2rgb_rainbow(CHSV(i * 2, 255, 255), ledsArray[i]);
//      hsv2rgb_spectrum(CHSV(i * 2, 255, 255), ledsArray[i]);
    }
    //fill_rainbow( ledsArray, numLeds, 0, 7);
  }

};


#endif // __AN_RAINBOW_H__
