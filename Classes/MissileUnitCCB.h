//
//  MissileUnitCCBCCB.h
//  DGproto
//
//  Created by ksoo k on 2014. 5. 22..
//
//

#ifndef DGproto_MissileUnitCCBCCB_h
#define DGproto_MissileUnitCCBCCB_h



#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include "FromTo.h"
#include "Well512.h"
#include <unordered_map>
#include <queue>
#include <map>
#include <functional>
#include <set>
#include "IntSeries.h"
USING_NS_CC_EXT;
USING_NS_CC;
#define MAX_SCALE_X	10.f
#define ADD_SCALE_X	2.5f

class MissileUnitCCB : public CCNode
{
public:
	static MissileUnitCCB * create (CCPoint t_sp, float t_angle, float t_distance, std::string imgFilename, CCSize t_cs, float t_da, float t_reduce_da);
private:
	float angle;
	float distance;
	CCSize crashSize;
	float da;
	float reduce_da;
	float is_checking;
	void startMove ();
	void stopMove ();
	void removeEffect ();
	void selfRemove ();
	void move ();
	void myInit (CCPoint t_sp, float t_angle, float t_distance, const std::string& ccbi, CCSize t_cs, float t_da, float t_reduce_da);
};


#endif
