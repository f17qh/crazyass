#include "game_scene.h"
#include "start_scene.h"

bool StartScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  return true;
}

void StartScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("cocostudio/MainScene/Export/MainScene/StartScene.json"));
  ui_layer_->addWidget(layout);

  this->addChild(ui_layer_, 0, 100);

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(StartScene::onBtnPlay));
    btn->setPressedActionEnabled(true);
  }
}

void StartScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
}
