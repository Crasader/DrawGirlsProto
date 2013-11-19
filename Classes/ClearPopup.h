//
//  ClearPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#ifndef __DGproto__ClearPopup__
#define __DGproto__ClearPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GDWebSprite.h"
#include "hspConnector.h"
#include "StarGoldData.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class HeartTime;
class ClearFriendRank
{
public:
	string nickname;
	string img_url;
	string user_id;
	float score;
	bool is_play;
	
	bool operator==(string t_id)
	{
		return user_id == t_id;
	};
};

enum ClearFriendCell_Tag
{
	kCFC_T_score = 1,
	kCFC_T_rank,
	kCFC_T_nickname,
	kCFC_T_img
};

enum ClearFriendCell_Zorder
{
	kCFC_Z_case = 1,
	kCFC_Z_img
};

class ClearPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ClearPopup);
	
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
	
	CCMenu* ok_menu;
	CCMenu* replay_menu;
	
	HeartTime* heart_time;
	
	CCLabelBMFont* score_label;
	CCLabelBMFont* gold_label;
	CCLabelBMFont* time_label;
	
	vector<ClearFriendRank> friend_list;
	CCTableView* rankTableView;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void cellAction(CCObject* sender)
	{
		
	}
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx){
		
		CCLabelTTF* nickname_label;
		CCLabelTTF* score_label;
		CCLabelTTF* rank_label;
		ClearFriendRank* member = &friend_list[idx];
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kCFC_T_img);
		profileImg->setPosition(ccp(62, 20));
		profileImg->setScale(33.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kCFC_Z_img);
		
		CCSprite* bg = CCSprite::create("ending_cell_back.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kCFC_Z_case);
		
		if((*member).user_id == hspConnector::get()->getKakaoID())
		{
			CCSprite* meBack = CCSprite::create("ending_cell_selected.png");
			meBack->setPosition(ccp(meBack->getContentSize().width - bg->getContentSize().width, 0));
			meBack->setAnchorPoint(CCPointZero);
			cell->addChild(meBack,kCFC_Z_case);
		}
		
		nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
		nickname_label->setPosition(ccp(90,22));
		nickname_label->setAnchorPoint(CCPointZero);
		nickname_label->setTag(kCFC_T_nickname);
		cell->addChild(nickname_label,kCFC_Z_img);
		
		score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 18);
		score_label->setPosition(ccp(90,0));
		score_label->setAnchorPoint(CCPointZero);
		score_label->setTag(kCFC_T_score);
		cell->addChild(score_label,kCFC_Z_img);
		
		rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 23);
		rank_label->setPosition(ccp(10,4));
		rank_label->setAnchorPoint(CCPointZero);
		rank_label->setTag(kCFC_T_rank);
		cell->addChild(rank_label,kCFC_Z_img);
		
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
	void startGoldAnimation();
	void goldAnimation(float dt);
	void stopGoldAnimation();
	
	float keep_score;
	float decrease_score;
	float increase_score;
	void startScoreAnimation();
	void scoreAnimation(float dt);
	void stopScoreAnimation();
	
	float keep_time;
	float decrease_time;
	float increase_time;
	void startTimeAnimation();
	void timeAnimation(float dt);
	void stopTimeAnimation();
	
	void startCalcAnimation();
	
	void endLoad();
	
	void resultLoadFriends(Json::Value result_data);
	void resultGetStageScoreList(Json::Value result_data);
	void resultSavedUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__ClearPopup__) */
