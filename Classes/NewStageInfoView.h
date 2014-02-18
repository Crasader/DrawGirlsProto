//
//  NewStageInfoView.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 7..
//
//

#ifndef __DGproto__NewStageInfoView__
#define __DGproto__NewStageInfoView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include <algorithm>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum NewStageInfoView_MenuTag{
	kNewStageInfoView_MenuTag_reward = 1,
	kNewStageInfoView_MenuTag_rank
};

enum NewStageInfoView_Mode{
	kNewStageInfoView_Mode_reward = 0,
	kNewStageInfoView_Mode_rank
};

class TouchSuctionLayer;
class RankFriendInfo;
class NewStageInfoView : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
	static NewStageInfoView* create(int t_touch_priority, function<void(bool, int)> t_change_button_challenge)
	{
		NewStageInfoView* t_nsiv = new NewStageInfoView();
		t_nsiv->myInit(t_touch_priority, t_change_button_challenge);
		t_nsiv->autorelease();
		return t_nsiv;
	}
	
	void setClickedStage(int t_stage);
	
	int getSelectedIdx()
	{
		return selected_friend_idx;
	}
	
	RankFriendInfo getSelectedIdxRankFriendInfo();
	
private:
	int touch_priority;
	
	function<void(bool, int)> change_button_challenge;
	
	CCSprite* back_img;
	CCNode* content_node;
	CCMenu* reward_menu;
	CCMenu* rank_menu;
	
	NewStageInfoView_Mode recent_mode;
	
	bool is_show;
	
	int recent_stage;
	bool is_menu_enable;
	
	vector<RankFriendInfo> friend_list;
	CCTableView* rank_table;
	int selected_friend_idx;
	
	void setContentNode();
	
	void resultGetStageScoreList(Json::Value result_data);
	
	virtual CCTableViewCell* tableCellAtIndex( CCTableView *table, unsigned int idx );
	
	virtual void scrollViewDidScroll(CCScrollView* view){}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table)
	{
		return CCSizeMake(110, 40);
	}
	virtual unsigned int numberOfCellsInTableView(CCTableView *table)
	{
		return friend_list.size();
	}
	
	void myInit(int t_touch_priority, function<void(bool, int)> t_change_button_challenge);
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable || !is_show)
			return;
		
		is_menu_enable = false;
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kNewStageInfoView_MenuTag_reward)
		{
			if(recent_mode != kNewStageInfoView_Mode_reward)
			{
				recent_mode = kNewStageInfoView_Mode_reward;
				setContentNode();
			}
			setRewardMenu();
			setRankMenu();
			
			is_menu_enable = true;
		}
		else if(tag == kNewStageInfoView_MenuTag_rank)
		{
			if(recent_mode != kNewStageInfoView_Mode_rank)
			{
				recent_mode = kNewStageInfoView_Mode_rank;
				setContentNode();
			}
			setRewardMenu();
			setRankMenu();
			
			is_menu_enable = true;
		}
	}
	
	void setRewardMenu()
	{
		if(recent_mode == kNewStageInfoView_Mode_reward && reward_menu)
		{
			reward_menu->removeFromParent();
			reward_menu = NULL;
		}
		else if(recent_mode == kNewStageInfoView_Mode_rank && !reward_menu)
		{
			CCSprite* n_reward = CCSprite::create("puzzle_right_reward_off.png");
			CCSprite* s_reward = CCSprite::create("puzzle_right_reward_off.png");
			s_reward->setColor(ccGRAY);
			
			CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(NewStageInfoView::menuAction));
			reward_item->setTag(kNewStageInfoView_MenuTag_reward);
			
			reward_menu = CCMenu::createWithItem(reward_item);
			reward_menu->setPosition(ccp(back_img->getContentSize().width/2.f - 33, back_img->getContentSize().height/2.f + 96));
			back_img->addChild(reward_menu);
			
			reward_menu->setTouchPriority(touch_priority-2);
		}
	}
	
	void setRankMenu()
	{
		if(recent_mode == kNewStageInfoView_Mode_rank && rank_menu)
		{
			rank_menu->removeFromParent();
			rank_menu = NULL;
		}
		else if(recent_mode == kNewStageInfoView_Mode_reward && !rank_menu)
		{
			CCSprite* n_rank = CCSprite::create("puzzle_right_rank_off.png");
			CCSprite* s_rank = CCSprite::create("puzzle_right_rank_off.png");
			s_rank->setColor(ccGRAY);
			
			CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(NewStageInfoView::menuAction));
			rank_item->setTag(kNewStageInfoView_MenuTag_rank);
			
			rank_menu = CCMenu::createWithItem(rank_item);
			rank_menu->setPosition(ccp(back_img->getContentSize().width/2.f + 32, back_img->getContentSize().height/2.f + 96));
			back_img->addChild(rank_menu);
			
			rank_menu->setTouchPriority(touch_priority-2);
		}
	}
	
	void startHideAction()
	{
		is_show = false;
		unschedule(schedule_selector(NewStageInfoView::showing));
		schedule(schedule_selector(NewStageInfoView::hiding));
	}
	void hiding();
	void endHideAction()
	{
		unschedule(schedule_selector(NewStageInfoView::hiding));
	}
	
	void startShowAction()
	{
		is_show = true;
		unschedule(schedule_selector(NewStageInfoView::hiding));
		schedule(schedule_selector(NewStageInfoView::showing));
	}
	void showing();
	void endShowAction()
	{
		unschedule(schedule_selector(NewStageInfoView::showing));
	}
	
	int opacity_value;
	vector<int> keep_touch_list;
	vector<int> inner_touch_list;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
	{
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = back_img->convertToNodeSpace(touchLocation);
		
		CCRect bounding_box = back_img->boundingBox();
		bounding_box.origin = CCPointZero;
		
		if(bounding_box.containsPoint(local))
		{
			inner_touch_list.push_back((int)pTouch);
			if(!is_show)
				startShowAction();
		}
		else
		{
			keep_touch_list.push_back((int)pTouch);
			if(inner_touch_list.empty() && is_show)
				startHideAction();
		}
		return true;
	}
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
	{
		
	}
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
	{
		vector<int>::iterator iter_out = find(keep_touch_list.begin(), keep_touch_list.end(), (int)pTouch);
		vector<int>::iterator iter_in = find(inner_touch_list.begin(), inner_touch_list.end(), (int)pTouch);
		if(iter_out != keep_touch_list.end()) // 바깥 터치 가 떼졌을때
		{
			keep_touch_list.erase(iter_out);
			
			if(keep_touch_list.empty() && !is_show)
				startShowAction();
		}
		else if(iter_in != inner_touch_list.end()) // 안 터치가 떼졌을때
		{
			inner_touch_list.erase(iter_in);
			
			if(inner_touch_list.empty() && !keep_touch_list.empty() && is_show)
				startHideAction();
		}
		else
		{
			CCLog("what?"); // 이럴 일이 있나?
		}
	}
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
	{
		vector<int>::iterator iter_out = find(keep_touch_list.begin(), keep_touch_list.end(), (int)pTouch);
		vector<int>::iterator iter_in = find(inner_touch_list.begin(), inner_touch_list.end(), (int)pTouch);
		if(iter_out != keep_touch_list.end()) // 바깥 터치 가 떼졌을때
		{
			keep_touch_list.erase(iter_out);
			
			if(keep_touch_list.empty() && !is_show)
				startShowAction();
		}
		else if(iter_in != inner_touch_list.end()) // 안 터치가 떼졌을때
		{
			inner_touch_list.erase(iter_in);
			
			if(inner_touch_list.empty() && !keep_touch_list.empty() && is_show)
				startHideAction();
		}
		else
		{
			CCLog("what?"); // 이럴 일이 있나?
		}
	}
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, false);
	}
};


#endif /* defined(__DGproto__NewStageInfoView__) */
