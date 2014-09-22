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
class KSLabelTTF;
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
	
	std::function<void()> replay_func;
	std::function<void()> goToMainFlow_func;
	std::function<void()> refresh_achieve_func;
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	virtual void onEnterTransitionDidFinish();
	
	virtual ~FailPopup();
	
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
	
	CCControlButton* main_menu;
	CCControlButton* replay_menu;
	
	bool is_end_take_diary;
	bool is_end_network;
	
	void controlButtonAction(CCObject* sender, CCControlEvent t_event);
	
	//	CCMenu* help_menu;
	
//	KSLabelTTF* score_label;
//	KSLabelTTF* gold_label;
//	KSLabelTTF* time_label;
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

	CCScale9Sprite* inner_left;
	
	CCTableView* left_table;
	
	vector<string> title_list;
	vector<string> left_content_list;
	
	KSProtectVar<int> left_life_base_score;
	KSProtectVar<float> left_life_decrease_score, left_time_base_score, left_time_decrease_score, left_grade_base_score, left_grade_decrease_score;
	KSProtectVar<int> left_damaged_score;
	KSProtectVar<int> left_total_score;
	KSProtectVar<float> play_limit_time;
	
	KSLabelTTF* left_total_content;
	
	float keep_left_value;
	float base_left_value;
	float decrease_left_value;
	float increase_left_value;
	float left_calc_time;
	bool is_left_decrease;
	KSLabelTTF* left_decrease_target;
	
	function<void()> left_end_func;
	
	void startLeftCalcAnimation(float t_keep_value, float t_base_value, float t_calc_time, KSLabelTTF* t_decrease_target, function<void()> t_end_func);
	void leftCalcAnimation(float dt);
	void stopLeftCalcAnimation();
	
	KSLabelTTF* n_replay_label2;
	KSLabelTTF* s_replay_label2;

//	float keep_gold;
//	float decrease_gold;
//	float increase_gold;
//	
//	float keep_score;
//	float decrease_score;
//	float increase_score;
//	
//	float keep_time;
//	float decrease_time;
//	float increase_time;
	
	void startCalcAnimation();
//	void startScoreAnimation();
//	void scoreAnimation(float dt);
//	void stopScoreAnimation();
//	void startGoldAnimation();
//	void goldAnimation(float dt);
//	void stopGoldAnimation();
//	void startTimeAnimation();
//	void timeAnimation(float dt);
//	void stopTimeAnimation();
	
	CCPoint getContentPosition(int t_tag);
	
	void closePopup();
	void endLoad();
	
	vector<CommandParam> send_command_list;
	
	bool is_today_mission_success;
	
	void tryTransaction(CCNode* t_loading);
	
	TouchSuctionLayer* suction;
	
//	void resultLoadFriends(Json::Value result_data);
//	void resultGetStageScoreList(Json::Value result_data);
	void resultSavedUserData(Json::Value result_data);
	
	CCSprite* loading_img;
	
	void resultGetRank(Json::Value result_data);
	bool is_go_to_mainflow;
	void resultGetTime(Json::Value result_data);
	
	void resultCompleteIntroducer(Json::Value result_data);
};

#endif /* defined(__DGproto__FailPopup__) */
