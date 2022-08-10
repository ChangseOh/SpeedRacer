#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform\android\jni\JniHelper.h"
#endif

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

#define SPRINTF StringUtils::format

enum GAMEEVENT {
	OPEN2CAR = 0,
	OPEN3CAR,
	OPEN2BG,
	OPEN4CAR,
	OPEN5CAR,
	OPEN6CAR,
	OPEN7CAR,
	OPEN3BG,
	OPEN8CAR,
	GET2X,
	OPEN9CAR,
	OPEN10CAR,
	OPEN11CAR,
	OPEN12CAR,
	OPEN13CAR,
	OPEN4BG,
	OPEN14CAR,
	OPEN15CAR,
	GET3X,
	OPEN16CAR,
	OPEN17CAR,
	OPEN18CAR,
	OPEN19CAR,
	OPEN5BG,
	OPEN20CAR
};

extern bool gearHigh;
extern bool footBreak;
extern Size visibleSize;
extern Vec2 origin;
extern int bgmID;
extern int selectedCar;
extern int myCountry;
extern int myRank;
extern int myHighscore;
extern string myName;

extern vector<int> haveCars;
extern vector<int> acceptBgs;
extern vector<GAMEEVENT> gameEvents;
extern int accumScore;
extern int openCars;
extern int openBgs;
extern bool clearAchieve[100];
extern int scoreBoost;

extern int signinmode;

extern string achieveStr[25];
extern int achieveScore[25];

extern vector<string> rankings;

extern int javaCallBack;

void playSound(string fname, bool);
void stopSound();

extern bool clearAchieve[100];
void checkAchieve();

void initGame();
void saveGame();
bool loadGame();
int getValue(int raw);
void checkMyHave();

void java_FIREBASE(int, int);
void java_AD();
void java_MYSCORE();
void java_RANKING();
void java_CALLBOARD();
void java_SIGNIN();
void java_SIGNOUT();
#endif
