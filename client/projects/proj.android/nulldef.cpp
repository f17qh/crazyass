#include <stdio.h>
#include <cocos2d.h>
#include "../platform/android/jni/JniHelper.h"

void CAReadFile(char *file, char *content, size_t) {}
void CAWriteFile(char *file, char *content) {}
void * ProductList() {return NULL;};
bool ProductBuy(void *iap, char *a, void *b) {return false;};
void * CATapjoyConnect(char *a) {return NULL;};

void CATapjoyShow() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/crazyass/game/crazyass", "startTAPOffers", "()V")) {
    CCLOG("find feedback\n");
    t.env->CallStaticObjectMethod(t.classID, t.methodID);
  } else {
    CCLOG("cannot find feedback\n");
  }
}

void ShowFeedback() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/crazyass/game/crazyass", "startFeedBack", "()V")) {
    CCLOG("find feedback\n");
    t.env->CallStaticObjectMethod(t.classID, t.methodID);
  } else {
    CCLOG("cannot find feedback\n");
  }
};

void PayTaobao() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/crazyass/game/crazyass", "payTaobao", "()V")) {
    CCLOG("find pay\n");
    t.env->CallStaticObjectMethod(t.classID, t.methodID);
  } else {
    CCLOG("cannot find feedback\n");
  }
};
