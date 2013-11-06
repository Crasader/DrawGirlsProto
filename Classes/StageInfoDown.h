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

USING_NS_CC;
using namespace std;

enum SID_Zorder{
	kSID_Z_back = 1,
	kSID_Z_content
};

enum SID_MenuTag{
	kSID_MT_cancel = 1
};

class StageInfoDown : public CCLayer
{
public:
	static StageInfoDown* create(CCObject* t_cancel, SEL_CallFunc d_cancel)
	{
		StageInfoDown* t_sid = new StageInfoDown();
		t_sid->myInit(t_cancel, d_cancel);
		t_sid->autorelease();
		return t_sid;
	}
	
private:
	CCObject* target_cancel;
	SEL_CallFunc delegate_cancel;
	
	int touch_number;
	bool is_menu_enable;
	
	CCLabelTTF* state_ment;
	CCMenu* cancel_menu;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int download_version;
	
	vector<DownloadFile> df_list;
	vector<CopyFile> cf_list;
	
	void myInit(CCObject* t_cancel, SEL_CallFunc d_cancel)
	{
		target_cancel = t_cancel;
		delegate_cancel = d_cancel;
		
		CCSprite* t_back = CCSprite::create("back_gray.png");
		t_back->setPosition(ccp(240,160));
		addChild(t_back, kSID_Z_back);
		
		CCMenuItem* cancel_item = CCMenuItemImage::create("sspl_cancel.png", "sspl_cancel.png", this, menu_selector(StageInfoDown::menuAction));
		cancel_item->setTag(kSID_MT_cancel);
		
		cancel_menu = CCMenu::createWithItem(cancel_item);
		cancel_menu->setPosition(ccp(350, 240));
		addChild(cancel_menu, kSID_Z_content);
		
		state_ment = CCLabelTTF::create("스테이지 정보를 받아오는 ing...", mySGD->getFont().c_str(), 20);
		state_ment->setAnchorPoint(ccp(0.5,0.5));
		state_ment->setPosition(ccp(240,160));
		state_ment->setHorizontalAlignment(kCCTextAlignmentCenter);
		state_ment->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		addChild(state_ment, kSID_Z_content);
		
		is_downloading = false;
		
		startGetStageInfo();
		
		touch_number = 0;
		is_menu_enable = true;
		setTouchEnabled(true);
	}
	
	void successAction();
	void failAction();
	void downloadingAction();
	
	void startGetStageInfo()
	{
		int stage_number = mySD->getSilType();
		
		if(stage_number < 10000)
		{
			Json::Value param;
			param["no"] = stage_number;
			param["version"] = NSDS_GI(stage_number, kSDS_SI_version_i);
			hspConnector::get()->command("getstageinfo", param, json_selector(this, StageInfoDown::resultGetStageInfo));
		}
		else // event stage
		{
			Json::Value param;
			param["no"] = stage_number;
			param["version"] = NSDS_GI(stage_number, kSDS_SI_version_i);
			hspConnector::get()->command("geteventstageinfo", param, json_selector(this, StageInfoDown::resultGetStageInfo));
		}
	}
	
	void resultGetStageInfo(Json::Value result_data);
	
	void startDownload();
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)	return;
		
		int tag = ((CCNode*)sender)->getTag();
		
		is_menu_enable = false;
		
		if(tag == kSID_MT_cancel)
		{
			graphdog->removeCommand(this);
			if(is_downloading)
				StageImgLoader::sharedInstance()->removeTD();
			(target_cancel->*delegate_cancel)();
			removeFromParent();
		}
	}
	
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__StageInfoDown__) */
