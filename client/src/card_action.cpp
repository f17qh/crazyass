#include "card_action.h"


void CardAction::init(CCSprite* owner) {
  CCAssert(owner != NULL, "owner is null");
  owner_ = owner;
  //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("play_images.plist", "play_images.png");
  owner_->initWithSpriteFrameName("card_back_01.png");

  //CCAction* move = CCMoveTo::create(2.0f, ccp(-100,0));
  //owner_->runAction(move);
}

void CardAction::moveWithParabola(CCPoint start_point, CCPoint end_point, float time) {
  owner_->setPosition(start_point);
  float sx = start_point.x;
  float sy = start_point.y;
  float ex = end_point.x;
  float ey = end_point.y;

  ccBezierConfig bezier; // 创建贝塞尔曲线
  bezier.controlPoint_1 = ccp(sx, sy+93); // 起始点
  bezier.controlPoint_2 = ccp(ex, ey+93); //控制点
  bezier.endPosition = ccp(ex, ey); // 结束位置
  CCBezierBy* action_move = CCBezierBy::create(time, bezier);
  owner_->runAction(action_move);
}
