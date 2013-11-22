//
//  FailScene.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#ifndef __galsprototype__FailScene__
#define __galsprototype__FailScene__

#include "cocos2d.h"
#include "AlertEngine.h"
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

class FailScene : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(FailScene);
	
	virtual void onEnter();
	
private:
	
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
	
	void cellAction(CCObject* sender);
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
	
    virtual void scrollViewDidZoom(CCScrollView* view);
    
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void endCall();
	
	
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
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
};

#endif /* defined(__galsprototype__FailScene__) */
