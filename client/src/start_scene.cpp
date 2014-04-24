#include "game_scene.h"
#include "start_scene.h"
#include "cocos-ext.h"
#include "user.h"
#include "lib_json/json_lib.h"
#include "common.h"
#include "loading.h"
//#include "net_web_socket.h"
#include "static_config.h"

USING_NS_CC_EXT;

bool StartScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!CCScene::init())
    return false;

  return true;
}

void StartScene::EnableBtnPlay() {
  UIButton *btn = (UIButton *)ui_layer_->getWidgetByName("BtnPlay");
  if (btn) {
    btn->addTouchEventListener(this, toucheventselector(StartScene::onBtnPlay));
    btn->setPressedActionEnabled(true);
    CCScaleTo* scale1 = CCScaleTo::create( 1.0f, 1.2f);
    CCScaleTo* scale2 = CCScaleTo::create( 1.0f, 0.8f);
    CCSequence* scale = CCSequence::createWithTwoActions(scale1, scale2);
    btn->getVirtualRenderer()->runAction(CCRepeatForever::create(scale));
  }
}

void StartScene::onEnter() {
  CCScene::onEnter();
  //gotostartscene = false;
  CCLOG("%s", __FUNCTION__);
  // load ui
  ui_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/StartScene.json"));
  ui_layer_->addWidget(layout);

  this->addChild(ui_layer_, 0, 100);

  // connect server
  SendRequest(this);
  // TextBox::Instance().Show(ui_layer_, true, "Connecting server...");
  // CCLOG("Connecting server...");
  // if(!ConfigInfo::Instence().is_local()){
  //   CAWS::Instance()->Init("ws://106.187.47.129:12345/ca");
  // } else {
  //   EnablePanty = true;
  //   EnableSound = true;
  // }

  //下面是测试的
  //CAWS::Instance()->Init("ws://10.32.91.155:12346/ca", this);
  //EnableBtnPlay();
  //schedule(schedule_selector(StartScene::update), 1, SCHEDULE_TIMEOUT, 1);
}
void StartScene::onBtnPlay(CCObject *target, TouchEventType e) {
  if (e != TOUCH_EVENT_ENDED)
    return;

  PLAY_BTNSOUND;

  CCLOG("%s\n", __FUNCTION__);
  // CCDirector::sharedDirector()->popScene();

  // CIapControl::getInstance()->RequesetProductList(true);
  CCScene *sc = GameScene::create();
  CCDirector::sharedDirector()->replaceScene(CCTransitionTurnOffTiles::create(1, sc));
}

void StartScene::SendRequest(CCObject * sender) {
  TextBox::Instance().Show(ui_layer_, true, "Connecting server...");
  CCLOG("Connecting server...");
  CCHttpRequest* request = new CCHttpRequest();
  // required fields
  request->setUrl("http://appspot.qfighting.com/ass/config.json");
  request->setRequestType(CCHttpRequest::kHttpGet);
  request->setResponseCallback(this, 
    httpresponse_selector(StartScene::onReceive));
  // optional fields
  request->setTag("GET Control");
  CCHttpClient::getInstance()->setTimeoutForRead(NETWORK_TIMEOUT);
  CCHttpClient::getInstance()->send(request);

  // don't forget to release it, pair to new
  request->release();
}

void StartScene::onReceive(CCHttpClient *sender, CCHttpResponse *response) {
  TextBox::Instance().Show(ui_layer_, false);
  CCLOG("receive server");
  if(!response) {
      CCLOG("receive server error, response is NULL");
      return;
  }

  // You can get original request type from: response->request->reqType
  if(0 != strlen(response->getHttpRequest()->getTag())) {
      CCLOG("%s:%s completed", __FUNCTION__, response->getHttpRequest()->getTag());
  }

  int statusCode = response->getResponseCode();
  char statusString[64] = {};
  sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode,
    response->getHttpRequest()->getTag());
  CCLOG("response code: %d", statusCode);

  if(!response->isSucceed()) {
      CCLOG("response failed");
      CCLOG("error buffer: %s", response->getErrorBuffer());
  } else {
    CSJson::Reader reader;
    CSJson::Value result;
    std::vector<char>& temp = *(response->getResponseData());
    std::string result_buff;
    for(int i = 0; i < temp.size(); i++) {
      result_buff.push_back(temp[i]);
    }
    if (!reader.parse(result_buff, result, false)) {
      CCLOG("parse %s error", result_buff.c_str());
    }

    EnablePanty = result.get("Panty", false).asBool();
    EnableSound = result.get("Sound", false).asBool();

    CSJson::Value control;
    CSJson::StyledWriter writer;
    control["Panty"] = EnablePanty;
    control["Sound"] = EnableSound;
    std::string buf = writer.write(control);
    CCLOG("Save Control info %s\n", buf.c_str());
    CCUserDefault::sharedUserDefault()->setStringForKey("control_data", buf);
  }

  {
    CSJson::Reader reader;
    CSJson::Value control;
    std::string buf = CCUserDefault::sharedUserDefault()->getStringForKey("control_data");
    CCLOG("load:%s\n", buf.c_str());

    if (!reader.parse(buf, control, false)) {
      CCLOG("parse error");
      return;
    }

    EnablePanty = control["Panty"].asBool();
    EnableSound = control["Sound"].asBool();
  }
  EnableBtnPlay();
  // dump data
  // std::vector<char> *buffer = response->getResponseData();
  // printf("Http Test, dump data: ");
  // for (unsigned int i = 0; i < buffer->size(); i++)
  // {
  //     printf("%c", (*buffer)[i]);
  // }
  // printf("\n");
}