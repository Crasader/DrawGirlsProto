//
//  Jack.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#ifndef DrawingJack_Jack_h
#define DrawingJack_Jack_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "AudioEngine.h"
#include "OtherEffect.h"
#include "StarGoldData.h"
#include "Well512.h"
#include "LogData.h"
#include "cocos-ext.h"

using namespace cocos2d;
USING_NS_CC_EXT;
using namespace std;
using namespace placeholders;
enum jackSpeed{
	jackSpeedSlow	= 1,
	jackSpeedNormal	= 2,
	jackSpeedFast	= 4
};

enum jackState{
	jackStateNormal = 0,
	jackStateDrawing,
	jackStateBackTracking
};

enum KeepDirection{
	kKeepDirection_empty = 0,
	kKeepDirection_left,
	kKeepDirection_right
};

enum jackZorder{
	kJackZ_line = 1,
	kJackZ_main,
	kJackZ_defaultBarrier,
	kJackZ_ActiveBarrier,
	kJackZ_stunEffect
};

class StopEffects : public CCNode
{
public:
	virtual void selfRemove() = 0;
	
};

class StunHammer : public StopEffects
{
public:
	static StunHammer* create(CCObject* t_jack, SEL_CallFunc d_stun);
	
	void startAction();
	
	void showHammer();
	
	void selfRemove();
	
private:
	CCSprite* hammerImg;
	CCObject* target_jack;
	SEL_CallFunc delegate_stun;
	CCSprite* starImg;
	
	void deleteTempHammer(CCObject* t_hammer);
	
	void afterAction();
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_stun);
};

class IceFog : public StopEffects
{
public:
	static IceFog* create(CCObject* t_jack, SEL_CallFunc d_freeze);
	
	void startAction();
	
	void showFog();
	
	void selfRemove();
	
private:
	CCObject* target_jack;
	SEL_CallFunc delegate_freeze;
	
	CCSprite* fogImg;
	CCBAnimationManager* fog_manager;
//	CCSprite* iceImg;
	
	void deleteFog();
	
	void afterAction();
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_freeze);
};

class Sleep : public StopEffects
{
public:
	
	static Sleep* create(CCObject* t_jack, SEL_CallFunc d_sleep);
	
	void startAction();
	
	void showCircle();
	
	void selfRemove();
	
private:
	CCObject* target_jack;
	SEL_CallFunc delegate_sleep;
	CircleCreater* my_cc;
	CCSprite* sleepImg;
	
	void deleteCircle(CCObject* t_remove);
	
	void afterAction();
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_sleep);
};

class Chaos : public CCNode
{
public:
	static Chaos* create(CCObject* t_jack, SEL_CallFunc d_chaos);
	
	void startAction();
	
	void showCircle();
	
	void selfRemove();
	
private:
	CCObject* target_jack;
	SEL_CallFunc delegate_chaos;
	CircleCreater* my_cc;
	CCSprite* chaosImg;
	
	void deleteCircle(CCObject* t_remove);
	
	void afterAction();
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_chaos);
};

class MissileBarrier : public CCSprite
{
public:
	static MissileBarrier* create();
	
private:
	
	void selfRemove();
	
	void myInit();
};

class Jack : public CCNode, public CCBAnimationManagerDelegate
{
public:
	static Jack* create();
	
	virtual void setPosition(CCPoint t_sp);
	
	void changeDirection(IntDirection t_d, IntDirection t_sd);
	
	void backTrackingAtAfterMoving(IntPoint t_p);
	
	void endBackTracking();
	
	void changeSpeed(float t_s);
	
	void createHammer();
	
	void createFog();
	
	const CCPoint& getPosition();
	void createSleep();
	
	void createChaos();
	
	void reverseOn();
	
	void reverseOff();
	
	void stunJack();
	
	IntDirection getDirection();
	IntDirection getSecondDirection();
	jackState getJackState();
	
	bool willBackTracking;
	bool isMoving;
	bool isStun;
	
	bool isDrawingOn;
	
	StopEffects* t_se;
	
	void stopMove();
	
	void stopJack();
	
	void startDieEffect(int die_type); // after coding;
	
	void showMB();
	
	IntDirection getRecentDirection();
	
	void setTouchPointByJoystick(CCPoint t_p, IntDirection t_direction, bool is_touchEnd);
	
	void takeSpeedUpItem();
	
	float getSpeedUpValue();
	
	float getAlphaSpeed();
	
	void setAlphaSpeed(float t_s);
	
	void initStartPosition(CCPoint t_p);
	
	void setJackState(jackState t_s);
	
	virtual void completedAnimationSequenceNamed(const char *name);
	
	bool isDie;
	
	CCNode* getJack();
	
	int getContinueOnCount();
	
private:
	
	KSProtectVar<int> continue_on_count;
	
	CCSprite* touchPointSpr_byJoystick;
	CCSprite* directionSpr_byJoystick;
	CCSprite* joystickSpr_byJoystick;
	
	bool isReverse;
	
	KeepDirection keep_direction; // rotary

	IntPoint btPoint;
	IntPoint move_before_point;
		   
	jackState myState;
	jackState afterState;
	jackSpeed mySpeed;
	jackSpeed afterSpeed;
	IntDirection direction;
	IntDirection afterDirection;
	IntDirection secondDirection;
	
	IntDirection no_draw_direction;
	IntDirection no_draw_secondDirection;
	
	IntDirection before_x_direction;
	int before_x_cnt;
	
	float test_speed;
	float after_speed;
	bool is_end_turn;
	
	bool is_double_moving;
	
	bool is_hard; // is_noShield
    
    bool isEnableIrregularDirection;
	
	int check_turn_cnt;
	
	Chaos* t_chaos;
	
	CCSprite* line_edge;
	
	CCSprite* jackImg;
	CCBAnimationManager* jack_ccb_manager;
	CCSprite* die_particle;
	CCBAnimationManager* die_manager;
//	CCSprite* jack_drawing;
//	CCSprite* jack_barrier;
	IntDirection jack_img_direction;
	SpeedChangeEffect* speed_change_img;
	
	int moveValue;
	
//	bool isReverseGesture;
	int reverseTurnCnt;
	
	IntPoint afterPoint;
	
	int dieEffectCnt;
	
	float speed_up_value;
	float alpha_speed_value;
	
	IntDirection reverseDirection(IntDirection t_d);
	
	void dieEffect();
	
	void endReviveJack();
	
	void continueGame();
	
	void endGame();
	
	void searchAndMoveOldline(IntMoveState searchFirstMoveState);
	
	void escapeJack();
	
	void dieEscapeJack();
	
	void startMove();
	
	int move_loop_cnt;
	
	void moveTest();
	
	void resetStopEffects();
	
	void positionRefresh();
	
	bool isDieJack();
	
	void myInit();
	
	void setStartPosition();
	
	void keepDirectionAction(IntPoint jp, IntDirection t_d);
	
	bool rotarySelection(IntPoint jp, IntDirection t_d);
	
	CCPoint checkOutlineTurnPosition(CCPoint turnPosition);
	
	void startReviveAnimation(CCSprite* t_jack_img);
	
	void startShowJackAnimation(CCSprite* t_jack_img);
	
	void startInnerParticle(CCNode* target_node);
	void startLightSprite(CCNode* target_node);
	void startOutterParticle(CCNode* target_node);
	void patternRemover();
};

#endif
