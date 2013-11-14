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
//#include "DataStorageHub.h"
//#include "ServerDataSave.h"
//#include "SilhouetteData.h"

using namespace cocos2d;
using namespace std;

class MissileDamageData : public CCObject
{
public:
	static int getMissileType(string t_code)
	{
		int return_value;
		
		if(t_code.size() == 2 && t_code[0] >= 'A' && t_code[0] <= 'G' && t_code[1] >= 'A' && t_code[1] <= 'I')
		{
			return_value = t_code[1]-'A';
			return_value += (t_code[0]-'A')*10;
		}
		else
		{
			return_value = 0;
		}
		
		return return_value;
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
