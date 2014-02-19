//
//  MapScanner.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#ifndef DrawingJack_MapScanner_h
#define DrawingJack_MapScanner_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include <pthread.h>
#include <random>
#include "StartMapLuckyItem.h"

using namespace cocos2d;
using namespace std;
#include <functional>
using namespace placeholders;
class BFS_Point
{
public:
	int x;
	int y;
};

//class BackObj : public CCSprite
//{
//public:
//	static BackObj* create(const char* filename)
//	{
//		BackObj* t_bo = new BackObj();
//		if(t_bo && t_bo->initWithFile(filename))
//		{
//			t_bo->myInit();
//			t_bo->autorelease();
//			return t_bo;
//		}
//		CC_SAFE_DELETE(t_bo);
//		return NULL;
//	}
//	
//	virtual void visit()
//	{
//		kmGLPushMatrix();
//		transform();
//		draw();
//		kmGLPopMatrix();
//	}
//	
//private:
//	
//	void myInit()
//	{
//		
//	}
//};

class InvisibleSprite : public CCNode
{
public:
	static InvisibleSprite* create(const char* filename, bool isPattern);
	
	virtual ~InvisibleSprite()
	{
		
	}
	
private:
	
	void myInit(const char* filename, bool isPattern);
};

//class TestEyeSprite : public CCSprite
//{
//public:
//	static TestEyeSprite* create(const char* filename, CCRect t_rect, CCArray* t_drawRects)
//	{
//		TestEyeSprite* t_tes = new TestEyeSprite();
//		t_tes->myInit(filename, t_rect, t_drawRects);
//		t_tes->autorelease();
//		return t_tes;
//	}
//	
//	virtual void visit()
//	{
//		unsigned int loopCnt = drawRects->count();
//		
//		for(int i=0;i<loopCnt;i++)
//		{
//			IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
//			
//			glEnable(GL_SCISSOR_TEST);
//			
//			int viewport [4];
//			glGetIntegerv (GL_VIEWPORT, viewport);
//			CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
//			float wScale = viewport[2] / rSize.width;
//			float hScale = viewport[3] / rSize.height;
//			float x = t_rect->origin.x*wScale + viewport[0];
//			float y = t_rect->origin.y*hScale + viewport[1];
//			float w = t_rect->size.width*wScale;
//			float h = t_rect->size.height*hScale;
//			glScissor(x,y,w,h);
//			
//			CCSprite::visit();
//			
//			glDisable(GL_SCISSOR_TEST);
//		}
//	}
//	
//private:
//	CCArray* drawRects;
//	
//	void myInit(const char* filename, CCRect t_rect, CCArray* t_drawRects)
//	{
//		initWithFile(filename, t_rect);
//		
//		drawRects = t_drawRects;
//	}
//};

class VisibleSprite : public CCSprite
{
public:
	static VisibleSprite* create(const char* filename, bool isPattern, CCArray* t_drawRects);
	
	void setMoveGamePosition(CCPoint t_p);
	
	CCPoint getMoveGamePosition();
	
	void setSceneNode(CCObject* t_scene_node);
	void visitForThumb();
	void replayVisitForThumb(int temp_time);
	virtual void visit();
	
	void setLight();
	void setDark();
	
private:
	CCArray* drawRects;
	
	CCPoint jack_position;
	CCSize screen_size;
	CCSize design_resolution_size;
	int viewport[4];
	CCNode* scene_node;
	bool is_set_scene_node;
	
	void myInit(const char* filename, bool isPattern, CCArray* t_drawRects);
};

class VisibleParent : public CCNode
{
public:
	static VisibleParent* create(const char* filename, bool isPattern);
	
	void setDrawRects(CCArray* t_rects);
	
	virtual ~VisibleParent()
	{
		drawRects->release();
	}
	
	CCArray* getDrawRects();
	
	CCSprite* getVisibleSprite()
	{
		return myVS;
	}
	
private:
	VisibleSprite* myVS;
	
	CCArray* drawRects;
	
	void divideRect(IntPoint crashPoint);
	
	void setMoveGamePosition(CCPoint t_p);
	
	CCPoint limitted_map_position;
	
	void setLimittedMapPosition();
	
	void changingGameStep(int t_step);
	
	void myInit(const char* filename, bool isPattern);
};

enum MSzorder{
	invisibleZorder = 1,
	visibleZorder,
	blockZorder,
//	topBottomZorder,
	boarderZorder
};

class BackFilename
{
public:
	string filename;
	bool isPattern;
};

class MapScanner : public CCNode
{
public:
	static MapScanner* create();

	void scanMap();
	
	virtual void visit();
	
	void exchangeMS();
	
	void randomingRectView(CCPoint t_p);
	
	void stopRandomingRectView();
	
	void startGame();
	
	CCSprite* getVisibleSprite()
	{
		return visibleImg->getVisibleSprite();
	}
	
private:
	InvisibleSprite* invisibleImg;
	VisibleParent* visibleImg;
	CCSprite* random_rect_img;
	CCNode* blockParent;
	
	int silType;
	
	IntRect init_rect;
	CCSize screen_size;
	int screen_height;
	
	bool my_tic_toc;
	
	CCObject* start_target;
	SEL_CallFunc start_delegate;
	
	void resetRects(bool is_after_scanmap);
	
	IntRect* newRectChecking(IntMoveState start);
	
	void bfsCheck(mapType beforeType, mapType afterType, IntPoint startPoint);
	
	BFS_Point directionVector(IntDirection direction);
	
	bool isInnerMap(BFS_Point t_p);
	
	BackFilename getBackVisibleFilename();
	
	BackFilename getBackInvisibleFilename();
	
	void setMapImg();
	
	CCSprite* top_boarder;
	CCSprite* bottom_boarder;
	CCSprite* left_boarder;
	CCSprite* right_boarder;
	
	CCSpriteBatchNode* top_block_manager;
	CCSpriteBatchNode* top_block_line_manager;
	CCSpriteBatchNode* bottom_block_manager;
	CCSpriteBatchNode* bottom_block_line_manager;
	
	CCSprite* top_block_lock;
	CCSprite* bottom_block_lock;
	
	void setTopBottomBlock();
	StartMapLuckyItem* start_map_lucky_item;
	
	bool is_removed_top_block, is_removed_bottom_block;
	int remove_block_cnt;
	void startRemoveBlock();
	
	void removingBlock();
	
	void showEmptyPoint(CCPoint t_point);
	
	void myInit();
};

#endif
