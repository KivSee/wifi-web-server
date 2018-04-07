#ifndef __JSON_COMMON_H__
#define __JSON_COMMON_H__

#include <map>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

class ObjectFromJsonIfc {

public:

  virtual bool setFromJson(const JsonObject &root) = 0;
  virtual bool validateJson(const JsonObject &root) = 0;

};

class ColorConvertor {

public:

  std::map<String, CHSV> colorNameToValue;

  ColorConvertor() {

    colorNameToValue["white"] = CHSV(0, 0, 255);
    colorNameToValue["black"] = CHSV(0, 0, 0);

    colorNameToValue["red"] = CHSV(0, 255, 255);
    colorNameToValue["green"] = CHSV(96, 255, 255);
    colorNameToValue["blue"] = CHSV(160, 255, 255);

    colorNameToValue["purple"] = CHSV(213, 255, 255);
    colorNameToValue["yellow"] = CHSV(40, 255, 255);
    colorNameToValue["turquoise"] = CHSV(110, 255, 255);
    colorNameToValue["pink"] = CHSV(240, 255, 255);
    colorNameToValue["orange"] = CHSV(10, 255, 255);
    colorNameToValue["lime"] = CHSV(60, 255, 255);
    colorNameToValue["cyan"] = CHSV(130, 255, 255);
  }

  bool parseColorFromJson(const JsonVariant &leadingColorJson, CHSV *outColor) const {

    if(leadingColorJson.is<const char *>()) {
      return parseColorFromJsonString(leadingColorJson.as<const char *>(), outColor);
    }

    if(leadingColorJson.is<JsonObject>()) {
      return parseColorAsJsonObject(leadingColorJson.as<const JsonObject &>(), outColor);
    }


    return false;
  }

  bool parseColorAsJsonObject(const JsonObject &jo, CHSV *outColor) const {

    CHSV choosenColor(0, 255, 255);

    for(JsonObject::const_iterator keysIt = jo.begin(); keysIt != jo.end(); ++keysIt) {

      const JsonVariant &valAsVariant = keysIt->value;

      String key = keysIt->key;
      key.toLowerCase();
      if(key == "hue") {
        if(!valAsVariant.is<uint8_t>()) {
          Serial.println("Hue value should be uint8_t and it isn't");
          return false;
        }
        choosenColor.hue = valAsVariant.as<uint8_t>();
      }
      else if(key == "sat" || key == "saturation") {
        if(!valAsVariant.is<uint8_t>()) {
          Serial.println("Saturation value should be uint8_t and it isn't");
          return false;
        }
        choosenColor.sat = valAsVariant.as<uint8_t>();
      }
      else if(key == "val" || key == "value" || key == "brightness") {
        if(!valAsVariant.is<uint8_t>()) {
          Serial.println("Brightness value should be uint8_t and it isn't");
          return false;
        }
        choosenColor.val = valAsVariant.as<uint8_t>();
      }
      else {
        Serial.println(String("unexpected color in json. key '") + keysIt->key + "' is not valid'");
        return false;
      }

    }

    if(outColor != NULL) {
      *outColor = choosenColor;
    }
    return true;
  }

  /*
  if colorAsString represents a valid color, true is returned and color is written to outColor (unless it is NULL).
  if colorAsString is not a valid color string, false is returned and outColor not changed
  */
  bool parseColorFromJsonString(const String &colorAsString, CHSV *outColor) const {
    // for(int i=0; i<256; i++) {
    //   CRGB colorRGB = CHSV(i, 255, 255);
    //   Serial.println(String("HUE = ") + i + " RGB: " + colorRGB.r + " " + colorRGB.g + " " + colorRGB.b );
    // }
    String colorLowerCase(colorAsString);
    colorLowerCase.toLowerCase();

    if(colorLowerCase.startsWith("hsv(") && colorLowerCase.endsWith(")")) {
      String colorsArrayStr = colorLowerCase.substring(4, colorLowerCase.length() - 1);

      int index1 = colorsArrayStr.indexOf(",");
      if(index1 == -1) {
        return false;
      }
      String hueAsStr = colorsArrayStr.substring(0, index1);
      hueAsStr.trim();

      int index2 = colorsArrayStr.indexOf(",", index1 + 1);
      if(index2 == -1) {
        return false;
      }
      String satAsStr = colorsArrayStr.substring(index1 + 1, index2);
      satAsStr.trim();

      String brightAsStr = colorsArrayStr.substring(index2 + 1);
      brightAsStr.trim();

      Serial.println(String("HUE = ") + hueAsStr);
      Serial.println(String("SAT = ") + satAsStr);
      Serial.println(String("BRIGHT = ") + brightAsStr);

      Serial.println(colorsArrayStr);
      return false;
    }

    std::map<String, CHSV>::const_iterator it = colorNameToValue.find(colorLowerCase);
    if(it != colorNameToValue.end()) {
      if(outColor != NULL) {
        *outColor = it->second;
      }
      return true;
    }
    return false;
  }

};
ColorConvertor colorConvertor;

template<typename T>
void readJsonParameter(const char *parameterName, const JsonObject &jo, T *outValue) {
  *outValue = jo[parameterName].as<T>();
  Serial.println(String(parameterName) + " = " + *outValue);
}

void readJsonColorParameter(const char *parameterName, const JsonObject &jo, CHSV *outValue) {
  colorConvertor.parseColorFromJson(jo[parameterName], outValue);
  Serial.println(String(parameterName) + ": hue = " + outValue->hue + " sat: " +
    outValue->sat + " val: " + outValue->val);
}

bool validateColorParameter(const char *parameterName, const JsonObject &jo) {

  if(!jo.containsKey(parameterName)) {
    Serial.println(String("missing key '") + parameterName + "' in json");
    return false;
  }

  const JsonVariant jv = jo[parameterName];
  bool success = colorConvertor.parseColorFromJson(jv, NULL);
  if(!success) {
    Serial.println(String("the value of parameter '") + parameterName + "' has invalid format");
    return false;
  }

  return true;
}

template<typename T>
bool validateParameter(const char *parameterName, const JsonObject &jo) {

  if(!jo.containsKey(parameterName)) {
    Serial.println(String("missing key '") + parameterName + "' in json");
    return false;
  }

  const JsonVariant jv = jo[parameterName];
  if(!jv.is<T>()) {
    Serial.println(String("the value of parameter '") + parameterName + "' has invalid type");
    return false;
  }

  return true;
}

bool loadObjectFromFS(ObjectFromJsonIfc *objToUpdate, const String &path) {

  Serial.println(String("will try to read object data as json from file ") + path);

  if(!SPIFFS.exists(path)) {
    Serial.println(String("file ") + path + " not found in the file system. will not use configuration from persistent storage");
    return false;
  }

  // file open
  File file = SPIFFS.open(path, "r");
  char json[1000];
  file.readBytes(json,1000);
  file.close();
  Serial.println("json data read from file system: ");
  Serial.println(json);

  // convert to json object
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if(!root.success()) {
    Serial.println(String("data in file ") + path + " is not a valid json. will not use data from persistency");
    return false;
  }

  // update the object with the json from the file
  bool success = objToUpdate->setFromJson(root);
  if(!success) {
    Serial.println("json object from file is not in correct format");
    return false;
  }

  Serial.println(String("json data successfully read from file ") + path);
  return true;
}

bool updateObjectFromHttpPost(ESP8266WebServer &server, ObjectFromJsonIfc *objToUpdate) {

  // extract the payload and write to Serial
  String payload = server.arg("plain");
  Serial.println(String("Will try to update uri: ") + server.uri() + " with new json: ");
  Serial.println(payload);

  // extract the file name and write to Serial
  String filename = server.uri();
  Serial.println(String("filename to update: ") + filename);

  // parse the payload as JSON and validate it's ok
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if(!root.success()) {
    Serial.println(String("failed to parse payload as json for uri: ") + server.uri());
    server.send(400, "text/plain", "failed to parse payload as json");
    return false;
  }

  // validate that the json is in the correct format
  if(objToUpdate->validateJson(root) == false) {
    Serial.println(String("json is not valid"));
    server.send(400, "text/plain", "json is not valid");
    return false;
  }

  // write the content to the file on the FS
  File fileToUpdate = SPIFFS.open(filename.c_str(), "w");;
  fileToUpdate.write((uint8_t*)payload.c_str(), payload.length());
  fileToUpdate.close();

  // update the object with the data from the new JSON
  // we do not check success since we validated the json is ok.
  objToUpdate->setFromJson(root);
  server.send(200, "text/plain", "");
  Serial.println("Done updating uri with new json");
  return true;
}


#endif //__JSON_COMMON_H__
