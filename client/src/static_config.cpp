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
#include "static_config.h"
#include "cocos2d.h"
#include "lib_json/json_lib.h"

USING_NS_CC;

StageInfo& ConfigInfo::GetStageInfo(int stage_id) {
  if(stage_id <= 0)
    return stage_vec_[0];
  return stage_vec_[stage_id - 1];
}
float ConfigInfo::GetEventST(int step_idx) {
  if(step_idx < 0 || step_idx > 2) {
    return 0.0f;
    CCLOG("%s error", __FUNCTION__);
  }
    CCLOG("%s time%f", __FUNCTION__, event_step_vec_[step_idx].duration_scale_time_);
  return event_step_vec_[step_idx].duration_scale_time_;
}

int ConfigInfo::GetEventPL(int step_idx, int finger_idx) {
  if(step_idx < 0 || step_idx > 2 || finger_idx < 0 || finger_idx > 3) {
    return 0;
    CCLOG("%s error", __FUNCTION__);
  }
  return event_finger_vec_[finger_idx].progress_lessen_ + 
    event_step_vec_[step_idx].progress_lessen_;
}

int ConfigInfo::GetEventPI(int step_idx, int finger_idx) {
  if(step_idx < 0 || step_idx > 2 || finger_idx < 0 || finger_idx > 3) {
    return 0;
    CCLOG("%s error", __FUNCTION__);
  }
  return event_finger_vec_[finger_idx].progress_increase_ + 
    event_step_vec_[step_idx].progress_increase_;
}

class LocalConfigInfo : public ConfigInfo {
public:
  int Load(const char *path);
protected:
  std::string path_;
  CSJson::Value root_;
};

int LocalConfigInfo::Load(const char *path) {
  CSJson::Reader reader;
  path_ = path;

  unsigned long size = 4096000;
  unsigned char *buf = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &size);

  if (!reader.parse(std::string((char *)buf), root_, false)) {
    CCLOG("parse %s error", path);
    return -1;
  }

  StageInfo stageinfo;
  CSJson::Value val_arry = root_["stage_arry"];
  unsigned int val_size = (unsigned int)val_arry.size();
  for(unsigned int i = 0; i < val_size; i++) {
    stageinfo.stage_id_ = val_arry[i]["stage_id"].asInt();
    stageinfo.card_count_ = val_arry[i]["card_count"].asInt();
    stageinfo.start_pos_x_ = val_arry[i]["start_pos_x"].asInt();
    stageinfo.start_pos_y_ = val_arry[i]["start_pos_y"].asInt();
    stageinfo.interval_ = val_arry[i]["interval"].asInt();
    stageinfo.shuffle_speed_[0] = val_arry[i]["shuffle_speed_0"].asFloat();
    stageinfo.shuffle_speed_[1] = val_arry[i]["shuffle_speed_1"].asFloat();
    stageinfo.shuffle_speed_[2] = val_arry[i]["shuffle_speed_2"].asFloat();
    stageinfo.shuffle_speed_[3] = val_arry[i]["shuffle_speed_3"].asFloat();
    stageinfo.play_count_[0] = val_arry[i]["play_count_0"].asInt();
    stageinfo.play_count_[1] = val_arry[i]["play_count_1"].asInt();
    stageinfo.play_count_[2] = val_arry[i]["play_count_2"].asInt();
    stageinfo.play_count_[3] = val_arry[i]["play_count_3"].asInt();
    stage_vec_.push_back(stageinfo);
  }

  CSJson::Value tips = root_["tips"];
  memcpy(tips_info_.sub_stage_begin_, tips["sub_stage_begin"].asString().c_str(), sizeof(tips_info_.sub_stage_begin_));
  memcpy(tips_info_.sub_stage_end_, tips["sub_stage_end"].asString().c_str(), sizeof(tips_info_.sub_stage_end_));

  EventStepInfo event_step_info;
  CSJson::Value val_arry_es = root_["event_step"];
  unsigned int val_size_es = (unsigned int)val_arry_es.size();
  for(unsigned int i = 0; i < val_size_es; i++) {
    event_step_info.duration_scale_time_ = val_arry_es[i]["duration_scale_time"].asFloat();
    event_step_info.progress_lessen_ = val_arry_es[i]["progress_lessen"].asInt();
    event_step_info.progress_increase_ = val_arry_es[i]["progress_increase"].asInt();
    event_step_vec_.push_back(event_step_info);
  }

  EventFingerInfo event_finger_info;
  CSJson::Value val_arry_ef = root_["event_finger"];
  unsigned int val_size_ef = (unsigned int)val_arry_ef.size();
  for(unsigned int i = 0; i < val_size_ef; i++) {
    event_finger_info.progress_lessen_ = val_arry_ef[i]["progress_lessen"].asInt();
    event_finger_info.progress_increase_ = val_arry_ef[i]["progress_increase"].asInt();
    event_finger_vec_.push_back(event_finger_info);
  }

  delete buf;
  return 0;
}

static ConfigInfo* config = NULL;
ConfigInfo& ConfigInfo::Instence() {
  if (config == NULL) {
    config = new LocalConfigInfo();
    config->Load("static_data.json");
  }
  return *config;
}

