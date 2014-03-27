#ifndef  __DOGRUN2_COMMON__
#define  __DOGRUN2_COMMON__

#include <cocos2d.h>
#include <cocos-ext.h>
#include "SimpleAudioEngine.h"
#include "lib_json/json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define RES_MAX_NAME  64

struct ButtonListener {
    const char *name;
    SEL_TouchEvent selector;
};

//extern void SendServer(const std::string& content, void (*func)(void *, char *, size_t), void *arg);

class CATarget {
public:
  virtual void CARecv(const CSJson::Value& value) {}
  virtual void CARecvDone() {}
  virtual void CARecvTimeout() {}
  virtual void CAOpen() {}
};

class CADelegate {
public:
  virtual void SendServer(CSJson::Value& value, CATarget *target) {};
  virtual bool CheckRecv() {return true;}
};

extern CADelegate * sharedDelegate();

extern bool EnablePanty;
extern bool EnableTapjoy;
extern bool EnableSound;

#define PLAY_BTNSOUND \
  if(EnableSound) \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/sfx_button_touched.caf");
#define PLAY_WIN \
  if(EnableSound) \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/sfx_girl_defeat.caf");
#define PLAY_LOSE \
  if(EnableSound) \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/sfx_player_lose.caf");

#define NETWORK_TIMEOUT	10
#define SCHEDULE_TIMEOUT 15
#endif
