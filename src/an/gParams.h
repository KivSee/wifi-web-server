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

    colorConvertor.parseColorFromJson(root["leadingColor"], &this->m_leadingColor);
    Serial.println(String("leadingColor = "));

    return true;
  }

  bool validateJson(const JsonObject &root) {

    const JsonVariant leadingColor = root["leadingColor"];
    bool leadingColorSuccess = colorConvertor.parseColorFromJson(leadingColor, NULL);
    if(!leadingColorSuccess) {
      Serial.println("leadingColor is not a valid format");
      return false;
    }

    return true;
  }


};

#endif // __AN__G_PARAMS_H__
