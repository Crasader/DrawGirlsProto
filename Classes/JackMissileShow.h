//
//  JackMissileShow.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 23..
//
//

#ifndef __DGproto__JackMissileShow__
#define __DGproto__JackMissileShow__

#include "cocos2d.h"
#include "MissileDamageData.h"
#include "DataStorageHub.h"
#include "ServerDataSave.h"
#include "JackMissile.h"
#include <random>

#define LZZ_INLINE inline

class JackMissileShow : public CCNode
{
public:
	virtual void startMoving ();
protected:
	CCParticleSystem * particle;
	CCPoint particlePosition;
	CCNode * targetNode;
	float damage;
	float dex;
	float boss_agi;
	CCObject* target_popup;
	SEL_CallFuncCCpI target_explosion;
	SEL_CallFuncCCpI target_showDamageMissile;
	SEL_CallFuncCCpI target_showMissMissile;
	MyElemental my_type;
};
class JM_UpgradeMissileShow : public JackMissileShow
{
public:
	static JM_UpgradeMissileShow * create (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	static JM_UpgradeMissileShow * create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	virtual ~ JM_UpgradeMissileShow ();
	virtual void startMoving ();
private:
	CCSprite * loadImg;
	CCSprite * shootImg;
	CCSprite * mainImg;
	CCSprite * edge;
	bool load_removing;
	bool shoot_removing;
	CCPoint load_dv;
	CCPoint shoot_dv;
	float shoot_dv_distance;
	int ingFrame;
	bool is_spin;
	string type_name;
	void stopMoving ();
	void moving ();
	void myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	void myInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	void realInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
};
class JM_BasicMissileShow : public JackMissileShow
{
public:
	static JM_BasicMissileShow * create (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	static JM_BasicMissileShow * create (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	virtual ~ JM_BasicMissileShow ();
	virtual void startMoving ();
private:
	float directionAngle;
	float myJM_SPEED;
	float myJM_CHANGE_DIRECTION_VAL;
	CCSprite * mainImg;
	CCMotionStreak* streak_img;
	bool is_spin;
	int ing_miss_counting;
	void moving ();
	void myInit (CCNode * t_target, int jm_type, float missile_speed, CCPoint s_p, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	void setStartPosition (CCPoint s_p);
	void myInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	void realInit (CCNode * t_target, int jm_type, float missile_speed, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
};

class UM_creatorShow : public CCNode
{
public:
	static UM_creatorShow* create(int t_um_tcnt, int t_create_type, float t_missile_speed, CCNode* t_target, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	static UM_creatorShow* create(int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p, CCNode* t_target, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	
	void startCreate();
	
private:
	
	int ing_frame;
	int um_tcnt;
	int ing_um_cnt;
	int create_type;
	float missile_speed;
	int shoot_frame;
	float boss_agi;
	CCNode* target_node;
	CCObject* target_popup;
	SEL_CallFuncCCpI target_explosion;
	SEL_CallFuncCCpI target_showDamageMissile;
	SEL_CallFuncCCpI target_showMissMissile;
	
	CCPoint start_position;
	
	void creating();
	void stopCreate();
	
	void myInit(int t_um_tcnt, int t_create_type, float t_missile_speed, CCNode* t_target, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
	void myInit(int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p, CCNode* t_target, float t_boss_agi, CCObject* t_popup, SEL_CallFuncCCpI t_explosion, SEL_CallFuncCCpI t_showDamageMissile, SEL_CallFuncCCpI t_showMissMissile);
};

#endif /* defined(__DGproto__JackMissileShow__) */
