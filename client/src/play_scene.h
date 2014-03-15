#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>
#include "card_manager.h"
#include "common.h"

USING_NS_CC;
USING_NS_CC_EXT;
enum {
  RESULT_PANEL_NORMAL = 0,
  RESULT_PANEL_WIN = 1,
  RESULT_PANEL_LOSE = 2,
  RESULT_PANEL_START = 3,
};

enum {
  TIPS_TYPE_ACTION_BEGIN = 1,
  TIPS_TYPE_RESULT_END = 2,

};

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
  void SubStageEnd(bool all_finish, bool sub_win, int sub_stage_id);
  void ShowStartTips(bool visible);
  void CARecv(const CSJson::Value&);
  void CARecvDone();
  void CARecvTimeout();
  void update(float delta);
  CREATE_FUNC(PlayScene);
protected:
  void TakeOff();
  void TakeOffAction(UIButton* btn);
  void RunAction(CCPoint pos, const char* name);
  void onBtnBack(CCObject *target, TouchEventType e);
  void onBtnStartPlay(CCObject *target, TouchEventType e);
  void onBtnClothes(CCObject *target, TouchEventType e);
  void onBtnMoveClothes(CCObject *target);
  void onPopBack(CCObject *target, TouchEventType e);
  void onPopShop(CCObject *target, TouchEventType e);
  void ArmatureCallBack(CCArmature * armature, MovementEventType e, const char * name);
  void SendEndPlay(bool);
  void onPanelSecond(CCObject *target, TouchEventType e);
  void SetResultPanelState(int state);
  void SetIamgeView(const char* name, bool b, const char* imgs_file=NULL);
  void ShowTips(bool visible, int type = 0);
protected:
  CardMgr card_mgr_;
  UILayer *ui_layer_;
  CCScene *back_scene_;
  int stageid_;
  CCPoint btn_start_pos_;
  int sub_stage_id_;
  bool sub_win_;
  bool all_finish_;
  bool be_back_;
};

