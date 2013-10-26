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
#include "SelectedMapData.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"

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

class BackObj : public CCSprite
{
public:
	static BackObj* create(const char* filename)
	{
		BackObj* t_bo = new BackObj();
		if(t_bo && t_bo->initWithFile(filename))
		{
			t_bo->myInit();
			t_bo->autorelease();
			return t_bo;
		}
		CC_SAFE_DELETE(t_bo);
		return NULL;
	}
	
	virtual void visit()
	{
		kmGLPushMatrix();
		transform();
		draw();
		kmGLPopMatrix();
	}
	
private:
	
	void myInit()
	{
		
	}
};

class InvisibleSprite : public CCNode
{
public:
	static InvisibleSprite* create(const char* filename, bool isPattern)
	{
		InvisibleSprite* t_iv = new InvisibleSprite();
		t_iv->myInit(filename, isPattern);
		t_iv->autorelease();
		return t_iv;
	}
	
	virtual ~InvisibleSprite()
	{
		
	}
	
private:
	
	void myInit(const char* filename, bool isPattern)
	{
		CCSprite* t_spr = mySIL->getLoadedImg(filename);
		t_spr->setPosition(ccp(160,215));
		addChild(t_spr);
	}
};

class TestEyeSprite : public CCSprite
{
public:
	static TestEyeSprite* create(const char* filename, CCRect t_rect, CCArray* t_drawRects)
	{
		TestEyeSprite* t_tes = new TestEyeSprite();
		t_tes->myInit(filename, t_rect, t_drawRects);
		t_tes->autorelease();
		return t_tes;
	}
	
	virtual void visit()
	{
		unsigned int loopCnt = drawRects->count();
		
		for(int i=0;i<loopCnt;i++)
		{
			IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
			
			glEnable(GL_SCISSOR_TEST);
			
			int viewport [4];
			glGetIntegerv (GL_VIEWPORT, viewport);
			CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
			float wScale = viewport[2] / rSize.width;
			float hScale = viewport[3] / rSize.height;
			float x = t_rect->origin.x*wScale + viewport[0];
			float y = t_rect->origin.y*hScale + viewport[1];
			float w = t_rect->size.width*wScale;
			float h = t_rect->size.height*hScale;
			glScissor(x,y,w,h);
			
			CCSprite::visit();
			
			glDisable(GL_SCISSOR_TEST);
		}
	}
	
private:
	CCArray* drawRects;
	
	void myInit(const char* filename, CCRect t_rect, CCArray* t_drawRects)
	{
		initWithFile(filename, t_rect);
		
		drawRects = t_drawRects;
	}
};

class VisibleSprite : public CCSprite
{
public:
	static VisibleSprite* create(const char* filename, bool isPattern, CCArray* t_drawRects)
	{
		VisibleSprite* t_v = new VisibleSprite();
		t_v->myInit(filename, isPattern, t_drawRects);
		t_v->autorelease();
		return t_v;
	}
	
	void setMoveGamePosition(CCPoint t_p)
	{
		jack_position = t_p;
	}
	
private:
	CCArray* drawRects;
	
	CCPoint jack_position;
	
	virtual void visit()
	{
		unsigned int loopCnt = drawRects->count();
		
		for(int i=0;i<loopCnt;i++)
		{
			IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
			
			glEnable(GL_SCISSOR_TEST);
			
			int viewport [4];
			glGetIntegerv (GL_VIEWPORT, viewport);
			CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
			float wScale = viewport[2] / rSize.width;
			float hScale = viewport[3] / rSize.height;
			
			float x = (t_rect->origin.x*myGD->game_scale+jack_position.x)*wScale + viewport[0]-1;
			float y = (t_rect->origin.y*myGD->game_scale+jack_position.y)*hScale + viewport[1]-1;
			float w = (t_rect->size.width*myGD->game_scale)*wScale+2;
			float h = (t_rect->size.height*myGD->game_scale)*hScale+2;
			
			glScissor(x,y,w,h);
			
			draw();
			
			glDisable(GL_SCISSOR_TEST);
		}
	}
	
	void myInit(const char* filename, bool isPattern, CCArray* t_drawRects)
	{
		initWithTexture(mySIL->addImage(filename));
		setPosition(ccp(160,215));

		drawRects = t_drawRects;
	}
};

class VisibleParent : public CCNode
{
public:
	static VisibleParent* create(const char* filename, bool isPattern)
	{
		VisibleParent* t_vp = new VisibleParent();
		t_vp->myInit(filename, isPattern);
		t_vp->autorelease();
		return t_vp;
	}
	
	void setDrawRects(CCArray* t_rects)
	{
		drawRects->removeAllObjects();
		drawRects->addObjectsFromArray(t_rects);
	}
	
	virtual ~VisibleParent()
	{
		drawRects->release();
	}
	
	CCArray* getDrawRects()
	{
		return drawRects;
	}
	
private:
	VisibleSprite* myVS;
	
	CCArray* drawRects;
	
	void divideRect(IntPoint crashPoint)
	{
		if(mySD->silData[crashPoint.x][crashPoint.y] == true)
			myGD->communication("UI_decreasePercentage");
		
		crashPoint.x = (crashPoint.x-1)*pixelSize;
		crashPoint.y = (crashPoint.y-1)*pixelSize;
		
		vector<IntRect*> removeArray;
		int loopCnt = drawRects->count();
		for(int i=0;i<loopCnt;i++)
		{
			IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
			if(crashPoint.x >= t_rect->origin.x && crashPoint.x < t_rect->origin.x + t_rect->size.width && crashPoint.y >= t_rect->origin.y && crashPoint.y < t_rect->origin.y + t_rect->size.height)
			{
				// divide rect
				IntSize t_size;
				
				// left down
				t_size.width = crashPoint.x - t_rect->origin.x;
				t_size.height = crashPoint.y - t_rect->origin.y + pixelSize;
				if(t_size.width >= pixelSize) // left down create
				{
					IntRect* n_rect = new IntRect(t_rect->origin.x, t_rect->origin.y, t_size.width, t_size.height);
					n_rect->autorelease();
					drawRects->addObject(n_rect);
				}
				
				// down right
				t_size.width = t_rect->origin.x + t_rect->size.width - crashPoint.x;
				t_size.height = crashPoint.y - t_rect->origin.y;
				if(t_size.height >= pixelSize) // down right create
				{
					IntRect* n_rect = new IntRect(crashPoint.x, t_rect->origin.y, t_size.width, t_size.height);
					n_rect->autorelease();
					drawRects->addObject(n_rect);
				}
				
				// right up
				t_size.width = t_rect->origin.x + t_rect->size.width - crashPoint.x - pixelSize;
				t_size.height = t_rect->origin.y + t_rect->size.height - crashPoint.y;
				if(t_size.width >= pixelSize)
				{
					IntRect* n_rect = new IntRect(crashPoint.x+pixelSize, crashPoint.y, t_size.width, t_size.height);
					n_rect->autorelease();
					drawRects->addObject(n_rect);
				}
				
				// up left
				t_size.width = crashPoint.x - t_rect->origin.x + pixelSize;
				t_size.height = t_rect->origin.y + t_rect->size.height - crashPoint.y - pixelSize;
				if(t_size.height >= pixelSize)
				{
					IntRect* n_rect = new IntRect(t_rect->origin.x, crashPoint.y + pixelSize, t_size.width, t_size.height);
					n_rect->autorelease();
					drawRects->addObject(n_rect);
				}
				removeArray.push_back(t_rect);
			}
		}
		
		while(!removeArray.empty())
		{
			IntRect* t_rect = removeArray.back();
			removeArray.pop_back();
			drawRects->removeObject(t_rect);
		}
	}
	
	void setMoveGamePosition(CCPoint t_p)
	{
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float y_value = -t_p.y*myGD->game_scale+480.f*frame_size.height/frame_size.width/2.f;// (160-t_p.y)*MY_SCALE-73.f+myDSH->bottom_base-myDSH->ui_jack_center_control;
		if(y_value > 60)																	y_value = 60;
		else if(y_value < -490*myGD->game_scale+480*frame_size.height/frame_size.width)		y_value = -490*myGD->game_scale+480*frame_size.height/frame_size.width;
		
		if(myGD->gamescreen_type == kGT_full)				myVS->setMoveGamePosition(ccp(myGD->boarder_value,y_value));
		else if(myGD->gamescreen_type == kGT_leftUI)		myVS->setMoveGamePosition(ccp(50+myGD->boarder_value,y_value));
		else if(myGD->gamescreen_type == kGT_rightUI)		myVS->setMoveGamePosition(ccp(myGD->boarder_value,y_value));
	}
	
	void myInit(const char* filename, bool isPattern)
	{
		drawRects = new CCArray(1);
		setPosition(CCPointZero);
		
		myGD->V_Ip["VS_divideRect"] = std::bind(&VisibleParent::divideRect, this, _1);
		myGD->V_CCP["VS_setMoveGamePosition"] = std::bind(&VisibleParent::setMoveGamePosition, this, _1);
		
		myVS = VisibleSprite::create(filename, isPattern, drawRects);
		myVS->setPosition(CCPointZero);
		addChild(myVS);
	}
};

enum MSzorder{
	invisibleZorder = 1,
	visibleZorder,
	blockZorder,
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
	static MapScanner* create()
	{
		MapScanner* myMS = new MapScanner();
		myMS->myInit();
		myMS->autorelease();
		return myMS;
	}

	void scanMap();
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);
		
		int viewport [4];
		glGetIntegerv (GL_VIEWPORT, viewport);
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float rate = frame_size.height/frame_size.width;
		CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
		float wScale = viewport[2] / rSize.width;
		float hScale = viewport[3] / rSize.height;
		
		float x = 0*wScale + viewport[0];
		float y = 0*hScale + viewport[1];
		float w = 480*wScale;
		float h = 320*(rate/(320.f/480.f))*hScale;
		
		glScissor(x,y,w,h);
		
		CCNode::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
	void exchangeMS()
	{
		CCTexture2D* top_texture = CCTextureCache::sharedTextureCache()->addImage("frame_top.png");
		CCAnimation* top_animation = CCAnimation::create();
		top_animation->setDelayPerUnit(0.1f);
		for(int i=0;i<4;i++)
			top_animation->addSpriteFrameWithTexture(top_texture, CCRectMake(0, i*7, 334, 7));
		CCAnimate* top_animate = CCAnimate::create(top_animation);
		CCRepeatForever* top_repeat = CCRepeatForever::create(top_animate);
		top_boarder->runAction(top_repeat);
		
		CCTexture2D* bottom_texture = CCTextureCache::sharedTextureCache()->addImage("frame_bottom.png");
		CCAnimation* bottom_animation = CCAnimation::create();
		bottom_animation->setDelayPerUnit(0.1f);
		for(int i=0;i<4;i++)
			bottom_animation->addSpriteFrameWithTexture(bottom_texture, CCRectMake(0, i*7, 334, 7));
		CCAnimate* bottom_animate = CCAnimate::create(bottom_animation);
		CCRepeatForever* bottom_repeat = CCRepeatForever::create(bottom_animate);
		bottom_boarder->runAction(bottom_repeat);
		
		CCTexture2D* left_texture = CCTextureCache::sharedTextureCache()->addImage("frame_left.png");
		CCAnimation* left_animation = CCAnimation::create();
		left_animation->setDelayPerUnit(0.1f);
		for(int i=0;i<4;i++)
			left_animation->addSpriteFrameWithTexture(left_texture, CCRectMake(i*7, 0, 7, 430));
		CCAnimate* left_animate = CCAnimate::create(left_animation);
		CCRepeatForever* left_repeat = CCRepeatForever::create(left_animate);
		left_boarder->runAction(left_repeat);
		
		CCTexture2D* right_texture = CCTextureCache::sharedTextureCache()->addImage("frame_right.png");
		CCAnimation* right_animation = CCAnimation::create();
		right_animation->setDelayPerUnit(0.1f);
		for(int i=0;i<4;i++)
			right_animation->addSpriteFrameWithTexture(right_texture, CCRectMake(i*7, 0, 7, 430));
		CCAnimate* right_animate = CCAnimate::create(right_animation);
		CCRepeatForever* right_repeat = CCRepeatForever::create(right_animate);
		right_boarder->runAction(right_repeat);
		
		if(invisibleImg)
		{
			invisibleImg->removeFromParentAndCleanup(true);
			invisibleImg = NULL;
		}
		
		invisibleImg = InvisibleSprite::create(CCString::createWithFormat("stage%d_level2_invisible.png",silType)->getCString(), false);
		invisibleImg->setPosition(CCPointZero);
		addChild(invisibleImg, invisibleZorder);
		
		CCArray* t_rect_array = new CCArray();;
		
		if(visibleImg)
		{
			t_rect_array->initWithArray(visibleImg->getDrawRects());
			visibleImg->removeFromParentAndCleanup(true);
			visibleImg = NULL;
		}
		
		visibleImg = VisibleParent::create(CCString::createWithFormat("stage%d_level2_visible.png",silType)->getCString(), false);
		visibleImg->setPosition(CCPointZero);
		addChild(visibleImg, visibleZorder);
		
		visibleImg->setDrawRects(t_rect_array);
		
		t_rect_array->autorelease();
		
		scanMap();
	}
	
	void randomingRectView(CCPoint t_p)
	{
		if(my_tic_toc)
			AudioEngine::sharedInstance()->playEffect("sound_casting_attack.mp3", false);
		
		my_tic_toc = !my_tic_toc;
		
		int base_value = roundf(-t_p.y/myGD->game_scale/2.f);
		
		int gacha_cnt = mySGD->getStartMapGachaCnt();
		if(gacha_cnt > 1)
			gacha_cnt = 1;
		
		gacha_cnt = rand()%(gacha_cnt*5+1);
		
		if(gacha_cnt <= 3)
		{
			init_rect.size.width = rand()%(30-16 + 1) + 16;//rand()%(maxSize.width-minSize.width + 1) + minSize.width;
			init_rect.size.height = rand()%(30-16 + 1) + 16;//rand()%(maxSize.height-minSize.height + 1) + minSize.height
		}
		else if(gacha_cnt <= 5)
		{
			init_rect.size.width = rand()%(60-36 + 1) + 36;//rand()%(maxSize.width-minSize.width + 1) + minSize.width;
			init_rect.size.height = rand()%(60-36 + 1) + 36;//rand()%(maxSize.height-minSize.height + 1) + minSize.height
		}
		
		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-init_rect.size.width-2-mapWidthInnerBegin-20, init_rect.size.height-2);
		
		init_rect.origin.x = rand()%maxPoint.x+10;//mapWidthInnerBegin+10;
		init_rect.origin.y = rand()%maxPoint.y+base_value+roundf(screen_height/myGD->game_scale/2.f)-init_rect.size.height+1;
		
		if(!random_rect_img)
		{
			random_rect_img = CCSprite::create("whitePaper.png");
			random_rect_img->setColor(ccGRAY);
			addChild(random_rect_img, blockZorder);
		}
		
		random_rect_img->setTextureRect(CCRectMake(0, 0, init_rect.size.width*2.f, init_rect.size.height*2.f));
		random_rect_img->setPosition(ccp(init_rect.origin.x*pixelSize + init_rect.size.width, init_rect.origin.y*pixelSize-1 + init_rect.size.height));
	}
	
	void stopRandomingRectView()
	{
		random_rect_img->removeFromParentAndCleanup(true);
		myGD->initUserSelectedStartRect(init_rect);
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
	
	IntRect* newRectChecking(IntMoveState start);
	
	void resetRects();
	
	void bfsCheck(mapType beforeType, mapType afterType, IntPoint startPoint);
	
	BFS_Point directionVector(IntDirection direction)
	{
		BFS_Point r_v;
		if(direction == directionLeftUp)			{		r_v.x = -1;		r_v.y = 1;		}
		else if(direction == directionLeft)			{		r_v.x = -1;		r_v.y = 0;		}
		else if(direction == directionLeftDown)		{		r_v.x = -1;		r_v.y = -1;		}
		else if(direction == directionDown)			{		r_v.x = 0;		r_v.y = -1;		}
		else if(direction == directionRightDown)	{		r_v.x = 1;		r_v.y = -1;		}
		else if(direction == directionRight)		{		r_v.x = 1;		r_v.y = 0;		}
		else if(direction == directionRightUp)		{		r_v.x = 1;		r_v.y = 1;		}
		else if(direction == directionUp)			{		r_v.x = 0;		r_v.y = 1;		}
		else										{		r_v.x = 0;		r_v.y = 0;		}
		return r_v;
	}
	
	bool isInnerMap(BFS_Point t_p)
	{
		if(t_p.x >= mapWidthInnerBegin && t_p.x < mapWidthInnerEnd && t_p.y >= mapHeightInnerBegin && t_p.y < mapHeightInnerEnd)
			return true;
		else
			return false;
	}
	
	BackFilename getBackVisibleFilename(int worldMap, int insideMap)
	{
		BackFilename r_value;
		
		r_value.filename = CCString::createWithFormat("stage%d_level1_visible.png", silType)->getCString();
		r_value.isPattern = false;
		
		return r_value;
	}
	
	BackFilename getBackInvisibleFilename(int worldMap, int insideMap)
	{
		BackFilename r_value;
		
		r_value.filename = CCString::createWithFormat("stage%d_level1_invisible.png", silType)->getCString();
		r_value.isPattern = false;
		
		return r_value;
	}
	
	void setMapImg()
	{
		if(invisibleImg)
		{
			invisibleImg->release();
			invisibleImg = NULL;
		}
		
		SelectedMapData* mySMD = SelectedMapData::sharedInstance();
		
		BackFilename visible_filename = getBackVisibleFilename(mySMD->getSelectedChapter(), mySMD->getSelectedStage());
		BackFilename invisible_filename = getBackInvisibleFilename(mySMD->getSelectedChapter(), mySMD->getSelectedStage());
		
		invisibleImg = InvisibleSprite::create(invisible_filename.filename.c_str(), invisible_filename.isPattern);
		invisibleImg->setPosition(CCPointZero);
		addChild(invisibleImg, invisibleZorder);
		
		if(visibleImg)
		{
			visibleImg->release();
			visibleImg = NULL;
		}
		
		visibleImg = VisibleParent::create(visible_filename.filename.c_str(), visible_filename.isPattern);
		visibleImg->setPosition(CCPointZero);
		addChild(visibleImg, visibleZorder);
		
		if(blockParent)
		{
			blockParent->release();
			blockParent = NULL;
		}
		
		blockParent = CCNode::create();
		addChild(blockParent, blockZorder);
		
		CCSprite* top_back = CCSprite::create("top_back.png");
		top_back->setAnchorPoint(ccp(0.5,0));
		top_back->setPosition(ccp(160,430));
		top_back->setScaleX((320.f+myGD->boarder_value*2.f)/480.f);
		addChild(top_back);
		
		CCSprite* bottom_back = CCSprite::create("bottom_back.png");
		bottom_back->setAnchorPoint(ccp(0.5,1));
		bottom_back->setPosition(ccp(160,0));
		bottom_back->setScaleX((320.f+myGD->boarder_value*2.f)/480.f);
		addChild(bottom_back);
		
		top_boarder = CCSprite::create("frame_top.png", CCRectMake(0, 0, 334, 7));
		top_boarder->setAnchorPoint(ccp(0.5,0));
		top_boarder->setPosition(ccp(160,430));
		addChild(top_boarder, boarderZorder);
		
		bottom_boarder = CCSprite::create("frame_bottom.png", CCRectMake(0, 0, 334, 7));
		bottom_boarder->setAnchorPoint(ccp(0.5,1));
		bottom_boarder->setPosition(ccp(160,0));
		addChild(bottom_boarder, boarderZorder);
		
		left_boarder = CCSprite::create("frame_left.png", CCRectMake(0,0,7,430));
		left_boarder->setAnchorPoint(ccp(1,0.5));
		left_boarder->setPosition(ccp(0,215));
		addChild(left_boarder, boarderZorder);
		
		right_boarder = CCSprite::create("frame_right.png", CCRectMake(0,0,7,430));
		right_boarder->setAnchorPoint(ccp(0,0.5));
		right_boarder->setPosition(ccp(320,215));
		addChild(right_boarder, boarderZorder);
	}
	
	CCSprite* top_boarder;
	CCSprite* bottom_boarder;
	CCSprite* left_boarder;
	CCSprite* right_boarder;
	
	void myInit()
	{
		screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		screen_height = roundf(480*screen_size.height/screen_size.width/2.f);
		
		silType = mySD->getSilType();
		
		random_rect_img = NULL;
		invisibleImg = NULL;
		visibleImg = NULL;
		blockParent = NULL;
		
		myGD->V_V["MS_scanMap"] = std::bind(&MapScanner::scanMap, this);
		myGD->V_V["MS_resetRects"] = std::bind(&MapScanner::resetRects, this);
		
		
		setMapImg();
	}
};

#endif
