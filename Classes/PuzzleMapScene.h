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

enum PuzzleMode{
	kPM_default = 0,
	kPM_thumb
};

class StagePiece : public CCNode
{
public:
	static StagePiece* create(string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder)
	{
		StagePiece* t_sp = new StagePiece();
		t_sp->myInit(t_piece, t_number, t_level, t_p, t_rect, t_gray, t_boarder);
		t_sp->autorelease();
		return t_sp;
	}
	
	bool isInnerRect(CCPoint t_p)
	{
		CCPoint location = convertToNodeSpace(t_p);
		
		return touch_rect.containsPoint(location);
	}
	
	void setChangable(string t_thumb, bool t_card1, bool t_card2, bool t_card3)
	{
		is_changable = true;
		thumb_name = t_thumb.c_str();
		is_have_card[0] = t_card1;
		is_have_card[1] = t_card2;
		is_have_card[2] = t_card3;
	}
	
	bool isChangable(){		return is_changable;	}
	void setPuzzleMode(PuzzleMode t_mode)
	{
		if(!is_changable)		return;
		piece_img->removeFromParent();
		piece_img = NULL;
		
		if(t_mode == kPM_default)
		{
			piece_img = GraySprite::create(piece_name.c_str());
			piece_img->setGray(is_gray);
			addChild(piece_img);
			
			if(is_boarder)
			{
				string boarder_filename;
				bool is_long = stage_number%2 == 1 ? true : false;
				if((stage_number-1)/6%2 == 1)	is_long = !is_long;
				
				if(is_long)			boarder_filename = "test_map_boarder_long.png";
				else				boarder_filename = "test_map_boarder_wide.png";
				
				CCSprite* boarder = CCSprite::create(boarder_filename.c_str());
				boarder->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
				piece_img->addChild(boarder);
			}
		}
		else if(t_mode == kPM_thumb)
		{
			piece_img = GraySprite::create(thumb_name.c_str());
			addChild(piece_img);
			
//			if(is_boarder)
//			{
				string boarder_filename;
				bool is_long = stage_number%2 == 1 ? true : false;
				if((stage_number-1)/6%2 == 1)	is_long = !is_long;
				
				if(is_long)			boarder_filename = "test_map_boarder_long.png";
				else				boarder_filename = "test_map_boarder_wide.png";
				
				CCSprite* boarder = CCSprite::create(boarder_filename.c_str());
				boarder->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
				piece_img->addChild(boarder);
//			}
			
			CCSprite* card_base = CCSprite::create("test_map_card_base.png");
			card_base->setPosition(ccp(piece_img->getContentSize().width*1.f/3.f, piece_img->getContentSize().height/2.f));
			piece_img->addChild(card_base);
			
			for(int i=0;i<3;i++)
			{
				if(is_have_card[i])
				{
					CCSprite* card_on = CCSprite::create(CCString::createWithFormat("test_map_card%d.png", i+1)->getCString());
					card_on->setPosition(ccpAdd(card_base->getPosition(), ccp(0,18.f+i*(-18.f))));
					piece_img->addChild(card_on);
				}
			}
		}
	}
	
	int getStageNumber(){	return stage_number;	}
	
	void setTouchBegin(){	piece_img->setColor(ccGRAY);	}
	void setTouchCancel(){	piece_img->setColor(ccWHITE);	}
	
	CCSprite* shadow_node;
	
private:
	GraySprite* piece_img;
	
	string piece_name;
	int stage_number;
	int stage_level;
	CCRect touch_rect;
	bool is_gray;
	bool is_boarder;
	
	bool is_changable;
	string thumb_name;
	bool is_have_card[3];
	
	void myInit(string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder)
	{
		piece_name = t_piece.c_str();
		stage_number = t_number;
		stage_level = t_level;
		touch_rect = t_rect;
		is_gray = t_gray;
		is_boarder = t_boarder;
		is_changable = false;
		
		setPosition(t_p);
		
		piece_img = GraySprite::create(piece_name.c_str());
		piece_img->setGray(is_gray);
		addChild(piece_img);
		
		if(is_boarder)
		{
			string boarder_filename;
			bool is_long = stage_number%2 == 1 ? true : false;
			if((stage_number-1)/6%2 == 1)	is_long = !is_long;
			
			if(is_long)			boarder_filename = "test_map_boarder_long.png";
			else				boarder_filename = "test_map_boarder_wide.png";
			
			CCSprite* boarder = CCSprite::create(boarder_filename.c_str());
			boarder->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
			piece_img->addChild(boarder);
		}
		
		if(piece_name == "test_puzzle_empty.png")
		{
			CCSprite* level_img = CCSprite::create(CCString::createWithFormat("test_map_level%d.png", stage_level)->getCString());
			level_img->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
			piece_img->addChild(level_img);
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
	
	PuzzleMode my_puzzle_mode;
	
	int stage_count;
	void setMapNode();
	void setUIs();
	
	CCSprite* addShadow(int i, int j, CCPoint base_position);
	
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
	
	MapModeState before_map_mode_state;
	int ing_check_puzzle;
	void startPuzzleModeChange(PuzzleMode t_mode)
	{
		before_map_mode_state = map_mode_state;
		map_mode_state = kMMS_changeMode;
		my_puzzle_mode = t_mode;
		ing_check_puzzle = 1;
		schedule(schedule_selector(PuzzleMapScene::ingPuzzleModeChange), 2.f/60.f);
	}
	void ingPuzzleModeChange()
	{
		bool is_found = false;
		CCNode* found_node;
		
		while(!is_found && ing_check_puzzle <= stage_count)
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
		
		if(ing_check_puzzle > stage_count)
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
	
	void changePiece(CCObject* sender)
	{
		StagePiece* t_sp = (StagePiece*)sender;
		t_sp->setPuzzleMode(my_puzzle_mode);
	}
	
	void endChangePiece(CCObject* sender)
	{
		CCFadeTo* t_fade = CCFadeTo::create(0.3f, 255);
		((StagePiece*)sender)->shadow_node->runAction(t_fade);
	}
	
	void resetStagePiece()
	{
		StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(touched_stage_number);
		t_sp->setTouchCancel();
		touched_stage_number = 0;
	}
	
	virtual void registerWithTouchDispatcher(void);
};


#endif /* defined(__DGproto__PuzzleMapScene__) */
