#ifndef __ANIMATIONS_CONTAINER_H__
#define __ANIMATIONS_CONTAINER_H__

#include "animations.h"
#include "an/gParams.h"
#include "an/current.h"

#include "an/rainbow.h"
#include "an/anSolidColor.h"
#include "an/confetti.h"
#include "an/blink.h"
#include "an/alternate.h"
#include "an/snake.h"

class AnimationsContainer {

public:

  AnimationsContainer(CHSV ledsArray[], const CurrentAnimation &currentAnimation, const AnimationsGlobalParams &anGlobalParams)
      :
    m_currentAnimation(currentAnimation), m_anGlobalParams(anGlobalParams)
  {
    allAnimations.push_back(new AnRainbow(ledsArray, anGlobalParams));
    allAnimations.push_back(new AnConfetti(ledsArray, anGlobalParams));
    allAnimations.push_back(new AnSolidColor(ledsArray, anGlobalParams));
    allAnimations.push_back(new AnBlink(ledsArray, anGlobalParams));
    allAnimations.push_back(new AnAlternate(ledsArray, anGlobalParams));
    allAnimations.push_back(new AnSnake(ledsArray, anGlobalParams));
  }

  bool changeCurrentAnimation() {
    return changeCurrAnimation(m_currentAnimation.m_currentAnimationName);
  }

  int getAnimationByName(const char * anName) {
    for(int i=0; i<allAnimations.size(); i++) {
      if(allAnimations[i]->getName() == anName) {
        return i;
      }
    }
    return -1;
  }

  bool changeCurrAnimation(const String &anName) {
    int index = getAnimationByName(anName.c_str());
    if(index < 0) {
      return false;
    }
    else {
      currAnIndex = index;
      return true;
    }
  }

  void paint() {
    allAnimations[currAnIndex]->paint();
  }

  void loadInitialJsonsFromFS() {
    for(int i=0; i < allAnimations.size(); i++) {
      AnIfc *currAnimation = allAnimations[i];
      String fileName = "/an/" + currAnimation->getName() + ".json";
      loadObjectFromFS(currAnimation, fileName);
    }
  }

  void mapAnimationToHttpHandler(ESP8266WebServer &server) {
    for(int i=0; i<allAnimations.size(); i++) {
      AnIfc *currAnimation = allAnimations[i];
      String uri = "/an/" + currAnimation->getName() + ".json";
      server.on(uri, HTTP_POST, [&server, currAnimation]() {
          updateObjectFromHttpPost(server, currAnimation);
      } );
    }
  }


private:

  int currAnIndex = 0;
  std::vector<AnIfc *> allAnimations;

  const CurrentAnimation &m_currentAnimation;
  const AnimationsGlobalParams &m_anGlobalParams;

};


#endif // __ANIMATIONS_CONTAINER_H__
