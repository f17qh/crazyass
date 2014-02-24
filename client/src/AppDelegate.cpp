#include "AppDelegate.h"
#include "game_scene.h"
#include "ui_config_init.h"
#include "dog.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
  DogRunInit();
  UIConfigInit();
  // init user
  User::InitCurrent();

  // initialize director
  CCDirector* pDirector = CCDirector::sharedDirector();
  CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

  pDirector->setOpenGLView(pEGLView);

  // turn on display FPS
  pDirector->setDisplayStats(true);

  // set FPS. the default value is 1.0/60 if you don't call this
  pDirector->setAnimationInterval(1.0 / 60);

  // create a scene. it's an autorelease object
  CCScene *pScene = GameScene::create();

  // run
  pDirector->runWithScene(pScene);

  CCLOG("%s\n", User::current()->id().c_str());
  return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
  CCDirector::sharedDirector()->stopAnimation();

  // if you use SimpleAudioEngine, it must be pause
  // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  CCDirector::sharedDirector()->startAnimation();

  // if you use SimpleAudioEngine, it must resume here
  // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
