#pragma once
#include "cocos2d.h"


USING_NS_CC;
class DogAction {
public:
  void init(CCSprite* owner);
protected:
  CCSprite* owner_;
  CCArray* animFrames_;
  CCAnimation* animation_;
  CCAnimate* animate_;
};

