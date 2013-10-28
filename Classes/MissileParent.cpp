//
//  MissileParent.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 6..
//
//

#include "MissileParent.h"

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

void MissileParent::createJackMissile( int jm_type, int cmCnt, float damage_per )
{
	if(jm_type >= 0 && jm_type <= 3)
	{
		CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
		int cumberCnt = 1 + subCumberArray->count();
		int random_value;

		for(int i=0;i<cmCnt;i++)
		{
			if(i == 0)
			{
				JackMissile* t_jm = JM_BasicMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), jm_type, damage_per);
				addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				random_value = rand()%cumberCnt;
				if(random_value == 0)
				{
					JackMissile* t_jm = JM_BasicMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), jm_type, damage_per);
					addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					JackMissile* t_jm = JM_BasicMissile::create((CCNode*)subCumberArray->objectAtIndex(random_value-1), jm_type, damage_per);
					addChild(t_jm);
					t_jm->startMoving();
				}
			}
		}
	}
	else if(jm_type >= 4 && jm_type <= 6)
	{
		UM_creator* t_c = UM_creator::create(cmCnt, jm_type, damage_per);
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

void MissileParent::actionAP6(CCObject* cb)
{
	AP_Missile6* t_m6 = AP_Missile6::create(startFirePosition, 1);
	addChild(t_m6);

	saveAP = t_m6;
	savedAP = true;
}

void MissileParent::ingAP7(CCObject* cb)
{
	keepAP7->myClock();
}

void MissileParent::actionAP7(CCObject* cb)
{
	myGD->communication("Main_allStopSchedule");
	keepAP7->startCut();
	keepAP7 = NULL;
}

void MissileParent::cancelAP7(CCObject* cb)
{
	keepAP7->stopCut();
	keepAP7 = NULL;
}

void MissileParent::actionAP8(CCObject* cb)
{
	BD_P8 t_bd = SelectedMapData::sharedInstance()->getValuePattern8();

	int m_img_rand_type = rand()%2 + 1;
	AP_Missile8* t_m8 = AP_Missile8::create(startFirePosition, t_bd.mCnt, t_bd.move_speed,
		CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), t_bd.crash_area);
	addChild(t_m8);
}

void MissileParent::actionAP9(CCObject* cb)
{
	BD_P9 t_bd = SelectedMapData::sharedInstance()->getValuePattern9();
	AP_Missile9* t_m9 = AP_Missile9::create(60*5, t_bd.create_frame, t_bd.move_speed, t_bd.crash_area, 1);
	addChild(t_m9);

	saveAP = t_m9;
	savedAP = true;
//	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP10(CCObject* cb)
{
	AP_Missile10* t_m10 = AP_Missile10::create(startFirePosition, 10, 60*2, 60, CCSizeMake(8.f, 8.f));
	addChild(t_m10);

	saveAP = t_m10;
	savedAP = true;
}

void MissileParent::actionAP11(CCObject* cb)
{
//	BD_P11 t_bd = SelectedMapData::sharedInstance()->getValuePattern11();
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile11* t_m11 = AP_Missile11::create(mainCumberPosition, 11, 1.6f, IntSize(round(14.f),round(14.f)));
	addChild(t_m11);
	
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP12(CCObject* cb)
{
	myGD->communication("CP_stopMovingMainCumber");
	int random_value = rand()%2 + 1;
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile12* t_m12 = AP_Missile12::create(mainCumberPosition, random_value, 120, 180);
	addChild(t_m12);

	saveAP = t_m12;
	savedAP = true;
}

void MissileParent::actionAP13(CCObject* cb)
{
	myGD->communication("CP_furyModeOn");
}

void MissileParent::actionAP14(CCObject* cb)
{
//	BD_P14 t_bd = SelectedMapData::sharedInstance()->getValuePattern14();
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile14* t_m14 = AP_Missile14::create(mainCumberPosition, 14, 2.f, 4, IntSize(round(10.f),round(10.f)));
	addChild(t_m14);
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP15(CCObject* cb)
{
	myGD->communication("CP_stopMovingMainCumber");
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile15* t_m15 = AP_Missile15::create(mainCumberPosition, 10, 180);
	addChild(t_m15);

	saveAP = t_m15;
	savedAP = true;
}

void MissileParent::actionAP16(CCObject* cb)
{
	int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
	selected_chapter = selected_chapter%10;
	int mType = (selected_chapter == 5 ? 1 : 2);
	AP_Missile16* t_m16 = AP_Missile16::create(mType, 2+mType, 60);
	addChild(t_m16);

	saveAP = t_m16;
	savedAP = true;
	
	myGD->communication("MP_endIngActionAP");
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP17(CCObject* cb)
{
	myGD->communication("CP_stopMovingMainCumber");
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile17* t_m17 = AP_Missile17::create(mainCumberPosition, 1, 10, 120, 180);
	addChild(t_m17);

	saveAP = t_m17;
	savedAP = true;
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP18(CCObject* cb)
{
	BD_P18 t_bd = SelectedMapData::sharedInstance()->getValuePattern18();
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile18* t_m18 = AP_Missile18::create(mainCumberPosition, t_bd.move_speed, t_bd.mCnt, t_bd.bomb_cnt, t_bd.crash_area.width > 10.f);
	addChild(t_m18);
	
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP19(CCObject* cb)
{
	myGD->communication("CP_tickingOn");
}

void MissileParent::actionAP20(CCObject* cb)
{
	myGD->communication("CP_subCumberBomb");
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP21(CCObject* cb)
{
//	AP_Missile21* t_m21 = AP_Missile21::create(startFirePosition);
//	addChild(t_m21);
//	
//	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP22(CCObject* cb)
{
//	AP_Missile22* t_m22 = AP_Missile22::create(startFirePosition);
//	addChild(t_m22);
//	
//	myGD->communication("MP_endIngActionAP");
//	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP23(CCObject* cb)
{
	if(keepAP23)
	{
		keepAP23->updateCobweb();
	}
	else
	{
		AP_Missile23* t_m23 = AP_Missile23::create(60*5);
		addChild(t_m23);
		keepAP23 = t_m23;
	}

	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP24(CCObject* cb)
{
	if(keepAP24)
	{
		keepAP24->updateSightOut();
	}
	else
	{
		AP_Missile24* t_m24 = AP_Missile24::create(60*5);
		addChild(t_m24);
		keepAP24 = t_m24;
	}myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP25(CCObject* cb)
{
	if(keepAP25)
	{
		keepAP25->updateStun();
	}
	else
	{
		AP_Missile25* t_m25 = AP_Missile25::create(200);
		addChild(t_m25);
		keepAP25 = t_m25;
	}
}

void MissileParent::actionAP26(CCObject* cb)
{
	if(keepAP26)
	{
		keepAP26->updateFreeze();
	}
	else
	{
		AP_Missile26* t_m26 = AP_Missile26::create(200);
		addChild(t_m26);
		keepAP26 = t_m26;
	}
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP27(CCObject* cb)
{
	if(keepAP27)
	{
		keepAP27->updateSleep();
	}
	else
	{
		AP_Missile27* t_m27 = AP_Missile27::create(200);
		addChild(t_m27);
		keepAP27 = t_m27;
	}
}

void MissileParent::actionAP28(CCObject* cb)
{
	int random_value = rand()%2 + 1;
	BD_P28 t_bd = SelectedMapData::sharedInstance()->getValuePattern28();
	AP_Missile28* t_m28 = AP_Missile28::create(startFirePosition, random_value, t_bd.size_radius, t_bd.obj_cnt);
	addChild(t_m28);
	t_m28->startMyAction();
	
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP29(CCObject* cb)
{
	myGD->communication("CP_createAllCumberSheild");
}

void MissileParent::actionAP30(CCObject* cb)
{
	myGD->communication("CP_createSubCumber", myGD->getMainCumberPoint());
}

void MissileParent::actionAP31(CCObject* cb)
{
	myGD->communication("CP_subCumberReplication");
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP32(CCObject* cb)
{
	AP_Missile32* t_m32 = AP_Missile32::create();
	addChild(t_m32);
}

void MissileParent::actionAP33(CCObject* cb)
{
	if(keepAP33)
	{
		keepAP33->updateChaos();
	}
	else
	{
		AP_Missile33* t_m33 = AP_Missile33::create(60*5);
		addChild(t_m33);
		keepAP33 = t_m33;
	}
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP34(CCObject* cb)
{
	myGD->communication("CP_mainCumberInvisibleOn");
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP101(CCObject* cb)
{
	int m_img_rand_type = rand()%2 + 1;
	AP_Missile101* t_m3 = AP_Missile101::create(startFirePosition, 90, 6, 2.f,
		CCString::createWithFormat("chapter1_basic_missile_%d.png", m_img_rand_type)->getCString(), CCSizeMake(6.f, 6.f));
	addChild(t_m3);

	saveAP = t_m3;
	savedAP = true;
}

void MissileParent::actionAP102(CCObject* cb)
{
	int m_img_rand_type = rand()%2 + 1;
	AP_Missile102* t_m4 = AP_Missile102::create(startFirePosition, 60, 6, 4, 20.f*2.f, 2.f,
		CCString::createWithFormat("chapter1_basic_missile_%d.png", m_img_rand_type)->getCString(), CCSizeMake(6.f, 6.f));
	addChild(t_m4);

	saveAP = t_m4;
	savedAP = true;
}

void MissileParent::actionAP103(CCObject* cb)
{
	int m_img_rand_type = rand()%2 + 1;
	AP_Missile102* t_m4 = AP_Missile102::create(startFirePosition, 120, 5, 1, 0, 2.5f,
		CCString::createWithFormat("chapter1_basic_missile_%d.png", m_img_rand_type)->getCString(), CCSizeMake(6.f, 6.f));
	addChild(t_m4);

	saveAP = t_m4;
	savedAP = true;
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
	
	if(pattern == "1")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern1* t = KSTargetAttackPattern1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);

			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "2")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSAttackPattern2* t = KSAttackPattern2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "3")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSAttackPattern3* t = KSAttackPattern3::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "4")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSAttackPattern4* t = KSAttackPattern4::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "5")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "6")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSAttackPattern6* t = KSAttackPattern6::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "7")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSAttackPattern7* t = KSAttackPattern7::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "8")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSAttackPattern8* t = KSAttackPattern8::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "9")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
																		[=](CCObject* cb)
																		{
																			KSAttackPattern9* t = KSAttackPattern9::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																			addChild(t);
																			saveAP = t;
																			savedAP = true;
																		}, cb);
			t_ccn->setChargeColor(ccc4f(0.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	
	else if(pattern == "101")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern1* t = KSTargetAttackPattern1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);

			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "102")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern2* t = KSTargetAttackPattern2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "103")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern3* t = KSTargetAttackPattern3::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "104")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern4* t = KSTargetAttackPattern4::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "105") // 나무 토막
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern5* t = KSTargetAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);

		}
	}
	else if(pattern == "106") // 사과 던지기.
	{
		if(exe)
		{
	
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern6* t = KSTargetAttackPattern6::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "107") // 태양광선.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern7* t = KSTargetAttackPattern7::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "108") // 다용도
	{
		if(exe)
		{
			startFirePosition = startPosition;
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
																		[=](CCObject* cb)
																		{
																			KSTargetAttackPattern8* t = KSTargetAttackPattern8::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																			addChild(t);
																			saveAP = t;
																			savedAP = true;
																		}, cb);

			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "109") // 개돌
	{
		if(exe)
		{
			startFirePosition = startPosition;
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
															[=](CCObject* cb)
															{
																KSCumberBase* t = dynamic_cast<KSCumberBase*>(cb);
																
																int totalFrame = patternData.get("totalframe", 300).asInt();
																t->furyModeOn(totalFrame);
															}, cb);
															
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "110") // 폭죽
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern10* t = KSTargetAttackPattern10::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.00, 1.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "111") // 움직이는 해바라기
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				KSTargetAttackPattern11* t = KSTargetAttackPattern11::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				addChild(t);
																				saveAP = t;
																				savedAP = true;
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.00, 1.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1002")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
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
																			}, cb);

			t_ccn->setChargeColor(ccc4f(1.00, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1003")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
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
																			}, cb);
			
			t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1004")
	{
		if(exe)
		{
			startFirePosition = startPosition;
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				int random_value = rand()%2 + 1;
																				int radius = 80;//pattern.get("radius", 100).asInt();
																				int objcnt = patternData.get("totalframe", 400).asInt();
																				PrisonPattern* t_m28 = PrisonPattern::create(startFirePosition, radius, objcnt);
																				addChild(t_m28);
																				t_m28->startMyAction();
																				
																				myGD->communication("CP_onPatternEnd");
																			}, cb);

			t_ccn->setChargeColor(ccc4f(0.30, 0.30, 0.30, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1005")
	{
		
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
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
																			}, cb);
			t_ccn->setChargeColor(ccc4f(0.20, 0.80, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
		
	}
	else if(pattern == "1006") // 혼란
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
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
																			}, cb);
			
			t_ccn->setChargeColor(ccc4f(0.00, 0.20, 0.30, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);

		}
	}
	else if(pattern == "1007") // 텔포.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				AP_Missile32* t_m32 = AP_Missile32::create();
																				addChild(t_m32);
																			}, cb);
			
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1008") // 인비지블
	{
		if(exe)
		{
			startFirePosition = startPosition;
		

			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				int totalFrame = patternData.get("totalframe", 300).asInt();
																				((KSCumberBase*)cb)->startInvisible(totalFrame);
																				myGD->communication("CP_onPatternEnd");
																			}, cb);
			
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1009") // 불지르기
	{
		if(exe)
		{
			startFirePosition = startPosition;
			
			

			
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn = SpecialChargeNodeLambda::create(startPosition, castFrame,
																			 
																			 [=](CCObject* cb)
																			 {
																				 int totalFrame = patternData.get("totalframe", 600).asInt();
																				 myGD->communication("CP_stopMovingMainCumber");
																				 IntPoint mainCumberPoint = myGD->getMainCumberPoint();
																				 CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
																				 AP_Missile15* t_m15 = AP_Missile15::create(mainCumberPosition, 10, totalFrame);
																				 addChild(t_m15);
																				 
																				 saveAP = t_m15;
																				 savedAP = true;
																			 }, cb);
			
			
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1010") // 위성빔.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn = SpecialChargeNodeLambda::create(startPosition, castFrame,
																			 
				 [=](CCObject* cb)
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
				 }, cb);

			
			
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1011") // 레이저 스캔.
	{
		if(exe)
		{			
			startFirePosition = startPosition;
			SpecialChargeNodeLambda* t_ccn = SpecialChargeNodeLambda::create(startPosition, castFrame,
																			 
																			 [=](CCObject* cb)
																			 {
																				 KSSpecialAttackPattern11* t_m6 = KSSpecialAttackPattern11::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																				 addChild(t_m6);
																				 
																				 saveAP = t_m6;
																				 savedAP = true;
																			 }, cb);
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1012") // 방사능 라인
	{
		if(exe)
		{
			startFirePosition = startPosition;
			SpecialChargeNodeLambda* t_ccn = SpecialChargeNodeLambda::create(startPosition, castFrame,
																			 
					[=](CCObject* cb)
					{
						KSSpecialAttackPattern12* t = KSSpecialAttackPattern12::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
						addChild(t);
						saveAP = t;
						savedAP = true;
					}, cb);
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1013") // 메테오
	{
		if(exe)
		{
			startFirePosition = startPosition;
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			
																			[=](CCObject* cb)
																			{
																				int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
																				selected_chapter = selected_chapter%10;
																				int mType = (selected_chapter == 5 ? 1 : 2);
																				int number = patternData.get("number", 3).asInt();
																				AP_Missile16* t_m16 = AP_Missile16::create(mType, number, 60);
																				addChild(t_m16);
																				
																				saveAP = t_m16;
																				savedAP = true;
																				
																				myGD->communication("MP_endIngActionAP");
																				myGD->communication("CP_onPatternEnd");
																			}, cb);

			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1014") // 떨어지는 돌
	{
		if(exe)
		{
			startFirePosition = startPosition;
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			
																			[=](CCObject* cb)
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
																			}, cb);
			
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
			
			
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
		}
	}
	else if(pattern == "1016") // 다이너마이트.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																			[=](CCObject* cb)
																			{
																				int remainSecond = patternData.get("remainsecond", 9).asInt();
																				
																				TickingTimeBomb* t_ttb = TickingTimeBomb::create(ccp2ip(startPosition), 120, remainSecond, 1, tickingArray, this, callfunc_selector(MissileParent::resetTickingTimeBomb));
																				addChild(t_ttb);
																			}, cb);
			t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
			
		}
	}
	else if(pattern == "1017") // 선을 따라가는 공격
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			CrashChargeNodeLambda* t_ccn =
			CrashChargeNodeLambda::create(startPosition, castFrame,
																		[=](CCObject* cb)
																		{
																			KSSpecialAttackPattern17* t = KSSpecialAttackPattern17::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																			addChild(t);
																			saveAP = t;
																			savedAP = true;
																		}, cb);
			t_ccn->setChargeColor(ccc4f(0.00, 1.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else if(pattern == "1018") // 폭탄 구름.
	{
		if(exe)
		{
			startFirePosition = startPosition;
			//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNodeLambda* t_ccn =
			SpecialChargeNodeLambda::create(startPosition, castFrame,
																		[=](CCObject* cb)
																		{
																			KSSpecialAttackPattern18* t = KSSpecialAttackPattern18::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb), patternD);
																			addChild(t);
																			saveAP = t;
																			savedAP = true;
																		}, cb);
			t_ccn->setChargeColor(ccc4f(0.00, 1.00, 0.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
	}
	else
	{
		return invalid;
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

void MissileParent::stopAutoAttacker()
{
	if(myAA)
		myAA->stopFraming();
}

void MissileParent::startAutoAttacker()
{
	if(myAA)
		myAA->startFraming();
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
	particle = CCParticleSystemQuad::createWithTotalParticles(25);
	particle->setPositionType(kCCPositionTypeRelative);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("circle1.png");
	particle->setTexture(texture);
	particle->setEmissionRate(2500.00); // inf
	particle->setAngle(90.0);
	particle->setAngleVar(360.0);
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	particle->setBlendFunc(blendFunc);
	particle->setDuration(0.01);
	particle->setEmitterMode(kCCParticleModeGravity);
	particle->setStartColor(t_color);
	ccColor4F startColorVar = {0,0,0,0};
	particle->setStartColorVar(startColorVar);
	ccColor4F endColor = {0.00,0.00,0.00,1.00};
	particle->setEndColor(endColor);
	ccColor4F endColorVar = {0,0,0,0};
	particle->setEndColorVar(endColorVar);
	particle->setStartSize(0.00);
	particle->setStartSizeVar(2.0);
	particle->setEndSize(10.0);
	particle->setEndSizeVar(2.0);
	particle->setGravity(ccp(2.0,10.0));
	particle->setRadialAccel(0.0);
	particle->setRadialAccelVar(10.0);
	particle->setSpeed(50);
	particle->setSpeedVar(5);
	particle->setTangentialAccel(0);
	particle->setTangentialAccelVar(10);
	particle->setTotalParticles(25);
	particle->setLife(0.50);
	particle->setLifeVar(0.1);
	particle->setStartSpin(0.0);
	particle->setStartSpinVar(0.0);
	particle->setEndSpin(0.0);
	particle->setEndSpinVar(0.0);
	particle->setPosition(startPosition);
	particle->setPosVar(ccp(0,0));
	particle->setAutoRemoveOnFinish(true);
	addChild(particle);
}

void MissileParent::myInit( CCNode* boss_eye )
{
	myAA = NULL;
	savedAP = false;
	keepAP7 = NULL;
	keepAP23 = NULL;
	keepAP25 = NULL;
	keepAP26 = NULL;
	keepAP27 = NULL;
	keepAP33 = NULL;
	keepAP24 = NULL;
	keepAP34 = NULL;
	keepAP35 = NULL;
	chargeArray = new CCArray(1);
	tickingArray = new CCArray(1);
	myRS = new RandomSelector();

	myRS->myInit(100);
	//		myRS->myInit(85);

	//		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
	//		
	//		if(selected_chapter == 1)								myRS->myInit(0);
	//		else if(mySGD->getIsHard() || selected_chapter > 30)	myRS->myInit(140);
	//		else													myRS->myInit(120);
	setBalance();

	//		if(selected_chapter == 1 || selected_chapter == 2 || selected_chapter == 11 || selected_chapter == 12)
	//		{
	//			if(selected_chapter == 1)
	//			{
	//				if(SelectedMapData::sharedInstance()->getSelectedStage() >= 3)
	//				{
	//					myAA = AutoATK1::create();
	//					addChild(myAA);
	//				}
	//			}
	//			else
	//			{
	//				myAA = AutoATK1::create();
	//				addChild(myAA);
	//			}
	//		}
	//		else if(selected_chapter == 3 || selected_chapter == 4 || selected_chapter == 13 || selected_chapter == 14)
	//		{
	//			myAA = AutoATK2::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 5 || selected_chapter == 6 || selected_chapter == 15 || selected_chapter == 16)
	//		{
	//			myAA = AutoATK3::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 7 || selected_chapter == 8 || selected_chapter == 17 || selected_chapter == 18)
	//		{
	//			myAA = AutoATK4::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 9 || selected_chapter == 10 || selected_chapter == 19 || selected_chapter == 20)
	//		{
	//			myAA = AutoATK5::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 21 || selected_chapter == 31 || selected_chapter == 41)
	//		{
	//			myAA = AutoATK6::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 22 || selected_chapter == 32 || selected_chapter == 42)
	//		{
	//			myAA = AutoATK7::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 23 || selected_chapter == 33 || selected_chapter == 43)
	//		{
	//			myAA = AutoATK8::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 24 || selected_chapter == 34 || selected_chapter == 44)
	//		{
	//			myAA = AutoATK9::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 25 || selected_chapter == 35 || selected_chapter == 45)
	//		{
	//			myAA = AutoATK10::create();
	//			addChild(myAA);
	//		}
	//		else if(selected_chapter == 26 || selected_chapter == 36 || selected_chapter == 46)
	//		{
	//			myAA = AutoATK11::create(boss_eye);
	//			addChild(myAA);
	//		}

	mySW = SW_Parent::create();
	addChild(mySW);


	//		myGD->regMP(this, callfuncCCpB_selector(MissileParent::startFire),
	//					callfuncCCpODv_selector(MissileParent::createSubCumberReplication),
	//					callfuncO_selector(MissileParent::removeChargeInArray),
	//					callfuncIIF_selector(MissileParent::createJackMissile),
	//					callfuncO_selector(MissileParent::bombCumber),
	//					callfuncCCpColor_selector(MissileParent::explosion),
	//					callfunc_selector(MissileParent::endIngActionAP),
	//					callfuncIpIII_selector(MissileParent::createTickingTimeBomb),
	//					callfunc_selector(MissileParent::deleteKeepAP25),
	//					callfunc_selector(MissileParent::deleteKeepAP23),
	//					callfunc_selector(MissileParent::deleteKeepAP26),
	//					callfunc_selector(MissileParent::deleteKeepAP27),
	//					callfunc_selector(MissileParent::deleteKeepAP33),
	//					callfunc_selector(MissileParent::deleteKeepAP24),
	//					callfunc_selector(MissileParent::deleteKeepAP34),
	//					callfunc_selector(MissileParent::protectedAP25),
	//					callfunc_selector(MissileParent::protectedAP26),
	//					callfunc_selector(MissileParent::protectedAP27),
	//					callfunc_selector(MissileParent::protectedAP33),
	//					callfunc_selector(MissileParent::deleteKeepAP35),
	//					callfunc_selector(MissileParent::stopAutoAttacker),
	//					callfuncIIFCCp_selector(MissileParent::shootPetMissile),
	//					callfunc_selector(MissileParent::resetTickingTimeBomb));

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
	myGD->V_V["MP_deleteKeepAP25"] = std::bind(&MissileParent::deleteKeepAP25, this);
	myGD->V_V["MP_deleteKeepAP23"] = std::bind(&MissileParent::deleteKeepAP23, this);
	myGD->V_V["MP_deleteKeepAP26"] = std::bind(&MissileParent::deleteKeepAP26, this);
	myGD->V_V["MP_deleteKeepAP27"] = std::bind(&MissileParent::deleteKeepAP27, this);
	myGD->V_V["MP_deleteKeepAP33"] = std::bind(&MissileParent::deleteKeepAP33, this);
	myGD->V_V["MP_deleteKeepAP24"] = std::bind(&MissileParent::deleteKeepAP24, this);
	myGD->V_V["MP_deleteKeepAP34"] = std::bind(&MissileParent::deleteKeepAP34, this);
	myGD->V_V["MP_protectedAP25"] = std::bind(&MissileParent::protectedAP25, this);
	myGD->V_V["MP_protectedAP26"] = std::bind(&MissileParent::protectedAP26, this);
	myGD->V_V["MP_protectedAP27"] = std::bind(&MissileParent::protectedAP27, this);
	myGD->V_V["MP_protectedAP33"] = std::bind(&MissileParent::protectedAP33, this);
	myGD->V_V["MP_deleteKeepAP35"] = std::bind(&MissileParent::deleteKeepAP35, this);
	myGD->V_V["MP_stopAutoAttacker"] = std::bind(&MissileParent::stopAutoAttacker, this);
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

void MissileParent::setBalance()
{
	SelectedMapData::sharedInstance()->setAttackPattern(myRS);
}
