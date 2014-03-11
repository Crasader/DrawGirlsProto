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
void MissileParent::bombCumber( CCObject* target )
{
	KSCumberBase* cumber = (KSCumberBase*)target;
	if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
	{
		if(cumber->getChargeParent())
		{
			cumber->getChargeParent()->cancelCharge();
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
		cumber->getAttackPattern()->stopMyAction();
		cumber->setAttackPattern(nullptr);
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
	CCLog("createJackMissile inner : %d, %d, %.2f", jm_type, cmCnt, missile_speed);
	
	int card_number;
	if(mySGD->getIsUsingFriendCard())
		card_number = mySGD->getSelectedFriendCardData().card_number;
	else
		card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	if(card_number > 0)
	{
		if(jm_type%10 >= 0 && jm_type%10 <= 3)
		{
			CCLog("base JackMissile");
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
			
			CCSprite* card_img = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png", card_number)->getCString()));
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
			CCLog("Upgrade JackMissile");
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
			
			CCSprite* card_img = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png", card_number)->getCString()));
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

void MissileParent::createJackMissileWithStone(StoneType stoneType, int grade, int level, int missileNumbers, CCPoint initPosition)
{
//CCNode* targetNode, CCPoint initPosition, float initSpeed, int power, bool cancelCasting, bool stiffen)
	int r = rand() % (myGD->getMainCumberCount());
	//stoneType = StoneType::kStoneType_spread;
	//level = level == 0 ? 1 : level;
	int power = grade * 20 + level * 10;
	AttackOption ao = getAttackOption(stoneType, grade);
	if(stoneType == StoneType::kStoneType_guided)
	{
		for(int i=0; i<missileNumbers; i++)
		{
			string fileName = boost::str(boost::format("me_guide%||.ccbi") % level);
			GuidedMissile* gm = GuidedMissile::create(myGD->getMainCumberCCNodeVector()[r], initPosition,
																								fileName,
																								1.2f, power, 30 + 10 * grade,
																								ao

																							 );
			jack_missile_node->addChild(gm);
		}
	}
	else if(stoneType == StoneType::kStoneType_mine)
	{
		for(int i=0; i<missileNumbers; i++)
		{
			IntPoint mapPoint;
			bool found = myGD->getEmptyRandomPoint(&mapPoint, 5);
			if(found == true)
			{
				MineAttack* ma = MineAttack::create(initPosition, ip2ccp(mapPoint), 10 + 3 * grade, power, ao);
				jack_missile_node->addChild(ma);	
			}
		}
	}
	else if(stoneType == StoneType::kStoneType_laser)
	{
		//LaserAttack* la = LaserAttack::create(0, 400, 33.f, AttackOption::kNoOption);
		LaserWrapper* lw = LaserWrapper::create(2 + (grade-1), 400 + missileNumbers * 50, 
																						power, ao);
		jack_missile_node->addChild(lw);
	}

	else if(stoneType == StoneType::kStoneType_range)
	{
		RangeAttack* ra = RangeAttack::create(initPosition, 30 + missileNumbers * 10, 500 + 50 * grade,
																				 	power, 30, ao);
		addChild(ra);
	}

	else if(stoneType == StoneType::kStoneType_global)
	{
		for(int i=0; i<=7 + 3 * missileNumbers; i++)
		{
			RandomBomb* rb = RandomBomb::create(70 + grade * 20, power, ao);
			addChild(rb);
		}
	}
	else if(stoneType == StoneType::kStoneType_spirit)
	{
		
		for(int i=0; i<missileNumbers; i++)
		{
			IntPoint mapPoint2;
			bool found2 = myGD->getEmptyRandomPoint(&mapPoint2, 5);
			if(found2 == true)
			{
				string fileName = boost::str(boost::format("me_pet%||.ccbi") % level);
				SpiritAttack* sa = SpiritAttack::create(initPosition, ip2ccp(mapPoint2), fileName, 
																								30 + grade * 10, power, 1.2f, 20, ao);
				jack_missile_node->addChild(sa);	
			}
		}
	}
	else if(stoneType == StoneType::kStoneType_spread)
	{
		for(int i=0; i<missileNumbers; i++)
		{
			int adderForGrade = 0;
			if(grade == 2)
				adderForGrade = 1;
			else if(grade == 3)
				adderForGrade = 3;
			else if(grade == 4)
				adderForGrade = 7;
			else if(grade == 5)
				adderForGrade = 14;
			else
				adderForGrade = 0;
			string fileName = boost::str(boost::format("me_redial%||.ccbi") % level);
			addChild(KSIntervalCall::create(20, missileNumbers, [=](int seq){
				SpreadMissile* sm = SpreadMissile::create(myGD->getMainCumberVector()[r], initPosition,
																									fileName,
																									1.2f, power, 4 + adderForGrade, AttackOption::kNoOption);
				jack_missile_node->addChild(sm);

			}));
		}
	}
	
	
	
	

	
}
AttackOption MissileParent::getAttackOption(StoneType st, int grade)
{
	
	if(st == StoneType::kStoneType_guided)
	{
		switch(grade)
		{
			case 1:
				return AttackOption::kNoOption;
			case 2:
				return AttackOption::kGold;
			case 3:
				return AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kStiffen;
			case 5:
				return AttackOption::kPlusScore;
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
				return AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kStiffen;
			case 5:
				return AttackOption::kPlusScore;
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
				return AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kUnbeatable;
			case 5:
				return AttackOption::kPlusScore;
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
				return AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kMonsterSpeedDown;
			case 5:
				return AttackOption::kPlusScore;
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
				return AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kJackSpeedUp;
			case 5:
				return AttackOption::kPlusScore;
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
				return AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kJackSpeedUp;
			case 5:
				return AttackOption::kPlusScore;
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
				return AttackOption::kCancelCasting;
			case 4:
				return AttackOption::kPoisonedNiddle;
			case 5:
				return AttackOption::kPlusScore;
			default:
				
				return AttackOption::kNoOption;
		}
	}
	return AttackOption::kNoOption;
}
int MissileParent::getJackMissileCnt()
{
	return jack_missile_node->getChildrenCount();
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
}

void MissileParent::endIngActionAP()
{
	CCLog("saveAP = null");
//	saveAP = NULL;
}


int MissileParent::attackWithKSCode(CCPoint startPosition, std::string patternD, KSCumberBase* cb, bool exe)
{
	Json::Value patternData;
	Json::Reader reader;
	reader.parse(patternD, patternData);
	
	int castFrame = patternData.get("castframe", 120).asInt();
	// 캐스팅 실패하면 캐스팅 시간 점점 줄음.
	castFrame = MAX(30, castFrame - (castFrame * 0.1f)* cb->getCastingCancelCount());
	int valid = 1;
	int invalid = 0;
	
	std::string pattern = patternData["pattern"].asString();
	std::string atype = patternData["atype"].asString();
	auto castBranch = [=](const std::string atype, std::function<void(CCObject*)> func, const std::string& warningFileName)
	{
		if(atype == "crash")
		{
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
										  func, cb, pattern);
			t_ccn->setChargeColor(ccc4f(0.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			CCLog("%x", t_ccn);
			cb->setChargeParent(t_ccn);
		}
		else if(atype == "special")
		{
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
											func, cb, pattern);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			cb->setChargeParent(t_ccn);
		}
		else // normal
		{
			ChargeNodeLambda* t_ccn =
			ChargeNodeLambda::create(startPosition, castFrame,
									 func, cb, pattern);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			cb->setChargeParent(t_ccn);
		}
		myGD->communication("Main_showScreenSideWarning"); // 화면에 빨간 테두리 만드는 함수
		myGD->communication("Main_showDetailMessage", warningFileName);
		myGD->communication("Main_showThumbWarning", startPosition);
	};
	string warningFileName = "warning_" + pattern + ".ccbi";
	if(pattern == "1")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			auto func = [=](CCObject* cb)
			{
				UnusedMissile1* t = UnusedMissile1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
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
				TornadoWrapper* t_m21 = TornadoWrapper::create(startFirePosition, totalFrame, 1.5f);
				addChild(t_m21);
				
				myGD->communication("CP_onPatternEndOf", cb);
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
					addChild(t_m24);
					cumber->setSightOutAttack(t_m24);
				}
				myGD->communication("CP_onPatternEndOf", cb);
				//		SightOutWrapper* t = SightOutWrapper::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
				//		addChild(t);
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
					addChild(t_m23);
					cumber->setCobWebAttack(t_m23);
				}
				
				myGD->communication("CP_onPatternEndOf", cb);
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
				PrisonPattern* t_m28 = PrisonPattern::create(startFirePosition, radius, objcnt);
				addChild(t_m28);
				t_m28->startMyAction();
				
				myGD->communication("CP_onPatternEndOf", cb);
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
					addChild(t_m26);
					cumber->setFreezeAttack(t_m26);
				}
				myGD->communication("CP_onPatternEndOf", cb);
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
					addChild(t_m33);
					cumber->setChaosAttack(t_m33);
				}
				myGD->communication("CP_onPatternEndOf", cb);
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
				TeleportWrapper* t_m32 = TeleportWrapper::create();
				addChild(t_m32);
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
				addChild(t_m15);
				
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t_m15);
				
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
				ThunderBoltWrapper* t_m12 = ThunderBoltWrapper::create(mainCumberPosition, (KSCumberBase*)cb, random_value, targetingFrame, shootFrame);
				addChild(t_m12);
				
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t_m12);
				
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
				addChild(t_m6);
				
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t_m6);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				
				MeteorWrapper* t_m16 = MeteorWrapper::create(mType, number, 60, patternData.get("area", 50).asInt());
				addChild(t_m16);
				
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t_m16);
				
				
				myGD->communication("MP_endIngActionAP");
				myGD->communication("CP_onPatternEndOf", cb);
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
				FallingStoneWrapper* t_m9 = FallingStoneWrapper::create(totalframe, (KSCumberBase*)cb, shootframe, speed, CCSizeMake(30, 30), 1);
				addChild(t_m9);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t_m9);
				
				
				myGD->communication("MP_endIngActionAP");
				myGD->communication("CP_onPatternEndOf", cb);
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
			addChild(t_to);
			
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
			addChild(t_ttb);
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				addChild(t);
				KSCumberBase* cumber = (KSCumberBase*)cb;
				cumber->setAttackPattern(t);
				
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
				myGD->communication("CP_onPatternEndOf", cb);
			};
			castBranch(atype, func, warningFileName);
			
		}
	}
	else if(pattern.size() >= 2 && pattern[0] == 'a' && pattern[1] == 't') // ccb 관련 공격.
	{
		CircleCCBPieceBase* ccbPiece = dynamic_cast<CircleCCBPieceBase*>(cb);
		if(!ccbPiece)
			return invalid;
		
		if(atype == "crash")
		{
			AudioEngine::sharedInstance()->playEffect("sound_attackpattern_crash.mp3", false);
			myGD->communication("Main_showWarning", 2);
			
			AudioEngine::sharedInstance()->playEffect("sound_casting_crash.mp3", true);
			
		}
		else if(atype == "special")
		{
			AudioEngine::sharedInstance()->playEffect("sound_casting_option.mp3", true);
			
			
		}
		else // normal
		{
			myGD->communication("Main_showWarning", 1);
			AudioEngine::sharedInstance()->playEffect("sound_casting_attack.mp3", true);
			AudioEngine::sharedInstance()->playEffect("sound_attackpattern_base.mp3", false);
		}
		ccbPiece->runTimeline(patternData); // "at" 같은게 넘어감.
	}
	else
	{
		return invalid;
	}
	
	
	//	if(!invalid)
	{
		myLog->addLog(kLOG_attackPattern_i, myGD->getCommunication("UI_getUseTime"), atoi(pattern.c_str()));
	}
	return valid;
}




//void MissileParent::createSubCumberReplication( CCPoint s_p,
												 //CCObject* sender, SEL_CallFuncO d_startMoving )
//{
	//CreateSubCumberOtherAction* t_cscaa = CreateSubCumberOtherAction::create(IntPoint(int(round((s_p.x-1)/pixelSize+1)), int(round((s_p.y-1)/pixelSize+1))), sender, d_startMoving, sender, d_startMoving);
	//addChild(t_cscaa);
	
	//ChargeNode* t_cn = ChargeNode::create(s_p, 60*3, NULL, NULL,
											//t_cscaa, callfuncO_selector(CreateSubCumberOtherAction::afterAction),
											//t_cscaa, callfuncO_selector(CreateSubCumberOtherAction::cancelAction), sender);
	//addChild(t_cn);
	//t_cn->startCharge();
	
	//chargeArray->addObject(t_cn);
//}

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
	tickingArray = new CCArray(1);
	
	mySW = SW_Parent::create();
	addChild(mySW);
	
	explosion_node = CCSpriteBatchNode::create("fx_monster_hit.png");
	addChild(explosion_node);
	
	jack_missile_node = CCNode::create();
	addChild(jack_missile_node, 10);
	
	myGD->I_CCPStrCumberBaseB["MP_attackWithKSCode"] =
	std::bind(&MissileParent::attackWithKSCode, this, _1, _2, _3, _4);
	myGD->V_CCO["MP_removeChargeInArray"] = std::bind(&MissileParent::removeChargeInArray, this, _1);
	myGD->V_IIFCCP["MP_createJackMissile"] = std::bind(&MissileParent::createJackMissile, this, _1, _2, _3, _4);
	myGD->V_CCO["MP_bombCumber"] = std::bind(&MissileParent::bombCumber, this, _1);
	myGD->createJackMissileWithStoneFunctor = std::bind(&MissileParent::createJackMissileWithStone, this,
																											_1, _2, _3, _4, _5);
	myGD->V_CCPCOLORF["MP_explosion"] = std::bind(&MissileParent::explosion, this, _1, _2, _3);
	myGD->V_V["MP_endIngActionAP"] = std::bind(&MissileParent::endIngActionAP, this);
	myGD->V_IIFCCP["MP_shootPetMissile"] = std::bind(&MissileParent::shootPetMissile, this, _1, _2, _3, _4);
	myGD->V_V["MP_resetTickingTimeBomb"] = std::bind(&MissileParent::resetTickingTimeBomb, this);
	myGD->V_V["MP_subOneDie"] = std::bind(&MissileParent::subOneDie, this);
	myGD->I_V["MP_getJackMissileCnt"] = std::bind(&MissileParent::getJackMissileCnt, this);
}

void MissileParent::removeChargeInArray( CCObject* remove_charge )
{
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


//CreateSubCumberOtherAction* CreateSubCumberOtherAction::create( IntPoint c_p, CCObject* t_after, SEL_CallFuncO d_after, CCObject* t_cancel, SEL_CallFuncO d_cancel )
//{
	//CreateSubCumberOtherAction* t_CSCAA = new CreateSubCumberOtherAction();
	//t_CSCAA->myInit(c_p, t_after, d_after, t_cancel, d_cancel);
	//t_CSCAA->autorelease();
	//return t_CSCAA;
//}

//void CreateSubCumberOtherAction::afterAction( CCObject* cb )
//{
	//myGD->communication("CP_createSubCumber", createPoint);
	//(after_target->*after_delegate)(cb);
	//removeFromParentAndCleanup(true);
//}

//void CreateSubCumberOtherAction::cancelAction( CCObject* cb )
//{
	//(cancel_target->*cancel_delegate)(cb);
	//removeFromParentAndCleanup(true);
//}

//void CreateSubCumberOtherAction::myInit( IntPoint c_p, CCObject* t_after, SEL_CallFuncO d_after, CCObject* t_cancel, SEL_CallFuncO d_cancel )
//{
	//createPoint = c_p;
	//after_target = t_after;
	//after_delegate = d_after;
	//cancel_target = t_cancel;
	//cancel_delegate = d_cancel;
//}

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
