//
//  Cumber.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 29..
//
//

#include "Cumber.h"
#include "SnakeCumber.h"
#include "ServerDataSave.h"
#include "KSJuniorBase.h"
#include "KSCircleBase.h"
#include "KSSnakeBase.h"
#include <functional>
#include <memory>
#include "CumberEmotion.h"
#include "AchieveNoti.h"


void CumberParent::onStartGame()
{
	for(auto i : mainCumbers)
	{
		i->onStartGame();
	}
	
	for(auto i : subCumberArray)
	{
		i->onStartGame();
	}
}
void CumberParent::onPatternEndOf(CCObject* cb)
{
	
	((KSCumberBase*)cb)->onPatternEnd();
	
	
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

void CumberParent::onJackDrawLine()
{
	for(auto mainCumber : mainCumbers)
		mainCumber->onJackDrawLine();
	
	for(auto sub : subCumberArray)
	{
		sub->onJackDrawLine();
	}	
	
	
}
void CumberParent::allStopSchedule()
{
	//##
	for(auto i : mainCumbers)
	{
		auto mainCumberState = i->getCumberState();
		if(mainCumberState & kCumberStateMoving)
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

//CCNode* CumberParent::getMainCumberPointer()
//{
//	////### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//return *mainCumbers.begin();
//////	return mainCumber;
//}

std::vector<KSCumberBase*>& CumberParent::getSubCumberArrayPointer()
{
	return subCumberArray;
}


std::vector<KSCumberBase*>& CumberParent::getMainCumbers()
{
	return mainCumbers;
}





bool CumberParent::startDamageReaction(CCObject* cb, float damage, float angle, bool castCancel, bool stiffen)
{
//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
//	mainCumber->startDamageReaction(userdata);
	KSCumberBase* cbp = dynamic_cast<KSCumberBase*>(cb);
	
	total_damage_to_gold = total_damage_to_gold.getV() + damage;
	
	float total_damage = total_damage_to_gold.getV();
	float max_life = mainCumbers[0]->getTotalLife();
	float rate;
	float d_damage;
	
	if(mySGD->is_endless_mode || mySGD->is_hell_mode)
		rate = 0.01f;
	else
		rate = ((mySGD->getUserdataHighPiece()+10)/(mySD->getSilType()+10)*9.f+1.f)/100.f; // 0.01f ~ 0.1f
	
	d_damage = max_life*rate;
	
	int gold_count = int(total_damage/d_damage);
	
	total_damage_to_gold = total_damage_to_gold.getV() - gold_count*d_damage;
	
	if(gold_count > 2)
		gold_count = 2;
	
	myGD->communication("GIM_showAttackFloatingCoin", cbp->getPosition(), gold_count);
	
	mySGD->stage_attack_count = mySGD->stage_attack_count.getV() + 1;
	
	for(int i=kAchievementCode_attacker1;i<=kAchievementCode_attacker3;i++)
	{
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
		   mySGD->stage_attack_count.getV() >= myAchieve->getCondition((AchievementCode)i))
		{
			myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	
	return cbp->startDamageReaction(damage, angle, castCancel, stiffen);
}


void CumberParent::subCumberReplication()
{
	// 미구현.
	//int cnt = subCumberArray->count();
	//for(int i = 0;i<cnt;i++)
	//{
		//KSCumberBase* t_sc = (KSJuniorBase*)subCumberArray->objectAtIndex(i);
		//CCPoint t_p = t_sc->getPosition();
		//KSJuniorBase* t_sc2 = KSJuniorBase::create("bear");
		//addChild(t_sc2);
		//t_sc2->setPosition(t_p);
		//subCumberArray->addObject(t_sc2);
	//}
}

void CumberParent::setGameover()
{
//	myEP->setGameover();
//	myGD->communication("MP_stopAutoAttacker");
	isGameover = true;
	for(auto mainCumber : mainCumbers)
		mainCumber->setGameover();
	
	for(auto subCumber : subCumberArray)
	{
		subCumber->setGameover();
	}
	
	for(int i=0;i<hp_graphs.size();i++)
	{
		hp_graphs[i]->setGameover();
	}
}

void CumberParent::startTeleport(CCObject* cumber)
{
	//##
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
	((KSCumberBase*)cumber)->startTeleport();
//	for(auto mainCumber : mainCumbers)
//		mainCumber->startTeleport();
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




void CumberParent::startAutoAttacker()
{

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
		
		realStartDieAnimation();
	}
}

void CumberParent::changeMaxSize(float t_p)
{
	
}



void CumberParent::realStartDieAnimation()
{
//	ccColor4F myColor;
//	int my_type = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_lastSelectedElement);
//	if(my_type == kMyElementalPlasma)				myColor = ccc4f(1.f, 0, 1.f, 1.f);
//	else if(my_type == kMyElementalLightning)		myColor = ccc4f(1.f, 1.f, 0, 1.f);
//	else if(my_type == kMyElementalWind)			myColor = ccc4f(0, 1.f, 1.f, 1.f);
//	else if(my_type == kMyElementalNonElemental)	myColor = ccc4f(1.f, 1.f, 1.f, 1.f);
//	else if(my_type == kMyElementalFire)			myColor = ccc4f(1.f, 0, 0, 1.f);
//	else if(my_type == kMyElementalLife)			myColor = ccc4f(0, 1.f, 0, 1.f);
//	else if(my_type == kMyElementalWater)			myColor = ccc4f(0, 0, 1.f, 1.f);
	
	//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//	auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
//	myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 0, 0, 0), rand()%360-180.f);
	
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
//		ccColor4F myColor;
//		int my_type = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_lastSelectedElement);
//		if(my_type == kMyElementalPlasma)				myColor = ccc4f(1.f, 0, 1.f, 1.f);
//		else if(my_type == kMyElementalLightning)		myColor = ccc4f(1.f, 1.f, 0, 1.f);
//		else if(my_type == kMyElementalWind)			myColor = ccc4f(0, 1.f, 1.f, 1.f);
//		else if(my_type == kMyElementalNonElemental)	myColor = ccc4f(1.f, 1.f, 1.f, 1.f);
//		else if(my_type == kMyElementalFire)			myColor = ccc4f(1.f, 0, 0, 1.f);
//		else if(my_type == kMyElementalLife)			myColor = ccc4f(0, 1.f, 0, 1.f);
//		else if(my_type == kMyElementalWater)			myColor = ccc4f(0, 0, 1.f, 1.f);
		//### : !@#!@#!@#!@#!#!@#!@#!@#!@#!@#!@#!@#!#@#!#@ 논란
//		auto mainCumber = *mainCumbers.begin(); // 첫번 째 포인터로 일단 판단
//		myGD->communication("MP_explosion", mainCumber->getPosition(), ccc4f(0, 0, 0, 0), rand()%360-180.f);
//		for(auto mainCumber : mainCumbers)
//			mainCumber->cumberImgStartRotating(180.f*((50-die_animation_rate)/10.f));
	}
}

void CumberParent::onJackDie()
{
	for(auto mainCumber : mainCumbers)
		mainCumber->onJackDie();
	
	for(auto subCumber : subCumberArray)
		subCumber->onJackDie();
}
void CumberParent::onJackRevived()
{
	for(auto mainCumber : mainCumbers)
		mainCumber->onJackRevived();
	
	for(auto subCumber : subCumberArray)
		subCumber->onJackRevived();
}

void CumberParent::changePassiveData(const std::string& passive_data)
{
	for(auto mainCumber : mainCumbers)
	{
		mainCumber->restoreBossData();
		mainCumber->applyPassiveData(passive_data);
		mainCumber->applyAutoBalance();
	}
	for(auto subCumber : subCumberArray)
	{
		subCumber->restoreBossData();
		subCumber->applyPassiveData(passive_data);
		subCumber->applyAutoBalance();
	}
}
int CumberParent::getSubCumberCount()
{
	return subCumberArray.size();
}

void CumberParent::createSubCumber(IntPoint s_p)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior"), root);
	int index = m_well512.GetValue(root.size() - 1);
	{
		int i = index;
		Json::Value boss = root[i];
		KS::KSLog("%", boss);
		std::string bossType = boss["type"].asString();
		
		KSJuniorBase* t_SC = KSJuniorBase::create(bossType);
		t_SC->assignBossData(root[i]); // 주니어 정보 대입.
		t_SC->applyPassiveData(mySD->getPassiveData());
		t_SC->applyAutoBalance();
		t_SC->startAnimationNoDirection();
		t_SC->onStartGame();
		t_SC->setScale(0.1);
		addChild(t_SC);
		subCumberArray.push_back(t_SC);	
		t_SC->setPosition(ip2ccp(s_p));
		
		MobHpGraph* t_sub_hp = MobHpGraph::create(t_SC, "junior_hp_bar.png");
		addChild(t_sub_hp);
		hp_graphs.push_back(t_sub_hp);
		
//		addChild(KSGradualValue<float>::create(0.1f, 1.0f, 0.5f, [=](float d){
//			t_SC->setScale(d);
//		}, [=](float d){
//			t_SC->setScale(1);
//		}));
	}
}

void CumberParent::initSubCumber()
{
	
}

void CumberParent::slowItem(float ratio)
{
	for(auto mainCumber : mainCumbers)
		mainCumber->setSpeedRatio(ratio);
	
	
//	for(auto mainCumber : mainCumbers)
//		mainCumber->changeSpeed(t_b);
	for(auto mainCumber : mainCumbers)
		mainCumber->setSpeedRatio(ratio);
}

void CumberParent::silenceItem(bool t_b)
{
	// 닥치라고 들어오면 true, else false;
	for(auto mainCumber : mainCumbers)
	{
		mainCumber->setSlience(t_b);
	}
	
//	for(auto i : subCumberArray)
//	{
//		i->setSlience(t_b);
//	}	
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
	auto iter = std::find(subCumberArray.begin(), subCumberArray.end(), r_sc);
	if(iter != subCumberArray.end())
	{
		subCumberArray.erase(iter);
	}
	
	for(auto iter = hp_graphs.begin();iter!=hp_graphs.end();++iter)
	{
		if((*iter)->getTargetNode() == r_sc)
		{
			removeChild(*iter);
			hp_graphs.erase(iter);
			break;
		}
	}
	
//	auto beginIter = std::remove_if(hp_graphs.begin(), hp_graphs.end(), [=](MobHpGraph* mg)
//				   {
//					   return mg->getTargetNode() == r_sc;
//				   });
//	for(auto iter = beginIter; iter != hp_graphs.end(); ++iter)
//		{
//			removeChild(*iter);
//		}
//	
//	hp_graphs.erase(beginIter, hp_graphs.end());
	
//	bool is_found = false;
//	for(int i=0;i<hp_graphs.size() && !is_found;i++)
//	{
//		MobHpGraph* t_hp = hp_graphs[i];
//		if(t_hp->getTargetNode() == r_sc)
//		{
//			is_found = true;
//			hp_graphs.erase(std::remove_if(hp_graphs.begin(), hp_graphs.end(), [=](MobHpGraph* mg)
//										   {
//												return ;
//										   }), hp_graphs.end());
//			removeChild(t_hp);
//		}
//	}
}
void CumberParent::removeMainCumber(CCObject* r_sc)
{
	auto iter = std::find(mainCumbers.begin(), mainCumbers.end(), r_sc);
	if(iter != mainCumbers.end())
	{
		mainCumbers.erase(iter);
	}
	
	for(auto iter = hp_graphs.begin();iter!=hp_graphs.end();++iter)
	{
		if((*iter)->getTargetNode() == r_sc)
		{
			removeChild(*iter);
			hp_graphs.erase(iter);
			break;
		}
	}
}


void CumberParent::myInit()
{
	myGD->getCumberParent = [=]()->CumberParent*{
		this;
	};
	is_die_animationing = false;
	isGameover = false;
	
	total_damage_to_gold = 0;
	
	Json::Reader reader;
	Json::Value root;
	reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "boss"), root);

	mappingFunctor();
	for (unsigned int i = 0; i < root.size(); ++i)
	{
		Json::Value boss = root[i];

		std::string bossShape = boss.get("shape", "circle").asString();
		std::string bossType = boss["type"].asString();


		KSCumberBase* mainCumber;
		if(bossShape == "circle")
		{
			mainCumber = KSCircleBase::create(bossType);
		}
		else if(bossShape == "snake")
		{
			mainCumber = KSSnakeBase::create(bossType);
		}	
		mainCumber->attachEmotion();
		mainCumber->assignBossData(root[i]);
		mainCumber->applyPassiveData(mySD->getPassiveData());
		mainCumber->settingAttackPercent(boss["attackpercent"].asDouble());
		mainCumber->settingPattern(boss["pattern"]);
		mainCumber->applyAutoBalance();
		mainCumber->applyDisableOfCharacter();
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

		//	);
		IntPoint mapPoint;
		bool finded;
		mainCumber->getRandomPosition(&mapPoint, &finded);
		myGD->setMainCumberPoint(mainCumber, mapPoint);
		mainCumber->setPosition(ip2ccp(mapPoint));
		mainCumber->m_circle.setRelocation(getPosition(), m_well512);
		mainCumber->m_snake.setRelocation(getPosition(), m_well512);
		mainCumber->startAnimationNoDirection();
		mainCumbers.push_back(mainCumber);
		addChild(mainCumber);

		// 에너지 달음.
		if(NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i) == kCLEAR_bossLifeZero)
		{
			MobHpGraph* main_hp = MobHpGraph::create(mainCumber, "junior_hp_bar.png");
			addChild(main_hp);
			main_hp->setMinimumRate();
			hp_graphs.push_back(main_hp);
		}
	}	
	
//	int create_cnt;
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior"), root);
		KS::KSLog("%", root);
		
		for(int i=0; i<root.size(); i++)
		{
			Json::Value boss = root[i];
			KS::KSLog("%", boss);
			std::string bossType = boss["type"].asString();
			
			KSJuniorBase* t_SC = KSJuniorBase::create(bossType);
			t_SC->assignBossData(root[i]); // 주니어 정보 대입.
			t_SC->applyPassiveData(mySD->getPassiveData());
			t_SC->applyAutoBalance();
			IntPoint mapPoint;
			bool finded;
			t_SC->getRandomPosition(&mapPoint, &finded);
			t_SC->setPosition(ip2ccp(mapPoint));
			t_SC->startAnimationNoDirection();
			addChild(t_SC);
			
			subCumberArray.push_back(t_SC);
		}
	}
//	initSubCumber();
	
	//myMP = MissileParent::create(mainCumber->getBossEye());
	myMP = MissileParent::create(nullptr);
	addChild(myMP, 1);
	
	myMFP = MapFragmentParent::create();
	addChild(myMFP);
	
	for(int i=0;i<subCumberArray.size(); i++)
	{
		MobHpGraph* t_sub_hp = MobHpGraph::create(subCumberArray[i], "junior_hp_bar.png");
		addChild(t_sub_hp);
		hp_graphs.push_back(t_sub_hp);
	}
	
	
//	myEP = EmotionParent::create(mainCumber, callfuncI_selector(KSCumberBase::showEmotion));
//	addChild(myEP);
}

void CumberParent::mappingFunctor()
{
	myGD->V_V["CP_movingMainCumber"] = std::bind(&CumberParent::movingMainCumber, this);
	myGD->V_V["CP_onJackDrawLine"] = std::bind(&CumberParent::onJackDrawLine, this);
	myGD->V_CCO["CP_removeSubCumber"] = std::bind(&CumberParent::removeSubCumber, this, _1);
	myGD->V_CCO["CP_removeMainCumber"] = std::bind(&CumberParent::removeMainCumber, this, _1);
	myGD->I_V["CP_getSubCumberCount"] = std::bind(&CumberParent::getSubCumberCount, this);
	myGD->V_Ip["CP_createSubCumber"] = std::bind(&CumberParent::createSubCumber, this, _1);
	myGD->V_I["CP_setMainCumberState"] = std::bind(&CumberParent::setMainCumberState, this, _1);
//	myGD->CCN_V["CP_getMainCumberPointer"] = std::bind(&CumberParent::getMainCumberPointer, this);
	//myGD->CCA_V["CP_getSubCumberArrayPointer"] = std::bind(&CumberParent::getSubCumberArrayPointer, this);
	myGD->getMainCumberVector = std::bind(&CumberParent::getMainCumbers, this);
	myGD->getSubCumberVector = std::bind(&CumberParent::getSubCumberArrayPointer, this);
//	myGD->B_CCOFF["CP_decreaseLifeForSubCumber"] = std::bind(&CumberParent::decreaseLifeForSubCumber, this, _1, _2, _3);
	
	myGD->V_V["CP_setGameover"] = std::bind(&CumberParent::setGameover, this);
	myGD->V_V["CP_tickingOn"] = std::bind(&CumberParent::tickingOn, this);
	myGD->V_CCO["CP_startTeleport"] = std::bind(&CumberParent::startTeleport, this, _1);
	myGD->V_V["CP_subCumberReplication"] = std::bind(&CumberParent::subCumberReplication, this);
	myGD->B_CCOFFBB["CP_startDamageReaction"] =
		std::bind(&CumberParent::startDamageReaction, this, _1, _2, _3, _4, _5);
	myGD->I_V["CP_getMainCumberSheild"] = std::bind(&CumberParent::getMainCumberSheild, this);

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
	//myGD->V_V["CP_onPatternEnd"]= std::bind(&CumberParent::onPatternEnd, this);
	myGD->V_CCO["CP_onPatternEndOf"] = std::bind(&CumberParent::onPatternEndOf, this, _1);
	myGD->V_V["CP_movingMainCumber"] = std::bind(&CumberParent::movingMainCumber, this);
	myGD->V_V["CP_onJackDie"] = std::bind(&CumberParent::onJackDie, this);
	myGD->V_V["CP_onJackRevived"] = std::bind(&CumberParent::onJackRevived, this);
	myGD->V_Str["CP_chagePassiveData"] = std::bind(&CumberParent::changePassiveData, this, _1);
	myGD->hideBosses = std::bind(&CumberParent::hideBosses, this);
	myGD->toFun = std::bind(&CumberParent::toFun, this);
	myGD->toScratch = std::bind(&CumberParent::toScratch, this);
}
void CumberParent::toFun()
{
	for(auto mainCumber : mainCumbers)
	{
		mainCumber->getEmotion()->onKillingJack(); // 비웃음.
	}
}
void CumberParent::toScratch()
{
	for(auto mainCumber : mainCumbers)
	{
		mainCumber->getEmotion()->toScratch(); // 긁적긁적
	}
}
void CumberParent::hideBosses()
{
	for(auto mainCumber : mainCumbers)
	{
		mainCumber->setVisible(false);
		mainCumber->onExit();
		mainCumber->getPoisonedNiddle()->removeFromParent();
		mainCumber->setPoisonedNiddle(nullptr);
//		mainCumber->stopAllActions();
//		mainCumber->unscheduleAllSelectors();
	}
	AudioEngine::sharedInstance()->stopEffect("se_missile.mp3");
}
