#pragma once
#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class CardMgr {

public:
  void Init(int stage_id);
  void StartAction();
  CCLayer* card_layer();
protected:
  CCFiniteTimeAction* MovePosBy(CCFiniteTimeAction* prve_action, int source_index, int target_index, int high, float time);
  CCFiniteTimeAction* MoveWithBezier(CCFiniteTimeAction* prve_action, CCPoint start_point, CCPoint end_point, int high, float time);
  CCFiniteTimeAction* MoveWithLine(CCFiniteTimeAction* prve_action, CCPoint end_point, float time);
protected:
  CCLayer *card_layer_;
  int bingo_index_;
  std::vector<int> all_card_index_;
};
