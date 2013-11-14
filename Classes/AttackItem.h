//
//  AttackItem.h
//  DrawingJack
//
//  Created by 사원3 on 13. 1. 14..
//
//

#ifndef DrawingJack_AttackItem_h
#define DrawingJack_AttackItem_h

#include "cocos2d.h"
#include "GameData.h"
#include "EnumDefine.h"
#include "DataStorageHub.h"
#include "AlertEngine.h"
#include "AudioEngine.h"
#include "MissileDamageData.h"
#include "ServerDataSave.h"

using namespace cocos2d;

#define AttackItemBaseCastTime		600
#define AttackItemBaseSubValue		50
#define AttackItemBaseHideTime		1000
#define AttackItemBaseUpRate		0.3f
#define AttackItemBaseDamage		3.f

class AttackItem : public CCNode
{
public:
	
	virtual void showItem()
	{
		
	}
	
	void startCast()
	{
		cast_cnt = 0;
		schedule(schedule_selector(AttackItem::casting));
	}
	
	int hide_cnt;
	int base_cast_time;
	int base_hide_time;
private:
	int cast_cnt;
	
	void stopCase()
	{
		unschedule(schedule_selector(AttackItem::casting));
	}
	
	void casting()
	{
		cast_cnt++;
		if(cast_cnt >= base_cast_time)
		{
			stopCase();
			showItem();
		}
	}
	
};

class Tornado : public CCNode
{
public:
	static Tornado* create(CCPoint t_sp, CCPoint t_fp, int t_spin_direction, int t_move_frame)
	{
		Tornado* t_t = new Tornado();
		t_t->myInit(t_sp, t_fp, t_spin_direction, t_move_frame);
		t_t->autorelease();
		return t_t;
	}
	
	void startMove()
	{
		ing_frame = 0;
		schedule(schedule_selector(Tornado::action1));
	}
	
	void startEnding()
	{
		tornadoImg->stopAllActions();
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=1;i<4;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("tornado1_final%d.png", i)->getCString());
		}
		
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Tornado::tornadoRemove));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
		tornadoImg->runAction(t_seq);
	}
	
private:
	CCSprite* tornadoImg;
	CCPoint fp;
	int move_frame;
	int ing_frame;
	float recent_angle;
	float change_angle;
	float recent_distance;
	float reduce_distance;
	
	
	void action1()
	{
		ing_frame++;
		
		recent_angle += change_angle;
		if(recent_angle >= 180.f)		recent_angle -= 360.f;
		if(recent_angle < -180.f)		recent_angle += 360.f;
		recent_distance -= reduce_distance;
		
		CCPoint afterPosition;
		afterPosition.x = 1;
		afterPosition.y = tanf(recent_angle/180.f*M_PI);
		
		float div_value = sqrtf(powf(afterPosition.x, 2.f) + powf(afterPosition.y, 2.f));
		afterPosition = ccpMult(afterPosition, 1.f/div_value);
		
		if(recent_angle >= 90.f || recent_angle < -90.f)
			afterPosition = ccpMult(afterPosition, -1.f);
		
		afterPosition = ccpMult(afterPosition, recent_distance);
		afterPosition = ccpAdd(fp, afterPosition);
		
		setPosition(afterPosition);
		
		if(ing_frame >= move_frame)
		{
			stopAction1();
		}
	}
	
	void stopAction1()
	{
		unschedule(schedule_selector(Tornado::action1));
	}
	
	void tornadoRemove()
	{
		tornadoImg->removeFromParentAndCleanup(true);
		removeFromParentAndCleanup(true);
	}
	
	
	void myInit(CCPoint t_sp, CCPoint t_fp, int t_spin_direction, int t_move_frame)
	{
		fp = t_fp;
		setPosition(t_sp);
		CCPoint subPosition = ccpSub(t_sp, fp);
		move_frame = t_move_frame;
		recent_distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		reduce_distance = recent_distance/move_frame;
		recent_angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		change_angle = 3.f*t_spin_direction;
		
		tornadoImg = CCSprite::create("tornado1_repeat1.png");
		addChild(tornadoImg);
		
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=1;i<=3;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("tornado1_repeat%d.png", i)->getCString());
		}
		
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		tornadoImg->runAction(t_repeat);
	}
};

class WindItem : public AttackItem
{
public:
	static WindItem* create()
	{
		WindItem* t_wi = new WindItem();
		t_wi->myInit();
		t_wi->autorelease();
		return t_wi;
	}
	
	virtual void showItem()
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
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "item position infinity loop", "ok", 1, this, alertfuncII_selector(WindItem::alertAction));
		}
		itemImg = CCSprite::create("wind_item.png");
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		itemImg->setPosition(item_point);
		addChild(itemImg);
		
		hide_cnt = 0;
		schedule(schedule_selector(WindItem::checkItemSide));
	}
	
	virtual ~WindItem()
	{
		CCTextureCache::sharedTextureCache()->removeAllTextures();
	}
	
private:
	IntPoint myPoint;
	CCSprite* itemImg;
	int damage_frame;
	int ing_frame;
	CCSprite* fogImg;
	Tornado* t_1;
	Tornado* t_2;
	Tornado* t_3;
	float damage;
	float total_damage;
	float sum_damage;
	int dex;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void checkItemSide()
	{
		hide_cnt++;
		
		if(hide_cnt >= base_hide_time)
		{
			unschedule(schedule_selector(WindItem::checkItemSide));
			itemImg->removeFromParentAndCleanup(true);
			startCast();
			return;
		}
		
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
		
		if(side_cnt == 4)
		{
//			myGD->communication("EP_addJackAttack");
			AudioEngine::sharedInstance()->playEffect("sound_pil_wind.mp3",false);
			itemImg->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(WindItem::checkItemSide));
//			myGD->communication("Main_startSpecialAttack");
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
			schedule(schedule_selector(WindItem::resetChecking));
		}
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
		unschedule(schedule_selector(WindItem::fogZoom));
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
				schedule(schedule_selector(WindItem::fogZoom));
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
			unschedule(schedule_selector(WindItem::resetChecking));
			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
			startCast();
		}
	}
	
	void myInit()
	{
		base_hide_time = AttackItemBaseHideTime;
		
		base_cast_time = AttackItemBaseCastTime - 4*AttackItemBaseSubValue;
		
		int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(recent_card_number >= 0)
		{
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
			damage = damage < 1 ? 1 : damage;
			dex = dex < 1 ? 1 : dex;
		}
		else
		{
			damage = 1;
			dex = 1;
		}
		
		total_damage = damage*(AttackItemBaseDamage+4*AttackItemBaseUpRate);
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

class LightningStorm : public CCNode
{
public:
	static LightningStorm* create()
	{
		LightningStorm* t_ls = new LightningStorm();
		t_ls->myInit();
		t_ls->autorelease();
		return t_ls;
	}
	
	void startMyAction()
	{
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=1;i<=4;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("lightning_storm_start%d.png", i)->getCString());
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LightningStorm::startRepeat));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
		
		mainImg->runAction(t_seq);
	}
	
	void stopRepeatRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
private:
	
	CCSprite* mainImg;
	
	void startRepeat()
	{
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=1;i<=3;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("lightning_storm_repeat%d.png", i)->getCString());
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		mainImg->stopAllActions();
		mainImg->runAction(t_repeat);
	}
	
	void myInit()
	{
		setPosition(ccp(160,240));
		mainImg = CCSprite::create("lightning_storm_start1.png");
		addChild(mainImg);
	}
};

class LightningItem : public AttackItem
{
public:
	static LightningItem* create()
	{
		LightningItem* t_li = new LightningItem();
		t_li->myInit();
		t_li->autorelease();
		return t_li;
	}
	
	virtual void showItem()
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
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "item position infinity loop", "ok", 1, this, alertfuncII_selector(LightningItem::alertAction));
		}
		itemImg = CCSprite::create("lightning_item.png");
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		itemImg->setPosition(item_point);
		addChild(itemImg);
		
		hide_cnt = 0;
		schedule(schedule_selector(LightningItem::checkItemSide));
	}
	
	virtual ~LightningItem()
	{
		CCTextureCache::sharedTextureCache()->removeAllTextures();
	}
	
private:
	IntPoint myPoint;
	CCSprite* itemImg;
	
	int damage_frame;
	int ing_frame;
	LightningStorm* myStorm;
	CCSprite* fogImg;
	CCSprite* bombImg;
	
	float damage;
	float total_damage;
	float sum_damage;
	
	int dex;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void checkItemSide()
	{
		hide_cnt++;
		
		if(hide_cnt >= base_hide_time)
		{
			unschedule(schedule_selector(LightningItem::checkItemSide));
			itemImg->removeFromParentAndCleanup(true);
			startCast();
			return;
		}
		
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
		
		if(side_cnt == 4)
		{
//			myGD->communication("EP_addJackAttack");
			AudioEngine::sharedInstance()->playEffect("sound_pil_lightning.mp3",false);
			itemImg->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(LightningItem::checkItemSide));
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
			schedule(schedule_selector(LightningItem::resetChecking));
		}
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
		unschedule(schedule_selector(LightningItem::fogZoom));
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
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LightningItem::removeBomb));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
			bombImg->runAction(t_seq);
			
			CCScaleTo* t_scale = CCScaleTo::create(0.1, 1.0);
			fogImg->runAction(t_scale);
			schedule(schedule_selector(LightningItem::fogZoom));
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
			unschedule(schedule_selector(LightningItem::resetChecking));
			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
			startCast();
		}
	}
	
	
	void myInit()
	{
		base_hide_time = AttackItemBaseHideTime;
		
		base_cast_time = AttackItemBaseCastTime - 4*AttackItemBaseSubValue;
		
		int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(recent_card_number >= 0)
		{
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
			damage = damage < 1 ? 1 : damage;
			dex = dex < 1 ? 1 : dex;
		}
		else
		{
			damage = 1;
			dex = 1;
		}
		
		total_damage = damage * (AttackItemBaseDamage+4*AttackItemBaseUpRate);;
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

class AnkleSnare : public CCNode
{
public:
	static AnkleSnare* create()
	{
		AnkleSnare* t_as = new AnkleSnare();
		t_as->myInit();
		t_as->autorelease();
		return t_as;
	}
	
	void startMyAction()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AnkleSnare::startAnimation));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		mainImg->runAction(t_seq);
	}
	
private:
	CCSprite* mainImg;
	CCSprite* fogImg;
	CCSprite* bombImg;
	
	void startAnimation()
	{
		mainImg->stopAllActions();
		CCAnimation* t_animation1 = CCAnimation::create();
		t_animation1->setDelayPerUnit(0.1);
		for(int i=2;i<=8;i++)
		{
			t_animation1->addSpriteFrameWithFileName(CCString::createWithFormat("ankle_snare%d.png", i)->getCString());
		}
		CCAnimate* t_animate1 = CCAnimate::create(t_animation1);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(AnkleSnare::addBomb));
		
		CCAnimation* t_animation1_1 = CCAnimation::create();
		t_animation1_1->setDelayPerUnit(0.1);
		t_animation1_1->addSpriteFrameWithFileName("ankle_snare9.png");
		CCAnimate* t_animate1_1 = CCAnimate::create(t_animation1_1);
		
		CCCallFunc* t_call1_1 = CCCallFunc::create(this, callfunc_selector(AnkleSnare::addFog));
		
		CCAnimation* t_animation2 = CCAnimation::create();
		t_animation2->setDelayPerUnit(0.1);
		for(int i=10;i<=11;i++)
		{
			t_animation2->addSpriteFrameWithFileName(CCString::createWithFormat("ankle_snare%d.png", i)->getCString());
		}
		CCAnimate* t_animate2 = CCAnimate::create(t_animation2);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(AnkleSnare::removeMainImg));
		
		
		CCAction* t_seq = CCSequence::create(t_animate1, t_call1, t_animate1_1, t_call1_1, t_animate2, t_call2, NULL);
		mainImg->runAction(t_seq);
	}
	
	void addBomb()
	{
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=1;i<=4;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("special_attack_bomb%d.png", i)->getCString());
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(AnkleSnare::removeBomb));
		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_animate, t_call2);
		bombImg->setVisible(true);
		bombImg->runAction(t_seq2);
	}
	
	void addFog()
	{
		CCScaleTo* t_scale1 = CCScaleTo::create(0.35, 16);
		CCScaleTo* t_scale2 = CCScaleTo::create(0.15, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(AnkleSnare::removeFog));
		CCAction* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
		fogImg->runAction(t_seq);
	}
	
	void removeBomb()
	{
		bombImg->removeFromParentAndCleanup(true);
	}
	
	void removeFog()
	{
		fogImg->removeFromParentAndCleanup(true);
	}
	void removeMainImg()
	{
		mainImg->removeFromParentAndCleanup(true);
	}
	
	void selfRemove()
	{
		if(getChildrenCount() <= 0)
		{
			unschedule(schedule_selector(AnkleSnare::selfRemove));
			removeFromParentAndCleanup(true);
		}
	}
	
	
	void myInit()
	{
		mainImg = CCSprite::create("ankle_snare1.png");
		mainImg->setAnchorPoint(ccp(0.5,0));
		mainImg->setPosition(ccp(160,80));
		mainImg->setScale(0);
		addChild(mainImg);
		
		fogImg = CCSprite::create("tornado_fog.png");
		fogImg->setPosition(ccp(160,180));
		fogImg->setScale(0);
		addChild(fogImg);
		
		bombImg = CCSprite::create("special_attack_bomb1.png");
		bombImg->setPosition(ccp(160,180));
		bombImg->setVisible(false);
		addChild(bombImg);
		
		schedule(schedule_selector(AnkleSnare::selfRemove));
	}
};

class LifeItem : public AttackItem
{
public:
	static LifeItem* create()
	{
		LifeItem* t_li = new LifeItem();
		t_li->myInit();
		t_li->autorelease();
		return t_li;
	}
	
	virtual void showItem()
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
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "item position infinity loop", "ok", 1, this, alertfuncII_selector(LifeItem::alertAction));
		}
		itemImg = CCSprite::create("life_item.png");
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		itemImg->setPosition(item_point);
		addChild(itemImg);
		
		hide_cnt = 0;
		schedule(schedule_selector(LifeItem::checkItemSide));
	}
	
	virtual ~LifeItem()
	{
		CCTextureCache::sharedTextureCache()->removeAllTextures();
	}
	
private:
	IntPoint myPoint;
	CCSprite* itemImg;
	int damage_frame;
	float damage;
	float total_damage;
	float sum_damage;
	
	int dex;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void checkItemSide()
	{
		hide_cnt++;
		
		if(hide_cnt >= base_hide_time)
		{
			unschedule(schedule_selector(LifeItem::checkItemSide));
			itemImg->removeFromParentAndCleanup(true);
			startCast();
			return;
		}
		
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
		
		if(side_cnt == 4)
		{
//			myGD->communication("EP_addJackAttack");
			AudioEngine::sharedInstance()->playEffect("sound_pil_life.mp3",false);
			itemImg->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(LifeItem::checkItemSide));
			//			myGD->communication("Main_startSpecialAttack");
			AnkleSnare* t_as = AnkleSnare::create();
			addChild(t_as);
			t_as->startMyAction();
			
			damage_frame = 0;
			schedule(schedule_selector(LifeItem::resetChecking));
		}
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
			unschedule(schedule_selector(LifeItem::resetChecking));
			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
			startCast();
		}
	}
	
	void myInit()
	{
		base_hide_time = AttackItemBaseHideTime;
		
		base_cast_time = AttackItemBaseCastTime - 4*AttackItemBaseSubValue;
		
		int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(recent_card_number >= 0)
		{
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
			damage = damage < 1 ? 1 : damage;
			dex = dex < 1 ? 1 : dex;
		}
		else
		{
			damage = 1;
			dex = 1;
		}
		
		total_damage = damage*(AttackItemBaseDamage+4*AttackItemBaseUpRate);;
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

class IceFragment : public CCSprite
{
public:
	static IceFragment* create(float t_angle, float t_speed)
	{
		IceFragment* t_if = new IceFragment();
		t_if->myInit(t_angle, t_speed);
		t_if->autorelease();
		return t_if;
	}
	
	void startMyAction()
	{
		CCDelayTime* t_delay = CCDelayTime::create(1);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(IceFragment::stopMyAction));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
	
private:
	CCPoint dv;
	
	void myAction()
	{
		setPosition(ccpAdd(getPosition(), dv));
		
		CCPoint r_point = ccpAdd(getPosition(), getParent()->getParent()->getPosition());
		
		if(r_point.x < -50 || r_point.x > 370 || r_point.y < -50 || r_point.y > 530)
		{
			unschedule(schedule_selector(IceFragment::myAction));
			removeFromParentAndCleanup(true);
		}
	}
	
	void stopMyAction()
	{
		stopAllActions();
		schedule(schedule_selector(IceFragment::myAction));
	}
	
	void myInit(float t_angle, float t_speed)
	{
		initWithFile("ice_storm_fragment.png");
		setRotation(-t_angle+90);
		
		dv.x = 1;
		dv.y = tanf(t_angle/180.f*M_PI);
		
		if((t_angle >= 90 && t_angle < 270) || t_angle < -90)
			dv = ccpMult(dv, -1.f);
		
		float div_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
		
		dv = ccpMult(dv, 1.f/div_value);
		dv = ccpMult(dv, t_speed);
	}
};

class IceStorm : public CCNode
{
public:
	static IceStorm* create()
	{
		IceStorm* t_is = new IceStorm();
		t_is->myInit();
		t_is->autorelease();
		return t_is;
	}
	
	void startMyAction()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(IceStorm::realStart));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		ice_main->runAction(t_seq);
	}
	
private:
	int ing_frame;
	CCSpriteBatchNode* batch_node;
	CCSprite* ice_main;
	
	void realStart()
	{
		ing_frame = 0;
		schedule(schedule_selector(IceStorm::myAction));
	}
	
	void myAction()
	{
		ing_frame++;
		
		IceFragment* t_ic = IceFragment::create(rand()%360 - 180, 12);
		batch_node->addChild(t_ic);
		t_ic->startMyAction();
		
		if(ing_frame >= 300)
		{
			stopMyAction();
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(IceStorm::myAction));
		startSelfRemove();
	}
	
	void startSelfRemove()
	{
		schedule(schedule_selector(IceStorm::selfRemove));
	}
	
	void selfRemove()
	{
		if(batch_node->getChildrenCount() <= 0)
		{
			unschedule(schedule_selector(IceStorm::selfRemove));
			CCScaleTo* t_scale = CCScaleTo::create(0.2f, 0.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(IceStorm::realRemove));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
			
			ice_main->runAction(t_seq);
		}
	}
	
	void realRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit()
	{
		setPosition(ccp(160,240));
		batch_node = CCSpriteBatchNode::create("ice_storm_fragment.png");
		addChild(batch_node);
		
		ice_main = CCSprite::create("ice_storm_main1.png");
		ice_main->setScale(0);
		addChild(ice_main);
		
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=1;i<=4;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("ice_storm_main%d.png",i)->getCString());
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		ice_main->runAction(t_repeat);
	}
};

class IceItem : public AttackItem
{
public:
	static IceItem* create()
	{
		IceItem* t_ii = new IceItem();
		t_ii->myInit();
		t_ii->autorelease();
		return t_ii;
	}
	
	virtual void showItem()
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
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "item position infinity loop", "ok", 1, this, alertfuncII_selector(IceItem::alertAction));
		}
		itemImg = CCSprite::create("ice_item.png");
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		itemImg->setPosition(item_point);
		addChild(itemImg);
		
		hide_cnt = 0;
		schedule(schedule_selector(IceItem::checkItemSide));
	}
	
	virtual ~IceItem()
	{
		CCTextureCache::sharedTextureCache()->removeAllTextures();
	}
	
private:
	IntPoint myPoint;
	CCSprite* itemImg;
	int damage_frame;
	float damage;
	float total_damage;
	float sum_damage;
	
	int dex;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void checkItemSide()
	{
		hide_cnt++;
		
		if(hide_cnt >= base_hide_time)
		{
			unschedule(schedule_selector(IceItem::checkItemSide));
			itemImg->removeFromParentAndCleanup(true);
			startCast();
			return;
		}
		
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
		
		if(side_cnt == 4)
		{
//			myGD->communication("EP_addJackAttack");
			AudioEngine::sharedInstance()->playEffect("sound_pil_ice.mp3",false);
			itemImg->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(IceItem::checkItemSide));
			//			myGD->communication("Main_startSpecialAttack");
			IceStorm* t_is = IceStorm::create();
			addChild(t_is);
			t_is->startMyAction();
			
			damage_frame = 0;
			schedule(schedule_selector(IceItem::resetChecking));
		}
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
			unschedule(schedule_selector(IceItem::resetChecking));
			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
			startCast();
		}
	}
	
	void myInit()
	{
		base_hide_time = AttackItemBaseHideTime;
		
		base_cast_time = AttackItemBaseCastTime - 4*AttackItemBaseSubValue;
		
		int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(recent_card_number >= 0)
		{
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
			damage = damage < 1 ? 1 : damage;
			dex = dex < 1 ? 1 : dex;
		}
		else
		{
			damage = 1;
			dex = 1;
		}
		
		total_damage = damage*(AttackItemBaseDamage+4*AttackItemBaseUpRate);;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_fragment.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main1.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main2.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main3.png");
		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main4.png");
		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
	}
};

#define ARROW_GRAVITY	-0.5
class FireArrow : public CCSprite
{
public:
	static FireArrow* create()
	{
		FireArrow* t_fa = new FireArrow();
		t_fa->myInit();
		t_fa->autorelease();
		return t_fa;
	}
	
	void startMyAction()
	{
		schedule(schedule_selector(FireArrow::myAction));
	}
	
private:
	CCPoint f_p;
	CCPoint dv;
	CCPoint a_p;
	
	void myAction()
	{
		dv.y += ARROW_GRAVITY;
		
		a_p = ccpAdd(getPosition(), dv);
		setPosition(a_p);
		
		if(a_p.y <= f_p.y)
		{
			unschedule(schedule_selector(FireArrow::myAction));
			stopAllActions();
			CCSprite* t_texture = CCSprite::create("fire_bomb.png");
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<3;i++)
				t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*80, 0, 80, 87));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(FireArrow::selfRemove));
			
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
			runAction(t_seq);
		}
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit()
	{
		CCSprite* t_texture = CCSprite::create("fire_arrow.png");
		initWithTexture(t_texture->getTexture(), CCRectMake(0, 0, 22, 50));
		
		setAnchorPoint(ccp(0.5, 0));
		
		f_p.x = rand()%320;
		f_p.y = rand()%350;
		
		setPosition(ccpAdd(f_p, ccp(0,500)));
		dv = ccp(0,-2);
		
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(0, 0, 22, 50));
		t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(22, 0, 22, 50));
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		runAction(t_repeat);
	}
};

class FireStorm : public CCNode
{
public:
	static FireStorm* create()
	{
		FireStorm* t_fs = new FireStorm();
		t_fs->myInit();
		t_fs->autorelease();
		return t_fs;
	}
	
	void startMyAction()
	{
		ing_frame = 0;
		schedule(schedule_selector(FireStorm::myAction));
	}
	
private:
	int ing_frame;
	
	void myAction()
	{
		ing_frame++;
		
		if(ing_frame%2 == 0)
		{
			FireArrow* t_fa = FireArrow::create();
			addChild(t_fa);
			t_fa->startMyAction();
		}
		
		if(ing_frame >= 180)
		{
			unschedule(schedule_selector(FireStorm::myAction));
			schedule(schedule_selector(FireStorm::selfRemove));
		}
	}
	
	void selfRemove()
	{
		if(getChildrenCount() <= 0)
		{
			unschedule(schedule_selector(FireStorm::selfRemove));
			removeFromParentAndCleanup(true);
		}
	}
	
	void myInit()
	{
		
	}
};

class FireItem : public AttackItem
{
public:
	static FireItem* create()
	{
		FireItem* t_fi = new FireItem();
		t_fi->myInit();
		t_fi->autorelease();
		return t_fi;
	}
	
	virtual void showItem()
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
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "item position infinity loop", "ok", 1, this, alertfuncII_selector(FireItem::alertAction));
		}
		itemImg = CCSprite::create("fire_item.png");
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		itemImg->setPosition(item_point);
		addChild(itemImg);
		
		hide_cnt = 0;
		schedule(schedule_selector(FireItem::checkItemSide));
	}
	
	virtual ~FireItem()
	{
		CCTextureCache::sharedTextureCache()->removeAllTextures();
	}
	
private:
	IntPoint myPoint;
	CCSprite* itemImg;
	int damage_frame;
	float damage;
	float total_damage;
	float sum_damage;
	
	int dex;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void checkItemSide()
	{
		hide_cnt++;
		
		if(hide_cnt >= base_hide_time)
		{
			unschedule(schedule_selector(FireItem::checkItemSide));
			itemImg->removeFromParentAndCleanup(true);
			startCast();
			return;
		}
		
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
		
		if(side_cnt == 4)
		{
//			myGD->communication("EP_addJackAttack");
			AudioEngine::sharedInstance()->playEffect("sound_pil_fire.mp3",false);
			itemImg->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(FireItem::checkItemSide));
			//			myGD->communication("Main_startSpecialAttack");
			FireStorm* t_fs = FireStorm::create();
			addChild(t_fs);
			t_fs->startMyAction();
			
			damage_frame = 0;
			schedule(schedule_selector(FireItem::resetChecking));
		}
	}
	
	void attack(float t_damage)
	{
		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
		
		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f));
		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
		myGD->communication("CP_startDamageReaction", mainCumber, t_damage, (float)(rand()%360-180.f));
		
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumber_cnt = subCumberArray->count();
		
		for(int i=0;i<cumber_cnt;i++)
		{
			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f)); // ccpoint
			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
			myGD->communication("CP_startDamageReaction", t_subCumber, t_damage, (float)(rand()%360-180.f));
//			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
			
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
			unschedule(schedule_selector(FireItem::resetChecking));
			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
			startCast();
		}
	}
	
	void myInit()
	{
		base_hide_time = AttackItemBaseHideTime;
		
		base_cast_time = AttackItemBaseCastTime - 4*AttackItemBaseSubValue;
		
		int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(recent_card_number >= 0)
		{
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
			damage = damage < 1 ? 1 : damage;
			dex = dex < 1 ? 1 : dex;
		}
		else
		{
			damage = 1;
			dex = 1;
		}
		
		total_damage = damage*(AttackItemBaseDamage+4*AttackItemBaseUpRate);;
		sum_damage = 0;
		
		CCTextureCache::sharedTextureCache()->addImage("fire_arrow.png");
		CCTextureCache::sharedTextureCache()->addImage("fire_bomb.png");
	}
};

class PlasmaStorm : public CCNode
{
public:
	static PlasmaStorm* create()
	{
		PlasmaStorm* t_ps = new PlasmaStorm();
		t_ps->myInit();
		t_ps->autorelease();
		return t_ps;
	}
	
	void startMyAction()
	{
		initStorm();
	}
	
private:
	// repeat 3 times
	CCSprite* mainImg;
	int ing_frame;
	CCSprite* fogImg;
	
	void initStorm()
	{
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.05);
		for(int i=1;i<=4;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("plasma_storm_init%d.png", i)->getCString());
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlasmaStorm::repeatStorm));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
		mainImg->runAction(t_seq);
	}
	
	void repeatStorm()
	{
		mainImg->stopAllActions();
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=1;i<=3;i++)
		{
			t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("plasma_storm_repeat%d.png", i)->getCString());
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		mainImg->runAction(t_repeat);
		ing_frame = 0;
		schedule(schedule_selector(PlasmaStorm::framing));
	}
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame > 120)
		{
			unschedule(schedule_selector(PlasmaStorm::framing));
			fogImg = CCSprite::create("tornado_fog.png");
			fogImg->setScale(0);
			fogImg->setPosition(ccp(160,215));
			addChild(fogImg);
			CCScaleTo* t_scale1 = CCScaleTo::create(0.7f, 16.f);
			CCCallFunc* t_mainRemove = CCCallFunc::create(this, callfunc_selector(PlasmaStorm::mainRemove));
			CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 0.f);
			CCCallFunc* t_selfRemove = CCCallFunc::create(this, callfunc_selector(PlasmaStorm::selfRemove));
			
			CCAction* t_action = CCSequence::create(t_scale1, t_mainRemove, t_scale2, t_selfRemove, NULL);
			fogImg->runAction(t_action);
		}
	}
	
	void mainRemove()
	{
		mainImg->stopAllActions();
		mainImg->removeFromParentAndCleanup(true);
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit()
	{
		mainImg = CCSprite::create("plasma_storm_init1.png");
		mainImg->setAnchorPoint(ccp(0.5,1.0));
		mainImg->setPosition(ccp(160,430));
		addChild(mainImg);
	}
};

class PlasmaItem : public AttackItem
{
public:
	static PlasmaItem* create()
	{
		PlasmaItem* t_pi = new PlasmaItem();
		t_pi->myInit();
		t_pi->autorelease();
		return t_pi;
	}
	
	virtual void showItem()
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
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "item position infinity loop", "ok", 1, this, alertfuncII_selector(PlasmaItem::alertAction));
		}
		itemImg = CCSprite::create("plasma_item.png");
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		itemImg->setPosition(item_point);
		addChild(itemImg);
		
		hide_cnt = 0;
		schedule(schedule_selector(PlasmaItem::checkItemSide));
	}
	
	virtual ~PlasmaItem()
	{
		CCTextureCache::sharedTextureCache()->removeAllTextures();
	}
	
private:
	IntPoint myPoint;
	CCSprite* itemImg;
	int damage_frame;
	float damage;
	float total_damage;
	float sum_damage;
	
	int dex;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void checkItemSide()
	{
		hide_cnt++;
		
		if(hide_cnt >= base_hide_time)
		{
			unschedule(schedule_selector(PlasmaItem::checkItemSide));
			itemImg->removeFromParentAndCleanup(true);
			startCast();
			return;
		}
		
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
		
		if(side_cnt == 4)
		{
//			myGD->communication("EP_addJackAttack");
			AudioEngine::sharedInstance()->playEffect("sound_pil_plasma.mp3",false);
			itemImg->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(PlasmaItem::checkItemSide));
			//			myGD->communication("Main_startSpecialAttack");
			PlasmaStorm* t_ps = PlasmaStorm::create();
			addChild(t_ps);
			t_ps->startMyAction();
			
			damage_frame = 0;
			schedule(schedule_selector(PlasmaItem::resetChecking));
		}
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
			unschedule(schedule_selector(PlasmaItem::resetChecking));
			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
			startCast();
		}
	}
	
	void myInit()
	{
		base_hide_time = AttackItemBaseHideTime;
		
		base_cast_time = AttackItemBaseCastTime - 4*AttackItemBaseSubValue;
		
		int recent_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(recent_card_number >= 0)
		{
			damage = NSDS_GI(kSDS_CI_int1_missile_power_i, recent_card_number); // * damage_per
			dex = NSDS_GI(kSDS_CI_int1_missile_dex_i, recent_card_number);
			
			damage = damage < 1 ? 1 : damage;
			dex = dex < 1 ? 1 : dex;
		}
		else
		{
			damage = 1;
			dex = 1;
		}
		
		total_damage = damage*(AttackItemBaseDamage+4*AttackItemBaseUpRate);;
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

class FastItem : public AttackItem
{
public:
	static FastItem* create()
	{
		FastItem* t_pi = new FastItem();
		t_pi->myInit();
		t_pi->autorelease();
		return t_pi;
	}
	
	virtual void showItem()
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
			if(check_loop_cnt > 3000)
				AlertEngine::sharedInstance()->addSingleAlert("error", "item position infinity loop", "ok", 1, this, alertfuncII_selector(FastItem::alertAction));
		}
		itemImg = CCSprite::create("fast_item.png");
		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
		itemImg->setPosition(item_point);
		addChild(itemImg);
		
		hide_cnt = 0;
		schedule(schedule_selector(FastItem::checkItemSide));
	}
	
private:
	IntPoint myPoint;
	CCSprite* itemImg;
	int ing_frame;
	
	void alertAction(int t1, int t2)
	{
		
	}
	
	void checkItemSide()
	{
		hide_cnt++;
		
		if(hide_cnt >= base_hide_time)
		{
			unschedule(schedule_selector(FastItem::checkItemSide));
			itemImg->removeFromParentAndCleanup(true);
			startCast();
			return;
		}
		
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
		
		if(side_cnt == 4)
		{
			itemImg->removeFromParentAndCleanup(true);
			unschedule(schedule_selector(FastItem::checkItemSide));
			
			myGD->setAlphaSpeed(myGD->getAlphaSpeed()+0.5f);
			
			ing_frame = 0;
			schedule(schedule_selector(FastItem::resetChecking));
		}
	}
	
	void resetChecking()
	{
		ing_frame++;
		if(ing_frame >= 300)
		{
			unschedule(schedule_selector(FastItem::resetChecking));
			myGD->setAlphaSpeed(myGD->getAlphaSpeed()-0.5f);
			startCast();
		}
	}
	
	void myInit()
	{
		base_hide_time = 3000 + rand()%601;
		
		base_cast_time = 60;
	}
};

#endif
