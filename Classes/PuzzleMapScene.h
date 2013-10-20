//
//  PuzzleMapScene.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 19..
//
//

#ifndef __DGproto__PuzzleMapScene__
#define __DGproto__PuzzleMapScene__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "GraySprite.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

class StagePiece : public GraySprite
{
public:
	static StagePiece* create(string filename, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder)
	{
		StagePiece* t_sp = new StagePiece();
		t_sp->myInit(filename, t_number, t_level, t_p, t_rect, t_gray, t_boarder);
		t_sp->autorelease();
		return t_sp;
	}
	
	bool isInnerRect(CCPoint t_p)
	{
		auto function = [&](CCNode* node)->CCAffineTransform
		{
			CCAffineTransform t = node->nodeToParentTransform();
			for (CCNode *p = node->getParent(); p != NULL; p = p->getParent())
				t = CCAffineTransformConcat(t, p->nodeToParentTransform());
			return t;
		};
		
		CCRect check_rect = CCRectApplyAffineTransform(touch_rect, function(this));
		return check_rect.containsPoint(t_p);
	}
	
	int getStageNumber(){	return stage_number;	}
	
	void setTouchBegin(){	setColor(ccGRAY);	}
	void setTouchCancel(){	setColor(ccWHITE);	}
	
private:
	int stage_number;
	int stage_level;
	CCRect touch_rect;
	bool is_gray;
	bool is_boarder;
	
	void myInit(string filename, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder)
	{
		stage_number = t_number;
		stage_level = t_level;
		touch_rect = t_rect;
		is_gray = t_gray;
		is_boarder = t_boarder;
		
		initWithFile(filename.c_str());
		setPosition(t_p);
		setGray(is_gray);
		if(is_boarder)
		{
			string boarder_filename;
			bool is_long = stage_number%2 == 1 ? true : false;
			if((stage_number-1)/6%2 == 1)	is_long = !is_long;
			
			if(is_long)			boarder_filename = "test_map_boarder_long.png";
			else				boarder_filename = "test_map_boarder_wide.png";
			
			CCSprite* boarder = CCSprite::create(boarder_filename.c_str());
			boarder->setPosition(ccp(getContentSize().width/2.f, getContentSize().height/2.f));
			addChild(boarder);
		}
	}
};

enum MapModeState
{
	kMMS_default = 0,
	kMMS_firstTouchDefault,
	kMMS_firstTouchStage,
	kMMS_multiTouchZoom,
	kMMS_changeMode,
	kMMS_uiMode
};

class PuzzleMapScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PuzzleMapScene);
	
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	MapModeState map_mode_state;
	
	CCNode* map_node;
	CCSpriteBatchNode* shadow_batchnode;
	int touched_stage_number;
	
	CCPoint getStagePosition(int stage);
	CCPoint getUiButtonPosition(int t_tag);
	
	int stage_count;
	void setMapNode();
	void setUIs();
	
	void addShadow(int i, int j, CCPoint base_position);
	
	int change_frame;
	float change_dy;
	float change_dscale;
	CCPoint change_dpoint;
	float change_top_dy;
	float change_bottom_dy;
	void startChangeUiMode();
	void changeUiMode();
	void stopChangeUiMode();
	void startChangeMapMode();
	void changeMapMode();
	void stopChangeMapMode();
	
	void resultLogin(Json::Value result_data);
	void startSceneSetting();
	
	void showEventButton();
	void onEventButton();
	void hideEventButton();
	
	//	void showPopup(int stage);
	void stageCancel();
	void popupClose();
	
	float minimum_scale;
	float maximum_scale;
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	
	map<int, CCPoint> multiTouchData;
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
	{
		if(map_mode_state == kMMS_changeMode)
			return;
		else if(map_mode_state == kMMS_uiMode)
		{
			startChangeMapMode();
		}
		else
		{
			CCSetIterator iter;
			CCTouch *touch;
			
			for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
			{
				touch = (CCTouch*)(*iter);
				CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
				location = ccpSub(location, myDSH->ui_zero_point);
				
				multiTouchData[(int)touch] = location;
				
				touch_p = location;
				
				this->unschedule(schedule_selector(PuzzleMapScene::moveAnimation));
				isAnimated=false;
				
				if(multiTouchData.size() == 1)
				{
					timeval time;
					gettimeofday(&time, NULL);
					touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
					touchStart_p = location;
					
					map_mode_state = kMMS_firstTouchDefault;
					for(int i=1;i<=stage_count && touched_stage_number == 0;i++)
					{
						StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
						if(t_sp->isInnerRect(location))
						{
							touched_stage_number = t_sp->getStageNumber();
							t_sp->setTouchBegin();
							map_mode_state = kMMS_firstTouchStage;
						}
					}
				}
				else if(multiTouchData.size() == 2)
				{
					if(map_mode_state == kMMS_firstTouchStage)
						resetStagePiece();
					
					map_mode_state = kMMS_multiTouchZoom;
					CCPoint sub_point = CCPointZero;
					map<int, CCPoint>::iterator it;
					for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
					{
						sub_point = ccpMult(sub_point, -1);
						sub_point = ccpAdd(sub_point, it->second);
					}
					
					zoom_base_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
				}
				else
				{
					
				}
			}
		}
	}
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
	{
		if(map_mode_state == kMMS_changeMode)
			return;
		else if(map_mode_state == kMMS_uiMode)
		{
			
		}
		else
		{
			CCSetIterator iter;
			CCTouch* touch;
			
			for(iter = pTouches->begin();iter != pTouches->end();++iter)
			{
				touch = (CCTouch*)(*iter);
				CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
				location = ccpSub(location, myDSH->ui_zero_point);
				
				map<int, CCPoint>::iterator o_it;
				o_it = multiTouchData.find((int)touch);
				if(o_it != multiTouchData.end())
				{
					o_it->second = location;
					if(multiTouchData.size() == 1)
					{
						this->moveListXY(ccpSub(touch_p, location));
						touch_p = location;
						
						if(map_mode_state == kMMS_firstTouchStage)
						{
							CCPoint sub_point = ccpSub(location, touchStart_p);
							float sub_value = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
							if(sub_value > 7.f)
							{
								resetStagePiece();
								map_mode_state = kMMS_firstTouchDefault;
							}
						}
					}
					else if(multiTouchData.size() == 2)
					{
						CCPoint sub_point = CCPointZero;
						map<int, CCPoint>::iterator it;
						for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
						{
							sub_point = ccpMult(sub_point, -1);
							sub_point = ccpAdd(sub_point, it->second);
						}
						
//						float before_scale = map_node->getScale();
						
						float changed_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
						float after_scale = map_node->getScale()*changed_distance/zoom_base_distance;
						if(after_scale > maximum_scale)				after_scale = maximum_scale;
						else if(after_scale < minimum_scale)		after_scale = minimum_scale;
						zoom_base_distance = changed_distance;
						map_node->setScale(after_scale);
						
						if(map_node->getPositionX() < 240-(map_node->getScale()-1.f)*240.f)
							map_node->setPositionX(240-(map_node->getScale()-1.f)*240.f);
						else if(map_node->getPositionX() > 240+(map_node->getScale()-1.f)*240.f)
							map_node->setPositionX(240+(map_node->getScale()-1.f)*240.f);
						
						if(map_node->getPositionY() < 160-(map_node->getScale()-1.f)*160.f)
							map_node->setPositionY(160-(map_node->getScale()-1.f)*160.f);
						else if(map_node->getPositionY() > 160+(map_node->getScale()-1.f)*160.f)
							map_node->setPositionY(160+(map_node->getScale()-1.f)*160.f);
					}
				}
			}
		}
	}
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
	{
		if(map_mode_state == kMMS_changeMode)
			return;
		else if(map_mode_state == kMMS_uiMode)
		{
			
		}
		else
		{
			CCSetIterator iter;
			CCTouch* touch;
			
			for(iter = pTouches->begin();iter != pTouches->end();++iter)
			{
				touch = (CCTouch*)(*iter);
				CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
				location = ccpSub(location, myDSH->ui_zero_point);
				
				map<int, CCPoint>::iterator o_it;
				o_it = multiTouchData.find((int)touch);
				if(o_it != multiTouchData.end())
				{
					multiTouchData.erase((int)touch);
					
					if(multiTouchData.size() == 0)
					{
						timeval time;
						gettimeofday(&time, NULL);
						long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
						CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
						
						float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
						if(isAnimated == false && fabsf(spd_value) > 2)
						{
							moveSpeed_p = _spd;
							this->schedule(schedule_selector(PuzzleMapScene::moveAnimation));
						}
						
						if(map_mode_state == kMMS_firstTouchStage)
						{
							stageAction(touched_stage_number);
							resetStagePiece();
						}
						
						map_mode_state = kMMS_default;
					}
					else if(multiTouchData.size() == 1)
					{
						map_mode_state = kMMS_firstTouchDefault;
					}
				}
			}
		}
	}
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
	{
		ccTouchesEnded(pTouches, pEvent);
	}
	
	void stageAction(int t_number);
	
	void resetStagePiece()
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(touched_stage_number);
		t_sp->setTouchCancel();
		touched_stage_number = 0;
	}
	
	virtual void registerWithTouchDispatcher(void);
};


#endif /* defined(__DGproto__PuzzleMapScene__) */
