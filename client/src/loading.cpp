#include "loading.h"

static Loading* loading = NULL;
Loading& Loading::Instence() {
  if (loading == NULL) {
    loading = new Loading();
    loading->Init();
  }
  return *loading;
}

void Loading::Init() {
  CCLOG("%s", __FUNCTION__);
  load_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/LoadScene.json"));
  load_layer_->addWidget(layout);
}

void Loading::ShowLoadScene(CCNode* parent, bool visible) {
  if(visible) {
    if(parent->getChildByTag(99) == NULL)
      load_layer_->retain();
      parent->addChild(load_layer_, 21, 99);
  }
  load_layer_->setVisible(visible);
}

static PopWin* pop_win = NULL;
PopWin& PopWin::Instence() {
  if (pop_win == NULL) {
    pop_win = new PopWin();
    pop_win->Init();
  }
  return *pop_win;
}

void PopWin::Init() {
  CCLOG("%s", __FUNCTION__);
  pop_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/PopWin.json"));
  pop_layer_->addWidget(layout);
}

void PopWin::ShowPopScene(CCNode* parent, bool visible, SEL_TouchEvent selector_back) {
  pop_layer_->setVisible(visible);
  pop_layer_->setTouchEnabled(visible);

  if(visible) {
    if(parent->getChildByTag(98) == NULL) {
      pop_layer_->retain();
      parent->addChild(pop_layer_, 20, 98);
      UIButton* btn = (UIButton *)pop_layer_->getWidgetByName("ButtonBack");
      if(btn != NULL) {
        btn->addTouchEventListener(parent, selector_back);
      }
    }
  }
}


static PopLose* pop_lose = NULL;
PopLose& PopLose::Instence() {
  if (pop_lose == NULL) {
    pop_lose = new PopLose();
    pop_lose->Init();
  }
  return *pop_lose;
}

void PopLose::Init() {
  CCLOG("%s", __FUNCTION__);
  pop_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/PopLose.json"));
  pop_layer_->addWidget(layout);
}

void PopLose::ShowPopScene(CCNode* parent, bool visible, SEL_TouchEvent selector_back, SEL_TouchEvent selector_shop) {
  pop_layer_->setVisible(visible);
  pop_layer_->setTouchEnabled(visible);
  if(visible) {
    if(parent->getChildByTag(97) == NULL) {
      pop_layer_->retain();
      parent->addChild(pop_layer_, 20, 97);
      UIButton* btn = (UIButton *)pop_layer_->getWidgetByName("ButtonBack");
      if(btn != NULL) {
        btn->addTouchEventListener(parent, selector_back);
      }

      UIButton* btn_shop = (UIButton *)pop_layer_->getWidgetByName("ButtonShop");
      if(btn_shop != NULL) {
        btn_shop->addTouchEventListener(parent, selector_shop);
      }
    }
  }
}

