#include <string>
#include "game_scene.h"
#include "play_scene.h"
#include "event_scene.h"
#include "shop_scene.h"
#include "event_mgr.h"
#include "user.h"
#include "common.h"

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

#if 0
#include <cocos-ext.h>
USING_NS_CC_EXT;
class TestObject : public CCObject {
public:
    bool init() {
  return true;
    }

    void Resp(CCHttpClient *client, CCHttpResponse *resp) {
  CCLOG("network response\n");
  CCLOG("resp %s\n", &((*resp->getResponseData())[0]));
  delete this;
    }
};

static void TestNetwork(void) {
    TestObject *to = new(TestObject);
    to->init();

    //CCHttpResponse rsp;
    CCHttpRequest *req = new CCHttpRequest;
    req->setRequestType(CCHttpRequest::kHttpGet);
    req->setUrl("http://127.0.0.1:8081/foo");
    req->setUserData((void *)"HelloWorld");
    req->setResponseCallback(to, httpresponse_selector(TestObject::Resp));

    CCHttpClient *cli = CCHttpClient::getInstance();
    cli->send(req);
}
#endif

extern "C" void tcpc_test();

bool GameScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;
  return true;
}

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

  // c->addSpriteFramesWithFile("dogrun2.plist", "dogrun2.png");

#if 0
  CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

  /////////////////////////////
  // 3. add your codes below...

  // add a label shows "Hello World"
  // create and initialize a label

  CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);

  // position the label on the center of the screen
  pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
    origin.y + visibleSize.height - pLabel->getContentSize().height));

  // add the label as a child to this layer
  this->addChild(pLabel, 1);
#endif
#if 0
  TestNetwork();
#endif

  ui_layer_ = UILayer::create();
  //Layout *layout = ::GUIReader::shareReader()->widgetFromJsonFile("..\cocostudio\MainScene\Export\MainScene_1\MainScene_1.json");
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/MainScene_1.json"));
  ui_layer_->addWidget(layout);
  this->addChild(ui_layer_, 0, 100);
  // UIImageView * lifeBar = (UIImageView *)ul->getWidgetByName("lifeBar");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnPlay));
  btn->setPressedActionEnabled(true);

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnEvent");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnEvent));
  btn->setPressedActionEnabled(true);

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnShop");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnShop));

  int nextstage = User::CurrentUser()->stageid();

#if 0
#define ADD_GIRLBTN(i) do { \
  snprintf(btn_name, 32, "BtnGirl%d", i); \
  UIButton *btn##i = (UIButton *)ui_layer_->getWidgetByName(btn_name); \
  if (btn##i) {\
  btn##i->addTouchEventListener(this, toucheventselector(GameScene::onBtnGirl##i)); \
  btn##i->setPressedActionEnabled(true);\
  }\
  snprintf(btn_name, 32, "ImageLock%d", i); \
  UIImageView *lock##i = (UIImageView *)ui_layer_->getWidgetByName(btn_name); \
  if (lock##i) {\
  if (i <= nextstage) {\
  lock##i->setVisible(false);\
  }\
  }\
  } while (0)

  char btn_name[32];
  ADD_GIRLBTN(1);
  ADD_GIRLBTN(2);
  ADD_GIRLBTN(3);
  ADD_GIRLBTN(4);
  ADD_GIRLBTN(5);
  ADD_GIRLBTN(6);
#endif
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
    // btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnGirl##i));
    btn->addTouchEventListener(this, selector);
    btn->setPressedActionEnabled(true);

#if 0
    // disable anti alias
    CCSprite *s = dynamic_cast<CCSprite*>(btn->getVirtualRenderer());
    s->getTexture()->setAliasTexParameters();
#endif
  }
  snprintf(name, RES_MAX_NAME, "ImageLock%d", idx); 
  UIImageView *lock = (UIImageView *)ui_layer_->getWidgetByName(name);;
  if (lock) {
    if (idx <= nextstage) {
      lock->setVisible(false);
    }
  }
}

void GameScene::onBtnShop(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  ShopScene *sc = ShopScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
}

void GameScene::CARecvTimeout() {
  unschedule(schedule_selector(GameScene::update));
}

void GameScene::CARecvDone() {
  PlayScene *sc = PlayScene::create();
  sc->set_stageid(select_stage_);
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
  // CCDirector::sharedDirector()->pushScene(sc);
}

void GameScene::CARecv(char *data, size_t len) {
  CSJson::Reader reader;
  CSJson::Value result;
  if (!reader.parse(std::string(data), result, false)) {
    CCLOG("parse %s error", data);
    // TODO logout to start scene
    return;
  }
  if (result.get("ErrCode", -1).asInt() == 0) {
    User *u = User::CurrentUser();
    CSJson::Value body = result["Body"];
    u->set_heart(body.get("Heart", 0).asInt());
    u->set_stageid(body.get("Stageid", 1).asInt());
  } else {
    // TODO: logout to start screen
    return;
  }
}

void GameScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
#if 0
  if (User::CurrentUser()->UseHeart(select_stage_))
    return;
  User::CurrentUser()->Flush();
  PlayScene *sc = PlayScene::create();
  sc->set_stageid(select_stage_);
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
  // CCDirector::sharedDirector()->pushScene(sc);
#endif
  play_ = 0;
  CSJson::Value value;
  value["userid"] = "TestUser";
  value["cmd"] = 2;
  CSJson::Value body;
  body["stageid"] = select_stage_;
  value["Body"] = body;

  CSJson::FastWriter writer;
  std::string content = writer.write(value);

  sharedDelegate()->SendServer(content, this);
  schedule(schedule_selector(GameScene::update), 1, 10, 1);
}

void GameScene::update(float delta) {
  if (sharedDelegate()->CheckRecv()) {
    unschedule(schedule_selector(GameScene::update));
  }
}

void GameScene::onBtnEvent(CCObject *target, TouchEventType e) {
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  EventScene *sc = EventScene::create();
  sc->set_stageid(select_stage_);
  CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5, sc));
  // CCDirector::sharedDirector()->pushScene(sc);
}

void GameScene::OnBtnGirl(CCObject *target, TouchEventType e, int i) {
  CCLOG("%s", __FUNCTION__); 
  if (e != TOUCH_EVENT_BEGAN) 
    return; 
  UIImageView *preview = (UIImageView *)ui_layer_->getWidgetByName("ImgGirlPreview"); 
  if (!preview) 
    return; 
  char name[RES_MAX_NAME];
  snprintf(name, RES_MAX_NAME, "stage_select_preview_girl%d.png", i); 
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
  btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  if (btn) {
    if (i > User::CurrentUser()->stageid() ||
        i > User::CurrentUser()->heart()) {
      btn->disable();
    } else {
      btn->setBright(true);
      btn->setTouchEnabled(true);
    }
  }
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
