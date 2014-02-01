//
//  RankChange.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 30..
//
//

#ifndef __DGproto__RankChange__
#define __DGproto__RankChange__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "GDWebSprite.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum RankChangeMenuTag{
	kRankChangeMenuTag_close = 0,
	kRankChangeMenuTag_ok
};

class RankChange : public CCLayer
{
public:
	static RankChange* create(string t_up_profile, int t_up_rank, string t_up_nick, float t_up_score, string t_down_profile, int t_down_rank, string t_down_nick, float t_down_score, function<void(void)> t_ok_action)
	{
		RankChange* t_rc = new RankChange();
		t_rc->myInit(t_up_profile, t_up_rank, t_up_nick, t_up_score, t_down_profile, t_down_rank, t_down_nick, t_down_score, t_ok_action);
		t_rc->autorelease();
		return t_rc;
	}
	
private:
	
	string up_profile;
	int up_rank;
	string up_nick;
	float up_score;
	string down_profile;
	int down_rank;
	string down_nick;
	float down_score;
	function<void(void)> ok_action;
	
	int touch_priority;
	
	bool is_menu_enable;
	
	void myInit(string t_up_profile, int t_up_rank, string t_up_nick, float t_up_score, string t_down_profile, int t_down_rank, string t_down_nick, float t_down_score, function<void(void)> t_ok_action)
	{
		is_menu_enable = false;
		
		touch_priority = -300;
		setTouchEnabled(true);
		
		up_profile = t_up_profile;
		up_rank = t_up_rank;
		up_nick = t_up_nick;
		up_score = t_up_score;
		down_profile = t_down_profile;
		down_rank = t_down_rank;
		down_nick = t_down_nick;
		down_score = t_down_score;
		ok_action = t_ok_action;
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray);
		
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		CCLayer* back = dynamic_cast<CCLayer*>(reader->readNodeGraphFromFile("gameresult_rankchange.ccbi",this));
		back->setPosition(CCPointZero);
		addChild(back);
		reader->release();
		
		CCSprite* down_cell_back = CCSprite::create("rankchange_cell_down.png");
		down_cell_back->setPosition(ccp(200, 200));
		addChild(down_cell_back);
		
		if(down_profile != "")
		{
			CCSprite* down_profileImg = GDWebSprite::create(down_profile.c_str(), "ending_noimg.png");
			down_profileImg->setAnchorPoint(ccp(0.5, 0.5));
			down_profileImg->setPosition(ccp(52, 22));
			down_profileImg->setScale(35.f / down_profileImg->getContentSize().width);
			down_cell_back->addChild(down_profileImg);
		}
		
		CCLabelTTF* down_rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", down_rank)->getCString(), mySGD->getFont().c_str(), 15);
		down_rank_label->setPosition(ccp(21,21));
		down_cell_back->addChild(down_rank_label);
	
		CCLabelTTF* down_nickname_label = CCLabelTTF::create(down_nick.c_str(), mySGD->getFont().c_str(), 12);
		down_nickname_label->setPosition(ccp(114,28));
		down_cell_back->addChild(down_nickname_label);
		
		CCLabelTTF* down_score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", down_score)->getCString(), mySGD->getFont().c_str(), 12);
		down_score_label->setColor(ccBLACK);
		down_score_label->setPosition(ccp(114,13));
		down_cell_back->addChild(down_score_label);
		
		
		CCSprite* up_cell_back = CCSprite::create("rankchange_cell_me.png");
		up_cell_back->setPosition(ccp(280, 120));
		addChild(up_cell_back);
		
		if(up_profile != "")
		{
			CCSprite* up_profileImg = GDWebSprite::create(up_profile.c_str(), "ending_noimg.png");
			up_profileImg->setAnchorPoint(ccp(0.5, 0.5));
			up_profileImg->setPosition(ccp(52, 22));
			up_profileImg->setScale(35.f / up_profileImg->getContentSize().width);
			up_cell_back->addChild(up_profileImg);
		}
		
		CCLabelTTF* up_rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", up_rank)->getCString(), mySGD->getFont().c_str(), 15);
		up_rank_label->setPosition(ccp(21,21));
		up_cell_back->addChild(up_rank_label);
		
		CCLabelTTF* up_nickname_label = CCLabelTTF::create(up_nick.c_str(), mySGD->getFont().c_str(), 12);
		up_nickname_label->setPosition(ccp(114,28));
		up_cell_back->addChild(up_nickname_label);
		
		CCLabelTTF* up_score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", up_score)->getCString(), mySGD->getFont().c_str(), 12);
		up_score_label->setColor(ccBLACK);
		up_score_label->setPosition(ccp(114,13));
		up_cell_back->addChild(up_score_label);
		
		
		CCSprite* down_arrow = CCSprite::create("rankchange_arrow_down.png");
		down_arrow->setPosition(ccp(140,150));
		addChild(down_arrow);
		
		CCSprite* up_arrow = CCSprite::create("rankchange_arrow_up.png");
		up_arrow->setPosition(ccp(340,170));
		addChild(up_arrow);
		
		
		CCSprite* n_close = CCSprite::create("cardchange_cancel.png");
		CCSprite* s_close = CCSprite::create("cardchange_cancel.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(RankChange::menuAction));
		close_item->setTag(kRankChangeMenuTag_close);
		
		CCMenu* close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(ccp(390,260));
		addChild(close_menu);
		close_menu->setTouchPriority(touch_priority-1);
		
		
		CCSprite* n_ok = CCSprite::create("rankchange_ok.png");
		CCSprite* s_ok = CCSprite::create("rankchange_ok.png");
		s_ok->setColor(ccGRAY);
		
		CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(RankChange::menuAction));
		ok_item->setTag(kRankChangeMenuTag_ok);
		
		CCMenu* ok_menu = CCMenu::createWithItem(ok_item);
		ok_menu->setPosition(ccp(240,50));
		addChild(ok_menu);
		ok_menu->setTouchPriority(touch_priority-1);
		
		
		is_menu_enable = true;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kRankChangeMenuTag_close)
		{
			removeFromParent();
		}
		else if(tag == kRankChangeMenuTag_ok)
		{
			ok_action();
		}
	}
	
	virtual bool ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
	{
		CCLog("touch swallow : RankChange");
		return true;
	}
	
	virtual void ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
	{
		
	}
	
	virtual void ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
	{
		
	}
	
	virtual void ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
	{
		
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
	}
};

#endif /* defined(__DGproto__RankChange__) */
