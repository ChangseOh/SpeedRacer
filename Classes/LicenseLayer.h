#ifndef __LICENSE_LAYER_H__
#define __LICENSE_LAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

using namespace cocos2d;
using namespace std;

class LicenseLayer : public cocos2d::Layer
{
public:
	static LicenseLayer* create();

	virtual bool init();
	virtual void update(float dt);

};

#endif