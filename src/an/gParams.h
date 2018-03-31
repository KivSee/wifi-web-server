#ifndef __AN__G_PARAMS_H__
#define __AN__G_PARAMS_H__

#include "jsonCommon.h"
#include <FastLED.h>

class AnimationsGlobalParams : public ObjectFromJsonIfc {

public:

  AnimationsGlobalParams()
    :
    m_leadingColor(0, 0, 255)
  {

  }

  CHSV m_leadingColor;

public:

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    colorConvertor.parseColorFromJsonString(root["leadingColor"].as<const char *>(), &this->m_leadingColor);
    Serial.println(String("leadingColor = "));

    return true;
  }

  bool validateJson(const JsonObject &root) {

    const JsonVariant leadingColor = root["leadingColor"];
    if(!leadingColor.is<const char *>()) {
      Serial.println("leadingColor field in the json is not string");
      return false;
    }


     if(colorConvertor.parseColorFromJsonString(leadingColor.as<const char *>(), NULL) == false) {
       Serial.println(String("leadingColor field contains value '") + leadingColor.as<const char *>() + "' which is not a valid color");
       return false;
     }

    return true;
  }


};

#endif // __AN__G_PARAMS_H__
