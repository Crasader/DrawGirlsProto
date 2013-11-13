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
#include "AudioEngine.h"
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


	
	void createSubCumberReplication(CCPoint s_p, CCObject* sender, SEL_CallFuncO d_startMoving);
	
	void explosion(CCPoint bombPosition, ccColor4F t_color);
	
	void createTickingTimeBomb(IntPoint t_point, int t_bombFrameOneTime, int t_bombTimes, int t_rangeCode);
	
	void resetTickingTimeBomb();
	
	
	
	void deleteKeepAP23(){		keepAP23 = NULL;	}
	void deleteKeepAP26(){		keepAP26 = NULL;	}
	void protectedAP26(){		keepAP26->stopFrame();	}
	

	
	void deleteKeepAP33(){		keepAP33 = NULL;	}
	void protectedAP33(){		keepAP33->stopFrame();	}
	
	void deleteKeepAP24(){		keepAP24 = NULL;	}

	
	virtual ~MissileParent()
	{
		chargeArray->release();
		tickingArray->release();
	}
	
	void shootPetMissile(int jm_type, int cmCnt, float damage_per, CCPoint s_p);
	
	
	
	
	enum PATTERN_RET_CODE{INVALID=0, VALID=1, NOCAST=2};
	
	int attackWithKSCode(CCPoint startPosition, std::string pattern, KSCumberBase* cb, bool exe);
private:
	
	SW_Parent* mySW;
	
	CCPoint startFirePosition;
	int slowAreaCnt;
	
	bool savedAP;
	AttackPattern* saveAP;

	AP_Missile23* keepAP23;
	AP_Missile26* keepAP26;
	AP_Missile33* keepAP33;
	AP_Missile24* keepAP24;
	CCArray* chargeArray;
	CCArray* tickingArray;
	
	CCParticleSystem* particle;
	
	void initParticle(CCPoint startPosition, ccColor4F t_color);
	
	void removeChargeInArray(CCObject* remove_charge);
	
	void movingMainCumber();

	void myInit(CCNode* boss_eye);
};

#endif
