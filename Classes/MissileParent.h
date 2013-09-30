//
//  MissileParent.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 4..
//
//

#ifndef DrawingJack_MissileParent_h
#define DrawingJack_MissileParent_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "AttackPattern.h"
#include "JackMissile.h"
#include "RandomSelector.h"
#include "SelectedMapData.h"
#include "AudioEngine.h"
#include "AutoAttacker.h"
#include "ShockWave.h"
#include "KSCumberBase.h"
#include "Charges.h"

using namespace cocos2d;
using namespace std;






class CreateSubCumberOtherAction : public CCNode
{
public:
	static CreateSubCumberOtherAction* create(IntPoint c_p, CCObject* t_after, SEL_CallFunc d_after, CCObject* t_cancel, SEL_CallFunc d_cancel)
	{
		CreateSubCumberOtherAction* t_CSCAA = new CreateSubCumberOtherAction();
		t_CSCAA->myInit(c_p, t_after, d_after, t_cancel, d_cancel);
		t_CSCAA->autorelease();
		return t_CSCAA;
	}
	
	void afterAction()
	{
		myGD->communication("CP_createSubCumber", createPoint);
		(after_target->*after_delegate)();
		removeFromParentAndCleanup(true);
	}
	
	void cancelAction()
	{
		(cancel_target->*cancel_delegate)();
		removeFromParentAndCleanup(true);
	}
	
private:
	
	CCObject* after_target;
	SEL_CallFunc after_delegate;
	CCObject* cancel_target;
	SEL_CallFunc cancel_delegate;
	IntPoint createPoint;
	
	void myInit(IntPoint c_p, CCObject* t_after, SEL_CallFunc d_after, CCObject* t_cancel, SEL_CallFunc d_cancel)
	{
		
		createPoint = c_p;
		after_target = t_after;
		after_delegate = d_after;
		cancel_target = t_cancel;
		cancel_delegate = d_cancel;
	}
};

class UM_creator : public CCNode
{
public:
	static UM_creator* create(int t_um_tcnt, int t_create_type, float t_damage_per)
	{
		UM_creator* t_c = new UM_creator();
		t_c->myInit(t_um_tcnt, t_create_type, t_damage_per);
		t_c->autorelease();
		return t_c;
	}
	
	static UM_creator* create(int t_um_tcnt, int t_create_type, float t_damage_per, CCPoint s_p)
	{
		UM_creator* t_c = new UM_creator();
		t_c->myInit(t_um_tcnt, t_create_type, t_damage_per, s_p);
		t_c->autorelease();
		return t_c;
	}
	
	void startCreate()
	{
		ing_frame = 0;
		ing_um_cnt = 0;
		schedule(schedule_selector(UM_creator::creating));
	}
	
	void startPetCreate()
	{
		ing_frame = 0;
		ing_um_cnt = 0;
		schedule(schedule_selector(UM_creator::petCreating));
	}
	
private:
	
	int ing_frame;
	int um_tcnt;
	int ing_um_cnt;
	int create_type;
	float damage_per;
	
	CCPoint start_position;
	
	void creating()
	{
		ing_frame++;
		
		if(ing_frame%6 == 0)
		{
			if(ing_frame/6 <= 1)
			{
				JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
				int cumberCnt = 1 + subCumberArray->count();
				int random_value;
				
				random_value = rand()%cumberCnt;
				if(random_value == 0)
				{
					JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per);
					getParent()->addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					JackMissile* t_jm = JM_UpgradeMissile::create((CCNode*)subCumberArray->objectAtIndex(random_value-1), create_type, damage_per);
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
	
	void petCreating()
	{
		ing_frame++;
		
		if(ing_frame%6 == 0)
		{
			if(ing_frame/6 <= 1)
			{
				JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per, start_position);
				getParent()->addChild(t_jm);
				t_jm->startMoving();
			}
			else
			{
				CCArray* subCumberArray = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
				int cumberCnt = 1 + subCumberArray->count();
				int random_value;
				
				random_value = rand()%cumberCnt;
				if(random_value == 0)
				{
					JackMissile* t_jm = JM_UpgradeMissile::create(myGD->getCommunicationNode("CP_getMainCumberPointer"), create_type, damage_per, start_position);
					getParent()->addChild(t_jm);
					t_jm->startMoving();
				}
				else
				{
					JackMissile* t_jm = JM_UpgradeMissile::create((CCNode*)subCumberArray->objectAtIndex(random_value-1), create_type, damage_per, start_position);
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
	
	void stopCreate()
	{
		unschedule(schedule_selector(UM_creator::creating));
		removeFromParentAndCleanup(true);
	}
	
	void stopPetCreate()
	{
		unschedule(schedule_selector(UM_creator::petCreating));
		removeFromParentAndCleanup(true);
	}
	
	void myInit(int t_um_tcnt, int t_create_type, float t_damage_per)
	{
		
		damage_per = t_damage_per;
		um_tcnt = t_um_tcnt;
		create_type = t_create_type;
	}
	
	void myInit(int t_um_tcnt, int t_create_type, float t_damage_per, CCPoint s_p)
	{
		start_position = s_p;
		myInit(t_um_tcnt, t_create_type, t_damage_per);
	}
};

class MissileParent : public CCNode
{
public:
	static MissileParent* create(CCNode* boss_eye)
	{
		MissileParent* t_mp = new MissileParent();
		t_mp->myInit(boss_eye);
		t_mp->autorelease();
		return t_mp;
	}
	
	void bombCumber(CCObject* target);
	
	void createJackMissile(int jm_type, int cmCnt, float damage_per);
	
	void subOneDie();
	
	void endIngActionAP();
	
	void actionAP6();
	void cancelAP6(){		}
	
	void ingAP7();
	void actionAP7();
	void cancelAP7();
	
	void actionAP8();
	void cancelAP8(){		}
	
	void actionAP9();
	void cancelAP9(){		}
	
	void actionAP10();
	void cancelAP10(){		}
	
	void actionAP11();
	void cancelAP11(){		}
	
	void actionAP12();
	void cancelAP12(){		}
	
	
	void ingAP13()
	{
//		myGD->communication("CP_checkingJackCrash");
	}
	void actionAP13();
	void cancelAP13(){		}
	
	void actionAP14();
	void cancelAP14(){		}
	
	void actionAP15();
	void cancelAP15(){		}
	
	void actionAP16();
	void cancelAP16(){		}
	
	void ingAP17()
	{
//		myGD->communication("CP_checkingJackCrash");
	}
	void actionAP17();
	void cancelAP17(){		}
	
	void actionAP18();
	void cancelAP18(){		}
	
	void actionAP19();
	void cancelAP19(){		}
	
	void actionAP20();
	void cancelAP20(){		}
	
	void actionAP21();
	void cancelAP21(){		}
	
	void actionAP22();
	void cancelAP22(){		}
	
	void actionAP23();
	void cancelAP23(){		}
	
	void actionAP24();
	void cancelAP24(){	}
	
	void actionAP25();
	void cancelAP25(){		}
	
	void actionAP26();
	void cancelAP26(){		}
	
	void actionAP27();
	void cancelAP27(){	}
	
	void actionAP28();
	void cancelAP28(){	}
	
	void actionAP29();
	void cancelAP29(){	}
	
	void actionAP30();
	void cancelAP30(){	}
	
	void actionAP31();
	void cancelAP31(){		}
	
	void actionAP32();
	void cancelAP32(){		}
	
	void actionAP33();
	void cancelAP33(){		}
	
	void actionAP34();
	void cancelAP34(){	}
	
	void actionAP101();
	void cancelAP101(){		}
	
	void actionAP102();
	void cancelAP102(){		}
	
	void actionAP103();
	void cancelAP103(){	}
	
	
	
	
	void attackWithCode(CCPoint startPosition, int pattern_code);
	void startFire(CCPoint startPosition, bool crash_attack = false);
	
	void createSubCumberReplication(CCPoint s_p, CCObject* sender, SEL_CallFunc d_startMoving);
	
	void explosion(CCPoint bombPosition, ccColor4F t_color);
	
	void createTickingTimeBomb(IntPoint t_point, int t_bombFrameOneTime, int t_bombTimes, int t_rangeCode);
	
	void resetTickingTimeBomb();
	
	void deleteKeepAP25(){		keepAP25 = NULL;	}
	void protectedAP25(){		keepAP25->stopFrame();	}
	
	void deleteKeepAP23(){		keepAP23 = NULL;	}
	void deleteKeepAP26(){		keepAP26 = NULL;	}
	void protectedAP26(){		keepAP26->stopFrame();	}
	
	void deleteKeepAP27(){		keepAP27 = NULL;	}
	void protectedAP27(){		keepAP27->stopFrame();	}
	
	void deleteKeepAP33(){		keepAP33 = NULL;	}
	void protectedAP33(){		keepAP33->stopFrame();	}
	
	void deleteKeepAP24(){		keepAP24 = NULL;	}
	void deleteKeepAP34(){		keepAP34 = NULL;	}
	void deleteKeepAP35()
	{
		if(keepAP35)
		{
			keepAP35->stopFraming();
			keepAP35 = NULL;
		}
	}
	
	void stopAutoAttacker();
	
	void startAutoAttacker();
	
	virtual ~MissileParent()
	{
		chargeArray->release();
		tickingArray->release();
		myRS->release();
	}
	
	void shootPetMissile(int jm_type, int cmCnt, float damage_per, CCPoint s_p);
	
	
	
	void actionKSAP1()
	{
		
		KSAttackPattern1* t = KSAttackPattern1::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;

	}
	void cancelKSAP1(){		}
	void actionKSAP2()
	{
		KSAttackPattern2* t = KSAttackPattern2::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
		
		
	}
	void cancelKSAP2(){		}
	
	void actionKSAP3()
	{
		KSAttackPattern3* t = KSAttackPattern3::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP3(){		}
	void actionKSAP4()
	{
		KSAttackPattern4* t = KSAttackPattern4::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP4(){		}
	void actionKSAP5()
	{
		KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP5(){		}
	void actionKSAP6()
	{
		KSAttackPattern6* t = KSAttackPattern6::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP6(){		}
	void actionKSAP7()
	{
		KSAttackPattern7* t = KSAttackPattern7::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP7(){		}
	void actionKSAP8()
	{
		KSAttackPattern8* t = KSAttackPattern8::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP8(){		}
	void actionKSAP9()
	{
		KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP9(){		}
	void actionKSAP10()
	{
		KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP10(){		}
	
	void actionTargetAP1()
	{
		KSTargetAttackPattern1* t = KSTargetAttackPattern1::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP1(){		}
	void actionTargetAP2()
	{
		KSTargetAttackPattern2* t = KSTargetAttackPattern2::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP2(){		}
	void actionTargetAP3()
	{
		KSTargetAttackPattern3* t = KSTargetAttackPattern3::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP3(){		}
	void actionTargetAP4()
	{
		KSTargetAttackPattern4* t = KSTargetAttackPattern4::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP4(){		}
	
	void actionTargetAP5()
	{
		KSTargetAttackPattern5* t = KSTargetAttackPattern5::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP5(){		}
	void actionTargetAP6()
	{
		KSTargetAttackPattern6* t = KSTargetAttackPattern6::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP6(){		}
	
	void actionTargetAP7()
	{
		KSTargetAttackPattern7* t = KSTargetAttackPattern7::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP7(){		}
	void actionTargetAP8()
	{
		KSTargetAttackPattern8* t = KSTargetAttackPattern8::create(startFirePosition, 0);
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP8(){		}
	
	bool attackWithKSCode(CCPoint startPosition, int pattern, KSCumberBase* cb, bool exe);
private:
	
	AutoAttacker* myAA;
	SW_Parent* mySW;
	
	RandomSelector* myRS;
	
	CCPoint startFirePosition;
	int slowAreaCnt;
	
	bool savedAP;
	AttackPattern* saveAP;
	AP_Missile7* keepAP7;
	AP_Missile25* keepAP25;
	AP_Missile23* keepAP23;
	AP_Missile26* keepAP26;
	AP_Missile27* keepAP27;
	AP_Missile33* keepAP33;
	AP_Missile24* keepAP24;
	AP_Missile34* keepAP34;
	AP_Missile35* keepAP35;
	CCArray* chargeArray;
	CCArray* tickingArray;
	
	CCParticleSystem* particle;
	
	void initParticle(CCPoint startPosition, ccColor4F t_color);
	
	void removeChargeInArray(CCObject* remove_charge);
	
	void movingMainCumber();
	
	void setBalance();

	void myInit(CCNode* boss_eye);
};

#endif
