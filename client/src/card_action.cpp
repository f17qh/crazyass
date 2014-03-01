#include "card_action.h"


void CardAction::init(CCSprite* owner) {
  CCAssert(owner != NULL, "owner is null");
  owner_ = owner;
  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("play_images.plist", "play_images.png");
  owner_->initWithSpriteFrameName("card_back_01.png");

  //CCAction* move = CCMoveTo::create(2.0f, ccp(-100,0));
  //owner_->runAction(move);
}

void CardAction::moveWithParabola(CCPoint start_point, CCPoint end_point, float time) {
  float sx = start_point.x;
  float sy = start_point.y;
  float ex =end_point.x+50;
  float ey =end_point.y+150;
  int h = owner_->getContentSize().height * 0.5;
  ccBezierConfig bezier; // 创建贝塞尔曲线
  bezier.controlPoint_1 = ccp(sx, sy); // 起始点
  bezier.controlPoint_2 = ccp(sx+(ex-sx)*0.5, sy+(ey-sy)*0.5+200); //控制点
  bezier.endPosition = ccp(end_point.x-30, end_point.y+h); // 结束位置
  CCBezierTo* action_move = CCBezierTo::create(time, bezier);
  owner_->runAction(action_move);
}
