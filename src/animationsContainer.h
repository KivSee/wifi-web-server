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

  AnimationsContainer(CHSV ledsArray[], const CurrentAnimation &currentAnimation, const AnimationsGlobalParams &anGlobalParams)
      :
    m_currentAnimation(currentAnimation), m_anGlobalParams(anGlobalParams)
  {
    allAnimations.push_back(new AnRainbow(ledsArray, anGlobalParams));
    allAnimations.push_back(new AnConfetti(ledsArray, anGlobalParams));
    allAnimations.push_back(new AnSolidColor(ledsArray, anGlobalParams));

    loadObjectFromFS(allAnimations[1], "/an/Confetti.json");
  }

  bool changeCurrentAnimation() {
    return changeCurrAnimation(m_currentAnimation.m_currentAnimationName);
  }

  bool updateAnimationFromHttpPost(ESP8266WebServer &server) {

    String uri = server.uri();
    if(!uri.startsWith("/an/")) {
      Serial.println("uri should start with '/an/'");
      return false;
    }
    // get rid of the "/an/" and the ".json" at the end
    String animationName = uri.substring(4, uri.length() - 5);

    int index = getAnimationByName(animationName.c_str());
    Serial.println(index);
    if(index < 0) {
      return false;
    }

    return updateObjectFromHttpPost(server, allAnimations[index]);
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

private:

  int currAnIndex = 0;
  std::vector<AnIfc *> allAnimations;

  const CurrentAnimation &m_currentAnimation;
  const AnimationsGlobalParams &m_anGlobalParams;

};


#endif // __ANIMATIONS_CONTAINER_H__
