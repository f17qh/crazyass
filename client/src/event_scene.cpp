#include "event_scene.h"
#include "game_scene.h"
#include "common.h"
#include "user.h"
#include "SimpleAudioEngine.h"
#include "static_config.h"
#include "shop_scene.h"

bool EventScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  stageid_ = 1;
  shirt_state_ = 0;
  panty_state_ = 0;
  stock1_state_ = 0;
  stock2_state_ = 0;
  distence_ = 0.0f;
  event_state_ = -1;
  girl_action_runing_ = -1;
  finger_idx_ = 0;
  emitter_ = NULL;
  return true;
}

void EventScene::onEnter() {
  CCScene::onEnter();
  
  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  char path[1024];
  snprintf(path, 1024, "MainScene/EventScene%d.json",
           stageid_);
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile(path));
  ui_layer_->addWidget(layout);
  this->addChild(ui_layer_, 0, 100);

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }

  struct ButtonListener bl[] = {
      {"BtnEvent1", toucheventselector(EventScene::onBtnEvent1)},
      {"BtnEvent2", toucheventselector(EventScene::onBtnEvent2)},
      {"BtnEvent3", toucheventselector(EventScene::onBtnEvent3)},
      {"BtnEvent4", toucheventselector(EventScene::onBtnEvent4)},
      {"BtnShirt", toucheventselector(EventScene::onBtnShirt)},
      {"BtnPanty", toucheventselector(EventScene::onBtnPanty)},
      {"BtnStock1", toucheventselector(EventScene::onBtnStock1)},
      {"BtnStock2", toucheventselector(EventScene::onBtnStock2)},
      {"Btn1Yes", toucheventselector(EventScene::onBtnYes)},
      {"Btn1No", toucheventselector(EventScene::onBtnNo)},
  };

  for (size_t i = 0; i < sizeof(bl) / sizeof(bl[0]); i++) {
      btn = (UIButton *)ui_layer_->getWidgetByName(bl[i].name);
      if (btn) {
        if(!EnablePanty && i == 5) {
          btn->setVisible(false);
          continue;
        }
        btn->addTouchEventListener(this, bl[i].selector);
        if(i == 8 || i == 9) {
          btn->setPressedActionEnabled(true);
        }
      }
  }

  int lock = User::CurrentUser()->EventLock(stageid_);
  char name[RES_MAX_NAME];
  UIImageView *img;
  for (int i = 2; i <= lock; i++) {
    snprintf(name, RES_MAX_NAME, "ImgLock%d", i);
    img = (UIImageView *)ui_layer_->getWidgetByName(name);
    if (img) {
      img->setVisible(false);
    }
  }

  UIPanel *panel = (UIPanel *)ui_layer_->getWidgetByName("PanelShop");
  if(panel == NULL) {
    return;
  }
  panel->setVisible(false);

}

void EventScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}

void EventScene::onBtnShirt(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  CCLOG("%s\n", __FUNCTION__);
  const char *res1 = "gallery_button_shirt_01.png";
  const char *res2 = "gallery_button_shirt_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgShirt");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnShirt");
  if (img) {
    if (shirt_state_ == 0) {
      img->setVisible(false);
      shirt_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      shirt_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

void EventScene::onBtnPanty(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  const char *res1 = "gallery_button_panty_01.png";
  const char *res2 = "gallery_button_panty_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgPanty");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPanty");
  if (img) {
    if (panty_state_ == 0) {
      img->setVisible(false);
      panty_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      panty_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

void EventScene::onBtnStock1(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  const char *res1 = "gallery_button_stocking_01_01.png";
  const char *res2 = "gallery_button_stocking_01_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgStock1");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnStock1");
  if (img) {
    if (stock1_state_== 0) {
      img->setVisible(false);
      stock1_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      stock1_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

void EventScene::onBtnStock2(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  const char *res1 = "gallery_button_stocking_02_01.png";
  const char *res2 = "gallery_button_stocking_02_02.png";
  UIImageView *img = (UIImageView *)ui_layer_->getWidgetByName("ImgStock2");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnStock2");
  if (img) {
    if (stock2_state_ == 0) {
      img->setVisible(false);
      stock2_state_ = 1;
      btn->loadTextures(res2, res2, NULL, UI_TEX_TYPE_PLIST);
    } else {
      img->setVisible(true);
      stock2_state_ = 0;
      btn->loadTextures(res1, res1, NULL, UI_TEX_TYPE_PLIST);
    }
  }
}

void EventScene::onBtnStarField(CCObject *target, TouchEventType e) {
  UIImageView* img = (UIImageView *)ui_layer_->getWidgetByName("ImgStar");
  UIButton* btn_star_field = (UIButton *)ui_layer_->getWidgetByName("BtnStarField");
  if (e == TOUCH_EVENT_BEGAN) {
    img->setVisible(true);
    img->setPosition(btn_star_field->getTouchStartPos());
    emitter_ = CCParticleSystemQuad::create("particles/star_particle.plist");
    emitter_->setPosition(btn_star_field->getPosition());
    ui_layer_->addChild(emitter_, 1, 1001);
    return;
  } else if (e == TOUCH_EVENT_ENDED) {
    img->setVisible(false);
    ui_layer_->removeChildByTag(1001);
  }
}

void EventScene::onBtnYes(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  if(User::CurrentUser()->heart() >= 12) {
    User::CurrentUser()->UseHeart(12);
    User::CurrentUser()->set_eventlock(stageid_,User::CurrentUser()->EventLock(stageid_) + 1);

    int lock = User::CurrentUser()->EventLock(stageid_);
    char name[RES_MAX_NAME];
    UIImageView *img;
    for (int i = 2; i <= lock; i++) {
      snprintf(name, RES_MAX_NAME, "ImgLock%d", i);
      img = (UIImageView *)ui_layer_->getWidgetByName(name);
      if (img) {
        img->setVisible(false);
      }
    }
    UIPanel *panel = (UIPanel *)ui_layer_->getWidgetByName("PanelShop");
    if(panel == NULL) {
      return;
    }
    panel->setVisible(false);
  } else {
    CCLOG("%s to shop", __FUNCTION__);
    ShopScene *sc = ShopScene::create();
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
  }
}

void EventScene::onBtnNo(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  UIPanel *panel = (UIPanel *)ui_layer_->getWidgetByName("PanelShop");
  if(panel == NULL) {
    return;
  }
  panel->setVisible(false);
}

void EventScene::onBtnMoveStar(CCObject *target) {
  UIButton* btn_star_field = (UIButton *)ui_layer_->getWidgetByName("BtnStarField");
  UIImageView* img = (UIImageView *)ui_layer_->getWidgetByName("ImgStar");
  if (!img ||!btn_star_field) {
    CCLOG("%s error btn or btn_move is null\n", __FUNCTION__);
    return;
  }
  const CCPoint& move_pos = btn_star_field->getTouchMovePos();
  if(btn_star_field->getRect().containsPoint(move_pos)) {
    img->setPosition(move_pos);
    if(emitter_ != NULL) {
      emitter_->setPosition(move_pos);
    }
    if(distence_ > 10000) {
      distence_ = 100*100 + ConfigInfo::Instence().GetEventPL(GetEventStep(),event_state_,stageid_)*10;
    } else {
      int add = ConfigInfo::Instence().GetEventPI(GetEventStep(),event_state_,stageid_);
      distence_ += add;
      if(distence_ > 10000) {
        distence_ = 11000;
      }
    }
    //CCLOG("%s distence:%d, add:%d\n", __FUNCTION__, distence_, add);
  } else {
    img->setVisible(false);
    ui_layer_->removeChildByTag(1001);
  }
}

#define BUILD_BTNEVENT(i) \
void EventScene::onBtnEvent##i(CCObject *target, TouchEventType e) { \
  onBtnEvent(target, e, i); \
}

BUILD_BTNEVENT(1)
BUILD_BTNEVENT(2)
BUILD_BTNEVENT(3)
BUILD_BTNEVENT(4)

void EventScene::onBtnEvent(CCObject *target, TouchEventType e, int i) {
  if (e == TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  if (i > User::CurrentUser()->EventLock(stageid_)) {
    //User::CurrentUser()->set_eventlock(stageid_, i);
    UIPanel *panel = (UIPanel *)ui_layer_->getWidgetByName("PanelShop");
    if(panel == NULL) {
      return;
    }
    panel->setVisible(true);
    //TODO: show
    return;
  }
  finger_idx_ = i;
  char name[RES_MAX_NAME];
  const char *resfmt = "gallery_button_gallery_0%d_0%d.png";
  CCLOG("%s\n", __FUNCTION__);
  snprintf(name, RES_MAX_NAME, "BtnEvent%d", i);
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName(name);
  if(event_state_ == i-1) {
    event_state_ = -1;
  } else {
    event_state_ = i-1;
  }

  int res1id,res2id;
  if(event_state_ >= 0) {
    res1id = 1;
    res2id = 2;
  } else {
    res1id = 2;
    res2id = 2;
  }
  snprintf(name, RES_MAX_NAME, resfmt, i, res1id);
  btn->loadTextures(name, name, NULL, UI_TEX_TYPE_PLIST);

  for(int idx = 1; idx <= 4; idx++) {
    if(idx != i) {
      snprintf(name, RES_MAX_NAME, "BtnEvent%d", idx);
      UIButton *btn = (UIButton *)ui_layer_->getWidgetByName(name);
      snprintf(name, RES_MAX_NAME, resfmt, idx, res2id);
      btn->loadTextures(name, name, NULL, UI_TEX_TYPE_PLIST);
    }
  }
  UIImageView* img_bg = (UIImageView *)ui_layer_->getWidgetByName("ImageBarGround");
  img_bg->setVisible(false);
  UIImageView* img_bar = (UIImageView *)ui_layer_->getWidgetByName("ImageBar");
  img_bar->setVisible(false);
  UIImageView* img_girl = (UIImageView *)ui_layer_->getWidgetByName("ImgGirlFace");
  img_girl->setVisible(false);
  UILoadingBar* loading_bar = (UILoadingBar *)ui_layer_->getWidgetByName("LoadingBar");
  loading_bar->setVisible(false);
  ((CCSprite*)img_girl->getVirtualRenderer())->stopAllActions();
  distence_ = 0;
    girl_action_runing_ = -1;
  loading_bar->setPercent(0);

  if(event_state_ >= 0) {
    int x,y,w,h;
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStarField");
    btn->setVisible(true);
    x = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star_field_x_;
    y = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star_field_y_;
    w = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star_field_w_;
    h = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star_field_h_;
    btn->setPosition(ccp(x,y));
    btn->setSize(CCSize(w,h));
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnStarField));
    btn->addMoveEvent(this, coco_moveselector(EventScene::onBtnMoveStar));

    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_1");
    x = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star1_x_;
    y = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star1_y_;
    btn->setPosition(ccp(x,y));
    btn->setVisible(true);

    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_2");
    x = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star2_x_;
    y = ConfigInfo::Instence().GetEventFingerInfo(event_state_,stageid_).btn_star2_y_;
    btn->setVisible(true);
    btn->setPosition(ccp(x,y));

    schedule(schedule_selector(EventScene::UpdateUI), 0.1f, -1, 0.1f);
  } else {
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStarField");
    btn->setVisible(false);
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_1");
    btn->setVisible(false);
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_2");
    btn->setVisible(false);

    unschedule(schedule_selector(EventScene::UpdateUI));
  }

}

static const char *soundfiles[]= {
  "sound/sfx_girl_event_start.caf",
  "sound/sfx_girl_event_climax1.caf",
  "sound/sfx_girl_event_climax2.caf",
  "sound/sfx_girl_event_climax3.caf",
  "sound/sfx_girl_event_climax4.caf",
};

void EventScene::UpdateUI(float delta) {
  if(distence_ >= 0) {
    ShowLoadingBar();
    int sub = ConfigInfo::Instence().GetEventPL(GetEventStep(),event_state_, stageid_);
    distence_ -= sub;
    //CCLOG("%s distence:%d, sub:%d\n", __FUNCTION__, distence_, sub);
    if(distence_ < 0) {
      distence_ = 0;
    }
  }
}
void EventScene::ShowLoadingBar() {
  bool show = false;
  if(distence_ > 0) {
    show = true;
  } else {
    girl_action_runing_ = -1;
  }

  UIImageView* img_bg = (UIImageView *)ui_layer_->getWidgetByName("ImageBarGround");
  UIImageView* img_bar = (UIImageView *)ui_layer_->getWidgetByName("ImageBar");
  UIImageView* img_girl = (UIImageView *)ui_layer_->getWidgetByName("ImgGirlFace");
  UILoadingBar* loading_bar = (UILoadingBar *)ui_layer_->getWidgetByName("LoadingBar");

  if(!show && img_bg->isVisible() && img_bar->isVisible() && loading_bar->isVisible()) {
    img_girl->getVirtualRenderer()->stopAllActions();
    img_bg->setVisible(false);
    img_bar->setVisible(false);
    img_girl->setVisible(false);
    loading_bar->setVisible(false);
    loading_bar->setPercent(0);
    return;
  }

  if(show && !img_bg->isVisible() && !img_bar->isVisible() && !loading_bar->isVisible()) {
    img_bg->setVisible(true);
    img_bar->setVisible(true);
    img_girl->setVisible(true);
    loading_bar->setVisible(true);
  }

  if(distence_ > 10000) {
    loading_bar->setPercent(100);
  } else {
    loading_bar->setPercent(distence_/100);
  }

  if(show && GetEventStep() != girl_action_runing_) {
    float time = ConfigInfo::Instence().GetEventST(GetEventStep());
    girl_action_runing_ = GetEventStep();
    if(girl_action_runing_ != 2){
      if(EnableSound)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundfiles[0]);
    } else {
      if(EnableSound)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundfiles[finger_idx_]);
    }
    RunGirlAction((CCSprite*)img_girl->getVirtualRenderer(), time);
  }
}

void EventScene::RunGirlAction(CCSprite* sprite, float time) {
  CCScaleTo * scale1 = CCScaleTo::create(time, 1.2f);
  CCScaleTo * scale2 = CCScaleTo::create(time, 1.0f);
  CCSequence* tmp = CCSequence::create(scale1, scale2, NULL);
  char name[32] = {};
  sprintf(name,"girl%d_face_000%d.png", stageid_, girl_action_runing_+1);
  sprite->initWithSpriteFrameName(name);
  sprite->runAction(CCRepeatForever::create(tmp));
}

int EventScene::GetEventStep() {
  int event_step = -1;
  if(distence_ >= 0) {
    if(distence_ < 5000) {
      event_step = 0;
    } else if(distence_ < 8000) {
      event_step = 1;
    } else {
      event_step = 2;
    }
  }
  return event_step;
}
