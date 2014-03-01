#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class GameScene :
  public CCScene
{
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();  

  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  // static cocos2d::CCScene* scene();

  void menuCloseCallback(CCObject* pSender);

  CREATE_FUNC(GameScene);

protected:
  void onBtnPlay(CCObject *target, TouchEventType e);
  void onBtnGirl1(CCObject *target, TouchEventType e);
  void onBtnGirl2(CCObject *target, TouchEventType e);
  void onBtnGirl3(CCObject *target, TouchEventType e);
  void onBtnGirl4(CCObject *target, TouchEventType e);
  void onBtnGirl5(CCObject *target, TouchEventType e);
  void onBtnGirl6(CCObject *target, TouchEventType e);
  void AddGirlBtn(int idx, int nextstage, SEL_TouchEvent selector);

  int select_stage_;
  UILayer *ui_layer_;
};

