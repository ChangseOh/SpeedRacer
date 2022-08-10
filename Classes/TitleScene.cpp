/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "TitleScene.h"
#include "GameLayer.h"
//#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "GameScene.h"
#include "AchieveLayer.h"
#include "RankLayer.h"
#include "audio/include/AudioEngine.h"
#include "LicenseLayer.h"
#include "CarlistLayer.h"

using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

USING_NS_CC;

Scene* TitleScene::createScene()
{
    return TitleScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in TitleScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	openCars = getValue(haveCars.size());
	openBgs = getValue(acceptBgs.size());

	//test
	//openCars = getValue(19);
	//openBgs = getValue(4);

	float _scale = 5.5;

	//log("Title 1");
	auto gamelayer = GameLayer::create(0);
	gamelayer->setAnchorPoint(Vec2(0, 0));
	gamelayer->setPosition(960 - (256 * _scale / 2), 540 - (256 * _scale / 2));
	gamelayer->setScale(_scale);
	addChild(gamelayer);
	//log("Title 2");

	auto checker1 = Sprite::create("checker.png");
	checker1->setPosition(0, visibleSize.height * 0.5);
	checker1->setAnchorPoint(Vec2(0, 0.5));
	addChild(checker1);

	auto checker2 = Sprite::create("checker.png");
	checker2->setPosition(visibleSize.width + 1, visibleSize.height * 0.5);
	checker2->setAnchorPoint(Vec2(1, 0.5));
	addChild(checker2);

	auto title1 = Label::createWithTTF("SUPER", "fonts/press-start.regular.ttf", 130, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	title1->setPosition(Vec2(visibleSize * 0.5f) + Vec2(-visibleSize.width, 350));
	title1->enableShadow(Color4B::BLACK, Size(10, -10), 5);
	title1->setTextColor(Color4B(255, 255, 100, 255));
	addChild(title1);

	auto title2 = Label::createWithTTF("SPEED RACER", "fonts/press-start.regular.ttf", 130, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	title2->setPosition(Vec2(visibleSize * 0.5f) + Vec2(visibleSize.width, 210));
	title2->enableShadow(Color4B::BLACK, Size(10, -10), 5);
	title2->setTextColor(Color4B(255, 255, 100, 255));
	addChild(title2);

	auto title3 = Label::createWithTTF("classic", "fonts/press-start.regular.ttf", 100, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	title3->setPosition(Vec2(visibleSize * 0.5f) + Vec2(-visibleSize.width, 50));
	title3->enableShadow(Color4B::BLACK, Size(10, -10), 5);
	title3->setTextColor(Color4B::RED);
	addChild(title3);

	title1->runAction(Sequence::create(
		DelayTime::create(0.5),
		MoveBy::create(0.4, Vec2(visibleSize.width, 0)),
		DelayTime::create(1),
		Repeat::create(Sequence::create(
			TintTo::create(0.03f, Color3B::MAGENTA),
			TintTo::create(0.03f, Color3B::YELLOW),
			TintTo::create(0.03f, Color3B::WHITE),
			TintTo::create(0.03f, Color3B::BLACK),
			TintTo::create(0.03f, Color3B::RED),
			TintTo::create(0.03f, Color3B::GREEN),
			TintTo::create(0.03f, Color3B::WHITE),
			NULL
		), 5),
		NULL
	));
	title2->runAction(Sequence::create(
		DelayTime::create(0.85),
		MoveBy::create(0.4, Vec2(-visibleSize.width, 0)),
		DelayTime::create(0.65),
		Repeat::create(Sequence::create(
			TintTo::create(0.03f, Color3B::MAGENTA),
			TintTo::create(0.03f, Color3B::YELLOW),
			TintTo::create(0.03f, Color3B::WHITE),
			TintTo::create(0.03f, Color3B::BLACK),
			TintTo::create(0.03f, Color3B::RED),
			TintTo::create(0.03f, Color3B::GREEN),
			TintTo::create(0.03f, Color3B::WHITE),
			NULL
		), 5),
		NULL
	));
	title3->runAction(Sequence::create(
		DelayTime::create(1.2),
		MoveBy::create(0.4, Vec2(visibleSize.width, 0)),
		DelayTime::create(0.3),
		Repeat::create(Sequence::create(
			TintTo::create(0.03f, Color3B::MAGENTA),
			TintTo::create(0.03f, Color3B::YELLOW),
			TintTo::create(0.03f, Color3B::WHITE),
			TintTo::create(0.03f, Color3B::BLACK),
			TintTo::create(0.03f, Color3B::RED),
			TintTo::create(0.03f, Color3B::GREEN),
			TintTo::create(0.03f, Color3B::WHITE),
			NULL
		), 5),
	//	CallFunc::create([=](void) {
	//	playSound("sound/highengine.mp3", false);
	//	_eventDispatcher->setEnabled(true);
	//}),
		NULL
	));

	vector<string> buttons = {
		"button_start",
		"button_car",
		"button_achieve",
		"button_lb",
		"button_license",
		"googlelogin",
		"googlelogout"
	};

	vector<Vec2> buttonpos = {
		Vec2(1200, 220),
		Vec2(720, 220),
		Vec2(1792, 326),
		Vec2(1792, 178),
		Vec2(1792, 474),
		Vec2(1792, 622),
		Vec2(1792, 622),
	};

	for (int i = 0; i < buttons.size(); i++)
	{
		Rect sz = Rect(0, 0, 128, 64);
		if (i >= 2)// || i == 3 || i == 4)
			sz = Rect(0, 0, 64, 64);
		auto frame = SpriteFrame::create(buttons[i] + ".png", sz);
		auto frame1 = SpriteFrame::create(buttons[i] + "_.png", sz);
		auto frame2 = SpriteFrame::create(buttons[i] + "_d.png", sz);

		frame->getTexture()->setAliasTexParameters();
		frame1->getTexture()->setAliasTexParameters();
		frame2->getTexture()->setAliasTexParameters();

		SpriteFrameCache::getInstance()->addSpriteFrame(frame, buttons[i] + "NORMAL");
		SpriteFrameCache::getInstance()->addSpriteFrame(frame1, buttons[i] + "SELECTED");
		SpriteFrameCache::getInstance()->addSpriteFrame(frame2, buttons[i] + "DISABLE");
	}

	//auto testbutton = Button::create("button_pause.png", "button_pause_.png", "button_pause_d.png", Widget::TextureResType::LOCAL);
	//testbutton->setScale(2);
	//testbutton->setPosition(Vec2(1800, 900));
	//addChild(testbutton);
	//testbutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
	//	Button* node = dynamic_cast<Button*>(sender);
	//	switch (type)
	//	{
	//	case Widget::TouchEventType::BEGAN:
	//		break;
	//	case Widget::TouchEventType::MOVED:
	//		break;
	//	case Widget::TouchEventType::ENDED:
	//	{
	//		auto futil = FileUtils::getInstance();
	//		string filePath = futil->getWritablePath().append("gamedata.dat");
	//		futil->removeFile(filePath);
	//		futil->destroyInstance();
	//		Director::getInstance()->end();
	//	}
	//		break;
	//	}
	//});

	for (int i = 0; i < buttons.size(); i++)
	{
		//remark is 4 test
		//if ((i == 1 && getValue(openCars) < 2) && gameEvents.size() == 0)
		//	continue;

		if (i == 1)
		{
			auto carnumlabel = Label::createWithTTF(SPRINTF("%d/%d", selectedCar + 1, haveCars.size()), "fonts/press-start.regular.ttf", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
			carnumlabel->setPosition(buttonpos[i] + Vec2(0, 88));
			carnumlabel->setName("CARNUM");
			addChild(carnumlabel);
		}

		auto titlebutton = Button::create();
		titlebutton->loadTextures(buttons[i] + "NORMAL", buttons[i] + "SELECTED", buttons[i] + "DISABLE", Widget::TextureResType::PLIST);
		titlebutton->setTag(i + 1);
		titlebutton->setScale(2);
		titlebutton->setPosition(buttonpos[i]);
		titlebutton->setEnabled(false);
		addChild(titlebutton);
		titlebutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
			Button* node = dynamic_cast<Button*>(sender);
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				break;
			case Widget::TouchEventType::MOVED:
				break;
			case Widget::TouchEventType::ENDED:
				switch (node->getTag() - 1)
				{
				case 0://game start
				{
					_eventDispatcher->removeAllEventListeners();
					AudioEngine::stopAll();
					int bgm = AudioEngine::play2d("sound/ME017-100125-logo02-wav.mp3", false, 1);
					AudioEngine::setFinishCallback(bgm, [=](int id, const string str) {
						UserDefault::getInstance()->setIntegerForKey("SELECTEDCAR", selectedCar);
						auto scene = GameScene::createScene();
						Director::getInstance()->replaceScene(scene);
						//log("game start");
						java_FIREBASE(0, selectedCar);
					});
				}
				break;
				case 1://car change
				{
					if (lock)
						break;
					lock = true;
					node->setEnabled(false);
					int bgm = AudioEngine::play2d("sound/se_maoudamashii_onepoint11.mp3", false, 1);

					auto layer = CarlistLayer::create();
					layer->regi_func([=](int aa) {
						selectedCar = aa;
						((Label*)getChildByName("CARNUM"))->setString(SPRINTF("%d/%d", selectedCar + 1, haveCars.size()));
						gamelayer->changeCar(haveCars.at(selectedCar));
						lock = false;
						node->setEnabled(true);
					});
					addChild(layer);

					//selectedCar++;
					//if (selectedCar > haveCars.size() - 1)
					//	selectedCar = 0;
					//runAction(Sequence::create(
					//	DelayTime::create(0.2f),
					//	CallFunc::create([=](void) {
					//	node->setEnabled(true);
					//	lock = false;
					//}),
					//	NULL
					//	));
				}
					break;
				case 2://achieve
				{
					if (lock)
						break;
					lock = true;
					int bgm = AudioEngine::play2d("sound/se_maoudamashii_onepoint11.mp3", false, 1);
					AudioEngine::setFinishCallback(bgm, [=](int id, const string str) {
						auto layer = AchieveLayer::create();
						addChild(layer);
						lock = false;
						if (getChildByName("REDDOT"))
							getChildByName("REDDOT")->removeFromParent();
					});
				}
					break;
				case 3://leaderboard
				{
					if (lock)
						break;
					lock = true;
					int bgm = AudioEngine::play2d("sound/se_maoudamashii_onepoint11.mp3", false, 1);
					AudioEngine::setFinishCallback(bgm, [=](int id, const string str) {
						auto layer = RankLayer::create();
						addChild(layer);
						lock = false;
					});
				}
					break;
				case 4:
				{
					if (lock)
						break;
					lock = true;
					int bgm = AudioEngine::play2d("sound/se_maoudamashii_onepoint11.mp3", false, 1);
					auto layer = LicenseLayer::create();
					addChild(layer);
					lock = false;
				}
					break;
				case 5://google log in
				{
					if (lock)
						break;
					lock = true;
					int bgm = AudioEngine::play2d("sound/se_maoudamashii_onepoint11.mp3", false, 1);
					java_SIGNIN();
					scheduleUpdate();
				}
					break;
				case 6://google log out
				{
					if (lock)
						break;
					lock = true;
					int bgm = AudioEngine::play2d("sound/se_maoudamashii_onepoint11.mp3", false, 1);
					java_SIGNOUT();
					scheduleUpdate();
				}
				break;
				}
			break;
			case Widget::TouchEventType::CANCELED:
				break;
			}
		});
	}
	//scheduleUpdate();

	//_eventDispatcher->setEnabled(false);

	log("Title 3");
	//AudioEngine::play2d("sound/titlecall02.mp3", false, 1);
	log("Title 4");
	if (gameEvents.size() > 0)
	{
		auto reddot = Sprite::create("reddot.png");
		reddot->getTexture()->setAliasTexParameters();
		reddot->setScale(4);
		reddot->setPosition(Vec2(1792, 346) + Vec2(48, 48));
		reddot->setName("REDDOT");
		addChild(reddot);
		reddot->runAction(RepeatForever::create(Sequence::create(
			DelayTime::create(0.5),
			FadeOut::create(0.2),
			DelayTime::create(0.2),
			FadeIn::create(0.5),
			NULL
		)));

		runAction(Sequence::create(
			DelayTime::create(1.5),
			CallFunc::create([=](void) {
			setAlarm();
		}),
			NULL
		));
	}
	else
	{
		enableUI(true);
	}
	auto klistener = EventListenerKeyboard::create();
	klistener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* type) {
		if (code == EventKeyboard::KeyCode::KEY_BACK)
		{
			auto popup = Sprite::create("dialogbox.png");
			popup->getTexture()->setAliasTexParameters();
			popup->setScale(2);
			popup->setPosition(visibleSize * 0.5);
			addChild(popup);

			auto desc = Label::createWithTTF("Sure to exit?", "fonts/press-start.regular.ttf", 24, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::TOP);
			desc->setPosition(227, 110);
			desc->enableShadow(Color4B(0x62, 0x10, 0xdc, 0xff), Size(2, -2), 0);
			desc->setTextColor(Color4B(0xdc, 0x92, 0x10, 0xff));
			popup->addChild(desc);

			auto ok = Button::create("button_ok.png", "button_ok_.png", "button_ok_d.png", Widget::TextureResType::LOCAL);
			ok->setPosition(Vec2(227 - 70, 48));
			popup->addChild(ok);
			ok->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == Widget::TouchEventType::ENDED)
				{
					Director::getInstance()->end();
				}
			});

			auto no = Button::create("button_close.png", "button_close_.png", "button_close_d.png", Widget::TextureResType::LOCAL);
			no->setPosition(Vec2(227 + 70, 48));
			popup->addChild(no);
			no->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == Widget::TouchEventType::ENDED)
				{
					AudioEngine::play2d("sound/spc1000_took.mp3", false, 1);
					popup->removeFromParent();
				}
			});
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(klistener, this);

	scheduleUpdate();

    return true;
}
void TitleScene::update(float dt)
{
	switch (signinmode)
	{
	case 0:
		AudioEngine::play2d("sound/titlecall02.mp3", false, 1);
		//signinmode = -1;
		unscheduleUpdate();
		((Button*)getChildByTag(6))->setEnabled(false);
		((Button*)getChildByTag(6))->setVisible(false);
		((Button*)getChildByTag(7))->setEnabled(true);
		((Button*)getChildByTag(7))->setVisible(true);
		lock = false;
		break;
	case 1:
		AudioEngine::play2d("sound/titlecall02.mp3", false, 1);
		((Button*)getChildByTag(6))->setEnabled(true);
		((Button*)getChildByTag(6))->setVisible(true);
		((Button*)getChildByTag(7))->setEnabled(false);
		((Button*)getChildByTag(7))->setVisible(false);
		unscheduleUpdate();
		lock = false;
		break;
	}
}
void TitleScene::setAlarm()
{
	if (gameEvents.size() > 0)
	{
		auto it = gameEvents.begin();
		auto eventlabel = Label::createWithTTF(achieveStr[*it], "fonts/press-start.regular.ttf", 70, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		eventlabel->setPosition(Vec2(visibleSize * 0.5) + Vec2(0, -150));
		addChild(eventlabel);
		java_FIREBASE(2, (int)*it);

		clearAchieve[*it] = true;
		gameEvents.erase(it);
		openCars = getValue(haveCars.size());
		openBgs = getValue(acceptBgs.size());

		//if (getChildByName("CARNUM"))
		//	((Label*)getChildByName("CARNUM"))->setString(SPRINTF("%d/%d", selectedCar + 1, haveCars.size()));

		eventlabel->runAction(Sequence::create(
			Blink::create(2, 6),
			DelayTime::create(0.5),
			CallFunc::create([=](void) {
			setAlarm();
		}),
			RemoveSelf::create(),
			NULL
			));

		AudioEngine::play2d("sound/ME017-100125-logo02-wav.mp3", false, 1);
	}
	else
	{
		enableUI(true);
		checkMyHave();
		saveGame();
		if (getChildByName("CARNUM"))
			((Label*)getChildByName("CARNUM"))->setString(SPRINTF("%d/%d", selectedCar + 1, haveCars.size()));
	}
}

void TitleScene::enableUI(bool onoff)
{

	for (int i = 0; i < 5; i++)
	{
		if (getChildByTag(i + 1))
			((Button*)getChildByTag(i + 1))->setEnabled(onoff);
	}
	lock = !onoff;
}
