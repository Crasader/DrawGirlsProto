//
//  Charges.cpp
//  DGproto
//
//  Created by ksoo k on 13. 9. 26..
//
//

#include "Charges.h"
#include "KSCumberBase.h"
#include "ks19937.h"

CCN_InnerNode* CCN_InnerNode::create( CCPoint t_fp, float t_distance, int t_moveFrame, ccColor4F t_color )
{
	CCN_InnerNode* t_in = new CCN_InnerNode();
	t_in->myInit(t_fp, t_distance, t_moveFrame, t_color);
	t_in->autorelease();
	return t_in;
}

void CCN_InnerNode::startMove()
{
	ingFrame = 0;
	schedule(schedule_selector(CCN_InnerNode::move));
}

void CCN_InnerNode::stopMove()
{
	unschedule(schedule_selector(CCN_InnerNode::move));
	removeFromParentAndCleanup(true);
}

void CCN_InnerNode::move()
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

void CCN_InnerNode::myInit( CCPoint t_fp, float t_distance, int t_moveFrame, ccColor4F t_color )
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

void ChargeParent::cancelCharge()
{

}

void ChargeBase::cancelCharge()
{
	
}
NoChargeNodeLambda* NoChargeNodeLambda::create( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt )
{
	NoChargeNodeLambda* n_charge = new NoChargeNodeLambda();
	n_charge->init(t_position, t_frame, func, t_rt);

	n_charge->autorelease();
	return n_charge;
}

bool NoChargeNodeLambda::init( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt )
{
	real_target = t_rt;
	create_position = t_position;
	charge_frame = t_frame;
	actionFunction = func;

	return true;
}

void NoChargeNodeLambda::update( float dt )
{
	if(getParent())
	{
		actionFunction(real_target);
		removeFromParentAndCleanup(true);
		KSCumberBase* cb = dynamic_cast<KSCumberBase*>(real_target);
		if(cb)
		{
			cb->resetCastingCancelCount();
		}
	}
}


ChargeNodeLambda* ChargeNodeLambda::create( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt, Json::Value pattern )
{
	ChargeNodeLambda* n_charge = new ChargeNodeLambda();
	n_charge->myInit(t_position, t_frame, func, t_rt, pattern);
	n_charge->autorelease();
	return n_charge;
}

void ChargeNodeLambda::setChargeColor( ccColor4F change_color )
{
	particle.first->setColor(ccc3(change_color.r, change_color.g, change_color.b));

	//		charge_img->setColor(change_color);
}

void ChargeNodeLambda::startCharge()
{
	//myGD->communication("Main_showWarning", 1);
	charge_cnt = 0;
	AudioEngine::sharedInstance()->playEffect("se_castmissile.mp3", true);
//	AudioEngine::sharedInstance()->playEffect("sound_casting_attack.mp3", true);
//	AudioEngine::sharedInstance()->playEffect("sound_attackpattern_base.mp3", false);
	AudioEngine::sharedInstance()->playEffect("ment_cast_missile.mp3", false, true);
	schedule(schedule_selector(ChargeNodeLambda::charging));
}

void ChargeNodeLambda::cancelCharge()
{
	auto p = dynamic_cast<KSCumberBase*>(real_target);
	if(p)
	{
		p->onCanceledCasting();
		
	}
	
//	AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_pattern_cancel%d.mp3", ks19937::getIntValue(1, 4))->getCString());
//	AudioEngine::sharedInstance()->stopEffect("se_castmissile.mp3");
//	AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
	removeSelf();
}

CCObject* ChargeNodeLambda::getRealTarget()
{
	return real_target;
}

void ChargeNodeLambda::charging()
{
//	setPosition(dynamic_cast<KSCumberBase*>(real_target)->getPosition());
	charge_cnt++;

//	particle->setStartRadius((charge_frame/3.0)*(charge_frame-charge_cnt)/charge_frame);
	particle.first->setPosition(dynamic_cast<KSCumberBase*>(real_target)->getPosition());
	if(charge_cnt >= charge_frame)
	{
		//AudioEngine::sharedInstance()->stopEffect("se_castmissile.mp3");
//		AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
		myGD->communication("UI_setIsCasting", false);
		actionFunction(real_target);
		
		myGD->communication("Jack_showPatternEffect", m_pattern["pattern"].asInt());
		removeSelf();
		KSCumberBase* cb = dynamic_cast<KSCumberBase*>(real_target);
		
		if(cb)
		{
			cb->m_castFrameCount = 0;
			cb->m_stopFrameCount = 0;
			cb->resetCastingCancelCount();
			cb->m_lastCastTime = cb->m_cumberTimer;
//			cb->m_cumberState = 0;
			if(m_pattern.get("movingshot", false).asInt())
				cb->setCumberState(cb->m_cumberState | kCumberStateMoving);
			else
				cb->setCumberState(cb->m_cumberState & ~kCumberStateMoving); // 움직이고 있었다면 무효로.

			
//			CCLog("cb == %d", cb->m_cumberState);
			auto end = chrono::system_clock::now();
			auto currentSecond = chrono::system_clock::to_time_t(end);
			LastPattern lp;
			lp.exeTime = currentSecond;
			lp.exePattern = m_pattern["pattern"].asString();
			cb->setLastPattern(lp);
			myGD->communication("Main_hideScreenSideWarning"); // 화면에 빨간 테두리 지우는 함수
		}
	}
}

void ChargeNodeLambda::removeSelf()
{
	unschedule(schedule_selector(ChargeNodeLambda::charging));
	AudioEngine::sharedInstance()->stopEffect("se_castmissile.mp3");
	KSCumberBase* cumber = (KSCumberBase*)real_target;
	removeFromParent();
	
	auto iter = find(cumber->getCharges().begin(), cumber->getCharges().end(), this);
	
	if(iter != cumber->getCharges().end())
	{
		cumber->getCharges().erase(iter);
	}
//	cumber->getChargeParent()->removeFromParent();
//	cumber->setChargeParent(nullptr);
	
//	myGD->communication("MP_removeChargeInArray", this);
}

void ChargeNodeLambda::myInit( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt, Json::Value pattern )
{
	delayCount = 0;
	m_pattern = pattern;
	real_target = t_rt;
	create_position = t_position;
	charge_frame = t_frame;

	actionFunction = func;

//	float chargeRate = t_frame/60.f;
	
	auto castImage = KS::loadCCBI<CCSprite*>(this, "fx_cast.ccbi");
	KS::setBlendFunc(castImage.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	particle = castImage;
	particle.first->setPosition(create_position);
	addChild(castImage.first);
}


SpecialChargeNodeLambda* SpecialChargeNodeLambda::create( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt, Json::Value pattern)
{
	SpecialChargeNodeLambda* n_charge = new SpecialChargeNodeLambda();
	n_charge->myInit(t_position, t_frame, func, t_rt, pattern);
	n_charge->autorelease();
	return n_charge;
}

void SpecialChargeNodeLambda::setChargeColor( ccColor4F change_color )
{
	particle.first->setColor(ccc3(change_color.r, change_color.g, change_color.b));
//	particle->setStartColor(change_color);
//	particle->setEndColor(change_color);

	//		charge_img->setColor(change_color);
}

void SpecialChargeNodeLambda::startCharge()
{
	charge_cnt = 0;
	AudioEngine::sharedInstance()->playEffect("se_castspecial.mp3", true);
	AudioEngine::sharedInstance()->playEffect("ment_cast_special.mp3", false, true);
//	AudioEngine::sharedInstance()->playEffect("sound_casting_option.mp3", true);
	//myGD->communication("Main_showWarning", 3);
	schedule(schedule_selector(SpecialChargeNodeLambda::charging));
}

void SpecialChargeNodeLambda::cancelCharge()
{
	TRACE();
	auto p = dynamic_cast<KSCumberBase*>(real_target);
	if(p) 
		p->onCanceledCasting();
//	AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_pattern_cancel%d.mp3", ks19937::getIntValue(1, 4))->getCString());
//	AudioEngine::sharedInstance()->stopEffect("se_castspecial.mp3");
//	AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
	//		if(cancel_target && cancel_delegate)
	//			(cancel_target->*cancel_delegate)(real_target);
	removeSelf();
}

CCObject* SpecialChargeNodeLambda::getRealTarget()
{
	return real_target;
}

void SpecialChargeNodeLambda::charging()
{
	charge_cnt++;
//	setPosition(dynamic_cast<KSCumberBase*>(real_target)->getPosition());
//	particle->setRotatePerSecond(particle->getRotatePerSecond() + chargeRate);
	particle.first->setPosition(dynamic_cast<KSCumberBase*>(real_target)->getPosition());
	if(charge_cnt >= charge_frame)
	{
//		AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
		myGD->communication("UI_setIsCasting", false);
		actionFunction(real_target);
		myGD->communication("Jack_showPatternEffect", m_pattern["pattern"].asInt());
		removeSelf();
		KSCumberBase* cb = dynamic_cast<KSCumberBase*>(real_target);
		
		if(cb)
		{
			cb->m_castFrameCount = 0;
			cb->m_stopFrameCount = 0;
			cb->m_lastCastTime = cb->m_cumberTimer;
			cb->resetCastingCancelCount();
//			cb->m_cumberState = 0;
			if(m_pattern.get("movingshot", false).asInt())
				cb->setCumberState(cb->m_cumberState | kCumberStateMoving); // Moving 정보
			else
				cb->setCumberState(cb->m_cumberState & ~kCumberStateMoving); // 움직이고 있었다면 무효로.

//			CCLog("cb == %d", cb->m_cumberState);
			auto end = chrono::system_clock::now();
			auto currentSecond = chrono::system_clock::to_time_t(end);
			LastPattern lp;
			lp.exeTime = currentSecond;
			lp.exePattern = m_pattern["pattern"].asString();
			cb->setLastPattern(lp);
			myGD->communication("Main_hideScreenSideWarning"); // 화면에 빨간 테두리 지우는 함수
		}
	}
}

void SpecialChargeNodeLambda::removeSelf()
{
	unschedule(schedule_selector(SpecialChargeNodeLambda::charging));
	AudioEngine::sharedInstance()->stopEffect("se_castspecial.mp3");
	KSCumberBase* cumber = (KSCumberBase*)real_target;
	removeFromParent();
//	cumber->getChargeParent()->removeFromParent();
//	cumber->setChargeParent(nullptr);
	auto iter = find(cumber->getCharges().begin(), cumber->getCharges().end(), this);
	
	if(iter != cumber->getCharges().end())
	{
		cumber->getCharges().erase(iter);
//		CCLOG("%x remove", this);
	}
	myGD->communication("MP_removeChargeInArray", this);
}

void SpecialChargeNodeLambda::myInit( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt, Json::Value pattern )
{
	TRACE();
	delayCount = 0;
	m_pattern = pattern;
	real_target = t_rt;
	create_position = t_position;
	charge_frame = t_frame;
	actionFunction = func;

	ing_rps = 0;
	auto castImage = KS::loadCCBI<CCSprite*>(this, "fx_cast.ccbi");
	KS::setBlendFunc(castImage.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	particle = castImage;
	particle.first->setPosition(create_position);
	addChild(castImage.first);
}


CrashChargeNodeLambda* CrashChargeNodeLambda::create( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt, Json::Value pattern )
{
	CrashChargeNodeLambda* n_charge = new CrashChargeNodeLambda();
	n_charge->myInit(t_position, t_frame, func, t_rt, pattern);
	n_charge->autorelease();
	return n_charge;
}

void CrashChargeNodeLambda::setChargeColor( ccColor4F change_color )
{
	myColor = change_color;
}

void CrashChargeNodeLambda::startCharge()
{
//	AudioEngine::sharedInstance()->playEffect("sound_attackpattern_crash.mp3", false);
	AudioEngine::sharedInstance()->playEffect("ment_cast_crash.mp3", false, true);
	//myGD->communication("Main_showWarning", 2);
	charge_cnt = 0;
	AudioEngine::sharedInstance()->playEffect("se_castmap.mp3", true);
//	AudioEngine::sharedInstance()->playEffect("sound_casting_crash.mp3", true);
	schedule(schedule_selector(CrashChargeNodeLambda::charging));
}

void CrashChargeNodeLambda::cancelCharge()
{
	auto p = dynamic_cast<KSCumberBase*>(real_target);
	if(p) p->onCanceledCasting();

	//		myGD->communication("CP_setCasting", false);
//	AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_pattern_cancel%d.mp3", ks19937::getIntValue(1, 4))->getCString());
//	AudioEngine::sharedInstance()->stopEffect("se_castmap.mp3");
//	AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
	//		if(cancel_target && cancel_delegate)
	//			(cancel_target->*cancel_delegate)(real_target);
	CCLOG("crash cancelCharge");
	removeSelf();
}

CCObject* CrashChargeNodeLambda::getRealTarget()
{
	return real_target;
}

void CrashChargeNodeLambda::charging()
{
	
	charge_cnt++;

//	IntPoint mainCumberPoint = myGD->getMainCumberPoint((CCNode*)real_target);
//	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
//
//	setPosition(ccpSub(mainCumberPosition, create_position));
//dynamic_cast<KSCumberBase*>(real_target)->getPosition()
	for(int i=0;i<2;i++)
	{
		CCN_InnerNode* t_in = CCN_InnerNode::create(dynamic_cast<KSCumberBase*>(real_target)->getPosition(), 40*((0.f + charge_frame - charge_cnt)/charge_frame), 10, myColor);
		addChild(t_in);
	}


	if(charge_cnt >= charge_frame)
	{
		//			myGD->communication("CP_setCasting", false);
		AudioEngine::sharedInstance()->stopAllEffects();
//		AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
		myGD->communication("UI_setIsCasting", false);
		actionFunction(real_target);
		myGD->communication("Jack_showPatternEffect", m_pattern["pattern"].asInt());
		CCLOG("removeSelf call");
		removeSelf();
		KSCumberBase* cb = dynamic_cast<KSCumberBase*>(real_target);
		
		if(cb)
		{
			cb->m_lastCastTime = cb->m_cumberTimer;
			cb->m_crashAttackTime = cb->m_cumberTimer;
			cb->m_castFrameCount = 0;
			cb->m_stopFrameCount = 0;
			cb->resetCastingCancelCount();
//			cb->m_cumberState = 0;
			if(m_pattern.get("movingshot", false).asInt())
				cb->setCumberState(cb->m_cumberState | kCumberStateMoving); // Moving 정보
			else
				cb->setCumberState(cb->m_cumberState & ~kCumberStateMoving); // 움직이고 있었다면 무효로.

			
//			CCLog("cb == %d", cb->m_cumberState);
			auto end = chrono::system_clock::now();
			auto currentSecond = chrono::system_clock::to_time_t(end);
			LastPattern lp;
			lp.exeTime = currentSecond;
			lp.exePattern = m_pattern["pattern"].asString();
			cb->setLastPattern(lp);
			myGD->communication("Main_hideScreenSideWarning"); // 화면에 빨간 테두리 지우는 함수
		}
	}
}

void CrashChargeNodeLambda::removeSelf()
{
	unschedule(schedule_selector(CrashChargeNodeLambda::charging));
	AudioEngine::sharedInstance()->stopEffect("se_castmap.mp3");
	KSCumberBase* cumber = (KSCumberBase*)real_target;
	removeFromParent();
	
//	cumber->getChargeParent()->removeFromParent();
//	cumber->setChargeParent(nullptr);
	auto iter = find(cumber->getCharges().begin(), cumber->getCharges().end(), this);
	
	if(iter != cumber->getCharges().end())
	{
		cumber->getCharges().erase(iter);
//		CCLOG("%x remove", this);
	}
	myGD->communication("MP_removeChargeInArray", this);
	
	CCLOG("crash removeFromParent1");
}

void CrashChargeNodeLambda::myInit( CCPoint t_position, int t_frame, std::function<void(CCObject*)> func, CCObject* t_rt, Json::Value pattern )
{
	delayCount = 0;
	m_pattern = pattern;
	real_target = t_rt;
	create_position = t_position;
	charge_frame = t_frame;
	actionFunction = func;

	myColor = ccc4f(1.0, 1.0, 1.0, 1.0);
}

