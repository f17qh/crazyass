#include <string>
#include "game_scene.h"
#include "play_scene.h"
#include "event_scene.h"
#include "shop_scene.h"
#include "event_mgr.h"
#include "user.h"
#include "common.h"
#include "loading.h"
#include "net_web_socket.h"

using namespace std;

#ifdef LINUX
#include "unistd.h"
#endif

static void AddSearchPath() {
#ifdef LINUX
  char fullpath[256] = {0};
  ssize_t length = readlink("/proc/self/exe", fullpath, sizeof(fullpath)-1);

  if (length <= 0) {
        return;
  }

  fullpath[length] = '\0';
  std::string appPath = fullpath;
  string path = appPath.substr(0, appPath.find_last_of("/"));
  path += "/../../Resources";
  CCFileUtils::sharedFileUtils()->addSearchPath(path.c_str());
#endif
#ifdef WIN32
  CCFileUtils::sharedFileUtils()->addSearchPath("Resources");
  // CCFileUtils::sharedFileUtils()->addSearchPath("..\\cocostudio\\MainScene\\Export\\MainScene_1\\Resources");
  // CCFileUtils::sharedFileUtils()->addSearchPath("cocostudio/MainScene/Export/MainScene_1/Resources");
#endif
}

extern "C" void tcpc_test();

void CATapPointUse(int p) {
  CCLOG("Use tap point %d", p);
}

bool GameScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;
  return true;
}

void * CATapjoyConnect(char *);
void CATapjoyShow();

static void *tapjoy_ = NULL;

void GameScene::onEnter() {
  //////////////////////////////
  // 1. super init first
  CCScene::onEnter();

  User *u = User::CurrentUser();

  select_stage_ = 1;
  CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
  AddSearchPath();

  const std::vector<std::string>& v = CCFileUtils::sharedFileUtils()->getSearchPaths();
  for (size_t i = 0; i < v.size(); i++) {
    printf("search path %s\n", v[i].c_str());
  }

  ui_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/MainScene_1.json"));
  ui_layer_->addWidget(layout);
  this->addChild(ui_layer_, 0, 100);
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnPlay));
  btn->setPressedActionEnabled(true);
  if (User::CurrentUser()->heart() < 2) {
    //btn->disable();
  } else {
    btn->setBright(true);
    btn->setTouchEnabled(true);
  }

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnEvent");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnEvent));
  btn->setPressedActionEnabled(true);
  if (User::CurrentUser()->stageid() > 1) {
    btn->setVisible(true);
    btn->setTouchEnabled(true);
  } else {
    btn->setVisible(false);
    btn->setTouchEnabled(false);
  }

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnShop");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnShop));
  CCRotateTo* rotate1 = CCRotateTo::create( 1.0f, -15.0f);
  CCRotateTo* rotate2 = CCRotateTo::create( 1.0f, 15.0f);
  CCSequence* rotate = CCSequence::createWithTwoActions(rotate1, rotate2);
  btn->getVirtualRenderer()->runAction(CCRepeatForever::create(rotate));


  btn = (UIButton *)ui_layer_->getWidgetByName("BtnFeedBack");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnFeedback));

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnFree");
  if (EnableTapjoy) {
    btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnFree));
  } else {
    btn->setVisible(false);
    btn->setTouchEnabled(false);
  }

  int nextstage = User::CurrentUser()->stageid();

  AddGirlBtn(1, nextstage, toucheventselector(GameScene::onBtnGirl1));
  AddGirlBtn(2, nextstage, toucheventselector(GameScene::onBtnGirl2));
  AddGirlBtn(3, nextstage, toucheventselector(GameScene::onBtnGirl3));
  AddGirlBtn(4, nextstage, toucheventselector(GameScene::onBtnGirl4));
  AddGirlBtn(5, nextstage, toucheventselector(GameScene::onBtnGirl5));
  AddGirlBtn(6, nextstage, toucheventselector(GameScene::onBtnGirl6));

  // show heart
  UILabelBMFont *l = (UILabelBMFont *)ui_layer_->getWidgetByName("LabelHeart");
  if (l) {
    char b[8];
    snprintf(b, 8, "%d", User::CurrentUser()->heart());
    l->setText(b);
  }

  if (!tapjoy_) {
    tapjoy_ = CATapjoyConnect((char *)User::CurrentUser()->userid().c_str());
  }

  scheduleUpdate();

}

void GameScene::AddGirlBtn(int idx, int nextstage, SEL_TouchEvent selector) {
  char name[RES_MAX_NAME];
  snprintf(name, RES_MAX_NAME, "BtnGirl%d", idx); 
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName(name);
  if (btn) {
    if (idx < nextstage) {
      snprintf(name, RES_MAX_NAME, "stage_select_preview_face_girl%d_open.png", idx);
      btn->loadTextureNormal(name, UI_TEX_TYPE_PLIST);
      btn->loadTexturePressed(name, UI_TEX_TYPE_PLIST);
    }
    btn->addTouchEventListener(this, selector);
    btn->setPressedActionEnabled(true);
  }
  snprintf(name, RES_MAX_NAME, "ImageLock%d", idx); 
  UIImageView *lock = (UIImageView *)ui_layer_->getWidgetByName(name);;
  if (lock) {
    if (idx <= nextstage) {
      lock->setVisible(false);
    }
  }
  //LabelBMFont_1
}

void ShowFeedback();
void CATapjoyShow();
void GameScene::onBtnFeedback(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_ENDED) {
    CCLOG("%s %d\n", __func__, e);
    PLAY_BTNSOUND;
    ShowFeedback();
  }
}

void GameScene::onBtnFree(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  PLAY_BTNSOUND;
  CATapjoyShow();
}

void GameScene::onBtnShop(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  PLAY_BTNSOUND;
  ShopScene *sc = ShopScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
}

void GameScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  PLAY_BTNSOUND;

  CCLOG("%s\n", __FUNCTION__);

  int need_arry[] = {2,3,4,5,6,7};
  if(User::CurrentUser()->heart() < need_arry[select_stage_-1]) {
    PopRecharge::Instance().Show(ui_layer_, this, 
      toucheventselector(GameScene::RechargeBack), toucheventselector(GameScene::RechargeShop));
    return;
  }
  play_ = 0;
  if (User::CurrentUser()->UseHeart(need_arry[select_stage_-1]))
    return;
  User::CurrentUser()->Flush();
  PlayScene *sc = PlayScene::create();
  sc->set_stageid(select_stage_);
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
}

void GameScene::RechargeBack(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  PopRecharge::Instance().Disappear(ui_layer_);
}
void GameScene::RechargeShop(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_BEGAN)
    return;
  PLAY_BTNSOUND;
  ShopScene *sc = ShopScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
}

void GameScene::onBtnEvent(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  PLAY_BTNSOUND;
  CCLOG("%s\n", __FUNCTION__);
  EventScene *sc = EventScene::create();
  int girl_id = select_stage_;
  if(girl_id == 3) {
    girl_id = 6;
  } else if (girl_id == 6) {
    girl_id = 3;
  }
  sc->set_stageid(girl_id);
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
  // CCDirector::sharedDirector()->pushScene(sc);
}

void GameScene::OnBtnGirl(CCObject *target, TouchEventType e, int i) {
  if (e != TOUCH_EVENT_ENDED) 
    return; 
  int girl_id = i;
  if(girl_id == 3) {
    girl_id = 6;
  } else if (girl_id == 6) {
    girl_id = 3;
  }
  CCLOG("%s", __FUNCTION__); 

  UIImageView *preview = (UIImageView *)ui_layer_->getWidgetByName("ImgGirlPreview"); 
  if (!preview) 
    return; 
  PLAY_BTNSOUND;
  char name[RES_MAX_NAME];
  snprintf(name, RES_MAX_NAME, "stage_select_preview_girl%d.png", girl_id); 
  preview->loadTexture(name, UI_TEX_TYPE_PLIST); 
  select_stage_ = i;

  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnEvent"); 
  if (btn) {
    if (i >= User::CurrentUser()->stageid()) {
      btn->setVisible(false);
      btn->setTouchEnabled(false);
    } else {
      btn->setVisible(true);
      btn->setTouchEnabled(true);
    }
  }

  UILabelBMFont *font = (UILabelBMFont *)ui_layer_->getWidgetByName("FontWindowHeart");
  if (font) {
    char b[8];
    snprintf(b, 8, "X%d", i+1);
    font->setText(b);
  }

  font = (UILabelBMFont *)ui_layer_->getWidgetByName("FontDescHeat");
  if (font) {
    char b[8];
    snprintf(b, 8, "%d", i+1);
    font->setText(b);
  }
  //btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  //if (btn) {
  //  if (i > User::CurrentUser()->stageid() ||
  //      i + 1 > User::CurrentUser()->heart()) {
  //    btn->disable();
  //  } else {
  //    btn->setBright(true);
  //    btn->setTouchEnabled(true);
  //  }
  //}
}

#define BUILD_BTNGIRLFUNC(i) \
  void GameScene::onBtnGirl##i(CCObject *target, TouchEventType e) { \
  OnBtnGirl(target, e, i);\
}

BUILD_BTNGIRLFUNC(1);
BUILD_BTNGIRLFUNC(2);
BUILD_BTNGIRLFUNC(3);
BUILD_BTNGIRLFUNC(4);
BUILD_BTNGIRLFUNC(5);
BUILD_BTNGIRLFUNC(6);

void GameScene::menuCloseCallback(CCObject* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
  CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
  CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
#endif
}

extern "C" void CASetUserHeart(int amount) {
  User::CurrentUser()->set_heart(
    User::CurrentUser()->heart() + amount);
}
