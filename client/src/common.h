#ifndef  __DOGRUN2_COMMON__
#define  __DOGRUN2_COMMON__

#include <cocos2d.h>
#include <cocos-ext.h>
#include "SimpleAudioEngine.h"
#include "lib_json/json_lib.h"
#include "ca_platform.h"

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
extern void ShouldGotoStart();
extern bool GotoStartSceneIfError();

extern bool EnablePanty;
extern bool EnableTapjoy;

#ifdef	CA_ANDROID
#define	BTNSOUND_FILE "sound/sfx_button_touched.ogg"
#define	WINSOUND_FILE "sound/sfx_girl_defeat.ogg"
#define	LOSTSOUND_FILE "sound/sfx_player_lose.ogg"
#else
#define	BTNSOUND_FILE "sound/sfx_button_touched.caf"
#define	WINSOUND_FILE "sound/sfx_girl_defeat.caf"
#define	LOSTSOUND_FILE "sound/sfx_player_lose.caf"
#endif

#define PLAY_BTNSOUND \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(BTNSOUND_FILE);
#define PLAY_WIN \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WINSOUND_FILE);
#define PLAY_LOSE \
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(LOSTSOUND_FILE);

#define	NETWORK_TIMEOUT	10
#define	SCHEDULE_TIMEOUT 15
#endif
