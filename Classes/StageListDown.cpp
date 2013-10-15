//
//  StageListDown.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 10..
//
//

#include "StageListDown.h"

void StageListDown::resultGetStageList(Json::Value result_data)
{
	CCLog("result data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["state"].asString() == "ok")
	{
		if(SDS_GI(kSDF_gameInfo, "version") < result_data["version"].asInt())
		{
			state_ment->setString("스테이지 목록을 받아오는ing...");
			
			int loop_cnt = result_data["count"].asInt();
			SDS_SI(kSDF_gameInfo, "stage_count", loop_cnt);
			
			int top_y, bottom_y, left_x, right_x;
			
			Json::Value stage_list = result_data["list"];
			for(int i=0;i<loop_cnt;i++)
			{
				int stage_number = stage_list[i]["no"].asInt();
				Json::Value thumbnail = stage_list[i]["thumbnail"];
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("stage%d_thumbnail_size", stage_number)->getCString(), thumbnail["size"].asInt());
				if(SDS_GS(kSDF_gameInfo, CCSTR_CWF("stage%d_thumbnail_image", stage_number)->getCString()) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asCString();
					t_df.filename = CCSTR_CWF("stage%d_thumbnail.png", stage_number)->getCString();
					t_df.key = CCSTR_CWF("stage%d_thumbnail_image", stage_number)->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				Json::Value path = thumbnail["path"];
				int path_length = path.size();
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("stage%d_path_length", stage_number)->getCString(), path_length);
				for(int j=0;j<path_length;j++)
				{
					SDS_SS(kSDF_gameInfo, CCSTR_CWF("stage%d_path_direction%d", stage_number, j)->getCString(), path[j]["d"].asCString());
					SDS_SI(kSDF_gameInfo, CCSTR_CWF("stage%d_path_count%d", stage_number, j)->getCString(), path[j]["c"].asInt());
				}
				
				if(i == 0)
				{
					top_y = thumbnail["y"].asInt();
					bottom_y = thumbnail["y"].asInt();
					left_x = thumbnail["x"].asInt();
					right_x = thumbnail["x"].asInt();
				}
				else
				{
					if(top_y < thumbnail["y"].asInt())
						top_y = thumbnail["y"].asInt();
					if(bottom_y > thumbnail["y"].asInt())
						bottom_y = thumbnail["y"].asInt();
					if(left_x > thumbnail["x"].asInt())
						left_x = thumbnail["x"].asInt();
					if(right_x < thumbnail["x"].asInt())
						right_x = thumbnail["x"].asInt();
				}
				
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("stage%d_x", stage_number)->getCString(), thumbnail["x"].asInt());
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("stage%d_y", stage_number)->getCString(), thumbnail["y"].asInt());
			}
			
			Json::Value coupon_list = result_data["cpList"];
			int cl_length = coupon_list.size();
			int non_ticket = 1;
			for(int i=0;i<cl_length;i++)
			{
				int stage_number = coupon_list[i]["stage"].asInt();
				for(;non_ticket<stage_number;non_ticket++)
					SDS_SB(kSDF_gameInfo, CCSTR_CWF("is_coupon%d", non_ticket)->getCString(), false);
				non_ticket++;
				
				SDS_SB(kSDF_gameInfo, CCSTR_CWF("is_coupon%d", stage_number)->getCString(), true);
				
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("coupon%d_ruby", stage_number)->getCString(), coupon_list[i]["ruby"].asInt());
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("coupon%d_ticket", stage_number)->getCString(), coupon_list[i]["ticket"].asInt());
				Json::Value thumbnail = coupon_list[i]["thumbnail"];
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("coupon%d_thumbnail_size", stage_number)->getCString(), thumbnail["size"].asInt());
				if(SDS_GS(kSDF_gameInfo, CCSTR_CWF("coupon%d_thumbnail_image", stage_number)->getCString()) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asCString();
					t_df.filename = CCSTR_CWF("coupon%d_thumbnail.png", stage_number)->getCString();
					t_df.key = CCSTR_CWF("coupon%d_thumbnail_image", stage_number)->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				Json::Value path = thumbnail["path"];
				int path_length = path.size();
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("coupon%d_path_length", stage_number)->getCString(), path_length);
				for(int j=0;j<path_length;j++)
				{
					SDS_SS(kSDF_gameInfo, CCSTR_CWF("coupon%d_path_direction%d", stage_number, j)->getCString(), path[j]["d"].asCString());
					SDS_SI(kSDF_gameInfo, CCSTR_CWF("coupon%d_path_count%d", stage_number, j)->getCString(), path[j]["c"].asInt());
				}
				
				if(top_y < thumbnail["y"].asInt())
					top_y = thumbnail["y"].asInt();
				if(bottom_y > thumbnail["y"].asInt())
					bottom_y = thumbnail["y"].asInt();
				if(left_x > thumbnail["x"].asInt())
					left_x = thumbnail["x"].asInt();
				if(right_x < thumbnail["x"].asInt())
					right_x = thumbnail["x"].asInt();
				
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("coupon%d_x", stage_number)->getCString(), thumbnail["x"].asInt());
				SDS_SI(kSDF_gameInfo, CCSTR_CWF("coupon%d_y", stage_number)->getCString(), thumbnail["y"].asInt());
			}
			
			SDS_SI(kSDF_gameInfo, "left_x", left_x);
			SDS_SI(kSDF_gameInfo, "right_x", right_x);
			SDS_SI(kSDF_gameInfo, "top_y", top_y);
			SDS_SI(kSDF_gameInfo, "bottom_y", bottom_y);
			
			if(df_list.size() > 0) // need download
			{
				download_version = result_data["version"].asInt();
				state_ment->setString("스테이지 썸네일을 다운로드 합니다.");
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
				SDS_SI(kSDF_gameInfo, "version", result_data["version"].asInt());
				state_ment->setString("스테이지 목록 확인 완료.");
				(target_success->*delegate_success)();
				removeFromParent();
			}
		}
		else
		{
			state_ment->setString("스테이지 목록 확인 완료.");
			(target_success->*delegate_success)();
			removeFromParent();
		}
	}
	else
	{
		state_ment->setString("스테이지 목록을 받아오는데 실패하였습니다.");
		
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
		state_ment->setString("다시 스테이지 목록을 확인하는ing...");
		startGetStageList();
	}
	else if(tag == kSLD_MT_redown)
	{
		removeChildByTag(kSLD_MT_redown);
		state_ment->setString("스테이지 썸네일을 다운로드 합니다.");
		ing_download_per = 0;
		download_state->setString(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		is_downloading = true;
		startDownload();
	}
}

void StageListDown::successAction()
{
	SDS_SS(kSDF_gameInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);

	if(ing_download_cnt >= df_list.size())
	{
		SDS_SI(kSDF_gameInfo, "version", download_version);
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		state_ment->setString("스테이지 썸네일 다운로드 완료.");
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
	state_ment->setString("이미지 정보 다운로드에 실패하였습니다.");
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
