#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include "common.h"
#include "lib_json/json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameScene :
  public CCScene
{
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();
  virtual void onEnter();

  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  // static cocos2d::CCScene* scene();

  void menuCloseCallback(CCObject* pSender);

  CREATE_FUNC(GameScene);

protected:
  void onBtnPlay(CCObject *target, TouchEventType e);
  void onBtnEvent(CCObject *target, TouchEventType e);
  void onBtnGirl1(CCObject *target, TouchEventType e);
  void onBtnGirl2(CCObject *target, TouchEventType e);
  void onBtnGirl3(CCObject *target, TouchEventType e);
  void onBtnGirl4(CCObject *target, TouchEventType e);
  void onBtnGirl5(CCObject *target, TouchEventType e);
  void onBtnGirl6(CCObject *target, TouchEventType e);
  void onBtnShop(CCObject *target, TouchEventType e);
  void OnBtnGirl(CCObject *target, TouchEventType e, int i);
  void AddGirlBtn(int idx, int nextstage, SEL_TouchEvent selector);
  void onBtnFeedback(CCObject *target, TouchEventType e);
  void onBtnFree(CCObject *target, TouchEventType e);
  void RechargeBack(CCObject *target, TouchEventType e);
  void RechargeShop(CCObject *target, TouchEventType e);
  int select_stage_;
  int play_;
  UILayer *ui_layer_;
};

