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

USING_NS_CC;

enum ClearShowTimeZorder{
	kCST_Z_clear = 1,
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
		
//		glScissor(floor(view_rect.origin.x*visit_factor + device_margine.width), floor(view_rect.origin.y*visit_factor + device_margine.height),
//				  ceil(view_rect.size.width*visit_factor), ceil(view_rect.size.height*visit_factor));
		
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
	
//	float scaleFactor;
//	float device_rate;
//	float visit_factor;
//	CCSize device_margine;
	
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
		view_rect.origin.y -= (320+DataStorageHub::sharedInstance()->bottom_base*2.f)/40.f;
		view_rect.size.height += (320+DataStorageHub::sharedInstance()->bottom_base*2.f)/20.f;
		
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
			
			CCMoveTo* t_move1 = CCMoveTo::create(0.5f, ccp((199-160)*1.5f-70.f/1.5f*1.25f,(160-(430-102+DataStorageHub::sharedInstance()->upper_limit-DataStorageHub::sharedInstance()->bottom_base/1.5f))*1.5f-73.f+DataStorageHub::sharedInstance()->bottom_base));
			CCDelayTime* t_delay = CCDelayTime::create(0.5f);
			CCMoveTo* t_move2 = CCMoveTo::create(1.3f, ccp((199-160)*1.5f-70.f/1.5f*1.25f,(160-111-DataStorageHub::sharedInstance()->bottom_base/1.5f)*1.5f-73.f+DataStorageHub::sharedInstance()->bottom_base));
			CCDelayTime* t_delay1 = CCDelayTime::create(1.f);
			CCMoveTo* t_move3 = CCMoveTo::create(1.3f, ccp((199-160)*1.5f-70.f/1.5f*1.25f,(160-(430-102+DataStorageHub::sharedInstance()->upper_limit-DataStorageHub::sharedInstance()->bottom_base/1.5f))*1.5f-73.f+DataStorageHub::sharedInstance()->bottom_base));
			CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
			
			CCCallFunc* t_call;
//			if(is_showtime)
//				t_call = CCCallFunc::create(this, callfunc_selector(ClearShowTime::startShowTime));
//			else
				t_call = CCCallFunc::create(this, callfunc_selector(ClearShowTime::changeScene));
			
			CCAction* t_seq = CCSequence::create(t_move1, t_delay, t_move2, t_delay1, t_move3, t_delay2, t_call, NULL);
			
			game_node->runAction(t_seq);
		}
	}
	
	void startShowTime()
	{
		AudioEngine::sharedInstance()->playEffect("sound_showtime_bgm.mp3", false);
		AudioEngine::sharedInstance()->playEffect("sound_showtime_ment.mp3", false);
		showtime_back = CCSprite::create("showtime_back.png");
		showtime_back->setOpacity(0);
		showtime_back->setPosition(ccp(160,430-106-DataStorageHub::sharedInstance()->upper_limit+DataStorageHub::sharedInstance()->ui_jack_center_control-DataStorageHub::sharedInstance()->bottom_base*2.f/1.5f));
		addChild(showtime_back, kCST_Z_showtime_back);
		
		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 255);
		CCDelayTime* t_delay = CCDelayTime::create(0.5f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearShowTime::loadedBack));
		
		CCAction* t_action = CCSequence::create(t_fade, t_delay, t_call, NULL);
		
		showtime_back->runAction(t_action);
	}
	
	void loadedBack()
	{
		showtime_img = CCSprite::create(CCString::createWithFormat("stage%d_level3_visible.png", silType)->getCString());
		showtime_img->setPosition(ccp(160,215));
		addChild(showtime_img, kCST_Z_showtime);
		
		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearShowTime::downUpView));
		
		CCAction* t_action = CCSequence::create(t_fade, t_delay, t_call, NULL);
		
		showtime_back->runAction(t_action);
	}
	
	void downUpView()
	{
		clear_img->removeFromParentAndCleanup(true);
		
		CCMoveTo* t_move1 = CCMoveTo::create(1.3f, ccp((199-160)*1.5f-70.f/1.5f*1.25f,(160-111-DataStorageHub::sharedInstance()->bottom_base/1.5f)*1.5f-73.f+DataStorageHub::sharedInstance()->bottom_base));
		CCDelayTime* t_delay1 = CCDelayTime::create(1.f);
		CCMoveTo* t_move2 = CCMoveTo::create(1.3f, ccp((199-160)*1.5f-70.f/1.5f*1.25f,(160-(430-102+DataStorageHub::sharedInstance()->upper_limit-DataStorageHub::sharedInstance()->bottom_base/1.5f))*1.5f-73.f+DataStorageHub::sharedInstance()->bottom_base));
		CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearShowTime::changeScene));
		
		CCAction* t_seq = CCSequence::create(t_move1, t_delay1, t_move2, t_delay2, t_call, NULL);
		
		game_node->runAction(t_seq);
		
		showtime_back->removeFromParentAndCleanup(true);
	}
	
	void myInit(bool t_exchanged, bool t_is, CCNode* t_game_node, CCObject* t_changeScene, SEL_CallFunc d_changeScene)
	{
		is_showtime = t_is;
		game_node = t_game_node;
		target_changeScene = t_changeScene;
		delegate_changeScene = d_changeScene;
		
		mySGD->is_exchanged = t_exchanged;
		mySGD->is_showtime = is_showtime;
		
		view_rect = CCRectMake(0, 160+DataStorageHub::sharedInstance()->bottom_base, 480, 0);
		
		silType = mySD->getSilType();
		
		string ttt;
		
		if(t_exchanged)
			ttt = CCString::createWithFormat("stage%d_level2_visible.png", silType)->getCString();
		else
			ttt = CCString::createWithFormat("stage%d_level1_visible.png", silType)->getCString();
		
		clear_img = CCSprite::create(ttt.c_str());
		clear_img->setPosition(ccp(160,215));
		addChild(clear_img, kCST_Z_clear);
		
//		scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
//		device_rate = DataStorageHub::sharedInstance()->device_rate;
//		visit_factor = scaleFactor*device_rate;
//		device_margine = DataStorageHub::sharedInstance()->device_margine;
		
		CCSprite* top_back = CCSprite::create("top_back.png");
		top_back->setAnchorPoint(ccp(0.5,0));
		top_back->setPosition(ccp(160,430));
		addChild(top_back);
		
		CCSprite* bottom_back = CCSprite::create("top_back.png");
		bottom_back->setAnchorPoint(ccp(0.5,1));
		bottom_back->setPosition(ccp(160,0));
		addChild(bottom_back);
	}
};

#endif /* defined(__galsprototype__ClearShowTime__) */
