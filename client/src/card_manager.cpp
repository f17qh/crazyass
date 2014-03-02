#include "card_manager.h"
#include <algorithm>
//using std::random_shuffle;

void CardMgr::Init(int stage_id) {
  card_layer_ = CCLayer::create();
  bingo_index_ = 2;
  //TODO:get cardname with stage_id
  char name[32] = {};
  sprintf(name, "card_front_0%d.png", 1);
  int size = 3;
  for(int i = 1; i <= size; i++) {
    if(i == 3) {
      sprintf(name, "card_back_0%d.png", 1);
    }

    CCSprite* sprite = CCSprite::createWithSpriteFrameName(name);
    sprite->setPosition(ccp(152 + 243*(i-1), 229));
    card_layer_->addChild(sprite, 0, i-1);
    all_card_index_.push_back(i-1);
    std::srand(time(0));
  }
}

void CardMgr::StartAction() {
  std::vector<std::vector<int> > temp;
  temp.push_back(all_card_index_);
  for(int times = 0; times < 10; ++times) {
    do {
      std::random_shuffle(all_card_index_.begin(), all_card_index_.end());
    } while (temp[temp.size() -1] == all_card_index_);
    temp.push_back(all_card_index_);
  }
  std::vector<CCFiniteTimeAction*> sprite_actions;
  for(int sprite_num = 0; sprite_num < (int)all_card_index_.size(); sprite_num++) {
    CCFiniteTimeAction* prve_action = NULL;
    for(int random_time = 0; random_time < (int)temp.size() - 1; random_time++) {
      prve_action = MovePosBy(prve_action, temp[random_time][sprite_num], temp[random_time+1][sprite_num], 186, 0.5f);
      if(prve_action != NULL)
        prve_action = CCSequence::createWithTwoActions(prve_action, CCDelayTime::create(0.3f));
    }
    if(prve_action != NULL) {
      sprite_actions.push_back(prve_action);
    }
  }
  for(int sprite_num = 0; sprite_num < (int)all_card_index_.size(); sprite_num++) {
    if(sprite_actions[sprite_num] != NULL) {
      CCSprite* sprite = (CCSprite*)card_layer_->getChildByTag(sprite_num);
      sprite->runAction(sprite_actions[sprite_num]);
    }
  }
}

CCLayer* CardMgr::card_layer() {
  CCAssert(card_layer_ != NULL, "card_layer_ is null");
  return card_layer_;
}

CCFiniteTimeAction* CardMgr::MovePosBy(CCFiniteTimeAction* prve_action, int source_index, int target_index, int high, float time) {
  //CCSprite* src = (CCSprite*)card_layer_->getChildByTag(source_index);
  CCPoint start_pos = card_layer_->getChildByTag(source_index)->getPosition();
  CCPoint end_pos = card_layer_->getChildByTag(target_index)->getPosition();
  //int dalt = source_index - target_index;
  int dalt = (start_pos.x - end_pos.x) / 243.0f;
  if(dalt < 0) {
    if(dalt == -1) {
      return MoveWithLine(prve_action, end_pos, time);
    } else {
      return MoveWithBezier(prve_action, start_pos, end_pos, high, time);
    }
  } else if(dalt > 0) {
    if(dalt == 1) {
      return MoveWithLine(prve_action, end_pos, time);
    } else {
      return MoveWithBezier(prve_action, start_pos, end_pos, -high, time);
    }
  } else {
    return prve_action;
  }
  
}

CCFiniteTimeAction* CardMgr::MoveWithBezier(CCFiniteTimeAction* prve_action, CCPoint start_point, CCPoint end_point, int high, float time) {
  //CCAssert(prve_action != NULL, "prve_action is null");
  //src->setPosition(start_point);
  float sx = start_point.x;
  float sy = start_point.y;
  float ex = end_point.x;
  float ey = end_point.y;

  ccBezierConfig bezier; // 创建贝塞尔曲线
  bezier.controlPoint_1 = ccp(sx, sy+high); // 起始点
  bezier.controlPoint_2 = ccp(ex, ey+high); //控制点
  bezier.endPosition = ccp(ex, ey); // 结束位置
  CCBezierTo* cur_action = CCBezierTo::create(time, bezier);
  if(prve_action == NULL) {
    return cur_action;
  } else {
    CCSequence* res = CCSequence::createWithTwoActions(prve_action, cur_action);
    return res;
  }
  //src->runAction(action_move);
}

CCFiniteTimeAction* CardMgr::MoveWithLine(CCFiniteTimeAction* prve_action, CCPoint end_point, float time) {
  CCMoveTo* cur_action = CCMoveTo::create(time, end_point);
  if(prve_action == NULL) {
    return cur_action;
  } else {
    CCSequence* res = CCSequence::createWithTwoActions(prve_action, cur_action);
    return res;
  }
  //src->runAction(action_move);
}
