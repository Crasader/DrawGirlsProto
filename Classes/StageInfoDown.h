//
//  StageInfoDown.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 24..
//
//

#ifndef __DGproto__StageInfoDown__
#define __DGproto__StageInfoDown__

#include "cocos2d.h"
#include "hspConnector.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "DownloadFile.h"
#include "LogData.h"

USING_NS_CC;
using namespace std;

enum SID_Zorder{
	kSID_Z_back = 1,
	kSID_Z_content
};

enum SID_MenuTag{
	kSID_MT_cancel = 1
};

class CommonButton;
class StageInfoDown : public CCLayer
{
public:
	static StageInfoDown* create(CCObject* t_success, SEL_CallFunc d_success, CCObject* t_cancel, SEL_CallFunc d_cancel);
private:
	CCObject* target_cancel;
	SEL_CallFunc delegate_cancel;
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	
	int touch_number;
	bool is_menu_enable;
	
	CCSprite* gray;
	
	CCLabelTTF* state_ment;
	CommonButton* cancel_menu;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int download_version;
	
	vector<DownloadFile> df_list;
//	vector<CopyFile> cf_list;
	
	void myInit(CCObject* t_success, SEL_CallFunc d_success, CCObject* t_cancel, SEL_CallFunc d_cancel);
	
	void startGetCardsInfo();
	void resultLoadedCardData( Json::Value result_data );
	void startDownloadCardImage();
	void startDownload2();
	void successAction2();
	void failAction2();
	
	void successAction();
	void failAction();
	void downloadingAction();
	
	void startGetStageInfo();
	
	void resultGetStageInfo(Json::Value result_data);
	
	void startDownload();
	
	void menuAction(CCObject* sender);
	
	
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -500, true);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLOG("touch swallow popup");
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

#endif /* defined(__DGproto__StageInfoDown__) */
