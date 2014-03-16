#include "game_scene.h"
#include "start_scene.h"
#include "cocos-ext.h"
#include "user.h"
#include "network/WebSocket.h"
#include "lib_json/json_lib.h"
#include "common.h"

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
    seq_ = 12345;
    lastseq_ = 0;
  }

  virtual void onOpen(WebSocket* ws) {
    CCLOG("%s", __FUNCTION__);
    if (target_) {
      target_->CAOpen();
    }
  }

  virtual void onClose(WebSocket* ws) {
    CCLOG("%s", __FUNCTION__);
    // if disconnect, goto start scene
    ShouldGotoStart();
  }

  virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    CCLOG("%s", __FUNCTION__);
    ShouldGotoStart();
  }

  virtual void onMessage(WebSocket* ws, const WebSocket::Data& data) {
    CCLOG("%s", __FUNCTION__);
    CCLOG("%s", data.bytes);

    CSJson::Reader reader;
    CSJson::Value result;
    if (!reader.parse(std::string(data.bytes), result, false)) {
      CCLOG("parse %s error", data.bytes);
      return;
    }

    int seq = result.get("Seq", 0).asInt();
    if (seq != lastseq_) {
      CCLOG("invalid seq %d expect %d", seq, lastseq_);
      return;
    }

    if (target_) {
      target_->CARecv(result);
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

  void SendServer(CSJson::Value& value, CATarget *target) {
    value["seq"] = seq_;
    lastseq_ = seq_;
    seq_++;

    CSJson::FastWriter writer;
    std::string content = writer.write(value);
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
  int seq_;
  int lastseq_;
};

class CAWS {
 public:
  static CAWS * Instance();
  static void Release();
  CAWS() {
    delegate_ = new MyDelegate;
    ws_ = new WebSocket;
  }

  ~CAWS() {
    caws_ = NULL;
    delete delegate_;
    delete ws_;
  }

  void Init(const char *addr, CATarget *target) {
    delegate_->Init(ws_, target);
    // close if connect
    if (ws_->getReadyState() == WebSocket::kStateOpen)
      ws_->close();
    bool t = ws_->init(*delegate_, addr);
    if (!t) {
      CCLOG("connec server faild");
    }
  }

  MyDelegate *delegate() {
    return delegate_;
  }

 private:
  static CAWS *caws_;
  MyDelegate *delegate_;
  WebSocket *ws_;
};

CAWS * CAWS::caws_ = NULL;

CAWS * CAWS::Instance() {
  if (caws_)
    return caws_;
  caws_ = new CAWS;
  return caws_;
}

void CAWS::Release() {
  if (caws_) {
    delete caws_;
    caws_ = NULL;
  }
}

static bool gotostartscene = false;
void ShouldGotoStart() {
  gotostartscene = true;
}

bool GotoStartSceneIfError() {
  if (gotostartscene) {
    CCLOG("Goto start scene");
    gotostartscene = false;
    CAWS::Release();
    CCScene *sc = StartScene::create();
    CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
    return true;
  }
  return false;
}

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

CADelegate * sharedDelegate() {
  return CAWS::Instance()->delegate();
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

  // connect server
#ifndef WIN32
  CAWS::Instance()->Init("ws://106.187.47.129:12345/ca", this);
#else
  //下面是测试的
  CAWS::Instance()->Init("ws://10.32.91.155:12346/ca", this);
#endif
}

void StartScene::CAOpen() {
  CSJson::Value value;
  value["userid"] = User::CurrentUser()->userid();
  value["cmd"] = 1;

  sharedDelegate()->SendServer(value, this);
  // scheduleUpdate();
  schedule(schedule_selector(StartScene::update), 1, 10, 1);
}

void StartScene::update(float dt) {
  if (sharedDelegate()->CheckRecv()) {
    // unscheduleUpdate();
    unschedule(schedule_selector(StartScene::update));
  }
}

void StartScene::CARecv(const CSJson::Value& result) {
  if (result.get("ErrCode", -1).asInt() == 0) {
    User *u = User::CurrentUser();
    CSJson::Value body = result["Body"];
    u->set_heart(body.get("Heart", 0).asInt());
    u->set_stageid(body.get("Stageid", 1).asInt());
    //u->set_stageid(6);

    // save new userid
    if (User::CurrentUser()->userid() == "") {
      User::CurrentUser()->set_userid(result.get("Userid", "").asString());
      User::CurrentUser()->Flush();
      CCLOG("save new userid %s", User::CurrentUser()->userid().c_str());
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
  if (GotoStartSceneIfError())
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  // CIapControl::getInstance()->RequesetProductList(true);
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
}
