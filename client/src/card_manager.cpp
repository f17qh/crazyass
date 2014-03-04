#include "card_manager.h"
#include <algorithm>

CCRect TouchableSprite::rect() {
  CCSize s = this->getTexture()->getContentSize();
  CCPoint pos = getPosition();
  return CCRectMake(pos.x-s.width/2, pos.y-s.height/2, s.width, s.height);
}

bool TouchableSprite::containsTouchLocation(CCTouch* touch) {
  return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

void TouchableSprite::onEnter() {
  CCDirector* pDirector = CCDirector::sharedDirector();
  pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
  CCSprite::onEnter();
}

bool TouchableSprite::ccTouchBegan(CCTouch* touch, CCEvent* event) {
  if (!containsTouchLocation(touch))
    return false;
  ((CardMgr*)getParent())->OnTouch(getTag());
  return true;
}

void TouchableSprite::touchDelegateRetain() {
  this->retain();
}

void TouchableSprite::touchDelegateRelease() {
  this->release();
}

void TouchableSprite::onExit() {
  CCDirector* pDirector = CCDirector::sharedDirector();
  pDirector->getTouchDispatcher()->removeDelegate(this);
  CCSprite::onExit();
}


void CardMgr::Init(int stage_id) {
  card_layer_ = CCLayer::create();
  bingo_index_ = 2;
  //TODO:get cardname with stage_id
  char name[32] = {};
  sprintf(name, "card_front_0%d.png", 1);
  int size = 3;
  for(int i = 1; i <= size; i++) {
    if(i == 3)
      sprintf(name, "card_back_0%d.png", 1);
    TouchableSprite* sprite = (TouchableSprite*)TouchableSprite::createWithSpriteFrameName(name);
    sprite->setPosition(ccp(152 + 243*(i-1), 229));
    card_layer_->addChild(sprite, 0, i-1);
    all_card_index_.push_back(i-1);
  }
  std::srand(time(0));
  CCNode::onEnter();
  InitData();
}

void CardMgr::InitData() {
  card_lines_.push_back(all_card_index_);
  for(int times = 0; times < 10; ++times) {
    do {
      std::random_shuffle(all_card_index_.begin(), all_card_index_.end());
    } while (card_lines_[card_lines_.size() -1] == all_card_index_);
    card_lines_.push_back(all_card_index_);
  }
  run_time_ = 0;
  action_nums_ = 0;
}

void CardMgr::StartAction() {
  if(run_time_ >= int(card_lines_.size() - 1)) {
    return;
  }
  for(int sprite_num = 0; sprite_num < (int)all_card_index_.size(); sprite_num++) {
    MovePosBy(card_lines_[run_time_][sprite_num], card_lines_[run_time_+1][sprite_num], 186, 0.5f);
  }
  run_time_++;
}

CCLayer* CardMgr::card_layer() {
  CCAssert(card_layer_ != NULL, "card_layer_ is null");
  return card_layer_;
}

void CardMgr::OnTouch(int child_tag) {
  if(child_tag == bingo_index_) {
    //TODO succ
  } else {
    //TODO fail
  }

}
void CardMgr::MovePosBy(int source_index, int target_index, int high, float time) {
  CCSprite* src = (CCSprite*)card_layer_->getChildByTag(source_index);
  CCPoint start_pos = card_layer_->getChildByTag(source_index)->getPosition();
  CCPoint end_pos = card_layer_->getChildByTag(target_index)->getPosition();
  int dalt = (start_pos.x - end_pos.x) / 243.0f;
  if(dalt < 0) {
    MoveWithBezier(src, start_pos, end_pos, high, time);
  } else if(dalt > 0) {
    if(dalt == 1) {
      MoveWithLine(src, end_pos, time);
    } else {
      MoveWithBezier(src, start_pos, end_pos, -high, time);
    }
  }
}

void CardMgr::MoveWithBezier(CCSprite* src, CCPoint start_point, CCPoint end_point, int high, float time) {
  CCAssert(src != NULL, "src is null");
  src->setPosition(start_point);
  float sx = start_point.x;
  float sy = start_point.y;
  float ex = end_point.x;
  float ey = end_point.y;

  ccBezierConfig bezier; // 创建贝塞尔曲线
  bezier.controlPoint_1 = ccp(sx, sy+high); // 起始点
  bezier.controlPoint_2 = ccp(ex, ey+high); //控制点
  bezier.endPosition = ccp(ex, ey); // 结束位置
  CCBezierTo* cur_action = CCBezierTo::create(time, bezier);
  CCCallFuncN* end = CCCallFuncN::create(this, callfuncN_selector(CardMgr::MoveEnd));
  CCSequence* res = CCSequence::createWithTwoActions(cur_action, end);
  action_nums_++;
  src->runAction(res);
}

void CardMgr::MoveWithLine(CCSprite* src, CCPoint end_point, float time) {
  CCMoveTo* cur_action = CCMoveTo::create(time, end_point);
  CCCallFuncN* end = CCCallFuncN::create(this, callfuncN_selector(CardMgr::MoveEnd));
  CCSequence* res = CCSequence::createWithTwoActions(cur_action, end);
  action_nums_++;
  src->runAction(res);
}

void CardMgr::OnTime(float f) {
   StartAction();
}
void CardMgr::MoveEnd(CCNode* sender) {
  action_nums_--;
  if(action_nums_ == 0)
    scheduleOnce(schedule_selector(CardMgr::OnTime), 0.3f);
}

