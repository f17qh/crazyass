#include "loading.h"
#include "static_config.h"
static Loading* loading = NULL;
Loading& Loading::Instence() {
  if (loading == NULL) {
    loading = new Loading();
    loading->Init();
  }
  return *loading;
}

void Loading::Init() {
  CCLOG("%s", __FUNCTION__);
  load_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/LoadScene.json"));
  load_layer_->addWidget(layout);
  index_ = (int)(time(NULL)%4);
}

void Loading::ShowLoadScene(CCNode* parent, bool visible) {
  if(visible) {
    if(parent->getChildByTag(99) == NULL)
      load_layer_->retain();
      parent->addChild(load_layer_, 21, 99);
      index_++;
      index_ = (index_)%4;
      std::map<int, std::vector<std::string> >& tips_map = 
        ConfigInfo::Instence().tips_info().loading_tips_map_;

      UILabelBMFont *ui_text = (UILabelBMFont *)load_layer_->getWidgetByName("LabelBMFont_1");
      ui_text->setText(tips_map[index_][0].c_str());
      ui_text = (UILabelBMFont *)load_layer_->getWidgetByName("LabelBMFont_2");
      ui_text->setText(tips_map[index_][1].c_str());
      ui_text = (UILabelBMFont *)load_layer_->getWidgetByName("LabelBMFont_3");
      ui_text->setText(tips_map[index_][2].c_str());
      ui_text = (UILabelBMFont *)load_layer_->getWidgetByName("LabelBMFont_4");
      ui_text->setText(tips_map[index_][3].c_str());

  }
  load_layer_->setVisible(visible);
}

static PopWin* pop_win = NULL;
PopWin& PopWin::Instance() {
  if (pop_win == NULL) {
    pop_win = new PopWin();
    pop_win->Init();
  }
  return *pop_win;
}

void PopWin::Init() {
  CCLOG("%s", __FUNCTION__);
  pop_layer_ = UILayer::create();
  Layout *layout = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/PopWin.json"));
  pop_layer_->addWidget(layout);
}

void PopWin::ShowPopScene(CCNode* parent, bool visible, SEL_TouchEvent selector_back) {
  pop_layer_->setVisible(visible);
  pop_layer_->setTouchEnabled(visible);

  if(visible) {
    if(parent->getChildByTag(98) == NULL) {
      pop_layer_->retain();
      parent->addChild(pop_layer_, 20, 98);
      UIButton* btn = (UIButton *)pop_layer_->getWidgetByName("ButtonBack");
      if(btn != NULL) {
        btn->addTouchEventListener(parent, selector_back);
      }
    }
  }
}


static PopRecharge* pop_recharge = NULL;
PopRecharge& PopRecharge::Instance() {
  if (pop_recharge == NULL) {
    pop_recharge = new PopRecharge();
    pop_recharge->Init();
  }
  return *pop_recharge;
}

void PopRecharge::Init() {
  CCLOG("%s", __FUNCTION__);
  layout_ = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/PopRecharge.json"));
  layout_->retain();
}

void PopRecharge::Show(UILayer *layer, CCNode* parent, SEL_TouchEvent selector_back, SEL_TouchEvent selector_shop) {
  layout_->retain();
  layer->addWidget(layout_);
  UIButton* btn = (UIButton *)layer->getWidgetByName("ButtonBack");
  if(btn != NULL) {
    btn->addTouchEventListener(parent, selector_back);
  }

  UIButton* btn_shop = (UIButton *)layer->getWidgetByName("ButtonShop");
  if(btn_shop != NULL) {
    btn_shop->addTouchEventListener(parent, selector_shop);
  }

  //UILabelBMFont *desc = (UILabelBMFont *)layer->getWidgetByName("LabelBMFontLoseDesc1");
  //desc->setVisible(true);
  //desc = (UILabelBMFont *)layer->getWidgetByName("LabelBMFontLoseDesc2");
  //desc->setVisible(true);
  //desc = (UILabelBMFont *)layer->getWidgetByName("LabelBMFontLoseDesc3");
  //desc->setVisible(true);

  //UIImageView *imgs = (UIImageView *)layer->getWidgetByName("ImagePop");
  //imgs->setVisible(true);
  //imgs = (UIImageView *)layer->getWidgetByName("ImageViewBackGround");
  //imgs->setVisible(true);
}

void PopRecharge::Disappear(UILayer *layer) {
  layer->removeWidget(layout_);
}
static TextBox* textbox = NULL;
TextBox& TextBox::Instance() {
  if (textbox == NULL) {
    textbox = new TextBox();
    textbox->Init();
  }
  return *textbox;
}

void TextBox::Init() {
  CCLOG("%s", __FUNCTION__);
  layout_ = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/TextBox.json"));
  layout_->retain();
}

void TextBox::Show(UILayer *layer, bool visible, const char *text, int z_order) {
  if(visible) {
    layout_->retain();
    layer->addWidget(layout_);
    UILabelBMFont *ui_text = (UILabelBMFont *)layer->getWidgetByName("LabelBMFontText");
    if(ui_text == NULL)
      return;
    ui_text->setVisible(true);
    ui_text->setText(text);
    ui_text->setZOrder(z_order+2);

    UIImageView *imgs = (UIImageView *)layer->getWidgetByName("ImgTextField");
    if(imgs == NULL)
      return;
    imgs->setVisible(true);
    imgs->setZOrder(z_order+1);

    UIImageView *imgs_bg = (UIImageView *)layer->getWidgetByName("ImgBG");
    if(imgs_bg == NULL)
      return;
    imgs_bg->setVisible(true);
    imgs_bg->setZOrder(z_order);

  } else {
    layer->removeWidget(layout_);
  }
}

static Finger* finger = NULL;
Finger& Finger::Instance() {
  if (finger == NULL) {
    finger = new Finger();
    finger->Init();
  }
  return *finger;
}

void Finger::Init() {
  CCLOG("%s", __FUNCTION__);
  layout_ = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/Finger.json"));
  layout_->retain();
}

void Finger::Show(UILayer *layer, CCNode* parent, SEL_TouchEvent selector_use) {
  layout_->retain();
  layer->addWidget(layout_);
  UIButton* btn_finger = (UIButton *)layer->getWidgetByName("BtnFinger");
  UIPanel* panel_finger = (UIPanel*)layer->getWidgetByName("PanelFinger");
  panel_finger->setVisible(true);
  btn_finger->addTouchEventListener(parent, selector_use);
  btn_finger->setPressedActionEnabled(true);
}

void Finger::Disappear(UILayer *layer) {
  layer->removeWidget(layout_);
}

static Replay* replay = NULL;
Replay& Replay::Instance() {
  if (replay == NULL) {
    replay = new Replay();
    replay->Init();
  }
  return *replay;
}

void Replay::Init() {
  CCLOG("%s", __FUNCTION__);
  layout_ = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("MainScene/Replay.json"));
  layout_->retain();
}

void Replay::Show(UILayer *layer, CCNode* parent, SEL_TouchEvent selector_use, char* show_num) {
  layout_->retain();
  layer->addWidget(layout_);
  UIButton* btn_replay = (UIButton *)layer->getWidgetByName("BtnReplay");
  UIPanel* panel_replay = (UIPanel*)layer->getWidgetByName("PanelReplay");
  panel_replay->setVisible(true);
  btn_replay->addTouchEventListener(parent, selector_use);
  btn_replay->setPressedActionEnabled(true);
  UILabelBMFont *ui_num = (UILabelBMFont *)layer->getWidgetByName("LabelBMFontReplayNum");
  ui_num->setText(show_num);
}

void Replay::Disappear(UILayer *layer) {
  layer->removeWidget(layout_);
}
