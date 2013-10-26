//
//  StageListDown.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 10..
//
//

#ifndef __DGproto__StageListDown__
#define __DGproto__StageListDown__

#include "cocos2d.h"
//#include "GraphDog.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "DownloadFile.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

enum SLD_Zorder{
	kSLD_Z_back = 1,
	kSLD_Z_content
};

enum SLD_MenuTag{
	kSLD_MT_receive = 1,
	kSLD_MT_redown
};

class StageListDown : public CCLayer
{
public:
	static StageListDown* create(CCObject* t_success, SEL_CallFunc d_success, int t_puzzle)
	{
		StageListDown* t_sid = new StageListDown();
		t_sid->myInit(t_success, d_success, t_puzzle);
		t_sid->autorelease();
		return t_sid;
	}
	
private:
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	
	int puzzle_number;
	
	CCLabelTTF* state_ment;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int download_version;
	
	vector<DownloadFile> df_list;
	vector<DownloadFile> ef_list;
	
	void myInit(CCObject* t_success, SEL_CallFunc d_success, int t_puzzle)
	{
		puzzle_number = t_puzzle;
		target_success = t_success;
		delegate_success = d_success;
		
		state_ment = CCLabelTTF::create("퍼즐 정보를 확인하는ing...", mySGD->getFont().c_str(), 20);
		state_ment->setAnchorPoint(ccp(0.5,0.5));
		state_ment->setPosition(ccp(240,160));
		state_ment->setHorizontalAlignment(kCCTextAlignmentCenter);
		state_ment->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		addChild(state_ment, kSLD_Z_content);
		
		is_downloading = false;
		
		startGetStageList();
	}
	
	void successAction();
	void failAction();
	void downloadingAction();
	
	void startGetStageList()
	{
		Json::Value param;
		param["version"] = NSDS_GI(puzzle_number, kSDS_PZ_version_i);
		param["no"] = puzzle_number;
		hspConnector::get()->command("getpuzzleinfo", param, json_selector(this, StageListDown::resultGetStageList));
	}
	
	void addDownlist(string t_key, const Json::Value& result_data);
	
	void resultGetStageList(Json::Value result_data);
	
	void menuAction(CCObject* sender);
	
	void startDownload();
};

#endif /* defined(__DGproto__StageListDown__) */
