//
//  Cumber.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 29..
//
//

#include "Cumber.h"

void CumberParent::setMainCumberState(int t_cs)
{
	mainCumber->setCumberState(t_cs);
}

void CumberParent::allStopSchedule()
{
	cumberState mainCumberState = mainCumber->getCumberState();
	if(mainCumberState == cumberStateMoving)
		mainCumber->stopMoving();
	else
		myMP->pauseSchedulerAndActions();
}



void CumberParent::tickingOn()
{
	mainCumber->tickingOn();
}

void CumberParent::movingMainCumber()
{
	if(!isGameover)
		mainCumber->startMoving();
}

void CumberParent::stopMovingMainCumber()
{
	mainCumber->stopMoving();
}

CCNode* CumberParent::getMainCumberPointer()
{
	return mainCumber;
}

CCArray* CumberParent::getSubCumberArrayPointer()
{
	return subCumberArray;
}

void CumberParent::decreaseLifeForSubCumber(CCObject* target, float t_damage, float t_directionAngle)
{
	if(target == mainCumber)
	{
		if(mainCumber->isSheild == 1)
			mainCumber->crashSheild();
	}
	else
	{
		for(int i=0;i<subCumberArray->count();i++)
		{
			SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
			
			if(target == t_sc)
			{
				if(t_sc->isSheild == 0)
					t_sc->decreaseLife(t_damage, t_directionAngle);
				else
					t_sc->crashSheild();
				break;
			}
		}
	}
}

void CumberParent::createAllCumberSheild()
{
	mainCumber->createSheild();
	for(int i = 0;i<subCumberArray->count();i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->createSheild();
	}
}


void CumberParent::subCumberBomb()
{
	for(int i = 0;i<subCumberArray->count();i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->startSelfBomb();
	}
}

void CumberParent::subCumberReplication()
{
	int cnt = subCumberArray->count();
	for(int i = 0;i<cnt;i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		IntPoint t_mp = t_sc->getMapPoint();
		SubCumber* t_sc2 = SubCumber::create(t_mp);
		addChild(t_sc2);
		t_sc2->startMoving();
		subCumberArray->addObject(t_sc2);
	}
}

void CumberParent::setGameover()
{
	myEP->setGameover();
	myGD->communication("MP_stopAutoAttacker");
	isGameover = true;
	mainCumber->setGameover();
	
	int loop_cnt = subCumberArray->count();
	for(int i=0;i<loop_cnt;i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->setGameover();
		t_sc->stopMoving();
	}
}

void CumberParent::startTeleport()
{
	mainCumber->startTeleport();
}

int CumberParent::getMainCumberSheild()
{
	return mainCumber->isSheild;
}

void CumberParent::mainCumberInvisibleOn()
{
	mainCumber->startInvisible();
}

void CumberParent::mainCumberInvisibleOff()
{
	mainCumber->stopInvisible();
}

void CumberParent::movingSubCumbers()
{
	int loop_cnt = subCumberArray->count();
	for(int i=0;i<loop_cnt;i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->startMoving();
	}
}

void CumberParent::startAutoAttacker()
{
	myMP->startAutoAttacker();
}

void CumberParent::jackCrashDie()
{
	mainCumber->showEmotion(kEmotionType_fun);
}

void CumberParent::setUI_forEP(CCObject* t_ui, SEL_CallFunc k_ui, SEL_CallFunc c_ui)
{
	myEP->set_UI(t_ui, k_ui, c_ui);
}

void CumberParent::mainCumberShowEmotion(int t_e)
{
	mainCumber->showEmotion(EmotionType(t_e));
}

void CumberParent::startDieAnimation()
{
	if(!is_die_animationing)
	{
		is_die_animationing = true;
		die_animation_cnt = 0;
		die_animation_rate = 40;
		
		if(!(SelectedMapData::sharedInstance()->getViewChapterNumber() == 1 && SelectedMapData::sharedInstance()->getSelectedStage() == 1) && rand()%CAUGHT_RATE == 0)
		{
			StarGoldData::sharedInstance()->caughtBoss();
			mainCumber->caughtBoss(this, callfunc_selector(CumberParent::realStartDieAnimation));
		}
		else
			realStartDieAnimation();
	}
}

void CumberParent::changeMaxSize(float t_p)
{
	mainCumber->changeMaxSize(t_p);
	
	int loop_cnt = subCumberArray->count();
	for(int i=0;i<loop_cnt;i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->changeMaxSize(t_p);
	}
}

void CumberParent::checkingJackCrash()
{
	mainCumber->checkingJackCrash();
}

void CumberParent::realStartDieAnimation()
{
	ccColor4F myColor;
	int my_type = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_lastSelectedElement);
	if(my_type == kMyElementalPlasma)				myColor = ccc4f(1.f, 0, 1.f, 1.f);
	else if(my_type == kMyElementalLightning)		myColor = ccc4f(1.f, 1.f, 0, 1.f);
	else if(my_type == kMyElementalWind)			myColor = ccc4f(0, 1.f, 1.f, 1.f);
	else if(my_type == kMyElementalNonElemental)	myColor = ccc4f(1.f, 1.f, 1.f, 1.f);
	else if(my_type == kMyElementalFire)			myColor = ccc4f(1.f, 0, 0, 1.f);
	else if(my_type == kMyElementalLife)			myColor = ccc4f(0, 1.f, 0, 1.f);
	else if(my_type == kMyElementalWater)			myColor = ccc4f(0, 0, 1.f, 1.f);
	
	myGD->communication("MP_explosion", mainCumber->getPosition(), myColor);
	
	mainCumber->setScale(die_animation_rate/40.f);
	mainCumber->cumberImgStartRotating(180.f*((50-die_animation_rate)/10.f));
	schedule(schedule_selector(CumberParent::dieAnimation));
}

void CumberParent::dieAnimation()
{
	die_animation_cnt++;
	
	if(die_animation_cnt >= die_animation_rate)
	{
		die_animation_cnt = 0;
		die_animation_rate -= 5;
		mainCumber->setScale(die_animation_rate/40.f);
		
		if(die_animation_rate <= 0)
		{
			unschedule(schedule_selector(CumberParent::dieAnimation));
		}
		ccColor4F myColor;
		int my_type = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_lastSelectedElement);
		if(my_type == kMyElementalPlasma)				myColor = ccc4f(1.f, 0, 1.f, 1.f);
		else if(my_type == kMyElementalLightning)		myColor = ccc4f(1.f, 1.f, 0, 1.f);
		else if(my_type == kMyElementalWind)			myColor = ccc4f(0, 1.f, 1.f, 1.f);
		else if(my_type == kMyElementalNonElemental)	myColor = ccc4f(1.f, 1.f, 1.f, 1.f);
		else if(my_type == kMyElementalFire)			myColor = ccc4f(1.f, 0, 0, 1.f);
		else if(my_type == kMyElementalLife)			myColor = ccc4f(0, 1.f, 0, 1.f);
		else if(my_type == kMyElementalWater)			myColor = ccc4f(0, 0, 1.f, 1.f);
		
		myGD->communication("MP_explosion", mainCumber->getPosition(), myColor);
		
		mainCumber->cumberImgStartRotating(180.f*((50-die_animation_rate)/10.f));
	}
}

int CumberParent::getSubCumberCount()
{
	return subCumberArray->count();
}

void CumberParent::createSubCumber(IntPoint s_p)
{
	SubCumber* t_SC = SubCumber::create(s_p);
	addChild(t_SC);
	t_SC->startMoving();
	subCumberArray->addObject(t_SC);
}

void CumberParent::initSubCumber()
{
	//		int create_cnt = SelectedMapData::sharedInstance()->getSubCumberCnt();
	int create_cnt = rand()%2+1;
	for(int i=0;i<create_cnt;i++)
	{
		SubCumber* t_SC = SubCumber::create();
		addChild(t_SC);
		subCumberArray->addObject(t_SC);
	}
}

void CumberParent::slowItem(bool t_b)
{
	mainCumber->changeSpeed(t_b);
	for(int i=0;i<subCumberArray->count();i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->changeSpeed(t_b);
	}
}

void CumberParent::silenceItem(bool t_b)
{
	if(t_b)		myMP->stopAutoAttacker();
	else		myMP->startAutoAttacker();
	mainCumber->silenceItem(t_b);
}

void CumberParent::setCasting(bool t_b)
{
	mainCumber->setCasting(t_b);
}

void CumberParent::removeSubCumber(CCObject* r_sc)
{
	subCumberArray->removeObject(r_sc);
}

void CumberParent::myInit()
{
	is_die_animationing = false;
	subCumberArray = new CCArray(1);
	isGameover = false;
	myGD = GameData::sharedGameData();
	
	myGD->V_V["CP_movingMainCumber"] = std::bind(&CumberParent::movingMainCumber, this);
	myGD->V_CCO["CP_removeSubCumber"] = std::bind(&CumberParent::removeSubCumber, this, _1);
	myGD->I_V["CP_getSubCumberCount"] = std::bind(&CumberParent::getSubCumberCount, this);
	myGD->V_Ip["CP_createSubCumber"] = std::bind(&CumberParent::createSubCumber, this, _1);
	myGD->V_I["CP_setMainCumberState"] = std::bind(&CumberParent::setMainCumberState, this, _1);
	myGD->CCN_V["CP_getMainCumberPointer"] = std::bind(&CumberParent::getMainCumberPointer, this);
	myGD->CCA_V["CP_getSubCumberArrayPointer"] = std::bind(&CumberParent::getSubCumberArrayPointer, this);
	myGD->V_CCOFF["CP_decreaseLifeForSubCumber"] = std::bind(&CumberParent::decreaseLifeForSubCumber, this, _1, _2, _3);

	myGD->V_V["CP_setGameover"] = std::bind(&CumberParent::setGameover, this);
	

	myGD->V_V["CP_tickingOn"] = std::bind(&CumberParent::tickingOn, this);
	myGD->V_V["CP_subCumberBomb"] = std::bind(&CumberParent::subCumberBomb, this);
	myGD->V_V["CP_startTeleport"] = std::bind(&CumberParent::startTeleport, this);
	myGD->V_V["CP_subCumberReplication"] = std::bind(&CumberParent::subCumberReplication, this);
	
	myGD->I_V["CP_getMainCumberSheild"] = std::bind(&CumberParent::getMainCumberSheild, this);
	myGD->V_V["CP_createAllCumberSheild"] = std::bind(&CumberParent::createAllCumberSheild, this);
	myGD->V_V["CP_mainCumberInvisibleOn"] = std::bind(&CumberParent::mainCumberInvisibleOn, this);
	myGD->V_V["CP_mainCumberInvisibleOff"] = std::bind(&CumberParent::mainCumberInvisibleOff, this);
	myGD->V_B["CP_slowItem"] = std::bind(&CumberParent::slowItem, this, _1);
	myGD->V_B["CP_silenceItem"] = std::bind(&CumberParent::silenceItem, this, _1);

	myGD->V_B["CP_setCasting"] = std::bind(&CumberParent::setCasting, this, _1);
	myGD->V_V["CP_stopMovingMainCumber"] = std::bind(&CumberParent::stopMovingMainCumber, this);
	myGD->V_V["CP_jackCrashDie"] = std::bind(&CumberParent::jackCrashDie, this);
	myGD->V_I["CP_mainCumberShowEmotion"] = std::bind(&CumberParent::mainCumberShowEmotion, this, _1);
	myGD->V_V["CP_startDieAnimation"] = std::bind(&CumberParent::startDieAnimation, this);
	myGD->V_F["CP_changeMaxSize"] = std::bind(&CumberParent::changeMaxSize, this, _1);
	myGD->V_V["CP_checkingJackCrash"] = std::bind(&CumberParent::checkingJackCrash, this);
	
	
	
	
	
	//std::function<void(int)>(&CumberParent::mainCumberShowEmotion);
	//		myGD->regCP(this, callfunc_selector(CumberParent::movingMainCumber),
	//					callfuncO_selector(CumberParent::removeSubCumber),
	//					icallfunc_selector(CumberParent::getSubCumberCount),
	//					callfuncIp_selector(CumberParent::createSubCumber),
	//					callfuncI_selector(CumberParent::setMainCumberState),
	//					ncallfunc_selector(CumberParent::getMainCumberPointer),
	//					acallfunc_selector(CumberParent::getSubCumberArrayPointer),
	//					callfuncOFF_selector(CumberParent::decreaseLifeForSubCumber),
	//					schedule_selector(CumberParent::startSpringMainCumber),
	//					callfunc_selector(CumberParent::setGameover),
	//					callfunc_selector(CumberParent::furyModeOn),
	//					callfunc_selector(CumberParent::tickingOn),
	//					callfunc_selector(CumberParent::subCumberBomb),
	//
	//					callfunc_selector(CumberParent::startTeleport),
	//					callfunc_selector(CumberParent::subCumberReplication),
	//					icallfunc_selector(CumberParent::getMainCumberSheild),
	//					callfunc_selector(CumberParent::createAllCumberSheild),
	//					callfunc_selector(CumberParent::mainCumberInvisibleOn),
	//					callfunc_selector(CumberParent::mainCumberInvisibleOff),
	//					callfuncB_selector(CumberParent::slowItem),
	//					callfuncB_selector(CumberParent::silenceItem),
	//					callfuncB_selector(CumberParent::setMovingShoot),
	//					callfuncB_selector(CumberParent::setCasting),
	//					callfunc_selector(CumberParent::stopMovingMainCumber),
	//					callfunc_selector(CumberParent::jackCrashDie),
	//					callfuncI_selector(CumberParent::mainCumberShowEmotion),
	//					callfunc_selector(CumberParent::startDieAnimation),
	//					callfuncF_selector(CumberParent::changeMaxSize),
	//					callfunc_selector(CumberParent::checkingJackCrash));
	
	mainCumber = MainCumberKS::create();
	addChild(mainCumber);
	
	initSubCumber();
	
	myMP = MissileParent::create(mainCumber->getBossEye());
	addChild(myMP);
	
	myMFP = MapFragmentParent::create();
	addChild(myMFP);
	
	myEP = EmotionParent::create(mainCumber, callfuncI_selector(MainCumber::showEmotion));
	addChild(myEP);
}

