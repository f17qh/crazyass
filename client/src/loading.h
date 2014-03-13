#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class Loading
{
public:
  void Init();
  void ShowLoadScene(CCNode*, bool visible);
protected:
  UILayer *load_layer_;
};

