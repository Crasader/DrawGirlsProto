// MissileUnit.h
//

#ifndef LZZ_MissileUnit_h
#define LZZ_MissileUnit_h
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
#define LZZ_INLINE inline
class MissileUnit : public CCSprite
{
public:
	static MissileUnit * create (CCPoint t_sp, float t_angle, float t_distance, std::string imgFilename, CCSize t_cs, float t_da, float t_reduce_da,
															 bool isSuper = false);
private:
	bool isSuper;
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
	void myInit (CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, float t_da, float t_reduce_da, bool isSuper);
};
class MissileUnit2 : public CCSprite
{
public:
	static MissileUnit2 * create (CCPoint t_sp, float t_angle, float t_distance, std::string imgFilename, CCSize t_cs, float t_da, float t_reduce_da);
private:
	float angle;
	float distance;
	CCSize crashSize;
	float da;
	float reduce_da;
	bool is_checking;
	void startMove ();
	void stopMove ();
	void removeEffect ();
	void selfRemove ();
	void move ();
	void myInit (CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, float t_da, float t_reduce_da);
};
class MissileUnit3 : public CCNode
{
public:
	static MissileUnit3 * create (int t_type, float t_distance, CCSize t_mSize, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
private:
	CCSprite* stoneSprite;
	int myType;
	float distance;
	bool shownWarning;
	CCSize mSize;
	CCObject * target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	void startMove ();
	void stopMove ();
	void removeEffect ();
	void selfRemove ();
	void move ();
	void myInit (int t_type, float t_distance, CCSize t_mSize, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
};
class Targeting : public CCSprite
{
public:
	static Targeting * create ();
	void remove ();
private:
	int rotate_value;
	void startMyRotate ();
	void myRotate ();
	void myInit ();
};
class TG_Shoot : public CCNode
{
public:
	static TG_Shoot * create (int t_type, CCPoint t_sp, CCPoint t_tp, int t_loadFrame, int t_shootFrame, Targeting * t_targeting, CCSize t_mSize, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
private:
	Targeting * remove_targeting;
	CCSize mSize;
	CCObject * target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	int type;
	CCPoint sp;
	CCPoint tp;
	int loadFrame;
	float loadScaleRate;
	float loadMaxScale;
	CCPoint load_dv;
	int shootFrame;
	float shootScaleRate;
	float shootMaxScale;
	CCPoint shoot_dv;
	int bombFrame;
	int ingFrame;
	CCSprite * loadImg;
	bool loadRemoving;
	CCSprite * shootImg;
	bool shootRemoving;
	CCSprite * bombImg;
	void startMove ();
	void stopMove ();
	void move ();
	void myInit (int t_type, CCPoint t_sp, CCPoint t_tp, int t_loadFrame, int t_shootFrame, Targeting * t_targeting, CCSize t_mSize, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
};
class DeathSwing : public CCNode
{
public:
	static DeathSwing * create (CCPoint t_sp, int t_rotate);
	void startAnimation ();
	void selfRemove ();
private:
	CCSprite * swingImg;
	void myInit (CCPoint t_sp, int t_rotate);
};
class CrashMapObject : public CCNode
{
public:
	bool crashMapForIntPoint (IntPoint t_p);
	int crashMapForIntRect (IntRect t_r);
	virtual void jackDie () = 0;
	virtual void lineDie (IntPoint t_p) = 0;
};
class WindmillObject : public CCSprite
{
public:
	static WindmillObject * create (IntPoint t_sp, int t_thornsFrame);
	void startMyAction ();
private:
	int thornsFrame;
	int ingFrame;
	bool is_action;
	IntPoint myPoint;
	void myAction ();
	void stopMyAction ();
	void myInit (IntPoint t_sp, int t_thornsFrame);
};
class ThrowObject : public CrashMapObject
{
public:
	ThrowObject()
	{
		crashCount = 0;
	}
	virtual ~ThrowObject()
	{
		CCLOG("ThrowObject Destroy");
	}
	static ThrowObject * create (CCPoint t_sp, int t_type, float t_speed, float t_angle, IntSize t_mSize);
	void startMyAction ();
private:
	CCNode* objects;
	int type;
	int random_spin;
	IntSize mSize;
	CCPoint dv;
	CCSprite * objImg;
	CCPoint b_c_p;
	int crashCount;
	void jackDie ();
	void lineDie (IntPoint t_p);
	void removeEffect ();
	void selfRemove ();
	void stopMyAction ();
	void myAction ();
	void myInit (CCPoint t_sp, int t_type, float t_speed, float t_angle, IntSize t_mSize);
};
class SB_FallStar : public CCNode
{
public:
	static SB_FallStar * create (int t_type);
	void startFalling ();
private:
	CCSprite * myStar;
	int dr;
	float vy;
	float dy;
	void falling ();
	void myInit (int t_type);
};
class SatelliteBeam : public CrashMapObject
{
public:
	static SatelliteBeam * create (CCPoint t_sp, int t_type, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
	void beamSetPosition (CCPoint t_p);
private:
	CCSprite * beam_main;
	FromToWithDuration <float> fadeFromToDuration;
	int type;
	CCObject * target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	void jackDie ();
	void lineDie (IntPoint t_p);
	void startFallingStar ();
	void fallingStar ();
	void myInit (CCPoint t_sp, int t_type, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
};
class FM_Targeting : public CCNode
{
public:
	static FM_Targeting * create (string imgFilename, CCPoint t_sp, int t_aniFrame, float t_sSize, float t_fSize, float t_sAngle, float t_fAngle, float inDegree);
	virtual ~FM_Targeting()
	{
		CCLOG("FM_Targeting destroy");
	}
	void startAction ();
private:
	CCSprite * targetingImg;
	float duration;
	float fSize;
	float rotateValue;
	void myInit (string imgFilename, CCPoint t_sp, int t_aniFrame, float t_sSize, float t_fSize, float t_sAngle, float t_fAngle, float inDegree);
};
class FallMeteor : public CrashMapObject
{
public:
	static FallMeteor * create (string t_imgFilename, int imgFrameCnt, CCSize imgFrameSize, CCPoint t_sp, CCPoint t_fp, int t_fallFrame, int t_explosionFrame,
															IntSize t_mSize, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect,
															std::function<void(int)> accum);
	virtual ~FallMeteor()
	{
		CCLOG("FallMeteor~");
	}
	void removeEffect ();
	void hidingAnimation (float dt);
private:
	FromToWithDuration <float> fadeFromToDuration;
	CCSprite * meteor;
	string imgFilename;
	FromToWithDuration2 <CCPoint> meteorChanger;
	CCPoint fp;
	int fallFrame;
	int explosionFrame;
	int ingFrame;
	IntSize mSize;
	CCPoint fall_dv;
	CCObject * target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	FM_Targeting* m_targetSprite;
	std::function<void(int)> accumer;
	int crashCount;
	void jackDie ();
	void lineDie (IntPoint t_p);
	void finalCrash ();
	void stopFall ();
	void fall ();
	void selfRemove ();
	void initParticle ();
	void myInit (string t_imgFilename, int imgFrameCnt, CCSize imgFrameSize, CCPoint t_sp,
							 CCPoint t_fp, int t_fallFrame, int t_explosionFrame, IntSize t_mSize,
							 CCObject * t_removeEffect, SEL_CallFunc d_removeEffect, std::function<void(int)> accum);
};
class Lazer_Ring : public CCNode
{
public:
	static Lazer_Ring * create (float t_ring_angle, CCPoint t_ring_sP, CCPoint t_ring_fP, float t_ring_sS, float t_ring_fS, int t_frame, ccColor3B t_color);
private:
	CCPoint dv;
	float ds;
	int frame;
	int ingFrame;
	CCSprite * ringImg;
	void startMyAction ();
	void stopMyAction ();
	void myAction ();
	void myInit (float t_ring_angle, CCPoint t_ring_sP, CCPoint t_ring_fP, float t_ring_sS, float t_ring_fS, int t_frame, ccColor3B t_color);
};
class ThreeCushion : public CrashMapObject
{
public:
	static ThreeCushion * create (CCPoint t_sp, float t_speed, int t_cushion_cnt, bool t_is_big_bomb, int t_crashArea, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
	void removeEffect ();
private:
	float speed;
	int angle;
	int crashArea;
	CCSprite * baseNode;
	CCLabelAtlas * cntLabel;
	int cushionCnt;
	bool is_big_bomb;
	CCObject * target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	void startMyAction ();
	void stopMyAction ();
	void myAction ();
	void selfRemove ();
	void jackDie ();
	void lineDie (IntPoint t_p);
	void crashMap ();
	CCPoint getAfterPosition (CCPoint b_p, int t_angle);
	CCPoint judgeAnglePoint (CCPoint b_p);
	void myInit (CCPoint t_sp, float t_speed, int t_cushion_cnt, bool t_is_big_bomb, int t_crashArea, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
	void initParticle ();
};
class TickingTimeBomb : public CrashMapObject
{
public:
	static TickingTimeBomb * create (IntPoint t_setPoint, int t_bombFrameOneTime, int t_bombTimes, int t_crashArea, int t_rangeCode, CCArray * t_tickingArray, CCObject * t_resetTickingTimeBomb, SEL_CallFunc d_resetTickingTimeBomb);
	IntPoint getSettedPoint ();
	virtual ~TickingTimeBomb()
	{
		CCLOG("z");
	}
private:
	IntPoint setPoint;
	int bombFrameOneTime;
	int bombTimes;
	int ingFrame;
	int rangeCode;
	int crashArea;
	CCSprite * ticking_main;
	CCLabelAtlas * ticking_label;
	CCArray * tickingArray;
	CCObject * target_resetTickingTimeBomb;
	SEL_CallFunc delegate_resetTickingTimeBomb;
	void startMyAction ();
	void myAction ();
	void crashMap ();
	void selfRemove ();
	void stopMyAction ();
	void jackDie ();
	void lineDie (IntPoint t_p);
	void myInit (IntPoint t_setPoint, int t_bombFrameOneTime, int t_bombTimes, int t_crashArea, int t_rangeCode, CCArray * t_tickingArray, CCObject * t_resetTickingTimeBomb, SEL_CallFunc d_resetTickingTimeBomb);
	void initParticle ();
};
class SightOut : public CCSprite
{
public:
	static SightOut * create (int totalFrame);
	void startAction ();
private:
	int ingFrame;
	float opa_value;
	int totalFrame;
	void myAction ();
	void stopAction ();
	void myInit (int totalFrame);
};
class BlindDrop : public CCNode, public CCBAnimationManagerDelegate
{
public:
	static BlindDrop * create (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame, float sc);
	void startAction ();
	virtual void completedAnimationSequenceNamed (char const * name);
	void ccbManagerDelegateNull();
	virtual ~BlindDrop()
	{
		CCLOG("~BlindDrop");
	}
private:
	float m_scale;
	CCPoint subPosition;
	int movingFrame;
	int blindFrame;
	int ingFrame;
	CCSprite * oilImg;
	CCBAnimationManager* reader;
	void myAction ();
	void stopAction ();
	void myInit (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int t_blindFrame, float sc);
};
class PrisonObject : public CCSprite
{
public:
	static PrisonObject * create (CCPoint t_sp, int t_type, int t_prisonFrame);
	void startMyAction ();
private:
	int prisonFrame;
	int ingFrame;
	void myAction ();
	void stopMyAction ();
	void myInit (CCPoint t_sp, int t_type, int t_prisonFrame);
};
class BurnFragment : public CrashMapObject
{
public:
	static BurnFragment * create (IntPoint t_sp, int t_life, CCNode * t_parent, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
private:
	IntPoint myPoint;
	int myLife;
	pair<CCSprite*, CCBAnimationManager*> fragmentImg;
	int ingFrame;
	CCNode * myParent;
	int mType;
	CCObject * target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	void jackDie ();
	void lineDie (IntPoint t_p);
	void startMyAction ();
	void myAction ();
	void diffusionBF (int t_direction);
	void stopMyAction ();
	void myInit (IntPoint t_sp, int t_life, CCNode * t_parent, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
};
class KSSequenceAndRemove : public CCActionInterval
{
public:
	~ KSSequenceAndRemove ();
public:
	static CCSequence * create (CCNode * thiz, std::initializer_list <CCFiniteTimeAction*> initList);
};
class Firework : public CrashMapObject
{
public:
	virtual ~ Firework ();
	static Firework * create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void crashMapForPoint (IntPoint point, int radius);
	void myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void setTwoStep ();
	void selfRemove (float dt);
	void jackDie ();
	void lineDie (IntPoint t_p);
	void update (float dt);
protected:
	bool m_1TO2;
	int m_numberAtOnce;
	int m_frameInterval;
	CCPoint m_sourcePosition;
	int m_step;
	int m_frame;
	int m_bombFrame;
	CCSprite * m_parentMissile;
	CCSpriteBatchNode * batchNode;
	FromToWithDuration2 <CCPoint> m_parentMissileGoal;
	Json::Value m_pattern;
};
class MovingSunflower : public CrashMapObject
{
public:
	static MovingSunflower * create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void crashMapForPoint (IntPoint point, int radius);
	void myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void setTwoStep ();
	void selfRemove (float dt);
	void jackDie ();
	void lineDie (IntPoint t_p);
	void update (float dt);
protected:
	Json::Value m_pattern;
	CCPoint m_sourcePosition;
	int m_step;
	int m_frame;
	int m_bombFrame;
	std::pair<CCSprite*, CCBAnimationManager*> m_parentMissile;
	CCSpriteBatchNode * batchNode;
	FromToWithDuration2 <CCPoint> m_parentMissileGoal;
	Well512 m_well512;
};
class AlongOfTheLine : public CCNode
{
public:
	static AlongOfTheLine * create (CCPoint cumberPosition, CCPoint jackPosition, int totalFrame, int number, float speed);
	void myInit (CCPoint cumberPosition, CCPoint jackPosition, int totalFrame, int number, float speed);
	void setTwoStep ();
	void hidingAnimation (float dt);
	void update (float dt);
protected:
	struct AlongPath
	{
		IntPoint point;
		IntPoint direction;
		AlongPath (IntPoint const & pt, IntPoint const & dir);
		AlongPath ();
	};
	struct Pollution
	{
		CCSprite * spr;
		CCSprite * goal;
		FromToWithDuration2 <CCPoint> glue;
		AlongPath alongPath;
		int step;
		Pollution ();
	};
	float m_lineSpeed;
	int m_step;
	int m_frame;
	int m_totalFrame;
	FromToWithDuration <float> m_scaleTo;
	std::vector <Pollution> m_pollutions;
	std::map <IntPoint, IntPoint> m_directions;
	Well512 m_well512;
	std::vector <CCSprite*> goalSprites;
};
class ThrowBomb : public CrashMapObject
{
public:
	static ThrowBomb * create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void crashMapForPoint (IntPoint point, int radius);
	void selfRemove (float dt);
	void jackDie ();
	void lineDie (IntPoint t_p);
	void myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void update (float dt);
protected:
	Json::Value m_pattern;
	int m_step;
	int m_frame;
	CCSprite * m_parentMissile;
	Well512 m_well512;
	float m_angle;
};
namespace std
{
	template <>
	struct hash <IntPoint>
	{
		size_t operator () (IntPoint const & ip) const;
	};
}
class ReaverScarab : public CrashMapObject
{
public:
	static ReaverScarab * create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	virtual ~ReaverScarab()
	{
		CCLOG("ReaverScarab destory");
	}
	void crashMapForPoint (IntPoint point, int radius);
	void selfRemove (float dt);
	void jackDie ();
	void lineDie (IntPoint t_p);
	void myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	int lengthToEnd (IntPoint point);
	bool processObject (IntPoint pointFrom, IntPoint pointTo, int distance);
	void aStar (IntPoint endPt);
	void update (float dt);
protected:
	struct CellInfo
	{
		int dx;
		int dy;
		int g;
		int h;
		CellInfo (int _dx, int _dy, int _g, int _h);
		CellInfo ();
		bool operator < (CellInfo const & ci) const;
	};
	Json::Value m_pattern;
	int m_step;
	int m_frame;
	IntPoint m_jackPoint;
	CCParticleSystem * m_parentMissile;
	Well512 m_well512;
	int m_crashArea;
	float m_angle;
	struct CoordAndCellInfo
	{
		int x;
		int y;
		int dx;
		int dy;
		int g;
		int h;
		int order;
		CoordAndCellInfo (int _x, int _y, int _dx, int _dy, int _g, int _h);
		CoordAndCellInfo ();
		bool operator < (CoordAndCellInfo const & ci) const;
		bool operator == (CoordAndCellInfo const & ci) const;
	};
	std::unordered_map <IntPoint, CellInfo> m_closeListMap;
	std::multiset <CoordAndCellInfo, less<CoordAndCellInfo> > m_openList;
	std::vector <CCPoint> m_bulletReversePath;
	std::vector <CCPoint>::reverse_iterator m_bulletIter;
	int m_insertCount;
	CCSprite* m_targetSprite;
};

class CloudBomb : public CCNode
{
public:
	static CloudBomb * create (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void myInit (CCPoint cumberPosition, CCPoint jackPosition, Json::Value pattern);
	void setTwoStep ();
	void selfRemove (float dt);
	void update (float dt);
protected:
	CCPoint m_sourcePosition;
	int m_step;
	int m_frame;
	CCParticleSystem * m_parentMissile;
	float m_bombProb;
	CCSpriteBatchNode * batchNode;
	Well512 m_well512;
	Json::Value m_pattern;
};
class Burn : public CCNode
{
public:
	static Burn * create (CCPoint t_sp, float t_baseDistance, float t_shootAngle, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
	void startMyAction ();
	void removeEffect ();
private:
	CCPoint movingDv;
	int ingFrame;
	bool positionSetted;
	int mType;
	CCObject * target_removeEffect;
	SEL_CallFunc delegate_removeEffect;
	void myAction ();
	void searchAndMoveOldline (IntMoveState searchFirstMoveState);
	void stopMyAction ();
	void myInit (CCPoint t_sp, float t_baseDistance, float t_shootAngle, int t_mType, CCObject * t_removeEffect, SEL_CallFunc d_removeEffect);
};
class PoisonLine : public CCNode
{
public:
	static PoisonLine * create (IntPoint t_sp, int frame);
	void startMyAction ();
private:
	CCParticleSystemQuad* line;
	IntPoint mapPoint;
	int ingFrame;
	int totalFrame;
	void myAction ();
	void stopMyAction ();
	void myInit (IntPoint t_sp, int frame);
};
class PoisonDrop : public CCNode
{
public:
	static PoisonDrop * create (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int area, int totalframe);
	void startAction ();
private:
	int m_area;
	int m_totalFrame;
	CCPoint subPosition;
	int movingFrame;
	int ingFrame;
	CCSprite * dropImg;
	CCSprite * oilImg;
	CCSprite* targetImg;
	void myAction ();
	void initParticle ();
	void stopAction ();
	void selfRemove ();
	void myInit (CCPoint t_sp, CCPoint t_fp, int t_movingFrame, int area, int totalframe);
};
class ReflectionLazer : public CCSprite
{
public:
	static ReflectionLazer * create (CCPoint t_sp, CCPoint t_fp, int t_frame, int t_type);
	void startMyAction ();
private:
	CCPoint dv;
	int move_frame;
	int ing_frame;
	void myAction ();
	void stopMyAction ();
	void selfRemove ();
	void myInit (CCPoint t_sp, CCPoint t_fp, int t_frame, int t_type);
};
class MirrorReflection : public CCNode
{
public:
	static MirrorReflection * create (int t_sn, float t_r, int t_direction, int t_type);
	void startMyAction ();
private:
	float radius;
	int sn;
	int fn;
	int direction;
	int ing_frame;
	int myType;
	int create_frame;
	int base_angle;
	int rand_rotate;
	CCSprite * effectImg;
	CCSprite * impactImg;
	CCPoint getNumberPosition (int t_number);
	void myAction ();
	void stopMyAction ();
	void myInit (int t_sn, float t_r, int t_direction, int t_type);
};
class MeteorStorm : public CCNode
{
public:
	static MeteorStorm * create (CCPoint t_sp, CCPoint t_fp, int t_moving_frame);
private:
	CCParticleSystemQuad * inner_fire_particle;
	CCParticleSystemQuad * outer_fire_particle;
	void myInit (CCPoint t_sp, CCPoint t_fp, int t_moving_frame);
};
class MathmaticalMissileUnit : public CCSprite
{
public:
	enum CurveDisposition
	{
		CURVE = 1,
		RIGHTLINE = 0
	};
	static MathmaticalMissileUnit * create (CCPoint t_sp, float t_angle, float t_speed, string imgFilename, CCSize t_cs, vector <CCPoint> const & path, enum CurveDisposition curve,
																					bool isSuper);
	static MathmaticalMissileUnit * create (CCPoint t_sp, float t_angle, float t_speed, string imgFilename, CCSize t_cs, bool isSuper);
	CCPoint CatMull (CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, float t, CurveDisposition curve);
	double CatMullLength (CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, CurveDisposition curve);
	double IntegralTarget (CCPoint P0, CCPoint P1, CCPoint P2, CCPoint P3, double t);
	double CatMullFunction (double a, double b, double c, double d, double t, CurveDisposition curve);
	double CatMullDerivativeFunction (double a, double b, double c, double d, double t);
	double smps (std::function <double(CCPoint, CCPoint, CCPoint, CCPoint, double)> f, CCPoint A, CCPoint B, CCPoint C, CCPoint D, double a, double b, int n);
	void removeEffect ();
	void selfRemove ();
	void myInit (CCPoint t_sp, float t_angle, float t_distance, CCSize t_cs, vector <CCPoint> const & path, enum CurveDisposition curve, std::string const & fn,
							 bool isSuper);
	CCPoint myFunction (float fc);
	void move (float dt);
protected:
	std::string m_fileName;
	vector <CCPoint> m_catmullPath;
	int m_catmullIndex;
	double m_catmullvar;
	CurveDisposition m_curve;
	CCPoint firePosition;
	float m_angle;
	float m_speed;
	CCSize m_crashSize;
	float m_isChecking;
	int m_frameCount;
	bool m_isSuper;
};
class RunDownSaw : public CrashMapObject
{
public:
	virtual ~RunDownSaw()
	{
		CCLOG("RunDownSaw Destroy");
	}
	static RunDownSaw * create (CCPoint startPosition, float speed, float angleDegree, IntSize mSize, int runDown);
	void startMyAction ();
private:
	float m_speed;
	float m_angleDegree;
	IntSize m_size;
	CCPoint m_lastSawPosition;
	CCSprite * m_objImg;
	int m_crashCount;
	int m_runDown;
	void jackDie ();
	void lineDie (IntPoint t_p);
	void removeEffect ();
	void selfRemove ();
	void stopMyAction ();
	void myAction (float dt);
	void myInit (CCPoint startPosition, float speed, float angleDegree, IntSize mSize, int runDown);
};
#undef LZZ_INLINE
#endif
