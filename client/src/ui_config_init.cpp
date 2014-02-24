#include <stdio.h>
#include "ui_config_init.h"
#include "common.h"
#include "protocol/uiconfig.pb.h"

static dogrun2::UIItemConfArray *UIFeedCfg = NULL;
static dogrun2::UIItemConfArray *UITrainCfg = NULL;
static dogrun2::UIItemConfArray *UIPlayCfg = NULL;

dogrun2::UIItemConfArray* GetUIFeedCfg() {
  return UIFeedCfg;
}

dogrun2::UIItemConfArray* GetUITrainCfg() {
  return UITrainCfg;
}

dogrun2::UIItemConfArray* GetUIPlayCfg() {
  return UIPlayCfg;
}


#include "cocos2d.h"
int UIConfigInit() {
  printf("%s\n", __FUNCTION__);
  if (!UIFeedCfg) {
    UIFeedCfg = new dogrun2::UIItemConfArray;
    assert(UIFeedCfg != NULL);
    ParseFromFile("etc/uifeed.cfg", UIFeedCfg);

    assert(UIFeedCfg != NULL);
  }
  if (!UITrainCfg) {
    UITrainCfg = new dogrun2::UIItemConfArray;
    assert(UITrainCfg != NULL);
    ParseFromFile("etc/uitrain.cfg", UITrainCfg);

    assert(UITrainCfg != NULL);
  }
  if (!UIPlayCfg) {
    UIPlayCfg = new dogrun2::UIItemConfArray;
    assert(UIPlayCfg != NULL);
    ParseFromFile("etc/uiplay.cfg", UIPlayCfg);
    assert(UIPlayCfg != NULL);
  }
  return 0;
}

