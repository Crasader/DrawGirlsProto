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
#include "GameData.h"
//#include "DataStorageHub.h"
//#include "MissileDamageData.h"

#define ABSORB_DISTANCE 100.f
#define MAGNET_DISTANCE 2000.f

GameItemBase::GameItemBase()
{
	m_absorb_distance = ABSORB_DISTANCE + mySGD->character_magnetic.getV();
	if(mySGD->isUsingItem(kIC_magnet))
		m_absorb_distance += MAGNET_DISTANCE;
}

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
	
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	if(myPoint.convertToCCP().getDistanceSq(jack_position) < m_absorb_distance)
	{
		unschedule(schedule_selector(GameItemBase::framing));
		
		startTraceCharacter();
		return;
	}
	
	if(getSideCount() > starting_side_cnt)
	{
		unschedule(schedule_selector(GameItemBase::framing));
		
		startTraceCharacter();
		return;
	}
	
	
	if(holding_time <= frame_cnt)
	{
		unschedule(schedule_selector(GameItemBase::framing));
		startHide();
	}
}

void GameItemBase::startTraceCharacter()
{
	schedule(schedule_selector(GameItemBase::traceCharacter));
}
void GameItemBase::traceCharacter()
{
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	CCPoint direction_vector = jack_position - item_img->getPosition();
	if(direction_vector.getLength() <= 0)
	{
		stopTraceCharacter();
		
//		CCLOG("take it item!!!");
		
		if(mySD->getClearCondition() == kCLEAR_itemCollect)
			myGD->communication("UI_takeItemCollect");
		
		myPoint = IntPoint::convertToIntPoint(item_img->getPosition());
		(target_effect->*delegate_effect)(myPoint.convertToCCP());
		acting();
		
		item_img->removeFromParentAndCleanup(true);
		
		return;
	}
	else
	{
		CCPoint move_vector = ccpMult(direction_vector, 2.5f/direction_vector.getLength());
		
		item_img->setPosition(item_img->getPosition() + move_vector);
		
		if(item_img->getPosition().getDistanceSq(jack_position) < 2.5f)
		{
			stopTraceCharacter();
			
//			CCLOG("take it item!!!");
			
			if(mySD->getClearCondition() == kCLEAR_itemCollect)
				myGD->communication("UI_takeItemCollect");
			
			myPoint = IntPoint::convertToIntPoint(item_img->getPosition());
			(target_effect->*delegate_effect)(myPoint.convertToCCP());
			acting();
			
			item_img->removeFromParentAndCleanup(true);
			
			return;
		}
	}
}
void GameItemBase::stopTraceCharacter()
{
	unschedule(schedule_selector(GameItemBase::traceCharacter));
}

void GameItemBase::acting()
{
	AudioEngine::sharedInstance()->playEffect("se_item.mp3", false, true);
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
	
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 0.5f,
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
	recreate->setTakeEffectFunc(target_effect, delegate_effect);
	getParent()->addChild(recreate);
	
	GameItemBase::selfRemove();
}

void GameItemAddTime::acting()
{
	GameItemBase::acting();
	myGD->communication("UI_takeAddTimeItem");
	
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "addTime");
	
	GameItemAddTime* recreate = GameItemAddTime::create(false);
	recreate->setTakeEffectFunc(target_effect, delegate_effect);
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
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_bonustime.ccbi",this));
	item_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_bonustime.ccbi").first;
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
//	reader->release();
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 1.f, [this](float t)
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
	GameItemBase::acting();
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
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_speed.ccbi",this));
	item_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_speed.ccbi").first;
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
//	reader->release();
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 1.f, [=](float t)
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
	GameItemBase::acting();
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "Fast");
	
	myGD->setAlphaSpeed(myGD->getAlphaSpeed() + 1.9f);
	
	CCDelayTime* t_delay = CCDelayTime::create(SDS_GI(kSDF_stageInfo, mySD->getSilType(), "itemOption_fast_sec"));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GameItemFast::ending));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	
	runAction(t_seq);
}

void GameItemFast::ending()
{
	myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 1.9f);
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
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_dash.ccbi",this));
	item_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_dash.ccbi").first;
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
//	reader->release();
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 1.f, [=](float t)
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
	GameItemBase::acting();
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "attack");
	
	AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_attack%d.mp3", rand()%4+1)->getCString(), false, true);
	
	int weapon_type = mySGD->getSelectedCharacterHistory().characterNo.getV()-1;
	int weapon_level = mySGD->getSelectedCharacterHistory().level.getV()+3;
	if(weapon_level>30)weapon_level=30;
	int weapon_rank = (weapon_level-1)/5 + 1;
	weapon_level = (weapon_level-1)%5 + 1;
	
	int t_damage = mySGD->getSelectedCharacterHistory().power.getV()*1.3f*((rand()%21-10+100)/100.f);
	
	myGD->createJackMissileWithStoneFunctor((StoneType)weapon_type, weapon_level, rand()%3 + 3, myPoint.convertToCCP(), t_damage);
	
//	string missile_code;
//	missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//	int missile_type = MissileDamageData::getMissileType(missile_code.c_str());
//	
//	//				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
//	float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//	
//	myGD->communication("MP_createJackMissile", missile_type, rand()%3 + 1, missile_speed, myPoint.convertToCCP());
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
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_attack.ccbi",this));
	item_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_attack.ccbi").first;
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
//	reader->release();
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 1.f, [=](float t)
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
	GameItemBase::acting();
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
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_onekill.ccbi",this));
	item_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_onekill.ccbi").first;
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
//	reader->release();
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
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
	GameItemBase::acting();
	myLog->addLog(kLOG_getItem_s, myGD->getCommunication("UI_getUseTime"), "silence");
	myGD->communication("UI_takeSilenceItem");
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
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_silence.ccbi",this));
	item_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_silence.ccbi").first;
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
//	reader->release();
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
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
	GameItemBase::acting();
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
	int boss_count = myGD->getMainCumberCount();
	for(int i=0;i<boss_count;i++)
	{
		CCNode* mainCumber = myGD->getMainCumberCCNodeVector()[i];
		
		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f), rand()%360-180.f);
		myGD->communication("MP_bombCumber", (CCObject*)mainCumber); // with startMoving
		myGD->communication("CP_startDamageReaction", mainCumber, t_damage, rand()%360-180.f, true, true);
	}

	
	int cumber_cnt = myGD->getSubCumberVector().size();
	for(int i=0; i<cumber_cnt; i++)
	{
		auto t_subCumber = myGD->getSubCumberVector()[i];

		myGD->communication("MP_explosion", t_subCumber->getPosition(), ccc4f(1.f, 0, 0, 1.f), rand()%360-180.f); // ccpoint
		myGD->communication("MP_bombCumber", (CCObject*)t_subCumber); // with startMoving
		myGD->communication("CP_startDamageReaction", t_subCumber, t_damage, rand()%360-180.f, true, true);
		if(myGD->getSubCumberVector().size() < cumber_cnt)
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
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	item_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("ingame_item_bomb.ccbi",this));
	item_img = KS::loadCCBI<CCSprite*>(this, "ingame_item_bomb.ccbi").first;
	item_img->setScale(0.f);
	CCPoint item_point = ccp((myPoint.x-1)*pixelSize + 1, (myPoint.y-1)*pixelSize + 1);
	item_img->setPosition(item_point);
	addChild(item_img);
//	reader->release();
	
	starting_side_cnt = getSideCount();
	
	startFraming();
	item_img->addChild(KSGradualValue<float>::create(0.f, 0.75f, 1.f, [=](float t)
													 {
														 item_img->setScale(t*0.5f);
													 }, [](float t){}));
	
	if(myDSH->getIntegerForKey(kDSH_Key_selectedCard) > 0)
//		damage = NSDS_GI(kSDS_CI_int1_missile_power_i, myDSH->getIntegerForKey(kDSH_Key_selectedCard))*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCard))-1)*0.1f+1.f);
//	else
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
	is_hiding = true;
	CCScaleTo* t_scale = CCScaleTo::create(1.f, 0);
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

bool ExchangeCoin::isLocked()
{
	if(myGD->mapState[myPoint.x][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x-1][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x+1][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x][myPoint.y-1] != mapEmpty &&
	   myGD->mapState[myPoint.x][myPoint.y+1] != mapEmpty)
	{
		return true;
	}
	
	return false;
}

void ExchangeCoin::startTraceCharacter()
{
	schedule(schedule_selector(ExchangeCoin::traceCharacter));
}
void ExchangeCoin::traceCharacter()
{
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	CCPoint direction_vector = jack_position - getPosition();
	
	CCPoint move_vector = ccpMult(direction_vector, 2.5f/direction_vector.getLength());
	
	setPosition(getPosition() + move_vector);
	
	if(getPosition().getDistanceSq(jack_position) < 2.5f)
	{
		stopTraceCharacter();
		
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
}
void ExchangeCoin::stopTraceCharacter()
{
	unschedule(schedule_selector(ExchangeCoin::traceCharacter));
}

void ExchangeCoin::moving()
{
//	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
//	if(myPoint.convertToCCP().getDistanceSq(jack_position) < ABSORB_DISTANCE)
//	{
//		stopMoving();
//		
//		startTraceCharacter();
//		return;
//	}
	
	if(myGD->mapState[myPoint.x][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x-1][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x+1][myPoint.y] != mapEmpty &&
	   myGD->mapState[myPoint.x][myPoint.y-1] != mapEmpty &&
	   myGD->mapState[myPoint.x][myPoint.y+1] != mapEmpty)
	{
		stopMoving();
		
		startTraceCharacter();
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
		
		float changeAngleValue = (rand()%31 - 15)/10.f; // -1.5 ~ 1.5 // 원래는 -2 ~ 2
		changeAngleValue *= (rand()%11)/10.f+1; // 1 ~ 2 원래는 1 ~ 5
		
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
			cutionAction();
			
			if(rand()%2)					directionAngle -= 90;
			else							directionAngle += 90;
			
			if(directionAngle < 0)			directionAngle += 360;
			else if(directionAngle > 360)	directionAngle -= 360;
		}
		else if(afterPoint.isInnerMap() && myGD->mapState[afterPoint.x][afterPoint.y] == mapNewline)
		{
			cutionAction();
			
			if(rand()%2)					directionAngle -= 90;
			else							directionAngle += 90;
			
			if(directionAngle < 0)			directionAngle += 360;
			else if(directionAngle > 360)	directionAngle -= 360;
		}
		else if(afterPoint.isInnerMap() && (myGD->mapState[afterPoint.x][afterPoint.y] == mapOldline || myGD->mapState[afterPoint.x][afterPoint.y] == mapOldget))
		{
			cutionAction();
			
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

void ExchangeCoin::cutionAction()
{
	if(!is_hiding && !is_cution)
	{
		is_cution = true;
		coin_img->stopAllActions();
		coin_img->setScale(1.f/myGD->game_scale);
		
		float first_x;
		float first_y;
		if(rand()%2 == 0)
		{
			first_x = 1.f/myGD->game_scale*0.7f;
			first_y = 1.f/myGD->game_scale*1.1f;
		}
		else
		{
			first_x = 1.f/myGD->game_scale*1.1f;
			first_y = 1.f/myGD->game_scale*0.7f;
		}
		
		CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, first_x, first_y);
		CCScaleTo* t_scale2 = CCScaleTo::create(0.2f, first_y, first_x);
		CCScaleTo* t_scale3 = CCScaleTo::create(0.1f, 1.f/myGD->game_scale);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ExchangeCoin::endCutionAction));
		CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_scale3, t_call, NULL);
		
		coin_img->runAction(t_seq);
	}
}

void ExchangeCoin::endCutionAction()
{
	is_cution = false;
}

void ExchangeCoin::myInit(int t_type, CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin)
{
	myType = t_type;
	target_ui = t_ui;
	delegate_takeExchangeCoin = d_takeExchangeCoin;
	
	is_hiding = false;
	is_cution = false;
	
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
	schedule(schedule_selector(FeverCoin::checking), 1.f/30.f);
}

void FeverCoin::checking()
{
	if(myGD->mapState[my_point.x][my_point.y] != mapOutline && (myGD->mapState[my_point.x-1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y-1] == mapOldline ||
																myGD->mapState[my_point.x-1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y+1] == mapOldline ||
																myGD->mapState[my_point.x+1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y-1] == mapOldline ||
																myGD->mapState[my_point.x+1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y+1] == mapOldline))
	{
		stopCheck();
		is_stan_by = true;
		if(target_add && delegate_add)
		{
			(target_add->*delegate_add)(this);
		}
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
	int start_cut = ((my_point.y-4)/9 + (my_point.x-4)/9)%6;
	
	CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_bronze.png");
	initWithTexture(t_texture, CCRectMake(start_cut*30, 0, 30, 30));
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1f);
	int add_count = 0;
	for(int i=start_cut;add_count < 6;i=(i+1)%6)
	{
		add_count++;
		t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*30, 0, 30, 30));
	}
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
	runAction(t_repeat);
	
	setPosition(my_point.convertToCCP());
	setScale(1.f/myGD->game_scale*0.8f);
	
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
	is_fevering = true;
	myGD->communication("Main_setLineParticle", true);
	for(int i=4;i<mapHeightInnerEnd;i+=9)
	{
		for(int j=4;j<mapWidthInnerEnd;j+=9)
		{
			if((myGD->mapState[j-1][i-1] == mapEmpty || myGD->mapState[j-1][i-1] == mapOutline) &&
			   (myGD->mapState[j-1][i+1] == mapEmpty || myGD->mapState[j-1][i+1] == mapOutline) &&
			   (myGD->mapState[j+1][i-1] == mapEmpty || myGD->mapState[j+1][i-1] == mapOutline) &&
			   (myGD->mapState[j+1][i+1] == mapEmpty || myGD->mapState[j+1][i+1] == mapOutline))
			{
				FeverCoin* t_fc = FeverCoin::create(IntPoint(j,i), this, callfuncO_selector(FeverCoinParent::addGetCoinList));
				addChild(t_fc);
			}
		}
	}
}

void FeverCoinParent::stopFever()
{
	is_fevering = false;
	myGD->communication("Main_setLineParticle", false);
	int loop_cnt = getChildrenCount();
	CCArray* my_childs = getChildren();
    if(my_childs)
    {
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
	
	mySGD->addChangeGoodsIngameGold(int(10.f*weight_value));
//	mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + int(10.f*weight_value), false);
	
	if(remove_target_list.empty())
	{
		stopRemove();
	}
}

void FeverCoinParent::stopRemove()
{
//	mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold));
	unschedule(schedule_selector(FeverCoinParent::removing));
	is_removing = false;
}

void FeverCoinParent::myInit()
{
	is_fevering = false;
	initWithFile("fever_coin_bronze.png", kDefaultSpriteBatchCapacity);
	is_removing = false;
//	weight_value = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)/(myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1.f)*NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1);
	weight_value = 1.f*NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, mySGD->getSelectedCharacterHistory().characterIndex.getV());
}

FloatingCoin::FloatingCoin()
{
	m_absorb_distance = ABSORB_DISTANCE + mySGD->character_magnetic.getV();
	if(mySGD->isUsingItem(kIC_magnet))
		m_absorb_distance += MAGNET_DISTANCE;
}

FloatingCoin* FloatingCoin::create(function<void(CCPoint)> t_take_func, int t_gold, CCPoint t_start_point, bool t_auto_take)
{
	FloatingCoin* t_fc = new FloatingCoin();
	t_fc->myInit(t_take_func, t_gold, t_start_point, t_auto_take);
	t_fc->autorelease();
	return t_fc;
}
void FloatingCoin::startAbsorbChecking()
{
	absorb_frame = 0;
	schedule(schedule_selector(FloatingCoin::absorbChecking), 1.f/30.f);
}
void FloatingCoin::absorbChecking()
{
	// 주변에 캐릭터 있는지 체크
	
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	if(getPosition().getDistanceSq(jack_position) < m_absorb_distance)
	{
		takeIt();
		return;
	}
	
	if(is_locked)
	{
		if(myGD->mapState[locked_point.x][locked_point.y] != mapOutline && (myGD->mapState[locked_point.x-1][locked_point.y-1] == mapOldget || myGD->mapState[locked_point.x-1][locked_point.y-1] == mapOldline ||
																	myGD->mapState[locked_point.x-1][locked_point.y+1] == mapOldget || myGD->mapState[locked_point.x-1][locked_point.y+1] == mapOldline ||
																	myGD->mapState[locked_point.x+1][locked_point.y-1] == mapOldget || myGD->mapState[locked_point.x+1][locked_point.y-1] == mapOldline ||
																	myGD->mapState[locked_point.x+1][locked_point.y+1] == mapOldget || myGD->mapState[locked_point.x+1][locked_point.y+1] == mapOldline))
		{
			takeIt();
			return;
		}
	}
	
	
	absorb_frame++;
	
	if(absorb_frame > 300)
		hideAction();
}
void FloatingCoin::hideAction()
{
	unschedule(schedule_selector(FloatingCoin::absorbChecking));
	unschedule(schedule_selector(FloatingCoin::floating));
	unschedule(schedule_selector(FloatingCoin::ting));
	unschedule(schedule_selector(FloatingCoin::asLonging));
	
	/*coin_img->*/runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.3f, 0), CCCallFunc::create(this, callfunc_selector(FloatingCoin::removeFromParent))));
}

void FloatingCoin::startTraceCharacter()
{
	schedule(schedule_selector(FloatingCoin::traceCharacter));
}
void FloatingCoin::traceCharacter()
{
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	CCPoint direction_vector = jack_position - getPosition();
	
	CCPoint move_vector = ccpMult(direction_vector, 2.5f/direction_vector.getLength());
	
	setPosition(getPosition() + move_vector);
	
	if(getPosition().getDistanceSq(jack_position) < 2.5f)
	{
		stopTraceCharacter();
		AudioEngine::sharedInstance()->playEffect("sound_fever_coin.m4a", false, true);
		mySGD->addChangeGoodsIngameGold(m_gold);
		
		take_func(getPosition());
		
		removeFromParent();
		return;
	}
}
void FloatingCoin::stopTraceCharacter()
{
	unschedule(schedule_selector(FloatingCoin::traceCharacter));
}

void FloatingCoin::takeIt()
{
	unschedule(schedule_selector(FloatingCoin::absorbChecking));
	unschedule(schedule_selector(FloatingCoin::floating));
	unschedule(schedule_selector(FloatingCoin::ting));
	unschedule(schedule_selector(FloatingCoin::asLonging));
	
	if(auto_take)
	{
		AudioEngine::sharedInstance()->playEffect("sound_fever_coin.m4a", false, true);
		mySGD->addChangeGoodsIngameGold(m_gold);
		
		take_func(getPosition());
		
		removeFromParent();
		return;
	}
	else
	{
		startTraceCharacter();
	}
}
void FloatingCoin::onLock()
{
	is_locked = true;
	locked_point = IntPoint::convertToIntPoint(getPosition());
}
void FloatingCoin::startFloating()
{
	ing_frame = 0;
	schedule(schedule_selector(FloatingCoin::floating));
}
void FloatingCoin::floating()
{
	ing_frame++;
	if(ing_frame == 20 && !auto_take)
		startAbsorbChecking();
	
	if(auto_take && ing_frame == auto_take_frame)
	{
		takeIt();
		return;
	}
	
	bool is_found = false;
	CCPoint after_position;
	float step = 5.f;
	int step_count = 0;
	int search_direction = rand()%2;
	while(!is_found)
	{
		if(step <= 0.f)
			removeFromParent();
		
		is_found = true;
		after_position = getPosition() + ccp(cosf(moving_direction/180.f*M_PI)*(moving_speed+5-step), sinf(moving_direction/180.f*M_PI)*(moving_speed+5-step));
		
		if(after_position.x > 318.f || after_position.x < 2.f || after_position.y > 428.f || after_position.y < 2.f)
		{
			is_found = false;
			if(search_direction == 0)
			{
				moving_direction += step;
				if(moving_direction >= 180.f)
					moving_direction -= 360.f;
			}
			else
			{
				moving_direction -= step;
				if(moving_direction < -180.f)
					moving_direction += 360.f;
			}
			
			step_count++;
			if(step_count*step >= 360.f)
			{
				step -= 1.f;
				step_count = 0;
			}
		}
	}
	
	setPosition(after_position);
	getParent()->reorderChild(this, 431-int(getPositionY()));
	
	if(!auto_take)
	{
		IntPoint check_point = IntPoint::convertToIntPoint(after_position);
		if(ing_frame > 20 && myGD->mapState[check_point.x][check_point.y] == mapEmpty &&
		   myGD->mapState[check_point.x-1][check_point.y-1] == mapEmpty &&
		   myGD->mapState[check_point.x-1][check_point.y+1] == mapEmpty &&
		   myGD->mapState[check_point.x+1][check_point.y-1] == mapEmpty &&
		   myGD->mapState[check_point.x+1][check_point.y+1] == mapEmpty)
		{
			unschedule(schedule_selector(FloatingCoin::floating));
			if(isAsLong())
			{
				startAsLonging();
			}
			else
			{
				onLock();
			}
		}
	}
}

bool FloatingCoin::isAsLong()
{
	CCPoint after_position = getPosition() + ccp(cosf(moving_direction/180.f*M_PI)*moving_speed, sinf(moving_direction/180.f*M_PI)*moving_speed);
	
	if(after_position.x > 319.f || after_position.x < 1.f || after_position.y > 429.f || after_position.y < 1.f)
	{
		return false;
	}
	IntPoint check_point = IntPoint::convertToIntPoint(after_position);
	if(ing_frame > 20 && myGD->mapState[check_point.x][check_point.y] == mapEmpty &&
	   myGD->mapState[check_point.x-1][check_point.y-1] == mapEmpty &&
	   myGD->mapState[check_point.x-1][check_point.y+1] == mapEmpty &&
	   myGD->mapState[check_point.x+1][check_point.y-1] == mapEmpty &&
	   myGD->mapState[check_point.x+1][check_point.y+1] == mapEmpty)
	{
		return true;
	}
	return false;
}

void FloatingCoin::startAsLonging()
{
	keeping_count = 2 + rand()%3;
	schedule(schedule_selector(FloatingCoin::asLonging));
}
void FloatingCoin::asLonging()
{
	CCPoint after_position = getPosition() + ccp(cosf(moving_direction/180.f*M_PI)*moving_speed, sinf(moving_direction/180.f*M_PI)*moving_speed);
	
	if(after_position.x > 319.f || after_position.x < 1.f || after_position.y > 429.f || after_position.y < 1.f)
	{
		unschedule(schedule_selector(FloatingCoin::asLonging));
		onLock();
		return;
	}
	
	setPosition(after_position);
	
	getParent()->reorderChild(this, 431-int(getPositionY()));
	
	
	keeping_count--;
	
	if(keeping_count <= 0 || !isAsLong())
	{
		unschedule(schedule_selector(FloatingCoin::asLonging));
		onLock();
	}
}

void FloatingCoin::startTing()
{
	ting_y = 0.f;
	start_speed = 2.f+(rand()%20)/10.f;
	schedule(schedule_selector(FloatingCoin::ting));
}
void FloatingCoin::ting()
{
	float after_y = /*coin_img->getPositionY()*/ting_y + start_speed;
	float before_y = getPositionY();
//	coin_img->setPositionY(MAX(coin_img->getPositionY() + start_speed, 0.f));
	setPositionY(before_y + MAX(ting_y + start_speed, 0.f));
	
	if(after_y < 0)
	{
		start_speed = -start_speed*0.7f;
		if(fabsf(start_speed) < 0.3f)
		{
//			coin_img->setPositionY(0);
			setPositionY(before_y);
			unschedule(schedule_selector(FloatingCoin::ting));
			return;
		}
	}
	
	start_speed -= 0.3f;
}
void FloatingCoin::myInit(function<void(CCPoint)> t_take_func, int t_gold, CCPoint t_start_point, bool t_auto_take)
{
	take_func = t_take_func;
	is_locked = false;
	auto_take = t_auto_take;
	
	auto_take_frame = rand()%10+20;
	
	moving_direction = rand()%360 - 180;
	moving_speed = rand()%10 / 10.f + 1.f;
	
	Json::Value goldBalance = mySGD->getGoldBalance();
	
	int random_value = rand()%100;
    if(mySGD->getStageGold() >= goldBalance.get("maxGold",500).asInt())
        random_value = 99;
  
	
	if(random_value < goldBalance["goldPercent"][0].asInt())
	{
		m_gold = t_gold*goldBalance["goldWeight"][0].asInt();
		
		int start_cut = rand()%6;
		
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_total.png");
		initWithTexture(t_texture, CCRectMake(start_cut*30, 0, 30, 30));
		setScale((50-10)/100.f);
		
//		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_gold.png");
//		coin_img = CCSprite::createWithTexture(t_texture, CCRectMake(start_cut*30, 0, 30, 30));
//		coin_img->setScale((50-10)/100.f);//(50 - rand()%20)/100.f);
//		coin_img->setPosition(CCPointZero);
//		addChild(coin_img);
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1f);
		int add_count = 0;
		for(int i=start_cut;add_count < 6;i=(i+1)%6)
		{
			add_count++;
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*30, 0, 30, 30));
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		/*coin_img->*/runAction(t_repeat);
		
//		CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(8);
//		t_particle->setPositionType(kCCPositionTypeRelative);
//		t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//		t_particle->setEmissionRate(50);
//		t_particle->setAngle(90.0);
//		t_particle->setAngleVar(10.0);
//		ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
//		t_particle->setBlendFunc(blendFunc);
//		t_particle->setDuration(-1.f);
//		t_particle->setEmitterMode(kCCParticleModeGravity);
//		t_particle->setStartColor(ccc4f(1.f, 0.835f, 0.1875f, 0.5f));
//		t_particle->setStartColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
//		t_particle->setEndColor(ccc4f(0.f, 0.f, 0.f, 0.f));
//		t_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
//		t_particle->setStartSize(4.0);
//		t_particle->setStartSizeVar(2.0);
//		t_particle->setEndSize(0.0);
//		t_particle->setEndSizeVar(0.0);
//		t_particle->setGravity(ccp(0,0));
//		t_particle->setRadialAccel(0.0);
//		t_particle->setRadialAccelVar(0.0);
//		t_particle->setSpeed(0);
//		t_particle->setSpeedVar(0.0);
//		t_particle->setTangentialAccel(0);
//		t_particle->setTangentialAccelVar(0);
//		t_particle->setTotalParticles(8);
//		t_particle->setLife(1.0);
//		t_particle->setLifeVar(1.0);
//		t_particle->setStartSpin(0);
//		t_particle->setStartSpinVar(40);
//		t_particle->setEndSpin(0);
//		t_particle->setEndSpinVar(40);
//		t_particle->setPosVar(ccp(7,7));
//		t_particle->setPosition(ccp(0, 0));
//		addChild(t_particle);
	}
	else if(random_value < goldBalance["goldPercent"][1].asInt()+goldBalance["goldPercent"][0].asInt())
	{
		m_gold = t_gold*goldBalance["goldWeight"][1].asInt();
		
		int start_cut = rand()%6;
		
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_total.png");
		initWithTexture(t_texture, CCRectMake(start_cut*30, 30, 30, 30));
		setScale((50-10)/100.f);
		
//		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_silver.png");
//		coin_img = CCSprite::createWithTexture(t_texture, CCRectMake(start_cut*30, 0, 30, 30));
//		coin_img->setScale((50-10)/100.f);//(50 - rand()%20)/100.f);
//		coin_img->setPosition(CCPointZero);
//		addChild(coin_img);
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1f);
		int add_count = 0;
		for(int i=start_cut;add_count < 6;i=(i+1)%6)
		{
			add_count++;
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*30, 30, 30, 30));
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		/*coin_img->*/runAction(t_repeat);
	}
	else
	{
		m_gold = t_gold*goldBalance["goldWeight"][2].asInt();
		
		int start_cut = rand()%6;
		
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_total.png");
		initWithTexture(t_texture, CCRectMake(start_cut*30, 60, 30, 30));
		setScale((50-10)/100.f);
		
//		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("fever_coin_bronze.png");
//		coin_img = CCSprite::createWithTexture(t_texture, CCRectMake(start_cut*30, 0, 30, 30));
//		coin_img->setScale((50-10)/100.f);//(50 - rand()%20)/100.f);
//		coin_img->setPosition(CCPointZero);
//		addChild(coin_img);
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1f);
		int add_count = 0;
		for(int i=start_cut;add_count < 6;i=(i+1)%6)
		{
			add_count++;
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*30, 60, 30, 30));
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		/*coin_img->*/runAction(t_repeat);
	}
	
	startTing();
	
	setPosition(t_start_point);
	
	startFloating();
}

FloatingCoinCreator* FloatingCoinCreator::create(CCSpriteBatchNode* t_add_parent, function<void(CCPoint)> t_take_func, int t_frame, int t_count, int t_gold, CCPoint t_start_point, bool t_auto_take)
{
	FloatingCoinCreator* t_fcc = new FloatingCoinCreator();
	t_fcc->myInit(t_add_parent, t_take_func, t_frame, t_count, t_gold, t_start_point, t_auto_take);
	t_fcc->autorelease();
	return t_fcc;
}
void FloatingCoinCreator::startCreate()
{
	ing_frame = 0;
	ing_count = 0;
	schedule(schedule_selector(FloatingCoinCreator::creating));
}
void FloatingCoinCreator::creating()
{
	ing_frame++;
	
	if(ing_frame >= m_frame)
	{
		ing_frame = 0;
		ing_count++;
		
		if(auto_take)
		{
			CCPoint boss_position = myGD->getMainCumberVector()[0]->getPosition();
			add_parent->addChild(FloatingCoin::create(take_func, m_gold, boss_position, auto_take));
		}
		else
		{
			add_parent->addChild(FloatingCoin::create(take_func, m_gold, start_point, auto_take));
		}
		
		if(ing_count >= m_count)
		{
			unschedule(schedule_selector(FloatingCoinCreator::creating));
			removeFromParent();
		}
	}
}
void FloatingCoinCreator::myInit(CCSpriteBatchNode* t_add_parent, function<void(CCPoint)> t_take_func, int t_frame, int t_count, int t_gold, CCPoint t_start_point, bool t_auto_take)
{
	take_func = t_take_func;
	add_parent = t_add_parent;
	m_frame = t_frame;
	m_count = t_count;
	m_gold = t_gold;
	start_point = t_start_point;
	auto_take = t_auto_take;
	
	startCreate();
}

FloatingCoinParent* FloatingCoinParent::create(function<void(CCPoint)> t_take_func)
{
	FloatingCoinParent* t_fcp = new FloatingCoinParent();
	t_fcp->myInit(t_take_func);
	t_fcp->autorelease();
	return t_fcp;
}
void FloatingCoinParent::showPercentFloatingCoin(float t_percent)
{
	float t_d = 2.f/100.f;
	
	int t_coin_count;
	if(mySGD->is_endless_mode || mySGD->is_hell_mode)
		t_coin_count = roundf(t_percent/t_d);
	else
		t_coin_count = roundf(t_percent/t_d*(mySD->getSilType()+10)/(mySGD->getUserdataHighPiece()+10));
	if(t_coin_count > 0)
		creator_node->addChild(FloatingCoinCreator::create(coin_node, take_func, 5, t_coin_count, int(NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, mySGD->getSelectedCharacterHistory().characterIndex.getV())), myGD->getJackPoint().convertToCCP()));
}
void FloatingCoinParent::showAttackFloatingCoin(CCPoint t_target_point, int t_coin_count)
{
	if(t_coin_count > 0)
		creator_node->addChild(FloatingCoinCreator::create(coin_node, take_func, 5, t_coin_count, int(NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, mySGD->getSelectedCharacterHistory().characterIndex.getV())), t_target_point));
}
void FloatingCoinParent::hideAllFloatingCoin()
{
	creator_node->removeAllChildren();
	
	CCArray* coin_array = coin_node->getChildren();
    if(coin_array)
    {
        int loop_count = coin_array->count();
        for(int i=loop_count-1;i>=0;i--)
        {
            ((FloatingCoin*)coin_array->objectAtIndex(i))->hideAction();
        }
    }
}

void FloatingCoinParent::startClearFloatCoin(float t_percent)
{
	float t_d = 2.f/100.f;
	
	int t_coin_count;
	float clear_reward = 10.f;
	
	if(mySGD->is_endless_mode || mySGD->is_hell_mode)
		t_coin_count = roundf(t_percent/t_d);
	else
	{
		t_coin_count = roundf(t_percent/(t_d*(mySD->getSilType()+10)/(mySGD->getUserdataHighPiece()+10)));
		clear_reward *= (mySD->getSilType()+10)/(mySGD->getUserdataHighPiece()+10);
	}
	
	t_coin_count += roundf(clear_reward/(NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, mySGD->getSelectedCharacterHistory().characterIndex.getV())));
	if(t_coin_count <= 0)
		t_coin_count = 1;
	
	creator_node->addChild(FloatingCoinCreator::create(coin_node, take_func, 3, t_coin_count, int(NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_gold_d, mySGD->getSelectedCharacterHistory().characterIndex.getV())), ccp(0,0), true));
}

void FloatingCoinParent::myInit(function<void(CCPoint)> t_take_func)
{
	take_func = t_take_func;
	
	creator_node = CCNode::create();
	creator_node->setPosition(CCPointZero);
	addChild(creator_node);
	
	coin_node = CCSpriteBatchNode::create("fever_coin_total.png");
	coin_node->setPosition(CCPointZero);
	addChild(coin_node);
}



GameItemManager* GameItemManager::create()
{
	GameItemManager* t_gim = new GameItemManager();
	t_gim->myInit();
	t_gim->autorelease();
	return t_gim;
}

bool GameItemManager::getIsFevering()
{
	return fever_coin_parent->is_fevering;
}

void GameItemManager::startItemSetting()
{
	for(int i=0;i<2;i++)
	{
		if(myGD->jack_base_speed + myGD->Fcommunication("Jack_getSpeedUpValue") >= 2.f)
		{
			GameItemAttack* t_gia = GameItemAttack::create(true);
			t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
			addChild(t_gia);
		}
		else
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
	}
	
	if(mySD->getClearCondition() == kCLEAR_timeLimit || is_on_addTime)
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

void GameItemManager::stopCounting()
{
	unschedule(schedule_selector(GameItemManager::counting));
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

bool GameItemManager::isChangeAllInner()
{
    bool t_all_locked = true;
    
	CCArray* t_array = coin_parent->getChildren();
    if(t_array)
    {
        int t_count = t_array->count();
        
        for(int i=0;i<t_count;i++)
        {
            ExchangeCoin* t_ec = (ExchangeCoin*)t_array->objectAtIndex(i);
            t_all_locked = t_all_locked && t_ec->isLocked();
        }
    }
	
	return t_all_locked;
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
    if(child)
    {
        for(int i=0;i<loop_cnt;i++)
        {
            ExchangeCoin* t_ec = (ExchangeCoin*)child->objectAtIndex(i);
            t_ec->smallScaleHiding();
        }
    }
	
	floating_coin_parent->hideAllFloatingCoin();
}

void GameItemManager::counting()
{
	counting_value++;
	
	if(clr_cdt_type == kCLEAR_bossLifeZero && getChildrenCount()-child_base_cnt < 2*(double_item_cnt > 0 ? 2 : 1))
	{
		if(myGD->jack_base_speed + myGD->Fcommunication("Jack_getSpeedUpValue") >= 2.f)
		{
			GameItemAttack* t_gia = GameItemAttack::create(false);
			t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
			addChild(t_gia);
		}
		else
		{
			if(rand()%2 == 0)
			{
				GameItemAttack* t_gia = GameItemAttack::create(false);
				t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
				addChild(t_gia);
			}
			else
			{
				GameItemSpeedUp* t_gisu = GameItemSpeedUp::create(false);
				t_gisu->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
				addChild(t_gisu);
			}
		}
		
		create_counting_value = rand()%5 + 10-selected_item_cnt-double_item_cnt;
		counting_value = 0;
	}
	else if(clr_cdt_type == kCLEAR_itemCollect && getChildrenCount()-child_base_cnt < 1*(double_item_cnt > 0 ? 2 : 1))
	{
		addItem();
		
		create_counting_value = rand()%5 + 10-selected_item_cnt-double_item_cnt;
		counting_value = 0;
	}
	
	if(counting_value >= create_counting_value)
	{
		if(getChildrenCount()-child_base_cnt < 6*(double_item_cnt > 0 ? 2 : 1))
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
		if(myGD->jack_base_speed + myGD->Fcommunication("Jack_getSpeedUpValue") >= 2.f)
		{
			GameItemAttack* t_gia = GameItemAttack::create(rand()%2 == 0);
			t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
			addChild(t_gia);
		}
		else
		{
			GameItemSpeedUp* t_gisu = GameItemSpeedUp::create(rand()%2 == 0);
			t_gisu->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
			addChild(t_gisu);
		}
	}
	else if(create_item == kIC_fast)
	{
		GameItemFast* t_fast = GameItemFast::create(false);
		t_fast->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
		addChild(t_fast);
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

void GameItemManager::showBeautyStone()
{
//	for(int i=0;i<3;i++)
//	{
//		int random_value = rand()%creatable_list.size();
//		ITEM_CODE create_item = creatable_list[random_value];
//		
//		if(create_item == kIC_attack)
//		{
//			GameItemAttack* t_gia = GameItemAttack::create(true);
//			t_gia->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
//			addChild(t_gia);
//			
//			beauty_stone_list.push_back(t_gia);
//		}
//		else// if(create_item == kIC_speedUp)
//		{
//			GameItemSpeedUp* t_gisu = GameItemSpeedUp::create(true);
//			t_gisu->setTakeEffectFunc(this, callfuncCCp_selector(GameItemManager::showTakeItemEffect));
//			addChild(t_gisu);
//			
//			beauty_stone_list.push_back(t_gisu);
//		}
//	}
}

void GameItemManager::removeBeautyStone()
{
//	while(!beauty_stone_list.empty())
//	{
//		CCNode* t_node = beauty_stone_list.back();
//		beauty_stone_list.pop_back();
//		
//		bool is_found = false;
//		CCArray* child_array = getChildren();
//		for(int i=0;i<child_array->count() && !is_found;i++)
//		{
//			CCNode* t_child = (CCNode*)child_array->objectAtIndex(i);
//			if(t_node == t_child)
//				is_found = true;
//		}
//		
//		if(is_found)
//		{
//			CCDelayTime* t_delay = CCDelayTime::create(0.1f);
//			CCCallFunc* t_call = CCCallFunc::create(t_node, callfunc_selector(CCNode::removeFromParent));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//			t_node->runAction(t_seq);
//		}
//	}
}

TakeEffectManager* TakeEffectManager::create()
{
	TakeEffectManager* t_tem = new TakeEffectManager();
	t_tem->init();
	t_tem->autorelease();
	return t_tem;
}

void TakeEffectManager::removeMyAction()
{
	t_4_1->removeFromParent();
	t_4_2->removeFromParent();
	t_1_1->removeFromParent();
	t_2->removeFromParent();
	t_5->removeFromParent();
	t_1_2->removeFromParent();
	t_3->removeFromParent();
	t_7->removeFromParent();
	removeFromParent();
}

void TakeEffectManager::startAnimation()
{
	float t_scale = 1.f/myGD->game_scale;
	
	t_4_1->setScale(0.f);
	t_4_1->setOpacity(150);
	t_4_2->setScale(0.f);
	t_4_2->setOpacity(0);
	t_1_1->setScale(0.f);
	t_2->setScale(1.2f*t_scale);
	t_5->setScale(1.2f*t_scale);
	t_5->setOpacity(0);
	t_1_2->setScale(0.f);
	t_3->setScale(0.f);
	t_3->setOpacity(200);
	t_7->setScale(t_scale);
	t_7->setOpacity(0);
	
	t_4_1->runAction(CCSequence::create(CCDelayTime::create(13.f/30.f), CCScaleTo::create(9.f/30.f, 1.3f*t_scale), NULL));
	t_4_1->runAction(CCSequence::create(CCDelayTime::create(13.f/30.f), CCFadeTo::create(9.f/30.f, 0), NULL));
	
	t_4_2->runAction(CCSequence::create(CCDelayTime::create(12.f/30.f), CCScaleTo::create(1.f/30.f, 1.2f*t_scale), CCScaleTo::create(7.f/30.f, 1.8f*t_scale), NULL));
	t_4_2->runAction(CCSequence::create(CCDelayTime::create(12.f/30.f), CCFadeTo::create(1.f/30.f, 255), CCFadeTo::create(7.f/30.f, 0), NULL));
	
	t_1_1->runAction(CCSequence::create(CCDelayTime::create(1.f/30.f), CCScaleTo::create(11.f/30.f, 1.f*t_scale), NULL));
	t_1_1->runAction(CCSequence::create(CCDelayTime::create(10.f/30.f), CCFadeTo::create(2.f/30.f, 0), NULL));
	
	t_2->runAction(CCScaleTo::create(8.f/30.f, 0.f));
	t_2->runAction(CCSequence::create(CCDelayTime::create(1.f/30.f), CCFadeTo::create(6.f/30.f, 0), NULL));
	
	t_5->runAction(CCSequence::create(CCDelayTime::create(4.f/30.f), CCScaleTo::create(6.f/30.f, 0.f), NULL));
	t_5->runAction(CCSequence::create(CCDelayTime::create(3.f/30.f), CCFadeTo::create(1.f/30.f, 255), CCFadeTo::create(6.f/30.f, 0), NULL));
	
	t_1_2->runAction(CCSequence::create(CCDelayTime::create(10.f/30.f), CCScaleTo::create(1.f/30.f, 0.909f*t_scale), CCScaleTo::create(5.f/30.f, 2.5f*t_scale), NULL));
	t_1_2->runAction(CCSequence::create(CCDelayTime::create(12.f/30.f), CCFadeTo::create(6.f/30.f, 0), NULL));
	
	t_3->runAction(CCSequence::create(CCDelayTime::create(12.f/30.f), CCScaleTo::create(1.f/30.f, 1.2f*t_scale), CCScaleTo::create(6.f/30.f, 1.8f*t_scale), NULL));
	t_3->runAction(CCSequence::create(CCDelayTime::create(13.f/30.f), CCFadeTo::create(3.f/30.f, 0), NULL));
	
	t_7->runAction(CCSequence::create(CCDelayTime::create(13.f/30.f), CCScaleTo::create(4.f/30.f, 1.5f*t_scale), NULL));
	t_7->runAction(CCSequence::create(CCDelayTime::create(12.f/30.f), CCFadeTo::create(1.f/30.f, 255), CCFadeTo::create(5.f/30.f, 0), NULL));
	
	runAction(CCSequence::create(CCDelayTime::create(22.f/30.f), CCCallFunc::create(this, callfunc_selector(TakeEffectManager::removeMyAction)), NULL));
}

void GameItemManager::showTakeItemEffect(CCPoint t_p)
{
	CCTexture2D* t_texture = take_effect_node->getTexture();
	
	CCSprite* t_4_1 = CCSprite::createWithTexture(t_texture, CCRectMake(0, 91, 91, 91));
	CCSprite* t_4_2 = CCSprite::createWithTexture(t_texture, CCRectMake(0, 91, 91, 91));
	CCSprite* t_1_1 = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 91, 91));
	CCSprite* t_2 = CCSprite::createWithTexture(t_texture, CCRectMake(91, 0, 91, 91));
	CCSprite* t_5 = CCSprite::createWithTexture(t_texture, CCRectMake(91, 91, 91, 91));
	CCSprite* t_1_2 = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 91, 91));
	CCSprite* t_3 = CCSprite::createWithTexture(t_texture, CCRectMake(0, 182, 91, 91));
	CCSprite* t_7 = CCSprite::createWithTexture(t_texture, CCRectMake(91, 182, 91, 91));
	
	t_4_1->setPosition(t_p);
	t_4_2->setPosition(t_p);
	t_1_1->setPosition(t_p);
	t_2->setPosition(t_p);
	t_5->setPosition(t_p);
	t_1_2->setPosition(t_p);
	t_3->setPosition(t_p);
	t_7->setPosition(t_p);
	
	take_effect_node->addChild(t_7);
	take_effect_node->addChild(t_3);
	take_effect_node->addChild(t_1_2);
	take_effect_node->addChild(t_5);
	take_effect_node->addChild(t_2);
	take_effect_node->addChild(t_1_1);
	take_effect_node->addChild(t_4_2);
	take_effect_node->addChild(t_4_1);
	
	
	TakeEffectManager* t_manager = TakeEffectManager::create();
	t_manager->t_4_1 = t_4_1;
	t_manager->t_4_2 = t_4_2;
	t_manager->t_1_1 = t_1_1;
	t_manager->t_2 = t_2;
	t_manager->t_5 = t_5;
	t_manager->t_1_2 = t_1_2;
	t_manager->t_3 = t_3;
	t_manager->t_7 = t_7;
	addChild(t_manager);
	
	t_manager->startAnimation();
	
//	auto t_ccb = KS::loadCCBI<CCSprite*>(this, "fx_item2.ccbi");
//	
//	CCSprite* take_effect = t_ccb.first;
//	take_effect->setPosition(t_p);
//	take_effect->setScale(1.f/myGD->game_scale);
//	addChild(take_effect);
//	t_ccb.second->setDelegate(this);
//	effect_animation_manager.push_back(t_ccb.second);
//	
//	effect_que.push_back(take_effect);
}

void GameItemManager::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	
	if(t_name == "Default Timeline")
	{
		CCSprite* remove_target = effect_que.front();
		effect_que.pop_front();
		CCBAnimationManager* remove_animation = effect_animation_manager.front();
		effect_animation_manager.pop_front();
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
	
	floating_coin_parent = FloatingCoinParent::create([=](CCPoint t_point){showTakeItemEffect(t_point);});
	addChild(floating_coin_parent);
	
//	take_item_effects = CCSpriteBatchNode::create("fx_take_item.png");
//	addChild(take_item_effects);
	
	child_base_cnt = getChildrenCount();
	
	is_on_addTime = false;
	int defItems_cnt = SDS_GI(kSDF_stageInfo, mySD->getSilType(), "defItems_cnt");
	for(int i=0;i<defItems_cnt;i++)
	{
		ITEM_CODE t_item = ITEM_CODE(SDS_GI(kSDF_stageInfo, mySD->getSilType(), CCString::createWithFormat("defItems_%d_type", i)->getCString()));
		if(t_item == kIC_addTime)
		{
			is_on_addTime = true;
		}
		else
			creatable_list.push_back(t_item);
	}
	
	selected_item_cnt = 0;
	
	double_item_cnt = mySGD->getDoubleItemValue();
	
	if(mySGD->isUsingItem(kIC_fast)){		creatable_list.push_back(kIC_fast);			selected_item_cnt++;	}
	if(mySGD->isUsingItem(kIC_subOneDie)){	creatable_list.push_back(kIC_subOneDie);	selected_item_cnt++;	}
	if(mySGD->isUsingItem(kIC_silence)){	creatable_list.push_back(kIC_silence);		selected_item_cnt++;	}
	
	if(selected_item_cnt+double_item_cnt > 10)
		double_item_cnt = 10 - selected_item_cnt;
	
	//		myGD->regGIM(this, callfunc_selector(GameItemManager::dieCreateItem));
	myGD->V_V["GIM_dieCreateItem"] = std::bind(&GameItemManager::dieCreateItem, this);
	myGD->V_V["GIM_startFever"] = std::bind(&FeverCoinParent::startFever, fever_coin_parent);
	myGD->V_V["GIM_stopFever"] = std::bind(&FeverCoinParent::stopFever, fever_coin_parent);
	myGD->V_V["GIM_stopCoin"] = std::bind(&GameItemManager::stopCoin, this);
	
	myGD->V_V["GIM_showBeautyStone"] = std::bind(&GameItemManager::showBeautyStone, this);
	myGD->V_V["GIM_removeBeautyStone"] = std::bind(&GameItemManager::removeBeautyStone, this);
	myGD->B_V["GIM_isChangeAllInner"] = std::bind(&GameItemManager::isChangeAllInner, this);
	myGD->V_F["GIM_showPercentFloatingCoin"] = std::bind(&FloatingCoinParent::showPercentFloatingCoin, floating_coin_parent, std::placeholders::_1);
	myGD->V_CCPI["GIM_showAttackFloatingCoin"] = std::bind(&FloatingCoinParent::showAttackFloatingCoin, floating_coin_parent, std::placeholders::_1, std::placeholders::_2);
	myGD->V_V["GIM_hideAllFloatingCoin"] = std::bind(&FloatingCoinParent::hideAllFloatingCoin, floating_coin_parent);
	myGD->V_F["GIM_startClearFloatingCoin"] = std::bind(&FloatingCoinParent::startClearFloatCoin, floating_coin_parent, std::placeholders::_1);
	myGD->V_V["GIM_stopCounting"] = std::bind(&GameItemManager::stopCounting, this);
	
	take_effect_node = CCSpriteBatchNode::create("fx_item2_total.png");
	take_effect_node->setPosition(CCPointZero);
	addChild(take_effect_node, 99999);
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
//			CCLOG("ice : %.1f", t_damage);
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
