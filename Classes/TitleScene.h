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

USING_NS_CC;
using namespace std;

enum TitleMenuTag
{
	kTitle_MT_replay = 1,
	kTitle_MT_puzzleBase = 10000
};

class TitleScene : public cocos2d::CCLayer
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
	
	void startGetPuzzleList()
	{
		Json::Value param;
		param["version"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
		hspConnector::get()->command("getpuzzlelist", param, json_selector(this, TitleScene::resultGetPuzzleList));
	}
	
	void resultLogin(Json::Value result_data)
	{
		CCLog("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		
		state_label->setString("퍼즐 목록을 받아오는 ing...");
		
		startGetPuzzleList();
	}
	
	void changeScene()
	{
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
	
	void resultGetPuzzleList(Json::Value result_data)
	{
		CCLog("resultGetPuzzleList data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		
		if(result_data["state"].asString() == "ok")
		{
			if(result_data["version"] > NSDS_GI(kSDS_GI_puzzleListVersion_i))
			{
				Json::Value puzzle_list = result_data["list"];
				
				NSDS_SI(kSDS_GI_puzzleListCount_i, puzzle_list.size());
				
				int puzzle_cnt = puzzle_list.size();
				for(int i=0;i<puzzle_cnt;i++)
				{
					NSDS_SI(kSDS_GI_puzzleList_int1_no_i, i+1, puzzle_list[i]["no"].asInt());
					NSDS_SS(kSDS_GI_puzzleList_int1_title_s, i+1, puzzle_list[i]["title"].asString().c_str());
					NSDS_SI(kSDS_GI_puzzleList_int1_version_i, i+1, puzzle_list[i]["version"].asInt());
				}
				NSDS_SI(kSDS_GI_puzzleListVersion_i, result_data["version"].asInt());
			}
			
			CCDelayTime* t_delay = CCDelayTime::create(0.2f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleScene::changeScene));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			runAction(t_seq);
		}
		else
		{
			state_label->setString("퍼즐 목록을 받아오는데 실패하였습니다. 재시도 해주세요.");
			
			CCSprite* n_replay = CCSprite::create("whitePaper.png", CCRectMake(0,0,120,70));
			CCLabelTTF* n_label = CCLabelTTF::create("재시도", mySGD->getFont().c_str(), 20);
			n_label->setColor(ccBLACK);
			n_label->setPosition(ccp(60,35));
			n_replay->addChild(n_label);
			
			CCSprite* s_replay = CCSprite::create("whitePaper.png", CCRectMake(0, 0,120,70));
			s_replay->setColor(ccGRAY);
			CCLabelTTF* s_label = CCLabelTTF::create("재시도", mySGD->getFont().c_str(), 20);
			s_label->setColor(ccBLACK);
			s_label->setPosition(ccp(60,35));
			s_replay->addChild(s_label);
			
			CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
			replay_item->setTag(kTitle_MT_replay);
			
			CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
			replay_menu->setPosition(ccp(240,160));
			addChild(replay_menu, 0, kTitle_MT_replay);
			
			is_menu_enable = true;
		}
	}
	
	bool is_menu_enable;
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)		return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kTitle_MT_replay)
		{
			removeChildByTag(kTitle_MT_replay);
			startGetPuzzleList();
		}
		else if(tag >= kTitle_MT_puzzleBase)
		{
			tag -= kTitle_MT_puzzleBase;
			
			myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, tag);
			CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
		}
	}
};

#endif /* defined(__DGproto__TitleScene__) */
