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
#include "StarGoldData.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace std;

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
			
			if(missile_rect.containsPoint(jackPosition)) //  && myGD->getJackState()
			{
				is_checking = false;
//				if(mySGD->getIsHard() || myGD->getJackState())
				if(myGD->getJackState())
				{
					myGD->communication("CP_jackCrashDie");
					myGD->communication("Jack_startDieEffect");
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
					myGD->communication("Jack_startDieEffect");
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

class MissileUnit3 : public CCSprite
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
			myGD->communication("Jack_startDieEffect");
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
			initWithFile("fallingStone.png", CCRectMake(0, 0, 35, 35));
			
			CCSprite* animation_sprite = CCSprite::create("fallingStone.png");
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<3;i++)
			{
				t_animation->addSpriteFrameWithTexture(animation_sprite->getTexture(), CCRectMake(i*35, 0, 35, 35));
			}
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
			
			runAction(t_repeat);
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
				myGD->communication("Jack_startDieEffect");
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
			myGD->communication("EP_crashed");
			myGD->communication("MFP_createNewFragment", t_p);
			myGD->communication("VS_divideRect", t_p);
		}
		
		if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect");
			jackDie();
		}
		
		if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
		{
			//					myGD->communication("PM_pathChainBomb", t_p);
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect");
			lineDie(t_p);
		}
	}
	
	virtual void jackDie() = 0;
	virtual void lineDie(IntPoint t_p) = 0;
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
		
		for(int i=beamPoint.y-10;i<=beamPoint.y+10;i++)
		{
			int up_down_value;
			if(i == beamPoint.y-10 || i == beamPoint.y+10)			up_down_value = 6;
			else if(i == beamPoint.y-9 || i == beamPoint.y+9)		up_down_value = 11;
			else if(i == beamPoint.y-8 || i == beamPoint.y+8)		up_down_value = 13;
			else if(i == beamPoint.y-7 || i == beamPoint.y+7)		up_down_value = 15;
			else if(i == beamPoint.y-6 || i == beamPoint.y+6)		up_down_value = 17;
			else if(i == beamPoint.y-5 || i == beamPoint.y+5)		up_down_value = 18;
			else if(i == beamPoint.y-4 || i == beamPoint.y+4 || i == beamPoint.y-3 || i == beamPoint.y+3)		up_down_value = 19;
			else if(i >= beamPoint.y-2 && i <= beamPoint.y+2)		up_down_value = 20;
			
			for(int j=beamPoint.x-up_down_value;j<=beamPoint.x+up_down_value;j++)
			{
				crashMapForIntPoint(IntPoint(j,i));
			}
		}
	}
	
private:
	CCSprite* beam_main;
	CCSprite* beam_wave;
	int type;
	
	CCObject* target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	
	void jackDie()
	{
		unschedule(schedule_selector(SatelliteBeam::fallingStar));
		removeEffect();
	}
	void lineDie(IntPoint t_p)
	{
		unschedule(schedule_selector(SatelliteBeam::fallingStar));
		myGD->communication("Main_showLineDiePosition", t_p);
		removeEffect();
	}
	
	void removeEffect()
	{
		(target_removeEffect->*delegate_removeEffect)();
		
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(SatelliteBeam::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		CCFadeTo* t_fade2 = CCFadeTo::create(1.f, 0);
		
		beam_main->runAction(t_seq);
		beam_wave->runAction(t_fade2);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
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
		beam_main = CCSprite::create(CCString::createWithFormat("satelliteBeam_main%d.png", type)->getCString());
		beam_main->setAnchorPoint(ccp(0.5,0.036));
		addChild(beam_main);
		
		beam_wave = CCSprite::create(CCString::createWithFormat("satelliteBeam_wave%d.png", type)->getCString(), CCRectMake(0, 0, 80, 40));
		
		CCSprite* t_texture = CCSprite::create(CCString::createWithFormat("satelliteBeam_wave%d.png", type)->getCString());
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<3;i++)
		{
			t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*80, 0, 80, 40));
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		beam_wave->runAction(t_repeat);
		
		addChild(beam_wave);
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
		
		targetingImg = CCSprite::create(("meteor_targeting" + imgFilename).c_str());
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
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(FallMeteor::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		meteor->runAction(t_seq);
	}
	
private:
	
	CCSprite* meteor;
	string imgFilename;
	
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
		unschedule(schedule_selector(FallMeteor::fall));
		(target_removeEffect->*delegate_removeEffect)();
		removeEffect();
	}
	
	void lineDie(IntPoint t_p)
	{
		myGD->communication("Main_showLineDiePosition", t_p);
		unschedule(schedule_selector(FallMeteor::fall));
		(target_removeEffect->*delegate_removeEffect)();
		removeEffect();
	}
	
	void startFall()
	{
		ingFrame = 0;
		AudioEngine::sharedInstance()->playEffect("sound_meteor.mp3", false);
		schedule(schedule_selector(FallMeteor::fall));
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
		
		meteor->setPosition(ccpAdd(meteor->getPosition(),fall_dv));
		
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
		
		
		if(ingFrame >= fallFrame)
		{
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
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void initParticle()
	{
		CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("meteor_bomb.png");
		particle->setTexture(texture);
		particle->setEmissionRate(250.00);
		particle->setAngle(90.0);
		particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(0.20);
		particle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor;
		if(imgFilename == "1.png")
		{
			startColor.r = 0.87;
			startColor.g = 0.81;
			startColor.b = 0.12;
			startColor.a = 1.00;
		}
		else
		{
			startColor.r = 0.4;
			startColor.g = 0.8;
			startColor.b = 1.0;
			startColor.a = 1.0;
		}
		particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		particle->setStartColorVar(startColorVar);
		ccColor4F endColor;
		if(imgFilename == "2.png")
		{
			endColor.r = 0.68;
			endColor.g = 0.16;
			endColor.b = 0.00;
			endColor.a = 1.00;
		}
		else
		{
			endColor.r = 0.00;
			endColor.g = 0.50;
			endColor.b = 1.00;
			endColor.a = 1.00;
		}
		particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0};
		particle->setEndColorVar(endColorVar);
		particle->setStartSize(20.00);
		particle->setStartSizeVar(10.0);
		particle->setEndSize(50.0);
		particle->setEndSizeVar(10.0);
		particle->setGravity(ccp(0,0));
		particle->setRadialAccel(0.0);
		particle->setRadialAccelVar(0.0);
		particle->setSpeed(200);
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
		particle->setPosition(meteor->getPosition());
		particle->setPosVar(CCPointZero);
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
		
		fall_dv = ccpSub(fp, t_sp);
		fall_dv = ccpMult(fall_dv, 1.f/fallFrame);
		
		meteor = CCSprite::create(("meteor_stone_test" + imgFilename).c_str(), CCRectMake(0, 0, imgFrameSize.width, imgFrameSize.height));
		meteor->setPosition(t_sp);
		
		CCSprite* t_t = CCSprite::create(("meteor_stone_test" + imgFilename).c_str());
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<imgFrameCnt;i++)
			t_animation->addSpriteFrameWithTexture(t_t->getTexture(), CCRectMake(i*imgFrameSize.width, 0, imgFrameSize.width, imgFrameSize.height));
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		meteor->runAction(t_repeat);
		
		addChild(meteor);
		
		int random_sign;
		if(rand()%2)
			random_sign = 360;
		else
			random_sign = -360;
		
		FM_Targeting* t_fmt = FM_Targeting::create(imgFilename, t_fp, t_fallFrame, 3.0, 0.7, 0, random_sign);
		addChild(t_fmt);
		
		t_fmt->startAction();
		
		startFall();
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
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade1, t_call2);
		baseNode->runAction(t_seq);
		
		CCFadeTo* t_fade2 = CCFadeTo::create(1.f, 0);
		colorControl->runAction(t_fade2);
		
		CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
		cntLabel->runAction(t_fade3);
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
		CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
		particle->setPositionType(kCCPositionTypeRelative);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("threeCushion_bomb.png");
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
		particle->setPosition(baseNode->getPosition());
		particle->setPosVar(CCPointZero);
		addChild(particle);
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
			myGD->communication("SW_createJDSW", setPoint, NULL, NULL);
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
		
		ticking_main = CCSprite::create(CCString::createWithFormat("tickingTimeBomb_main_%d.png", rangeCode)->getCString());
		addChild(ticking_main);
		
		if(rangeCode == 1)
		{
			CCSprite* ticking_pad = CCSprite::create("tickingTimeBomb_pad.png");
			ticking_pad->setPosition(ccp(17,12));
			ticking_main->addChild(ticking_pad);
			
			ticking_label = CCLabelAtlas::create(CCString::createWithFormat("%d",bombTimes)->getCString(), "tickingTimeBomb_number.png", 9, 11, '1');
			ticking_label->setPosition(ccp(1,3));
			ticking_pad->addChild(ticking_label);
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

class BlindDrop : public CCNode
{
public:
	static BlindDrop* create(CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame)
	{
		BlindDrop* t_bd = new BlindDrop();
		t_bd->myInit(t_sp, t_fp, t_movingFrame, t_blindFrame);
		t_bd->autorelease();
		return t_bd;
	}
	
	void startAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_throw_obj_shot.mp3", false);
		ingFrame = 0;
		schedule(schedule_selector(BlindDrop::myAction));
	}
	
private:
	CCPoint subPosition;
	int movingFrame;
	int blindFrame;
	int ingFrame;
	
	CCSprite* dropImg;
	CCSprite* oilImg;
	
	
	void myAction()
	{
		ingFrame++;
		
		if(ingFrame <= movingFrame)
		{
			setPosition(ccpAdd(getPosition(), subPosition));
			
			if(ingFrame == movingFrame)
			{
				int rand_value = rand()%3 + 1;
				oilImg = CCSprite::create(CCString::createWithFormat("blind_oil%d.png", rand_value)->getCString());
				oilImg->setScale(0.2);
				addChild(oilImg);
				
				CCScaleTo* t_scale = CCScaleTo::create(0.3, 1.f);
				
				oilImg->runAction(t_scale);
				
				dropImg->removeFromParentAndCleanup(true);
			}
		}
		else if(ingFrame <= movingFrame+(blindFrame/2))
		{
			
		}
		else if(ingFrame <= movingFrame+blindFrame)
		{
			oilImg->setOpacity(oilImg->getOpacity()-(200/(blindFrame/2)));
		}
		
		
		if(ingFrame >= movingFrame+blindFrame)
		{
			stopAction();
		}
	}
	
	void stopAction()
	{
		unschedule(schedule_selector(BlindDrop::myAction));
		oilImg->removeFromParentAndCleanup(true);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame)
	{
		subPosition = ccpSub(t_fp, t_sp);
		subPosition = ccpMult(subPosition, 1.f/t_movingFrame);
		movingFrame = t_movingFrame;
		blindFrame = t_blindFrame;
		
		dropImg = CCSprite::create("blind_drop.png");
		addChild(dropImg);
		
		setPosition(t_sp);
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
			myGD->communication("Jack_startDieEffect");
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
		(target_removeEffect->*delegate_removeEffect)();
		removeFromParentAndCleanup(true);
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
			myGD->communication("Jack_startDieEffect");
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
				BurnFragment* t_bf = BurnFragment::create(mapPoint, 5, getParent(), mType, this, callfunc_selector(Burn::removeEffect));
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

class PoisonLine : public CCSprite
{
public:
	static PoisonLine* create(IntPoint t_sp)
	{
		PoisonLine* t_pl = new PoisonLine();
		t_pl->myInit(t_sp);
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
	
	void myAction()
	{
		ingFrame++;
		
		ccColor3B t_color = getColor();
		t_color.r += rand()%3-1;
		t_color.g += rand()%3-1;
		t_color.b += rand()%3-1;
		
		setColor(t_color);
		
		int emptyCnt = 0;
		if(myGD->mapState[mapPoint.x-1][mapPoint.y] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[mapPoint.x+1][mapPoint.y] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[mapPoint.x][mapPoint.y-1] == mapEmpty)	emptyCnt++;
		if(myGD->mapState[mapPoint.x][mapPoint.y+1] == mapEmpty)	emptyCnt++;
		
		if(emptyCnt == 0)
		{
			stopMyAction();
			return;
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		if(jackPoint.x == mapPoint.x && jackPoint.y == mapPoint.y)
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect");
			stopMyAction();
			return;
		}
		
		if(ingFrame >= 300)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(PoisonLine::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(IntPoint t_sp)
	{
		mapPoint = t_sp;
		
		initWithFile("poison_line.png");
		CCPoint myPosition = ccp((t_sp.x-1)*pixelSize+1, (t_sp.y-1)*pixelSize+1);
		setPosition(myPosition);
		
		setColor(ccc3(0, 66, 75));
	}
};

class PoisonDrop : public CCNode
{
public:
	static PoisonDrop* create(CCPoint t_sp, CCPoint t_fp, int t_movingFrame)
	{
		PoisonDrop* t_bd = new PoisonDrop();
		t_bd->myInit(t_sp, t_fp, t_movingFrame);
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
				
				for(int i=-10;i<=10;i++)
				{
					for(int j=-10;j<=10;j++)
					{
						IntPoint checkPoint = IntPoint(basePoint.x+i,basePoint.y+j);
						if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapOldline)
						{
							PoisonLine* t_pl = PoisonLine::create(IntPoint(checkPoint.x, checkPoint.y));
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
	
	void myInit(CCPoint t_sp, CCPoint t_fp, int t_movingFrame)
	{
		
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



#endif
