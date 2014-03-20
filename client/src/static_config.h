#pragma once
#include <string>
#include <vector>
#include <map>
struct StageInfo {
  StageInfo() {
    memset(this, 0 ,sizeof(StageInfo));
  }
  int stage_id_;
  int card_count_;
  float shuffle_speed_[4];
  float play_count_[4];
  int start_pos_x_;
  int start_pos_y_;
  int interval_;
};

struct TipsInfo {
  TipsInfo() {
    memset(sub_stage_begin_, 0 ,sizeof(sub_stage_begin_));
    memset(sub_stage_end_, 0 ,sizeof(sub_stage_end_));
    loading_tips_map_.clear();
  }
  char sub_stage_begin_[32];
  char sub_stage_end_[32];
  std::map<int, std::vector<std::string> > loading_tips_map_;
};

struct EventStepInfo {
  EventStepInfo() {
    memset(this, 0 ,sizeof(EventStepInfo));
  }
  float duration_scale_time_;
  int progress_lessen_;
  int progress_increase_;
};

struct EventFingerInfo {
  EventFingerInfo() {
    memset(this, 0 ,sizeof(EventFingerInfo));
  }
  int progress_lessen_;
  int progress_increase_;
  int btn_star1_x_;
  int btn_star1_y_;
  int btn_star2_x_;
  int btn_star2_y_;
  int btn_star_field_x_;
  int btn_star_field_y_;
  int btn_star_field_w_;
  int btn_star_field_h_;
};

namespace CSJson{
  class Value;
}

class ConfigInfo
{
public:
  static ConfigInfo& Instence();

  StageInfo& GetStageInfo(int stage_id);
  TipsInfo& tips_info(){return tips_info_;};
  float GetEventST(int step_idx);
  int GetEventPL(int step_idx, int finger_idx, int stageid);
  int GetEventPI(int step_idx, int finger_idx, int stageid);
  EventFingerInfo& GetEventFingerInfo(int finger_idx, int stageid);
  virtual int Load(const char *path) = 0;
protected:
  void SetEventFingerInfo( CSJson::Value& val, int stageid);
  void SetLoadingTipsInfo( CSJson::Value& val, int idx);
protected:
  std::vector<StageInfo> stage_vec_;
  std::vector<EventStepInfo> event_step_vec_;
  std::map<int, std::vector<EventFingerInfo> > event_finger_map_;
  TipsInfo tips_info_;
};

