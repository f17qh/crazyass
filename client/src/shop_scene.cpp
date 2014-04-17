#include "event_scene.h"
#include "game_scene.h"
#include "common.h"
#include "SimpleAudioEngine.h"
#include "shop_scene.h"
#include "user.h"
#include "loading.h"
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
bool ProductBuy(void *iap, char *, void *);

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

#ifdef CA_ANDROID
static const float productCost[] = {
  0.99f, 5.99f, 9.99f, 30.0f, 99.0f,
};
#endif

extern void PayGooglePlay(const char*, const char*, float);
extern std::string CAGetDeviceID();
void ShopScene::onBtnSale(CCObject *target, TouchEventType e, int i) {
  if (e == TOUCH_EVENT_ENDED) {
#ifdef CA_ANDROID
    PayGooglePlay(CAGetDeviceID().c_str(), productId[i - 1], productCost[i - 1]);
#else
    if (!iap_)
      return;

    if (in_iap_)
      return;

    in_iap_ = ProductBuy(iap_, (char *)productId[i - 1], (void *)this);
    if (in_iap_) {
      TextBox::Instance().Show(ui_layer_, true, "Please waiting...");
    }
#endif
  }
}

void ShopScene::onBtnBack(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  if (in_iap_)
    return;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}

void ShopScene::DisableTextBox() {
  TextBox& box = TextBox::Instance();
  box.Show(ui_layer_, false);
}

void CAProductByNotify(char *name, void *target) {
  CCLOG("%s: %s", __FUNCTION__, name);
  in_iap_ = false;
  ShopScene *ss = (ShopScene *)target;
  if (ss) {
    ss->DisableTextBox();
  }
  if (name == NULL)
    return;

  int addheart = 0;
  for (int i = 0; i < sizeof(productId)/sizeof(productId[0]); i++) {
    if (strcmp(name, productId[i]) == 0) {
      addheart = productHeart[i];
      User::CurrentUser()->set_heart(
        User::CurrentUser()->heart() + productHeart[i]);
    }
  }

  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}
