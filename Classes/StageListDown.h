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
#include "cocos-ext.h"
//#include "GraphDog.h"
#include "DownloadFile.h"
#include "hspConnector.h"


USING_NS_CC;
USING_NS_CC_EXT;
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
class CumberShowWindow;
class KSLabelTTF;
class DownloadImgInfo;
class ConvexGraph;
class StageListDown : public CCLayer
{
public:
	static StageListDown* create(CCObject* t_success, SEL_CallFunc d_success, int t_puzzle, function<void(function<void()>)> t_download_start = nullptr, function<void()> t_success_func = nullptr);
	
private:
	CCObject* target_success;
	SEL_CallFunc delegate_success;
	
	function<void(function<void()>)> download_start;
	function<void()> success_func;
	
	CCSprite* gray;
	CCNode* tip_img;
	void changeTipImage();
	
	void startOpenning();
	void endOpenning();
	
	int puzzle_number;
	
	KSLabelTTF* talk_label;
	KSLabelTTF* state_ment;
	KSLabelTTF* download_state;
	
	CumberShowWindow* loading_character;
	CCScale9Sprite* talk_box;
//	CCSprite* progress_back;
//	CCProgressTimer* loading_progress;
	ConvexGraph* loading_progress;
	
	void outOpenning();
	
	int recent_tip_index;
	vector<string> tip_list;
	
	void changeTipMent();
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int download_version;
	
	vector<DownloadFile> df_list;
	vector<DownloadFile> cut_list;
	vector<DownloadFile> sf_list;
//	vector<CopyFile> cf_list;
	
	vector<DownloadImgInfo> download_set;
	
	vector<IntPoint> save_version_list;
	
	void myInit(CCObject* t_success, SEL_CallFunc d_success, int t_puzzle, function<void(function<void()>)> t_download_start, function<void()> t_success_func);
	
	void successAction();
	void failAction();
	void downloadingAction();
	
	vector<int> is_enable_index;
	
	int success_download_cnt;
	void checkDownloading();
	
	void startGetStageList();
	
	void addDownlist(string t_key, const Json::Value& result_data);
	
	void resultGetStageList(Json::Value result_data);
	void resultGetStageRankList(Json::Value result_data);
	
	void menuAction(CCObject* sender);
	
    int reduce_frame;
    void reduceAction();
    
	void startDownload();
	void startDownloadSet();
	
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -200, true);
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

#endif /* defined(__DGproto__StageListDown__) */
