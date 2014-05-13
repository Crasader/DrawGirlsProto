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

class TitleRenewalScene : public cocos2d::CCLayer, public CCEditBoxDelegate//, public CCTextFieldDelegate
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
	CCSprite* title_name;
	CCLabelBMFont* download_state;
	CCSprite* black_img;
	vector<string> tip_list;
	int recent_tip_index;
	
	bool is_loaded_cgp;
	bool is_loaded_server;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int puzzlelist_download_version;
	
	vector<int> card_data_load_list;
	
	CCEditBox* input_text;
	FlagSelector* flag;
	CCScale9Sprite* nick_back;
	
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
	
	void changeTipMent();
	
	CCProgressTimer* progress_timer;
	
	void resultLogin(Json::Value result_data);
	
	void resultHSLogin(Json::Value result_data);
	void joinAction();
	
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
	
	void resultGetCommonSetting(Json::Value result_data);
	void resultGetAchieveList(Json::Value result_data);
	
	void resultGetShopList(Json::Value result_data);
	
	void resultGetNoticeList(Json::Value result_data);
	
	void resultGetCharacterInfo(Json::Value result_data);
	vector<DownloadFile> character_download_list;
	int character_download_version;
	
	void resultGetMonsterList(Json::Value result_data);
	vector<DownloadFile> monster_download_list;
	int monster_download_version;
	
	void resultGetUserData(Json::Value result_data);
	
	void resultGetAchieveHistory(Json::Value result_data);
	void resultGetCardHistory(Json::Value result_data);
	void resultGetPuzzleHistory(Json::Value result_data);
	void resultGetPieceHistory(Json::Value result_data);
	void resultGetCharacterHistory(Json::Value result_data);
	void resultGetTodayMission(Json::Value result_data);
	
	void resultGetUserProperties(Json::Value result_data);
	
	Json::Value friendList;
	
	void resultLoadedCardData(Json::Value result_data);
	vector<DownloadFile> card_download_list;
	vector<CopyFile> card_reduction_list;
	
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
