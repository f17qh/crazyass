#include "loading_scene.h"
#include "play_scene.h"
#include "game_scene.h"

bool LoadingScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  return true;
}

void LoadingScene::onEnter() {
  CCScene::onEnter();

  CCLOG("%s", __FUNCTION__);
  // load ui
  load_layer_ = UILayer::create();

  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/LoadScene.json"));
  load_layer_->addWidget(layout);

  this->addChild(load_layer_, 20, 99);
  load_layer_->setVisible(false);

  /*UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }*/
}

void LoadingScene::ShowLoadScene() {
  load_layer_->setVisible(true);
}
void LoadingScene::CARecvDone() {
  // CCDirector::sharedDirector()->popScene();
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
}


