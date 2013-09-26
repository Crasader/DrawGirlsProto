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

void MissileParent::actionAP6()
{
	AP_Missile6* t_m6 = AP_Missile6::create(startFirePosition, 1);
	addChild(t_m6);

	saveAP = t_m6;
	savedAP = true;
}

void MissileParent::ingAP7()
{
	keepAP7->myClock();
}

void MissileParent::actionAP7()
{
	myGD->communication("Main_allStopSchedule");
	keepAP7->startCut();
	keepAP7 = NULL;
}

void MissileParent::cancelAP7()
{
	keepAP7->stopCut();
	keepAP7 = NULL;
}

void MissileParent::actionAP8()
{
	BD_P8 t_bd = SelectedMapData::sharedInstance()->getValuePattern8();

	int m_img_rand_type = rand()%2 + 1;
	AP_Missile8* t_m8 = AP_Missile8::create(startFirePosition, t_bd.mCnt, t_bd.move_speed,
		CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), t_bd.crash_area);
	addChild(t_m8);
}

void MissileParent::actionAP9()
{
	BD_P9 t_bd = SelectedMapData::sharedInstance()->getValuePattern9();
	AP_Missile9* t_m9 = AP_Missile9::create(60*5, t_bd.create_frame, t_bd.move_speed, t_bd.crash_area, 1);
	addChild(t_m9);

	saveAP = t_m9;
	savedAP = true;
}

void MissileParent::actionAP10()
{
	AP_Missile10* t_m10 = AP_Missile10::create(startFirePosition, 10, 60*2, 60, CCSizeMake(8.f, 8.f));
	addChild(t_m10);

	saveAP = t_m10;
	savedAP = true;
}

void MissileParent::actionAP11()
{
	BD_P11 t_bd = SelectedMapData::sharedInstance()->getValuePattern11();
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile11* t_m11 = AP_Missile11::create(mainCumberPosition, t_bd.mType, t_bd.move_speed, IntSize(round(t_bd.crash_area.width),round(t_bd.crash_area.height)));
	addChild(t_m11);
	
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP12()
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

void MissileParent::actionAP13()
{
	myGD->communication("CP_furyModeOn");
}

void MissileParent::actionAP14()
{
	BD_P14 t_bd = SelectedMapData::sharedInstance()->getValuePattern14();
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile14* t_m14 = AP_Missile14::create(mainCumberPosition, t_bd.mType, t_bd.move_speed, t_bd.mCnt, IntSize(round(t_bd.crash_area.width),round(t_bd.crash_area.height)));
	addChild(t_m14);
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP15()
{
	myGD->communication("CP_stopMovingMainCumber");
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile15* t_m15 = AP_Missile15::create(mainCumberPosition, 10, 180);
	addChild(t_m15);

	saveAP = t_m15;
	savedAP = true;
}

void MissileParent::actionAP16()
{
	int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
	selected_chapter = selected_chapter%10;
	int mType = (selected_chapter == 5 ? 1 : 2);
	AP_Missile16* t_m16 = AP_Missile16::create(mType, 2+mType, 60);
	addChild(t_m16);

	saveAP = t_m16;
	savedAP = true;
}

void MissileParent::actionAP17()
{
	myGD->communication("CP_stopMovingMainCumber");
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile17* t_m17 = AP_Missile17::create(mainCumberPosition, 1, 10, 120, 180);
	addChild(t_m17);

	saveAP = t_m17;
	savedAP = true;
}

void MissileParent::actionAP18()
{
	BD_P18 t_bd = SelectedMapData::sharedInstance()->getValuePattern18();
	IntPoint mainCumberPoint = myGD->getMainCumberPoint();
	CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
	AP_Missile18* t_m18 = AP_Missile18::create(mainCumberPosition, t_bd.move_speed, t_bd.mCnt, t_bd.bomb_cnt, t_bd.crash_area.width > 10.f);
	addChild(t_m18);
}

void MissileParent::actionAP19()
{
	myGD->communication("CP_tickingOn");
}

void MissileParent::actionAP20()
{
	myGD->communication("CP_subCumberBomb");
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP21()
{
	AP_Missile21* t_m21 = AP_Missile21::create(startFirePosition);
	addChild(t_m21);
	
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP22()
{
	AP_Missile22* t_m22 = AP_Missile22::create(startFirePosition);
	addChild(t_m22);
}

void MissileParent::actionAP23()
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

void MissileParent::actionAP24()
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

void MissileParent::actionAP25()
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

void MissileParent::actionAP26()
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

void MissileParent::actionAP27()
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

void MissileParent::actionAP28()
{
	int random_value = rand()%2 + 1;
	BD_P28 t_bd = SelectedMapData::sharedInstance()->getValuePattern28();
	AP_Missile28* t_m28 = AP_Missile28::create(startFirePosition, random_value, t_bd.size_radius, t_bd.obj_cnt);
	addChild(t_m28);
	t_m28->startMyAction();
	
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP29()
{
	myGD->communication("CP_createAllCumberSheild");
}

void MissileParent::actionAP30()
{
	myGD->communication("CP_createSubCumber", myGD->getMainCumberPoint());
}

void MissileParent::actionAP31()
{
	myGD->communication("CP_subCumberReplication");
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP32()
{
	AP_Missile32* t_m32 = AP_Missile32::create();
	addChild(t_m32);
}

void MissileParent::actionAP33()
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

void MissileParent::actionAP34()
{
	myGD->communication("CP_mainCumberInvisibleOn");
	myGD->communication("CP_onPatternEnd");
}

void MissileParent::actionAP101()
{
	int m_img_rand_type = rand()%2 + 1;
	AP_Missile101* t_m3 = AP_Missile101::create(startFirePosition, 90, 6, 2.f,
		CCString::createWithFormat("chapter1_basic_missile_%d.png", m_img_rand_type)->getCString(), CCSizeMake(6.f, 6.f));
	addChild(t_m3);

	saveAP = t_m3;
	savedAP = true;
}

void MissileParent::actionAP102()
{
	int m_img_rand_type = rand()%2 + 1;
	AP_Missile102* t_m4 = AP_Missile102::create(startFirePosition, 60, 6, 4, 20.f*2.f, 2.f,
		CCString::createWithFormat("chapter1_basic_missile_%d.png", m_img_rand_type)->getCString(), CCSizeMake(6.f, 6.f));
	addChild(t_m4);

	saveAP = t_m4;
	savedAP = true;
}

void MissileParent::actionAP103()
{
	int m_img_rand_type = rand()%2 + 1;
	AP_Missile102* t_m4 = AP_Missile102::create(startFirePosition, 120, 5, 1, 0, 2.5f,
		CCString::createWithFormat("chapter1_basic_missile_%d.png", m_img_rand_type)->getCString(), CCSizeMake(6.f, 6.f));
	addChild(t_m4);

	saveAP = t_m4;
	savedAP = true;
}

void MissileParent::attackWithKSCode(CCPoint startPosition, int pattern, KSCumberBase* cb)
{
	if(pattern == kNonTargetAttack1)
	{	
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP1),
															 this, callfunc_selector(MissileParent::cancelKSAP1),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack2)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP2),
															 this, callfunc_selector(MissileParent::cancelKSAP2),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack3)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP3),
															 this, callfunc_selector(MissileParent::cancelKSAP3),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack4)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP4),
															 this, callfunc_selector(MissileParent::cancelKSAP4),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack5)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP5),
															 this, callfunc_selector(MissileParent::cancelKSAP5),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack6)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP6),
															 this, callfunc_selector(MissileParent::cancelKSAP6),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack7)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP7),
															 this, callfunc_selector(MissileParent::cancelKSAP7),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack8)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionKSAP8),
															 this, callfunc_selector(MissileParent::cancelKSAP8),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kNonTargetAttack9)
	{
//		startFirePosition = startPosition;
//		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
//		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
//															 this, NULL,
//															 this, callfunc_selector(MissileParent::actionKSAP9),
//															 this, callfunc_selector(MissileParent::cancelKSAP9),
//															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
//		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
//		addChild(t_ccn);
//		t_ccn->startCharge();
//		chargeArray->addObject(t_ccn);
		
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*2,
														 this, NULL,
														 this, callfunc_selector(MissileParent::actionAP18),
														 this, callfunc_selector(MissileParent::cancelAP18),
														 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);		
	}
	
	else if(pattern == kTargetAttack1)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionTargetAP1),
															 this, callfunc_selector(MissileParent::cancelTargetAP1),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kTargetAttack2)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionTargetAP2),
															 this, callfunc_selector(MissileParent::cancelTargetAP2),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kTargetAttack3)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionTargetAP3),
															 this, callfunc_selector(MissileParent::cancelTargetAP3),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kTargetAttack4)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionTargetAP4),
															 this, callfunc_selector(MissileParent::cancelTargetAP4),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kTargetAttack5) // 나무 토막
	{
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*4,
														 this, NULL,
														 this, callfunc_selector(MissileParent::actionAP11),
														 this, callfunc_selector(MissileParent::cancelAP11),
														 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kTargetAttack6) // 사과 던지기.
	{
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*4,
														 this, NULL,
														 this, callfunc_selector(MissileParent::actionAP14),
														 this, callfunc_selector(MissileParent::cancelAP14),
														 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kTargetAttack7) // 태양광선.
	{
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 90,
														 this, callfunc_selector(MissileParent::ingAP17),
														 this, callfunc_selector(MissileParent::actionAP17),
														 this, callfunc_selector(MissileParent::cancelAP17),
														 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kSpecialAttack1)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionAP21),
															 this, callfunc_selector(MissileParent::cancelAP21),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 1.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kSpecialAttack2)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*2,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionAP24),
															 this, callfunc_selector(MissileParent::cancelAP24),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kSpecialAttack3)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionAP23),
															 this, callfunc_selector(MissileParent::cancelAP23),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kSpecialAttack4)
	{
		startFirePosition = startPosition;
		myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionAP28),
															 this, callfunc_selector(MissileParent::cancelAP28),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.30, 0.30, 0.30, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern == kSpecialAttack5)
	{
		
		
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionAP26),
															 this, callfunc_selector(MissileParent::cancelAP26),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.20, 0.80, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);

	}
	else if(pattern == kSpecialAttack6)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionAP33),
															 this, callfunc_selector(MissileParent::cancelAP33),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.20, 0.30, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
		
	}
	else if(pattern == kSpecialAttack7)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
															 this, NULL,
															 this, callfunc_selector(MissileParent::actionAP32),
															 this, callfunc_selector(MissileParent::cancelAP32),
															 myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
		
	}
	
}

void MissileParent::attackWithCode( CCPoint startPosition, int pattern_code )
{
	if(pattern_code == kAP_CODE_pattern0)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		CCPoint subPosition = ccpSub(jackPosition, startPosition);

		float shootAngle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
		int m_img_rand_type = rand()%2 + 1;
		AP_Missile0* t_m0 = AP_Missile0::create(startPosition, shootAngle, 1.8f,
			CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), CCSize(6.f,6.f));
		addChild(t_m0);
	}
	else if(pattern_code == kAP_CODE_pattern1)
	{
		CCPoint dv;
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);

		dv = ccpSub(jackPosition, startPosition);
		float distance = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
		dv = ccpMult(dv, 1.f/distance);
		BD_P1 t_bd = SelectedMapData::sharedInstance()->getValuePattern1();
		dv = ccpMult(dv, t_bd.move_speed); // speed 2

		int m_img_rand_type = rand()%2 + 1;
		AP_Missile1* t_m1 = AP_Missile1::create(startPosition, dv, t_bd.mCnt, t_bd.spin_radius,
			CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), t_bd.crash_area);
		addChild(t_m1);

	}
	else if(pattern_code == kAP_CODE_pattern2)
	{
		BD_P2 t_bd = SelectedMapData::sharedInstance()->getValuePattern2();

		int m_img_rand_type = rand()%2 + 1;
		AP_Missile2* t_m2 = AP_Missile2::create(startPosition, t_bd.mCnt, t_bd.move_speed,
			CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), t_bd.crash_area);
		addChild(t_m2);


	}
	else if(pattern_code == kAP_CODE_pattern3)
	{
		BD_P3 t_bd = SelectedMapData::sharedInstance()->getValuePattern3();

		int m_img_rand_type = rand()%2 + 1;
		AP_Missile3* t_m3 = AP_Missile3::create(startPosition, t_bd.keep_frame, t_bd.shoot_frame, t_bd.move_speed,
			CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), t_bd.crash_area);
		addChild(t_m3);

		saveAP = t_m3;
		savedAP = true;
	}
	else if(pattern_code == kAP_CODE_pattern4)
	{
		BD_P4 t_bd = SelectedMapData::sharedInstance()->getValuePattern4();

		int m_img_rand_type = rand()%2 + 1;
		AP_Missile4* t_m4 = AP_Missile4::create(startPosition, t_bd.keep_frame, t_bd.shoot_frame, t_bd.shoot_cnt, t_bd.shoot_angle*2.f, t_bd.move_speed,
			CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), t_bd.crash_area);
		addChild(t_m4);

		saveAP = t_m4;
		savedAP = true;
	}
	else if(pattern_code == kAP_CODE_pattern5)
	{
		BD_P5 t_bd = SelectedMapData::sharedInstance()->getValuePattern5();

		int m_img_rand_type = rand()%2 + 1;
		AP_Missile5* t_m5 = AP_Missile5::create(startPosition, t_bd.mCnt, t_bd.move_speed,
			CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), t_bd.crash_area, 1.5f, 0.995f);
		addChild(t_m5);


	}
	else if(pattern_code == kAP_CODE_pattern6)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 60*4,
			this, NULL,			// ing
			this, callfunc_selector(MissileParent::actionAP6),			// after
			this, callfunc_selector(MissileParent::cancelAP6),	// cancel
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(0.00, 0.00, 1.00, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
	else if(pattern_code == kAP_CODE_pattern7)
	{
		AP_Missile7* t_m7 = AP_Missile7::create(60*8);
		addChild(t_m7);

		keepAP7 = t_m7;

		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 60*6,
			this, callfunc_selector(MissileParent::ingAP7),			// ing
			this, callfunc_selector(MissileParent::actionAP7),			// after
			this, callfunc_selector(MissileParent::cancelAP7),	// cancel
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(1.00, 1.00, 1.00, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
	else if(pattern_code == kAP_CODE_pattern8)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 90,
			this, NULL,			// ing
			this, callfunc_selector(MissileParent::actionAP8),			// after
			this, callfunc_selector(MissileParent::cancelAP8),	// cancel
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(0.00, 1.00, 0.00, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
	else if(pattern_code == kAP_CODE_pattern9)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP9),
			this, callfunc_selector(MissileParent::cancelAP9),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
	else if(pattern_code == kAP_CODE_pattern10)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 60,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP10),
			this, callfunc_selector(MissileParent::cancelAP10),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(1.00, 0.00, 1.00, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
	else if(pattern_code == kAP_CODE_pattern11)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*4,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP11),
			this, callfunc_selector(MissileParent::cancelAP11),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern12)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 70*2,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP12),
			this, callfunc_selector(MissileParent::cancelAP12),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 0.80, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern13)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 150,
			this, callfunc_selector(MissileParent::ingAP13),
			this, callfunc_selector(MissileParent::actionAP13),
			this, callfunc_selector(MissileParent::cancelAP13),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 0.70, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern14)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*4,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP14),
			this, callfunc_selector(MissileParent::cancelAP14),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern15)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*2,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP15),
			this, callfunc_selector(MissileParent::cancelAP15),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 0.30, 0.05, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern16)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*6,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP16),
			this, callfunc_selector(MissileParent::cancelAP16),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern17)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 90,
			this, callfunc_selector(MissileParent::ingAP17),
			this, callfunc_selector(MissileParent::actionAP17),
			this, callfunc_selector(MissileParent::cancelAP17),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);

		//			movingMainCumber();
	}
	else if(pattern_code == kAP_CODE_pattern18)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*2,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP18),
			this, callfunc_selector(MissileParent::cancelAP18),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern19)
	{
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*2,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP19),
			this, callfunc_selector(MissileParent::cancelAP19),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.60, 0.20, 0.60, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);


	}
	else if(pattern_code == kAP_CODE_pattern20)
	{
		if(myGD->getCommunication("CP_getSubCumberCount") > 1)
		{
			//				myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			CrashChargeNode* t_ccn = CrashChargeNode::create(startPosition, 60*2,
				this, NULL,
				this, callfunc_selector(MissileParent::actionAP20),
				this, callfunc_selector(MissileParent::cancelAP20),
				myGD->getCommunicationNode("CP_getMainCumberPointer"));
			t_ccn->setChargeColor(ccc4f(0.00, 0.70, 0.60, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);


		}
		else
		{
			myGD->communication("CP_onPatternEnd");
		}
	}
	else if(pattern_code == kAP_CODE_pattern21)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP21),
			this, callfunc_selector(MissileParent::cancelAP21),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 1.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern22)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 90,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP22),
			this, callfunc_selector(MissileParent::cancelAP22),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.50, 0.50, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern23)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP23),
			this, callfunc_selector(MissileParent::cancelAP23),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.80, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern24)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*2,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP24),
			this, callfunc_selector(MissileParent::cancelAP24),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 1.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern25)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP25),
			this, callfunc_selector(MissileParent::cancelAP25),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 1.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern26)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP26),
			this, callfunc_selector(MissileParent::cancelAP26),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.20, 0.80, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern27)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP27),
			this, callfunc_selector(MissileParent::cancelAP27),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 1.00, 0.30, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern28)
	{
		startFirePosition = startPosition;
		myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP28),
			this, callfunc_selector(MissileParent::cancelAP28),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.30, 0.30, 0.30, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern29)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 90,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP29),
			this, callfunc_selector(MissileParent::cancelAP29),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.50, 0.50, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern30)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP30),
			this, callfunc_selector(MissileParent::cancelAP30),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.00, 1.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern31)
	{
		if(myGD->getCommunication("CP_getSubCumberCount") >= 1)
		{
			startFirePosition = startPosition;
			//				myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
			SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*5,
				this, NULL,
				this, callfunc_selector(MissileParent::actionAP31),
				this, callfunc_selector(MissileParent::cancelAP31),
				myGD->getCommunicationNode("CP_getMainCumberPointer"));
			t_ccn->setChargeColor(ccc4f(1.00, 1.00, 1.00, 1.00));
			addChild(t_ccn);
			t_ccn->startCharge();
			chargeArray->addObject(t_ccn);
		}
		else
		{
			myGD->communication("CP_onPatternEnd");
		}
	}
	else if(pattern_code == kAP_CODE_pattern32)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP32),
			this, callfunc_selector(MissileParent::cancelAP32),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(1.00, 0.00, 0.00, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern33)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP33),
			this, callfunc_selector(MissileParent::cancelAP33),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.00, 0.20, 0.30, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern34)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		SpecialChargeNode* t_ccn = SpecialChargeNode::create(startPosition, 60*3,
			this, NULL,
			this, callfunc_selector(MissileParent::actionAP34),
			this, callfunc_selector(MissileParent::cancelAP34),
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_ccn->setChargeColor(ccc4f(0.10, 0.10, 0.10, 1.00));
		addChild(t_ccn);
		t_ccn->startCharge();
		chargeArray->addObject(t_ccn);
	}
	else if(pattern_code == kAP_CODE_pattern35)
	{
		int m_img_rand_type = rand()%2 + 1;
		AP_Missile35* t_m35 = AP_Missile35::create(300, 5, 10, 1.8f, CCString::createWithFormat("chapter%d_basic_missile_%d.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile), m_img_rand_type)->getCString(), CCSize(6.f,6.f));
		addChild(t_m35);
		keepAP35 = t_m35;


	}
	else if(pattern_code == kAP_CODE_pattern101)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 60,
			this, NULL,			// ing
			this, callfunc_selector(MissileParent::actionAP101),			// after
			this, callfunc_selector(MissileParent::cancelAP101),	// cancel
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(1.00, 1.00, 1.00, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
	else if(pattern_code == kAP_CODE_pattern102)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 60,
			this, NULL,			// ing
			this, callfunc_selector(MissileParent::actionAP102),			// after
			this, callfunc_selector(MissileParent::cancelAP102),	// cancel
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(0.30, 0.70, 1.00, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
	else if(pattern_code == kAP_CODE_pattern103)
	{
		startFirePosition = startPosition;
		//			myGD->communication("CP_setMainCumberState", CUMBER_STATE::CUMBERSTATEATTACKREADY); // cumberStateAttackReady
		ChargeNode* t_cn = ChargeNode::create(startPosition, 60,
			this, NULL,			// ing
			this, callfunc_selector(MissileParent::actionAP103),			// after
			this, callfunc_selector(MissileParent::cancelAP103),	// cancel
			myGD->getCommunicationNode("CP_getMainCumberPointer"));
		t_cn->setChargeColor(ccc4f(0.50, 1.00, 0.50, 1.00));
		addChild(t_cn);
		t_cn->startCharge();
		chargeArray->addObject(t_cn);
	}
}

void MissileParent::startFire( CCPoint startPosition, bool crash_attack /*= false*/ )
{
	int pattern_code;

	if(!crash_attack)
	{
		if(myGD->getCommunication("CP_getSubCumberCount") > 40)
			pattern_code = kAP_CODE_pattern20;
		else
		{
			pattern_code = myRS->getNotCrashCode();

			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			IntPoint cumberPoint = myGD->getMainCumberPoint();
			CCPoint cumberPosition = ccp((cumberPoint.x-1)*pixelSize+1,(cumberPoint.y-1)*pixelSize+1);
			CCPoint subPosition = ccpSub(jackPosition, cumberPosition);

			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			if(distance <= 150.f)
			{
				myGD->communication("CP_movingMainCumber");
				return;
			}
		}
	}
	else
	{
		pattern_code = myRS->getCrashCode();
	}

	attackWithCode(startPosition, pattern_code);
}

void MissileParent::createSubCumberReplication( CCPoint s_p, CCObject* sender, SEL_CallFunc d_startMoving )
{
	CreateSubCumberOtherAction* t_cscaa = CreateSubCumberOtherAction::create(IntPoint(int(round((s_p.x-1)/pixelSize+1)), int(round((s_p.y-1)/pixelSize+1))), sender, d_startMoving, sender, d_startMoving);
	addChild(t_cscaa);

	ChargeNode* t_cn = ChargeNode::create(s_p, 60*3, NULL, NULL,
		t_cscaa, callfunc_selector(CreateSubCumberOtherAction::afterAction),
		t_cscaa, callfunc_selector(CreateSubCumberOtherAction::cancelAction), sender);
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

	myGD->V_CCPB["MP_startFire"] = std::bind(&MissileParent::startFire, this, _1, _2);
	myGD->V_CCPI["MP_attackWithCode"] = std::bind(&MissileParent::attackWithCode, this, _1, _2);
//	myGD->V_CCPI["MP_attackWithKSCode"] = std::bind(&MissileParent::attackWithKSCode, this, _1, _2);
	myGD->V_CCPICumberBase["MP_attackWithKSCode"] = std::bind(&MissileParent::attackWithKSCode, this, _1, _2, _3);
	myGD->V_CCPCCOCallfunc["MP_createSubCumberReplication"] = std::bind(&MissileParent::createSubCumberReplication, this, _1, _2, _3);
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
