#include "event_scene.h"
#include "game_scene.h"
#include "common.h"

bool EventScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  stageid_ = 1;
  shirt_state_ = 0;
  panty_state_ = 0;
  stock1_state_ = 0;
  stock2_state_ = 0;
  memset(event_state_, 0, sizeof(event_state_));
  return true;
}

void EventScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  char path[1024];
  snprintf(path, 1024, "MainScene/EventScene%d.json",
           stageid_);
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile(path));
  ui_layer_->addWidget(layout);
  this->addChild(ui_layer_, 0, 100);

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }

  struct ButtonListener bl[] = {
      {"BtnEvent1", toucheventselector(EventScene::onBtnEvent1)},
      {"BtnEvent2", toucheventselector(EventScene::onBtnEvent2)},
      {"BtnEvent3", toucheventselector(EventScene::onBtnEvent3)},
      {"BtnEvent4", toucheventselector(EventScene::onBtnEvent4)},
      {"BtnShirt", toucheventselector(EventScene::onBtnShirt)},
      {"BtnPanty", toucheventselector(EventScene::onBtnPanty)},
      {"BtnStock1", toucheventselector(EventScene::onBtnStock1)},
      {"BtnStock2", toucheventselector(EventScene::onBtnStock2)},
  };

  for (size_t i = 0; i < sizeof(bl) / sizeof(bl[0]); i++) {
      btn = (UIButton *)ui_layer_->getWidgetByName(bl[i].name);
      if (btn) {
	  btn->addTouchEventListener(this, bl[i].selector);
      }
  }
}

void EventScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}

void EventScene::onBtnShirt(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  const char *res1 = "gallery_button_shirt_01.png";
  const char *res2 = "gallery_button_shirt_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgShirt");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnShirt");
  if (img) {
    if (shirt_state_ == 0) {
      img->setVisible(false);
      shirt_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      shirt_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

void EventScene::onBtnPanty(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  const char *res1 = "gallery_button_panty_01.png";
  const char *res2 = "gallery_button_panty_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgPanty");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPanty");
  if (img) {
    if (panty_state_ == 0) {
      img->setVisible(false);
      panty_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      panty_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

void EventScene::onBtnStock1(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  const char *res1 = "gallery_button_stocking_01_01.png";
  const char *res2 = "gallery_button_stocking_01_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgStock1");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnStock1");
  if (img) {
    if (stock1_state_== 0) {
      img->setVisible(false);
      stock1_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      stock1_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

void EventScene::onBtnStock2(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  const char *res1 = "gallery_button_stocking_02_01.png";
  const char *res2 = "gallery_button_stocking_02_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgStock2");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnStock2");
  if (img) {
    if (stock2_state_ == 0) {
      img->setVisible(false);
      stock2_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      stock2_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

#define BUILD_BTNEVENT(i) \
void EventScene::onBtnEvent##i(CCObject *target, TouchEventType e) { \
  onBtnEvent(target, e, i); \
}

BUILD_BTNEVENT(1)
BUILD_BTNEVENT(2)
BUILD_BTNEVENT(3)
BUILD_BTNEVENT(4)

void EventScene::onBtnEvent(CCObject *target, TouchEventType e, int i) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  char name[RES_MAX_NAME];
  const char *resfmt = "gallery_button_gallery_0%d_0%d.png";
  CCLOG("%s\n", __FUNCTION__);

  snprintf(name, RES_MAX_NAME, "BtnEvent%d", i);
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName(name);
  event_state_[i] = !event_state_[i];
  int resid = event_state_[i] ? 1 : 2;
  snprintf(name, RES_MAX_NAME, resfmt, i, resid);
  btn->loadTextures(name, name, NULL, UI_TEX_TYPE_PLIST);
}
