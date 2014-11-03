//
//  PlayTutorial.h
//  DGproto
//
//  Created by 사원3 on 2014. 3. 19..
//
//

#ifndef __DGproto__PlayTutorial__
#define __DGproto__PlayTutorial__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "IntSeries.h"
#include "SelectorDefine.h"
#include "jsoncpp/json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum TutoMapType{
	kTutoMapType_empty = 0,
	kTutoMapType_line,          // 1
	kTutoMapType_get,           // 2
	kTutoMapType_checkLine,     // 3
	kTutoMapType_checkGet,      // 4
	kTutoMapType_newLine,       // 5
	kTutoMapType_scanningEmpty, // 6
	kTutoMapType_newGet         // 7
};

class T_BFS_Point
{
public:
	int x;
	int y;
};

class TutoPathNode : public CCNode
{
public:
	IntPointVector myPointVector;
	CCSprite* pathImg;
	int pathScale;
	
	static TutoPathNode* create(IntPointVector t_pv, string line_color, int color_value)
	{
		TutoPathNode* t_pn = new TutoPathNode();
		t_pn->myInit(t_pv, line_color, color_value);
		t_pn->autorelease();
		return t_pn;
	}
	
	void changeScaleImg(int changeValue)
	{
		pathScale += changeValue;
		pathImg->setScaleX(float(pathScale));
	}
	
private:
	CCSprite* pathEdge;
	
	void myInit(IntPointVector t_pv, string line_color, int color_value)
	{
		myPointVector = t_pv;
		
		ccColor3B t_color;
		t_color.r = color_value/256/256;
		t_color.g = (color_value/256)%256;
		t_color.b = color_value%256;
		
		pathImg = CCSprite::create(("path_" + line_color + ".png").c_str());
		pathImg->setColor(t_color);
		pathImg->setAnchorPoint(ccp(0.0, 0.5));
		pathImg->setRotation(myPointVector.distance.getAngle());
		pathScale = 1;
		pathImg->setScaleX(pathScale);
		pathImg->setScaleY(1.f);
		addChild(pathImg);
		pathImg->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
		
		setPosition(ccp((myPointVector.origin.x)*pixelSize+1, (myPointVector.origin.y)*pixelSize+1));
		
		CCSprite* path_edge = CCSprite::create(("path_edge_" + line_color + ".png").c_str());
		path_edge->setScale(1.15f);
		addChild(path_edge);
		path_edge->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	}
};

class TutoPathManager : public CCNode
{
public:
	static TutoPathManager* create(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height)
	{
		TutoPathManager* t_PM = new TutoPathManager();
		t_PM->myInit(t_getMapData, t_setMapData, t_height);
		t_PM->autorelease();
		return t_PM;
	}
	
	void addPath(IntPointVector t_pv) // t_ms.origin == beforePoint
	{
		if(myList.empty()) // first path
		{
			newPathAdd(t_pv);
			linked_list.clear();
		}
		else if(myList.back()->myPointVector.distance.getAngle() != t_pv.distance.getAngle()) // differ last path
			newPathAdd(t_pv);
		else
			originalExpansion();
		
		IntPoint t_p = IntPoint(t_pv.origin.x, t_pv.origin.y);
		linked_list.push_back(t_p);
	}
	
	void checkBeforeNewline(IntPoint t_p)
	{
		if(getMapData(t_p) == kTutoMapType_newLine)
			overlap_map[t_p.y*240+t_p.x]++;
	}
	
	bool checkRemoveNewline(IntPoint t_p)
	{
		if(overlap_map[t_p.y*240+t_p.x] > 0)
		{
			overlap_map[t_p.y*240+t_p.x]--;
			return false;
		}
		else
		{
			overlap_map[t_p.y*240+t_p.x] = 0;
			return true;
		}
	}
	
	void cleanPath()
	{
		for(int i=0;i<240;i++)
		{
			for(int j=0;j<height_value;j++)
			{
				overlap_map[j*240+i] = 0;
			}
		}
		
		myList.clear();
		
		linked_list.clear();
		
		bool isRemoveTargetNull = false;
		
		while (!isRemoveTargetNull)
		{
			CCNode* t_child = getChildByTag(1);
			if(t_child == NULL)
				isRemoveTargetNull = true;
			else
				removeChildByTag(1, true);
		}
	}
	
	IntPoint pathBackTracking()
	{
		if(!myList.empty())
		{
			linked_list.pop_back();
			
			TutoPathNode* b_node = myList.back();
			
			if(b_node->pathScale > 1) // reduce
			{
				b_node->changeScaleImg(-1);
				IntPointVector b_pv = b_node->myPointVector;
				IntPoint r_p = IntPoint(roundf(b_pv.origin.x + b_pv.distance.dx*b_node->pathScale), roundf(b_pv.origin.y + b_pv.distance.dy*b_node->pathScale));
				if(getMapData(r_p) == kTutoMapType_newLine)
				{
					aloneNewlineRemove(r_p);
					if(checkRemoveNewline(r_p))
						setMapData(r_p, kTutoMapType_empty);
				}
				
				return r_p;
			}
			else // pop back
			{
				IntPointVector b_pv = b_node->myPointVector;
				IntPoint r_p = IntPoint(b_pv.origin.x, b_pv.origin.y);
				
				if(getMapData(r_p) == kTutoMapType_newLine)
				{
					aloneNewlineRemove(r_p);
					if(checkRemoveNewline(r_p))
						setMapData(r_p, kTutoMapType_empty);
				}
				
				myList.pop_back();
				b_node->removeFromParent();
				return r_p;
			}
		}
		else
		{
			return IntPoint();
		}
	}
	
private:
	list<TutoPathNode*> myList;
	vector<IntPoint> linked_list;
	vector<int> overlap_map;
	
	function<TutoMapType(IntPoint)> getMapData;
	function<void(IntPoint, TutoMapType)> setMapData;
	int height_value;
	
	void aloneNewlineRemove(IntPoint r_p)
	{
		for(int i=r_p.x-2;i<=r_p.x+2;i++)
		{
			for(int j=r_p.y-2;j<=r_p.y+2;j++)
			{
				if(i == r_p.x && j == r_p.y)
					continue;
				IntPoint t_p = IntPoint(i,j);
				bool p_valid = t_p.x >= 0 && t_p.x < 240 && t_p.y >= 0 && t_p.y < height_value;
				if(p_valid && getMapData(t_p) == kTutoMapType_newLine)
				{
					if(getMapData(IntPoint(t_p.x-1, t_p.y)) != kTutoMapType_newLine && getMapData(IntPoint(t_p.x, t_p.y-1)) != kTutoMapType_newLine &&
					   getMapData(IntPoint(t_p.x+1, t_p.y)) != kTutoMapType_newLine && getMapData(IntPoint(t_p.x, t_p.y+1)) != kTutoMapType_newLine)
						setMapData(t_p, kTutoMapType_empty);
				}
			}
		}
	}
	
	void originalExpansion()
	{
		myList.back()->changeScaleImg(1);
	}
	
	void newPathAdd(IntPointVector t_pv)
	{
		TutoPathNode* t_pn = TutoPathNode::create(t_pv, path_color, path_color_value);
		t_pn->setTag(1);
		addChild(t_pn, 1);
		
		myList.push_back(t_pn);
	}
	
	string path_color;
	int path_color_value;
	
	void checkLastAddPath(IntPointVector t_pointvector)
	{
		if(myList.empty())
			return;
		TutoPathNode* last_path_node = myList.back();
		if(last_path_node->myPointVector.distance.getAngle() == t_pointvector.distance.getAngle() && last_path_node->pathScale == 1 &&
		   (last_path_node->myPointVector.origin.x != t_pointvector.origin.x || last_path_node->myPointVector.origin.y != t_pointvector.origin.y))
		{
			if(getMapData(t_pointvector.origin) != kTutoMapType_line)
			{
				t_pointvector.origin.x -= t_pointvector.distance.dx;
				t_pointvector.origin.y -= t_pointvector.distance.dy;
			}
			
			last_path_node->myPointVector = t_pointvector;
			last_path_node->setPosition(t_pointvector.origin.convertToCCP());
			last_path_node->changeScaleImg(-1);
		}
	}
	
	void lastPathRemove()
	{
		if(myList.empty())
			return;
		TutoPathNode* last_path_node = myList.back();
		if(last_path_node->pathScale == 1)
		{
			myList.pop_back();
			last_path_node->removeFromParent();
		}
		else
			last_path_node->changeScaleImg(-1);
	}
	
	void myInit(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height);
};

class TutoCharacter : public CCNode
{
public:
	static TutoCharacter* create(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height, function<void(IntPointVector)> t_addPath, function<void(void)> t_cleanPath, function<void(void)> t_scanMap, function<int(void)> t_getRecentStep, function<void(void)> t_nextStep)
	{
		TutoCharacter* t_tc = new TutoCharacter();
		t_tc->myInit(t_getMapData, t_setMapData, t_height, t_addPath, t_cleanPath, t_scanMap, t_getRecentStep, t_nextStep);
		t_tc->autorelease();
		return t_tc;
	}
	
	void changeDirection(IntDirection t_d, IntDirection t_sd);
	bool isDrawingOn;
	bool isMoving;
	
	IntPoint my_point;
	
	void setCharacterPoint(IntPoint t_point);
	
	void rewindAnimation();
	void drawAnimation();
	
	function<void(void)> controlerStop;
    function<bool(void)> is_controler_backing;
	
private:
	
	int height_value;
	
	IntDirection jack_img_direction;
	
	function<TutoMapType(IntPoint)> getMapData;
	function<void(IntPoint, TutoMapType)> setMapData;
	function<void(IntPointVector)> addPath;
	function<void(void)> cleanPath;
	function<void(void)> scanMap;
	
	function<int(void)> getRecentStep;
	function<void(void)> nextStep;
	
	void escapeCharacter();
	
	CCSprite* character_img;
	CCBAnimationManager* character_manager;
    
    CCSprite* line_edge;
	
	void myInit(function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, int t_height, function<void(IntPointVector)> t_addPath, function<void(void)> t_cleanPath, function<void(void)> t_scanMap, function<int(void)> t_getRecentStep, function<void(void)> t_nextStep);
};
class KSLabelTTF;
class TutoControler : public CCLayer
{
public:
	static TutoControler* create(TutoCharacter* t_char, int t_height, function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, function<void(IntPoint)> t_checkBeforeNewLine);
	
	void stopMySchedule();
	bool isBacking;
	
	CCObject* target_main;
	SEL_CallFunc delegate_readyBack;
	SEL_CallFunc pauseBackTracking;
	
	void resetTouch();
	void invisibleControl();
	
	void buttonSetVisible(bool t_b);
	void joystickSetVisible(bool t_b);
	
	virtual ~TutoControler()
	{
		
	}
	
private:
	
	function<TutoMapType(IntPoint)> getMapData;
	function<void(IntPoint, TutoMapType)> setMapData;
	function<void(IntPoint)> checkBeforeNewLine;
	
	TutoCharacter* my_char;
	int height_value;
	
	IntDirection beforeDirection;
	IntDirection beforeSecondDirection;
	
	CCSprite* control_circle;
	CCSprite* control_ball;
	
	CCSprite* draw_button;
	KSLabelTTF* click_label;
	
	CCTouch* button_touch;
	CCTouch* joystick_touch;
	
	float regular_spectrum;
	float irregular_spectrum;
	
	double TUTO_TouchOutWidth;
	double TUTO_JOYSTICK_FOLLOW_DISTANCE;
	double minimumDistanceJ;
	double JoystickCenterLimit;
	
	float joystick_size_value;
	
	bool isButtonAction;
	
	CCBAnimationManager* button_ani;
	
	bool isEnableIrregularDirection;
	bool isControlJoystickNotFixed;
	bool isAlwaysInvisibleJoystick;
	int controlJoystickDirection;
	bool isDisableDrawButton;
	bool isDisableLineOver;
	bool isJoystickCenterNotFixed;
	
	void myInit(TutoCharacter* t_char, int t_height, function<TutoMapType(IntPoint)> t_getMapData, function<void(IntPoint, TutoMapType)> t_setMapData, function<void(IntPoint)> t_checkBeforeNewLine);
	
	void onButton();
	void offButton();
	void touchAction(CCPoint t_p, bool t_b);
	void directionKeeping();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
};

class BackImg : public CCSprite
{
public:
	static BackImg* create()
	{
		BackImg* t_bi = new BackImg();
		t_bi->myInit();
		t_bi->autorelease();
		return t_bi;
	}
	
	virtual void visit();
	void setDrawRects(CCArray* t_rects)
	{
		drawRects->removeAllObjects();
		drawRects->initWithArray(t_rects);
	}
	
	void startSilhouette();
	void stopSilhouette();
	
	virtual ~BackImg()
	{
		if(drawRects)
		{
			drawRects->removeAllObjects();
			drawRects->release();
			drawRects = NULL;
		}
	}
	
private:
	CCArray* drawRects;
	CCSize screen_size;
	CCSize design_resolution_size;
	int viewport[4];
	
	CCSprite* silhouette;
	
	int ing_cnt;
	void silhouetteAction();
	
	void myInit();
};

class LoadingLayer;
class PlayTutorial : public CCLayer
{
public:
	virtual bool init();
	
	static cocos2d::CCScene* scene();
	
	virtual void onEnterTransitionDidFinish();
	
	CREATE_FUNC(PlayTutorial);
	
private:
	
	vector<TutoMapType> map_data;
	int height_value;
	TutoMapType getMapData(int x, int y);
	void setMapData(int x, int y, TutoMapType value);
	
	double back_tracking_cnt;
	int ing_back_tracking_cnt;
	void startBackTracking();
	void backTracking();
	void stopBackTracking();
	
	int tutorial_step;
	
	void scanMap();
	void bfsCheck(TutoMapType beforeType, TutoMapType afterType, IntPoint startPoint);
	void resetRects();
	IntRect* newRectChecking(IntMoveState start);
	
	BackImg* view_img;
	TutoCharacter* character;
	TutoControler* controler;
	TutoPathManager* path_manager;
	
	KSLabelTTF* top_label;
	CCSprite* mark_img;
	
	CCClippingNode* area_take_sample;
	
	int getRecentStep();
	void nextStep();
	
	LoadingLayer* t_loading;
	CCNode* t_scenario_node;
	
	void resultAchieve(Json::Value result_data);
	
	void startCatching();
	void catchingAction();
	void stopCatching();
};


#endif /* defined(__DGproto__PlayTutorial__) */
