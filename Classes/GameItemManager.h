//
//  GameItemManager.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#ifndef __galsprototype__GameItemManager__
#define __galsprototype__GameItemManager__

#include "cocos2d.h"
#include "GameData.h"
#include "AlertEngine.h"
#include "AttackItem.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include <deque>
#include "LogData.h"

USING_NS_CC;
using namespace std;

enum GameItemType{
	kGIT_speedup = 1,
	kGIT_attack,
	kGIT_fast,
	kGIT_bomb
};

enum GameItemAttackType{
	kGIAT_empty = 0,
	kGIAT_life,
	kGIAT_fire,
	kGIAT_water,
	kGIAT_wind,
	kGIAT_lightning,
	kGIAT_plasma
};

enum GameItemBombType{
	kGIBT_life = 1,
	kGIBT_fire,
	kGIBT_water,
	kGIBT_wind,
	kGIBT_lightning,
	kGIBT_plasma
};

class GameItemBase : public CCNode
{
public:
	virtual void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	virtual void startFraming()
	{
		frame_cnt = 0;
		schedule(schedule_selector(GameItemBase::framing));
	}
	
protected:
	virtual void acting() = 0;
	
	int holding_time;
	int frame_cnt;
	int starting_side_cnt;
	
	CCSprite* item_img;
	IntPoint myPoint;
	
	virtual void framing()
	{
		frame_cnt++;
		
		if(getSideCount() > starting_side_cnt)
		{
			if(mySD->getClearCondition() == kCLEAR_itemCollect)
				myGD->communication("UI_takeItemCollect");
			
			item_img->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(GameItemBase::framing));
			
			acting();
		}
		
		
		if(holding_time <= frame_cnt)
		{
			unschedule(schedule_selector(GameItemBase::framing));
			startHide();
		}
	}
	
	int getSideCount()
	{
		int side_cnt = 0;
		IntPoint check_point;
		IntVector t_v;
		
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
		
		t_v = IntVector::directionVector(directionLeftDown);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		t_v = IntVector::directionVector(directionRightDown);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		t_v = IntVector::directionVector(directionRightUp);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		t_v = IntVector::directionVector(directionLeftUp);
		check_point = IntPoint(myPoint.x + t_v.dx , myPoint.y + t_v.dy);
		if(!check_point.isInnerMap() || (myGD->mapState[check_point.x][check_point.y] == mapOldget || myGD->mapState[check_point.x][check_point.y] == mapOldline))		side_cnt++;
		
		return side_cnt;
	}
	
	void startHide()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.5f, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
	}
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void setMyPoint(bool is_near)
	{
		if(is_near)
		{
			bool foundPoint = false;
			int check_loop_cnt = 0;
			IntPoint jack_point = myGD->getJackPoint();
			
			int random_value;
			
			while(!foundPoint)
			{
				random_value = rand()%30 - 15;
				if(random_value >= 0)		random_value += 9;
				else if(random_value < 0)	random_value -= 9;
				
				myPoint.x = jack_point.x + random_value;
				
				random_value = rand()%30 - 15;
				if(random_value >= 0)		random_value += 9;
				else if(random_value < 0)	random_value -= 9;
				
				myPoint.y = jack_point.y + random_value;
				
				if(myPoint.isInnerMap() && myGD->mapState[myPoint.x][myPoint.y] == mapEmpty)
				{
					foundPoint = true;
				}
				check_loop_cnt++;
				if(check_loop_cnt > 1000)
				{
					myPoint.x = -1;
					myPoint.y = -1;
					break;
				}
			}
		}
		else
		{
			bool foundPoint = false;
			int check_loop_cnt = 0;
			while(!foundPoint)
			{
				myPoint.x = rand()%(mapWidthInnerEnd-mapWidthInnerBegin+1) + mapWidthInnerBegin;
				myPoint.y = rand()%(mapHeightInnerEnd-mapHeightInnerBegin+1) + mapHeightInnerBegin;
				
				if(myPoint.isInnerMap() && myGD->mapState[myPoint.x][myPoint.y] == mapEmpty)
				{
					foundPoint = true;
				}
				check_loop_cnt++;
				if(check_loop_cnt > 1000)
				{
					myPoint.x = -1;
					myPoint.y = -1;
					break;
				}
			}
		}
	}
};

class GameItemAddTime : public GameItemBase
{
public:
	static GameItemAddTime* create(bool is_near)
	{
		GameItemAddTime* t_giat = new GameItemAddTime();
		t_giat->myInit(is_near);
		t_giat->autorelease();
		return t_giat;
	}
	
	virtual void selfRemove()
	{
		GameItemAddTime* recreate = GameItemAddTime::create(false);
		getParent()->addChild(recreate);
		
		GameItemBase::selfRemove();
	}
	
private:
	
	virtual void acting()
	{
		myGD->communication("UI_takeAddTimeItem");
		myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "addTime");
		
		GameItemAddTime* recreate = GameItemAddTime::create(false);
		getParent()->addChild(recreate);
		
		removeFromParent();
	}
	
	void myInit(bool is_near)
	{
		holding_time = rand()%10 + 20;
		holding_time *= 60;
		
		setMyPoint(is_near);
		
		if(myPoint.isNull())
		{
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			return;
		}
		
		item_img = CCSprite::create("item3.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		startFraming();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f*0.7f);
		
		item_img->runAction(t_scale);
	}
};

class GameItemSpeedUp : public GameItemBase
{
public:
	static GameItemSpeedUp* create(bool is_near)
	{
		GameItemSpeedUp* t_gisu = new GameItemSpeedUp();
		t_gisu->myInit(is_near);
		t_gisu->autorelease();
		return t_gisu;
	}
	
private:
	
	virtual void acting()
	{
		myGD->communication("Jack_takeSpeedUpItem");
		myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "speedUp");
		removeFromParent();
	}
	
	void myInit(bool is_near)
	{
		holding_time = rand()%10 + 20;
		holding_time *= 60;
		
		setMyPoint(is_near);
		
		if(myPoint.isNull())
		{
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			return;
		}
		
		item_img = CCSprite::create("item2.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		startFraming();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f*0.7f);
		
		item_img->runAction(t_scale);
	}
};

class GameItemFast : public GameItemBase
{
public:
	static GameItemFast* create(bool is_near)
	{
		GameItemFast* t_gisu = new GameItemFast();
		t_gisu->myInit(is_near);
		t_gisu->autorelease();
		return t_gisu;
	}
	
private:
	
	virtual void acting()
	{
		myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "Fast");
		
		myGD->setAlphaSpeed(myGD->getAlphaSpeed() + 10.f);
		
		CCDelayTime* t_delay = CCDelayTime::create(SDS_GI(kSDF_stageInfo, mySD->getSilType(), "itemOption_fast_sec"));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemFast::ending));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
	
	void ending()
	{
		myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 10.f);
		removeFromParentAndCleanup(true);
	}
	
	void myInit(bool is_near)
	{
		holding_time = rand()%10 + 20;
		holding_time *= 60;
		
		setMyPoint(is_near);
		
		if(myPoint.isNull())
		{
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			return;
		}
		
		item_img = CCSprite::create("item4.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		startFraming();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f*0.7f);
		
		item_img->runAction(t_scale);
	}
};

class GameItemAttack : public GameItemBase
{
public:
	static GameItemAttack* create(bool is_near)
	{
		GameItemAttack* t_gisu = new GameItemAttack();
		t_gisu->myInit(is_near);
		t_gisu->autorelease();
		return t_gisu;
	}
	
private:
	int my_elemental;
	
	virtual void acting()
	{
		myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "attack");
		
		myGD->communication("MP_createJackMissile", my_elemental, rand()%3 + 1, float(NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard))));
		removeFromParent();
	}
	
	void myInit(bool is_near)
	{
//		my_elemental = rand()%kElementCode_plasma + 1;
		my_elemental = rand()%7 + (rand()%9)*10;
		
		holding_time = rand()%10 + 20;
		holding_time *= 60;
		
		setMyPoint(is_near);
		
		if(myPoint.isNull())
		{
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			return;
		}
		
		item_img = CCSprite::create("item1.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		startFraming();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f*0.7f);
		
		item_img->runAction(t_scale);
	}
};

class GameItemSubOneDie : public GameItemBase
{
public:
	static GameItemSubOneDie* create(bool is_near)
	{
		GameItemSubOneDie* t_gisod = new GameItemSubOneDie();
		t_gisod->myInit(is_near);
		t_gisod->autorelease();
		return t_gisod;
	}
	
private:
	
	virtual void acting()
	{
		myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "subOneDie");
		myGD->communication("MP_subOneDie");
		removeFromParent();
	}
	
	void myInit(bool is_near)
	{
		holding_time = rand()%10 + 20;
		holding_time *= 60;
		
		setMyPoint(is_near);
		
		if(myPoint.isNull())
		{
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			return;
		}
		
		item_img = CCSprite::create("item6.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		startFraming();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f*0.7f);
		
		item_img->runAction(t_scale);
	}
};

class GameItemSilence : public GameItemBase
{
public:
	static GameItemSilence* create(bool is_near)
	{
		GameItemSilence* t_gisod = new GameItemSilence();
		t_gisod->myInit(is_near);
		t_gisod->autorelease();
		return t_gisod;
	}
	
private:
	
	virtual void acting()
	{
		myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "silence");
		myGD->communication("CP_silenceItem", true);
		CCDelayTime* t_delay = CCDelayTime::create(mySD->getSilenceItemOption());
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemSilence::finalAction));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
//		removeFromParent();
	}
	
	void finalAction()
	{
		myGD->communication("CP_silenceItem", false);
		removeFromParent();
	}
	
	void myInit(bool is_near)
	{
		holding_time = rand()%10 + 20;
		holding_time *= 60;
		
		setMyPoint(is_near);
		
		if(myPoint.isNull())
		{
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			return;
		}
		
		item_img = CCSprite::create("item8.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		startFraming();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f*0.7f);
		
		item_img->runAction(t_scale);
	}
};

class GameItemFire : public GameItemBase
{
public:
	static GameItemFire* create(bool is_near)
	{
		GameItemFire* t_gisu = new GameItemFire();
		t_gisu->myInit(is_near);
		t_gisu->autorelease();
		return t_gisu;
	}
	
private:
	int my_elemental;
	int damage_frame;
	int ing_frame;
	
	float damage;
	float total_damage;
	float sum_damage;
	
	virtual void acting()
	{
		myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "fire");
//		myGD->communication("EP_addJackAttack");
		AudioEngine::sharedInstance()->playEffect("sound_pil_fire.mp3",false);
		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
		unschedule(schedule_selector(GameItemFire::framing));
		//			myGD->communication("Main_startSpecialAttack");
		FireStorm* t_fs = FireStorm::create();
		addChild(t_fs);
		t_fs->startMyAction();
		
		damage_frame = 0;
		schedule(schedule_selector(GameItemFire::resetChecking));
	}
	
	void attack(float t_damage)
	{
		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
		
		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f));
		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
		
//		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
		{
//			myGD->communication("UI_subBossLife", t_damage); //## 보스쪽에서 호출
			
		}
		
		//## 아래에서 부하 루프 돌면서 다 "CP_startDamageReaction 호출 시킴.
		
//		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//		int cumber_cnt = subCumberArray->count();
//		for(auto iter = subArray.begin(); iter != subArray.end(); )
//		{
//			// 작업1
//			
//			if(지워졌는가)
//			{
//				;
//			}
//			else
//				++iter;
//		}
//		std::vector<std::string>::size_type i = 0;
//		while ( i < v.size() ) {
//			if ( shouldBeRemoved( v[i] ) ) {
//				v.erase( v.begin() + i );
//			} else {
//				++i;
//			}
//		}
		myGD->communication("CP_startDamageReaction", mainCumber, t_damage, rand()%360-180.f);
		
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumber_cnt = subCumberArray->count();
		for(int i=0;i<cumber_cnt;i++)
		{
			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f)); // ccpoint
			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
			myGD->communication("CP_startDamageReaction", t_subCumber, t_damage, rand()%360-180.f);
			
			if(subCumberArray->count() < cumber_cnt)
			{
				cumber_cnt--;
				i--;
			}
		}
	}
	
	void resetChecking()
	{
		damage_frame++;
		if(damage_frame >= 30 && damage_frame <= 210)
		{
			if((damage_frame-30)%9 == 0)
			{
				attack(total_damage/24.f);
				sum_damage += total_damage/24.f;
			}
		}
		
		if(getChildrenCount() <= 0)
		{
			attack(total_damage-sum_damage);
			//			myGD->communication("Main_stopSpecialAttack");
			unschedule(schedule_selector(GameItemFire::resetChecking));
			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
		}
	}

	
	void myInit(bool is_near)
	{
		my_elemental = kElementCode_fire;
		
		holding_time = rand()%10 + 20;
		holding_time *= 60;
		
		setMyPoint(is_near);
		
		if(myPoint.isNull())
		{
			CCDelayTime* t_delay = CCDelayTime::create(1.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			
			runAction(t_seq);
			
			return;
		}
		
		item_img = CCSprite::create("item5.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		startFraming();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f*0.7f);
		
		item_img->runAction(t_scale);
		
		damage = 20;
		total_damage = damage*10;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("fire_arrow.png");
		CCTextureCache::sharedTextureCache()->addImage("fire_bomb.png");
	}
};

class ExchangeCoin : public CCNode
{
public:
	static ExchangeCoin* create(int t_type, CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin)
	{
		ExchangeCoin* t_ec = new ExchangeCoin();
		t_ec->myInit(t_type, t_ui, d_takeExchangeCoin);
		t_ec->autorelease();
		return t_ec;
	}
	
	void showCoin()
	{
		int total_empty = 0;
		
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
				if(myGD->mapState[i][j] == mapEmpty)
					total_empty++;
		
		int random_value = rand()%total_empty;
		
		int search_empty = 0;
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
			{
				if(myGD->mapState[i][j] == mapEmpty)
				{
					if(search_empty == random_value)
					{
						myPoint = IntPoint(i,j);
						i = mapWidthInnerEnd;
						break;
					}
					search_empty++;
				}
			}
		}
		
		setPosition(ccp(myPoint.x*pixelSize + 1, myPoint.y*pixelSize + 1));
		
		CCScaleTo* t_scale = CCScaleTo::create(0.5f, 2.f/3.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ExchangeCoin::startMoving));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		coin_img->runAction(t_seq);
	}
	
	void stopMoving()
	{
		unschedule(schedule_selector(ExchangeCoin::moving));
	}
	
private:
	int myType;
	CCObject* target_ui;
	SEL_CallFuncCCpI delegate_takeExchangeCoin;
	CCSprite* coin_img;
	
	IntPoint myPoint;
	int directionAngle;
	float move_speed;
	
	void startMoving()
	{
		directionAngle = rand()%360;
		move_speed = 1.f;
		schedule(schedule_selector(ExchangeCoin::moving));
	}
	
	void moving()
	{
		if(myGD->mapState[myPoint.x][myPoint.y] != mapEmpty &&
		   myGD->mapState[myPoint.x-1][myPoint.y] != mapEmpty &&
		   myGD->mapState[myPoint.x+1][myPoint.y] != mapEmpty &&
		   myGD->mapState[myPoint.x][myPoint.y-1] != mapEmpty &&
		   myGD->mapState[myPoint.x][myPoint.y+1] != mapEmpty)
		{
			stopMoving();
			(target_ui->*delegate_takeExchangeCoin)(getPosition(), myType);
			removeFromParentAndCleanup(true);
			
			return;
		}
		
		bool isFindedAfterPoint = false;
		CCPoint afterPosition;
		IntPoint afterPoint;
		int check_loop_cnt = 0;
		while(!isFindedAfterPoint)
		{
			check_loop_cnt++;
			if(check_loop_cnt >= 100 && check_loop_cnt%100 == 0)
			{
				if(check_loop_cnt > 3000)
				{
					stopMoving();
					(target_ui->*delegate_takeExchangeCoin)(getPosition(), myType);
					removeFromParentAndCleanup(true);
					
					return;
				}
				move_speed += 0.2f;
			}
			
			int changeAngleValue = rand()%5 - 2;
			changeAngleValue *= rand()%5+1;
			
			directionAngle += changeAngleValue;
			if(directionAngle < 0)			directionAngle += 360;
			else if(directionAngle > 360)	directionAngle -= 360;
			
			CCPoint d_p;
			
			d_p.x = 1.f;
			d_p.y = tanf(directionAngle/180.f*M_PI);
			
			if(directionAngle > 90 && directionAngle < 270)
			{
				d_p.x *= -1.f;
				d_p.y *= -1.f;
			}
			
			float length = sqrtf(powf(d_p.x, 2.f) + powf(d_p.y, 2.f));
			
			d_p = ccpMult(d_p, move_speed/length);
			
			afterPosition = ccpAdd(getPosition(), d_p);
			afterPoint.x = (afterPosition.x-1)/pixelSize + 1.f;
			afterPoint.y = (afterPosition.y-1)/pixelSize + 1.f;
			
			if(afterPosition.x < 0 || afterPosition.x > 320 || afterPosition.y < 0 || afterPosition.y > 430)
			{
				if(rand()%2)					directionAngle -= 90;
				else							directionAngle += 90;
				
				if(directionAngle < 0)			directionAngle += 360;
				else if(directionAngle > 360)	directionAngle -= 360;
			}
			else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] == mapNewline)
			{
				if(rand()%2)					directionAngle -= 90;
				else							directionAngle += 90;
				
				if(directionAngle < 0)			directionAngle += 360;
				else if(directionAngle > 360)	directionAngle -= 360;
			}
			else if(afterPoint.isInnerMap() && (myGD->mapState[afterPoint.x][afterPoint.y] == mapOldline || myGD->mapState[afterPoint.x][afterPoint.y] == mapOldget))
			{
				if(rand()%2)					directionAngle -= 90;
				else							directionAngle += 90;
				
				if(directionAngle < 0)			directionAngle += 360;
				else if(directionAngle > 360)	directionAngle -= 360;
			}
			else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] == mapEmpty)
			{
				isFindedAfterPoint = true;
			}
		}
		
		myPoint = afterPoint;
		setPosition(afterPosition);
		move_speed = 1.f;
	}
	
	
	void myInit(int t_type, CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin)
	{
		myType = t_type;
		target_ui = t_ui;
		delegate_takeExchangeCoin = d_takeExchangeCoin;
		
		
		coin_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", myType)->getCString());
		coin_img->setScale(0);
		coin_img->setPosition(CCPointZero);
		addChild(coin_img);
	}
};

class FeverCoin : public CCSprite
{
public:
	static FeverCoin* create(IntPoint t_point, CCObject* t_add, SEL_CallFuncO d_add)
	{
		FeverCoin* t_fc = new FeverCoin();
		t_fc->myInit(t_point, t_add, d_add);
		t_fc->autorelease();
		return t_fc;
	}
	
	void startRemove()
	{
		remove_frame = 0;
		schedule(schedule_selector(FeverCoin::removing));
	}
	
	bool is_stan_by;
	
private:
	IntPoint my_point;
	CCObject* target_add;
	SEL_CallFuncO delegate_add;
	int remove_frame;
	
	void removing()
	{
		remove_frame++;
		if(remove_frame <= 10)
			setPositionY(getPositionY() + 2.5f);
		else if(remove_frame <= 20)
			setPositionY(getPositionY() - 2.5f);
		else
		{
			unschedule(schedule_selector(FeverCoin::removing));
			removeFromParent();
		}
	}
	
	void startCheck()
	{
		schedule(schedule_selector(FeverCoin::checking));
	}
	void checking()
	{
		if(myGD->mapState[my_point.x][my_point.y] != mapOutline && (myGD->mapState[my_point.x][my_point.y] == mapOldget || myGD->mapState[my_point.x][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y] == mapOldget || myGD->mapState[my_point.x-1][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y] == mapOldget || myGD->mapState[my_point.x+1][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x][my_point.y-1] == mapOldget || myGD->mapState[my_point.x][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x][my_point.y+1] == mapOldget || myGD->mapState[my_point.x][my_point.y+1] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y+1] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y+1] == mapOldline))
		{
			stopCheck();
			is_stan_by = true;
			(target_add->*delegate_add)(this);
		}
	}
	void stopCheck()
	{
		unschedule(schedule_selector(FeverCoin::checking));
	}
	
	void myInit(IntPoint t_point, CCObject* t_add, SEL_CallFuncO d_add) // 0 ~ 5
	{
		is_stan_by = false;
		target_add = t_add;
		delegate_add = d_add;
		my_point = IntPoint(t_point.x, t_point.y);
		int start_cut = ((my_point.y-15)/17 + (my_point.x-4)/17)%12/2;
		
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin.png");
		initWithTexture(t_texture, CCRectMake(start_cut*31, 0, 31, 31));
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1f);
		int add_count = 0;
		for(int i=start_cut;add_count < 6;i=(i+1)%6)
		{
			add_count++;
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*31, 0, 31, 31));
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		runAction(t_repeat);
		
		setPosition(my_point.convertToCCP());
		setScale(1.f/1.5f);
		
		startCheck();
	}
};

class FeverCoinParent : public CCSpriteBatchNode
{
public:
	static FeverCoinParent* create()
	{
		FeverCoinParent* t_fcp = new FeverCoinParent();
		t_fcp->myInit();
		t_fcp->autorelease();
		return t_fcp;
	}
	
	void startFever()
	{
		for(int i=6;i<mapHeightInnerEnd;i+=12)
		{
			for(int j=3;j<mapWidthInnerEnd;j+=12)
			{
				if((myGD->mapState[j][i] == mapEmpty || myGD->mapState[j][i] == mapOutline) &&
				   (myGD->mapState[j-1][i-1] == mapEmpty || myGD->mapState[j-1][i-1] == mapOutline) && (myGD->mapState[j-1][i] == mapEmpty || myGD->mapState[j-1][i] == mapOutline) &&
				   (myGD->mapState[j-1][i+1] == mapEmpty || myGD->mapState[j-1][i+1] == mapOutline) && (myGD->mapState[j][i-1] == mapEmpty || myGD->mapState[j][i-1] == mapOutline) &&
				   (myGD->mapState[j][i+1] == mapEmpty || myGD->mapState[j][i+1] == mapOutline) && (myGD->mapState[j+1][i-1] == mapEmpty || myGD->mapState[j+1][i-1] == mapOutline) &&
				   (myGD->mapState[j+1][i] == mapEmpty || myGD->mapState[j+1][i] == mapOutline) && (myGD->mapState[j+1][i+1] == mapEmpty || myGD->mapState[j+1][i+1] == mapOutline))
				{
					FeverCoin* t_fc = FeverCoin::create(IntPoint(j,i), this, callfuncO_selector(FeverCoinParent::addGetCoinList));
					addChild(t_fc);
				}
			}
		}
	}
	
	void stopFever()
	{
		int loop_cnt = getChildrenCount();
		CCArray* my_childs = getChildren();
		CCArray* delete_target_list = CCArray::createWithCapacity(1);
		for(int i=0;i<loop_cnt;i++)
		{
			FeverCoin* t_fc = (FeverCoin*)my_childs->objectAtIndex(i);
			if(!t_fc->is_stan_by)
			{
				delete_target_list->addObject(t_fc);
			}
		}
		
		while(delete_target_list->count() > 0)
		{
			CCNode* t_node = (CCNode*)delete_target_list->randomObject();
			delete_target_list->removeObject(t_node);
			t_node->removeFromParent();
		}
	}
	
private:
	bool is_removing;
	deque<CCObject*> remove_target_list;
	
	void addGetCoinList(CCObject* t_coin)
	{
		remove_target_list.push_back(t_coin);
		
		if(!is_removing)
		{
			startRemove();
		}
	}
	
	void startRemove()
	{
		is_removing = true;
		schedule(schedule_selector(FeverCoinParent::removing), 3.f/60.f);
	}
	void removing()
	{
		AudioEngine::sharedInstance()->playEffect("sound_fever_coin.m4a", false);
		FeverCoin* remove_target = (FeverCoin*)remove_target_list.front();
		remove_target->startRemove();
		remove_target_list.pop_front();
		
		mySGD->setGold(mySGD->getGold() + 10);
		
		if(remove_target_list.empty())
		{
			stopRemove();
		}
	}
	void stopRemove()
	{
		unschedule(schedule_selector(FeverCoinParent::removing));
		is_removing = false;
	}
	
	void myInit()
	{
		initWithFile("fever_coin.png", kDefaultSpriteBatchCapacity);
		is_removing = false;
	}
};

class GameItemManager : public CCNode
{
public:
	static GameItemManager* create()
	{
		GameItemManager* t_gim = new GameItemManager();
		t_gim->myInit();
		t_gim->autorelease();
		return t_gim;
	}
	
	void startItemSetting()
	{
		for(int i=0;i<2;i++)
		{
			if(rand()%2 == 0)
			{
				GameItemAttack* t_gia = GameItemAttack::create(true);
				addChild(t_gia);
			}
			else
			{
				GameItemSpeedUp* t_gisu = GameItemSpeedUp::create(true);
				addChild(t_gisu);
			}
		}
		
		if(mySD->getClearCondition() == kCLEAR_timeLimit)
		{
			GameItemAddTime* t_giat = GameItemAddTime::create(false);
			addChild(t_giat);
		}
		
		if(mySGD->isUsingItem(kIC_fast))
		{
			GameItemFast* t_fast = GameItemFast::create(false);
			addChild(t_fast);
		}
		
		if(mySGD->isUsingItem(kIC_critical))
		{
			GameItemFire* t_fire = GameItemFire::create(false);
			addChild(t_fire);
		}
		
		if(mySGD->isUsingItem(kIC_subOneDie))
		{
			GameItemSubOneDie* t_sod = GameItemSubOneDie::create(false);
			addChild(t_sod);
		}
	
		if(mySGD->isUsingItem(kIC_silence))
		{
			GameItemSilence* t_silence = GameItemSilence::create(false);
			addChild(t_silence);
		}
	}
	
	void startCounting()
	{
		create_counting_value = rand()%5 +  10-selected_item_cnt-double_item_cnt;
		counting_value = 0;
		schedule(schedule_selector(GameItemManager::counting), 1.f);
	}
	
	void dieCreateItem()
	{
		for(int i=0;i<5;i++)
		{
			if(getChildrenCount()-child_base_cnt < 6)
			{
				addItem();
			}
			else
				return;
		}
	}
	
	void showCoin(CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin)
	{
		for(int i=1;i<=6;i++)
		{
			ExchangeCoin* t_ec = ExchangeCoin::create(i, t_ui, d_takeExchangeCoin);
			coin_parent->addChild(t_ec);
			t_ec->showCoin();
		}
	}
	
	void stopCoin()
	{
		int loop_cnt = coin_parent->getChildrenCount();
		CCArray* child = coin_parent->getChildren();
		for(int i=0;i<loop_cnt;i++)
		{
			ExchangeCoin* t_ec = (ExchangeCoin*)child->objectAtIndex(i);
			t_ec->stopMoving();
		}
	}
	
private:
	
	int counting_value;
	int create_counting_value;
	CLEAR_CONDITION clr_cdt_type;
	
	CCNode* coin_parent;//change coin
	FeverCoinParent* fever_coin_parent;

	vector<ITEM_CODE> creatable_list;
	int selected_item_cnt;
	
	int double_item_cnt;
	int child_base_cnt;

	void counting()
	{
		counting_value++;
		
		if(clr_cdt_type == kCLEAR_bossLifeZero && getChildrenCount()-child_base_cnt < 2)
		{
			GameItemAttack* t_gia = GameItemAttack::create(false);
			addChild(t_gia);
			
			create_counting_value = rand()%5 + 10-selected_item_cnt-double_item_cnt;
			counting_value = 0;
		}
		else if(clr_cdt_type == kCLEAR_itemCollect && getChildrenCount()-child_base_cnt < 1)
		{
			addItem();
			
			create_counting_value = rand()%5 + 10-selected_item_cnt-double_item_cnt;
			counting_value = 0;
		}
		
		if(counting_value >= create_counting_value)
		{
			if(getChildrenCount()-child_base_cnt < 6)
				addItem();
			
			create_counting_value = rand()%5 + 10-selected_item_cnt-double_item_cnt;
			counting_value = 0;
		}
	}

	void addItem()
	{
		int random_value = rand()%creatable_list.size();
		ITEM_CODE create_item = creatable_list[random_value];
		
		if(create_item == kIC_attack)
		{
			GameItemAttack* t_gia = GameItemAttack::create(rand()%2 == 0);
			addChild(t_gia);
		}
		else if(create_item == kIC_speedUp)
		{
			GameItemSpeedUp* t_gisu = GameItemSpeedUp::create(rand()%2 == 0);
			addChild(t_gisu);
		}
		else if(create_item == kIC_fast)
		{
			GameItemFast* t_fast = GameItemFast::create(false);
			addChild(t_fast);
		}
		else if(create_item == kIC_critical)
		{
			GameItemFire* t_fire = GameItemFire::create(false);
			addChild(t_fire);
		}
		else if(create_item == kIC_subOneDie)
		{
			GameItemSubOneDie* t_sod = GameItemSubOneDie::create(false);
			addChild(t_sod);
		}
		else if(create_item == kIC_silence)
		{
			GameItemSilence* t_silence = GameItemSilence::create(false);
			addChild(t_silence);
		}
	}
	
	void myInit()
	{
		clr_cdt_type = mySD->getClearCondition();
		
		coin_parent = CCNode::create();
		addChild(coin_parent);
		
		fever_coin_parent = FeverCoinParent::create();
		addChild(fever_coin_parent);
		
		child_base_cnt = getChildrenCount();
		
		int defItems_cnt = SDS_GI(kSDF_stageInfo, mySD->getSilType(), "defItems_cnt");
		for(int i=0;i<defItems_cnt;i++)
			creatable_list.push_back(ITEM_CODE(SDS_GI(kSDF_stageInfo, mySD->getSilType(), CCString::createWithFormat("defItems_%d_type", i)->getCString())));
		
		selected_item_cnt = 0;
		
		double_item_cnt = mySGD->getDoubleItemValue();
		
		if(mySGD->isUsingItem(kIC_fast)){		creatable_list.push_back(kIC_fast);			selected_item_cnt++;	}
		if(mySGD->isUsingItem(kIC_critical)){	creatable_list.push_back(kIC_critical);		selected_item_cnt++;	}
		if(mySGD->isUsingItem(kIC_subOneDie)){	creatable_list.push_back(kIC_subOneDie);	selected_item_cnt++;	}
		if(mySGD->isUsingItem(kIC_silence)){	creatable_list.push_back(kIC_silence);		selected_item_cnt++;	}
		
		if(selected_item_cnt+double_item_cnt > 10)
			double_item_cnt = 10 - selected_item_cnt;
		
//		myGD->regGIM(this, callfunc_selector(GameItemManager::dieCreateItem));
		myGD->V_V["GIM_dieCreateItem"] = std::bind(&GameItemManager::dieCreateItem, this);
		myGD->V_V["GIM_startFever"] = std::bind(&FeverCoinParent::startFever, fever_coin_parent);
		myGD->V_V["GIM_stopFever"] = std::bind(&FeverCoinParent::stopFever, fever_coin_parent);
		myGD->V_V["GIM_stopCoin"] = std::bind(&GameItemManager::stopCoin, this);
	}
};

#endif /* defined(__galsprototype__GameItemManager__) */
