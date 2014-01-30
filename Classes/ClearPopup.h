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

class RankFriendInfo;
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
	
	CCNode* tutorial_node;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCSprite* main_case;
	
	bool is_menu_enable;
	bool is_saved_user_data;
	bool is_loaded_list;
	
	CCSprite* bronze_star;
	CCSprite* silver_star;
	CCSprite* gold_star;
	
	CCMenu* ok_menu;
	CCMenu* replay_menu;
	
	CCLabelBMFont* score_label;
	CCLabelBMFont* gold_label;
	CCLabelBMFont* time_label;
	
	bool is_rank_changed;
	int before_my_rank;
	int recent_my_rank;
	RankFriendInfo next_rank_info;
	
	CCSprite* take_star_animation_node;
	
	vector<RankFriendInfo> friend_list;
	CCTableView* rankTableView;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void cellAction(CCObject* sender);
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
	
    virtual void scrollViewDidZoom(CCScrollView* view);
    
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void endTakeCard();
	void checkMiniGame();
	void checkRentCard();
	void checkChallengeOrHelp();
	
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
