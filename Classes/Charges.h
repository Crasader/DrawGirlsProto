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
#include "KSCumberBase.h"
USING_NS_CC;

class CCN_InnerNode : public CCSprite
{
public:
	static CCN_InnerNode* create(CCPoint t_fp, float t_distance, int t_moveFrame, ccColor4F t_color);
	
private:
	CCPoint dv;
	int moveFrame;
	int ingFrame;
	float ds;
	
	
	void startMove();
	void stopMove();
	void move();
	
	void myInit(CCPoint t_fp, float t_distance, int t_moveFrame, ccColor4F t_color);
};


class ChargeParent : public CCNode
{
public:
	virtual void cancelCharge();
};


class NoChargeNodeLambda : public ChargeParent
{
public:
	static NoChargeNodeLambda* create(CCPoint t_position, int t_frame,
															std::function<void(CCObject*)> func,
															CCObject* t_rt);
	bool init(CCPoint t_position, int t_frame,
						std::function<void(CCObject*)> func,
						CCObject* t_rt);
	void update(float dt);
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
								CCObject* t_rt);
	bool init(CCPoint t_position, int t_frame,
			  CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
			  CCObject* t_a_t, SEL_CallFuncO t_a_d,
			  CCObject* t_c_t, SEL_CallFuncO t_c_d,
			  CCObject* t_rt);
	void update(float dt);
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
														CCObject* t_rt);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
	
private:
	
	
	CCPoint create_position;
	int charge_frame;
	std::function<void(CCObject*)> actionFunction;
	CCObject* real_target;
	
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
							std::function<void(CCObject*)> func,
							CCObject* t_rt);
};


class ChargeNode : public ChargeParent
{
public:
	static ChargeNode* create(CCPoint t_position, int t_frame,
							  CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
							  CCObject* t_a_t, SEL_CallFuncO t_a_d,
							  CCObject* t_c_t, SEL_CallFuncO t_c_d,
							  CCObject* t_rt);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
	
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
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
				CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
				CCObject* t_a_t, SEL_CallFuncO t_a_d,
				CCObject* t_c_t, SEL_CallFuncO t_c_d,
				CCObject* t_rt);
};

class SpecialChargeNodeLambda : public ChargeParent
{
public:
	static SpecialChargeNodeLambda* create(CCPoint t_position, int t_frame,
									 std::function<void(CCObject*)> func, CCObject* t_rt);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
private:
	CCPoint create_position;
	int charge_frame;
	CCObject* real_target;
	std::function<void(CCObject*)> actionFunction;
	float ing_rps;
	float chargeRate;
	
	int charge_cnt;
	
	CCParticleSystemQuad* particle;
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
				std::function<void(CCObject*)> func,
				CCObject* t_rt);
};


class SpecialChargeNode : public ChargeParent
{
public:
	static SpecialChargeNode* create(CCPoint t_position, int t_frame,
									 CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
									 CCObject* t_a_t,	SEL_CallFuncO t_a_d,
									 CCObject* t_c_t,	SEL_CallFuncO t_c_d,
									 CCObject* t_rt);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
	
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
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
				CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
				CCObject* t_a_t, SEL_CallFuncO t_a_d,
				CCObject* t_c_t, SEL_CallFuncO t_c_d,
				CCObject* t_rt);
};

class CrashChargeNodeLambda : public ChargeParent
{
public:
	static CrashChargeNodeLambda* create(CCPoint t_position, int t_frame,
																 std::function<void(CCObject*)> func,
																 CCObject* t_rt);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
	
private:
	
	
	CCPoint create_position;
	int charge_frame;
	CCObject* real_target;
	std::function<void(CCObject*)> actionFunction;
	
	
	int charge_cnt;
	
	ccColor4F myColor;
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
							std::function<void(CCObject*)> func,
							CCObject* t_rt);
};

class CrashChargeNode : public ChargeParent
{
public:
	static CrashChargeNode* create(CCPoint t_position, int t_frame,
								   CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
								   CCObject* t_a_t, SEL_CallFuncO t_a_d,
								   CCObject* t_c_t, SEL_CallFuncO t_c_d,
								   CCObject* t_rt);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
	
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
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
				CCObject* t_ing_t, SEL_CallFuncO t_ing_d,
				CCObject* t_a_t, SEL_CallFuncO t_a_d,
				CCObject* t_c_t, SEL_CallFuncO t_c_d,
				CCObject* t_rt);
};

#endif /* defined(__DGproto__Charges__) */
