#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <cocos2d.h>
#include "user.h"

int User::Load(const char *path) {
  Json::Reader reader;
  Json::Value root;

#ifdef WIN32
  int fd = _open(path, _O_RDONLY);
#else
  int fd = open(path, O_RDONLY);
#endif

  char buf[4096];
  _read(fd, buf, 4096);

  if (!reader.parse(std::string(buf), root, false)) {
    CCLOG("parse %s error", path);
    return -1;
  }

  stageid_ = root.get("stage", 0).asInt();
  heart_ = root.get("heart", 0).asInt();
  CCLOG("Tset %d %d", stageid_, heart_);
  return 0;
}

void User::Flush() {

}

static User * current_user_ = NULL;
User * User::CurrentUser() {
  if (current_user_ == NULL) {
    current_user_ = new User();
    current_user_->Load("etc/user.json");
  }
  return current_user_;
}