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
#include "SelectorDefine.h"
#include <algorithm>

using namespace cocos2d;
using namespace std;

#define myGD GameData::sharedGameData()

enum GAMESCREEN_TYPE{
	kGT_leftUI = -1,
	kGT_full,
	kGT_rightUI
};

enum GAMESTEP{
	kGS_limited = 0,
	kGS_changing,
	kGS_unlimited
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
	mapBlock = -4, // 잠금에서 쓰는지 확인
	mapScaningEmptyCumber, // 스캐닝들은 연산 중간에 쓰는거
	mapScaningEmptySide, // 연산 중간에 쓰는거
	mapOutline, // 아래쪽 아웃라인 (완전히 바깥라인) & 잠금 영역도 이것으로 씀.
	mapEmpty, // 아무것도 없는거(획득할 수 있는 영역)
	mapOldline, // 획득한 라인
	mapOldget, // 획득한 영역
	mapNewline, // 긋고 있는 선
	mapNewget, // 스캔중에 임시로 쓰는거.
	mapScaningCheckLine, // 임시로 쓰는거
	mapScaningCheckGet, // 연산중에 임시로 쓰는거
	mapRealNewline // 연산 할 때 쓰는거..
};


enum JackBaseSpeedTag{
	kJackBaseSpeedTag_level1 = 0,
	kJackBaseSpeedTag_level2,
	kJackBaseSpeedTag_level3
};

#include <functional>
#include <map>
#include <string>

class KSCumberBase;
class MissileParent;
class CumberParent;
class GameData : public CCObject
{
public:
	std::map<std::string, std::function<void(void)>> V_V;
	
	std::map<std::string, std::function<void(bool)>> V_B;
	std::map<std::string, std::function<void(int)>> V_I;
	std::map<std::string, std::function<void(float)>> V_F;
	std::map<std::string, std::function<void(float, bool)>> V_FB;
//	std::map<std::string, std::function<bool(KSCumberBase*, float, float)>> B_CumberBaseFF;
	std::map<std::string, std::function<void(IntPoint)>> V_Ip;
	std::map<std::string, std::function<void(IntRect)>> V_Ir;
	std::map<std::string, std::function<void(CCObject*)>> V_CCO;
	std::map<std::string, std::function<void(CCPoint)>> V_CCP;
	std::map<std::string, std::function<void(CCPoint, ccColor4F, float)>> V_CCPCOLORF;
	std::map<std::string, std::function<int(CCPoint, std::string&, KSCumberBase*, bool)>> I_CCPStrCumberBaseB;
	std::map<std::string, std::function<void(CCPoint, int)>> V_CCPI;
	
	std::map<std::string, std::function<void(CCPoint, bool)>> V_CCPB;
	std::map<std::string, std::function<void(int, int, float)>> V_IIF;
	std::map<std::string, std::function<void(IntPointVector)>> V_Ipv;
	
	std::map<std::string, std::function<void(int, int, float, CCPoint)>> V_IIFCCP;
	std::map<std::string, std::function<int(void)>> I_V;
	std::map<std::string, std::function<float(void)>> F_V;
	
	std::map<std::string, std::function<void(CCPoint, CCObject*, SEL_CallFuncO)>> V_CCPCCOCallfuncO;
	std::map<std::string, std::function<bool(void)>> B_V;
	std::map<std::string, std::function<bool(IntPoint)>> B_Ip;
	std::map<std::string, std::function<CCNode*(void)>> CCN_V;
	std::map<std::string, std::function<CCArray*(void)>> CCA_V;
	//std::map<std::string, std::function<std::vector<KSCumberBase*>&(void)>> CCA_V;
	std::map<std::string, std::function<bool(CCObject*, float, float, bool, bool)>> B_CCOFFBB;
	std::map<std::string, std::function<bool(CCObject*, float, float)>> B_CCOFF;
	std::map<std::string, std::function<void(IntPoint, CCObject*, SEL_CallFuncI)>> V_IpCCOCallfunci;
	std::map<std::string, std::function<void(IntPoint, int, int, int)>> V_IpIII;
	std::map<std::string, std::function<void(float, bool, CCPoint)>> V_FBCCP;
	std::map<std::string, std::function<void(CCObject*, SEL_CallFunc, CCObject*, SEL_CallFunc)>> V_TDTD;
	std::map<std::string, std::function<CCPoint(void)>> CCP_V;
	std::map<std::string, std::function<void(std::string)>> V_Str;
	std::map<std::string, std::function<void(CCPoint, float)>> V_CCPF;

	
	std::function<std::vector<KSCumberBase*>&(void)> getMainCumberVector;
	std::function<std::vector<KSCumberBase*>&(void)> getSubCumberVector;
	std::function<void(const std::string& fileName, const std::string& type)> showDetailMessage;
	std::function<void(StoneType, int level, float percent, CCPoint initPosition, int missile_damage)> createJackMissileWithStoneFunctor;
	std::function<void(void)> hideBosses;
	std::function<void(void)> toFun;
	std::function<void(void)> toScratch;
	std::function<void(void)> removeAllPattern;
	std::function<CumberParent*(void)> getCumberParent;
	std::function<MissileParent*(void)> getMissileParent;
	vector<CCNode*> getMainCumberCCNodeVector()
	{
		vector<KSCumberBase*> t_vector = getMainCumberVector();
		vector<CCNode*> return_vector;
		for(int i=0;i<t_vector.size();i++)
			return_vector.push_back((CCNode*)t_vector[i]);
		
		return return_vector;
	}
	vector<CCNode*> getSubCumberCCNodeVector()
	{
		vector<KSCumberBase*> t_vector = getSubCumberVector();
		vector<CCNode*> return_vector;
		for(int i=0;i<t_vector.size();i++)
			return_vector.push_back((CCNode*)t_vector[i]);
		
		return return_vector;
	}
	
	int getMainCumberCount()
	{
		return getMainCumberVector().size();
	}
	bool isValidMainCumber(CCNode* t_boss)
	{
		vector<CCNode*> boss_vector = getMainCumberCCNodeVector();
		auto iter = find(boss_vector.begin(), boss_vector.end(), t_boss);
		if(iter != boss_vector.end())
			return true;
		return false;
	}
	
	int getSubCumberCount()
	{
		return getSubCumberVector().size();
	}
	bool isValidSubCumber(CCNode* t_monster)
	{
		vector<CCNode*> monster_vector = getSubCumberCCNodeVector();
		auto iter = find(monster_vector.begin(), monster_vector.end(), t_monster);
		if(iter != monster_vector.end())
			return true;
		return false;
	}
	
	mapType mapState[mapWidthOutlineEnd][mapHeightOutlineEnd];
	
	static GameData* sharedGameData();
	
	virtual ~GameData()
	{
		jackPoint->release();
		mainCumberPoint->release();
		otherTargetPoints->release();
	}
	
	void resetGameData();
	
	void changeJackBaseSpeed(float t_s);
	
	void setInitRect(IntPoint initPoint, IntSize initSize);
	
	void removeMapNewline();
	
	void setJackPoint(IntPoint t_jp);
	IntPoint getJackPoint();
	
	void setMainCumberPoint(KSCumberBase* cb, IntPoint t_mcbp);
	IntPoint getMainCumberPoint(CCNode* cb)
	{
		return mainCumberPoints[(KSCumberBase*)cb];
	}
	float getAlphaSpeed();
	void setAlphaSpeed(float t_f);
	
	int getCommunication(string funcName);
	bool getCommunicationBool(string funcName);
	bool getCommunicationBool(string funcName, IntPoint t_p);
	CCPoint getCommunicationCCPoint(string funcName);
	CCNode* getCommunicationNode(string funcName);
	CCArray* getCommunicationArray(string funcName);
	float Fcommunication(string funcName);
	int communication(std::string &funcName, CCPoint t_p, std::string &t_i, KSCumberBase* cb, bool t_b);
	//bool communication(string funcName, CCObject* t_obj, float f_val, float f_val2);
	
	bool communication(string funcName, CCObject* t_obj, float f_val, float f_val2, bool b1, bool b2);
	void communication(string funcName);
	void communication(string funcName, CCPoint t_p, int t_i);
	void communication(string funcName, float t_f, bool t_b, CCPoint t_p);
	void communication(string funcName, bool t_b);
	void communication(string funcName, CCObject* t_obj);
	void communication(string funcName, IntPointVector t_addPath);
	void communication(string funcName, IntPoint t_p);
	void communication(string funcName, IntRect t_r);
	void communication(string funcName, IntPoint t_p, int t_i1, int t_i2, int t_i3);
	void communication(string funcName, IntPoint t_p, CCObject* t_t, SEL_CallFuncI t_d);
	void communication(string funcName, CCPoint t_p, CCObject* t_target, SEL_CallFuncO d_func);
	void communication(string funcName, CCPoint t_startFire, ccColor4F t_color, float t_angle);
	void communication(string funcName, CCPoint t_startFire, bool t_b);
	void communication(string funcName, int t_int);
	void communication(string funcName, int t_int1, int t_int2, float t_float);
	void communication(string funcName, int t_int1, int t_int2, float t_float, CCPoint t_p);
	void communication(string funcName, float t_float);
	void communication(string funcName, float t_float, bool t_bool);
	void communication(string funcName, CCPoint t_p);
	void communication(string funcName, CCObject* t_t1, SEL_CallFunc t_d1, CCObject* t_t2, SEL_CallFunc t_d2);
	void communication(string funcName, string t_str);
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
	
	GAMESTEP game_step;
	int limited_step_top;
	int limited_step_bottom;
	bool is_setted_jack;
	bool getEmptyRandomPoint(IntPoint* point, float radius);
	
	bool is_changed_map;
	bool is_need_resetRects;
	
private:
	
	SetMapType recent_map_type;
	
	int jackState;
	IntPoint* jackPoint;
	IntPoint* mainCumberPoint;

	std::map<KSCumberBase*, IntPoint> mainCumberPoints;
	CCArray* otherTargetPoints;
	
	CCObject* target_Main;
		
	bool isGameover;
	
	void myInit();
	
	void setStartMap(SetMapType set_map_type);


	CC_SYNTHESIZE(bool, m_jackIsUnbeatable, JackIsUnbeatable);
};



#endif
