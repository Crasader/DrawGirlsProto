//
//  Cumber.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 29..
//
//

#include "Cumber.h"
#include "SnakeCumber.h"
#include "MetalSnake.h"
#include "KSCumber.h"
void CumberParent::onStartGame()
{
	for(auto i : mainCumbers)
	{
		i->onStartGame();
	}
}
void CumberParent::onPatternEnd()
{
	for(auto i : mainCumbers)
	{
		i->onPatternEnd();
	}
}
void CumberParent::setMainCumberState(int t_cs)
{
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	
	// 일단 제거 시키자.
	// 근본적으로 "CP_setMainCumberState" 자체를 없애야 하겠지만,
	// 몬스터의 상태 변화는 몬스터 클래스가 정하자.
	// 이렇게 하는 쪽이 구조상 유연하지 않을까 생각해본다.
	
	
//	for(auto i : mainCumbers)
//		i->setCumberState(t_cs);
}

void CumberParent::allStopSchedule()
{
	//##
	for(auto i : mainCumbers)
	{
		cumberState mainCumberState = i->getCumberState();
		if(mainCumberState == cumberStateMoving)
			i->stopMoving();
		else
			myMP->pauseSchedulerAndActions();
	}
}

void CumberParent::furyModeOn()
{
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
	mainCumber->furyModeOn();
}

void CumberParent::tickingOn()
{
	for(auto i : mainCumbers)
	{
		i->tickingOn();
	}
}

void CumberParent::movingMainCumber()
{
	if(!isGameover)
	{
		for(auto i : mainCumbers)
			i->startMoving();
	}
}

void CumberParent::stopMovingMainCumber()
{
	for(auto mainCumber : mainCumbers)
		mainCumber->stopMoving();
}

CCNode* CumberParent::getMainCumberPointer()
{
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	return *mainCumbers.begin();
//	return mainCumber;
}

CCArray* CumberParent::getSubCumberArrayPointer()
{
	return subCumberArray;
}

void CumberParent::decreaseLifeForSubCumber(CCObject* target, float t_damage, float t_directionAngle)
{
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
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
	for(auto mainCumber : mainCumbers)
		mainCumber->createSheild();
	for(int i = 0;i<subCumberArray->count();i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->createSheild();
	}
}

void CumberParent::startDamageReaction(float userdata)
{
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
	mainCumber->startDamageReaction(userdata);
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
	for(auto mainCumber : mainCumbers)
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
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	for(auto mainCumber : mainCumbers)
		mainCumber->startTeleport();
}

int CumberParent::getMainCumberSheild()
{
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	return (*mainCumbers.begin())->isSheild; // 첫번 째 포인터
//	return (mainCumbers->begin()->isSh);
//	return mainCumber->isSheild;
}

void CumberParent::mainCumberInvisibleOn()
{
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
	mainCumber->startInvisible();
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
	for(auto mainCumber : mainCumbers)
		mainCumber->showEmotion(kEmotionType_fun);
}

void CumberParent::setUI_forEP(CCObject* t_ui, SEL_CallFunc k_ui, SEL_CallFunc c_ui)
{
	myEP->set_UI(t_ui, k_ui, c_ui);
}

void CumberParent::mainCumberShowEmotion(int t_e)
{
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
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
			mySGD->caughtBoss();
			for(auto mainCumber : mainCumbers)
				mainCumber->caughtBoss(this, callfunc_selector(CumberParent::realStartDieAnimation));
		}
		else
			realStartDieAnimation();
	}
}

void CumberParent::changeMaxSize(float t_p)
{
	for(auto mainCumber : mainCumbers)
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
	for(auto mainCumber : mainCumbers)
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
	
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
	myGD->communication("MP_explosion", mainCumber->getPosition(), myColor);
	
	for(auto mainCumber : mainCumbers)
	{
		mainCumber->setScale(die_animation_rate/40.f);
		mainCumber->cumberImgStartRotating(180.f*((50-die_animation_rate)/10.f));
	}
	schedule(schedule_selector(CumberParent::dieAnimation));
}

void CumberParent::dieAnimation()
{
	die_animation_cnt++;
	
	if(die_animation_cnt >= die_animation_rate)
	{
		die_animation_cnt = 0;
		die_animation_rate -= 5;
		for(auto mainCumber : mainCumbers)
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
		//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
		auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
		myGD->communication("MP_explosion", mainCumber->getPosition(), myColor);
		for(auto mainCumber : mainCumbers)
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
	if(mySD->getClearCondition() != kCLEAR_subCumberCatch)
	{
		int create_cnt = rand()%2+1;
		for(int i=0;i<create_cnt;i++)
		{
			SubCumber* t_SC = SubCumber::create();
			addChild(t_SC);
			subCumberArray->addObject(t_SC);
		}
	}
	else
	{
		for(int i=0;i<2;i++)
		{
			SubCumber* t_SC = SubCumber::create();
			addChild(t_SC);
			subCumberArray->addObject(t_SC);
		}
	}
}

void CumberParent::slowItem(bool t_b)
{
	for(auto mainCumber : mainCumbers)
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
	
	for(auto mainCumber : mainCumbers)
		mainCumber->silenceItem(t_b);
}

void CumberParent::setCasting(bool t_b)
{
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
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
	myGD->V_F["CP_startDamageReaction"] = std::bind(&CumberParent::startDamageReaction, this, _1);
	myGD->I_V["CP_getMainCumberSheild"] = std::bind(&CumberParent::getMainCumberSheild, this);
	myGD->V_V["CP_createAllCumberSheild"] = std::bind(&CumberParent::createAllCumberSheild, this);
	myGD->V_V["CP_mainCumberInvisibleOn"] = std::bind(&CumberParent::mainCumberInvisibleOn, this);
	myGD->V_B["CP_slowItem"] = std::bind(&CumberParent::slowItem, this, _1);
	myGD->V_B["CP_silenceItem"] = std::bind(&CumberParent::silenceItem, this, _1);
	myGD->V_V["CP_furyModeOn"] = std::bind(&CumberParent::furyModeOn, this);
	myGD->V_B["CP_setCasting"] = std::bind(&CumberParent::setCasting, this, _1);
	myGD->V_V["CP_stopMovingMainCumber"] = std::bind(&CumberParent::stopMovingMainCumber, this);
	myGD->V_V["CP_jackCrashDie"] = std::bind(&CumberParent::jackCrashDie, this);
	myGD->V_I["CP_mainCumberShowEmotion"] = std::bind(&CumberParent::mainCumberShowEmotion, this, _1);
	myGD->V_V["CP_startDieAnimation"] = std::bind(&CumberParent::startDieAnimation, this);
	myGD->V_F["CP_changeMaxSize"] = std::bind(&CumberParent::changeMaxSize, this, _1);
	myGD->V_V["CP_checkingJackCrash"] = std::bind(&CumberParent::checkingJackCrash, this);
	myGD->V_V["CP_onStartGame"] = std::bind(&CumberParent::onStartGame, this);
	myGD->V_V["CP_onPatternEnd"] = std::bind(&CumberParent::onPatternEnd, this);
	
	
	void onStartGame();
	void onPatternEnd();
//	auto mainCumber = MainCumber::create();
	auto mainCumber = MetalSnake::create();
	mainCumbers.push_back(mainCumber);
	addChild(mainCumber);
	
	initSubCumber();
	
	myMP = MissileParent::create(mainCumber->getBossEye());
	addChild(myMP);
	
	myMFP = MapFragmentParent::create();
	addChild(myMFP);
	
	myEP = EmotionParent::create(mainCumber, callfuncI_selector(MainCumber::showEmotion));
	addChild(myEP);
}

