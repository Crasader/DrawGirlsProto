//
//  MissileParent.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 4..
//
//

#ifndef DrawingJack_MissileParent_h
#define DrawingJack_MissileParent_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "AttackPattern.h"
#include "JackMissile.h"
#include "RandomSelector.h"
#include "SelectedMapData.h"
#include "AudioEngine.h"
#include "AutoAttacker.h"
#include "ShockWave.h"
#include "KSCumberBase.h"

using namespace cocos2d;
using namespace std;

class ChargeParent : public CCNode
{
public:
	virtual void cancelCharge()
	{
		
	}
};

class ChargeNode : public ChargeParent
{
public:
	static ChargeNode* create(CCPoint t_position, int t_frame, CCObject* t_ing_t, SEL_CallFunc t_ing_d, CCObject* t_a_t, SEL_CallFunc t_a_d, CCObject* t_c_t, SEL_CallFunc t_c_d, CCObject* t_rt)
	{
		ChargeNode* n_charge = new ChargeNode();
		n_charge->myInit(t_position, t_frame, t_ing_t, t_ing_d, t_a_t, t_a_d, t_c_t, t_c_d, t_rt);
		n_charge->autorelease();
		return n_charge;
	}
	
	void setChargeColor(ccColor4F change_color)
	{
		particle->setStartColor(change_color);
		particle->setEndColor(change_color);
		
//		charge_img->setColor(change_color);
	}
	
	void startCharge()
	{
		myGD->communication("Main_showWarning", 1);
		charge_cnt = 0;
		AudioEngine::sharedInstance()->playEffect("sound_casting_attack.mp3", true);
		AudioEngine::sharedInstance()->playEffect("sound_attackpattern_base.mp3", false);
		schedule(schedule_selector(ChargeNode::charging));
	}

	void cancelCharge()
	{
		AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
		if(cancel_target && cancel_delegate)
			(cancel_target->*cancel_delegate)();
		removeSelf();
	}
	
	CCObject* getRealTarget()
	{
		return real_target;
	}
	
private:
	
	
	CCPoint create_position;
	int charge_frame;
	CCObject* real_target;
	CCObject* charging_target;
	CCObject* after_target;
	CCObject* cancel_target;
	SEL_CallFunc charging_delegate;
	SEL_CallFunc after_delegate;
	SEL_CallFunc cancel_delegate;
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging()
	{
		charge_cnt++;
		
		particle->setStartRadius((charge_frame/3.0)*(charge_frame-charge_cnt)/charge_frame);
		if(charging_target && charging_delegate)
			(charging_target->*charging_delegate)();
		
		if(charge_cnt >= charge_frame)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
			if(after_target && after_delegate)
				(after_target->*after_delegate)();
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(ChargeNode::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame, CCObject* t_ing_t, SEL_CallFunc t_ing_d, CCObject* t_a_t, SEL_CallFunc t_a_d, CCObject* t_c_t, SEL_CallFunc t_c_d, CCObject* t_rt)
	{
		
		
		real_target = t_rt;
		create_position = t_position;
		charge_frame = t_frame;
		charging_target = t_ing_t;
		charging_delegate = t_ing_d;
		after_target = t_a_t;
		after_delegate = t_a_d;
		cancel_target = t_c_t;
		cancel_delegate = t_c_d;
		
		float chargeRate = t_frame/60.f;
		
		particle = CCParticleSystemQuad::createWithTotalParticles(40 + chargeRate*5);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("charge_particle.png");
		particle->setTexture(texture);
		particle->setEmissionRate(40.00 + chargeRate*5); // inf
		particle->setAngle(90.0);
		particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(-1.00);
		particle->setEmitterMode(kCCParticleModeRadius);
		ccColor4F startColor = {1.00,1.00,1.00,1.00};
		particle->setStartColor(startColor);
		ccColor4F startColorVar = {0.30,0.30,0.30,0.30};
		particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {0.00,0.00,0.00,1.00};
		particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		particle->setEndColorVar(endColorVar);
		particle->setStartSize(5.00 + chargeRate);
		particle->setStartSizeVar(2.0);
		particle->setEndSize(chargeRate);
		particle->setEndSizeVar(1.0);
		particle->setRotatePerSecond(20.00);
		particle->setRotatePerSecondVar(0.00);
		particle->setStartRadius(charge_frame/3.0);
		particle->setStartRadiusVar(3.00);
		particle->setEndRadius(0.00);
		particle->setTotalParticles(50);
		particle->setLife(1.00);
		particle->setLifeVar(0.25);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(50.0);
		particle->setEndSpin(0.0);
		particle->setEndSpinVar(0.0);
		particle->setPosVar(ccp(0,0));
		particle->setPosition(create_position);
		addChild(particle);
	}
};

class SpecialChargeNode : public ChargeParent
{
public:
	static SpecialChargeNode* create(CCPoint t_position, int t_frame, CCObject* t_ing_t, SEL_CallFunc t_ing_d, CCObject* t_a_t, SEL_CallFunc t_a_d, CCObject* t_c_t, SEL_CallFunc t_c_d, CCObject* t_rt)
	{
		SpecialChargeNode* n_charge = new SpecialChargeNode();
		n_charge->myInit(t_position, t_frame, t_ing_t, t_ing_d, t_a_t, t_a_d, t_c_t, t_c_d, t_rt);
		n_charge->autorelease();
		return n_charge;
	}
	
	void setChargeColor(ccColor4F change_color)
	{
		particle->setStartColor(change_color);
		particle->setEndColor(change_color);
		
		//		charge_img->setColor(change_color);
	}
	
	void startCharge()
	{
		charge_cnt = 0;
		AudioEngine::sharedInstance()->playEffect("sound_casting_option.mp3", true);
		schedule(schedule_selector(SpecialChargeNode::charging));
	}
	
	void cancelCharge()
	{
		AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
		if(cancel_target && cancel_delegate)
			(cancel_target->*cancel_delegate)();
		removeSelf();
	}
	
	CCObject* getRealTarget()
	{
		return real_target;
	}
	
private:
	
	
	CCPoint create_position;
	int charge_frame;
	CCObject* real_target;
	CCObject* charging_target;
	CCObject* after_target;
	CCObject* cancel_target;
	SEL_CallFunc charging_delegate;
	SEL_CallFunc after_delegate;
	SEL_CallFunc cancel_delegate;
	float ing_rps;
	float chargeRate;
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging()
	{
		charge_cnt++;
		
		particle->setRotatePerSecond(particle->getRotatePerSecond() + chargeRate);
		if(charging_target && charging_delegate)
			(charging_target->*charging_delegate)();
		
		if(charge_cnt >= charge_frame)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
			if(after_target && after_delegate)
				(after_target->*after_delegate)();
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(SpecialChargeNode::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame, CCObject* t_ing_t, SEL_CallFunc t_ing_d, CCObject* t_a_t, SEL_CallFunc t_a_d, CCObject* t_c_t, SEL_CallFunc t_c_d, CCObject* t_rt)
	{
		
		
		real_target = t_rt;
		create_position = t_position;
		charge_frame = t_frame;
		charging_target = t_ing_t;
		charging_delegate = t_ing_d;
		after_target = t_a_t;
		after_delegate = t_a_d;
		cancel_target = t_c_t;
		cancel_delegate = t_c_d;
		
		ing_rps = 0;
		int second = t_frame/60;
		
		chargeRate = 21600.f/powf(t_frame, 2.f)*(3.f+second); // 21600 = 360(angle)*60(frameRate),   360/(t_frame/60)/t_frame
		
		particle = CCParticleSystemQuad::createWithTotalParticles(50);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("charge_particle.png");
		particle->setTexture(texture);
		particle->setEmissionRate(50.00); // inf
		particle->setAngle(90.0);
		particle->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(-1.00);
		particle->setEmitterMode(kCCParticleModeRadius);
		ccColor4F startColor = {1.00,1.00,1.00,1.00};
		particle->setStartColor(startColor);
		ccColor4F startColorVar = {0.30,0.30,0.30,0.30};
		particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {0.00,0.00,0.00,1.00};
		particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		particle->setEndColorVar(endColorVar);
		particle->setStartSize(5+second);
		particle->setStartSizeVar(3+second);
		particle->setEndSize(5+second);
		particle->setEndSizeVar(3+second);
		particle->setRotatePerSecond(ing_rps);
		particle->setRotatePerSecondVar(0.00);
		particle->setStartRadius(12+3*second);
		particle->setStartRadiusVar(0.00);
		particle->setEndRadius(12+3*second);
		particle->setTotalParticles(50);
		particle->setLife(1.00);
		particle->setLifeVar(0.0);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(45.0);
		particle->setEndSpin(0.0);
		particle->setEndSpinVar(90.0);
		particle->setPosVar(ccp(0,0));
		particle->setPosition(create_position);
		addChild(particle);
	}
};

class CCN_InnerNode : public CCSprite
{
public:
	static CCN_InnerNode* create(CCPoint t_fp, float t_distance, int t_moveFrame, ccColor4F t_color)
	{
		CCN_InnerNode* t_in = new CCN_InnerNode();
		t_in->myInit(t_fp, t_distance, t_moveFrame, t_color);
		t_in->autorelease();
		return t_in;
	}
	
private:
	CCPoint dv;
	int moveFrame;
	int ingFrame;
	float ds;
	
	
	void startMove()
	{
		ingFrame = 0;
		schedule(schedule_selector(CCN_InnerNode::move));
	}
	void stopMove()
	{
		unschedule(schedule_selector(CCN_InnerNode::move));
		removeFromParentAndCleanup(true);
	}
	void move()
	{
		ingFrame++;
		
		setPosition(ccpSub(getPosition(), dv));
		setOpacity(getOpacity() - 10);
		setScaleX(getScaleX() - ds);
		
		if(ingFrame >= moveFrame)
		{
			stopMove();
		}
	}
	
	void myInit(CCPoint t_fp, float t_distance, int t_moveFrame, ccColor4F t_color)
	{
		moveFrame = t_moveFrame;
		
		t_distance += (rand()%7 - 3);
		
		ds = 0.06;
		
		initWithFile("crash_charge.png");
		
		ccBlendFunc t_b;
		t_b.src = GL_SRC_ALPHA;
		t_b.dst = GL_ONE;
		
		setBlendFunc(t_b);
		
		setOpacity(rand()%56 + 200);
		ccColor3B myColor = ccc3(t_color.r*255, t_color.g*255, t_color.b*255);
		
		myColor.r = myColor.r + (rand()%31 - 15);
		myColor.g = myColor.g + (rand()%31 - 15);
		myColor.b = myColor.b + (rand()%31 - 15);
		
		if(myColor.r > 255)	myColor.r = 255;
		if(myColor.r < 0) myColor.r = 0;
		if(myColor.g > 255)	myColor.g = 255;
		if(myColor.g < 0) myColor.g = 0;
		if(myColor.b > 255)	myColor.b = 255;
		if(myColor.b < 0) myColor.b = 0;
		
		setColor(myColor);
		
		float randomScaleY = (rand()%7)/10.f+0.7;
		setScaleY(randomScaleY);
		
		setScaleX(t_distance/40*3);
		
		int random_angle = rand()%360;
		setRotation(-random_angle);
		
		CCPoint sp;
		sp.x = 1;
		sp.y = tanf(random_angle/180.f*M_PI);
		
		float div_value = sqrtf(powf(sp.x, 2.f) + powf(sp.y, 2.f));
		
		sp = ccpMult(sp, 1.f/div_value);
		sp = ccpMult(sp, t_distance);
		
		if(random_angle > 90 && random_angle < 270)
			sp = ccpMult(sp, -1.f);
		
		sp = ccpAdd(sp, t_fp);
		
		setPosition(sp);
		
		dv = ccpSub(sp, t_fp);
		dv = ccpMult(dv, 1.f/moveFrame);
		
		startMove();
	}
};

class CrashChargeNode : public ChargeParent
{
public:
	static CrashChargeNode* create(CCPoint t_position, int t_frame, CCObject* t_ing_t, SEL_CallFunc t_ing_d, CCObject* t_a_t, SEL_CallFunc t_a_d, CCObject* t_c_t, SEL_CallFunc t_c_d, CCObject* t_rt)
	{
		CrashChargeNode* n_charge = new CrashChargeNode();
		n_charge->myInit(t_position, t_frame, t_ing_t, t_ing_d, t_a_t, t_a_d, t_c_t, t_c_d, t_rt);
		n_charge->autorelease();
		return n_charge;
	}
	
	void setChargeColor(ccColor4F change_color)
	{
		myColor = change_color;
	}
	
	void startCharge()
	{
		AudioEngine::sharedInstance()->playEffect("sound_attackpattern_crash.mp3", false);
		myGD->communication("Main_showWarning", 2);
		charge_cnt = 0;
		AudioEngine::sharedInstance()->playEffect("sound_casting_crash.mp3", true);
		schedule(schedule_selector(CrashChargeNode::charging));
	}
	
	void cancelCharge()
	{
		myGD->communication("CP_setCasting", false);
		AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
		if(cancel_target && cancel_delegate)
			(cancel_target->*cancel_delegate)();
		removeSelf();
	}
	
	CCObject* getRealTarget()
	{
		return real_target;
	}
	
private:
	
	
	CCPoint create_position;
	int charge_frame;
	CCObject* real_target;
	CCObject* charging_target;
	CCObject* after_target;
	CCObject* cancel_target;
	SEL_CallFunc charging_delegate;
	SEL_CallFunc after_delegate;
	SEL_CallFunc cancel_delegate;
	
	int charge_cnt;
	
	ccColor4F myColor;
	
	void charging()
	{
		charge_cnt++;
		
		IntPoint mainCumberPoint = myGD->getMainCumberPoint();
		CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
		
		setPosition(ccpSub(mainCumberPosition, create_position));
		
		for(int i=0;i<2;i++)
		{
			CCN_InnerNode* t_in = CCN_InnerNode::create(create_position, 40*((0.f + charge_frame - charge_cnt)/charge_frame), 10, myColor);
			addChild(t_in);
		}
		
		if(charging_target && charging_delegate)
			(charging_target->*charging_delegate)();
		
		if(charge_cnt >= charge_frame)
		{
			myGD->communication("CP_setCasting", false);
			AudioEngine::sharedInstance()->stopAllEffects();
			AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
			if(after_target && after_delegate)
				(after_target->*after_delegate)();
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(CrashChargeNode::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame, CCObject* t_ing_t, SEL_CallFunc t_ing_d, CCObject* t_a_t, SEL_CallFunc t_a_d, CCObject* t_c_t, SEL_CallFunc t_c_d, CCObject* t_rt)
	{
		
		
		real_target = t_rt;
		create_position = t_position;
		charge_frame = t_frame;
		charging_target = t_ing_t;
		charging_delegate = t_ing_d;
		after_target = t_a_t;
		after_delegate = t_a_d;
		cancel_target = t_c_t;
		cancel_delegate = t_c_d;
		
		myColor = ccc4f(1.0, 1.0, 1.0, 1.0);
	}
};

class CreateSubCumberOtherAction : public CCNode
{
public:
	static CreateSubCumberOtherAction* create(IntPoint c_p, CCObject* t_after, SEL_CallFunc d_after, CCObject* t_cancel, SEL_CallFunc d_cancel)
	{
		CreateSubCumberOtherAction* t_CSCAA = new CreateSubCumberOtherAction();
		t_CSCAA->myInit(c_p, t_after, d_after, t_cancel, d_cancel);
		t_CSCAA->autorelease();
		return t_CSCAA;
	}
	
	void afterAction()
	{
		myGD->communication("CP_createSubCumber", createPoint);
		(after_target->*after_delegate)();
		removeFromParentAndCleanup(true);
	}
	
	void cancelAction()
	{
		(cancel_target->*cancel_delegate)();
		removeFromParentAndCleanup(true);
	}
	
private:
	
	CCObject* after_target;
	SEL_CallFunc after_delegate;
	CCObject* cancel_target;
	SEL_CallFunc cancel_delegate;
	IntPoint createPoint;
	
	void myInit(IntPoint c_p, CCObject* t_after, SEL_CallFunc d_after, CCObject* t_cancel, SEL_CallFunc d_cancel)
	{
		
		createPoint = c_p;
		after_target = t_after;
		after_delegate = d_after;
		cancel_target = t_cancel;
		cancel_delegate = d_cancel;
	}
};

class UM_creator : public CCNode
{
public:
	static UM_creator* create(int t_um_tcnt, int t_create_type, float t_damage_per)
	{
		UM_creator* t_c = new UM_creator();
		t_c->myInit(t_um_tcnt, t_create_type, t_damage_per);
		t_c->autorelease();
		return t_c;
	}
	
	static UM_creator* create(int t_um_tcnt, int t_create_type, float t_damage_per, CCPoint s_p)
	{
		UM_creator* t_c = new UM_creator();
		t_c->myInit(t_um_tcnt, t_create_type, t_damage_per, s_p);
		t_c->autorelease();
		return t_c;
	}
	
	void startCreate()
	{
		ing_frame = 0;
		ing_um_cnt = 0;
		schedule(schedule_selector(UM_creator::creating));
	}
	
	void startPetCreate()
	{
		ing_frame = 0;
		ing_um_cnt = 0;
		schedule(schedule_selector(UM_creator::petCreating));
	}
	
private:
	
	int ing_frame;
	int um_tcnt;
	int ing_um_cnt;
	int create_type;
	float damage_per;
	
	CCPoint start_position;
	
	void creating()
	{
		ing_frame++;
		
		if(ing_frame%6 == 0)
		{
			if(ing_frame/6 <= 1)
			{
				JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
				int cumberCnt = 1 + subCumberArray->count();
				int random_value;
				
				random_value = rand()%cumberCnt;
				if(random_value == 0)
				{
					JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per);
					getParent()->addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					JackMissile* t_jm = JM_UpgradeMissile::create((CCNode*)subCumberArray->objectAtIndex(random_value-1), create_type, damage_per);
					getParent()->addChild(t_jm);
					t_jm->startMoving();
				}
			}
			ing_um_cnt++;
		}
		
		if(ing_um_cnt >= um_tcnt)
		{
			stopCreate();
		}
	}
	
	void petCreating()
	{
		ing_frame++;
		
		if(ing_frame%6 == 0)
		{
			if(ing_frame/6 <= 1)
			{
				JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per, start_position);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
				int cumberCnt = 1 + subCumberArray->count();
				int random_value;
				
				random_value = rand()%cumberCnt;
				if(random_value == 0)
				{
					JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per, start_position);
					getParent()->addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					JackMissile* t_jm = JM_UpgradeMissile::create((CCNode*)subCumberArray->objectAtIndex(random_value-1), create_type, damage_per, start_position);
					getParent()->addChild(t_jm);
					t_jm->startMoving();
				}
			}
			ing_um_cnt++;
		}
		
		if(ing_um_cnt >= um_tcnt)
		{
			stopPetCreate();
		}
	}
	
	void stopCreate()
	{
		unschedule(schedule_selector(UM_creator::creating));
		removeFromParentAndCleanup(true);
	}
	
	void stopPetCreate()
	{
		unschedule(schedule_selector(UM_creator::petCreating));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(int t_um_tcnt, int t_create_type, float t_damage_per)
	{
		
		damage_per = t_damage_per;
		um_tcnt = t_um_tcnt;
		create_type = t_create_type;
	}
	
	void myInit(int t_um_tcnt, int t_create_type, float t_damage_per, CCPoint s_p)
	{
		start_position = s_p;
		myInit(t_um_tcnt, t_create_type, t_damage_per);
	}
};

class MissileParent : public CCNode
{
public:
	static MissileParent* create(CCNode* boss_eye)
	{
		MissileParent* t_mp = new MissileParent();
		t_mp->myInit(boss_eye);
		t_mp->autorelease();
		return t_mp;
	}
	
	void bombCumber(CCObject* target);
	
	void createJackMissile(int jm_type, int cmCnt, float damage_per);
	
	void subOneDie();
	
	void endIngActionAP();
	
	void actionAP6();
	void cancelAP6(){		}
	
	void ingAP7();
	void actionAP7();
	void cancelAP7();
	
	void actionAP8();
	void cancelAP8(){		}
	
	void actionAP9();
	void cancelAP9(){		}
	
	void actionAP10();
	void cancelAP10(){		}
	
	void actionAP11();
	void cancelAP11(){		}
	
	void actionAP12();
	void cancelAP12(){		}
	
	
	void ingAP13()
	{
//		myGD->communication("CP_checkingJackCrash");
	}
	void actionAP13();
	void cancelAP13(){		}
	
	void actionAP14();
	void cancelAP14(){		}
	
	void actionAP15();
	void cancelAP15(){		}
	
	void actionAP16();
	void cancelAP16(){		}
	
	void ingAP17()
	{
//		myGD->communication("CP_checkingJackCrash");
	}
	void actionAP17();
	void cancelAP17(){		}
	
	void actionAP18();
	void cancelAP18(){		}
	
	void actionAP19();
	void cancelAP19(){		}
	
	void actionAP20();
	void cancelAP20(){		}
	
	void actionAP21();
	void cancelAP21(){		}
	
	void actionAP22();
	void cancelAP22(){		}
	
	void actionAP23();
	void cancelAP23(){		}
	
	void actionAP24();
	void cancelAP24(){	}
	
	void actionAP25();
	void cancelAP25(){		}
	
	void actionAP26();
	void cancelAP26(){		}
	
	void actionAP27();
	void cancelAP27(){	}
	
	void actionAP28();
	void cancelAP28(){	}
	
	void actionAP29();
	void cancelAP29(){	}
	
	void actionAP30();
	void cancelAP30(){	}
	
	void actionAP31();
	void cancelAP31(){		}
	
	void actionAP32();
	void cancelAP32(){		}
	
	void actionAP33();
	void cancelAP33(){		}
	
	void actionAP34();
	void cancelAP34(){	}
	
	void actionAP101();
	void cancelAP101(){		}
	
	void actionAP102();
	void cancelAP102(){		}
	
	void actionAP103();
	void cancelAP103(){	}
	void attackWithCode(CCPoint startPosition, int pattern_code);
	void startFire(CCPoint startPosition, bool crash_attack = false);
	
	void createSubCumberReplication(CCPoint s_p, CCObject* sender, SEL_CallFunc d_startMoving);
	
	void explosion(CCPoint bombPosition, ccColor4F t_color);
	
	void createTickingTimeBomb(IntPoint t_point, int t_bombFrameOneTime, int t_bombTimes, int t_rangeCode);
	
	void resetTickingTimeBomb();
	
	void deleteKeepAP25(){		keepAP25 = NULL;	}
	void protectedAP25(){		keepAP25->stopFrame();	}
	
	void deleteKeepAP23(){		keepAP23 = NULL;	}
	void deleteKeepAP26(){		keepAP26 = NULL;	}
	void protectedAP26(){		keepAP26->stopFrame();	}
	
	void deleteKeepAP27(){		keepAP27 = NULL;	}
	void protectedAP27(){		keepAP27->stopFrame();	}
	
	void deleteKeepAP33(){		keepAP33 = NULL;	}
	void protectedAP33(){		keepAP33->stopFrame();	}
	
	void deleteKeepAP24(){		keepAP24 = NULL;	}
	void deleteKeepAP34(){		keepAP34 = NULL;	}
	void deleteKeepAP35()
	{
		if(keepAP35)
		{
			keepAP35->stopFraming();
			keepAP35 = NULL;
		}
	}
	
	void stopAutoAttacker();
	
	void startAutoAttacker();
	
	virtual ~MissileParent()
	{
		chargeArray->release();
		tickingArray->release();
		myRS->release();
	}
	
	void shootPetMissile(int jm_type, int cmCnt, float damage_per, CCPoint s_p);
	
private:
	
	AutoAttacker* myAA;
	SW_Parent* mySW;
	
	RandomSelector* myRS;
	
	CCPoint startFirePosition;
	int slowAreaCnt;
	
	bool savedAP;
	AttackPattern* saveAP;
	AP_Missile7* keepAP7;
	AP_Missile25* keepAP25;
	AP_Missile23* keepAP23;
	AP_Missile26* keepAP26;
	AP_Missile27* keepAP27;
	AP_Missile33* keepAP33;
	AP_Missile24* keepAP24;
	AP_Missile34* keepAP34;
	AP_Missile35* keepAP35;
	
	CCArray* chargeArray;
	CCArray* tickingArray;
	
	CCParticleSystem* particle;
	
	void initParticle(CCPoint startPosition, ccColor4F t_color);
	
	void removeChargeInArray(CCObject* remove_charge);
	
	void movingMainCumber();
	
	void setBalance();

	void myInit(CCNode* boss_eye);
};

#endif
