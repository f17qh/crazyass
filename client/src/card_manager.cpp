#include "card_manager.h"
#include "stage_config.h"
#include "play_scene.h"
#include <algorithm>

CCRect TouchableSprite::rect() {
  CCSize s = this->getContentSize();
  CCPoint pos = getPosition();
  return CCRectMake(-s.width*getAnchorPoint().x, -s.height*getAnchorPoint().y, s.width, s.height);
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

CardMgr::CardMgr() {
  std::srand(time(0));
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
  stage_id_ = stage_id;
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  int size = config.card_count_;
  for(int i = 0;  i < size; i++) {
    all_card_index_.push_back(i);
  }
  CCNode::onEnter();
  play_count_ = 0;
  moved_card_nums_ = 0;
  sub_stage_ = 0;
}

void CardMgr::SetCardSprite() {
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  char name[32] = {};
  sprintf(name, "card_front_0%d.png", sub_stage_+1);
  int size = config.card_count_;
  bingo_index_ = std::rand() % size;
  for(int i = 1; i <= size; i++) {
    TouchableSprite* sprite = TouchableSprite::create();
    sprite->initWithSpriteFrameName(name);
    sprite->card_mgr_ = this;
    sprite->setPosition(ccp(config.start_pos_x_ + config.interval_*(i-1), config.start_pos_y_));
    if(card_layer_->getChildByTag(i-1) != NULL){
      card_layer_->removeChildByTag(i-1);
    }
    card_layer_->addChild(sprite, 0, i-1);
  }
}
void CardMgr::StartSubStage() {
  //获取主关卡的配置数据
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  //设置该substage的精灵纹理
  SetCardSprite();
  //设置cardmgr为有效
  SetEnable(true);
  //生成该sub_stage的card的移动路线
  MakeLinesData(config.play_count_[sub_stage_]);
  //执行开始动画
  RunBeginAction();

}

void CardMgr::FinishSubStage() {
  //结束了，要设置card可点击
  SetTouchable(true);
  //子stage的id++
  sub_stage_++;
  //数据重新设置
  play_count_ = 0;
}

bool CardMgr::TryFinishStage() {
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  if(sub_stage_ >= (int)sizeof(config.play_count_))
    return true;
  return false;
}

void CardMgr::RunActionByPlayCount() {
  StageInfo& config = StageConfig::Instence().GetStageInfo(stage_id_);
  //判断该substage是否已经结束
  if(play_count_ >= int(config.play_count_[sub_stage_] - 1)) {
    FinishSubStage();
    return;
  }

  SetTouchable(false);

  //如果没有结束要继续执行动画
  for(int sprite_num = 0; sprite_num < (int)all_card_index_.size(); sprite_num++) {
    MovePosBy(card_lines_[play_count_][sprite_num], 
      card_lines_[play_count_+1][sprite_num], 186, 
      config.shuffle_speed_[sub_stage_], config.interval_);
  }

  play_count_++;
}

CCLayer* CardMgr::card_layer() {
  CCAssert(card_layer_ != NULL, "card_layer_ is null");
  return card_layer_;
}

void CardMgr::OnTouch(int child_tag) {
  if(child_tag == bingo_index_) {
    //猜对了
    SetEnable(false);
    //这个时候已经finishsubstage，所以sub_stage_要减1
    play_scene_->TakeOff(sub_stage_-1);
  } else {
    //TODO 猜错了
  }

}

////////////////////////////////////////////////////////////////////////////////////////
void CardMgr::MakeLinesData(int card_move_times) {
  card_lines_.clear();
  card_lines_.push_back(all_card_index_);
  for(int times = 0; times < card_move_times; ++times) {
    do {
      std::random_shuffle(all_card_index_.begin(), all_card_index_.end());
    } while (card_lines_[card_lines_.size() -1] == all_card_index_);
    card_lines_.push_back(all_card_index_);
  }
}

void CardMgr::MovePosBy(int source_index, int target_index, int high,
                        float time, float interval) {
  CCSprite* src = (CCSprite*)card_layer_->getChildByTag(source_index);
  CCPoint start_pos = card_layer_->getChildByTag(source_index)->getPosition();
  CCPoint end_pos = card_layer_->getChildByTag(target_index)->getPosition();
  int dalt = (start_pos.x - end_pos.x) / interval;
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

void CardMgr::MoveWithBezier(CCSprite* src, CCPoint start_point, CCPoint end_point,
                             int high, float time) {
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
  //动画结束调用的callback函数
  CCCallFuncN* end = CCCallFuncN::create(this, callfuncN_selector(CardMgr::MoveEnd));
  CCSequence* tmp = CCSequence::createWithTwoActions(cur_action, end);
  CCSequence* res = CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), tmp);
  //移动的card的动画次数++
  moved_card_nums_++;
  src->runAction(res);
}

void CardMgr::MoveWithLine(CCSprite* src, CCPoint end_point, float time) {
  CCMoveTo* cur_action = CCMoveTo::create(time, end_point);
  CCCallFuncN* end = CCCallFuncN::create(this, callfuncN_selector(CardMgr::MoveEnd));
  CCSequence* tmp = CCSequence::createWithTwoActions(cur_action, end);
  CCSequence* res = CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), tmp);
  //移动的card的动画次数++
  moved_card_nums_++;
  src->runAction(res);
}

void CardMgr::RunBeginAction() {

  //这个要设置为false，否则会宕机，原因未知。。。
  SetTouchable(false);

  CCOrbitCamera * orbit1 = CCOrbitCamera::create(0.25, 0.5, 0, 0, 90, 0, 0);
  CCCallFuncN* change_card1 = CCCallFuncN::create(this, callfuncN_selector(CardMgr::ChangeCardToSpriteBack));
  CCOrbitCamera * orbit2 = CCOrbitCamera::create(0.25, 0, -0.5, 90, 90, 0, 0);
  CCMoveBy* move1 = CCMoveBy::create(0.25, ccp(0,50));
  CCMoveBy* move2 = CCMoveBy::create(0.25, ccp(0,-50));
  CCOrbitCamera * orbit3 = CCOrbitCamera::create(0.25, 0.5, 0, 0, 90, 0, 0);
  CCCallFuncN* change_card2 = CCCallFuncN::create(this, callfuncN_selector(CardMgr::ChangeCardToSpriteFront));
  CCOrbitCamera * orbit4 = CCOrbitCamera::create(0.25, 0, -0.5, 90, 90, 0, 0);
  CCCallFuncN* end = CCCallFuncN::create(this, callfuncN_selector(CardMgr::BeginActionEnd));
  CCSequence* tmp = CCSequence::create(orbit1,change_card1,orbit2,move1,move2,orbit3,change_card2,orbit4,end,NULL);
  CCSprite* src = (CCSprite*)card_layer_->getChildByTag(bingo_index_);
  src->runAction(tmp);
}

void CardMgr::ChangeCardToSpriteFront(CCNode* sender) {
  CCSprite* src = (CCSprite*)card_layer_->getChildByTag(bingo_index_);
  char name[32] = {};
  sprintf(name, "card_front_0%d.png", sub_stage_+1);
  src->initWithSpriteFrameName(name);
}

void CardMgr::ChangeCardToSpriteBack(CCNode* sender) {
  CCSprite* src = (CCSprite*)card_layer_->getChildByTag(bingo_index_);
  char name[32] = {};
  sprintf(name, "card_back_0%d.png", stage_id_);
  src->initWithSpriteFrameName(name);
}

void CardMgr::BeginActionEnd(CCNode* sender) {
  //为每一个card执行移动动画，下面这个函数要执行action_times_次，有moveend函数触发非第一次的。
  RunActionByPlayCount();
}

void CardMgr::MoveEnd(CCNode* sender) {
  //移动的card的动画次数--
  moved_card_nums_--;
  //移动的card的动画次数如果等于0，表示本次移动结束
  if(moved_card_nums_ == 0)
    RunActionByPlayCount();
}

