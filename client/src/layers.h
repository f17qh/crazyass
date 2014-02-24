#pragma once
#include "cocos2d.h"
#include "sprites.h"
#include "menu_items.h"

USING_NS_CC;

class DogLayer : public CCLayer {
public:
  virtual bool init();
  CREATE_FUNC(DogLayer);

  void MenuClickCallback(CCObject* sender);

protected:
  void InitMenuItem();

protected:
  CCSprite *bg_;
  DogSprite *dog_;
};

class DogMenuLayer : public CCLayer {
public:
  virtual bool init();
  CREATE_FUNC(DogMenuLayer);
  void GameItemClickCallback(CCObject* sender);

protected:
  void InitMenuItem();

protected:
  CCMenu *menu_;
};

class MainMenuLayer : public CCLayer {
public:
  virtual bool init();
  CREATE_FUNC(MainMenuLayer);

  void MenuClickCallback(CCObject* sender);

protected:
  void InitMenuItem();

protected:
  CCMenu *menu_;
};

class FeedMenuLayer : public CCLayer {
public:
  virtual bool init();
  CREATE_FUNC(FeedMenuLayer);

  void BackItemClickCallback(CCObject* sender);
protected:
  void InitMenuItem();
protected:
  CCMenu *menu_;
};

class TrainMenuLayer : public CCLayer {
public:
  virtual bool init();
  CREATE_FUNC(TrainMenuLayer);

  void BackItemClickCallback(CCObject* sender);
protected:
  void InitMenuItem();
protected:
  CCMenu *menu_;
};

class PlayMenuLayer : public CCLayer {
public:
  virtual bool init();
  CREATE_FUNC(PlayMenuLayer);

  void BackItemClickCallback(CCObject* sender);
protected:
  void InitMenuItem();

protected:
  CCMenu *menu_;
};

class UserLayer : public CCLayer {
public:
  virtual bool init();
  CREATE_FUNC(UserLayer);
  void MailClickCallback(CCObject* sender);
  void GoldBarClickCallback(CCObject* sender);
  void NameClickCallback(CCObject* sender);
  void UpdataUserInfo(CCObject* sender);
protected:
  User* user_;
  HeartProgressBar heart_progress_bar_;
  CCLabelTTF* gold_num_;
  CCMenu *menu_;
};

