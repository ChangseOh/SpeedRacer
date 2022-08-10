#ifndef __ACHIEVE_LAYER_H__
#define __ACHIEVE_LAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

using namespace cocos2d;
using namespace std;

class AchieveLayer : public cocos2d::Layer
{
public:
	static AchieveLayer* create();

	virtual bool init();
	virtual void update(float dt);

};

#endif