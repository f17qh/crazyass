#include "play_scene.h"
#include "game_scene.h"
#include "card_action.h"

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

  card_layer_ = CCLayer::create();
  // load ui
  ui_layer_ = UILayer::create();
  char path[1024];
  snprintf(path, 1024, "cocostudio/MainScene/Export/GirlScene%d/GirlScene%d.json",
           stageid_, stageid_);
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile(path));
  ui_layer_->addWidget(layout);


  this->addChild(ui_layer_, 0, 100);
  this->addChild(card_layer_, 1, 101);

  test_ = CCSprite::create();
  card_layer_->addChild(test_);

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
}

void PlayScene::onBtnStartPlay(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  CCLOG("%s\n", __FUNCTION__);
  CardAction card_action;
  card_action.init(test_);
  card_action.moveWithParabola(CCPoint(62,93), CCPoint(300,0), 1.0f);
}

void PlayScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}
