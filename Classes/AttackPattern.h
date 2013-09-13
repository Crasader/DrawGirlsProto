//
//  AttackPattern.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 21..
//
//

#ifndef DrawingJack_AttackPattern_h
#define DrawingJack_AttackPattern_h

#include "cocos2d.h"
#include "MissileUnit.h"
#include "GameData.h"
#include <deque>
#include "AudioEngine.h"

using namespace cocos2d;
using namespace std;

class AttackPattern : public CCNode
{
public:
	
	void startSelfRemoveSchedule()
	{
		schedule(schedule_selector(AttackPattern::selfRemoveSchedule));
	}
	
	virtual void stopMyAction()
	{
		
	}
	
protected:
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			removeFromParentAndCleanup(true);
		}
	}
};

class SelfSpinMissile : public CCNode
{
public:
	static SelfSpinMissile* create(CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize)
	{
		SelfSpinMissile* t_ssm = new SelfSpinMissile();
		t_ssm->myInit(t_sp, t_dv, t_mCnt, t_r, imgFilename, t_mSize);
		t_ssm->autorelease();
		return t_ssm;
	}
	
private:
	CCPoint dv;
	float r;
	
	void startMove()
	{
		schedule(schedule_selector(SelfSpinMissile::move));
	}
	void stopMove()
	{
		unschedule(schedule_selector(SelfSpinMissile::move));
	}
	void move()
	{
		CCPoint r_p = getPosition();
		
		r_p = ccpAdd(r_p, dv);
		
		setPosition(r_p);
		
		if(r_p.x < 0.f - r || r_p.x > 320.f + r || r_p.y < 0.f - r || r_p.y > 430.f + r)
		{
			stopMove();
			removeFromParentAndCleanup(true);
			return;
		}
	}
	
	void myInit(CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize)
	{
		setPosition(t_sp);
		dv = t_dv;
		r = t_r;
		
		int start_angle = rand()%(360/t_mCnt);
		
		int random_value = rand()%7 - 3;
		
		for(int i=0;i<t_mCnt;i++)
		{
			float temp_angle = start_angle+(360.f/t_mCnt)*i;
			
			CCPoint t_position;
			t_position.x = 1;
			t_position.y = tanf(temp_angle/180.f*M_PI);
			
			float div_value = sqrtf(powf(t_position.x, 2.f) + powf(t_position.y, 2.f));
			t_position = ccpMult(t_position, 1.f/div_value);
			
			if(temp_angle >= 90 && temp_angle < 270)		t_position = ccpMult(t_position, -1.f);
			
			t_position = ccpMult(t_position, t_r);
			
			MissileUnit* t_mu = MissileUnit::create(t_position, temp_angle+90.f, 1.f, imgFilename, t_mSize, 6.28f+random_value, 1.f);
			addChild(t_mu);
		}
		
		startMove();
	}
};

class AP_Missile0 : public AttackPattern
{
public:
	static AP_Missile0* create(CCPoint t_sp, float t_angle, float t_distance, string imgFilename, CCSize t_mSize)
	{
		AP_Missile0* t_m0 = new AP_Missile0();
		t_m0->myInit(t_sp, t_angle, t_distance, imgFilename, t_mSize);
		t_m0->autorelease();
		return t_m0;
	}
private:
	void myInit(CCPoint t_sp, float t_angle, float t_distance, string imgFilename, CCSize t_mSize)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
		MissileUnit* t_mu = MissileUnit::create(t_sp, t_angle, t_distance, imgFilename, t_mSize, 0.f, 0.f);
		addChild(t_mu);
		startSelfRemoveSchedule();
	}
};

class AP_Missile1 : public AttackPattern
{
public:
	static AP_Missile1* create(CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize)
	{
		AP_Missile1* t_m1 = new AP_Missile1();
		t_m1->myInit(t_sp, t_dv, t_mCnt, t_r, imgFilename, t_mSize);
		t_m1->autorelease();
		return t_m1;
	}
	
private:
	
	void myInit(CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize)
	{
		AudioEngine::sharedInstance()->playEffect("sound_spin_missile.mp3", false);
		SelfSpinMissile* t_ssm = SelfSpinMissile::create(t_sp, t_dv, t_mCnt, t_r, imgFilename, t_mSize);
		addChild(t_ssm);
		
		startSelfRemoveSchedule();
	}
};

class AP_Missile2 : public AttackPattern
{
public:
	static AP_Missile2* create(CCPoint t_sp, int t_mCnt, float t_distance, string imgFilename, CCSize t_mSize)
	{
		AP_Missile2* t_m2 = new AP_Missile2();
		t_m2->myInit(t_sp, t_mCnt, t_distance, imgFilename, t_mSize);
		t_m2->autorelease();
		return t_m2;
	}
	
private:
	
	void myInit(CCPoint t_sp, int t_mCnt, float t_distance, string imgFilename, CCSize t_mSize)
	{
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
		int start_angle = rand()%(360/t_mCnt);
		
		for(int i=0;i<t_mCnt;i++)
		{
			float temp_angle = start_angle+(360.f/t_mCnt)*i;
			
			MissileUnit* t_mu = MissileUnit::create(t_sp, temp_angle, t_distance, imgFilename, t_mSize, 0.f, 0.f);
			addChild(t_mu);
		}
		
		startSelfRemoveSchedule();
	}
};

class AP_Missile3 : public AttackPattern
{
public:
	static AP_Missile3* create(CCPoint t_sp, int t_keepFrame, int t_shootFrame, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		AP_Missile3* t_m3 = new AP_Missile3();
		t_m3->myInit(t_sp, t_keepFrame, t_shootFrame, t_distance, t_imgFilename, t_mSize);
		t_m3->autorelease();
		return t_m3;
	}
	
	virtual void stopMyAction()
	{
		unschedule(schedule_selector(AP_Missile3::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemoveSchedule();
	}
	
private:
	
	CCPoint sp;
	int keepFrame;
	int shootFrame;
	int ingFrame;
	float distance;
	string imgFilename;
	CCSize mSize;
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile3::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame%shootFrame == 0)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			CCPoint subPosition = ccpSub(jackPosition, sp);
			
			float t_angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
			AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
			MissileUnit* t_mu = MissileUnit::create(sp, t_angle, distance, imgFilename, mSize, 0.f, 0.f);
			addChild(t_mu);
		}
		
		if(ingFrame >= keepFrame)
		{
			stopMyAction();
		}
	}
	
	void myInit(CCPoint t_sp, int t_keepFrame, int t_shootFrame, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		
		sp = t_sp;
		keepFrame = t_keepFrame;
		shootFrame = t_shootFrame;
		distance = t_distance;
		imgFilename = t_imgFilename;
		mSize = t_mSize;
		
		startMyAction();
	}
};

class AP_Missile4 : public AttackPattern
{
public:
	static AP_Missile4* create(CCPoint t_sp, int t_keepFrame, int t_shootFrame, int t_tmCnt, float t_shootAngle, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		AP_Missile4* t_m4 = new AP_Missile4();
		t_m4->myInit(t_sp, t_keepFrame, t_shootFrame, t_tmCnt, t_shootAngle, t_distance, t_imgFilename, t_mSize);
		t_m4->autorelease();
		return t_m4;
	}
	
	virtual void stopMyAction()
	{
		unschedule(schedule_selector(AP_Missile4::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemoveSchedule();
	}
	
private:
	
	CCPoint sp;
	float shootBaseAngle;
	int keepFrame;
	int shootFrame;
	int ingFrame;
	int tmCnt;
	float shootAngle;
	float distance;
	string imgFilename;
	CCSize mSize;
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile4::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame%shootFrame == 0)
		{
			float t_angle = shootBaseAngle - shootAngle/2.f;
			float unit_angle = shootAngle/tmCnt;
			
			for(int i=0;i<tmCnt;i++)
			{
				AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
				MissileUnit* t_mu = MissileUnit::create(sp, t_angle+unit_angle*i, distance, imgFilename, mSize, 0.f, 0.f);
				addChild(t_mu);
			}
		}
		
		if(ingFrame >= keepFrame)
		{
			stopMyAction();
		}
	}
	
	void myInit(CCPoint t_sp, int t_keepFrame, int t_shootFrame, int t_tmCnt, float t_shootAngle, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		
		sp = t_sp;
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		CCPoint subPosition = ccpSub(jackPosition, sp);
		
		shootBaseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		
		keepFrame = t_keepFrame;
		shootFrame = t_shootFrame;
		tmCnt = t_tmCnt;
		shootAngle = t_shootAngle;
		distance = t_distance;
		imgFilename = t_imgFilename;
		mSize = t_mSize;
		
		startMyAction();
	}
};

class AP_Missile5 : public AttackPattern
{
public:
	static AP_Missile5* create(CCPoint t_sp, int t_mCnt, float t_distance, string imgFilename, CCSize t_mSize, float t_da, float t_reduce_da)
	{
		AP_Missile5* t_m5 = new AP_Missile5();
		t_m5->myInit(t_sp, t_mCnt, t_distance, imgFilename, t_mSize, t_da, t_reduce_da);
		t_m5->autorelease();
		return t_m5;
	}
	
private:
	
	void myInit(CCPoint t_sp, int t_mCnt, float t_distance, string imgFilename, CCSize t_mSize, float t_da, float t_reduce_da)
	{
		int start_angle = rand()%(360/t_mCnt);
		
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
		for(int i=0;i<t_mCnt;i++)
		{
			float temp_angle = start_angle+(360.f/t_mCnt)*i;
			
			MissileUnit* t_mu = MissileUnit::create(t_sp, temp_angle, t_distance, imgFilename, t_mSize, t_da, t_reduce_da);
			addChild(t_mu);
		}
		
		startSelfRemoveSchedule();
	}
};

class AP_Missile6 : public AttackPattern
{
public:
	static AP_Missile6* create(CCPoint t_sp, int t_type)
	{
		AP_Missile6* t_m6 = new AP_Missile6();
		t_m6->myInit(t_sp, t_type);
		t_m6->autorelease();
		return t_m6;
	}
	
	virtual void stopMyAction()
	{
		unschedule(schedule_selector(AP_Missile6::myAction));
		if (beamImg)
		{
			beamImg->removeFromParentAndCleanup(true);
		}
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemoveSchedule();
	}
	
private:
	
	int type;
	CCSprite* beamImg;
	float beamBaseAngle;
	int ingFrame;
	CCPoint startPosition;
	
	void removeEffect()
	{
		unschedule(schedule_selector(AP_Missile6::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile6::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		beamImg->runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile6::myAction));
	}
	void myAction()
	{
		ingFrame++;
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		CCPoint subPosition = ccpSub(jackPosition, startPosition);
		
		if(myGD->getJackState())
		{
			float jackAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180;
			float beforeAngle = -beamImg->getRotation();
			float afterAngle = beamBaseAngle-10 + 2*ingFrame;
			
			if(beforeAngle <= afterAngle && jackAngle >= beforeAngle && jackAngle <= afterAngle)
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect");
				unschedule(schedule_selector(AP_Missile6::myAction));
				removeEffect();
			}
			else if(beforeAngle > afterAngle && (jackAngle >= beforeAngle || jackAngle <= afterAngle))
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect");
				unschedule(schedule_selector(AP_Missile6::myAction));
				removeEffect();
			}
		}
		
		beamImg->setRotation(-(beamBaseAngle-10 + 2*ingFrame));
		
		if(ingFrame >= 10)
		{
			beamImg->removeFromParentAndCleanup(true);
			stopMyAction();
		}
	}
	
	void myInit(CCPoint t_sp, int t_type)
	{
		
		type = t_type;
		startPosition = t_sp;
		
		if(type == 1) // lazer
		{
			beamImg = CCSprite::create("beam.png");
			beamImg->setAnchorPoint(ccp(0,0.5));
			beamImg->setPosition(startPosition);
			beamImg->setColor(ccRED);
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, startPosition);
		beamBaseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		beamImg->setRotation(-(beamBaseAngle-10));
		
		addChild(beamImg);
		
		startMyAction();
	}
};

class AP_Missile7 : public AttackPattern
{
public:
	static AP_Missile7* create(int t_castFrame)
	{
		AP_Missile7* t_m7 = new AP_Missile7();
		t_m7->myInit(t_castFrame);
		t_m7->autorelease();
		return t_m7;
	}
	
	void myClock()
	{
		ingFrame++;
		
		int t_opacity = 255.f*ingFrame/castFrame;
		if(t_opacity > 255)		t_opacity = 255;
		
		background->setOpacity(t_opacity);
		death_side->setOpacity(t_opacity);
	}
	
	void startCut()
	{
		ingCut = 0;
		schedule(schedule_selector(AP_Missile7::cuting), 0.2);
		CCRotateBy* t_rotate = CCRotateBy::create(2, 360*10);
		death_side->runAction(t_rotate);
	}
	
	void selfDie()
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect");
		stopCut();
	}
	
	void stopCut()
	{
		unschedule(schedule_selector(AP_Missile7::cuting));
		background->removeFromParentAndCleanup(true);
		death_side->removeFromParentAndCleanup(true);
		startSelfRemoveSchedule();
	}
	
private:
	
	CCSprite* background;
	CCSprite* death_side;
	int castFrame;
	int ingFrame;
	int totalCutCnt;
	int ingCut;
	
	
	void cuting()
	{
		ingCut++;
		
		int random_value = rand()%360;
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		DeathSwing* t_ds = DeathSwing::create(jackPosition, random_value);
		addChild(t_ds);
		t_ds->startAnimation();
		
		if(ingCut >= totalCutCnt)
		{
			selfDie();
			stopCut();
		}
	}
	
	void myInit(int t_castFrame)
	{
		
		castFrame = t_castFrame;
		totalCutCnt = 6;
		
		background = CCSprite::create("death_background.png");
		background->setOpacity(0);
		background->setPosition(ccp(160,215));
		addChild(background);
		
		death_side = CCSprite::create("death_side.png");
		death_side->setOpacity(0);
		death_side->setAnchorPoint(ccp(0.4,0.4));
		death_side->setPosition(ccp(140,200));
		addChild(death_side);
		
		ingFrame = 0;
	}
};

class AP_Missile8 : public AttackPattern
{
public:
	static AP_Missile8* create(CCPoint t_sp, int t_mCnt, float t_distance, string imgFilename, CCSize t_mSize)
	{
		AP_Missile8* t_m8 = new AP_Missile8();
		t_m8->myInit(t_sp, t_mCnt, t_distance, imgFilename, t_mSize);
		t_m8->autorelease();
		return t_m8;
	}
	
private:
	
	void myInit(CCPoint t_sp, int t_mCnt, float t_distance, string imgFilename, CCSize t_mSize)
	{
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		CCPoint subPosition = ccpSub(jackPosition, t_sp);
		
		float shootAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		float createAngle;
		CCPoint startPosition;
		CCPoint dv;
		
		if(shootAngle >= 0.f && shootAngle <= 180.f)
		{
			createAngle = shootAngle - 90.f;
			CCPoint t_dv;
			t_dv.x = -1;
			t_dv.y = tanf((createAngle+180.f)/180.f*M_PI)*(-1.f);
			
			float div_value = sqrtf(powf(t_dv.x, 2.f) + powf(t_dv.y, 2.f));
			
			t_dv = ccpMult(t_dv, 1.f/div_value);
			dv = ccpMult(t_dv, t_mSize.width*(-1.f)*2.f);
			t_dv = ccpMult(t_dv, t_mSize.width*2.f*t_mCnt/2.f);
			startPosition = ccpAdd(t_sp, t_dv);
		}
		else
		{
			createAngle = shootAngle + 90.f;
			CCPoint t_dv;
			t_dv.x = 1;
			t_dv.y = tanf((createAngle-180.f)/180.f*M_PI);
			
			float div_value = sqrtf(powf(t_dv.x, 2.f) + powf(t_dv.y, 2.f));
			
			t_dv = ccpMult(t_dv, 1.f/div_value);
			dv = ccpMult(t_dv, t_mSize.width*(-1.f)*2.f);
			t_dv = ccpMult(t_dv, t_mSize.width*2.f*t_mCnt/2.f);
			startPosition = ccpAdd(t_sp, t_dv);
		}
		AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
		for(int i=0;i<t_mCnt;i++)
		{
			MissileUnit2* t_mu = MissileUnit2::create(ccpAdd(startPosition, ccpMult(dv, i)), shootAngle, t_distance, imgFilename, t_mSize, 0.f, 0.f);
			addChild(t_mu);
		}
		
		startSelfRemoveSchedule();
	}
};

class AP_Missile9 : public AttackPattern
{
public:
	static AP_Missile9* create(int t_keepFrame, int t_shootFrame, float t_distance, CCSize mSize, int t_type)
	{
		AP_Missile9* t_m9 = new AP_Missile9();
		t_m9->myInit(t_keepFrame, t_shootFrame, t_distance, mSize, t_type);
		t_m9->autorelease();
		return t_m9;
	}
	
	virtual void stopMyAction()
	{
		AudioEngine::sharedInstance()->stopEffect("sound_rock_falling.mp3");
		unschedule(schedule_selector(AP_Missile9::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemoveSchedule();
	}
	
	void removeEffect()
	{
		if(!isRemoveEffect)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_rock_falling.mp3");
			unschedule(schedule_selector(AP_Missile9::myAction));
			
			myGD->communication("MP_endIngActionAP");
			myGD->communication("CP_onPatternEnd");
			
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile9::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
		}
	}
	
private:
	
	int keepFrame;
	int shootFrame;
	int ingFrame;
	float distance;
	int type;
	CCSize mSize;
	
	bool isRemoveEffect;
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void startMyAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_rock_falling.mp3", true);
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile9::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame%shootFrame == 0)
		{
			MissileUnit3* t_mu = MissileUnit3::create(type, distance, mSize, this, callfunc_selector(AP_Missile9::removeEffect));
			addChild(t_mu);
		}
		
		if(ingFrame >= keepFrame)
		{
			stopMyAction();
		}
	}
	
	void myInit(int t_keepFrame, int t_shootFrame, float t_distance, CCSize t_mSize, int t_type)
	{
		
		keepFrame = t_keepFrame;
		shootFrame = t_shootFrame;
		distance = t_distance;
		type = t_type;
		mSize = t_mSize;
		
		startMyAction();
	}
};

class AP_Missile10 : public AttackPattern
{
public:
	static AP_Missile10* create(CCPoint t_sp, int t_tmCnt, int t_targetingFrame, int t_shootFrame, CCSize t_mSize)
	{
		AP_Missile10* t_m10 = new AP_Missile10();
		t_m10->myInit(t_sp, t_tmCnt, t_targetingFrame, t_shootFrame, t_mSize);
		t_m10->autorelease();
		return t_m10;
	}
	
	virtual void stopMyAction()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.5f, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile10::removeBiz));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		biz_spr->runAction(t_seq);
		
		unschedule(schedule_selector(AP_Missile10::myAction));
		
		while(!targetingArray.empty())
		{
			Targeting* t_t = targetingArray.front();
			targetingArray.pop_front();
			t_t->remove();
		}
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemoveSchedule();
	}
	
	void removeEffect()
	{
		if(!isRemoveEffect)
		{
			isRemoveEffect = true;
			unschedule(schedule_selector(AP_Missile10::myAction));
			myGD->communication("MP_endIngActionAP");
			myGD->communication("CP_onPatternEnd");
			
			CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile10::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
			
			biz_spr->runAction(t_seq);
		}
	}
	
private:
	
	CCPoint sp;
	int tmCnt;
	int targetingFrame;
	int targetRate;
	int shootFrame;
	int shootRate;
	int ingFrame;
	CCSize mSize;
	CCSprite* biz_spr;
	
	bool isRemoveEffect;
	
	deque<Targeting*> targetingArray;
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void removeBiz()
	{
		biz_spr->removeFromParentAndCleanup(true);
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		
		biz_spr = CCSprite::create("lazer_bead.png");
		biz_spr->setScale(0);
		biz_spr->setPosition(sp);
		addChild(biz_spr, 3);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.5f, 1.f);
		biz_spr->runAction(t_scale);
		
		schedule(schedule_selector(AP_Missile10::myAction));
	}
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame <= targetingFrame)
		{
			if(ingFrame%targetRate == 0)
			{
				// targeting
				IntPoint jackPoint = myGD->getJackPoint();
				CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
				
				Targeting* t_targeting = Targeting::create();
				t_targeting->setPosition(jackPosition);
				addChild(t_targeting, 2);
				
				targetingArray.push_back(t_targeting);
			}
		}
		else if(ingFrame <= targetingFrame + shootFrame)
		{
			if((ingFrame-targetingFrame)%shootRate == 0)
			{
				// shoot
				Targeting* t_t = targetingArray.front();
				targetingArray.pop_front();
				
				TG_Shoot* t_s = TG_Shoot::create(rand()%4+1, sp, t_t->getPosition(), 20, 10, t_t, mSize, this, callfunc_selector(AP_Missile10::removeEffect));
				addChild(t_s, 2);
			}
		}
		
		if(ingFrame >= targetingFrame + shootFrame)
		{
			stopMyAction();
		}
	}
	
	void myInit(CCPoint t_sp, int t_tmCnt, int t_targetingFrame, int t_shootFrame, CCSize t_mSize)
	{
		isRemoveEffect = false;
		
		mSize = t_mSize;
		sp = t_sp;
		tmCnt = t_tmCnt;
		targetingFrame = t_targetingFrame;
		targetRate = targetingFrame/tmCnt;
		shootFrame = t_shootFrame;
		shootRate = shootFrame/tmCnt;
		
		startMyAction();
	}
};

class AP_Missile11 : public AttackPattern
{
public:
	static AP_Missile11* create(CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize)
	{
		AP_Missile11* t_m11 = new AP_Missile11();
		t_m11->myInit(t_sp, t_type, t_speed, t_mSize);
		t_m11->autorelease();
		return t_m11;
	}
	
	virtual void stopMyAction()
	{
		myGD->communication("MP_endIngActionAP");
		
		startSelfRemoveSchedule();
	}
	
private:
	
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			myGD->communication("EP_stopCrashAction");
			myGD->communication("MS_resetRects");
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit(CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize)
	{
		
		myGD->communication("EP_startCrashAction");
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, t_sp);
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		float throwAngle;
		
		if(distance < 200)			throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%91-45;
		else						throwAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%31-15;
		
		
		ThrowObject* t_to = ThrowObject::create(t_sp, t_type, 2.f, throwAngle, t_mSize);
		addChild(t_to);
		t_to->startMyAction();
		stopMyAction();
	}
};

class AP_Missile12 : public AttackPattern
{
public:
	static AP_Missile12* create(CCPoint t_sp, int t_type, int t_targetingFrame, int t_shootFrame)
	{
		AP_Missile12* t_m12 = new AP_Missile12();
		t_m12->myInit(t_sp, t_type, t_targetingFrame, t_shootFrame);
		t_m12->autorelease();
		return t_m12;
	}
	
	virtual void stopMyAction()
	{
		unschedule(schedule_selector(AP_Missile12::myAction));
		
		if(wifiImg)			wifiImg->removeFromParentAndCleanup(true);
		if(targetingImg)	targetingImg->removeFromParentAndCleanup(true);
		if(myBeam)			myBeam->removeFromParentAndCleanup(true);
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		startSelfRemoveSchedule();
	}
	
	void removeEffect()
	{
		unschedule(schedule_selector(AP_Missile12::myAction));
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile12::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade1, t_call);
		
		CCFadeTo* t_fade2 = CCFadeTo::create(1.f, 0);
		
		wifiImg->runAction(t_seq);
		targetingImg->runAction(t_fade2);
	}
	
private:
	
	int type;
	int targetingFrame;
	int shootFrame;
	int ingFrame;
	deque<CCPoint> pJackArray;
	
	CCSprite* targetingImg;
	CCSprite* wifiImg;
	
	SatelliteBeam* myBeam;
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			myGD->communication("EP_stopCrashAction");
			myGD->communication("MS_resetRects");
			removeFromParentAndCleanup(true);
		}
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile12::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		targetingImg->setRotation(targetingImg->getRotation() + 2);
		if(ingFrame <= targetingFrame)
		{
			targetingImg->setScale(targetingImg->getScale() - 2.f/targetingFrame);
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			pJackArray.push_back(jackPosition);
			
			if(ingFrame == targetingFrame)
			{
				myBeam = SatelliteBeam::create(pJackArray.front(), type, this, callfunc_selector(AP_Missile12::removeEffect));
				addChild(myBeam);
			}
		}
		else if(ingFrame <= targetingFrame+shootFrame)
		{
			CCPoint t_p = pJackArray.front();
			pJackArray.pop_front();
			
			targetingImg->setPosition(t_p);
			myBeam->beamSetPosition(t_p);
			
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			pJackArray.push_back(jackPosition);
			
			if(ingFrame == targetingFrame+shootFrame)
			{
				wifiImg->removeFromParentAndCleanup(true);
				wifiImg = NULL;
			}
		}
		else
		{
			if(pJackArray.empty())
			{
				targetingImg->removeFromParentAndCleanup(true);
				targetingImg = NULL;
				myBeam->removeFromParentAndCleanup(true);
				myBeam = NULL;
				stopMyAction();
			}
			else
			{
				CCPoint t_p = pJackArray.front();
				pJackArray.pop_front();
				
				targetingImg->setPosition(t_p);
				myBeam->beamSetPosition(t_p);
			}
		}
	}
	
	void myInit(CCPoint t_sp, int t_type, int t_targetingFrame, int t_shootFrame)
	{
		type = t_type;
		targetingFrame = t_targetingFrame;
		shootFrame = t_shootFrame;
		
		
		
		myGD->communication("EP_startCrashAction");
		
		wifiImg = CCSprite::create("satelliteBeam_wifi.png", CCRectMake(0, 0, 30, 22));
		CCPoint wifiPosition = ccpAdd(t_sp, ccp(0,40));
		wifiImg->setPosition(wifiPosition);
		addChild(wifiImg);
		
		CCSprite* t_texture = CCSprite::create("satelliteBeam_wifi.png");
		
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.2);
		for(int i=0;i<5;i++)
		{
			t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*30, 0, 30, 22));
		}
		
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		wifiImg->runAction(t_repeat);
		
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		targetingImg = CCSprite::create("satelliteBeam_targeting.png");
		targetingImg->setScale(2.7);
		
		targetingImg->setPosition(jackPosition);
		
		addChild(targetingImg);
		
		startMyAction();
	}
};

class AP_Missile13 : public AttackPattern // dash
{
public:
	
	
private:
	
	
};

class AP_Missile14 : public AttackPattern
{
public:
	static AP_Missile14* create(CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize)
	{
		AP_Missile14* t_m14 = new AP_Missile14();
		t_m14->myInit(t_sp, t_type, t_speed, t_tmCnt, t_mSize);
		t_m14->autorelease();
		return t_m14;
	}
	
	virtual void stopMyAction()
	{
		myGD->communication("MP_endIngActionAP");
		
		startSelfRemoveSchedule();
	}
	
private:
	
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			myGD->communication("EP_stopCrashAction");
			myGD->communication("MS_resetRects");
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit(CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize)
	{
		
		
		myGD->communication("EP_startCrashAction");
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, t_sp);
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		float baseAngle;
		
		if(distance < 200)			baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%91 - 45;
		else						baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%31 - 15;
		
		for(int i=0;i<t_tmCnt;i++)
		{
			float t_angle = baseAngle + rand()%61 - 30;
			if(t_angle >= 180)		t_angle -= 360;
			if(t_angle < -180)		t_angle += 360;
			
			ThrowObject* t_to = ThrowObject::create(t_sp, t_type, 2.f, t_angle, t_mSize);
			addChild(t_to);
			t_to->startMyAction();
		}
		
		stopMyAction();
	}
};

class AP_Missile15 : public AttackPattern // burn
{
public:
	static AP_Missile15* create(CCPoint t_sp, int t_tmCnt, int t_burnFrame)
	{
		AP_Missile15* t_m15 = new AP_Missile15();
		t_m15->myInit(t_sp, t_tmCnt, t_burnFrame);
		t_m15->autorelease();
		return t_m15;
	}
	
	virtual void stopMyAction()
	{
		if(!is_remove_called)
		{
			is_remove_called = true;
			unschedule(schedule_selector(AP_Missile15::myAction));
			myParticle->setDuration(0);
			
			myGD->communication("MP_endIngActionAP");
			myGD->communication("CP_onPatternEnd");
			
			CCDelayTime* t_delay = CCDelayTime::create(1.2f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile15::particleRemove));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			startSelfRemoveSchedule();
		}
	}
	
	void removeEffect()
	{
		if(!is_remove_called)
		{
			is_remove_called = true;
			unschedule(schedule_selector(AP_Missile15::myAction));
			myParticle->setDuration(0);
			
			myGD->communication("MP_endIngActionAP");
			myGD->communication("CP_onPatternEnd");
			
			CCDelayTime* t_delay = CCDelayTime::create(1.2f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile15::selfRemove));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
		}
	}
	
private:
	
	int mType;
	int tmCnt;
	float baseAngle;
	float minAngle;
	float maxAngle;
	int burnFrame;
	int ingFrame;
	int createBurnFrame;
	CCParticleSystemQuad* myParticle;
	float baseDistance;
	CCPoint myPosition;
	
	bool is_remove_called;
	
	void selfRemove()
	{
		particleRemove();
		removeFromParentAndCleanup(true);
	}
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			myGD->communication("EP_stopCrashAction");
			myGD->communication("MS_resetRects");
			removeFromParentAndCleanup(true);
		}
	}
	
	void particleRemove()
	{
		myParticle->removeFromParentAndCleanup(true);
	}
	
	void startMyAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_fire_shot.mp3", false);
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile15::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		float shootAngle = minAngle + (maxAngle - minAngle + 0.f)/burnFrame*ingFrame;
		myParticle->setAngle(shootAngle);
		
		if(ingFrame%createBurnFrame == 0)
		{
			Burn* t_b = Burn::create(myPosition, baseDistance, shootAngle, mType, this, callfunc_selector(AP_Missile15::removeEffect));
			addChild(t_b);
			t_b->startMyAction();
		}
		
		
		if(ingFrame >= burnFrame)
		{
			stopMyAction();
		}
	}
	
	void initParticle()
	{
		myParticle = CCParticleSystemQuad::createWithTotalParticles(100);
		myParticle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture;
		if(mType == 1)
			texture = CCTextureCache::sharedTextureCache()->addImage("prison_fire.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("prison_ice.png");
		myParticle->setTexture(texture);
		myParticle->setEmissionRate(100.00);
		myParticle->setAngle(minAngle);						//
		myParticle->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		myParticle->setBlendFunc(blendFunc);
		myParticle->setDuration(-1.0);
		myParticle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor;
		if(mType == 1)
		{
			startColor.r = 1.0;
			startColor.g = 0.43;
			startColor.b = 0.08;
			startColor.a = 1.0;
		}
		else
		{
			startColor.r = 0.4;
			startColor.g = 0.8;
			startColor.b = 1.0;
			startColor.a = 1.0;
		}
		myParticle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		myParticle->setStartColorVar(startColorVar);
		ccColor4F endColor;
		if(mType == 1)
		{
			endColor.r = 1.0;
			endColor.g = 0.09;
			endColor.b = 0.05;
			endColor.a = 1.0;
		}
		else
		{
			endColor.r = 0;
			endColor.g = 0;
			endColor.b = 1.0;
			endColor.a = 1.0;
		}
		myParticle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		myParticle->setEndColorVar(endColorVar);
		myParticle->setStartSize(30.00);
		myParticle->setStartSizeVar(0.0);
		myParticle->setEndSize(50.0);
		myParticle->setEndSizeVar(10.0);
		myParticle->setGravity(ccp(0,0));
		myParticle->setRadialAccel(0.0);
		myParticle->setRadialAccelVar(0.0);
		myParticle->setSpeed(baseDistance);					//
		myParticle->setSpeedVar(20.0);						//
		myParticle->setTangentialAccel(0);
		myParticle->setTangentialAccelVar(0);
		myParticle->setTotalParticles(100);
		myParticle->setLife(1.00);
		myParticle->setLifeVar(0.3);
		myParticle->setStartSpin(0.0);
		myParticle->setStartSpinVar(0.0);
		myParticle->setEndSpin(0.0);
		myParticle->setEndSpinVar(0.0);
		myParticle->setPosition(myPosition);
		myParticle->setPosVar(CCPointZero);
		addChild(myParticle);
	}
	
	void myInit(CCPoint t_sp, int t_tmCnt, int t_burnFrame)
	{
		is_remove_called = false;
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		if(selected_chapter <= 20)
			mType = 1;
		else
		{
			int chapter_number = selected_chapter%10;
			if(chapter_number <= 3)
				mType = 2;
			else
				mType = 1;
		}
		
		myPosition = t_sp;
		tmCnt = t_tmCnt;
		burnFrame = t_burnFrame;
		createBurnFrame = burnFrame/tmCnt;
		
		
		
		myGD->communication("EP_startCrashAction");
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, t_sp);
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		if(distance < 200)
		{
			baseDistance = 200;
			baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%31 - 15;
		}
		else
		{
			baseDistance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			baseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		}
		
		baseDistance *= 1.01;

		minAngle = baseAngle - 30;
		maxAngle = baseAngle + 30;
		
		initParticle();
		startMyAction();
	}
};

class AP_Missile16 : public AttackPattern
{
public:
	static AP_Missile16* create(int t_type, int t_tmCnt, int t_totalFrame)
	{
		AP_Missile16* t_m16 = new AP_Missile16();
		t_m16->myInit(t_type, t_tmCnt, t_totalFrame);
		t_m16->autorelease();
		return t_m16;
	}
	
	virtual void stopMyAction()
	{
		if(!isRemoveEffect)
		{
			isRemoveEffect = true;
			unschedule(schedule_selector(AP_Missile16::myAction));
			
			myGD->communication("MP_endIngActionAP");
			
			startSelfRemoveSchedule();
		}
	}
	
	void removeEffect()
	{
		if(!isRemoveEffect)
		{
			isRemoveEffect = true;
			unschedule(schedule_selector(AP_Missile16::myAction));
			
			myGD->communication("MP_endIngActionAP");
			
			startSelfRemoveSchedule();
		}
	}
	
private:
	
	int type;
	int tmCnt;
	int mRate;
	int totalFrame;
	int ingFrame;
	string imgFilename;
	
	bool isRemoveEffect;
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			myGD->communication("EP_stopCrashAction");
			myGD->communication("MS_resetRects");
			removeFromParentAndCleanup(true);
		}
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile16::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame%mRate == 0)
		{
			CCPoint random_fp;
			random_fp.x = rand()%240;
			random_fp.y = rand()%320;
			
			CCPoint random_sp;
			random_sp.x = random_fp.x + 320;
			random_sp.y = random_fp.y + 320;
			
			FallMeteor* t_fm = FallMeteor::create(imgFilename, 1, CCSizeMake(90, 109), random_sp, random_fp, 60, 20, IntSize(15, 15), this, callfunc_selector(AP_Missile16::removeEffect)); // imgSize, crashSize
			addChild(t_fm);
		}
		
		if(ingFrame >= totalFrame)
		{
			stopMyAction();
		}
	}
	
	void myInit(int t_type, int t_tmCnt, int t_totalFrame)
	{
		isRemoveEffect = false;
		
		myGD->communication("EP_startCrashAction");
		type = t_type;
		tmCnt = t_tmCnt;
		totalFrame = t_totalFrame;
		mRate = totalFrame/tmCnt;
		
		if(type == 1) // stone meteor
		{
			imgFilename = "1.png";
		}
		else
		{
			imgFilename = "2.png";
		}
		
		startMyAction();
	}
};

class AP_Missile17 : public AttackPattern
{
public:
	static AP_Missile17* create(CCPoint t_sp, int t_type, int t_createRingFrame, int t_chargeFrame, int t_crashFrame)
	{
		AP_Missile17* t_m17 = new AP_Missile17();
		t_m17->myInit(t_sp, t_type, t_createRingFrame, t_chargeFrame, t_crashFrame);
		t_m17->autorelease();
		return t_m17;
	}
	
	virtual void stopMyAction()
	{
		stopMySchedule();
		removeObject();
	}
	
	void removeEffect()
	{
		unschedule(schedule_selector(AP_Missile17::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AP_Missile17::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		CCFadeTo* t_fade2 = CCFadeTo::create(1.f, 0);
		
		lazer_main->runAction(t_seq);
		lazer_sub->runAction(t_fade2);
	}
	
private:
	
	int type;
	CCPoint sp;
	int createRingFrame;
	int chargeFrame;
	int crashFrame;
	int ingFrame;
	float angle;
	CCPoint dv;
	CCSprite* lazer_main;
	int dcolor;
	CCSprite* t_bead;
	CCRect crashRect;
	CCSprite* lazer_sub;
	float dscale;
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			myGD->communication("EP_stopCrashAction");
			myGD->communication("MS_resetRects");
			removeFromParentAndCleanup(true);
		}
	}
	
	void stopMySchedule()
	{
		unschedule(schedule_selector(AP_Missile17::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemoveSchedule();
	}
	
	void removeObject()
	{
		if(lazer_sub)		lazer_sub->removeFromParentAndCleanup(true);
		if(lazer_main)		lazer_main->removeFromParentAndCleanup(true);
		if(t_bead)			t_bead->removeFromParentAndCleanup(true);
	}
	
	void startMyAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_angle_beem.mp3", false);
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile17::myAction));
	}
	
	void myAction()
	{
		if(ingFrame <= chargeFrame)
		{
			ccColor3B tcolor = t_bead->getColor();
			tcolor.g -= dcolor;
			tcolor.b -= dcolor;
			t_bead->setColor(tcolor);
			lazer_sub->setScaleY(lazer_sub->getScaleY()-dscale);
			lazer_sub->setColor(tcolor);
			if(ingFrame%createRingFrame == 0)
			{
				int random_sp = rand()%21-10;
				CCPoint r_sp = ccpMult(dv, 60 + random_sp);
				r_sp = ccpAdd(sp, r_sp);
				CCPoint r_fp = ccpMult(dv, 20);
				r_fp = ccpAdd(sp, r_fp);
				
				int random_frame = rand()%20 + 20;
				float random_s = (rand()%3)/10.f;
				
				Lazer_Ring* t_lr = Lazer_Ring::create(angle, r_sp, r_fp, 1.f-random_s, 0.3f-random_s, random_frame, tcolor);
				addChild(t_lr);
			}
			
			
			if(ingFrame == chargeFrame)
			{
				lazer_main = CCSprite::create("lazer_main.png", CCRectMake(0, 0, 460, 50));
				lazer_main->setAnchorPoint(ccp(0,0.5));
				lazer_main->setRotation(-angle);
				
				CCPoint mp = ccpMult(dv, 30);
				mp = ccpAdd(sp, mp);
				lazer_main->setPosition(mp);
				
				addChild(lazer_main);
				
				CCSprite* t_texture = CCSprite::create("lazer_main.png");
				CCAnimation* t_animation = CCAnimation::create();
				t_animation->setDelayPerUnit(0.1);
				for(int i=0;i<3;i++)
				{
					t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(0, i*50, 460, 50));
				}
				
				CCAnimate* t_animate = CCAnimate::create(t_animation);
				CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
				
				lazer_main->runAction(t_repeat);
				
				CCPoint c_sp = ccpMult(dv, 30);
				c_sp = ccpAdd(sp, c_sp);
				
				crashRect = CCRectMake(0, -60/2.f+10, 460, 60/2.f+10);
				
				lineCrashMap(c_sp, angle, 460, 60);
			}
		}
		else if(ingFrame <= chargeFrame+crashFrame)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
			
			CCPoint t_jp = spinTransform(jackPosition, sp, angle);
			
			if(crashRect.containsPoint(t_jp))
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect");
//				stopMySchedule();
				removeEffect();
			}
		}
		
		if(ingFrame >= chargeFrame+crashFrame)
		{
			stopMyAction();
		}
		ingFrame++;
	}
	
	void lineCrashMap(CCPoint t_sp, float t_angle, int t_width, int t_height)
	{
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
			{
				CCPoint t_tp = ccp((i-1)*pixelSize+1,(j-1)*pixelSize+1);
				CCPoint a_tp = spinTransform(t_tp, t_sp, t_angle);
				if(crashRect.containsPoint(a_tp))
				{
					crashMapForIntPoint(IntPoint(i,j));
				}
			}
		}
	}
	
	void crashMapForIntPoint(IntPoint t_p)
	{
		if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline || myGD->mapState[t_p.x][t_p.y] == mapOldget)) // just moment, only map crash
		{
			myGD->mapState[t_p.x][t_p.y] = mapEmpty;
			for(int k = -1;k<=1;k++)
			{
				for(int l = -1;l<=1;l++)
				{
					if(k == 0 && l == 0)	continue;
					if(myGD->mapState[t_p.x+k][t_p.y+l] == mapOldget)		myGD->mapState[t_p.x+k][t_p.y+l] = mapOldline;
				}
			}
			myGD->communication("EP_crashed");
			myGD->communication("MFP_createNewFragment", t_p);
			myGD->communication("VS_divideRect", t_p);
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		
		if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect");
			removeEffect();
		}
		
		if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
		{
			//					myGD->communication("PM_pathChainBomb", t_p);
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect");
			myGD->communication("Main_showLineDiePosition", t_p);
			removeEffect();
		}
	}
	
	CCPoint spinTransform(CCPoint t_tp, CCPoint t_bp, float t_angle)
	{
		CCPoint a_tp = ccpSub(t_tp, t_bp);
		float b_angle = atan2f(a_tp.y, a_tp.x)/M_PI*180.f;
		float a_angle = b_angle - t_angle;
		
		if(a_angle >= 180.f)	a_angle -= 360.f;
		if(a_angle < -180.f)	a_angle += 360.f;
		
		float distance = sqrtf(powf(a_tp.x, 2.f) + powf(a_tp.y, 2.f));
		
		a_tp.x = 1;
		a_tp.y = tanf(a_angle/180.f*M_PI);
		
		float div_value = sqrtf(powf(a_tp.x, 2.f) + powf(a_tp.y, 2.f));
		
		if(a_angle > 90 || a_angle < -90)
			a_tp = ccpMult(a_tp, -1.f);
		
		a_tp = ccpMult(a_tp, 1.f/div_value);
		
		a_tp = ccpMult(a_tp, distance);
		
		return a_tp;
	}
	
	void myInit(CCPoint t_sp, int t_type, int t_createRingFrame, int t_chargeFrame, int t_crashFrame)
	{
		
		myGD->communication("EP_startCrashAction");
		sp = t_sp;
		type = t_type;
		createRingFrame = t_createRingFrame;
		chargeFrame = t_chargeFrame;
		dcolor = 255.f/chargeFrame;
		dscale = 0.7f/chargeFrame;
		crashFrame = t_crashFrame;
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, t_sp);
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		if(distance < 200)			angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%91 - 45;
		else						angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + rand()%31 - 15;
		
		CCPoint beadPosition;
		beadPosition.x = 1;
		beadPosition.y = tanf(angle/180.f*M_PI);
		
		if((angle > 90.f && angle < 270.f) || angle < -90.f)
		{
			beadPosition = ccpMult(beadPosition, -1.f);
		}
		
		float div_value = sqrtf(powf(beadPosition.x, 2.f) + powf(beadPosition.y, 2.f));
		dv = ccpMult(beadPosition, 1.f/div_value);
		beadPosition = ccpMult(dv, 20.f);
		
		beadPosition = ccpAdd(beadPosition, t_sp);
		
		t_bead = CCSprite::create("lazer_bead.png");
		t_bead->setPosition(beadPosition);
		addChild(t_bead);
		
		lazer_sub = CCSprite::create("lazer_sub.png");
		lazer_sub->setAnchorPoint(ccp(0,0.5));
		lazer_sub->setRotation(-angle);
		
		CCPoint subP = ccpMult(dv, 5);
		subP = ccpAdd(beadPosition, subP);
		lazer_sub->setPosition(subP);
		addChild(lazer_sub);
		
		startMyAction();
	}
};

class AP_Missile18 : public AttackPattern
{
public:
	static AP_Missile18* create(CCPoint t_sp, float t_move_speed, int t_tmCnt, int t_cushion_cnt, bool t_is_big_bomb)
	{
		AP_Missile18* t_m18 = new AP_Missile18();
		t_m18->myInit(t_sp, t_move_speed, t_tmCnt, t_cushion_cnt, t_is_big_bomb);
		t_m18->autorelease();
		return t_m18;
	}
	
	void removeEffect()
	{
		if(!isRemoveEffect)
		{
			isRemoveEffect = true;
			int loop_cnt = getChildrenCount();
			for(int i=0;i<loop_cnt;i++)
			{
				((ThreeCushion*)getChildren()->objectAtIndex(i))->removeEffect();
			}
		}
	}
	
private:
	
	bool isRemoveEffect;
	
	virtual void selfRemoveSchedule()
	{
		if(getChildrenCount() == 0)
		{
			myGD->communication("EP_stopCrashAction");
			myGD->communication("MS_resetRects");
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit(CCPoint t_sp, float t_move_speed, int t_tmCnt, int t_cushion_cnt, bool t_is_big_bomb)
	{
		isRemoveEffect = false;
		myGD->communication("EP_startCrashAction");
		for(int i=0;i<t_tmCnt;i++)
		{
			// create
			ThreeCushion* t_tc = ThreeCushion::create(t_sp, t_move_speed, t_cushion_cnt, t_is_big_bomb, this, callfunc_selector(AP_Missile18::removeEffect));
			addChild(t_tc);
		}
		
		startSelfRemoveSchedule();
	}
};

class AP_Missile19 : public AttackPattern // ticking time bomb
{
public:
	
	
private:
	
	
};

class AP_Missile21 : public AttackPattern // blind
{
public:
	static AP_Missile21* create(CCPoint t_sp)
	{
		AP_Missile21* t_m21 = new AP_Missile21();
		t_m21->myInit(t_sp);
		t_m21->autorelease();
		return t_m21;
	}
	
private:
	
	
	void myInit(CCPoint t_sp)
	{
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		BlindDrop* t_bd = BlindDrop::create(t_sp, jackPosition, 20, 300);
		addChild(t_bd);
		t_bd->startAction();
		
		startSelfRemoveSchedule();
	}
};

class AP_Missile22 : public AttackPattern // poison line
{
public:
	static AP_Missile22* create(CCPoint t_sp)
	{
		AP_Missile22* t_m22 = new AP_Missile22();
		t_m22->myInit(t_sp);
		t_m22->autorelease();
		return t_m22;
	}
	
private:
	
	
	
	void myInit(CCPoint t_sp)
	{
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		PoisonDrop* t_pd = PoisonDrop::create(t_sp, jackPosition, 120);
		addChild(t_pd);
		t_pd->startAction();
		
		startSelfRemoveSchedule();
	}
};

class AP_Missile23 : public AttackPattern // cobweb
{
public:
	static AP_Missile23* create(int t_frame)
	{
		AP_Missile23* t_m23 = new AP_Missile23();
		t_m23->myInit(t_frame);
		t_m23->autorelease();
		return t_m23;
	}
	
	void updateCobweb()
	{
		ingFrame = 0;
	}
	
private:
	
	int slowFrame;
	int ingFrame;
	CCSprite* cobwebImg;
	
	void startFrame()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile23::framing));
	}
	
	void framing()
	{
		ingFrame++;
		
		if(ingFrame >= slowFrame)
		{
			stopFrame();
		}
	}
	
	void stopFrame()
	{
		unschedule(schedule_selector(AP_Missile23::framing));
		
		CCScaleTo* t_scale = CCScaleTo::create(0.3, 0.f);
		cobwebImg->runAction(t_scale);
		
		myGD->setAlphaSpeed(myGD->getAlphaSpeed()+0.5f);
		myGD->communication("MP_deleteKeepAP23");
		
		startSelfRemoveSchedule();
	}
	
	void myInit(int t_frame)
	{
		
		slowFrame = t_frame;
		
		cobwebImg = CCSprite::create("cobweb.png");
		cobwebImg->setPosition(ccp(160,215));
		cobwebImg->setScale(0);
		
		addChild(cobwebImg);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.3, 1.f);
		cobwebImg->runAction(t_scale);
		
		myGD->setAlphaSpeed(myGD->getAlphaSpeed()-0.5f);
		
		startFrame();
	}
};

class AP_Missile24 : public AttackPattern // sight out
{
public:
	static AP_Missile24* create(int t_frame)
	{
		AP_Missile24* t_m24 = new AP_Missile24();
		t_m24->myInit(t_frame);
		t_m24->autorelease();
		return t_m24;
	}
	
	void updateSightOut()
	{
		ingFrame = 0;
		
		SightOut* t_so = SightOut::create();
		t_so->setPosition(ccp(160,215));
		addChild(t_so);
		t_so->startAction();
	}
	
private:
	
	int sightOutFrame;
	int ingFrame;
	
	void startFrame()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile24::framing));
	}
	
	void framing()
	{
		ingFrame++;
		
		if(ingFrame >= sightOutFrame)
		{
			stopFrame();
		}
	}
	
	void stopFrame()
	{
		unschedule(schedule_selector(AP_Missile24::framing));
		
		myGD->communication("MP_deleteKeepAP24");
		startSelfRemoveSchedule();
	}
	
	void myInit(int t_frame)
	{
		
		sightOutFrame = t_frame;
		
		SightOut* t_so = SightOut::create();
		t_so->setPosition(ccp(160,215));
		addChild(t_so);
		t_so->startAction();
		
		startFrame();
	}
};

class AP_Missile25 : public AttackPattern // stun hammer
{
public:
	static AP_Missile25* create(int t_frame)
	{
		AP_Missile25* t_m25 = new AP_Missile25();
		t_m25->myInit(t_frame);
		t_m25->autorelease();
		return t_m25;
	}
	
	void updateStun()
	{
		myGD->communication("Jack_createHammer");
		ingFrame = 0;
	}
	
	void stopFrame()
	{
		unschedule(schedule_selector(AP_Missile25::framing));
		myGD->communication("MP_deleteKeepAP25");
		removeFromParentAndCleanup(true);
	}
	
private:
	
	int stunFrame;
	int ingFrame;
	
	void startFrame()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile25::framing));
	}
	
	void framing()
	{
		ingFrame++;
		
		if(ingFrame >= stunFrame)
		{
			myGD->communication("Main_touchOn");
			stopFrame();
		}
	}
	
	void myInit(int t_frame)
	{
		
		stunFrame = t_frame;
		
		myGD->communication("Jack_createHammer");
		startFrame();
	}
};

class AP_Missile26 : public AttackPattern // freeze
{
public:
	static AP_Missile26* create(int t_frame)
	{
		AP_Missile26* t_m26 = new AP_Missile26();
		t_m26->myInit(t_frame);
		t_m26->autorelease();
		return t_m26;
	}
	
	void updateFreeze()
	{
		myGD->communication("Jack_createFog");
		ingFrame = 0;
	}
	
	void stopFrame()
	{
		unschedule(schedule_selector(AP_Missile26::framing));
		myGD->communication("MP_deleteKeepAP26");
		removeFromParentAndCleanup(true);
	}
	
private:
	
	int freezingFrame;
	int ingFrame;
	
	void startFrame()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile26::framing));
	}
	
	void framing()
	{
		ingFrame++;
		
		if(ingFrame >= freezingFrame)
		{
			myGD->communication("Main_touchOn");
			stopFrame();
		}
	}
	
	void myInit(int t_frame)
	{
		
		freezingFrame = t_frame;
		
		myGD->communication("Jack_createFog");
		startFrame();
	}
};

class AP_Missile27 : public AttackPattern // sleep
{
public:
	static AP_Missile27* create(int t_frame)
	{
		AP_Missile27* t_m27 = new AP_Missile27();
		t_m27->myInit(t_frame);
		t_m27->autorelease();
		return t_m27;
	}
	
	void updateSleep()
	{
		myGD->communication("Jack_createSleep");
		ingFrame = 0;
	}
	
	void stopFrame()
	{
		unschedule(schedule_selector(AP_Missile27::framing));
		myGD->communication("MP_deleteKeepAP27");
		removeFromParentAndCleanup(true);
	}
	
private:
	
	int sleepFrame;
	int ingFrame;
	
	void startFrame()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile27::framing));
	}
	
	void framing()
	{
		ingFrame++;
		
		if(ingFrame >= sleepFrame)
		{
			myGD->communication("Main_touchOn");
			stopFrame();
		}
	}
	
	void myInit(int t_frame)
	{
		
		sleepFrame = t_frame;
		
		myGD->communication("Jack_createSleep");
		startFrame();
	}
};

class AP_Missile28 : public AttackPattern // prison
{
public:
	static AP_Missile28* create(CCPoint t_sp, int t_type, int t_distance, int t_tmCnt)
	{
		AP_Missile28* t_m28 = new AP_Missile28();
		t_m28->myInit(t_sp, t_type, t_distance, t_tmCnt);
		t_m28->autorelease();
		return t_m28;
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile28::myAction));
	}
	
private:
	
	CCPoint jackPosition;
	CCPoint sp;
	int type;
	int distance;
	int tmCnt;
	int prisonFrame;
	int ingFrame;
	int createFrame;
	float da;
	float baseAngle;
	
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame <= 60 && ingFrame%createFrame == 0)
		{
			for(int i=0;i<4;i++)
			{
				float t_angle = baseAngle+i*90.f;
				if(t_angle > 180)		t_angle -= 360;
				if(t_angle < -180)		t_angle += 360;
				
				CCPoint t_p;
				t_p.x = 1;
				t_p.y = tanf(t_angle/180.f*M_PI);
				
				if((t_angle > 90 && t_angle < 270) || t_angle < -90)
					t_p = ccpMult(t_p, -1.f);
				
				float div_value = sqrtf(powf(t_p.x, 2.f) + powf(t_p.y, 2.f));
				t_p = ccpMult(t_p, 1.f/div_value);
				t_p = ccpMult(t_p, distance);
				t_p = ccpAdd(t_p, jackPosition);
				
				PrisonObject* t_po = PrisonObject::create(t_p, type, 300);
				addChild(t_po);
				t_po->startMyAction();
			}
			
			baseAngle += da;
		}
		
		
		if(ingFrame-60 >= prisonFrame)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(AP_Missile28::myAction));
		startSelfRemoveSchedule();
	}
	
	void myInit(CCPoint t_sp, int t_type, int t_distance, int t_tmCnt) // create 0.5 second
	{
		
		IntPoint jackPoint = myGD->getJackPoint();
		jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		sp = t_sp;
		type = t_type;
		distance = t_distance;
		tmCnt = t_tmCnt;
		baseAngle = rand()%360-180.f;
		da = 90.f/(tmCnt/4.f);
		
		createFrame = 60/(tmCnt/4);
		
		prisonFrame = 300;
	}
};

class AP_Missile32 : public AttackPattern // teleport
{
public:
	static AP_Missile32* create()
	{
		AP_Missile32* t_m32 = new AP_Missile32();
		t_m32->myInit();
		t_m32->autorelease();
		return t_m32;
	}
	
private:
	
	
	void myInit()
	{
		
		myGD->communication("CP_startTeleport");
		startSelfRemoveSchedule();
	}
};

class AP_Missile33 : public AttackPattern // chaos
{
public:
	static AP_Missile33* create(int t_frame)
	{
		AP_Missile33* t_m33 = new AP_Missile33();
		t_m33->myInit(t_frame);
		t_m33->autorelease();
		return t_m33;
	}
	
	void updateChaos()
	{
		ingFrame = 0;
	}
	
	void stopFrame()
	{
		unschedule(schedule_selector(AP_Missile33::framing));
		myGD->communication("MP_deleteKeepAP33");
		startSelfRemoveSchedule();
	}
	
private:
	
	int chaosFrame;
	int ingFrame;
	
	void startFrame()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile33::framing));
	}
	
	void framing()
	{
		ingFrame++;
		
		if(ingFrame >= chaosFrame)
		{
			myGD->communication("Jack_reverseOff");
			stopFrame();
		}
	}
	
	void myInit(int t_frame)
	{
		
		chaosFrame = t_frame;
		
		myGD->communication("Jack_createChaos");
		startFrame();
	}
};

class AP_Missile34 : public AttackPattern // invisible
{
public:
	static AP_Missile34* create(int t_frame)
	{
		AP_Missile34* t_m34 = new AP_Missile34();
		t_m34->myInit(t_frame);
		t_m34->autorelease();
		return t_m34;
	}
	
	void updateInvisible()
	{
		ingFrame = 0;
	}
	
private:
	
	int invisibleFrame;
	int ingFrame;
	
	void startFrame()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile34::framing));
	}
	
	void framing()
	{
		ingFrame++;
		
		if(ingFrame >= invisibleFrame)
		{
			stopFrame();
		}
	}
	
	void stopFrame()
	{
		unschedule(schedule_selector(AP_Missile34::framing));
		myGD->communication("CP_mainCumberInvisibleOff");
		myGD->communication("MP_deleteKeepAP34");
		
		startSelfRemoveSchedule();
	}
	
	void myInit(int t_frame)
	{
		
		invisibleFrame = t_frame;
		
		myGD->communication("CP_mainCumberInvisibleOn");
		startFrame();
	}
};

class AP_Missile35 : public AttackPattern // moving shoot
{
public:
	static AP_Missile35* create(int t_ing_frame, int t_shoot_frame, int t_d_angle, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		AP_Missile35* t_m35 = new AP_Missile35();
		t_m35->myInit(t_ing_frame, t_shoot_frame, t_d_angle, t_distance, t_imgFilename, t_mSize);
		t_m35->autorelease();
		return t_m35;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AP_Missile35::framing));
		myGD->communication("CP_setMovingShoot", false);
		startSelfRemoveSchedule();
	}
	
private:
	
	int ing_frame;
	int shoot_frame;
	int recent_frame;
	int d_angle;
	int recent_angle;
	float m_speed;
	string imgFilename;
	CCSize mSize;
	
	void startFraming()
	{
		recent_frame = 0;
		recent_angle = rand()%360;
		schedule(schedule_selector(AP_Missile35::framing));
	}
	
	void framing()
	{
		recent_frame++;
		
		if(recent_frame%shoot_frame == 0)
		{
			recent_angle = (recent_angle+d_angle)%360;
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1, (mainCumberPoint.y-1)*pixelSize+1);
			MissileUnit* t_mu = MissileUnit::create(mainCumberPosition, recent_angle, m_speed, imgFilename, mSize, 0.f, 0.f);
			addChild(t_mu);
		}
		
		if(recent_frame >= ing_frame)
		{
			myGD->communication("MP_deleteKeepAP35");
		}
	}
	
	void myInit(int t_ing_frame, int t_shoot_frame, int t_d_angle, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		
		myGD->communication("CP_setMovingShoot", true);
		ing_frame = t_ing_frame;
		shoot_frame = t_shoot_frame;
		d_angle = t_d_angle;
		m_speed = t_distance;
		imgFilename = t_imgFilename;
		mSize = t_mSize;
		
		startFraming();
	}
};

class AP_Missile101 : public AttackPattern
{
public:
	static AP_Missile101* create(CCPoint t_sp, int t_keepFrame, int t_shootFrame, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		AP_Missile101* t_m3 = new AP_Missile101();
		t_m3->myInit(t_sp, t_keepFrame, t_shootFrame, t_distance, t_imgFilename, t_mSize);
		t_m3->autorelease();
		return t_m3;
	}
	
	virtual void stopMyAction()
	{
		unschedule(schedule_selector(AP_Missile101::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemove();
	}
	
private:
	
	CCPoint sp;
	int keepFrame;
	int shootFrame;
	int ingFrame;
	float distance;
	string imgFilename;
	CCSize mSize;
	int start_angle;
	
	CCSpriteBatchNode* t_node;
	
	void startSelfRemove()
	{
		schedule(schedule_selector(AP_Missile101::selfRemove));
	}
	
	void selfRemove()
	{
		if(t_node->getChildrenCount() <= 0)
		{
			unschedule(schedule_selector(AP_Missile101::selfRemove));
			removeFromParentAndCleanup(true);
		}
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile101::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame%shootFrame == 0)
		{
			AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
			for(int i=0;i<12;i++) // each missile 30do
			{
				float temp_angle = start_angle+(360.f/12)*i;
				
				MissileUnit* t_mu = MissileUnit::create(sp, temp_angle, distance, imgFilename, mSize, 0.f, 0.f);
				t_node->addChild(t_mu);
			}
			start_angle = start_angle+(360.f/12)/3;
		}
		
		if(ingFrame >= keepFrame)
		{
			stopMyAction();
		}
	}
	
	void myInit(CCPoint t_sp, int t_keepFrame, int t_shootFrame, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		
		sp = t_sp;
		keepFrame = t_keepFrame;
		shootFrame = t_shootFrame;
		distance = t_distance;
		imgFilename = t_imgFilename;
		mSize = t_mSize;
		
		t_node = CCSpriteBatchNode::create(imgFilename.c_str());
		addChild(t_node);
		
		start_angle = rand()%360;
		
		startMyAction();
	}
};

class AP_Missile102 : public AttackPattern
{
public:
	static AP_Missile102* create(CCPoint t_sp, int t_keepFrame, int t_shootFrame, int t_tmCnt, float t_shootAngle, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		AP_Missile102* t_m4 = new AP_Missile102();
		t_m4->myInit(t_sp, t_keepFrame, t_shootFrame, t_tmCnt, t_shootAngle, t_distance, t_imgFilename, t_mSize);
		t_m4->autorelease();
		return t_m4;
	}
	
	virtual void stopMyAction()
	{
		unschedule(schedule_selector(AP_Missile102::myAction));
		
		myGD->communication("MP_endIngActionAP");
		myGD->communication("CP_onPatternEnd");
		
		startSelfRemove();
	}
	
private:
	
	CCPoint sp;
	float shootBaseAngle;
	int keepFrame;
	int shootFrame;
	int ingFrame;
	int tmCnt;
	float shootAngle;
	float distance;
	string imgFilename;
	CCSize mSize;
	
	CCSpriteBatchNode* t_node;
	
	void startSelfRemove()
	{
		schedule(schedule_selector(AP_Missile102::selfRemove));
	}
	
	void selfRemove()
	{
		if(t_node->getChildrenCount() <= 0)
		{
			unschedule(schedule_selector(AP_Missile102::selfRemove));
			removeFromParentAndCleanup(true);
		}
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(AP_Missile102::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame%shootFrame == 0)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			CCPoint subPosition = ccpSub(jackPosition, sp);
			
			shootBaseAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f+(shootAngle <= 0.f ? 0 : rand()%30-15);
			
			float t_angle = shootBaseAngle - shootAngle/2.f;
			float unit_angle = shootAngle/tmCnt;
			
			for(int i=0;i<tmCnt;i++)
			{
				AudioEngine::sharedInstance()->playEffect("sound_basic_missile_shoot.mp3", false);
				MissileUnit* t_mu = MissileUnit::create(sp, t_angle+unit_angle*i, distance, imgFilename, mSize, 0.f, 0.f);
				t_node->addChild(t_mu);
			}
		}
		
		if(ingFrame >= keepFrame)
		{
			stopMyAction();
		}
	}
	
	void myInit(CCPoint t_sp, int t_keepFrame, int t_shootFrame, int t_tmCnt, float t_shootAngle, float t_distance, string t_imgFilename, CCSize t_mSize)
	{
		
		sp = t_sp;
		
		keepFrame = t_keepFrame;
		shootFrame = t_shootFrame;
		tmCnt = t_tmCnt;
		shootAngle = t_shootAngle;
		distance = t_distance;
		imgFilename = t_imgFilename;
		mSize = t_mSize;
		
		t_node = CCSpriteBatchNode::create(imgFilename.c_str());
		addChild(t_node);
		
		startMyAction();
	}
};

#endif
