#ifndef __RANK_LAYER_H__
#define __RANK_LAYER_H__

#include "cocos2d.h"
#include "GameManager.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d::experimental;
using namespace cocos2d;
using namespace std;

class RankLayer : public cocos2d::Layer
{
public:
	static RankLayer* create();

	virtual bool init();
	virtual void update(float dt);

	void setRanks();
	void drawRank(string, int);
};

#endif