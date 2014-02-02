//
//  PauseContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#ifndef __DGproto__PauseContent__
#define __DGproto__PauseContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum PauseContentMenuTag{
	kPauseContentMenuTag_resume = 0,
	kPauseContentMenuTag_gohome,
	kPauseContentMenuTag_replay
};
class PauseContent : public CCNode
{
public:
	static PauseContent* create(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay)
	{
		PauseContent* t_ctc = new PauseContent();
		t_ctc->myInit(t_touch_priority, t_resume, t_gohome, t_replay);
		t_ctc->autorelease();
		return t_ctc;
	}
	
	void startShow()
	{
		setScale(0);
		setPosition(ccp(480-25, myDSH->ui_top-25));
		
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.f);
		CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(240,myDSH->ui_center_y));
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_move);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PauseContent::endShow));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		
		runAction(t_seq);
	}
	
private:
	int touch_priority;
	function<void(void)> resume_selector;
	function<void(void)> gohome_selector;
	function<void(void)> replay_selector;
	
	bool is_menu_enable;
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kPauseContentMenuTag_resume)
		{
			startHide();
		}
		else if(tag == kPauseContentMenuTag_gohome)
		{
			gohome_selector();
		}
		else if(tag == kPauseContentMenuTag_replay)
		{
			replay_selector();
		}
	}
	
	void endShow()
	{
		is_menu_enable = true;
	}
	void startHide()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 0.f);
		CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(480-25, myDSH->ui_top-25));
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_move);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PauseContent::endHide));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		
		runAction(t_seq);
	}
	void endHide()
	{
		resume_selector();
	}
	
	void myInit(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay)
	{
		is_menu_enable = false;
		
		touch_priority = t_touch_priority;
		resume_selector = t_resume;
		gohome_selector = t_gohome;
		replay_selector = t_replay;
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(CCPointZero);
		addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(180, 190));
		
		CCLabelTTF* title_label = CCLabelTTF::create("일시정지", mySGD->getFont().c_str(), 15);
		title_label->setPosition(ccp(0, 77));
		addChild(title_label);
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
		content_back->setPosition(ccp(0,-10));
		addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(160,140));
		
		CCSprite* n_resume = CCSprite::create("pause_popup_continue.png");
		CCSprite* s_resume = CCSprite::create("pause_popup_continue.png");
		s_resume->setColor(ccGRAY);
		
		CCMenuItem* resume_item = CCMenuItemSprite::create(n_resume, s_resume, this, menu_selector(PauseContent::menuAction));
		resume_item->setTag(kPauseContentMenuTag_resume);
		
		CCMenu* resume_menu = CCMenu::createWithItem(resume_item);
		resume_menu->setPosition(0,33);
		addChild(resume_menu);
		resume_menu->setTouchPriority(touch_priority-1);
		
		
		CCSprite* n_gohome = CCSprite::create("pause_popup_home.png");
		CCSprite* s_gohome = CCSprite::create("pause_popup_home.png");
		s_gohome->setColor(ccGRAY);
		
		CCMenuItem* gohome_item = CCMenuItemSprite::create(n_gohome, s_gohome, this, menu_selector(PauseContent::menuAction));
		gohome_item->setTag(kPauseContentMenuTag_gohome);
		
		CCMenu* gohome_menu = CCMenu::createWithItem(gohome_item);
		gohome_menu->setPosition(0,-9);
		addChild(gohome_menu);
		gohome_menu->setTouchPriority(touch_priority-1);
		
		
		CCSprite* n_replay = CCSprite::create("pause_popup_replay.png");
		CCSprite* s_replay = CCSprite::create("pause_popup_replay.png");
		s_replay->setColor(ccGRAY);
		
		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(PauseContent::menuAction));
		replay_item->setTag(kPauseContentMenuTag_replay);
		
		CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setPosition(0,-51);
		addChild(replay_menu);
		replay_menu->setTouchPriority(touch_priority-1);
	}
};

#endif /* defined(__DGproto__PauseContent__) */
