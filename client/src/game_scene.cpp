#include <string>
#include "game_scene.h"
#include "PlayScene.h"
#include "event_mgr.h"

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
  //CCFileUtils::sharedFileUtils()->addSearchPath("Resources");
  // CCFileUtils::sharedFileUtils()->addSearchPath("..\\cocostudio\\MainScene\\Export\\MainScene_1\\Resources");
  CCFileUtils::sharedFileUtils()->addSearchPath("cocostudio/MainScene/Export/MainScene_1/Resources");
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

  select_stage_ = 1;
  CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
  // init plist
  // CCSpriteFrameCache *c = CCSpriteFrameCache::sharedSpriteFrameCache();
  // CCFileUtils::sharedFileUtils()->addSearchPath("Resources");
  // CCFileUtils::sharedFileUtils()->addSearchPath("..\cocostudio\\MainScene\\Export\\MainScene_1\\Resources");
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
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("cocostudio/MainScene/Export/MainScene_1/MainScene_1.json"));
  ui_layer_->addWidget(layout);
  this->addChild(ui_layer_, 0, 100);

  // UIImageView * lifeBar = (UIImageView *)ul->getWidgetByName("lifeBar");
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  btn->addTouchEventListener(this, toucheventselector(GameScene::onBtnPlay));
  btn->setPressedActionEnabled(true);

  btn = (UIButton *)ui_layer_->getWidgetByName("BtnEvent");
  btn->setPressedActionEnabled(true);

#define ADD_GIRLBTN(i) do { \
  snprintf(btn_name, 32, "BtnGirl%d", i); \
  UIButton *btn##i = (UIButton *)ui_layer_->getWidgetByName(btn_name); \
  if (btn##i) \
  btn##i->addTouchEventListener(this, toucheventselector(GameScene::onBtnGirl##i)); \
  btn##i->setPressedActionEnabled(true);\
  } while (0)

  char btn_name[32];
  ADD_GIRLBTN(1);
  ADD_GIRLBTN(2);
  ADD_GIRLBTN(3);
  ADD_GIRLBTN(4);
  ADD_GIRLBTN(5);
  ADD_GIRLBTN(6);
#undef ADD_GIRLBTN

  UILabelAtlas *l = (UILabelAtlas *)ui_layer_->getWidgetByName("LabelHeart");
  if (l)
    l->setStringValue("99");
  return true;
}

void GameScene::onBtnPlay(CCObject *target, TouchEventType e)
{
  if (e == TOUCH_EVENT_BEGAN)
    return;

  CCLOG("%s\n", __FUNCTION__);
  PlayScene *sc = PlayScene::create();
  sc->set_stageid(select_stage_);
  CCDirector::sharedDirector()->replaceScene(sc);
  // CCDirector::sharedDirector()->pushScene(sc);
}

#define BUILD_BTNGIRLFUNC(i) \
  void GameScene::onBtnGirl##i(CCObject *target, TouchEventType e) { \
  CCLOG("%s", __FUNCTION__); \
  if (e != TOUCH_EVENT_BEGAN) \
  return; \
  UIImageView *preview = (UIImageView *)ui_layer_->getWidgetByName("ImgGirlPreview"); \
  if (!preview) \
  return; \
  char name[32];\
  snprintf(name, 32, "stage_select_preview_girl%d.png", i); \
  preview->loadTexture(name, UI_TEX_TYPE_PLIST); \
  select_stage_ = i;\
}

BUILD_BTNGIRLFUNC(1);
BUILD_BTNGIRLFUNC(2);
BUILD_BTNGIRLFUNC(3);
BUILD_BTNGIRLFUNC(4);
BUILD_BTNGIRLFUNC(5);
BUILD_BTNGIRLFUNC(6);
#undef BUILD_BTNGIRLFUNC

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
