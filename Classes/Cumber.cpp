//
//  Cumber.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 29..
//
//

#include "Cumber.h"
#include "SnakeCumber.h"
#include "Apple.h"
#include "Coconut.h"
#include "Melon.h"
#include "Banana.h"
#include "Blueberry.h"
#include "Cherry.h"
#include "Mango.h"
#include "Bear.h"
#include "Apricot.h"
#include "Grape.h"
#include "Kiwi.h"
#include "Lime.h"
#include "Orange.h"
#include "Peach.h"
#include "Pear.h"
#include "ServerDataSave.h"

void CumberParent::onStartGame()
{
	for(auto i : mainCumbers)
	{
		i->onStartGame();
	}
	
	int loop_cnt = subCumberArray->count();
	for(int i=0;i<loop_cnt;i++)
	{
		KSCumberBase* t_sc = (KSCumberBase*)subCumberArray->objectAtIndex(i);
		t_sc->onStartGame();
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
		auto mainCumberState = i->getCumberState();
		if(mainCumberState == CUMBER_STATE::CUMBERSTATEMOVING)
		{
			i->stopMoving();
		}
	}
	
	myMP->pauseSchedulerAndActions();
}



void CumberParent::tickingOn()
{
	for(auto i : mainCumbers)
	{
//		i->tickingOn(); //##
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
//		if(mainCumber->isSheild == 1)
//			mainCumber->crashSheild();
	}
	else
	{
		// RTTI
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
//	for(auto mainCumber : mainCumbers)
//		mainCumber->createSheild();
	for(int i = 0;i<subCumberArray->count();i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->createSheild();
	}
}

bool CumberParent::startDamageReaction(CCObject* cb, float damage, float angle)
{
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
//	mainCumber->startDamageReaction(userdata);
	KSCumberBase* cbp = dynamic_cast<KSCumberBase*>(cb);
	return cbp->startDamageReaction(damage, angle);
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
		Bear* t_sc = (Bear*)subCumberArray->objectAtIndex(i);
		CCPoint t_p = t_sc->getPosition();
		Bear* t_sc2 = Bear::create();
		addChild(t_sc2);
		t_sc2->setPosition(t_p);
		subCumberArray->addObject(t_sc2);
	}
}

void CumberParent::setGameover()
{
//	myEP->setGameover();
	myGD->communication("MP_stopAutoAttacker");
	isGameover = true;
	for(auto mainCumber : mainCumbers)
		mainCumber->setGameover();
	
	int loop_cnt = subCumberArray->count();
	for(int i=0;i<loop_cnt;i++)
	{
		KSCumberBase* t_sc = (KSCumberBase*)subCumberArray->objectAtIndex(i);
		t_sc->setGameover();
//		t_sc->stopMoving();
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
	return 0;
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//	return (*mainCumbers.begin())->isSheild; // 첫번 째 포인터
//	return (mainCumbers->begin()->isSh);
//	return mainCumber->isSheild;
}


void CumberParent::stopSubCumbers()
{
	int loop_cnt = subCumberArray->count();
	for(int i=0;i<loop_cnt;i++)
	{
		SubCumber* t_sc = (SubCumber*)subCumberArray->objectAtIndex(i);
		t_sc->stopMoving();
	}
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
//	for(auto mainCumber : mainCumbers)
//		mainCumber->showEmotion(kEmotionType_fun);
}

//void CumberParent::setUI_forEP(CCObject* t_ui, SEL_CallFunc k_ui, SEL_CallFunc c_ui)
//{
//	myEP->set_UI(t_ui, k_ui, c_ui);
//}

//void CumberParent::mainCumberShowEmotion(int t_e)
//{
//	//##
//	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
//	mainCumber->showEmotion(EmotionType(t_e));
//}

void CumberParent::startDieAnimation()
{
	if(!is_die_animationing)
	{
		is_die_animationing = true;
		die_animation_cnt = 0;
		die_animation_rate = 40;
		
//		if(!(SelectedMapData::sharedInstance()->getViewChapterNumber() == 1 && SelectedMapData::sharedInstance()->getSelectedStage() == 1) && rand()%CAUGHT_RATE == 0)
//		{
//			mySGD->caughtBoss();
//			for(auto mainCumber : mainCumbers)
//				mainCumber->caughtBoss(this, callfunc_selector(CumberParent::realStartDieAnimation));
//		}
//		else
			realStartDieAnimation();
	}
}

void CumberParent::changeMaxSize(float t_p)
{
	
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
	
//	for(auto mainCumber : mainCumbers)
//	{
////		mainCumber->setScale(die_animation_rate/40.f);
//		mainCumber->cumberImgStartRotating(180.f*((50-die_animation_rate)/10.f));
//	}
	schedule(schedule_selector(CumberParent::dieAnimation));
}

void CumberParent::dieAnimation()
{
	die_animation_cnt++;
	
	if(die_animation_cnt >= die_animation_rate)
	{
		die_animation_cnt = 0;
		die_animation_rate -= 5;
//		for(auto mainCumber : mainCumbers)
//			mainCumber->setScale(die_animation_rate/40.f);
		
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
//		for(auto mainCumber : mainCumbers)
//			mainCumber->cumberImgStartRotating(180.f*((50-die_animation_rate)/10.f));
	}
}

int CumberParent::getSubCumberCount()
{
	return subCumberArray->count();
}

void CumberParent::createSubCumber(IntPoint s_p)
{
	int index = m_well512.GetValue(m_juniors.size() - 1);
	jrType junior = m_juniors[index];
	//## if(junior.m_jrType)
	//## 에 따라 분기 해야됨.
	
	
	Bear* t_SC = Bear::create();
	t_SC->settingHp(junior.m_jrHp);
	t_SC->settingScale(junior.m_jrStartScale, junior.m_jrMinScale, junior.m_jrMaxScale);
	t_SC->settingSpeed(junior.m_jrStartSpeed, junior.m_jrMinSpeed, junior.m_jrMaxSpeed);
	t_SC->settingMovement((enum MOVEMENT)junior.m_jrNormalMovement, (enum MOVEMENT)junior.m_jrDrawMovement,
						  (enum MOVEMENT)junior.m_jrFuryMovement);
	addChild(t_SC);
	subCumberArray->addObject(t_SC);
	t_SC->setPosition(ip2ccp(s_p));
}

void CumberParent::initSubCumber()
{
	//		int create_cnt = SelectedMapData::sharedInstance()->getSubCumberCnt();
	
}

void CumberParent::slowItem(float ratio)
{
	for(auto mainCumber : mainCumbers)
		mainCumber->setSpeedRatio(ratio);
	
	
//	for(auto mainCumber : mainCumbers)
//		mainCumber->changeSpeed(t_b);
	for(int i=0;i<subCumberArray->count();i++)
	{
		KSCumberBase* t_sc = (KSCumberBase*)subCumberArray->objectAtIndex(i);
		t_sc->setSpeedRatio(ratio);
	}
}

void CumberParent::silenceItem(bool t_b)
{
	// 닥치라고 들어오면 true, else false;
	for(auto mainCumber : mainCumbers)
	{
		mainCumber->setSlience(t_b);
	}
	
	for(int i=0;i<subCumberArray->count();i++)
	{
		KSCumberBase* t_sc = (KSCumberBase*)subCumberArray->objectAtIndex(i);
		t_sc->setSlience(t_b);
	}
}

void CumberParent::setCasting(bool t_b)
{
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
//	mainCumber->setCasting(t_b);
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
	
//	myGD->B_CCOFF["CP_decreaseLifeForSubCumber"] = std::bind(&CumberParent::decreaseLifeForSubCumber, this, _1, _2, _3);
	
	myGD->V_V["CP_setGameover"] = std::bind(&CumberParent::setGameover, this);
	myGD->V_V["CP_tickingOn"] = std::bind(&CumberParent::tickingOn, this);
	myGD->V_V["CP_subCumberBomb"] = std::bind(&CumberParent::subCumberBomb, this);
	myGD->V_V["CP_startTeleport"] = std::bind(&CumberParent::startTeleport, this);
	myGD->V_V["CP_subCumberReplication"] = std::bind(&CumberParent::subCumberReplication, this);
	myGD->B_CCOFF["CP_startDamageReaction"] =
		std::bind(&CumberParent::startDamageReaction, this, _1, _2, _3);
	myGD->I_V["CP_getMainCumberSheild"] = std::bind(&CumberParent::getMainCumberSheild, this);
	myGD->V_V["CP_createAllCumberSheild"] = std::bind(&CumberParent::createAllCumberSheild, this);
	myGD->V_B["CP_slowItem"] = std::bind(&CumberParent::slowItem, this, _1);
	myGD->V_B["CP_silenceItem"] = std::bind(&CumberParent::silenceItem, this, _1);
//	myGD->V_V["CP_furyModeOn"] = std::bind(&CumberParent::furyModeOn, this);
//	myGD->V_B["CP_setCasting"] = std::bind(&CumberParent::setCasting, this, _1);
	myGD->V_V["CP_stopMovingMainCumber"] = std::bind(&CumberParent::stopMovingMainCumber, this);
	myGD->V_V["CP_jackCrashDie"] = std::bind(&CumberParent::jackCrashDie, this);
//	myGD->V_I["CP_mainCumberShowEmotion"] = std::bind(&CumberParent::mainCumberShowEmotion, this, _1);
	myGD->V_V["CP_startDieAnimation"] = std::bind(&CumberParent::startDieAnimation, this);
	myGD->V_F["CP_changeMaxSize"] = std::bind(&CumberParent::changeMaxSize, this, _1);
//	myGD->V_V["CP_checkingJackCrash"] = std::bind(&CumberParent::checkingJackCrash, this);
	myGD->V_V["CP_onStartGame"] = std::bind(&CumberParent::onStartGame, this);
	myGD->V_V["CP_onPatternEnd"] = std::bind(&CumberParent::onPatternEnd, this);
	myGD->V_V["CP_movingMainCumber"] = std::bind(&CumberParent::movingMainCumber, this);
	
	
	void onStartGame();
	void onPatternEnd();
	
	Json::Reader reader;
	Json::Value root;
	reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "boss"), root);
	Json::Value boss = root[0u];

	int bossType = boss["type"].asInt();
	
	float hp = boss["hp"].asInt();
	float minSpeed = boss["speed"]["min"].asDouble();// getNumberFromJsonValue(speed["max"]);
	float startSpeed = boss["speed"]["start"].asDouble(); //getNumberFromJsonValue(speed["start"]);
	float maxSpeed = boss["speed"]["max"].asDouble();// getNumberFromJsonValue(speed["min"]);
	
	float minScale = boss["scale"]["min"].asDouble(); // getNumberFromJsonValue(scale["min"]);
	float startScale = boss["scale"]["start"].asDouble(); // getNumberFromJsonValue(scale["start"]);
	float maxScale = boss["scale"]["max"].asDouble(); // getNumberFromJsonValue(scale["max"]);
	
	int normalMovement = boss["movement"]["normal"].asInt();
	int drawMovement = boss["movement"]["draw"].asInt();
	int furyMovement = boss["movement"]["fury"].asInt();
	
	if(furyMovement == 0)
	{
		furyMovement = normalMovement;
	}
	KSCumberBase* mainCumber;
	switch(bossType)
	{
		case 1:
			mainCumber = Apple::create();
			break;
		case 2:
			mainCumber = Coconut::create();
			break;
		case 3:
			mainCumber = Melon::create();
			break;
		case 4:
			mainCumber = Banana::create();
			break;
		case 5:
			mainCumber = Blueberry::create();
			break;
		case 6:
			mainCumber = Cherry::create();
			break;
		case 7:
			mainCumber = Mango::create();
			break;
		case 8:
		{

		}
			
			mainCumber = Apricot::create();
			break;
		case 9:
			mainCumber = Grape::create();
			break;
		case 10:
			mainCumber = Kiwi::create();
			break;
		case 11:
			mainCumber = Lime::create();
			break;
		case 12:
			mainCumber = Orange::create();
			break;
		case 13:
			mainCumber = Peach::create();
			break;
		case 14:
			mainCumber = Pear::create();
			break;
	}
	mainCumber->settingHp(hp);
	KS::KSLog("%", boss);
	mainCumber->settingFuryRule(boss["fury"]);
	mainCumber->settingScale(startScale, minScale, maxScale);
	mainCumber->settingSpeed(startSpeed, minSpeed, maxSpeed);
	mainCumber->settingMovement((enum MOVEMENT)normalMovement, (enum MOVEMENT)drawMovement,
								(enum MOVEMENT)furyMovement);
//	mainCumber->settingPattern(boss["pattern"]);
//	mainCumber->settingPattern("{\"test\":123");
	Json::Reader temp_reader;
	Json::Value temp_root;
//	temp_reader.parse(R"(     [{"pattern":"5", "atype":"normal","percent":2,"perframe":10,"totalframe":60,"speed":250,"numberperframe":5,"color":5},
//							 {"pattern":"103", "atype":"crash", "percent":2,"perframe":10,"totalframe":60,"speed":200,"numberperframe":5},
//							 {"pattern":"105", "atype":"special", "percent":1}]           )", temp_root);
	
	
//	temp_reader.parse(R"(     [{"pattern":"108", "atype":"normal", "oneshot":5,"oneshotterm":10,"gunnumber":4,"targettype":1,"degreev":5,"color":1,"totalframe":200,"castframe":120,"path":[0,0,0,0,50,-50,100, 50, 150, -50, 200, 50, 250, -50, 300, 50, 350, -50],"percent":10},
//										{"pattern":"103", "atype":"crash", "percent":2,"perframe":10,"totalframe":60,"speed":200,"numberperframe":5},
//										{"pattern":"105", "atype":"special", "percent":1}]           )", temp_root);
	
//	temp_reader.parse(boss["pattern"])
	mainCumber->settingPattern(boss["pattern"]);
		
//	);
	mainCumber->settingAttackPercent(boss["attackpercent"].asDouble());
	mainCumbers.push_back(mainCumber);
	addChild(mainCumber);
	
	int create_cnt;
	if(mySGD->isUsingItem(kIC_subNothing))
		return;
	if(mySD->getClearCondition() != kCLEAR_subCumberCatch)
	{
		 create_cnt = m_well512.GetValue(1, 2);
	}
	else
	{
		create_cnt = 2;
	}
	for(int i=0;i<create_cnt;i++)
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior"), root);
		
		
		for(int i=0; i<root.size(); i++)
		{
			Json::Value boss = root[i];
			KS::KSLog("%", boss);
			int bossType = boss["type"].asInt();
			
			float hp = boss["hp"].asInt();
			float minSpeed = boss["speed"]["min"].asDouble();// getNumberFromJsonValue(speed["max"]);
			float startSpeed = boss["speed"]["start"].asDouble(); //getNumberFromJsonValue(speed["start"]);
			float maxSpeed = boss["speed"]["max"].asDouble();// getNumberFromJsonValue(speed["min"]);
			
			float minScale = boss["scale"]["min"].asDouble(); // getNumberFromJsonValue(scale["min"]);
			float startScale = boss["scale"]["start"].asDouble(); // getNumberFromJsonValue(scale["start"]);
			float maxScale = boss["scale"]["max"].asDouble(); // getNumberFromJsonValue(scale["max"]);
			
			int normalMovement = boss["movement"]["normal"].asInt();
			int drawMovement = boss["movement"]["draw"].asInt();
			int furyMovement = boss["movement"]["fury"].asInt();
			
			if(furyMovement == 0)
			{
				furyMovement = normalMovement;
			}
			//## 여기서 부하몹 분기가 들어감...
			//## 지금은 그냥 Bear 가 부하임.
			jrType jt(bossType, minSpeed, startSpeed, maxSpeed, minScale, startScale, maxScale, normalMovement,
					  drawMovement, furyMovement, hp);
			m_juniors.push_back(jt);
			
			Bear* t_SC = Bear::create();
			t_SC->settingHp(hp);
			t_SC->settingScale(startScale, minScale, maxScale);
			t_SC->settingSpeed(startSpeed, minSpeed, maxSpeed);
			t_SC->settingMovement((enum MOVEMENT)normalMovement, (enum MOVEMENT)drawMovement
								  , (enum MOVEMENT)furyMovement);
			addChild(t_SC);
			
			subCumberArray->addObject(t_SC);

		}
	}
//	initSubCumber();
	
	myMP = MissileParent::create(mainCumber->getBossEye());
	addChild(myMP);
	
	myMFP = MapFragmentParent::create();
	addChild(myMFP);
	
//	myEP = EmotionParent::create(mainCumber, callfuncI_selector(KSCumberBase::showEmotion));
//	addChild(myEP);
}

