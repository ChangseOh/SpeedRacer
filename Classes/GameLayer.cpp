#pragma execution_character_set("utf-8")
#include "GameLayer.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "TitleScene.h"
#include "NewrecordScene.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d::experimental;
using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

GameLayer* GameLayer::create(int mode)
{
	GameLayer* layer = new GameLayer();
	layer->layerMode = mode;
	if (layer->init())
		return layer;

	CC_SAFE_DELETE(layer);
	return NULL;
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	if (layerMode == 0)
	{
		speed = 6;
		gameMode = GAMEMODE::LOW;
		gearHigh = false;
		stage = random(0, getValue(openBgs) - 1);
	}
	else
	{
		stage = random(0, getValue(openBgs) - 1);
	}

	makeRoad();

	//test
	//stage = 3;
	//immortal = true;
	//gameTime = 3;
	//gameScore = 1000;

	roadTime = roadTimes[0];
	switch (stage)
	{
	case 0:
	{
		auto frame = SpriteFrame::create("trees.png", Rect(0, 0, 30, 17));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "TREE");
	}
	{
		auto frame = SpriteFrame::create("house_l.png", Rect(0, 0, 96, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSEL");
	}
	{
		auto frame = SpriteFrame::create("house_r.png", Rect(0, 0, 95, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSER");
	}
	{
		auto frame = SpriteFrame::create("house_lb.png", Rect(0, 0, 96, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSELB");
	}
	{
		auto frame = SpriteFrame::create("house_rg.png", Rect(0, 0, 95, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSERG");
	}
	break;
	case 1:
	{
		auto frame = SpriteFrame::create("ship.png", Rect(0, 0, 88, 39));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "SHIP");
	}
	{
		auto frame = SpriteFrame::create("boat.png", Rect(0, 0, 29, 15));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "BOAT");
	}
		break;
	case 2:
	{
		auto frame = SpriteFrame::create("treesnow.png", Rect(0, 0, 30, 17));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "TREE");
	}
	{
		auto frame = SpriteFrame::create("house_snowl.png", Rect(0, 0, 96, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSEL");
	}
	{
		auto frame = SpriteFrame::create("house_snowr.png", Rect(0, 0, 95, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSER");
	}
	{
		auto frame = SpriteFrame::create("house_snowlb.png", Rect(0, 0, 96, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSELB");
	}
	{
		auto frame = SpriteFrame::create("house_snowrg.png", Rect(0, 0, 95, 100));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "HOUSERG");
	}
	break;
	case 3:
	{
		auto frame = SpriteFrame::create("vacancepeople.png", Rect(0, 0, 26, 61));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "VACANCE");
	}
	{
		auto frame = SpriteFrame::create("vacanceparasols.png", Rect(0, 0, 26, 61));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "PARASOL");
	}
	break;
	case 4:
	{
		auto frame = SpriteFrame::create("trees.png", Rect(0, 0, 30, 17));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "TREE");
	}
	{
		auto frame = SpriteFrame::create("beds.png", Rect(0, 0, 31, 31));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "BEDS");
	}
	{
		auto frame = SpriteFrame::create("building1.png", Rect(0, 0, 124, 80));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "BUILDING1");
	}
	{
		auto frame = SpriteFrame::create("building2.png", Rect(0, 0, 77, 71));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "BUILDING2");
	}
	{
		auto frame = SpriteFrame::create("fountain.png", Rect(0, 0, 52, 52));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "FOUNTAIN");
	}
	{
		auto frame = SpriteFrame::create("parkings.png", Rect(0, 0, 117, 102));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "PARKING");
	}
	break;
	}
	//auto car = Sprite3D::create("car.c3b");
	//car->setPosition3D(Vec3(960, 540, 0));
	//addChild(car);
	for (int i = 0; i < 5; i++)
	{
		auto frame = SpriteFrame::create("explosion.png", Rect(i * 35, 0, 35, 36));
		frame->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, SPRINTF("EXPLODE%d", i));
	}

	for (int i = 0; i < 18; i++)
		rivalRate.push_back(1);
	rivalRate.push_back(0);
	rivalRate.push_back(2);

	//스크롤되는 배경 = 차량 레이어
	scrollLayer = Layer::create();
	addChild(scrollLayer);

	//아스팔트
	for (int i = 0; i < 8; i += 2)
	{
		auto asphalt0 = Sprite::create("road_0.png");
		asphalt0->setAnchorPoint(Vec2(0.5, 1));
		asphalt0->setPosition(128, i * 48);
		asphalt0->setTag(100 + i);
		asphalt0->getTexture()->setAliasTexParameters();
		scrollLayer->addChild(asphalt0);
		asphalt0->setName("ASPHALT");

		auto asphalt1 = Sprite::create("road_1.png");
		asphalt1->setAnchorPoint(Vec2(0.5, 1));
		asphalt1->setPosition(128, (i + 1) * 48);
		asphalt1->setTag(100 + i + 1);
		asphalt1->getTexture()->setAliasTexParameters();
		scrollLayer->addChild(asphalt1);
		asphalt1->setName("ASPHALT");
	}

	string borders[10] = {
		"road_green_left.png",
		"road_green_right.png",
		"road_water_left.png",
		"road_water_right.png",
		"road_snow_left.png",
		"road_snow_right.png",
		"road_beach_left.png",
		"road_beach_right.png",
		"road_city_left.png",
		"road_city_right.png"
	};
	for (int i = 0; i < 10; i++)
	{
		auto border = SpriteFrame::create(borders[i], Rect(0, 0, 200, 64));
		border->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(border, bordername[i]);
	}

	//왼쪽 보더
	for (int i = 0; i < 6; i++)
	{
		auto leftborder = Sprite::createWithSpriteFrameName(bordername[stage * 2]);
		leftborder->setAnchorPoint(Vec2(1, 1));
		leftborder->setPosition(leftX, i * 64);
		leftborder->setTag(200 + i * 2);
		leftborder->getTexture()->setAliasTexParameters();
		scrollLayer->addChild(leftborder);
		leftborder->setName("WALL");
	}

	//오른쪽 보더
	for (int i = 0; i < 6; i++)
	{
		auto rightborder = Sprite::createWithSpriteFrameName(bordername[stage * 2 + 1]);
		rightborder->setAnchorPoint(Vec2(0, 1));
		rightborder->setPosition(leftX + roadWidth, i * 64);
		rightborder->setTag(201 + i * 2);
		rightborder->getTexture()->setAliasTexParameters();
		scrollLayer->addChild(rightborder);
		rightborder->setName("WALL");
	}

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			auto tiretex = SpriteFrame::create(SPRINTF("trtire%d.png", j), Rect(i * 24, 0, 24, 73));
			tiretex->getTexture()->setAliasTexParameters();
			SpriteFrameCache::getInstance()->addSpriteFrame(tiretex, StringUtils::format("TRTIRE%d%d", j, i));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		auto tiretex = SpriteFrame::create(SPRINTF("mytire%d.png", selectedCar), Rect(i * 22, 0, 22, 31));
		tiretex->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(tiretex, StringUtils::format("TIRE%d", i));
	}

	for (int j = 0; j < 6; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			auto tiretex = SpriteFrame::create(SPRINTF("gentire%d.png", j), Rect(i * 22, 0, 22, 31));
			tiretex->getTexture()->setAliasTexParameters();
			SpriteFrameCache::getInstance()->addSpriteFrame(tiretex, StringUtils::format("GENTIRE%d%d", j, i));
		}
	}
	for (int i = 0; i < 4; i++)
	{
		auto tiretex = SpriteFrame::create("rivaltire.png", Rect(i * 22, 0, 22, 31));
		tiretex->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(tiretex, StringUtils::format("RIVALTIRE%d", i));
	}

	mycar = Sprite::create("blank1x1.png");
	auto mybody = Sprite::create("mycar.png");
	mybody->setName("BODY");
	mytire = Sprite::createWithSpriteFrameName("TIRE1");
	mybody->setVisible(false);
	mytire->setPositionY(-1);
	mycar->addChild(mytire);
	mycar->addChild(mybody);
	mycar->setLocalZOrder(2);
	if (layerMode == 0)
		animaFast();

	mycar->setPosition(128, 64);
	scrollLayer->addChild(mycar);
	mycar->setName("CAR");

	scheduleUpdate();


	auto klistener = EventListenerKeyboard::create();
	klistener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		isKeyDown = true;
		kcode = keyCode;
	};
	klistener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		isKeyDown = false;
		if (keyCode == EventKeyboard::KeyCode::KEY_0)
			stage = 1 - stage;
		if (keyCode == EventKeyboard::KeyCode::KEY_1)
			Director::getInstance()->pause();
		if (keyCode == EventKeyboard::KeyCode::KEY_2)
			Director::getInstance()->resume();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(klistener, this);

	if (layerMode != 0)
	{
		int bgm = AudioEngine::play2d("sound/ME019-100511-fanfare03-wav.mp3", false, 1);
		AudioEngine::setFinishCallback(bgm, [=](int id, const string str) {
			modeReady(0);
		});
	}

	return true;
}
void GameLayer::modeReady(float dt)
{
	((Sprite*)Director::getInstance()->getRunningScene()->getChildByName("SIGNAL"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("LIGHT2"));
	playSound("sound/alarm_ready.mp3", false);
	runAction(Sequence::create(
		DelayTime::create(1.4f),
		CallFunc::create([=](void) {
			//beep 1
			//get ready
			//log("get ready");
			((Sprite*)Director::getInstance()->getRunningScene()->getChildByName("SIGNAL"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("LIGHT3"));
			((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("READY");
			playSound("sound/alarm_ready.mp3", false);
	}),
		DelayTime::create(1.4f),
		CallFunc::create([=](void) {
			//beep 2
			//go
			gameMode = GAMEMODE::GO;
			speed = 0;
			//log("go");
			((Sprite*)Director::getInstance()->getRunningScene()->getChildByName("SIGNAL"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("LIGHT4"));
			playSound("sound/alarm_go2.mp3", false);
			((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("GO");
			animaSlow();
			((Sprite*)Director::getInstance()->getRunningScene()->getChildByName("SIGNAL"))->runAction(Sequence::create(
				DelayTime::create(1.2),
				MoveBy::create(0.9, Vec2(0, 200)),
				CallFunc::create([=](void) {
				((Sprite*)Director::getInstance()->getRunningScene()->getChildByName("SIGNAL"))->removeFromParent();
				playSound("sound/ignition.mp3", false);
			}),
				NULL
			));
	}),
		NULL
	));
}
void GameLayer::modeGo(float dt)
{
	if (footBreak)
	{
		speed -= 0.1;
		if (speed < 0)
			speed = 0;
	}

	if (!gearHigh)
	{
		if (speed < 6)
		{
			speed += 0.05f;
		}
		else
		{
			nowRegenDelay = regenDelay[0] * 2;
			speed = 6;
			gameMode = GAMEMODE::LOW;
			animaFast();
			playSound("sound/lowengine.mp3", true);
		}
	}
	else
	{
		//끼기기기기
		((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("OVER RUN");
	}

	scrollStage(dt);

	if (speed < 6)
	{
		mycar->setPositionX(mycar->getPositionX() + carMover / 6.0f);
		mycar->setRotation(carMover * 2 / 6.0f);
	}
	else
	{
		mycar->setPositionX(mycar->getPositionX() + carMover);
		mycar->setRotation(carMover * 2);
	}
}
void GameLayer::modePlay(float dt)
{
	if (gameMode == GAMEMODE::BREAKING)
	{
		if (!footBreak)
		{
			if (speed < 6)
			{
				if(!gearHigh)
					speed += 0.05f;
				else
				{
					((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("OVER RUN");
				}
			}
			else
			{
				speed = 6;
				gameMode = GAMEMODE::LOW;
				animaFast();
				playSound("sound/lowengine.mp3", true);
			}
		}
	}
	else
	{
		if (!footBreak)
		{
			if (!gearHigh)
			{
				if (gameMode != GAMEMODE::LOW)
					playSound("sound/lowengine.mp3", true);
				gameMode = GAMEMODE::LOW;
				speed = 6;

			}
			else
				if (gearHigh)
				{
					if (gameMode != GAMEMODE::HIGH)
						playSound("sound/highengine.mp3", true);
					gameMode = GAMEMODE::HIGH;
					speed = 9;
				}
		}
	}

	if (footBreak)
	{
		float prespd = speed;
		speed -= 0.1;
		if (speed < 0)
			speed = 0;
		if (speed <= 6 && gameMode == GAMEMODE::HIGH)
		{
			playSound("sound/lowengine.mp3", true);
			gameMode = GAMEMODE::LOW;
		}
		if (speed <= 2)// && gameMode == GAMEMODE::LOW)
		{
			gameMode = GAMEMODE::BREAKING;
			animaSlow();
		}
	}

	scrollStage(dt);

	if (layerMode == 0)
		return;

	roadTime--;// -= speed * 0.002;
	if (roadTime <= 0)
	{
		_roadMode1++;
		_roadMode2++;
		roadTime = roadTimes[_roadMode1 % roadTimes.size()];
	}

	mycar->setPositionX(mycar->getPositionX() + carMover);
	mycar->setRotation(carMover * 2);

	//충돌 체크
	auto rect1 = mycar->getChildByName("BODY")->getBoundingBox();// +Rect(my->getPositionX(), my->getPositionY(), 0, 0);
	Rect rect_1 = Rect(Vec2(rect1.getMinX() + mycar->getPositionX(), rect1.getMinY() + mycar->getPositionY()),
		mycar->getChildByName("BODY")->getContentSize()
	);
	bool isCrash = false;
	for (const auto temp : scrollLayer->getChildren())
	{
		if (temp == mycar || immortal)
			continue;

		Sprite* othercar = (Sprite*)(temp);
		if (othercar->getName().compare("CAR") == 0)
		{
			auto rect2 = othercar->getChildByName("BODY")->getBoundingBox();
			Rect rect_2 = Rect(Vec2(rect2.getMinX() + othercar->getPositionX(), rect2.getMinY() + othercar->getPositionY()),
				othercar->getChildByName("BODY")->getContentSize()
			);
			if (rect_1.intersectsRect(rect_2))
			{
				pushCrashPos();
				gameMode = GAMEMODE::CRASH;
				isCrash = true;
				//setCrashAni();
				mycar->setName("");
				mytire->stopAllActions();
				//mytire->runAction(RepeatForever::create(Animate::create(tireanima_slow)));
			}
			else
			{
			}
		}
		if (othercar->getName().compare("WALL") == 0)
		{
			Sprite* border = (Sprite*)othercar;
			if (rect_1.intersectsRect(border->getBoundingBox()))
			{
				pushCrashPos();
				gameMode = GAMEMODE::CRASH;
				isCrash = true;
				//setCrashAni();
				mycar->setName("");
				mytire->stopAllActions();
				//mytire->runAction(RepeatForever::create(Animate::create(tireanima_slow)));
			}
		}
	}
	if (isCrash)
		setCrashAni();

	//에너미 카 처리/생성
	if (layerMode != 0)
	{

		auto lit = scrollLayer->getChildren();
		for (auto iter = lit.begin(); iter != lit.end();)
		{
			if ((*iter)->getName().compare("CAR") == 0 && (*iter) != mycar)
			{
				Sprite* ecar = (Sprite*)(*iter);
				CarData* temp = (CarData*)ecar->getUserData();
				temp->scrollspeed = speed;
				temp->update(dt);

				if ((*iter)->getPositionY() < -100 || (*iter)->getPositionY() > 500)
				{
					if ((*iter)->getPositionY() < -100)
						gameScore += temp->score * getValue(scoreBoost);
					delete(temp);
					(*iter)->removeFromParent();
					iter = lit.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			else
			{
				iter++;
			}
		}

		if (speed >= 6)
		{
			nowRegenDelay -= dt;
			if (nowRegenDelay <= 0)
			{
				regenStep++;
				nowRegenDelay = regenDelay[regenStep % 40] * 2;

				auto car = createRival(rivalRate.at(random(0, 19)));
				float _width = roadWidth / 2.0f;
				if (_width < 30)
					_width = 30;
				float _cx = 0;
				if (regenPosX[regenStep % 30])
					_cx = random(leftX + 15, leftX + _width - 15);
				else
					_cx = random(leftX + 15 + _width, leftX + roadWidth - 15);
				if (_cx < leftX + 15)
					_cx = leftX + 15;
				if (_cx > leftX + roadWidth - 15)
					_cx = leftX + roadWidth - 15;
				car->setPosition(_cx, 260);
				car->setName("CAR");
				car->setLocalZOrder(1);
				scrollLayer->addChild(car);
				cnt++;
			}
		}
	}

}
void GameLayer::modeCrashNRestart(float dt)
{

	auto lit = scrollLayer->getChildren();
	for (auto iter = lit.begin(); iter != lit.end();)
	{
		if ((*iter)->getName().compare("CAR") == 0 && (*iter) != mycar)
		{
			Sprite* ecar = (Sprite*)(*iter);
			CarData* temp = (CarData*)ecar->getUserData();
			temp->scrollspeed = 0;
			temp->update(dt);

			if ((*iter)->getPositionY() < -100 || (*iter)->getPositionY() > 500)
			{
				delete(temp);
				(*iter)->removeFromParent();
				iter = lit.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}

	if (gameMode == GAMEMODE::CRASH)
	{
		((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("CRASH");
		speed = 0;
		gameMode = GAMEMODE::DELAY;
		runAction(Sequence::create(
			DelayTime::create(1.0),
			CallFunc::create([=](void) {
				if (gameMode != GAMEMODE::GAMEOVER)
					gameMode = GAMEMODE::REGEN;
		}),
			NULL
			));
	}
	if (gameMode == GAMEMODE::REGEN)
	{
		((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("READY");
		gameMode = GAMEMODE::DELAY;
		runAction(Sequence::create(
			DelayTime::create(1.0),
			CallFunc::create([=](void) {
				//마이카 도로 중앙에 재위치
				mycar->setPositionX(restartX + restartWidth / 2);//충돌할 때 같은 높이에 있었던 도로의 포지션을 기억하도록 한다
				if (gameMode != GAMEMODE::GAMEOVER)
				{
					gameMode = GAMEMODE::RESTART;
					playSound("sound/ignition.mp3", false);
				}

				animaSlow();

				speed = 0.1;
				mycar->setName("CAR");
				mycar->runAction(Sequence::create(
					Blink::create(2.1, 3),
					CallFunc::create([=](void) {
					mycar->setVisible(true);
				}),
					NULL
				));
		}),
			NULL
			));
	}
	if (gameMode == GAMEMODE::RESTART)
	{
		//기어가 로우에 있으면 다시 가속
		if (!gearHigh)
		{
			if (speed < 6)
				speed += 0.05f;
			else
			{
				speed = 6;
				gameMode = GAMEMODE::LOW;
				animaFast();
				playSound("sound/lowengine.mp3", true);
			}
			((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString(SPRINTF("%03.2f", speed * 18));
		}
		else
		{
			//끼기기기기
			//speed = 0.1;
			((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("OVER RUN");
		}
		scrollStage(dt);

		if (speed < 6)
		{
			mycar->setPositionX(mycar->getPositionX() + carMover / 6.0f);
			mycar->setRotation(carMover * 2 / 6.0f);
		}
		else
		{
			mycar->setPositionX(mycar->getPositionX() + carMover);
			mycar->setRotation(carMover * 2);
		}
	}
}
void GameLayer::modeGameOver(float dt)
{
	if (mytire->getNumberOfRunningActions() > 0)
		mytire->stopAllActions();
}
void GameLayer::scrollStage(float dt)
{

	float newBorderY = 0;
	auto lists = scrollLayer->getChildren();
	for (int i=0;i<lists.size();i++)
	{
		Node* node = lists.at(i);

		//도로
		if (node->getName().compare("ASPHALT") == 0)
		{
			node->setPositionY(node->getPositionY() - speed);
			if (node->getPositionY() <= -48)
			{
				node->setPositionY(node->getPositionY() + 48 * 8);
			}
		}

		//좌우벽
		if (node->getName().compare("WALL") == 0)
		{
			node->setPositionY(node->getPositionY() - speed);
			if (node->getPositionY() <= -64)
			{
				int tag = node->getTag();
				auto newnode = Sprite::createWithSpriteFrameName(bordername[stage * 2 + tag % 2]);
				newnode->setTag(tag);
				newnode->setAnchorPoint(node->getAnchorPoint());
				newnode->setPositionY(node->getPositionY() + 64 * 6);
				newBorderY = node->getPositionY() + 64 * 6;
				if (tag % 2 == 0)
				{
					if (roadMode1[_roadMode1 % roadMode1.size()] == 1 && leftX > 50)
						leftX--;
					if (roadMode1[_roadMode1 % roadMode1.size()] == 2 && leftX < 150)
						leftX++;

					if (roadMode2[_roadMode2 % roadMode2.size()] == 1 && roadWidth < 280)
						roadWidth++;
					if (roadMode2[_roadMode2 % roadMode2.size()] == 2 && roadWidth > 100)
						roadWidth--;

					newnode->setPositionX(leftX);
				}
				else
					newnode->setPositionX(leftX + roadWidth);
				bgObjCreate++;
				scrollLayer->addChild(newnode);
				newnode->setName("WALL");
				node->removeFromParent();
				//log("DELETE");
				//deleteList.pushBack(node);
			}
		}

		//통상 오브제
		if (node->getName().compare("OBJ") == 0)
		{
			node->setPositionY(node->getPositionY() - speed);
			if (node->getPositionY() <= -100)
			{
				node->removeFromParent();
				//log("DELETE");
				//deleteList.pushBack(node);
			}
		}
	}

	//for (int i = 0; i < 8; i++)
	//{
	//	auto road = scrollLayer->getChildByTag(100 + i);
	//	road->setPositionY(road->getPositionY() - speed);
	//	if (road->getPositionY() <= -48)
	//	{
	//		road->setPositionY(road->getPositionY() + 48 * 8);
	//	}
	//}

	//float newBorderY = 0;
	//for (int i = 0; i < 12; i++)
	//{
	//	auto border = scrollLayer->getChildByTag(200 + i);
	//	border->setPositionY(border->getPositionY() - speed);
	//	if (border->getPositionY() <= -64)
	//	{
	//		int tag = border->getTag();
	//		auto newBorder = Sprite::createWithSpriteFrameName(bordername[stage * 2 + tag % 2]);
	//		newBorder->setTag(tag);
	//		newBorder->setAnchorPoint(border->getAnchorPoint());
	//		newBorder->setPositionY(border->getPositionY() + 64 * 6);
	//		newBorderY = border->getPositionY() + 64 * 6;
	//		if (tag % 2 == 0)
	//			newBorder->setPositionX(leftX);
	//		else
	//			newBorder->setPositionX(leftX + roadWidth);
	//		bgObjCreate++;
	//		scrollLayer->addChild(newBorder);
	//		newBorder->setName("WALL");
	//		border->removeFromParent();
	//	}
	//}

	//도로 오브제 생성
	switch (stage)
	{
	case 0:
	case 2:
		if (newBorderY != 0)
		{
			auto treeL = Sprite::createWithSpriteFrameName("TREE");
			treeL->setPosition(leftX - 20, newBorderY);
			treeL->setName("OBJ");
			treeL->setLocalZOrder(1);
			scrollLayer->addChild(treeL);

			auto treeR = Sprite::createWithTexture(treeL->getTexture());
			treeR->setPosition(leftX + roadWidth + 20, newBorderY);
			treeR->setName("OBJ");
			treeR->setLocalZOrder(1);
			scrollLayer->addChild(treeR);

			int housecreate = 0;
			if (bgObjCreate % 16 == 0)
				housecreate = random(10, 30) / 10;

			if (housecreate == 1 || housecreate == 3)
			{
				Sprite* houseL;
				bool blue = (random(0, 5) == 4);
				if (blue)
					houseL = Sprite::createWithSpriteFrameName("HOUSELB");
				else
					houseL = Sprite::createWithSpriteFrameName("HOUSEL");
				houseL->setAnchorPoint(Vec2(1, 0));
				houseL->setLocalZOrder(1);
				houseL->setPosition(leftX - 35, newBorderY);
				houseL->setName("OBJ");
				scrollLayer->addChild(houseL);
			}
			if (housecreate == 2 || housecreate == 3)
			{
				Sprite* houseR;
				bool green = (random(0, 5) == 4);
				if (green)
					houseR = Sprite::createWithSpriteFrameName("HOUSERG");
				else
					houseR = Sprite::createWithSpriteFrameName("HOUSER");
				houseR->setAnchorPoint(Vec2(0, 0));
				houseR->setLocalZOrder(1);
				houseR->setPosition(leftX + roadWidth + 35, newBorderY);
				houseR->setName("OBJ");
				scrollLayer->addChild(houseR);
			}
		}
		break;
	case 1:
		if (newBorderY != 0)
		{
			int housecreate = 0;
			if (bgObjCreate % 16 == 0)
			{
				housecreate = random(10, 30) / 10;

				if (housecreate == 3)
				{
					auto ship = Sprite::createWithSpriteFrameName("SHIP");
					ship->setPosition(leftX + roadWidth + 60, newBorderY);
					ship->setLocalZOrder(1);
					ship->setAnchorPoint(Vec2(0, 0.5));
					ship->setName("OBJ");
					scrollLayer->addChild(ship);
				}
				else
				{
					auto boat = Sprite::createWithSpriteFrameName("BOAT");
					boat->setLocalZOrder(1);
					boat->setName("OBJ");
					scrollLayer->addChild(boat);

					if (housecreate == 1)
					{
						boat->setPosition(leftX - random(30, 60), newBorderY);
					}
					if (housecreate == 2)
					{
						boat->setPosition(leftX + roadWidth + random(30, 60), newBorderY);
					}
				}
			}
		}
		break;
	case 3:
		if (newBorderY != 0)
		{
			if (bgObjCreate % 20 > 10 && bgObjCreate % 2 == 0)
			{
				int housecreate = random(1, 2);
				auto people = Sprite::createWithSpriteFrameName(random(0, 1) == 0 ? "VACANCE" : "PARASOL");
				people->setLocalZOrder(1);
				people->setName("OBJ");
				scrollLayer->addChild(people);

				if (housecreate == 1)
				{
					people->setPosition(leftX - random(23, 56), newBorderY);
				}
				if (housecreate == 2)
				{
					people->setPosition(leftX + roadWidth + random(23, 56), newBorderY);
				}
			}
		}
		break;
	case 4:
		if (newBorderY != 0)
		{
			auto treeL = Sprite::createWithSpriteFrameName("TREE");
			treeL->setPosition(leftX - 35, newBorderY);
			treeL->setName("OBJ");
			treeL->setLocalZOrder(1);
			scrollLayer->addChild(treeL);

			auto treeR = Sprite::createWithTexture(treeL->getTexture());
			treeR->setPosition(leftX + roadWidth + 35, newBorderY);
			treeR->setName("OBJ");
			treeR->setLocalZOrder(1);
			scrollLayer->addChild(treeR);

			//log("bgObjCreate %% 20 %d", bgObjCreate % 20);
			if (bgObjCreate % 20 > 10)
				log("bgObjCreate %% 12 %d", bgObjCreate % 12);
			if (bgObjCreate % 20 > 10 && (bgObjCreate % 12 == 2 || bgObjCreate % 12 == 8))
			{
				int housecreate = random(10, 31) / 10;
				string fname[3] = {
					"BUILDING1",
					"BUILDING2",
					"PARKING",
				};
				auto buil = Sprite::createWithSpriteFrameName(fname[housecreate - 1]);
				buil->getTexture()->setAliasTexParameters();
				buil->setLocalZOrder(1);
				buil->setName("OBJ");
				buil->setAnchorPoint(Vec2(1, 0.5));
				buil->setScale(2);
				buil->setPosition(leftX - 55, newBorderY);
				scrollLayer->addChild(buil);
			}
			if (bgObjCreate % 20 > 10 && (bgObjCreate % 12 == 4 || bgObjCreate % 12 == 6 || bgObjCreate % 12 == 10))
			{
				int housecreate = random(10, 21) / 10;
				string fname[2] = {
					"BEDS",
					"FOUNTAIN",
				};
				auto buil = Sprite::createWithSpriteFrameName(fname[housecreate - 1]);
				buil->getTexture()->setAliasTexParameters();
				buil->setLocalZOrder(1);
				buil->setName("OBJ");
				buil->setAnchorPoint(Vec2(0, 0.5));
				buil->setPosition(leftX + roadWidth + random(9, 15) * 5 + (housecreate - 1) * 30, newBorderY);
				scrollLayer->addChild(buil);
			}
		}
		break;
	}

	//if (deleteList.size() > 0)
	//{
	//	log("DELETE BEGIN");
	//	for (auto it = deleteList.begin(); it != deleteList.end(); ++it)
	//	{
	//		(*it)->removeFromParent();
	//	}
	//}
}

void GameLayer::update(float dt)
{
	switch (gameMode)
	{
	case GAMEMODE::GO:
		modeGo(dt);
		break;
	case GAMEMODE::LOW:
	case GAMEMODE::HIGH:
	case GAMEMODE::BREAKING:
		modePlay(dt);
		break;
	case GAMEMODE::CRASH:
	case GAMEMODE::DELAY:
	case GAMEMODE::REGEN:
	case GAMEMODE::RESTART:
		modeCrashNRestart(dt);
		break;
	case GAMEMODE::GAMEOVER:
		modeGameOver(dt);
		break;
	}

	if (gameMode != GAMEMODE::GAMEOVER && gameMode != GAMEMODE::READY && layerMode != 0)
	{
		gameTime -= dt;
		if (gameTime <= 0)
		{
			if (gameMode == GAMEMODE::HIGH)
			{
				gameTime += 6;
				extCount++;
				((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("EXTEND");
				((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->runAction(Sequence::create(
					Blink::create(1, 5),
					CallFunc::create([=](void) {
					((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setVisible(false);
				}),
					NULL
					));
				AudioEngine::play2d("sound/se_maoudamashii_chime13.mp3", false, 1);
				//se_maoudamashii_retro16
			}
			else
			{
				stopSound();
				gameMode = GAMEMODE::GAMEOVER;
				((Label*)Director::getInstance()->getRunningScene()->getChildByName("LOG"))->setString("GAME OVER");

				runAction(Sequence::create(
					DelayTime::create(1),
					CallFunc::create([=](void) {
						auto touchmen = Button::create("blank1x1.png", "blank1x1.png", "blank1x1.png", Widget::TextureResType::LOCAL);
						touchmen->setSwallowTouches(true);
						touchmen->setScale(1920);
						touchmen->setPosition(Vec2(visibleSize * 0.5f) + origin);
						touchmen->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
							if (type == Widget::TouchEventType::ENDED)
							{
								//log("GAME OVER %f", scoreAdd);
								accumScore = getValue(getValue(accumScore) + (int)gameScore);
								checkAchieve();
								java_FIREBASE(1, (int)gameScore);
								if (getValue(myHighscore) < gameScore)
								{
									myHighscore = getValue((int)gameScore);
									java_MYSCORE();
									saveGame();
									auto scene = NewrecordScene::createScene();
									Director::getInstance()->replaceScene(scene);
									return;
								}
								else
									saveGame();
								auto scene = TitleScene::createScene();
								Director::getInstance()->replaceScene(scene);
								java_AD();
							}
						});
						Director::getInstance()->getRunningScene()->addChild(touchmen);

						auto tabscreen = Label::createWithTTF("TAB SCREEN", "fonts/press-start.regular.ttf", 70, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
						tabscreen->setPosition(Vec2(visibleSize * 0.5f) + Vec2(0, -250));
						tabscreen->enableShadow(Color4B::BLACK, Size(10, -10), 0);
						Director::getInstance()->getRunningScene()->addChild(tabscreen);
				}),
					NULL
				));

				auto gameover = Label::createWithTTF("GAME OVER", "fonts/press-start.regular.ttf", 120, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
				gameover->setPosition(visibleSize * 0.5f);
				gameover->runAction(RepeatForever::create(Sequence::create(
					//TintTo::create(0.03f, Color3B::MAGENTA),
					TintTo::create(0.03f, Color3B::YELLOW),
					TintTo::create(0.03f, Color3B::WHITE),
					TintTo::create(0.03f, Color3B::BLACK),
					//TintTo::create(0.03f, Color3B::RED),
					TintTo::create(0.03f, Color3B::GREEN),
					TintTo::create(0.03f, Color3B::WHITE),
					NULL
				)));
				Director::getInstance()->getRunningScene()->addChild(gameover);

				return;
			}
		}

		gameScore += (speed * scoreAdd) * getValue(scoreBoost);
		scoreAdd += 0.00001f;

		if(((Label*)Director::getInstance()->getRunningScene()->getChildByName("TIME")))
			((Label*)Director::getInstance()->getRunningScene()->getChildByName("TIME"))->setString(SPRINTF("%02d", (int)(gameTime + 0.9)));
		if (((Label*)Director::getInstance()->getRunningScene()->getChildByName("SCORE")))
			((Label*)Director::getInstance()->getRunningScene()->getChildByName("SCORE"))->setString(SPRINTF("%03d", (int)gameScore));

		//carAniTime -= dt;
		//if (carAniTime <= 0)
		//{
		//	carAniTime = carAniTimeDef;
		//	myAnimStep++;
		//	//mytire->set
		//}
	}


}
Sprite* GameLayer::createRival(int kind)
{
	string tireFn = "RIVALTIRE";
	string bodyFn = "rivalcar.png";
	int wd = 23;
	int ht = 30;
	int adjx = 0;
	CarData* cardata = new CarData();
	cardata->score = 12;

	cardata->scrollspeed = speed;
	if (random(1, 10) > 5)
		cardata->speed = 8;

	if (random(0, 10) <= extCount)
		cardata->xmove = random(-0.3f, 0.3f);
	else
		cardata->xmove = 0;

	if (kind == 1)
	{
		bodyFn = "gencar.png";
		int kkind = random(0, 30);
		if (kkind > 4)
			kkind = 0;
		if (kkind == 4 && gameScore > 5000)
			kkind += random(0, 1);
		tireFn = SPRINTF("GENTIRE%d", kkind);
		switch (kkind)
		{
		case 0:
			cardata->speed = 4.2;
			cardata->score = 1.5;
			cardata->hp = 3;
			break;
		case 1:
			cardata->speed = 3.4;
			cardata->score = 2.0;
			cardata->hp = 4;
			break;
		case 2:
			cardata->speed = 4.7;
			cardata->score = 2.5;
			cardata->hp = 2;
			if(cardata->xmove == 0)
				cardata->xmove = random(-0.5f, 0.5f);
			break;
		case 3:
			cardata->speed = 4.7;
			cardata->score = 2.5;
			cardata->hp = 2;
			if (cardata->xmove == 0)
				cardata->xmove = random(-0.5f, 0.5f);
			break;
		case 4:
			cardata->speed = 5.2;
			cardata->score = 3;
			cardata->hp = 3;
			break;
		case 5:
			cardata->speed = 7.5;
			cardata->score = 3;
			cardata->hp = 3;
			break;
		}
	}
	if (kind == 2)
	{
		tireFn = SPRINTF("TRTIRE%d", random(0, 1));
		bodyFn = "trailer.png";
		wd = 24;
		ht = 73;
		adjx = 1;
		cardata->score = 3.5;
		cardata->speed = 6.5;
		cardata->hp = 1000;
		if (random(1, 10) < 8)
			cardata->xmove = 0;
	}

	Vector<SpriteFrame*> tireani;
	for (int i = 0; i < 4; i++)
	{
		tireani.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s%d", tireFn.c_str(), i)));
	}
	auto tireanima = Animation::createWithSpriteFrames(tireani, 0.05f);

	auto car = Sprite::create("blank1x1.png");
	auto body = Sprite::create(bodyFn);
	auto tire = Sprite::createWithSpriteFrameName(StringUtils::format("%s1", tireFn.c_str()));
	body->setName("BODY");
	body->getTexture()->setAliasTexParameters();
	body->setVisible(false);
	tire->setPosition(adjx , -1);
	car->addChild(tire);
	car->addChild(body);
	tire->runAction(RepeatForever::create(Animate::create(tireanima)));

	cardata->my = car;
	car->setUserData(cardata);
	//car->schedule(schedule_selector(CarData::update));

	//log("new car %d", kind);
	return car;
}
void GameLayer::pushCrashPos()
{
	Rect rect = Rect(0, mycar->getPositionY(), 256, 1);
	for (int i = 0; i < 6; i++)
	{
		auto temp = scrollLayer->getChildByTag(200 + i * 2);
		if (rect.intersectsRect(temp->getBoundingBox()))
		{
			restartX = temp->getPositionX();
			restartWidth = scrollLayer->getChildByTag(200 + i * 2 + 1)->getPositionX() - restartX;
		}
	}
}
void GameLayer::animaFast()
{
	mytire->stopAllActions();
	if (gameMode == GAMEMODE::GAMEOVER)
		return;
	Vector<SpriteFrame*> tireani;
	for (int i = 0; i < 4; i++)
	{
		tireani.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(SPRINTF("TIRE%d", i)));
	}
	mytire->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(tireani, 0.05f))));
}
void GameLayer::animaSlow()
{
	mytire->stopAllActions();
	if (gameMode == GAMEMODE::GAMEOVER)
		return;
	Vector<SpriteFrame*> tireani;
	for (int i = 0; i < 4; i++)
	{
		tireani.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(SPRINTF("TIRE%d", i)));
	}
	mytire->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(tireani, 0.2f))));
}
void GameLayer::setCrashAni()
{
	auto boom = Sprite::createWithSpriteFrameName("EXPLODE0");
	Vector<SpriteFrame*> explodeani;
	for (int i = 0; i < 5; i++)
	{
		explodeani.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(SPRINTF("EXPLODE%d", i)));
	}
	boom->runAction(Sequence::create(
		Animate::create(Animation::createWithSpriteFrames(explodeani, 0.15f)),
		RemoveSelf::create(),
		NULL
	));
	boom->setPosition(mycar->getPosition());
	boom->setLocalZOrder(3);
	mycar->setVisible(false);
	scrollLayer->addChild(boom);
	for (int i = 0; i < 8; i++)
	{
		auto road = scrollLayer->getChildByTag(100 + i);
		road->runAction(Repeat::create(Sequence::create(
			TintTo::create(0.03f, Color3B::MAGENTA),
			TintTo::create(0.03f, Color3B::YELLOW),
			TintTo::create(0.03f, Color3B::WHITE),
			TintTo::create(0.03f, Color3B::BLACK),
			TintTo::create(0.03f, Color3B::RED),
			TintTo::create(0.03f, Color3B::GREEN),
			TintTo::create(0.03f, Color3B::WHITE),
			NULL
		), 5));
	}

	for (int i = 0; i < 12; i++)
	{
		auto border = scrollLayer->getChildByTag(200 + i);
		border->runAction(Repeat::create(Sequence::create(
			TintTo::create(0.03f, Color3B::MAGENTA),
			TintTo::create(0.03f, Color3B::YELLOW),
			TintTo::create(0.03f, Color3B::WHITE),
			TintTo::create(0.03f, Color3B::BLACK),
			TintTo::create(0.03f, Color3B::RED),
			TintTo::create(0.03f, Color3B::GREEN),
			TintTo::create(0.03f, Color3B::WHITE),
			NULL
		), 5));

	}
	stopSound();
	playSound("sound/se_maoudamashii_explosion06.mp3", false);
}
void GameLayer::changeCar(int kind)
{
	for (int i = 0; i < 4; i++)
	{
		SpriteFrameCache::getInstance()->removeSpriteFrameByName(SPRINTF("TIRE%d", i));
		auto tiretex = SpriteFrame::create(SPRINTF("mytire%d.png", kind), Rect(i * 22, 0, 22, 31));
		tiretex->getTexture()->setAliasTexParameters();
		SpriteFrameCache::getInstance()->addSpriteFrame(tiretex, SPRINTF("TIRE%d", i));
	}
	animaFast();
}
void GameLayer::makeRoad()
{
	int kind = random(0,3);
	switch (kind)
	{
	case 0:
	{
		int _roadTimes[8] = {
			300,
			100,
			200,
			100,
			100,
			300,
			100,
			100
		};
		int _roadModes1[8] = {
			0,//neurtral
			1,//left
			0,
			2,//right
			0,
			2,
			0,
			1
		};
		int _roadModes2[9] = {
			0,
			0,
			0,//neurtral
			1,//wide
			0,
			2,//narrow
			0,
			2,
			0
		};
		for (int i = 0; i < 8; i++)
			roadTimes.push_back(_roadTimes[i]);
		for (int i = 0; i < 8; i++)
			roadMode1.push_back(_roadModes1[i]);
		for (int i = 0; i < 9; i++)
			roadMode2.push_back(_roadModes2[i]);
	}
	case 1:
	{
		int _roadTimes[9] = {
			240,
			300,
			300,
			300,
			300,
			400,
			100,
			160
		};
		int _roadModes1[14] = {
			0,
			0,
			0,
			0,
			1,
			1,
			2,
			2,
			2,
			2,
			0,
			0,
			1,
			1
		};
		int _roadModes2[9] = {
			0,
			2,
			2,
			0,
			0,
			1,
			0,
			2,
			0
		};
		for (int i = 0; i < 9; i++)
			roadTimes.push_back(_roadTimes[i]);
		for (int i = 0; i < 14; i++)
			roadMode1.push_back(_roadModes1[i]);
		for (int i = 0; i < 9; i++)
			roadMode2.push_back(_roadModes2[i]);
	}
	case 2:
	{
		int _roadTimes[7] = {
			300,
			180,
			180,
			180,
			180,
			120,
			120
		};
		int _roadModes1[6] = {
			0,
			1,
			2,
			2,
			1,
			0
		};
		int _roadModes2[11] = {
			0,
			0,
			0,
			0,
			0,
			1,
			1,
			0,
			0,
			2,
			2
		};
		for (int i = 0; i < 7; i++)
			roadTimes.push_back(_roadTimes[i]);
		for (int i = 0; i < 6; i++)
			roadMode1.push_back(_roadModes1[i]);
		for (int i = 0; i < 11; i++)
			roadMode2.push_back(_roadModes2[i]);
	}
	case 3:
	{
		int _roadTimes[8] = {
			300,
			100,
			200,
			100,
			100,
			300,
			100
		};
		int _roadModes1[8] = {
			0,//neurtral
			1,//left
			0,
			2,//right
			0,
			2,
			0,
			1
		};
		int _roadModes2[9] = {
			0,
			0,
			0,//neurtral
			1,//wide
			0,
			2,//narrow
			0,
			2,
			0
		};
		for (int i = 0; i < 8; i++)
			roadTimes.push_back(_roadTimes[i]);
		for (int i = 0; i < 8; i++)
			roadMode1.push_back(_roadModes1[i]);
		for (int i = 0; i < 9; i++)
			roadMode2.push_back(_roadModes2[i]);
	}
	break;
	}


	if (random(0, 1) == 1)
	{
		float _regenDelay[27] = {
			0.4f,
			1.9f,
			0.4f,
			1.5f,
			0.7f,
			1.5f,
			0.5f,
			0.5f,
			1.2f,
			1.1f,
			1,
			1.1f,
			1.1f,
			0.5f,
			1,
			1,
			0.5f,
			1.1f,
			1.5f,
			0.8f,
			0.4f,
			1.9f,
			0.4f,
			2.5f,
			0.9f,
			1.7f,
			0.5f
		};
		for (int i = 0; i < 40; i++)
			regenDelay[i] = _regenDelay[i % 27];
	}
}