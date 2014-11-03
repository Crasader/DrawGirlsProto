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
#include "cocos-ext.h"
#include "AudioEngine.h"
#include "GameData.h"
#include "jsoncpp/json.h"
#include "KSUtil.h"
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

class ChargeBase : public CCNode
{
public:
	virtual void cancelCharge();
	CC_SYNTHESIZE(int, charge_frame, ChargeFrame);
	CC_SYNTHESIZE(int, delayCount, DelayCount);
};
class ChargeNodeLambda : public ChargeBase
{
public:
	static ChargeNodeLambda* create(CCPoint t_position, int t_frame,
														std::function<void(CCObject*)> func,
														CCObject* t_rt, Json::Value pattern);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
	
private:
	Json::Value m_pattern;		
	//std::string m_pattern;
	CCPoint create_position;
	std::function<void(CCObject*)> actionFunction;
	CCObject* real_target;
	
	
	int charge_cnt;
	
	std::pair<CCSprite*, cocos2d::extension::CCBAnimationManager*> particle;
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
							std::function<void(CCObject*)> func,
							CCObject* t_rt, Json::Value pattern);
};



class SpecialChargeNodeLambda : public ChargeBase
{
public:
	static SpecialChargeNodeLambda* create(CCPoint t_position, int t_frame,
									 std::function<void(CCObject*)> func, CCObject* t_rt, Json::Value pattern);
	virtual ~SpecialChargeNodeLambda(){
		TRACE();
	}
	virtual void removeFromParent()
	{
    this->removeFromParentAndCleanup(true);
	}
	void removeFromParentAndCleanup(bool cleanup)
	{
    if (m_pParent != NULL)
    {
			m_pParent->removeChild(this,cleanup);
    }
	}
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
private:
	Json::Value m_pattern;
	CCPoint create_position;
	CCObject* real_target;
	std::function<void(CCObject*)> actionFunction;
	float ing_rps;
//	float chargeRate;
	
	int charge_cnt;
	
	std::pair<CCSprite*, cocos2d::extension::CCBAnimationManager*> particle;
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
				std::function<void(CCObject*)> func,
				CCObject* t_rt, Json::Value pattern);
};



class CrashChargeNodeLambda : public ChargeBase
{
public:
	virtual ~CrashChargeNodeLambda()
	{
		CCLOG("~CrashChargeNodeLambda");
	}
	static CrashChargeNodeLambda* create(CCPoint t_position, int t_frame,
																 std::function<void(CCObject*)> func,
																			 CCObject* t_rt, Json::Value pattern);
	
	void setChargeColor(ccColor4F change_color);
	
	void startCharge();
	
	void cancelCharge();
	
	CCObject* getRealTarget();
	
private:
	
	Json::Value m_pattern;
	CCPoint create_position;
	CCObject* real_target;
	std::function<void(CCObject*)> actionFunction;
	
	
	int charge_cnt;
	
	ccColor4F myColor;
	
	void charging();
	
	void removeSelf();
	
	void myInit(CCPoint t_position, int t_frame,
							std::function<void(CCObject*)> func,
							CCObject* t_rt, Json::Value pattern);
};


#endif /* defined(__DGproto__Charges__) */
