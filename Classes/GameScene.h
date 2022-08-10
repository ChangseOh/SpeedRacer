#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	GameLayer* gamelayer;
	Slider* controller;
	Sprite* steering;
	bool steerDowned = false;
	int breakFXID;

	bool isPaused = false;
	void setPause();

	float steerWeight;
};

#endif // __GameScene_SCENE_H__
