#include "game_scene.h"
#include "start_scene.h"
#include "cocos-ext.h"
#include "user.h"
#include "lib_json/json_lib.h"
#include "common.h"
#include "loading.h"
#include "net_web_socket.h"
#include "static_config.h"
USING_NS_CC_EXT;

bool StartScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  return true;
}

void StartScene::EnableBtnPlay() {
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(StartScene::onBtnPlay));
    btn->setPressedActionEnabled(true);
    CCScaleTo* scale1 = CCScaleTo::create( 1.0f, 1.2f);
    CCScaleTo* scale2 = CCScaleTo::create( 1.0f, 0.8f);
    CCSequence* scale = CCSequence::createWithTwoActions(scale1, scale2);
    btn->getVirtualRenderer()->runAction(CCRepeatForever::create(scale));
  }
}

void StartScene::onEnter() {
  CCScene::onEnter();
  //gotostartscene = false;
  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/StartScene.json"));
  ui_layer_->addWidget(layout);

  this->addChild(ui_layer_, 0, 100);

  // connect server
  //TextBox::Instance().Show(ui_layer_, true, "Connecting server...");
  //CCLOG("Connecting server...");
  if(!ConfigInfo::Instence().is_local()){
    CAWS::Instance()->Init("ws://106.187.47.129:12345/ca");
  } else {
    EnablePanty = true;
    EnableTapjoy = true;
    EnableSound = true;
  }

  //下面是测试的
  //CAWS::Instance()->Init("ws://10.32.91.155:12346/ca", this);
  EnableBtnPlay();
  //schedule(schedule_selector(StartScene::update), 1, SCHEDULE_TIMEOUT, 1);
}
void StartScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  PLAY_BTNSOUND;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  // CIapControl::getInstance()->RequesetProductList(true);
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
}
