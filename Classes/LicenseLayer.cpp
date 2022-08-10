#pragma execution_character_set("utf-8")
#include "LicenseLayer.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

LicenseLayer* LicenseLayer::create()
{
	LicenseLayer* layer = new LicenseLayer();

	if (layer->init())
		return layer;

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool LicenseLayer::init()
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
	layer->setInnerContainerSize(Size(visibleSize.width, 72 * 13));
	layer->setPositionY(120);
	addChild(layer);

	string licenselist[13] = {
		"DESIGN",
		"Control UI : looneybits (opengameart.org)",
		"Font : Raymond Larabie's Joystix",
		"",
		"SOUND/MUSIC",
		"NAKANO SOUND",
		"http://www.nakano-sound.com/",
		"",
		"MAOUDAMASHII",
		"https://maoudamashii.jokersounds.com/",
		"",
		"PANICPUMPKIN",
		"http://pansound.com/panicpumpkin/",
	};

	for (int i = 0; i < 13; i++)
	{
		auto str = Label::createWithTTF(licenselist[i], "fonts/press-start.regular.ttf", 36, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
		str->setPosition(visibleSize.width * 0.5, layer->getInnerContainerSize().height - i * 72 - 18);
		layer->addChild(str);
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
void LicenseLayer::update(float dt)
{

}