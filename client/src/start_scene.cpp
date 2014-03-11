#include "game_scene.h"
#include "start_scene.h"
#include "cocos-ext.h"
#include "network/WebSocket.h"

USING_NS_CC_EXT;

class MyDelegate : public WebSocket::Delegate {
public:
  virtual void onOpen(WebSocket* ws) {
    CCLOG("%s", __FUNCTION__);
    sc_->EnableBtnPlay();
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
  StartScene *sc_;
};

bool StartScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  return true;
}

static void ServerConnected(void *arg) {
  StartScene *sc = (StartScene *)arg;
  sc->EnableBtnPlay();
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
#if 0
  // static WebSocket websocket;
  static MyDelegate mydelegate;
  // mydelegate = new MyDelegate;
  WebSocket *ws = new WebSocket();
  ws->init(mydelegate, "ws://echo.websocket.org");
#else
  EnableBtnPlay();
#endif
}

void StartScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
}
