//
//  StageListDown.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 10..
//
//

#include "StageListDown.h"
#include "PuzzleCache.h"
#include "LoadingTipScene.h"
#include "CommonButton.h"
#include "DataStorageHub.h"
#include "LogData.h"

#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "CumberShowWindow.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "ASPopupView.h"
#include "ConvexGraph.h"

void StageListDown::addDownlist(string t_key, const Json::Value& result_data)
{
	// check, after download ----------
	DownloadFile t_df;
	t_df.size = result_data[t_key.c_str()]["size"].asInt();
	t_df.img = result_data[t_key.c_str()]["image"].asString().c_str();
	t_df.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, t_key.c_str())->getCString();
	t_df.key = t_key.c_str();
	
	auto iter = find(df_list.begin(), df_list.end(), t_df);
	if(iter == df_list.end())
		df_list.push_back(t_df);
	// ================================
}

void StageListDown::resultGetStageList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("getPuzzleInfo : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		
		NSDS_SS(puzzle_number, kSDS_PZ_title_s, result_data["title"].asString(), false);
		NSDS_SS(puzzle_number, kSDS_PZ_condition_s, result_data["condition"].asString(), false);
		NSDS_SB(puzzle_number, kSDS_PZ_isEvent_b, result_data["isEvent"].asBool(), false);
		
		NSDS_SI(puzzle_number, kSDS_PZ_color_r_d, result_data["color"]["r"].asInt(), false);
		NSDS_SI(puzzle_number, kSDS_PZ_color_g_d, result_data["color"]["g"].asInt(), false);
		NSDS_SI(puzzle_number, kSDS_PZ_color_b_d, result_data["color"]["b"].asInt(), false);
		
//		NSDS_SI(puzzle_number, kSDS_PZ_ticket_i, result_data["ticket"].asInt(), false);
//		NSDS_SI(puzzle_number, kSDS_PZ_point_i, result_data["point"].asInt(), false);
		
//		if(NSDS_GS(puzzle_number, kSDS_PZ_map_s) != result_data["map"]["image"].asString())
//		{
//			addDownlist("map", result_data);
//		}
		if(NSDS_GS(puzzle_number, kSDS_PZ_center_s) != result_data["center"]["image"].asString())
		{
			addDownlist("center", result_data);
			if(puzzle_number < 10000)
			{
				DownloadFile t_cut;
				t_cut.size = result_data["center"]["size"].asInt();
				t_cut.img = result_data["center"]["image"].asString().c_str();
				t_cut.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, "center")->getCString();
				t_cut.key = "center";
				cut_list.push_back(t_cut);
			}
		}
		if(NSDS_GS(puzzle_number, kSDS_PZ_original_s) != result_data["original"]["image"].asString())
		{
			addDownlist("original", result_data);
			DownloadFile t_cut;
			t_cut.size = result_data["original"]["size"].asInt();
			t_cut.img = result_data["original"]["image"].asString().c_str();
			t_cut.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, "original")->getCString();
			t_cut.key = "original";
			cut_list.push_back(t_cut);
		}
		if(NSDS_GS(puzzle_number, kSDS_PZ_face_s) != result_data["face"]["image"].asString())
		{
			addDownlist("face", result_data);
			DownloadFile t_cut;
			t_cut.size = result_data["face"]["size"].asInt();
			t_cut.img = result_data["face"]["image"].asString().c_str();
			t_cut.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, "face")->getCString();
			t_cut.key = "face";
			cut_list.push_back(t_cut);
		}
		
		NSDS_SI(puzzle_number, kSDS_PZ_startStage_i, result_data["startStage"].asInt(), false);
		
		Json::Value stage_list = result_data["list"];
		int loop_cnt = stage_list.size();
		NSDS_SI(puzzle_number, kSDS_PZ_stageCount_i, loop_cnt, false);
		
		for(int i=0;i<loop_cnt;i++)
		{
			int stage_number = stage_list[i]["no"].asInt();
			
			NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_level_i, stage_number, stage_list[i]["level"].asInt(), false);
			NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number, stage_list[i]["pieceNo"].asInt(), false);
			
			if(!stage_list[i]["condition"].isNull())
			{
				NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number, stage_list[i]["condition"]["gold"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number, stage_list[i]["condition"]["no"].asInt(), false);
			}
			
			CCLOG("saved version : %d", NSDS_GI(stage_number, kSDS_SI_version_i));
			if(NSDS_GI(stage_number, kSDS_SI_version_i) < stage_list[i]["version"].asInt())
			{
				save_version_list.push_back(IntPoint(stage_number, stage_list[i]["version"].asInt()));
				
				NSDS_SI(stage_number, kSDS_SI_puzzle_i, puzzle_number, false);
				NSDS_SI(stage_number, kSDS_SI_playtime_i, stage_list[i]["playtime"].asInt(), false);
				NSDS_SD(stage_number, kSDS_SI_scoreRate_d, stage_list[i]["scoreRate"].asDouble(), false);
				NSDS_SD(stage_number, kSDS_SI_scale_d, stage_list[i]["scale"].asDouble(), false);
//				NSDS_SB(stage_number, kSDS_SI_minigame_b, stage_list[i]["minigame"].asBool(), false);
				NSDS_SS(stage_number, kSDS_SI_type_s, stage_list[i]["type"].asString(), false);
				
				Json::Value t_mission = stage_list[i]["mission"];
				NSDS_SI(stage_number, kSDS_SI_missionType_i, t_mission["type"].asInt(), false);
				
				Json::Value t_option;
				if(!t_mission["option"].isObject())			t_option["key"]="value";
				else										t_option =t_mission["option"];
				
				if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
					NSDS_SI(stage_number, kSDS_SI_missionOptionEnergy_i, t_option["energy"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_bigArea)
				{
					NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
				}
				else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_perfect)
					NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
					NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_hellMode)
					NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_percentage)
					NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_score)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["score"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_combo)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["combo"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_gold)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["gold"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_turns)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["turns"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_casting)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_littlePercent)
					NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
				
				
				Json::Value shopItems = stage_list[i]["shopItems"];
				NSDS_SI(stage_number, kSDS_SI_shopItemsCnt_i, shopItems.size(), false);
				for(int i=0;i<shopItems.size();i++)
				{
					Json::Value t_item = shopItems[i];
					NSDS_SS(stage_number, kSDS_SI_shopItems_int1_currency_s, i, t_item["currency"].asString(), false);
					NSDS_SI(stage_number, kSDS_SI_shopItems_int1_type_i, i, t_item["type"].asInt(), false);
					NSDS_SI(stage_number, kSDS_SI_shopItems_int1_price_i, i, t_item["price"].asInt(), false);
					
					
					Json::Value t_option;
					if(!t_item["option"].isObject())				t_option["key"]="value";
					else											t_option =t_item["option"];
					
					if(t_item["type"].asInt() == kIC_attack)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option.get("power",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_addTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option.get("sec",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_fast)
						NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option.get("sec",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_silence)
						NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option.get("sec",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_doubleItem)
						NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_longTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_baseSpeedUp)
						NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
				}
				
				Json::Value defItems = stage_list[i]["defItems"];
				NSDS_SI(stage_number, kSDS_SI_defItemsCnt_i, defItems.size(), false);
				for(int i=0;i<defItems.size();i++)
				{
					Json::Value t_item = defItems[i];
					NSDS_SI(stage_number, kSDS_SI_defItems_int1_type_i, i, t_item["type"].asInt(), false);
					
					Json::Value t_option;
					if(!t_item["option"].isObject())				t_option["key"]="value";
					else											t_option =t_item["option"];
					
					if(t_item["type"].asInt() == kIC_attack)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option["power"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_addTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_fast)
						NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_silence)
						NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_doubleItem)
						NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_longTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_baseSpeedUp)
						NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
				}
				
				Json::Value cards = stage_list[i]["cards"];
				NSDS_SI(stage_number, kSDS_SI_cardCount_i, cards.size(), false);
				for(int i=0;i<cards.size();i++)
				{
					Json::Value t_card = cards[i];
					if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
						continue;
					NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
					NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
					NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
					NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
					NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//					NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//					NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
					
//					NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
					NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
					NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
					
//					Json::Value t_card_missile = t_card["missile"];
//					NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//					NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//					NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//					NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//					
//					NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//					
//					Json::Value t_ability = t_card["ability"];
//					NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//					for(int j=0;j<t_ability.size();j++)
//					{
//						Json::Value t_abil = t_ability[j];
//						NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//						
//						Json::Value t_option;
//						if(!t_abil["option"].isObject())                    t_option["key"]="value";
//						else												t_option =t_abil["option"];
//						
//						if(t_abil["type"].asInt() == kIC_attack)
//							NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//						else if(t_abil["type"].asInt() == kIC_addTime)
//							NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//						else if(t_abil["type"].asInt() == kIC_fast)
//							NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//						else if(t_abil["type"].asInt() == kIC_silence)
//							NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//						else if(t_abil["type"].asInt() == kIC_doubleItem)
//							NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//						else if(t_abil["type"].asInt() == kIC_longTime)
//							NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//						else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//							NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//					}
					
					NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
					NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
					
					Json::Value t_imgInfo = t_card["imgInfo"];
					
//					bool is_add_cf = false;
					
					if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
					{
						// check, after download ----------
						DownloadFile t_sf;
						t_sf.size = t_imgInfo["size"].asInt();
						t_sf.img = t_imgInfo["img"].asString().c_str();
						t_sf.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
						t_sf.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
						
						auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
						if(iter == sf_list.end())
							sf_list.push_back(t_sf);
						// ================================
						
//						CopyFile t_cf;
//						t_cf.from_filename = t_sf.filename.c_str();
//						t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//						cf_list.push_back(t_cf);
//						
//						is_add_cf = true;
					}
					
					Json::Value t_aniInfo = t_card["aniInfo"];
					NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
					if(t_aniInfo["isAni"].asBool())
					{
						Json::Value t_detail = t_aniInfo["detail"];
						NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
						
						Json::Value t_loopSeq = t_detail["loopSeq"];
						for(int j=0;j<t_loopSeq.size();j++)
							NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
						
						NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
						
						if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
						{
							// check, after download ----------
							DownloadFile t_sf;
							t_sf.size = t_detail["size"].asInt();
							t_sf.img = t_detail["img"].asString().c_str();
							t_sf.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
							t_sf.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
							
							auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
							if(iter == sf_list.end())
								sf_list.push_back(t_sf);
							// ================================
						}
						
//						if(is_add_cf)
//						{
//							CopyFile t_cf = cf_list.back();
//							cf_list.pop_back();
//							t_cf.is_ani = true;
//							t_cf.cut_width = t_detail["cutWidth"].asInt();
//							t_cf.cut_height = t_detail["cutHeight"].asInt();
//							t_cf.position_x = t_detail["positionX"].asInt();
//							t_cf.position_y = t_detail["positionY"].asInt();
//							
//							t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//							
//							cf_list.push_back(t_cf);
//						}
					}
					
					NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
					NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
					NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
					NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
					NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
					
					NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
					NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
					NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
					
					int sound_cnt = t_card["sound"].size();
					NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
					for(int j=1;j<=sound_cnt;j++)
					{
						CCLOG("sound is %s",t_card["sound"][j-1].asString().c_str());
						NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
					}
					
					NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
					
					Json::Value t_silImgInfo = t_card["silImgInfo"];
					NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
					if(t_silImgInfo["isSil"].asBool())
					{
						if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
						{
							// check, after download ----------
							DownloadFile t_sf;
							t_sf.size = t_silImgInfo["size"].asInt();
							t_sf.img = t_silImgInfo["img"].asString().c_str();
							t_sf.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
							t_sf.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
							
							auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
							if(iter == sf_list.end())
								sf_list.push_back(t_sf);
							// ================================
						}
					}
					
					Json::Value t_faceInfo = t_card["faceInfo"];
					if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
					{
						NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
//						NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
						
						DownloadFile t_df1;
						t_df1.size = t_faceInfo["size"].asInt();
						t_df1.img = t_faceInfo["ccbi"].asString().c_str();
						t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
						t_df1.key = mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str();
						sf_list.push_back(t_df1);
						
						DownloadFile t_df2;
						t_df2.size = t_faceInfo["size"].asInt();
						t_df2.img = t_faceInfo["plist"].asString().c_str();
						t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
						t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
						sf_list.push_back(t_df2);
						
						DownloadFile t_df3;
						t_df3.size = t_faceInfo["size"].asInt();
						t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
						t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
						t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
						sf_list.push_back(t_df3);
						
//						if(!is_add_cf)
//						{
//							CopyFile t_cf;
//							t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//							t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//							cf_list.push_back(t_cf);
//							
//							is_add_cf = true;
//						}
//						
//						CopyFile t_cf = cf_list.back();
//						cf_list.pop_back();
//						t_cf.is_ccb = true;
//						t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//						
//						cf_list.push_back(t_cf);
					}
				}
				
//				NSDS_SI(stage_number, kSDS_SI_level_i, stage_list[i]["level"].asInt(), false);
				
				NSDS_SS(stage_number, kSDS_SI_boss_s, stage_list[i]["boss"].asString(), false);
				NSDS_SS(stage_number, kSDS_SI_junior_s, stage_list[i]["junior"].asString(), false);
				NSDS_SI(stage_number, kSDS_SI_autoBalanceTry_i, stage_list[i]["autoBalanceTry"].asInt(), false);
				
				mySDS->fFlush(stage_number, kSDS_SI_base);
			}
		}
		
		NSDS_SI(puzzle_number, kSDS_PZ_clearCard_i, result_data["clearReward"]["normal"]["no"].asInt(), false);
		{
			Json::Value t_card = result_data["clearReward"]["normal"];
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
//			Json::Value t_card_missile = t_card["missile"];
//			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//			
//			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//			
//			Json::Value t_ability = t_card["ability"];
//			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//			for(int j=0;j<t_ability.size();j++)
//			{
//				Json::Value t_abil = t_ability[j];
//				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//				
//				Json::Value t_option;
//				if(!t_abil["option"].isObject())                    t_option["key"]="value";
//				else												t_option =t_abil["option"];
//				
//				if(t_abil["type"].asInt() == kIC_attack)
//					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_addTime)
//					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_fast)
//					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_silence)
//					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_doubleItem)
//					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_longTime)
//					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//					NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//			}
			
			NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
			NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
//			bool is_add_cf = false;
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_sf;
				t_sf.size = t_imgInfo["size"].asInt();
				t_sf.img = t_imgInfo["img"].asString().c_str();
				t_sf.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
				t_sf.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				
				auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
				if(iter == sf_list.end())
					sf_list.push_back(t_sf);
				// ================================
				
//				CopyFile t_cf;
//				t_cf.from_filename = t_sf.filename.c_str();
//				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//				cf_list.push_back(t_cf);
//				
//				is_add_cf = true;
			}
			
			Json::Value t_aniInfo = t_card["aniInfo"];
			NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
			if(t_aniInfo["isAni"].asBool())
			{
				Json::Value t_detail = t_aniInfo["detail"];
				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
				
				Json::Value t_loopSeq = t_detail["loopSeq"];
				for(int j=0;j<t_loopSeq.size();j++)
					NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
				
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
				
				if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_sf;
					t_sf.size = t_detail["size"].asInt();
					t_sf.img = t_detail["img"].asString().c_str();
					t_sf.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					t_sf.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					
					auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
					if(iter == sf_list.end())
						sf_list.push_back(t_sf);
					// ================================
				}
				
//				if(is_add_cf)
//				{
//					CopyFile t_cf = cf_list.back();
//					cf_list.pop_back();
//					t_cf.is_ani = true;
//					t_cf.cut_width = t_detail["cutWidth"].asInt();
//					t_cf.cut_height = t_detail["cutHeight"].asInt();
//					t_cf.position_x = t_detail["positionX"].asInt();
//					t_cf.position_y = t_detail["positionY"].asInt();
//					
//					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//					
//					cf_list.push_back(t_cf);
//				}
			}
			
			NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
			NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
			NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
			NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
			NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
			
			int sound_cnt = t_card["sound"].size();
			NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
			for(int j=1;j<=sound_cnt;j++)
			{
				NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
			}
			
			NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
			
			Json::Value t_silImgInfo = t_card["silImgInfo"];
			NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
			if(t_silImgInfo["isSil"].asBool())
			{
				if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_sf;
					t_sf.size = t_silImgInfo["size"].asInt();
					t_sf.img = t_silImgInfo["img"].asString().c_str();
					t_sf.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
					t_sf.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					
					auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
					if(iter == sf_list.end())
						sf_list.push_back(t_sf);
					// ================================
				}
			}
			
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
//				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				DownloadFile t_df1;
				t_df1.size = t_faceInfo["size"].asInt();
				t_df1.img = t_faceInfo["ccbi"].asString().c_str();
				t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
				t_df1.key = mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str();
				sf_list.push_back(t_df1);
				
				DownloadFile t_df2;
				t_df2.size = t_faceInfo["size"].asInt();
				t_df2.img = t_faceInfo["plist"].asString().c_str();
				t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
				t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
				sf_list.push_back(t_df2);
				
				DownloadFile t_df3;
				t_df3.size = t_faceInfo["size"].asInt();
				t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
				t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
				t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
				sf_list.push_back(t_df3);
				
//				if(!is_add_cf)
//				{
//					CopyFile t_cf;
//					t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					cf_list.push_back(t_cf);
//					
//					is_add_cf = true;
//				}
//				
//				CopyFile t_cf = cf_list.back();
//				cf_list.pop_back();
//				t_cf.is_ccb = true;
//				t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//				
//				cf_list.push_back(t_cf);
			}
			
			mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
		}
		
		
		NSDS_SI(puzzle_number, kSDS_PZ_perfectCard_i, result_data["clearReward"]["perfect"]["no"].asInt(), false);
		{
			Json::Value t_card = result_data["clearReward"]["perfect"];
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
//			Json::Value t_card_missile = t_card["missile"];
//			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//			
//			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//			
//			Json::Value t_ability = t_card["ability"];
//			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//			for(int j=0;j<t_ability.size();j++)
//			{
//				Json::Value t_abil = t_ability[j];
//				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//				
//				Json::Value t_option;
//				if(!t_abil["option"].isObject())                    t_option["key"]="value";
//				else												t_option =t_abil["option"];
//				
//				if(t_abil["type"].asInt() == kIC_attack)
//					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_addTime)
//					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_fast)
//					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_silence)
//					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_doubleItem)
//					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_longTime)
//					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//					NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//			}
			
			NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
			NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
//			bool is_add_cf = false;
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_sf;
				t_sf.size = t_imgInfo["size"].asInt();
				t_sf.img = t_imgInfo["img"].asString().c_str();
				t_sf.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
				t_sf.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				
				auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
				if(iter == sf_list.end())
				{
					sf_list.push_back(t_sf);
					// ================================
					
//					CopyFile t_cf;
//					t_cf.from_filename = t_sf.filename.c_str();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					cf_list.push_back(t_cf);
//					
//					is_add_cf = true;
				}
			}
			
			Json::Value t_aniInfo = t_card["aniInfo"];
			NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
			if(t_aniInfo["isAni"].asBool())
			{
				Json::Value t_detail = t_aniInfo["detail"];
				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
				
				Json::Value t_loopSeq = t_detail["loopSeq"];
				for(int j=0;j<t_loopSeq.size();j++)
					NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
				
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
				
				if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_sf;
					t_sf.size = t_detail["size"].asInt();
					t_sf.img = t_detail["img"].asString().c_str();
					t_sf.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					t_sf.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					
					auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
					if(iter == sf_list.end())
						sf_list.push_back(t_sf);
					// ================================
				}
				
//				if(is_add_cf)
//				{
//					CopyFile t_cf = cf_list.back();
//					cf_list.pop_back();
//					t_cf.is_ani = true;
//					t_cf.cut_width = t_detail["cutWidth"].asInt();
//					t_cf.cut_height = t_detail["cutHeight"].asInt();
//					t_cf.position_x = t_detail["positionX"].asInt();
//					t_cf.position_y = t_detail["positionY"].asInt();
//					
//					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//					
//					cf_list.push_back(t_cf);
//				}
			}
			
			NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
			NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
			NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
			NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
			NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
			
			int sound_cnt = t_card["sound"].size();
			NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
			for(int j=1;j<=sound_cnt;j++)
			{
				NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
			}
			
			NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
			
			Json::Value t_silImgInfo = t_card["silImgInfo"];
			NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
			if(t_silImgInfo["isSil"].asBool())
			{
				if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_sf;
					t_sf.size = t_silImgInfo["size"].asInt();
					t_sf.img = t_silImgInfo["img"].asString().c_str();
					t_sf.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
					t_sf.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					
					auto iter = find(sf_list.begin(), sf_list.end(), t_sf);
					if(iter == sf_list.end())
						sf_list.push_back(t_sf);
					// ================================
				}
			}
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
//				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				DownloadFile t_df1;
				t_df1.size = t_faceInfo["size"].asInt();
				t_df1.img = t_faceInfo["ccbi"].asString().c_str();
				t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
				t_df1.key = mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str();
				sf_list.push_back(t_df1);
				
				DownloadFile t_df2;
				t_df2.size = t_faceInfo["size"].asInt();
				t_df2.img = t_faceInfo["plist"].asString().c_str();
				t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
				t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
				sf_list.push_back(t_df2);
				
				DownloadFile t_df3;
				t_df3.size = t_faceInfo["size"].asInt();
				t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
				t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
				t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
				sf_list.push_back(t_df3);
				
//				if(!is_add_cf)
//				{
//					CopyFile t_cf;
//					t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					cf_list.push_back(t_cf);
//					
//					is_add_cf = true;
//				}
//				
//				CopyFile t_cf = cf_list.back();
//				cf_list.pop_back();
//				t_cf.is_ccb = true;
//				t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//				
//				cf_list.push_back(t_cf);
			}
			mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
		}
		
		mySDS->fFlush(puzzle_number, kSDS_PZ_bottom_s);
		mySDS->fFlush(kSDS_CI_base);
		
		if(df_list.size() + sf_list.size() > 0) // need download
		{
			if(download_start == nullptr)
			{
				tip_img = LoadingTipScene::getLoadingTipImage();
				tip_img->setPosition(ccp(240,160));
				addChild(tip_img, kSLD_Z_back);
				
				CCDelayTime* t_delay = CCDelayTime::create(7);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StageListDown::changeTipImage));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
				
				tip_img->runAction(t_seq);
				
				gray->removeFromParent();
				
//				CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//				CCBReader* reader = new CCBReader(nodeLoader);
//				CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
				CCSprite* loading_progress_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
				loading_progress_img->setPosition(ccp(240,38));
				addChild(loading_progress_img, kSLD_Z_content);
//				reader->release();
				
				download_version = result_data["version"].asInt();
				state_ment->setPosition(ccp(240,80));
				success_download_cnt = 0;
				ing_download_cnt = 1;
				ing_download_per = 0;
				download_state = KSLabelTTF::create(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString(), mySGD->getFont().c_str(), 11);
				download_state->enableOuterStroke(ccBLACK, 1.f);
				download_state->setPosition(ccp(240,80+20));
				addChild(download_state, kSLD_Z_content);
				
				download_set.clear();
				
				is_enable_index.clear();
				for(int i=0;i<5;i++)
				{
					is_enable_index.push_back(i);
				}
				
				startDownloadSet();
			}
			else
			{
				download_version = result_data["version"].asInt();
				
				download_start([=]()
							   {
								   startOpenning();
							   });
			}
		}
		else
		{
			NSDS_SI(puzzle_number, kSDS_PZ_version_i, result_data["version"].asInt());
			mySDS->fFlush(puzzle_number, kSDS_PZ_bottom_s);
			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
			
			for(int i=0;i<save_version_list.size();i++)
			{
				NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
				mySDS->fFlush(save_version_list[i].x, kSDS_SI_autoBalanceTry_i);
			}
			
			(target_success->*delegate_success)();
//			removeFromParent();
		}
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		(target_success->*delegate_success)();
//		removeFromParent();
	}
	else
	{
		ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
			CCNode* t_node = CCNode::create();
			t_node->setTag(kSLD_MT_receive);
			menuAction(t_node);
		}, 1);
		if(alert)
			((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		
//		CommonButton* replay_menu = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, -201);
//		replay_menu->setPosition(ccp(300,100));
//		replay_menu->setFunction([=](CCObject* sender)
//								 {
//									 CCNode* t_node = CCNode::create();
//									 t_node->setTag(kSLD_MT_receive);
//									 menuAction(t_node);
//								 });
//		
//		addChild(replay_menu, kSLD_Z_content, kSLD_MT_receive);
	}
}

void StageListDown::startOpenning()
{
	loading_character = CumberShowWindow::createLoading();
	loading_character->setPosition(ccp(240,500));
	addChild(loading_character, kSLD_Z_content);
	
	CCSequence* t_seq = CCSequence::createWithTwoActions(CCMoveTo::create(0.5f, ccp(240,160-12)), CCCallFunc::create(this, callfunc_selector(StageListDown::endOpenning)));
	loading_character->runAction(t_seq);
}

void StageListDown::endOpenning()
{
	talk_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_stageListDown), mySGD->getFont().c_str(), 12, CCSizeMake(300, 70), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	talk_label->disableOuterStroke();//enableOuterStroke(ccBLACK, 1.f);
	talk_label->setAnchorPoint(ccp(0.5,0.5f));
	
	talk_box = CCScale9Sprite::create("loading_talkbox.png", CCRectMake(0, 0, 150, 64.5f), CCRectMake(50, 21.5f, 50, 21.5f));
	talk_box->setContentSize(CCSizeMake(100+talk_label->getContentSize().width, 43+talk_label->getContentSize().height));
	talk_box->setAnchorPoint(ccp(0.5,0));
	talk_box->setPosition(ccp(240,200));
	talk_box->setVisible(false);
	addChild(talk_box, kSLD_Z_content);
	
	talk_label->setPosition(ccp(240, 230-159));
	addChild(talk_label);
	
	KS::setOpacity(talk_label, 0);
	
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
										   {
											   KS::setOpacity(talk_label, t*255);
										   }, [=](float t)
										   {
											   KS::setOpacity(talk_label, 255);
										   }));
	
	
	
	
	gray->removeFromParent();

	loading_progress = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
	loading_progress->setPosition(ccp(240,100));
	addChild(loading_progress);
	
	loading_progress->setCover("loading_progress_front1.png", "loading_progress_mask.png");
	loading_progress->setBack("loading_progress_back.png");
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
//	loading_progress_img->setPosition(ccp(240,38));
//	addChild(loading_progress_img, kSLD_Z_content);
//	reader->release();
	
	tip_list.clear();
	
	for(int i=int(LK::kMyLocalKey_titleLoadingBegin)+1;i<int(LK::kMyLocalKey_titleLoadingEnd);i++)
	{
		tip_list.push_back(myLoc->getLocalForKey((LK)i));
	}
	
	random_shuffle(tip_list.begin(), tip_list.end());
	
	recent_tip_index = 0;
	
	talk_label->setString(tip_list[recent_tip_index].c_str());
	talk_box->setContentSize(CCSizeMake(100+talk_label->getContentSize().width, 43+talk_label->getContentSize().height));
	
	addChild(KSTimer::create(4.f, [=](){changeTipMent();}));
	
	state_ment->setPosition(ccp(240,80));
	ing_download_cnt = 1;
	success_download_cnt = 0;
	ing_download_per = 0;
	download_state = KSLabelTTF::create(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString(), mySGD->getFont().c_str(), 11);
	download_state->enableOuterStroke(ccBLACK, 1.f);
	download_state->setPosition(ccp(240,80+20));
	addChild(download_state, kSLD_Z_content);
	download_set.clear();
	
	is_enable_index.clear();
	for(int i=0;i<5;i++)
	{
		is_enable_index.push_back(i);
	}
	
	startDownloadSet();
}

void StageListDown::changeTipMent()
{
	recent_tip_index++;
	recent_tip_index %= int(tip_list.size());
	
	talk_label->setString(tip_list[recent_tip_index].c_str());
	talk_box->setContentSize(CCSizeMake(100+talk_label->getContentSize().width, 43+talk_label->getContentSize().height));
	
	addChild(KSTimer::create(4.f, [=](){changeTipMent();}));
}

void StageListDown::outOpenning()
{
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.28f, [=](float t)
										   {
											   KS::setOpacity(talk_label, 255-t*255);
										   }, [=](float t)
										   {
											   KS::setOpacity(talk_label, 0);
										   }));
	
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
										   {
											   state_ment->setVisible(false);
											   download_state->setPosition(ccp(240,80+20-300.f*t));
											   loading_progress->setPosition(ccp(240,100-300.f*t));
											   loading_character->setPosition(ccp(240,160-12+400.f*t));
											   talk_box->setPosition(ccp(240,200+300.f*t));
											   talk_box->setOpacity(255-t*255.f);
										   }, [=](float t)
										   {
											   download_state->setPosition(ccp(240,80+20-300.f));
											   loading_progress->setPosition(ccp(240,100-300.f));
											   loading_character->setPosition(ccp(240,160-12+400.f));
											   talk_box->setPosition(ccp(240,200+300.f));
											   talk_box->setOpacity(0);
											   
											   success_func();
										   }));
}

void StageListDown::menuAction(CCObject *sender)
{
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kSLD_MT_receive)
	{
		removeChildByTag(kSLD_MT_receive);
		startGetStageList();
	}
	else if(tag == kSLD_MT_redown)
	{
		removeChildByTag(kSLD_MT_redown);
		ing_download_per = 0;
		ing_download_cnt--;
		if(ing_download_cnt < 1)
			ing_download_cnt = 1;
		if(int(df_list.size()+sf_list.size()) > 0)
			download_state->setString(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString());
		startDownload();
	}
}

void StageListDown::successAction()
{
	unschedule(schedule_selector(StageListDown::downloadingAction));
	
	if(ing_download_cnt < df_list.size())
	{
		SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		if(int(df_list.size()+sf_list.size()) > 0)
		{
			download_state->setString(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString());
			if(loading_progress)
			{
				loading_progress->setPercentage((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()));
			}
		}
		startDownload();
	}
	else if(ing_download_cnt == df_list.size())
	{
		SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
		
		for(int j=0;j<cut_list.size();j++)
		{
			CCImage *img = new CCImage;
			img->initWithEncryptedImageFileFullPath((mySIL->getDocumentPath() + cut_list[j].filename).c_str()); //퍼즐이미지를 불러옵니다.
			
			CCImage *st_w = new CCImage;
			CCImage *st_h = new CCImage;
			
			int puzzleCol,puzzleRow;
			float puzzleColDis, puzzleRowDis, puzzleOffsetX, puzzleOffsetY;
			float faceColDis, faceRowDis; //172, 172
			float puzzleWidth,puzzleHeight;
			
			
			if(cut_list[j].key == "face")
			{
				if(puzzle_number > 10000)
				{
					st_w->initWithImageFile("stage_scissor.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("stage_scissor.png"); //피스조각(세로형)을 불러옵니다.
				}
				else
				{
					st_w->initWithImageFile("puzzle_stencil_1_pw.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("puzzle_stencil_1_ph.png"); //피스조각(세로형)을 불러옵니다.
				}
			}
			else
			{
				if(puzzle_number > 10000)
				{
					st_w->initWithImageFile("temp_puzzle_stencil_pw.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("temp_puzzle_stencil_ph.png"); //피스조각(세로형)을 불러옵니다.
				}
				else
				{
					st_w->initWithImageFile("puzzle_stencil_1_pw.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("puzzle_stencil_1_ph.png"); //피스조각(세로형)을 불러옵니다.
				}
			}
			
			if(puzzle_number > 10000)
			{
				puzzleCol=6;
				puzzleRow=4;
				puzzleColDis=100.f;
				puzzleRowDis=100.f;
				puzzleOffsetX=76.f;
				puzzleOffsetY=76.f;
				faceColDis=132.f;
				faceRowDis=132.f;
				puzzleWidth=652.f;
				puzzleHeight=452.f;
			}
			else
			{
				puzzleCol=5;
				puzzleRow=4;
				puzzleColDis=120.f;
				puzzleRowDis=120.f;
				puzzleOffsetX=92.f;
				puzzleOffsetY=92.f;
				faceColDis=154.f;
				faceRowDis=154.f;
				puzzleWidth=664;
				puzzleHeight=544;
			}
			
			
			for(int i=0;i<puzzleCol*puzzleRow;i++){
				//피스의 좌표를 구합니다. 퍼즐은 5*4 개로 이루어져있습니다.
				int x = i%puzzleCol;
				int y = i/puzzleCol;
				
				CCImage *st = st_h;
				if(puzzle_number > 10000)
				{
					if((x+(puzzleRow-1-y))%2 == 1)
						st=st_w;
				}
				else
				{
					if(i%2==0)
						st=st_w; //피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
				}
				
				//저장할파일명을 지정합니다.
				string filename;
				if(puzzle_number > 10000)
					filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, cut_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol)+1)->getCString();
				else
					filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, cut_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol))->getCString();
				
				//원본파일에서 자를 위치를 계산합니다.
				int cutx, cuty;
				if(cut_list[j].key == "face")
				{
					cutx = x*faceColDis+faceColDis/2.f;
					cuty = y*faceRowDis+faceRowDis/2.f;
				}
				else if(cut_list[j].key == "original" || cut_list[j].key == "center")
				{
					cutx =x*puzzleColDis+puzzleOffsetX;
					cuty =y*puzzleRowDis+puzzleOffsetY;
				}
				
				//자르고 저장합니다.
				bool isSuccess = PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				//실패했으면 한번더 자르게 해줍니다.
				if(!isSuccess){
					i--;
					continue;
				}
				
				//테스트로 한번 붙여봅니다.
//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//				spr->setAnchorPoint(ccp(0.5,0.5));
//				spr->setPosition(ccp(cutx/2,cuty/2));
//				addChild(spr,1000);
			}
			
			st_w->release(); //가로 피스 메모리해제
			st_h->release(); //세로 피스 메모리해제
			
			//가장자리 자르기
			//위쪽부터 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_top.png");
				else
					st->initWithImageFile("puzzle_stencil_3_top.png");
				
				int cutx =puzzleWidth/2;
				int cuty =puzzleHeight-st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//				spr->setAnchorPoint(ccp(0.5,0.5));
//				spr->setPosition(ccp(cutx/2,cuty/2));
//				addChild(spr,1000);
			}
			
			//아래쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_bottom.png");
				else
					st->initWithImageFile("puzzle_stencil_3_bottom.png");
				
				int cutx =puzzleWidth/2;
				int cuty =st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//				spr->setAnchorPoint(ccp(0.5,0.5));
//				spr->setPosition(ccp(cutx/2,cuty/2));
//				addChild(spr,1000);
			}
			
			//왼쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_left.png");
				else
					st->initWithImageFile("puzzle_stencil_3_left.png");
				
				int cutx =st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//				spr->setAnchorPoint(ccp(0.5,0.5));
//				spr->setPosition(ccp(cutx/2,cuty/2));
//				addChild(spr,1000);
			}
			//오른쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_right.png");
				else
					st->initWithImageFile("puzzle_stencil_3_right.png");
				
				int cutx =puzzleWidth-st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//				spr->setAnchorPoint(ccp(0.5,0.5));
//				spr->setPosition(ccp(cutx/2,cuty/2));
//				addChild(spr,1000);
			}
			//메모리해제
			img->release();
		}
		
		if(sf_list.empty())
		{
			NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version, false);
			mySDS->fFlush(puzzle_number, kSDS_PZ_bottom_s);
			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
			
			for(int i=0;i<save_version_list.size();i++)
			{
				NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
				mySDS->fFlush(save_version_list[i].x, kSDS_SI_autoBalanceTry_i);
			}
			
			if(int(df_list.size()+sf_list.size()) > 0)
			{
				download_state->setString(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString());
				if(loading_progress)
				{
					loading_progress->setPercentage((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()));
				}
			}
			if(success_func == nullptr)
				(target_success->*delegate_success)();
			else
				outOpenning();
//			removeFromParent();
		}
		else
		{
			ing_download_cnt++;
			ing_download_per = 0.f;
			if(int(df_list.size()+sf_list.size()) > 0)
			{
				download_state->setString(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString());
				if(loading_progress)
				{
					loading_progress->setPercentage((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()));
				}
			}
			startDownload();
		}
	}
	else if(ing_download_cnt < df_list.size() + sf_list.size())
	{
		SDS_SS(kSDF_cardInfo, sf_list[ing_download_cnt-df_list.size()-1].key, sf_list[ing_download_cnt-df_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		if(int(df_list.size()+sf_list.size()) > 0)
		{
			download_state->setString(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString());
			if(loading_progress)
			{
				loading_progress->setPercentage((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()));
			}
		}
		startDownload();
	}
	else
	{
		SDS_SS(kSDF_cardInfo, sf_list[ing_download_cnt-df_list.size()-1].key, sf_list[ing_download_cnt-df_list.size()-1].img, false);
		
//		for(int i=0;i<cf_list.size();i++)
//		{
//			mySIL->removeTextureCache(cf_list[i].from_filename);
//			mySIL->removeTextureCache(cf_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(cf_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
////				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
////			
////			if(cf_list[i].is_ccb)
////			{
////				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + cf_list[i].ccb_filename.c_str()).first;
////				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////				target_img->addChild(face_img);
////			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->beginWithClear(0, 0, 0, 0);
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(cf_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", cf_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
		
		NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version, false);
		mySDS->fFlush(puzzle_number, kSDS_PZ_bottom_s);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		for(int i=0;i<save_version_list.size();i++)
		{
			NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
			mySDS->fFlush(save_version_list[i].x, kSDS_SI_autoBalanceTry_i);
		}
		
		if(int(df_list.size()+sf_list.size()) > 0)
		{
			download_state->setString(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString());
			if(loading_progress)
			{
				loading_progress->setPercentage((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()));
			}
		}
		
		addChild(KSTimer::create(0.3f, [=]()
		{
			if(success_func == nullptr)
				(target_success->*delegate_success)();
			else
				outOpenning();
		}));
		
//		removeFromParent();
	}
}

void StageListDown::failAction()
{
	unschedule(schedule_selector(StageListDown::downloadingAction));
	is_downloading = false;
	
	CommonButton* replay_menu = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, -201);
	replay_menu->setPosition(ccp(300,100));
	replay_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kSLD_MT_redown);
								 menuAction(t_node);
							 });
	
	addChild(replay_menu, kSLD_Z_content, kSLD_MT_redown);
}

void StageListDown::downloadingAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	
	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;
	
	ing_download_per = t_per;
	
	if(int(df_list.size()+sf_list.size()) > 0)
		download_state->setString(CCSTR_CWF("%.0f%%", clampf((100.f*ing_download_cnt)/int(df_list.size()+sf_list.size()), 0.f, 100.f))->getCString());
}

void StageListDown::startDownload()
{
	if(ing_download_cnt > df_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, sf_list[ing_download_cnt-df_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(sf_list[ing_download_cnt-df_list.size()-1].img, sf_list[ing_download_cnt-df_list.size()-1].size,
													  sf_list[ing_download_cnt-df_list.size()-1].filename, this, callfunc_selector(StageListDown::successAction),
													  this, callfunc_selector(StageListDown::failAction));
	}
	else
	{
		CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
													  this, callfunc_selector(StageListDown::successAction), this, callfunc_selector(StageListDown::failAction));
	}
	
	schedule(schedule_selector(StageListDown::downloadingAction));
}

//void StageListDown::successActionSet(string t_filename)
//{
//	success_download_cnt++;
//	float download_percent = 100.f*success_download_cnt/int(df_list.size() + sf_list.size());
//	if(download_percent > 100.f)
//		download_percent = 100.f;
//	download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
//	
//	loading_progress->stopAllActions();
//	CCProgressFromTo* t_to = CCProgressFromTo::create(0.5f, loading_progress->getPercentage(), download_percent);
//	loading_progress->runAction(t_to);
//	
//	vector<DownloadImgInfo>::iterator iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(t_filename));
//	
//	if(iter != download_set.end())
//	{
//		download_set.erase(iter);
//	}
//	
//	rest_download_cnt--;
//	
//	if(rest_download_cnt <= 0)
//	{
//		if(ing_download_cnt > df_list.size() + sf_list.size() && download_set.size() == 0)
//		{
//			// divide
//			for(int j=0;j<cut_list.size();j++)
//			{
//				CCImage *img = new CCImage;
//				img->initWithImageFileThreadSafe((mySIL->getDocumentPath() + cut_list[j].filename).c_str()); //퍼즐이미지를 불러옵니다.
//				
//				CCImage *st_w = new CCImage;
//				CCImage *st_h = new CCImage;
//				
//				int puzzleCol,puzzleRow;
//				float puzzleColDis, puzzleRowDis, puzzleOffsetX, puzzleOffsetY;
//				float faceColDis, faceRowDis; //172, 172
//				float puzzleWidth,puzzleHeight;
//				
//				
//				if(cut_list[j].key == "face")
//				{
//					if(puzzle_number > 10000)
//					{
//						st_w->initWithImageFile("stage_scissor.png"); //피스조각(가로형)을 불러옵니다.
//						st_h->initWithImageFile("stage_scissor.png"); //피스조각(세로형)을 불러옵니다.
//					}
//					else
//					{
//						st_w->initWithImageFile("puzzle_stencil_1_pw.png"); //피스조각(가로형)을 불러옵니다.
//						st_h->initWithImageFile("puzzle_stencil_1_ph.png"); //피스조각(세로형)을 불러옵니다.
//					}
//				}
//				else
//				{
//					if(puzzle_number > 10000)
//					{
//						st_w->initWithImageFile("temp_puzzle_stencil_pw.png"); //피스조각(가로형)을 불러옵니다.
//						st_h->initWithImageFile("temp_puzzle_stencil_ph.png"); //피스조각(세로형)을 불러옵니다.
//					}
//					else
//					{
//						st_w->initWithImageFile("puzzle_stencil_1_pw.png"); //피스조각(가로형)을 불러옵니다.
//						st_h->initWithImageFile("puzzle_stencil_1_ph.png"); //피스조각(세로형)을 불러옵니다.
//					}
//				}
//				
//				if(puzzle_number > 10000)
//				{
//					puzzleCol=6;
//					puzzleRow=4;
//					puzzleColDis=100.f;
//					puzzleRowDis=100.f;
//					puzzleOffsetX=76.f;
//					puzzleOffsetY=76.f;
//					faceColDis=132.f;
//					faceRowDis=132.f;
//					puzzleWidth=652.f;
//					puzzleHeight=452.f;
//				}
//				else
//				{
//					puzzleCol=5;
//					puzzleRow=4;
//					puzzleColDis=120.f;
//					puzzleRowDis=120.f;
//					puzzleOffsetX=92.f;
//					puzzleOffsetY=92.f;
//					faceColDis=154.f;
//					faceRowDis=154.f;
//					puzzleWidth=664;
//					puzzleHeight=544;
//				}
//				
//				
//				for(int i=0;i<puzzleCol*puzzleRow;i++){
//					//피스의 좌표를 구합니다. 퍼즐은 5*4 개로 이루어져있습니다.
//					int x = i%puzzleCol;
//					int y = i/puzzleCol;
//					
//					CCImage *st = st_h;
//					if(puzzle_number > 10000)
//					{
//						if((x+(puzzleRow-1-y))%2 == 1)
//							st=st_w;
//					}
//					else
//					{
//						if(i%2==0)
//							st=st_w; //피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
//					}
//					
//					//저장할파일명을 지정합니다.
//					string filename;
//					if(puzzle_number > 10000)
//						filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, cut_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol)+1)->getCString();
//					else
//						filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, cut_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol))->getCString();
//					
//					//원본파일에서 자를 위치를 계산합니다.
//					int cutx, cuty;
//					if(cut_list[j].key == "face")
//					{
//						cutx = x*faceColDis+faceColDis/2.f;
//						cuty = y*faceRowDis+faceRowDis/2.f;
//					}
//					else if(cut_list[j].key == "original" || cut_list[j].key == "center")
//					{
//						cutx =x*puzzleColDis+puzzleOffsetX;
//						cuty =y*puzzleRowDis+puzzleOffsetY;
//					}
//					
//					//자르고 저장합니다.
//					bool isSuccess = PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					//실패했으면 한번더 자르게 해줍니다.
//					if(!isSuccess){
//						i--;
//						continue;
//					}
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				
//				st_w->release(); //가로 피스 메모리해제
//				st_h->release(); //세로 피스 메모리해제
//				
//				//가장자리 자르기
//				//위쪽부터 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					if(puzzle_number > 10000)
//						st->initWithImageFile("temp_puzzle_stencil_top.png");
//					else
//						st->initWithImageFile("puzzle_stencil_3_top.png");
//					
//					int cutx =puzzleWidth/2;
//					int cuty =puzzleHeight-st->getHeight()/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, cut_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				
//				//아래쪽 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					if(puzzle_number > 10000)
//						st->initWithImageFile("temp_puzzle_stencil_bottom.png");
//					else
//						st->initWithImageFile("puzzle_stencil_3_bottom.png");
//					
//					int cutx =puzzleWidth/2;
//					int cuty =st->getHeight()/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, cut_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				
//				//왼쪽 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					if(puzzle_number > 10000)
//						st->initWithImageFile("temp_puzzle_stencil_left.png");
//					else
//						st->initWithImageFile("puzzle_stencil_3_left.png");
//					
//					int cutx =st->getWidth()/2;
//					int cuty =puzzleHeight/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, cut_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				//오른쪽 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					if(puzzle_number > 10000)
//						st->initWithImageFile("temp_puzzle_stencil_right.png");
//					else
//						st->initWithImageFile("puzzle_stencil_3_right.png");
//					
//					int cutx =puzzleWidth-st->getWidth()/2;
//					int cuty =puzzleHeight/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, cut_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				//메모리해제
//				img->release();
//			}
//			
//			
//			// reduce
//			for(int i=0;i<cf_list.size();i++)
//			{
//				CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//				target_img->setAnchorPoint(ccp(0,0));
//				
//				if(cf_list[i].is_ani)
//				{
//					CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
//					ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
//					target_img->addChild(ani_img);
//				}
//				
//				target_img->setScale(0.2f);
//				
//				CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
//				t_texture->setSprite(target_img);
//				t_texture->begin();
//				t_texture->getSprite()->visit();
//				t_texture->end();
//				
//				t_texture->saveToFile(cf_list[i].to_filename.c_str(), kCCImageFormatPNG);
//			}
//			
//			
//			if(!df_list.empty())
//			{
//				for(int i=0;i<df_list.size();i++)
//				{
//					SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[i].key, df_list[i].img, false);
//				}
//			}
//			
//			if(!sf_list.empty())
//			{
//				for(int i=0;i<sf_list.size();i++)
//				{
//					SDS_SS(kSDF_cardInfo, sf_list[i].key, sf_list[i].img, false);
//				}
//				mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//			}
//			
//			for(int i=0;i<save_version_list.size();i++)
//			{
//				NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
//				mySDS->fFlush(save_version_list[i].x, kSDS_SI_autoBalanceTry_i);
//			}
//			
//			NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version, false);
//			mySDS->fFlush(puzzle_number, kSDS_PZ_base);
//			
//			addChild(KSTimer::create(0.3f, [=]()
//									 {
//										 if(success_func == nullptr)
//											 (target_success->*delegate_success)();
//										 else
//											 outOpenning();
//									 }));
//		}
//		else
//		{
//			startDownloadSet();
//		}
//	}
//}

//void StageListDown::failActionSet(string t_filename)
//{
//	CCLOG("download fail : %s", t_filename.c_str());
//	
//	rest_download_cnt--;
//	
//	if(rest_download_cnt <= 0)
//	{
//		startDownloadSet();
//	}
//}

void StageListDown::startDownloadSet()
{
	int max_thread_cnt = 5;
	int total_download_cnt = sf_list.size() + df_list.size();
	int before_download_size = download_set.size();
	
	for(int i=download_set.size();ing_download_cnt <= total_download_cnt && i<max_thread_cnt;i++)
	{
		DownloadImgInfo t_info;
		if(df_list.size() > 0 && ing_download_cnt <= df_list.size())
		{
			t_info.download_url = df_list[ing_download_cnt-1].img;
			t_info.download_size = df_list[ing_download_cnt-1].size;
			t_info.download_filename = df_list[ing_download_cnt-1].filename;
		}
		else if(sf_list.size() > 0 && ing_download_cnt <= df_list.size() + sf_list.size())
		{
			t_info.download_url = sf_list[ing_download_cnt-df_list.size()-1].img;
			t_info.download_size = sf_list[ing_download_cnt-df_list.size()-1].size;
			t_info.download_filename = sf_list[ing_download_cnt-df_list.size()-1].filename;
		}
		else
		{
			break;
		}
		
		t_info.is_fail = false;
		t_info.is_end = false;
		download_set.push_back(t_info);
		
		ing_download_cnt++;
	}
	
	if(download_set.size() > 0)
	{
		for(int i=before_download_size;i<download_set.size();i++)
		{
			int will_download_index = is_enable_index.back();
			CCLOG("start download idx : %d / filename : %s", will_download_index, download_set[i].download_filename.c_str());
			mySIL->downloadImg(download_set[i], will_download_index);
			is_enable_index.pop_back();
		}
		if(!is_downloading)
		{
			is_downloading = true;
			schedule(schedule_selector(StageListDown::checkDownloading));
		}
	}
	else
	{
		unschedule(schedule_selector(StageListDown::checkDownloading));
		
		// divide
		for(int j=0;j<cut_list.size();j++)
		{
			CCImage *img = new CCImage;
			img->initWithEncryptedImageFileFullPath((mySIL->getDocumentPath() + cut_list[j].filename).c_str()); //퍼즐이미지를 불러옵니다.
			
			CCImage *st_w = new CCImage;
			CCImage *st_h = new CCImage;
			
			int puzzleCol,puzzleRow;
			float puzzleColDis, puzzleRowDis, puzzleOffsetX, puzzleOffsetY;
			float faceColDis, faceRowDis; //172, 172
			float puzzleWidth,puzzleHeight;
			
			
			if(cut_list[j].key == "face")
			{
				if(puzzle_number > 10000)
				{
					st_w->initWithImageFile("stage_scissor.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("stage_scissor.png"); //피스조각(세로형)을 불러옵니다.
				}
				else
				{
					st_w->initWithImageFile("puzzle_stencil_1_pw.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("puzzle_stencil_1_ph.png"); //피스조각(세로형)을 불러옵니다.
				}
			}
			else
			{
				if(puzzle_number > 10000)
				{
					st_w->initWithImageFile("temp_puzzle_stencil_pw.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("temp_puzzle_stencil_ph.png"); //피스조각(세로형)을 불러옵니다.
				}
				else
				{
					st_w->initWithImageFile("puzzle_stencil_1_pw.png"); //피스조각(가로형)을 불러옵니다.
					st_h->initWithImageFile("puzzle_stencil_1_ph.png"); //피스조각(세로형)을 불러옵니다.
				}
			}
			
			if(puzzle_number > 10000)
			{
				puzzleCol=6;
				puzzleRow=4;
				puzzleColDis=100.f;
				puzzleRowDis=100.f;
				puzzleOffsetX=76.f;
				puzzleOffsetY=76.f;
				faceColDis=132.f;
				faceRowDis=132.f;
				puzzleWidth=652.f;
				puzzleHeight=452.f;
			}
			else
			{
				puzzleCol=5;
				puzzleRow=4;
				puzzleColDis=120.f;
				puzzleRowDis=120.f;
				puzzleOffsetX=92.f;
				puzzleOffsetY=92.f;
				faceColDis=154.f;
				faceRowDis=154.f;
				puzzleWidth=664;
				puzzleHeight=544;
			}
			
			
			for(int i=0;i<puzzleCol*puzzleRow;i++){
				//피스의 좌표를 구합니다. 퍼즐은 5*4 개로 이루어져있습니다.
				int x = i%puzzleCol;
				int y = i/puzzleCol;
				
				CCImage *st = st_h;
				if(puzzle_number > 10000)
				{
					if((x+(puzzleRow-1-y))%2 == 1)
						st=st_w;
				}
				else
				{
					if(i%2==0)
						st=st_w; //피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
				}
				
				//저장할파일명을 지정합니다.
				string filename;
				if(puzzle_number > 10000)
					filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, cut_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol)+1)->getCString();
				else
					filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, cut_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol))->getCString();
				
				//원본파일에서 자를 위치를 계산합니다.
				int cutx, cuty;
				if(cut_list[j].key == "face")
				{
					cutx = x*faceColDis+faceColDis/2.f;
					cuty = y*faceRowDis+faceRowDis/2.f;
				}
				else if(cut_list[j].key == "original" || cut_list[j].key == "center")
				{
					cutx =x*puzzleColDis+puzzleOffsetX;
					cuty =y*puzzleRowDis+puzzleOffsetY;
				}
				
				//자르고 저장합니다.
				bool isSuccess = PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				//실패했으면 한번더 자르게 해줍니다.
				if(!isSuccess){
					i--;
					continue;
				}
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			st_w->release(); //가로 피스 메모리해제
			st_h->release(); //세로 피스 메모리해제
			
			//가장자리 자르기
			//위쪽부터 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_top.png");
				else
					st->initWithImageFile("puzzle_stencil_3_top.png");
				
				int cutx =puzzleWidth/2;
				int cuty =puzzleHeight-st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			//아래쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_bottom.png");
				else
					st->initWithImageFile("puzzle_stencil_3_bottom.png");
				
				int cutx =puzzleWidth/2;
				int cuty =st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			//왼쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_left.png");
				else
					st->initWithImageFile("puzzle_stencil_3_left.png");
				
				int cutx =st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			//오른쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				if(puzzle_number > 10000)
					st->initWithImageFile("temp_puzzle_stencil_right.png");
				else
					st->initWithImageFile("puzzle_stencil_3_right.png");
				
				int cutx =puzzleWidth-st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, cut_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			//메모리해제
			img->release();
		}
		
//		if(cf_list.size() > 0)
//		{
//			reduce_frame = 0;
//			schedule(schedule_selector(StageListDown::reduceAction));
//		}
//		else
//		{
			if(!df_list.empty())
			{
				for(int i=0;i<df_list.size();i++)
				{
					SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[i].key, df_list[i].img, false);
				}
			}
			
			if(!sf_list.empty())
			{
				for(int i=0;i<sf_list.size();i++)
				{
					SDS_SS(kSDF_cardInfo, sf_list[i].key, sf_list[i].img, false);
				}
				mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
			}
			
			for(int i=0;i<save_version_list.size();i++)
			{
				NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
				mySDS->fFlush(save_version_list[i].x, kSDS_SI_autoBalanceTry_i);
			}
			
			NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version, false);
			mySDS->fFlush(puzzle_number, kSDS_PZ_base);
			
			addChild(KSTimer::create(0.3f, [=]()
									 {
										 if(success_func == nullptr)
											 (target_success->*delegate_success)();
										 else
											 outOpenning();
									 }));
//		}
		
//		// reduce
//		for(int i=0;i<cf_list.size();i++)
//		{
//			mySIL->removeTextureCache(cf_list[i].from_filename);
//			mySIL->removeTextureCache(cf_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
//			if(cf_list[i].is_ani)
//			{
//				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
//				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
//				target_img->addChild(ani_img);
//			}
//			
//			if(cf_list[i].is_ccb)
//			{
//				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + cf_list[i].ccb_filename.c_str()).first;
//				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
//				target_img->addChild(face_img);
//			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->beginWithClear(0, 0, 0, 0);
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(cf_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", cf_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
//		
//		
//		if(!df_list.empty())
//		{
//			for(int i=0;i<df_list.size();i++)
//			{
//				SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[i].key, df_list[i].img, false);
//			}
//		}
//		
//		if(!sf_list.empty())
//		{
//			for(int i=0;i<sf_list.size();i++)
//			{
//				SDS_SS(kSDF_cardInfo, sf_list[i].key, sf_list[i].img, false);
//			}
//			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//		}
//		
//		for(int i=0;i<save_version_list.size();i++)
//		{
//			NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
//			mySDS->fFlush(save_version_list[i].x, kSDS_SI_autoBalanceTry_i);
//		}
//		
//		NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version, false);
//		mySDS->fFlush(puzzle_number, kSDS_PZ_base);
//		
//		addChild(KSTimer::create(0.3f, [=]()
//								 {
//									 if(success_func == nullptr)
//										 (target_success->*delegate_success)();
//									 else
//										 outOpenning();
//								 }));
	}
}

void StageListDown::reduceAction()
{
//    float download_percent = 100.f*reduce_frame/cf_list.size();
//	if(download_percent > 100.f)
//		download_percent = 100.f;
//	download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
//    
//	loading_progress->setPercentage(download_percent);
//    
//    int i = reduce_frame;
//    
//    mySIL->removeTextureCache(cf_list[i].from_filename);
//    mySIL->removeTextureCache(cf_list[i].to_filename);
//    
//    CCSprite* target_img = new CCSprite();
//    target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//    target_img->setAnchorPoint(ccp(0,0));
//    
////    if(cf_list[i].is_ani)
////    {
////        CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
////        ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
////        target_img->addChild(ani_img);
////    }
////    
////    if(cf_list[i].is_ccb)
////    {
////        CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + cf_list[i].ccb_filename.c_str()).first;
////        face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////        target_img->addChild(face_img);
////    }
//    
//    target_img->setScale(0.4f);
//    
//    CCRenderTexture* t_texture = new CCRenderTexture();
//    t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//    t_texture->setSprite(target_img);
//    t_texture->beginWithClear(0, 0, 0, 0);
//    t_texture->getSprite()->visit();
//    t_texture->end();
//    
//    if(!(t_texture->saveToFileNoAlpha(cf_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//        CCLOG("failed!!! card reduce : %s", cf_list[i].to_filename.c_str());
//    
//    t_texture->release();
//    target_img->release();
//    
//    if(i % 3 == 0)
//    {
//        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//    }
//    
//    reduce_frame++;
//    
//    if(reduce_frame >= cf_list.size())
//    {
//        unschedule(schedule_selector(StageListDown::reduceAction));
//        
//        if(!df_list.empty())
//		{
//			for(int i=0;i<df_list.size();i++)
//			{
//				SDS_SS(kSDF_puzzleInfo, puzzle_number, df_list[i].key, df_list[i].img, false);
//			}
//		}
//		
//		if(!sf_list.empty())
//		{
//			for(int i=0;i<sf_list.size();i++)
//			{
//				SDS_SS(kSDF_cardInfo, sf_list[i].key, sf_list[i].img, false);
//			}
//			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//		}
//		
//		for(int i=0;i<save_version_list.size();i++)
//		{
//			NSDS_SI(save_version_list[i].x, kSDS_SI_version_i, save_version_list[i].y);
//			mySDS->fFlush(save_version_list[i].x, kSDS_SI_autoBalanceTry_i);
//		}
//		
//		NSDS_SI(puzzle_number, kSDS_PZ_version_i, download_version, false);
//		mySDS->fFlush(puzzle_number, kSDS_PZ_base);
//		
//		addChild(KSTimer::create(0.3f, [=]()
//								 {
//									 if(success_func == nullptr)
//										 (target_success->*delegate_success)();
//									 else
//										 outOpenning();
//								 }));
//    }
}

void StageListDown::checkDownloading()
{
	int max_thread_cnt = 5;
	bool is_successed = false;
	for(int i=0;i<max_thread_cnt;i++)
	{
		if(mySIL->downloading_list[i].is_end)
		{
			if(mySIL->downloading_list[i].is_fail)
			{
				CCLOG("fail download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					CCLOG("start download idx : %d / filename : %s", i, iter->download_filename.c_str());
					mySIL->downloadImg((*iter), i);
				}
				else
				{
					CCLOG("not found fail set");
				}
			}
			else
			{
				CCLOG("success download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					download_set.erase(iter);
				}
				else
				{
					CCLOG("not found success set");
				}
				is_enable_index.push_back(i);
				success_download_cnt++;
				is_successed = true;
			}
		}
	}
	
	float download_percent = 100.f*success_download_cnt/int(df_list.size() + sf_list.size());
	if(download_percent > 100.f)
		download_percent = 100.f;
	download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());

	loading_progress->setPercentage(download_percent);
//	loading_progress->stopAllActions();
//	CCProgressFromTo* t_to = CCProgressFromTo::create(0.5f, loading_progress->getPercentage(), download_percent);
//	loading_progress->runAction(t_to);
	
	if(is_successed || success_download_cnt >= df_list.size() + sf_list.size())
	{
		startDownloadSet();
	}
}

StageListDown* StageListDown::create( CCObject* t_success, SEL_CallFunc d_success, int t_puzzle, function<void(function<void()>)> t_download_start, function<void()> t_success_func )
{
	StageListDown* t_sid = new StageListDown();
	t_sid->myInit(t_success, d_success, t_puzzle, t_download_start, t_success_func);
	t_sid->autorelease();
	return t_sid;
}

void StageListDown::myInit( CCObject* t_success, SEL_CallFunc d_success, int t_puzzle, function<void(function<void()>)> t_download_start, function<void()> t_success_func )
{
	download_start = t_download_start;
	success_func = t_success_func;
	
	loading_progress = NULL;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	gray->setVisible(false);
	addChild(gray);
	
	setTouchEnabled(true);
	
	puzzle_number = t_puzzle;
	target_success = t_success;
	delegate_success = d_success;

	state_ment = KSLabelTTF::create("", mySGD->getFont().c_str(), 20);
	state_ment->setAnchorPoint(ccp(0.5,0.5));
	state_ment->setPosition(ccp(240,160));
	state_ment->setHorizontalAlignment(kCCTextAlignmentCenter);
	state_ment->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	state_ment->setVisible(false);
	addChild(state_ment, kSLD_Z_content);

	is_downloading = false;

	startGetStageList();
}

void StageListDown::changeTipImage()
{
	tip_img->removeFromParent();
	
	tip_img = LoadingTipScene::getLoadingTipImage();
	tip_img->setPosition(ccp(240,160));
	addChild(tip_img, kSLD_Z_back);
	
	CCDelayTime* t_delay = CCDelayTime::create(7);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StageListDown::changeTipImage));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	
	tip_img->runAction(t_seq);
}

void StageListDown::startGetStageList()
{
	myLog->addLog(kLog_getPuzzleInfo_i, -1, puzzle_number);

	vector<CommandParam> command_vector;
	
//	Json::Value rank_param;
//	rank_param["memberIDList"].append(hspConnector::get()->myKakaoInfo["user_id"].asString());
//	for(auto i : KnownFriends::getInstance()->getFriends())
//		rank_param["memberIDList"].append(i.userId);
//	for(auto i : UnknownFriends::getInstance()->getFriends())
//		rank_param["memberIDList"].append(i.userId);
//	
//	int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
//	int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
//	for(int i=start_stage;i<start_stage+stage_count;i++)
//		rank_param["stageNoList"].append(i);
//	
//	rank_param["limit"] = 1;
//	command_vector.push_back(CommandParam("getstageranklist", rank_param, json_selector(this, StageListDown::resultGetStageRankList)));
	
	Json::Value param;
	param["version"] = NSDS_GI(puzzle_number, kSDS_PZ_version_i);
	param["order"] = puzzle_number;
	command_vector.push_back(CommandParam("getpuzzleinfo", param, json_selector(this, StageListDown::resultGetStageList)));
//	hspConnector::get()->command("getpuzzleinfo", param, json_selector(this, StageListDown::resultGetStageList));
	hspConnector::get()->command(command_vector);
}

void StageListDown::resultGetStageRankList(Json::Value result_data)
{
//	CCLOG("resultGetStageRankList : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
//	mySGD->temp_stage_ranker_list = result_data;
}