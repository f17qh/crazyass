#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class EventScene :
  public CCScene
{
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();  

  virtual void onEnter();

  void Update(float delta);
  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  // static cocos2d::CCScene* scene();

  void menuCloseCallback(CCObject* pSender);
  void set_stageid(int id) { stageid_ = id; }
  CREATE_FUNC(EventScene);

protected:
  UILayer *ui_layer_;
  CCScene *back_scene_;
  int stageid_;
  int shirt_state_;
  int panty_state_;
  int stock1_state_;
  int stock2_state_;
  int event_state_;
  void onBtnBack(CCObject *target, TouchEventType e);
  void onBtnShirt(CCObject *target, TouchEventType e);
  void onBtnPanty(CCObject *target, TouchEventType e);
  void onBtnStock1(CCObject *target, TouchEventType e);
  void onBtnStock2(CCObject *target, TouchEventType e);
  void onBtnEvent1(CCObject *target, TouchEventType e);
  void onBtnEvent2(CCObject *target, TouchEventType e);
  void onBtnEvent3(CCObject *target, TouchEventType e);
  void onBtnEvent4(CCObject *target, TouchEventType e);
  void onBtnEvent(CCObject *target, TouchEventType e, int i);
  void onBtnStarField(CCObject *target, TouchEventType e);
  void onBtnMoveStar(CCObject *target);
  void RunGirlAction(CCSprite* sprite, float time);
  void CreateStarSprite();
  void ShowLoadingBar();
  int GetEventStep();
protected:
  int distence_;
  CCSprite* star_sprite_;
};

