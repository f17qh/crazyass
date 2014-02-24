#pragma once
#include "cocos2d.h"
#include <map>
#include <list>
USING_NS_CC;

enum {
  kEventClickDogMenu = 1,
  kEventClickTrainItem = 2,
  kEventClickPlayItem = 3,
  kEventClickFeedItem = 4,
  kEventClickBackItem = 5,
  kEventDogAttrChange = 6,
  kEventUserInfoChange = 7,
  kEventChangeToTrainingItem = 8,
  kEventChangeToPlayingItem = 9,
  kEventChangeToFeedingItem = 10,
};

class ResponserInfo {
public:
  ResponserInfo(){}
  ResponserInfo(CCObject* sender, SEL_CallFuncO selector)
    :sender_(sender)
    ,selector_(selector){}
  ~ResponserInfo(){}

public:
  CCObject* sender_;
  SEL_CallFuncO selector_;
};

class EventMgr {
public:
  typedef std::map<int, std::list<ResponserInfo> > MapResponser;
  static EventMgr& Instance() {
    static EventMgr s;
    return s;
  }

  EventMgr() {}

  ~EventMgr() {
    map_responser_.clear();
  }

  bool Register(int event_id, CCObject* sender, SEL_CallFuncO selector ) {
    if (CheckEventId(event_id) && (sender != NULL)) {
      map_responser_[event_id].push_back(ResponserInfo(sender, selector));
      sender->retain();
      return true;
    }
    return false;
  }

  bool UnRegister(int event_id, CCObject* sender) {
    if (CheckEventId(event_id) && (sender != NULL)) {
      MapResponser::iterator it_find = map_responser_.find(event_id);
      if(it_find == map_responser_.end()) {
        return false;
      }
      std::list<ResponserInfo>::iterator it = it_find->second.begin();
      std::list<ResponserInfo>::iterator itend = it_find->second.end();
      for(; it != itend; ++it) {
        if((*it).sender_ == sender) {
          sender->release();
          it_find->second.erase(it);
          return true;
        }
      }
    }
    return false;
  }

  bool Response(int event_id) {
    if (CheckEventId(event_id)) {
    MapResponser::iterator it_find = map_responser_.find(event_id);
      if(it_find == map_responser_.end()) {
        return false;
      }
      std::list<ResponserInfo>::iterator it = it_find->second.begin();
      std::list<ResponserInfo>::iterator itend = it_find->second.end();
      for(; it != itend; ++it) {
        ResponserInfo& res = (*it);
        (res.sender_->*res.selector_)(res.sender_);
        continue;
      }
    }
    return true;
  }

protected:
  bool CheckEventId(int event_id) {
    return true;
  }
protected:
  MapResponser map_responser_;
};

