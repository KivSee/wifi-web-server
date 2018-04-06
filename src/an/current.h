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

    readJsonParameter<String>("currentAnimationName", root, &this->m_currentAnimationName);

    return true;
  }

  bool validateJson(const JsonObject &root) {
    bool success = true;
    success &= validateParameter<const char *>("currentAnimationName", root);
    return success;
  }


};

#endif // __AN__CURRENT_H__
