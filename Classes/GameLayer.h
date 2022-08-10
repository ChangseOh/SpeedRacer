#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "GameManager.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d::experimental;
using namespace cocos2d;
using namespace std;

class CarData
{
public:
	float xmove = 0;
	float ymove = -1;
	float speed = 6;
	float scrollspeed;
	float score;
	int hp = 7;
	Sprite* my;
	void update(float dt)
	{
		auto lists = my->getParent()->getChildren();
		auto ebody = my->getChildByName("BODY");
		auto rect1 = ebody->getBoundingBox();
		for (const auto others : lists)
		{
			if (my == others)
				continue;

			Rect rect_1 = Rect(Vec2(rect1.getMinX() + my->getPositionX(), rect1.getMinY() + my->getPositionY()),
				ebody->getContentSize()
			);
			Sprite* othercar = (Sprite*)(others);
			if (others->getName().compare("CAR") == 0)
			{
				auto rect2 = othercar->getChildByName("BODY")->getBoundingBox();
				Rect rect_2 = Rect(Vec2(rect2.getMinX() + othercar->getPositionX(), rect2.getMinY() + othercar->getPositionY()),
					othercar->getChildByName("BODY")->getContentSize()
				);
				if (rect_1.intersectsRect(rect_2))
				{
					hp--;
					if (my->getPositionY() > othercar->getPositionY())
						ymove = -1;
					if (my->getPositionX() < othercar->getPositionX())
						xmove = abs(xmove) * (-1);
					else
						if (my->getPositionX() > othercar->getPositionX())
							xmove = abs(xmove);
				}
				else
				{
				}
			}
			if (others->getName().compare("WALL") == 0)
			{
				Sprite* border = (Sprite*)others;
				if (rect_1.intersectsRect(border->getBoundingBox()))
				{
					//hp--;
					//if (hp > 10 || xmove == 0)
					//	speed = speed * 0.5f;
					if (xmove == 0)
						speed = speed * 0.5f;
					else
					{
						if (border->getPositionX() < my->getPositionX())
							xmove = abs(xmove);
						else
							xmove = -abs(xmove);
					}
					break;
				}
			}
		}
		my->setPositionX(my->getPositionX() + xmove);

		if (ymove != 0)
		{
			my->setPositionY(my->getPositionY() - ymove);
			if (ymove > 0)
			{
				ymove -= 0.05;
				if (ymove <= 0)
					ymove = 0;
			}
			else
			{
				ymove += 0.05;
				if (ymove >= 0)
					ymove = 0;
			}
		}
		else
			my->setPositionY(my->getPositionY() - scrollspeed + speed);

		if (hp <= 0)
		{
			//if (-50 <= my->getPositionY() && my->getPositionY() <= 550)
			//{
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
				boom->setPosition(my->getPosition());
				my->getParent()->addChild(boom);

				AudioEngine::play2d("sound/se_maoudamashii_explosion04.mp3", false, 1);
			//}
			my->removeFromParent();
		}
	}
};
enum GAMEMODE {
	READY = 0,
	GO,
	LOW,
	HIGH,
	BREAKING,
	CRASH,
	DELAY,
	REGEN,
	RESTART,
	GAMEOVER
};
class GameLayer : public cocos2d::Layer
{
public:
	static GameLayer* create(int);

	virtual bool init();
	virtual void update(float dt);

	int layerMode;

	string bordername[10] = {
		"GREENLEFT",
		"GREENRIGHT",
		"WATERLEFT",
		"WATERRIGHT",
		"SNOWLEFT",
		"SNOWRIGHT",
		"BEACHLEFT",
		"BEACHRIGHT",
		"CITYLEFT",
		"CITYRIGHT",
	};
	GAMEMODE gameMode = GAMEMODE::READY;

	//Animation* tireanima, *tireanima_slow;
	//Animate* tirego, *tireslow;
	Sprite *mycar, *mytire;
	float restartX, restartWidth;
	int stage = 0;
	void pushCrashPos();
	void animaSlow();
	void animaFast();
	void setCrashAni();

	int bgObjCreate = 0;//장식용 오브제 그리는 정보
	Layer* scrollLayer;
	float leftX = 70;//왼쪽 보더의 위치(X앵커 1)
	float roadWidth = 116;//길의 폭 (왼쪽 보더의 위치 + 길 폭 = 오른쪽 보더의 위치(X앵커 0))

	int gameStep = 0;//보너스 타임 조정
	float gameTime = 60;//게임시간
	float gameScore = 0;//게임득점
	int extCount = 0;//익스텐드 횟수

	bool isKeyDown = false;
	EventKeyboard::KeyCode kcode;
	float speed = 0;//스피드(스크롤 스피드)
	float carMover = 0;
	float scoreAdd = 0.01f;//점수 가산치

	//int myAnimStep = 0;//플레이어 차 애니메이션 프레임
	//float carAniTime;//플레이어 차 애니메이션 프레임 스텝값
	//float carAniTimeDef;//carAniTime의 회복값

	vector<int> rivalRate;
	float fcnt = 0;
	int cnt = 0;
	Sprite* createRival(int kind);

	bool isHigh = false;
	int steer = 50;

	void scrollStage(float);

	void modeReady(float);
	void modeGo(float);
	void modePlay(float);
	void modeCrashNRestart(float);
	void modeGameOver(float);

	void changeCar(int);

	int regenStep = 0;
	float nowRegenDelay;
	float regenDelay[40] = {
		2,
		1.5f,
		1.2f,
		0.5f,
		1,
		1,
		0.8f,
		1.2f,
		2,
		0.4f,
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
		1.0f,
		0.8f,
		0.4f,
		1.9f,
		0.4f,
		1.2f,
		0.9f,
		1.1f,
		0.5f,
		0.3f,
		0.7f,
		1.1f
	};
	bool regenPosX[30] = {
		true,
		false,
		true,
		false,
		true,
		false,
		true,
		true,
		false,
		false,
		true,
		false,
		true,
		false,
		false,
		true,
		true,
		false,
		true,
		false,
		false,
		false,
		true,
		false,
		true,
		true,
		false,
		false,
		true,
		true
	};

	bool immortal = false;

	int roadTime;
	int _roadMode1 = 0;
	int _roadMode2 = 0;
	vector<int> roadTimes;
	vector<int> roadMode1;
	vector<int> roadMode2;

	void makeRoad();
};

#endif