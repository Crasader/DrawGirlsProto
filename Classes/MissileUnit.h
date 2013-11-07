//
//  MissileUnit.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 21..
//
//

#ifndef DrawingJack_MissileUnit_h
#define DrawingJack_MissileUnit_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include <map>
#include "StarGoldData.h"
#include "cocos-ext.h"
#include "FromTo.h"
#include "KSUtil.h"
#include "Well512.h"
#include "ProbSelector.h"
#include <unordered_map>
#include <queue>
#include <functional>
#include <set>

USING_NS_CC_EXT;
using namespace cocos2d;
using namespace std;
using namespace placeholders;
class MissileUnit : public CCSprite
{
public:
	static MissileUnit* create(CCPoint t_sp, float t_angle, float t_distance, string imgFilename, CCSize t_cs, float t_da, float t_reduce_da)
	{
		MissileUnit* t_mu = new MissileUnit();
		if(t_mu && t_mu->initWithFile(imgFilename.c_str()))
		{
			t_mu->myInit(t_sp, t_angle, t_distance, t_cs, t_da, t_reduce_da);
			t_mu->autorelease();
			return t_mu;
		}
		CC_SAFE_DELETE(t_mu);
		return NULL;
	}
	
private:
	
	float angle;
	float distance;
	CCSize crashSize;
	float da;
	float reduce_da;
	float is_checking;
	
	void startMove()
	{
		schedule(schedule_selector(MissileUnit::move));
	}
	void stopMove()
	{
		unschedule(schedule_selector(MissileUnit::move));
	}
	
	void removeEffect()
	{
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileUnit::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void move()
	{
		setRotation(getRotation()-2);
		angle += da;
		if(angle >= 180.f)	angle -= 360.f;
		if(angle < -180.f)	angle += 360.f;
		
		
		CCPoint beforePosition = getPosition() + getParent()->getPosition();
//		CCLog("%x -- prev collision %f %f", this, beforePosition.x, beforePosition.y);
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
		
//		CCLog("%x -- collision %f %f", this, p_p.x, p_p.y);
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
				if(myGD->getJackState())
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
						myGD->communication("PM_addPathBreaking", t);
					}
				}
				IntPoint t = ccp2ip(p_p);
				if(t.isInnerMap() && myGD->mapState[t.x][t.y] == mapType::mapNewline)
				{
					myGD->communication("PM_addPathBreaking", t);
				}
			}
		}
		
		da *= reduce_da;
	}
	
	void myInit(CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, float t_da, float t_reduce_da)
	{
		is_checking = true;
		
		crashSize = t_cs;
		angle = t_angle;
		distance = t_distance;
		da = t_da;
		reduce_da = t_reduce_da;
		
		setPosition(t_sp);
		startMove();
	}
};

class MissileUnit2 : public CCSprite
{
public:
	static MissileUnit2* create(CCPoint t_sp, float t_angle, float t_distance, string imgFilename, CCSize t_cs, float t_da, float t_reduce_da)
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
	
private:
	
	float angle;
	float distance;
	CCSize crashSize;
	float da;
	float reduce_da;
	bool is_checking;
	
	void startMove()
	{
		schedule(schedule_selector(MissileUnit2::move));
	}
	void stopMove()
	{
		unschedule(schedule_selector(MissileUnit2::move));
	}
	
	void removeEffect()
	{
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileUnit2::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void move()
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
				if(myGD->getJackState())
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
	
	void myInit(CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, float t_da, float t_reduce_da)
	{
		is_checking = true;
		
		crashSize = t_cs;
		angle = t_angle;
		distance = t_distance;
		da = t_da;
		reduce_da = t_reduce_da;
		
		setPosition(t_sp);
		startMove();
	}
};

class MissileUnit3 : public CCNode
{
public:
	static MissileUnit3* create(int t_type, float t_distance, CCSize t_mSize, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		MissileUnit3* t_mu3 = new MissileUnit3();
		t_mu3->myInit(t_type, t_distance, t_mSize, t_removeEffect, d_removeEffect);
		t_mu3->autorelease();
		return t_mu3;
	}
	
private:
	int myType;
	float distance;
	CCSize mSize;
	
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	void startMove()
	{
		schedule(schedule_selector(MissileUnit3::move));
	}
	void stopMove()
	{
		unschedule(schedule_selector(MissileUnit3::move));
	}
	
	void removeEffect()
	{
		(target_removeEffect->*delegate_removeEffect)();
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileUnit3::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void move()
	{
		CCPoint afterPosition = ccp(getPositionX(), getPositionY()-distance);
		setPosition(afterPosition);
		
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
		
		if(afterPosition.y < -mSize.height-60.f)
		{
			stopMove();
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit(int t_type, float t_distance, CCSize t_mSize, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		target_removeEffect = t_removeEffect;
		delegate_removeEffect = d_removeEffect;
		
		myType = t_type;
		distance = t_distance;
		mSize = t_mSize;
		
		if(myType == 1) // stone
		{
//			initWithFile("fallingStone.png", CCRectMake(0, 0, 35, 35));
			
			auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_marble1.ccbi");
			CCSprite* stone = ret.first;
			addChild(stone);
		}
		else
		{
			
		}
		
		int randomX = rand()%321;
		setPosition(ccp(randomX, 480));
		
		startMove();
	}
};

class Targeting : public CCSprite
{
public:
	static Targeting* create()
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
	
	void remove()
	{
		removeFromParentAndCleanup(true);
	}
	
private:
	int rotate_value;
	
	void startMyRotate()
	{
		schedule(schedule_selector(Targeting::myRotate));
	}
	void myRotate()
	{
		setRotation(getRotation() + rotate_value);
	}
	
	void myInit()
	{
		if(rand()%2)			rotate_value = 5;
		else					rotate_value = -5;
		
		startMyRotate();
	}
};

class TG_Shoot : public CCNode
{
public:
	static TG_Shoot* create(int t_type, CCPoint t_sp, CCPoint t_tp, int t_loadFrame, int t_shootFrame, Targeting* t_targeting, CCSize t_mSize, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		TG_Shoot* t_tgs = new TG_Shoot();
		t_tgs->myInit(t_type, t_sp, t_tp, t_loadFrame, t_shootFrame, t_targeting, t_mSize, t_removeEffect, d_removeEffect);
		t_tgs->autorelease();
		return t_tgs;
	}
	
private:
	
	Targeting* remove_targeting;
	CCSize mSize;
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	int type;
	CCPoint sp;
	CCPoint tp;
	int loadFrame;
	float loadScaleRate;
	float loadMaxScale;
	CCPoint load_dv;
	int shootFrame;
	float shootScaleRate;
	float shootMaxScale;
	CCPoint shoot_dv;
	int bombFrame;
	int ingFrame;
	
	CCSprite* loadImg;
	bool loadRemoving;
	CCSprite* shootImg;
	bool shootRemoving;
	CCSprite* bombImg;
	
	void startMove()
	{
		ingFrame = 0;
		schedule(schedule_selector(TG_Shoot::move));
	}
	void stopMove()
	{
		unschedule(schedule_selector(TG_Shoot::move));
		removeFromParentAndCleanup(true);
	}
	void move()
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
	
	void myInit(int t_type, CCPoint t_sp, CCPoint t_tp, int t_loadFrame, int t_shootFrame, Targeting* t_targeting, CCSize t_mSize, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
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
};

class DeathSwing : public CCNode
{
public:
	static DeathSwing* create(CCPoint t_sp, int t_rotate)
	{
		DeathSwing* t_ds = new DeathSwing();
		t_ds->myInit(t_sp, t_rotate);
		t_ds->autorelease();
		return t_ds;
	}
	
	void startAnimation()
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
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
private:
	
	CCSprite* swingImg;
	
	void myInit(CCPoint t_sp, int t_rotate)
	{
		swingImg = CCSprite::create("death_swing_effect.png", CCRectMake(0, 0, 37, 100));
		swingImg->setPosition(t_sp);
		swingImg->setRotation(t_rotate);
		addChild(swingImg);
	}
};

class CrashMapObject : public CCNode
{
public:
	void crashMapForIntPoint(IntPoint t_p)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		
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
	}
	
	virtual void jackDie() = 0;
	virtual void lineDie(IntPoint t_p) = 0;
};

class WindmillObject : public CCSprite
{
public:
	static WindmillObject* create(IntPoint t_sp, int t_thornsFrame)
	{
		WindmillObject* t_to = new WindmillObject();
		t_to->myInit(t_sp, t_thornsFrame);
		t_to->autorelease();
		return t_to;
	}
	
	void startMyAction()
	{
		ingFrame = -30;
		
		CCRotateBy* t_rotate = CCRotateBy::create(1.f, 240);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
		
		runAction(t_repeat);
		
		schedule(schedule_selector(WindmillObject::myAction));
	}
	
private:
	
	int thornsFrame;
	int ingFrame;
	bool is_action;
	IntPoint myPoint;
	
	void myAction()
	{
		ingFrame++;
		
		int surroundCnt = 0;
		IntPoint checkPoint = IntPoint(myPoint.x-1, myPoint.y);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(myPoint.x+1, myPoint.y);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(myPoint.x, myPoint.y-1);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(myPoint.x, myPoint.y+1);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		
		if(surroundCnt == 0)
		{
			stopMyAction();
			return;
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, getPosition());
		
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		if(distance < 16*getScale())
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
		}
		
		if(!is_action)
		{
			if(ingFrame < 0)
				setScale((30+ingFrame)*0.033);
			else if(ingFrame == 0)
			{
				setScale((30+ingFrame)*0.033);
				is_action = true;
			}
			else if(ingFrame > 0)
			{
				setScale((30-ingFrame)*0.033);
				if(ingFrame >= 30)
				{
					stopMyAction();
					return;
				}
			}
		}
		else
		{
			if(ingFrame >= thornsFrame)
			{
				is_action = false;
				ingFrame = 0;
			}
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(WindmillObject::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(IntPoint t_sp, int t_thornsFrame)
	{
		CCSprite::init();
		myPoint = t_sp;
		is_action = false;
		thornsFrame = t_thornsFrame;
//		initWithFile("thorns_wall.png");
		
		auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_thornswall1.ccbi");
		addChild(ret.first);
		setScale(0.01f);
		setPosition(ccp((myPoint.x-1)*pixelSize+1,(myPoint.y-1)*pixelSize+1));
		
		startMyAction();
	}
};
class ThrowObject : public CrashMapObject
{
public:
	static ThrowObject* create(CCPoint t_sp, int t_type, float t_speed, float t_angle, IntSize t_mSize)
	{
		ThrowObject* t_to = new ThrowObject();
		t_to->myInit(t_sp, t_type, t_speed, t_angle, t_mSize);
		t_to->autorelease();
		return t_to;
	}
	
	void startMyAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
		b_c_p = getPosition();
		schedule(schedule_selector(ThrowObject::myAction));
	}
	
private:
	
	int type;
	int random_spin;
	IntSize mSize;
	CCPoint dv;
	CCSprite* objImg;
	CCPoint b_c_p;
	
	void jackDie()
	{
		unschedule(schedule_selector(ThrowObject::myAction));
		removeEffect();
	}
	
	void lineDie(IntPoint t_p)
	{
		unschedule(schedule_selector(ThrowObject::myAction));
		myGD->communication("Main_showLineDiePosition", t_p);
		removeEffect();
	}
	
	void removeEffect()
	{
//		objImg->unscheduleAllSelectors();
		objImg->stopAllActions();
		CCScaleTo* t_fade = CCScaleTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ThrowObject::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		objImg->runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(ThrowObject::myAction));
		removeFromParentAndCleanup(true);
	}
	void myAction()
	{
//		objImg->setRotation(objImg->getRotation() + random_spin);
		setPosition(ccpAdd(getPosition(), dv));
		
		CCPoint myPosition = getPosition();
		CCPoint subPosition = ccpSub(myPosition, b_c_p);
		subPosition.x = fabsf(subPosition.x);
		subPosition.y = fabsf(subPosition.y);
		
		if(subPosition.x > mSize.width || subPosition.y > mSize.height)
		{
			b_c_p = myPosition;
			IntPoint myPoint = IntPoint((myPosition.x-1)/pixelSize + 1 - mSize.width/2, (myPosition.y-1)/pixelSize + 1 - mSize.height/2);
			for(int i=0;i<mSize.height;i++)
			{
				for(int j=0;j<mSize.width;j++)
				{
					crashMapForIntPoint(IntPoint(myPoint.x+j,myPoint.y+i));
				}
			}
		}
		
		if(myPosition.x < -30 || myPosition.x > 350 || myPosition.y < -30 || myPosition.y > 460)
		{
			stopMyAction();
		}
	}
	
	void myInit(CCPoint t_sp, int t_type, float t_speed, float t_angle, IntSize t_mSize)
	{
		setPosition(t_sp);
		
		type = t_type;
//		random_spin = rand()%11 - 5;
		mSize = t_mSize;
		
//		type = 1;
		if(type == 11)
		{
//			objImg = CCSprite::create(CCString::createWithFormat("chapter1_throw_%d.png", randomValue)->getCString());
//			int randomValue = rand()%2 + 1;
//
			CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
			CCBReader* reader = new CCBReader(nodeLoader);
			objImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_saw1.ccbi",this));
//			m_headAnimationManager = reader->getAnimationManager();
//			this->addChild(m_headImg, 10);
			reader->release();
			addChild(objImg);
		}
		else if(type == 14)
		{
			CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
			CCBReader* reader = new CCBReader(nodeLoader);
			objImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_saw4.ccbi",this));
			reader->release();
			addChild(objImg);
		}
//		else if(type == 2) // ice
//		{
//			objImg = CCSprite::create("chapter2_throw_1.png");
//			addChild(objImg);
//		}
//		else if(type == 3) // wood
//		{
//			objImg = CCSprite::create("chapter2_throw_2.png");
//			addChild(objImg);
//		}
//		else if(type == 11)
//		{
//			objImg = CCSprite::create("chapter1_multiThrow_1.png");
//			addChild(objImg);
//		}
//		else if(type == 12)
//		{
//			objImg = CCSprite::create("chapter2_multiThrow_1.png");
//			addChild(objImg);
//		}
//		else if(type == 13)
//		{
//			objImg = CCSprite::create("chapter2_multiThrow_2.png");
//			addChild(objImg);
//		}
		
		dv.x = 1;
		dv.y = tanf(t_angle/180.f*M_PI);
		
		if((t_angle > 90 && t_angle < 270) || t_angle < -90)
			dv = ccpMult(dv, -1.f);
		
		float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
		dv = ccpMult(dv, 1.f/div_value);
		
		dv = ccpMult(dv, t_speed);
	}
};

class SB_FallStar : public CCNode
{
public:
	static SB_FallStar* create(int t_type)
	{
		SB_FallStar* t_fs = new SB_FallStar();
		t_fs->myInit(t_type);
		t_fs->autorelease();
		return t_fs;
	}
	
	void startFalling()
	{
		schedule(schedule_selector(SB_FallStar::falling));
	}
	
private:
	CCSprite* myStar;
	int dr;
	float vy;
	float dy;
	
	void falling()
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
	
	void myInit(int t_type)
	{
		myStar = CCSprite::create(CCString::createWithFormat("satelliteBeam_star%d.png", t_type)->getCString());
		addChild(myStar);
		dr = rand()%5 - 2;
		vy = rand()%3 + 2.f;
		dy = 0.098f;
		int random_x = rand()%51 - 25;
		setPosition(ccp(random_x, 450));
	}
};

class SatelliteBeam : public CrashMapObject
{
public:
	static SatelliteBeam* create(CCPoint t_sp, int t_type, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		SatelliteBeam* t_sb = new SatelliteBeam();
		t_sb->myInit(t_sp, t_type, t_removeEffect, d_removeEffect);
		t_sb->autorelease();
		return t_sb;
	}
	
	void beamSetPosition(CCPoint t_p)
	{
		setPosition(t_p);
		
		IntPoint beamPoint = IntPoint((t_p.x-1)/pixelSize+1, (t_p.y-1)/pixelSize+1); // center
		
		int crashSize = 3;
		for(int x=beamPoint.x-crashSize; x<=beamPoint.x+crashSize; x++)
		{
			for(int y=beamPoint.y-crashSize; y<=beamPoint.y+crashSize; y++)
			{
				crashMapForIntPoint(IntPoint(x, y));
			}
		}
		
//		for(int i=beamPoint.y-10;i<=beamPoint.y+10;i++)
//		{
//			int up_down_value;
//			if(i == beamPoint.y-10 || i == beamPoint.y+10)			up_down_value = 6;
//			else if(i == beamPoint.y-9 || i == beamPoint.y+9)		up_down_value = 11;
//			else if(i == beamPoint.y-8 || i == beamPoint.y+8)		up_down_value = 13;
//			else if(i == beamPoint.y-7 || i == beamPoint.y+7)		up_down_value = 15;
//			else if(i == beamPoint.y-6 || i == beamPoint.y+6)		up_down_value = 17;
//			else if(i == beamPoint.y-5 || i == beamPoint.y+5)		up_down_value = 18;
//			else if(i == beamPoint.y-4 || i == beamPoint.y+4 || i == beamPoint.y-3 || i == beamPoint.y+3)		up_down_value = 19;
//			else if(i >= beamPoint.y-2 && i <= beamPoint.y+2)		up_down_value = 20;
//			
//			for(int j=beamPoint.x-up_down_value;j<=beamPoint.x+up_down_value;j++)
//			{
//				crashMapForIntPoint(IntPoint(j,i));
//			}
//		}
	}
	
private:
	CCSprite* beam_main;
	FromToWithDuration<float> fadeFromToDuration;
//	CCSprite* beam_wave;
	int type;
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	void jackDie()
	{
		unschedule(schedule_selector(SatelliteBeam::fallingStar));
		(target_removeEffect->*delegate_removeEffect)();
	}
	void lineDie(IntPoint t_p)
	{
		unschedule(schedule_selector(SatelliteBeam::fallingStar));
		myGD->communication("Main_showLineDiePosition", t_p);
		(target_removeEffect->*delegate_removeEffect)();
	}
	

	
	void startFallingStar()
	{
		AudioEngine::sharedInstance()->playEffect("sound_satellitebeam.mp3", false);
		schedule(schedule_selector(SatelliteBeam::fallingStar));
	}
	
	void fallingStar()
	{
		if(rand()%2)
		{
			SB_FallStar* t_fs = SB_FallStar::create(type);
			addChild(t_fs);
			t_fs->startFalling();
		}
	}
	
	void myInit(CCPoint t_sp, int t_type, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		target_removeEffect = t_removeEffect;
		delegate_removeEffect = d_removeEffect;
		
		type = t_type;
		setPosition(t_sp);
		auto ret = KS::loadCCBI<CCSprite*>(this, "pattern_lightning.ccbi");
		
		beam_main = ret.first;
//		beam_main->setAnchorPoint(ccp(0.5,0.036));
		addChild(beam_main);
		{
			auto ret2 = KS::loadCCBI<CCSprite*>(this, "pattern_lightning_targeting.ccbi");
			beam_main->addChild(ret2.first, -1);
		}
//		beam_main->setVisible(false);

		startFallingStar();
	}
};

class FM_Targeting : public CCNode
{
public:
	static FM_Targeting* create(string imgFilename, CCPoint t_sp, int t_aniFrame, float t_sSize, float t_fSize, float t_sAngle, float t_fAngle)
	{
		FM_Targeting* t_fmt = new FM_Targeting();
		t_fmt->myInit(imgFilename, t_sp, t_aniFrame, t_sSize, t_fSize, t_sAngle, t_fAngle);
		t_fmt->autorelease();
		return t_fmt;
	}
	
	void startAction()
	{
		CCScaleTo* t_scaleAction = CCScaleTo::create(duration, fSize);
		
		CCRotateBy* t_rotateAction = CCRotateBy::create(duration, rotateValue);
		
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scaleAction, t_rotateAction);
		
		targetingImg->runAction(t_spawn);
	}
	
private:
	CCSprite* targetingImg;
	float duration;
	float fSize;
	float rotateValue;
	
	void myInit(string imgFilename, CCPoint t_sp, int t_aniFrame, float t_sSize, float t_fSize, float t_sAngle, float t_fAngle)
	{
		duration = t_aniFrame/60.f;
		fSize = t_fSize;
		rotateValue = t_fAngle - t_sAngle;
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		targetingImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_meteor3_targeting.ccbi",this));
		reader->release();
		targetingImg->setPosition(t_sp);
		
		targetingImg->setScale(t_sSize);
		targetingImg->setRotation(t_sAngle);
		
		addChild(targetingImg);
	}
};

class FallMeteor : public CrashMapObject
{
public:
	static FallMeteor* create(string t_imgFilename, int imgFrameCnt, CCSize imgFrameSize, CCPoint t_sp, CCPoint t_fp, int t_fallFrame, int t_explosionFrame, IntSize t_mSize, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		FallMeteor* t_fm = new FallMeteor();
		t_fm->myInit(t_imgFilename, imgFrameCnt, imgFrameSize, t_sp, t_fp, t_fallFrame, t_explosionFrame, t_mSize, t_removeEffect, d_removeEffect);
		t_fm->autorelease();
		return t_fm;
	}
	
	void removeEffect()
	{
		fadeFromToDuration.init(255, 0, 0.3f);
		schedule(schedule_selector(FallMeteor::hidingAnimation));
	}
	
	void hidingAnimation(float dt)
	{
		if(fadeFromToDuration.step(1.f/60.f) == false)
		{
			selfRemove();
		}
		else
		{
			KS::setOpacity(this, fadeFromToDuration.getValue());
		}
	}
private:
	FromToWithDuration<float> fadeFromToDuration;
	CCSprite* meteor;
	string imgFilename;

	FromToWithDuration2<CCPoint> meteorChanger;
	CCPoint fp;
	int fallFrame;
	int explosionFrame;
	int ingFrame;
	IntSize mSize;
	CCPoint fall_dv;
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	void jackDie()
	{
//		unschedule(schedule_selector(FallMeteor::fall));
//		(target_removeEffect->*delegate_removeEffect)();
		AudioEngine::sharedInstance()->playEffect("sound_meteor.mp3", false);
//		IntPoint rightUpPoint = IntPoint((meteor->getPositionX()-1)/pixelSize+1,(meteor->getPositionY()-1)/pixelSize+1); // right up
//		IntPoint leftDownPoint = IntPoint(rightUpPoint.x-mSize.width,rightUpPoint.y-mSize.height);		// left down point
//		
//		IntSize size = IntSize(rightUpPoint.x - leftDownPoint.x + 1, rightUpPoint.y - leftDownPoint.y + 1); // size
//		
//		for(int i=0;i<11;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x-1, leftDownPoint.y+2+i));
//		}
//		for(int i=0;i<7;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x-2, leftDownPoint.y+4+i));
//		}
//		crashMapForIntPoint(IntPoint(leftDownPoint.x-3, leftDownPoint.y+7));
//		
//		for(int i=0;i<11;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x+2+i, leftDownPoint.y-1));
//		}
//		for(int i=0;i<7;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x+4+i, leftDownPoint.y-2));
//		}
//		crashMapForIntPoint(IntPoint(leftDownPoint.x+7, leftDownPoint.y-3));
		
		stopFall();
//		removeEffect();
	}
	
	void lineDie(IntPoint t_p)
	{
		myGD->communication("Main_showLineDiePosition", t_p);
//		unschedule(schedule_selector(FallMeteor::fall));
		AudioEngine::sharedInstance()->playEffect("sound_meteor.mp3", false);
//		IntPoint rightUpPoint = IntPoint((meteor->getPositionX()-1)/pixelSize+1,(meteor->getPositionY()-1)/pixelSize+1); // right up
//		IntPoint leftDownPoint = IntPoint(rightUpPoint.x-mSize.width,rightUpPoint.y-mSize.height);		// left down point
//		
//		IntSize size = IntSize(rightUpPoint.x - leftDownPoint.x + 1, rightUpPoint.y - leftDownPoint.y + 1); // size
//		
//		for(int i=0;i<11;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x-1, leftDownPoint.y+2+i));
//		}
//		for(int i=0;i<7;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x-2, leftDownPoint.y+4+i));
//		}
//		crashMapForIntPoint(IntPoint(leftDownPoint.x-3, leftDownPoint.y+7));
//		
//		for(int i=0;i<11;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x+2+i, leftDownPoint.y-1));
//		}
//		for(int i=0;i<7;i++)
//		{
//			crashMapForIntPoint(IntPoint(leftDownPoint.x+4+i, leftDownPoint.y-2));
//		}
//		crashMapForIntPoint(IntPoint(leftDownPoint.x+7, leftDownPoint.y-3));
		
		stopFall();
//		(target_removeEffect->*delegate_removeEffect)();
//		removeEffect();
	}
	

	void stopFall()
	{
		unschedule(schedule_selector(FallMeteor::fall));
		initParticle();
		meteor->removeFromParentAndCleanup(true);
		
		IntPoint mapPoint = IntPoint(round((fp.x-1)/pixelSize+1),round((fp.y-1)/pixelSize+1));
		
		for(int i=-20;i<=20;i++)
		{
			int addValue;
			if(i == -20 || i == 20)									addValue = 4;
			else if(i == -19 || i == 19)							addValue = 8;
			else if(i == -18 || i == 18)							addValue = 9;
			else if(i == -17 || i == 17)							addValue = 11;
			else if(i == -16 || i == 16)							addValue = 12;
			else if(i == -15 || i == 15)							addValue = 14;
			else if(i == -14 || i == 14 || i == -13 || i == 13)		addValue = 15;
			else if(i == -12 || i == 12)							addValue = 16;
			else if(i == -11 || i == 11 || i == -10 || i == 10)		addValue = 17;
			else if(i == -9 || i == 9)								addValue = 18;
			else if((i >= -8 && i <= -5) || (i >= 5 && i <= 8))		addValue = 19;
			else													addValue = 20;
			for(int j=-addValue;j<=addValue;j++)
			{
				crashMapForIntPoint(IntPoint(mapPoint.x+i, mapPoint.y+j));
			}
		}
		
		CCDelayTime* t_delay = CCDelayTime::create(0.5);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(FallMeteor::selfRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call2);
		runAction(t_seq);
		
	}
	void fall()
	{
		ingFrame++;
		bool notFinish = meteorChanger.step(1/60.f);
		
//		meteor->setPosition(ccpAdd(meteor->getPosition(),fall_dv));
		meteor->setPosition(meteorChanger.getValue());
		
		IntPoint rightUpPoint = IntPoint((meteor->getPositionX()-1)/pixelSize+1,(meteor->getPositionY()-1)/pixelSize+1); // right up
		IntPoint leftDownPoint = IntPoint(rightUpPoint.x-mSize.width,rightUpPoint.y-mSize.height);		// left down point
		
		IntSize size = IntSize(rightUpPoint.x - leftDownPoint.x + 1, rightUpPoint.y - leftDownPoint.y + 1); // size
		
		for(int i = leftDownPoint.x;i<leftDownPoint.x+size.width;i++)
		{
			for(int j = leftDownPoint.y;j<leftDownPoint.y+size.height;j++)
			{
				crashMapForIntPoint(IntPoint(i, j));
			}
		}
		if(!notFinish)
		{
			AudioEngine::sharedInstance()->playEffect("sound_meteor.mp3", false);
			for(int i=0;i<11;i++)
			{
				crashMapForIntPoint(IntPoint(leftDownPoint.x-1, leftDownPoint.y+2+i));
			}
			for(int i=0;i<7;i++)
			{
				crashMapForIntPoint(IntPoint(leftDownPoint.x-2, leftDownPoint.y+4+i));
			}
			crashMapForIntPoint(IntPoint(leftDownPoint.x-3, leftDownPoint.y+7));
			
			for(int i=0;i<11;i++)
			{
				crashMapForIntPoint(IntPoint(leftDownPoint.x+2+i, leftDownPoint.y-1));
			}
			for(int i=0;i<7;i++)
			{
				crashMapForIntPoint(IntPoint(leftDownPoint.x+4+i, leftDownPoint.y-2));
			}
			crashMapForIntPoint(IntPoint(leftDownPoint.x+7, leftDownPoint.y-3));
			
			stopFall();
		}
		
//		if(ingFrame >= fallFrame)
//		{
//			for(int i=0;i<11;i++)
//			{
//				crashMapForIntPoint(IntPoint(leftDownPoint.x-1, leftDownPoint.y+2+i));
//			}
//			for(int i=0;i<7;i++)
//			{
//				crashMapForIntPoint(IntPoint(leftDownPoint.x-2, leftDownPoint.y+4+i));
//			}
//			crashMapForIntPoint(IntPoint(leftDownPoint.x-3, leftDownPoint.y+7));
//			
//			for(int i=0;i<11;i++)
//			{
//				crashMapForIntPoint(IntPoint(leftDownPoint.x+2+i, leftDownPoint.y-1));
//			}
//			for(int i=0;i<7;i++)
//			{
//				crashMapForIntPoint(IntPoint(leftDownPoint.x+4+i, leftDownPoint.y-2));
//			}
//			crashMapForIntPoint(IntPoint(leftDownPoint.x+7, leftDownPoint.y-3));
//			
//			stopFall();
//		}
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void initParticle()
	{
		auto ret = KS::loadCCBI<CCSprite*>(this, "fx_bomb1.ccbi");
		CCSprite* particle = ret.first;
		
		
		particle->setPosition(meteor->getPosition());
//		particle->setPosVar(CCPointZero);
		addChild(particle);
	}
	
	void myInit(string t_imgFilename, int imgFrameCnt, CCSize imgFrameSize, CCPoint t_sp, CCPoint t_fp, int t_fallFrame, int t_explosionFrame, IntSize t_mSize, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		target_removeEffect = t_removeEffect;
		delegate_removeEffect = d_removeEffect;
		
		imgFilename = t_imgFilename;
		fp = t_fp;
		fallFrame = t_fallFrame;
		explosionFrame = t_explosionFrame;
		mSize = t_mSize;

		meteorChanger.init(t_sp, fp, 0.75f);
//		fall_dv = ccpSub(fp, t_sp);
////		fall_dv = (fall_dv / ccpLength(fall_dv)) * 10.f;
//		fall_dv = ccpMult(fall_dv, 1.f/fallFrame);
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		meteor = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("pattern_meteor3.ccbi",this));
		meteor->setRotation(90 + 45);
		reader->release();
		
		
//		 = CCSprite::create(("meteor_stone_test" + imgFilename).c_str(), CCRectMake(0, 0, imgFrameSize.width, imgFrameSize.height));
		meteor->setPosition(t_sp);
		addChild(meteor);
		
		int random_sign;
		if(rand()%2)
			random_sign = 360;
		else
			random_sign = -360;
		
		FM_Targeting* t_fmt = FM_Targeting::create(imgFilename, t_fp, t_fallFrame, 3.0, 0.7, 0, random_sign);
		addChild(t_fmt);
		
		t_fmt->startAction();
		
		ingFrame = 0;
		
//		schedule(schedule_selector(FallMeteor::fall));
		schedule(schedule_selector(FallMeteor::fall), 0, kCCRepeatForever, fallFrame / 60.f);
	}
};

class Lazer_Ring : public CCNode
{
public:
	static Lazer_Ring* create(float t_ring_angle, CCPoint t_ring_sP, CCPoint t_ring_fP, float t_ring_sS, float t_ring_fS, int t_frame, ccColor3B t_color)
	{
		Lazer_Ring* t_lr = new Lazer_Ring();
		t_lr->myInit(t_ring_angle, t_ring_sP, t_ring_fP, t_ring_sS, t_ring_fS, t_frame, t_color);
		t_lr->autorelease();
		return t_lr;
	}
	
private:
	CCPoint dv;
	float ds;
	int frame;
	int ingFrame;
	CCSprite* ringImg;
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(Lazer_Ring::myAction));
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(Lazer_Ring::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myAction()
	{
		ingFrame++;
		
		ringImg->setPosition(ccpAdd(ringImg->getPosition(), dv));
		ringImg->setScale(ringImg->getScale()+ds);
		
		if(ingFrame >= frame)
		{
			stopMyAction();
		}
	}
	
	void myInit(float t_ring_angle, CCPoint t_ring_sP, CCPoint t_ring_fP, float t_ring_sS, float t_ring_fS, int t_frame, ccColor3B t_color)
	{
		frame = t_frame;
		ringImg = CCSprite::create("lazer_ring.png");
		ringImg->setPosition(t_ring_sP);
		ringImg->setScale(t_ring_sS);
		ringImg->setRotation(-t_ring_angle);
		
		ringImg->setOpacity(rand()%100+100);
		ringImg->setColor(t_color);
		
		addChild(ringImg);
		
		dv = ccpSub(t_ring_fP, t_ring_sP);
		dv = ccpMult(dv, 1.f/frame);
		
		ds = (t_ring_fS - t_ring_sS)/frame;
		
		startMyAction();
	}
};

class ThreeCushion : public CrashMapObject
{
public:
	static ThreeCushion* create(CCPoint t_sp, float t_speed, int t_cushion_cnt, bool t_is_big_bomb, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		ThreeCushion* t_tc = new ThreeCushion();
		t_tc->myInit(t_sp, t_speed, t_cushion_cnt, t_is_big_bomb, t_removeEffect, d_removeEffect);
		t_tc->autorelease();
		return t_tc;
	}
	
	void removeEffect()
	{
		unschedule(schedule_selector(ThreeCushion::myAction));
		CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(ThreeCushion::selfRemove));
		
		if(baseNode)
		{
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade1, t_call2);
			baseNode->runAction(t_seq);
		}
		
		if(colorControl)
		{
			CCFadeTo* t_fade2 = CCFadeTo::create(1.f, 0);
			colorControl->runAction(t_fade2);
		}
		
		if(cntLabel)
		{
			CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
			cntLabel->runAction(t_fade3);
		}
	}
	
private:
	
	float speed;
	int angle;
	CCSprite* baseNode;
	CCSprite* colorControl;
	CCLabelAtlas* cntLabel;
	int cushionCnt;
	bool is_big_bomb;
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	ccColor3B getCushionColor(int t_cushion_cnt)
	{
		if(t_cushion_cnt == 1)			return ccc3(227, 0, 0); // red
		else if(t_cushion_cnt == 2)		return ccc3(205, 70, 22); // orange
		else if(t_cushion_cnt == 3)		return ccc3(220, 100, 10); // light orange
		else if(t_cushion_cnt == 4)		return ccc3(80, 137, 50); // green
		else if(t_cushion_cnt == 5)		return ccc3(20, 142, 155); // blue green
		else if(t_cushion_cnt == 6)		return ccc3(14, 120, 215); // blue
		else							return ccc3(0, 0, 0); // black
	}
	
	void startMyAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
		schedule(schedule_selector(ThreeCushion::myAction));
	}
	void stopMyAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_threecusion_bomb.mp3", false);
		unschedule(schedule_selector(ThreeCushion::myAction));
		initParticle();
		crashMap();
		baseNode->removeFromParentAndCleanup(true);
		baseNode = NULL;
		colorControl = NULL;
		cntLabel = NULL;
		
		CCDelayTime* t_delay = CCDelayTime::create(0.5);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(ThreeCushion::selfRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call2);
		runAction(t_seq);
	}
	void myAction()
	{
		int side_cnt = 0;
		IntPoint check_point;
		IntVector t_v;
		
		IntPoint myPoint = IntPoint(round((baseNode->getPositionX()-1)/pixelSize+1),round((baseNode->getPositionY()-1)/pixelSize+1));
		
		t_v = IntVector::directionVector(directionLeft);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		t_v = IntVector::directionVector(directionRight);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		t_v = IntVector::directionVector(directionDown);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		t_v = IntVector::directionVector(directionUp);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		if(side_cnt == 4)
		{
			unschedule(schedule_selector(ThreeCushion::myAction));
			AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
			int missile_type = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_lastSelectedElement);
			
			int rmCnt = 2/2 + 1;
			float damage_per = 2*0.8f / rmCnt;
			myGD->communication("MP_createJackMissile", missile_type, rmCnt, damage_per);
			
			removeFromParentAndCleanup(true);
			return;
		}
		
		CCPoint after_position = getAfterPosition(baseNode->getPosition(), angle);
		IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
		if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
		{
			cushionCnt--;
			if(cushionCnt <= 0)
			{
				stopMyAction(); // bomb
				return;
			}
			colorControl->setColor(getCushionColor(cushionCnt));
			cntLabel->setString(CCString::createWithFormat("%d", cushionCnt)->getCString());
			
			after_position = judgeAnglePoint(baseNode->getPosition());
			if(after_position.x == -1 && after_position.y == -1)
			{
				return;
			}
		}
		
		baseNode->setPosition(after_position);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void jackDie()
	{
		(target_removeEffect->*delegate_removeEffect)();
	}
	void lineDie(IntPoint t_p)
	{
		myGD->communication("Main_showLineDiePosition", t_p);
		(target_removeEffect->*delegate_removeEffect)();
	}
	
	void crashMap()
	{
		IntPoint centerPoint = IntPoint(round((baseNode->getPositionX()-1)/pixelSize+1),round((baseNode->getPositionY()-1)/pixelSize+1));
		
		if(!is_big_bomb)
		{
			for(int i=-8;i<=8;i++)
			{
				int addValue;
				if(i == -8 || i == 8)									addValue = 3;
				else if(i == -7 || i == 7)								addValue = 5;
				else if(i == -6 || i == 6)								addValue = 6;
				else if(i == -5 || i == 5 || i == -4 || i == 4)			addValue = 7;
				else													addValue = 8;
				for(int j=-addValue;j<=addValue;j++)
				{
					crashMapForIntPoint(IntPoint(centerPoint.x+i, centerPoint.y+j));
				}
			}
		}
		else
		{
			for(int i=-14;i<=14;i++)
			{
				int addValue;
				if(i == -14 || i == 14)									addValue = 3;
				else if(i == -13 || i == 13)							addValue = 6;
				else if(i == -12 || i == 12)							addValue = 8;
				else if(i == -11 || i == 11)							addValue = 9;
				else if(i == -10 || i == 10)							addValue = 10;
				else if(i == -9 || i == 9)								addValue = 11;
				else if(i == -8 || i == 8 || i == -7 || i == 7)			addValue = 12;
				else if((i >= -6 && i <= -4) || (i >= 4 && i <= 6))		addValue = 13;
				else													addValue = 14;
				for(int j=-addValue;j<=addValue;j++)
				{
					crashMapForIntPoint(IntPoint(centerPoint.x+i, centerPoint.y+j));
				}
			}
		}
	}
	
	CCPoint getAfterPosition(CCPoint b_p, int t_angle)
	{
		CCPoint dv;
		dv.x = 1;
		dv.y = tanf(t_angle/180.f*M_PI);
		
		if((t_angle >= 90 && t_angle < 270) || (t_angle < -90 && t_angle >= -270))
		{
			dv = ccpMult(dv, -1.f);
		}
		
		float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
		
		dv = ccpMult(dv, speed/div_value);
		
		CCPoint after_position = ccpAdd(b_p, dv);
		return after_position;
	}
	
	CCPoint judgeAnglePoint(CCPoint b_p)
	{
		CCPoint returnPoint;
		int a_angle;
		
		if((angle >= -45 && angle < 0) || (angle >= 0 && angle < 45) || (angle >= 135 && angle < 180) || (angle >= -180 && angle < -135))
		{
			// 180 - x, -x, reverse
			a_angle = 180 - angle;
			if(a_angle < -180)		a_angle += 360;
			if(a_angle >= 180)		a_angle -= 360;
			
			CCPoint after_position = getAfterPosition(b_p, a_angle);
			IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
			if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
			{
				a_angle = -angle;
				if(a_angle < -180)		a_angle += 360;
				if(a_angle >= 180)		a_angle -= 360;
			}
			else
			{
				angle = a_angle;
				return after_position;
			}
			
			after_position = getAfterPosition(b_p, a_angle);
			after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
			if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
			{
				a_angle = angle + 180;
				if(a_angle < -180)		a_angle += 360;
				if(a_angle >= 180)		a_angle -= 360;
			}
			else
			{
				angle = a_angle;
				return after_position;
			}
			
			after_position = getAfterPosition(b_p, a_angle);
			angle = a_angle;
			return after_position;
		}
		else if((angle >= 45 && angle < 90) || (angle >= 90 && angle < 135) || (angle >= -135 && angle < -90) || (angle >= -90 && angle < -45))
		{
			// -x, 180 - x, reverse
			a_angle = -angle;
			if(a_angle < -180)		a_angle += 360;
			if(a_angle >= 180)		a_angle -= 360;
			
			CCPoint after_position = getAfterPosition(b_p, a_angle);
			IntPoint after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
			if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
			{
				a_angle = 180 - angle;
				if(a_angle < -180)		a_angle += 360;
				if(a_angle >= 180)		a_angle -= 360;
			}
			else
			{
				angle = a_angle;
				return after_position;
			}
			
			after_position = getAfterPosition(b_p, a_angle);
			after_point = IntPoint(round((after_position.x-1)/pixelSize+1),round((after_position.y-1)/pixelSize+1));
			if(!after_point.isInnerMap() || myGD->mapState[after_point.x][after_point.y] != mapEmpty)
			{
				a_angle = angle + 180;
				if(a_angle < -180)		a_angle += 360;
				if(a_angle >= 180)		a_angle -= 360;
			}
			else
			{
				angle = a_angle;
				return after_position;
			}
			
			after_position = getAfterPosition(b_p, a_angle);
			angle = a_angle;
			return after_position;
		}
		else
		{
			// ?!
		}
		stopMyAction();
		return ccp(-1,-1);
	}
	
	void myInit(CCPoint t_sp, float t_speed, int t_cushion_cnt, bool t_is_big_bomb, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		target_removeEffect = t_removeEffect;
		delegate_removeEffect = d_removeEffect;
		
		
		cushionCnt = t_cushion_cnt;
		speed = t_speed;
		angle = 0;
		is_big_bomb = t_is_big_bomb;
		while (angle%45 == 0)
		{
			angle = rand()%360 - 180;
		}
		
		baseNode = CCSprite::create("threeCushion_main.png");
		baseNode->setPosition(t_sp);
		addChild(baseNode);
		
		colorControl = CCSprite::create("threeCushion_color.png");
		colorControl->setPosition(ccp(12,12));
		colorControl->setColor(getCushionColor(cushionCnt));
		baseNode->addChild(colorControl);
		
		cntLabel = CCLabelAtlas::create(CCString::createWithFormat("%d", cushionCnt)->getCString(), "threeCushion_number.png", 13, 13, '1');
		cntLabel->setPosition(ccp(6,5));
		baseNode->addChild(cntLabel);
		
		startMyAction();
	}
	
	void initParticle()
	{
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		CCSprite* particle = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_bomb1.ccbi",this));
		reader->release();
		particle->setPosition(baseNode->getPosition());
		particle->setRotation(rand()%360);
		addChild(particle);
		
//		CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
//		particle->setPositionType(kCCPositionTypeRelative);
//		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("threeCushion_bomb.png");
//		particle->setTexture(texture);
//		particle->setEmissionRate(250.00);
//		particle->setAngle(90.0);
//		particle->setAngleVar(360.0);
//		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
//		particle->setBlendFunc(blendFunc);
//		particle->setDuration(0.20);
//		particle->setEmitterMode(kCCParticleModeGravity);
//		ccColor4F startColor = {0.87,0.81,0.12,1.00}; // 0.76 0.25 0.12
//		particle->setStartColor(startColor);
//		ccColor4F startColorVar = {0,0,0,0};
//		particle->setStartColorVar(startColorVar);
//		ccColor4F endColor = {0.68,0.16,0.00,1.00};
//		particle->setEndColor(endColor);
//		ccColor4F endColorVar = {0,0,0,0};
//		particle->setEndColorVar(endColorVar);
//		particle->setStartSize(20.00);
//		particle->setStartSizeVar(10.0);
//		particle->setEndSize(40.0);
//		particle->setEndSizeVar(10.0);
//		particle->setGravity(ccp(0,0));
//		particle->setRadialAccel(0.0);
//		particle->setRadialAccelVar(0.0);
//		particle->setSpeed(170);
//		particle->setSpeedVar(60.0);
//		particle->setTangentialAccel(0);
//		particle->setTangentialAccelVar(0);
//		particle->setTotalParticles(50);
//		particle->setLife(0.20);
//		particle->setLifeVar(0.0);
//		particle->setStartSpin(0.0);
//		particle->setStartSpinVar(0.0);
//		particle->setEndSpin(0.0);
//		particle->setEndSpinVar(0.0);
//		particle->setPosition(baseNode->getPosition());
//		particle->setPosVar(CCPointZero);
//		addChild(particle);
	}
};


class TickingTimeBomb : public CrashMapObject
{
public:
	static TickingTimeBomb* create(IntPoint t_setPoint, int t_bombFrameOneTime, int t_bombTimes, int t_rangeCode, CCArray* t_tickingArray, CCObject* t_resetTickingTimeBomb, SEL_CallFunc d_resetTickingTimeBomb)
	{
		TickingTimeBomb* t_ttb = new TickingTimeBomb();
		t_ttb->myInit(t_setPoint, t_bombFrameOneTime, t_bombTimes, t_rangeCode, t_tickingArray, t_resetTickingTimeBomb, d_resetTickingTimeBomb);
		t_ttb->autorelease();
		return t_ttb;
	}
	
	IntPoint getSettedPoint()
	{
		return setPoint;
	}
	
private:
	
	IntPoint setPoint;
	int bombFrameOneTime;
	int bombTimes;
	int ingFrame;
	int rangeCode;
	CCSprite* ticking_main;
	CCLabelAtlas* ticking_label;
	CCArray* tickingArray;
	
	CCObject* target_resetTickingTimeBomb;
	SEL_CallFunc delegate_resetTickingTimeBomb;
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(TickingTimeBomb::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame == bombFrameOneTime)
		{
			ingFrame = 0;
			bombTimes--;
			ticking_label->setString(CCString::createWithFormat("%d",bombTimes)->getCString());
		}
		
		int surroundCnt = 0;
		IntPoint checkPoint = IntPoint(setPoint.x-1, setPoint.y);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(setPoint.x+1, setPoint.y);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(setPoint.x, setPoint.y-1);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(setPoint.x, setPoint.y+1);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		
		if(surroundCnt == 0)
		{
			stopMyAction();
			selfRemove();
			return;
		}
		
		if(ingFrame == bombFrameOneTime-1 && bombTimes <= 1)
		{
			crashMap();
			ticking_main->removeFromParentAndCleanup(true);
			
			CCDelayTime* t_delay = CCDelayTime::create(0.5);
			CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(TickingTimeBomb::selfRemove));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call2);
			runAction(t_seq);
			stopMyAction();
		}
	}
	
	void crashMap()
	{
		initParticle();
		AudioEngine::sharedInstance()->playEffect("sound_tickingTimeBomb.mp3", false);
		if(rangeCode == 1)
		{
			for(int i=-40;i<=40;i++)
			{
				int addValue;
				if(i == -40 || i == -39 || i == 40 || i == 39)			addValue = 8;
				else if(i == -38 || i == -37 || i == 38 || i == 37)		addValue = 16;
				else if(i == -36 || i == -35 || i == 36 || i == 35)		addValue = 19;
				else if(i == -34 || i == -33 || i == 34 || i == 33)		addValue = 22;
				else if(i == -32 || i == -31 || i == 32 || i == 31)		addValue = 24;
				else if(i == -30 || i == -29 || i == 30 || i == 29)		addValue = 28;
				else if((i >= -28 && i <= -25) || (i <= 28 && i >= 25))	addValue = 30;
				else if(i == -24 || i == -23 || i == 24 || i == 23)		addValue = 32;
				else if((i >= -22 && i <= -19) || (i <= 22 && i >= 19))	addValue = 34;
				else if(i == -18 || i == -17 || i == 18 || i == 17)		addValue = 36;
				else if((i >= -16 && i <= -9) || (i >= 9 && i <= 16))	addValue = 38;
				else													addValue = 40;
				for(int j=-addValue;j<=addValue;j++)
				{
					crashMapForIntPoint(IntPoint(setPoint.x+i, setPoint.y+j));
				}
			}
		}
		else if(rangeCode == 2)
		{
			myGD->communication("SW_createJDSW", setPoint);
		}
	}
	
	void selfRemove()
	{
		tickingArray->removeObject(this);
		removeFromParentAndCleanup(true);
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(TickingTimeBomb::myAction));
	}
	
	void jackDie()
	{
		unschedule(schedule_selector(TickingTimeBomb::myAction));
		(target_resetTickingTimeBomb->*delegate_resetTickingTimeBomb)();
	}
	
	void lineDie(IntPoint t_p)
	{
		unschedule(schedule_selector(TickingTimeBomb::myAction));
		myGD->communication("Main_showLineDiePosition", t_p);
		(target_resetTickingTimeBomb->*delegate_resetTickingTimeBomb)();
	}
	
	void myInit(IntPoint t_setPoint, int t_bombFrameOneTime, int t_bombTimes, int t_rangeCode, CCArray* t_tickingArray, CCObject* t_resetTickingTimeBomb, SEL_CallFunc d_resetTickingTimeBomb)
	{
		target_resetTickingTimeBomb = t_resetTickingTimeBomb;
		delegate_resetTickingTimeBomb = d_resetTickingTimeBomb;
		
		tickingArray = t_tickingArray;
		setPoint = t_setPoint;
		bombFrameOneTime = t_bombFrameOneTime;
		bombTimes = t_bombTimes;
		rangeCode = t_rangeCode;
		
		CCPoint myPosition = ccp((setPoint.x-1)*pixelSize+1, (setPoint.y-1)*pixelSize+1);
		setPosition(myPosition);
		
		ticking_main = KS::loadCCBI<CCSprite*>(this, "pattern_timebomb1.ccbi").first;
//		ticking_main->setVisible(false);
		ticking_main->setAnchorPoint(ccp(0.5f, 0.5f));
		addChild(ticking_main);
		
		if(rangeCode == 1)
		{
//			CCSprite* ticking_pad = CCSprite::create("tickingTimeBomb_pad.png");
////			ticking_pad->setPosition(ccp(17,12));
//			ticking_main->addChild(ticking_pad);
			
			ticking_label = CCLabelAtlas::create(CCString::createWithFormat("%d",bombTimes)->getCString(), "tickingTimeBomb_number.png", 9, 11, '1');
			ticking_label->setAnchorPoint(ccp(0.5f, 0.5f));
			ticking_label->setPosition(ccp(0, 0));
			addChild(ticking_label);
		}
		else if(rangeCode == 2)
		{
			ticking_label = CCLabelAtlas::create(CCString::createWithFormat("%d",bombTimes)->getCString(), "tickingTimeBomb_number.png", 9, 11, '1');
			ticking_label->setPosition(ccp(13,13));
			ticking_main->addChild(ticking_label);
		}
		
		startMyAction();
	}
	
	void initParticle()
	{
		CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("tickingTime_bomb.png");
		particle->setTexture(texture);
		particle->setEmissionRate(250.00);
		particle->setAngle(90.0);
		particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(0.20);
		particle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor = {0.87,0.81,0.12,1.00}; // 0.76 0.25 0.12
		particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {0.68,0.16,0.00,1.00};
		particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		particle->setEndColorVar(endColorVar);
		particle->setStartSize(20.00);
		particle->setStartSizeVar(10.0);
		particle->setEndSize(40.0);
		particle->setEndSizeVar(10.0);
		particle->setGravity(ccp(0,0));
		particle->setRadialAccel(0.0);
		particle->setRadialAccelVar(0.0);
		particle->setSpeed(250);
		particle->setSpeedVar(60.0);
		particle->setTangentialAccel(0);
		particle->setTangentialAccelVar(0);
		particle->setTotalParticles(50);
		particle->setLife(0.30);
		particle->setLifeVar(0.0);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(0.0);
		particle->setEndSpin(0.0);
		particle->setEndSpinVar(0.0);
		particle->setPosition(CCPointZero);
		particle->setPosVar(CCPointZero);
		addChild(particle);
	}
};


class SightOut : public CCSprite
{
public:
	static SightOut* create()
	{
		SightOut* t_so = new SightOut();
		t_so->myInit();
		t_so->autorelease();
		return t_so;
	}
	
	void startAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(SightOut::myAction));
	}
	
private:
	int ingFrame;
	float opa_value;
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame <= 20)
		{
			setScale(ingFrame/5.f);
		}
		else if(ingFrame <= 100)
		{
			
		}
		else if(ingFrame <= 300)
		{
			opa_value -= 255.f/200.f;
			
			setOpacity(opa_value);
		}
		
		if(ingFrame >= 300)
		{
			stopAction();
		}
	}
	
	void stopAction()
	{
		unschedule(schedule_selector(SightOut::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit()
	{
		initWithFile("sight_out.png");
		setScale(0);
		opa_value = 255;
	}
};

class BlindDrop : public CCNode, public CCBAnimationManagerDelegate
{
public:
	static BlindDrop* create(CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame, float sc)
	{
		BlindDrop* t_bd = new BlindDrop();
		t_bd->myInit(t_sp, t_fp, t_movingFrame, t_blindFrame, sc);
		t_bd->autorelease();
		return t_bd;
	}
	
	void startAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
		ingFrame = 0;
		schedule(schedule_selector(BlindDrop::myAction));
	}
	
	virtual void completedAnimationSequenceNamed(const char *name)
	{
		string t_name = name;
		if(t_name == "end_cast1stop")
		{
			reader->release();
			oilImg->removeFromParentAndCleanup(true);
			removeFromParentAndCleanup(true);
		}
	}
	
private:
	float m_scale;
	CCPoint subPosition;
	int movingFrame;
	int blindFrame;
	int ingFrame;
	
//	CCSprite* dropImg;
	CCSprite* oilImg;
	CCBReader* reader;
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame <= blindFrame)
		{
			oilImg->setOpacity(oilImg->getOpacity()-(200/(blindFrame/2)));
		}
		if(ingFrame >= blindFrame)
		{
			stopAction();
		}
	}
	
	void stopAction()
	{
		unschedule(schedule_selector(BlindDrop::myAction));
		reader->getAnimationManager()->runAnimationsForSequenceNamed("cast1stop");
	}
	
	void myInit(CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame, float sc)
	{
//		subPosition = ccpSub(t_fp, t_sp);
//		subPosition = ccpMult(subPosition, 1.f/t_movingFrame);
		movingFrame = t_movingFrame;
		blindFrame = t_blindFrame;
		m_scale = sc;
//		dropImg = CCSprite::create("blind_drop.png");
//		addChild(dropImg);
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		reader = new CCBReader(nodeLoader);
		oilImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_tornado1.ccbi",this));
		addChild(oilImg);
		oilImg->setScale(m_scale);
		setPosition(t_fp); // t_sp
	}
};

class PrisonObject : public CCSprite
{
public:
	static PrisonObject* create(CCPoint t_sp, int t_type, int t_prisonFrame)
	{
		PrisonObject* t_po = new PrisonObject();
		t_po->myInit(t_sp, t_type, t_prisonFrame);
		t_po->autorelease();
		return t_po;
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(PrisonObject::myAction));
	}
	
private:
	
	int prisonFrame;
	int ingFrame;
	
	void myAction()
	{
		ingFrame++;
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, getPosition());
		
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		if(distance < 10)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			
			unschedule(schedule_selector(PrisonObject::myAction));
			CCFadeTo* t_fade = CCFadeTo::create(getOpacity()/255.f, 0);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PrisonObject::removeFromParent));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
			runAction(t_seq);
			return;
		}
		
		if(ingFrame <= 51)
		{
			setOpacity(ingFrame*5);
		}
		
		if(ingFrame >= prisonFrame-51)
		{
			setOpacity(255-(ingFrame-prisonFrame+51)*5);
		}
		
		
		if(ingFrame >= prisonFrame)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(PrisonObject::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_sp, int t_type, int t_prisonFrame)
	{
		prisonFrame = t_prisonFrame;
		if(t_type == 1) // fire
		{
			initWithFile("prison_fire.png");
		}
		else if(t_type == 2) // thorn
		{
			initWithFile("prison_thorn.png");
		}
		
		setOpacity(0);
		
		setPosition(t_sp);
	}
};

class BurnFragment : public CrashMapObject
{
public:
	static BurnFragment* create(IntPoint t_sp, int t_life, CCNode* t_parent, int t_mType, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		BurnFragment* t_bf = new BurnFragment();
		t_bf->myInit(t_sp, t_life, t_parent, t_mType, t_removeEffect, d_removeEffect);
		t_bf->autorelease();
		return t_bf;
	}
	
private:
	
	IntPoint myPoint;
	int myLife;
	CCSprite* fragmentImg;
	int ingFrame;
	CCNode* myParent;
	int mType;
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	void jackDie()
	{
		unschedule(schedule_selector(BurnFragment::myAction));
		CCLog("call %x", target_removeEffect);
		(target_removeEffect->*delegate_removeEffect)();
		removeFromParentAndCleanup(true);
		CCLog("%x dest", this);
	}
	
	void lineDie(IntPoint t_p)
	{
		unschedule(schedule_selector(BurnFragment::myAction));
		myGD->communication("Main_showLineDiePosition", t_p);
		(target_removeEffect->*delegate_removeEffect)();
		removeFromParentAndCleanup(true);
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(BurnFragment::myAction));
	}
	
	void myAction()
	{
		ingFrame++;
		
		int emptyCnt = 0;
		if(myGD->mapState[myPoint.x-1][myPoint.y] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[myPoint.x+1][myPoint.y] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[myPoint.x][myPoint.y-1] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[myPoint.x][myPoint.y+1] == mapEmpty)	emptyCnt++;
		
		if(emptyCnt == 0)
		{
			stopMyAction();
			return;
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		if(jackPoint.x == myPoint.x && jackPoint.y == myPoint.y)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			jackDie();
			return;
		}
		
		if(ingFrame <= 30)
		{
			fragmentImg->setScale(fragmentImg->getScale() + 1.f/30.f);
			if(ingFrame == 30)
			{
				crashMapForIntPoint(myPoint);
				// diffusion
				int random_value = rand()%2;
				if(random_value == 0) // 2 direction
				{
					int first_rd = rand()%4;
					diffusionBF(first_rd);
					int other_rd = first_rd + rand()%3+1;
					other_rd %= 4;
					diffusionBF(other_rd);
				}
				else // 3 direction
				{
					int first_rd = rand()%4;
					diffusionBF(first_rd);
					int second_rd = first_rd + rand()%3+1;
					second_rd %= 4;
					diffusionBF(second_rd);
					int other_rd = second_rd;
					while(other_rd == second_rd || other_rd == first_rd)
					{
						other_rd = rand()%4;
					}
					diffusionBF(other_rd);
				}
			}
		}
		else if(ingFrame <= 60)
		{
			fragmentImg->setScale(getScale() - 1.f/30.f);
		}
		
		
		if(ingFrame >= 60)
		{
			stopMyAction();
		}
	}
	
	void diffusionBF(int t_direction)
	{
		if(myLife <= 1)
			return;
		if(t_direction == 0) // left
		{
			if(myGD->mapState[myPoint.x-1][myPoint.y] == mapOldget || myGD->mapState[myPoint.x-1][myPoint.y] == mapOldline)
			{
				BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x-1, myPoint.y), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
				myParent->addChild(t_bf);
			}
		}
		else if(t_direction == 1) // right
		{
			if(myGD->mapState[myPoint.x+1][myPoint.y] == mapOldget || myGD->mapState[myPoint.x+1][myPoint.y] == mapOldline)
			{
				BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x+1, myPoint.y), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
				myParent->addChild(t_bf);
			}
		}
		else if(t_direction == 2) // down
		{
			if(myGD->mapState[myPoint.x][myPoint.y-1] == mapOldget || myGD->mapState[myPoint.x][myPoint.y-1] == mapOldline)
			{
				BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x, myPoint.y-1), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
				myParent->addChild(t_bf);
			}
		}
		else // up
		{
			if(myGD->mapState[myPoint.x][myPoint.y+1] == mapOldget || myGD->mapState[myPoint.x][myPoint.y+1] == mapOldline)
			{
				BurnFragment* t_bf = BurnFragment::create(IntPoint(myPoint.x, myPoint.y+1), myLife-1, myParent, mType, target_removeEffect, delegate_removeEffect);
				myParent->addChild(t_bf);
			}
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(BurnFragment::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(IntPoint t_sp, int t_life, CCNode* t_parent, int t_mType, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		target_removeEffect = t_removeEffect;
		delegate_removeEffect = d_removeEffect;
		
		setPosition(ccp((t_sp.x-1)*pixelSize+1, (t_sp.y-1)*pixelSize+1));
		mType = t_mType;
		myParent = t_parent;
		
		myPoint = t_sp;
		myLife = t_life;
		
		if(mType == 1)
		{
			fragmentImg = CCSprite::create("burnFragment.png");
			fragmentImg->setScale(0);
			addChild(fragmentImg);
		}
		else
		{
			fragmentImg = CCSprite::create("iceFragment.png");
			fragmentImg->setScale(0);
			addChild(fragmentImg);
		}
		
		startMyAction();
	}
};
class KSSequenceAndRemove : public CCActionInterval
{
public:
	~KSSequenceAndRemove(void){}
	
public:
	
	static CCSequence* create(CCNode* thiz, std::initializer_list<CCFiniteTimeAction*> initList)
	{
		CCArray* actions = CCArray::create();
		for(auto action : initList)
		{
			actions->addObject(action);
		}
		
		auto _remove = CCCallFunc::create(thiz, callfunc_selector(CCNode::removeFromParent));
		actions->addObject(_remove);
		
		return CCSequence::create(actions);
	}
};


class Firework : public CrashMapObject
{
public:
	static Firework* create(CCPoint cumberPosition, CCPoint jackPosition)
	{
		Firework* t_bf = new Firework();
		t_bf->myInit(cumberPosition, jackPosition);
		t_bf->autorelease();
		return t_bf;
	}
	void crashMapForPoint(IntPoint point, int radius)
	{
		for(int y = - radius; y <= radius; y++)
		{
			for(int x = - radius; x <= radius; x++)
			{
				if(sqrt(x*x + y*y) <= radius)
					crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
			}
		}
	}
	void myInit(CCPoint cumberPosition, CCPoint jackPosition)
	{
		m_step = 1;
		m_bombFrame = 300;
		m_parentMissile = CCParticleSystemQuad::create("pm.plist");
		m_parentMissile->setPositionType(kCCPositionTypeRelative);
		m_parentMissile->setPosition(cumberPosition);
		addChild(m_parentMissile);
		
//		CCPoint jackPosition = ip2ccp(myGD->getJackPoint());
		CCPoint subtract = jackPosition - cumberPosition;
		m_parentMissileGoal.init(cumberPosition, jackPosition, 0.01f * ccpLength(subtract));
		scheduleUpdate();
		
		int m_color = 1;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
		else
			batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
		
		addChild(batchNode);
		
	}
	
	void setTwoStep()
	{
		myGD->communication("MS_resetRects", false);
		m_step = 2;
		m_frame = 0;
		m_sourcePosition = m_parentMissile->getPosition();
		m_parentMissile->setStartColor(ccc4f(0, 0, 0, 0));
		m_parentMissile->setEndColor(ccc4f(0, 0, 0, 0));
		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCDelayTime::create(3.f)}));
		
		schedule(schedule_selector(ThisClassType::selfRemove));
	}
	void selfRemove(float dt)
	{
		if(batchNode->getChildrenCount() == 0 && getChildrenCount() == 1)
		{
			removeFromParentAndCleanup(true);
		}
	}
	void jackDie()
	{
//		unscheduleUpdate();
		if(m_step == 1)
		{
			setTwoStep();
		}
	}
	
	void lineDie(IntPoint t_p)
	{
//		unscheduleUpdate();
		myGD->communication("Main_showLineDiePosition", t_p);
//		(target_removeEffect->*delegate_removeEffect)();
		if(m_step == 1)
		{
			setTwoStep();
		}
	}
	
	void update(float dt)
	{		
//		CCLog("pokjuk %d", m_frame);
		
		bool r = m_parentMissileGoal.step(1.f / 60.f);
		if(m_step == 1)
		{
			m_frame++;
			m_parentMissile->setPosition(m_parentMissileGoal.getValue());
			if(m_frame % 5 == 0)
				crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), 10);
		}
		
		if(!r && m_step == 1)
		{
			setTwoStep();
			
		}
		
		if(m_step == 2)
		{
			m_frame++;
			
			if(m_frame % 15 == 0)
			{
				float bulletSpeed = 2.f;
				int m_color = 1;
				std::string imgFileName;
				std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
				if(KS::isExistFile(fileName))
					imgFileName = fileName;
				else
					imgFileName = "cumber_missile1.png";
				CCSize t_mSize = CCSize(4.f, 4.f);
				
				for(int i=0; i<=360; i+= 10)
				{
					MissileUnit* t_mu = MissileUnit::create(m_sourcePosition, i, bulletSpeed,
																									imgFileName.c_str(), t_mSize,0, 0);
					batchNode->addChild(t_mu);
				}
			
			}
			
			
			if(m_frame == m_bombFrame)
			{
				
				m_step = 3;
			}
			
		}
	}
protected:
	CCPoint m_sourcePosition;
	int m_step;
	int m_frame;
	int m_bombFrame;
	CCParticleSystem* m_parentMissile;
	CCSpriteBatchNode* batchNode;
	FromToWithDuration2<CCPoint> m_parentMissileGoal;
};

class MovingSunflower : public CrashMapObject
{
public:
	static MovingSunflower* create(CCPoint cumberPosition, CCPoint jackPosition)
	{
		CCLog("%f %f", cumberPosition.x, cumberPosition.y);
		MovingSunflower* t_bf = new MovingSunflower();
		t_bf->myInit(cumberPosition, jackPosition);
		t_bf->autorelease();
		return t_bf;
	}
	void crashMapForPoint(IntPoint point, int radius)
	{
		for(int y = - radius; y <= radius; y++)
		{
			for(int x = - radius; x <= radius; x++)
			{
				if(sqrt(x*x + y*y) <= radius)
					crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
			}
		}
	}
	void myInit(CCPoint cumberPosition, CCPoint jackPosition)
	{
		m_step = 1;
		m_bombFrame = 300;
		m_parentMissile = CCParticleSystemQuad::create("pm.plist");
		m_parentMissile->setPositionType(kCCPositionTypeRelative);
		m_parentMissile->setPosition(cumberPosition);
		addChild(m_parentMissile);
		
		//		CCPoint jackPosition = ip2ccp(myGD->getJackPoint());
		
		auto mat = CCAffineTransformMakeIdentity();
		auto mat2 = CCAffineTransformRotate(mat, m_well512.GetFloatValue(-15 * M_PI / 180.f, +15 * M_PI / 180.f));
		jackPosition = CCPointApplyAffineTransform(jackPosition, mat2);
		CCPoint subtract = jackPosition - cumberPosition;
		
		m_parentMissileGoal.init(cumberPosition, jackPosition, 0.01f * ccpLength(subtract));
		scheduleUpdate();
		
		int m_color = 1;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
		else
			batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
		
		addChild(batchNode);
		
	}
	
	void setTwoStep()
	{
		schedule(schedule_selector(ThisClassType::selfRemove));
		myGD->communication("MS_resetRects", false);
		m_step = 2;
		m_frame = 0;
		m_sourcePosition = m_parentMissile->getPosition();
		m_parentMissile->setStartColor(ccc4f(0, 0, 0, 0));
		m_parentMissile->setEndColor(ccc4f(0, 0, 0, 0));
		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCDelayTime::create(3.f)}));
	}
	void selfRemove(float dt)
	{
		if(batchNode->getChildrenCount() == 0 && getChildrenCount() == 1)
		{
			removeFromParentAndCleanup(true);
		}
	}
	void jackDie()
	{
		//		unscheduleUpdate();
		if(m_step == 1)
		{
			setTwoStep();
		}
	}
	
	void lineDie(IntPoint t_p)
	{
		//		unscheduleUpdate();
		myGD->communication("Main_showLineDiePosition", t_p);
		//		(target_removeEffect->*delegate_removeEffect)();
		if(m_step == 1)
		{
			setTwoStep();
		}
	}
	
	void update(float dt)
	{
		//		CCLog("pokjuk %d", m_frame);
		
		bool r = m_parentMissileGoal.step(1.f / 60.f);
		if(m_step == 1)
		{
			m_frame++;
			m_parentMissile->setPosition(m_parentMissileGoal.getValue());
			if(m_frame % 5 == 0)
				crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), 10);
			
			if(m_frame % 15 == 0)
			{
				float bulletSpeed = 1.f;
				int m_color = 1;
				std::string imgFileName;
				std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
				if(KS::isExistFile(fileName))
					imgFileName = fileName;
				else
					imgFileName = "cumber_missile1.png";
				CCSize t_mSize = CCSize(4.f, 4.f);
				
				for(int i=0; i<=360; i+= 10)
				{
					MissileUnit* t_mu = MissileUnit::create(m_parentMissileGoal.getValue(), i, bulletSpeed,
																									imgFileName.c_str(), t_mSize,0, 0);
					batchNode->addChild(t_mu);
				}
			}

		}
		
		if(!r && m_step == 1)
		{
			setTwoStep();
			
		}
		
		if(m_step == 2)
		{
						
		}
	}
protected:
	CCPoint m_sourcePosition;
	int m_step;
	int m_frame;
	int m_bombFrame;
	CCParticleSystem* m_parentMissile;
	CCSpriteBatchNode* batchNode;
	FromToWithDuration2<CCPoint> m_parentMissileGoal;
	Well512 m_well512;
};

class AlongOfTheLine : public CCNode
{
public:
	static AlongOfTheLine* create(CCPoint cumberPosition, CCPoint jackPosition)
	{
		AlongOfTheLine* t_bf = new AlongOfTheLine();
		t_bf->myInit(cumberPosition, jackPosition);
		t_bf->autorelease();
		return t_bf;
	}
	void myInit(CCPoint cumberPosition, CCPoint jackPosition);
	
	void setTwoStep()
	{
		m_step = 2;
	}
	
	void hidingAnimation(float dt);
	void update(float dt);
protected:
	struct AlongPath
	{
		IntPoint point;
		IntPoint direction;
		AlongPath(const IntPoint& pt, const IntPoint& dir) : point(pt), direction(dir){}
		AlongPath(){}
	};
	struct Pollution
	{
		CCSprite* spr;
		CCSprite* goal;
		FromToWithDuration2<CCPoint> glue; // 맵에 처음 붙이는 용도
		AlongPath alongPath; // 현재 어느 방향으로 이동하고 있는가.
		int step;
		Pollution() : step(1) {}
	};
	
	int m_step;
	int m_frame;
	int m_totalFrame;
	FromToWithDuration<float> m_scaleTo;
	std::vector<Pollution> m_pollutions;
	std::map<IntPoint, IntPoint> m_directions; // 좌표별 이동 방향.
	Well512 m_well512;
	std::vector<CCSprite*> goalSprites;
};


class ThrowBomb : public CrashMapObject
{
public:
	static ThrowBomb* create(CCPoint cumberPosition, CCPoint jackPosition)
	{
		ThrowBomb* t_bf = new ThrowBomb();
		t_bf->myInit(cumberPosition, jackPosition);
		t_bf->autorelease();
		return t_bf;
	}
	void crashMapForPoint(IntPoint point, int radius)
	{
		for(int y = - radius; y <= radius; y++)
		{
			for(int x = - radius; x <= radius; x++)
			{
				if(sqrt(x*x + y*y) <= radius)
					crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
			}
		}
	}
	void selfRemove(float dt)
	{
		if(getChildrenCount() == 0)
		{
			removeFromParentAndCleanup(true);
		}
	}
	void jackDie()
	{
		//		unscheduleUpdate();
//		if(m_step == 1)
//		{
//			setTwoStep();
//		}
	}
	
	void lineDie(IntPoint t_p)
	{
		//		unscheduleUpdate();
		myGD->communication("Main_showLineDiePosition", t_p);
		//		(target_removeEffect->*delegate_removeEffect)();
//		if(m_step == 1)
//		{
//			m_step
//		}
	}

	void myInit(CCPoint cumberPosition, CCPoint jackPosition)
	{
		m_step = 1;
		
		m_angle = atan2(jackPosition.y - cumberPosition.y, jackPosition.x - cumberPosition.x);
		m_angle += m_well512.GetFloatValue(-10 * M_PI/180.f, +10 * M_PI/180.f);
		m_parentMissile = CCParticleSystemQuad::create("throwbomb.plist");
		m_parentMissile->setPositionType(kCCPositionTypeRelative);
		
		
		
		m_parentMissile->setPosition(cumberPosition);
		addChild(m_parentMissile);
		
		scheduleUpdate();
	}
	
//	void setTwoStep()
//	{
//		m_step = 2;
//		m_frame = 0;
//		m_sourcePosition = m_parentMissile->getPosition();
//		m_parentMissile->setStartColor(ccc4f(0, 0, 0, 0));
//		m_parentMissile->setEndColor(ccc4f(0, 0, 0, 0));
//		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCDelayTime::create(3.f)}));
//		
//		schedule(schedule_selector(ThisClassType::selfRemove));
//	}
//	void selfRemove(float dt)
//	{
//		if(batchNode->getChildrenCount() == 0 && getChildrenCount() == 1)
//		{
//			removeFromParentAndCleanup(true);
//		}
//	}
	
	void update(float dt)
	{
		//		CCLog("pokjuk %d", m_frame);
		if(m_step == 1)
		{
			m_frame++;
			CCPoint delta = ccp(cos(m_angle) * 3.5f, sin(m_angle) * 3.5f);
			
			CCPoint missilePosition = m_parentMissile->getPosition();
			m_parentMissile->setPosition(missilePosition + delta);
			
			IntPoint mapPoint = ccp2ip(m_parentMissile->getPosition());
			float half_distance = 10;
			bool crash = false;
			for(int i=mapPoint.x-half_distance/2;i<=mapPoint.x+half_distance/2 && !crash;i++)
			{
				for(int j=mapPoint.y-half_distance/2;j<=mapPoint.y+half_distance/2 && !crash;j++)
				{
					float calc_distance = sqrtf(powf((mapPoint.x - i)*pixelSize,2) + powf((mapPoint.y - j)*pixelSize, 2));
					if(calc_distance < half_distance)
					{
						IntPoint check_position = IntPoint(i,j);
						if(!check_position.isInnerMap() || myGD->mapState[check_position.x][check_position.y] != mapEmpty)
						{
							crash = true;
						}
					}
				}
			}
			
			if(crash)
			{
				crashMapForPoint(ccp2ip(m_parentMissile->getPosition()), 15);
				m_step = 2;
			}
		}
		
		if(m_step == 2) // 폭발.
		{
			m_parentMissile->removeFromParent();
			schedule(schedule_selector(ThisClassType::selfRemove));
			CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
			
			particle->setAutoRemoveOnFinish(true);
			particle->setPositionType(kCCPositionTypeRelative);
			CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("tickingTime_bomb.png");
			particle->setTexture(texture);
			particle->setEmissionRate(250.00);
			particle->setAngle(90.0);
			particle->setAngleVar(360.0);
			ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
			particle->setBlendFunc(blendFunc);
			particle->setDuration(0.20);
			particle->setEmitterMode(kCCParticleModeGravity);
			ccColor4F startColor = {0.87,0.81,0.12,1.00}; // 0.76 0.25 0.12
			particle->setStartColor(startColor);
			ccColor4F startColorVar = {0,0,0,0};
			particle->setStartColorVar(startColorVar);
			ccColor4F endColor = {0.68,0.16,0.00,1.00};
			particle->setEndColor(endColor);
			ccColor4F endColorVar = {0,0,0,0};
			particle->setEndColorVar(endColorVar);
			particle->setStartSize(20.00);
			particle->setStartSizeVar(10.0);
			particle->setEndSize(40.0);
			particle->setEndSizeVar(10.0);
			particle->setGravity(ccp(0,0));
			particle->setRadialAccel(0.0);
			particle->setRadialAccelVar(0.0);
			particle->setSpeed(250);
			particle->setSpeedVar(60.0);
			particle->setTangentialAccel(0);
			particle->setTangentialAccelVar(0);
			particle->setTotalParticles(50);
			particle->setLife(0.30);
			particle->setLifeVar(0.0);
			particle->setStartSpin(0.0);
			particle->setStartSpinVar(0.0);
			particle->setEndSpin(0.0);
			particle->setEndSpinVar(0.0);
			particle->setPosition(m_parentMissile->getPosition());
			particle->setPosVar(CCPointZero);
			addChild(particle);
			m_step = 3;

			
		}
	}
protected:
	int m_step;
	int m_frame;
	CCParticleSystem* m_parentMissile;
	Well512 m_well512;
	float m_angle;
};


namespace std
{
	template <> struct hash<IntPoint>
	{
		size_t operator()(const IntPoint& ip) const
		{
			int x = ip.x;
			int y = ip.y;
			return hash<int>()(x) ^ hash<int>()(y);
//			return x + y;
		}
	};
}


static ProbSelector randomObj = {9, 1};
class ReaverScarab : public CrashMapObject
{
public:
	static ReaverScarab* create(CCPoint cumberPosition, CCPoint jackPosition)
	{
		ReaverScarab* t_bf = new ReaverScarab();
		t_bf->myInit(cumberPosition, jackPosition);
		t_bf->autorelease();
		return t_bf;
	}
	void crashMapForPoint(IntPoint point, int radius)
	{
		for(int y = - radius; y <= radius; y++)
		{
			for(int x = - radius; x <= radius; x++)
			{
				if(sqrt(x*x + y*y) <= radius)
					crashMapForIntPoint(IntPoint(point.x + x, point.y + y));
			}
		}
	}
	void selfRemove(float dt)
	{
		if(getChildrenCount() == 0)
		{
			removeFromParentAndCleanup(true);
		}
	}
	void jackDie()
	{
		//		unscheduleUpdate();
		//		if(m_step == 1)
		//		{
		//			setTwoStep();
		//		}
	}
	
	void lineDie(IntPoint t_p)
	{
		//		unscheduleUpdate();
		myGD->communication("Main_showLineDiePosition", t_p);
		//		(target_removeEffect->*delegate_removeEffect)();
		//		if(m_step == 1)
		//		{
		//			m_step
		//		}
	}
	
	void myInit(CCPoint cumberPosition, CCPoint jackPosition)
	{
		m_insertCount = 0;
		m_step = 1;
		m_jackPoint = ccp2ip(jackPosition);
		CCLog("init %d %d", m_jackPoint.x, m_jackPoint.y);
		m_parentMissile = CCParticleSystemQuad::create("throwbomb.plist");
		m_parentMissile->setPositionType(kCCPositionTypeRelative);

		m_parentMissile->setPosition(cumberPosition);
		addChild(m_parentMissile);
		
		scheduleUpdate();
		
		aStar(m_jackPoint);
		
	}
	int lengthToEnd(IntPoint point)
	{
		return (abs(m_jackPoint.x - point.x) + abs(m_jackPoint.y - point.y)) * 10;
	}
	// from 에서 to 로 퍼져나갈 때 블럭 처리.
	bool processObject(IntPoint pointFrom, IntPoint pointTo, int distance);
	void aStar(IntPoint endPt);
	void update(float dt);
protected:
	struct CellInfo
	{
		int dx, dy, g, h;
		CellInfo(int _dx, int _dy, int _g, int _h)
		: dx(_dx), dy(_dy), g(_g), h(_h)
		{}
		CellInfo(){}
		bool operator<(const CellInfo& ci) const
		{
			return g+h < ci.g + ci.h;
		}
	};
	int m_step;
	int m_frame;
	IntPoint m_jackPoint;
	CCParticleSystem* m_parentMissile;
	Well512 m_well512;
	float m_angle;
//	IntPoint m_endPoint;
	
	
//	struct IntPointHashFunctor
//	{
//		bool operator(const IntPoint& a, const IntPoint& b)
//		{
//			
//		}
//	};
//	std::unordered_map<IntPoint, int> m_test;
//	std::unordered_map<IntPoint, CellInfo> m_prevCloseList;
	struct CoordAndCellInfo
	{
		int x, y, dx, dy, g, h, order;
		CoordAndCellInfo(int _x, int _y, int _dx, int _dy, int _g, int _h)
		: x(_x), y(_y), dx(_dx), dy(_dy), g(_g), h(_h)
		{
			order = 0;
		}
		CoordAndCellInfo(){
			order = 0;
		}
//		bool operator<(const CoordAndCellInfo& ci) const
//		{
//			bool r = false;
//			if(g+h == ci.g + ci.h)
//				
//				r = -order < -ci.order;
//			else
//				r = g+h < ci.g + ci.h;
//			return r;
//		}
		
		bool operator<(const CoordAndCellInfo& ci) const
		{
			return g+h - order< ci.g + ci.h - ci.order;
		}
		
//		bool operator<(const CoordAndCellInfo& ci) const
//		{
//			return -order < -ci.order;
//		}
		bool operator==(const CoordAndCellInfo& ci) const
		{
			return x == ci.x && y == ci.y;
		}
	};
	
	std::unordered_map<IntPoint, CellInfo> m_closeListMap;

	std::multiset<CoordAndCellInfo, less<CoordAndCellInfo> > m_openList;

	
	std::vector<CCPoint> m_bulletReversePath;
	decltype(m_bulletReversePath.rbegin()) m_bulletIter;
	int m_insertCount;
};




class CloudBomb : public CCNode
{
public:
	static CloudBomb* create(CCPoint cumberPosition, CCPoint jackPosition)
	{
		CloudBomb* t_bf = new CloudBomb();
		t_bf->myInit(cumberPosition, jackPosition);
		t_bf->autorelease();
		return t_bf;
	}
	void myInit(CCPoint cumberPosition, CCPoint jackPosition)
	{
		m_step = 1;
		m_bombProb = 0.001f;
		
		m_parentMissile = CCParticleSystemQuad::create("cloudbomb.plist");
		m_parentMissile->setPositionType(kCCPositionTypeRelative);
		
		
		
		m_parentMissile->setPosition(cumberPosition);
		addChild(m_parentMissile);
		
		scheduleUpdate();
		
		int m_color = 1;
		std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
		if(KS::isExistFile(fileName))
			batchNode = CCSpriteBatchNode::create(fileName.c_str(), 300);
		else
			batchNode = CCSpriteBatchNode::create("cumber_missile1.png", 300);
		
		addChild(batchNode);
		
	}
	
	void setTwoStep()
	{
		m_step = 2;
		m_frame = 0;
		m_sourcePosition = m_parentMissile->getPosition();
		m_parentMissile->setStartColor(ccc4f(0, 0, 0, 0));
		m_parentMissile->setEndColor(ccc4f(0, 0, 0, 0));
		m_parentMissile->runAction(KSSequenceAndRemove::create(m_parentMissile, {CCDelayTime::create(3.f)}));
		
		schedule(schedule_selector(ThisClassType::selfRemove));
	}
	void selfRemove(float dt)
	{
		if(batchNode->getChildrenCount() == 0 && getChildrenCount() == 1)
		{
			removeFromParentAndCleanup(true);
		}
	}
	
	void update(float dt)
	{
		//		CCLog("pokjuk %d", m_frame);
		if(m_step == 1)
		{
			m_frame++;
			IntPoint pos = ccp2ip(m_parentMissile->getPosition());
			IntPoint afterPos;
			while(1)
			{
				afterPos = IntPoint(pos.x + m_well512.GetValue(-2, +2),
																		 pos.y + m_well512.GetValue(-2, +2));
				if(afterPos.isInnerMap())
					break;
			}
			
			m_parentMissile->setPosition(ip2ccp(afterPos));
			
			int p = ProbSelector::sel(m_bombProb, 1.f - m_bombProb, 0.f);
			if(p == 0)
			{
				setTwoStep();
			}
		}
		
		if(m_step == 2) // 폭발.
		{
			m_frame++;
			
			float bulletSpeed = 4.f;
			int m_color = 1;
			std::string imgFileName;
			std::string fileName = CCString::createWithFormat("cumber_missile%d.png", m_color)->getCString();
			if(KS::isExistFile(fileName))
				imgFileName = fileName;
			else
				imgFileName = "cumber_missile1.png";
			CCSize t_mSize = CCSize(4.f, 4.f);
			for(int i=0; i<=360; i+= 10)
			{
				MissileUnit* t_mu = MissileUnit::create(m_sourcePosition, i, bulletSpeed,
																								imgFileName.c_str(), t_mSize,0, 0);
				batchNode->addChild(t_mu);
			}
			m_step = 3;
		}
	}
protected:
	CCPoint m_sourcePosition;
	int m_step;
	int m_frame;
	CCParticleSystem* m_parentMissile;
	float m_bombProb; // 폭발 확률
	CCSpriteBatchNode* batchNode;
	Well512 m_well512;
};


class Burn : public CCNode
{
public:
	static Burn* create(CCPoint t_sp, float t_baseDistance, float t_shootAngle, int t_mType, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		Burn* t_b = new Burn();
		t_b->myInit(t_sp, t_baseDistance, t_shootAngle, t_mType, t_removeEffect, d_removeEffect);
		t_b->autorelease();
		return t_b;
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(Burn::myAction));
	}
	
	void removeEffect()
	{
		unschedule(schedule_selector(Burn::myAction));
		(target_removeEffect->*delegate_removeEffect)();
		removeFromParentAndCleanup(true);
		CCLog("%x destroy", this);
	}
	
private:
	
	CCPoint movingDv;
	int ingFrame;
	bool positionSetted;
	int mType;
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	void myAction()
	{
		ingFrame++;
		
		if(!positionSetted)
		{
			CCPoint afterPosition = ccpAdd(getPosition(), movingDv);
			setPosition(afterPosition);
			
			IntPoint mapPoint = IntPoint((afterPosition.x-1)/pixelSize + 1, (afterPosition.y-1)/pixelSize + 1);
			if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapOldline)
			{
				positionSetted = true;
				BurnFragment* t_bf = BurnFragment::create(mapPoint, 5, getParent(), mType, target_removeEffect, delegate_removeEffect);
				getParent()->addChild(t_bf);
				stopMyAction();
			}
			else if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapOldget)
			{
				IntMoveState searchFirstMoveState = IntMoveState(mapPoint.x, mapPoint.y, directionStop);
				searchAndMoveOldline(searchFirstMoveState);
				stopMyAction();
			}
		}
		
		
		if(ingFrame > 60 && !positionSetted)
		{
			stopMyAction();
		}
	}
	
	void searchAndMoveOldline(IntMoveState searchFirstMoveState)
	{
		queue<IntMoveState> bfsArray;
		bfsArray.push(searchFirstMoveState);
		
		bool isFinded = false;
		IntPoint findedPoint;
		
		while(!bfsArray.empty() && !isFinded)
		{
			IntMoveState t_ms = bfsArray.front();
			bfsArray.pop();
			if(myGD->mapState[t_ms.origin.x][t_ms.origin.y] == mapOldline)
			{
				isFinded = true;
				findedPoint = t_ms.origin;
			}
			else
			{
				if(t_ms.direction == directionStop)
				{
					for(int i = directionLeftUp;i <= directionUp; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionLeftUp)
				{
					for(int i = directionLeftUp;i <= directionLeft; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
					
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionLeft)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x-1, t_ms.origin.y, directionLeft);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionLeftDown)
				{
					for(int i = directionLeft;i <= directionDown; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionDown)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y-1, directionDown);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionRightDown)
				{
					for(int i = directionDown;i <= directionRight; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionRight)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x+1, t_ms.origin.y, directionRight);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
				else if(t_ms.direction == directionRightUp)
				{
					for(int i = directionRight;i <= directionUp; i++)
					{
						IntVector t_v = IntVector::directionVector((IntDirection)i);
						IntMoveState n_ms = IntMoveState(t_ms.origin.x+t_v.dx, t_ms.origin.y+t_v.dy, (IntDirection)i);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);
					}
				}
				else if(t_ms.direction == directionUp)
				{
					{	IntMoveState n_ms = IntMoveState(t_ms.origin.x, t_ms.origin.y+1, directionUp);
						if(n_ms.origin.isInnerMap())
							bfsArray.push(n_ms);}
				}
			}
		}
		
		if(isFinded)
		{
			positionSetted = true;
			BurnFragment* t_bf = BurnFragment::create(findedPoint, 10, getParent(), mType, target_removeEffect, delegate_removeEffect);
			getParent()->addChild(t_bf);
		}
		else // escape point not found
		{
			CCLog("escape point not found!");
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(Burn::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_sp, float t_baseDistance, float t_shootAngle, int t_mType, CCObject* t_removeEffect, SEL_CallFunc d_removeEffect)
	{
		target_removeEffect = t_removeEffect;
		delegate_removeEffect = d_removeEffect;
		
		positionSetted = false;
		
		mType = t_mType;
		
		setPosition(t_sp);
		
		movingDv.x = 1;
		movingDv.y = tanf(t_shootAngle/180.f*M_PI);
		
		if((t_shootAngle > 90 && t_shootAngle < 270) || t_shootAngle < -90)
		{
			movingDv = ccpMult(movingDv, -1.f);
		}
		
		float div_value = sqrtf(powf(movingDv.x, 2.f) + powf(movingDv.y, 2.f));
		movingDv = ccpMult(movingDv, 1.f/div_value);
		movingDv = ccpMult(movingDv, t_baseDistance/60.f);
		
		startMyAction();
	}
};

class PoisonLine : public CCNode
{
public:
	static PoisonLine* create(IntPoint t_sp, int frame)
	{
		PoisonLine* t_pl = new PoisonLine();
		t_pl->myInit(t_sp, frame);
		t_pl->autorelease();
		return t_pl;
	}
	
	void startMyAction()
	{
		ingFrame = 0;
		schedule(schedule_selector(PoisonLine::myAction));
	}
	
private:
	
	IntPoint mapPoint;
	int ingFrame;
	int totalFrame;
	void myAction()
	{
		ingFrame++;
		
//		ccColor3B t_color = getColor();
//		t_color.r += rand()%3-1;
//		t_color.g += rand()%3-1;
//		t_color.b += rand()%3-1;
//		
//		setColor(t_color);
		
		int emptyCnt = 0;
		if(myGD->mapState[mapPoint.x-1][mapPoint.y] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[mapPoint.x+1][mapPoint.y] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[mapPoint.x][mapPoint.y-1] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[mapPoint.x][mapPoint.y+1] == mapEmpty)	emptyCnt++;
		
		if(emptyCnt == 0)
		{
			unschedule(schedule_selector(PoisonLine::myAction));
			removeFromParentAndCleanup(true);
			return;
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		if(jackPoint.x == mapPoint.x && jackPoint.y == mapPoint.y)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
			stopMyAction();
			
			return;
		}
		
		if(ingFrame >= totalFrame)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(PoisonLine::myAction));
		getParent()->removeFromParentAndCleanup(true);
//		removeFromParentAndCleanup(true);
	}
	
	void myInit(IntPoint t_sp, int frame)
	{
		totalFrame = frame;
		mapPoint = t_sp;
		
//		initWithFile("poison_line.png");
		auto ret = KS::loadCCBI<CCSprite*>(this, "fx_pollution2.ccbi");
		CCSprite* line = ret.first;
		addChild(line);
		CCPoint myPosition = ccp((t_sp.x-1)*pixelSize+1, (t_sp.y-1)*pixelSize+1);
		setPosition(myPosition);
		
//		setColor(ccc3(0, 66, 75));
	}
};

class PoisonDrop : public CCNode
{
public:
	static PoisonDrop* create(CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int area, int totalframe)
	{
		PoisonDrop* t_bd = new PoisonDrop();
		t_bd->myInit(t_sp, t_fp, t_movingFrame, area, totalframe);
		t_bd->autorelease();
		return t_bd;
	}
	
	void startAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
		ingFrame = 0;
		schedule(schedule_selector(PoisonDrop::myAction));
	}
	
private:
	int m_area;
	int m_totalFrame;
	CCPoint subPosition;
	int movingFrame;
	int ingFrame;
	
	CCSprite* dropImg;
	CCSprite* oilImg;
	
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame <= movingFrame)
		{
			CCPoint afterPosition = ccpAdd(getPosition(), subPosition);
			setPosition(afterPosition);
			
			if(ingFrame == movingFrame)
			{
				AudioEngine::sharedInstance()->playEffect("sound_threecusion_bomb.mp3",false);
				initParticle();
				
				IntPoint basePoint = IntPoint((afterPosition.x-1)/pixelSize + 1, (afterPosition.y-1)/pixelSize + 1);
				
				for(int i=-m_area;i<=m_area;i++)
				{
					for(int j=-m_area;j<=m_area;j++)
					{
						IntPoint checkPoint = IntPoint(basePoint.x+i,basePoint.y+j);
						if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapOldline)
						{
							PoisonLine* t_pl = PoisonLine::create(IntPoint(checkPoint.x, checkPoint.y), m_totalFrame);
							getParent()->addChild(t_pl);
							t_pl->startMyAction();
						}
					}
				}
				dropImg->removeFromParentAndCleanup(true);
			}
		}
		
		
		if(ingFrame >= movingFrame)
		{
			stopAction();
		}
	}
	
	void initParticle()
	{
		CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("tickingTime_bomb.png");
		particle->setTexture(texture);
		particle->setEmissionRate(250.00);
		particle->setAngle(90.0);
		particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(0.20);
		particle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor = {0.00,0.25,0.30,1.00}; // 0.76 0.25 0.12
		particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {0.00,0.00,0.00,1.00};
		particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		particle->setEndColorVar(endColorVar);
		particle->setStartSize(20.00);
		particle->setStartSizeVar(10.0);
		particle->setEndSize(40.0);
		particle->setEndSizeVar(10.0);
		particle->setGravity(ccp(0,0));
		particle->setRadialAccel(0.0);
		particle->setRadialAccelVar(0.0);
		particle->setSpeed(170);
		particle->setSpeedVar(60.0);
		particle->setTangentialAccel(0);
		particle->setTangentialAccelVar(0);
		particle->setTotalParticles(50);
		particle->setLife(0.20);
		particle->setLifeVar(0.0);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(0.0);
		particle->setEndSpin(0.0);
		particle->setEndSpinVar(0.0);
		particle->setPosition(CCPointZero);
		particle->setPosVar(CCPointZero);
		addChild(particle);
	}
	
	void stopAction()
	{
		unschedule(schedule_selector(PoisonDrop::myAction));
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PoisonDrop::selfRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int area, int totalframe)
	{
		m_area = area;
		m_totalFrame = totalframe;
		subPosition = ccpSub(t_fp, t_sp);
		subPosition = ccpMult(subPosition, 1.f/t_movingFrame);
		movingFrame = t_movingFrame;
		
		dropImg = CCSprite::create("blind_drop.png");
		addChild(dropImg);
		
		setPosition(t_sp);
	}
};

#define MAX_SCALE_X	10.f
#define ADD_SCALE_X	2.5f

class ReflectionLazer : public CCSprite
{
public:
	static ReflectionLazer* create(CCPoint t_sp, CCPoint t_fp, int t_frame, int t_type)
	{
		ReflectionLazer* t_rl = new ReflectionLazer();
		t_rl->myInit(t_sp, t_fp, t_frame, t_type);
		t_rl->autorelease();
		return t_rl;
	}
	
	void startMyAction()
	{
		ing_frame = 0;
		schedule(schedule_selector(ReflectionLazer::myAction));
	}
	
private:
	CCPoint dv;
	int move_frame;
	int ing_frame;
	
	void myAction()
	{
		ing_frame++;
		
		setPosition(ccpAdd(getPosition(), dv));
		
		if(getScaleX() < MAX_SCALE_X)
		{
			setScaleX(getScaleX() + ADD_SCALE_X);
		}
		
		if(ing_frame >= move_frame)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(ReflectionLazer::myAction));
		schedule(schedule_selector(ReflectionLazer::selfRemove));
	}
	
	void selfRemove()
	{
		if(getScaleX() > ADD_SCALE_X)
		{
			setScaleX(getScaleX() - ADD_SCALE_X);
		}
		else if(getScaleX() == ADD_SCALE_X)
		{
			setScaleX(0.f);
		}
		else
		{
			unschedule(schedule_selector(ReflectionLazer::selfRemove));
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit(CCPoint t_sp, CCPoint t_fp, int t_frame, int t_type)
	{
		move_frame = t_frame;
		initWithFile(CCString::createWithFormat("lazerMissile%d.png", t_type)->getCString());
		setAnchorPoint(ccp(0,0.5));
		setPosition(t_sp);
		setScaleX(0);
		
		CCPoint subPosition = ccpSub(t_fp, t_sp);
		
		dv = ccpMult(subPosition, 1.f/move_frame);
		
		float t_angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f + 180;
		setRotation(-t_angle);
	}
};

class MirrorReflection : public CCNode
{
public:
	static MirrorReflection* create(int t_sn, float t_r, int t_direction, int t_type)
	{
		MirrorReflection* t_mr = new MirrorReflection();
		t_mr->myInit(t_sn, t_r, t_direction, t_type);
		t_mr->autorelease();
		return t_mr;
	}
	
	void startMyAction()
	{
		ing_frame = -1;
		schedule(schedule_selector(MirrorReflection::myAction));
	}
	
private:
	float radius;
	int sn;
	int fn;
	int direction;
	int ing_frame;
	int myType;
	int create_frame;
	int base_angle;
	int rand_rotate;
	
	CCSprite* effectImg;
	CCSprite* impactImg;
	
	CCPoint getNumberPosition(int t_number)
	{
		float t_angle = t_number*40.f + base_angle;
		CCPoint r_value;
		r_value.x = 1;
		r_value.y = tanf(t_angle/180.f*M_PI);
		
		if(t_angle >= 90 && t_angle < 270)
			r_value = ccpMult(r_value, -1.f);
		
		float div_value = sqrtf(powf(r_value.x, 2.f) + powf(r_value.y, 2.f));
		
		r_value = ccpMult(r_value, 1.f/div_value);
		r_value = ccpMult(r_value, radius);
		
		return r_value;
	}
	
	void myAction()
	{
		ing_frame++;
		
		setRotation(getRotation() + rand_rotate);
		int checking_value = ing_frame%create_frame;
		if(checking_value == 0)
		{
			CCPoint s_p = getNumberPosition(sn);
			CCPoint f_p = getNumberPosition(fn);
			
			ReflectionLazer* t_rl = ReflectionLazer::create(s_p, f_p, create_frame, myType);
			addChild(t_rl, 1);
			t_rl->startMyAction();
			
			CCPoint subPosition = ccpSub(f_p, s_p);
			
			float rotate_value = -(atan2f(subPosition.y, subPosition.x)/M_PI*180.f + 90.f);
			float keep_rotate = effectImg->getRotation();
			
			effectImg->setRotation(rotate_value);
			effectImg->setPosition(ccpMidpoint(f_p, s_p));
			
			impactImg->setPosition(s_p);
			impactImg->setRotation(keep_rotate);
			impactImg->setOpacity(255);
			
			sn = fn;
			fn = (fn+direction)%9;
		}
		else if(checking_value == 4)
		{
			effectImg->setOpacity(255);
			impactImg->setOpacity(0);
		}
		else if(checking_value == 7)
		{
			effectImg->setOpacity(0);
		}
		
		if(ing_frame >= 900)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(MirrorReflection::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(int t_sn, float t_r, int t_direction, int t_type)
	{
		effectImg = CCSprite::create("lazer_effect.png");
//		ccBlendFunc t_b;
//		t_b.src = GL_SRC_ALPHA;
//		t_b.dst = GL_ONE;
//		effectImg->setBlendFunc(t_b);
		effectImg->setOpacity(0);
		addChild(effectImg, 2);
		
		impactImg = CCSprite::create("reflection_impact.png");
		impactImg->setAnchorPoint(ccp(0.5,0));
		impactImg->setOpacity(0);
		addChild(impactImg, 2);
		
		rand_rotate = rand()%2;
		if(rand_rotate == 0) rand_rotate = -1;
		base_angle = rand()%40;
		create_frame = rand()%5 + 9;
		myType = t_type;
		radius = t_r;
		sn = t_sn;
		if(t_direction == 1)
			direction = 4;
		else if(t_direction == -1)
			direction = 5;
		
		fn = (sn + direction)%9;
	}
};



class MeteorStorm : public CCNode
{
public:
	static MeteorStorm* create(CCPoint t_sp, CCPoint t_fp, int t_moving_frame)
	{
		MeteorStorm* t_ms = new MeteorStorm();
		t_ms->myInit(t_sp, t_fp, t_moving_frame);
		t_ms->autorelease();
		return t_ms;
	}
	
private:
	
	CCParticleSystemQuad* inner_fire_particle;
	CCParticleSystemQuad* outer_fire_particle;
	
	void myInit(CCPoint t_sp, CCPoint t_fp, int t_moving_frame)
	{
		

		inner_fire_particle = CCParticleSystemQuad::create("meteor_storm.plist");
		inner_fire_particle->setPositionType(kCCPositionTypeRelative);
		addChild(inner_fire_particle);
		
		outer_fire_particle	= CCParticleSystemQuad::createWithTotalParticles(100);
		outer_fire_particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("meteor_storm_particle.png");
		outer_fire_particle->setTexture(texture);
		outer_fire_particle->setEmissionRate(100.00);
		outer_fire_particle->setAngle(45.0);
		outer_fire_particle->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		outer_fire_particle->setBlendFunc(blendFunc);
		outer_fire_particle->setDuration(-1.0);
		outer_fire_particle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor = {1.00,0.98,0.40,1.00}; // 0.76 0.25 0.12
		outer_fire_particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		outer_fire_particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {1.00,0.07,0.03,1.00};
		outer_fire_particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		outer_fire_particle->setEndColorVar(endColorVar);
		outer_fire_particle->setStartSize(60.00);
		outer_fire_particle->setStartSizeVar(0.0);
		outer_fire_particle->setEndSize(80.0);
		outer_fire_particle->setEndSizeVar(0.0);
		outer_fire_particle->setGravity(ccp(-400,400));
		outer_fire_particle->setRadialAccel(0.0);
		outer_fire_particle->setRadialAccelVar(0.0);
		outer_fire_particle->setSpeed(0);
		outer_fire_particle->setSpeedVar(200.0);
		outer_fire_particle->setTangentialAccel(0);
		outer_fire_particle->setTangentialAccelVar(0);
		outer_fire_particle->setTotalParticles(100);
		outer_fire_particle->setLife(0.0);
		outer_fire_particle->setLifeVar(1.00);
		outer_fire_particle->setStartSpin(0.0);
		outer_fire_particle->setStartSpinVar(0.0);
		outer_fire_particle->setEndSpin(0.0);
		outer_fire_particle->setEndSpinVar(0.0);
		outer_fire_particle->setPosition(ccp(20,-20));
		outer_fire_particle->setPosVar(ccp(0,0));
		addChild(outer_fire_particle);

	}
};


class MathmaticalMissileUnit : public CCSprite
{
public:
	enum CurveDisposition{CURVE = 1, RIGHTLINE = 0};
	static MathmaticalMissileUnit* create(CCPoint t_sp, float t_angle, float t_speed, string imgFilename, CCSize t_cs,
																				const vector<CCPoint>& path, enum CurveDisposition curve)
	{
		MathmaticalMissileUnit* t_mu = new MathmaticalMissileUnit();
		if(t_mu && t_mu->initWithFile(imgFilename.c_str()))
		{
			t_mu->myInit(t_sp, t_angle, t_speed, t_cs, path, curve, imgFilename);
			t_mu->autorelease();
			return t_mu;
		}
		CC_SAFE_DELETE(t_mu);
		return NULL;
	}
	static MathmaticalMissileUnit* create(CCPoint t_sp, float t_angle, float t_speed, string imgFilename, CCSize t_cs)
	{
		MathmaticalMissileUnit* t_mu = new MathmaticalMissileUnit();
		if(t_mu && t_mu->initWithFile(imgFilename.c_str()))
		{
			t_mu->myInit(t_sp, t_angle, t_speed, t_cs, std::vector<CCPoint>(), CurveDisposition::RIGHTLINE, "");
			t_mu->autorelease();
			return t_mu;
		}
		CC_SAFE_DELETE(t_mu);
		return NULL;
	}

	// CatMull-rom spine ... t = [0, 1]  일때 P1 과 P2 사이의 위치를 반환하는 함수.
	CCPoint CatMull(CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, float t, CurveDisposition curve)
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
	double CatMullLength(CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, CurveDisposition curve)
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
	double IntegralTarget(CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, double t)
	{
		double dx = CatMullDerivativeFunction(2 * P1.x, (-P0.x + P2.x), (2*P0.x - 5*P1.x + 4*P2.x - P3.x), (-P0.x + 3*P1.x- 3*P2.x + P3.x), t);
		double dy = CatMullDerivativeFunction(2 * P1.y, (-P0.y + P2.y), (2*P0.y - 5*P1.y + 4*P2.y - P3.y), (-P0.y + 3*P1.y- 3*P2.y + P3.y), t);
		
		double dx2 = dx * dx;
		double dy2 = dy * dy;
		return sqrt(dx2 + dy2);
	}
	double CatMullFunction(double a, double b, double c, double d, double t, CurveDisposition curve)
	{
		// a + bt + ct2 + dt3;
		return 0.5 * (a + b*t + c*t*t + d*t*t*t);
	}
	double CatMullDerivativeFunction(double a, double b, double c, double d, double t)
	{
		return 0.5 * (b + 2*c*t + 3*d*t*t);
	}
	
	double smps(std::function<double(CCPoint, CCPoint, CCPoint, CCPoint, double)> f,
							CCPoint A, CCPoint B, CCPoint C, CCPoint D,
							double a, double b, int n)
	{
		double h = (b-a)/n, sum = 0;
		for(int i=0; i<n; i++)
		{
			sum += (f(A,B,C,D,a+i*h)+4*f(A,B,C,D,a+i*h + h/2) + f(A,B,C,D,a+(i+1)*h)) * h / 6;
		}
		return sum;
	}
	

	void removeEffect()
	{
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ThisClassType::removeFromParent));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		runAction(t_seq);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, const vector<CCPoint>& path, enum CurveDisposition curve, const std::string& fn)
	{
		m_frameCount = 0;
		m_isChecking = true;
		m_angle = t_angle;
		m_speed = t_distance;
		m_crashSize = t_cs;
		firePosition = t_sp;
		
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
		schedule(schedule_selector(ThisClassType::move));
	}

	CCPoint myFunction(float fc)
	{
		// 120 프레임에 한 싸이클...
		return ccp(fc, sinf(fc * 2*M_PI/60) * 30); // 120 이 주기이고 10이 높이.
//		return ccp(fc, 0);
	}
	void move(float dt)
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
		
 		
//		CCLog("dis %f - %d, len = %f, u %f", ccpLength(prevPosition - dv), m_catmullIndex, len, m_catmullvar);
		
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
//		CCLog("Dv = %f %f", dv.x, dv.y);
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
				if(myGD->getJackState())
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
					CCLog("%d %d", t.x, t.y);
					if(t.isInnerMap() && myGD->mapState[t.x][t.y] == mapType::mapNewline)
					{
						myGD->communication("PM_addPathBreaking", t);
					}
					
				}
				IntPoint t = ccp2ip(p_p);
				if(t.isInnerMap() && myGD->mapState[t.x][t.y] == mapType::mapNewline)
				{
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

protected:
	std::string m_fileName;
	vector<CCPoint> m_catmullPath;
	int m_catmullIndex;
	double m_catmullvar;
	CurveDisposition m_curve;
	CCPoint firePosition;
	float m_angle;
	float m_speed;
	CCSize m_crashSize;
	float m_isChecking;
	int m_frameCount;
};

#endif
