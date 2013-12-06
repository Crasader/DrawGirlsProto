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
				
				
				CCLog("saved version : %d", NSDS_GI(stage_number, kSDS_SI_version_i));
				if(NSDS_GI(stage_number, kSDS_SI_version_i) < stage_list[i]["version"].asInt())
				{
					save_version_list.push_back(IntPoint(stage_number, stage_list[i]["version"].asInt()));
					
					NSDS_SI(stage_number, kSDS_SI_puzzle_i, stage_list[i]["puzzle"].asInt());
					NSDS_SI(stage_number, kSDS_SI_playtime_i, stage_list[i]["playtime"].asInt());
					NSDS_SD(stage_number, kSDS_SI_scoreRate_d, stage_list[i]["scoreRate"].asDouble());
					
					Json::Value t_mission = stage_list[i]["mission"];
					NSDS_SI(stage_number, kSDS_SI_missionType_i, t_mission["type"].asInt());
					
					Json::Value t_option;
					if(!t_mission["option"].isObject())			t_option["key"]="value";
					else										t_option =t_mission["option"];
					
					if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
						NSDS_SI(stage_number, kSDS_SI_missionOptionEnergy_i, t_option["energy"].asInt());
					else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
						NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt());
					else if(t_mission["type"].asInt() == kCLEAR_bigArea)
					{
						NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt());
						NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt());
					}
					else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
						NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt());
					else if(t_mission["type"].asInt() == kCLEAR_perfect)
						NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt());
					else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
						NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt());
					
					
					Json::Value shopItems = stage_list[i]["shopItems"];
					NSDS_SI(stage_number, kSDS_SI_shopItemsCnt_i, shopItems.size());
					for(int i=0;i<shopItems.size();i++)
					{
						Json::Value t_item = shopItems[i];
						NSDS_SS(stage_number, kSDS_SI_shopItems_int1_currency_s, i, t_item["currency"].asString());
						NSDS_SI(stage_number, kSDS_SI_shopItems_int1_type_i, i, t_item["type"].asInt());
						NSDS_SI(stage_number, kSDS_SI_shopItems_int1_price_i, i, t_item["price"].asInt());
						
						
						Json::Value t_option;
						if(!t_item["option"].isObject())				t_option["key"]="value";
						else											t_option =t_item["option"];
						
						if(t_item["type"].asInt() == kIC_attack)
							NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option.get("power",0).asInt());
						else if(t_item["type"].asInt() == kIC_addTime)
							NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option.get("sec",0).asInt());
						else if(t_item["type"].asInt() == kIC_fast)
							NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option.get("sec",0).asInt());
						else if(t_item["type"].asInt() == kIC_silence)
							NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option.get("sec",0).asInt());
						else if(t_item["type"].asInt() == kIC_doubleItem)
							NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_longTime)
							NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt());
						else if(t_item["type"].asInt() == kIC_bossLittleEnergy)
							NSDS_SI(stage_number, kSDS_SI_itemOptionBossLittleEnergyPercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_subSmallSize)
							NSDS_SI(stage_number, kSDS_SI_itemOptionSubSmallSizePercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_smallArea)
							NSDS_SI(stage_number, kSDS_SI_itemOptionSmallAreaPercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_widePerfect)
							NSDS_SI(stage_number, kSDS_SI_itemOptionWidePerfectPercent_i, t_option["percent"].asInt());
					}
					
					Json::Value defItems = stage_list[i]["defItems"];
					NSDS_SI(stage_number, kSDS_SI_defItemsCnt_i, defItems.size());
					for(int i=0;i<defItems.size();i++)
					{
						Json::Value t_item = defItems[i];
						NSDS_SI(stage_number, kSDS_SI_defItems_int1_type_i, i, t_item["type"].asInt());
						
						Json::Value t_option;
						if(!t_item["option"].isObject())				t_option["key"]="value";
						else											t_option =t_item["option"];
						
						if(t_item["type"].asInt() == kIC_attack)
							NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option["power"].asInt());
						else if(t_item["type"].asInt() == kIC_addTime)
							NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option["sec"].asInt());
						else if(t_item["type"].asInt() == kIC_fast)
							NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option["sec"].asInt());
						else if(t_item["type"].asInt() == kIC_silence)
							NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option["sec"].asInt());
						else if(t_item["type"].asInt() == kIC_doubleItem)
							NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_longTime)
							NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt());
						else if(t_item["type"].asInt() == kIC_bossLittleEnergy)
							NSDS_SI(stage_number, kSDS_SI_itemOptionBossLittleEnergyPercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_subSmallSize)
							NSDS_SI(stage_number, kSDS_SI_itemOptionSubSmallSizePercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_smallArea)
							NSDS_SI(stage_number, kSDS_SI_itemOptionSmallAreaPercent_i, t_option["percent"].asInt());
						else if(t_item["type"].asInt() == kIC_widePerfect)
							NSDS_SI(stage_number, kSDS_SI_itemOptionWidePerfectPercent_i, t_option["percent"].asInt());
					}
					
					Json::Value cards = stage_list[i]["cards"];
					for(int i=0;i<cards.size();i++)
					{
						Json::Value t_card = cards[i];
						NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt());
						NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt());
						NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt());
						NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt());
						
						NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1);
						NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["stage"].asInt());
						NSDS_SI(t_card["stage"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
						
						Json::Value t_card_missile = t_card["missile"];
						NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str());
						NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt());
						NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt());
						NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble());
						
						NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str());
						
						Json::Value t_ability = t_card["ability"];
						NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), t_ability.size());
						for(int j=0;j<t_ability.size();j++)
						{
							Json::Value t_abil = t_ability[j];
							NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt());
							
							Json::Value t_option;
							if(!t_abil["option"].isObject())                    t_option["key"]="value";
							else												t_option =t_abil["option"];
							
							if(t_abil["type"].asInt() == kIC_attack)
								NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt());
							else if(t_abil["type"].asInt() == kIC_addTime)
								NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
							else if(t_abil["type"].asInt() == kIC_fast)
								NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
							else if(t_abil["type"].asInt() == kIC_silence)
								NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
							else if(t_abil["type"].asInt() == kIC_doubleItem)
								NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
							else if(t_abil["type"].asInt() == kIC_longTime)
								NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
							else if(t_abil["type"].asInt() == kIC_bossLittleEnergy)
								NSDS_SI(kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
							else if(t_abil["type"].asInt() == kIC_subSmallSize)
								NSDS_SI(kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
							else if(t_abil["type"].asInt() == kIC_smallArea)
								NSDS_SI(kSDS_CI_int1_abilitySmallAreaOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
							else if(t_abil["type"].asInt() == kIC_widePerfect)
								NSDS_SI(kSDS_CI_int1_abilityWidePerfectOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
						}
						
						Json::Value t_imgInfo = t_card["imgInfo"];
						
						bool is_add_cf = false;
						
						if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
						{
							// check, after download ----------
							DownloadFile t_sf;
							t_sf.size = t_imgInfo["size"].asInt();
							t_sf.img = t_imgInfo["img"].asString().c_str();
							t_sf.filename = CCSTR_CWF("stage%d_level%d_visible.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
							t_sf.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
							sf_list.push_back(t_sf);
							// ================================
							
							CopyFile t_cf;
							t_cf.from_filename = t_sf.filename.c_str();
							t_cf.to_filename = CCSTR_CWF("stage%d_level%d_thumbnail.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
							cf_list.push_back(t_cf);
							
							is_add_cf = true;
						}
						
						Json::Value t_aniInfo = t_card["aniInfo"];
						NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool());
						if(t_aniInfo["isAni"].asBool())
						{
							Json::Value t_detail = t_aniInfo["detail"];
							NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt());
							
							Json::Value t_loopSeq = t_detail["loopSeq"];
							for(int j=0;j<t_loopSeq.size();j++)
								NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt());
							
							NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt());
							NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt());
							NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt());
							NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt());
							NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt());
							
							if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
							{
								// check, after download ----------
								DownloadFile t_sf;
								t_sf.size = t_detail["size"].asInt();
								t_sf.img = t_detail["img"].asString().c_str();
								t_sf.filename = CCSTR_CWF("stage%d_level%d_animation.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
								t_sf.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
								sf_list.push_back(t_sf);
								// ================================
							}
							
							if(is_add_cf)
							{
								CopyFile t_cf = cf_list.back();
								cf_list.pop_back();
								t_cf.is_ani = true;
								t_cf.cut_width = t_detail["cutWidth"].asInt();
								t_cf.cut_height = t_detail["cutHeight"].asInt();
								t_cf.position_x = t_detail["positionX"].asInt();
								t_cf.position_y = t_detail["positionY"].asInt();
								
								t_cf.ani_filename = CCSTR_CWF("stage%d_level%d_animation.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
								
								cf_list.push_back(t_cf);
							}
						}
						
						NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString());
						
						Json::Value t_silImgInfo = t_card["silImgInfo"];
						NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool());
						if(t_silImgInfo["isSil"].asBool())
						{
							if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
							{
								// check, after download ----------
								DownloadFile t_sf;
								t_sf.size = t_silImgInfo["size"].asInt();
								t_sf.img = t_silImgInfo["img"].asString().c_str();
								t_sf.filename = CCSTR_CWF("stage%d_level%d_invisible.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
								t_sf.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
								sf_list.push_back(t_sf);
								// ================================
							}
						}
					}
					
					NSDS_SI(stage_number, kSDS_SI_level_i, stage_list[i]["level"].asInt());
					
					NSDS_SS(stage_number, kSDS_SI_boss_s, stage_list[i]["boss"].asString());
					NSDS_SS(stage_number, kSDS_SI_junior_s, stage_list[i]["junior"].asString());
					NSDS_SI(stage_number, kSDS_SI_autoBalanceTry_i, stage_list[i]["autoBalanceTry"].asInt());
				}
			}
			
			if(df_list.size() + sf_list.size() > 0) // need download
			{
				download_version = result_data["version"].asInt();
				state_ment->setString("퍼즐 이미지를 다운로드 합니다.");
				ing_download_cnt = 1;
				ing_download_per = 0;
				download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + sf_list.size()))->getCString(), "etc_font.fnt");
				download_state->setPosition(ccp(240,130));
				addChild(download_state, kSLD_Z_content);
				is_downloading = true;
				startDownload();
			}
			else
			{
				NSDS_SI(puzzle_number, kSDS_PZ_version_i, result_data["version"].asInt());
				
				for(int i=0;i<save_version_list.size();i++)
					NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
				
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
		button_menu->setTouchPriority(-200-1);
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
		download_state->setString(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + sf_list.size()))->getCString());
		is_downloading = true;
		startDownload();
	}
}

void StageListDown::successAction()
{
	unschedule(schedule_selector(StageListDown::downloadingAction));
	if(ing_download_cnt >= df_list.size() + sf_list.size())
	{
		SDS_SS(kSDF_cardInfo, sf_list[ing_download_cnt-df_list.size()-1].key, sf_list[ing_download_cnt-df_list.size()-1].img);
		
		state_ment->setString("카드 섬네일 만드는 중...");
		for(int i=0;i<cf_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(cf_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(cf_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		
		NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version);
		
		for(int i=0;i<save_version_list.size();i++)
			NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
		
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()+sf_list.size()))->getCString());
		state_ment->setString("퍼즐 이미지 다운로드 완료.");
		(target_success->*delegate_success)();
		removeFromParent();
	}
	else if(ing_download_cnt > df_list.size())
	{
		SDS_SS(kSDF_cardInfo, sf_list[ing_download_cnt-df_list.size()-1].key, sf_list[ing_download_cnt-df_list.size()-1].img);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()+sf_list.size()))->getCString());
		startDownload();
	}
	else
	{
		SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()+sf_list.size()))->getCString());
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
	button_menu->setTouchPriority(-200-1);
}

void StageListDown::downloadingAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	
	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;
	
	ing_download_per = t_per;
	
	download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()+sf_list.size()))->getCString());
}

void StageListDown::startDownload()
{
	if(ing_download_cnt > df_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, sf_list[ing_download_cnt-df_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(sf_list[ing_download_cnt-df_list.size()-1].img, sf_list[ing_download_cnt-df_list.size()-1].size,
													  sf_list[ing_download_cnt-df_list.size()-1].filename, this, callfunc_selector(StageListDown::successAction),
													  this, callfunc_selector(StageListDown::failAction));
	}
	else
	{
		CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
													  this, callfunc_selector(StageListDown::successAction), this, callfunc_selector(StageListDown::failAction));
	}
	
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
	setTouchEnabled(true);
	
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
