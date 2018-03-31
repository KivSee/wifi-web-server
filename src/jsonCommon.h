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
    colorNameToValue["red"] = CHSV(0, 255, 255);
    colorNameToValue["green"] = CHSV(96, 255, 255);
    colorNameToValue["blue"] = CHSV(160, 255, 255);
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

bool loadObjectFromFS(ObjectFromJsonIfc *objToUpdate, const String &path) {

  Serial.println(String("will try to read object data as json from file ") + path);

  if(!SPIFFS.exists(path)) {
    Serial.println(String("file ") + path + " not found in the file system. will not use configuration from persistent storage");
    return false;
  }

  // file open
  File file = SPIFFS.open(path, "r");
  char json[200];
  file.readBytes(json,200);
  file.close();
  Serial.println("json data read from file system: ");
  Serial.println(json);

  // convert to json object
  StaticJsonBuffer<200> jsonBuffer;
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
  String filename = server.uri() + ".json";
  Serial.println(String("filename to update: ") + filename);

  // parse the payload as JSON and validate it's ok
  StaticJsonBuffer<200> jsonBuffer;
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
