//
//  Charges.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 26..
//
//

#ifndef __DGproto__Charges__
#define __DGproto__Charges__

#include "cocos2d.h"
#include "AudioEngine.h"
#include "GameData.h"
USING_NS_CC;

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


class ChargeParent : public CCNode
{
public:
	virtual void cancelCharge()
	{
		
	}
};


class NoChargeNodeLambda : public ChargeParent
{
public:
	static NoChargeNodeLambda* create(CCPoint t_position, int t_frame,
															std::function<void(CCObject*)> func,
															CCObject* t_rt)
	{
		NoChargeNodeLambda* n_charge = new NoChargeNodeLambda();
		n_charge->init(t_position, t_frame, func, t_rt);
		
		n_charge->autorelease();
		return n_charge;
	}
	bool init(CCPoint t_position, int t_frame,
						std::function<void(CCObject*)> func,
						CCObject* t_rt)
	{
		real_target = t_rt;
		create_position = t_position;
		charge_frame = t_frame;
		actionFunction = func;
		
		return true;
	}
	void update(float dt)
	{
		if(getParent())
		{
			actionFunction(real_target);
			removeFromParentAndCleanup(true);
		}
	}
protected:
	int charge_frame;
	CCObject* real_target;
	std::function<void(CCObject*)> actionFunction;
	CCPoint create_position;
};


class NoChargeNode : public ChargeParent
{
public:
	static NoChargeNode* create(CCPoint t_position, int t_frame,
								CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
								CCObject* t_a_t, SEL_CallFuncO t_a_d,
								CCObject* t_c_t, SEL_CallFuncO t_c_d,
								CCObject* t_rt)
	{
		NoChargeNode* n_charge = new NoChargeNode();
		n_charge->init(t_position, t_frame, t_ing_t, t_ing_d, t_a_t, t_a_d, t_c_t, t_c_d, t_rt);
		
		n_charge->autorelease();
		return n_charge;
	}
	bool init(CCPoint t_position, int t_frame,
			  CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
			  CCObject* t_a_t, SEL_CallFuncO t_a_d,
			  CCObject* t_c_t, SEL_CallFuncO t_c_d,
			  CCObject* t_rt)
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
		
		return true;
	}
	void update(float dt)
	{
		if(getParent())
		{
			if(after_target && after_delegate)
				(after_target->*after_delegate)(real_target);
			
			removeFromParentAndCleanup(true);
		}
	}
protected:
	int charge_frame;
	CCObject* real_target;
	CCObject* charging_target;
	CCObject* after_target;
	CCObject* cancel_target;
	SEL_CallFuncO charging_delegate;
	SEL_CallFuncO after_delegate;
	SEL_CallFuncO cancel_delegate;
	CCPoint create_position;
};


class ChargeNodeLambda : public ChargeParent
{
public:
	static ChargeNodeLambda* create(CCPoint t_position, int t_frame,
														std::function<void(CCObject*)> func,
														CCObject* t_rt)
	{
		ChargeNodeLambda* n_charge = new ChargeNodeLambda();
		n_charge->myInit(t_position, t_frame, func, t_rt);
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
		schedule(schedule_selector(ChargeNodeLambda::charging));
	}
	
	void cancelCharge()
	{
		AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
		removeSelf();
	}
	
	CCObject* getRealTarget()
	{
		return real_target;
	}
	
private:
	
	
	CCPoint create_position;
	int charge_frame;
	std::function<void(CCObject*)> actionFunction;
	CCObject* real_target;
	
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging()
	{
		charge_cnt++;
		
		particle->setStartRadius((charge_frame/3.0)*(charge_frame-charge_cnt)/charge_frame);

		if(charge_cnt >= charge_frame)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
			actionFunction(real_target);
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(ChargeNodeLambda::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame,
							std::function<void(CCObject*)> func,
							CCObject* t_rt)
	{
		
		
		real_target = t_rt;
		create_position = t_position;
		charge_frame = t_frame;
		
		actionFunction = func;
		
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


class ChargeNode : public ChargeParent
{
public:
	static ChargeNode* create(CCPoint t_position, int t_frame,
							  CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
							  CCObject* t_a_t, SEL_CallFuncO t_a_d,
							  CCObject* t_c_t, SEL_CallFuncO t_c_d,
							  CCObject* t_rt)
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
			(cancel_target->*cancel_delegate)(real_target);
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
	SEL_CallFuncO charging_delegate;
	SEL_CallFuncO after_delegate;
	SEL_CallFuncO cancel_delegate;
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging()
	{
		charge_cnt++;
		
		particle->setStartRadius((charge_frame/3.0)*(charge_frame-charge_cnt)/charge_frame);
		if(charging_target && charging_delegate)
			(charging_target->*charging_delegate)(real_target);
		
		if(charge_cnt >= charge_frame)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
			if(after_target && after_delegate)
				(after_target->*after_delegate)(real_target);
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(ChargeNode::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame,
				CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
				CCObject* t_a_t, SEL_CallFuncO t_a_d,
				CCObject* t_c_t, SEL_CallFuncO t_c_d,
				CCObject* t_rt)
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

class SpecialChargeNodeLambda : public ChargeParent
{
public:
	static SpecialChargeNodeLambda* create(CCPoint t_position, int t_frame,
									 std::function<void(CCObject*)> func, CCObject* t_rt)
	{
		SpecialChargeNodeLambda* n_charge = new SpecialChargeNodeLambda();
		n_charge->myInit(t_position, t_frame, func, t_rt);
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
		schedule(schedule_selector(SpecialChargeNodeLambda::charging));
	}
	
	void cancelCharge()
	{
		AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
//		if(cancel_target && cancel_delegate)
//			(cancel_target->*cancel_delegate)(real_target);
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
	std::function<void(CCObject*)> actionFunction;
	float ing_rps;
	float chargeRate;
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging()
	{
		charge_cnt++;
		
		particle->setRotatePerSecond(particle->getRotatePerSecond() + chargeRate);
		
		if(charge_cnt >= charge_frame)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
			actionFunction(real_target);
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(SpecialChargeNodeLambda::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame,
				std::function<void(CCObject*)> func,
				CCObject* t_rt)
	{
		
		
		real_target = t_rt;
		create_position = t_position;
		charge_frame = t_frame;
		actionFunction = func;
		
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


class SpecialChargeNode : public ChargeParent
{
public:
	static SpecialChargeNode* create(CCPoint t_position, int t_frame,
									 CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
									 CCObject* t_a_t,	SEL_CallFuncO t_a_d,
									 CCObject* t_c_t,	SEL_CallFuncO t_c_d,
									 CCObject* t_rt)
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
			(cancel_target->*cancel_delegate)(real_target);
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
	SEL_CallFuncO charging_delegate;
	SEL_CallFuncO after_delegate;
	SEL_CallFuncO cancel_delegate;
	float ing_rps;
	float chargeRate;
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging()
	{
		charge_cnt++;
		
		particle->setRotatePerSecond(particle->getRotatePerSecond() + chargeRate);
		if(charging_target && charging_delegate)
			(charging_target->*charging_delegate)(real_target);
		
		if(charge_cnt >= charge_frame)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
			if(after_target && after_delegate)
				(after_target->*after_delegate)(real_target);
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(SpecialChargeNode::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame,
				CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
				CCObject* t_a_t, SEL_CallFuncO t_a_d,
				CCObject* t_c_t, SEL_CallFuncO t_c_d,
				CCObject* t_rt)
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

class CrashChargeNodeLambda : public ChargeParent
{
public:
	static CrashChargeNodeLambda* create(CCPoint t_position, int t_frame,
																 std::function<void(CCObject*)> func,
																 CCObject* t_rt)
	{
		CrashChargeNodeLambda* n_charge = new CrashChargeNodeLambda();
		n_charge->myInit(t_position, t_frame, func, t_rt);
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
		schedule(schedule_selector(CrashChargeNodeLambda::charging));
	}
	
	void cancelCharge()
	{
//		myGD->communication("CP_setCasting", false);
		AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
//		if(cancel_target && cancel_delegate)
//			(cancel_target->*cancel_delegate)(real_target);
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
	std::function<void(CCObject*)> actionFunction;
	
	
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
		
		
		if(charge_cnt >= charge_frame)
		{
//			myGD->communication("CP_setCasting", false);
			AudioEngine::sharedInstance()->stopAllEffects();
			AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
			actionFunction(real_target);
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(CrashChargeNodeLambda::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame,
							std::function<void(CCObject*)> func,
							CCObject* t_rt)
	{
		real_target = t_rt;
		create_position = t_position;
		charge_frame = t_frame;
		actionFunction = func;
		
		myColor = ccc4f(1.0, 1.0, 1.0, 1.0);
	}
};

class CrashChargeNode : public ChargeParent
{
public:
	static CrashChargeNode* create(CCPoint t_position, int t_frame,
								   CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
								   CCObject* t_a_t, SEL_CallFuncO t_a_d,
								   CCObject* t_c_t, SEL_CallFuncO t_c_d,
								   CCObject* t_rt)
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
//		myGD->communication("CP_setCasting", false);
		AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
		if(cancel_target && cancel_delegate)
			(cancel_target->*cancel_delegate)(real_target);
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
	SEL_CallFuncO charging_delegate;
	SEL_CallFuncO after_delegate;
	SEL_CallFuncO cancel_delegate;
	
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
			(charging_target->*charging_delegate)(real_target);
		
		if(charge_cnt >= charge_frame)
		{
			myGD->communication("CP_setCasting", false);
			AudioEngine::sharedInstance()->stopAllEffects();
			AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
			if(after_target && after_delegate)
				(after_target->*after_delegate)(real_target);
			removeSelf();
		}
	}
	
	void removeSelf()
	{
		unschedule(schedule_selector(CrashChargeNode::charging));
		myGD->communication("MP_removeChargeInArray", this);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_position, int t_frame,
				CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
				CCObject* t_a_t, SEL_CallFuncO t_a_d,
				CCObject* t_c_t, SEL_CallFuncO t_c_d,
				CCObject* t_rt)
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

#endif /* defined(__DGproto__Charges__) */
