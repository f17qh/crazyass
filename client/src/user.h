#pragma once
#include "json\json.h"
class User
{
public:
  static User * CurrentUser();

  int stageid() { return stageid_; }
  void set_stageid(int id) { stageid_ = id;}
  int heart() { return heart_; }
  void set_heart(int heart) { heart_ = heart; }

  // save user data to disk
  void Flush();
  int Load(const char *path);
protected:

  int stageid_;
  int heart_;
};

