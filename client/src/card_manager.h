#pragma once
#include "cocos2d.h"
#include <vector>

USING_NS_CC;
class TouchableSprite: public CCSprite, public CCTargetedTouchDelegate {

public:
  TouchableSprite();
  virtual ~TouchableSprite();
  virtual void onEnter();
  virtual void onExit();
  CREATE_FUNC(TouchableSprite);
  CCRect rect();
  bool containsTouchLocation(CCTouch *touch);
  void touchDelegateRetain();
  void touchDelegateRelease();
  virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
};

class CardMgr : public CCNode {

public:
  void Init(int stage_id);
  void StartAction();
  CCLayer* card_layer();
  void OnTouch(int child_tag);
protected:
  void MovePosBy(int source_index, int target_index, int high, float time);
  void MoveWithBezier(CCSprite* src, CCPoint start_point, CCPoint end_point, int high, float time);
  void MoveWithLine(CCSprite* src, CCPoint end_point, float time);
  void MoveEnd(CCNode* sender);
  void OnTime(float);
  void InitData();
protected:
  CCLayer *card_layer_;
  int bingo_index_;
  int run_time_;
  int action_nums_;
  std::vector<int> all_card_index_;
  std::vector<std::vector<int> > card_lines_;
};
