#include <stdio.h>
#include <cocos2d.h>
#include <string>
#include "../platform/android/jni/JniHelper.h"

#undef CA_ZH
#define CA_EN

void CAReadFile(char *file, char *content, size_t) {}
void CAWriteFile(char *file, char *content) {}
void * ProductList() {return NULL;};
bool ProductBuy(void *iap, char *a, void *b) {return false;};
void * CATapjoyConnect(char *a) {return NULL;};

// zh ver
#ifdef CA_ZH
#define JAVA_CLASS_NAME "com/crazyass/game/crazyass"
#endif

#ifdef CA_EN
// en ver
#define JAVA_CLASS_NAME "com/qfighting/hipmania_en/crazyass"
#endif

void CATapjoyShow() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "startTAPOffers", "()V")) {
    CCLOG("find feedback\n");
    t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  } else {
    CCLOG("cannot find feedback\n");
  }
}

void ShowFeedback() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "startFeedBack", "()V")) {
    CCLOG("find feedback\n");
    t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  } else {
    CCLOG("cannot find feedback\n");
  }
};

int CAGetTapjoyPoint() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "GetTapjoyPoint", "()I")) {
    jint ret = (jint)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
    CCLOG("%s %d", __func__, ret);
    return ret;
  } else {
    CCLOG("cannot find feedback\n");
    return 0;
  }
}

std::string CAGetDeviceID() {
  cocos2d::JniMethodInfo t;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "getDeviceID", "()Ljava/lang/String;")) {
    jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
    cocos2d::CCString *ret = new cocos2d::CCString(cocos2d::JniHelper::jstring2string(str).c_str());
    ret->autorelease();
    return ret->m_sString;
  } else {
    CCLOG("cannot find feedback\n");
    return "";
  }
}

void PayGooglePlay(const char *user, const char *item, float cost) {
  cocos2d::JniMethodInfo t;
#if 0
  jobject activity;
  if (cocos2d::JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "getJavaActivity", "()Ljava/lang/Object;")) {
    activity = t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  } else {
    CCLOG("cannot find getJavaActivity");
    return;
  }
#endif
  if (cocos2d::JniHelper::getStaticMethodInfo(t, JAVA_CLASS_NAME, "payGooglePlay", "(Ljava/lang/String;Ljava/lang/String;F)V")) {
    CCLOG("find pay\n");
    jstring userid = t.env->NewStringUTF(user);
    jstring itemid = t.env->NewStringUTF(item);
    t.env->CallStaticVoidMethod(t.classID, t.methodID, userid, itemid, cost);
  } else {
    CCLOG("cannot find payGooglePlay\n");
  }
};

#include <jni.h>
extern "C" void CASetUserHeart(int);

// zh ver
#if 0
extern "C" void Java_com_crazyass_game_ca_CAAddTapjoyPoint(JNIEnv *env, jobject thiz, jint amount) {
  CCLOG("%s %d\n", __func__, amount);
  CASetUserHeart(amount);
}
#endif

// en ver
#if 1
extern "C" void Java_com_qfighting_hipmania_ca_CAAddTapjoyPoint(JNIEnv *env, jobject thiz, jint amount) {
  CCLOG("%s %d\n", __func__, amount);
  CASetUserHeart(amount);
}
#endif
