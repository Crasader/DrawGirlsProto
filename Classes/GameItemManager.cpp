//
//  GameItemManager.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#include "GameItemManager.h"
#include "LogData.h"
#include "KSUtil.h"
#include "AttackItem.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"

void GameItemBase::selfRemove()
{
	removeFromParentAndCleanup(true);
}

void GameItemBase::startFraming()
{
	frame_cnt = 0;
	schedule(schedule_selector(GameItemBase::framing));
}

void GameItemBase::framing()
{
	frame_cnt++;
	
	if(getSideCount() > starting_side_cnt)
	{
		if(mySD->getClearCondition() == kCLEAR_itemCollect)
			myGD->communication("UI_takeItemCollect");
		
		item_img->removeFromParentAndCleanup(true);
		unschedule(schedule_selector(GameItemBase::framing));
		
		(target_effect->*delegate_effect)(myPoint.convertToCCP());
		
		acting();
		return;
	}
	
	
	if(holding_time <= frame_cnt)
	{
		unschedule(schedule_selector(GameItemBase::framing));
		startHide();
	}
}

void GameItemBase::setTakeEffectFunc(CCObject* t_effect, SEL_CallFuncCCp d_effect)
{
	target_effect = t_effect;
	delegate_effect = d_effect;
}

int GameItemBase::getSideCount()
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

void GameItemBase::startHide()
{
	keep_scale_value = item_img->getScale();
	
	item_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f,
													 [=](float t)
													 {
														 item_img->setScale(keep_scale_value - t*(keep_scale_value - 0.f));
													 },
													 [=](float t)
													 {
														 selfRemove();
													 }));
}

void GameItemBase::setMyPoint(bool is_near)
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


GameItemAddTime* GameItemAddTime::create(bool is_near)
{
	GameItemAddTime* t_giat = new GameItemAddTime();
	t_giat->myInit(is_near);
	t_giat->autorelease();
	return t_giat;
}

void GameItemAddTime::selfRemove()
{
	GameItemAddTime* recreate = GameItemAddTime::create(false);
	getParent()->addChild(recreate);
	
	GameItemBase::selfRemove();
}

void GameItemAddTime::acting()
{
	myGD->communication("UI_takeAddTimeItem");
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "addTime");
	
	GameItemAddTime* recreate = GameItemAddTime::create(false);
	getParent()->addChild(recreate);
	
	removeFromParent();
}

void GameItemAddTime::myInit(bool is_near)
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
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_bonustime.ccbi",this));
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
}



GameItemSpeedUp* GameItemSpeedUp::create(bool is_near)
{
	GameItemSpeedUp* t_gisu = new GameItemSpeedUp();
	t_gisu->myInit(is_near);
	t_gisu->autorelease();
	return t_gisu;
}

void GameItemSpeedUp::acting()
{
	myGD->communication("Jack_takeSpeedUpItem");
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "speedUp");
	removeFromParent();
}

void GameItemSpeedUp::myInit(bool is_near)
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
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_speed.ccbi",this));
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
}



GameItemFast* GameItemFast::create(bool is_near)
{
	GameItemFast* t_gisu = new GameItemFast();
	t_gisu->myInit(is_near);
	t_gisu->autorelease();
	return t_gisu;
}

void GameItemFast::acting()
{
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "Fast");
	
	myGD->setAlphaSpeed(myGD->getAlphaSpeed() + 10.f);
	
	CCDelayTime* t_delay = CCDelayTime::create(SDS_GI(kSDF_stageInfo, mySD->getSilType(), "itemOption_fast_sec"));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemFast::ending));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	
	runAction(t_seq);
}

void GameItemFast::ending()
{
	myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 10.f);
	removeFromParentAndCleanup(true);
}

void GameItemFast::myInit(bool is_near)
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
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_dash.ccbi",this));
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
}



GameItemAttack* GameItemAttack::create(bool is_near)
{
	GameItemAttack* t_gisu = new GameItemAttack();
	t_gisu->myInit(is_near);
	t_gisu->autorelease();
	return t_gisu;
}

void GameItemAttack::acting()
{
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "attack");
	
	string missile_code;
	if(mySGD->getIsUsingFriendCard())
		missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, mySGD->getSelectedFriendCardData().card_number);
	else
		missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
	int missile_type = MissileDamageData::getMissileType(missile_code.c_str());
	
	//				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
	float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
	
	myGD->communication("MP_createJackMissile", missile_type, rand()%3 + 1, missile_speed, myPoint.convertToCCP());
	removeFromParent();
}

void GameItemAttack::myInit(bool is_near)
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
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_attack.ccbi",this));
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
}



GameItemSubOneDie* GameItemSubOneDie::create(bool is_near)
{
	GameItemSubOneDie* t_gisod = new GameItemSubOneDie();
	t_gisod->myInit(is_near);
	t_gisod->autorelease();
	return t_gisod;
}

void GameItemSubOneDie::acting()
{
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "subOneDie");
	myGD->communication("MP_subOneDie");
	removeFromParent();
}

void GameItemSubOneDie::myInit(bool is_near)
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



GameItemSilence* GameItemSilence::create(bool is_near)
{
	GameItemSilence* t_gisod = new GameItemSilence();
	t_gisod->myInit(is_near);
	t_gisod->autorelease();
	return t_gisod;
}

void GameItemSilence::acting()
{
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "silence");
	myGD->communication("CP_silenceItem", true);
	CCDelayTime* t_delay = CCDelayTime::create(mySD->getSilenceItemOption());
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemSilence::finalAction));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	
	runAction(t_seq);
	//		removeFromParent();
}

void GameItemSilence::finalAction()
{
	myGD->communication("CP_silenceItem", false);
	removeFromParent();
}

void GameItemSilence::myInit(bool is_near)
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



GameItemFire* GameItemFire::create(bool is_near)
{
	GameItemFire* t_gisu = new GameItemFire();
	t_gisu->myInit(is_near);
	t_gisu->autorelease();
	return t_gisu;
}

void GameItemFire::acting()
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

void GameItemFire::attack(float t_damage)
{
	CCNode* mainCumber = myGD->getCommunicationNode("CP_getMainCumberPointer");
	
	myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f), rand()%360-180.f);
	myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
	myGD->communication("CP_startDamageReaction", mainCumber, t_damage, rand()%360-180.f);
	
	CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
	int cumber_cnt = subCumberArray->count();
	for(int i=0;i<cumber_cnt;i++)
	{
		CCNode* t_subCumber = (CCNode*)subCumberArray->objectAtIndex(i);
		myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f), rand()%360-180.f); // ccpoint
		myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
		myGD->communication("CP_startDamageReaction", t_subCumber, t_damage, rand()%360-180.f);
		
		if(subCumberArray->count() < cumber_cnt)
		{
			cumber_cnt--;
			i--;
		}
	}
}

void GameItemFire::resetChecking()
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

void GameItemFire::myInit(bool is_near)
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
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_bomb.ccbi",this));
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
	
	if(mySGD->getIsUsingFriendCard())
		damage = NSDS_GI(kSDS_CI_int1_missile_power_i, mySGD->getSelectedFriendCardData().card_number)*((mySGD->getSelectedFriendCardData().card_level-1)*0.1f+1.f);
	else if(myDSH->getIntegerForKey(kDSH_Key_selectedCard) > 0)
		damage = NSDS_GI(kSDS_CI_int1_missile_power_i, myDSH->getIntegerForKey(kDSH_Key_selectedCard))*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCard))-1)*0.1f+1.f);
	else
		damage = 1;
	
	total_damage = damage*10;
	sum_damage = 0;
	
	CCTextureCache::sharedTextureCache()->addImage("fire_arrow.png");
	CCTextureCache::sharedTextureCache()->addImage("fire_bomb.png");
}



ExchangeCoin* ExchangeCoin::create(int t_type, CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin)
{
	ExchangeCoin* t_ec = new ExchangeCoin();
	t_ec->myInit(t_type, t_ui, d_takeExchangeCoin);
	t_ec->autorelease();
	return t_ec;
}

void ExchangeCoin::showCoin()
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
	
	CCScaleTo* t_scale = CCScaleTo::create(0.5f, 1.f/myGD->game_scale);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ExchangeCoin::startMoving));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	
	coin_img->runAction(t_seq);
}

void ExchangeCoin::stopMoving()
{
	unschedule(schedule_selector(ExchangeCoin::moving));
}

void ExchangeCoin::smallScaleHiding()
{
	CCScaleTo* t_scale = CCScaleTo::create(1.5f, 0);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ExchangeCoin::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	runAction(t_seq);
}

void ExchangeCoin::startMoving()
{
	directionAngle = rand()%360;
	move_speed = 1.f;
	schedule(schedule_selector(ExchangeCoin::moving));
}

void ExchangeCoin::endTakeAction()
{
	(target_ui->*delegate_takeExchangeCoin)(getPosition(), myType);
	removeFromParent();
}

void ExchangeCoin::changeBack()
{
	back_img->setVisible(true);
	coin_img->setVisible(false);
}

void ExchangeCoin::changeFront()
{
	coin_img->setVisible(true);
	back_img->setVisible(false);
}

void ExchangeCoin::moving()
{
	if(myGD->mapState[myPoint.x][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x-1][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x+1][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x][myPoint.y-1] != mapEmpty &&
	   myGD->mapState[myPoint.x][myPoint.y+1] != mapEmpty)
	{
		stopMoving();
		
		back_img = CCSprite::create(CCString::createWithFormat("exchange_%d_unact.png", myType)->getCString());
		back_img->setVisible(false);
		back_img->setScale(1.f/myGD->game_scale);
		back_img->setPosition(CCPointZero);
		addChild(back_img);
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.05f, 0.2f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(ExchangeCoin::changeBack));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.05f, 0.2f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(ExchangeCoin::changeFront));
		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		CCRepeat* t_repeat = CCRepeat::create(t_seq, 10);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(ExchangeCoin::endTakeAction));
		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_repeat, t_call3);
		
		runAction(t_seq2);
		
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

void ExchangeCoin::myInit(int t_type, CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin)
{
	myType = t_type;
	target_ui = t_ui;
	delegate_takeExchangeCoin = d_takeExchangeCoin;
	
	
	coin_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", myType)->getCString());
	coin_img->setScale(0);
	coin_img->setPosition(CCPointZero);
	addChild(coin_img);
}

FeverCoin* FeverCoin::create(IntPoint t_point, CCObject* t_add, SEL_CallFuncO d_add)
{
	FeverCoin* t_fc = new FeverCoin();
	t_fc->myInit(t_point, t_add, d_add);
	t_fc->autorelease();
	return t_fc;
}

void FeverCoin::startRemove()
{
	remove_frame = 0;
	schedule(schedule_selector(FeverCoin::removing));
}

void FeverCoin::removing()
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

void FeverCoin::startCheck()
{
	schedule(schedule_selector(FeverCoin::checking));
}

void FeverCoin::checking()
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

void FeverCoin::stopCheck()
{
	unschedule(schedule_selector(FeverCoin::checking));
}

void FeverCoin::myInit(IntPoint t_point, CCObject* t_add, SEL_CallFuncO d_add) // 0 ~ 5
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
	setScale(1.f/myGD->game_scale);
	
	startCheck();
}

FeverCoinParent* FeverCoinParent::create()
{
	FeverCoinParent* t_fcp = new FeverCoinParent();
	t_fcp->myInit();
	t_fcp->autorelease();
	return t_fcp;
}

void FeverCoinParent::startFever()
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

void FeverCoinParent::stopFever()
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

void FeverCoinParent::addGetCoinList(CCObject* t_coin)
{
	remove_target_list.push_back(t_coin);
	
	if(!is_removing)
	{
		startRemove();
	}
}

void FeverCoinParent::startRemove()
{
	is_removing = true;
	schedule(schedule_selector(FeverCoinParent::removing), 3.f/60.f);
}

void FeverCoinParent::removing()
{
	AudioEngine::sharedInstance()->playEffect("sound_fever_coin.m4a", false);
	FeverCoin* remove_target = (FeverCoin*)remove_target_list.front();
	remove_target->startRemove();
	remove_target_list.pop_front();
	
	mySGD->setGold(mySGD->getGold() + int(10.f*weight_value), false);
	
	if(remove_target_list.empty())
	{
		stopRemove();
	}
}

void FeverCoinParent::stopRemove()
{
	mySGD->setGold(mySGD->getGold());
	unschedule(schedule_selector(FeverCoinParent::removing));
	is_removing = false;
}

void FeverCoinParent::myInit()
{
	initWithFile("fever_coin.png", kDefaultSpriteBatchCapacity);
	is_removing = false;
//	weight_value = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)/(myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1.f)*NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1);
	weight_value = 1.f*NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1);
}

GameItemManager* GameItemManager::create()
{
	GameItemManager* t_gim = new GameItemManager();
	t_gim->myInit();
	t_gim->autorelease();
	return t_gim;
}

void GameItemManager::startItemSetting()
{
	for(int i=0;i<2;i++)
	{
		if(rand()%2 == 0)
		{
			GameItemAttack* t_gia = GameItemAttack::create(true);
			t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
			addChild(t_gia);
		}
		else
		{
			GameItemSpeedUp* t_gisu = GameItemSpeedUp::create(true);
			t_gisu->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
			addChild(t_gisu);
		}
	}
	
	if(mySD->getClearCondition() == kCLEAR_timeLimit)
	{
		GameItemAddTime* t_giat = GameItemAddTime::create(false);
		t_giat->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_giat);
	}
	
	if(mySGD->isUsingItem(kIC_fast))
	{
		GameItemFast* t_fast = GameItemFast::create(false);
		t_fast->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_fast);
	}
	
	if(mySGD->isUsingItem(kIC_critical))
	{
		GameItemFire* t_fire = GameItemFire::create(false);
		t_fire->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_fire);
	}
	
	if(mySGD->isUsingItem(kIC_subOneDie))
	{
		GameItemSubOneDie* t_sod = GameItemSubOneDie::create(false);
		t_sod->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_sod);
	}
	
	if(mySGD->isUsingItem(kIC_silence))
	{
		GameItemSilence* t_silence = GameItemSilence::create(false);
		t_silence->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_silence);
	}
}

void GameItemManager::startCounting()
{
	create_counting_value = rand()%5 +  10-selected_item_cnt-double_item_cnt;
	counting_value = 0;
	schedule(schedule_selector(GameItemManager::counting), 1.f);
}

void GameItemManager::dieCreateItem()
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

void GameItemManager::showCoin(CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin)
{
	for(int i=1;i<=6;i++)
	{
		ExchangeCoin* t_ec = ExchangeCoin::create(i, t_ui, d_takeExchangeCoin);
		coin_parent->addChild(t_ec);
		t_ec->showCoin();
	}
}

void GameItemManager::stopCoin()
{
	int loop_cnt = coin_parent->getChildrenCount();
	CCArray* child = coin_parent->getChildren();
	for(int i=0;i<loop_cnt;i++)
	{
		ExchangeCoin* t_ec = (ExchangeCoin*)child->objectAtIndex(i);
		t_ec->smallScaleHiding();
	}
}

void GameItemManager::counting()
{
	counting_value++;
	
	if(clr_cdt_type == kCLEAR_bossLifeZero && getChildrenCount()-child_base_cnt < 2)
	{
		GameItemAttack* t_gia = GameItemAttack::create(false);
		t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
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

void GameItemManager::addItem()
{
	int random_value = rand()%creatable_list.size();
	ITEM_CODE create_item = creatable_list[random_value];
	
	if(create_item == kIC_attack)
	{
		GameItemAttack* t_gia = GameItemAttack::create(rand()%2 == 0);
		t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_gia);
	}
	else if(create_item == kIC_speedUp)
	{
		GameItemSpeedUp* t_gisu = GameItemSpeedUp::create(rand()%2 == 0);
		t_gisu->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_gisu);
	}
	else if(create_item == kIC_fast)
	{
		GameItemFast* t_fast = GameItemFast::create(false);
		t_fast->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_fast);
	}
	else if(create_item == kIC_critical)
	{
		GameItemFire* t_fire = GameItemFire::create(false);
		t_fire->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_fire);
	}
	else if(create_item == kIC_subOneDie)
	{
		GameItemSubOneDie* t_sod = GameItemSubOneDie::create(false);
		t_sod->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_sod);
	}
	else if(create_item == kIC_silence)
	{
		GameItemSilence* t_silence = GameItemSilence::create(false);
		t_silence->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_silence);
	}
}

void GameItemManager::showTakeItemEffect(CCPoint t_p)
{
//	CCSprite* t_effect = CCSprite::createWithTexture(take_item_effects->getTexture(), CCRectMake(0, 0, 109, 109));
//	t_effect->setPosition(t_p);
//	take_item_effects->addChild(t_effect);
//	
//	CCAnimation* t_animation = CCAnimation::create();
//	t_animation->setDelayPerUnit(0.07f);
//	for(int i=0;i<2;i++)
//		for(int j=0;j<5;j++)
//			t_animation->addSpriteFrameWithTexture(take_item_effects->getTexture(), CCRectMake(j*109, i*109, 109, 109));
//	CCAnimate* t_animate = CCAnimate::create(t_animation);
//	CCFadeTo* t_fade = CCFadeTo::create(0.1f, 0);
//	CCCallFunc* t_call = CCCallFunc::create(t_effect, callfunc_selector(CCSprite::removeFromParent));
//	CCSequence* t_seq = CCSequence::create(t_animate, t_fade, t_call, NULL);
//	t_effect->runAction(t_seq);
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	CCSprite* take_effect = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("fx_item2.ccbi",this));
	take_effect->setPosition(t_p);
	take_effect->setScale(1.f/myGD->game_scale);
	addChild(take_effect);
	reader->getAnimationManager()->setDelegate(this);
	
	effect_que.push_back(take_effect);
}

void GameItemManager::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	
	if(t_name == "Default Timeline")
	{
		CCSprite* remove_target = effect_que.front();
		effect_que.pop_front();
		removeChild(remove_target);
	}
}

void GameItemManager::myInit()
{
	clr_cdt_type = mySD->getClearCondition();
	
	coin_parent = CCNode::create();
	addChild(coin_parent);
	
	fever_coin_parent = FeverCoinParent::create();
	addChild(fever_coin_parent);
	
//	take_item_effects = CCSpriteBatchNode::create("fx_take_item.png");
//	addChild(take_item_effects);
	
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