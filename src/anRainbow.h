#ifndef __AN_RAINBOW_H__
#define __AN_RAINBOW_H__

#include "animations.h"


class AnRainbow : public AnIfc {

public:

  AnRainbow(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() {return "Rainbow"; }

  void paint() {
    for(int i=0; i < numLeds(); i++) {
      ledsArray[i] = CHSV(i * 2, 255, 255);
    }
  }

};


#endif // __AN_RAINBOW_H__
