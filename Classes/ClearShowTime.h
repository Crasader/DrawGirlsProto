//
//  ClearShowTime.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 26..
//
//

#ifndef __galsprototype__ClearShowTime__
#define __galsprototype__ClearShowTime__

#include "cocos2d.h"
#include "GameData.h"
#include "AudioEngine.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"

USING_NS_CC;

enum ClearShowTimeZorder{
	kCST_Z_clear = 1,
	kCST_Z_boader,
	kCST_Z_showtime,
	kCST_Z_showtime_back
};

class ClearShowTime : public CCNode
{
public:
	static ClearShowTime* create(bool t_exchanged, bool t_is, CCNode* t_game_node, CCObject* t_changeScene, SEL_CallFunc d_changeScene)
	{
		ClearShowTime* t_cst = new ClearShowTime();
		t_cst->myInit(t_exchanged, t_is, t_game_node, t_changeScene, d_changeScene);
		t_cst->autorelease();
		return t_cst;
	}
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);
		
		int viewport [4];
		glGetIntegerv (GL_VIEWPORT, viewport);
		CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
		float wScale = viewport[2] / rSize.width;
		float hScale = viewport[3] / rSize.height;
		float x = view_rect.origin.x*wScale + viewport[0];
		float y = view_rect.origin.y*hScale + viewport[1];
		float w = view_rect.size.width*wScale;
		float h = view_rect.size.height*hScale;
		glScissor(x,y,w,h);
		
		CCNode::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
	void startClearView()
	{
		frame_cnt = 0;
		schedule(schedule_selector(ClearShowTime::addViewHeight));
	}
	
private:
	
	CCRect view_rect;
	bool is_showtime;
	CCNode* game_node;
	
	int frame_cnt;
	
	int silType;
	
	CCSprite* clear_img;
	CCSprite* showtime_back;
	CCSprite* showtime_img;
	
	CCObject* target_changeScene;
	SEL_CallFunc delegate_changeScene;
	
	void changeScene()
	{
		(target_changeScene->*delegate_changeScene)();
	}
	
	void addViewHeight()
	{
		frame_cnt++;
		view_rect.origin.y -= myDSH->ui_top/40.f;
		view_rect.size.height += myDSH->ui_top/20.f;
		
		if(frame_cnt >= 20)
		{
			unschedule(schedule_selector(ClearShowTime::addViewHeight));
			
			int remove_target = 0;
			while(game_node->getChildrenCount() <= 1)
			{
				CCNode* t_target = (CCNode*)game_node->getChildren()->objectAtIndex(remove_target);
				if(t_target == this)
				{
					remove_target++;
				}
				else
				{
					game_node->removeChild(t_target, true);
				}
			}
			
			CCDelayTime* t_delay = CCDelayTime::create(1.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearShowTime::changeScene));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			runAction(t_seq);
		}
	}
	
	void myInit(bool t_exchanged, bool t_is, CCNode* t_game_node, CCObject* t_changeScene, SEL_CallFunc d_changeScene)
	{
		is_showtime = t_is;
		game_node = t_game_node;
		target_changeScene = t_changeScene;
		delegate_changeScene = d_changeScene;
		
		mySGD->is_exchanged = t_exchanged;
		mySGD->is_showtime = is_showtime;
		
		view_rect = CCRectMake(0, myDSH->ui_center_y, 480, 0);
		
		silType = mySD->getSilType();
		
		string ttt;
		
		if(t_exchanged)
			ttt = CCString::createWithFormat("stage%d_level2_visible.png", silType)->getCString();
		else
			ttt = CCString::createWithFormat("stage%d_level1_visible.png", silType)->getCString();
		
		clear_img = mySIL->getLoadedImg(ttt.c_str());
		clear_img->setPosition(ccp(160,215));
		addChild(clear_img, kCST_Z_clear);
		
		CCSprite* top_back = CCSprite::create("top_back.png");
		top_back->setAnchorPoint(ccp(0.5,0));
		top_back->setPosition(ccp(160,430));
		top_back->setScaleX(340.f/320.f);
		addChild(top_back);
		
		CCSprite* bottom_back = CCSprite::create("top_back.png");
		bottom_back->setAnchorPoint(ccp(0.5,1));
		bottom_back->setPosition(ccp(160,0));
		bottom_back->setScaleX(340.f/320.f);
		addChild(bottom_back);
		
		CCSprite* top_boarder = CCSprite::create("screen_boarder.png");
		top_boarder->setAnchorPoint(ccp(0.5,0));
		top_boarder->setPosition(ccp(160,430));
		top_boarder->setScaleX(34.f);
		addChild(top_boarder, kCST_Z_boader);
		
		CCSprite* bottom_boarder = CCSprite::create("screen_boarder.png");
		bottom_boarder->setAnchorPoint(ccp(0.5,1));
		bottom_boarder->setPosition(ccp(160,0));
		bottom_boarder->setScaleX(34.f);
		addChild(bottom_boarder, kCST_Z_boader);
		
		CCSprite* left_boarder = CCSprite::create("screen_boarder.png");
		left_boarder->setAnchorPoint(ccp(1,0.5));
		left_boarder->setPosition(ccp(0,215));
		left_boarder->setScaleY(45.f);
		addChild(left_boarder, kCST_Z_boader);
		
		CCSprite* right_boarder = CCSprite::create("screen_boarder.png");
		right_boarder->setAnchorPoint(ccp(0,0.5));
		right_boarder->setPosition(ccp(320,215));
		right_boarder->setScaleY(45.f);
		addChild(right_boarder, kCST_Z_boader);
		
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
		
//		CCSprite* top_back = CCSprite::create("top_back.png");
//		top_back->setAnchorPoint(ccp(0.5,0));
//		top_back->setPosition(ccp(160,430));
//		addChild(top_back);
//		
//		CCSprite* bottom_back = CCSprite::create("top_back.png");
//		bottom_back->setAnchorPoint(ccp(0.5,1));
//		bottom_back->setPosition(ccp(160,0));
//		addChild(bottom_back);
	}
};

#endif /* defined(__galsprototype__ClearShowTime__) */
