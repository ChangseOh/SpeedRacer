#ifndef __CARLIST_LAYER_H__
#define __CARLIST_LAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

using namespace cocos2d;
using namespace std;

class CarlistLayer : public cocos2d::Layer
{
public:
	static CarlistLayer* create();

	virtual bool init();
	virtual void update(float dt);

	typedef std::function<void(int)> FUNC;
	FUNC _func;
	void regi_func(FUNC pFunc);

	int lastSelected;
	//void graySprite(Sprite* sprite);
};

#endif