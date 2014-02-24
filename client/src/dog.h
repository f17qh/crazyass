#ifndef  __DOGRUN2_DOG__
#define  __DOGRUN2_DOG__

#include <string>
#include <vector>
#include "protocol/dogattr.pb.h"

using std::string;
using std::vector;
using dogrun2::DogAttr;

const int kDefalutFeedCD = 60;
const int kDefaultTrainCD = 60;
const int kDefaultPlayCD = 60;

enum {
  kFeedCD = 0,
  kPlayCD = 1,
  kTrainCD = 2,
  kOperationCDNR,
};

enum {
  kDogStarD = 1,
  kDogStarC = 2,
  kDogStarB = 3,
  kDogStarA = 4,
  kDogStarS = 5,
  kDogStarSS = 6,
  kDogStarSSS = 7,
};

class Dog;

class User {
public:
  // attr
  const string& id() {return id_;};
  const string& title(){return title_;};
  int money(){return money_;};
  int heart(){return heart_;};

  static User *current() {return current_;};
  static void InitCurrent();
  static void set_current(User *u) {current_ = u;}

  void set_id(const string& id) {id_ = id;}
  void set_title(const string& title) {title_ = title;}
  void set_money(int money) {money_ = money;}
  void set_heart(int heart) {heart_ = heart;}

  int AddDog(Dog *dog);
  Dog *dogs(int idx) {return dogs_[idx];}
  size_t dogs_count(void) {return dogs_.size();}

  int UseMoney(int money);
protected:
  string id_;
  string title_;
  int money_;
  int heart_;

  vector<Dog *> dogs_;
  static User* current_;
};

const int kTimeDaySeconds = 60 * 1024 * 1024;

class Dog {
public:
  Dog();
  int Feed(int foodtype);
  int Train(int traintype);
  int Play(int playtype);
  int ClearCD(int cdtype);

  const DogAttr& attr() {return attr_;}
  User *owner();

  void set_owner(User *owner) {owner_ = owner;}
  time_t RemainTime(int type);
  time_t TrainRemainTime(){return cd_[kTrainCD] - time(NULL);};
  time_t FeedRemainTime(){return cd_[kFeedCD] - time(NULL);};
  time_t PlayRemainTime(){return cd_[kPlayCD] - time(NULL);};
protected:
  DogAttr attr_;
  time_t cd_[kOperationCDNR];
  time_t cd_clear_time_[kOperationCDNR];
  int cd_clear_count_[kOperationCDNR];
  User *owner_;

  int TryLevelup();
};

int DogRunInit();

#endif // ! __DOGRUN2_DOG__
