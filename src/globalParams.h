#ifndef __GLOBAL_PARAMS_H__
#define __GLOBAL_PARAMS_H__

#include <ArduinoJson.h>
#include "jsonCommon.h"

class GlobalParams : public ObjectFromJsonIfc {

public:
    double m_globalBrightness;

    GlobalParams()
        :
      m_globalBrightness(0.5)
    {

    }

    bool setFromJson(const JsonObject &root) {
      if(validateJson(root) == false) {
        return false;
      }

      this->m_globalBrightness = root["globalBrightness"].as<double>();
      Serial.println(String("global brightness = ") + this->m_globalBrightness);

      return true;
    }

    bool validateJson(const JsonObject &root) {

      const JsonVariant globalBrightness = root["globalBrightness"];
      if(!globalBrightness.is<double>()) {
        Serial.println("GlobalParams::validateJson: globalBrightness is not double");
        return false;
      }
      double gb = globalBrightness.as<double>();
      if(gb < 0.0 || gb > 1.0) {
        Serial.println("GlobalParams::validateJson: globalBrightness is not in legal range [0.0-1.0]");
        return false;
      }

      return true;
    }

};


#endif //__GLOBAL_PARAMS_H__
