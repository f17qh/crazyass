#pragma once
#include "cocos2d.h"


USING_NS_CC;
class CardAction {
public:
  void init(CCSprite* owner);
  void moveWithParabola(CCPoint start_point, CCPoint end_point, float time);
protected:
  CCSprite* owner_;
};

