#include "event_scene.h"
#include "game_scene.h"
#include "common.h"
#include "SimpleAudioEngine.h"
#include "shop_scene.h"
#include "user.h"
#include <vector>
#include <string>

bool ShopScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  return true;
}

void * ProductList();
bool ProductBuy(void *iap, char *);

static void *iap_;
static bool in_iap_ = false;

void ShopScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/ShopScene.json"));
  ui_layer_->addWidget(layout);
  this->addChild(ui_layer_, 0, 100);

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(ShopScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }

  struct ButtonListener bl[] = {
      {"BtnSale10", toucheventselector(ShopScene::onBtnSale1)},
      {"BtnSale20", toucheventselector(ShopScene::onBtnSale2)},
      {"BtnSale70", toucheventselector(ShopScene::onBtnSale3)},
      {"BtnSale150", toucheventselector(ShopScene::onBtnSale4)},
      {"BtnSaleSpecial", toucheventselector(ShopScene::onBtnSale5)},
  };

  for (size_t i = 0; i < sizeof(bl) / sizeof(bl[0]); i++) {
      btn = (UIButton *)ui_layer_->getWidgetByName(bl[i].name);
      if (btn) {
	  btn->addTouchEventListener(this, bl[i].selector);
      }
  }

  if (iap_ == NULL) {
    iap_ = ProductList();
    CCLOG("init iap %p\n", iap_);
  }
}

#define BUILD_BTNBUY(i) \
void ShopScene::onBtnSale##i(CCObject *target, TouchEventType e) { \
  onBtnSale(target, e, i); \
}

BUILD_BTNBUY(1)
BUILD_BTNBUY(2)
BUILD_BTNBUY(3)
BUILD_BTNBUY(4)
BUILD_BTNBUY(5)

const char *productId[]= {
  "com.qfkj.crazyass.peach5",
  "com.qfkj.crazyass.peach30",
  "com.qfkj.crazyass.peach60",
  "com.qfkj.crazyass.peach200",
  "com.qfkj.crazyass.peach700",
};

const int productHeart[] = {
  5,30,60,200,700,
};

void ShopScene::onBtnSale(CCObject *target, TouchEventType e, int i) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  if (!iap_)
    return;

  if (in_iap_)
    return;

  in_iap_ = ProductBuy(iap_, (char *)productId[i - 1]);
}

void ShopScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  if (in_iap_)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}

void CAProductByNotify(char *name) {
  CCLOG("%s: %s", __FUNCTION__, name);
  in_iap_ = false;
  if (name == NULL)
    return;

  for (int i = 0; i < sizeof(productId)/sizeof(productId[0]); i++) {
    if (strcmp(name, productId[i]) == 0) {
      User::CurrentUser()->set_heart(
        User::CurrentUser()->heart() + productHeart[i]);
    }
  }

  // TODO: notify server
}
