#pragma execution_character_set("utf-8")
#include "RankLayer.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

RankLayer* RankLayer::create()
{
	RankLayer* layer = new RankLayer();

	if (layer->init())
		return layer;

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool RankLayer::init()
{
	if (!Layer::init())
		return false;

	auto black = Button::create("white.png", "white.png", "white.png", Widget::TextureResType::LOCAL);
	black->setOpacity(160);
	black->setScale(10);
	black->setColor(Color3B::BLACK);
	black->setPosition(visibleSize * 0.5);
	black->setSwallowTouches(true);
	addChild(black);

	//auto layer = NodeGrid::create();
	//addChild(layer);

	//auto checker = Sprite::create("checker.png");
	//auto checkerframe = checker->getSpriteFrame();
	//for (int i = 0; i < (1920 / checker->getContentSize().width + 1); i++)
	//{
	//	auto spr = Sprite::createWithSpriteFrame(checkerframe);
	//	spr->setPosition(i * checker->getContentSize().width, 540);
	//	layer->addChild(spr);
	//}

	//layer->runAction(RepeatForever::create(
	//	Waves3D::create(20, Size(32, 32), 10, 25)
	//));

	auto ttl = Label::createWithTTF("BEST DRIVERS", "fonts/press-start.regular.ttf", 120, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	ttl->setPosition(visibleSize.width * 0.5, visibleSize.height - 156);
	ttl->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	ttl->setTextColor(Color4B::YELLOW);
	addChild(ttl);

	auto closebutton = Button::create();
	closebutton->loadTextures("button_close.png", "button_close_.png", "button_close_d.png", Widget::TextureResType::LOCAL);
	closebutton->setLocalZOrder(1);
	closebutton->setScale(2);
	closebutton->setPosition(Vec2(1790, visibleSize.height - 156));
	addChild(closebutton);
	closebutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			java_MYSCORE();
			removeFromParent();
		}
	});

	javaCallBack = 0;

	rankings.clear();

	//로딩서클
	auto circle = Sprite::create("loading.png");
	circle->getTexture()->setAliasTexParameters();
	circle->setScale(12);
	circle->setPosition(visibleSize * 0.5);
	circle->setName("CIRCLE");
	addChild(circle);
	circle->runAction(Sequence::create(
		RotateBy::create(7, 1440),
		CallFunc::create([=](void) {
		javaCallBack = 1;
	}),
		NULL
	));

	//읽기 시작
	//java_RANKING();
	java_MYSCORE();



	scheduleUpdate();


	auto testbutton = Button::create("button_pause.png", "button_pause_.png", "button_pause_d.png", Widget::TextureResType::LOCAL);
	testbutton->setScale(2);
	testbutton->setPosition(Vec2(120, 900));
	addChild(testbutton);
	testbutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		Button* node = dynamic_cast<Button*>(sender);
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			break;
		case Widget::TouchEventType::MOVED:
			break;
		case Widget::TouchEventType::ENDED:
		{
			java_CALLBOARD();
		}
			break;
		}
	});


	return true;
}
void RankLayer::update(float dt)
{
	switch (javaCallBack)
	{
	case 1://로딩실패
	{
		string namestr = "READINFO FAIL.PLEASERETRYAFTER";
		for (int i = 0; i < 10; i++)
		{
			string rank = SPRINTF("%04d%s%06d%03d", i + 1, namestr.substr(i * 3, 3).c_str(), (10 - i) * (10 - i), i);
			rankings.push_back(rank);
		}
		setRanks();
		unscheduleUpdate();
		javaCallBack = 0;
	}
		break;
	case 2://리더보드 콜 ok
		if (rankings.size() == 0)
		{
			javaCallBack = 1;
			break;
		}
		setRanks();
		unscheduleUpdate();
		javaCallBack = 0;
		break;
	case 3:
		java_RANKING();
		javaCallBack = 0;
		break;
	}
}
void RankLayer::setRanks()
{
	if (getChildByName("SCROLLER"))
		getChildByName("SCROLLER")->removeFromParent();
	if (getChildByName("CIRCLE"))
		getChildByName("CIRCLE")->removeFromParent();

	int ysize = 140 * (rankings.size() + 1);

	auto layer = ScrollView::create();
	layer->setContentSize(Size(visibleSize.width, 660));
	layer->setInnerContainerSize(Size(visibleSize.width, ysize));
	layer->setName("SCROLLER");
	layer->setPositionY(120);
	addChild(layer);

	for (int i = 0; i < (rankings.size() > 10 ? 10: rankings.size()); i++)
	{
		string rank = rankings.at(i);
		drawRank(rank, i);
	}
	string myrank = SPRINTF("%04d%s%06d%03d", myRank, myName.c_str(), getValue(myHighscore), myCountry);
	drawRank(myrank, -1);

	if (myName.compare("YOU") == 0)
	{
		auto reddot = Sprite::create("reddot.png");
		reddot->getTexture()->setAliasTexParameters();
		reddot->setScale(4);
		reddot->setPosition(Vec2(1792, 346) + Vec2(48, 48) + Vec2(-440, 336));
		reddot->setName("REDDOT");
		addChild(reddot);
		reddot->runAction(RepeatForever::create(Sequence::create(
			DelayTime::create(0.5),
			FadeOut::create(0.2),
			DelayTime::create(0.2),
			FadeIn::create(0.5),
			NULL
		)));
	}
}
void RankLayer::drawRank(string rank, int i)
{
	ScrollView* layer = (ScrollView*)getChildByName("SCROLLER");

	int ysize = layer->getInnerContainerSize().height;

	auto ranks = Label::createWithTTF(SPRINTF("%d", atoi(rank.substr(0, 4).c_str())), "fonts/press-start.regular.ttf", 90, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	ranks->setAnchorPoint(Vec2(0, 0.5));
	ranks->setPosition(250, ysize - (i * 140) - 70 - 140);
	ranks->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	ranks->setTextColor(Color4B::WHITE);
	layer->addChild(ranks);

	if (i >= 0)
	{
		auto names = Label::createWithTTF(SPRINTF("%s", rank.substr(4, 3).c_str()), "fonts/press-start.regular.ttf", 90, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		names->setAnchorPoint(Vec2(0, 0.5));
		names->setPosition(1260, ysize - (i * 140) - 70 - 140);
		names->enableShadow(Color4B::BLACK, Size(10, -10), 0);
		names->setTextColor(Color4B::WHITE);
		layer->addChild(names);
	}

	auto scores = Label::createWithTTF(SPRINTF("%6d", atoi(rank.substr(7, 6).c_str())), "fonts/press-start.regular.ttf", 90, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scores->setAnchorPoint(Vec2(1, 0.5));
	scores->setPosition(1130, ysize - (i * 140) - 70 - 140);
	scores->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	scores->setTextColor(Color4B::WHITE);
	layer->addChild(scores);
	if (i == 0)
		scores->setColor(Color3B(176, 255, 255));
	if (i == 1)
		scores->setColor(Color3B(255, 243, 106));
	if (i == 2)
		scores->setColor(Color3B(245, 224, 255));

	//국기
	int country = 28;// atoi(rank.substr(13, 3).c_str());
	auto flag = Sprite::create("flags.png", Rect((country % 15) * 64, (country / 15) * 64, 64, 64));
	flag->getTexture()->setAliasTexParameters();
	flag->setPosition(1660, ysize - (i * 140) - 70 - 140);
	layer->addChild(flag);
	if (i < 3)
		flag->setScale(3);
	else
		flag->setScale(2);

	if (i < 0)
	{
		auto textfield = TextField::create(SPRINTF("%s", rank.substr(4, 3).c_str()), "fonts/press-start.regular.ttf", 90);
		textfield->setMaxLength(3);
		textfield->setMaxLengthEnabled(true);
		textfield->setPosition(Vec2(1260, ysize - (i * 140) - 70 - 140));
		textfield->setTextColor(Color4B::YELLOW);
		textfield->setSwallowTouches(true);
		textfield->setAnchorPoint(Vec2(0, 0.5));
		textfield->setTouchEnabled(true);
		textfield->setString(myName);
		textfield->addEventListener([=](Ref* sender, TextField::EventType type) {
			TextField* field = dynamic_cast<TextField*>(sender);
			if (type == TextField::EventType::ATTACH_WITH_IME)
			{
				field->setString("");
				if (getChildByName("REDDOT"))
					getChildByName("REDDOT")->setVisible(false);
			}
			else
			//if (type == TextField::EventType::DETACH_WITH_IME || type == TextField::EventType::INSERT_TEXT)
			{
				myName = field->getString();
				//log("%s", myName.c_str());
				UserDefault::getInstance()->setStringForKey("NAME", myName);
				//saveGame();
			}
		});

		layer->addChild(textfield);

		//버튼 2 추가
		//auto flagbutton = Button::create("button_start.png", "button_start.png", "button_start.png", Widget::TextureResType::LOCAL);
		//flagbutton->setContentSize(Size(180, 120));
		//flagbutton->setPosition(Vec2(1660, ysize - (i * 140) - 70 - 140));
		//flagbutton->setOpacity(0);
		//flagbutton->setScale(1.8);
		//flagbutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		//	if (type == Widget::TouchEventType::ENDED)
		//	{

		//	}
		//});
		//layer->addChild(flagbutton);

		scores->setColor(Color3B(176, 255, 176));
	}
}
