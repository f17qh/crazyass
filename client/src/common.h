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

#endif
