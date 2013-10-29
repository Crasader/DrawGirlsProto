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
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int puzzlelist_download_version;
	int eventstagelist_download_version;
	
	bool successed_get_puzzle_list;
	
	void startGetPuzzleList()
	{
		Json::Value param;
		param["puzzlelistversion"] = NSDS_GI(kSDS_GI_puzzleList_int1_version_i);
		param["eventstagelistversion"] = NSDS_GI(kSDS_GI_eventListVersion_i);
		hspConnector::get()->command("getpuzzlelist", param, json_selector(this, TitleScene::resultGetPuzzleList));
		
//		vector<CommandParam> cmds;
//		Json::Value param1;
//		param1["version"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
//		CommandParam cmd1 = CommandParam("getpuzzlelist", param1, json_selector(this, TitleScene::resultGetPuzzleList));
//		cmds.push_back(cmd1);
//		
//		Json::Value param2;
//		param2["version"] = NSDS_GI(kSDS_GI_eventListVersion_i);
//		CommandParam cmd2 = CommandParam("geteventstagelist", param2, json_selector(this, TitleScene::resultGetEventStageList));
//		cmds.push_back(cmd2);
//		hspConnector::get()->command(cmds);
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
	
	vector<DownloadFile> df_list;
	vector<DownloadFile> ef_list;
	
//	void resultGetEventStageList(Json::Value result_data)
//	{
//		CCLog("resultGetEventStageList data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
//		
//		if(result_data["state"].asString() == "ok")
//		{
//			if(result_data["version"] > NSDS_GI(kSDS_GI_eventListVersion_i))
//			{
//				Json::Value event_list = result_data["list"];
//				int el_length = event_list.size();
//				NSDS_SI(kSDS_GI_eventCount_i, el_length);
//				for(int i=0;i<el_length;i++)
//				{
//					int event_code = event_list[i]["no"].asInt();
//					NSDS_SI(kSDS_GI_event_int1_code_i, i, event_code);
//					Json::Value thumbnail = event_list[i]["thumbnail"];
//					if(NSDS_GS(kSDS_GI_event_int1_thumbnail_s, i) != thumbnail["image"].asString())
//					{
//						// check, after download ----------
//						DownloadFile t_df;
//						t_df.size = thumbnail["size"].asInt();
//						t_df.img = thumbnail["image"].asString().c_str();
//						t_df.filename = CCSTR_CWF("event%d_thumbnail.png", i)->getCString();
//						t_df.key = CCSTR_CWF("event%d_thumbnail", i)->getCString();
//						ef_list.push_back(t_df);
//						// ================================
//					}
//				}
//				
//				if(ef_list.size() > 0) // need download
//				{
//					download_version = result_data["version"].asInt();
//					state_label->setString("이벤트 정보를 받아옵니다.");
//					ing_download_cnt = 1;
//					ing_download_per = 0;
//					download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(ef_list.size()))->getCString(), "etc_font.fnt");
//					download_state->setPosition(ccp(240,130));
//					addChild(download_state);
//					is_downloading = true;
//					startDownload();
//				}
//				else
//					endingAction();
//			}
//			else
//				endingAction();
//		}
//		else
//		{
//			if(successed_get_puzzle_list)
//				endingAction();
//			else
//			{
//				state_label->setString("퍼즐 목록을 받아오는데 실패하였습니다. 재시도 해주세요.");
//				
//				CCSprite* n_replay = CCSprite::create("whitePaper.png", CCRectMake(0,0,120,70));
//				CCLabelTTF* n_label = CCLabelTTF::create("재시도", mySGD->getFont().c_str(), 20);
//				n_label->setColor(ccBLACK);
//				n_label->setPosition(ccp(60,35));
//				n_replay->addChild(n_label);
//				
//				CCSprite* s_replay = CCSprite::create("whitePaper.png", CCRectMake(0, 0,120,70));
//				s_replay->setColor(ccGRAY);
//				CCLabelTTF* s_label = CCLabelTTF::create("재시도", mySGD->getFont().c_str(), 20);
//				s_label->setColor(ccBLACK);
//				s_label->setPosition(ccp(60,35));
//				s_replay->addChild(s_label);
//				
//				CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
//				replay_item->setTag(kTitle_MT_replay);
//				
//				CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
//				replay_menu->setPosition(ccp(240,160));
//				addChild(replay_menu, 0, kTitle_MT_replay);
//				
//				is_menu_enable = true;
//			}
//		}
//	}

	void resultGetPuzzleList(Json::Value result_data)
	{
		CCLog("resultGetPuzzleList data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		
		if(result_data["state"].asString() == "ok")
		{
			successed_get_puzzle_list = true;
			if(result_data["puzzlelistversion"] > NSDS_GI(kSDS_GI_puzzleListVersion_i))
			{
				Json::Value puzzle_list = result_data["puzzlelist"];
				
				NSDS_SI(kSDS_GI_puzzleListCount_i, puzzle_list.size());
				
				int puzzle_cnt = puzzle_list.size();
				for(int i=0;i<puzzle_cnt;i++)
				{
					NSDS_SI(kSDS_GI_puzzleList_int1_no_i, i+1, puzzle_list[i]["no"].asInt());
					NSDS_SS(kSDS_GI_puzzleList_int1_title_s, i+1, puzzle_list[i]["title"].asString().c_str());
					NSDS_SI(kSDS_GI_puzzleList_int1_version_i, i+1, puzzle_list[i]["version"].asInt());
					
					Json::Value thumbnail = puzzle_list[i]["thumbnail"];
					if(NSDS_GS(kSDS_GI_puzzleList_int1_thumbnail_s, i+1) != thumbnail["image"].asString())
					{
						// check, after download ----------
						DownloadFile t_df;
						t_df.size = thumbnail["size"].asInt();
						t_df.img = thumbnail["image"].asString().c_str();
						t_df.filename = CCSTR_CWF("puzzleList%d_thumbnail.png", i+1)->getCString();
						t_df.key = CCSTR_CWF("puzzleList%d_thumbnail", i+1)->getCString();
						df_list.push_back(t_df);
						// ================================
					}
				}
				
				if(df_list.size() > 0)
					puzzlelist_download_version = result_data["puzzlelistversion"].asInt();
				else
					NSDS_SI(kSDS_GI_puzzleListVersion_i, result_data["puzzlelistversion"].asInt());
			}
			
			if(result_data["eventstagelistversion"] > NSDS_GI(kSDS_GI_eventListVersion_i))
			{
				Json::Value event_list = result_data["eventstagelist"];
				int el_length = event_list.size();
				NSDS_SI(kSDS_GI_eventCount_i, el_length);
				for(int i=0;i<el_length;i++)
				{
					int event_code = event_list[i]["no"].asInt();
					NSDS_SI(kSDS_GI_event_int1_code_i, i, event_code);
					Json::Value thumbnail = event_list[i]["thumbnail"];
					if(NSDS_GS(kSDS_GI_event_int1_thumbnail_s, i) != thumbnail["image"].asString())
					{
						// check, after download ----------
						DownloadFile t_df;
						t_df.size = thumbnail["size"].asInt();
						t_df.img = thumbnail["image"].asString().c_str();
						t_df.filename = CCSTR_CWF("event%d_thumbnail.png", i)->getCString();
						t_df.key = CCSTR_CWF("event%d_thumbnail", i)->getCString();
						ef_list.push_back(t_df);
						// ================================
					}
				}
				
				if(ef_list.size() > 0) // need download
					eventstagelist_download_version = result_data["eventstagelistversion"].asInt();
				else
					NSDS_SI(kSDS_GI_eventListVersion_i, result_data["eventstagelistversion"].asInt());
			}
			
			
			if(df_list.size() + ef_list.size() > 0)
			{
				state_label->setString("게임 정보를 받아옵니다.");
				ing_download_cnt = 1;
				ing_download_per = 0;
				download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString(), "etc_font.fnt");
				download_state->setPosition(ccp(240,130));
				addChild(download_state);
				is_downloading = true;
				startDownload();
			}
			else
				endingAction();
			
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
	
	
	void successAction()
	{
		if(ing_download_cnt <= df_list.size())
			SDS_SS(kSDF_gameInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
		else
			SDS_SS(kSDF_gameInfo, ef_list[ing_download_cnt-df_list.size()-1].key, ef_list[ing_download_cnt-df_list.size()-1].img);
		
		if(ing_download_cnt >= df_list.size() + ef_list.size())
		{
			if(df_list.size() > 0)
				NSDS_SI(kSDS_GI_puzzleListVersion_i, puzzlelist_download_version);
			if(ef_list.size() > 0)
				NSDS_SI(kSDS_GI_eventListVersion_i, eventstagelist_download_version);
			download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString());
			state_label->setString("이벤트 정보 다운로드 완료.");
			
			endingAction();
		}
		else
		{
			ing_download_cnt++;
			ing_download_per = 0.f;
			download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString());
			startDownload();
		}
	}
	
	void failAction()
	{
		state_label->setString("게임 정보를 받아오는데 실패하였습니다. 재시도 해주세요.");
		
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
	
	void downloadingAction()
	{
		float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
		
		if(t_per < 0.f)			t_per = 0.f;
		else if(t_per > 1.f)	t_per = 1.f;
		
		ing_download_per = t_per;
		
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString());
	}
	
	void startDownload()
	{
		if(ing_download_cnt <= df_list.size())
		{
			CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
			StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size,
														  df_list[ing_download_cnt-1].filename, this, callfunc_selector(TitleScene::successAction), this, callfunc_selector(TitleScene::failAction));
		}
		else
		{
			CCLog("%d : %s", ing_download_cnt, ef_list[ing_download_cnt-df_list.size()-1].filename.c_str());
			StageImgLoader::sharedInstance()->downloadImg(ef_list[ing_download_cnt-df_list.size()-1].img, ef_list[ing_download_cnt-df_list.size()-1].size,
														  ef_list[ing_download_cnt-df_list.size()-1].filename, this, callfunc_selector(TitleScene::successAction), this, callfunc_selector(TitleScene::failAction));
		}
		
		schedule(schedule_selector(TitleScene::downloadingAction));
	}
	
	void endingAction()
	{
		CCDelayTime* t_delay = CCDelayTime::create(0.2f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleScene::changeScene));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
};

#endif /* defined(__DGproto__TitleScene__) */
