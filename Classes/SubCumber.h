//
//  SubCumber.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 20..
//
//

#ifndef __DrawingJack__SubCumber__
#define __DrawingJack__SubCumber__

#include "cocos2d.h"
#include "CumberBase.h"
#include "GameData.h"
#include "MissileDamageData.h"
#include "CumberEmotion.h"
#include "StarGoldData.h"
#include "AlertEngine.h"

using namespace cocos2d;

class SubCumber : public Cumber
{
public:
	static SubCumber* create()
	{
		SubCumber* t_SC = new SubCumber();
		t_SC->myInit();
		t_SC->autorelease();
		return t_SC;
	}
	
	static SubCumber* create(IntPoint s_p)
	{
		SubCumber* t_SC = new SubCumber();
		t_SC->myInit(s_p);
		t_SC->autorelease();
		return t_SC;
	}
	
	void startSpringCumber(float t_springAngle)
	{
		stopMoving();
		stopSpringCumber();
		cumberImg->setColor(ccRED);
		springCnt = 0;
		springAngle = t_springAngle;
		if(springAngle > -90.f && springAngle <= 90.f)
			cumberImg->setFlipX(false);
		else
			cumberImg->setFlipX(true);
		
		myScale -= 0.05f;
		
		if(myScale > maxScale)
			myScale = maxScale;
		else if(myScale < minScale)
			myScale = minScale;
		
		CCScaleTo* t_scale = CCScaleTo::create(0.5f, myScale);
		cumberImg->runAction(t_scale);
		
		springPower = 1.8;
		schedule(schedule_selector(SubCumber::springCumber));
	}
	
	void stopSpringCumber()
	{
		cumberImg->setColor(ccWHITE);
		unschedule(schedule_selector(SubCumber::springCumber));
	}
	
	void setGameover()
	{
		isGameover = true;
	}
	
	void startMoving()
	{
		myState = cumberStateMoving;
		
//		if(SelectedMapData::sharedInstance()->getSelectedChapter() <= 20)
//		{
//			directionAngle = (rand()%4)*90+45+(rand()%30 - 15);
//			schedule(schedule_selector(SubCumber::easyMoving));
//		}
//		else
//		{
			directionAngle = rand()%360;
			schedule(schedule_selector(SubCumber::moving));
//		}
	}
	void stopMoving()
	{
//		if(SelectedMapData::sharedInstance()->getSelectedChapter() <= 20)
//		{
//			unschedule(schedule_selector(SubCumber::easyMoving));
//		}
//		else
//		{
			unschedule(schedule_selector(SubCumber::moving));
//		}
		myState = cumberStateStop;
	}
	
	void decreaseLife(float t_damage, float t_directionAngle)
	{
		stopSelfBomb();
		life -= MissileDamageData::getCorrelationDamage(t_damage, my_element);
		if(life <= 0)
		{
			stopMoving();
			myGD->communication("CP_removeSubCumber", this);
			removeFromParentAndCleanup(true);
		}
		else
		{
			startSpringCumber(t_directionAngle);
		}
	}
	
	void startSelfBomb()
	{
		if(!isSelfBomb)
		{
			stopMoving();
			ingFrame = 0;
			isSelfBomb = true;
			self_bomb_cnt = 45 + rand()%40;
			schedule(schedule_selector(SubCumber::selfBomb));
		}
	}
	
	IntPoint getMapPoint()
	{
		return mapPoint;
	}
	
	void createSheild()
	{
		if(isSheild == 0)
		{
			isSheild = 1;
			sheildImg = CCSprite::create("sheild.png", CCRectMake(0, 0, 90, 90));
			sheildImg->setScale(0.5);
			addChild(sheildImg);
			
			CCSprite* t_texture = CCSprite::create("sheild.png");
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<4;i++)
			{
				t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*90, 0, 90, 90));
			}
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
			
			sheildImg->runAction(t_repeat);
		}
	}
	
	void crashSheild()
	{
		if(isSheild == 1)
		{
			isSheild = 0;
			sheildImg->removeFromParentAndCleanup(true);
		}
	}
	
	void showEmotion(EmotionType t_type)
	{
		if(mEmotion)	mEmotion->selfRemove();
		mEmotion = Emotion::create(t_type, this, callfunc_selector(SubCumber::nullmEmotion));
		mEmotion->setPosition(ccp(20,15));
		addChild(mEmotion);
	}
	
	void nullmEmotion()
	{
		mEmotion = NULL;
	}
	
	void changeMaxSize(float t_p)
	{
		if(t_p < 0.1f)
			maxScale = 1.2f;
		else if(t_p > 0.85f)
			maxScale = 0.4f;
		else
		{
			maxScale = 1.4f - ((t_p - 0.1f) / (0.85f-0.1f));
		}
	}
	
private:
	GameData* myGD;
	int life;
	int ingFrame;
	bool isSelfBomb;
	ElementCode my_element;
	int springCnt;
	float springAngle;
	double springPower;
	bool isGameover;
	int self_bomb_cnt;
	
	bool isMainType;
	
	Emotion* mEmotion;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void moving();
	void easyMoving();
	void searchAfterDirection();
	CCPoint getAfterPosition(int a_angle);
	
	void caughtAnimation();
	void caughtSpinAnimation();
	void caughtSelfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void springCumber()
	{
		springCnt++;
		
		CCPoint movePosition;
		movePosition.x = 1.f;
		movePosition.y = tanf(springAngle/180.f*M_PI);
		
		if(springAngle > 90 && springAngle < 270)
			movePosition = ccpMult(movePosition, -1);
		
		float  collectionValue = sqrtf(powf(movePosition.x, 2.f) + powf(movePosition.y, 2.f));
		movePosition = ccpMult(movePosition, 1/collectionValue);
		movePosition = ccpMult(movePosition, springPower);
		
		movePosition = ccpAdd(getPosition(), movePosition);
		
		IntPoint movePoint;
		movePoint.x = int(roundf((movePosition.x-1)/pixelSize + 1.f));
		movePoint.y = int(roundf((movePosition.y-1)/pixelSize + 1.f));
		
		IntPoint half_movePoint;
		half_movePoint.x = int(roundf(((movePosition.x-getPositionX())/2.f+getPositionX()-1)/pixelSize + 1.f));
		half_movePoint.y = int(roundf(((movePosition.y-getPositionY())/2.f+getPositionY()-1)/pixelSize + 1.f));
		
		if(movePosition.x < 0 || movePosition.x > 320 || movePosition.y < 0 || movePosition.y > 430 || !movePoint.isInnerMap() ||
		   myGD->mapState[movePoint.x][movePoint.y] != mapEmpty || myGD->mapState[half_movePoint.x][half_movePoint.y] != mapEmpty)
		{
			stopSpringCumber();
			if(!isGameover)
				startMoving();
			return;
		}
		
		
		setPosition(movePosition);
		
		springPower -= 0.1;
		
		if(springCnt >= 30)
		{
			stopSpringCumber();
			if(!isGameover)
				startMoving();
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
		particle->setPosition(CCPointZero);
		particle->setPosVar(CCPointZero);
		addChild(particle);
	}
	
	void selfBomb()
	{
		ingFrame++;
		
		ccColor3B t_color = cumberImg->getColor();
		t_color.g-=3;
		t_color.b-=3;
		
		cumberImg->setColor(t_color);
		
		if(ingFrame >= self_bomb_cnt)
		{
			unschedule(schedule_selector(SubCumber::selfBomb));
			AudioEngine::sharedInstance()->playEffect("sound_tickingTimeBomb.mp3", false);
			initParticle();
			
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
//			for(int i=-14;i<=14;i++)
//			{
//				int addValue;
//				if(i == -14 || i == 14)									addValue = 3;
//				else if(i == -13 || i == 13)							addValue = 6;
//				else if(i == -12 || i == 12)							addValue = 8;
//				else if(i == -11 || i == 11)							addValue = 9;
//				else if(i == -10 || i == 10)							addValue = 10;
//				else if(i == -9 || i == 9)								addValue = 11;
//				else if(i == -8 || i == 8 || i == -7 || i == 7)			addValue = 12;
//				else if((i >= -6 && i <= -4) || (i >= 4 && i <= 6))		addValue = 13;
//				else													addValue = 14;
//				for(int j=-addValue;j<=addValue;j++)
//				{
//					crashMapForIntPoint(IntPoint(mapPoint.x+i, mapPoint.y+j));
//				}
//			}
			
			CCDelayTime* t_delay = CCDelayTime::create(0.5);
			CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(SubCumber::selfRemove));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call2);
			runAction(t_seq);
		}
	}
	
	void selfRemove()
	{
		myGD->communication("CP_removeSubCumber", this);
		removeFromParentAndCleanup(true);
	}
	
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
			myGD->communication("MFP_createNewFragment", t_p);
			myGD->communication("VS_divideRect", t_p);
		}
		
		if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
		{
			myGD->communication("Jack_startDieEffect");
		}
		
		if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
		{
			//					myGD->communication("PM_pathChainBomb", t_p);
			myGD->communication("Jack_startDieEffect");
			myGD->communication("Main_showLineDiePosition", t_p);
		}
	}
	
	void stopSelfBomb()
	{
		if(isSelfBomb)
		{
			unschedule(schedule_selector(SubCumber::selfBomb));
			cumberImg->setColor(ccWHITE);
			isSelfBomb = false;
		}
	}
	
	void myInit()
	{
		mEmotion = NULL;
		isGameover = false;
		is_slowed = false;
		isSheild = 0;
		life = SelectedMapData::sharedInstance()->getMaxBossLife()/10.f;
		isSelfBomb = false;
		myGD = GameData::sharedGameData();
		myState = cumberStateStop;
//		move_speed = SelectedMapData::sharedInstance()->getSubCumberSpeed();
		move_speed = 1.f;
		
//		int worldMap = SelectedMapData::sharedInstance()->getSelectedChapter();
//		int secondType = SelectedMapData::sharedInstance()->getSecondSubCumberType();
//		
//		isMainType = StarGoldData::sharedInstance()->getIsSubType();
//		int resultType = isMainType ? worldMap : secondType;
		int resultType = 20;
		
		if(resultType <= 20)
		{
			if((resultType-1)%10/2+1 == 1)				my_element = kElementCode_life;
			else if((resultType-1)%10/2+1 == 2)			my_element = kElementCode_fire;
			else if((resultType-1)%10/2+1 == 3)			my_element = kElementCode_water;
			else if((resultType-1)%10/2+1 == 4)			my_element = kElementCode_water;
			else if((resultType-1)%10/2+1 == 5)			my_element = kElementCode_fire;
		}
		else
		{
			int last_p = resultType%10;
			if(last_p == 1)				my_element = kElementCode_empty;
			else if(last_p == 2)		my_element = kElementCode_water;
			else if(last_p == 3)		my_element = kElementCode_life;
			else if(last_p == 4)		my_element = kElementCode_life;
			else if(last_p == 5)		my_element = kElementCode_fire;
			else if(last_p == 6)		my_element = kElementCode_water;
		}
		
//		if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 1 && SelectedMapData::sharedInstance()->getSelectedStage() == 1)
//		{
//			cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_1_monster.png", worldMap)->getCString());
//			addChild(cumberImg);
//			my_element = kElementCode_empty;
//		}
//		else
//		{
			cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_monster.png", resultType)->getCString());
			addChild(cumberImg);
//		}
		
		myScale = 1.f;
		maxScale = 1.2f;
		minScale = 0.4f;
		
		areacrash_frame_cnt = 0;
		move_frame = 0;
		
		bool isGoodPointed = false;
		int check_loop_cnt = 0;
		while(!isGoodPointed)
		{
			check_loop_cnt++;
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "subcumber init infinity loop", "ok", 1, this, alertfuncII_selector(SubCumber::alertAction));
			
			mapPoint.x = rand()%160 + 1;
			mapPoint.y = rand()%215 + 1;
			
			if(mapPoint.isInnerMap() && myGD->mapState[mapPoint.x][mapPoint.y] == mapEmpty)
				isGoodPointed = true;
		}
		
		setPosition(ccp((mapPoint.x-1)*pixelSize + 1,(mapPoint.y-1)*pixelSize + 1));
	}
	
	void myInit(IntPoint s_p)
	{
		mEmotion = NULL;
		is_slowed = false;
		isSheild = 0;
		life = SelectedMapData::sharedInstance()->getMaxBossLife()/10.f;
		isSelfBomb = false;
		myGD = GameData::sharedGameData();
		myState = cumberStateStop;
		move_speed = SelectedMapData::sharedInstance()->getSubCumberSpeed();
		
		int worldMap = SelectedMapData::sharedInstance()->getSelectedChapter();
		int secondType = SelectedMapData::sharedInstance()->getSecondSubCumberType();
		
		isMainType = StarGoldData::sharedInstance()->getIsSubType();
		int resultType = isMainType ? worldMap : secondType;
		
		if(resultType <= 20)
		{
			if((resultType-1)%10/2+1 == 1)				my_element = kElementCode_life;
			else if((resultType-1)%10/2+1 == 2)			my_element = kElementCode_fire;
			else if((resultType-1)%10/2+1 == 3)			my_element = kElementCode_water;
			else if((resultType-1)%10/2+1 == 4)			my_element = kElementCode_water;
			else if((resultType-1)%10/2+1 == 5)			my_element = kElementCode_fire;
		}
		else
		{
			int last_p = resultType%10;
			if(last_p == 1)				my_element = kElementCode_empty;
			else if(last_p == 2)		my_element = kElementCode_water;
			else if(last_p == 3)		my_element = kElementCode_life;
			else if(last_p == 4)		my_element = kElementCode_life;
			else if(last_p == 5)		my_element = kElementCode_fire;
			else if(last_p == 6)		my_element = kElementCode_water;
		}
		
		if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 1 && SelectedMapData::sharedInstance()->getSelectedStage() == 1)
		{
			cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_1_monster.png", worldMap)->getCString());
			addChild(cumberImg);
			my_element = kElementCode_empty;
		}
		else
		{
			cumberImg = CCSprite::create(CCString::createWithFormat("chapter%d_monster.png", resultType)->getCString());
			addChild(cumberImg);
		}
		
		mapPoint = s_p;
		
		setPosition(ccp((mapPoint.x-1)*pixelSize + 1,(mapPoint.y-1)*pixelSize + 1));
	}
};

#endif /* defined(__DrawingJack__SubCumber__) */
