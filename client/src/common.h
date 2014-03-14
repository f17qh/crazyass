#ifndef  __DOGRUN2_COMMON__
#define  __DOGRUN2_COMMON__

#include <cocos2d.h>
#include <cocos-ext.h>
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define RES_MAX_NAME  64

struct ButtonListener {
    const char *name;
    SEL_TouchEvent selector;
};

extern void SendServer(const std::string& content, void (*func)(void *, char *, size_t), void *arg);

class CATarget {
public:
  virtual void CARecv(char *data, size_t len) {}
  virtual void CARecvDone() {}
  virtual void CARecvTimeout() {}
  virtual void CAOpen() {}
};

class CADelegate {
public:
  virtual void SendServer(const std::string& content, CATarget *target) {};
  virtual bool CheckRecv() {return true;}
};

extern CADelegate * sharedDelegate();

#define PLAY_BTNSOUND \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/sfx_button_touched.caf");
#define PLAY_WIN \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/sfx_girl_defeat.caf");
#define PLAY_LOSE \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/sfx_player_lose.caf");

#endif
