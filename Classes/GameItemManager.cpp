//
//  GameItemManager.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#include "GameItemManager.h"



//class GameItemPlasma : public GameItemBase
//{
//public:
//	static GameItemPlasma* create(bool is_near)
//	{
//		GameItemPlasma* t_gisu = new GameItemPlasma();
//		t_gisu->myInit(is_near);
//		t_gisu->autorelease();
//		return t_gisu;
//	}
//
//private:
//	int my_elemental;
//	int damage_frame;
//	int ing_frame;
//
//	float damage;
//	float total_damage;
//	float sum_damage;
//
//	virtual void acting()
//	{
//		myGD->communication("EP_addJackAttack");
//		AudioEngine::sharedInstance()->playEffect("sound_pil_plasma.mp3",false);
//		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
//		unschedule(schedule_selector(GameItemPlasma::framing));
//		//			myGD->communication("Main_startSpecialAttack");
//		PlasmaStorm* t_ps = PlasmaStorm::create();
//		addChild(t_ps);
//		t_ps->startMyAction();
//
//		damage_frame = 0;
//		schedule(schedule_selector(GameItemPlasma::resetChecking));
//	}
//
//	void attack(float t_damage)
//	{
//		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
//
//		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 0, 1.f, 1.f));
//		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
//
//		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
//		{
//			myGD->communication("UI_subBossLife", t_damage);
//			myGD->communication("CP_startDamageReaction", rand()%360-180.f);
//		}
//
//
//		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//		int cumber_cnt = subCumberArray->count();
//
//		for(int i=0;i<cumber_cnt;i++)
//		{
//			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
//			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 0, 1.f, 1.f)); // ccpoint
//			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
//			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
//
//			if(subCumberArray->count() < cumber_cnt)
//			{
//				cumber_cnt--;
//				i--;
//			}
//		}
//	}
//
//	void resetChecking()
//	{
//		damage_frame++;
//		if(damage_frame%12 == 0)
//		{
//			attack(total_damage/20.f);
//			sum_damage += total_damage/20.f;
//		}
//
//		if(getChildrenCount() <= 0)
//		{
//			attack(total_damage-sum_damage);
//			//			myGD->communication("Main_stopSpecialAttack");
//			unschedule(schedule_selector(GameItemPlasma::resetChecking));
//			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
//		}
//	}
//
//	void myInit(bool is_near)
//	{
//
//
//		my_elemental = kElementCode_plasma;
//
//		holding_time = rand()%10 + 20;
//		holding_time *= 60;
//
//		setMyPoint(is_near);
//
//		if(myPoint.isNull())
//		{
//			CCDelayTime* t_delay = CCDelayTime::create(1.f);
//			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//
//			runAction(t_seq);
//
//			return;
//		}
//
//		item_img = CCSprite::create("item_bomb.png");
//		item_img->setScale(0.f);
//		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
//		item_img->setPosition(item_point);
//		addChild(item_img);
//
//		starting_side_cnt = getSideCount();
//
//		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemPlasma::startFraming));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
//
//		item_img->runAction(t_seq);
//
//		damage = 20;
//		total_damage = damage*10;
//		sum_damage = 0;
//
//		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init1.png");
//		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init2.png");
//		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init3.png");
//		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_init4.png");
//		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_repeat1.png");
//		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_repeat2.png");
//		CCTextureCache::sharedTextureCache()->addImage("plasma_storm_repeat3.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
//	}
//};

//class GameItemWind : public GameItemBase
//{
//public:
//	static GameItemWind* create(bool is_near)
//	{
//		GameItemWind* t_gisu = new GameItemWind();
//		t_gisu->myInit(is_near);
//		t_gisu->autorelease();
//		return t_gisu;
//	}
//
//private:
//	int my_elemental;
//	int damage_frame;
//	int ing_frame;
//	CCSprite* fogImg;
//	Tornado* t_1;
//	Tornado* t_2;
//	Tornado* t_3;
//	float damage;
//	float total_damage;
//	float sum_damage;
//
//	virtual void acting()
//	{
//		// wind
//		myGD->communication("EP_addJackAttack");
//		AudioEngine::sharedInstance()->playEffect("sound_pil_wind.mp3",false);
//		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
//		unschedule(schedule_selector(GameItemWind::framing));
//		t_1 = Tornado::create(ccp(-100,100), ccp(160,230), -1, 240);
//		addChild(t_1);
//
//		t_2 = Tornado::create(ccp(420,100), ccp(160,230), -1, 240);
//		addChild(t_2);
//
//		t_3 = Tornado::create(ccp(160,520), ccp(160,230), -1, 240);
//		addChild(t_3);
//
//		t_1->startMove();
//		t_2->startMove();
//		t_3->startMove();
//
//		fogImg = CCSprite::create("tornado_fog.png");
//		fogImg->setPosition(ccp(160,215));
//		fogImg->setScale(0);
//		addChild(fogImg);
//
//		damage_frame = 0;
//		schedule(schedule_selector(GameItemWind::resetChecking));
//		///
//	}
//	// wind
//	void fogZoom()
//	{
//		ing_frame++;
//
//		if(ing_frame%6 == 0)
//		{
//			if(ing_frame/6 >= 1 && ing_frame/6 <= 4)
//			{
//				fogImg->stopAllActions();
//				CCScaleTo* t_scale = CCScaleTo::create(0.1, fogImg->getScale()*2);
//				fogImg->runAction(t_scale);
//			}
//			else if(ing_frame/6 == 5)
//			{
//				fogImg->stopAllActions();
//				CCScaleTo* t_scale = CCScaleTo::create(0.2, 0);
//				fogImg->runAction(t_scale);
//			}
//		}
//
//		if(ing_frame >= 36)
//		{
//			stopFogZoom();
//		}
//	}
//
//	void stopFogZoom()
//	{
//		unschedule(schedule_selector(GameItemWind::fogZoom));
//		fogImg->removeFromParentAndCleanup(true);
//	}
//
//	void resetChecking()
//	{
//		damage_frame++;
//		if(damage_frame%40 == 0)
//		{
//			if(damage_frame/40 <= 4)
//			{
//				CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
//
//				myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f));
//				myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
//
//				if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
//				{
//					myGD->communication("UI_subBossLife", total_damage/10.f);
//					sum_damage += total_damage/10.f;
//					myGD->communication("CP_startDamageReaction", rand()%360-180.f);
//				}
//
//
//				CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//				int cumber_cnt = subCumberArray->count();
//
//				for(int i=0;i<cumber_cnt;i++)
//				{
//					CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
//					myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f)); // ccpoint
//					myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
//					myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage/10.f, rand()%360-180.f);
//
//					if(subCumberArray->count() < cumber_cnt)
//					{
//						cumber_cnt--;
//						i--;
//					}
//				}
//			}
//			else if(damage_frame/40 == 6)
//			{
//				t_1->startEnding();
//				t_2->startEnding();
//				t_3->startEnding();
//
//				ing_frame = 0;
//				CCScaleTo* t_scale = CCScaleTo::create(0.1, 1.0);
//				fogImg->runAction(t_scale);
//				schedule(schedule_selector(GameItemWind::fogZoom));
//			}
//		}
//
//		if(getChildrenCount() <= 0)
//		{
//			CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
//
//			myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f));
//			myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
//
//			if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
//			{
//				myGD->communication("UI_subBossLife", total_damage-sum_damage);
//				myGD->communication("CP_startDamageReaction", rand()%360-180.f);
//			}
//
//
//			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//			int cumber_cnt = subCumberArray->count();
//
//			for(int i=0;i<cumber_cnt;i++)
//			{
//				CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
//				myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 1.f, 1.f, 1.f)); // ccpoint
//				myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
//				myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage-sum_damage, rand()%360-180.f);
//
//				if(subCumberArray->count() < cumber_cnt)
//				{
//					cumber_cnt--;
//					i--;
//				}
//			}
//			unschedule(schedule_selector(GameItemWind::resetChecking));
//			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
//			removeFromParentAndCleanup(true);
//		}
//	}
//	///
//
//	void myInit(bool is_near)
//	{
//		my_elemental = kElementCode_wind;
//
//		holding_time = rand()%10 + 20;
//		holding_time *= 60;
//
//		setMyPoint(is_near);
//
//		if(myPoint.isNull())
//		{
//			CCDelayTime* t_delay = CCDelayTime::create(1.f);
//			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//
//			runAction(t_seq);
//
//			return;
//		}
//
//		item_img = CCSprite::create("item_bomb.png");
//		item_img->setScale(0.f);
//		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
//		item_img->setPosition(item_point);
//		addChild(item_img);
//
//		starting_side_cnt = getSideCount();
//
//		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemWind::startFraming));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
//
//		item_img->runAction(t_seq);
//
//		damage = 20;
//		total_damage = damage*10;
//		sum_damage = 0;
//
//		CCTextureCache::sharedTextureCache()->addImage("tornado1_repeat1.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado1_repeat2.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado1_repeat3.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado1_final1.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado1_final2.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado1_final3.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado1_final4.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
//	}
//};
//
//class GameItemLightning : public GameItemBase
//{
//public:
//	static GameItemLightning* create(bool is_near)
//	{
//		GameItemLightning* t_gisu = new GameItemLightning();
//		t_gisu->myInit(is_near);
//		t_gisu->autorelease();
//		return t_gisu;
//	}
//
//private:
//	int my_elemental;
//	int damage_frame;
//	int ing_frame;
//	LightningStorm* myStorm;
//	CCSprite* fogImg;
//	CCSprite* bombImg;
//
//	float damage;
//	float total_damage;
//	float sum_damage;
//
//	virtual void acting()
//	{
//		myGD->communication("EP_addJackAttack");
//		AudioEngine::sharedInstance()->playEffect("sound_pil_lightning.mp3",false);
//		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
//		unschedule(schedule_selector(GameItemLightning::framing));
//		//			myGD->communication("Main_startSpecialAttack");
//
//		myStorm = LightningStorm::create();
//		addChild(myStorm);
//		myStorm->startMyAction();
//
//		fogImg = CCSprite::create("tornado_fog.png");
//		fogImg->setPosition(ccp(160,240));
//		fogImg->setScale(0);
//		addChild(fogImg);
//
//		bombImg = CCSprite::create("special_attack_bomb1.png");
//		bombImg->setPosition(ccp(160,240));
//		bombImg->setVisible(false);
//		addChild(bombImg);
//
//		damage_frame = 0;
//		schedule(schedule_selector(GameItemLightning::resetChecking));
//	}
//
//	void fogZoom()
//	{
//		ing_frame++;
//
//		if(ing_frame%6 == 0)
//		{
//			if(ing_frame/6 >= 1 && ing_frame/6 <= 4)
//			{
//				fogImg->stopAllActions();
//				CCScaleTo* t_scale = CCScaleTo::create(0.1, fogImg->getScale()*2);
//				fogImg->runAction(t_scale);
//			}
//			else if(ing_frame/6 == 5)
//			{
//				myStorm->stopRepeatRemove();
//				fogImg->stopAllActions();
//				CCScaleTo* t_scale = CCScaleTo::create(0.2, 0);
//				fogImg->runAction(t_scale);
//			}
//		}
//
//		if(ing_frame >= 36)
//		{
//			stopFogZoom();
//		}
//	}
//
//	void stopFogZoom()
//	{
//		unschedule(schedule_selector(GameItemLightning::fogZoom));
//		fogImg->removeFromParentAndCleanup(true);
//	}
//
//	void removeBomb()
//	{
//		bombImg->removeFromParentAndCleanup(true);
//	}
//
//	void resetChecking()
//	{
//		damage_frame++;
//		if(damage_frame%9 == 0)
//		{
//			CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
//
//			myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f));
//			myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
//
//			if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
//			{
//				myGD->communication("UI_subBossLife", total_damage/23.f);
//				sum_damage += total_damage/23.f;
//				myGD->communication("CP_startDamageReaction", rand()%360-180.f);
//			}
//
//
//			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//			int cumber_cnt = subCumberArray->count();
//
//			for(int i=0;i<cumber_cnt;i++)
//			{
//				CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
//				myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f)); // ccpoint
//				myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
//				myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage/23.f, rand()%360-180.f);
//
//				if(subCumberArray->count() < cumber_cnt)
//				{
//					cumber_cnt--;
//					i--;
//				}
//			}
//		}
//
//		if(damage_frame == 140)
//		{
//			ing_frame = 0;
//			bombImg->setVisible(true);
//			CCAnimation* t_animation = CCAnimation::create();
//			t_animation->setDelayPerUnit(0.1);
//			for(int i=1;i<=4;i++)
//			{
//				t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("special_attack_bomb%d.png", i)->getCString());
//			}
//			CCAnimate* t_animate = CCAnimate::create(t_animation);
//			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemLightning::removeBomb));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
//			bombImg->runAction(t_seq);
//
//			CCScaleTo* t_scale = CCScaleTo::create(0.1, 1.0);
//			fogImg->runAction(t_scale);
//			schedule(schedule_selector(GameItemLightning::fogZoom));
//		}
//
//		if(getChildrenCount() <= 0)
//		{
//			CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
//
//			myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f));
//			myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
//
//			if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
//			{
//				myGD->communication("UI_subBossLife", total_damage-sum_damage);
//				myGD->communication("CP_startDamageReaction", rand()%360-180.f);
//			}
//
//
//			CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//			int cumber_cnt = subCumberArray->count();
//
//			for(int i=0;i<cumber_cnt;i++)
//			{
//				CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
//				myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 1.f, 0, 1.f)); // ccpoint
//				myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
//				myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, total_damage-sum_damage, rand()%360-180.f);
//
//				if(subCumberArray->count() < cumber_cnt)
//				{
//					cumber_cnt--;
//					i--;
//				}
//			}
//			unschedule(schedule_selector(GameItemLightning::resetChecking));
//			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
//		}
//	}
//
//	void myInit(bool is_near)
//	{
//		my_elemental = kElementCode_lightning;
//
//		holding_time = rand()%10 + 20;
//		holding_time *= 60;
//
//		setMyPoint(is_near);
//
//		if(myPoint.isNull())
//		{
//			CCDelayTime* t_delay = CCDelayTime::create(1.f);
//			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//
//			runAction(t_seq);
//
//			return;
//		}
//
//		item_img = CCSprite::create("item_bomb.png");
//		item_img->setScale(0.f);
//		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
//		item_img->setPosition(item_point);
//		addChild(item_img);
//
//		starting_side_cnt = getSideCount();
//
//		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemLightning::startFraming));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
//
//		item_img->runAction(t_seq);
//
//		damage = 20;
//		total_damage = damage*10;
//		sum_damage = 0;
//
//		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start1.png");
//		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start2.png");
//		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start3.png");
//		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_start4.png");
//		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_repeat1.png");
//		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_repeat2.png");
//		CCTextureCache::sharedTextureCache()->addImage("lightning_storm_repeat3.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb1.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb2.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb3.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb4.png");
//	}
//};
//
//class GameItemLife : public GameItemBase
//{
//public:
//	static GameItemLife* create(bool is_near)
//	{
//		GameItemLife* t_gisu = new GameItemLife();
//		t_gisu->myInit(is_near);
//		t_gisu->autorelease();
//		return t_gisu;
//	}
//
//private:
//	int my_elemental;
//	int damage_frame;
//	int ing_frame;
//
//	float damage;
//	float total_damage;
//	float sum_damage;
//
//	virtual void acting()
//	{
//		myGD->communication("EP_addJackAttack");
//		AudioEngine::sharedInstance()->playEffect("sound_pil_life.mp3",false);
//		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
//		unschedule(schedule_selector(GameItemLife::framing));
//		//			myGD->communication("Main_startSpecialAttack");
//		AnkleSnare* t_as = AnkleSnare::create();
//		addChild(t_as);
//		t_as->startMyAction();
//
//		damage_frame = 0;
//		schedule(schedule_selector(GameItemLife::resetChecking));
//	}
//
//	void attack(float t_damage)
//	{
//		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
//
//		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 1.f, 0, 1.f));
//		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
//
//		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
//		{
//			myGD->communication("UI_subBossLife", t_damage);
//			myGD->communication("CP_startDamageReaction", rand()%360-180.f);
//		}
//
//
//		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//		int cumber_cnt = subCumberArray->count();
//
//		for(int i=0;i<cumber_cnt;i++)
//		{
//			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
//			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 1.f, 0, 1.f)); // ccpoint
//			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
//			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
//
//			if(subCumberArray->count() < cumber_cnt)
//			{
//				cumber_cnt--;
//				i--;
//			}
//		}
//	}
//
//	void resetChecking()
//	{
//		damage_frame++;
//		if(damage_frame == 18 || damage_frame == 42 || damage_frame == 54 || damage_frame == 60 || damage_frame == 66 || damage_frame == 72)
//		{
//			attack(total_damage/10.f);
//			sum_damage += total_damage/10.f;
//		}
//
//		if(getChildrenCount() <= 0)
//		{
//			attack(total_damage-sum_damage);
//			//			myGD->communication("Main_stopSpecialAttack");
//			unschedule(schedule_selector(GameItemLife::resetChecking));
//			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
//		}
//	}
//
//	void myInit(bool is_near)
//	{
//
//
//		my_elemental = kElementCode_life;
//
//		holding_time = rand()%10 + 20;
//		holding_time *= 60;
//
//		setMyPoint(is_near);
//
//		if(myPoint.isNull())
//		{
//			CCDelayTime* t_delay = CCDelayTime::create(1.f);
//			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//
//			runAction(t_seq);
//
//			return;
//		}
//
//		item_img = CCSprite::create("item_bomb.png");
//		item_img->setScale(0.f);
//		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
//		item_img->setPosition(item_point);
//		addChild(item_img);
//
//		starting_side_cnt = getSideCount();
//
//		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemLife::startFraming));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
//
//		item_img->runAction(t_seq);
//
//		damage = 20;
//		total_damage = damage*10;
//		sum_damage = 0;
//
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare1.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare2.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare3.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare4.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare5.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare6.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare7.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare8.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare9.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare10.png");
//		CCTextureCache::sharedTextureCache()->addImage("ankle_snare11.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb1.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb2.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb3.png");
//		CCTextureCache::sharedTextureCache()->addImage("special_attack_bomb4.png");
//	}
//};
//
//class GameItemIce : public GameItemBase
//{
//public:
//	static GameItemIce* create(bool is_near)
//	{
//		GameItemIce* t_gisu = new GameItemIce();
//		t_gisu->myInit(is_near);
//		t_gisu->autorelease();
//		return t_gisu;
//	}
//
//private:
//	int my_elemental;
//	int damage_frame;
//	int ing_frame;
//
//	float damage;
//	float total_damage;
//	float sum_damage;
//
//	virtual void acting()
//	{
//		myGD->communication("EP_addJackAttack");
//		AudioEngine::sharedInstance()->playEffect("sound_pil_ice.mp3",false);
//		AudioEngine::sharedInstance()->playEffect("sound_bomb_common.mp3",false);
//		unschedule(schedule_selector(GameItemIce::framing));
//		//			myGD->communication("Main_startSpecialAttack");
//		IceStorm* t_is = IceStorm::create();
//		addChild(t_is);
//		t_is->startMyAction();
//
//		damage_frame = 0;
//		schedule(schedule_selector(GameItemIce::resetChecking));
//	}
//
//	void attack(float t_damage)
//	{
//		CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
//
//		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 0, 1.f, 1.f));
//		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
//
//		if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
//		{
//			myGD->communication("UI_subBossLife", t_damage);
//			CCLog("ice : %.1f", t_damage);
//			myGD->communication("CP_startDamageReaction", rand()%360-180.f);
//		}
//
//
//		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
//		int cumber_cnt = subCumberArray->count();
//
//		for(int i=0;i<cumber_cnt;i++)
//		{
//			CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
//			myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(0, 0, 1.f, 1.f)); // ccpoint
//			myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
//			myGD->communication("CP_decreaseLifeForSubCumber", (CCObject*)t_subCumber, t_damage, rand()%360-180.f);
//
//			if(subCumberArray->count() < cumber_cnt)
//			{
//				cumber_cnt--;
//				i--;
//			}
//		}
//	}
//
//	void resetChecking()
//	{
//		damage_frame++;
//		if(damage_frame >= 42 && damage_frame <= 342)
//		{
//			if((damage_frame-42)%18 == 0)
//			{
//				attack(total_damage/20.f);
//				sum_damage += total_damage/20.f;
//			}
//		}
//
//		if(getChildrenCount() <= 0)
//		{
//			attack(total_damage-sum_damage);
//			//			myGD->communication("Main_stopSpecialAttack");
//			unschedule(schedule_selector(GameItemIce::resetChecking));
//			sum_damage = 0;
//			myGD->communication("EP_subJackAttack");
//		}
//	}
//
//	void myInit(bool is_near)
//	{
//
//
//		my_elemental = kElementCode_water;
//
//		holding_time = rand()%10 + 20;
//		holding_time *= 60;
//
//		setMyPoint(is_near);
//
//		if(myPoint.isNull())
//		{
//			CCDelayTime* t_delay = CCDelayTime::create(1.f);
//			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemBase::selfRemove));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//
//			runAction(t_seq);
//
//			return;
//		}
//
//		item_img = CCSprite::create("item_bomb.png");
//		item_img->setScale(0.f);
//		CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
//		item_img->setPosition(item_point);
//		addChild(item_img);
//
//		starting_side_cnt = getSideCount();
//
//		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemIce::startFraming));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
//
//		item_img->runAction(t_seq);
//
//		damage = 20;
//		total_damage = damage*10;
//		sum_damage = 0;
//
//		CCTextureCache::sharedTextureCache()->addImage("ice_storm_fragment.png");
//		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main1.png");
//		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main2.png");
//		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main3.png");
//		CCTextureCache::sharedTextureCache()->addImage("ice_storm_main4.png");
//		CCTextureCache::sharedTextureCache()->addImage("tornado_fog.png");
//	}
//};