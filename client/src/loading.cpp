#include "loading.h"
#include "play_scene.h"
#include "game_scene.h"

void Loading::Init() {
  
  CCLOG("%s", __FUNCTION__);
  // load ui
  load_layer_ = UILayer::create();

  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/LoadScene.json"));
  load_layer_->addWidget(layout);

  

  /*UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnBack");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(PlayScene::onBtnBack));
    btn->setPressedActionEnabled(true);
  }*/
}

void Loading::ShowLoadScene(CCNode* p, bool visible) {
  p->addChild(load_layer_, 20, 99);
  load_layer_->setVisible(visible);
}
//void Loading::CARecvDone() {
//  // CCDirector::sharedDirector()->popScene();
//  CCScene *sc = GameScene::create();
//  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5, sc));
//}


