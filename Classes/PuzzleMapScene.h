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
#include "StagePiece.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

enum MapModeState
{
	kMMS_default = 0,
	kMMS_firstTouchDefault,
	kMMS_firstTouchStage,
	kMMS_multiTouchZoom,
	kMMS_changeMode,
	kMMS_uiMode,
	kMMS_frameMode
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
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);
		
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		
		float x = 0;
		float y = 0;
		float w = frame_size.width;
		float h = frame_size.height;
		
		glScissor(x,y,w,h);
		
		CCLayer::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	MapModeState map_mode_state;
	
	CCNode* main_node;
	
	CCNode* map_node;
	CCSpriteBatchNode* shadow_batchnode;
	int touched_stage_number;
	
	CCPoint getStagePosition(int stage);
	CCPoint getUiButtonPosition(int t_tag);
	
	PuzzleMode my_puzzle_mode;
	int recent_puzzle_number;
	
	int start_stage_number;
	int stage_count;
	void setMapNode();
	void setUIs();
	
	CCSprite* addShadow(int i, string t_type, CCPoint sp_position);
	
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
	
	void startChangeFrameMode();
	void stopChangeFrameMode();
	
	void startReturnUiMode();
	void stopReturnUiMode();
	
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
	
	bool is_gesturable_map_mode;
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
	{
		if(map_mode_state == kMMS_changeMode)
			return;
		else if(map_mode_state == kMMS_uiMode)
		{
			CCSetIterator iter;
			CCTouch *touch;
			
			for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
			{
				touch = (CCTouch*)(*iter);
				CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
				location = ccpSub(location, myDSH->ui_zero_point);
				
				multiTouchData[(int)touch] = location;
				
				if(multiTouchData.size() == 1)
				{
					touchStart_p = location;
				}
			}
		}
		else if(map_mode_state == kMMS_frameMode)
		{
			CCSetIterator iter;
			CCTouch *touch;
			
			for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
			{
				touch = (CCTouch*)(*iter);
				CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
				location = ccpSub(location, myDSH->ui_zero_point);
				
				multiTouchData[(int)touch] = location;
				
				if(multiTouchData.size() == 1)
				{
					touchStart_p = location;
				}
			}
		}
		else
		{
			CCSetIterator iter;
			CCTouch *touch;
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			CCSize my_size = CCSizeMake(480, 320);
			
			CCPoint touch_convert = ccp(0,screen_size.height*(my_size.width/screen_size.width) - my_size.height);
			
			for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
			{
				touch = (CCTouch*)(*iter);
				CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
				location = ccpSub(location, touch_convert);
				
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
					
//					CCPoint convert_location = convertToWorldSpace(location);
					
					for(int i=start_stage_number;i<start_stage_number+stage_count && touched_stage_number == 0;i++)
					{
						StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(i);
						
						if(t_sp->touchBegan(touch, pEvent))
//						if(t_sp->isInnerRect(convert_location))
						{
							touched_stage_number = t_sp->getStageNumber();
							t_sp->setTouchBegin();
							map_mode_state = kMMS_firstTouchStage;
						}
					}
				}
				else if(multiTouchData.size() == 2)
				{
					is_gesturable_map_mode = false;
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
					is_gesturable_map_mode = false;
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
						if(location.y < touchStart_p.y - 50.f)
						{
							startChangeFrameMode();
							multiTouchData.erase((int)touch);
						}
					}
				}
			}
		}
		else if(map_mode_state == kMMS_frameMode)
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
						if(location.y > touchStart_p.y + 50.f)
						{
							startReturnUiMode();
							multiTouchData.erase((int)touch);
						}
					}
				}
			}
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
							if(sub_value > 20.f)
							{
								resetStagePiece();
								map_mode_state = kMMS_firstTouchDefault;
							}
						}
						else if(is_gesturable_map_mode && location.y < touchStart_p.y - 50.f)
						{
							startChangeUiMode();
							return;
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
				}
			}
		}
		else if(map_mode_state == kMMS_uiMode)
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
				}
			}
			
			startChangeMapMode();
		}
		else if(map_mode_state == kMMS_frameMode)
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
				}
			}
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
							StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(touched_stage_number);
							t_sp->touchEnded(touch, pEvent);
							t_sp->setTouchCancel();
						}
						
						map_mode_state = kMMS_default;
					}
					else if(multiTouchData.size() == 1)
					{
						map_mode_state = kMMS_firstTouchDefault;
						if(map_node->getScale() == 1.f)
						{
							is_gesturable_map_mode = true;
						}
					}
				}
			}
		}
	}
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
	{
		ccTouchesEnded(pTouches, pEvent);
	}
	
	void stageAction(CCObject* sender);
	
	MapModeState before_map_mode_state;
	int ing_check_puzzle;
	void startPuzzleModeChange(PuzzleMode t_mode)
	{
		before_map_mode_state = map_mode_state;
		map_mode_state = kMMS_changeMode;
		my_puzzle_mode = t_mode;
		myDSH->setIntegerForKey(kDSH_Key_puzzleMode, my_puzzle_mode);
		ing_check_puzzle = start_stage_number;
		schedule(schedule_selector(PuzzleMapScene::ingPuzzleModeChange), 2.f/60.f);
	}
	void ingPuzzleModeChange()
	{
		bool is_found = false;
		CCNode* found_node;
		
		while(!is_found && ing_check_puzzle < start_stage_number+stage_count)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(ing_check_puzzle);
			if(t_sp->isChangable())
			{
				t_sp->shadow_node->stopAllActions();
				t_sp->shadow_node->setOpacity(0);
				found_node = t_sp;
				is_found = true;
			}
			ing_check_puzzle++;
		}
		
		if(is_found)
		{
			CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.2f, 1.f, 0, 0, 90, 0, 0);
			CCCallFuncO* t_call1 = CCCallFuncO::create(this, callfuncO_selector(PuzzleMapScene::changePiece), found_node);
			CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.2f, 1.f, 0, -90, 90, 0, 0);
			CCCallFuncO* t_call2 = CCCallFuncO::create(this, callfuncO_selector(PuzzleMapScene::endChangePiece), found_node);
			CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
			found_node->runAction(t_seq);
		}
		
		if(ing_check_puzzle >= start_stage_number+stage_count)
		{
			stopPuzzleModeChange();
		}
	}
	void stopPuzzleModeChange()
	{
		unschedule(schedule_selector(PuzzleMapScene::ingPuzzleModeChange));
		
		is_menu_enable = true;
		map_mode_state = before_map_mode_state;
	}
	
	void changePiece(CCObject* sender);
	
	void endChangePiece(CCObject* sender)
	{
		CCFadeTo* t_fade = CCFadeTo::create(0.3f, 255);
		((StagePiece*)sender)->shadow_node->runAction(t_fade);
	}
	
	void resetStagePiece()
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(touched_stage_number);
		t_sp->touchCancelled(NULL, NULL);
		touched_stage_number = 0;
	}
	
	virtual void registerWithTouchDispatcher(void);
};


#endif /* defined(__DGproto__PuzzleMapScene__) */
