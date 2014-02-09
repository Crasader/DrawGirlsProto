//
//  EventListDown.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 2. 9..
//
//

#include "EventListDown.h"
#include "PuzzleCache.h"
#include "LoadingTipScene.h"
#include "CommonButton.h"
#include "DataStorageHub.h"
#include "LogData.h"

#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"

void EventListDown::resultGetEventList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		if(result_data["version"] > NSDS_GI(kSDS_GI_eventListVersion_i))
		{
			Json::Value event_list = result_data["list"];
			
			NSDS_SI(kSDS_GI_eventListCount_i, event_list.size(), false);
			
			int event_cnt = event_list.size();
			for(int i=0;i<event_cnt;i++)
			{
				int event_number = event_list[i]["order"].asInt();
				
				NSDS_SI(kSDS_GI_eventList_int1_no_i, i+1, event_number, false);
				NSDS_SS(kSDS_GI_eventList_int1_title_s, i+1, event_list[i]["title"].asString().c_str(), false);
				NSDS_SS(event_number, kSDS_PZ_title_s, event_list[i]["title"].asString().c_str(), false);
				NSDS_SI(event_number, kSDS_PZ_startStage_i, event_list[i]["startStage"].asInt(), false);
				NSDS_SI(event_number, kSDS_PZ_stageCount_i, event_list[i]["stageCount"].asInt(), false);
				NSDS_SI(event_number, kSDS_PZ_point_i, event_list[i]["point"].asInt(), false);
				NSDS_SI(event_number, kSDS_PZ_ticket_i, event_list[i]["ticket"].asInt(), false);
				
				Json::Value thumbnail = event_list[i]["thumbnail"];
				if(NSDS_GS(kSDS_GI_eventList_int1_thumbnail_s, i+1) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("eventList%d_thumbnail.png", event_number)->getCString();
					t_df.key = CCSTR_CWF("eventList%d_thumbnail", event_number)->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				mySDS->fFlush(event_number, kSDS_PZ_base);
			}
			mySDS->fFlush(kSDS_GI_characterCount_i);
			if(df_list.size() > 0)
			{
				download_version = result_data["version"].asInt();
				state_ment->setString("이벤트 이미지를 다운로드 합니다.");
				state_ment->setPosition(ccp(240,80));
				ing_download_cnt = 1;
				ing_download_per = 0;
				download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f", (100.f*ing_download_cnt)/df_list.size())->getCString(), "etc_font.fnt");
				download_state->setPosition(ccp(240,50));
				addChild(download_state, kELD_Z_content);
				is_downloading = true;
				startDownload();
			}
			else
			{
				state_ment->setString("이벤트 정보 확인 완료.");
				NSDS_SI(kSDS_GI_eventListVersion_i, result_data["version"].asInt(), false);
				mySDS->fFlush(kSDS_GI_characterCount_i);
				(target_success->*delegate_success)();
				removeFromParent();
			}
		}
		else
		{
			state_ment->setString("이벤트 정보 확인 완료.");
			(target_success->*delegate_success)();
			removeFromParent();
		}
	}
	else
	{
		state_ment->setString("이벤트 정보를 받아오는데 실패하였습니다.");
		
		CommonButton* replay_menu = CommonButton::create("재시도", 12, CCSizeMake(80,45), CommonButtonYellow, -201);
		replay_menu->setPosition(ccp(300,100));
		replay_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kELD_MT_receive);
									 menuAction(t_node);
								 });
		
		addChild(replay_menu, kELD_Z_content, kELD_MT_receive);
	}
}

void EventListDown::menuAction(CCObject *sender)
{
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kELD_MT_receive)
	{
		removeChildByTag(kELD_MT_receive);
		state_ment->setString("다시 이벤트 정보를 확인하는ing...");
		startGetEventList();
	}
	else if(tag == kELD_MT_redown)
	{
		removeChildByTag(kELD_MT_redown);
		state_ment->setString("이벤트 이미지를 다운로드 합니다.");
		ing_download_per = 0;
		download_state->setString(CCSTR_CWF("%.0f", (100.f*ing_download_cnt)/df_list.size())->getCString());
		is_downloading = true;
		startDownload();
	}
}

void EventListDown::successAction()
{
	unschedule(schedule_selector(EventListDown::downloadingAction));
	
	if(ing_download_cnt < df_list.size())
	{
		SDS_SS(kSDF_gameInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,	int(df_list.size()))->getCString());
		startDownload();
	}
	else if(ing_download_cnt == df_list.size())
	{
		NSDS_SI(kSDS_GI_eventListVersion_i, download_version);
		download_state->setString(CCSTR_CWF("%.0f", (100.f*ing_download_cnt)/df_list.size())->getCString());
		state_ment->setString("이벤트 이미지 다운로드 완료.");
		(target_success->*delegate_success)();
		removeFromParent();
	}
}

void EventListDown::failAction()
{
	unschedule(schedule_selector(EventListDown::downloadingAction));
	state_ment->setString("이벤트 이미지 다운로드에 실패하였습니다.");
	is_downloading = false;
	
	CommonButton* replay_menu = CommonButton::create("재시도", 12, CCSizeMake(80,45), CommonButtonYellow, -201);
	replay_menu->setPosition(ccp(300,100));
	replay_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kELD_MT_redown);
								 menuAction(t_node);
							 });
	
	addChild(replay_menu, kELD_Z_content, kELD_MT_redown);
}

void EventListDown::downloadingAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	
	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;
	
	ing_download_per = t_per;
	
	download_state->setString(CCSTR_CWF("%.0f", (100.f*ing_download_cnt)/df_list.size())->getCString());
}

void EventListDown::startDownload()
{
	if(ing_download_cnt <= df_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size,
													  df_list[ing_download_cnt-1].filename, this, callfunc_selector(EventListDown::successAction),
													  this, callfunc_selector(EventListDown::failAction));
		
		schedule(schedule_selector(EventListDown::downloadingAction));
	}
}

EventListDown* EventListDown::create( CCObject* t_success, SEL_CallFunc d_success)
{
	EventListDown* t_sid = new EventListDown();
	t_sid->myInit(t_success, d_success);
	t_sid->autorelease();
	return t_sid;
}

void EventListDown::myInit( CCObject* t_success, SEL_CallFunc d_success)
{
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	setTouchEnabled(true);
	
	target_success = t_success;
	delegate_success = d_success;
	
	state_ment = CCLabelTTF::create("이벤트 정보를 확인하는ing...", mySGD->getFont().c_str(), 20);
	state_ment->setAnchorPoint(ccp(0.5,0.5));
	state_ment->setPosition(ccp(240,160));
	state_ment->setHorizontalAlignment(kCCTextAlignmentCenter);
	state_ment->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	addChild(state_ment, kELD_Z_content);
	
	is_downloading = false;
	
	startGetEventList();
}

void EventListDown::changeTipImage()
{
	tip_img->removeFromParent();
	
	tip_img = LoadingTipScene::getLoadingTipImage();
	tip_img->setPosition(ccp(240,160));
	addChild(tip_img, kELD_Z_back);
	
	CCDelayTime* t_delay = CCDelayTime::create(7);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(EventListDown::changeTipImage));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	
	tip_img->runAction(t_seq);
}

void EventListDown::startGetEventList()
{
	myLog->addLog(kLog_getPuzzleInfo_i, -1, -1);
	
	Json::Value eventlist_param;
	eventlist_param["version"] = NSDS_GI(kSDS_GI_eventListVersion_i);
	hspConnector::get()->command("geteventpuzzlelist", eventlist_param, json_selector(this, EventListDown::resultGetEventList));
}
