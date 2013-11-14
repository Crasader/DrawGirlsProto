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
void MissileParent::bombCumber( CCObject* target )
{
	if(myGD->getCommunication("CP_getMainCumberSheild") == 0)
	{
		if(target == myGD->getCommunicationNode("CP_getMainCumberPointer"))
		{
			for(int i=0;i<chargeArray->count();i++)
			{
				ChargeParent* t_cn = (ChargeNode*)chargeArray->objectAtIndex(i);
				t_cn->cancelCharge();
			}
		}
	}

	if(target == myGD->getCommunicationNode("CP_getMainCumberPointer") && savedAP)
	{
		saveAP->stopMyAction();
		endIngActionAP();
	}
}

void MissileParent::createJackMissile( int jm_type, int cmCnt, float missile_speed )
{
	if(jm_type%10 >= 0 && jm_type%10 <= 3)
	{
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumberCnt = 1 + subCumberArray->count();
		int random_value;

		for(int i=0;i<cmCnt;i++)
		{
			if(i == 0)
			{
				JackMissile* t_jm = JM_BasicMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), jm_type, missile_speed);
				addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				random_value = rand()%cumberCnt;
				if(random_value == 0)
				{
					JackMissile* t_jm = JM_BasicMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), jm_type, missile_speed);
					addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					JackMissile* t_jm = JM_BasicMissile::create((CCNode*)subCumberArray->objectAtIndex(random_value-1), jm_type, missile_speed);
					addChild(t_jm);
					t_jm->startMoving();
				}
			}
		}
	}
	else if(jm_type%10 >= 4 && jm_type%10 <= 6)
	{
		UM_creator* t_c = UM_creator::create(cmCnt, jm_type, missile_speed);
		addChild(t_c);
		t_c->startCreate();
	}
}

void MissileParent::subOneDie()
{
	CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");

	if(subCumberArray->count() > 0)
	{
		JackMissile* t_jm = JM_BasicMissile::create((CCNode*)subCumberArray->randomObject(), kElementCode_life, 999999.f);
		addChild(t_jm);
		t_jm->startMoving();
	}
}

void MissileParent::endIngActionAP()
{
	CCLog("saveAP = null");
	saveAP = NULL;
	savedAP = false;
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
	auto castBranch = [=](const std::string atype, std::function<void(CCObject*)> func)
	{
		if(atype == "crash")
		{
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
																			func, cb);
			t_ccn->setChargeColor(ccc4f(0.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
		else if(atype == "special")
		{
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			func, cb);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
		else // normal
		{
			ChargeNodeLambda* t_ccn =
			ChargeNodeLambda::create(startPosition, castFrame,
																			func, cb);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	};
	if(pattern == "1")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern1* t = KSTargetAttackPattern1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
		}
	}
	
	else if(pattern == "2")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern2* t = KSAttackPattern2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "3")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern3* t = KSAttackPattern3::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "4")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern4* t = KSAttackPattern4::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "5")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "6")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern6* t = KSAttackPattern6::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
			
		}
	}
	else if(pattern == "7")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern7* t = KSAttackPattern7::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
			
		}
	}
	else if(pattern == "8")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern8* t = KSAttackPattern8::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
			
		}
	}
	else if(pattern == "9")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSAttackPattern9* t = KSAttackPattern9::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	
	else if(pattern == "101")
	{
		if(exe)
		{
			
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern1* t = KSTargetAttackPattern1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		
		}
	}
	else if(pattern == "102")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern2* t = KSTargetAttackPattern2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "103")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern3* t = KSTargetAttackPattern3::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "104")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern4* t = KSTargetAttackPattern4::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
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
				KSTargetAttackPattern5* t = KSTargetAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
			
		}
	}
	else if(pattern == "106") // 사과 던지기.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern6* t = KSTargetAttackPattern6::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "107") // 태양광선.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern7* t = KSTargetAttackPattern7::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "108") // 다용도
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern8* t = KSTargetAttackPattern8::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
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
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "110") // 폭죽
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern10* t = KSTargetAttackPattern10::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "111") // 움직이는 해바라기
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern11* t = KSTargetAttackPattern11::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
			
		}
	}
	else if(pattern == "112") // 폭탄 여러개 던지기
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern12* t = KSTargetAttackPattern12::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "113") // 리버 스크럽
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSTargetAttackPattern13* t = KSTargetAttackPattern13::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "1001")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				AP_Missile21* t_m21 = AP_Missile21::create(startFirePosition, 400, 1.5f);
				addChild(t_m21);
				
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
		}
	}

	else if(pattern == "1002")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				if(keepAP24)
				{
					keepAP24->updateSightOut();
				}
				else
				{
					int totalFrame = patternData.get("totalframe", 300).asInt();
					AP_Missile24* t_m24 = AP_Missile24::create(totalFrame);
					addChild(t_m24);
					keepAP24 = t_m24;
				}
				myGD->communication("CP_onPatternEnd");
				//		KSSpecialAttackPattern2* t = KSSpecialAttackPattern2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
				//		addChild(t);
				//		saveAP = t;
				//		savedAP = true;
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "1003")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				if(keepAP23)
				{
					keepAP23->updateCobweb();
				}
				else
				{
					int totalFrame = patternData.get("totalframe", 300).asInt();
					
					AP_Missile23* t_m23 = AP_Missile23::create(totalFrame);
					addChild(t_m23);
					keepAP23 = t_m23;
				}
				
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "1004")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				int random_value = rand()%2 + 1;
				int radius = 80;//pattern.get("radius", 100).asInt();
				int objcnt = patternData.get("totalframe", 400).asInt();
				PrisonPattern* t_m28 = PrisonPattern::create(startFirePosition, radius, objcnt);
				addChild(t_m28);
				t_m28->startMyAction();
				
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "1005")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				if(keepAP26)
				{
					keepAP26->updateFreeze();
				}
				else
				{
					int totalFrame = patternData.get("totalframe", 200).asInt();
					AP_Missile26* t_m26 = AP_Missile26::create(totalFrame);
					addChild(t_m26);
					keepAP26 = t_m26;
				}
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
			
		}
		
	}
	else if(pattern == "1006") // 혼란
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				if(keepAP33)
				{
					keepAP33->updateChaos();
				}
				else
				{
					int totalFrame = patternData.get("totalframe", 300).asInt();
					AP_Missile33* t_m33 = AP_Missile33::create(totalFrame);
					addChild(t_m33);
					keepAP33 = t_m33;
				}
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "1007") // 텔포.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				AP_Missile32* t_m32 = AP_Missile32::create();
				addChild(t_m32);
			};
			castBranch(atype, func);
			
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
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "1009") // 불지르기
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				int totalFrame = patternData.get("totalframe", 600).asInt();
				myGD->communication("CP_stopMovingMainCumber");
				IntPoint mainCumberPoint = myGD->getMainCumberPoint();
				CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
				AP_Missile15* t_m15 = AP_Missile15::create(mainCumberPosition, 10, totalFrame);
				addChild(t_m15);
				
				saveAP = t_m15;
				savedAP = true;
			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "1010") // 위성빔.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				int targetingFrame = patternData.get("targetingframe", 120).asInt();
				int shootFrame = patternData.get("shootframe", 180).asInt();
				int random_value = rand()%2 + 1;
				
				IntPoint mainCumberPoint = myGD->getMainCumberPoint();
				CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
				AP_Missile12* t_m12 = AP_Missile12::create(mainCumberPosition, random_value, targetingFrame, shootFrame);
				addChild(t_m12);
				
				saveAP = t_m12;
				savedAP = true;
				myGD->communication("CP_onPatternEnd");

			};
			castBranch(atype, func);
		}
	}
	else if(pattern == "1011") // 레이저 스캔.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSSpecialAttackPattern11* t_m6 = KSSpecialAttackPattern11::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t_m6);
				
				saveAP = t_m6;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "1012") // 방사능 라인
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSSpecialAttackPattern12* t = KSSpecialAttackPattern12::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			

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
				AP_Missile16* t_m16 = AP_Missile16::create(mType, number, 60);
				addChild(t_m16);
				
				saveAP = t_m16;
				savedAP = true;
				
				myGD->communication("MP_endIngActionAP");
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
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
				AP_Missile9* t_m9 = AP_Missile9::create(totalframe, shootframe, speed, CCSizeMake(30, 30), 1);
				addChild(t_m9);
				saveAP = t_m9;
				savedAP = true;
				
				myGD->communication("MP_endIngActionAP");
				myGD->communication("CP_onPatternEnd");
			};
			castBranch(atype, func);
					
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
			
			TickingTimeBomb* t_ttb = TickingTimeBomb::create(ccp2ip(startPosition), 120, remainSecond, 1, tickingArray, this, callfunc_selector(MissileParent::resetTickingTimeBomb));
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
				KSSpecialAttackPattern17* t = KSSpecialAttackPattern17::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
		}
	}
	else if(pattern == "1018") // 폭탄 구름.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			auto func = [=](CCObject* cb)
			{
				KSSpecialAttackPattern18* t = KSSpecialAttackPattern18::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
				addChild(t);
				saveAP = t;
				savedAP = true;
			};
			castBranch(atype, func);
			
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




void MissileParent::createSubCumberReplication( CCPoint s_p,
											   CCObject* sender, SEL_CallFuncO d_startMoving )
{
	CreateSubCumberOtherAction* t_cscaa = CreateSubCumberOtherAction::create(IntPoint(int(round((s_p.x-1)/pixelSize+1)), int(round((s_p.y-1)/pixelSize+1))), sender, d_startMoving, sender, d_startMoving);
	addChild(t_cscaa);

	ChargeNode* t_cn = ChargeNode::create(s_p, 60*3, NULL, NULL,
		t_cscaa, callfuncO_selector(CreateSubCumberOtherAction::afterAction),
		t_cscaa, callfuncO_selector(CreateSubCumberOtherAction::cancelAction), sender);
	addChild(t_cn);
	t_cn->startCharge();

	chargeArray->addObject(t_cn);
}

void MissileParent::explosion( CCPoint bombPosition, ccColor4F t_color )
{
	AudioEngine::sharedInstance()->playEffect("sound_jack_missile_bomb.mp3",false);
	initParticle(bombPosition, t_color);
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
		TickingTimeBomb* t_ttb = TickingTimeBomb::create(t_point, t_bombFrameOneTime, t_bombTimes, t_rangeCode, tickingArray, this, callfunc_selector(MissileParent::resetTickingTimeBomb));
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
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumberCnt = 1 + subCumberArray->count();
		int random_value;

		for(int i=0;i<cmCnt;i++)
		{
			if(i == 0)
			{
				JackMissile* t_jm = JM_BasicMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), jm_type, damage_per, s_p);
				addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				random_value = rand()%cumberCnt;
				if(random_value == 0)
				{
					JackMissile* t_jm = JM_BasicMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), jm_type, damage_per, s_p);
					addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					JackMissile* t_jm = JM_BasicMissile::create((CCNode*)subCumberArray->objectAtIndex(random_value-1), jm_type, damage_per, s_p);
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

void MissileParent::initParticle( CCPoint startPosition, ccColor4F t_color )
{
	CCSprite* t_explosion = CCSprite::create("fx_boss_hit1.png");
	t_explosion->setScale(1.f/1.5f);
	t_explosion->setPosition(startPosition);
	addChild(t_explosion);
	
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.1f);
	for(int i=1;i<=7;i++)
		t_animation->addSpriteFrameWithFileName(CCString::createWithFormat("fx_boss_hit%d.png", i)->getCString());
	
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
	savedAP = false;
	keepAP23 = NULL;
	keepAP26 = NULL;
	keepAP33 = NULL;
	keepAP24 = NULL;

	chargeArray = new CCArray(1);
	tickingArray = new CCArray(1);

	mySW = SW_Parent::create();
	addChild(mySW);

//	myGD->V_CCPB["MP_startFire"] = std::bind(&MissileParent::startFire, this, _1, _2);
	
//	myGD->V_CCPI["MP_attackWithCode"] = std::bind(&MissileParent::attackWithCode, this, _1, _2);
	
//	myGD->V_CCPI["MP_attackWithKSCode"] = std::bind(&MissileParent::attackWithKSCode, this, _1, _2);
	myGD->I_CCPStrCumberBaseB["MP_attackWithKSCode"] =
		std::bind(&MissileParent::attackWithKSCode, this, _1, _2, _3, _4);
	myGD->V_CCPCCOCallfuncO["MP_createSubCumberReplication"] = std::bind(&MissileParent::createSubCumberReplication, this, _1, _2, _3);
	myGD->V_CCO["MP_removeChargeInArray"] = std::bind(&MissileParent::removeChargeInArray, this, _1);
	myGD->V_IIF["MP_createJackMissile"] = std::bind(&MissileParent::createJackMissile, this, _1, _2, _3);
	myGD->V_CCO["MP_bombCumber"] = std::bind(&MissileParent::bombCumber, this, _1);
	myGD->V_CCPCOLOR["MP_explosion"] = std::bind(&MissileParent::explosion, this, _1, _2);
	myGD->V_V["MP_endIngActionAP"] = std::bind(&MissileParent::endIngActionAP, this);
	myGD->V_IpIII["MP_createTickingTimeBomb"] = std::bind(&MissileParent::createTickingTimeBomb, this, _1, _2, _3, _4);
//	myGD->V_V["MP_deleteKeepAP25"] = std::bind(&MissileParent::deleteKeepAP25, this);
	myGD->V_V["MP_deleteKeepAP23"] = std::bind(&MissileParent::deleteKeepAP23, this);
	myGD->V_V["MP_deleteKeepAP26"] = std::bind(&MissileParent::deleteKeepAP26, this);
//	myGD->V_V["MP_deleteKeepAP27"] = std::bind(&MissileParent::deleteKeepAP27, this);
	myGD->V_V["MP_deleteKeepAP33"] = std::bind(&MissileParent::deleteKeepAP33, this);
	myGD->V_V["MP_deleteKeepAP24"] = std::bind(&MissileParent::deleteKeepAP24, this);
//	myGD->V_V["MP_deleteKeepAP34"] = std::bind(&MissileParent::deleteKeepAP34, this);
//	myGD->V_V["MP_protectedAP25"] = std::bind(&MissileParent::protectedAP25, this);
	myGD->V_V["MP_protectedAP26"] = std::bind(&MissileParent::protectedAP26, this);
//	myGD->V_V["MP_protectedAP27"] = std::bind(&MissileParent::protectedAP27, this);
	myGD->V_V["MP_protectedAP33"] = std::bind(&MissileParent::protectedAP33, this);
//	myGD->V_V["MP_deleteKeepAP35"] = std::bind(&MissileParent::deleteKeepAP35, this);
//	myGD->V_V["MP_stopAutoAttacker"] = std::bind(&MissileParent::stopAutoAttacker, this);
	myGD->V_IIFCCP["MP_shootPetMissile"] = std::bind(&MissileParent::shootPetMissile, this, _1, _2, _3, _4);
	myGD->V_V["MP_resetTickingTimeBomb"] = std::bind(&MissileParent::resetTickingTimeBomb, this);
	myGD->V_V["MP_subOneDie"] = std::bind(&MissileParent::subOneDie, this);
}

void MissileParent::removeChargeInArray( CCObject* remove_charge )
{
	chargeArray->removeObject(remove_charge);
}

void MissileParent::movingMainCumber()
{
	myGD->communication("CP_movingMainCumber");
}
