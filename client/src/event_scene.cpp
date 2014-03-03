#include "event_scene.h"
#include "game_scene.h"

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

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnShirt");
  if (btn) {
    CCLOG("add button BtnEventShirt\n");
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnShirt));
  }
  btn = (UIButton *)ui_layer_->getWidgetByName("BtnPanty");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnPanty));
  }
  btn = (UIButton *)ui_layer_->getWidgetByName("BtnStock1");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnStock1));
  }
  btn = (UIButton *)ui_layer_->getWidgetByName("BtnStock2");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnStock2));
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
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgShirt");
  if (img) {
    if (shirt_state_ == 0) {
      img->setVisible(false);
      shirt_state_ = 1;
    } else {
      img->setVisible(true);
      shirt_state_ = 0;
    }
  }
}

void EventScene::onBtnPanty(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgPanty");
  if (img) {
    if (panty_state_ == 0) {
      img->setVisible(false);
      panty_state_ = 1;
    } else {
      img->setVisible(true);
      panty_state_ = 0;
    }
  }
}

void EventScene::onBtnStock1(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgStock1");
  if (img) {
    if (stock1_state_== 0) {
      img->setVisible(false);
      stock1_state_ = 1;
    } else {
      img->setVisible(true);
      stock1_state_ = 0;
    }
  }
}

void EventScene::onBtnStock2(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgStock2");
  if (img) {
    if (stock2_state_ == 0) {
      img->setVisible(false);
      stock2_state_ = 1;
    } else {
      img->setVisible(true);
      stock2_state_ = 0;
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
  CCLOG("%s\n", __FUNCTION__);
}