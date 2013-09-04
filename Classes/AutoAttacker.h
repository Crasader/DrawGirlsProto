//
//  AutoAttacker.h
//  DrawingJack
//
//  Created by 사원3 on 13. 2. 18..
//
//

#ifndef DrawingJack_AutoAttacker_h
#define DrawingJack_AutoAttacker_h

#include "cocos2d.h"
#include "SelectedMapData.h"
#include "AttackPattern.h"
#include "GameData.h"
#include "BossEye.h"

using namespace cocos2d;

class AutoAttacker : public CCNode
{
public:

	virtual void stopFraming()
	{
		
	}
	
	virtual void startFraming()
	{
		
	}
};

class AutoATK1 : public AutoAttacker
{
public:
	static AutoATK1* create()
	{
		AutoATK1* t_atk1 = new AutoATK1();
		t_atk1->myInit();
		t_atk1->autorelease();
		return t_atk1;
	}
	
	virtual ~AutoATK1()
	{
		delete my_balance;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK1::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK1::framing));
	}
	
private:
	BD_P2* my_balance;
	int shoot_frame;
	int ing_frame;
	int chapter_missile_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
			
			CCPoint subPosition = ccpSub(jackPosition, mainCumberPosition);
			
			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			if(distance >= 80)
			{
				int m_img_rand_type = rand()%2 + 1;
				AP_Missile2* t_m2 = AP_Missile2::create(mainCumberPosition, my_balance->mCnt, my_balance->move_speed,
														CCString::createWithFormat("chapter%d_basic_missile_%d.png", chapter_missile_number, m_img_rand_type)->getCString(), my_balance->crash_area);
				addChild(t_m2);
			}
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		my_balance = new BD_P2(1.f, 10, CCSize(6.f,6.f));
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		chapter_missile_number = SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile);
		if(selected_chapter == 1 || selected_chapter == 11)
		{
			if(selected_stage == 1)			shoot_frame = 240;
			else if(selected_stage == 2)	shoot_frame = 230;
			else if(selected_stage == 3)	shoot_frame = 220;
			else if(selected_stage == 4)	shoot_frame = 240;
			else if(selected_stage == 5)	shoot_frame = 260;
		}
		else if(selected_chapter == 2 || selected_chapter == 12)
		{
			if(selected_stage == 1)			shoot_frame = 280;
			else if(selected_stage == 2)	shoot_frame = 300;
			else if(selected_stage == 3)	shoot_frame = 320;
			else if(selected_stage == 4)	shoot_frame = 340;
			else if(selected_stage == 5)	shoot_frame = 360;
		}
	}
};

class AutoATK2 : public AutoAttacker
{
public:
	static AutoATK2* create()
	{
		AutoATK2* t_atk2 = new AutoATK2();
		t_atk2->myInit();
		t_atk2->autorelease();
		return t_atk2;
	}
	
	virtual ~AutoATK2()
	{
		delete my_balance;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK2::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		ing_angle = rand()%360;
		schedule(schedule_selector(AutoATK2::framing));
	}
	
private:
	BD_P2* my_balance;
	int shoot_frame;
	int ing_frame;
	int ing_angle;
	int chapter_missile_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
			
			CCPoint subPosition = ccpSub(jackPosition, mainCumberPosition);
			
			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			if(distance >= 80)
			{
				int m_img_rand_type = rand()%2 + 1;
				
				ing_angle = (ing_angle+36)%360;
				MissileUnit* t_mu = MissileUnit::create(mainCumberPosition, ing_angle, my_balance->move_speed,
														CCString::createWithFormat("chapter%d_basic_missile_%d.png", chapter_missile_number, m_img_rand_type)->getCString(), my_balance->crash_area, 0.f, 0.f);
				addChild(t_mu);
			}
			
			ing_frame = 0;
		}
	}

	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		my_balance = new BD_P2(1.f, 36, CCSize(6.f,6.f));
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		chapter_missile_number = SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile);
		if(selected_chapter == 3 || selected_chapter == 13)
		{
			if(selected_stage == 1)			shoot_frame = 30;
			else if(selected_stage == 2)	shoot_frame = 33;
			else if(selected_stage == 3)	shoot_frame = 32;
			else if(selected_stage == 4)	shoot_frame = 35;
			else if(selected_stage == 5)	shoot_frame = 38;
		}
		else if(selected_chapter == 4 || selected_chapter == 14)
		{
			if(selected_stage == 1)			shoot_frame = 35;
			else if(selected_stage == 2)	shoot_frame = 40;
			else if(selected_stage == 3)	shoot_frame = 38;
			else if(selected_stage == 4)	shoot_frame = 40;
			else if(selected_stage == 5)	shoot_frame = 43;
		}
	}
};

class AutoATK3 : public AutoAttacker
{
public:
	static AutoATK3* create()
	{
		AutoATK3* t_atk3 = new AutoATK3();
		t_atk3->myInit();
		t_atk3->autorelease();
		return t_atk3;
	}
	
	virtual ~AutoATK3()
	{
		delete my_balance;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK3::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK3::framing));
	}
	
private:
	BD_P2* my_balance;
	int shoot_frame;
	int ing_frame;
	int chapter_missile_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
			
			CCPoint subPosition = ccpSub(jackPosition, mainCumberPosition);
			
			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			if(distance >= 80)
			{
				int m_img_rand_type = rand()%2 + 1;
				
				float angle = atan2f(subPosition.y, subPosition.x)/M_PI*180.f;
				
				MissileUnit* t_mu = MissileUnit::create(mainCumberPosition, angle, my_balance->move_speed,
														CCString::createWithFormat("chapter%d_basic_missile_%d.png", chapter_missile_number, m_img_rand_type)->getCString(), my_balance->crash_area, 0.f, 0.f);
				addChild(t_mu);
			}
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		my_balance = new BD_P2(1.2f, 0, CCSize(6.f,6.f));
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		chapter_missile_number = SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile);
		if(selected_chapter == 5 || selected_chapter == 15)
		{
			if(selected_stage == 1)			shoot_frame = 120;
			else if(selected_stage == 2)	shoot_frame = 125;
			else if(selected_stage == 3)	shoot_frame = 130;
			else if(selected_stage == 4)	shoot_frame = 135;
			else if(selected_stage == 5)	shoot_frame = 140;
		}
		else if(selected_chapter == 6 || selected_chapter == 16)
		{
			if(selected_stage == 1)			shoot_frame = 145;
			else if(selected_stage == 2)	shoot_frame = 150;
			else if(selected_stage == 3)	shoot_frame = 155;
			else if(selected_stage == 4)	shoot_frame = 160;
			else if(selected_stage == 5)	shoot_frame = 165;
		}
	}
};

class AutoATK4 : public AutoAttacker
{
public:
	static AutoATK4* create()
	{
		AutoATK4* t_atk4 = new AutoATK4();
		t_atk4->myInit();
		t_atk4->autorelease();
		return t_atk4;
	}
	
	virtual ~AutoATK4()
	{
		delete my_balance;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK4::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK4::framing));
	}
	
private:
	BD_P2* my_balance;
	int shoot_frame;
	int ing_frame;
	int chapter_missile_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
			
			CCPoint subPosition = ccpSub(jackPosition, mainCumberPosition);
			
			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			if(distance >= 80)
			{
				int m_img_rand_type = rand()%2 + 1;
				AP_Missile5* t_m5 = AP_Missile5::create(mainCumberPosition, my_balance->mCnt, my_balance->move_speed,
														CCString::createWithFormat("chapter%d_basic_missile_%d.png", chapter_missile_number, m_img_rand_type)->getCString(), my_balance->crash_area, 1.5f, 0.995f);
				addChild(t_m5);
			}
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		my_balance = new BD_P2(1.f, 10, CCSize(6.f,6.f));
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		chapter_missile_number = SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile);
		if(selected_chapter == 7 || selected_chapter == 17)
		{
			if(selected_stage == 1)			shoot_frame = 270;
			else if(selected_stage == 2)	shoot_frame = 280;
			else if(selected_stage == 3)	shoot_frame = 290;
			else if(selected_stage == 4)	shoot_frame = 310;
			else if(selected_stage == 5)	shoot_frame = 330;
		}
		else if(selected_chapter == 8 || selected_chapter == 18)
		{
			if(selected_stage == 1)			shoot_frame = 350;
			else if(selected_stage == 2)	shoot_frame = 370;
			else if(selected_stage == 3)	shoot_frame = 390;
			else if(selected_stage == 4)	shoot_frame = 410;
			else if(selected_stage == 5)	shoot_frame = 430;
		}
	}
};

class AutoATK5 : public AutoAttacker
{
public:
	static AutoATK5* create()
	{
		AutoATK5* t_atk5 = new AutoATK5();
		t_atk5->myInit();
		t_atk5->autorelease();
		return t_atk5;
	}
	
	virtual ~AutoATK5()
	{
		delete my_balance;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK5::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK5::framing));
	}
	
private:
	BD_P2* my_balance;
	int shoot_frame;
	int ing_frame;
	int chapter_missile_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
			
			CCPoint subPosition = ccpSub(jackPosition, mainCumberPosition);
			
			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			if(distance >= 80)
			{
				int m_img_rand_type = rand()%2 + 1;
				CCPoint dv = ccpMult(subPosition, my_balance->move_speed/distance);
				AP_Missile1* t_m1 = AP_Missile1::create(mainCumberPosition, dv, my_balance->mCnt, 20,
														CCString::createWithFormat("chapter%d_basic_missile_%d.png", chapter_missile_number, m_img_rand_type)->getCString(), my_balance->crash_area);
				addChild(t_m1);
			}
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		my_balance = new BD_P2(1.f, 3, CCSize(6.f,6.f));
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		chapter_missile_number = SelectedMapData::sharedInstance()->getImageNumber(kIN_baseMissile);
		if(selected_chapter == 9 || selected_chapter == 19)
		{
			if(selected_stage == 1)			shoot_frame = 180;
			else if(selected_stage == 2)	shoot_frame = 200;
			else if(selected_stage == 3)	shoot_frame = 220;
			else if(selected_stage == 4)	shoot_frame = 240;
			else if(selected_stage == 5)	shoot_frame = 260;
		}
		else if(selected_chapter == 10 || selected_chapter == 20)
		{
			if(selected_stage == 1)			shoot_frame = 280;
			else if(selected_stage == 2)	shoot_frame = 300;
			else if(selected_stage == 3)	shoot_frame = 320;
			else if(selected_stage == 4)	shoot_frame = 340;
			else if(selected_stage == 5)	shoot_frame = 360;
		}
	}
};

class AutoATK6 : public AutoAttacker // line poison
{
public:
	static AutoATK6* create()
	{
		AutoATK6* t_atk6 = new AutoATK6();
		t_atk6->myInit();
		t_atk6->autorelease();
		return t_atk6;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK6::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK6::framing));
	}
	
private:
	int shoot_frame;
	int ing_frame;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint jackPoint = myGD->getJackPoint();
			CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
			
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
			
			CCPoint subPosition = ccpSub(jackPosition, mainCumberPosition);
			
			float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
			if(distance >= 80)
			{
				AP_Missile22* t_m22 = AP_Missile22::create(mainCumberPosition);
				addChild(t_m22);
			}
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		if(selected_chapter == 21)
		{
			if(selected_stage == 1)			shoot_frame = 260;
			else if(selected_stage == 2)	shoot_frame = 250;
			else if(selected_stage == 3)	shoot_frame = 240;
			else if(selected_stage == 4)	shoot_frame = 230;
			else if(selected_stage == 5)	shoot_frame = 220;
		}
		else if(selected_chapter == 31)
		{
			if(selected_stage == 1)			shoot_frame = 210;
			else if(selected_stage == 2)	shoot_frame = 200;
			else if(selected_stage == 3)	shoot_frame = 190;
			else if(selected_stage == 4)	shoot_frame = 180;
			else if(selected_stage == 5)	shoot_frame = 170;
		}
		else if(selected_chapter == 41)
		{
			if(selected_stage == 1)			shoot_frame = 260;
			else if(selected_stage == 2)	shoot_frame = 240;
			else if(selected_stage == 3)	shoot_frame = 220;
			else if(selected_stage == 4)	shoot_frame = 200;
			else if(selected_stage == 5)	shoot_frame = 180;
		}
	}
};

class AutoATK7 : public AutoAttacker // tick bomb
{
public:
	static AutoATK7* create()
	{
		AutoATK7* t_atk7 = new AutoATK7();
		t_atk7->myInit();
		t_atk7->autorelease();
		return t_atk7;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK7::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK7::framing));
	}
	
private:
	int shoot_frame;
	int ing_frame;
	int rangeCode;
	int bombFrameOneTime;
	int bombTimes;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			
			myGD->communication("MP_createTickingTimeBomb", mainCumberPoint, bombFrameOneTime, bombTimes, rangeCode);
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		
		bombTimes = 9;
		if(selected_chapter == 22)
		{
			rangeCode = 1;
			bombFrameOneTime = 120;
			if(selected_stage == 1)			shoot_frame = 600;
			else if(selected_stage == 2)	shoot_frame = 550;
			else if(selected_stage == 3)	shoot_frame = 500;
			else if(selected_stage == 4)	shoot_frame = 450;
			else if(selected_stage == 5)	shoot_frame = 400;
		}
		else if(selected_chapter == 32)
		{
			rangeCode = 2;
			bombFrameOneTime = 240;
			if(selected_stage == 1)			shoot_frame = 1000;
			else if(selected_stage == 2)	shoot_frame = 900;
			else if(selected_stage == 3)	shoot_frame = 800;
			else if(selected_stage == 4)	shoot_frame = 700;
			else if(selected_stage == 5)	shoot_frame = 600;
		}
		else if(selected_chapter == 42)
		{
			rangeCode = 1;
			bombFrameOneTime = 90;
			if(selected_stage == 1)			shoot_frame = 600;
			else if(selected_stage == 2)	shoot_frame = 550;
			else if(selected_stage == 3)	shoot_frame = 500;
			else if(selected_stage == 4)	shoot_frame = 450;
			else if(selected_stage == 5)	shoot_frame = 400;
		}
	}
};

class AutoATK8 : public AutoAttacker // create subCumber
{
public:
	static AutoATK8* create()
	{
		AutoATK8* t_atk8 = new AutoATK8();
		t_atk8->myInit();
		t_atk8->autorelease();
		return t_atk8;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK8::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK8::framing));
	}
	
private:
	int shoot_frame;
	int ing_frame;
	int create_subCumber_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			for(int i=0;i<create_subCumber_number;i++)
				myGD->communication("CP_createSubCumber", myGD->getMainCumberPoint());
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		if(selected_chapter == 23)
		{
			create_subCumber_number = 1;
			if(selected_stage == 1)			shoot_frame = 260;
			else if(selected_stage == 2)	shoot_frame = 250;
			else if(selected_stage == 3)	shoot_frame = 240;
			else if(selected_stage == 4)	shoot_frame = 230;
			else if(selected_stage == 5)	shoot_frame = 220;
		}
		else if(selected_chapter == 33)
		{
			create_subCumber_number = 2;
			if(selected_stage == 1)			shoot_frame = 300;
			else if(selected_stage == 2)	shoot_frame = 290;
			else if(selected_stage == 3)	shoot_frame = 280;
			else if(selected_stage == 4)	shoot_frame = 270;
			else if(selected_stage == 5)	shoot_frame = 260;
		}
		else if(selected_chapter == 43)
		{
			create_subCumber_number = 2;
			if(selected_stage == 1)			shoot_frame = 340;
			else if(selected_stage == 2)	shoot_frame = 320;
			else if(selected_stage == 3)	shoot_frame = 300;
			else if(selected_stage == 4)	shoot_frame = 280;
			else if(selected_stage == 5)	shoot_frame = 260;
		}
	}
};

class ThornsObject : public CCSprite
{
public:
	static ThornsObject* create(IntPoint t_sp, int t_thornsFrame)
	{
		ThornsObject* t_to = new ThornsObject();
		t_to->myInit(t_sp, t_thornsFrame);
		t_to->autorelease();
		return t_to;
	}
	
	void startMyAction()
	{
		ingFrame = -30;
		
		CCRotateBy* t_rotate = CCRotateBy::create(1.f, 240);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
		
		runAction(t_repeat);
		
		schedule(schedule_selector(ThornsObject::myAction));
	}
	
private:
	GameData* myGD;
	int thornsFrame;
	int ingFrame;
	bool is_action;
	IntPoint myPoint;
	
	void myAction()
	{
		ingFrame++;
		
		int surroundCnt = 0;
		IntPoint checkPoint = IntPoint(myPoint.x-1, myPoint.y);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(myPoint.x+1, myPoint.y);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(myPoint.x, myPoint.y-1);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		checkPoint = IntPoint(myPoint.x, myPoint.y+1);
		if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)		surroundCnt++;
		
		if(surroundCnt == 0)
		{
			stopMyAction();
			return;
		}
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1, (jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(jackPosition, getPosition());
		
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		if(distance < 16*getScale())
		{
			myGD->communication("CP_jackCrashDie");
			myGD->communication("Jack_startDieEffect");
		}
		
		if(!is_action)
		{
			if(ingFrame < 0)
				setScale((30+ingFrame)*0.033);
			else if(ingFrame == 0)
			{
				setScale((30+ingFrame)*0.033);
				is_action = true;
			}
			else if(ingFrame > 0)
			{
				setScale((30-ingFrame)*0.033);
				if(ingFrame >= 30)
				{
					stopMyAction();
					return;
				}
			}
		}
		else
		{
			if(ingFrame >= thornsFrame)
			{
				is_action = false;
				ingFrame = 0;
			}
		}
	}
	
	void stopMyAction()
	{
		unschedule(schedule_selector(ThornsObject::myAction));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(IntPoint t_sp, int t_thornsFrame)
	{
		myGD = GameData::sharedGameData();
		myPoint = t_sp;
		is_action = false;
		thornsFrame = t_thornsFrame;
		initWithFile("thorns_wall.png");
		setScale(0.01);
		setPosition(ccp((myPoint.x-1)*pixelSize+1,(myPoint.y-1)*pixelSize+1));
		
		startMyAction();
	}
};

class AutoATK9 : public AutoAttacker // thorns wall
{
public:
	static AutoATK9* create()
	{
		AutoATK9* t_atk9 = new AutoATK9();
		t_atk9->myInit();
		t_atk9->autorelease();
		return t_atk9;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK9::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK9::framing));
	}
	
private:
	int shoot_frame;
	int ing_frame;
	int create_thorns_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			
			if(create_thorns_number == 1)
			{
				ThornsObject* t_to = ThornsObject::create(mainCumberPoint, 3000);
				addChild(t_to);
			}
			else if(create_thorns_number == 2)
			{
				IntPoint checkPoint = IntPoint(mainCumberPoint.x-8, mainCumberPoint.y);
				if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)
				{
					ThornsObject* t_to_left = ThornsObject::create(checkPoint, 3000);
					addChild(t_to_left);
				}
				checkPoint = IntPoint(mainCumberPoint.x+8, mainCumberPoint.y);
				if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)
				{
					ThornsObject* t_to_right = ThornsObject::create(checkPoint, 3000);
					addChild(t_to_right);
				}
				checkPoint = IntPoint(mainCumberPoint.x, mainCumberPoint.y-8);
				if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)
				{
					ThornsObject* t_to_up = ThornsObject::create(checkPoint, 3000);
					addChild(t_to_up);
				}
				checkPoint = IntPoint(mainCumberPoint.x, mainCumberPoint.y+8);
				if(checkPoint.isInnerMap() && myGD->mapState[checkPoint.x][checkPoint.y] == mapEmpty)
				{
					ThornsObject* t_to_down = ThornsObject::create(checkPoint, 3000);
					addChild(t_to_down);
				}
			}
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		if(selected_chapter == 24)
		{
			create_thorns_number = 1;
			if(selected_stage == 1)			shoot_frame = 260;
			else if(selected_stage == 2)	shoot_frame = 250;
			else if(selected_stage == 3)	shoot_frame = 240;
			else if(selected_stage == 4)	shoot_frame = 230;
			else if(selected_stage == 5)	shoot_frame = 220;
		}
		else if(selected_chapter == 34)
		{
			create_thorns_number = 2;
			if(selected_stage == 1)			shoot_frame = 400;
			else if(selected_stage == 2)	shoot_frame = 380;
			else if(selected_stage == 3)	shoot_frame = 360;
			else if(selected_stage == 4)	shoot_frame = 340;
			else if(selected_stage == 5)	shoot_frame = 320;
		}
		else if(selected_chapter == 44)
		{
			create_thorns_number = 2;
			if(selected_stage == 1)			shoot_frame = 440;
			else if(selected_stage == 2)	shoot_frame = 420;
			else if(selected_stage == 3)	shoot_frame = 400;
			else if(selected_stage == 4)	shoot_frame = 380;
			else if(selected_stage == 5)	shoot_frame = 360;
		}
	}
};

class AutoATK10 : public AutoAttacker // targeting shoot
{
public:
	static AutoATK10* create()
	{
		AutoATK10* t_atk10 = new AutoATK10();
		t_atk10->myInit();
		t_atk10->autorelease();
		return t_atk10;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK10::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK10::framing));
	}
	
private:
	int shoot_frame;
	int ing_frame;
	int create_shoot_number;
	GameData* myGD;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame)
		{
			IntPoint mainCumberPoint = myGD->getMainCumberPoint();
			CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1,(mainCumberPoint.y-1)*pixelSize+1);
			
			AP_Missile10* t_m10 = AP_Missile10::create(mainCumberPosition, 5*create_shoot_number, 60*2, 30*create_shoot_number, CCSizeMake(8.f, 8.f));
			addChild(t_m10);
			
			ing_frame = 0;
		}
	}
	
	void myInit()
	{
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		if(selected_chapter == 25)
		{
			create_shoot_number = 1;
			if(selected_stage == 1)			shoot_frame = 300;
			else if(selected_stage == 2)	shoot_frame = 290;
			else if(selected_stage == 3)	shoot_frame = 280;
			else if(selected_stage == 4)	shoot_frame = 270;
			else if(selected_stage == 5)	shoot_frame = 260;
		}
		else if(selected_chapter == 35)
		{
			create_shoot_number = 3;
			if(selected_stage == 1)			shoot_frame = 500;
			else if(selected_stage == 2)	shoot_frame = 470;
			else if(selected_stage == 3)	shoot_frame = 440;
			else if(selected_stage == 4)	shoot_frame = 410;
			else if(selected_stage == 5)	shoot_frame = 380;
		}
		else if(selected_chapter == 45)
		{
			create_shoot_number = 3;
			if(selected_stage == 1)			shoot_frame = 550;
			else if(selected_stage == 2)	shoot_frame = 520;
			else if(selected_stage == 3)	shoot_frame = 490;
			else if(selected_stage == 4)	shoot_frame = 460;
			else if(selected_stage == 5)	shoot_frame = 430;
		}
	}
};

class Deather : public CCNode
{
public:
	static Deather* create(int type, CCPoint s_p)
	{
		Deather* t_d = new Deather();
		t_d->myInit(type, s_p);
		t_d->autorelease();
		return t_d;
	}
	
private:
	CCSprite* magic_square;	// 1
	CCSprite* deather_body;	// 2
	CCSprite* deather_side; // 2-1
	CCSprite* swing;		// 3
	
	CCSprite* small_warning; // 3
	CCSprite* leftup;		// 3
	CCSprite* leftdown;		// 3
	CCSprite* rightup;		// 3
	CCSprite* rightdown;	// 3
	
	GameData* myGD;
	
	float speed;
	
	void startAction()
	{
		leftup = CCSprite::create("warning_side.png");
		leftup->setScale(2.0);
		leftup->setOpacity(0);
		leftup->setPosition(ccp(-120,120));
		addChild(leftup, 3);
		CCMoveTo* move1 = CCMoveTo::create(1.f, ccp(-50,50));
		CCFadeIn* fade1 = CCFadeIn::create(0.7f);
		CCScaleTo* scale1 = CCScaleTo::create(1.f, 1.f);
		CCFadeOut* fade1_1 = CCFadeOut::create(0.3f);
		CCCallFunc* call1 = CCCallFunc::create(this, callfunc_selector(Deather::removeLeftUp));
		CCAction* action1 = CCSequence::create(CCSpawn::create(move1, fade1, scale1, NULL), fade1_1, call1, NULL);
		
		leftdown = CCSprite::create("warning_side.png");
		leftdown->setRotation(-90);
		leftdown->setScale(2.0);
		leftdown->setOpacity(0);
		leftdown->setPosition(ccp(-120,-120));
		addChild(leftdown, 3);
		CCMoveTo* move2 = CCMoveTo::create(1.f, ccp(-50,-50));
		CCFadeIn* fade2 = CCFadeIn::create(0.7f);
		CCScaleTo* scale2 = CCScaleTo::create(1.f, 1.f);
		CCFadeOut* fade2_1 = CCFadeOut::create(0.3f);
		CCCallFunc* call2 = CCCallFunc::create(this, callfunc_selector(Deather::removeLeftDown));
		CCAction* action2 = CCSequence::create(CCSpawn::create(move2, fade2, scale2, NULL), fade2_1, call2, NULL);
		
		rightdown = CCSprite::create("warning_side.png");
		rightdown->setRotation(180);
		rightdown->setScale(2.0);
		rightdown->setOpacity(0);
		rightdown->setPosition(ccp(120,-120));
		addChild(rightdown, 3);
		CCMoveTo* move3 = CCMoveTo::create(1.f, ccp(50,-50));
		CCFadeIn* fade3 = CCFadeIn::create(0.7f);
		CCScaleTo* scale3 = CCScaleTo::create(1.f, 1.f);
		CCFadeOut* fade3_1 = CCFadeOut::create(0.3f);
		CCCallFunc* call3 = CCCallFunc::create(this, callfunc_selector(Deather::removeRightDown));
		CCAction* action3 = CCSequence::create(CCSpawn::create(move3, fade3, scale3, NULL), fade3_1, call3, NULL);
		
		rightup = CCSprite::create("warning_side.png");
		rightup->setRotation(90);
		rightup->setScale(2.0);
		rightup->setOpacity(0);
		rightup->setPosition(ccp(120,120));
		addChild(rightup, 3);
		CCMoveTo* move4 = CCMoveTo::create(1.f, ccp(50,50));
		CCFadeIn* fade4 = CCFadeIn::create(0.7f);
		CCScaleTo* scale4 = CCScaleTo::create(1.f, 1.f);
		CCFadeOut* fade4_1 = CCFadeOut::create(0.3f);
		CCCallFunc* call4 = CCCallFunc::create(this, callfunc_selector(Deather::removeRightUp));
		CCAction* action4 = CCSequence::create(CCSpawn::create(move4, fade4, scale4, NULL), fade4_1, call4, NULL);
		
		small_warning = CCSprite::create("small_warning.png");
		small_warning->setOpacity(0);
		small_warning->setPosition(ccp(0,40));
		addChild(small_warning, 3);
		CCFadeIn* fade5 = CCFadeIn::create(0.7f);
		CCDelayTime* delay = CCDelayTime::create(0.3f);
		CCFadeOut* fade5_1 = CCFadeOut::create(0.3f);
		CCCallFunc* call5 = CCCallFunc::create(this, callfunc_selector(Deather::removeSmallWarning));
		CCAction* action5 = CCSequence::create(fade5, delay, fade5_1, call5, NULL);
		
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("magic_square.png");
		magic_square = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 100, 60));
		magic_square->setPosition(CCPointZero);
		addChild(magic_square, 1);
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1f);
		for(int i=0;i<2;i++)
			for(int j=0;j<3;j++)
				t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(j*100, i*60, 100, 60));
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCDelayTime* delay6_1 = CCDelayTime::create(0.1f);
		CCDelayTime* delay6_2 = CCDelayTime::create(0.5f);
		CCCallFunc* call6 = CCCallFunc::create(this, callfunc_selector(Deather::removeMagicSquare));
		CCAction* action6 = CCSequence::create(delay6_1, t_animate, delay6_2, call6, NULL);
		
		CCDelayTime* t_delay = CCDelayTime::create(0.5f);
		CCRotateBy* t_rotate = CCRotateBy::create(1.f, 720);
		CCScaleTo* t_scale = CCScaleTo::create(1.f, 1.f);
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_rotate, t_scale);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Deather::startTrace));
		CCAction* t_action = CCSequence::create(t_delay, t_spawn, t_call, NULL);
		
		leftup->runAction(action1);
		leftdown->runAction(action2);
		rightdown->runAction(action3);
		rightup->runAction(action4);
		small_warning->runAction(action5);
		magic_square->runAction(action6);
		deather_body->runAction(t_action);
	}
	
	void removeLeftUp(){	leftup->removeFromParentAndCleanup(true);	}
	void removeLeftDown(){	leftdown->removeFromParentAndCleanup(true);	}
	void removeRightUp(){	rightup->removeFromParentAndCleanup(true);	}
	void removeRightDown(){	rightdown->removeFromParentAndCleanup(true);}
	void removeSmallWarning(){	small_warning->removeFromParentAndCleanup(true);	}
	void removeMagicSquare(){	magic_square->removeFromParentAndCleanup(true);		}
	
	void startTrace()
	{
		schedule(schedule_selector(Deather::traceAction));
	}
	
	void traceAction()
	{
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		CCPoint dv = ccpSub(getPosition(), jackPosition);
		dv = ccpMult(dv, -1);
		
		float divide_value = sqrtf(powf(dv.x, 2.f) + powf(dv.y, 2.f));
		dv = ccpMult(dv, speed/divide_value);
		
		if(dv.x > 0)
		{
			deather_body->setFlipX(true);
			deather_side->setFlipX(true);
		}
		else
		{
			deather_body->setFlipX(false);
			deather_side->setFlipX(false);
		}
		
		setPosition(ccpAdd(getPosition(), dv));
		
		if(divide_value < 15)
		{
			unschedule(schedule_selector(Deather::traceAction));
			
			deather_side->setRotation(40);
			CCRotateTo* t_rotate = CCRotateTo::create(0.5f, -100);
			deather_side->runAction(t_rotate);
			
			float angle = atan2f(dv.y, dv.x)/M_PI*180.f;
			DeathSwing* t_ds = DeathSwing::create(ccpSub(getPosition(), jackPosition), -angle);
			addChild(t_ds, 4);
			t_ds->startAnimation();
			
			myGD->communication("Jack_startDieEffect");
		}
	}
	
	void myInit(int type, CCPoint s_p)
	{
		myGD = GameData::sharedGameData();
		setPosition(s_p);
		if(type == 1)
		{
			speed = 0.4f;
			deather_body = CCSprite::create("chapter21_boss_body.png");
			deather_side = CCSprite::create("chapter21_boss_side.png");
			deather_side->setPosition(ccp(deather_body->getContentSize().width/2.f,deather_body->getContentSize().height/2.f));
			deather_body->addChild(deather_side, -1);
			deather_body->setScale(0);
			addChild(deather_body, 2);
		}
		else if(type == 2)
		{
			speed = 0.6f;
			deather_body = CCSprite::create("chapter31_boss_body.png");
			deather_side = CCSprite::create("chapter31_boss_side.png");
			deather_side->setPosition(ccp(deather_body->getContentSize().width/2.f,deather_body->getContentSize().height/2.f));
			deather_body->addChild(deather_side, -1);
			deather_body->setScale(0);
			addChild(deather_body, 2);
		}
		else if(type == 3)
		{
			speed = 0.6f;
			deather_body = CCSprite::create("chapter41_boss_body.png");
			deather_side = CCSprite::create("chapter41_boss_side.png");
			deather_side->setPosition(ccp(deather_body->getContentSize().width/2.f,deather_body->getContentSize().height/2.f));
			deather_body->addChild(deather_side, -1);
			deather_body->setScale(0);
			addChild(deather_body, 2);
		}
		
		startAction();
	}
};

class AutoATK11 : public AutoAttacker // pause and resume
{
public:
	static AutoATK11* create(CCNode* boss_eye)
	{
		AutoATK11* t_atk11 = new AutoATK11();
		t_atk11->myInit(boss_eye);
		t_atk11->autorelease();
		return t_atk11;
	}
	
	void stopFraming()
	{
		unschedule(schedule_selector(AutoATK11::framing));
	}
	
	void startFraming()
	{
		ing_frame = 0;
		schedule(schedule_selector(AutoATK11::framing));
	}
	
private:
	int shoot_frame;
	int ing_frame;
	int step;
	int type;
	GameData* myGD;
	BossEye* my_eye;
	
	void framing()
	{
		ing_frame++;
		
		if(ing_frame >= shoot_frame && step < 7)
		{
			step++;
			my_eye->setStep(step);
			
			if(step == 7)
			{
				stopFraming();
				
				IntPoint mainCumberPoint = myGD->getMainCumberPoint();
				CCPoint mainCumberPosition = ccp((mainCumberPoint.x-1)*pixelSize+1, (mainCumberPoint.y-1)*pixelSize+1);
				Deather* my_deather = Deather::create(type, mainCumberPosition);
				addChild(my_deather);
			}
			
			ing_frame = 0;
		}
	}
	
	void myInit(CCNode* boss_eye)
	{
		my_eye = (BossEye*)boss_eye;
		
		myGD = GameData::sharedGameData();
		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		
		step = 1;
		
		if(selected_chapter == 26)
		{
			type = 1;
			if(selected_stage == 1)			shoot_frame = 600;
			else if(selected_stage == 2)	shoot_frame = 500;
			else if(selected_stage == 3)	shoot_frame = 400;
			else if(selected_stage == 4)	shoot_frame = 300;
			else if(selected_stage == 5)	shoot_frame = 200;
		}
		else if(selected_chapter == 36)
		{
			type = 2;
			if(selected_stage == 1)			shoot_frame = 600;
			else if(selected_stage == 2)	shoot_frame = 500;
			else if(selected_stage == 3)	shoot_frame = 400;
			else if(selected_stage == 4)	shoot_frame = 300;
			else if(selected_stage == 5)	shoot_frame = 200;
		}
		else if(selected_chapter == 46)
		{
			type = 3;
			if(selected_stage == 1)			shoot_frame = 650;
			else if(selected_stage == 2)	shoot_frame = 550;
			else if(selected_stage == 3)	shoot_frame = 450;
			else if(selected_stage == 4)	shoot_frame = 350;
			else if(selected_stage == 5)	shoot_frame = 250;
		}
	}
};

#endif
