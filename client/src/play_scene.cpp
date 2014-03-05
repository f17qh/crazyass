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

void PlayScene::TakeOff(int step) {
  step_ = step;
  switch(step) {
    case 1: {
        UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step-1]);
        if (btn) {
          btn->setTouchEnable(true);
          btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnClothes));
          btn->addMoveEvent(this, coco_moveselector(PlayScene::onBtnMoveClothes));
        }
    }
    case 2: {
        UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step-1]);
        if (btn) {
          btn->setTouchEnable(true);
          btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnClothes));
          btn->addMoveEvent(this, coco_moveselector(PlayScene::onBtnMoveClothes));
        }
    }
    case 3: {
        UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step-1]);
        if (btn) {
          btn->setTouchEnable(true);
          btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnClothes));
          btn->addMoveEvent(this, coco_moveselector(PlayScene::onBtnMoveClothes));
        }
    }
    case 4: {
        UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step-1]);
        if (btn) {
          btn->setTouchEnable(true);
          btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnClothes));
          btn->addMoveEvent(this, coco_moveselector(PlayScene::onBtnMoveClothes));
        }
    }
  }

}

void PlayScene::onBtnMoveClothes(CCObject *target) {
  UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step_-1]);
  if (btn) {
    CCPoint move_pos = btn->getTouchMovePos();
    btn->setPosition(move_pos);
  }
  CCLOG("%s\n", __FUNCTION__);
}

void PlayScene::onBtnClothes(CCObject *target, TouchEventType e) {
  UIButton* btn = (UIButton *)ui_layer_->getWidgetByName(btn_name[step_-1]);
  if (btn) {
    if(e == TOUCH_EVENT_BEGAN) {
      btn_start_pos_ = btn->getPosition();
    } else if(e == TOUCH_EVENT_ENDED) {
      CCPoint pos = btn->getPosition();
      if(btn_start_pos_.fuzzyEquals(pos, 100.0f)) {
        btn->setPosition(btn_start_pos_);
      } else {
        if(step_ < 4) {
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
  card_mgr_.SetEnable(true);
  card_mgr_.StartAction();
}

void PlayScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}
