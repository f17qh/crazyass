#include "assert.h"
#include "time.h"
#include "cocos2d.h"
#include "dog.h"
#include "common.h"

static dogrun2::FoodConfigArray *FoodCfg = NULL;
static dogrun2::TrainConfigArray *TrainCfg = NULL;
static dogrun2::PlayConfigArray *PlayCfg = NULL;
static dogrun2::DogLevelConfigArray *DogLevelCfg = NULL;

Dog::Dog() {
  attr_.set_name("FirstDog");
  memset(cd_, 0, sizeof(cd_));
}

time_t Dog::RemainTime(int type) {
  if(type < 0 || type >= kOperationCDNR) {
    return -1;
  }
  return cd_[type] - time(NULL);
}

int Dog::Feed(int feedtype) {
  time_t now = time(NULL);
  if (cd_[kFeedCD] > now) {
    CCLOGERROR("feed in cd.\n");
    return -1;
  }

  const dogrun2::FoodConfig& cfg = FoodCfg->cfg(feedtype);
  attr_.set_strong(attr_.strong() + cfg.strong());
  attr_.set_exp(attr_.exp() + cfg.exp());
  cd_[kFeedCD] = now + cfg.cd();
  owner_->set_money(owner_->money() - cfg.consume_gold());
  owner_->set_heart(owner_->heart() - cfg.consume_heart());
  TryLevelup();
  return 0;
}

int Dog::Train(int traintype) {
  time_t now = time(NULL);
  if (cd_[kTrainCD] > now) {
    CCLOGERROR("Train in cd.\n");
    return -1;
  }

  const dogrun2::TrainConfig& cfg = TrainCfg->cfg(traintype);
  attr_.set_speed(attr_.speed() + cfg.speed());
  attr_.set_exp(attr_.exp() + cfg.exp());
  cd_[kTrainCD] = now + cfg.cd();
  owner_->set_money(owner_->money() - cfg.consume_gold());
  owner_->set_heart(owner_->heart() - cfg.consume_heart());
  TryLevelup();
  return 0;
}

int Dog::Play(int playtype) {
  time_t now = time(NULL);
  if (cd_[kPlayCD] > now) {
    CCLOGERROR("Play in cd.\n");
    return -1;
  }

  const dogrun2::PlayConfig& cfg = PlayCfg->cfg(playtype);
  attr_.set_exp(attr_.exp() + cfg.exp());
  attr_.set_intimacy(attr_.intimacy() + cfg.intimacy());
  cd_[kPlayCD] = now + cfg.cd();
  owner_->set_money(owner_->money() - cfg.consume_gold());
  owner_->set_heart(owner_->heart() - cfg.consume_heart());
  TryLevelup();
  return 0;
}

int Dog::TryLevelup() {
  const dogrun2::DogLevelConfig& cur = DogLevelCfg->cfg(attr_.lv());
  while (attr_.exp() >= cur.exp()) {
    if (attr_.lv() < DogLevelCfg->cfg_size()) {
      attr_.set_lv(attr_.lv() + 1);
      attr_.set_exp(attr_.exp() - cur.exp());
      CCLOG("levelup %d -> %d\n", attr_.lv() - 1, attr_.lv());
    } else {
      CCLOGERROR("Dog reaches max level %d\n", attr_.lv());
      return -1;
    }
  }

  return 0;
}

int Dog::ClearCD(int cdtype) {
  if (cdtype >= kOperationCDNR)
    return -1;

  time_t now = time(NULL);
  if (cd_clear_count_[cdtype] < 10) {
    // first clear
    if (cd_clear_count_[cdtype] == 0) {
      cd_clear_time_[cdtype] = now;
    }
  }

  // if beyond one day
  time_t last = cd_clear_time_[cdtype];
  if (now - last >= kTimeDaySeconds) {
    cd_clear_count_[cdtype] = 0;
    cd_clear_time_[cdtype] = now;
  } else {
    return -1;
  }

  cd_clear_count_[cdtype]++;
  cd_[cdtype] = 0;
  return 0;
}

User* User::current_;

void User::InitCurrent() {
  User *u = new User();
  assert(u != NULL);

  u->set_id("FirstUser");
  u->set_title("吉娃娃饲养家");
  u->set_money(10000);
  u->set_heart(10000);

  Dog *dog = new Dog();
  assert(u != NULL);
  u->AddDog(dog);
  dog->set_owner(u);

  User::set_current(u);
}

int User::AddDog(Dog *dog) {
  dogs_.push_back(dog);
  return 0;
}

int User::UseMoney(int money) {
  if (money_ < money)
    return -1;

  CCLOGINFO("Use Money %d -> %d\n", money_, money_ - money);
  money_ -= money;
  return 0;
}

int DogRunInit() {
  printf("%s\n", __FUNCTION__);
  if (!FoodCfg) {
    FoodCfg = new dogrun2::FoodConfigArray;
    assert(FoodCfg != NULL);
    ParseFromFile("../common/etc/food.cfg", FoodCfg);
  }
  if (!TrainCfg) {
    TrainCfg = new dogrun2::TrainConfigArray;
    assert(TrainCfg != NULL);
    ParseFromFile("../common/etc/train.cfg", TrainCfg);
  }
  if (!PlayCfg) {
    PlayCfg = new dogrun2::PlayConfigArray;
    assert(PlayCfg != NULL);
    ParseFromFile("../common/etc/play.cfg", PlayCfg);
  }

  if (!DogLevelCfg) {
    DogLevelCfg = new dogrun2::DogLevelConfigArray;
    assert(DogLevelCfg != NULL);
    ParseFromFile("../common/etc/doglevel.cfg", DogLevelCfg);
  }
  
  return 0;
}
