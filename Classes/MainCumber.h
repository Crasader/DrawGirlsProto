//
//  MainCumber.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 20..
//
//

#ifndef __DrawingJack__MainCumber__
#define __DrawingJack__MainCumber__

#include "cocos2d.h"
#include "CumberBase.h"
#include "AudioEngine.h"
#include "AlertEngine.h"
#include "CumberEmotion.h"
#include "BossEye.h"
#include <algorithm>
#include <vector>
#include <map>
#include <queue>

using namespace cocos2d;
using namespace std;


class MainCumber : public Cumber
{
public:
	static MainCumber* create()
	{
		MainCumber* t_MC = new MainCumber();
		t_MC->myInit();
		t_MC->autorelease();
		return t_MC;
	}
	
	void cumberImgStartRotating(float speed);
	void checkingJackCrash();
	void startSpringCumber(float t_springAngle);
	void stopSpringCumber();
	void startMoving();
	void stopMoving();
	void setCumberState(int t_cs);
	cumberState getCumberState();
	void setGameover();
	void furyModeOn();
	void furyModeOff();
	void tickingOn();
	void tickingOff();
	void startTeleport();
	void smaller();
	void changePosition();
	void lightSmaller();
	void endTeleport();
	void createSheild();
	void crashSheild();
	void startInvisible();
	void stopInvisible();
	void silenceItem(bool t_b);
	void setMovingShoot(bool t_b);
	void setCasting(bool t_b);
	void showEmotion(EmotionType t_type);
	void nullmEmotion();
	CCNode* getBossEye();
	void caughtBoss(CCObject* t_setCaught, SEL_CallFunc d_setCaught);
	void endCaughtBoss();
	void changeMaxSize(float t_p);
	
private:
	CCObject* target_setCaught;
	SEL_CallFunc delegate_setCaught;
	
	GameData* myGD;
	float springAngle;
	float springPower;
	int springCnt;
	bool isGameover;
	bool isFuryMode;
	int furyFrame;
	CCParticleSystemQuad* furyMode;
	bool ableTickingTimeBomb;
	int tickingFrame;
	CCSprite* tickingImg;
	CCSprite* teleportImg;
	SilenceEffect* silenceImg;
	int invisibleFrame;
	int add_value;
	int zero_cnt;
	bool is_silenced;
	bool is_casting;
	bool is_moving_shoot;
	CCPoint b_c_p;
	BossEye* my_eye;
	
	//	int t_chapter_number;
	//	int t_stage_number;
	
	bool is_attackOn;
	
	int attack_rate;
	int noattack_cnt;
	
	int before_keep_frame_cnt;
	map<int,IntPoint> before_points;
	int keep_number;
	
	int map_visit_cnt;
	int max_visit_cnt;
	
	Emotion* mEmotion;
	
	float dash_angle;
	int checking_array[360];
	
	void alertAction(int t1, int t2);
	void moving();
	
	void invisibling();
	
	void springCumber();
	
	void crashMapForIntPoint(IntPoint t_p);
	void myInit();
	void randomShuffle();
	bool checkingFunc(IntPoint check_position, bool& is_not_position, bool& notEmotion, int& before_areacrash_cnt, bool& is_map_visited);
};


#endif /* defined(__DrawingJack__MainCumber__) */
