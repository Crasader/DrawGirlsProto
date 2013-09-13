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

class InvisibleSprite : public CCSpriteBatchNode
{
public:
	static InvisibleSprite* create(const char* filename, bool isPattern)
	{
		InvisibleSprite* t_iv = new InvisibleSprite();
		if(t_iv && t_iv->initWithFile(filename, isPattern ? kDefaultSpriteBatchCapacity : 1))
		{
			t_iv->myInit(filename, isPattern);
			t_iv->autorelease();
			return t_iv;
		}
		CC_SAFE_DELETE(t_iv);
		return NULL;
	}
	
	virtual ~InvisibleSprite()
	{
		
	}
	
private:
	
	void myInit(const char* filename, bool isPattern)
	{
		if(isPattern)
		{
			for(int i=0;i<4;i++)
			{
				for(int j=0;j<6;j++)
				{
					CCSprite* t_spr = CCSprite::create(filename);
					t_spr->setColor(ccc3(50, 50, 50));
					t_spr->setPosition(ccp(40+i*80, 40+j*80));
					addChild(t_spr);
				}
			}
		}
		else
		{
			CCSprite* t_spr = CCSprite::create(filename);
			t_spr->setPosition(ccp(160,215));
			addChild(t_spr);
		}
	}
};

//class InvisibleSprite : public CCSprite
//{
//public:
//	static InvisibleSprite* create(const char* filename)
//	{
//		InvisibleSprite* t_iv = new InvisibleSprite();
//		if(t_iv && t_iv->initWithFile(filename))
//		{
//			t_iv->myInit();
//			t_iv->autorelease();
//			return t_iv;
//		}
//		CC_SAFE_DELETE(t_iv);
//		return NULL;
//	}
//	
//	virtual ~InvisibleSprite()
//	{
//
//	}
//	
//private:
//
//	
//	void myInit()
//	{
//		
//	}
//};

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
			
//			CCRect r_rect = CCRectMake(t_rect->origin.x, t_rect->origin.y, t_rect->size.width, t_rect->size.height);
			
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
			
//			glScissor(floor(t_rect->origin.x*visit_factor + device_margine.width), floor(t_rect->origin.y*visit_factor + device_margine.height),
//					  ceil(t_rect->size.width*visit_factor), ceil(t_rect->size.height*visit_factor));
			CCSprite::visit();
			
			glDisable(GL_SCISSOR_TEST);
		}
	}
	
private:
	
//	float scaleFactor;
//	float device_rate;
//	float visit_factor;
//	CCSize device_margine;
	CCArray* drawRects;
	
	void myInit(const char* filename, CCRect t_rect, CCArray* t_drawRects)
	{
		initWithFile(filename, t_rect);
		
		
		drawRects = t_drawRects;
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
	}
};

class VisibleSprite : public CCSpriteBatchNode
{
public:
	static VisibleSprite* create(const char* filename, bool isPattern, CCArray* t_drawRects)
	{
		VisibleSprite* t_v = new VisibleSprite();
		if (t_v)
		{
			t_v->myInit(filename, isPattern, t_drawRects);
			t_v->autorelease();
			return t_v;
		}
		CC_SAFE_DELETE(t_v);
		return NULL;
	}
	
	void setMoveGamePosition(CCPoint t_p)
	{
		jack_position = t_p;
	}
	
private:
	
//	float scaleFactor;
//	float device_rate;
//	float visit_factor;
//	CCSize device_margine;
	CCArray* drawRects;
	
	CCPoint jack_position;
	
	virtual void visit()
	{
		unsigned int loopCnt = drawRects->count();
		
		for(int i=0;i<loopCnt;i++)
		{
			IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
			
//			CCRect r_rect = CCRectMake(t_rect->origin.x, t_rect->origin.y, t_rect->size.width, t_rect->size.height);
			
			glEnable(GL_SCISSOR_TEST);
			
			int viewport [4];
			glGetIntegerv (GL_VIEWPORT, viewport);
			CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
			float wScale = viewport[2] / rSize.width;
			float hScale = viewport[3] / rSize.height;
			
//			CCLog("viewport2 : %d , rSize.width : %.2f", viewport[2], rSize.width);
//			CCLog("viewport3 : %d , rSize.height : %.2f", viewport[3], rSize.height);
			
//			CCPoint convert_origin = convertToNodeSpace(ccp(t_rect->origin.x, t_rect->origin.y));
			float x = (t_rect->origin.x*1.5f)*wScale + viewport[0]-1;
			float y = (t_rect->origin.y*1.5f+jack_position.y)*hScale + viewport[1]-1;
			float w = (t_rect->size.width*1.5f)*wScale+2;
			float h = (t_rect->size.height*1.5f)*hScale+2;
			
			glScissor(x,y,w,h);
			
//			glScissor(floor((t_rect->origin.x*1.5f + jack_position.x)*visit_factor + device_margine.width), floor((t_rect->origin.y*1.5f + jack_position.y)*visit_factor + device_margine.height),
//					  ceil(t_rect->size.width*visit_factor*1.5f), ceil(t_rect->size.height*visit_factor*1.5f));
			
			draw();
			
			glDisable(GL_SCISSOR_TEST);
		}
	}
	
	void myInit(const char* filename, bool isPattern, CCArray* t_drawRects)
	{
		CCSprite* t_texture = NULL;
//		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
//		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
		
//		if(selected_stage == 5 && selected_chapter <= 21)
//		{
//			initWithFile(filename, isPattern ? kDefaultSpriteBatchCapacity : 1);
//		}
//		else if(selected_stage == 5 && selected_chapter != 1 && selected_chapter != 11 && selected_chapter >= 2)
//		{
//			t_texture = StageImgLoader::sharedInstance()->getLoadedImg(selected_chapter, selected_stage);
//			initWithTexture(t_texture->getTexture(), 1);
//		}
//		else
			initWithFile(filename, isPattern ? kDefaultSpriteBatchCapacity : 1);
		
		if(isPattern)
		{
			for(int i=0;i<4;i++)
			{
				for(int j=0;j<6;j++)
				{
					CCSprite* t_spr = CCSprite::create(filename);
					t_spr->setPosition(ccp(40+i*80, 40+j*80));
					addChild(t_spr);
				}
			}
		}
		else
		{
			if(t_texture)
			{
				CCSprite* t_spr = CCSprite::createWithTexture(t_texture->getTexture());
				CCSize t_size = t_spr->getContentSize();
				t_spr->setScaleX(320.f/t_size.width);
				t_spr->setScaleY(480.f/t_size.height);
				t_spr->setPosition(ccp(160,240));
				addChild(t_spr);
				
				t_texture->release();
			}
			else
			{
				CCSprite* t_spr = CCSprite::create(filename);
				t_spr->setPosition(ccp(160,215));
				addChild(t_spr);
			}
		}
		
		
		drawRects = t_drawRects;
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
	}
};

//class ObjParent : public CCNode
//{
//public:
//	static ObjParent* create(CCArray* t_drawRects)
//	{
//		ObjParent* t_op = new ObjParent();
//		t_op->myInit(t_drawRects);
//		t_op->autorelease();
//		return t_op;
//	}
//	
//	virtual ~ObjParent()
//	{
//		innerObjs->release();
//	}
//	
//	CCArray* innerObjs;
//	
//private:
//	
//	float scaleFactor;
//	float device_rate;
//	float visit_factor;
//	CCSize device_margine;
//	CCArray* drawRects;
//	
//	virtual void visit()
//	{
//		unsigned int loopCnt = drawRects->count();
//		
//		for(int i=0;i<loopCnt;i++)
//		{
//			IntRect* t_rect = (IntRect*)drawRects->objectAtIndex(i);
//			
//			CCRect r_rect = CCRectMake(t_rect->origin.x, t_rect->origin.y, t_rect->size.width, t_rect->size.height);
//			
//			glEnable(GL_SCISSOR_TEST);
//			
//			glScissor(floor(t_rect->origin.x*visit_factor + device_margine.width), floor(t_rect->origin.y*visit_factor + device_margine.height),
//					  ceil(t_rect->size.width*visit_factor), ceil(t_rect->size.height*visit_factor));
//			draw();
//			for(int j=0;j<innerObjs->count();j++)
//			{
//				CCRect* child_rect = (CCRect*)innerObjs->objectAtIndex(j);
//				if(child_rect->intersectsRect(r_rect))
//				{
//					BackObj* r_child = (BackObj*)getChildren()->objectAtIndex(j);
//					r_child->visit();
//				}
//			}
//			glDisable(GL_SCISSOR_TEST);
//		}
//	}
//	
//	void myInit(CCArray* t_drawRects)
//	{
//		
//		drawRects = t_drawRects;
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
//		
//		innerObjs = new CCArray(1);
//		
//		int selected_chapter = SelectedMapData::sharedInstance()->getSelectedChapter();
//		int selected_stage = SelectedMapData::sharedInstance()->getSelectedStage();
//		if(selected_chapter <= 20 && !((selected_chapter%10 == 1 && selected_stage == 1) || selected_stage == 5))
//		{
//			for(int i=0;i<8;i++)
//			{
//				int objCode = rand()%4 + 1;
//				
//				BackObj* n_obj = BackObj::create(CCString::createWithFormat("chapter%d_back_obj%d.png", (selected_chapter-1)%10/2+1+(selected_chapter-1)/10*10, objCode)->getCString());
//				CCPoint newposition;
//				
//				bool findedSafetyPosition = false;
//				while(!findedSafetyPosition)
//				{
//					newposition.x = rand()%321;
//					newposition.y = rand()%431;
//					findedSafetyPosition = true;
//					for(int j=0;j<getChildrenCount();j++)
//					{
//						CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(j);
//						CCPoint c_position = t_child->getPosition();
//						
//						c_position = ccpSub(newposition, c_position);
//						
//						float distance = sqrtf(powf(c_position.x, 2.f) + powf(c_position.y, 2.f));
//						
//						if(distance < 40)
//						{
//							findedSafetyPosition = false;
//							break;
//						}
//					}
//				}
//				n_obj->setPosition(newposition);
//				addChild(n_obj);
//				
//				CCSize objSize = n_obj->getContentSize();
//				
//				CCRect* t_rect = new CCRect(newposition.x - objSize.width/2.f, newposition.y - objSize.height/2.f, objSize.width, objSize.height);
//				t_rect->autorelease();
//				innerObjs->addObject(t_rect);
//			}
//		}
//	}
//};

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
//	ObjParent* myOP;
	
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
//		myVS->setMoveGamePosition(ccp((280-t_p.x)*1.25f-70.f,(160-t_p.y)*1.25f-43.f));
		
		if(t_p.y < 70+DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f)
			t_p.y = 70+DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f;
		else if(t_p.y > 430-65+DataStorageHub::sharedInstance()->upper_limit-DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f)//430-65
			t_p.y = 430-65+DataStorageHub::sharedInstance()->upper_limit-DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f;
		myVS->setMoveGamePosition(ccp((199-160)*1.5f-70.f/1.5f*1.25f,(160-t_p.y)*1.5f-73.f+DataStorageHub::sharedInstance()->bottom_base-DataStorageHub::sharedInstance()->ui_jack_center_control));
	}
	
	void myInit(const char* filename, bool isPattern)
	{
		drawRects = new CCArray(1);
		setPosition(CCPointZero);
		
//		myGD->regVS(this, callfuncIp_selector(VisibleParent::divideRect),
//					callfuncCCp_selector(VisibleParent::setMoveGamePosition));
		myGD->V_Ip["VS_divideRect"] = std::bind(&VisibleParent::divideRect, this, _1);
		myGD->V_CCP["VS_setMoveGamePosition"] = std::bind(&VisibleParent::setMoveGamePosition, this, _1);
		
		myVS = VisibleSprite::create(filename, isPattern, drawRects);
		myVS->setPosition(CCPointZero);
		addChild(myVS);
		
		if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 1 && SelectedMapData::sharedInstance()->getViewChapterSubNumber() == 1 && SelectedMapData::sharedInstance()->getSelectedStage() == 1)
		{
			TestEyeSprite* t_tes = TestEyeSprite::create("beach_eye.png", CCRectMake(0, 0, 100, 50), drawRects);
			t_tes->setPosition(ccp(184,322));
			addChild(t_tes);
		}
		
//		myOP = ObjParent::create(drawRects);
//		myOP->setPosition(CCPointZero);
//		addChild(myOP);
	}
};

enum MSzorder{
	invisibleZorder = 1,
	visibleZorder,
	blockZorder
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
	
	void exchangeMS()
	{
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
		
		int base_value = roundf(-t_p.y/3.f);
		
		init_rect.size.width = rand()%(50-20 + 1) + 20;//rand()%(maxSize.width-minSize.width + 1) + minSize.width;
		init_rect.size.height = rand()%(50-20 + 1) + 20;//rand()%(maxSize.height-minSize.height + 1) + minSize.height
		
		IntPoint maxPoint = IntPoint(mapWidthInnerEnd-init_rect.size.width-2-mapWidthInnerBegin-20, init_rect.size.height-2);
		
		init_rect.origin.x = rand()%maxPoint.x+10;//mapWidthInnerBegin+10;
		init_rect.origin.y = rand()%maxPoint.y+base_value+roundf(screen_height/3)-init_rect.size.height+1;
		
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
//		init_rect.origin.x -= mapWidthInnerBegin;
//		init_rect.origin.y -= mapHeightInnerBegin;
		myGD->initUserSelectedStartRect(init_rect);
	}
	
private:
	InvisibleSprite* invisibleImg;
	VisibleParent* visibleImg;
	CCSprite* random_rect_img;
//	VisibleSprite* visibleImg;
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
		
//		if(worldMap <= 20)
//		{
//			if(worldMap == 1 && insideMap == 1)
//			{
//				r_value.filename = "beach_back.png";
//				r_value.isPattern = false;
//			}
//			else if(insideMap == 5 && worldMap <= 20)
//			{
//				r_value.filename = CCString::createWithFormat("test%d_visible.png", worldMap)->getCString();
//				r_value.isPattern = false;
//			}
//			else if(SelectedMapData::sharedInstance()->getViewChapterNumber() == 1)
//			{
//				if(insideMap == 1)
//				{
//					r_value.filename = CCString::createWithFormat("c%d_s1_visible.png", worldMap)->getCString();
//					r_value.isPattern = false;
//				}
//				else if(insideMap == 5)
//				{
//					r_value.filename = CCString::createWithFormat("c%d_s5_visible.png", worldMap)->getCString();
//					r_value.isPattern = false;
//				}
//				else
//				{
//					r_value.filename = CCString::createWithFormat("chapter%d_backImg.png", (worldMap-1)%10/2+1+(worldMap-1)/10*10)->getCString();
//					r_value.isPattern = true;
//				}
//			}
//			else
//			{
//				if(insideMap == 5)
//				{
//					r_value.filename = CCString::createWithFormat("c%d_s5_visible.png", worldMap)->getCString();
//					r_value.isPattern = false;
//				}
//				else
//				{
//					r_value.filename = CCString::createWithFormat("chapter%d_backImg.png", (worldMap-1)%10/2+1+(worldMap-1)/10*10)->getCString();
//					r_value.isPattern = true;
//				}
//			}
//		}
//		else
//		{
//			if(insideMap == 5 && worldMap <= 21)
//			{
//				r_value.filename = CCString::createWithFormat("test%d_visible.png", worldMap)->getCString();
//				r_value.isPattern = false;
//			}
//			else if(insideMap == 5)
//			{
//				r_value.filename = CCString::createWithFormat("c%d_s5_visible.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_bonusImage))->getCString();
//				r_value.isPattern = false;
//			}
//			else
//			{
//				r_value.filename = CCString::createWithFormat("chapter%d_backImg.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_backPattern))->getCString();
//				r_value.isPattern = true;
//			}
//		}
		return r_value;
	}
	
	BackFilename getBackInvisibleFilename(int worldMap, int insideMap)
	{
		BackFilename r_value;
		
		r_value.filename = CCString::createWithFormat("stage%d_level1_invisible.png", silType)->getCString();
		r_value.isPattern = false;
		
//		if(worldMap <= 20)
//		{
//			if(worldMap == 1 && insideMap == 1)
//			{
//				r_value.filename = "beach_silhouette.png";
//				r_value.isPattern = false;
//			}
//			else if(insideMap == 5 && worldMap <= 20)
//			{
//				r_value.filename = CCString::createWithFormat("test%d_invisible.png", worldMap)->getCString();
//				r_value.isPattern = false;
//			}
//			else if(worldMap%10 == 1)
//			{
//				if(insideMap == 1)
//				{
//					r_value.filename = CCString::createWithFormat("c%d_s1_invisible.png", worldMap)->getCString();
//					r_value.isPattern = false;
//				}
//				else if(insideMap == 5)
//				{
//					r_value.filename = CCString::createWithFormat("c%d_s5_invisible.png", worldMap)->getCString();
//					r_value.isPattern = false;
//				}
//				else
//				{
//					r_value.filename = CCString::createWithFormat("chapter%d_backImg.png", (worldMap-1)%10/2+1+(worldMap-1)/10*10)->getCString();
//					r_value.isPattern = true;
//				}
//			}
//			else
//			{
//				if(insideMap == 5)
//				{
//					r_value.filename = CCString::createWithFormat("c%d_s5_invisible.png", worldMap)->getCString();
//					r_value.isPattern = false;
//				}
//				else
//				{
//					r_value.filename = CCString::createWithFormat("chapter%d_backImg.png", (worldMap-1)%10/2+1+(worldMap-1)/10*10)->getCString();
//					r_value.isPattern = true;
//				}
//			}
//		}
//		else
//		{
//			if(insideMap == 5 && worldMap <= 21)
//			{
//				r_value.filename = CCString::createWithFormat("test%d_invisible.png", worldMap)->getCString();
//				r_value.isPattern = false;
//			}
//			else if(insideMap == 5)
//			{
//				r_value.filename = CCString::createWithFormat("c%d_s5_invisible.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_bonusImage))->getCString();
//				r_value.isPattern = false;
//			}
//			else
//			{
//				r_value.filename = CCString::createWithFormat("chapter%d_backImg.png", SelectedMapData::sharedInstance()->getImageNumber(kIN_backPattern))->getCString();
//				r_value.isPattern = true;
//			}
//		}
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
		
//		SetMapType map_type = myGD->getRecentMapType();
//		if(map_type == kSMT_oneRect)
//		{
//			CCSprite* block1_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 160, 6));
//			block1_img->setColor(ccRED);
//			block1_img->setPosition(ccp(80,139));
//			blockParent->addChild(block1_img);
//			
//			CCSprite* block2_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 160, 6));
//			block2_img->setColor(ccRED);
//			block2_img->setPosition(ccp(240,279));
//			blockParent->addChild(block2_img);
//		}
		
		CCSprite* top_back = CCSprite::create("top_back.png");
		top_back->setAnchorPoint(ccp(0.5,0));
		top_back->setPosition(ccp(160,430));
		addChild(top_back);
		
		CCSprite* bottom_back = CCSprite::create("top_back.png");
		bottom_back->setAnchorPoint(ccp(0.5,1));
		bottom_back->setPosition(ccp(160,0));
		addChild(bottom_back);
	}
	
	
	void myInit()
	{
		
		
		
		screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		screen_height = roundf(480*screen_size.height/screen_size.width/2.f);
		
		silType = mySD->getSilType();
		
		random_rect_img = NULL;
		invisibleImg = NULL;
		visibleImg = NULL;
		blockParent = NULL;
		
//		myGD->regMS(this, callfunc_selector(MapScanner::scanMap), callfunc_selector(MapScanner::resetRects));
		myGD->V_V["MS_scanMap"] = std::bind(&MapScanner::scanMap, this);
		myGD->V_V["MS_resetRects"] = std::bind(&MapScanner::resetRects, this);
		
		
		setMapImg();
	}
};

#endif
