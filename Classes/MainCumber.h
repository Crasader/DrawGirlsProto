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
	
	virtual ~MainCumber()
	{
		
	}
	
	virtual void cumberImgStartRotating(float speed);
	virtual void checkingJackCrash();
	virtual void startDamageReaction(float userdata);
	virtual void onStartGame(){}
	virtual void onEndGame(){}
	virtual void onPatternEnd(){}
	virtual void startMoving();
	virtual void stopMoving();
	virtual void setCumberState(int t_cs);
	virtual cumberState getCumberState();
	virtual void setGameover();
	virtual void furyModeOn();
	virtual void furyModeOff();
	virtual void tickingOn();
	virtual void tickingOff();
	virtual void startTeleport();
	virtual void smaller();
	virtual void changePosition();
	virtual void lightSmaller();
	virtual void endTeleport();
	virtual void createSheild();
	virtual void crashSheild();
	virtual void startInvisible();
	virtual void stopInvisible();
	virtual void silenceItem(bool t_b);
	virtual void setMovingShoot(bool t_b);
	virtual void setCasting(bool t_b);
	virtual void showEmotion(EmotionType t_type);
	virtual void nullmEmotion();
	virtual CCNode* getBossEye();
	virtual void caughtBoss(CCObject* t_setCaught, SEL_CallFunc d_setCaught);
	virtual void endCaughtBoss();
	virtual void changeMaxSize(float t_p);
	virtual CCPoint getMissilePoint()
	{
		return getPosition();
	}
//private:
protected:
	CCObject* target_setCaught;
	SEL_CallFunc delegate_setCaught;
	
	
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
	virtual void moving();
	
	void invisibling();
	
	virtual void startSpringCumber(float t_springAngle);
	virtual void stopSpringCumber();
	
	virtual void springCumber();
	
	virtual void crashMapForIntPoint(IntPoint t_p);
	virtual void myInit();
	virtual void randomShuffle();
	virtual bool checkingFunc(IntPoint check_position, bool& is_not_position, bool& notEmotion, int& before_areacrash_cnt, bool& is_map_visited);
};


#endif /* defined(__DrawingJack__MainCumber__) */
