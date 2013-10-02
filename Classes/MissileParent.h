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
	static CreateSubCumberOtherAction* create(IntPoint c_p,
											  CCObject* t_after, SEL_CallFuncO d_after,
											  CCObject* t_cancel, SEL_CallFuncO d_cancel)
	{
		CreateSubCumberOtherAction* t_CSCAA = new CreateSubCumberOtherAction();
		t_CSCAA->myInit(c_p, t_after, d_after, t_cancel, d_cancel);
		t_CSCAA->autorelease();
		return t_CSCAA;
	}
	
	void afterAction(CCObject* cb)
	{
		myGD->communication("CP_createSubCumber", createPoint);
		(after_target->*after_delegate)(cb);
		removeFromParentAndCleanup(true);
	}
	
	void cancelAction(CCObject* cb)
	{
		(cancel_target->*cancel_delegate)(cb);
		removeFromParentAndCleanup(true);
	}
	
private:
	
	CCObject* after_target;
	SEL_CallFuncO after_delegate;
	CCObject* cancel_target;
	SEL_CallFuncO cancel_delegate;
	IntPoint createPoint;
	
	void myInit(IntPoint c_p, CCObject* t_after, SEL_CallFuncO d_after, CCObject* t_cancel, SEL_CallFuncO d_cancel)
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
	
	void actionAP6(CCObject* cb);
	void cancelAP6(CCObject* cb){		}
	
	void ingAP7(CCObject* cb);
	void actionAP7(CCObject* cb);
	void cancelAP7(CCObject* cb);
	
	void actionAP8(CCObject* cb);
	void cancelAP8(CCObject* cb){		}
	
	void actionAP9(CCObject* cb);
	void cancelAP9(CCObject* cb){		}
	
	void actionAP10(CCObject* cb);
	void cancelAP10(CCObject* cb){		}
	
	void actionAP11(CCObject* cb);
	void cancelAP11(CCObject* cb){		}
	
	void actionAP12(CCObject* cb);
	void cancelAP12(CCObject* cb){		}
	
	
	void ingAP13()
	{
//		myGD->communication("CP_checkingJackCrash");
	}
	void actionAP13(CCObject* cb);
	void cancelAP13(CCObject* cb){		}
	
	void actionAP14(CCObject* cb);
	void cancelAP14(CCObject* cb){		}
	
	void actionAP15(CCObject* cb);
	void cancelAP15(CCObject* cb){		}
	
	void actionAP16(CCObject* cb);
	void cancelAP16(CCObject* cb){		}
	
	void ingAP17(CCObject* cb)
	{
//		myGD->communication("CP_checkingJackCrash");
	}
	void actionAP17(CCObject* cb);
	void cancelAP17(CCObject* cb){		}
	
	void actionAP18(CCObject* cb);
	void cancelAP18(CCObject* cb){		}
	
	void actionAP19(CCObject* cb);
	void cancelAP19(CCObject* cb){		}
	
	void actionAP20(CCObject* cb);
	void cancelAP20(CCObject* cb){		}
	
	void actionAP21(CCObject* cb);
	void cancelAP21(CCObject* cb){		}
	
	void actionAP22(CCObject* cb);
	void cancelAP22(CCObject* cb){		}
	
	void actionAP23(CCObject* cb);
	void cancelAP23(CCObject* cb){		}
	
	void actionAP24(CCObject* cb);
	void cancelAP24(CCObject* cb){	}
	
	void actionAP25(CCObject* cb);
	void cancelAP25(CCObject* cb){		}
	
	void actionAP26(CCObject* cb);
	void cancelAP26(CCObject* cb){		}
	
	void actionAP27(CCObject* cb);
	void cancelAP27(CCObject* cb){	}
	
	void actionAP28(CCObject* cb);
	void cancelAP28(CCObject* cb){	}
	
	void actionAP29(CCObject* cb);
	void cancelAP29(CCObject* cb){	}
	
	void actionAP30(CCObject* cb);
	void cancelAP30(CCObject* cb){	}
	
	void actionAP31(CCObject* cb);
	void cancelAP31(CCObject* cb){		}
	
	void actionAP32(CCObject* cb);
	void cancelAP32(CCObject* cb){		}
	
	void actionAP33(CCObject* cb);
	void cancelAP33(CCObject* cb){		}
	
	void actionAP34(CCObject* cb);
	void cancelAP34(CCObject* cb){	}
	
	void actionAP101(CCObject* cb);
	void cancelAP101(CCObject* cb){		}
	
	void actionAP102(CCObject* cb);
	void cancelAP102(CCObject* cb){		}
	
	void actionAP103(CCObject* cb);
	void cancelAP103(CCObject* cb){	}
	
	
	
	

	
	void createSubCumberReplication(CCPoint s_p, CCObject* sender, SEL_CallFuncO d_startMoving);
	
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
	
	
	
	void actionKSAP1(CCObject* cb)
	{
		
		KSAttackPattern1* t = KSAttackPattern1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;

	}
	void cancelKSAP1(CCObject* cb){		}
	void actionKSAP2(CCObject* cb)
	{
		KSAttackPattern2* t = KSAttackPattern2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
		
		
	}
	void cancelKSAP2(CCObject* cb){		}
	
	void actionKSAP3(CCObject* cb)
	{
		KSAttackPattern3* t = KSAttackPattern3::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP3(CCObject* cb){		}
	void actionKSAP4(CCObject* cb)
	{
		KSAttackPattern4* t = KSAttackPattern4::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP4(CCObject* cb){		}
	void actionKSAP5(CCObject* cb)
	{
		KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP5(CCObject* cb){		}
	void actionKSAP6(CCObject* cb)
	{
		KSAttackPattern6* t = KSAttackPattern6::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP6(CCObject* cb){		}
	void actionKSAP7(CCObject* cb)
	{
		KSAttackPattern7* t = KSAttackPattern7::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP7(CCObject* cb){		}
	void actionKSAP8(CCObject* cb)
	{
		KSAttackPattern8* t = KSAttackPattern8::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP8(CCObject* cb){		}
	void actionKSAP9(CCObject* cb)
	{
		KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP9(CCObject* cb){		}
	void actionKSAP10(CCObject* cb)
	{
		KSAttackPattern5* t = KSAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelKSAP10(CCObject* cb){		}
	
	void actionTargetAP1(CCObject* cb)
	{
		KSTargetAttackPattern1* t = KSTargetAttackPattern1::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP1(CCObject* cb){		}
	void actionTargetAP2(CCObject* cb)
	{
		KSTargetAttackPattern2* t = KSTargetAttackPattern2::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP2(CCObject* cb){		}
	void actionTargetAP3(CCObject* cb)
	{
		KSTargetAttackPattern3* t = KSTargetAttackPattern3::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP3(CCObject* cb){		}
	void actionTargetAP4(CCObject* cb)
	{
		KSTargetAttackPattern4* t = KSTargetAttackPattern4::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP4(CCObject* cb){		}
	
	void actionTargetAP5(CCObject* cb)
	{
		KSTargetAttackPattern5* t = KSTargetAttackPattern5::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP5(CCObject* cb){		}
	void actionTargetAP6(CCObject* cb)
	{
		KSTargetAttackPattern6* t = KSTargetAttackPattern6::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP6(CCObject* cb){		}
	
	void actionTargetAP7(CCObject* cb)
	{
		KSTargetAttackPattern7* t = KSTargetAttackPattern7::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
		
		
	}
	void cancelTargetAP7(CCObject* cb){		}
	void actionTargetAP8(CCObject* cb)
	{
		KSTargetAttackPattern8* t = KSTargetAttackPattern8::create(startFirePosition, dynamic_cast<KSCumberBase*>(cb));
		addChild(t);
		saveAP = t;
		savedAP = true;
	}
	void cancelTargetAP8(CCObject* cb){		}
	
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
