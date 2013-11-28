//
//  TitleScene.h
//  DGproto
//
//  Created by LitQoo on 13. 10. 22..
//
//

#ifndef __DGproto__TitleScene__
#define __DGproto__TitleScene__

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

enum TitleMenuTag
{
	kTitle_MT_replay = 1,
	kTitle_MT_nick,
	kTitle_MT_puzzleBase = 10000
};

class TitleScene : public cocos2d::CCLayer, public CCEditBoxDelegate//, public CCTextFieldDelegate
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(TitleScene);
	
private:
	
	CCLabelTTF* state_label;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int puzzlelist_download_version;
	int eventstagelist_download_version;
	
	bool successed_get_puzzle_list;
	
	vector<int> card_data_load_list;
	vector<DownloadFile> df_list;
	vector<CopyFile> cf_list;
	vector<DownloadFile> ef_list;
	
	CCObject* save_target;
	SEL_CallFunc save_delegate;
	
	CCEditBox* input_text;
//	CCTextFieldTTF* input_text;
    bool was_open_text;
	
	void endCloseTextInput();
	
	CCSprite* nick_back;
	
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
	
//	virtual bool onTextFieldInsertText(CCTextFieldTTF* sender, const char* text, int nLen);
//	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
//	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
	
//	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//	virtual void registerWithTouchDispatcher();
	
	void startGetPuzzleList();
	
	void resultLogin(Json::Value result_data);
	
	void startGetUserData();
	
	void resultGetUserData(Json::Value result_data);
	
	void resultSaveUserData(Json::Value result_data);
	
	void startGetCardsInfo();
	
	void resultLoadedCardData(Json::Value result_data);
	
	void successAction2();
	
	void failAction2();
	
	void startDownloadCardImage();
	
	void downloadingAction2();
	
	void startDownload2();
	
	void startSaveUserData();
	 
	void changeScene();

	void resultGetPuzzleList(Json::Value result_data);
	void startGetCommonSetting();
	void resultGetCommonSetting(Json::Value result_data);
	
	void startDownloadGameInfo();
	
	bool is_menu_enable;
	
	void menuAction(CCObject* sender);
	
	
	void successAction();
	
	void failAction();
	
	void downloadingAction();
	
	void startDownload();
	
	void endingAction();
};

#endif /* defined(__DGproto__TitleScene__) */
