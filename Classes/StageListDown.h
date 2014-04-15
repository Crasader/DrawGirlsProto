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

class IntPoint;

class StageListDown : public CCLayer
{
public:
	static StageListDown* create(CCObject* t_success, SEL_CallFunc d_success, int t_puzzle);
	
private:
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	
	CCSprite* gray;
	CCNode* tip_img;
	void changeTipImage();
	
	int puzzle_number;
	
	CCLabelTTF* state_ment;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int download_version;
	
	vector<DownloadFile> df_list;
	vector<DownloadFile> cut_list;
	vector<DownloadFile> sf_list;
	vector<CopyFile> cf_list;
	
	vector<IntPoint> save_version_list;
	
	void myInit(CCObject* t_success, SEL_CallFunc d_success, int t_puzzle);
	
	void successAction();
	void failAction();
	void downloadingAction();
	
	void startGetStageList();
	
	void addDownlist(string t_key, const Json::Value& result_data);
	
	void resultGetStageList(Json::Value result_data);
	void resultGetStageRankList(Json::Value result_data);
	
	void menuAction(CCObject* sender);
	
	void startDownload();
	
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -200, true);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLog("touch swallow popup");
		return true;
	}
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		
	}
};

#endif /* defined(__DGproto__StageListDown__) */
