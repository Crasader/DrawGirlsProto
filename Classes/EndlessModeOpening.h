//
//  EndlessModeOpening.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 19..
//
//
#ifndef END_TEST__
#define END_TEST__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include "KSProtect.h"

//#include "CommonButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;

class EndlessRankInfo
{
public:
	KSProtectVar<long long> memberID;
	KSProtectVar<int> regWeek;
	KSProtectVar<int> score;
	KSProtectStr nick;
	KSProtectVar<int> level;
	KSProtectStr flag;
	KSProtectVar<int> victory;
	KSProtectVar<long long> regDate;
	KSProtectStr data;
};

class KSLabelTTF;
class LoadingLayer;
class DownloadFile;
//class CopyFile;
class CommonButton;
class StyledLabelTTF;
class CCMenuLambda;
class CCMenuItemLambda;

class CellScale9Sprite : public CCScale9Sprite
{
public:
	function<void()> destroy_func;
	static CellScale9Sprite* create(const char* file, CCRect rect,  CCRect capInsets)
	{
		CellScale9Sprite* pReturn = new CellScale9Sprite();
		if ( pReturn && pReturn->initWithFile(file, rect, capInsets) )
		{
			pReturn->autorelease();
			return pReturn;
		}
		CC_SAFE_DELETE(pReturn);
		return NULL;
	}
	
	virtual ~CellScale9Sprite()
	{
		destroy_func();
	}
};

class EndlessModeOpening : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	EndlessModeOpening()
	{
		currentSelectedCell = nullptr;
		currentSelectedIdx = -1;
		right_flag = nullptr;
		right_nick = nullptr;
		record_content = nullptr;
	}
	virtual ~EndlessModeOpening()
	{
		myHSP->removeTarget(this);
	}
	virtual bool init();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	function<void()> refresh_ing_win_func;
	
	CREATE_FUNC(EndlessModeOpening);
	
private:
	
	int touch_priority;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	TouchSuctionLayer* suction;
	TouchSuctionLayer* t_suction;
	
	function<void()> tutorial_success_func;
	function<void()> tutorial_fail_func;
	
	function<void()> kind_tutorial_pvp;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	CCSprite* loading_circle;
	CCSprite* loading_right_circle;
	CCSprite* main_case;
	
	KSProtectVar<int> myscore;
	KSProtectVar<int> victory;
	KSProtectVar<int> alluser;
	KSProtectVar<int> myrank;
	KSProtectVar<int> remainTime;
	KSLabelTTF* highscore_content;
	KSLabelTTF* straight_content;
	StyledLabelTTF* record_content;
	
	CCNode* right_info_node;
	CCNode* right_reward_node;
	
	CCMenuLambda* right_top_menu;
	CCMenuItemLambda* info_item;
	CCMenuItemLambda* reward_item;
	
	CommonButton* ready_button;
	
	CCSprite* rank_percent_case;
	KSLabelTTF* percent_label;
	CCScale9Sprite* left_back;
	CCScale9Sprite* rest_back;
	KSLabelTTF* rest_time_title;
	KSLabelTTF* rest_time_value;
	
	CCTableView* rank_table;
	
	vector<EndlessRankInfo> rank_list;
	
	LoadingLayer* ready_loading;
	
	vector<DownloadFile> df_list;
//	vector<CopyFile> cf_list;
	CellScale9Sprite* currentSelectedCell;
	int currentSelectedIdx;
	CommonButton* mySelection;
	CCSprite* right_flag;
	KSLabelTTF* right_nick;
	CCScale9Sprite* right_back;
	CCScale9Sprite* record_back;
	CCTableView* reward_table;
	float current_rank_percent;
	CCNode* animation_node1;
	CCNode* animation_node2;
	void setMain();
	
	vector<CommandParam> transaction_list;
	
	void failTransaction();
	void resultGetEndlessPlayData(Json::Value result_data);
	void saveStageInfo(Json::Value result_data);
	
	int download_version;
	int ing_download_cnt;
	float ing_download_per;
	
	void startDownload();
	void successAction();
	void failAction();
	
	int stage_number;
	
	void successGetStageInfo();
	
	void resultGetEndlessRank(Json::Value result_data);
	
	void menuAction(CCObject* sender);
	
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
		return cellSizeForTable(table);
	};
	virtual CCSize cellSizeForTable(CCTableView *table) {
		return CCSizeMake(225, 32);
	};
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(CCScrollView* view){
	}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	void putInformation(Json::Value info);
};


#endif