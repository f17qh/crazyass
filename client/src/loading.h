#pragma once
#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class Loading {
public:
  static Loading& Instence();
  void Init();
  void ShowLoadScene(CCNode* parent, bool visible);
protected:
  UILayer *load_layer_;
};

class PopWin {
public:
  static PopWin& Instence();
  void Init();
  void ShowPopScene(CCNode* parent, bool visible, SEL_TouchEvent selector_back);
protected:
  UILayer *pop_layer_;
};

class PopLose {
public:
  static PopLose& Instence();
  void Init();
  void ShowPopScene(CCNode* parent, bool visible, SEL_TouchEvent selector_back, SEL_TouchEvent selector_shop);
protected:
  UILayer *pop_layer_;
};

class TextBox {
public:
  static TextBox& Instence();
  void Init();
  void Show(UILayer *layer, bool visible, const char *text=NULL, int z_order=100);
protected:
  Layout* layout_;
};
