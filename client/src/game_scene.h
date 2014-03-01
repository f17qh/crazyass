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

  #define ADD_GIRLBTN(i) do { \
    char btn_name[32]; \
    snprintf(btn_name, 32, "BtnGirl%d", i); \
    UIButton *btn##i = (UIButton *)ui_layer_->getWidgetByName(btn_name); \
    if (btn##i) { \
      btn##i->addTouchEventListener(this, toucheventselector(GameScene::onBtnGirl##i)); \
      btn##i->setPressedActionEnabled(true);\
    } \
  } while (0)

  #define BUILD_BTNGIRLFUNC(i) \
  void GameScene::onBtnGirl##i(CCObject *target, TouchEventType e) { \
    CCLOG("%s", __FUNCTION__); \
    if (e != TOUCH_EVENT_BEGAN) \
      return; \
    UIImageView *preview = (UIImageView *)ui_layer_->getWidgetByName("ImgGirlPreview"); \
    if (!preview) \
      return; \
    char name[32];\
    snprintf(name, 32, "stage_select_preview_girl%d.png", i); \
    preview->loadTexture(name, UI_TEX_TYPE_PLIST); \
    select_stage_ = i;\
  }

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

