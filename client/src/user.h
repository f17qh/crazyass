#pragma once
#include <string>

class User
{
public:
  static User * CurrentUser();

  int stageid() { return stageid_; }
  void set_stageid(int id) { stageid_ = id;}
  int heart() { return heart_; }
  void set_heart(int heart) { heart_ = heart; }
  virtual int EventLock(int stageid) = 0;
  virtual void set_eventlock(int stageid, int lock) = 0;
  const std::string& userid() { return userid_; }
  void set_userid(const std::string& userid) { userid_ = userid; }
  // save user data to disk
  virtual void Flush() = 0;
  virtual int Load(const char *path) = 0;

  int UseHeart(int count) {
    if (heart_ < count)
      return -1;
    heart_ -= count;
    return 0;
  }
protected:
  int stageid_;
  int heart_;
  std::string userid_;
};

