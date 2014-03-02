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
};

