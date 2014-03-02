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
#include "cocos2d.h"
#include "lib_json/json_lib.h"

USING_NS_CC;

class LocalUser : public User {
public:
  int Load(const char *path);
  void Flush();
protected:
  std::string path_;
  CSJson::Value root_;
};

int LocalUser::Load(const char *path) {
  CSJson::Reader reader;
  path_ = path;

  unsigned long size = 4096;
  unsigned char *buf = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &size);

  if (!reader.parse(std::string((char *)buf), root_, false)) {
    CCLOG("parse %s error", path);
    return -1;
  }

  stageid_ = root_.get("stage", 0).asInt();
  heart_ = root_.get("heart", 0).asInt();
  CCLOG("Tset %d %d", stageid_, heart_);
  delete buf;
  return 0;
}

void LocalUser::Flush() {
  CSJson::StyledWriter writer;

  root_["heart"] = heart_;
  root_["stage"] = stageid_;
  std::string content = writer.write(root_);
  CCLOG("Flush %s to %s\n", content.c_str(), path_.c_str());
#ifdef WIN32
  int fd = _open(path_.c_str(), _O_RDWR);
#else
  int fd = open(path_.c_str(), O_RDWR);
#endif

  _write(fd, content.c_str(), content.size());
  _close(fd);
}

static User * current_user_ = NULL;
User * User::CurrentUser() {
  if (current_user_ == NULL) {
    current_user_ = new LocalUser();
    current_user_->Load("etc/user.json");
  }
  return current_user_;
}