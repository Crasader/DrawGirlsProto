//
//  StageListDown.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 10..
//
//

#include "StageListDown.h"

void StageListDown::addDownlist(string t_key, const Json::Value& result_data)
{
	// check, after download ----------
	DownloadFile t_df;
	t_df.size = result_data[t_key.c_str()]["size"].asInt();
	t_df.img = result_data[t_key.c_str()]["image"].asString().c_str();
	t_df.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, t_key.c_str())->getCString();
	t_df.key = t_key.c_str();
	df_list.push_back(t_df);
	// ================================
}

void StageListDown::resultGetStageList(Json::Value result_data)
{
	CCLog("result data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["state"].asString() == "ok")
	{
		if(NSDS_GI(puzzle_number, kSDS_PZ_version_i) < result_data["version"].asInt())
		{
			state_ment->setString("퍼즐 정보를 받아오는ing...");
			
			NSDS_SS(puzzle_number, kSDS_PZ_title_s, result_data["title"].asString());
			NSDS_SI(puzzle_number, kSDS_PZ_ticket_i, result_data["ticket"].asInt());
			NSDS_SI(puzzle_number, kSDS_PZ_point_i, result_data["point"].asInt());
			
			if(NSDS_GS(puzzle_number, kSDS_PZ_center_s) != result_data["center"]["image"].asString())		addDownlist("center", result_data);
			if(NSDS_GS(puzzle_number, kSDS_PZ_original_s) != result_data["original"]["image"].asString())	addDownlist("original", result_data);
			if(NSDS_GS(puzzle_number, kSDS_PZ_face_s) != result_data["face"]["image"].asString())			addDownlist("face", result_data);
			
			NSDS_SI(puzzle_number, kSDS_PZ_startStage_i, result_data["startStage"].asInt());
			
			Json::Value stage_list = result_data["list"];
			int loop_cnt = stage_list.size();
			NSDS_SI(puzzle_number, kSDS_PZ_stageCount_i, loop_cnt);
			for(int i=0;i<loop_cnt;i++)
			{
				int stage_number = stage_list[i]["no"].asInt();
				
				NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_level_i, stage_number, stage_list[i]["level"].asInt());
				NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number, stage_list[i]["pieceNo"].asInt());
				if(!stage_list[i]["condition"].isNull())
				{
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number, stage_list[i]["condition"]["gold"].asInt());
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number, stage_list[i]["condition"]["stageNo"].asInt());
				}
			}
			
			if(df_list.size() > 0) // need download
			{
				download_version = result_data["version"].asInt();
				state_ment->setString("퍼즐 이미지를 다운로드 합니다.");
				ing_download_cnt = 1;
				ing_download_per = 0;
				download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString(), "etc_font.fnt");
				download_state->setPosition(ccp(240,130));
				addChild(download_state, kSLD_Z_content);
				is_downloading = true;
				startDownload();
			}
			else
			{
				NSDS_SI(puzzle_number, kSDS_PZ_version_i, result_data["version"].asInt());
				state_ment->setString("퍼즐 정보 갱신 완료.");
				(target_success->*delegate_success)();
				removeFromParent();
			}
		}
		else
		{
			state_ment->setString("퍼즐 정보 확인 완료.");
			(target_success->*delegate_success)();
			removeFromParent();
		}
	}
	else
	{
		state_ment->setString("퍼즐 정보를 받아오는데 실패하였습니다.");
		
		CCSprite* n_button = CCSprite::create("cardsetting_zoom.png");
		CCSprite* s_button = CCSprite::create("cardsetting_zoom.png");
		s_button->setColor(ccGRAY);
		
		CCMenuItem* button_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(StageListDown::menuAction));
		button_item->setTag(kSLD_MT_receive);
		
		CCMenu* button_menu = CCMenu::createWithItem(button_item);
		button_menu->setPosition(ccp(300,100));
		addChild(button_menu, kSLD_Z_content, kSLD_MT_receive);
	}
}

void StageListDown::menuAction(CCObject *sender)
{
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kSLD_MT_receive)
	{
		removeChildByTag(kSLD_MT_receive);
		state_ment->setString("다시 퍼즐 정보를 확인하는ing...");
		startGetStageList();
	}
	else if(tag == kSLD_MT_redown)
	{
		removeChildByTag(kSLD_MT_redown);
		state_ment->setString("퍼즐 이미지를 다운로드 합니다.");
		ing_download_per = 0;
		download_state->setString(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		is_downloading = true;
		startDownload();
	}
}

void StageListDown::successAction()
{
	unschedule(schedule_selector(StageListDown::downloadingAction));
	SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);

	if(ing_download_cnt >= df_list.size())
	{
		NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version);
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		state_ment->setString("퍼즐 이미지 다운로드 완료.");
		(target_success->*delegate_success)();
		removeFromParent();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		startDownload();
	}
}

void StageListDown::failAction()
{
	unschedule(schedule_selector(StageListDown::downloadingAction));
	state_ment->setString("퍼즐 이미지 다운로드에 실패하였습니다.");
	is_downloading = false;
	CCSprite* n_button = CCSprite::create("cardsetting_zoom.png");
	CCSprite* s_button = CCSprite::create("cardsetting_zoom.png");
	s_button->setColor(ccGRAY);
	
	CCMenuItem* button_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(StageListDown::menuAction));
	button_item->setTag(kSLD_MT_redown);
	
	CCMenu* button_menu = CCMenu::createWithItem(button_item);
	button_menu->setPosition(ccp(300,100));
	addChild(button_menu, kSLD_Z_content, kSLD_MT_redown);
}

void StageListDown::downloadingAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	
	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;
	
	ing_download_per = t_per;
	
	download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
}

void StageListDown::startDownload()
{
	CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(StageListDown::successAction), this, callfunc_selector(StageListDown::failAction));
	
	schedule(schedule_selector(StageListDown::downloadingAction));
}

StageListDown* StageListDown::create( CCObject* t_success, SEL_CallFunc d_success, int t_puzzle )
{
	StageListDown* t_sid = new StageListDown();
	t_sid->myInit(t_success, d_success, t_puzzle);
	t_sid->autorelease();
	return t_sid;
}

void StageListDown::myInit( CCObject* t_success, SEL_CallFunc d_success, int t_puzzle )
{
	puzzle_number = t_puzzle;
	target_success = t_success;
	delegate_success = d_success;

	state_ment = CCLabelTTF::create("퍼즐 정보를 확인하는ing...", mySGD->getFont().c_str(), 20);
	state_ment->setAnchorPoint(ccp(0.5,0.5));
	state_ment->setPosition(ccp(240,160));
	state_ment->setHorizontalAlignment(kCCTextAlignmentCenter);
	state_ment->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	addChild(state_ment, kSLD_Z_content);

	is_downloading = false;

	startGetStageList();
}

void StageListDown::startGetStageList()
{
	myLog->addLog(kLog_getPuzzleInfo_i, -1, puzzle_number);

	Json::Value param;
	param["version"] = NSDS_GI(puzzle_number, kSDS_PZ_version_i);
	param["no"] = puzzle_number;
	hspConnector::get()->command("getpuzzleinfo", param, json_selector(this, StageListDown::resultGetStageList));
}
