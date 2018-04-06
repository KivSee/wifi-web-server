#ifndef __AN__G_PARAMS_H__
#define __AN__G_PARAMS_H__

#include "jsonCommon.h"
#include <FastLED.h>

class AnimationsGlobalParams : public ObjectFromJsonIfc {

public:

  AnimationsGlobalParams(uint16_t maxPixels)
    :
    m_maxPixels(maxPixels), m_numOfPixels(maxPixels), m_leadingColor(0, 0, 255),
    m_bpm(60)
  {

  }

  const uint16_t m_maxPixels;

  uint16_t m_numOfPixels;
  CHSV m_leadingColor;
  double m_bpm;

public:

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    colorConvertor.parseColorFromJson(root["leadingColor"], &this->m_leadingColor);
    Serial.println(String("leadingColor = "));

    readJsonParameter<>("numOfPixels", root, &this->m_numOfPixels);
    readJsonParameter<>("bpm", root, &this->m_bpm);

    return true;
  }

  bool validateJson(const JsonObject &root) {

    const JsonVariant leadingColor = root["leadingColor"];
    bool leadingColorSuccess = colorConvertor.parseColorFromJson(leadingColor, NULL);
    if(!leadingColorSuccess) {
      Serial.println("leadingColor is not a valid format");
      return false;
    }

    bool success = true;
    success &= validateParameter<uint16_t>("bpm", root);
    success &= validateParameter<uint16_t>("numOfPixels", root);
    if(success) {
      if(root["numOfPixels"].as<uint16_t>() > m_maxPixels) {
        Serial.println(String("GlobalParams::validateJson: numOfPixels is larger than max allowed pixels [") + m_maxPixels + "]");
        return false;
      }
    }
    return success;

  }


};

#endif // __AN__G_PARAMS_H__
