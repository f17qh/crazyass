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
};

class StageConfig
{
public:
  static StageConfig& Instence();

  StageInfo& GetStageInfo(int stage_id);

  virtual int Load(const char *path) = 0;
protected:
  std::vector<StageInfo> stage_vec_;
};

