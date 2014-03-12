#ifndef  __DOGRUN2_COMMON__
#define  __DOGRUN2_COMMON__

#include <cocos2d.h>
#include <cocos-ext.h>

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
#endif
