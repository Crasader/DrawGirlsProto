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
#include "hspConnector.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

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

class TouchSuctionLayer;
class RankFriendInfo;
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
//	CCSprite* top_case;
	
	bool is_menu_enable;
	bool is_saved_user_data;
//	bool is_loaded_list;
	bool is_end_popup_animation;
	
	CCMenu* main_menu;
	CCMenu* replay_menu;
	//	CCMenu* help_menu;
	
	CCLabelTTF* score_label;
	CCLabelTTF* gold_label;
	CCLabelTTF* time_label;
//
//	CCSprite* friend_point_popup;
//	void closeFriendPointPopup();
//	
//	HeartTime* heart_time;
	
	void endDecreaseCardDuration();
	
//	vector<RankFriendInfo> friend_list;
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
	
	void closePopup();
	void endLoad();
	
	TouchSuctionLayer* suction;
	
//	void resultLoadFriends(Json::Value result_data);
//	void resultGetStageScoreList(Json::Value result_data);
	void resultSavedUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__FailPopup__) */
