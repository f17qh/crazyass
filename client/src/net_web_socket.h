// #ifndef  __NET_WEB_SOCKET__
// #define  __NET_WEB_SOCKET__

// #include "cocos-ext.h"
// #include <cocos2d.h>
// #include "user.h"
// #include "network/WebSocket.h"
// #include "lib_json/json_lib.h"
// #include "common.h"

// USING_NS_CC_EXT;

// class MyDelegate : public CCObject,
//                    public WebSocket::Delegate, 
//                    public CADelegate {
// public:
//   void Init(WebSocket *ws, CATarget *target) {
//     ws_ = ws;
//     target_ = target;
//     seq_ = 12345;
//     lastseq_ = 0;
//     recvcount_ = 0;
//     recvflag_ = false;
//   }

//   virtual void onOpen(WebSocket* ws) {
//     CCLOG("%s", __FUNCTION__);
//     if (target_) {
//       target_->CAOpen();
//       //recvflag_ = true;
//     }
//   }

//   virtual void onClose(WebSocket* ws) {
//     CCLOG("%s", __FUNCTION__);
//     delete this;
//   }

//   virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
//     CCLOG("%s", __FUNCTION__);
//     delete this;
//   }

//   virtual void onMessage(WebSocket* ws, const WebSocket::Data& data) {
//     CCLOG("%s", __FUNCTION__);
//     CCLOG("%s", data.bytes);

//     CSJson::Reader reader;
//     CSJson::Value result;
//     if (!reader.parse(std::string(data.bytes), result, false)) {
//       CCLOG("parse %s error", data.bytes);
//       return;
//     }

//     int seq = result.get("Seq", 0).asInt();
//     if (seq != lastseq_) {
//       CCLOG("invalid seq %d expect %d", seq, lastseq_);
//       return;
//     }

//     if (target_) {
//       target_->CARecv(result);
//       recvflag_ = true;
//     }

//     return;
//   }

//   bool CheckRecv() {
//     CCLOG("%s %d", __FUNCTION__, recvcount_);
//     if (recvflag_) {
//       if (target_) {
//         target_->CARecvDone();
//         CCDirector::sharedDirector()->getScheduler()->
//           unscheduleSelector(schedule_selector(MyDelegate::Update),(CCObject*)this);
//       }
//       return true;
//     } else {
//       if (++recvcount_ >= NETWORK_TIMEOUT) {
//         target_->CARecvTimeout();
//         return true;
//       }
//     }
//     return false;
//   }

//   void Update(float dt) {
//     CheckRecv();
//   }

//   void SendServer(CATarget *target) {
//     CSJson::Value value;
//     value["userid"] = 1;
//     value["version"] = 0x010000;
//     value["cmd"] = 10;
//     value["seq"] = seq_;
//     lastseq_ = seq_;
//     seq_++;

//     CSJson::FastWriter writer;
//     std::string content = writer.write(value);
//     recvflag_ = false;
//     recvcount_ = 0;
//     target_ = target;
//     ws_->send(content);
//     CCDirector::sharedDirector()->getScheduler()->
//       unscheduleSelector(schedule_selector(MyDelegate::Update),(CCObject*)this);
//     CCDirector::sharedDirector()->getScheduler()->
//       scheduleSelector(schedule_selector(MyDelegate::Update),(CCObject*)this,1.0f,SCHEDULE_TIMEOUT,1.0f,false);
//   }

//   void SendServer(CSJson::Value& value, CATarget *target) {
//     value["seq"] = seq_;
//     lastseq_ = seq_;
//     seq_++;

//     CSJson::FastWriter writer;
//     std::string content = writer.write(value);
//     recvflag_ = false;
//     recvcount_ = 0;
//     target_ = target;
//     ws_->send(content);
//   }
// private:
//   WebSocket *ws_;
//   CATarget *target_;
//   bool recvflag_;
//   int recvcount_;
//   int seq_;
//   int lastseq_;
// };

// class MyTarget:public CATarget {
// public:
//   virtual void CARecv(const CSJson::Value& result);
//   virtual void CARecvDone() {}
//   virtual void CARecvTimeout();
//   virtual void CAOpen();
// };

// class CAWS {
//  public:
//   static CAWS * Instance();
//   static void Release();
//   CAWS() {
//     delegate_ = new MyDelegate;
//     ws_ = new WebSocket;
//     target_ = new MyTarget;
//   }

//   ~CAWS() {
//     caws_ = NULL;
//     // delete delegate_;
//     delete ws_;
//   }

//   void Init(const char *addr) {
//     delegate_->Init(ws_, target_);
//     // close if connect
//     if (ws_->getReadyState() == WebSocket::kStateOpen)
//       ws_->close();
//     bool t = ws_->init(*delegate_, addr);
//     if (!t) {
//       CCLOG("connect server faild");
//     }
//     // delegate_->SendServer(target_);
//   }

//   MyDelegate *delegate() {
//     return delegate_;
//   }

//  private:
//   static CAWS *caws_;
//   MyDelegate *delegate_;
//   WebSocket *ws_;
//   CATarget *target_;
// };

// #endif
