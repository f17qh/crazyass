#include "play_scene.h"
#include "game_scene.h"
#include "shop_scene.h"
#include "card_manager.h"
#include "static_config.h"
#include "user.h"
#include "loading.h"

bool PlayScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  stageid_ = 1;
  return true;
}

void PlayScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  ui_layer_text_ = UILayer::create();
  char path[1024];
  snprintf(path, 1024, "MainScene/GirlScene%d.json",
           stageid_);

  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile(path));
  ui_layer_->addWidget(layout);

  card_mgr_.CreateLayer(this);
  this->addChild(ui_layer_, 0, 100);
  this->addChild(card_mgr_.card_layer(), 1, 101);
  this->addChild(ui_layer_text_, 2, 100);

  card_mgr_.Init(stageid_);

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }

  UIPanel *panel = (UIPanel *)ui_layer_->getWidgetByName("PanelSecond");
  if(panel == NULL) {
    return;
  }
  panel->setVisible(false);
  panel->setTouchEnable(false);

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnStartPlay");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnStartPlay));
    CCScaleTo* scale1 = CCScaleTo::create( 1.0f, 1.2f);
    CCScaleTo* scale2 = CCScaleTo::create( 1.0f, 0.8f);
    CCSequence* scale = CCSequence::createWithTwoActions(scale1, scale2);
    btn->getVirtualRenderer()->runAction(CCRepeatForever::create(scale));
  }
  btn = (UIButton *)ui_layer_->getWidgetByName("BtnShirt");
  if (btn) {
    btn->setTouchEnable(false);
  }
  card_mgr_.SetEnable(false);

  be_back_ = false;
  property_state_ = -1;
}

static const char* btn_name [] = {
      "BtnShirt",
      "BtnStock1",
      "BtnStock2",
      "BtnPanty",
  };

void PlayScene::CARecvDone() {
  if(cmd_ == 3) {
    be_back_ = true;
  } else if(cmd_ == 6) {
    TextBox::Instance().Show(ui_layer_text_, false);
    if(property_state_ == 7) {
      card_mgr_.ShowWrongCard();
    } else {
      ShowProperty(false);
      card_mgr_.ReStartSubStage();
      card_mgr_.StartSubStage();
    }
    property_state_ = -1;
  }
}

void PlayScene::CARecv(const CSJson::Value& result) {
  if (result.get("ErrCode", -1).asInt() != 0) {
    return;
  }
  cmd_ = result["Cmd"].asInt();
  switch(cmd_) {
  case 3: {
      User *u = User::CurrentUser();
      CSJson::Value body = result["Body"];
      u->set_heart(body.get("Heart", 0).asInt());
      u->set_stageid(body.get("Stageid", 1).asInt());
      
    }
    break;
  case 6: {
      User *u = User::CurrentUser();
      CSJson::Value body = result["Body"];
      u->set_heart(body.get("Heart", 0).asInt());
    }
    break;
  default:
    CCLOG("invalid cmd%d", cmd_);
    break;
  }
}

void PlayScene::CARecvTimeout() {
  TextBox::Instance().Show(ui_layer_text_, false);
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

void PlayScene::TakeOff() {
  UIButton* btn = NULL;
  btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[sub_stage_id_]);

  if (btn) {
    btn->setTouchEnable(true);
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnClothes));
    btn->addMoveEvent(this, coco_moveselector(PlayScene::onBtnMoveClothes));
    TakeOffAction(btn);
  }
  CCLOG("%s error sub_stage_id_:%d\n", __FUNCTION__, sub_stage_id_);
  return;
}

void PlayScene::onBtnMoveClothes(CCObject *target) {
  UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[sub_stage_id_]);
  if (btn) {
    CCPoint move_pos = btn->getTouchMovePos();
    btn->setPosition(move_pos);
  }
  CCLOG("%s\n", __FUNCTION__);
}

void PlayScene::onBtnClothes(CCObject *target, TouchEventType e) {
  UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[sub_stage_id_]);
  if (btn) {
    if(e == TOUCH_EVENT_BEGAN) {
      btn_start_pos_ = btn->getPosition();
    } else if(e == TOUCH_EVENT_ENDED) {
      CCPoint pos = btn->getPosition();
      if(btn_start_pos_.fuzzyEquals(pos, 100.0f)) {
        btn->setPosition(btn_start_pos_);
      } else {
        if(!all_finish_) {
          btn->setEnabled(false);
          btn = (UIButton *)ui_layer_->getWidgetByName("BtnStartPlay");
          btn->setEnabled(true); 
        } else {
          btn->setEnabled(false);
          User::CurrentUser()->set_stageid(stageid_ + 1);
          User::CurrentUser()->Flush();
          PopWin::Instance().ShowPopScene(this, true, toucheventselector(PlayScene::onPopBack));
        }
      }
    }
  }
  CCLOG("%s\n", __FUNCTION__);
}

void PlayScene::onPopBack(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;
  if(be_back_) {
    CCScene *sc = GameScene::create();
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
  }
  CCLOG("%s\n", __FUNCTION__);
}

void PlayScene::onPopShop(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;
  ShopScene *sc = ShopScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
  CCLOG("%s\n", __FUNCTION__);
}

void PlayScene::onBtnStartPlay(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;
  PLAY_BTNSOUND;
  if (GotoStartSceneIfError())
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
  value["userid"] = User::CurrentUser()->userid();
  value["cmd"] = 3;
  CSJson::Value body;
  body["stageid"] = stageid_;
  body["pass"] = pass;
  value["Body"] = body;

  sharedDelegate()->SendServer(value, this);
  schedule(schedule_selector(PlayScene::update), 1, SCHEDULE_TIMEOUT, 1);
}

void PlayScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  CCLOG("%s\n", __FUNCTION__);
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
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

void PlayScene::SetResultPanelState(int state) {
  UIPanel *panel = (UIPanel *)ui_layer_->getWidgetByName("PanelSecond");
  if(panel == NULL) {
    return;
  }

  switch(state) {
  case RESULT_PANEL_NORMAL:
    panel->setVisible(false);
    panel->setTouchEnable(false);
    break;
  case RESULT_PANEL_WIN:
    {
      panel->setVisible(true);
      panel->setTouchEnable(true);
      SetIamgeView("WinOrLose", true, "play_banner_win.png");
      char name[32] = {};
      sprintf(name,"girl%d_face_0004.png",stageid_);
      SetIamgeView("WinOrLoseGirl", true, name);
      ShowTips(true, TIPS_TYPE_RESULT_END);
      panel->addTouchEventListener(this, toucheventselector(PlayScene::onPanelSecond));
      break;
    }
  case RESULT_PANEL_LOSE:
    {
      panel->setVisible(true);
      panel->setTouchEnable(true);
      SetIamgeView("WinOrLose", true, "play_banner_lose.png");
      char name[32] = {};
      sprintf(name,"girl%d_face_0005.png",stageid_);
      SetIamgeView("WinOrLoseGirl", true, name);
      ShowTips(true, TIPS_TYPE_RESULT_END);
      panel->addTouchEventListener(this, toucheventselector(PlayScene::onPanelSecond));
      break;
    }
  case RESULT_PANEL_START:
    {
      panel->setVisible(true);
      panel->setTouchEnable(false);
      SetIamgeView("WinOrLose", false);
      SetIamgeView("WinOrLoseGirl", false);
      ShowTips(true, TIPS_TYPE_ACTION_BEGIN);
    }
  default:
    break;
  }
}

void PlayScene::SetIamgeView(const char* name, bool b, const char* imgs_file) {
  UIImageView *imgs = (UIImageView *)ui_layer_->getWidgetByName(name);
    if(imgs == NULL)
      return;
    imgs->setVisible(b);
    if(imgs_file != NULL) {
      ((CCSprite*)imgs->getVirtualRenderer())->initWithSpriteFrameName(imgs_file);
    }
    return;
}

void PlayScene::ShowStartTips(bool visible) {
  if(visible) {
    SetResultPanelState(RESULT_PANEL_START);
  } else {
    SetResultPanelState(RESULT_PANEL_NORMAL);
  }
}

void PlayScene::ShowTips(bool visible, int type) {
  UILabelBMFont *tips = (UILabelBMFont *)ui_layer_->getWidgetByName("Tips");
  if(tips == NULL)
    return;
  UIImageView *imgs = (UIImageView *)ui_layer_->getWidgetByName("ImgTips");
  if(imgs == NULL)
    return;
  imgs->setVisible(visible);
  TipsInfo& config = ConfigInfo::Instence().tips_info();
  if(visible) {
    switch(type) {
    case TIPS_TYPE_ACTION_BEGIN:
      {
        char name[32];
        snprintf(name, 32, "%s", config.sub_stage_begin_);
        tips->setText(name);
        break;
      }
    case TIPS_TYPE_RESULT_END:
      {
        char name[32];
        snprintf(name, 32, "%s", config.sub_stage_end_);
        tips->setText(name);
        break;
      }
    default:
      break;
    }
  }
}

void PlayScene::onPanelSecond(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  SetResultPanelState(RESULT_PANEL_NORMAL);
  if(sub_win_) {
    TakeOff();
  } else {
    //失败
    CCScene *sc = GameScene::create();
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
  }
}

void PlayScene::SubStageEnd(bool all_finish, bool sub_win, int sub_stage_id) {
  sub_win_ = sub_win;
  sub_stage_id_ = sub_stage_id;
  all_finish_ = all_finish;
  /*if(all_finish) {
    return;
  }*/
  
  if(sub_win) {
    PLAY_WIN;
    SetResultPanelState(RESULT_PANEL_WIN);
  } else {
    PLAY_LOSE;
    SetResultPanelState(RESULT_PANEL_LOSE);
  }
  if(all_finish_ && sub_win_)
    SendEndPlay(true);
}
void PlayScene::ShowProperty(bool visiable) {
  if(visiable) {
    char b[8] = {};
    sprintf(b, "%d", stageid_+2);
    Finger::Instance().Show(ui_layer_, this, toucheventselector(PlayScene::onUseFinger));
    Replay::Instance().Show(ui_layer_, this, toucheventselector(PlayScene::onUseReplay), b);
  } else {
    Finger::Instance().Disappear(ui_layer_);
    Replay::Instance().Disappear(ui_layer_);
  }
}

void PlayScene::onUseFinger(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  if(property_state_ > 0 ) {
    CCLOG("%s property is used", __FUNCTION__);
  }

  CSJson::Value value;
  value["userid"] = User::CurrentUser()->userid();
  value["cmd"] = 6;
  CSJson::Value body;
  body["itemid"] = 7;
  value["Body"] = body;
  property_state_ = 7;
  sharedDelegate()->SendServer(value, this);
  schedule(schedule_selector(PlayScene::update), 1, SCHEDULE_TIMEOUT, 1);
  TextBox::Instance().Show(ui_layer_text_, true, "Please waiting...", 200);
}

void PlayScene::onUseReplay(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  if(property_state_ > 0 ) {
    CCLOG("%s property is used", __FUNCTION__);
  }
  CSJson::Value value;
  value["userid"] = User::CurrentUser()->userid();
  value["cmd"] = 6;
  CSJson::Value body;
  body["itemid"] = stageid_;
  value["Body"] = body;
  property_state_ = stageid_;
  sharedDelegate()->SendServer(value, this);
  schedule(schedule_selector(PlayScene::update), 1, SCHEDULE_TIMEOUT, 1);
  TextBox::Instance().Show(ui_layer_text_, true, "Please waiting...", 200);
}
