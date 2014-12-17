//
//  StageInfoDown.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 24..
//
//

#include "StageInfoDown.h"
#include "DataStorageHub.h"
#include "CommonButton.h"


void StageInfoDown::resultGetStageInfo(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		NSDS_SI(mySD->getSilType(), kSDS_SI_puzzle_i, result_data["puzzle"].asInt(), false);
		NSDS_SI(mySD->getSilType(), kSDS_SI_playtime_i, result_data["playtime"].asInt(), false);
		NSDS_SD(mySD->getSilType(), kSDS_SI_scoreRate_d, result_data["scoreRate"].asDouble(), false);
		NSDS_SD(mySD->getSilType(), kSDS_SI_scale_d, result_data["scale"].asDouble(), false);
//		NSDS_SB(mySD->getSilType(), kSDS_SI_minigame_b, result_data["minigame"].asBool(), false);
		NSDS_SS(mySD->getSilType(), kSDS_SI_type_s, result_data["type"].asString(), false);
		
		Json::Value t_mission = result_data["mission"];
		NSDS_SI(mySD->getSilType(), kSDS_SI_missionType_i, t_mission["type"].asInt(), false);
		
		Json::Value t_option;
		if(!t_mission["option"].isObject())			t_option["key"]="value";
		else										t_option =t_mission["option"];
		
		if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionEnergy_i, t_option["energy"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_bigArea)
		{
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
		}
		else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_perfect)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_hellMode)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_percentage)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_score)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["score"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_combo)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["combo"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_gold)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["gold"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_turns)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["turns"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_casting)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
		else if(t_mission["type"].asInt() == kCLEAR_littlePercent)
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
		
		
		Json::Value shopItems = result_data["shopItems"];
		NSDS_SI(mySD->getSilType(), kSDS_SI_shopItemsCnt_i, shopItems.size(), false);
		for(int i=0;i<shopItems.size();i++)
		{
			Json::Value t_item = shopItems[i];
			NSDS_SS(mySD->getSilType(), kSDS_SI_shopItems_int1_currency_s, i, t_item["currency"].asString(), false);
			NSDS_SI(mySD->getSilType(), kSDS_SI_shopItems_int1_type_i, i, t_item["type"].asInt(), false);
			NSDS_SI(mySD->getSilType(), kSDS_SI_shopItems_int1_price_i, i, t_item["price"].asInt(), false);
			
			
			Json::Value t_option;
			if(!t_item["option"].isObject())				t_option["key"]="value";
			else											t_option =t_item["option"];
			
			if(t_item["type"].asInt() == kIC_attack)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAttackPower_i, t_option.get("power",0).asInt(), false);
			else if(t_item["type"].asInt() == kIC_addTime)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAddTimeSec_i, t_option.get("sec",0).asInt(), false);
			else if(t_item["type"].asInt() == kIC_fast)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionFastSec_i, t_option.get("sec",0).asInt(), false);
			else if(t_item["type"].asInt() == kIC_silence)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSilenceSec_i, t_option.get("sec",0).asInt(), false);
			else if(t_item["type"].asInt() == kIC_doubleItem)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_longTime)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_baseSpeedUp)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
		}
		
		Json::Value defItems = result_data["defItems"];
		NSDS_SI(mySD->getSilType(), kSDS_SI_defItemsCnt_i, defItems.size(), false);
		for(int i=0;i<defItems.size();i++)
		{
			Json::Value t_item = defItems[i];
			NSDS_SI(mySD->getSilType(), kSDS_SI_defItems_int1_type_i, i, t_item["type"].asInt(), false);
			
			Json::Value t_option;
			if(!t_item["option"].isObject())				t_option["key"]="value";
			else											t_option =t_item["option"];
			
			if(t_item["type"].asInt() == kIC_attack)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAttackPower_i, t_option["power"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_addTime)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAddTimeSec_i, t_option["sec"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_fast)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionFastSec_i, t_option["sec"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_silence)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSilenceSec_i, t_option["sec"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_doubleItem)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_longTime)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
			else if(t_item["type"].asInt() == kIC_baseSpeedUp)
				NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
		}
		
		Json::Value cards = result_data["cards"];
		NSDS_SI(mySD->getSilType(), kSDS_SI_cardCount_i, cards.size(), false);
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
				continue;
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			
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
				DownloadFile t_df;
				t_df.size = t_imgInfo["size"].asInt();
				t_df.img = t_imgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
				
//				CopyFile t_cf;
//				t_cf.from_filename = t_df.filename.c_str();
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
					DownloadFile t_df;
					t_df.size = t_detail["size"].asInt();
					t_df.img = t_detail["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
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
					DownloadFile t_df;
					t_df.size = t_silImgInfo["size"].asInt();
					t_df.img = t_silImgInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
			}
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && t_faceInfo.asString() != " ")
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
				{
					DownloadFile t_df1;
					t_df1.size = t_faceInfo["size"].asInt();
					t_df1.img = t_faceInfo["ccbi"].asString().c_str();
					t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
					t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df1);
				}
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
				{
					DownloadFile t_df2;
					t_df2.size = t_faceInfo["size"].asInt();
					t_df2.img = t_faceInfo["plist"].asString().c_str();
					t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
					t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df2);
				}
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
				{
					DownloadFile t_df3;
					t_df3.size = t_faceInfo["size"].asInt();
					t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
					t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
					t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df3);
				}
				
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
			else
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
			}
		}
		
//		NSDS_SI(mySD->getSilType(), kSDS_SI_level_i, result_data["level"].asInt(), false);
		
		NSDS_SS(mySD->getSilType(), kSDS_SI_boss_s, result_data["boss"].asString(), false);
		NSDS_SS(mySD->getSilType(), kSDS_SI_junior_s, result_data["junior"].asString(), false);
		NSDS_SI(mySD->getSilType(), kSDS_SI_autoBalanceTry_i, result_data["autoBalanceTry"].asInt(), false);
		
		if(df_list.size() > 0) // need download
		{
			download_version = result_data["version"].asInt();
			state_ment->setString("이미지 정보를 다운로드 합니다.");
			ing_download_cnt = 1;
			ing_download_per = 0;
			download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString(), "allfont.fnt");
			download_state->setPosition(ccp(240,130));
			addChild(download_state, kSID_Z_content);
			is_downloading = true;
			startDownload();
		}
		else
		{
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "version", result_data["version"].asInt(), false);
			mySDS->fFlush(mySD->getSilType(), kSDS_SI_scale_d);
			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
			state_ment->setString("스테이지 정보 확인 완료.");
			removeFromParent();
			(target_success->*delegate_success)();
			
		}
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		state_ment->setString("스테이지 정보 확인 완료.");
		removeFromParent();
		(target_success->*delegate_success)();
	}
	else
	{
		state_ment->setString("스테이지 정보를 받아오는데 실패하였습니다.");
	}
}

void StageInfoDown::successAction()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
	unschedule(schedule_selector(StageInfoDown::downloadingAction));
	
	if(ing_download_cnt >= df_list.size())
	{
		state_ment->setString("카드 섬네일 만드는 중...");
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
//			t_texture->begin();
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
		
		NSDS_SI(mySD->getSilType(), kSDS_SI_version_i, download_version, false);
		mySDS->fFlush(mySD->getSilType(), kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		state_ment->setString("이미지 정보 다운로드 완료.");
		removeFromParent();
		(target_success->*delegate_success)();
		
//		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		startDownload();
	}
}

void StageInfoDown::failAction()
{
	unschedule(schedule_selector(StageInfoDown::downloadingAction));
	state_ment->setString("이미지 정보 다운로드에 실패하였습니다.");
}

void StageInfoDown::downloadingAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	
	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;
	
	ing_download_per = t_per;
	
	download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
}

void StageInfoDown::startDownload()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(StageInfoDown::successAction), this, callfunc_selector(StageInfoDown::failAction));
	
	schedule(schedule_selector(StageInfoDown::downloadingAction));
}

StageInfoDown* StageInfoDown::create( CCObject* t_success, SEL_CallFunc d_success, CCObject* t_cancel, SEL_CallFunc d_cancel )
{
	StageInfoDown* t_sid = new StageInfoDown();
	t_sid->myInit(t_success, d_success, t_cancel, d_cancel);
	t_sid->autorelease();
	return t_sid;
}

void StageInfoDown::myInit( CCObject* t_success, SEL_CallFunc d_success, CCObject* t_cancel, SEL_CallFunc d_cancel )
{
	target_success = t_success;
	delegate_success = d_success;
	target_cancel = t_cancel;
	delegate_cancel = d_cancel;

	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);

	cancel_menu = CommonButton::createCloseButton(-510);
	cancel_menu->setPosition(ccp(350,240));
	cancel_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kSID_MT_cancel);
								 menuAction(t_node);
							 });
	addChild(cancel_menu, kSID_Z_content);

	state_ment = CCLabelTTF::create("스테이지 정보를 받아오는 ing...", mySGD->getFont().c_str(), 20);
	state_ment->setAnchorPoint(ccp(0.5,0.5));
	state_ment->setPosition(ccp(240,160));
	state_ment->setHorizontalAlignment(kCCTextAlignmentCenter);
	state_ment->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	addChild(state_ment, kSID_Z_content);

	is_downloading = false;

//	int selected_friend_card_number = mySGD->getSelectedFriendCardData().card_number;
//	if(selected_friend_card_number != 0 && NSDS_GS(kSDS_CI_int1_imgInfo_s, selected_friend_card_number) == "")
//	{
//		startGetCardsInfo();
//	}
//	else
//	{
		startGetStageInfo();
//	}

	touch_number = 0;
	is_menu_enable = true;
	setTouchEnabled(true);
}

void StageInfoDown::startGetCardsInfo()
{
//	myLog->addLog(kLOG_network_loadCardData, -1);
//	
//	Json::Value param;
//	param["noList"][0] = mySGD->getSelectedFriendCardData().card_number;
//	hspConnector::get()->command("getcardlist", param, json_selector(this, StageInfoDown::resultLoadedCardData));
}

void StageInfoDown::resultLoadedCardData( Json::Value result_data )
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
				continue;
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			
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
				DownloadFile t_df;
				t_df.size = t_imgInfo["size"].asInt();
				t_df.img = t_imgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
				
//				CopyFile t_cf;
//				t_cf.from_filename = t_df.filename.c_str();
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
					DownloadFile t_df;
					t_df.size = t_detail["size"].asInt();
					t_df.img = t_detail["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
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
					DownloadFile t_df;
					t_df.size = t_silImgInfo["size"].asInt();
					t_df.img = t_silImgInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
			}
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && t_faceInfo.asString() != " ")
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
				{
					DownloadFile t_df1;
					t_df1.size = t_faceInfo["size"].asInt();
					t_df1.img = t_faceInfo["ccbi"].asString().c_str();
					t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
					t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df1);
				}
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
				{
					DownloadFile t_df2;
					t_df2.size = t_faceInfo["size"].asInt();
					t_df2.img = t_faceInfo["plist"].asString().c_str();
					t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
					t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df2);
				}
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
				{
					DownloadFile t_df3;
					t_df3.size = t_faceInfo["size"].asInt();
					t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
					t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
					t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df3);
				}
				
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
			else
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
			}
		}
		
		if(df_list.size() > 0) // need download
		{
			startDownloadCardImage();
		}
		else
		{
			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
			startGetStageInfo();
		}
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		startGetStageInfo();
	}
	else
	{
		state_ment->setString("이미지 정보 다운로드에 실패하였습니다.");
	}
}

void StageInfoDown::startDownloadCardImage()
{
	myLog->addLog(kLOG_network_downloadCardImg, -1);
	
	ing_download_cnt = 1;
	ing_download_per = 0;
	is_downloading = true;
	startDownload2();
}

void StageInfoDown::startDownload2()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(StageInfoDown::successAction2), this, callfunc_selector(StageInfoDown::failAction2));
}

void StageInfoDown::successAction2()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
	
	if(ing_download_cnt >= df_list.size())
	{
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
//			t_texture->begin();
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
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		df_list.clear();
//		cf_list.clear();
		
		startGetStageInfo();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
		startDownload2();
	}
}

void StageInfoDown::failAction2()
{
	state_ment->setString("이미지 정보 다운로드에 실패하였습니다.");
}

void StageInfoDown::startGetStageInfo()
{
	int stage_number = mySD->getSilType();

	myLog->addLog(kLOG_getStageInfo_i, -1, stage_number);

	Json::Value param;
	param["no"] = stage_number;
	param["version"] = NSDS_GI(stage_number, kSDS_SI_version_i);
	hspConnector::get()->command("getpieceinfo", param, json_selector(this, StageInfoDown::resultGetStageInfo));
}

void StageInfoDown::menuAction( CCObject* sender )
{
	if(!is_menu_enable)	return;

	int tag = ((CCNode*)sender)->getTag();

	is_menu_enable = false;

	if(tag == kSID_MT_cancel)
	{
		graphdog->removeCommand(this);
		if(is_downloading)
			StageImgLoader::sharedInstance()->removeTD();
		(target_cancel->*delegate_cancel)();
		removeFromParent();
	}
}
