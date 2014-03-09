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
#include "stage_config.h"
#include "cocos2d.h"
#include "lib_json/json_lib.h"

USING_NS_CC;

StageInfo& StageConfig::GetStageInfo(int stage_id) {
  if(stage_id <= 0)
    return stage_vec_[0];
  return stage_vec_[stage_id - 1];
}

class LocalStageConfig : public StageConfig {
public:
  int Load(const char *path);
protected:
  std::string path_;
  CSJson::Value root_;
};

int LocalStageConfig::Load(const char *path) {
  CSJson::Reader reader;
  path_ = path;

  unsigned long size = 4096;
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

  delete buf;
  return 0;
}

static StageConfig* stage_config = NULL;
StageConfig& StageConfig::Instence() {
  if (stage_config == NULL) {
    stage_config = new LocalStageConfig();
    stage_config->Load("stage_data.json");
  }
  return *stage_config;
}