#include <string>
#include "assert.h"
#include "time.h"
#include "cocos2d.h"
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"
#include "common.h"

using google::protobuf::io::FileInputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::TextFormat;
using std::string;

int ParseFromFile(const string& filepath, google::protobuf::Message *output) {
#ifdef WIN32
  int fd = _open(filepath.c_str(), _O_RDONLY);
#else
  int fd = open(filepath.c_str(), O_RDONLY);
#endif
  if (fd < 0) {
    CCLOGERROR("open %s error\n", filepath.c_str());
    return -1;
  }
  ZeroCopyInputStream *input = new FileInputStream(fd);
  bool succ = TextFormat::Parse(input, output);
  assert(succ);
#ifdef WIN32
  _close(fd);
#else
  close(fd);
#endif
  string s = output->DebugString();
  CCLOG("Parse %s\n", s.c_str());
  return 0;
}

