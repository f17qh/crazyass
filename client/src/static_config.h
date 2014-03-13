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
class ConfigInfo
{
public:
  static ConfigInfo& Instence();

  StageInfo& GetStageInfo(int stage_id);
  TipsInfo& tips_info(){return tips_info_;};

  virtual int Load(const char *path) = 0;
protected:
  std::vector<StageInfo> stage_vec_;
  TipsInfo tips_info_;
};

