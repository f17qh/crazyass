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
    t.env->DeleteLocalRef(t.classID);
  } else {
    CCLOG("cannot find feedback\n");
  }
}

void ShowFeedback() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/crazyass/game/crazyass", "startFeedBack", "()V")) {
    CCLOG("find feedback\n");
    t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  } else {
    CCLOG("cannot find feedback\n");
  }
};

int CAGetTapjoyPoint() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/crazyass/game/crazyass", "GetTapjoyPoint", "()I")) {
    jint ret = (jint)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
    return ret;
  } else {
    CCLOG("cannot find feedback\n");
    return 0;
  }
}

void CASetTapjoyPoint(int amount) {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/crazyass/game/crazyass", "SetTapjoyPoint", "(I)V")) {
    t.env->CallStaticObjectMethod(t.classID, t.methodID, amount);
    t.env->DeleteLocalRef(t.classID);
  } else {
    CCLOG("cannot find feedback\n");
    return;
  }
}

void PayTaobao() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/crazyass/game/crazyass", "payTaobao", "()V")) {
    CCLOG("find pay\n");
    t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  } else {
    CCLOG("cannot find feedback\n");
  }
};
