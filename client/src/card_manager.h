#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

USING_NS_CC;
class CardMgr;
class PlayScene;
class TouchableSprite: public CCSprite, public CCTargetedTouchDelegate {
public:
  TouchableSprite(){};
  virtual ~TouchableSprite(){};
  virtual void onEnter();
  virtual void onExit();
  CREATE_FUNC(TouchableSprite);
  CCRect rect();
  bool containsTouchLocation(CCTouch *touch);
  void touchDelegateRetain();
  void touchDelegateRelease();
  virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
public:
  CardMgr* card_mgr_;
};

class CardMgr : public CCNode {

public:
  void CreateLayer(PlayScene* play_scene_);
  void Init(int stage_id);
  void StartAction();
  CCLayer* card_layer();
  void OnTouch(int child_tag);
  void SetEnable(bool b);
  void SetTouchable(bool b);
protected:
  void MovePosBy(int source_index, int target_index, int high, float time);
  void MoveWithBezier(CCSprite* src, CCPoint start_point, CCPoint end_point, int high, float time);
  void MoveWithLine(CCSprite* src, CCPoint end_point, float time);
  void MoveEnd(CCNode* sender);
  void MakeLinesData(int play_count);
protected:
  PlayScene* play_scene_;
  CCLayer *card_layer_;
  int bingo_index_;
  int sub_stage_;
  int play_count_;
  int action_nums_;
  int stage_id_;
  bool enable_;
  std::vector<int> all_card_index_;
  std::vector<std::vector<int> > card_lines_;
};
