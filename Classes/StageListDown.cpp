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
		if(SDS_GI(kSDF_puzzleInfo, puzzle_number, "version") < result_data["version"].asInt())
		{
			state_ment->setString("퍼즐 정보를 받아오는ing...");
			
			SDS_SS(kSDF_puzzleInfo, puzzle_number, "title", result_data["title"].asString());
			SDS_SI(kSDF_puzzleInfo, puzzle_number, "ticket", result_data["ticket"].asInt());
			SDS_SI(kSDF_puzzleInfo, puzzle_number, "point", result_data["point"].asInt());
			
			if(SDS_GS(kSDF_puzzleInfo, puzzle_number, "center") != result_data["center"]["image"].asString())		addDownlist("center", result_data);
			if(SDS_GS(kSDF_puzzleInfo, puzzle_number, "bottom") != result_data["bottom"]["image"].asString())		addDownlist("bottom", result_data);
			if(SDS_GS(kSDF_puzzleInfo, puzzle_number, "top") != result_data["top"]["image"].asString())				addDownlist("top", result_data);
			if(SDS_GS(kSDF_puzzleInfo, puzzle_number, "left") != result_data["left"]["image"].asString())			addDownlist("left", result_data);
			if(SDS_GS(kSDF_puzzleInfo, puzzle_number, "right") != result_data["right"]["image"].asString())			addDownlist("right", result_data);
			
			SDS_SI(kSDF_puzzleInfo, puzzle_number, "start_stage_number", result_data["startStage"].asInt());
			
			Json::Value stage_list = result_data["list"];
			int loop_cnt = stage_list.size();
			SDS_SI(kSDF_puzzleInfo, puzzle_number, "stage_count", loop_cnt);
			for(int i=0;i<loop_cnt;i++)
			{
				int stage_number = stage_list[i]["no"].asInt();
				
				SDS_SI(kSDF_puzzleInfo, puzzle_number, CCSTR_CWF("stage%d_level", stage_number)->getCString(), stage_list[i]["level"].asInt());
				SDS_SS(kSDF_puzzleInfo, puzzle_number, CCSTR_CWF("stage%d_pieceType", stage_number)->getCString(), stage_list[i]["pieceType"].asString());
				SDS_SD(kSDF_puzzleInfo, puzzle_number, CCSTR_CWF("stage%d_x", stage_number)->getCString(), stage_list[i]["x"].asDouble());
				SDS_SD(kSDF_puzzleInfo, puzzle_number, CCSTR_CWF("stage%d_y", stage_number)->getCString(), stage_list[i]["y"].asDouble());
				
				if(SDS_GS(kSDF_puzzleInfo, puzzle_number, CCSTR_CWF("stage%d_piece", stage_number)->getCString()) != stage_list[i]["piece"]["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = stage_list[i]["piece"]["size"].asInt();
					t_df.img = stage_list[i]["piece"]["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("puzzle%d_stage%d_piece.png", puzzle_number, stage_number)->getCString();
					t_df.key = CCSTR_CWF("stage%d_piece", stage_number)->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				if(SDS_GS(kSDF_puzzleInfo, puzzle_number, CCSTR_CWF("stage%d_thumbnail", stage_number)->getCString()) != stage_list[i]["thumbnail"]["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = stage_list[i]["thumbnail"]["size"].asInt();
					t_df.img = stage_list[i]["thumbnail"]["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("puzzle%d_stage%d_thumbnail.png", puzzle_number, stage_number)->getCString();
					t_df.key = CCSTR_CWF("stage%d_thumbnail", stage_number)->getCString();
					df_list.push_back(t_df);
					// ================================
				}
			}
			
			Json::Value event_list = result_data["eventList"];
			int el_length = event_list.size();
			SDS_SI(kSDF_gameInfo, "event_count", el_length);
			for(int i=0;i<el_length;i++)
			{
				int event_code = event_list[i]["no"].asInt();
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("event%d_code", i)->getCString(), event_code);
				Json::Value thumbnail = event_list[i]["thumbnail"];
				if(SDS_GS(kSDF_gameInfo, CCSTR_CWF("event%d_thumbnail_image", i)->getCString()) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("event%d_thumbnail.png", i)->getCString();
					t_df.key = CCSTR_CWF("event%d_thumbnail_image", i)->getCString();
					ef_list.push_back(t_df);
					// ================================
				}
			}
			
			if(df_list.size() + ef_list.size() > 0) // need download
			{
				download_version = result_data["version"].asInt();
				state_ment->setString("퍼즐 이미지를 다운로드 합니다.");
				ing_download_cnt = 1;
				ing_download_per = 0;
				download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()+ef_list.size()))->getCString(), "etc_font.fnt");
				download_state->setPosition(ccp(240,130));
				addChild(download_state, kSLD_Z_content);
				is_downloading = true;
				startDownload();
			}
			else
			{
				SDS_SI(kSDF_puzzleInfo, puzzle_number, "version", result_data["version"].asInt());
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
		download_state->setString(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()+ef_list.size()))->getCString());
		is_downloading = true;
		startDownload();
	}
}

void StageListDown::successAction()
{
	if(ing_download_cnt <= df_list.size())
		SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
	else
		SDS_SS(kSDF_gameInfo, ef_list[ing_download_cnt-df_list.size()-1].key, ef_list[ing_download_cnt-df_list.size()-1].img);

	if(ing_download_cnt >= df_list.size()+ef_list.size())
	{
		SDS_SI(kSDF_puzzleInfo, puzzle_number, "version", download_version);
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()+ef_list.size()))->getCString());
		state_ment->setString("퍼즐 이미지 다운로드 완료.");
		(target_success->*delegate_success)();
		removeFromParent();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()+ef_list.size()))->getCString());
		startDownload();
	}
}

void StageListDown::failAction()
{
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
	
	download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()+ef_list.size()))->getCString());
}

void StageListDown::startDownload()
{
	if(ing_download_cnt <= df_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(StageListDown::successAction), this, callfunc_selector(StageListDown::failAction));
	}
	else
	{
		CCLog("%d : %s", ing_download_cnt, ef_list[ing_download_cnt-df_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(ef_list[ing_download_cnt-df_list.size()-1].img, ef_list[ing_download_cnt-df_list.size()-1].size,
													  ef_list[ing_download_cnt-df_list.size()-1].filename, this, callfunc_selector(StageListDown::successAction), this, callfunc_selector(StageListDown::failAction));
	}
	
	schedule(schedule_selector(StageListDown::downloadingAction));
}
