#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class LoadingScene :
{
public:
  bool init();
  void onEnter();
  CREATE_FUNC(LoadingScene);
protected:
  void ShowLoadScene();
protected:
  UILayer *load_layer_;
};

