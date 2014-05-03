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
#include "StarGoldData.h"
#include "hspConnector.h"
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

class KSLabelTTF;
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
	
	bool is_take_star_effect;
	std::function<void()> replay_func;
	std::function<void()> goToMainFlow_func;
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	virtual ~ClearPopup();
	
private:
	
	CCNode* tutorial_node;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCScale9Sprite* main_case;
	
	CCSprite* title;
	
	Json::Value rank_data;
	
	bool is_menu_enable;
	bool is_saved_user_data;
//	bool is_loaded_list;
	bool is_end_popup_animation;
	
	bool is_end_take_card;
	
	int delay_index;
	vector<function<void()>> cell_action_list;
	vector<CommandParam> send_command_list;
	
	vector<CCSprite*> ani_stars;
	
	CCMenu* ok_menu;
	CCMenu* replay_menu;
	
	KSLabelTTF* score_label;
	KSLabelTTF* gold_label;
	KSLabelTTF* time_label;
	
	bool is_rank_changed;
	int before_my_rank;
	int recent_my_rank;
//	RankFriendInfo next_rank_info;
	
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
	
	bool is_end_call_score_calc;
	function<void()> end_score_calc_func;
	
	float base_score;
	float keep_score;
	float decrease_score;
	float increase_score;
	void startScoreAnimation(float t_score);
	void scoreAnimation(float dt);
	void stopScoreAnimation();
	
	bool is_high_score;
	bool is_today_mission_success;
	
	float keep_time;
	float decrease_time;
	float increase_time;
	void startTimeAnimation();
	void timeAnimation(float dt);
	void stopTimeAnimation();
	
	void startCalcAnimation();
	
	void starSound();
	
	void closePopup();
	void endLoad();
	
	void tryTransaction(CCNode* t_loading);
	
//	void resultLoadFriends(Json::Value result_data);
//	void resultGetStageScoreList(Json::Value result_data);
	void resultSavedUserData(Json::Value result_data);
	
	CCSprite* loading_img;
	
	CCParticleSystemQuad* getStarParticle();
	void addStarParticle(CCNode* t_node);
	
	void resultGetRank(Json::Value result_data);
	void resultGetTime(Json::Value result_data);
	bool is_go_to_mainflow;
};

#endif /* defined(__DGproto__ClearPopup__) */
