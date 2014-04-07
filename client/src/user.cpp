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
  int Load();
  void Flush();
  int EventLock(int stageid);
  void set_eventlock(int stageid, int lock);
  virtual void set_stageid(int id);
  virtual void set_heart(int heart);
  virtual int UseHeart(int count);

protected:
  std::string encode(const std::string &str);
  std::string decode(const std::string &str);
  std::string path_;
  CSJson::Value root_;
  CSJson::Value eventlock_;

  int AddUser();
};

std::string LocalUser::encode(const std::string &strString) {
  int nByteSrc = strString.length();
  std::string pszSource = strString;

  int i = 0;
  for(i; i < nByteSrc; i++)
    if( pszSource[i] < 0 || pszSource[i] > 127 )
      throw "can not encode Non-ASCII characters";

  const char *enkey = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string pszEncode(nByteSrc*4/3 + 4, '\0');
  int nLoop = nByteSrc %3 == 0 ? nByteSrc : nByteSrc - 3;
  int n = 0;
  for(i=0; i < nLoop; i+=3 ) {
    pszEncode[n] = enkey[pszSource[i]>>2];
    pszEncode[n+1] = enkey[((pszSource[i]&3)<<4) | ((pszSource[i+1] & 0xF0)>>4)];
    pszEncode[n+2] = enkey[((pszSource[i+1] & 0x0f)<<2) | ((pszSource[i+2] & 0xc0 )>>6)];
    pszEncode[n+3] = enkey[pszSource[i+2] & 0x3F];
    n += 4;
  }

  switch(nByteSrc%3) {
  case 0:
    pszEncode[n] = '\0';
    break;

  case 1:
    pszEncode[n] = enkey[pszSource[i]>>2];
    pszEncode[n+1] = enkey[((pszSource[i]&3)<<4) | ((0&0xf0)>>4)];
    pszEncode[n+2] = '=';
    pszEncode[n+3] = '=';
    pszEncode[n+4] = '\0';
    break;

  case 2:
    pszEncode[n] = enkey[pszSource[i]>>2];
    pszEncode[n+1] = enkey[((pszSource[i]&3)<<4) | ((pszSource[i+1]&0xf0)>>4)];
    pszEncode[n+2] = enkey[(( pszSource[i+1]&0xf)<<2 ) | ((0&0xc0)>>6)];
    pszEncode[n+3] = '=';
    pszEncode[n+4] = '\0';
    break;
  }

  return pszEncode.c_str();
}

std::string LocalUser::decode(const std::string &strString) {
  int nByteSrc = strString.length();
  if(nByteSrc == 0) {
    return "";
  }
  std::string pszSource = strString;

  const int dekey[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    62, // '+'
    -1, -1, -1,
    63, // '/'
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
    -1, -1, -1, -1, -1, -1, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
    -1, -1, -1, -1, -1, -1,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
  };

  if(nByteSrc%4 != 0)
    throw "bad base64 string";

  std::string pszDecode(nByteSrc*3/4+4, '\0');
  int nLoop = pszSource[nByteSrc-1]  == '=' ? nByteSrc - 4 : nByteSrc;
  int b[4];
  int i = 0, n = 0;
  for(i = 0; i < nLoop; i += 4 ) {
    b[0] = dekey[pszSource[i]];        b[1] = dekey[pszSource[i+1]];
    b[2] = dekey[pszSource[i+2]];    b[3] = dekey[pszSource[i+3]];
    if(b[0] == -1 || b[1] == -1 || b[2] == -1 || b[3] == -1)
      throw "bad base64 string";

    pszDecode[n] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
    pszDecode[n+1] = ((b[1] & 0xf) << 4) | ((b[2] & 0x3c) >> 2);
    pszDecode[n+2] =  ((b[2] & 0x3) << 6) | b[3];

    n+=3;
  }

  if( pszSource[nByteSrc-1] == '=' && pszSource[nByteSrc-2] == '=' ) {
    b[0] = dekey[pszSource[i]];
    b[1] = dekey[pszSource[i+1]];
    if(b[0] == -1 || b[1] == -1)
      throw "bad base64 string";

    pszDecode[n] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
    pszDecode[n+1] = '\0';
  }

  if( pszSource[nByteSrc-1] == '=' && pszSource[nByteSrc-2] != '=' ) {
    b[0] = dekey[pszSource[i]];
    b[1] = dekey[pszSource[i+1]];
    b[2] = dekey[pszSource[i+2]];
    if(b[0] == -1 || b[1] == -1 || b[2] == -1)
      throw "bad base64 string";

    pszDecode[n] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
    pszDecode[n+1] = ((b[1] & 0xf) << 4) | ((b[2] & 0x3c) >> 2);
    pszDecode[n+2] = '\0';
  }

  if( pszSource[nByteSrc-1] != '=' && pszSource[nByteSrc-2] != '=' )
    pszDecode[n] = '\0';

  return pszDecode;
}

int LocalUser::UseHeart(int count) {
  if (heart_ < count)
    return -1;
  heart_ -= count;
  Flush();
  return 0;
}

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
  Flush();
  return 0;
}
void LocalUser::set_stageid(int id) {
  if(stageid_ < id) {
    stageid_ = id;
    Flush();
  }
}

void LocalUser::set_heart(int heart) { 
  heart_ = heart;
  Flush();
}
void LocalUser::set_eventlock(int stageid, int lock) {
  if(eventlock_[stageid - 1] < lock) {
    eventlock_[stageid - 1] = lock;
    Flush();
  }
}
int LocalUser::Load() {
  CSJson::Reader reader;

  std::string tmp = CCUserDefault::sharedUserDefault()->getStringForKey("user_data");
  std::string buf = decode(tmp);
  CCLOG("load:%s\n", buf.c_str());
  if(buf.size() == 0) {
    return AddUser();
  }
  if (!reader.parse(buf, root_, false)) {
    CCLOG("parse error");
    return -1;
  }

  stageid_ = root_.get("stage", 0).asInt();
  heart_ = root_.get("heart", 0).asInt();
  eventlock_ = root_["eventlock"];
  CCLOG("Tset %d %d", stageid_, heart_);

  return 0;
}

void LocalUser::Flush() {
  CSJson::StyledWriter writer;
  root_["heart"] = heart_;
  root_["stage"] = stageid_;
  root_["eventlock"] = eventlock_;
  std::string tmp = writer.write(root_);
  std::string buf = encode(tmp);
  CCLOG("Flush %s to %s\n", buf.c_str(), path_.c_str());
  CCUserDefault::sharedUserDefault()->setStringForKey("user_data", buf);
  CCUserDefault::sharedUserDefault()->flush();
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
    current_user_ = new LocalUser();
    current_user_->Load();
  }
  return current_user_;
}
