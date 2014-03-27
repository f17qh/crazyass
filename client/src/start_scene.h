#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include "common.h"
#include "lib_json/json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;

class StartScene :
  public CCScene
{
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();  

  virtual void onEnter();
  void EnableBtnPlay();
  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  // static cocos2d::CCScene* scene();

  void menuCloseCallback(CCObject* pSender);

  CREATE_FUNC(StartScene);

protected:
  void onBtnPlay(CCObject *target, TouchEventType e);
  UILayer *ui_layer_;
};

