#include "event_scene.h"
#include "game_scene.h"
#include "common.h"
#include "user.h"
#include "SimpleAudioEngine.h"
#include "loading.h"
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
  memset(event_state_, 0, sizeof(event_state_));
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
  };

  for (size_t i = 0; i < sizeof(bl) / sizeof(bl[0]); i++) {
      btn = (UIButton *)ui_layer_->getWidgetByName(bl[i].name);
      if (btn) {
        btn->addTouchEventListener(this, bl[i].selector);
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
}

void EventScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}

void EventScene::onBtnShirt(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

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
    start_point_ = btn_star_field->getTouchStartPos();
    img->setVisible(true);
    img->setPosition(btn_star_field->getTouchStartPos());

    return;
  } else if (e == TOUCH_EVENT_ENDED) {
    img->setVisible(false);
  }
}

void EventScene::onBtnMoveStar(CCObject *target) {
  CCLOG("%s\n", __FUNCTION__);
  UIButton* btn_star_field = (UIButton *)ui_layer_->getWidgetByName("BtnStarField");
  UIImageView* img = (UIImageView *)ui_layer_->getWidgetByName("ImgStar");
  if (!img ||!btn_star_field) {
    CCLOG("%s error btn or btn_move is null\n", __FUNCTION__);
    return;
  }
  const CCPoint& move_pos = btn_star_field->getTouchMovePos();
  if(btn_star_field->getRect().containsPoint(move_pos)) {
    img->setPosition(move_pos);
    //distence_ += move_pos.getDistanceSq(start_point_);
    distence_++;
    CCLOG("%s distence:%f\n", __FUNCTION__, distence_);
    ShowLoadingBar();
  } else {
    img->setVisible(false);
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

  if (i > User::CurrentUser()->EventLock(stageid_))
    return;

  char name[RES_MAX_NAME];
  const char *resfmt = "gallery_button_gallery_0%d_0%d.png";
  CCLOG("%s\n", __FUNCTION__);

  snprintf(name, RES_MAX_NAME, "BtnEvent%d", i);
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName(name);
  event_state_[i - 1] = !event_state_[i - 1];
  int resid = event_state_[i - 1] ? 1 : 2;
  snprintf(name, RES_MAX_NAME, resfmt, i, resid);
  btn->loadTextures(name, name, NULL, UI_TEX_TYPE_PLIST);
  if(event_state_[i - 1]) {
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStarField");
    btn->setVisible(true);
    btn->addTouchEventListener(this, toucheventselector(EventScene::onBtnStarField));
    btn->addMoveEvent(this, coco_moveselector(EventScene::onBtnMoveStar));
    /*UIImageView* img_bg = (UIImageView *)ui_layer_->getWidgetByName("ImgBarGround");
    img_bg->setVisible(true);*/
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_1");
    btn->setVisible(true);
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_2");
    btn->setVisible(true);
  } else {
    //UIImageView* img = (UIImageView *)ui_layer_->getWidgetByName("ImgStar");
    //img->setVisible(true);
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStarField");
    btn->setVisible(false);
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_1");
    btn->setVisible(false);
    btn = (UIButton *)ui_layer_->getWidgetByName("BtnStar1_2");
    btn->setVisible(false);
  }

  //const char *soundfiles[]= {
  //  "sound/sfx_girl_event_start.caf",
  //  "sound/sfx_girl_event_climax1.wav",
  //  "sound/sfx_girl_event_climax2.wav",
  //  "sound/sfx_girl_event_climax3.wav",
  //};

  //// play
  //if (event_state_[i - 1])
  //  CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundfiles[i - 1]);
}

void EventScene::ShowLoadingBar() {
  bool show = false;
  if(distence_ > 0.0f)
    show = true;

  UIImageView* img_bg = (UIImageView *)ui_layer_->getWidgetByName("ImageBarGround");
  UIImageView* img_bar = (UIImageView *)ui_layer_->getWidgetByName("ImageBar");
  UILoadingBar* loading_bar = (UILoadingBar *)ui_layer_->getWidgetByName("LoadingBar");

  if(show && !img_bg->isVisible() && !img_bar->isVisible() && !loading_bar->isVisible()) {
    img_bg->setVisible(true);
    img_bar->setVisible(true);
    loading_bar->setVisible(true);
  }

  if(!show) {
    img_bg->setVisible(false);
    img_bar->setVisible(false);
    loading_bar->setVisible(false);
    loading_bar->setPercent(0);
  }
  loading_bar->setPercent(distence_/10.0f);
}

