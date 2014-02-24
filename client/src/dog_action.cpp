#include "dog_action.h"


void DogAction::init(CCSprite* owner) {
  CCAssert(owner != NULL, "owner is null");
  owner_ = owner;
  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("run.plist", "run.png");
  animFrames_ = CCArray::create();
  char temp[64] = {0};
  for(int i = 1; i <= 6; i++)
  {  
    sprintf(temp,"run_%d.png", i);
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
    animFrames_->addObject(frame);
  }

  animation_ = CCAnimation::createWithSpriteFrames(animFrames_, 0.15f);
  animate_ = CCAnimate::create(animation_);
  owner_->runAction(CCRepeatForever::create(animate_));

  CCAction* move = CCMoveTo::create(2.0f, ccp(-100,0));
  owner_->runAction(move);
}
