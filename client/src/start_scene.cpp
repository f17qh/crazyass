#include "game_scene.h"
#include "start_scene.h"
#include "cocos-ext.h"
#include "user.h"
#include "network/WebSocket.h"
#include "lib_json/json_lib.h"
#include "common.h"
#include "CIapControl.h"

USING_NS_CC_EXT;

#if 0
class MyDelegate;
class CAObject : public CCNode {
public:
  CREATE_FUNC(CAObject);

  bool init() {
    CCNode::init();
    return true;
  }

  void CheckRecv(float dt);
  void StartCheck();
  void set_delegate(MyDelegate *d);
   
private:
  MyDelegate *delegate_;
};
#endif

class MyDelegate : public WebSocket::Delegate, public CADelegate {
public:
  void Init(WebSocket *ws, CATarget *target) {
    ws_ = ws;
    target_ = target;
  }

  virtual void onOpen(WebSocket* ws) {
    CCLOG("%s", __FUNCTION__);
    if (target_) {
      target_->CAOpen();
    }
  }

  virtual void onClose(WebSocket* ws) {
    CCLOG("%s", __FUNCTION__);
  }

  virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    CCLOG("%s", __FUNCTION__);
  }

  virtual void onMessage(WebSocket* ws, const WebSocket::Data& data) {
    CCLOG("%s", __FUNCTION__);
    CCLOG("%s", data.bytes);

    if (target_) {
      target_->CARecv(data.bytes, data.len);
      recvflag_ = true;
    }

    return;
  }

  bool CheckRecv() {
    CCLOG("%s", __FUNCTION__);
    if (recvflag_) {
      if (target_)
        target_->CARecvDone();
      return true;
    } else {
      if (++recvcount_ >= 5) {
        target_->CARecvTimeout();
        return true;
      }
    }
    return false;
  }

  void SendServer(const std::string& content, CATarget *target) {
    recvflag_ = false;
    recvcount_ = 0;
    target_ = target;
    ws_->send(content);
  }
private:
  WebSocket *ws_;
  CATarget *target_;
  bool recvflag_;
  int recvcount_;
};

#if 0
void CAObject::CheckRecv(float dt) {
  if (delegate_->CheckRecv())
    unscheduleUpdate();
}

void CAObject::StartCheck() {
  // scheduleOnce(schedule_selector(CAObject::CheckRecv), 1);
  scheduleUpdate();
}

void CAObject::set_delegate(MyDelegate *d) {
  delegate_ = d;
}
#endif

static MyDelegate *mydelegate;
CADelegate * sharedDelegate() {
  return mydelegate;
}

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
  // mydelegate = MyDelegate::create();
  mydelegate = new MyDelegate;
  WebSocket *ws = new WebSocket();
  mydelegate->Init(ws, this);
  ws->init(*mydelegate, "ws://106.187.47.129:12345/ca");
}

void StartScene::CAOpen() {
  CSJson::Value value;
  value["userid"] = User::CurrentUser()->userid();
  value["cmd"] = 1;

  CSJson::FastWriter writer;
  std::string content = writer.write(value);
  sharedDelegate()->SendServer(content, this);
  // scheduleUpdate();
  schedule(schedule_selector(StartScene::update), 1, 10, 1);
}

void StartScene::update(float dt) {
  if (sharedDelegate()->CheckRecv()) {
    // unscheduleUpdate();
    unschedule(schedule_selector(StartScene::update));
  }
}

void StartScene::CARecv(char *data, size_t len) {
  CSJson::Reader reader;
  CSJson::Value result;
  if (!reader.parse(std::string(data), result, false)) {
    CCLOG("parse %s error", data);
    // TODO: logout to start screen
    SetLoginState(2);
    return;
  }
  if (result.get("ErrCode", -1).asInt() == 0) {
    User *u = User::CurrentUser();
    CSJson::Value body = result["Body"];
    u->set_heart(body.get("Heart", 0).asInt());
    u->set_stageid(body.get("Stageid", 1).asInt());

    // save new userid
    if (User::CurrentUser()->userid() == "") {
      User::CurrentUser()->set_userid(result.get("Userid", "").asString());
      User::CurrentUser()->Flush();
    }
  } else {
    // TODO: logout to start screen
    SetLoginState(2);
    return;
  }

  SetLoginState(1);
  return;
}

void StartScene::CARecvDone() {
  // succ
  if (login_state_ == 1) {
    EnableBtnPlay();
  } else if (login_state_ == 2) {
    // show error
  }
}

void StartScene::CARecvTimeout() {
  unschedule(schedule_selector(StartScene::update));
}

void StartScene::SetLoginState(int s) {
  login_state_ = s;
}

void StartScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  PLAY_BTNSOUND;
  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  // CIapControl::getInstance()->RequesetProductList(true);
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
}
