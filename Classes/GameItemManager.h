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

USING_NS_CC;

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
		
		item_img = CCSprite::create("addtime_item.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemAddTime::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
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
		
		item_img = CCSprite::create("fast_item.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemSpeedUp::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
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
		
		item_img = CCSprite::create("item3.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemFast::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
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
		myGD->communication("MP_createJackMissile", my_elemental, rand()%3 + 1, 0.7f);
		removeFromParent();
	}
	
	void myInit(bool is_near)
	{
//		my_elemental = rand()%kElementCode_plasma + 1;
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
		
		string item_string;

//		if(my_elemental == kElementCode_life)			item_string = "life";
//		else if(my_elemental == kElementCode_fire)
			item_string = "fire";
//		else if(my_elemental == kElementCode_water)		item_string = "ice";
//		else if(my_elemental == kElementCode_wind)		item_string = "wind";
//		else if(my_elemental == kElementCode_lightning)	item_string = "lightning";
//		else if(my_elemental == kElementCode_plasma)	item_string = "plasma";
		
		item_img = CCSprite::create(CCString::createWithFormat("%s_item.png", item_string.c_str())->getCString());
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemAttack::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
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
		
		item_img = CCSprite::create("item5.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
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
		
		item_img = CCSprite::create("item7.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
	}
};

class GameItemWind : public GameItemBase
{
public:
	static GameItemWind* create(bool is_near)
	{
		GameItemWind* t_gisu = new GameItemWind();
		t_gisu->myInit(is_near);
		t_gisu->autorelease();
		return t_gisu;
	}
	
private:
	int my_elemental;
	int damage_frame;
	int ing_frame;
	CCSprite* fogImg;
	Tornado* t_1;
	Tornado* t_2;
	Tornado* t_3;
	float damage;
	float total_damage;
	float sum_damage;
	
	virtual void acting()
	{
		// wind
		myGD->communication("EP_addJackAttack");
		AudioEngine::sharedInstance()->playEffect("sound_pil_wind.mp3",false);
		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
		unschedule(schedule_selector(GameItemWind::framing));
		t_1 = Tornado::create(ccp(-100,100), ccp(160,230), -1, 240);
		addChild(t_1);
		
		t_2 = Tornado::create(ccp(420,100), ccp(160,230), -1, 240);
		addChild(t_2);
		
		t_3 = Tornado::create(ccp(160,520), ccp(160,230), -1, 240);
		addChild(t_3);
		
		t_1->startMove();
		t_2->startMove();
		t_3->startMove();
		
		fogImg = CCSprite::create("tornado_fog.png");
		fogImg->setPosition(ccp(160,215));
		fogImg->setScale(0);
		addChild(fogImg);
		
		damage_frame = 0;
		schedule(schedule_selector(GameItemWind::resetChecking));
		///
	}
	// wind
	void fogZoom()
	{
		ing_frame++;
		
		if(ing_frame%6 == 0)
		{
			if(ing_frame/6 >= 1 && ing_frame/6 <= 4)
			{
				fogImg->stopAllActions();
				CCScaleTo* t_scale = CCScaleTo::create(0.1, fogImg->getScale()*2);
				fogImg->runAction(t_scale);
			}
			else if(ing_frame/6 == 5)
			{
				fogImg->stopAllActions();
				CCScaleTo* t_scale = CCScaleTo::create(0.2, 0);
				fogImg->runAction(t_scale);
			}
		}
		
		if(ing_frame >= 36)
		{
			stopFogZoom();
		}
	}
	
	void stopFogZoom()
	{
		unschedule(schedule_selector(GameItemWind::fogZoom));
		fogImg->removeFromParentAndCleanup(true);
	}
	
	void resetChecking()
	{
		damage_frame++;
		if(damage_frame%40 == 0)
		{
			if(damage_frame/40 <= 4)
			{
				CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
				
				myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f));
				myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
				
				if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
				{
					myGD->communication("UI_subBossLife", total_damage/10.f);
					sum_damage += total_damage/10.f;
					myGD->communication("CP_startDamageReaction", rand()%360-180.f);
				}
				
				
				CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
				int cumber_cnt = subCumberArray->count();
				
				for(int i=0;i<cumber_cnt;i++)
				{
					CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
					myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f)); // ccpoint
					myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
					myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage/10.f, rand()%360-180.f);
					
					if(subCumberArray->count() < cumber_cnt)
					{
						cumber_cnt--;
						i--;
					}
				}
			}
			else if(damage_frame/40 == 6)
			{
				t_1->startEnding();
				t_2->startEnding();
				t_3->startEnding();
				
				ing_frame = 0;
				CCScaleTo* t_scale = CCScaleTo::create(0.1, 1.0);
				fogImg->runAction(t_scale);
				schedule(schedule_selector(GameItemWind::fogZoom));
			}
		}
		
		if(getChildrenCount() <= 0)
		{
			CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
			
			myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f));
			myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
			
			if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
			{
				myGD->communication("UI_subBossLife", total_damage-sum_damage);
				myGD->communication("CP_startDamageReaction", rand()%360-180.f);
			}
			
			
			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
			int cumber_cnt = subCumberArray->count();
			
			for(int i=0;i<cumber_cnt;i++)
			{
				CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
				myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f)); // ccpoint
				myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
				myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage-sum_damage, rand()%360-180.f);
				
				if(subCumberArray->count() < cumber_cnt)
				{
					cumber_cnt--;
					i--;
				}
			}
			unschedule(schedule_selector(GameItemWind::resetChecking));
			sum_damage = 0;
			myGD->communication("EP_subJackAttack");
			removeFromParentAndCleanup(true);
		}
	}
	///
	
	void myInit(bool is_near)
	{
		my_elemental = kElementCode_wind;
		
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
		
		item_img = CCSprite::create("item_bomb.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemWind::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
		
		damage = 20;
		total_damage = damage*10;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("tornado1_repeat1.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado1_repeat2.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado1_repeat3.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado1_final1.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado1_final2.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado1_final3.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado1_final4.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
	}
};

class GameItemLightning : public GameItemBase
{
public:
	static GameItemLightning* create(bool is_near)
	{
		GameItemLightning* t_gisu = new GameItemLightning();
		t_gisu->myInit(is_near);
		t_gisu->autorelease();
		return t_gisu;
	}
	
private:
	int my_elemental;
	int damage_frame;
	int ing_frame;
	LightningStorm* myStorm;
	CCSprite* fogImg;
	CCSprite* bombImg;
	
	float damage;
	float total_damage;
	float sum_damage;
	
	virtual void acting()
	{
		myGD->communication("EP_addJackAttack");
		AudioEngine::sharedInstance()->playEffect("sound_pil_lightning.mp3",false);
		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
		unschedule(schedule_selector(GameItemLightning::framing));
		//			myGD->communication("Main_startSpecialAttack");
		
		myStorm = LightningStorm::create();
		addChild(myStorm);
		myStorm->startMyAction();
		
		fogImg = CCSprite::create("tornado_fog.png");
		fogImg->setPosition(ccp(160,240));
		fogImg->setScale(0);
		addChild(fogImg);
		
		bombImg = CCSprite::create("special_attack_bomb1.png");
		bombImg->setPosition(ccp(160,240));
		bombImg->setVisible(false);
		addChild(bombImg);
		
		damage_frame = 0;
		schedule(schedule_selector(GameItemLightning::resetChecking));
	}
	
	void fogZoom()
	{
		ing_frame++;
		
		if(ing_frame%6 == 0)
		{
			if(ing_frame/6 >= 1 && ing_frame/6 <= 4)
			{
				fogImg->stopAllActions();
				CCScaleTo* t_scale = CCScaleTo::create(0.1, fogImg->getScale()*2);
				fogImg->runAction(t_scale);
			}
			else if(ing_frame/6 == 5)
			{
				myStorm->stopRepeatRemove();
				fogImg->stopAllActions();
				CCScaleTo* t_scale = CCScaleTo::create(0.2, 0);
				fogImg->runAction(t_scale);
			}
		}
		
		if(ing_frame >= 36)
		{
			stopFogZoom();
		}
	}
	
	void stopFogZoom()
	{
		unschedule(schedule_selector(GameItemLightning::fogZoom));
		fogImg->removeFromParentAndCleanup(true);
	}
	
	void removeBomb()
	{
		bombImg->removeFromParentAndCleanup(true);
	}
	
	void resetChecking()
	{
		damage_frame++;
		if(damage_frame%9 == 0)
		{
			CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
			
			myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f));
			myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
			
			if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
			{
				myGD->communication("UI_subBossLife", total_damage/23.f);
				sum_damage += total_damage/23.f;
				myGD->communication("CP_startDamageReaction", rand()%360-180.f);
			}
			
			
			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
			int cumber_cnt = subCumberArray->count();
			
			for(int i=0;i<cumber_cnt;i++)
			{
				CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
				myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f)); // ccpoint
				myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
				myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage/23.f, rand()%360-180.f);
				
				if(subCumberArray->count() < cumber_cnt)
				{
					cumber_cnt--;
					i--;
				}
			}
		}
		
		if(damage_frame == 140)
		{
			ing_frame = 0;
			bombImg->setVisible(true);
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=1;i<=4;i++)
			{
				t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("special_attack_bomb%d.png", i)->getCString());
			}
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemLightning::removeBomb));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
			bombImg->runAction(t_seq);
			
			CCScaleTo* t_scale = CCScaleTo::create(0.1, 1.0);
			fogImg->runAction(t_scale);
			schedule(schedule_selector(GameItemLightning::fogZoom));
		}
		
		if(getChildrenCount() <= 0)
		{
			CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
			
			myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f));
			myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
			
			if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
			{
				myGD->communication("UI_subBossLife", total_damage-sum_damage);
				myGD->communication("CP_startDamageReaction", rand()%360-180.f);
			}
			
			
			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
			int cumber_cnt = subCumberArray->count();
			
			for(int i=0;i<cumber_cnt;i++)
			{
				CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
				myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f)); // ccpoint
				myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
				myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage-sum_damage, rand()%360-180.f);
				
				if(subCumberArray->count() < cumber_cnt)
				{
					cumber_cnt--;
					i--;
				}
			}
			unschedule(schedule_selector(GameItemLightning::resetChecking));
			sum_damage = 0;
			myGD->communication("EP_subJackAttack");
		}
	}
	
	void myInit(bool is_near)
	{
		my_elemental = kElementCode_lightning;
		
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
		
		item_img = CCSprite::create("item_bomb.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemLightning::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
		
		damage = 20;
		total_damage = damage*10;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start1.png");
		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start2.png");
		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start3.png");
		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start4.png");
		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_repeat1.png");
		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_repeat2.png");
		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_repeat3.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb1.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb2.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb3.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb4.png");
	}
};

class GameItemLife : public GameItemBase
{
public:
	static GameItemLife* create(bool is_near)
	{
		GameItemLife* t_gisu = new GameItemLife();
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
		myGD->communication("EP_addJackAttack");
		AudioEngine::sharedInstance()->playEffect("sound_pil_life.mp3",false);
		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
		unschedule(schedule_selector(GameItemLife::framing));
		//			myGD->communication("Main_startSpecialAttack");
		AnkleSnare* t_as = AnkleSnare::create();
		addChild(t_as);
		t_as->startMyAction();
		
		damage_frame = 0;
		schedule(schedule_selector(GameItemLife::resetChecking));
	}
	
	void attack(float t_damage)
	{
		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
		
		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 1.f, 0, 1.f));
		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
		
		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
		{
			myGD->communication("UI_subBossLife", t_damage);
			myGD->communication("CP_startDamageReaction", rand()%360-180.f);
		}
		
		
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumber_cnt = subCumberArray->count();
		
		for(int i=0;i<cumber_cnt;i++)
		{
			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 1.f, 0, 1.f)); // ccpoint
			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
			
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
		if(damage_frame == 18 || damage_frame == 42 || damage_frame == 54 || damage_frame == 60 || damage_frame == 66 || damage_frame == 72)
		{
			attack(total_damage/10.f);
			sum_damage += total_damage/10.f;
		}
		
		if(getChildrenCount() <= 0)
		{
			attack(total_damage-sum_damage);
			//			myGD->communication("Main_stopSpecialAttack");
			unschedule(schedule_selector(GameItemLife::resetChecking));
			sum_damage = 0;
			myGD->communication("EP_subJackAttack");
		}
	}
	
	void myInit(bool is_near)
	{
		
		
		my_elemental = kElementCode_life;
		
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
		
		item_img = CCSprite::create("item_bomb.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemLife::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
		
		damage = 20;
		total_damage = damage*10;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare1.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare2.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare3.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare4.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare5.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare6.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare7.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare8.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare9.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare10.png");
		CCTextureCache::sharedTextureCache()->addImage("ankle_snare11.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb1.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb2.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb3.png");
		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb4.png");
	}
};

class GameItemIce : public GameItemBase
{
public:
	static GameItemIce* create(bool is_near)
	{
		GameItemIce* t_gisu = new GameItemIce();
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
		myGD->communication("EP_addJackAttack");
		AudioEngine::sharedInstance()->playEffect("sound_pil_ice.mp3",false);
		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
		unschedule(schedule_selector(GameItemIce::framing));
		//			myGD->communication("Main_startSpecialAttack");
		IceStorm* t_is = IceStorm::create();
		addChild(t_is);
		t_is->startMyAction();
		
		damage_frame = 0;
		schedule(schedule_selector(GameItemIce::resetChecking));
	}
	
	void attack(float t_damage)
	{
		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
		
		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 0, 1.f, 1.f));
		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
		
		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
		{
			myGD->communication("UI_subBossLife", t_damage);
			CCLog("ice : %.1f", t_damage);
			myGD->communication("CP_startDamageReaction", rand()%360-180.f);
		}
		
		
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumber_cnt = subCumberArray->count();
		
		for(int i=0;i<cumber_cnt;i++)
		{
			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 0, 1.f, 1.f)); // ccpoint
			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
			
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
		if(damage_frame >= 42 && damage_frame <= 342)
		{
			if((damage_frame-42)%18 == 0)
			{
				attack(total_damage/20.f);
				sum_damage += total_damage/20.f;
			}
		}
		
		if(getChildrenCount() <= 0)
		{
			attack(total_damage-sum_damage);
			//			myGD->communication("Main_stopSpecialAttack");
			unschedule(schedule_selector(GameItemIce::resetChecking));
			sum_damage = 0;
			myGD->communication("EP_subJackAttack");
		}
	}
	
	void myInit(bool is_near)
	{
		
		
		my_elemental = kElementCode_water;
		
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
		
		item_img = CCSprite::create("item_bomb.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemIce::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
		
		damage = 20;
		total_damage = damage*10;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_fragment.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main1.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main2.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main3.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main4.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
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
		myGD->communication("EP_addJackAttack");
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
		
		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
		{
			myGD->communication("UI_subBossLife", t_damage);
			myGD->communication("CP_startDamageReaction", rand()%360-180.f);
		}
		
		
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumber_cnt = subCumberArray->count();
		
		for(int i=0;i<cumber_cnt;i++)
		{
			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f)); // ccpoint
			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
			
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
			myGD->communication("EP_subJackAttack");
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
		
		item_img = CCSprite::create("item4.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemFire::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
		
		damage = 20;
		total_damage = damage*10;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("fire_arrow.png");
		CCTextureCache::sharedTextureCache()->addImage("fire_bomb.png");
	}
};

class GameItemPlasma : public GameItemBase
{
public:
	static GameItemPlasma* create(bool is_near)
	{
		GameItemPlasma* t_gisu = new GameItemPlasma();
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
		myGD->communication("EP_addJackAttack");
		AudioEngine::sharedInstance()->playEffect("sound_pil_plasma.mp3",false);
		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
		unschedule(schedule_selector(GameItemPlasma::framing));
		//			myGD->communication("Main_startSpecialAttack");
		PlasmaStorm* t_ps = PlasmaStorm::create();
		addChild(t_ps);
		t_ps->startMyAction();
		
		damage_frame = 0;
		schedule(schedule_selector(GameItemPlasma::resetChecking));
	}
	
	void attack(float t_damage)
	{
		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
		
		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 0, 1.f, 1.f));
		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
		
		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
		{
			myGD->communication("UI_subBossLife", t_damage);
			myGD->communication("CP_startDamageReaction", rand()%360-180.f);
		}
		
		
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumber_cnt = subCumberArray->count();
		
		for(int i=0;i<cumber_cnt;i++)
		{
			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 0, 1.f, 1.f)); // ccpoint
			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
			
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
		if(damage_frame%12 == 0)
		{
			attack(total_damage/20.f);
			sum_damage += total_damage/20.f;
		}
		
		if(getChildrenCount() <= 0)
		{
			attack(total_damage-sum_damage);
			//			myGD->communication("Main_stopSpecialAttack");
			unschedule(schedule_selector(GameItemPlasma::resetChecking));
			sum_damage = 0;
			myGD->communication("EP_subJackAttack");
		}
	}
	
	void myInit(bool is_near)
	{
		
		
		my_elemental = kElementCode_plasma;
		
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
		
		item_img = CCSprite::create("item_bomb.png");
		item_img->setScale(0.f);
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		item_img->setPosition(item_point);
		addChild(item_img);
		
		starting_side_cnt = getSideCount();
		
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemPlasma::startFraming));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		item_img->runAction(t_seq);
		
		damage = 20;
		total_damage = damage*10;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init1.png");
		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init2.png");
		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init3.png");
		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init4.png");
		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_repeat1.png");
		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_repeat2.png");
		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_repeat3.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
	}
};

class ExchangeCoin : public CCNode
{
public:
	static ExchangeCoin* create(int t_type, CCObject* t_ui, SEL_CallFuncI d_takeExchangeCoin)
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
	
private:
	int myType;
	CCObject* target_ui;
	SEL_CallFuncI delegate_takeExchangeCoin;
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
			(target_ui->*delegate_takeExchangeCoin)(myType);
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
			if(check_loop_cnt > 3000)
			{
				stopMoving();
				(target_ui->*delegate_takeExchangeCoin)(myType);
				removeFromParentAndCleanup(true);
				
				return;
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
	}
	
	void stopMoving()
	{
		unschedule(schedule_selector(ExchangeCoin::moving));
	}
	
	void myInit(int t_type, CCObject* t_ui, SEL_CallFuncI d_takeExchangeCoin)
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
			if(getChildrenCount() < 7)
			{
				addItem();
			}
			else
				return;
		}
	}
	
	void showCoin(CCObject* t_ui, SEL_CallFuncI d_takeExchangeCoin)
	{
		for(int i=1;i<=6;i++)
		{
			ExchangeCoin* t_ec = ExchangeCoin::create(i, t_ui, d_takeExchangeCoin);
			coin_parent->addChild(t_ec);
			t_ec->showCoin();
		}
	}
	
private:
	
	int counting_value;
	int create_counting_value;
	CLEAR_CONDITION clr_cdt_type;
	
	CCNode* coin_parent;

	vector<ITEM_CODE> creatable_list;
	int selected_item_cnt;
	
	int double_item_cnt;

	void counting()
	{
		counting_value++;
		
		if(clr_cdt_type == kCLEAR_bossLifeZero && getChildrenCount() < 3)
		{
			GameItemAttack* t_gia = GameItemAttack::create(false);
			addChild(t_gia);
			
			create_counting_value = rand()%5 + 10-selected_item_cnt-double_item_cnt;
			counting_value = 0;
		}
		else if(clr_cdt_type == kCLEAR_itemCollect && getChildrenCount() < 2)
		{
			addItem();
			
			create_counting_value = rand()%5 + 10-selected_item_cnt-double_item_cnt;
			counting_value = 0;
		}
		
		if(counting_value >= create_counting_value)
		{
			if(getChildrenCount() < 7)
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
	}
};

#endif /* defined(__galsprototype__GameItemManager__) */
