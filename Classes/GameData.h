//
//  GameData.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#ifndef DrawingJack_GameData_h
#define DrawingJack_GameData_h

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "EnumDefine.h"
#include "SelectedMapData.h"
#include "SelectorDefine.h"

using namespace cocos2d;
using namespace std;

#define pixelSize 2

#define myGD GameData::sharedGameData()

enum GAMESCREEN_TYPE{
	kGT_leftUI = -1,
	kGT_full,
	kGT_rightUI
};


float deg2Rad(float x) ;
float rad2Deg(float x);

enum SetMapType{
	kSMT_side = 1,
	kSMT_randRect,
	kSMT_dotLine,
	kSMT_fixRect,
	kSMT_leftRight,
	kSMT_topBottom,
	kSMT_cross,
	kSMT_oneRect,
	kSMT2_sLine,
	kSMT2_cornerPrison,
	kSMT2_centerPrison,
	kSMT2_dddPrison,
	kSMT2_oneLineCenter,
	kSMT2_centerCross,
	kSMT2_man
};

enum mapType{
	mapBlock = -4,
	mapScaningEmptyCumber,
	mapScaningEmptySide,
	mapOutline,
	mapEmpty,
	mapOldline,
	mapOldget,
	mapNewline,
	mapNewget,
	mapScaningCheckLine,
	mapScaningCheckGet
};

enum mapLoopRange{
	mapWidthOutlineBegin = 0,
	mapWidthOutlineEnd = 162,
	mapHeightOutlineBegin = 0,
	mapHeightOutlineEnd = 217,
	
	mapWidthInnerBegin = 1,
	mapWidthInnerEnd = 161,
	mapHeightInnerBegin = 1,
	mapHeightInnerEnd = 216
};

class IntVector : public CCObject
{
public:
	int dx;
	int dy;
	
	IntVector()
	{
		dx = dy = 0;
	}
	
	IntVector(int t_dx, int t_dy)
	{
		dx = t_dx;
		dy = t_dy;
	}
	
	static IntDirection getLeftDirection(IntDirection direction)
	{
		IntDirection return_value;
		if(direction == directionLeft)			return_value = directionDown;
		else if(direction == directionUp)		return_value = directionLeft;
		else if(direction == directionRight)	return_value = directionUp;
		else if(direction == directionDown)		return_value = directionRight;
		else									return_value = directionStop;
		return return_value;
	}
	
	static IntDirection getRightDirection(IntDirection direction)
	{
		IntDirection return_value;
		if(direction == directionLeft)			return_value = directionUp;
		else if(direction == directionUp)		return_value = directionRight;
		else if(direction == directionRight)	return_value = directionDown;
		else if(direction == directionDown)		return_value = directionLeft;
		else									return_value = directionStop;
		return return_value;
	}
	
	static IntDirection getReverseDirection(IntDirection direction)
	{
		IntDirection return_value;
		if(direction == directionLeft)			return_value = directionRight;
		else if(direction == directionUp)		return_value = directionDown;
		else if(direction == directionRight)	return_value = directionLeft;
		else if(direction == directionDown)		return_value = directionUp;
		else									return_value = directionStop;
		return return_value;
	}
	
	static IntVector reverseDirectionVector(IntDirection direction)
	{
		IntVector r_v;
		if(direction == directionLeftUp)			r_v = IntVector(1,	-1);
		else if(direction == directionLeft)			r_v = IntVector(1,	0);
		else if(direction == directionLeftDown)		r_v = IntVector(1,	1);
		else if(direction == directionDown)			r_v = IntVector(0,	1);
		else if(direction == directionRightDown)	r_v = IntVector(-1,	1);
		else if(direction == directionRight)		r_v = IntVector(-1,	0);
		else if(direction == directionRightUp)		r_v = IntVector(-1,	-1);
		else if(direction == directionUp)			r_v = IntVector(0,	-1);
		else										r_v = IntVector(0, 0);
		return r_v;
	}
	
	static IntVector directionVector(IntDirection direction)
	{
		IntVector r_v;
		if(direction == directionLeftUp)			r_v = IntVector(-1,	1);
		else if(direction == directionLeft)			r_v = IntVector(-1,	0);
		else if(direction == directionLeftDown)		r_v = IntVector(-1,	-1);
		else if(direction == directionDown)			r_v = IntVector(0,	-1);
		else if(direction == directionRightDown)	r_v = IntVector(1,	-1);
		else if(direction == directionRight)		r_v = IntVector(1,	0);
		else if(direction == directionRightUp)		r_v = IntVector(1,	1);
		else if(direction == directionUp)			r_v = IntVector(0,	1);
		else										r_v = IntVector(0, 0);
		return r_v;
	}
	
	float getAngle()
	{
		float r_angle;
		if(dx == 0 && dy == 0)				r_angle = 0.f;
		else if(dx < 0 && dy > 0)			r_angle = 135.f;
		else if(dx < 0 && dy == 0)			r_angle = 180.f;
		else if(dx < 0 && dy < 0)			r_angle = -135.f;
		else if(dx == 0 && dy < 0)			r_angle = 90.f;
		else if(dx > 0 && dy < 0)			r_angle = -45.f;
		else if(dx > 0 && dy == 0)			r_angle = 0.f;
		else if(dx > 0 && dy > 0)			r_angle = 45.f;
		else if(dx == 0 && dy > 0)			r_angle = -90.f;
		
		return r_angle;
	}
	
	void multiply(int t_m)
	{
		dx *= t_m;
		dy *= t_m;
	}
};

#define IntPointValueIsNULL		-1

class IntPoint : public CCObject
{
public:
	int x;
	int y;
	
	IntPoint()
	{
		x = y = IntPointValueIsNULL;
	}
	
	IntPoint(int t_x, int t_y)
	{
		x = t_x;
		y = t_y;
	}
	
	bool operator<(const IntPoint& ip) const
	{
		if(x == ip.x)
			return y < ip.y;
		else
			return x < ip.x;
	}
	static IntPoint convertToIntPoint(CCPoint t_p)
	{
		return IntPoint(roundf((t_p.x-1)/pixelSize+1), roundf((t_p.y-1)/pixelSize+1));
	}
	
	CCPoint convertToCCP()
	{
		return ccp((x-1.f)*pixelSize+1.f, (y-1.f)*pixelSize+1.f);
	}
	
	bool isInnerMap()
	{
		if(x >= mapWidthInnerBegin && x < mapWidthInnerEnd && y >= mapHeightInnerBegin && y < mapHeightInnerEnd)
			return true;
		else
			return false;
	}
	
	bool isNull()
	{
		if(x == IntPointValueIsNULL && y == IntPointValueIsNULL)
			return true;
		else
			return false;
	}
};

CCPoint ip2ccp(const IntPoint& ip);
IntPoint ccp2ip(const CCPoint& cc);

typedef void (CCObject::*SEL_CallFuncIp)(IntPoint);
#define callfuncIp_selector(_SELECTOR) (SEL_CallFuncIp)(&_SELECTOR)

class IntPointVector : public CCObject
{
public:
	IntPoint origin;
	IntVector distance;
	
	IntPointVector()
	{
		origin = IntPoint();
		distance = IntVector();
	}
	
	IntPointVector(int t_x, int t_y, int t_dx, int t_dy)
	{
		origin = IntPoint(t_x, t_y);
		distance = IntVector(t_dx, t_dy);
	}
};

typedef void (CCObject::*SEL_CallFuncIpv)(IntPointVector);
#define callfuncIpv_selector(_SELECTOR) (SEL_CallFuncIpv)(&_SELECTOR)

class IntMoveState : public CCObject
{
public:
	IntPoint origin;
	IntDirection direction;
	
	IntMoveState()
	{
		origin = IntPoint();
		direction = directionStop;
	}
	
	IntMoveState(int t_x, int t_y, IntDirection t_d)
	{
		origin = IntPoint(t_x, t_y);
		direction = t_d;
	}
};

class IntSize : public CCObject
{
public:
	int width;
	int height;
	
	IntSize()
	{
		width = height = 0;
	}
	
	IntSize(int t_w, int t_h)
	{
		width = t_w;
		height = t_h;
	}
};

class IntRect : public CCObject
{
public:
	IntPoint origin;
	IntSize size;
	
	IntRect()
	{
		origin = IntPoint();
		size = IntSize();
	}
	
	IntRect(int t_x, int t_y, int t_w, int t_h)
	{
		origin = IntPoint(t_x, t_y);
		size = IntSize(t_w, t_h);
	}
};

typedef void (CCObject::*SEL_CallFuncCCp)(CCPoint);
#define callfuncCCp_selector(_SELECTOR) (SEL_CallFuncCCp)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCpColor)(CCPoint, ccColor4F);
#define callfuncCCpColor_selector(_SELECTOR) (SEL_CallFuncCCpColor)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCpI)(CCPoint, int);
#define callfuncCCpI_selector(_SELECTOR) (SEL_CallFuncCCpI)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCpB)(CCPoint, bool);
#define callfuncCCpB_selector(_SELECTOR) (SEL_CallFuncCCpB)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIIF)(int, int, float);
#define callfuncIIF_selector(_SELECTOR) (SEL_CallFuncIIF)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIIFCCp)(int, int, float, CCPoint);
#define callfuncIIFCCp_selector(_SELECTOR) (SEL_CallFuncIIFCCp)(&_SELECTOR)

typedef int (CCObject::*SEL_ICallFunc)();
#define icallfunc_selector(_SELECTOR) (SEL_ICallFunc)(&_SELECTOR)

typedef float (CCObject::*SEL_FCallFunc)();
#define fcallfunc_selector(_SELECTOR) (SEL_FCallFunc)(&_SELECTOR)




typedef void (CCObject::*SEL_CallFuncCCpODv)(CCPoint, CCObject*, SEL_CallFunc);
#define callfuncCCpODv_selector(_SELECTOR) (SEL_CallFuncCCpODv)(&_SELECTOR)

typedef bool (CCObject::*SEL_BCallFunc)();
#define bcallfunc_selector(_SELECTOR) (SEL_BCallFunc)(&_SELECTOR)

typedef CCNode* (CCObject::*SEL_NCallFunc)();
#define ncallfunc_selector(_SELECTOR) (SEL_NCallFunc)(&_SELECTOR)

typedef CCArray* (CCObject::*SEL_ACallFunc)();
#define acallfunc_selector(_SELECTOR) (SEL_ACallFunc)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncOFF)(CCObject*, float, float);
#define callfuncOFF_selector(_SELECTOR) (SEL_CallFuncOFF)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIpOC)(IntPoint, CCObject*, SEL_CallFuncI);
#define callfuncIpOC_selector(_SELECTOR) (SEL_CallFuncIpOC)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIpIII)(IntPoint, int, int, int);
#define callfuncIpIII_selector(_SELECTOR) (SEL_CallFuncIpIII)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncFBCCp)(float, bool, CCPoint);
#define callfuncFBCCp_selector(_SELECTOR) (SEL_CallFuncFBCCp)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncTDTD)(CCObject*, SEL_CallFunc, CCObject*, SEL_CallFunc);
#define callfuncTDTD_selector(_SELECTOR) (SEL_CallFuncTDTD)(&_SELECTOR)
			
typedef void (CCObject::*SEL_CallFuncII)(int, int);
#define callfuncII_selector(_SELECTOR) (SEL_CallFuncII)(&_SELECTOR)

#include <functional>
#include <map>
#include <string>

class GameData : public CCObject
{
public:
	std::map<std::string, std::function<void(void)>> V_V;
	
	std::map<std::string, std::function<void(bool)>> V_B;
	std::map<std::string, std::function<void(int)>> V_I;
	std::map<std::string, std::function<void(float)>> V_F;
	std::map<std::string, std::function<void(IntPoint)>> V_Ip;
	std::map<std::string, std::function<void(CCObject*)>> V_CCO;
	std::map<std::string, std::function<void(CCPoint)>> V_CCP;
	std::map<std::string, std::function<void(CCPoint, ccColor4F)>> V_CCPCOLOR;
	std::map<std::string, std::function<void(CCPoint, int)>> V_CCPI;
	std::map<std::string, std::function<void(CCPoint, bool)>> V_CCPB;
	std::map<std::string, std::function<void(int, int, float)>> V_IIF;
	std::map<std::string, std::function<void(IntPointVector)>> V_Ipv;
	
	std::map<std::string, std::function<void(int, int, float, CCPoint)>> V_IIFCCP;
	std::map<std::string, std::function<int(void)>> I_V;
	std::map<std::string, std::function<float(void)>> F_V;
	
	std::map<std::string, std::function<void(CCPoint, CCObject*, SEL_CallFunc)>> V_CCPCCOCallfunc;
	std::map<std::string, std::function<bool(void)>> B_V;
	std::map<std::string, std::function<CCNode*(void)>> CCN_V;
	std::map<std::string, std::function<CCArray*(void)>> CCA_V;
	std::map<std::string, std::function<void(CCObject*, float, float)>> V_CCOFF;
	std::map<std::string, std::function<void(IntPoint, CCObject*, SEL_CallFuncI)>> V_IpCCOCallfunci;
	std::map<std::string, std::function<void(IntPoint, int, int, int)>> V_IpIII;
	std::map<std::string, std::function<void(float, bool, CCPoint)>> V_FBCCP;
	std::map<std::string, std::function<void(CCObject*, SEL_CallFunc, CCObject*, SEL_CallFunc)>> V_TDTD;

	
	mapType mapState[162][217];
	
	static GameData* sharedGameData()
	{
		static GameData* t_GD = NULL;
		if(t_GD == NULL)
		{
			t_GD = new GameData();
			t_GD->myInit();
		}
		return t_GD;
	}
	
	virtual ~GameData()
	{
		jackPoint->release();
		mainCumberPoint->release();
		otherTargetPoints->release();
	}
	
	void resetGameData();
	
	void changeJackBaseSpeed(float t_s);
	
	void initStartRect();
	
	void setInitRect(IntPoint initPoint, IntSize initSize);
	
	void removeMapNewline();
	
	void setJackPoint(IntPoint t_jp);
	
	IntPoint getJackPoint();
	
	void setMainCumberPoint(IntPoint t_mcbp);
	
	IntPoint getMainCumberPoint();
	
	int getCommunication(string funcName);
	
	bool getCommunicationBool(string funcName);
	
	float getAlphaSpeed();
	
	void setAlphaSpeed(float t_f);
	
	CCNode* getCommunicationNode(string funcName)
	{
		CCAssert(CCN_V.find(funcName) != CCN_V.end(), funcName.c_str());
		return CCN_V[funcName]();

	}
	
	CCArray* getCommunicationArray(string funcName)
	{
		CCAssert(CCA_V.find(funcName) != CCA_V.end(), funcName.c_str());
		return CCA_V[funcName]();
		
	}
	
	void communication(string funcName)
	{
		CCAssert(V_V.find(funcName) != V_V.end(), funcName.c_str());
		V_V[funcName]();
		return;
	}
	
	void communication(string funcName, CCPoint t_p, int t_i)
	{
		CCAssert(V_CCPI.find(funcName) != V_CCPI.end(), funcName.c_str());
		V_CCPI[funcName](t_p, t_i);
		return;
	}
	
	void communication(string funcName, float t_f, bool t_b, CCPoint t_p)
	{
		CCAssert(V_FBCCP.find(funcName) != V_FBCCP.end(), funcName.c_str());
		V_FBCCP[funcName](t_f, t_b, t_p);
		return;
	}
	
	void communication(string funcName, bool t_b)
	{
		CCAssert(V_B.find(funcName) != V_B.end(), funcName.c_str());
		V_B[funcName](t_b);
		return;
	}
	
	void communication(string funcName, CCObject* t_obj)
	{
		CCAssert(V_CCO.find(funcName) != V_CCO.end(), funcName.c_str());
		V_CCO[funcName](t_obj);
		return;
	}
	
	void communication(string funcName, CCObject* t_obj, float f_val, float f_val2)
	{
		CCAssert(V_CCOFF.find(funcName) != V_CCOFF.end(), funcName.c_str());
		V_CCOFF[funcName](t_obj, f_val, f_val2);
		return;
	}
	
	void communication(string funcName, IntPointVector t_addPath)
	{
		CCAssert(V_Ipv.find(funcName) != V_Ipv.end(), funcName.c_str());
		V_Ipv[funcName](t_addPath);
		return;
	}
	
	void communication(string funcName, IntPoint t_p)
	{
		CCAssert(V_Ip.find(funcName) != V_Ip.end(), funcName.c_str());
		V_Ip[funcName](t_p);
		return;
	}
	
	void communication(string funcName, IntPoint t_p, int t_i1, int t_i2, int t_i3)
	{
		CCAssert(V_IpIII.find(funcName) != V_IpIII.end(), funcName.c_str());
		V_IpIII[funcName](t_p, t_i1, t_i2, t_i3);
		return;
	}
	
	void communication(string funcName, IntPoint t_p, CCObject* t_t, SEL_CallFuncI t_d)
	{
		CCAssert(V_IpCCOCallfunci.find(funcName) != V_IpCCOCallfunci.end(), funcName.c_str());
		V_IpCCOCallfunci[funcName](t_p, t_t, t_d);
		return;
	}
	
	void communication(string funcName, CCPoint t_p, CCObject* t_target, SEL_CallFunc d_func)
	{
		CCAssert(V_CCPCCOCallfunc.find(funcName) != V_CCPCCOCallfunc.end(), funcName.c_str());
		V_CCPCCOCallfunc[funcName](t_p, t_target, d_func);
		return;
	}
	
	void communication(string funcName, CCPoint t_startFire, ccColor4F t_color)
	{
		CCAssert(V_CCPCOLOR.find(funcName) != V_CCPCOLOR.end(), funcName.c_str());
		V_CCPCOLOR[funcName](t_startFire, t_color);
		return;
//		else if(funcName == "Main_movePosition" && target_Main)
//			(target_Main->*delegate_Main_movePosition)(t_startFire);
//		else if(funcName == "VS_setMovePosition" && target_VS)
//			(target_VS->*delegate_VS_setMovePosition)(t_startFire);
	}
	
	void communication(string funcName, CCPoint t_startFire, bool t_b)
	{
		CCAssert(V_CCPB.find(funcName) != V_CCPB.end(), funcName.c_str());
		V_CCPB[funcName](t_startFire, t_b);
		return;
	}
	
	void communication(string funcName, int t_int)
	{
		CCAssert(V_I.find(funcName) != V_I.end(), funcName.c_str());
		V_I[funcName](t_int);
		return;
	}
	
	void communication(string funcName, int t_int1, int t_int2, float t_float)
	{
		CCAssert(V_IIF.find(funcName) != V_IIF.end(), funcName.c_str());
		V_IIF[funcName](t_int1, t_int2, t_float);
		return;
	}
	
	void communication(string funcName, int t_int1, int t_int2, float t_float, CCPoint t_p)
	{
		CCAssert(V_IIFCCP.find(funcName) != V_IIFCCP.end(), funcName.c_str());
		V_IIFCCP[funcName](t_int1, t_int2, t_float, t_p);
		return;
	}
	
	void communication(string funcName, float t_float)
	{
		CCAssert(V_F.find(funcName) != V_F.end(), funcName.c_str());
		V_F[funcName](t_float);
		return;
	}
	
	void communication(string funcName, CCPoint t_p)
	{
		CCAssert(V_CCP.find(funcName) != V_CCP.end(), funcName.c_str());
		V_CCP[funcName](t_p);
		return;
	}
	
	void communication(string funcName, CCObject* t_t1, SEL_CallFunc t_d1, CCObject* t_t2, SEL_CallFunc t_d2)
	{
		CCAssert(V_TDTD.find(funcName) != V_TDTD.end(), funcName.c_str());
		V_TDTD[funcName](t_t1, t_d1, t_t2, t_d2);
		return;
	}
	
	
	void setJackSpeed(float t_s);
	
	void setJackState(int t_s);
	
	int getJackState();
	
	void setIsGameover(bool t_b);
	
	bool getIsGameover();
	
	SetMapType getRecentMapType();
	
	float jack_base_speed;
	
	void initUserSelectedStartRect(IntRect t_rect);
	
	float boarder_value;
	GAMESCREEN_TYPE gamescreen_type;
	float game_scale;
	
	void setUItype(GAMESCREEN_TYPE t_type);
	
private:
	
	SetMapType recent_map_type;
	
	int jackState;
	IntPoint* jackPoint;
	IntPoint* mainCumberPoint;
	CCArray* otherTargetPoints;
	
	CCObject* target_Main;
		
	bool isGameover;
	
	void myInit();
	
	void setStartMap(SetMapType set_map_type);
};

extern GameData* gameData;

#endif
