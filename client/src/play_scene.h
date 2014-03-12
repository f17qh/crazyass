#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include "card_manager.h"
#include "common.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PlayScene :
  public CCScene, public CATarget
{
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();  

  virtual void onEnter();

  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  // static cocos2d::CCScene* scene();

  void menuCloseCallback(CCObject* pSender);

  void set_back_scene(CCScene *sc) { back_scene_ = sc; }
  void set_stageid(int id) { stageid_ = id; }
  void TakeOff(int step);
  void CARecv(char *data, size_t len);
  void CARecvDone();
  void CARecvTimeout();
  void update(float delta);

  CREATE_FUNC(PlayScene);
protected:
  void TakeOffAction(UIButton* btn);
  void RunAction(CCPoint pos, const char* name);
  void onBtnBack(CCObject *target, TouchEventType e);
  void onBtnStartPlay(CCObject *target, TouchEventType e);
  void onBtnClothes(CCObject *target, TouchEventType e);
  void onBtnMoveClothes(CCObject *target);
  void ArmatureCallBack(CCArmature * armature, MovementEventType e, const char * name);
  void SendEndPlay(bool);
protected:
  CardMgr card_mgr_;
  UILayer *ui_layer_;
  CCScene *back_scene_;
  int stageid_;
  CCPoint btn_start_pos_;
  int step_;
};

