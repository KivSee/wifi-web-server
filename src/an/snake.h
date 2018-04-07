#ifndef __AN_SNAKE_H__
#define __AN_SNAKE_H__

#include "animations.h"


class AnSnake : public AnIfc {

public:

  AnSnake(CHSV *ledsArray, const AnimationsGlobalParams &anGlobalParams)
    : AnIfc(ledsArray, anGlobalParams)
  { }

  String getName() const {return "Snake"; }

  void paint() {

    float headPos = m_relHeadStart + beat8(bpm()) * (m_relTailEnd - m_relHeadStart) / 255.0f;
    float tailPos = headPos - m_relSnakeSize;

    CHSV leadingColorAsBlack = m_anGlobalParams.m_leadingColor;
    leadingColorAsBlack.val = 0;
    for(int i=0; i<numLeds(); i++) {
      ledsArray[i] = leadingColorAsBlack;
    }

    int headIndex = headPos * numLeds();
    int tailIndex = tailPos * numLeds();

    int loopStartIndex = max(0, tailIndex);
    int loopEndIndex = min((int)numLeds(), headIndex);
    for(int i=loopStartIndex; i < loopEndIndex; i++) {
      int relVal = (i - tailIndex) * 255 / (headIndex - tailIndex);
      ledsArray[i].val = relVal;
    }

    // handle the first pixels (head pixel), so the snake will advance smoothely
    if(loopEndIndex < numLeds() && loopEndIndex >= 0) {
      float headPixelRelBrightness = (headPos * numLeds()) - headIndex;
      ledsArray[loopEndIndex].val = headPixelRelBrightness * 255;
    }
  }

  bool setFromJson(const JsonObject &root) {
    if(validateJson(root) == false) {
      return false;
    }

    readJsonParameter<>("relSnakeSize", root, &this->m_relSnakeSize);
    readJsonParameter<>("relHeadStart", root, &this->m_relHeadStart);
    readJsonParameter<>("relTailEnd", root, &this->m_relTailEnd);

    return true;
  }

  bool validateJson(const JsonObject &root) {
    bool success = true;
    success &= validateParameter<float>("relSnakeSize", root);
    success &= validateParameter<float>("relHeadStart", root);
    success &= validateParameter<float>("relTailEnd", root);
    if(success) {
        if(root["relSnakeSize"].as<double>() <= 0.0) {
          Serial.println("relSnakeSize should be larger than 0");
          return false;
        }
        float relHeadEnd = root["relTailEnd"].as<double>() +
            root["relSnakeSize"].as<double>();
        float relHeadStart = root["relHeadStart"].as<double>();
        if(relHeadEnd <= relHeadStart) {
          Serial.println("config parameters cause snake head to end prior to its start");
          return false;
        }
    }
    return success;
  }

  float m_relSnakeSize = 0.3;
  float m_relHeadStart = 0.0;
  float m_relTailEnd = 1.0;

};



#endif // __AN_SNAKE_H__
