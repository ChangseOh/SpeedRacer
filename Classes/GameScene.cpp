#include "GameScene.h"
#include "GameLayer.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "TitleScene.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	steerWeight = UserDefault::getInstance()->getFloatForKey("STEER", 0.075f);

	breakFXID = AudioEngine::INVALID_AUDIO_ID;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (int i = 0; i < 4; i++)
	{
		auto frame = SpriteFrame::create(SPRINTF("light_%d.png", i), Rect(0, 0, 64, 32));
		frame->getTexture()->setAliasTexParameters();

		SpriteFrameCache::getInstance()->addSpriteFrame(frame, SPRINTF("LIGHT%d", i + 1));
	}

	float _scale = 4.5;

	gamelayer = GameLayer::create(1);
	gamelayer->setAnchorPoint(Vec2(0, 0));
	gamelayer->setPosition(960 - (256 * _scale / 2), 540 - (256 * _scale / 2));
	gamelayer->setScale(_scale);
	addChild(gamelayer);

	auto checker1 = Sprite::create("checker.png");
	checker1->setPosition(0, visibleSize.height * 0.5);
	checker1->setAnchorPoint(Vec2(0, 0.5));
	addChild(checker1);

	auto checker2 = Sprite::create("checker.png");
	checker2->setPosition(visibleSize.width + 1, visibleSize.height * 0.5);
	checker2->setAnchorPoint(Vec2(1, 0.5));
	addChild(checker2);

	//일시정지
	auto pausebutton = Button::create("button_pause.png", "button_pause_.png", "button_pause_d.png", Widget::TextureResType::LOCAL);
	pausebutton->setScale(2);
	pausebutton->setPosition(Vec2(1800, 900));
	addChild(pausebutton);
	pausebutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		Button* node = dynamic_cast<Button*>(sender);
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			break;
		case Widget::TouchEventType::MOVED:
			break;
		case Widget::TouchEventType::ENDED:
			setPause();
			break;
		}
	});

	//신호등
	auto signal = Sprite::createWithSpriteFrameName("LIGHT1");
	signal->setScale(6);
	signal->setPosition(960, visibleSize.height - 150);
	signal->setName("SIGNAL");
	addChild(signal);

	//UI표시
	auto loglabel = Label::createWithTTF("", "fonts/press-start.regular.ttf", 120, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	loglabel->setName("LOG");
	loglabel->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.5f);
	loglabel->enableShadow(Color4B::RED, Size(10, -10), 0);
	addChild(loglabel);
	loglabel->setVisible(false);

	auto timename = Label::createWithTTF("TIME", "fonts/press-start.regular.ttf", 60, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	timename->setAnchorPoint(Vec2(1, 0.5));
	timename->setPosition(visibleSize.width * 0.5 - 340, visibleSize.height - 48);
	timename->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	addChild(timename);

	auto timelabel = Label::createWithTTF("60", "fonts/press-start.regular.ttf", 120, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	timelabel->setName("TIME");
	timelabel->setAnchorPoint(Vec2(1, 0.5));
	timelabel->setPosition(visibleSize.width * 0.5 - 340, visibleSize.height - 160);
	timelabel->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	addChild(timelabel);

	auto scorename = Label::createWithTTF("SCORE", "fonts/press-start.regular.ttf", 60, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scorename->setAnchorPoint(Vec2(0, 0.5));
	scorename->setPosition(visibleSize.width * 0.5 + 340, visibleSize.height - 48);
	scorename->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	addChild(scorename);

	auto scorelabel = Label::createWithTTF("000", "fonts/press-start.regular.ttf", 120, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scorelabel->setName("SCORE");
	scorelabel->setAnchorPoint(Vec2(0, 0.5));
	scorelabel->setPosition(visibleSize.width * 0.5 + 340, visibleSize.height - 160);
	scorelabel->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	addChild(scorelabel);

	//기어
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("blank1x1.png", Rect(0, 0, 1, 1)), "BLANK");
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("gear.png", Rect(0, 0, 290, 490)), "GEAR_LOW");
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("gear.png", Rect(356, 0, 290, 490)), "GEAR_HIGH");
	auto gear = CheckBox::create("GEAR_HIGH", "GEAR_HIGH", "GEAR_LOW", "GEAR_LOW", "GEAR_LOW", Widget::TextureResType::PLIST);
	gear->setPosition(Vec2(visibleSize.width - 100, 50));
	gear->setSelected(true);
	gear->setAnchorPoint(Vec2(1, 0));
	gear->setScale(0.8);
	addChild(gear);
	gear->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		log("%s", gear->isSelected() ? "selected" : "unselect");
		if (gear->isSelected())
		{
			gearHigh = true;
		}
		else
		{
			gearHigh = false;
		}
	});

	//브레이크
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("break.png", Rect(0, 0, 119, 180)), "BREAKRELEASE");
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("break_.png", Rect(0, 0, 119, 180)), "BREAKFOOT");
	auto breaker = Button::create("BREAKRELEASE", "BREAKFOOT", "BREAKRELEASE", Widget::TextureResType::PLIST);
	breaker->setPosition(Vec2(visibleSize.width - 100, 460));
	breaker->setScale(1.75);
	breaker->setAnchorPoint(Vec2(1, 0));
	addChild(breaker);
	breaker->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			footBreak = true;
			breakFXID = AudioEngine::play2d("sound/breaking.mp3", false, 1);
			//playSound("sound/breaking.mp3", true);
			break;
		case Widget::TouchEventType::ENDED:
			footBreak = false;
			AudioEngine::stop(breakFXID);
			break;
		case Widget::TouchEventType::CANCELED:
			footBreak = false;
			AudioEngine::stop(breakFXID);
			break;
		}
	});
	//footBreak

	//스티어링/그림
	steering = Sprite::create("steer.png");
	steering->setPosition(277, 292);

	//스티어링/콘트롤
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("steer_guage.png", Rect(0, 0, 508, 310)), "BASE");
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("steer_button.png", Rect(0, 0, 22, 364)), "CONTROL");
	controller = Slider::create("BASE", "CONTROL", Widget::TextureResType::PLIST);
	controller->setPercent(50);
	controller->setPosition(Vec2(278, 279));
	addChild(controller);
	controller->setOpacity(0);
	controller->addEventListener([=](Ref* sender, Slider::EventType type) {
		Slider* node = dynamic_cast<Slider*>(sender);
		switch (type)
		{
		case Slider::EventType::ON_SLIDEBALL_DOWN:
			steerDowned = true;
			break;
		case Slider::EventType::ON_PERCENTAGE_CHANGED:
			break;
		case Slider::EventType::ON_SLIDEBALL_UP:
			steerDowned = false;
			break;
		case Slider::EventType::ON_SLIDEBALL_CANCEL:
			steerDowned = false;
			break;
		}
		steering->setRotation(node->getPercent() - 50);
	});
	//controller->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
	//	Slider* node = dynamic_cast<Slider*>(sender);
	//	log("%d", node->getPercent());
	//});

	addChild(steering);

	//콘트롤러 투명도
	auto opaciter = Slider::create("opacity_bar.png", "opacity_ball.png", Widget::TextureResType::LOCAL);
	//opaciter->setRotation(-90);
	//opaciter->setScale(0.8);
	opaciter->setPercent(-(((steerWeight - 0.02f) / 0.07f) * 100.0f - 100.0f));
	opaciter->setPosition(Vec2(258, visibleSize.height - 315));
	addChild(opaciter);
	opaciter->addEventListener([=](Ref* sender, Slider::EventType type) {
		Slider* node = dynamic_cast<Slider*>(sender);
		switch (type)
		{
		case Slider::EventType::ON_SLIDEBALL_DOWN:
			break;
		case Slider::EventType::ON_PERCENTAGE_CHANGED:
			//if (node->getPercent() < 20)
			//	node->setPercent(20);

			//steering->setOpacity((float)node->getPercent() * 2.55f);
			//breaker->setOpacity((float)node->getPercent() * 2.55f);
			////gear->setOpacity((float)node->getPercent() * 2.55f);
			//opaciter->setOpacity((float)node->getPercent() * 2.55f);
			//gear->getRendererBackgroundSelected()->setOpacity((float)node->getPercent() * 2.55f);

			steerWeight = ((float)(100 - node->getPercent()) / 100.0f) * 0.07f + 0.02f;
			break;
		case Slider::EventType::ON_SLIDEBALL_UP:
			UserDefault::getInstance()->setFloatForKey("STEER", steerWeight);
			break;
		case Slider::EventType::ON_SLIDEBALL_CANCEL:
			break;
		}
	});
	opaciter->setVisible(true);

	scheduleUpdate();

	auto klistener = EventListenerKeyboard::create();
	klistener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* type) {
		if (code == EventKeyboard::KeyCode::KEY_BACK)
			setPause();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(klistener, this);

    return true;
}
void GameScene::update(float dt)
{
	if (!steerDowned)
	{
		int conrp = controller->getPercent();
		if (conrp < 50)
		{
			conrp += 5;
			if (conrp > 50)
				conrp = 50;
		}
		if (conrp > 50)
		{
			conrp -= 5;
			if (conrp < 50)
				conrp = 50;
		}
		controller->setPercent(conrp);
		steering->setRotation(conrp - 50);
	}
	gamelayer->carMover = (controller->getPercent() - 50) * steerWeight;// 0.075;
}
void GameScene::setPause()
{
	if (!isPaused)
		gamelayer->pauseSchedulerAndActions();
	else
		gamelayer->resumeSchedulerAndActions();
	isPaused = !isPaused;

	if (isPaused)
	{
		auto resumebutton = Button::create("button_resume.png", "button_resume_.png", "button_resume_d.png", Widget::TextureResType::LOCAL);
		resumebutton->setScale(4);
		resumebutton->setPosition(Vec2(visibleSize * 0.5f) + Vec2(240, 0));
		resumebutton->setName("RESUME");
		addChild(resumebutton);
		resumebutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
			Button* node = dynamic_cast<Button*>(sender);
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				break;
			case Widget::TouchEventType::MOVED:
				break;
			case Widget::TouchEventType::ENDED:
				setPause();
				break;
			}
		});

		auto exitbutton = Button::create("button_exit.png", "button_exit_.png", "button_exit_d.png", Widget::TextureResType::LOCAL);
		exitbutton->setScale(4);
		exitbutton->setPosition(Vec2(visibleSize * 0.5f) + Vec2(-240, 0));
		exitbutton->setName("EXIT");
		addChild(exitbutton);
		exitbutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
			Button* node = dynamic_cast<Button*>(sender);
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				break;
			case Widget::TouchEventType::MOVED:
				break;
			case Widget::TouchEventType::ENDED:
			{
				AudioEngine::stopAll();
				//playSound("sound/se_maoudamashii_explosion06.mp3", false);
				auto scene = TitleScene::createScene();
				Director::getInstance()->replaceScene(scene);
			}
				break;
			}
		});
	}
	else
	{
		if (getChildByName("RESUME"))
			getChildByName("RESUME")->removeFromParent();
		if (getChildByName("EXIT"))
			getChildByName("EXIT")->removeFromParent();
	}
}

