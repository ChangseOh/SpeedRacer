#ifndef __NEWRECORD_SCENE_H__
#define __NEWRECORD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

class NewrecordScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float);

    // implement the "static create()" method manually
    CREATE_FUNC(NewrecordScene);

};

#endif // __NewrecordScene_SCENE_H__
