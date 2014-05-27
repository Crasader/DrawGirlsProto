//
//  MissileUnitCCBCCB.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 5. 22..
//
//

#include "MissileUnitCCB.h"


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

#define LZZ_INLINE inline

using namespace std;
using namespace placeholders;
static ProbSelector randomObj = {9, 1};
MissileUnitCCB * MissileUnitCCB::create (CCPoint t_sp, float t_angle, float t_distance, string imgFilename, CCSize t_cs, float t_da, float t_reduce_da)
{
	MissileUnitCCB* t_mu = new MissileUnitCCB();
	t_mu->init();
	t_mu->myInit(t_sp, t_angle, t_distance, imgFilename, t_cs, t_da, t_reduce_da);
	t_mu->autorelease();
	
	return t_mu;
}
void MissileUnitCCB::startMove ()
{
	schedule(schedule_selector(MissileUnitCCB::move));
		
}
void MissileUnitCCB::stopMove ()
{
	unschedule(schedule_selector(MissileUnitCCB::move));
}
void MissileUnitCCB::removeEffect ()
{
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileUnitCCB::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	
	runAction(t_seq);
}
void MissileUnitCCB::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void MissileUnitCCB::move ()
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
		//##
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
	
	da *= reduce_da;
}
void MissileUnitCCB::myInit (CCPoint t_sp, float t_angle, float t_distance, const std::string& ccbi, CCSize t_cs, float t_da, float t_reduce_da)
{
	addChild(KS::loadCCBI<CCSprite*>(this, ccbi.c_str()).first);
	is_checking = true;
	
	crashSize = t_cs;
	angle = t_angle;
	distance = t_distance;
	da = t_da;
	reduce_da = t_reduce_da;
//	setOpacity(0);
//	auto missile = KS::loadCCBI<CCSprite*>(this, "ingame_missile.ccbi").first;
//	missile->setPosition(ccp(getContentSize().width / 2.f, getContentSize().height / 2.f));
//	missile->setAnchorPoint(ccp(0, 0));
//	addChild(missile);
	setPosition(t_sp);
	startMove();
}