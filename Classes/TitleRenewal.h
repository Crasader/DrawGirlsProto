//
//  TitleRenewal.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 9..
//
//

#ifndef __DGproto__TitleRenewal__
#define __DGproto__TitleRenewal__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "PuzzleMapScene.h"
#include "hspConnector.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"
#include "LogData.h"
#include "StageImgLoader.h"
#include "cocos-ext.h"
#include "FlagSelector.h"
#include "BackKeyBase.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum TitleRenewalMenuTag
{
	kTitleRenewal_MT_replay = 1,
	kTitleRenewal_MT_redown,
	kTitleRenewal_MT_nick,
	kTitleRenewal_MT_puzzleBase = 10000
};

enum ReceiveState{
	kReceiveState_notRecieved = 0,
	kReceiveState_Received
};

class KSLabelTTF;
class ConvexGraph;
class TitleRenewalScene : public cocos2d::CCLayer, public CCEditBoxDelegate//, public BackKeyBase//, public CCTextFieldDelegate
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(TitleRenewalScene);
	
	virtual void onEnterTransitionDidFinish();
	
private:
	KSLabelTTF* state_label;
	CCSprite* white_back;
	CCSprite* title_img;
	CCBAnimationManager* title_manager;
//	CCSprite* title_name;
	KSLabelTTF* download_state;
	CCSprite* black_img;
	vector<string> tip_list;
	int recent_tip_index;
	
	bool is_loaded_cgp;
	bool is_loaded_server;
    bool is_loaded_productInfo;
	int loginCnt;
	int ing_download_cnt;
	float ing_download_per;
	int success_download_cnt;
	
	bool is_downloading;
	int puzzlelist_download_version;
	
	vector<int> card_data_load_list;
	
	CCEditBox* input_text;
	FlagSelector* flag;
	CCSprite* nick_back;
	
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
	
	void changeTipMent();
	
//	CCProgressTimer* progress_timer;
	ConvexGraph* progress_timer;
	
	void resultLogin(Json::Value result_data);
	
	void resultHSLogin(Json::Value result_data);
	void joinAction();
	
	bool is_preloaded_effect;
	
	void successLogin();
	
	vector<CommandParam> command_list;
//	bool must_be_load_friends;
	int receive_cnt;
	
	bool is_receive_fail;
	
	void startCommand();
	void checkReceive();
	void startFileDownload();
	void downloadingFileAction();
	void successDownloadAction();
	void failDownloadAction();
	void checkDownloading();
	
	vector<DownloadImgInfo> download_set;
	void startFileDownloadSet();
	vector<int> is_enable_index;
	std::function<void(Json::Value value)> termsFunctor;
//	void successDownloadActionSet(string t_filename);
//	void failDownloadActionSet(string t_filename);
	
    void resultRequestItemDelivery(Json::Value result_data);
	void resultGetCommonSetting(Json::Value result_data);
	void resultGetPuzzleEvent(Json::Value result_data);
	void resultGetAchieveList(Json::Value result_data);
	void resultGetShopList(Json::Value result_data);
	void resultGetNoticeList(Json::Value result_data);
	void resultGetTimeEvent(Json::Value result_data);
	void resultGetHellModeList(Json::Value result_data);
	
	void resultGetCharacterInfo(Json::Value result_data);
	vector<DownloadFile> character_download_list;
	int character_download_version;
	
	void resultGetMonsterList(Json::Value result_data);
	vector<DownloadFile> monster_download_list;
	int monster_download_version;
	
	void resultGetHeart(Json::Value result_data);
	
	void resultGetUserData(Json::Value result_data);
	
	void resultCheckAttendanceEvent(Json::Value result_data);
	void resultCheckWeeklyReward(Json::Value result_data);
	
	void resultGetAchieveHistory(Json::Value result_data);
	void resultGetCardHistory(Json::Value result_data);
	void resultGetPuzzleHistory(Json::Value result_data);
	void resultGetPieceHistory(Json::Value result_data);
	void resultGetCharacterHistory(Json::Value result_data);
	void resultGetTodayMission(Json::Value result_data);
	
	void resultGetUserProperties(Json::Value result_data);
	
	void resultGetTimeInfo(Json::Value result_data);
	
    void resultRequestProductInfos(Json::Value result_data);
    
    int reduce_frame;
    void reduceAction();
    
	int audio_preload_step;
	void nextPreloadStep();
	void checkThreadPreload();
	
	Json::Value friendList;
	
	void resultLoadedCardData(Json::Value result_data);
	vector<DownloadFile> card_download_list;
//	vector<CopyFile> card_reduction_list;
	
//	void resultGetKnownFriendList(Json::Value result_data);
//	void startGetKnownFriendUserData();
//	void resultGetKnownFriendUserData(Json::Value result_data);
//	
//	void startGetUnknownFriendUserData();
//	void resultGetUnknownFriendUserData(Json::Value result_data);
	
	void resultGetPuzzleList(Json::Value result_data);
	vector<DownloadFile> puzzle_download_list;
	vector<int> puzzle_download_list_puzzle_number;
	
//	void resultGetPathInfo(Json::Value result_data);
	
	int splash_load_cnt;
	int splash_ing_cnt;
	
	int card_data_version;
	
	void loadCounting(CCObject* sender);
	
	void endSplash();
	void realInit();
	
	void endingCheck();
	void endingAction();
	void changeScene();
	
	bool is_menu_enable;
	void menuAction(CCObject* sender);
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
	
};

#endif /* defined(__DGproto__TitleRenewal__) */
