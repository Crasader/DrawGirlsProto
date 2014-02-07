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

class TitleRenewalScene : public cocos2d::CCLayer, public CCEditBoxDelegate//, public CCTextFieldDelegate
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(TitleRenewalScene);
	
private:
	CCLabelTTF* state_label;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int puzzlelist_download_version;
	int eventstagelist_download_version;
	
	vector<int> card_data_load_list;
	
	CCEditBox* input_text;
	
	CCSprite* nick_back;
	
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
	
	
	void resultLogin(Json::Value result_data);
	
	vector<CommandParam> command_list;
	bool must_be_load_friends;
	int receive_cnt;
	
	bool is_receive_fail;
	
	void startCommand();
	void checkReceive();
	void startFileDownload();
	void downloadingFileAction();
	void successDownloadAction();
	void failDownloadAction();
	
	void resultGetCommonSetting(Json::Value result_data);
	
	void resultGetNoticeList(Json::Value result_data);
	
	void resultGetCharacterInfo(Json::Value result_data);
	vector<DownloadFile> character_download_list;
	int character_download_version;
	
	void resultGetMonsterList(Json::Value result_data);
	vector<DownloadFile> monster_download_list;
	int monster_download_version;
	
	void resultGetUserData(Json::Value result_data);
	Json::Value friendList;
	
	void resultLoadedCardData(Json::Value result_data);
	vector<DownloadFile> card_download_list;
	vector<CopyFile> card_reduction_list;
	
	void resultGetKnownFriendList(Json::Value result_data);
	void startGetKnownFriendUserData();
	void resultGetKnownFriendUserData(Json::Value result_data);
	
	void startGetUnknownFriendUserData();
	void resultGetUnknownFriendUserData(Json::Value result_data);
	
	void resultGetPuzzleList(Json::Value result_data);
	vector<DownloadFile> puzzle_download_list;
	vector<DownloadFile> event_download_list;
	
	void resultGetPathInfo(Json::Value result_data);
	
	
	void endingCheck();
	void endingAction();
	void changeScene();
	
	bool is_menu_enable;
	void menuAction(CCObject* sender);
};

#endif /* defined(__DGproto__TitleRenewal__) */
