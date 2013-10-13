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
#include "GraphDog.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "DownloadFile.h"

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
	static StageListDown* create(CCObject* t_success, SEL_CallFunc d_success)
	{
		StageListDown* t_sid = new StageListDown();
		t_sid->myInit(t_success, d_success);
		t_sid->autorelease();
		return t_sid;
	}
	
private:
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	
	CCLabelTTF* state_ment;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int download_version;
	
	vector<DownloadFile> df_list;
	
	void myInit(CCObject* t_success, SEL_CallFunc d_success)
	{
		target_success = t_success;
		delegate_success = d_success;
		
		state_ment = CCLabelTTF::create("스테이지 목록을 확인하는ing...", mySGD->getFont().c_str(), 20);
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
		param["version"] = SDS_GI(kSDF_gameInfo, "version");
		graphdog->command("getstagelist", param, this, gd_selector(StageListDown::resultGetStageList));
	}
	
	void resultGetStageList(Json::Value result_data);
	
	void menuAction(CCObject* sender);
	
	void startDownload();
};

#endif /* defined(__DGproto__StageListDown__) */
