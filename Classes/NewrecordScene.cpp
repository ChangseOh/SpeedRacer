#include "NewrecordScene.h"
//#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "audio/include/AudioEngine.h"
#include "TitleScene.h"

using namespace cocos2d::experimental;

using namespace std;
using namespace cocos2d::ui;

USING_NS_CC;

Scene* NewrecordScene::createScene()
{
    return NewrecordScene::create();
}

// on "init" you need to initialize your instance
bool NewrecordScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto layer = NodeGrid::create();
	addChild(layer);

	auto checker = Sprite::create("checker.png");
	auto checkerframe = checker->getSpriteFrame();
	for (int i = 0; i < (1920 / checker->getContentSize().width + 1); i++)
	{
		auto spr = Sprite::createWithSpriteFrame(checkerframe);
		spr->setPosition(i * checker->getContentSize().width, 540);
		layer->addChild(spr);
	}

	layer->runAction(RepeatForever::create(
		Waves3D::create(20, Size(32,32), 10, 25)
	));

	auto white = Sprite::create("white.png");
	white->setScale(10);
	white->setPosition(960, 540);
	white->setOpacity(64);
	white->setColor(Color3B::GRAY);
	addChild(white);

	auto newrecord = Label::createWithTTF("NEW RECORD!", "fonts/press-start.regular.ttf", 130, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	newrecord->setPosition(Vec2(visibleSize * 0.5f) + Vec2(0, 250));
	newrecord->enableShadow(Color4B::BLACK, Size(10, -10), 5);
	newrecord->setTextColor(Color4B(255, 255, 100, 255));
	addChild(newrecord);

	auto nrscore = Label::createWithTTF(SPRINTF("%d", getValue(myHighscore)), "fonts/press-start.regular.ttf", 200, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	nrscore->setPosition(Vec2(visibleSize * 0.5f) + Vec2(0, 0));
	nrscore->enableShadow(Color4B::BLACK, Size(10, -10), 5);
	nrscore->setTextColor(Color4B::WHITE);
	addChild(nrscore);

	auto cong = Label::createWithTTF("CONGRATULATIONS!", "fonts/press-start.regular.ttf", 70, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	cong->setPosition(Vec2(visibleSize * 0.5f) + Vec2(0, -300));
	cong->enableShadow(Color4B::BLACK, Size(10, -10), 5);
	cong->setTextColor(Color4B(245, 10, 10, 255));
	addChild(cong);
	cong->runAction(Sequence::create(
		Blink::create(3, 6),
		CallFunc::create([=](void) {
		cong->setVisible(true);
	}),
		NULL
		));

	int bgm = AudioEngine::play2d("sound/nakano_fanfare8.mp3", false, 1, NULL);
	AudioEngine::setFinishCallback(bgm, [=](int id, const string ad) {
		log("callback");
		auto tabscreen = Label::createWithTTF("TAB SCREEN", "fonts/press-start.regular.ttf", 70, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		tabscreen->setPosition(Vec2(visibleSize * 0.5f) + Vec2(0, -400));
		tabscreen->enableShadow(Color4B::BLACK, Size(10, -10), 0);
		addChild(tabscreen);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [=](Touch* touch, Event* event) {
			return true;
		};
		listener->onTouchEnded = [=](Touch* touch, Event* event) {
			_eventDispatcher->removeAllEventListeners();
			auto scene = TitleScene::createScene();
			Director::getInstance()->replaceScene(scene);
			java_AD();
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	});

    return true;
}
void NewrecordScene::update(float dt)
{

}

