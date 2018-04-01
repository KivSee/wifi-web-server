#ifndef __ANIMATIONS_CONTAINER_H__
#define __ANIMATIONS_CONTAINER_H__

#include "animations.h"
#include "an/gParams.h"
#include "an/current.h"

#include "anRainbow.h"
#include "anSolidColor.h"
#include "an/confetti.h"

class AnimationsContainer {

public:

  AnimationsContainer(CHSV ledsArray[], int numOfLeds, const CurrentAnimation &currentAnimation, const AnimationsGlobalParams &anGlobalParams)
      :
    m_currentAnimation(currentAnimation), m_anGlobalParams(anGlobalParams)
  {
    allAnimations.push_back(new AnRainbow(ledsArray, numOfLeds, anGlobalParams));
    allAnimations.push_back(new AnSolidColor(ledsArray, numOfLeds, anGlobalParams));
    allAnimations.push_back(new AnConfetti(ledsArray, numOfLeds, anGlobalParams));
  }

  bool changeCurrentAnimation() {
    return changeCurrAnimation(m_currentAnimation.m_currentAnimationName);
  }

  bool changeCurrAnimation(const String &anName) {
    for(int i=0; i<allAnimations.size(); i++) {
      if(allAnimations[i]->getName() == anName) {
        currAnIndex = i;
        return true;
      }
    }
    return false;
  }

  void paint() {
    allAnimations[currAnIndex]->paint();
  }

private:

  int currAnIndex = 0;
  std::vector<AnIfc *> allAnimations;

  const CurrentAnimation &m_currentAnimation;
  const AnimationsGlobalParams &m_anGlobalParams;

};


#endif // __ANIMATIONS_CONTAINER_H__
