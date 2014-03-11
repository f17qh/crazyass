#include "game_scene.h"
#include "start_scene.h"
#include "cocos-ext.h"
#include "network/WebSocket.h"
#include "lib_json/json_lib.h"

USING_NS_CC_EXT;

class MyDelegate : public WebSocket::Delegate {
public:
  virtual void onOpen(WebSocket* ws) {
    CCLOG("%s", __FUNCTION__);
    SendLogin(ws);
  }

  virtual void onClose(WebSocket* ws) {
    CCLOG("%s", __FUNCTION__);
  }

  virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    CCLOG("%s", __FUNCTION__);
  }

  virtual void onMessage(WebSocket* ws, const WebSocket::Data& data) {
    CCLOG("%s", __FUNCTION__);
  }

private:
  void SendLogin(WebSocket *ws) {
    CSJson::Value value;
    value["userid"] = "TestUser";
    value["cmd"] = 1;

    CSJson::FastWriter writer;
    std::string content = writer.write(value);
    ws->send(content);
  }

  StartScene *sc_;
};

bool StartScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  login_state_ = 0;
  return true;
}

void StartScene::EnableBtnPlay() {
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(StartScene::onBtnPlay));
    btn->setPressedActionEnabled(true);
  }
}

void StartScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/StartScene.json"));
  ui_layer_->addWidget(layout);

  this->addChild(ui_layer_, 0, 100);

  // static WebSocket websocket;
  static MyDelegate mydelegate;
  // mydelegate = new MyDelegate;
  WebSocket *ws = new WebSocket();
  ws->init(mydelegate, "ws://127.0.0.1:12345/ca");

  schedule(schedule_selector(StartScene::CheckLogin), 1, 2, 1);
}

void StartScene::CheckLogin() {
  // succ
  if (login_state_ == 1) {
    EnableBtnPlay();
  } else if (login_state_ == 2) {
    // show error
  }
}

void StartScene::SetLoginState(int s) {
  login_state_ = s;
}

void StartScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
}
