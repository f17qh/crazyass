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
  LocalUser();
  int Load(const char *path);
  void Flush();
  int EventLock(int stageid);
protected:
  std::string path_;
  CSJson::Value root_;
  CSJson::Value eventlock_;

  int AddUser();
};

LocalUser::LocalUser() {
  heart_ = 10;
  stageid_ = 1;

  for (int i = 0; i < 6; i++) {
    eventlock_[i] = 1;
  }
}

int LocalUser::EventLock(int stageid) {
  return eventlock_[stageid - 1].asInt();
}

int LocalUser::AddUser() {
  root_["heart"] = heart_;
  root_["stage"] = stageid_;
  root_["eventlock"] = eventlock_;
  return 0;
}

int LocalUser::Load(const char *path) {
  CSJson::Reader reader;
  path_ = path;

  if (!CCFileUtils::sharedFileUtils()->isFileExist(std::string(path))) {
    return AddUser();
  }

  unsigned long size = 4096;
  unsigned char *buf = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &size);

  if (!reader.parse(std::string((char *)buf), root_, false)) {
    CCLOG("parse %s error", path);
    return -1;
  }

  stageid_ = root_.get("stage", 0).asInt();
  heart_ = root_.get("heart", 0).asInt();
  eventlock_ = root_["eventlock"];
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
  int fd = _open(path_.c_str(), _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE);
#else
  int fd = open(path_.c_str(), O_RDWR | O_CREAT);
#endif

#ifdef WIN32
  _write(fd, content.c_str(), content.size());
  _close(fd);
#else
    write(fd, content.c_str(), content.size());
    close(fd);
#endif
}

class RemoteUser : public User {
public:
  RemoteUser() {
      for (size_t i = 0; i < sizeof(event_lock_) / sizeof(event_lock_[0]); i++) {
          event_lock_[i] = 1;
      }
      userid_ = "";
  };
  int Load(const char *path);
  void Flush();
  int EventLock(int stageid) {
    return event_lock_[stageid - 1];
  }
  void set_eventlock(int stageid, int lock) {
    event_lock_[stageid - 1] = lock;
  }
protected:
  int event_lock_[10];
};

void CAWriteFile(char *file, char *content);
void CAReadFile(char *file, char *content, size_t);

void RemoteUser::Flush() {
  if (userid_.size() == 0)
    return;

  CSJson::FastWriter writer;
  CSJson::Value value;
  value["userid"] = userid_;
  std::string content = writer.write(value);
  CAWriteFile("userdata", (char *)content.c_str());
}

int RemoteUser::Load(const char *path) {
  char buf[512];
  memset(buf, 0, sizeof(buf));
  CAReadFile("userdata", buf, 512);
  if (strlen(buf) == 0) {
    CCLOG("read userdata faild.");
    return -1;
  }

  CSJson::Reader reader;
  CSJson::Value value;
  if (!reader.parse(std::string((char *)buf), value, false)) {
    CCLOG("parse %s error", path);
    return -1;
  }

  userid_ = value.get("userid", "").asString();
  CCLOG("userid %s", userid_.c_str());
  return 0;
}

static User * current_user_ = NULL;
User * User::CurrentUser() {
  if (current_user_ == NULL) {
    current_user_ = new RemoteUser();
    current_user_->Load("user.json");
  }
  return current_user_;
}
