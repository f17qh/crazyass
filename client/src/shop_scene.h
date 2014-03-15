#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class ShopScene :
  public CCScene
{
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();  

  virtual void onEnter();

  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  // static cocos2d::CCScene* scene();

  void menuCloseCallback(CCObject* pSender);
  CREATE_FUNC(ShopScene);

protected:
  UILayer *ui_layer_;
  CCScene *back_scene_;
  void onBtnBack(CCObject *target, TouchEventType e);

  void onBtnSale1(CCObject *target, TouchEventType e);
  void onBtnSale2(CCObject *target, TouchEventType e);
  void onBtnSale3(CCObject *target, TouchEventType e);
  void onBtnSale4(CCObject *target, TouchEventType e);
  void onBtnSale5(CCObject *target, TouchEventType e);
  void onBtnSale(CCObject *target, TouchEventType e, int i);
};

