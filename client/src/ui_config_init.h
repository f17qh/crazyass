#ifndef  __DOGRUN2_UI_CINFIG_INIT__
#define  __DOGRUN2_UI_CINFIG_INIT__

namespace dogrun2 {
  class UIItemConfArray;
}

dogrun2::UIItemConfArray* GetUITrainCfg();
dogrun2::UIItemConfArray* GetUIPlayCfg();
dogrun2::UIItemConfArray* GetUIFeedCfg();
int UIConfigInit();

#endif
