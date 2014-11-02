//
//  GachaData.h
//  DGproto
//
//  Created by 사원3 on 2014. 11. 3..
//
//

#ifndef __DGproto__GachaData__
#define __DGproto__GachaData__

#include "KSProtect.h"
#include <vector>

using namespace std;

class GachaRewardData
{
public:
	KSProtectStr type;
	KSProtectVar<int> count;
	GachaRewardData()
	{
		type = "";
		count = 0;
	}
};
class GachaData
{
public:
	KSProtectStr exchangeID;
	vector<GachaRewardData> reward_list;
	KSProtectVar<int> level;
	KSProtectVar<int> percent;
	KSProtectVar<bool> is_take;
	
	GachaData()
	{
		exchangeID = "";
		level = 0;
		percent = 0;
		is_take = false;
	}
};

#endif /* defined(__DGproto__GachaData__) */
