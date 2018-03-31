#ifndef __AN__CURRENT_H__
#define __AN__CURRENT_H__

#include "jsonCommon.h"

class CurrentAnimation : public ObjectFromJsonIfc {

public:

  String m_currentAnimationName;

public:

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    this->m_currentAnimationName = root["currentAnimationName"].as<const char *>();
    Serial.println(String("current animation name = ") + this->m_currentAnimationName);

    return true;
  }

  bool validateJson(const JsonObject &root) {

    const JsonVariant currAnName = root["currentAnimationName"];
    if(!currAnName.is<const char *>()) {
      Serial.println("CurrentAnimation: currentAnimationName field in the json is not string");
      return false;
    }

    return true;
  }


};

#endif // __AN__CURRENT_H__
