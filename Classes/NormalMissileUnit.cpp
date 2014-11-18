//
//  NormalMissileUnit.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 11. 18..
//
//

#include "MissileUnit.h"

#include "StarGoldData.h"
#include "KSUtil.h"

#include "ProbSelector.h"
#include "MissileDamageData.h"

#include "KSUtil.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "Jack.h"
#include "EffectSprite.h"
#include "EasingAction.h"

using namespace std;
using namespace placeholders;
static ProbSelector randomObj = {9, 1};
MissileUnit * MissileUnit::create (CCPoint t_sp, float t_angle, float t_distance, string imgFilename, CCSize t_cs, float t_da, float t_reduce_da,
																	 bool isSuper)
{
	MissileUnit* t_mu = new MissileUnit();
	if(t_mu && t_mu->initWithFile(imgFilename.c_str()))
	{
		t_mu->myInit(t_sp, t_angle, t_distance, t_cs, t_da, t_reduce_da, isSuper);
		t_mu->autorelease();
		return t_mu;
	}
	
	CC_SAFE_DELETE(t_mu);
	
	return NULL;
}
void MissileUnit::startMove ()
{
	schedule(schedule_selector(MissileUnit::move));
	
}
void MissileUnit::stopMove ()
{
	unschedule(schedule_selector(MissileUnit::move));
}
void MissileUnit::removeEffect ()
{
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileUnit::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	runAction(t_seq);
}
void MissileUnit::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void MissileUnit::move ()
{
	//	setRotation(getRotation()-2);
	angle += da;
	if(angle >= 180.f)	angle -= 360.f;
	if(angle < -180.f)	angle += 360.f;
	
	
	CCPoint beforePosition = getPosition() + getParent()->getPosition();
	//		CCLOG("%x -- prev collision %f %f", this, beforePosition.x, beforePosition.y);
	CCPoint r_p = getPosition(); // recent
	CCPoint dv;
	
	dv.x = 1;
	dv.y = tanf(angle/180.f*M_PI);
	
	if(angle >= 90.f || angle <= -90.f)			dv = ccpMult(dv, -1.f);
	
	float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
	dv = ccpMult(dv, 1.f/div_value);
	dv = ccpMult(dv, distance);
	setRotation(-rad2Deg(atan2(dv.y, dv.x)));
	r_p = ccpAdd(r_p, dv);
	CCPoint p_p = getParent()->getPosition(); // parent
	p_p = ccpAdd(r_p, p_p);
	
	//		CCLOG("%x -- collision %f %f", this, p_p.x, p_p.y);
	if(p_p.x < 0.f - 40.f || p_p.x > 320.f + 40.f || p_p.y < -60.f - 40.f || p_p.y > 490.f + 40.f) // fixed 40.f
	{
		stopMove();
		removeFromParentAndCleanup(true);
		return;
	}
	
	setPosition(r_p);
	if(enabled)
	{
		if(is_checking)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			CCRect missile_rect = CCRectMake(p_p.x - crashSize.width/2.f, p_p.y - crashSize.height/2.f, crashSize.width, crashSize.height);
			
			//			IntPoint p_pBeforePoint = ccp2ip(beforePosition);
			//			IntPoint p_pPoint = ccp2ip(p_p);
			if(missile_rect.containsPoint(jackPosition)) //  && myGD->getJackState()
			{
				is_checking = false;
				//				if(mySGD->getIsHard() || myGD->getJackState())
				// 수퍼 미사일이거나 맵 밖이라면 죽임.
				if(isSuper ||
					 (myGD->getJackState() != jackStateNormal && !myGD->getCommunicationBool("PM_isShortLine")))
				{
					myGD->communication("CP_jackCrashDie");
					myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
					stopMove();
					removeEffect();
				}
				else
				{
					myGD->communication("Jack_showMB");
				}
			}
			//##
			else
			{
				if(lineTouch)
				{
					float angle = atan2(p_p.y - beforePosition.y, p_p.x - beforePosition.x);
					int loop = ccpLength(p_p - beforePosition) / 1.414f;
					CCPoint t2 = beforePosition;
					for(int i=0; i<loop; i++)
					{
						
						t2.x += 1.414f * cos(angle);
						t2.y += 1.414f * sin(angle);
						
						IntPoint t = ccp2ip(t2);
						if(t.isInnerMap() && myGD->mapState[t.x][t.y] == mapType::mapNewline)
						{
							if(!myGD->getCommunicationBool("PM_isShortLine"))
								myGD->communication("PM_addPathBreaking", t);
						}
					}
					IntPoint t = ccp2ip(p_p);
					if(t.isInnerMap() && myGD->mapState[t.x][t.y] == mapType::mapNewline)
					{
						if(!myGD->getCommunicationBool("PM_isShortLine"))
							myGD->communication("PM_addPathBreaking", t);
					}
					
				}
			}
		}
	}
	
	
	da *= reduce_da;
}
void MissileUnit::myInit (CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, float t_da, float t_reduce_da,
													bool t_isSuper)
{
	lineTouch = true; // 선 건듬.
	is_checking = true;
	isSuper = t_isSuper;
	crashSize = t_cs;
	angle = t_angle;
	distance = t_distance;
	da = t_da;
	reduce_da = t_reduce_da;
	enabled = true;
	//	setOpacity(0);
	//	auto missile = KS::loadCCBI<CCSprite*>(this, "ingame_missile.ccbi").first;
	//	missile->setPosition(ccp(getContentSize().width / 2.f, getContentSize().height / 2.f));
	//	missile->setAnchorPoint(ccp(0, 0));
	//	addChild(missile);
	setPosition(t_sp);
	startMove();
}

void MissileUnit::setEnabled(bool e)
{
	enabled = e;
}
void MissileUnit::setLineTouch(bool e)
{
	lineTouch = e;
}
MissileUnit2 * MissileUnit2::create (CCPoint t_sp, float t_angle, float t_distance, string imgFilename, CCSize t_cs, float t_da, float t_reduce_da)
{
	MissileUnit2* t_mu = new MissileUnit2();
	if(t_mu && t_mu->initWithFile(imgFilename.c_str()))
	{
		t_mu->myInit(t_sp, t_angle, t_distance, t_cs, t_da, t_reduce_da);
		t_mu->autorelease();
		return t_mu;
	}
	CC_SAFE_DELETE(t_mu);
	return NULL;
}
void MissileUnit2::startMove ()
{
	schedule(schedule_selector(MissileUnit2::move));
}
void MissileUnit2::stopMove ()
{
	unschedule(schedule_selector(MissileUnit2::move));
}
void MissileUnit2::removeEffect ()
{
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileUnit2::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	runAction(t_seq);
}
void MissileUnit2::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void MissileUnit2::move ()
{
	setRotation(getRotation()-2);
	angle += da;
	if(angle >= 180.f)	angle -= 360.f;
	if(angle < -180.f)	angle += 360.f;
	
	CCPoint r_p = getPosition(); // recent
	CCPoint dv;
	
	dv.x = 1;
	dv.y = tanf(angle/180.f*M_PI);
	
	if(angle >= 90.f || angle <= -90.f)			dv = ccpMult(dv, -1.f);
	
	float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
	dv = ccpMult(dv, 1.f/div_value);
	dv = ccpMult(dv, distance);
	
	r_p = ccpAdd(r_p, dv);
	CCPoint p_p = getParent()->getPosition(); // parent
	p_p = ccpAdd(r_p, p_p);
	
	if(p_p.x < 0.f - 40.f || p_p.x > 320.f + 40.f || p_p.y < -60.f - 40.f || p_p.y > 490.f + 40.f) // fixed 40.f
	{
		stopMove();
		removeFromParentAndCleanup(true);
		return;
	}
	
	setPosition(r_p);
	
	if(is_checking)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		CCRect missile_rect = CCRectMake(p_p.x - crashSize.width/2.f, p_p.y - crashSize.height/2.f, crashSize.width, crashSize.height);
		
		if(missile_rect.containsPoint(jackPosition))
		{
			is_checking = false;
			if(myGD->getJackState() && !myGD->getCommunicationBool("PM_isShortLine"))
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				stopMove();
				removeEffect();
			}
			else
			{
				myGD->communication("Jack_showMB");
			}
		}
	}
	
	da *= reduce_da;
}
void MissileUnit2::myInit (CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, float t_da, float t_reduce_da)
{
	is_checking = true;
	
	crashSize = t_cs;
	angle = t_angle;
	distance = t_distance;
	da = t_da;
	reduce_da = t_reduce_da;
	//	setOpacity(0);
	//	auto missile = KS::loadCCBI<CCSprite*>(this, "ingame_missile.ccbi").first;
	//	missile->setAnchorPoint(ccp(0, 0));
	//	addChild(missile);
	setPosition(t_sp);
	startMove();
}
MissileUnit3 * MissileUnit3::create (int t_type, float t_distance, CCSize t_mSize, float enablePercent, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	MissileUnit3* t_mu3 = new MissileUnit3();
	t_mu3->myInit(t_type, t_distance, t_mSize, enablePercent, t_removeEffect, d_removeEffect);
	t_mu3->autorelease();
	return t_mu3;
}
void MissileUnit3::startMove ()
{
	schedule(schedule_selector(MissileUnit3::move));
}
void MissileUnit3::stopMove ()
{
	unschedule(schedule_selector(MissileUnit3::move));
}
void MissileUnit3::removeEffect ()
{
	(target_removeEffect->*delegate_removeEffect)();
	
	addChild(KSGradualValue<float>::create(255, 0, 0.01f, [=](float t)
																				 {
																					 stoneSprite->setOpacity(t);
																					 stoneSprite->setVisible(false);
																				 },
																				 [=](float t)
																				 {
																					 
																					 auto ret = KS::loadCCBI<CCSprite*>(this, "bomb_8_4.ccbi");
																					 KS::setBlendFunc(ret.first, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
																					 ret.first->setPosition(stoneSprite->getPosition());
																					 addChild(ret.first, 2);
																					 addChild(KSTimer::create(2.f, [=]()
																																		{
																																			this->selfRemove();
																																		}));
																				 }));
}
void MissileUnit3::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void MissileUnit3::move ()
{
	CCPoint afterPosition = ccp(stoneSprite->getPositionX(), stoneSprite->getPositionY() - distance);
	stoneSprite->setPosition(afterPosition);
	
	IntPoint jackPoint = myGD->getJackPoint();
	CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
	
	CCRect afterRect = CCRectMake(afterPosition.x-mSize.width/2.f, afterPosition.y-mSize.height/2.f, mSize.width, mSize.height);
	if(afterRect.containsPoint(jackPosition))
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		stopMove();
		removeEffect();
	}
	if(afterRect.origin.y + afterRect.size.height / 2.f - jackPosition.y < 200 && shownWarning == false)
	{
		shownWarning = true;
		
		
		
		ProbSelector ps = {enableProb, 1.f - enableProb};
		if(ps.getResult() == 1) // disable 확률 되면
		{
			
			
			EffectSprite* vertical = EffectSprite::create("lazer_sub.png");
			vertical->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
			vertical->setColor(ccc3(255, 0, 0));
			vertical->setScaleX(2.f);
			vertical->setScaleY(0.25f);
			vertical->setAnchorPoint(ccp(0.5f, 0.5f));
			vertical->setRotation(90.f);
			vertical->setPosition(ccp((afterRect.origin.x + afterRect.size.width / 2.f), mapLoopRange::mapHeightInnerEnd));
			vertical->setGray();
			
			addChild(vertical);
			
			CCSprite* cancelMark = CCSprite::create("stone_warning_cancel.png");
			cancelMark->setPosition(ccp((afterRect.origin.x + afterRect.size.width / 2.f), jackPosition.y + 40));
			addChild(cancelMark);
			addChild(KSTimer::create(1.f, [=]()
															 {
																 addChild(KSGradualValue<float>::create(255, 0, 0.3f, [=](float t)
																																				{
																																					cancelMark->setOpacity(t);
																																				}, [=](float t)
																																				{
																																					//																						 vertical->removeFromParent();
																																					//																						 feelMark
																																					removeFromParentAndCleanup(true);
																																				}));
															 }));
			unschedule(schedule_selector(MissileUnit3::move));
			
			
		}
		else
		{
			CCSprite* vertical = CCSprite::create("lazer_sub.png");
			vertical->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
			vertical->setColor(ccc3(255, 0, 0));
			vertical->setScaleX(2.f);
			vertical->setScaleY(0.25f);
			vertical->setAnchorPoint(ccp(0.5f, 0.5f));
			vertical->setRotation(90.f);
			vertical->setPosition(ccp((afterRect.origin.x + afterRect.size.width / 2.f), mapLoopRange::mapHeightInnerEnd));
			
			addChild(vertical);
			
			
			CCSprite* feelMark = CCSprite::create("stone_warning.png");
			feelMark->setPosition(ccp((afterRect.origin.x + afterRect.size.width / 2.f), jackPosition.y + 40));
			addChild(feelMark);
			addChild(KSTimer::create(1.f, [=]()
															 {
																 addChild(KSGradualValue<float>::create(255, 0, 0.3f, [=](float t)
																																				{
																																					vertical->setOpacity(t);
																																					feelMark->setOpacity(t);
																																				}, [=](float t)
																																				{
																																					//																						 vertical->removeFromParent();
																																					//																						 feelMark
																																				}));
															 }));
		}
		//		CCLOG("warning!!");
	}
	if(afterPosition.y < -mSize.height-60.f)
	{
		stopMove();
		removeFromParentAndCleanup(true);
	}
}
void MissileUnit3::myInit (int t_type, float t_distance, CCSize t_mSize, float enableP, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	target_removeEffect = t_removeEffect;
	delegate_removeEffect = d_removeEffect;
	
	enableProb = enableP;
	shownWarning = false;
	myType = t_type;
	distance = t_distance;
	mSize = t_mSize;
	
	if(myType == 1) // stone
	{
		//			initWithFile("fallingStone.png", CCRectMake(0, 0, 35, 35));
		
		auto ret = KS::loadCCBI<CCSprite*>(this, "stone_1.ccbi");//"pattern_marble1.ccbi");
		stoneSprite = ret.first;
		//KS::setBlendFunc(stoneSprite, ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		//CCSprite* stone = ret.first;
		addChild(stoneSprite, 1);
	}
	else
	{
		
	}
	
	int randomX = rand()%321;
	IntPoint jackPoint = myGD->getJackPoint();
	
	stoneSprite->setPosition(ccp(randomX, ip2ccp(jackPoint).y + 300));
	
	// 운석 떨어질때
	addChild(KSTimer::create(0.4f, [=](){
		AudioEngine::sharedInstance()->playEffect("se_meteor.mp3");
	}));
	
	startMove();
}
Targeting * Targeting::create ()
{
	Targeting* t_t = new Targeting();
	if(t_t && t_t->initWithFile("targeting.png"))
	{
		t_t->myInit();
		t_t->autorelease();
		return t_t;
	}
	CC_SAFE_DELETE(t_t);
	return NULL;
}
void Targeting::remove ()
{
	removeFromParentAndCleanup(true);
}
void Targeting::startMyRotate ()
{
	schedule(schedule_selector(Targeting::myRotate));
}
void Targeting::myRotate ()
{
	setRotation(getRotation() + rotate_value);
}
void Targeting::myInit ()
{
	if(rand()%2)			rotate_value = 5;
	else					rotate_value = -5;
	
	startMyRotate();
}
TG_Shoot * TG_Shoot::create (int t_type, CCPoint t_sp, CCPoint t_tp, int t_loadFrame, int t_shootFrame, Targeting * t_targeting, CCSize t_mSize, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	TG_Shoot* t_tgs = new TG_Shoot();
	t_tgs->myInit(t_type, t_sp, t_tp, t_loadFrame, t_shootFrame, t_targeting, t_mSize, t_removeEffect, d_removeEffect);
	t_tgs->autorelease();
	return t_tgs;
}
void TG_Shoot::startMove ()
{
	ingFrame = 0;
	schedule(schedule_selector(TG_Shoot::move));
}
void TG_Shoot::stopMove ()
{
	unschedule(schedule_selector(TG_Shoot::move));
	removeFromParentAndCleanup(true);
}
void TG_Shoot::move ()
{
	ingFrame++;
	
	if(ingFrame < loadFrame)
	{
		loadImg->setPosition(ccpAdd(loadImg->getPosition(),load_dv));
		if(loadImg->getScaleX() < loadMaxScale)
			loadImg->setScaleX(loadImg->getScaleX() + loadScaleRate);
	}
	else if(ingFrame == loadFrame)
	{
		loadImg->setPosition(ccpAdd(loadImg->getPosition(),load_dv));
		if(loadImg->getScaleX() < loadMaxScale)
			loadImg->setScaleX(loadImg->getScaleX() + loadScaleRate);
		loadRemoving = true;
		
		CCPoint subPosition = ccpSub(tp, loadImg->getPosition());
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		float shootAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		
		shoot_dv.x = 1;
		shoot_dv.y = tanf(shootAngle/180.f*M_PI);
		
		float div_value = sqrtf(powf(shoot_dv.x, 2.f) + powf(shoot_dv.y, 2.f));
		shoot_dv = ccpMult(shoot_dv, 1.f/div_value);
		shoot_dv = ccpMult(shoot_dv, distance/shootFrame); // img width 4
		
		if((shootAngle > 90.f && shootAngle < 270.f) || shootAngle < -90.f)
			shoot_dv = ccpMult(shoot_dv, -1.f);
		
		shootImg = CCSprite::create(CCString::createWithFormat("lazerMissile%d.png", type)->getCString());
		shootImg->setAnchorPoint(ccp(1,0.5));
		shootImg->setRotation(-shootAngle);
		shootImg->setPosition(loadImg->getPosition());
		
		addChild(shootImg);
	}
	else if(ingFrame < loadFrame + shootFrame)
	{
		shootImg->setPosition(ccpAdd(shootImg->getPosition(),shoot_dv));
		if(shootImg->getScaleX() < shootMaxScale)
			shootImg->setScaleX(shootImg->getScaleX() + shootScaleRate);
	}
	else if(ingFrame == loadFrame + shootFrame)
	{
		shootImg->setPosition(ccpAdd(shootImg->getPosition(),shoot_dv));
		if(shootImg->getScaleX() < shootMaxScale)
			shootImg->setScaleX(shootImg->getScaleX() + shootScaleRate);
		shootRemoving = true;
		
		CCSprite* t_texture = CCSprite::create("tg_bomb.png");
		
		bombImg = CCSprite::createWithTexture(t_texture->getTexture(), CCRectMake(rand()%3*97, 0, 97, 60));
		bombImg->setPosition(shootImg->getPosition());
		
		addChild(bombImg);
		
		remove_targeting->remove();
		
		AudioEngine::sharedInstance()->playEffect("sound_TGshoot.mp3", false);
	}
	else if(ingFrame < loadFrame + shootFrame + bombFrame)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		CCPoint bombPosition = bombImg->getPosition();
		CCRect checkRect = CCRectMake(bombPosition.x - mSize.width/2.f, bombPosition.y - mSize.height/2.f, mSize.width, mSize.height);
		
		if(checkRect.containsPoint(jackPosition))
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			(target_removeEffect->*delegate_removeEffect)();
		}
	}
	else if(ingFrame == loadFrame + shootFrame + bombFrame)
	{
		bombImg->removeFromParentAndCleanup(true);
	}
	
	if(loadRemoving)
	{
		if(loadImg->getScaleX() > loadScaleRate)
			loadImg->setScaleX(loadImg->getScaleX() - loadScaleRate);
		else
		{
			loadImg->removeFromParentAndCleanup(true);
			loadRemoving = false;
		}
	}
	
	if(shootRemoving)
	{
		if(shootImg->getScaleX() > shootScaleRate)
			shootImg->setScaleX(shootImg->getScaleX() - shootScaleRate);
		else
		{
			shootImg->removeFromParentAndCleanup(true);
			shootRemoving = false;
		}
	}
	
	
	if(ingFrame >= loadFrame + shootFrame + bombFrame && !loadRemoving && !shootRemoving)
	{
		stopMove();
		removeFromParentAndCleanup(true);
	}
}
void TG_Shoot::myInit (int t_type, CCPoint t_sp, CCPoint t_tp, int t_loadFrame, int t_shootFrame, Targeting * t_targeting, CCSize t_mSize, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect)
{
	target_removeEffect = t_removeEffect;
	delegate_removeEffect = d_removeEffect;
	
	remove_targeting = t_targeting;
	mSize = t_mSize;
	type = t_type;
	sp = t_sp;
	tp = t_tp;
	loadFrame = t_loadFrame;
	loadMaxScale = 10.f;
	loadScaleRate = 2.f; // img size 4 x 4
	loadRemoving = false;
	
	shootFrame = t_shootFrame;
	shootMaxScale = 30.f;
	shootScaleRate = 4.f;
	shootRemoving = false;
	
	bombFrame = 10;
	
	CCPoint subPosition = ccpSub(tp, sp);
	float loadAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
	
	float rate = (rand()%11 + 5)/10.f;
	
	if(rand()%2)
		loadAngle += 90.f + 30.f*rate;
	else
		loadAngle -= 90.f + 30.f*rate;
	
	load_dv.x = 1;
	load_dv.y = tanf(loadAngle/180.f*M_PI);
	
	float div_value = sqrtf(powf(load_dv.x, 2.f) + powf(load_dv.y, 2.f));
	load_dv = ccpMult(load_dv, 1.f/div_value);
	load_dv = ccpMult(load_dv, 4.f); // img width 4
	
	if((loadAngle > 90.f && loadAngle < 270.f) || loadAngle < -90.f)
		load_dv = ccpMult(load_dv, -1.f);
	
	loadImg = CCSprite::create(CCString::createWithFormat("lazerMissile%d.png", type)->getCString());
	loadImg->setAnchorPoint(ccp(1,0.5));
	loadImg->setRotation(-loadAngle);
	loadImg->setPosition(sp);
	addChild(loadImg);
	
	startMove();
}
DeathSwing * DeathSwing::create (CCPoint t_sp, int t_rotate)
{
	DeathSwing* t_ds = new DeathSwing();
	t_ds->myInit(t_sp, t_rotate);
	t_ds->autorelease();
	return t_ds;
}
void DeathSwing::startAnimation ()
{
	CCSprite* t_spr = CCSprite::create("death_swing_effect.png");
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1);
	for(int i=0;i<4;i++)
		t_animation->addSpriteFrameWithTexture(t_spr->getTexture(), CCRectMake(i*37, 0, 37, 100));
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(DeathSwing::selfRemove));
	
	CCSequence* sequ = CCSequence::createWithTwoActions(t_animate, t_call);
	
	swingImg->runAction(sequ);
}
void DeathSwing::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void DeathSwing::myInit (CCPoint t_sp, int t_rotate)
{
	swingImg = CCSprite::create("death_swing_effect.png", CCRectMake(0, 0, 37, 100));
	swingImg->setPosition(t_sp);
	swingImg->setRotation(t_rotate);
	addChild(swingImg);
}
bool CrashMapObject::crashMapForIntPoint (IntPoint t_p)
{
	IntPoint jackPoint = myGD->getJackPoint();
	bool crash = false;
	if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline ||
													myGD->mapState[t_p.x][t_p.y] == mapOldget)) // just moment, only map crash
	{
		myGD->mapState[t_p.x][t_p.y] = mapEmpty;
		crash = true;
		for(int k = -1;k<=1;k++)
		{
			for(int l = -1;l<=1;l++)
			{
				if(k == 0 && l == 0)	continue;
				if(myGD->mapState[t_p.x+k][t_p.y+l] == mapOldget)		myGD->mapState[t_p.x+k][t_p.y+l] = mapOldline;
			}
		}
		//			myGD->communication("EP_crashed");
		myGD->communication("MFP_createNewFragment", t_p);
		myGD->communication("VS_divideRect", t_p);
	}
	
	if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
	{
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		jackDie();
	}
	
	if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
	{
		//					myGD->communication("PM_pathChainBomb", t_p);
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		lineDie(t_p);
	}
	
	return crash;
}
int CrashMapObject::crashMapForIntRect (IntRect t_r)
{
	IntPoint jackPoint = myGD->getJackPoint();
	
	bool is_die = false;
	int crashCount = 0;
	for(int i=t_r.origin.x;i<t_r.origin.x+t_r.size.width;i++)
	{
		for(int j=t_r.origin.y;j<t_r.origin.y+t_r.size.height;j++)
		{
			IntPoint t_p = IntPoint(i,j);
			if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline || myGD->mapState[t_p.x][t_p.y] == mapOldget))
			{
				myGD->mapState[t_p.x][t_p.y] = mapEmpty;
				myGD->communication("MFP_createNewFragment", t_p);
				crashCount++;
			}
			
			if(!is_die && t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
			{
				is_die = true;
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				lineDie(t_p);
			}
		}
	}
	
	for(int i=t_r.origin.x-1;i<=t_r.origin.x+t_r.size.width;i++)
	{
		if(IntPoint(i, t_r.origin.y+t_r.size.height).isInnerMap() && myGD->mapState[i][t_r.origin.y+t_r.size.height] == mapOldget)		myGD->mapState[i][t_r.origin.y+t_r.size.height] = mapOldline;
		if(IntPoint(i, t_r.origin.y-1).isInnerMap() && myGD->mapState[i][t_r.origin.y-1] == mapOldget)									myGD->mapState[i][t_r.origin.y-1] = mapOldline;
	}
	for(int j=t_r.origin.y;j<t_r.origin.y+t_r.size.height;j++)
	{
		if(IntPoint(t_r.origin.x-1, j).isInnerMap() && myGD->mapState[t_r.origin.x-1][j] == mapOldget)									myGD->mapState[t_r.origin.x-1][j] = mapOldline;
		if(IntPoint(t_r.origin.x+t_r.size.width, j).isInnerMap() && myGD->mapState[t_r.origin.x+t_r.size.width][j] == mapOldget)		myGD->mapState[t_r.origin.x+t_r.size.width][j] = mapOldline;
	}
	
	myGD->communication("VS_divideRects", t_r);
	
	if(!is_die && jackPoint.x >= t_r.origin.x && jackPoint.x < t_r.origin.x+t_r.size.width && jackPoint.y >= t_r.origin.y && jackPoint.y < t_r.origin.y+t_r.size.height)
	{
		is_die = true;
		myGD->communication("CP_jackCrashDie");
		myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		jackDie();
	}
	
	return crashCount;
}

SB_FallStar * SB_FallStar::create (int t_type)
{
	SB_FallStar* t_fs = new SB_FallStar();
	t_fs->myInit(t_type);
	t_fs->autorelease();
	return t_fs;
}
void SB_FallStar::startFalling ()
{
	schedule(schedule_selector(SB_FallStar::falling));
}
void SB_FallStar::falling ()
{
	myStar->setRotation(myStar->getRotation() + dr);
	float ap_y = getPositionY() - vy;
	vy += dy;
	setPositionY(ap_y);
	
	if(ap_y < 0)
	{
		removeFromParentAndCleanup(true);
	}
}
void SB_FallStar::myInit (int t_type)
{
	myStar = CCSprite::create(CCString::createWithFormat("satelliteBeam_star%d.png", t_type)->getCString());
	addChild(myStar);
	dr = rand()%5 - 2;
	vy = rand()%3 + 2.f;
	dy = 0.098f;
	int random_x = rand()%51 - 25;
	setPosition(ccp(random_x, 450));
}


MathmaticalMissileUnit * MathmaticalMissileUnit::create (CCPoint t_sp, float t_angle, float t_speed, string imgFilename, CCSize t_cs, vector <CCPoint> const & path, enum CurveDisposition curve, bool isSuper)
{
	MathmaticalMissileUnit* t_mu = new MathmaticalMissileUnit();
	if(t_mu && t_mu->initWithFile(imgFilename.c_str()))
	{
		t_mu->myInit(t_sp, t_angle, t_speed, t_cs, path, curve, imgFilename, isSuper);
		t_mu->autorelease();
		return t_mu;
	}
	CC_SAFE_DELETE(t_mu);
	return NULL;
}
MathmaticalMissileUnit * MathmaticalMissileUnit::create (CCPoint t_sp, float t_angle, float t_speed, string imgFilename, CCSize t_cs, bool isSuper)
{
	MathmaticalMissileUnit* t_mu = new MathmaticalMissileUnit();
	if(t_mu && t_mu->initWithFile(imgFilename.c_str()))
	{
		t_mu->myInit(t_sp, t_angle, t_speed, t_cs, std::vector<CCPoint>(), CurveDisposition::RIGHTLINE, "", isSuper);
		t_mu->autorelease();
		return t_mu;
	}
	CC_SAFE_DELETE(t_mu);
	return NULL;
}
CCPoint MathmaticalMissileUnit::CatMull (CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, float t, CurveDisposition curve)
{
	
	if(curve == CurveDisposition::CURVE)
	{
		float x = CatMullFunction(2 * P1.x, (-P0.x + P2.x), (2*P0.x - 5*P1.x + 4*P2.x - P3.x), (-P0.x + 3*P1.x- 3*P2.x + P3.x), t, curve);
		float y = CatMullFunction(2 * P1.y, (-P0.y + P2.y), (2*P0.y - 5*P1.y + 4*P2.y - P3.y), (-P0.y + 3*P1.y- 3*P2.y + P3.y), t, curve);
		
		return ccp(x, y);
	}
	else if(curve == CurveDisposition::RIGHTLINE)
	{
		CCPoint ret = P1 + (P2 - P1)*t;
		return ret;
	}
	
	return ccp(0, 0);
}
double MathmaticalMissileUnit::CatMullLength (CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, CurveDisposition curve)
{
	if(curve == CurveDisposition::CURVE)
	{
		return smps(bind(&ThisClassType::IntegralTarget, this, _1, _2, _3, _4, _5),
								P0, P1, P2, P3, 0.0, 1.0, 100);
	}
	else if(curve == CurveDisposition::RIGHTLINE)
	{
		return ccpLength(P1 - P2);
	}
	
	return -1;
}
double MathmaticalMissileUnit::IntegralTarget (CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, double t)
{
	double dx = CatMullDerivativeFunction(2 * P1.x, (-P0.x + P2.x), (2*P0.x - 5*P1.x + 4*P2.x - P3.x), (-P0.x + 3*P1.x- 3*P2.x + P3.x), t);
	double dy = CatMullDerivativeFunction(2 * P1.y, (-P0.y + P2.y), (2*P0.y - 5*P1.y + 4*P2.y - P3.y), (-P0.y + 3*P1.y- 3*P2.y + P3.y), t);
	
	double dx2 = dx * dx;
	double dy2 = dy * dy;
	return sqrt(dx2 + dy2);
}
double MathmaticalMissileUnit::CatMullFunction (double a, double b, double c, double d, double t, CurveDisposition curve)
{
	// a + bt + ct2 + dt3;
	return 0.5 * (a + b*t + c*t*t + d*t*t*t);
}
double MathmaticalMissileUnit::CatMullDerivativeFunction (double a, double b, double c, double d, double t)
{
	return 0.5 * (b + 2*c*t + 3*d*t*t);
}
double MathmaticalMissileUnit::smps (std::function <double(CCPoint, CCPoint, CCPoint, CCPoint, double)> f, CCPoint A, CCPoint B, CCPoint C, CCPoint D, double a, double b, int n)
{
	double h = (b-a)/n, sum = 0;
	for(int i=0; i<n; i++)
	{
		sum += (f(A,B,C,D,a+i*h)+4*f(A,B,C,D,a+i*h + h/2) + f(A,B,C,D,a+(i+1)*h)) * h / 6;
	}
	return sum;
}
void MathmaticalMissileUnit::removeEffect ()
{
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ThisClassType::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	runAction(t_seq);
}
void MathmaticalMissileUnit::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void MathmaticalMissileUnit::myInit (CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, vector <CCPoint> const & path, enum CurveDisposition curve, std::string const & fn,
																		 bool isSuper)
{
	m_frameCount = 0;
	m_isChecking = true;
	m_angle = t_angle;
	m_speed = t_distance;
	m_crashSize = t_cs;
	firePosition = t_sp;
	m_isSuper = isSuper;
	m_fileName = fn;
	m_catmullIndex = 0;
	m_catmullvar = 0.0;
	
	//		m_catmullPath = {ccp(-50, -50), ccp(0,0), ccp(50, 50), ccp(100, 0), ccp(150, 50),
	//			ccp(200, 0), ccp(250, 50), ccp(300, 0), ccp(350, 50), ccp(400, 50),
	//			ccp( 450, 0), ccp(500, 50)};
	m_catmullPath = path;
	m_curve = curve;
	m_fileName = fn;
	//		m_catmullPath = {ccp(-25, 0), ccp(0,0), ccp(25, 0), ccp(50, 0),
	//			ccp(75, 0), ccp(100, 0), ccp(125, 0), ccp(150, 0), ccp(175, 0),
	//			ccp(200, 0), ccp(250, 0)};
	setPosition(t_sp);
	
	//	setOpacity(0);
	//	auto missile = KS::loadCCBI<CCSprite*>(this, "ingame_missile.ccbi").first;
	//	missile->setPosition(ccp(getContentSize().width / 2.f, getContentSize().height / 2.f));
	////	missile->setAnchorPoint(ccp(0, 0));
	//	addChild(missile);
	
	
	schedule(schedule_selector(ThisClassType::move));
}
CCPoint MathmaticalMissileUnit::myFunction (float fc)
{
	// 120 프레임에 한 싸이클...
	return ccp(fc, sinf(fc * 2*M_PI/60) * 30); // 120 이 주기이고 10이 높이.
	//		return ccp(fc, 0);
}
void MathmaticalMissileUnit::move (float dt)
{
	setRotation(getRotation()-2);
	CCPoint beforePosition = getPosition() + getParent()->getPosition();
	CCPoint r_p = getPosition(); // recent
	
	CCPoint dv = ccp(0, 0);
	float len = 0;
	if(m_catmullIndex + 4 < m_catmullPath.size())
	{
		dv = CatMull(m_catmullPath[m_catmullIndex], m_catmullPath[m_catmullIndex+1],
								 m_catmullPath[m_catmullIndex+2], m_catmullPath[m_catmullIndex+3],
								 MIN(1.0, m_catmullvar), m_curve);
		len = CatMullLength(m_catmullPath[m_catmullIndex], m_catmullPath[m_catmullIndex+1],
												m_catmullPath[m_catmullIndex+2], m_catmullPath[m_catmullIndex+3],
												m_curve);
		
	}
	
	
	//		CCLOG("dis %f - %d, len = %f, u %f", ccpLength(prevPosition - dv), m_catmullIndex, len, m_catmullvar);
	
	m_catmullvar += m_speed / len;
	if(m_catmullvar > 1.0)
	{
		if(m_catmullIndex + 4 < m_catmullPath.size())
		{
			m_catmullvar = 0.0;
			m_catmullIndex++;
		}
		else
		{
			unschedule(schedule_selector(ThisClassType::move));
			removeFromParentAndCleanup(true);
			return;
		}
	}
	
	float angleRad = deg2Rad(m_angle);
	dv = ccp(dv.x * cosf(angleRad) - dv.y * sinf(angleRad), dv.x * sinf(angleRad) + dv.y * cosf(angleRad));
	
	//		if(ccpLength(dv) != 0)
	//			dv = dv / ccpLength(dv);
	//		dv = dv * m_speed;
	//		CCLOG("Dv = %f %f", dv.x, dv.y);
	r_p = firePosition + dv;
	CCPoint p_p = getParent()->getPosition() + r_p; // parent
	
	if(p_p.x < 0.f - 40.f || p_p.x > 320.f + 40.f || p_p.y < -60.f - 40.f || p_p.y > 490.f + 40.f) // fixed 40.f
	{
		unschedule(schedule_selector(ThisClassType::move));
		removeFromParentAndCleanup(true);
		return;
	}
	
	//		IntPoint p_pPoint = ccp2ip(p_p);
	//		IntPoint p_pBeforePoint = ccp2ip(beforePosition);
	if(m_isChecking)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		CCRect missile_rect = CCRectMake(p_p.x - m_crashSize.width/2.f, p_p.y - m_crashSize.height/2.f,
																		 m_crashSize.width, m_crashSize.height);
		
		if(missile_rect.containsPoint(jackPosition)) //  && myGD->getJackState()
		{
			m_isChecking = false;
			//				if(mySGD->getIsHard() || myGD->getJackState())
			if(m_isSuper || (myGD->getJackState() && !myGD->getCommunicationBool("PM_isShortLine")))
			{
				myGD->communication("CP_jackCrashDie");
				myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
				unschedule(schedule_selector(ThisClassType::move));
				removeEffect();
			}
			else
			{
				myGD->communication("Jack_showMB");
			}
		}
		else
		{
			float angle = atan2(p_p.y - beforePosition.y, p_p.x - beforePosition.x);
			int loop = ccpLength(p_p - beforePosition) / 1.414f;
			CCPoint t2 = beforePosition;
			for(int i=0; i<loop; i++)
			{
				t2.x += 1.414f * cos(angle);
				t2.y += 1.414f * sin(angle);
				
				IntPoint t = ccp2ip(t2);
				CCLOG("%d %d", t.x, t.y);
				if(t.isInnerMap() && myGD->mapState[t.x][t.y] == mapType::mapNewline)
				{
					if(!myGD->getCommunicationBool("PM_isShortLine"))
						myGD->communication("PM_addPathBreaking", t);
				}
				
			}
			IntPoint t = ccp2ip(p_p);
			if(t.isInnerMap() && myGD->mapState[t.x][t.y] == mapType::mapNewline)
			{
				if(!myGD->getCommunicationBool("PM_isShortLine"))
					myGD->communication("PM_addPathBreaking", t);
			}
		}
		//			//##
		//			else if(myGD->mapState[p_pPoint.x][p_pPoint.y] == mapType::mapNewline)
		//			{
		//				myGD->communication("PM_addPathBreaking", p_pPoint);
		//			}
		
	}
	
	setPosition(r_p);
	m_frameCount++;
	
	
}

