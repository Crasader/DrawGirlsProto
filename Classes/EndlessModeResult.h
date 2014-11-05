//
//  EndlessModeResult.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 20..
//
//

#ifndef __DGproto__EndlessModeResult__
#define __DGproto__EndlessModeResult__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include "KSProtect.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class KSLabelTTF;
class LoadingLayer;
class DownloadFile;
//class CopyFile;
class CCMenuLambda;
class EndlessModeResult : public CCLayer, public CCTableViewDataSource, public CCScrollViewDelegate
{
public:
	virtual bool init();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	CREATE_FUNC(EndlessModeResult);
	
	virtual void onEnterTransitionDidFinish();
	
	std::function<void()> refresh_achieve_func;
	
private:
	
	int touch_priority;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	TouchSuctionLayer* suction;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	CCSprite* main_case;
	
	CCSprite* result_stamp;
	
	LoadingLayer* ready_loading;
	
	virtual CCSize cellSizeForTable(CCTableView *table) {
        return CCSizeMake((480-30)/2.f-20, 21);
    };
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	KSProtectVar<int> left_life_base_score;
	KSProtectVar<float> left_life_decrease_score, left_time_base_score, left_time_decrease_score, left_grade_base_score, left_grade_decrease_score;
	KSProtectVar<int> left_damaged_score;
	KSProtectVar<int> left_total_score;
	
	KSProtectVar<int> right_area_score, right_damage_score, right_combo_score, right_life_cnt, right_game_time, right_clear_grade;
	KSProtectVar<int> right_life_base_score;
	KSProtectVar<float> right_life_decrease_score, right_time_base_score, right_time_decrease_score, right_grade_base_score, right_grade_decrease_score;
	KSProtectVar<int> right_damaged_score;
	KSProtectVar<int> right_total_score;
	
	function<void(function<void()>)> gold_calc_func;
	
	CCScale9Sprite* left_back;
	CCScale9Sprite* right_back;
	
	CCControlButton* next_button;
	CCControlButton* stop_button;
	
	CCTableView* left_table;
	CCTableView* right_table;
	
	vector<string> title_list;
	vector<string> left_content_list;
	vector<string> right_content_list;
	
	KSLabelTTF* left_total_content;
	KSLabelTTF* right_total_content;
	
	KSLabelTTF* left_gold_content;
	KSLabelTTF* right_gold_content;
	
	vector<CommandParam> send_command_list;
	void tryTransaction();
	
	void startCalcAnimation();
	
	float keep_left_value;
	float keep_right_value;
	float base_left_value;
	float base_right_value;
	float decrease_left_value;
	float decrease_right_value;
	float increase_left_value;
	float increase_right_value;
	float left_calc_time;
	float right_calc_time;
	bool is_left_decrease;
	bool is_right_decrease;
	KSLabelTTF* left_decrease_target;
	KSLabelTTF* right_decrease_target;
	
	function<void()> left_end_func;
	function<void()> right_end_func;
	
	bool is_left_calc_end;
	bool is_right_calc_end;
	
	bool is_calc;
	
	void startLeftCalcAnimation(float t_keep_value, float t_base_value, float t_calc_time, KSLabelTTF* t_decrease_target, function<void()> t_end_func);
	void leftCalcAnimation(float dt);
	void stopLeftCalcAnimation();
	
	void startRightCalcAnimation(float t_keep_value, float t_base_value, float t_calc_time, KSLabelTTF* t_decrease_target, function<void()> t_end_func);
	void rightCalcAnimation(float dt);
	void stopRightCalcAnimation();
	
	void flipLeft(string t_top_title, int t_top_content, string t_center_title, int t_center_content, string t_bottom_title, int t_bottom_content, function<void()> t_end_func);
	
	void flipTarget(CCScale9Sprite* t_back, KSLabelTTF* t_title, KSLabelTTF* t_content, string t_flip_title, int t_flip_content);
	
	void flipRight(string t_top_title, int t_top_content, string t_center_title, int t_center_content, string t_bottom_title, int t_bottom_content, function<void()> t_end_func);
	
	vector<DownloadFile> df_list;
//	vector<CopyFile> cf_list;
	
	void setMain();
	
	void resultGetEndlessPlayData(Json::Value result_data);
	void saveStageInfo(Json::Value result_data);
	
	void reSetEndlessRank();
	
	int download_version;
	int ing_download_cnt;
	float ing_download_per;
	
	void startDownload();
	void successAction();
	void failAction();
	
	int stage_number;
	
	void successGetStageInfo();
	
	void menuAction(CCObject* sender);
	void controlButtonAction(CCObject* sender, CCControlEvent t_event);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	
	CCParticleSystemQuad* getStarParticle();
	void addStarParticle(CCNode* t_node);
};

#endif /* defined(__DGproto__EndlessModeResult__) */
