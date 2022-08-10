#pragma execution_character_set("utf-8")
#include "GameManager.h"
#include "audio/include/AudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform\android\jni\JniHelper.h"
#endif

#define MAINFILE "gamedata.dat"
char codeKey[] = "superspeedracer!@#(*&_9^^)*^096agnkgbdfl;g2";

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

bool gearHigh;
bool footBreak;
Size visibleSize;
Vec2 origin;
int selectedCar;
int myCountry = 28;
int myRank = 99;
int myHighscore;
string myName = "YOU";

int bgmID = AudioEngine::INVALID_AUDIO_ID;

vector<int> haveCars;
vector<int> acceptBgs;
vector<GAMEEVENT> gameEvents;
int accumScore;
int openCars;
int openBgs;
bool clearAchieve[100];
int scoreBoost;

int signinmode;

string achieveStr[25] = {
	"UNLOCK NEW YELLOW CAR!!",//0
	"UNLOCK NEW BLUE CAR!!",
	"OPEN BRIDGE STAGE!",
	"UNLOCK NEW OFF-ROADER!!",
	"UNLOCK NEW SUPER!!",
	"UNLOCK NEW FORMULA!!",//5
	"UNLOCK NEW LOG!!",
	"OPEN SNOW STAGE!",
	"UNLOCK NEW SUPER RED!!",
	"GET SCORE BOOST X2!!",
	"UNLOCK NEW PANZER!!",//10
	"UNLOCK NEW FORMULA WHITE!!",
	"UNLOCK NEW JACKAL!!",
	"UNLOCK NEW 4WD!!",
	"UNLOCK NEW SUPER GOLD!!",
	"OPEN BEACH STAGE!",//15
	"UNLOCK NEW YELLOW COUPE!!",
	"UNLOCK NEW BEAST!!",
	"GET SCORE BOOST X3!!",
	"UNLOCK NEW FORMULA RED!!",
	"UNLOCK NEW BLUE COUPE!!",//20
	"UNLOCK NEW BOAT!!",
	"UNLOCK NEW DOLPHIN!!",
	"OPEN CITY STAGE!",
	"UNLOCK NEW HERO!!",//24
};
int achieveScore[25];

vector<string> rankings;

int javaCallBack;


void playSound(string fname, bool repeat)
{
	if (bgmID != AudioEngine::INVALID_AUDIO_ID)
		AudioEngine::stop(bgmID);
	bgmID = AudioEngine::play2d(fname, repeat, 1.0f);
}
void stopSound()
{
	AudioEngine::stop(bgmID);
}
void initGame()
{
	myHighscore = getValue(0);
	selectedCar = 0;

	accumScore = getValue(0);
	openCars = getValue(1);
	openBgs = getValue(1);
	scoreBoost = getValue(1);

	for (int i = 0; i < 100; i++)
		clearAchieve[i] = false;
}
void saveGame()
{
	ValueMap saveData;
	saveData["HIGH"] = myHighscore;
	//saveData["NAME"] = myName;
	saveData["ACCUM"] = accumScore;
	//saveData["CARS"] = openCars;
	//saveData["BGS"] = openBgs;
	for (int i = 0; i < 100; i++)
		saveData[SPRINTF("ACHIEVES%d", i)] = clearAchieve[i] ? random(0, 100) : random(-100, -1);
	
	auto futil = FileUtils::getInstance();

	//ValueMap 저장 (Parsed XML type)
	string filePath = futil->getWritablePath().append("tempdata.db");
	futil->writeValueMapToFile(saveData, filePath);

	//ValueMap 형태의 temp파일을 다시 바이너리로 읽어들인다
	auto data = futil->getDataFromFile(filePath);
	ssize_t fsize = data.getSize();//futil->getFileSize(filePath);
	unsigned char* temp = data.getBytes();

	//읽어들인 바이너리를 코드화해서 저장한다
	for (int i = 0; i < fsize; i++)
		temp[i] = (temp[i] ^ codeKey[i % sizeof(codeKey)]);
	filePath = futil->getWritablePath().append(MAINFILE);

	auto fp = fopen(futil->getSuitableFOpen(filePath).c_str(), "wb");
	if (fp != NULL)
	{
		fwrite(temp, sizeof(char), fsize, fp);
		fclose(fp);
	}

	//temp파일을 삭제한다
	filePath = futil->getWritablePath().append("tempdata.db");
	futil->removeFile(filePath);

	futil->destroyInstance();
}
bool loadGame()
{
	//바이너리를 읽는다
	Data data;
	ssize_t fsize;
	string filePath;
	auto futil = FileUtils::getInstance();
	string temptilepath = futil->getWritablePath().append("tempdata.tmp");
	if (futil->isFileExist(temptilepath))
	{
		//Google Cloud에서 읽어온 파일로 대체한다
		//CCLOG("load from temp");
		fsize = futil->getFileSize(temptilepath);
		if (fsize == 0)
			return false;

		data = futil->getDataFromFile(temptilepath);

		//읽었으므로 곧바로 삭제한다
		//클라우드에서 받아온 파일이 뻑나있다면 디코딩된 파일을 파싱할 때 앱이 죽는다
		//템프파일은 이미 삭제됐으므로 다시 실행하면 일단 정상 실행 가능하고, 다시 클라우드에서 다운받으면 된다.
		futil->removeFile(temptilepath);
	}
	else
	{
		//CCLOG("load from local");
		filePath = futil->getWritablePath().append(MAINFILE);

		if (!futil->isFileExist(filePath))
			return false;
		fsize = futil->getFileSize(filePath);
		if (fsize == 0)
			return false;

		data = futil->getDataFromFile(filePath);
	}
	unsigned char* szTmp = data.getBytes();

	//읽은 바이너리를 디코딩한다
	for (int i = 0; i < fsize; i++)
		szTmp[i] = (szTmp[i] ^ codeKey[i % sizeof(codeKey)]);

	//디코딩된 데이타로 temp파일을 생성한다
	filePath = futil->getWritablePath().append("tempdata.db");
	auto fp = fopen(futil->getSuitableFOpen(filePath).c_str(), "wb");
	if (fp != NULL)
	{
		fwrite(szTmp, sizeof(char), fsize, fp);
		fclose(fp);
	}
	else return false;

	//temp파일을 ValueMap 형태로 읽어들인다
	ValueMap gameData = futil->getValueMapFromFile(filePath);
	myHighscore = gameData["HIGH"].asInt();
	//myName = gameData["NAME"].asString();
	accumScore = gameData["ACCUM"].asInt();
	//openCars = gameData["CARS"].asInt();
	//openBgs = gameData["BGS"].asInt();
	for (int i = 0; i < 100; i++)
	{
		int aa = gameData[SPRINTF("ACHIEVES%d", i)].asInt();
		clearAchieve[i] = (0 <= aa && aa <= 100);
	}

	//temp파일을 삭제한다
	futil->removeFile(filePath);
	futil->destroyInstance();

	return true;
}
void checkAchieve()
{
	for (int i = 0; i < 25; i++)
	{
		if (!clearAchieve[i] && getValue(accumScore) > getValue(achieveScore[i]))
		{
			gameEvents.push_back((GAMEEVENT)(GAMEEVENT::OPEN2CAR + i));
			clearAchieve[i] = true;
		}
	}
	if (gameEvents.size() > 0)
		saveGame();
}
int getValue(int raw)
{
	char temp[4];
	temp[0] = (char)(raw >> 24);
	temp[1] = (char)(raw >> 16);
	temp[2] = (char)(raw >> 8);
	temp[3] = (char)(raw);

	for (int i = 0; i<4; i++)
		temp[i] ^= codeKey[i % sizeof(codeKey)];

	return (
		((temp[0] & 0xff) << 24) +
		((temp[1] & 0xff) << 16) +
		((temp[2] & 0xff) << 8) +
		(temp[3] & 0xff)
		);
}
void checkMyHave()
{
	haveCars.clear();
	acceptBgs.clear();

	haveCars.push_back(0);
	acceptBgs.push_back(0);
	scoreBoost = getValue(1);

	for (int i = 0; i < 25; i++)
	{
		if (clearAchieve[i])
		{
			switch (GAMEEVENT::OPEN2CAR + i)
			{
			case GAMEEVENT::OPEN2CAR:
				haveCars.push_back(1);
				break;
			case GAMEEVENT::OPEN3CAR:
				haveCars.push_back(2);
				break;
			case GAMEEVENT::OPEN2BG:
				acceptBgs.push_back(1);
				break;
			case GAMEEVENT::OPEN4CAR:
				haveCars.push_back(3);
				break;
			case GAMEEVENT::OPEN5CAR:
				haveCars.push_back(4);
				break;
			case GAMEEVENT::OPEN6CAR:
				haveCars.push_back(5);
				break;
			case GAMEEVENT::OPEN7CAR:
				haveCars.push_back(6);
				break;
			case GAMEEVENT::OPEN3BG:
				acceptBgs.push_back(2);
				break;
			case GAMEEVENT::OPEN8CAR:
				haveCars.push_back(7);
				break;
			case GAMEEVENT::GET2X:
				scoreBoost = getValue(2);
				break;
			case GAMEEVENT::OPEN9CAR:
				haveCars.push_back(8);
				break;
			case GAMEEVENT::OPEN10CAR:
				haveCars.push_back(9);
				break;
			case GAMEEVENT::OPEN11CAR:
				haveCars.push_back(10);
				break;
			case GAMEEVENT::OPEN12CAR:
				haveCars.push_back(11);
				break;
			case GAMEEVENT::OPEN13CAR:
				haveCars.push_back(12);
				break;
			case GAMEEVENT::OPEN4BG:
				acceptBgs.push_back(3);
				break;
			case GAMEEVENT::OPEN14CAR:
				haveCars.push_back(13);
				break;
			case GAMEEVENT::OPEN15CAR:
				haveCars.push_back(14);
				break;
			case GAMEEVENT::GET3X:
				scoreBoost = getValue(3);
				break;
			case GAMEEVENT::OPEN16CAR:
				haveCars.push_back(15);
				break;
			case GAMEEVENT::OPEN17CAR:
				haveCars.push_back(16);
				break;
			case GAMEEVENT::OPEN18CAR:
				haveCars.push_back(17);
				break;
			case GAMEEVENT::OPEN19CAR:
				haveCars.push_back(18);
				break;
			case GAMEEVENT::OPEN5BG:
				acceptBgs.push_back(4);
				break;
			case GAMEEVENT::OPEN20CAR:
				haveCars.push_back(19);
				break;
			}
		}
	}
}
void java_INAPP(int inappkind)
{
}
void java_FIREBASE(int eventNo, int score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"setFirebase",
			"(II)V")) {
			t.env->CallVoidMethod(act, t.methodID, eventNo, score);
			t.env->DeleteLocalRef(t.classID);
		}
	}
#else
#endif
}
void java_AD()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"viewInterstitialAd",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}

	}
#else
#endif
}
void java_MYSCORE()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {
		//log("java_MYSCORE %d %s", getValue(myHighscore), myName.c_str());
		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"setLeaderboard",
			"(ILjava/lang/String;)V")) {
			t.env->CallVoidMethod(act, t.methodID, getValue(myHighscore), t.env->NewStringUTF(myName.c_str()));
			t.env->DeleteLocalRef(t.classID);
			//log("java_MYSCORE_");
		}
		//else {
		//	log("java_MYSCORE fail");
		//}

	}
#else
#endif
}
void java_RANKING()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"getLeaderboard",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}

	}
#else
#endif
}
void java_CALLBOARD()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"callLeaderboard",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}

	}
#else
#endif
}
void java_SIGNIN()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"signIn",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}

	}
#else
	signinmode = 1;
#endif
}
void java_SIGNOUT()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",
		"getInstance",
		"()Ljava/lang/Object;");

	jobject act = t.env->CallStaticObjectMethod(t.classID, t.methodID);

	if (isHave) {

		if (JniHelper::getMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",
			"signOut",
			"()V")) {
			t.env->CallVoidMethod(act, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}

	}
#else
	signinmode = 1;
#endif
}
