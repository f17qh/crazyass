#include "game_scene.h"
#include "event_mgr.h"

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

  CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
  // init plist
  CCSpriteFrameCache *c = CCSpriteFrameCache::sharedSpriteFrameCache();
  CCFileUtils::sharedFileUtils()->addSearchPath("Resources");
  AddSearchPath();

  const std::vector<std::string>& v = CCFileUtils::sharedFileUtils()->getSearchPaths();
  for (size_t i = 0; i < v.size(); i++) {
    printf("search path %s\n", v[i].c_str());
  }

  // c->addSpriteFramesWithFile("dogrun2.plist", "dogrun2.png");

#if 1
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
#if 1
  //tcpc_test();
#endif
  return true;
}

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
