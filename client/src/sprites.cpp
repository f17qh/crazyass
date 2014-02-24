#include "sprites.h"
#include "dog.h"
#include "event_mgr.h"

bool DogAttrPanelSprite::init() {
  if (!CCSprite::init())
    return false;

  this->initWithSpriteFrameName("dog_ability.png");
  dog_ = User::current()->dogs(0);

  char dog_name[32] = {0};
  snprintf(dog_name,sizeof(dog_name),"%s", dog_->attr().name().c_str());
  label_dog_name_ = CCLabelTTF::create( dog_name, "Arial", 20);
  label_dog_name_->setAnchorPoint(ccp(0, 0));
  label_dog_name_->setPosition(ccp(98, 139));
  addChild(label_dog_name_, 1);

  char dog_character[32] = {0};
  snprintf(dog_character,sizeof(dog_character),"%s", "A等级");
  CCLabelTTF* label_dog_character = CCLabelTTF::create( dog_character, "WenQuanYi Zen Hei", 20);
  label_dog_character->setAnchorPoint(ccp(0, 0));
  label_dog_character->setPosition(ccp(98, 111));
  addChild(label_dog_character, 1);

  char dog_lv[32] = {0};
  snprintf(dog_lv,sizeof(dog_lv),"%s  %d", "Lv", dog_->attr().lv());
  label_dog_lv_ = CCLabelTTF::create( dog_lv, "Arial", 20);
  label_dog_lv_->setAnchorPoint(ccp(0, 0));
  label_dog_lv_->setPosition(ccp(30, 80));
  addChild(label_dog_lv_, 1);

  char dog_strong[32] = {0};
  snprintf(dog_strong,sizeof(dog_strong),"%d", dog_->attr().strong());
  label_dog_strong_ = CCLabelTTF::create( dog_strong, "Arial", 20);
  label_dog_strong_->setAnchorPoint(ccp(0, 0));
  label_dog_strong_->setPosition(ccp(30, 24));
  addChild(label_dog_strong_, 1);

  char dog_speed[32] = {0};
  snprintf(dog_speed,sizeof(dog_speed),"%d", dog_->attr().speed());
  label_dog_speed_ = CCLabelTTF::create( dog_speed, "Arial", 20);
  label_dog_speed_->setAnchorPoint(ccp(0, 0));
  label_dog_speed_->setPosition(ccp(110, 24));
  addChild(label_dog_speed_, 1);

  char dog_intimacy[32] = {0};
  snprintf(dog_intimacy,sizeof(dog_intimacy),"%d", dog_->attr().intimacy());
  label_dog_intimacy_ = CCLabelTTF::create( dog_intimacy, "Arial", 20);
  label_dog_intimacy_->setAnchorPoint(ccp(0, 0));
  label_dog_intimacy_->setPosition(ccp(185, 24));
  addChild(label_dog_intimacy_, 1);

  lv_bar_ = CCSprite::create();
  lv_bar_frame_ = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lv_bar.png");
  lv_bar_->initWithSpriteFrameName("lv_bar.png");
  lv_bar_->setAnchorPoint(ccp(0,0));
  lv_bar_->setPosition(ccp(101, 82));
  addChild(lv_bar_, 1);

  float lv_bar_width = dog_->attr().exp()*(150.0f/100.0f);
  CCRect frame_rect = lv_bar_frame_->getRect();
  CCRect rect(frame_rect.getMinX(), frame_rect.getMinY(), lv_bar_width, frame_rect.size.height);
  lv_bar_->setTextureRect(rect);

  return true;
}

void DogAttrPanelSprite::UpdateAttr() {
  char dog_name[32] = {0};
  snprintf(dog_name,sizeof(dog_name),"%s", dog_->attr().name().c_str());
  label_dog_name_->setString(dog_name);

  char dog_lv[32] = {0};
  snprintf(dog_lv,sizeof(dog_lv),"%s  %d", "Lv", dog_->attr().lv());
  label_dog_lv_->setString(dog_lv);

  char dog_strong[32] = {0};
  snprintf(dog_strong,sizeof(dog_strong),"%d", dog_->attr().strong());
  label_dog_strong_->setString(dog_strong);

  char dog_speed[32] = {0};
  snprintf(dog_speed,sizeof(dog_speed),"%d", dog_->attr().speed());
  label_dog_speed_->setString(dog_speed);

  char dog_intimacy[32] = {0};
  snprintf(dog_intimacy,sizeof(dog_intimacy),"%d", dog_->attr().intimacy());
  label_dog_intimacy_->setString(dog_intimacy);

  float lv_bar_width = dog_->attr().exp()*(150.0f/100.0f);
  CCRect frame_rect = lv_bar_frame_->getRect();
  CCRect rect(frame_rect.getMinX(), frame_rect.getMinY(), lv_bar_width, frame_rect.size.height);
  lv_bar_->setTextureRect(rect);

}

void DogSprite::DogAttrChangeCallBack(CCObject* sender) {
  dog_attr_sprite_->UpdateAttr();
}

bool DogSprite::init() {
  if (!CCSprite::init())
    return false;

  dog_attr_status_ = false;
  this->initWithSpriteFrameName("dog.png");
  dog_ = User::current()->dogs(0);

  dog_attr_sprite_ = DogAttrPanelSprite::create();
  dog_attr_sprite_->setAnchorPoint(ccp(0,0));
  dog_attr_sprite_->setPosition(ccp(-96,96));
  dog_attr_sprite_->setVisible(dog_attr_status_);
  addChild(dog_attr_sprite_, 1);


  dog_action_.init(this);

  EventMgr::Instance().Register(kEventDogAttrChange,
    this, callfuncO_selector(DogSprite::DogAttrChangeCallBack));

  return true;
}

CCRect DogSprite::rect() {
  CCSize s = this->getTexture()->getContentSize();
  return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool DogSprite::containsTouchLocation(CCTouch* touch) {
  return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

void DogSprite::onEnter() {
  CCDirector* pDirector = CCDirector::sharedDirector();
  pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
  CCSprite::onEnter();
}

bool DogSprite::ccTouchBegan(CCTouch* touch, CCEvent* event) {
  if (!containsTouchLocation(touch))
    return false;
  last_xy_ = touch->getLocation();
  start_xy_ = touch->getLocation();
  return true;
}

void DogSprite::ccTouchEnded(CCTouch* touch, CCEvent* event) {
  CCPoint cur_xy = touch->getLocation();
  if(start_xy_.fuzzyEquals(cur_xy, 2.0f)) {
    dog_attr_status_ = !dog_attr_status_;
    dog_attr_sprite_->setVisible(dog_attr_status_);
    EventMgr::Instance().Response(kEventClickDogMenu);
  }
}

void DogSprite::ccTouchMoved(CCTouch* touch, CCEvent* event) {
  CCPoint loc = touch->getLocation();
  float x, y;
  x = loc.x - last_xy_.x;
  y = loc.y - last_xy_.y;

  CCPoint pos = this->getPosition();
  this->setPosition(ccp(pos.x + x, pos.y + y));
  last_xy_ = loc;
}

void DogSprite::touchDelegateRetain() {
  this->retain();
}

void DogSprite::touchDelegateRelease() {
  this->release();
}

void DogSprite::onExit() {
  CCDirector* pDirector = CCDirector::sharedDirector();
  pDirector->getTouchDispatcher()->removeDelegate(this);
  CCSprite::onExit();
} 

void HeartProgressBar::SetMaxNum(int max_num) {
  max_num_ = max_num;
}

void HeartProgressBar::CreateSprite(CCNode* parent) {
  heart_left_ = CCSprite::create();
  heart_left_->initWithSpriteFrameName("like_left.png");
  heart_left_->setAnchorPoint(ccp(0,0));
  heart_left_->setPosition(ccp(2, 2));
  parent->addChild(heart_left_, 1);

  int heart_left_width = heart_left_->getTextureRect().size.width;
  heart_mid_ = CCSprite::create();
  heart_mid_frame_ = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("like_mid.png");
  heart_mid_->initWithSpriteFrameName("like_mid.png");
  heart_mid_->setAnchorPoint(ccp(0,0));
  heart_mid_->setPosition(ccp(heart_left_width +2, 2));
  heart_mid_->setVisible(false);

  parent->addChild(heart_mid_, 1);

  heart_right_ = CCSprite::create();
  heart_right_->initWithSpriteFrameName("like_right.png");
  heart_right_->setAnchorPoint(ccp(0,0));
  heart_right_->setVisible(false);
  parent->addChild(heart_right_, 1);

  heart_num_ = CCLabelTTF::create("", "Arial", 24);
  parent->addChild(heart_num_, 3);
  heart_num_->setPosition(ccp(70, 20));
}

void HeartProgressBar::SetHeartProgressBar(float cur_num) {
  float mid_real_width = cur_num*(100/max_num_);
  CCRect frame_rect = heart_mid_frame_->getRect();
  CCRect rect(frame_rect.getMinX(), frame_rect.getMinY(), mid_real_width, frame_rect.size.height);
  
  heart_mid_->setVisible(true);
  heart_mid_->setTextureRect(rect);
  int heart_right_width = heart_right_->getTextureRect().size.width;
  if(mid_real_width > heart_right_width) {
    int heart_left_width = heart_left_->getTextureRect().size.width;
    heart_right_->setVisible(true);
    heart_right_->setPosition(ccp(mid_real_width + heart_left_width+2, 2));
  }

  char num[32] = {0};
  snprintf(num,sizeof(num),"%d", (int)cur_num);
  heart_num_->setString(num);
}

