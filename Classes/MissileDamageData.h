//
//  MissileDamageData.h
//  DrawingJack
//
//  Created by 사원3 on 13. 1. 30..
//
//

#ifndef DrawingJack_MissileDamageData_h
#define DrawingJack_MissileDamageData_h

#include "cocos2d.h"
#include "EnumDefine.h"
#include "DataStorageHub.h"
#include "ServerDataSave.h"
#include "SilhouetteData.h"

using namespace cocos2d;

class MissileDamageData : public CCObject
{
public:
	static int getJMDamage()
	{
		return SDS_GI(kSDF_stageInfo, mySD->getSilType(), "itemOption_attack_power");
	}
	
	static float getCorrelationDamage(float t_damage, int cumber_elemental)
	{
		int missile_elemental = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_lastSelectedElement);
		float level_penalty = 0;
		
		if(missile_elemental == kElementCode_empty || cumber_elemental == kElementCode_empty)			t_damage *= 0.7f;
		else if(missile_elemental == kElementCode_life)
		{
			level_penalty = DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_elementLevelLife)*0.03;
			if(cumber_elemental == kElementCode_life)			t_damage *= 0.8f-level_penalty;
			else if(cumber_elemental == kElementCode_fire)		t_damage *= 0.5f-level_penalty;
			else if(cumber_elemental == kElementCode_water)
			{
				t_damage *= 1.2f;
			}
		}
		else if(missile_elemental == kElementCode_fire)
		{
			level_penalty = (DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_elementLevelFire)-1)*0.03;
			if(cumber_elemental == kElementCode_life)			t_damage *= 1.2f;
			else if(cumber_elemental == kElementCode_fire)		t_damage *= 0.8f-level_penalty;
			else if(cumber_elemental == kElementCode_water)		t_damage *= 0.5f-level_penalty;
		}
		else if(missile_elemental == kElementCode_water)
		{
			level_penalty = (DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_elementLevelWater)-1)*0.03;
			if(cumber_elemental == kElementCode_life)			t_damage *= 0.5f-level_penalty;
			else if(cumber_elemental == kElementCode_fire)		t_damage *= 1.2f;
			else if(cumber_elemental == kElementCode_water)		t_damage *= 0.8f-level_penalty;
		}
		else if(missile_elemental == kElementCode_wind)
		{
			level_penalty = (DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_elementLevelWind)-1)*0.03;
			if(cumber_elemental == kElementCode_life)			t_damage *= 1.2f;
			else if(cumber_elemental == kElementCode_fire)		t_damage *= 1.2f;
			else if(cumber_elemental == kElementCode_water)		t_damage *= 0.3f-level_penalty;
		}
		else if(missile_elemental == kElementCode_lightning)
		{
			level_penalty = (DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_elementLevelLightning)-1)*0.03;
			if(cumber_elemental == kElementCode_life)			t_damage *= 1.2f;
			else if(cumber_elemental == kElementCode_fire)		t_damage *= 0.3f-level_penalty;
			else if(cumber_elemental == kElementCode_water)		t_damage *= 1.2f;
		}
		else if(missile_elemental == kElementCode_plasma)
		{
			level_penalty = (DataStorageHub::sharedInstance()->getIntegerForKey(kDSH_Key_elementLevelPlasma)-1)*0.03;
			if(cumber_elemental == kElementCode_life)			t_damage *= 0.3f-level_penalty;
			else if(cumber_elemental == kElementCode_fire)		t_damage *= 1.2f;
			else if(cumber_elemental == kElementCode_water)		t_damage *= 1.2f;
		}
		
		return t_damage;
	}
	
	static ElementCode getReverseElemental(int t_elemental)
	{
		if(t_elemental == kElementCode_fire)			return kElementCode_lightning;
		else if(t_elemental == kElementCode_life)		return kElementCode_plasma;
		else if(t_elemental == kElementCode_water)		return kElementCode_wind;
		else if(t_elemental == kElementCode_wind)		return kElementCode_water;
		else if(t_elemental == kElementCode_lightning)	return kElementCode_fire;
		else if(t_elemental == kElementCode_plasma)		return kElementCode_life;
		else											return kElementCode_empty;
	}
};

#endif
