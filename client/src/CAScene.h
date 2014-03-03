#ifndef __CRAZYASS_CASCENE_H__
#define __CRAZYASS_CASCENE_H__

#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class CAScene : public CCScene {
  public:
    void onEnter() {
        CCScene::onEnter();

        ui_layer_ = UILayer::create();
        Layout *layout = dynamic_cast<Layout*>(
            CCUIHELPER->createWidgetFromJsonFile(scene_jsonfile_));
        ui_layer_->addWidget(layout);
        this->addChild(ui_layer_, 0, 100);
    }

  protected:
    UILayer *ui_layer_;
}

#endif
