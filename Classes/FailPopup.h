//
//  FailPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#ifndef __DGproto__FailPopup__
#define __DGproto__FailPopup__

#include "cocos2d.h"
#include "MyLocalization.h"
#include "hspConnector.h"
#include "HeartTime.h"
#include "cocos-ext.h"
#include "GDWebSprite.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class FailFriendRank
{
public:
	string nickname;
	string img_url;
	string user_id;
	float score;
	bool is_play;
	bool is_message_blocked;
	
	bool operator==(string t_id)
	{
		return user_id == t_id;
	};
};

enum FailFriendCell_Tag
{
	kFFC_T_score = 1,
	kFFC_T_rank,
	kFFC_T_nickname,
	kFFC_T_img,
	kFFC_T_menuBase = 100000
};

enum FailFriendCell_Zorder
{
	kFFC_Z_case = 1,
	kFFC_Z_img
};

class FailPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(FailPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCSprite* main_case;
	CCSprite* top_case;
	
	bool is_menu_enable;
	bool is_saved_user_data;
	bool is_loaded_list;
	
	CCMenu* main_menu;
	CCMenu* replay_menu;
	//	CCMenu* help_menu;
	
	CCLabelBMFont* score_label;
	CCLabelBMFont* gold_label;
	CCLabelBMFont* time_label;
	
	HeartTime* heart_time;
	
	void closeReplayPopup();
	void popupClose();
	
	vector<FailFriendRank> friend_list;
	CCTableView* rankTableView;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void cellAction(CCObject* sender)
	{
		int tag = ((CCNode*)sender)->getTag();
		tag -= kFFC_T_menuBase;
		
		CCLog("help memberID : %s", friend_list[tag].user_id.c_str());
	}
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx){
		
		CCLabelTTF* nickname_label;
		CCLabelTTF* score_label;
		CCLabelTTF* rank_label;
		FailFriendRank* member = &friend_list[idx];
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kFFC_T_img);
		profileImg->setPosition(ccp(27, 20));
		profileImg->setScale(33.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kFFC_Z_img);
		
		CCSprite* bg = CCSprite::create("ending_cell_back.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kFFC_Z_case);
		
		nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
		nickname_label->setPosition(ccp(47,22));
		nickname_label->setAnchorPoint(CCPointZero);
		nickname_label->setTag(kFFC_T_nickname);
		cell->addChild(nickname_label,kFFC_Z_img);
		
		score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 18);
		score_label->setPosition(ccp(47,0));
		score_label->setAnchorPoint(CCPointZero);
		score_label->setTag(kFFC_T_score);
		cell->addChild(score_label,kFFC_Z_img);
		
		rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 10);
		rank_label->setPosition(ccp(12,3));
		rank_label->setAnchorPoint(CCPointZero);
		rank_label->setTag(kFFC_T_rank);
		cell->addChild(rank_label,kFFC_Z_img);
		
		if((*member).user_id == hspConnector::get()->getKakaoID())
		{
			CCSprite* meBack = CCSprite::create("ending_cell_selected.png");
			meBack->setPosition(ccp(meBack->getContentSize().width - bg->getContentSize().width, 0));
			meBack->setAnchorPoint(CCPointZero);
			cell->addChild(meBack,kFFC_Z_case);
		}
		else
		{
			if(!(*member).is_message_blocked)
			{
				CCSprite* n_help = CCSprite::create("ending_help_on.png");
				CCSprite* s_help = CCSprite::create("ending_help_on.png");
				s_help->setColor(ccGRAY);
				
				CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailPopup::cellAction));
				help_item->setTag(kFFC_T_menuBase + idx);
				
				CCMenu* help_menu = CCMenu::createWithItem(help_item);
				help_menu->setPosition(ccp(165,21));
				cell->addChild(help_menu, kFFC_Z_img);
			}
			else
			{
				CCSprite* not_help = CCSprite::create("ending_help_off.png");
				not_help->setPosition(ccp(165,21));
				cell->addChild(not_help, kFFC_Z_img);
			}
		}
		
		return cell;
	}
	
	virtual void scrollViewDidScroll(CCScrollView* view) {
	}
	
    virtual void scrollViewDidZoom(CCScrollView* view) {
		
	}
    
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){
		// 영호
		//		CCLog("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
		
		
	}
    virtual CCSize cellSizeForTable(CCTableView *table){
		return CCSizeMake(195, 45);
	}
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table){
		return friend_list.size();
	}
	
	
	float keep_gold;
	float decrease_gold;
	float increase_gold;
	
	float keep_score;
	float decrease_score;
	float increase_score;
	
	float keep_time;
	float decrease_time;
	float increase_time;
	
	void startCalcAnimation();
	void startScoreAnimation();
	void scoreAnimation(float dt);
	void stopScoreAnimation();
	void startGoldAnimation();
	void goldAnimation(float dt);
	void stopGoldAnimation();
	void startTimeAnimation();
	void timeAnimation(float dt);
	void stopTimeAnimation();
	
	CCPoint getContentPosition(int t_tag);
	
	void endLoad();
	
	void resultLoadFriends(Json::Value result_data);
	void resultGetStageScoreList(Json::Value result_data);
	void resultSavedUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__FailPopup__) */