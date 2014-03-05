#include "card_manager.h"
#include "stage_config.h"
#include "play_scene.h"
#include <algorithm>

CCRect TouchableSprite::rect() {
  CCSize s = this->getContentSize();//getTexture()->get();
  CCPoint pos = getPosition();
  return CCRectMake(-s.width*getAnchorPoint().x, -s.height*getAnchorPoint().y, s.width, s.height);
  //return CCRectMake(pos.x, pos.y, s.width, s.height);
}

bool TouchableSprite::containsTouchLocation(CCTouch* touch) {
  CCPoint pos = convertTouchToNodeSpaceAR(touch);
  return rect().containsPoint(pos);
}

void TouchableSprite::onEnter() {
  CCDirector* pDirector = CCDirector::sharedDirector();
  pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
  CCSprite::onEnter();
}

bool TouchableSprite::ccTouchBegan(CCTouch* touch, CCEvent* event) {
  if (!containsTouchLocation(touch))
    return false;
  card_mgr_->OnTouch(getTag());
  CCPoint pos = getPosition();
  CCLOG("%s, this:%p %d, x%f, y%f\n", __FUNCTION__, this, getTag(), pos.x, pos.y);
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

void CardMgr::SetTouchable(bool b) {
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  int size = config.card_count_;
  for(int i = 0; i < size; i++) {
    CCDirector* pDirector = CCDirector::sharedDirector();
    if(b) {
      pDirector->getTouchDispatcher()->addTargetedDelegate((TouchableSprite*)card_layer_->getChildByTag(i), 0, true);
    } else {
      pDirector->getTouchDispatcher()->removeDelegate((TouchableSprite*)card_layer_->getChildByTag(i));
    }
  }
}
void CardMgr::SetEnable(bool b) {
  card_layer_->setVisible(b);
  SetTouchable(b);
}

void CardMgr::CreateLayer(PlayScene* play_scene) {
  play_scene_ = play_scene;
  card_layer_ = CCLayer::create();
}

void CardMgr::Init(int stage_id) {
  std::srand(time(0));
  stage_id_ = stage_id;
  bingo_index_ = 2;
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  char name[32] = {};
  sprintf(name, "card_front_0%d.png", 1);
  int size = config.card_count_;
  for(int i = 1; i <= size; i++) {
    if(i == 3)
      sprintf(name, "card_back_0%d.png", 1);
    TouchableSprite* sprite = TouchableSprite::create();
    sprite->initWithSpriteFrameName(name);
    sprite->card_mgr_ = this;
    sprite->setPosition(ccp(152 + 243*(i-1), 229));
    card_layer_->addChild(sprite, 0, i-1);
    all_card_index_.push_back(i-1);
    CCPoint pos = sprite->getPosition();
    CCLOG("%s, sprite:%p %d x%f, y%f\n", __FUNCTION__, sprite, i-1, pos.x, pos.y);
  }
  CCNode::onEnter();
  play_count_ = 0;
  action_nums_ = 0;
  sub_stage_ = 0;
  //为第一次生成路径
  MakeLinesData(config.play_count_[sub_stage_]);
}

void CardMgr::MakeLinesData(int run_times) {
  card_lines_.clear();
  card_lines_.push_back(all_card_index_);
  for(int times = 0; times < run_times; ++times) {
    do {
      std::random_shuffle(all_card_index_.begin(), all_card_index_.end());
    } while (card_lines_[card_lines_.size() -1] == all_card_index_);
    card_lines_.push_back(all_card_index_);
  }
}

void CardMgr::StartAction() {
  if(play_count_ >= int(card_lines_.size() - 1)) {
    SetTouchable(true);
    return;
  }
  SetTouchable(false);
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);

  for(int sprite_num = 0; sprite_num < (int)all_card_index_.size(); sprite_num++) {
    MovePosBy(card_lines_[play_count_][sprite_num], 
              card_lines_[play_count_+1][sprite_num], 186, 
              config.shuffle_speed_[sub_stage_]);
  }
  play_count_++;
}

CCLayer* CardMgr::card_layer() {
  CCAssert(card_layer_ != NULL, "card_layer_ is null");
  return card_layer_;
}

void CardMgr::OnTouch(int child_tag) {
  if(sub_stage_ > 3)
    return;
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  MakeLinesData(config.play_count_[sub_stage_]);
  sub_stage_++;
  play_count_ = 0;

  if(child_tag == bingo_index_) {
    //猜对了
    SetEnable(false);
    play_scene_->TakeOff(sub_stage_);
  } else {
    //TODO 猜错了
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
  CCSequence* tmp = CCSequence::createWithTwoActions(cur_action, end);
  CCSequence* res = CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), tmp);
  action_nums_++;
  src->runAction(res);
}

void CardMgr::MoveWithLine(CCSprite* src, CCPoint end_point, float time) {
  CCMoveTo* cur_action = CCMoveTo::create(time, end_point);
  CCCallFuncN* end = CCCallFuncN::create(this, callfuncN_selector(CardMgr::MoveEnd));
  CCSequence* tmp = CCSequence::createWithTwoActions(cur_action, end);
  CCSequence* res = CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), tmp);
  action_nums_++;
  src->runAction(res);
}

void CardMgr::MoveEnd(CCNode* sender) {
  action_nums_--;
  if(action_nums_ == 0)
    StartAction();
    //scheduleOnce(schedule_selector(CardMgr::OnTime), 0.3f);
}

