#include "menu_items.h"
#include "ui_config_init.h"
#include "protocol/uiconfig.pb.h"
#include "dog.h"
#include "event_mgr.h"

void MultiSpriteMenuItem::onEnter() {
  CCNode::onEnter();
  StartCD();
}

void MultiSpriteMenuItem::onExit() {
  CCNode::onExit();
  label_cd_->setString("");
  unschedule(schedule_selector(MultiSpriteMenuItem::OnTime));
}

MultiSpriteMenuItem * MultiSpriteMenuItem::create(std::string item_type, CCPoint pos) {
  MultiSpriteMenuItem *ret = new MultiSpriteMenuItem();
  if (ret) {
    ret->autorelease();
  } else {
    delete ret;
    ret = NULL;
    return ret;
  }
  
  if(item_type == "train") {
    EventMgr::Instance().Register(kEventChangeToTrainingItem, ret, 
      callfuncO_selector(MultiSpriteMenuItem::ChangeDoingItemCallback));
  }

  if(item_type == "play") {
    EventMgr::Instance().Register(kEventChangeToPlayingItem, ret, 
      callfuncO_selector(MultiSpriteMenuItem::ChangeDoingItemCallback));
  }

  if(item_type == "food") {
    EventMgr::Instance().Register(kEventChangeToFeedingItem, ret, 
      callfuncO_selector(MultiSpriteMenuItem::ChangeDoingItemCallback));
  }

  ret->item_type_ = item_type;
  ret->item_pos_ = pos;
  ret->InitItem();
  ret->InitItemPos();
  return ret;
}

int MultiSpriteMenuItem::GetItemWidth() {
  return (int)normal_item_->getContentSize().width;
}

void MultiSpriteMenuItem::AddToArray(CCArray* array) {
  array->addObject(normal_item_);
  array->addObject(doing_item_);
  array->addObject(done_item_);
}

void MultiSpriteMenuItem::InitItem() {
  std::string normal = item_type_ + std::string(".png");
  std::string doing = item_type_ + std::string("ing.png");
  std::string done = item_type_ + std::string("_done.png");
  CCSprite* normal_sprite = CCSprite::create();
  normal_sprite->initWithSpriteFrameName(normal.c_str());
  normal_item_ = CCMenuItemSprite::create(normal_sprite, normal_sprite, normal_sprite,
    this, menu_selector(MultiSpriteMenuItem::ClickItemCallback));

  CCSprite* doing_sprite = CCSprite::create();
  doing_sprite->initWithSpriteFrameName(doing.c_str());
  doing_item_ = CCMenuItemSprite::create(doing_sprite, doing_sprite, doing_sprite, 
    this, menu_selector(MultiSpriteMenuItem::ClickDoingItemCallback));
  
  CCLabelTTF *label_speed_up_num = CCLabelTTF::create("100", "Arial", 17);
  doing_item_->addChild(label_speed_up_num, 1);
  label_speed_up_num->setAnchorPoint(ccp(0,0));
  label_speed_up_num->setPosition(ccp(139, 13));

  label_cd_ = CCLabelTTF::create("", "Arial", 17);
  doing_item_->addChild(label_cd_, 1);
  label_cd_->setAnchorPoint(ccp(0,0));
  label_cd_->setPosition(ccp(100, 35));


  CCSprite* done_sprite = CCSprite::create();
  done_sprite->initWithSpriteFrameName(done.c_str());
  done_item_ = CCMenuItemSprite::create(done_sprite, done_sprite, done_sprite, 
    this, menu_selector(MultiSpriteMenuItem::ClickDoneItemCallback));

}

void MultiSpriteMenuItem::InitItemPos() {

  normal_item_->setAnchorPoint(ccp(0, 0));
  normal_item_->setPosition(ccp(item_pos_.x, item_pos_.y));

  doing_item_->setAnchorPoint(ccp(0, 0));
  doing_item_->setPosition(ccp(item_pos_.x, item_pos_.y));
  doing_item_->setEnabled(false);
  doing_item_->setVisible(false);

  done_item_->setAnchorPoint(ccp(0, 0));
  done_item_->setPosition(ccp(item_pos_.x, item_pos_.y));
  done_item_->setEnabled(false);
  done_item_->setVisible(false);
}

void MultiSpriteMenuItem::ClickItemCallback(CCObject* sender) {
  if(item_type_ == "train") {
    EventMgr::Instance().Response(kEventClickTrainItem);
  }

  if(item_type_ == "play") {
    EventMgr::Instance().Response(kEventClickPlayItem);
  }

  if(item_type_ == "food") {
    EventMgr::Instance().Response(kEventClickFeedItem);
  }
}

void MultiSpriteMenuItem::ClickDoingItemCallback(CCObject* sender) {
  if(item_type_ == "train") {
    if (User::current()->UseMoney(100) == 0) {
      User::current()->dogs(0)->ClearCD(kTrainCD);
      EventMgr::Instance().Response(kEventUserInfoChange);
      FinishDoing();
    }
  }

  if(item_type_ == "play") {
    if (User::current()->UseMoney(100) == 0) {
      User::current()->dogs(0)->ClearCD(kPlayCD);
      EventMgr::Instance().Response(kEventUserInfoChange);
      FinishDoing();
    }
  }

  if(item_type_ == "food") {
    if (User::current()->UseMoney(100) == 0) {
      EventMgr::Instance().Response(kEventUserInfoChange);
      User::current()->dogs(0)->ClearCD(kFeedCD);
      FinishDoing();
    }
  }

}

void MultiSpriteMenuItem::ClickDoneItemCallback(CCObject* sender) {
  ChangeShowItem("normal");
}

void MultiSpriteMenuItem::ChangeDoingItemCallback(CCObject* sender) {
  ChangeShowItem("doing");
  StartCD();
}

void MultiSpriteMenuItem::OnTime(float f) {
  char str[32] = {};
  snprintf(str, sizeof(str), "%d%d:%d%d", cd_time_/60/10, cd_time_/60%10, cd_time_%60/10, cd_time_%60%10);
  cd_time_--;
  if(cd_time_ > 0) {
    label_cd_->setString(str);
  } else {
    FinishDoing();
  }
}

void MultiSpriteMenuItem::FinishDoing() {
  cd_time_ = 0;
  label_cd_->setString("");
  EventMgr::Instance().Response(kEventDogAttrChange);
  ChangeShowItem("done");
  unschedule(schedule_selector(MultiSpriteMenuItem::OnTime));
}

void MultiSpriteMenuItem::StartCD() {
  if(item_type_ == "train") {
    cd_time_ = User::current()->dogs(0)->RemainTime(kTrainCD);
  }

  if(item_type_ == "play") {
    cd_time_ = User::current()->dogs(0)->RemainTime(kPlayCD);
  }

  if(item_type_ == "food") {
    cd_time_ = User::current()->dogs(0)->RemainTime(kFeedCD);
  }
  if(cd_time_ > 1) {
    this->schedule(schedule_selector(MultiSpriteMenuItem::OnTime), 1.0f, (unsigned int)cd_time_, 0.0f);
  }
}

void MultiSpriteMenuItem::ChangeShowItem(std::string item) {
  if(item == "normal") {
    normal_item_->setEnabled(true);
    normal_item_->setVisible(true);
    doing_item_->setEnabled(false);
    doing_item_->setVisible(false);
    done_item_->setEnabled(false);
    done_item_->setVisible(false);
  }
  else if(item == "doing") {
    normal_item_->setEnabled(false);
    normal_item_->setVisible(false);
    doing_item_->setEnabled(true);
    doing_item_->setVisible(true);
    done_item_->setEnabled(false);
    done_item_->setVisible(false);
  }
  else if(item == "done") {
    normal_item_->setEnabled(false);
    normal_item_->setVisible(false);
    doing_item_->setEnabled(false);
    doing_item_->setVisible(false);
    done_item_->setEnabled(true);
    done_item_->setVisible(true);
  }
}


//FSMenuItem

FSMenuItem * FSMenuItem::create(std::string sprite_name, int item_type, int data_index) {
  CCSprite* sprite = CCSprite::create();
  sprite->initWithSpriteFrameName(sprite_name.c_str());
  FSMenuItem *ret = new FSMenuItem();
  if (ret) {
    ret->autorelease();
  } else {
    delete ret;
    ret = NULL;
    return ret;
  }
  ret->initWithNormalSprite(sprite, sprite, sprite, ret, menu_selector(FSMenuItem::ItemClickCallback));
  ret->InitData(item_type, data_index);
  return ret;
}

void FSMenuItem::InitData(int item_type, int data_index) {
  const dogrun2::UIItemConf* cfg = NULL;
  switch(item_type) {
  case dogrun2::kItemFeed:
    {
      cfg = &(GetUIFeedCfg()->conf(data_index));
      break;
    }
  case dogrun2::kItemTrain:
    {
      cfg = &(GetUITrainCfg()->conf(data_index));
      break;
    }
  case dogrun2::kItemPlay:
    {
      cfg = &(GetUIPlayCfg()->conf(data_index));
      break;
    }
  default:
    return;
  }
  item_type_ = item_type;
  data_index_ = data_index;

  if(cfg->has_title()) {
    CCLabelTTF *label_desc = CCLabelTTF::create(
      cfg->title().desc().c_str(),
      cfg->title().font().c_str(),
      cfg->title().font_size());

    addChild(label_desc, 1);
    label_desc->setAnchorPoint(ccp(0,0));
    label_desc->setPosition(ccp(cfg->title().pos_x(), cfg->title().pos_y()));

    if(cfg->title().font_color_size() > 0) {
      label_desc->setColor(ccc3(
        cfg->title().font_color().Get(0),
        cfg->title().font_color().Get(1),
        cfg->title().font_color().Get(2)));
    }
  }

  if(cfg->has_strong()) {
    CCLabelTTF *label_strong_desc = CCLabelTTF::create(
      cfg->strong().desc().c_str(),
      cfg->strong().font().c_str(),
      cfg->strong().font_size());
    addChild(label_strong_desc, 1);
    label_strong_desc->setAnchorPoint(ccp(0,0));
    label_strong_desc->setPosition(ccp(cfg->strong().pos_x(), cfg->strong().pos_y()));
    if(cfg->strong().font_color_size() > 0) {
      label_strong_desc->setColor(ccc3(
        cfg->strong().font_color().Get(0),
        cfg->strong().font_color().Get(1),
        cfg->strong().font_color().Get(2)));
    }
  }

  if(cfg->has_speed()) {
    CCLabelTTF *label_speed_desc = CCLabelTTF::create(
      cfg->speed().desc().c_str(),
      cfg->speed().font().c_str(),
      cfg->speed().font_size());
    addChild(label_speed_desc, 1);
    label_speed_desc->setAnchorPoint(ccp(0,0));
    label_speed_desc->setPosition(ccp(cfg->speed().pos_x(), cfg->speed().pos_y()));
    if(cfg->speed().font_color_size() > 0) {
      label_speed_desc->setColor(ccc3(
        cfg->speed().font_color().Get(0),
        cfg->speed().font_color().Get(1),
        cfg->speed().font_color().Get(2)));
    }
  }

  if(cfg->has_exp()) {
    CCLabelTTF *label_exp_desc = CCLabelTTF::create(
      cfg->exp().desc().c_str(),
      cfg->exp().font().c_str(),
      cfg->exp().font_size());
    addChild(label_exp_desc, 1);
    label_exp_desc->setAnchorPoint(ccp(0,0));
    label_exp_desc->setPosition(ccp(cfg->exp().pos_x(), cfg->exp().pos_y()));
    if(cfg->exp().font_color_size() > 0) {
      label_exp_desc->setColor(ccc3(
        cfg->exp().font_color().Get(0),
        cfg->exp().font_color().Get(1),
        cfg->exp().font_color().Get(2)));
    }
  }

  if(cfg->has_icon()) {
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(cfg->icon().icon().c_str());
    addChild(sprite, 1);
    sprite->setScale((float)cfg->icon().scale()/100.0f);
    sprite->setAnchorPoint(ccp(0,0));
    sprite->setPosition(ccp(cfg->icon().pos_x(), cfg->icon().pos_y()));
  }

  if(cfg->has_icon_num()) {
    CCLabelTTF *label_icon_num = CCLabelTTF::create(
      cfg->icon_num().desc().c_str(),
      cfg->icon_num().font().c_str(),
      cfg->icon_num().font_size());
    addChild(label_icon_num, 1);
    label_icon_num->setAnchorPoint(ccp(0,0));
    label_icon_num->setPosition(ccp(cfg->icon_num().pos_x(), cfg->icon_num().pos_y()));
    if(cfg->icon_num().font_color_size() > 0) {
      label_icon_num->setColor(ccc3(
        cfg->icon_num().font_color().Get(0),
        cfg->icon_num().font_color().Get(1),
        cfg->icon_num().font_color().Get(2)));
    }
  }

  if(cfg->has_intimacy()) {
    CCLabelTTF *label_intimacy_desc = CCLabelTTF::create(
      cfg->intimacy().desc().c_str(),
      cfg->intimacy().font().c_str(),
      cfg->intimacy().font_size());
    addChild(label_intimacy_desc, 1);
    label_intimacy_desc->setAnchorPoint(ccp(0,0));
    label_intimacy_desc->setPosition(ccp(cfg->intimacy().pos_x(), cfg->intimacy().pos_y()));
    if(cfg->intimacy().font_color_size() > 0) {
      label_intimacy_desc->setColor(ccc3(
        cfg->intimacy().font_color().Get(0),
        cfg->intimacy().font_color().Get(1),
        cfg->intimacy().font_color().Get(2)));
    }
  }
  return ;
}

void FSMenuItem::ItemClickCallback(CCObject* sender) {
  int ret = 0;
  switch(item_type_) {
  case dogrun2::kItemFeed:
    {
      ret = User::current()->dogs(0)->Feed(data_index_);
      if(ret >= 0) {
        EventMgr::Instance().Response(kEventUserInfoChange);
        EventMgr::Instance().Response(kEventClickBackItem);
        EventMgr::Instance().Response(kEventChangeToFeedingItem);
      }
      break;
    }
  case dogrun2::kItemTrain:
    {
      ret = User::current()->dogs(0)->Train(data_index_);
      if(ret >= 0) {
        EventMgr::Instance().Response(kEventUserInfoChange);
        EventMgr::Instance().Response(kEventClickBackItem);
        EventMgr::Instance().Response(kEventChangeToTrainingItem);
      }
      break;
    }
  case dogrun2::kItemPlay:
    {
      ret = User::current()->dogs(0)->Play(data_index_);
      if(ret >= 0) {
        EventMgr::Instance().Response(kEventUserInfoChange);
        EventMgr::Instance().Response(kEventClickBackItem);
        EventMgr::Instance().Response(kEventChangeToPlayingItem);
      }
      break;
    }
  default:
    return;
  }

}

