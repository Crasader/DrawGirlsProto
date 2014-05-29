//
//  EndlessModeOpening.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 19..
//
//

#ifndef __DGproto__EndlessModeOpening__
#define __DGproto__EndlessModeOpening__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include "KSProtect.h"
#include "ScrollBar.h"

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
};

class KSLabelTTF;
class LoadingLayer;
class DownloadFile;
class CopyFile;
class CommonButton;
class StyledLabelTTF;
class EndlessModeOpening : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	EndlessModeOpening()
	{
		currentSelectedCell = nullptr;
		m_scrollBar = nullptr;
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
	
	CREATE_FUNC(EndlessModeOpening);
	
private:
	
	int touch_priority;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	TouchSuctionLayer* suction;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	CCSprite* loading_circle;
	CCSprite* loading_right_circle;
	CCScale9Sprite* main_case;
	
	KSProtectVar<int> myscore;
	KSProtectVar<int> victory;
	KSProtectVar<int> alluser;
	KSProtectVar<int> myrank;
	KSProtectVar<int> remainTime;
	KSLabelTTF* highscore_content;
	KSLabelTTF* straight_content;
	StyledLabelTTF* record_content;
	
	KSLabelTTF* n_ready_label;
	KSLabelTTF* n_ready_label2;
	KSLabelTTF* s_ready_label;
	KSLabelTTF* s_ready_label2;
	
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
	vector<CopyFile> cf_list;
	CCScale9Sprite* currentSelectedCell;
	int currentSelectedIdx;
	ScrollBar* m_scrollBar;
	CommonButton* mySelection;
	CCSprite* right_flag;
	KSLabelTTF* right_nick;
	CCScale9Sprite* right_back;
	CCScale9Sprite* record_back;
	void setMain();
	
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
		if(m_scrollBar)
		{
			m_scrollBar->setBarRefresh();
		}
	}
	virtual void scrollViewDidZoom(CCScrollView* view){}
	void putInformation(Json::Value info);
};

#endif /* defined(__DGproto__EndlessModeOpening__) */
