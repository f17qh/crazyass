#include "event_scene.h"
#include "game_scene.h"
#include "common.h"
#include "SimpleAudioEngine.h"
#include "shop_scene.h"

bool ShopScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  return true;
}

void ShopScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/ShopScene.json"));
  ui_layer_->addWidget(layout);
  this->addChild(ui_layer_, 0, 100);

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(ShopScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }
}

void ShopScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}

