#pragma execution_character_set("utf-8")
#include "AchieveLayer.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

AchieveLayer* AchieveLayer::create()
{
	AchieveLayer* layer = new AchieveLayer();

	if (layer->init())
		return layer;

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool AchieveLayer::init()
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

	auto layer = ScrollView::create();
	layer->setContentSize(Size(visibleSize.width, 660));
	layer->setInnerContainerSize(Size(visibleSize.width, 240 * 25));
	layer->setPositionY(120);
	addChild(layer);

	auto ttl = Label::createWithTTF("ACHIEVEMENTS", "fonts/press-start.regular.ttf", 120, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	ttl->setPosition(visibleSize.width * 0.5, visibleSize.height - 156);
	ttl->enableShadow(Color4B::BLACK, Size(10, -10), 0);
	ttl->setTextColor(Color4B::YELLOW);
	addChild(ttl);

	//auto expl = Label::createWithTTF("", "fonts/press-start.regular.ttf", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	//expl->setPosition(visibleSize.width * 0.5, visibleSize.height - 1000);
	//expl->enableShadow(Color4B::BLACK, Size(2, -2), 0);
	//expl->setTextColor(Color4B::WHITE);
	//addChild(expl);

	//auto expl = Label::createWithTTF("Can buy Achieve with TAB Achieve box", "fonts/press-start.regular.ttf", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	//expl->setPosition(visibleSize.width * 0.5, visibleSize.height - 1000);
	//expl->enableShadow(Color4B::BLACK, Size(2, -2), 0);
	//expl->setTextColor(Color4B::WHITE);
	//addChild(expl);

	string subpics[25] = {
		"mytire1.png",
		"mytire2.png",
		"road_water_left.png",//bridge
		"mytire3.png",
		"mytire4.png",
		"mytire5.png",
		"mytire6.png",
		"road_snow_left.png",//snow
		"mytire7.png",
		"boost2.png",//2x
		"mytire8.png",
		"mytire9.png",
		"mytire10.png",
		"mytire11.png",
		"mytire12.png",
		"road_beach_left.png",//beach
		"mytire13.png",
		"mytire14.png",
		"boost3.png",//3x
		"mytire15.png",
		"mytire16.png",
		"mytire17.png",
		"mytire18.png",
		"road_city_right.png",//city
		"mytire19.png",
	};
	Rect picrect[25] = {
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(166,16,32,32),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(84,0,32,32),
		Rect(0,0,22,31),
		Rect(0,0,32,32),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(116,0,32,32),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,32,32),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(0,0,22,31),
		Rect(12,0,32,32),
		Rect(0,0,22,31),
	};

	for (int i = 0; i < 25; i++)
	{
		//auto base = Button::create("achieves.png", "achieves_.png", "achieves.png", Widget::TextureResType::LOCAL);
		auto base = Sprite::create("achieves.png");
		base->getTexture()->setAliasTexParameters();
		base->setPosition(Vec2(960, layer->getInnerContainerSize().height - i * 240 - 120));
		base->setScale(2);
		base->setTag(i);
		layer->addChild(base);
		//if (!clearAchieve[i])
		//{
		//	base->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		//		Button* node = dynamic_cast<Button*>(sender);
		//		if (type == Widget::TouchEventType::ENDED)
		//			log("touched %d", node->getTag());
		//	});
		//}

		auto exppic = Sprite::create(subpics[i]);
		exppic->getTexture()->setAliasTexParameters();
		exppic->setTextureRect(picrect[i]);
		exppic->setScale(64 / exppic->getContentSize().height);
		exppic->setPosition(590, 54);
		base->addChild(exppic);

		int _accumScore = getValue(accumScore);// random(1, 3000);
		string str = SPRINTF("#%d %s\n%d/%d", i + 1, achieveStr[i].c_str(), _accumScore, getValue(achieveScore[i]));
		if (clearAchieve[i])
			str = SPRINTF("#%d %s\nCLEARED", i + 1, achieveStr[i].c_str());
		auto desc = Label::createWithTTF(str, "fonts/press-start.regular.ttf", 18, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::TOP);
		desc->setPosition(320, 72);
		desc->enableShadow(Color4B(0x62, 0x10, 0xdc, 0xff), Size(2, -2), 0);
		desc->setTextColor(Color4B(0xdc, 0x92, 0x10, 0xff));
		base->addChild(desc);

		auto accum = LoadingBar::create("accum.png", _accumScore * 100 / getValue(achieveScore[i]));
		accum->setScale(4);
		accum->setPosition(Vec2(320, 24));
		base->addChild(accum);
	}

	auto closebutton = Button::create();
	closebutton->loadTextures("button_close.png", "button_close_.png", "button_close_d.png", Widget::TextureResType::LOCAL);
	closebutton->setScale(2);
	closebutton->setPosition(Vec2(1790, visibleSize.height - 156));
	addChild(closebutton);
	closebutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			removeFromParent();
		}
	});
	return true;
}
void AchieveLayer::update(float dt)
{

}