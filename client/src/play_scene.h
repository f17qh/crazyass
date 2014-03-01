#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class PlayScene :
  public CCScene
{
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();  

  virtual void onEnter();

  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  // static cocos2d::CCScene* scene();

  void menuCloseCallback(CCObject* pSender);

  void set_back_scene(CCScene *sc) { back_scene_ = sc; }
  void set_stageid(int id) { stageid_ = id; }
  CREATE_FUNC(PlayScene);

protected:
  UILayer *ui_layer_;
  CCScene *back_scene_;
  CCLayer *card_layer_;
  CCSprite * test_;
  int stageid_;
  void onBtnBack(CCObject *target, TouchEventType e);
  void onBtnStartPlay(CCObject *target, TouchEventType e);
};

