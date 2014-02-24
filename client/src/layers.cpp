#include "layers.h"
#include "event_mgr.h"
#include "protocol/uiconfig.pb.h"
#include "ui_config_init.h"

void DogLayer::MenuClickCallback(CCObject* sender) {
}

bool DogLayer::init() {
  if (!CCLayer::init())
    return false;

  bg_ = CCSprite::create("background.png");
  bg_->setAnchorPoint(ccp(0,0));
  bg_->setPosition(ccp(0, 0));
  this->addChild(bg_);

  dog_ = DogSprite::create();
  dog_->setAnchorPoint(ccp(0,0));
  dog_->setPosition(ccp(400, 240));
  this->addChild(dog_);
  return true;
}

void UserLayer::MailClickCallback(CCObject* sender) {

}

void UserLayer::GoldBarClickCallback(CCObject* sender) {

}

void UserLayer::NameClickCallback(CCObject* sender) {

}

void UserLayer::UpdataUserInfo(CCObject* sender) {
  char num[32] = {0};
  snprintf(num,sizeof(num),"%d", user_->money());
  gold_num_->setString(num);
  heart_progress_bar_.SetHeartProgressBar(user_->heart());
}

bool UserLayer::init() {
  if (!CCLayer::init())
    return false;

  user_ = User::current();

  EventMgr::Instance().Register(kEventUserInfoChange,
    this, callfuncO_selector(UserLayer::UpdataUserInfo));

  CCSprite* plank_sprite = CCSprite::create("plank.png");
  plank_sprite->setAnchorPoint(ccp(0,0));
  plank_sprite->setPosition(ccp((800-769)/2, 480-122));
  addChild(plank_sprite);

  CCSprite* gold_bar_sprite = CCSprite::create();
  gold_bar_sprite->initWithSpriteFrameName("gold_bar.png");
  CCMenuItemSprite* gold_bar_item = CCMenuItemSprite::create(
    gold_bar_sprite, gold_bar_sprite, gold_bar_sprite, this, menu_selector(UserLayer::GoldBarClickCallback));
  gold_bar_item->setAnchorPoint(ccp(0,0));
  gold_bar_item->setPosition(ccp(335,67));

  char num[32] = {0};
  snprintf(num,sizeof(num),"%d", user_->money());
  gold_num_ = CCLabelTTF::create( num, "Arial", 24);
  gold_num_->setAnchorPoint(ccp(0, 0));
  gold_num_->setPosition(ccp(90, 4));
  gold_bar_item->addChild(gold_num_, 3);

  CCSprite* achievement_sprite = CCSprite::create();
  achievement_sprite->initWithSpriteFrameName("down.png");
  CCMenuItemSprite* achievement_item = CCMenuItemSprite::create(
    achievement_sprite, achievement_sprite, achievement_sprite, 
    this, menu_selector(UserLayer::GoldBarClickCallback));
  achievement_item->setAnchorPoint(ccp(0,0));
  achievement_item->setPosition(ccp(287,62));

  char achievement_str[32] = {0};
  snprintf(achievement_str,sizeof(achievement_str),"%s", user_->title().c_str());
  CCLabelTTF* achievement_desc = CCLabelTTF::create( achievement_str, "WenQuanYi Zen Hei", 20);
  achievement_desc->setAnchorPoint(ccp(0, 0));
  achievement_desc->setPosition(ccp(150, 62));
  plank_sprite->addChild(achievement_desc, 3);

  char name_str[32] = {0};
  snprintf(name_str,sizeof(name_str),"%s", "clowl");
  CCLabelTTF* name_desc = CCLabelTTF::create( name_str, "Arial", 20);
  name_desc->setAnchorPoint(ccp(0, 0));
  name_desc->setPosition(ccp(150, 92));
  plank_sprite->addChild(name_desc, 3);

  CCSprite* heart_bar = CCSprite::create();
  heart_bar->initWithSpriteFrameName("like_bar.png");
  heart_bar->setAnchorPoint(ccp(0,0));
  heart_bar->setPosition(ccp(610, 67));
  plank_sprite->addChild(heart_bar, 2);

  CCSprite* heart = CCSprite::create();
  heart->initWithSpriteFrameName("like.png");
  heart->setAnchorPoint(ccp(0,0));
  heart->setPosition(ccp(-15, 0));
  heart_bar->addChild(heart, 2);

  heart_progress_bar_.SetMaxNum(20000);
  heart_progress_bar_.CreateSprite(heart_bar);
  heart_progress_bar_.SetHeartProgressBar(user_->heart());

  CCSprite* mail_sprite = CCSprite::create();
  mail_sprite->initWithSpriteFrameName("mail.png");

  CCMenuItemSprite* mail_item = CCMenuItemSprite::create(
  mail_sprite, mail_sprite, mail_sprite, this, menu_selector(UserLayer::MailClickCallback));
  mail_item->setAnchorPoint(ccp(0,0));
  mail_item->setPosition(ccp(0, -80));

  menu_ = CCMenu::create(mail_item, gold_bar_item, achievement_item, NULL);
  menu_->setAnchorPoint(ccp(0, 0));
  menu_->setPosition(ccp(0, 0));
  plank_sprite->addChild(menu_, 1);
  return true;
}

void MainMenuLayer::MenuClickCallback(CCObject* sender) {

}

bool MainMenuLayer::init() {
  if (!CCLayer::init())
    return false;

  InitMenuItem();

  return true;
}

void MainMenuLayer::InitMenuItem() {
  CCSprite* friend_sprite = CCSprite::create();
  friend_sprite->initWithSpriteFrameName("friend.png");

  CCSprite* step_sprite = CCSprite::create();
  step_sprite->initWithSpriteFrameName("step.png");

  CCSprite* warehouse_sprite = CCSprite::create();
  warehouse_sprite->initWithSpriteFrameName("warehouse.png");

  CCSprite* gold_sprite = CCSprite::create();
  gold_sprite->initWithSpriteFrameName("chest.png");

  CCSprite* shop_sprite = CCSprite::create();
  shop_sprite->initWithSpriteFrameName("shop.png");

  CCSprite* reward_sprite = CCSprite::create();
  reward_sprite->initWithSpriteFrameName("reward.png");

  CCMenuItemSprite* friend_item = CCMenuItemSprite::create(
    friend_sprite, friend_sprite, friend_sprite, this, menu_selector(MainMenuLayer::MenuClickCallback));
  
  CCMenuItemSprite* step_item = CCMenuItemSprite::create(
    step_sprite, step_sprite, step_sprite, this, menu_selector(MainMenuLayer::MenuClickCallback));

  CCMenuItemSprite* warehouse_item = CCMenuItemSprite::create(
    warehouse_sprite, warehouse_sprite, warehouse_sprite, this, menu_selector(MainMenuLayer::MenuClickCallback));

  CCMenuItemSprite* gold_item = CCMenuItemSprite::create(
    gold_sprite, gold_sprite, gold_sprite, this, menu_selector(MainMenuLayer::MenuClickCallback));

  CCMenuItemSprite* shop_item = CCMenuItemSprite::create(
    shop_sprite, shop_sprite, shop_sprite, this, menu_selector(MainMenuLayer::MenuClickCallback));

  CCMenuItemSprite* reward_item = CCMenuItemSprite::create(
    reward_sprite, reward_sprite, reward_sprite, this, menu_selector(MainMenuLayer::MenuClickCallback));

  menu_ = CCMenu::create( friend_item, step_item, warehouse_item,
    gold_item, shop_item, reward_item, NULL);
  menu_->setPosition(ccp(0, 0));
  // CCPoint base_Position = menu_->getPosition();
  int base_x = 350; int base_y = 40; int delt = 80;

  friend_item->setPosition   ( ccp(50,   base_y));
  step_item->setPosition     ( ccp(base_x + delt*1,  base_y));
  warehouse_item->setPosition( ccp(base_x + delt*2,  base_y));
  gold_item->setPosition     ( ccp(base_x + delt*3,  base_y));
  shop_item->setPosition     ( ccp(base_x + delt*4,  base_y));
  reward_item->setPosition   ( ccp(base_x + delt*5,  base_y));
  //main_menu_->alignItemsVertically();

  addChild(menu_);
}

void DogMenuLayer::GameItemClickCallback(CCObject* sender) {
}

bool DogMenuLayer::init() {
  if (!CCLayer::init())
    return false;

  InitMenuItem();
  return true;
}

void DogMenuLayer::InitMenuItem() {
  CCSprite* game_sprite = CCSprite::create();
  game_sprite->initWithSpriteFrameName("game.png");

  CCMenuItemSprite* game_item = CCMenuItemSprite::create(
    game_sprite, game_sprite, game_sprite, this, menu_selector(DogMenuLayer::GameItemClickCallback));

  int base_x = 8; int base_y = 10; int delt = 18; int last_item_x = 0;
  MultiSpriteMenuItem* food_items = MultiSpriteMenuItem::create(string("food"), ccp(base_x, base_y));

  last_item_x += food_items->GetItemWidth() + delt;
  MultiSpriteMenuItem* train_items = MultiSpriteMenuItem::create("train", ccp(base_x + last_item_x, base_y));

  last_item_x += train_items->GetItemWidth() + delt;
  MultiSpriteMenuItem* play_items = MultiSpriteMenuItem::create("play", ccp(base_x + last_item_x, base_y));

  last_item_x += play_items->GetItemWidth() + delt;
  game_item->setAnchorPoint(ccp(0, 0));
  game_item->setPosition ( ccp(base_x + last_item_x, base_y));

  CCArray* array = CCArray::create();
  food_items->AddToArray(array);
  train_items->AddToArray(array);
  play_items->AddToArray(array);
  array->addObject(game_item);

  menu_ = CCMenu::createWithArray(array);
  menu_->setAnchorPoint(ccp(0, 0));
  menu_->setPosition(ccp(0, 0));

  addChild(menu_);
  addChild(food_items);
  addChild(train_items);
  addChild(play_items);
}

void FeedMenuLayer::BackItemClickCallback(CCObject* sender) {
  EventMgr::Instance().Response(kEventClickBackItem);
}

bool FeedMenuLayer::init() {
  if (!CCLayer::init())
    return false;

  InitMenuItem();
  return true;
}

void FeedMenuLayer::InitMenuItem() {
  CCSprite* back_sprite = CCSprite::create();
  back_sprite->initWithSpriteFrameName("back.png");

  CCMenuItemSprite* back_item = CCMenuItemSprite::create(
    back_sprite, back_sprite, back_sprite, this, menu_selector(TrainMenuLayer::BackItemClickCallback));
  
  CCArray* item_arry = CCArray::create();
  item_arry->addObject(back_item);
  FSMenuItem* item = NULL;
  for(int i = 0; i < GetUITrainCfg()->conf_size(); ++i)
  {
    item = FSMenuItem::create("white_mid.png", dogrun2::kItemFeed, i);
    item_arry->addObject(item);
  }
  menu_ = CCMenu::createWithArray(item_arry);
  menu_->setAnchorPoint(ccp(0, 0));
  menu_->setPosition(ccp(0, 0));

  int base_x = 15; int base_y = 10; int delt = 10;
  int last_item_x = back_item->getContentSize().width + delt;

  back_item->setAnchorPoint(ccp(0, 0));
  back_item->setPosition(ccp(base_x, base_y));

  int menu_item_size = menu_->getChildrenCount();
  for(int i = 0; i < menu_item_size; ++i)
  {
    if(i == 0) continue;

    item = (FSMenuItem*)(menu_->getChildren()->objectAtIndex(i));
    item->setAnchorPoint(ccp(0, 0));
    item->setPosition(ccp(base_x + last_item_x, base_y));
    last_item_x += (int)item->getContentSize().width + delt;
  }
  addChild(menu_);
}

void TrainMenuLayer::BackItemClickCallback(CCObject* sender) {
  EventMgr::Instance().Response(kEventClickBackItem);
}

bool TrainMenuLayer::init() {
  if (!CCLayer::init())
    return false;

  InitMenuItem();
  return true;
}

void TrainMenuLayer::InitMenuItem() {
  CCSprite* back_sprite = CCSprite::create();
  back_sprite->initWithSpriteFrameName("back.png");

  CCMenuItemSprite* back_item = CCMenuItemSprite::create(
    back_sprite, back_sprite, back_sprite, this, menu_selector(TrainMenuLayer::BackItemClickCallback));
  
  CCArray* item_arry = CCArray::create();
  item_arry->addObject(back_item);
  FSMenuItem* item = NULL;
  for(int i = 0; i < GetUITrainCfg()->conf_size(); ++i)
  {
    item = FSMenuItem::create("white_mid.png", dogrun2::kItemTrain, i);
    item_arry->addObject(item);
  }
  menu_ = CCMenu::createWithArray(item_arry);
  menu_->setAnchorPoint(ccp(0, 0));
  menu_->setPosition(ccp(0, 0));

  int base_x = 15; int base_y = 10; int delt = 10;
  int last_item_x = back_item->getContentSize().width + delt;

  back_item->setAnchorPoint(ccp(0, 0));
  back_item->setPosition(ccp(base_x, base_y));

  int menu_item_size = menu_->getChildrenCount();
  for(int i = 0; i < menu_item_size; ++i)
  {
    if(i == 0) continue;

    item = (FSMenuItem*)(menu_->getChildren()->objectAtIndex(i));
    item->setAnchorPoint(ccp(0, 0));
    item->setPosition(ccp(base_x + last_item_x, base_y));
    last_item_x += (int)item->getContentSize().width + delt;
  }
  addChild(menu_);
}

void PlayMenuLayer::BackItemClickCallback(CCObject* sender) {
  EventMgr::Instance().Response(kEventClickBackItem);
}

bool PlayMenuLayer::init() {
  if (!CCLayer::init())
    return false;

  InitMenuItem();
  return true;
}

void PlayMenuLayer::InitMenuItem() {
  
  CCSprite* back_sprite = CCSprite::create();
  back_sprite->initWithSpriteFrameName("back.png");

  CCMenuItemSprite* back_item = CCMenuItemSprite::create(
    back_sprite, back_sprite, back_sprite, this, menu_selector(PlayMenuLayer::BackItemClickCallback));
  

  CCArray* item_arry = CCArray::create();
  item_arry->addObject(back_item);
  FSMenuItem* item = NULL;
  for(int i = 0; i < GetUIPlayCfg()->conf_size(); ++i)
  {
    item = FSMenuItem::create("white.png", dogrun2::kItemPlay, i);
    item_arry->addObject(item);
  }

  menu_ = CCMenu::createWithArray(item_arry);
  menu_->setAnchorPoint(ccp(0, 0));
  menu_->setPosition(ccp(0, 0));

  int base_x = 15; int base_y = 10; int delt = 35;
  int last_item_x = back_item->getContentSize().width + delt;

  back_item->setAnchorPoint(ccp(0, 0));
  back_item->setPosition(ccp(base_x, base_y));

  int menu_item_size = menu_->getChildrenCount();
  for(int i = 0; i < menu_item_size; ++i)
  {
    if(i == 0) continue;

    item = (FSMenuItem*)(menu_->getChildren()->objectAtIndex(i));
    item->setAnchorPoint(ccp(0, 0));
    item->setPosition(ccp(base_x + last_item_x, base_y));
    last_item_x += (int)item->getContentSize().width + delt;
  }

  addChild(menu_);
}
