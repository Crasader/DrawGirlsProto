//
//  MissileParent.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 6..
//
//

#include "MissileParent.h"
#include "LogData.h"
#include "KSCircleBase.h"
#include "StageImgLoader.h"
#include "AttackPattern.h"
#include "StarGoldData.h"
#include "StoneMissile.h"
#include "ks19937.h"
#include <boost/format.hpp>

void MissileParent::bombCumber( CCObject* target )
{
	KSCumberBase* cumber = (KSCumberBase*)target;
	bool cancelSound = false;
	if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
	{
		bool canceled = false;
		for(auto i : cumber->getCharges())
		{
			TRACE();
			i->cancelCharge();
			canceled = true;
			
			cancelSound = true;
		}
		
		if(canceled)
		{
			myGD->communication("Main_hideScreenSideWarning"); // 화면에 빨간 테두리 지우는 함수
		}

		//if(target == myGD->getCommunicationNode("CP_getMainCumberPointer"))
		//{
			//for(int i=0;i<chargeArray->count();i++)
			//{
				//ChargeParent* t_cn = (ChargeNode*)chargeArray->objectAtIndex(i);
				//t_cn->cancelCharge();
			//}
		//}
	}
	
	
	if(cumber->getAttackPattern())
	{
		auto temp = cumber->getAttackPattern();
		if(temp)
		{
			temp->stopMyAction();
		}
		else
		{ 
			CCLOG("hue~~");
		}
		cumber->setAttackPattern(nullptr);
		cancelSound = true;
	}
	
	
	if(cancelSound)
	{
		AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_pattern_cancel%d.mp3", ks19937::getIntValue(1, 4))->getCString(), false, true);
//		AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("ment_pattern_cancel%d.mp3", rand()%4+1)->getCString());
	}
	//if(target == myGD->getCommunicationNode("CP_getMainCumberPointer") && saveAP)
	//{
		//saveAP->stopMyAction();
		//endIngActionAP();
	//}
}

void MissileParent::createJackMissile( int jm_type, int cmCnt, float missile_speed, CCPoint missile_position )
{

	return;
//	CCLOG("createJackMissile inner : %d, %d, %.2f", jm_type, cmCnt, missile_speed);
	
	int card_number;
	card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	if(card_number > 0)
	{
		if(jm_type%10 >= 0 && jm_type%10 <= 3)
		{
//			CCLOG("base JackMissile");
			vector<KSCumberBase*> subCumberArray = myGD->getSubCumberVector();
			int cumberCnt = subCumberArray.size();
			int random_value;
			
			int boss_count = myGD->getMainCumberCount();
			
			for(int i=0;i<cmCnt;i++)
			{
				if(i < boss_count)
				{
					JackMissile* t_jm = JM_BasicMissile::create(myGD->getMainCumberCCNodeVector()[i], jm_type, missile_speed, missile_position);
					jack_missile_node->addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					if(cumberCnt == 0)
					{
						JackMissile* t_jm = JM_BasicMissile::create(myGD->getMainCumberCCNodeVector()[0], jm_type, missile_speed, missile_position);
						jack_missile_node->addChild(t_jm);
						t_jm->startMoving();
					}
					else
					{
						random_value = rand()%cumberCnt;
						
						JackMissile* t_jm = JM_BasicMissile::create((CCNode*)subCumberArray[random_value], jm_type, missile_speed, missile_position);
						jack_missile_node->addChild(t_jm);
						t_jm->startMoving();
					}
				}
			}
			
			CCSprite* card_img = CCSprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png", card_number)->getCString()));
			card_img->setScale(0.f);
			card_img->setPosition(missile_position);
			jack_missile_node->addChild(card_img);
			
			CCScaleTo* t_scale = CCScaleTo::create(0.2f, 0.5f/myGD->game_scale);
			CCDelayTime* t_delay = CCDelayTime::create(0.8f);
			CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
			CCCallFunc* t_call = CCCallFunc::create(card_img, callfunc_selector(CCSprite::removeFromParent));
			CCSequence* t_seq = CCSequence::create(t_scale, t_delay, t_fade, t_call, NULL);
			card_img->runAction(t_seq);
			
			CCSprite* no_img = CCSprite::create("cardchange_noimg.png");
			no_img->setScale(1.f/0.73f);
			no_img->setPosition(ccp(card_img->getContentSize().width/2.f, card_img->getContentSize().height/2.f));
			card_img->addChild(no_img);
			
			CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
			CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 0);
			CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, NULL);
			no_img->runAction(t_seq2);
		}
		else if(jm_type%10 >= 4 && jm_type%10 <= 6)
		{
//			CCLOG("Upgrade JackMissile");
			UM_creator* t_c = UM_creator::create(cmCnt, jm_type, missile_speed, missile_position);
			jack_missile_node->addChild(t_c);
			t_c->startPetCreate();
			
			float t_missile_speed = missile_speed;
			
			if(t_missile_speed < 2.f)
				t_missile_speed = 2.f;
			else if(t_missile_speed > 9.f)
				t_missile_speed = 9.f;
			else
				t_missile_speed = t_missile_speed;
			int shoot_frame = t_missile_speed*3;
			shoot_frame *= cmCnt;
			
			CCSprite* card_img = CCSprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("card%d_thumbnail.png", card_number)->getCString()));
			card_img->setScale(0.f);
			card_img->setPosition(missile_position);
			jack_missile_node->addChild(card_img);
			
			CCScaleTo* t_scale = CCScaleTo::create(0.2f, 0.5f/myGD->game_scale);
			CCDelayTime* t_delay = CCDelayTime::create(0.8f);
			CCFadeTo* t_fade = CCFadeTo::create(shoot_frame/60.f+0.5f, 0);
			CCCallFunc* t_call = CCCallFunc::create(card_img, callfunc_selector(CCSprite::removeFromParent));
			CCSequence* t_seq = CCSequence::create(t_scale, t_delay, t_fade, t_call, NULL);
			card_img->runAction(t_seq);
			
			CCSprite* no_img = CCSprite::create("cardchange_noimg.png");
			no_img->setScale(1.f/0.73f);
			no_img->setPosition(ccp(card_img->getContentSize().width/2.f, card_img->getContentSize().height/2.f));
			card_img->addChild(no_img);
			
			CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
			CCFadeTo* t_fade2 = CCFadeTo::create(shoot_frame/60.f+0.5f, 0);
			CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, NULL);
			no_img->runAction(t_seq2);
		}
	}
	else
	{
		CCSprite* no_img = CCSprite::create("cardchange_noimg.png");
		no_img->setScale(0.5f/myGD->game_scale);
		no_img->setPosition(missile_position);
		jack_missile_node->addChild(no_img);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 0);
		CCSequence* t_seq2 = CCSequence::create(t_delay, t_fade2, NULL);
		no_img->runAction(t_seq2);
	}
}

void MissileParent::createJackMissileWithStone(StoneType stoneType, int level, float missileNumbers, CCPoint initPosition,
																							 int missile_damage, int missile_sub_damage)
{
//	switch(stoneType)
//	{
//		case StoneType::kStoneType_laser:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//		case StoneType::kStoneType_mine:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//		case StoneType::kStoneType_protector:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//		case StoneType::kStoneType_range:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//		case StoneType::kStoneType_spirit:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//		case StoneType::kStoneType_spread:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//		case StoneType::kStoneType_guided:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//		case StoneType::kStoneType_tornado:
//			missileNumbers = MIN(30.f, missileNumbers);
//			break;
//
//	}
	int grade = ceilf((float)level / 5.f);
	int power = missile_damage;
	AttackOption ao = getAttackOption(stoneType, grade);
	int missileNumbersInt = floor(missileNumbers);
	CCLOG("char no = %d", mySGD->getUserdataSelectedCharNO());
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
	int subType = mInfo.get("subType", 1).asInt();
//	stoneType = StoneType::kStoneType_guided;
	if(stoneType == StoneType::kStoneType_guided)
	{
		addChild(KSSchedule::create([=](float dt)
																{
																	return false; // stop
																}));
		

		
		for(int i=0; i<missileNumbersInt; i++)
		{
			auto creator = [=](){
				string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
				KSCumberBase* nearCumber = getNearestCumber(myGD->getJackPointCCP());
				
				int random_value = rand()%7 - 3;
				float random_float = random_value/10.f;
				bool selfRotation = false;

				switch(subType)
				{
					case 1:
						if(1 <= level && level <= 5 || 16 <= level && level <= 20)
							selfRotation = true;
						else
							selfRotation = false;
						break;
					case 2:
					case 4:
					case 7:
						selfRotation = true;
						break;
					case 3:
					case 5:
						selfRotation = false;
						break;
					default:
						selfRotation = true;
				}
//				if(grade == 1 || grade == 4)
//					selfRotation = true;
				random_float = 0.f;
				float randomAdj = (rand()%21-10+100)/100.f;
				GuidedMissile* gm = GuidedMissile::create(nearCumber, myGD->getJackPoint().convertToCCP(),
																									fileName,
																									1.4f+random_float + grade / 10.f, int(power*randomAdj),
																									int(missile_sub_damage * randomAdj),
																									10 + 15 * grade,
																									ao, selfRotation
																									);
				
				gm->beautifier(level);
				jack_missile_node->addChild(gm);
				
//				auto diff = nearCumber->getPosition() - myGD->getJackPoint().convertToCCP();
//				StraightMissile* sm = StraightMissile::create(myGD->getJackPoint().convertToCCP(), "stone_1.ccbi",
//																											atan2f(diff.y, diff.x),
//																											1.4f+random_float + grade / 10.f, power, ao);
//				jack_missile_node->addChild(sm);
				
//				SpreadMissile* spm = SpreadMissile::create(nearCumber, myGD->getJackPoint().convertToCCP(), "stone_1.ccbi",
//																									 1.4f+random_float + grade / 10.f, power, 8, ao);
//				jack_missile_node->addChild(spm);
				
				// ccbi 가 없음 me_timebomb.ccbi
//				MineAttack* ma = MineAttack::create(myGD->getJackPoint().convertToCCP(), myGD->getJackPoint().convertToCCP(), 8,
//																						power, ao);
//				jack_missile_node->addChild(ma);
				
//				SpiritAttack* sa = SpiritAttack::create(myGD->getJackPoint().convertToCCP(), nearCumber->getPosition(), "stone_1.ccbi",
//																								8, power, 1.4f+random_float + grade / 10.f, 180, ao);
//				jack_missile_node->addChild(sa);
				
				// me_scope.ccbi 없음
//				RangeAttack* ra = RangeAttack::create(myGD->getJackPoint().convertToCCP(), 30.f, 180, power, 3, ao);
//				jack_missile_node->addChild(ra);
				
				// me_allattack_arrow.ccbi 없음
//				RandomBomb* rb = RandomBomb::create(30.f, power, ao);
//				jack_missile_node->addChild(rb);
//
				// me_laser_head.ccbi me_laser_body.ccbi 없음
//				LaserWrapper* lw = LaserWrapper::create(2 + MIN((grade-1), 3), 60*2 + missileNumbers * 60,
//																								power / 3.f, ao);
//				jack_missile_node->addChild(lw);
				
			};
			addChild(KSTimer::create(0.30 * (i + 1), [=](){
				creator();
			}));
		}
	}
	else if(stoneType == StoneType::kStoneType_mine)
	{
		string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
		for(int i=0; i<missileNumbersInt; i++)
		{
			auto creator = [=](){
				IntPoint mapPoint;
				bool found = myGD->getEmptyRandomPoint(&mapPoint, 5);
				if(found == true)
				{
					
					MineAttack* ma = MineAttack::create(myGD->getJackPoint().convertToCCP(), ip2ccp(mapPoint), fileName.c_str(), 15 + 3 * grade, power, missile_sub_damage, ao);
					ma->beautifier(level);
					jack_missile_node->addChild(ma);
				}
			};
			
			addChild(KSTimer::create(0.30 * (i + 1), [=](){
				if(myGD->getIsGameover() == false)
					creator();
			}));
		}
	}
	
	else if(stoneType == StoneType::kStoneType_laser)
	{
		string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
		//LaserAttack* la = LaserAttack::create(0, 400, 33.f, AttackOption::kNoOption);
		float addRad = ks19937::getDoubleValue(0, M_PI * 2.f);
		addChild(KSIntervalCall::create(30, missileNumbers * 3, [=](int seq){
			int dirs = 2 + MIN((grade-1), 3);
			if(myGD->getIsGameover() == false)
			{
				for(int r=0; r<dirs; r++)
				{
					float rad = deg2Rad(360.f / dirs * r);
					
					rad += addRad;
					StraightMissile* sm = StraightMissile::create(initPosition, fileName.c_str(), rad, 2.f, power / 3.f, missile_sub_damage / 3.f, ao);
					jack_missile_node->addChild(sm);
					sm->beautifier(level);
				}
			}
		}));

		
//		LaserWrapper* lw = LaserWrapper::create(2 + MIN((grade-1), 3), 60*2 + missileNumbers * 60,
//																						power / 3.f, missile_sub_damage / 3.f, ao);
//		jack_missile_node->addChild(lw);

	}
	else if(stoneType == StoneType::kStoneType_spread)
	{
		addChild(KSIntervalCall::create(20, missileNumbers, [=](int seq){
			if(myGD->getIsGameover() == true)
				return;
			
			int adderForGrade = 0;
			if(grade == 1){
				adderForGrade = 4;
			}
			else if(grade == 2){
				adderForGrade = 6;
			}
			else if(grade == 3){
				adderForGrade = 8;
			}
			else if(grade == 4){
				adderForGrade = 10;
			}
			else if(grade >= 5){
				adderForGrade = 12;
			}
			else{
				adderForGrade = 0;
			}
			string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
			//			string fileName = boost::str(boost::format("me_redial%||.ccbi") % level);
			KSCumberBase* target = nullptr;
			std::vector<KSCumberBase*> targets;
			targets.insert(targets.end(), myGD->getMainCumberVector().begin(), myGD->getMainCumberVector().end());
			targets.insert(targets.end(), myGD->getSubCumberVector().begin(), myGD->getSubCumberVector().end());
			target = targets[ks19937::getIntValue(0, targets.size() - 1)];
			SpreadMissile* sm = SpreadMissile::create(target, myGD->getJackPoint().convertToCCP(),
																								fileName,
																								1.8f + 0.3f * grade, power, missile_sub_damage,
																								adderForGrade, // 방향.
																								level,
																								ao);
//			sm->beautifier(level);
			jack_missile_node->addChild(sm);
			
		}));
	}
	else if(stoneType == StoneType::kStoneType_range)
	{
		string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);

		float radius = 25 + missileNumbers * 5;
		
		KSCumberBase* nearCumber = getNearestCumber(myGD->getJackPointCCP());
		float ny = nearCumber->getPosition().y;
		float nx = nearCumber->getPosition().x;
		int j = 0;
//		for(float i=missileNumbersInt; i>=0; i-=100.f, j++)
		{
			auto creator = [=](){
//				float mNumber = MIN(i, 100);
				RangeAttack* ra = RangeAttack::create(initPosition, fileName, radius, MIN(120 * 10 / 35.f * radius, 3000),
																							power / 3.f, missile_sub_damage / 3.f, ao);
				jack_missile_node->addChild(ra);
				
			};
			addChild(KSTimer::create(0.80 * (j), [=](){
				if(myGD->getIsGameover() == false)
				{
					creator();
				}
			}));
			
		}
		
		
	}

	else if(stoneType == StoneType::kStoneType_global)
	{
		for(int i=0; i<=4 + 2 * missileNumbersInt; i++)
		{
			RandomBomb* rb = RandomBomb::create(40 + grade * 20, power, missile_sub_damage, ao);
			addChild(rb);
		}
	}
	else if(stoneType == StoneType::kStoneType_spirit)
	{
		for(int i=0; i<missileNumbersInt; i++)
		{
			auto creator = [=](){
				IntPoint mapPoint2;
				bool found2 = myGD->getEmptyRandomPoint(&mapPoint2, 5);
				if(found2 == true)
				{
					string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
					//				string fileName = boost::str(boost::format("me_pet%||.ccbi") % level);
					SpiritAttack* sa = SpiritAttack::create(myGD->getJackPoint().convertToCCP(), ip2ccp(mapPoint2), fileName,
																									3 + grade * 1, power / 2.f, missile_sub_damage / 2.f, 1.2f, 30, ao);
					sa->beautifier(level);
					jack_missile_node->addChild(sa);
				}
			};
			addChild(KSTimer::create(0.30 * (i + 1), [=](){
				if(myGD->getIsGameover() == false)
				{
					creator();
				}
			}));
		}
	}
	
	else if(stoneType == StoneType::kStoneType_protector)
	{
		// 프로텍터 미사일 형식
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		int subType = mInfo.get("subType", 1).asInt();
		
		for(int i=0; i<missileNumbersInt; i++)
		{
			auto creator = [=](){
				string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
				KSCumberBase* target = nullptr;
				std::vector<KSCumberBase*> targets;
				targets.insert(targets.end(), myGD->getMainCumberVector().begin(), myGD->getMainCumberVector().end());
				targets.insert(targets.end(), myGD->getSubCumberVector().begin(), myGD->getSubCumberVector().end());
				target = targets[ks19937::getIntValue(0, targets.size() - 1)];
				
				CCPoint minDis = ccp(99999, 99999);
				KSCumberBase* nearCumber = myGD->getMainCumberVector()[0];
				for(int i = 0; i<myGD->getMainCumberCount();i++){
					KSCumberBase* cumber = myGD->getMainCumberVector()[i];
					CCPoint nowDis = cumber->getPosition()-myGD->getJackPoint().convertToCCP();
					if(ccpLength(nowDis)<ccpLength(minDis))
					{
						nearCumber=cumber;
						minDis = nowDis;
					}
				}
				
				for(int i = 0; i<myGD->getSubCumberCount();i++){
					KSCumberBase* cumber = myGD->getSubCumberVector()[i];
					CCPoint nowDis = cumber->getPosition() - myGD->getJackPoint().convertToCCP();
					if(cumber->getDeadState() == false)
					{
						if(ccpLength(nowDis)<ccpLength(minDis))
						{
							nearCumber=cumber;
							minDis = nowDis;
						}
					}
				}
				
				int random_value = rand()%7 - 3;
				float random_float = random_value/10.f;
				bool selfRotation = false;
				
//				switch(subType)
//				{
//					case 2:
//					case 4:
//					case 7:
//						selfRotation = true;
//						break;
//					case 3:
//					case 5:
//						selfRotation = false;
//						break;
//					default:
//						selfRotation = true;
//				}
				//				if(grade == 1 || grade == 4)
				//					selfRotation = true;
				random_float = 0.f;
			
				float randomAdj = (rand()%21-10+100)/100.f;
				ProtectorMissile* gm = ProtectorMissile::create(nearCumber, myGD->getJackPoint().convertToCCP(),
																												fileName,
																												1.4f+random_float + grade / 10.f,
																												int(power*(randomAdj)),
																												missile_sub_damage * randomAdj,
																												10 + 15 * grade,
																												30.f, 300,
																												ao, selfRotation
																												);
				
				gm->beautifier(level);
				jack_missile_node->addChild(gm);
			};
			addChild(KSTimer::create(0.30 * (i + 1), [=](){
				if(myGD->getIsGameover() == false)
					creator();
			}));
		}
	}
	else if(stoneType == StoneType::kStoneType_slow)
	{
		// 프로텍터 미사일 형식
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		int subType = mInfo.get("subType", 1).asInt();
		
		for(int i=0; i<missileNumbersInt; i++)
		{
			auto creator = [=](){
				string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
				
				
				float randomAdj = (rand()%21-10+100)/100.f;
				SlowAttack* gm = SlowAttack::create(nullptr, myGD->getJackPoint().convertToCCP(),
																												fileName,
																												1.4f + grade / 10.f,
																												M_PI / 180.f * 90.f ,
																												int(power*(randomAdj)),
																												missile_sub_damage * randomAdj,
																												ao
																												);
				
				gm->beautifier(level);
				jack_missile_node->addChild(gm);
			};
			addChild(KSTimer::create(0.30 * (i + 1), [=](){
				if(myGD->getIsGameover() == false)
					creator();
			}));
		}
	}
	else if(stoneType == StoneType::kStoneType_tornado)
	{
		// 프로텍터 미사일 형식
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		int subType = mInfo.get("subType", 1).asInt();
		
		missileNumbersInt /= 2;
		
		int shoot_frame_cnt, shoot_final_cnt;
		if(level <= 2){
			shoot_frame_cnt = 20;}
		else if(level <= 5){
			shoot_frame_cnt = 16;}
		else if(level <= 9){
			shoot_frame_cnt = 14;}
		else if(level <= 14){
			shoot_frame_cnt = 12;}
		else if(level <= 20){
			shoot_frame_cnt = 11;}
		else if(level <= 27){
			shoot_final_cnt = 10;}
		else{
			shoot_frame_cnt = 9;}
		
		if(level <= 4)
			shoot_final_cnt = 3;
		else if(level <= 9)
			shoot_final_cnt = 4;
		else if(level <= 14)
			shoot_final_cnt = 5;
		else if(level <= 20)
			shoot_final_cnt = 6;
		else if(level <= 27)
			shoot_final_cnt = 7;
		else
			shoot_final_cnt = 8;
		
		for(int i=0; i<missileNumbersInt; i++)
		{
			auto creator = [=](){
				string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
				
				std::vector<KSCumberBase*> main_vector = myGD->getMainCumberVector();
				
				KSCumberBase* nearCumber = getNearestCumber(myGD->getJackPointCCP());
				
				Tornado* ms = Tornado::create(myGD->getJackPointCCP(), fileName,
											  0.f, // 반지름
											  (nearCumber->getPosition() - myGD->getJackPointCCP()).getAngle() + (rand()%141-70)/10.f/180.f*M_PI,//ks19937::getFloatValue(0, 2*M_PI), // 시작 방향
											  1.2f, // 본체 속도
											  2, // 방향개수.
											  M_PI / 180.f * 132.f, // 각속도
											  shoot_frame_cnt, // 인타발.
											  power,
											  missile_sub_damage,
											  ao, 100.f + rand()%41-20, shoot_final_cnt, level);
				ms->beautifier(level);
				jack_missile_node->addChild(ms);
			};
			addChild(KSTimer::create(0.50 * (i + 1), [=](){
				if(myGD->getIsGameover() == false)
					creator();
			}));
		}
	}
	else if(stoneType == StoneType::kStoneType_circleDance)
	{
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		int subType = mInfo.get("subType", 1).asInt();
		
//		level = 1;
		string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
		
		
		
		
		missileNumbersInt *= 3;
		addChild(KSTimer::create(0.0f, [=](){
			KSCumberBase* nearCumber = getNearestCumber(myGD->getJackPointCCP());
			float ny = nearCumber->getPosition().y;
			float nx = nearCumber->getPosition().x;
			int j = 0;
			for(int i=missileNumbersInt; i>=0; i-=20, j++)
			{
				auto creator = [=](){
					int mNumber = MIN(i, 20);
					CircleDance* ms = CircleDance::create(myGD->getJackPointCCP(), fileName, MAX(mNumber, 10.f) * 1.2f,
																								atan2f(ny - myGD->getJackPointCCP().y,
																											 nx - myGD->getJackPointCCP().x),// 방향
																								1.2f, // 속도
																								mNumber, // 개수
																								M_PI / 180.f * 10.f,
																								level,
																								power, missile_sub_damage, ao);
					//		ms->beautifier(level);
					jack_missile_node->addChild(ms);
				};
				addChild(KSTimer::create(0.80 * (j), [=](){
					if(myGD->getIsGameover() == false)
					{
						creator();
					}
				}));
				
			}
			
		}));
		
	}
	else if(stoneType == StoneType::kStoneType_boomerang)
	{
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		int subType = mInfo.get("subType", 1).asInt();
		
		//		level = 1;
		string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
		
		
		
		
		//		missileNumbersInt *= 3;
		KSCumberBase* nearCumber = getNearestCumber(myGD->getJackPointCCP());
		float ny = nearCumber->getPosition().y;
		float nx = nearCumber->getPosition().x;
		
		auto creator = [=](int nu){
			Boomerang::Params params;
			params.initPosition = myGD->getJackPointCCP();
			params.goalPosition = ccp(nx, ny);
			params.centerSpeed = 1.5f;
			params.subPower = missile_sub_damage;
			params.power = power;
			params.numbers = nu; // 10개.
			params.revelutionA = 20.f;
			params.fileName = fileName;
			params.ao = ao;
			
			Boomerang* ms = Boomerang::create(params);
			jack_missile_node->addChild(ms);
		};
		
	
		
		missileNumbersInt *= 1.5f;
		addChild(KSTimer::create(0.0f, [=](){
			KSCumberBase* nearCumber = getNearestCumber(myGD->getJackPointCCP());
			float ny = nearCumber->getPosition().y;
			float nx = nearCumber->getPosition().x;
			int j = 0;
			for(int i=missileNumbersInt; i>=0; i-=10, j++)
			{
				int mNumber = MIN(i, 10);
				addChild(KSTimer::create(0.80 * (j), [=](){
					if(myGD->getIsGameover() == false)
					{
						creator(mNumber);
					}
				}));
				
			}
			
		}));
		
		
		
	}
	else if(stoneType == StoneType::kStoneType_chain)
	{
		CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
		Json::Value mInfo = NSDS_GS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, t_history.characterIndex.getV(), t_history.characterLevel.getV());
		int subType = mInfo.get("subType", 1).asInt();
		
		//		level = 1;
		string fileName = ccsf("jack_missile_%02d_%02d.png", subType, level);
		
		KSCumberBase* nearCumber = getNearestCumber(myGD->getJackPointCCP());
		float ny = nearCumber->getPosition().y;
		float nx = nearCumber->getPosition().x;
		
		auto creator = [=](){
			Chain::Params params;
			
			
			
			params.initPosition = myGD->getJackPointCCP();
			params.fileName = fileName;
			params.depth = 3;
			params.chainDistance = 100;
			
			params.subPower = missile_sub_damage;
			params.speed = 3.f;
			params.power = power;
			params.ao = ao;
			params.level = level;
			
			Chain* ms = Chain::create(params);
			jack_missile_node->addChild(ms);
		};
		int j = 0;
		for(int i=missileNumbersInt; i>=0; i-=1, j++)
		{
			addChild(KSTimer::create(0.40 * (j), [=](){
				if(myGD->getIsGameover() == false)
				{
					creator();
				}
			}));
			
		}
		
		
	}


}
AttackOption MissileParent::getAttackOption(StoneType st, int grade)
{ 
	if(st == StoneType::kStoneType_guided)
	{
		return AttackOption::kStiffen;
	}
	else if(st == StoneType::kStoneType_spread)
	{
		return AttackOption::kStiffen;
	}
	else if(st == StoneType::kStoneType_laser)
	{
		return AttackOption::kStiffen;
	}
	else if(st == StoneType::kStoneType_mine)
	{
		return AttackOption::kStiffen;
	}
	else if(st == StoneType::kStoneType_spirit)
	{
		return AttackOption::kStiffen;
	}
	else if(st == StoneType::kStoneType_global)
	{
		return AttackOption::kStiffen;
	}

	else if(st == StoneType::kStoneType_protector)
	{
		return AttackOption::kStiffen;
	}
	else if(st == StoneType::kStoneType_tornado)
	{
		return AttackOption::kStiffen;
	}
	else if(st == StoneType::kStoneType_circleDance)
	{
		return AttackOption::kStiffen;
	}
	else
	{
		return AttackOption::kStiffen;
	}


	
// 등급에 따라 옵션이 붙게 하는 코든데 없앰.
#if 0 
	if(st == StoneType::kStoneType_guided)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kCancelCasting | AttackOption::kGold;
			case 4:
				return AttackOption::kCancelCasting | AttackOption::kGold | AttackOption::kStiffen;
			case 5:
				return AttackOption::kCancelCasting | AttackOption::kGold | AttackOption::kStiffen | AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}
	else if(st == StoneType::kStoneType_spread)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kGold | AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kStiffen;
			case 5:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kStiffen | AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}
	else if(st == StoneType::kStoneType_laser)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kGold | AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kUnbeatable;
			case 5:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kUnbeatable | AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}
	else if(st == StoneType::kStoneType_mine)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kGold | AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kMonsterSpeedDown;
			case 5:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kMonsterSpeedDown | AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}
	else if(st == StoneType::kStoneType_spirit)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kGold | AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kJackSpeedUp;
			case 5:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kJackSpeedUp | AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}


	else if(st == StoneType::kStoneType_range)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kGold | AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kJackSpeedUp;
			case 5:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kJackSpeedUp | AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}

	else if(st == StoneType::kStoneType_global)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kGold | AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kPoisonedNiddle;
			case 5:
				return AttackOption::kGold | AttackOption::kCancelCasting | AttackOption::kPoisonedNiddle | AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}
#endif
	return AttackOption::kNoOption;
}
int MissileParent::getJackMissileCnt()
{
	return jack_missile_node->getChildrenCount();
}

void MissileParent::removeAllPattern()
{
	vector<CCNode*> remover;
	for(int i=0; i<pattern_container->getChildrenCount(); i++)
	{
		CCNode* tt = (CCNode*)pattern_container->getChildren()->objectAtIndex(i);
		if(dynamic_cast<CobWeb*>(tt) || dynamic_cast<ChaosAttack*>(tt) ||
			 dynamic_cast<FreezeAttack*>(tt))
		{
			remover.push_back(tt);
		}
		else
		{
			remover.push_back(tt);
		}
	}
	for(auto i : remover)
	{
//		pattern_container->getChildren()->removeObject(i);
	}
	pattern_container->removeAllChildren();
}

KSCumberBase* MissileParent::getNearestCumber(cocos2d::CCPoint pt)
{
	KSCumberBase* target = nullptr;
	std::vector<KSCumberBase*> targets;
	targets.insert(targets.end(), myGD->getMainCumberVector().begin(), myGD->getMainCumberVector().end());
	targets.insert(targets.end(), myGD->getSubCumberVector().begin(), myGD->getSubCumberVector().end());
	target = targets[ks19937::getIntValue(0, targets.size() - 1)];
	
	CCPoint minDis = ccp(99999, 99999);
	KSCumberBase* nearCumber = myGD->getMainCumberVector()[0];
	for(int i = 0; i<myGD->getMainCumberCount();i++)
	{
		KSCumberBase* cumber = myGD->getMainCumberVector()[i];
		IntPoint ip = ccp2ip(cumber->getPosition());
		if(myGD->mapState[ip.x][ip.y] != mapEmpty)
			continue;
		CCPoint nowDis = cumber->getPosition()-pt;
		if(ccpLength(nowDis)<ccpLength(minDis))
		{
			nearCumber=cumber;
			minDis = nowDis;
		}
	}
	
	for(int i = 0; i<myGD->getSubCumberCount();i++){
		KSCumberBase* cumber = myGD->getSubCumberVector()[i];
		IntPoint ip = ccp2ip(cumber->getPosition());
		if(myGD->mapState[ip.x][ip.y] != mapEmpty)
			continue;
		CCPoint nowDis = cumber->getPosition()-pt;
		if(cumber->getDeadState() == false)
		{
			if(ccpLength(nowDis)<ccpLength(minDis))
			{
				nearCumber=cumber;
				minDis = nowDis;
			}
		}
	}
	return nearCumber;
}
KSCumberBase* MissileParent::getNearestCumberWithExclude(cocos2d::CCPoint pt, const std::vector<KSCumberBase*>& exclude)
{
	KSCumberBase* target = nullptr;
	std::vector<KSCumberBase*> targets;
	targets.insert(targets.end(), myGD->getMainCumberVector().begin(), myGD->getMainCumberVector().end());
	targets.insert(targets.end(), myGD->getSubCumberVector().begin(), myGD->getSubCumberVector().end());
	
	
	// exclude 에 있는것들 다 뺌
	for(auto iter = exclude.begin(); iter != exclude.end(); ++iter)
	{
		auto removeIter = find(targets.begin(), targets.end(), *iter);
		if(removeIter != targets.end())
		{
			targets.erase(removeIter);
		}
	}
	if(targets.empty() == true)
	{
		return nullptr;
	}
	target = targets[ks19937::getIntValue(0, targets.size() - 1)];
	
	CCPoint minDis = ccp(99999, 99999);
	KSCumberBase* nearCumber = target;
	for(auto cumber : targets)
	{
		IntPoint ip = ccp2ip(cumber->getPosition());
		if(myGD->mapState[ip.x][ip.y] != mapEmpty)
			continue;
		CCPoint nowDis = cumber->getPosition()-pt;
		if(cumber->getDeadState() == false)
		{
			if(ccpLength(nowDis)<ccpLength(minDis))
			{
				nearCumber=cumber;
				minDis = nowDis;
			}
		}
	}
	return nearCumber;
}

void MissileParent::attachGodOfDeath(KSCumberBase* cb, Json::Value patternD)
{
	GodOfDeath* t = GodOfDeath::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD.asString());
	getPatternContainer()->addChild(t);
}


void MissileParent::subOneDie()
{
	vector<KSCumberBase*> subCumberArray = myGD->getSubCumberVector();
	std::mt19937 rEngine(std::random_device{}());
	std::uniform_int_distribution<> r(0, subCumberArray.size() - 1);
	if(subCumberArray.size() > 0)
	{
		JackMissile* t_jm = JM_BasicMissile::create((CCNode*)subCumberArray[r(rEngine)], 80+kMyElementalLife, 999999.f);
		addChild(t_jm);
		t_jm->startMoving();
	}
	else
	{
		
		JackMissile* t_jm = JM_BasicMissile::create((CCNode*)myGD->getMainCumberVector()[0], 80+kMyElementalLife, mySGD->getUserdataMissileInfoPower()*5.f);
		addChild(t_jm);
		t_jm->startMoving();
	}
}

//void MissileParent::endIngActionAP()
//{
//	CCLOG("saveAP = null");
////	saveAP = NULL;
//}


int MissileParent::attackWithKSCode(CCPoint startPosition, std::string &patternDParam, KSCumberBase* cb, bool exe)
{
	int valid = 1;
	int invalid = 0;
	// 캐스팅한지 1초 내면 캐스팅 노노
	if(cb->m_lastCastTime + 3.f > cb->m_cumberTimer)
	{
		return invalid;
	}
	Json::Value patternData;
	bool forceSubCreateCondition = clear_condition == kCLEAR_subCumberCatch && myGD->getSubCumberCount() <= 0;
	// Attack Queue 가 있으면 patternD 무시하고 Attack Queue 에서 하나하나 빼서 씀.
	if(cb->getAttackQueue().empty() == false && cb->getAttackQueue().size()>0)
	{
//        CCLOG("A");
		patternData = cb->getAttackQueue().front();
		cb->getAttackQueue().pop_front();
	}  
	else
	{
		// 사냥꾼 미션이고 부하몹이 0 이하라면 부하몹 생성.
		if(forceSubCreateCondition)
		{
//			CCLOG("B");
			Json::Reader reader;
			Json::Value createCumberPattern;
			reader.parse(R"({
									 "atype" : "special",
									 "childs" : 1,
									 "name" : "부하몹리젠",
									 "pattern" : 1020,
									 "percent" : 1,
									 "target" : "no"
									 } )", patternData);
		}
		else
		{
			patternData = patternDParam;
		}
	}
    
	if(patternData.isNull() || patternData.asString()=="" || patternData.isBool()){
//		CCLOG("it's null in attackWithKSCode");
		patternData = patternDParam;
	}
	
//	KS::KSLog("%", patternData);
	if(patternData["pattern"].asInt() == 1020)
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior"), root);
		
		// 기본값으로 서버에서 설정된 부하몹 개수로 함.
		
		// 여기서 계산함
		
		float totalMapSize = (mapWidthInnerEnd - mapWidthInnerBegin) * (mapHeightInnerEnd - mapHeightInnerBegin);
		float obtainSize = 0;
		
		
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
			{
				if(myGD->mapState[i][j] == mapType::mapOldget ||
					 myGD->mapState[i][j] == mapType::mapOldline)
				{
					obtainSize++;
				}
			}
		}
		
		int remainPercent = (totalMapSize - obtainSize) / totalMapSize * 100.f;
//		CCLOG("___ 남은 영역 : %d", remainPercent);
		
		int adjustmentMax = (patternData.get("maxchilds", root.size()).asInt() - 1) / (100.f - 5.f) * (remainPercent - 5.f);
//		CCLOG("___ 계산된 맥스 : %d", adjustmentMax);
		int n = MIN(adjustmentMax - myGD->getSubCumberCount(), patternData.get("childs", 1).asInt());
		// 부하몹 테스트가 우선.
		if(n <= 0 && !forceSubCreateCondition) // 부하몹 생성할것거나 자연스러운 생성이었다면 false 리턴함.
		{
			return invalid;
		}
		else
		{
		}
	}
	if(patternData["pattern"].asInt() == 115) // 부하몹 터트리기.
	{
		if(myGD->getSubCumberCount() == 0)
		{
			return invalid;
		}
	}
//	Json::Value patternDisableArray = R"( [{"type":"pattern","target":"112", "oper":"*","crasharea":0.8},
//	{"type":"pattern","target":"1014","prop":"불발확률","value":0.3}])";
	
//	Json::Value patternDisableValue = Json::objectValue;
//	
//	for(int i=0; i<patternDisableArray.size(); i++)
//	{
//		if(patternDisableArray[i].get("target", "").asString() == pattern)
//		{
//			patternDisableValue = patternDisableArray[i];
//			break;
//		}
//	}

	std::string patternD = patternData.asString();
//	CCLOG(".. %s", patternDParam.c_str());
//	CCLOG("%s", boost::str(boost::format("%||") % patternData).c_str());
	int castFrame = patternData.get("castframe", 120).asInt();
	// 캐스팅 실패하면 캐스팅 시간 점점 줄음.
	castFrame = MAX(30, castFrame - (castFrame * 0.1f)* cb->getCastingCancelCount());
	
	
	std::string pattern = patternData["pattern"].asString();
	std::string atype = patternData["atype"].asString();
	auto castBranch = [=](const std::string atype, std::function<void(CCObject*)> func, const std::string& warningFileName)
	{
		myGD->communication("UI_setIsCasting", true);
		if(atype == "crash")
		{
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
										  func, cb, patternData);
			t_ccn->setChargeColor(ccc4f(0.00, 0.00, 0.00, 1.00));
			cb->getParent()->addChild(t_ccn);
			t_ccn->startCharge();
//			CCLOG("%x", t_ccn);
			
			cb->getCharges().push_back(t_ccn);
//			cb->m_cumberState |= kCumberStateAttack; // 지금 공격중이라는 정보 넣음.
//			m_charges.push_back(t_ccn);
//			cb->setChargeParent(t_ccn);
		}
		else if(atype == "special")
		{
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
											func, cb, patternData);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			cb->getParent()->addChild(t_ccn);
			t_ccn->startCharge();
			
			cb->getCharges().push_back(t_ccn);
//			cb->m_cumberState |= kCumberStateAttack; // 지금 공격중이라는 정보 넣음.
//			cb->setChargeParent(t_ccn);
		}
		else // normal
		{
			ChargeNodeLambda* t_ccn =
			ChargeNodeLambda::create(startPosition, castFrame,
									 func, cb, patternData);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			cb->getParent()->addChild(t_ccn);
			t_ccn->startCharge();
			
			cb->getCharges().push_back(t_ccn);
//			cb->m_cumberState |= kCumberStateAttack; // 지금 공격중이라는 정보 넣음.

//			cb->setChargeParent(t_ccn);
		}
		
//		cb->setDamageMeasure(0.f);
		myGD->communication("Main_showScreenSideWarning"); // 화면에 빨간 테두리 만드는 함수
		myGD->showDetailMessage(warningFileName, "w");
		myGD->communication("Main_showThumbWarning", startPosition);
	};
	string warningFileName = "warning_" + pattern + ".ccbi";
	
	if(cb->m_cumberTimer - 1.f >= cb->m_lastCastTime)
	{
		
		
		if(pattern == "1")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				
				//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
				auto func = [=](CCObject* cb)
				{
					UnusedMissile1* t = UnusedMissile1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		
		else if(pattern == "2")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile6* t = UnusedMissile6::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "3")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile7* t = UnusedMissile7::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "4")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile8* t = UnusedMissile8::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "5")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile9* t = UnusedMissile9::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "6")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile10* t = UnusedMissile10::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
				
			}
		}
		else if(pattern == "7")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile11* t = UnusedMissile11::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
				
			}
		}
		else if(pattern == "8")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					Mugunghwa* t = Mugunghwa::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "9")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					CaromWrapper* t = CaromWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "101")
		{
			if(exe)
			{
				
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile1* t = UnusedMissile1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
				
			}
		}
		else if(pattern == "102")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile2* t = UnusedMissile2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "103")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile3* t = UnusedMissile3::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "104")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					UnusedMissile4* t = UnusedMissile4::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "105") // 나무 토막
		{
			if(exe)
			{
				startFirePosition = startPosition;
				
				//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
				auto func = [=](CCObject* cb)
				{
					SawWrapper* t = SawWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
				
			}
		}
		else if(pattern == "106") // 사과 던지기.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					SmallSawWrapper* t = SmallSawWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "107") // 태양광선.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					CrashLazerWrapper* t = CrashLazerWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "108") // 다용도
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					CommonBulletPattern* t = CommonBulletPattern::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "109") // 개돌
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					KSCumberBase* t = dynamic_cast<KSCumberBase*>(cb);
					
					int totalFrame = patternData.get("totalframe", 300).asInt();
					t->furyModeOn(totalFrame);
					t->setRushCrashSize(patternData.get("crashratio", 1.f).asFloat());
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "110") // 폭죽
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					FireWorkWrapper* t = FireWorkWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "111") // 움직이는 해바라기
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					MovingSunflowerWrapper* t = MovingSunflowerWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					
				};
				castBranch(atype, func, warningFileName);
				
				
			}
		}
		else if(pattern == "112") // 폭탄 여러개 던지기
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					ThrowBombWrapper* t = ThrowBombWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "113") // 리버 스크럽
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					ScarabWrapper* t = ScarabWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "114") // 닳는 톱니.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					RunDownSawWrapper* t = RunDownSawWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "115") // 부하몹 폭발
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
//					KSCumberBase* cumber = (KSCumberBase*)cb;
//					RunDownSawWrapper* t = RunDownSawWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
//					pattern_container->addChild(t);
					
					JunirBombWrapper* t_m32 = JunirBombWrapper::create(dynamic_cast<KSCumberBase*>(cb), patternData);
					pattern_container->addChild(t_m32);
//					cumber->setAttackPattern(nullptr);
//					RunDownSawWrapper* t = RunDownSawWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
//					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1001")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					int totalFrame = patternData.get("totalframe", 400).asInt();
					TornadoWrapper* t_m21 = TornadoWrapper::create(startFirePosition, totalFrame, 1.5f,
																												 dynamic_cast<KSCumberBase*>(cb));
					pattern_container->addChild(t_m21);
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		
		else if(pattern == "1002")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					KSCumberBase* cumber = (KSCumberBase*)cb;
					if(cumber->getSightOutAttack())
					{
						cumber->getSightOutAttack()->updateSightOut();
					}
					else
					{
						int totalFrame = patternData.get("totalframe", 300).asInt();
						SightOutAttack* t_m24 = SightOutAttack::create(totalFrame, cumber);
						pattern_container->addChild(t_m24);
						cumber->setSightOutAttack(t_m24);
					}
					//		SightOutWrapper* t = SightOutWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
					//		pattern_container->addChild(t);
					//
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1003")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					KSCumberBase* cumber = (KSCumberBase*)cb;
					if(cumber->getCobWebAttack())
					{
						cumber->getCobWebAttack()->updateCobWeb();
					}
					else
					{
						//					int totalFrame = patternData.get("totalframe", 60*4).asInt();
						
						//					Cobweb* t_m23 = Cobweb::create(totalFrame);
						CobWeb* t_m23 = CobWeb::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
						pattern_container->addChild(t_m23);
						cumber->setCobWebAttack(t_m23);
					}
					
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1004")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					//				int random_value = rand()%2 + 1;
					int radius = 80;//pattern.get("radius", 100).asInt();
					int objcnt = patternData.get("totalframe", 240).asInt();
					PrisonPattern* t_m28 = PrisonPattern::create(startFirePosition, radius, objcnt, dynamic_cast<KSCumberBase*>(cb));
					pattern_container->addChild(t_m28);
					t_m28->startMyAction();
					KSCumberBase* cumber = (KSCumberBase*)cb;
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1005") // 빙결.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					KSCumberBase* cumber = (KSCumberBase*)cb;
					if(cumber->getFreezeAttack())
					{
						cumber->getFreezeAttack()->updateFreeze();
					}
					else
					{
						int totalFrame = patternData.get("totalframe", 200).asInt();
						FreezeAttack* t_m26 = FreezeAttack::create(totalFrame, cumber);
						pattern_container->addChild(t_m26);
						cumber->setFreezeAttack(t_m26);
					}
					
				};
				castBranch(atype, func, warningFileName);
				
			}
			
		}
		else if(pattern == "1006") // 혼란
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					KSCumberBase* cumber = (KSCumberBase*)cb;
					if(cumber->getChaosAttack())
					{
						cumber->getChaosAttack()->updateChaos();
					}
					else
					{
						int totalFrame = patternData.get("totalframe", 300).asInt();
						ChaosAttack* t_m33 = ChaosAttack::create(totalFrame, cumber);
						pattern_container->addChild(t_m33);
						cumber->setChaosAttack(t_m33);
					}
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1007") // 텔포.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					KSCumberBase* cumber = (KSCumberBase*)cb;
					TeleportWrapper* t_m32 = TeleportWrapper::create(cumber);
					pattern_container->addChild(t_m32);
					cumber->setAttackPattern(nullptr);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1008") // 인비지블
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					int totalFrame = patternData.get("totalframe", 300).asInt();
					((KSCumberBase*)cb)->startInvisible(totalFrame);
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(nullptr);
					myGD->communication("CP_onPatternEndOf", cb);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1009") // 불지르기
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					int totalFrame = patternData.get("totalframe", 60*3).asInt();
					IntPoint mainCumberPoint = myGD->getMainCumberPoint((CCNode*)cb);
					CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
					FlameWrapper* t_m15 = FlameWrapper::create(mainCumberPosition, (KSCumberBase*)cb, 10, totalFrame);
					pattern_container->addChild(t_m15);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1010") // 위성빔.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					// 2.f 하는 이유는 AP12 의 myAction 이 1/60 이 아니고 1/30.f 이라서.
					int targetingFrame = patternData.get("targetingframe", 120).asInt() / 2.f;
					int shootFrame = patternData.get("shootframe", 180).asInt() / 2.f;
					int random_value = rand()%2 + 1;
					
					IntPoint mainCumberPoint = myGD->getMainCumberPoint((CCNode*)cb);
					CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
					ThunderBoltWrapper* t_m12 = ThunderBoltWrapper::create(mainCumberPosition, (KSCumberBase*)cb, random_value,
																																 targetingFrame, shootFrame,
																																 patternData.get("speedratio", 1.f).asFloat());
					pattern_container->addChild(t_m12);
					
					//				myGD->communication("CP_onPatternEndOf", cb);
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1011") // 레이저 스캔.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					LazerScanWrapper* t_m6 = LazerScanWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t_m6);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1012") // 방사능 라인
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					RadioactivityWrapper* t = RadioactivityWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1013") // 메테오
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					int mType = 1;
					int number = patternData.get("number", 3).asInt();
					
					MeteorWrapper* t_m16 = MeteorWrapper::create(mType, number, 60, patternData.get("area", 50).asInt(),
																											 patternData.get("enableprob", 1.f).asFloat(),
																											 dynamic_cast<KSCumberBase*>(cb));
					pattern_container->addChild(t_m16);
					
					//				cumber->setAttackPattern(t_m16);
					
					//				cumber->setAttackPattern(t_m16);
					
				};
				castBranch(atype, func, warningFileName);
			}
		}
		else if(pattern == "1014") // 떨어지는 돌
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					int totalframe = patternData.get("totalframe", 300).asInt();
					int shootframe = patternData.get("shootframe", 30).asInt();
					float speed = patternData.get("speed", 250.f).asDouble() / 100.f;
					FallingStoneWrapper* t_m9 = FallingStoneWrapper::create((KSCumberBase*)cb, patternData, CCSizeMake(25, 25), 1);
					pattern_container->addChild(t_m9);
					KSCumberBase* cumber = (KSCumberBase*)cb;
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1015") // 풍차벽.
		{
			if(exe)
			{
				int totalframe = patternData.get("totalframe", 800).asInt();
				
				startFirePosition = startPosition;
				WindmillObject* t_to = WindmillObject::create(ccp2ip(startPosition), totalframe);
				pattern_container->addChild(t_to);
				
				return invalid; // 노 캐스팅
			}
		}
		else if(pattern == "1016") // 다이너마이트.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				int remainSecond = patternData.get("remainsecond", 9).asInt();
				int crashArea = patternData.get("area", 50).asInt();
				TickingTimeBomb* t_ttb = TickingTimeBomb::create(ccp2ip(startPosition), 120, remainSecond, crashArea, 1, tickingArray, this, callfunc_selector(MissileParent::resetTickingTimeBomb));
				pattern_container->addChild(t_ttb);
				return invalid; // 노 캐스팅
			}
		}
		else if(pattern == "1017") // 선을 따라가는 공격
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					AlongOfTheLineWrapper* t = AlongOfTheLineWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					//cumber->setAttackPattern(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1018") // 폭탄 구름.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					CloudWrapper* t = CloudWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
					//cumber->setAttackPattern(t);
					
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1019") // 복어 공격.
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					int totalFrame = patternData.get("totalframe", 300).asInt();
					float scale = patternData.get("scale", 2.f).asFloat();
					((KSCumberBase*)cb)->startSwell(scale, totalFrame);
					
					KSCumberBase* cumber = (KSCumberBase*)cb;
					cumber->setAttackPattern(nullptr);
					myGD->communication("CP_onPatternEndOf", cb);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1020") // 자식들 낳기 공격
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					PutChildWrapper* t = PutChildWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1021")
		{
			if(exe)
			{
				startFirePosition = startPosition;
				auto func = [=](CCObject* cb)
				{
					GodOfDeath* t = GodOfDeath::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
					pattern_container->addChild(t);
				};
				castBranch(atype, func, warningFileName);
				
			}
		}
		else if(pattern == "1022")
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				
				MeshWrapper* t = MeshWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				pattern_container->addChild(t);
			};
			castBranch(atype, func, warningFileName);
		}
		else if(pattern == "1023")
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				WiperMissileWrapper* t = WiperMissileWrapper::create(dynamic_cast<KSCumberBase*>(cb), patternData);
				pattern_container->addChild(t);
			};
			castBranch(atype, func, warningFileName);

			
		}
		else if(pattern == "1024")
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				HideCloudWrapper* t = HideCloudWrapper::create(dynamic_cast<KSCumberBase*>(cb), patternData);
				pattern_container->addChild(t);
			};
			castBranch(atype, func, warningFileName);
			
			
		}
		else if(pattern == "1025")
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				JellyWrapper* t = JellyWrapper::create(dynamic_cast<KSCumberBase*>(cb), patternData);
				pattern_container->addChild(t);
			};
			castBranch(atype, func, warningFileName);
		}
			
		else if(pattern.size() >= 2 && pattern[0] == 'a' && pattern[1] == 't') // ccb 관련 공격.
		{
			CircleCCBPieceBase* ccbPiece = dynamic_cast<CircleCCBPieceBase*>(cb);
			if(!ccbPiece)
				return invalid;
			
			if(atype == "crash")
			{
				//			AudioEngine::sharedInstance()->playEffect("sound_attackpattern_crash.mp3", false);
				AudioEngine::sharedInstance()->playEffect("ment_cast_crash.mp3", false, true);
				myGD->communication("Main_showWarning", 2);
				
				AudioEngine::sharedInstance()->playEffect("se_castmap.mp3", true);
				//			AudioEngine::sharedInstance()->playEffect("sound_casting_crash.mp3", true);
				
			}
			else if(atype == "special")
			{
				AudioEngine::sharedInstance()->playEffect("se_castspecial.mp3", true);
				AudioEngine::sharedInstance()->playEffect("ment_cast_special.mp3", false, true);
				//			AudioEngine::sharedInstance()->playEffect("sound_casting_option.mp3", true);
			}
			else // normal
			{
				myGD->communication("Main_showWarning", 1);
				AudioEngine::sharedInstance()->playEffect("se_castmissile.mp3", true);
				//			AudioEngine::sharedInstance()->playEffect("sound_casting_attack.mp3", true);
				//			AudioEngine::sharedInstance()->playEffect("sound_attackpattern_base.mp3", false);
				AudioEngine::sharedInstance()->playEffect("ment_cast_missile.mp3", false, true);
			}
			ccbPiece->runTimeline(patternData); // "at" 같은게 넘어감.
		}
		else
		{
			return invalid;
		}
	}
//	else
//	{
//		return invalid;
//	}
	
	
	
	//	if(!invalid)
	{
		myLog->addLog(kLOG_attackPattern_i, myGD->getCommunication("UI_getUseTime"), atoi(pattern.c_str()));
	}
	return valid;
}





void MissileParent::explosion( CCPoint bombPosition, ccColor4F t_color, float t_angle )
{
	AudioEngine::sharedInstance()->playEffect("sound_jack_missile_bomb.mp3",false);
	initParticle(bombPosition, t_color, t_angle);
}

void MissileParent::createTickingTimeBomb( IntPoint t_point, int t_bombFrameOneTime, int t_bombTimes, int t_rangeCode )
{
	bool is_check = false;
	for(int i=0;i<tickingArray->count();i++)
	{
		TickingTimeBomb* t_ttb = (TickingTimeBomb*)tickingArray->objectAtIndex(i);
		IntPoint settedPoint = t_ttb->getSettedPoint();
		if(settedPoint.x == t_point.x && settedPoint.y == t_point.y)
		{
			is_check = true;
			break;
		}
	}
	
	if(!is_check)
	{
		TickingTimeBomb* t_ttb = TickingTimeBomb::create(t_point, t_bombFrameOneTime, t_bombTimes, 50, t_rangeCode, tickingArray, this, callfunc_selector(MissileParent::resetTickingTimeBomb));
		addChild(t_ttb);
	}
}

void MissileParent::resetTickingTimeBomb()
{
	while(tickingArray->count() > 0)
	{
		TickingTimeBomb* t_ttb = (TickingTimeBomb*)tickingArray->lastObject();
		removeChild(t_ttb, true);
		tickingArray->removeObject(t_ttb);
	}
}


void MissileParent::shootPetMissile( int jm_type, int cmCnt, float damage_per, CCPoint s_p )
{
	AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
	if(jm_type >= 0 && jm_type <= 3)
	{
		vector<KSCumberBase*> subCumberArray = myGD->getSubCumberVector();
		int cumberCnt = subCumberArray.size();

		int random_value;
		
		int boss_count = myGD->getMainCumberCount();
		
		for(int i=0;i<cmCnt;i++)
		{
			if(i < boss_count)
			{
				JackMissile* t_jm = JM_BasicMissile::create(myGD->getMainCumberCCNodeVector()[i], jm_type, damage_per, s_p);
				addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				if(cumberCnt == 0)
				{
					JackMissile* t_jm = JM_BasicMissile::create(myGD->getMainCumberCCNodeVector()[0], jm_type, damage_per, s_p);
					addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					random_value = rand()%cumberCnt;
					JackMissile* t_jm = JM_BasicMissile::create((CCNode*)subCumberArray[random_value], jm_type, damage_per, s_p);
					addChild(t_jm);
					t_jm->startMoving();
				}
			}
		}
	}
	else if(jm_type >= 4 && jm_type <= 6)
	{
		UM_creator* t_c = UM_creator::create(cmCnt, jm_type, damage_per, s_p);
		addChild(t_c);
		t_c->startPetCreate();
	}
}

void MissileParent::initParticle( CCPoint startPosition, ccColor4F t_color, float t_angle )
{
	CCSprite* t_explosion = CCSprite::createWithTexture(explosion_node->getTexture(), CCRectMake(0, 0, 167, 191));
	t_explosion->setScale(1.f/myGD->game_scale);
	t_explosion->setRotation(-t_angle-90);
	t_explosion->setPosition(startPosition);
	explosion_node->addChild(t_explosion);
	
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1f);
	t_animation->addSpriteFrameWithTexture(explosion_node->getTexture(), CCRectMake(0, 0, 167, 191));
	for(int i=0;i<2;i++)
		for(int j=0;j<3;j++)
			t_animation->addSpriteFrameWithTexture(explosion_node->getTexture(), CCRectMake(j*167, i*191, 167, 191));
	
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCFadeTo* t_fade = CCFadeTo::create(0.2f, 0);
	CCRemoveSelf* t_remove = CCRemoveSelf::create();
	CCSequence* t_seq = CCSequence::create(t_animate, t_fade, t_remove, NULL);
	t_explosion->runAction(t_seq);
	
	
	//	particle = CCParticleSystemQuad::createWithTotalParticles(25);
	//	particle->setPositionType(kCCPositionTypeRelative);
	//	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("circle1.png");
	//	particle->setTexture(texture);
	//	particle->setEmissionRate(2500.00); // inf
	//	particle->setAngle(90.0);
	//	particle->setAngleVar(360.0);
	//	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	//	particle->setBlendFunc(blendFunc);
	//	particle->setDuration(0.01);
	//	particle->setEmitterMode(kCCParticleModeGravity);
	//	particle->setStartColor(t_color);
	//	ccColor4F startColorVar = {0,0,0,0};
	//	particle->setStartColorVar(startColorVar);
	//	ccColor4F endColor = {0.00,0.00,0.00,1.00};
	//	particle->setEndColor(endColor);
	//	ccColor4F endColorVar = {0,0,0,0};
	//	particle->setEndColorVar(endColorVar);
	//	particle->setStartSize(0.00);
	//	particle->setStartSizeVar(2.0);
	//	particle->setEndSize(10.0);
	//	particle->setEndSizeVar(2.0);
	//	particle->setGravity(ccp(2.0,10.0));
	//	particle->setRadialAccel(0.0);
	//	particle->setRadialAccelVar(10.0);
	//	particle->setSpeed(50);
	//	particle->setSpeedVar(5);
	//	particle->setTangentialAccel(0);
	//	particle->setTangentialAccelVar(10);
	//	particle->setTotalParticles(25);
	//	particle->setLife(0.50);
	//	particle->setLifeVar(0.1);
	//	particle->setStartSpin(0.0);
	//	particle->setStartSpinVar(0.0);
	//	particle->setEndSpin(0.0);
	//	particle->setEndSpinVar(0.0);
	//	particle->setPosition(startPosition);
	//	particle->setPosVar(ccp(0,0));
	//	particle->setAutoRemoveOnFinish(true);
	//	addChild(particle);
}

void MissileParent::myInit( CCNode* boss_eye )
{
	//chargeArray = new CCArray(1);
	
	clear_condition = NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i);
	
	tickingArray = NULL;
	
	myGD->getMissileParent = [=]()->MissileParent*{
		return this;
	};
	pattern_container = CCClippingNode::create();
	CCSprite* clearMask = CCSprite::create("clear_mask.png");
	pattern_container->setStencil(clearMask);
	pattern_container->getStencil()->setScale(0.0001f);
	pattern_container->setAlphaThreshold(0.1f);
	pattern_container->setInverted(true);
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	float screen_scale_y = myDSH->ui_top/320.f;
	
	
	CCPoint change_origin = ccp(0,0);
	pattern_container->setRectYH(CCRectMake(change_origin.x - 200, change_origin.y - 200, 480, 320*screen_scale_y * 3.f));
	
//	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//	if(screen_scale_x < 1.f)
//		screen_scale_x = 1.f;
//	
//	float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//	float change_scale = 1.f;
//	CCPoint change_origin = ccp(0,0);
//	if(screen_scale_x > 1.f)
//	{
//		change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
//		change_scale = screen_scale_x;
//	}
//	if(screen_scale_y > 1.f)
//		change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
//	CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//	pattern_container->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
	
	
	
	addChild(pattern_container);
	tickingArray = new CCArray(1);
	
	mySW = SW_Parent::create();
	addChild(mySW);
	
	explosion_node = CCSpriteBatchNode::create("fx_monster_hit.png");
	addChild(explosion_node);
	
	jack_missile_node = CCNode::create();
	addChild(jack_missile_node, 10);
//	lastCastTime = 0;
	myGD->I_CCPStrCumberBaseB["MP_attackWithKSCode"] = std::bind(&MissileParent::attackWithKSCode, this, _1, _2, _3, _4);
	myGD->V_CCO["MP_removeChargeInArray"] = std::bind(&MissileParent::removeChargeInArray, this, _1);
	myGD->V_IIFCCP["MP_createJackMissile"] = std::bind(&MissileParent::createJackMissile, this, _1, _2, _3, _4);
	myGD->V_CCO["MP_bombCumber"] = std::bind(&MissileParent::bombCumber, this, _1);
	myGD->createJackMissileWithStoneFunctor = std::bind(&MissileParent::createJackMissileWithStone, this,
																											_1, _2, _3, _4, _5, _6);
	myGD->V_CCPCOLORF["MP_explosion"] = std::bind(&MissileParent::explosion, this, _1, _2, _3);
	myGD->V_IIFCCP["MP_shootPetMissile"] = std::bind(&MissileParent::shootPetMissile, this, _1, _2, _3, _4);
	myGD->V_V["MP_resetTickingTimeBomb"] = std::bind(&MissileParent::resetTickingTimeBomb, this);
	myGD->V_V["MP_subOneDie"] = std::bind(&MissileParent::subOneDie, this);
	myGD->I_V["MP_getJackMissileCnt"] = std::bind(&MissileParent::getJackMissileCnt, this);
	myGD->removeAllPattern = std::bind(&MissileParent::removeAllPattern, this);
	myGD->getNearestCumber = std::bind(&MissileParent::getNearestCumber, this, _1);
	myGD->getNearestCumberWithExclude = std::bind(&MissileParent::getNearestCumberWithExclude, this, _1, _2);
	myGD->attachGodOfDeath = std::bind(&MissileParent::attachGodOfDeath, this, _1, _2);

}

void MissileParent::removeChargeInArray( CCObject* remove_charge )
{
//	auto iter = find(m_charges.begin(), m_charges.end(), remove_charge);
//	if(iter != m_charges.end())
//		m_charges.erase(iter);
//	chargeArray->removeObject(remove_charge);
}

void MissileParent::movingMainCumber()
{
	myGD->communication("CP_movingMainCumber");
}

MissileParent* MissileParent::create( CCNode* boss_eye )
{
	MissileParent* t_mp = new MissileParent();
	t_mp->myInit(boss_eye);
	t_mp->autorelease();
	return t_mp;
}



UM_creator* UM_creator::create( int t_um_tcnt, int t_create_type, float t_missile_speed )
{
	UM_creator* t_c = new UM_creator();
	t_c->myInit(t_um_tcnt, t_create_type, t_missile_speed);
	t_c->autorelease();
	return t_c;
}

UM_creator* UM_creator::create( int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p )
{
	UM_creator* t_c = new UM_creator();
	t_c->myInit(t_um_tcnt, t_create_type, t_missile_speed, s_p);
	t_c->autorelease();
	return t_c;
}

void UM_creator::startCreate()
{
	ing_frame = 0;
	ing_um_cnt = 0;
	schedule(schedule_selector(UM_creator::creating));
}

void UM_creator::startPetCreate()
{
	ing_frame = 0;
	ing_um_cnt = 0;
	schedule(schedule_selector(UM_creator::petCreating));
}

void UM_creator::creating()
{
	ing_frame++;
	
	if(ing_frame%shoot_frame == 0)
	{
		if(ing_frame/shoot_frame <= 1)
		{
			JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getMainCumberCCNodeVector()[ing_frame/shoot_frame-1], create_type, missile_speed);
			getParent()->addChild(t_jm);
			t_jm->startMoving();
		}
		else
		{
			vector<KSCumberBase*> subCumberArray = myGD->getSubCumberVector();
			int cumberCnt = subCumberArray.size();
			int random_value;
			
			if(cumberCnt == 0)
			{
				JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getMainCumberCCNodeVector()[0], create_type, missile_speed);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				random_value = rand()%cumberCnt;
				JackMissile* t_jm = JM_UpgradeMissile::create((CCNode*)subCumberArray[random_value], create_type, missile_speed);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
		}
		ing_um_cnt++;
	}
	
	if(ing_um_cnt >= um_tcnt)
	{
		stopCreate();
	}
}

void UM_creator::petCreating()
{
	ing_frame++;
	
	if(ing_frame%shoot_frame == 0)
	{
		if(ing_frame/shoot_frame <= 1)
		{
			JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getMainCumberCCNodeVector()[ing_frame/shoot_frame-1], create_type, missile_speed, start_position);
			getParent()->addChild(t_jm);
			t_jm->startMoving();
		}
		else
		{
			vector<KSCumberBase*> subCumberArray = myGD->getSubCumberVector();
			int cumberCnt = subCumberArray.size();
			int random_value;
			
			if(cumberCnt == 0)
			{
				JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getMainCumberCCNodeVector()[0], create_type, missile_speed, start_position);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				random_value = rand()%cumberCnt;
				JackMissile* t_jm = JM_UpgradeMissile::create((CCNode*)subCumberArray[random_value], create_type, missile_speed, start_position);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
		}
		ing_um_cnt++;
	}
	
	if(ing_um_cnt >= um_tcnt)
	{
		stopPetCreate();
	}
}

void UM_creator::stopCreate()
{
	unschedule(schedule_selector(UM_creator::creating));
	removeFromParentAndCleanup(true);
}

void UM_creator::stopPetCreate()
{
	unschedule(schedule_selector(UM_creator::petCreating));
	removeFromParentAndCleanup(true);
}

void UM_creator::myInit( int t_um_tcnt, int t_create_type, float t_missile_speed )
{
	if(t_missile_speed < 2.f)
		missile_speed = 2.f;
	else if(t_missile_speed > 9.f)
		missile_speed = 9.f;
	else
		missile_speed = t_missile_speed;
	shoot_frame = missile_speed*3;
	um_tcnt = t_um_tcnt;
	create_type = t_create_type;
}

void UM_creator::myInit( int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p )
{
	start_position = s_p;
	myInit(t_um_tcnt, t_create_type, t_missile_speed);
}
