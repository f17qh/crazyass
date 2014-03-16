#pragma once
#include <string>
#include <vector>

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
    memset(this, 0 ,sizeof(TipsInfo));
  }
  char sub_stage_begin_[32];
  char sub_stage_end_[32];
};

struct EventStepInfo {
  EventStepInfo() {
    memset(this, 0 ,sizeof(EventStepInfo));
  }
  int duration_scale_time_;
  int progress_lessen_;
  int progress_increase_;
};

struct EventFingerInfo {
  EventFingerInfo() {
    memset(this, 0 ,sizeof(EventFingerInfo));
  }
  int progress_lessen_;
  int progress_increase_;
};

class ConfigInfo
{
public:
  static ConfigInfo& Instence();

  StageInfo& GetStageInfo(int stage_id);
  TipsInfo& tips_info(){return tips_info_;};
  float GetEventST(int step_idx);
  int GetEventPL(int step_idx, int finger_idx);
  int GetEventPI(int step_idx, int finger_idx);
  virtual int Load(const char *path) = 0;
protected:
  std::vector<StageInfo> stage_vec_;
  std::vector<EventStepInfo> event_step_vec_;
  std::vector<EventFingerInfo> event_finger_vec_;
  TipsInfo tips_info_;
};

