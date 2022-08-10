#pragma execution_character_set("utf-8")
#include "CarlistLayer.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

CarlistLayer* CarlistLayer::create()
{
	CarlistLayer* layer = new CarlistLayer();

	if (layer->init())
		return layer;

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool CarlistLayer::init()
{
	if (!Layer::init())
		return false;

	lastSelected = selectedCar;

	auto black = Button::create("white.png", "white.png", "white.png", Widget::TextureResType::LOCAL);
	black->setOpacity(200);
	black->setScale(10);
	black->setColor(Color3B::BLACK);
	black->setPosition(visibleSize * 0.5);
	black->setSwallowTouches(true);
	addChild(black);

	auto girl = Sprite::create("girl.png", Rect(0, 0, 36, 72));
	girl->getTexture()->setAliasTexParameters();
	girl->setScale(14);
	girl->setPosition(Vec2(270, visibleSize.height * 0.5f) + Vec2(0, -50));
	girl->setColor(Color3B(220, 220, 220));
	addChild(girl);

	for (int j = 0; j < 3; j++)
	{
		auto girltex = SpriteFrame::create("girl.png", Rect(j * 36, 0, 36, 72));
		girltex->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(girltex, StringUtils::format("GIRL%d", j));
	}
	Vector<SpriteFrame*> aniframe;
	aniframe.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("GIRL0"));
	aniframe.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("GIRL1"));
	aniframe.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("GIRL2"));
	aniframe.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("GIRL1"));
	girl->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(aniframe, 0.16f))));

	auto layer = ScrollView::create();
	layer->setContentSize(Size(visibleSize.width * 0.8, 750));
	layer->setInnerContainerSize(Size(visibleSize.width * 0.8f, 300 * 4));
	layer->setPosition(Vec2(362, 70));
	layer->setScrollBarAutoHideEnabled(true);
	layer->setScrollBarColor(Color3B::WHITE);
	layer->setScrollBarEnabled(true);
	layer->setName("SCLAYER");
	addChild(layer);

	auto ttl = Label::createWithTTF("SELECT YOUR MACHINE", "fonts/press-start.regular.ttf", 80, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
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
		"mytire3.png",
		"mytire4.png",
		"mytire5.png",
		"mytire6.png",
		"mytire7.png",
		"mytire8.png",
		"mytire9.png",
		"mytire10.png",
		"mytire11.png",
		"mytire12.png",
		"mytire13.png",
		"mytire14.png",
		"mytire15.png",
		"mytire16.png",
		"mytire17.png",
		"mytire18.png",
		"mytire19.png",
	};

	for (int i = 0; i < 20; i++)
	{
		auto car = Button::create("blank300.png");
		car->setPosition(Vec2(
			(i % 5) * 307,
			1080 - (i / 5) * 300
		) + Vec2(150, -50));
		car->setOpacity(0);
		car->setName(SPRINTF("BUTTON%d", i));
		car->setTag(i);
		layer->addChild(car);

		auto pic = Sprite::create(SPRINTF("mytire%d.png", i), Rect(0, 0, 22, 31));
		pic->getTexture()->setAliasTexParameters();
		pic->setScale(5);
		pic->setPosition(150, 100);
		pic->setName("PIC");
		car->addChild(pic);

		for (int j = 0; j < 4; j++)
		{
			auto tiretex = SpriteFrame::create(SPRINTF("mytire%d.png", i), Rect(j * 22, 0, 22, 31));
			tiretex->getTexture()->setAliasTexParameters();
			SpriteFrameCache::getInstance()->addSpriteFrame(tiretex, StringUtils::format("TIRE%d_%d", i, j));
		}

		if (i >= haveCars.size())
		{
			//graySprite(pic);
			pic->setOpacity(128);
			car->setEnabled(false);

			auto llbl = Label::createWithTTF("LOCKED", "fonts/press-start.regular.ttf", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
			llbl->setPosition(150, 100);
			llbl->enableShadow(Color4B::BLACK, Size(2, -2));
			llbl->setTextColor(Color4B::WHITE);
			car->addChild(llbl);

		}
		else
		{

			car->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == Widget::TouchEventType::ENDED)
				{
					int bgm = AudioEngine::play2d("sound/se_maoudamashii_onepoint11.mp3", false, 1);
					Button* node = dynamic_cast<Button*>(sender);
					if (node->getTag() == lastSelected)
						return;

					getChildByName("SCLAYER")->getChildByName(SPRINTF("BUTTON%d", lastSelected))->getChildByName("PIC")->stopAllActions();
					Sprite* spr = (Sprite*)node->getChildByName("PIC");
					Vector<SpriteFrame*> tireani;
					for (int j = 0; j < 4; j++)
					{
						tireani.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(SPRINTF("TIRE%d_%d", node->getTag(), j)));
					}
					spr->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(tireani, 0.05f))));
					lastSelected = node->getTag();
				}
			});

		}

		if (i == lastSelected)
		{
			Vector<SpriteFrame*> tireani;
			for (int j = 0; j < 4; j++)
			{
				tireani.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(SPRINTF("TIRE%d_%d", i, j)));
			}
			pic->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(tireani, 0.05f))));
		}
	}

	auto closebutton = Button::create();
	closebutton->loadTextures("button_close.png", "button_close_.png", "button_close_d.png", Widget::TextureResType::LOCAL);
	closebutton->setScale(2);
	closebutton->setPosition(Vec2(1790, visibleSize.height - 156));
	addChild(closebutton);
	closebutton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			_func(lastSelected);
			removeFromParent();
		}
	});
	return true;
}
void CarlistLayer::update(float dt)
{

}
void CarlistLayer::regi_func(FUNC pFunc)
{
	_func = std::move(pFunc);
}
//void CarlistLayer::graySprite(Sprite * sprite) {
//	if (sprite) {
//		GLProgram * p = new GLProgram();
//		p->initWithFilenames("gray.vsh", "gray.fsh");
//		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
//		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
//		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
//		p->link();
//		p->updateUniforms();
//		sprite->setGLProgram(p);
//	}
//}
