#pragma once
#include "cocos2d.h"
#include "dog.h"
#include "dog_action.h"

USING_NS_CC;
class DogAttrPanelSprite;

class DogSprite : public CCSprite,
  public CCTargetedTouchDelegate {
public:
  virtual bool init();
  virtual void onEnter();
  virtual void onExit();

  virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
  virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
  virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

  virtual void touchDelegateRetain();
  virtual void touchDelegateRelease();
  bool containsTouchLocation(CCTouch* touch);

  void DogAttrChangeCallBack(CCObject* sender);
  CREATE_FUNC(DogSprite);
protected:
  CCPoint last_xy_;
  CCPoint start_xy_;
  CCRect rect();
  Dog* dog_;
  DogAttrPanelSprite* dog_attr_sprite_;
  bool dog_attr_status_;
  DogAction dog_action_;
};

class DogAttrPanelSprite : public CCSprite {
public:
  virtual bool init();
  CREATE_FUNC(DogAttrPanelSprite);
  void UpdateAttr();
protected:
  Dog* dog_;
  CCLabelTTF* label_dog_name_;
  CCLabelTTF* label_dog_lv_;
  CCLabelTTF* label_dog_strong_;
  CCLabelTTF* label_dog_speed_;
  CCLabelTTF* label_dog_intimacy_;
  CCSprite* lv_bar_;
  CCSpriteFrame* lv_bar_frame_;
};

class HeartProgressBar{
public:
  HeartProgressBar(){};
  void SetMaxNum(int max_num);
  void SetHeartProgressBar(float cur_num);
  void CreateSprite(CCNode* parent);
protected:
  float max_num_;
  CCSprite* heart_left_;
  CCSprite* heart_mid_;
  CCSprite* heart_right_;
  CCLabelTTF* heart_num_;
  CCSpriteFrame* heart_mid_frame_;
};


