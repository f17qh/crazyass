#include "play_scene.h"
#include "game_scene.h"
#include "card_manager.h"
#include "user.h"

bool PlayScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  stageid_ = 1;
  step_ = 0;
  return true;
}

void PlayScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  char path[1024];
  snprintf(path, 1024, "MainScene/GirlScene%d.json",
           stageid_);

  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile(path));
  ui_layer_->addWidget(layout);

  card_mgr_.CreateLayer(this);
  this->addChild(ui_layer_, 0, 100);
  this->addChild(card_mgr_.card_layer(), 1, 101);
  card_mgr_.Init(stageid_);

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnStartPlay");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnStartPlay));
    btn->setPressedActionEnabled(true);
  }
  btn = (UIButton *)ui_layer_->getWidgetByName("BtnShirt");
  if (btn) {
    btn->setTouchEnable(false);
  }
  card_mgr_.SetEnable(false);
}

static const char* btn_name [] = {
      "BtnShirt",
      "BtnStock1",
      "BtnStock2",
      "BtnPanty",
  };

void PlayScene::CARecvDone() {
  // CCDirector::sharedDirector()->popScene();
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}

void PlayScene::CARecv(char *data, size_t len) {
  CSJson::Reader reader;
  CSJson::Value result;
  if (!reader.parse(std::string(data), result, false)) {
    CCLOG("parse %s error", data);
    // TODO logout to start scene
    return;
  }
  if (result.get("ErrCode", -1).asInt() == 0) {
    User *u = User::CurrentUser();
    CSJson::Value body = result["Body"];
    u->set_heart(body.get("Heart", 0).asInt());
    u->set_stageid(body.get("Stageid", 1).asInt());
  } else {
    // TODO: logout to start screen
    return;
  }
}

void PlayScene::CARecvTimeout() {
  unschedule(schedule_selector(PlayScene::update));
}

void PlayScene::RunAction(CCPoint pos, const char* name) {
  CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Action/Action.ExportJson");
  CCArmature *armature = CCArmature::create("Action");
  armature->getAnimation()->play(name);
  armature->setPosition(pos);
  this->addChild(armature, 2, 102);
  armature->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(PlayScene::ArmatureCallBack));
}

void PlayScene::TakeOffAction(UIButton* btn) {
  RunAction(btn->getPosition(), "HandTouchAnimation");
  btn->getVirtualRenderer()->runAction(CCBlink::create(1.0f,3));
}

void PlayScene::TakeOff(int step) {
  step_ = step;
  UIButton* btn = NULL;
  if(step_ >= 0 && step_ <= 3)
    btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step_]);
  else 
    return;
  if (btn) {
    btn->setTouchEnable(true);
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnClothes));
    btn->addMoveEvent(this, coco_moveselector(PlayScene::onBtnMoveClothes));
    TakeOffAction(btn);
  }
  return;
}

void PlayScene::onBtnMoveClothes(CCObject *target) {
  UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step_]);
  if (btn) {
    CCPoint move_pos = btn->getTouchMovePos();
    btn->setPosition(move_pos);
  }
  CCLOG("%s\n", __FUNCTION__);
}

void PlayScene::onBtnClothes(CCObject *target, TouchEventType e) {
  UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step_]);
  if (btn) {
    if(e == TOUCH_EVENT_BEGAN) {
      btn_start_pos_ = btn->getPosition();
    } else if(e == TOUCH_EVENT_ENDED) {
      CCPoint pos = btn->getPosition();
      if(btn_start_pos_.fuzzyEquals(pos, 100.0f)) {
        btn->setPosition(btn_start_pos_);
      } else {
        if(step_ < 3) {
          btn->setEnabled(false);
          btn = (UIButton *)ui_layer_->getWidgetByName("BtnStartPlay");
          btn->setEnabled(true); 
        } else {
          User::CurrentUser()->set_stageid(stageid_ + 1);
          User::CurrentUser()->Flush();
          CCScene *sc = GameScene::create();
          CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
        }
      }
    }
  }
  CCLOG("%s\n", __FUNCTION__);
}

void PlayScene::onBtnStartPlay(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;
  UIButton* btn = (UIButton *)ui_layer_->getWidgetByName("BtnStartPlay");
  if (btn) {
    btn->setEnabled(false);
  }
  CCLOG("%s\n", __FUNCTION__);
  card_mgr_.StartSubStage();
}

void PlayScene::SendEndPlay(bool pass) {
  CSJson::Value value;
  value["userid"] = "TestUser";
  value["cmd"] = 3;
  CSJson::Value body;
  body["stageid"] = stageid_;
  body["pass"] = pass;
  value["Body"] = body;

  CSJson::FastWriter writer;
  std::string content = writer.write(value);

  sharedDelegate()->SendServer(content, this);
  schedule(schedule_selector(PlayScene::update), 1, 10, 1);
}

void PlayScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
#if 0
  // CCDirector::sharedDirector()->popScene();
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
#endif
  SendEndPlay(false);
}

void PlayScene::update(float delta) {
  if (sharedDelegate()->CheckRecv()) {
    unschedule(schedule_selector(PlayScene::update));
  }
}

void PlayScene::ArmatureCallBack(CCArmature * armature, MovementEventType e, const char * name) {
  if(e == COMPLETE)
    removeChild(armature);
  return;
}
