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
class CopyFile;
class EndlessModeResult : public CCLayer
{
public:
	virtual bool init();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	CREATE_FUNC(EndlessModeResult);
	
private:
	
	int touch_priority;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	TouchSuctionLayer* suction;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	CCScale9Sprite* main_case;
	
	LoadingLayer* ready_loading;
	
	KSLabelTTF* n_stop_label2;
	KSLabelTTF* s_stop_label2;
	KSLabelTTF* n_next_label2;
	KSLabelTTF* s_next_label2;
	
	vector<DownloadFile> df_list;
	vector<CopyFile> cf_list;
	
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
};

#endif /* defined(__DGproto__EndlessModeResult__) */
