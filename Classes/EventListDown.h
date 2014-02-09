//
//  EventListDown.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 9..
//
//

#ifndef __DGproto__EventListDown__
#define __DGproto__EventListDown__

#include "cocos2d.h"
//#include "GraphDog.h"
#include "DownloadFile.h"
#include "hspConnector.h"


USING_NS_CC;
using namespace std;

enum ELD_Zorder{
	kELD_Z_back = 1,
	kELD_Z_content
};

enum ELD_MenuTag{
	kELD_MT_receive = 1,
	kELD_MT_redown
};

class IntPoint;

class EventListDown : public CCLayer
{
public:
	static EventListDown* create(CCObject* t_success, SEL_CallFunc d_success);
	
private:
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	
	CCSprite* gray;
	CCSprite* tip_img;
	void changeTipImage();
	
	CCLabelTTF* state_ment;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int download_version;
	
	vector<DownloadFile> df_list;
	
	void myInit(CCObject* t_success, SEL_CallFunc d_success);
	
	void successAction();
	void failAction();
	void downloadingAction();
	
	void startGetEventList();
	
	void resultGetEventList(Json::Value result_data);
	
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

#endif /* defined(__DGproto__EventListDown__) */
