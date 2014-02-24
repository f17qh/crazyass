#ifndef  __DOGRUN2_COMMON__
#define  __DOGRUN2_COMMON__

#include <string>
using std::string;
namespace google {
  namespace protobuf {
    class Message;
  }
}

int ParseFromFile(const string& filepath,
                  google::protobuf::Message *output);

#endif
