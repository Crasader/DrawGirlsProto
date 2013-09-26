//
//  StageInfoDown.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 24..
//
//

#include "StageInfoDown.h"
#include "StageSettingScene.h"


void StageInfoDown::resultGetStageInfo(JsonBox::Object result_data)
{
	if(result_data["state"].getString() == "ok")
	{
		if(SDS_GI(kSDF_stageInfo, mySD->getSilType(), "version") < result_data["version"].getInt())
		{
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "playtime", result_data["playtime"].getInt());
			
			JsonBox::Object t_mission = result_data["mission"].getObject();
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_type", t_mission["type"].getInt());
			
			if(t_mission["type"].getInt() == kCLEAR_bossLifeZero)
			{
				JsonBox::Object t_option = t_mission["option"].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_energy", t_option["energy"].getInt());
			}
			else if(t_mission["type"].getInt() == kCLEAR_subCumberCatch)
			{
				JsonBox::Object t_option = t_mission["option"].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_count", t_option["count"].getInt());
			}
			else if(t_mission["type"].getInt() == kCLEAR_bigArea)
			{
				JsonBox::Object t_option = t_mission["option"].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_percent", t_option["percent"].getInt());
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_count", t_option["count"].getInt());
			}
			else if(t_mission["type"].getInt() == kCLEAR_itemCollect)
			{
				JsonBox::Object t_option = t_mission["option"].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_count", t_option["count"].getInt());
			}
			else if(t_mission["type"].getInt() == kCLEAR_perfect)
			{
				JsonBox::Object t_option = t_mission["option"].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_percent", t_option["percent"].getInt());
			}
			else if(t_mission["type"].getInt() == kCLEAR_timeLimit)
			{
				JsonBox::Object t_option = t_mission["option"].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_sec", t_option["sec"].getInt());
			}
			
			
			JsonBox::Array shopItems = result_data["shopItems"].getArray();
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "shopItems_cnt", shopItems.size());
			for(int i=0;i<shopItems.size();i++)
			{
				JsonBox::Object t_item = shopItems[i].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("shopItems_%d_type", i)->getCString(), t_item["type"].getInt());
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("shopItems_%d_price", i)->getCString(), t_item["price"].getInt());
				
				if(t_item["type"].getInt() == kIC_attack)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_attack_power", t_option["power"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_addTime)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_addTime_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_fast)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_fast_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_silence)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_silence_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_doubleItem)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_doubleItem_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_longTime)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_longTime_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_bossLittleEnergy)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_bossLittleEnergy_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_subSmallSize)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_subSmallSize_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_smallArea)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_smallArea_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_widePerfect)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_widePerfect_percent", t_option["percent"].getInt());
				}
			}
			
			JsonBox::Array defItems = result_data["defItems"].getArray();
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "defItems_cnt", defItems.size());
			for(int i=0;i<defItems.size();i++)
			{
				JsonBox::Object t_item = defItems[i].getObject();
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("defItems_%d_type", i)->getCString(), t_item["type"].getInt());
				
				if(t_item["type"].getInt() == kIC_attack)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_attack_power", t_option["power"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_addTime)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_addTime_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_fast)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_fast_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_silence)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_silence_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_doubleItem)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_doubleItem_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_longTime)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_longTime_sec", t_option["sec"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_bossLittleEnergy)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_bossLittleEnergy_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_subSmallSize)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_subSmallSize_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_smallArea)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_smallArea_percent", t_option["percent"].getInt());
				}
				else if(t_item["type"].getInt() == kIC_widePerfect)
				{
					JsonBox::Object t_option = t_item["option"].getObject();
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_widePerfect_percent", t_option["percent"].getInt());
				}
			}
			
			JsonBox::Array cards = result_data["cards"].getArray();
			for(int i=0;i<cards.size();i++)
			{
				JsonBox::Object t_card = cards[i].getObject();
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_rank", t_card["no"].getInt())->getCString(), t_card["rank"].getInt());
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_durability", t_card["no"].getInt())->getCString(), t_card["durability"].getInt());
				
				JsonBox::Array t_ability = t_card["ability"].getArray();
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_cnt", t_card["no"].getInt())->getCString(), t_ability.size());
				for(int j=0;j<t_ability.size();j++)
				{
					JsonBox::Object t_abil = t_ability[j].getObject();
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_%d_type", t_card["no"].getInt(), t_abil["type"].getInt())->getCString(), t_abil["type"].getInt());
					
					if(t_abil["type"].getInt() == kIC_attack)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_attack_option_power", t_card["no"].getInt())->getCString(), t_option["power"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_addTime)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_addTime_option_sec", t_card["no"].getInt())->getCString(), t_option["sec"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_fast)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_fast_option_sec", t_card["no"].getInt())->getCString(), t_option["sec"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_silence)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_silence_option_sec", t_card["no"].getInt())->getCString(), t_option["sec"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_doubleItem)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_doubleItem_option_percent", t_card["no"].getInt())->getCString(), t_option["percent"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_longTime)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_longTime_option_sec", t_card["no"].getInt())->getCString(), t_option["sec"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_bossLittleEnergy)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_bossLittleEnergy_option_percent", t_card["no"].getInt())->getCString(), t_option["percent"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_subSmallSize)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_subSmallSize_option_percent", t_card["no"].getInt())->getCString(), t_option["percent"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_smallArea)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_smallArea_option_percent", t_card["no"].getInt())->getCString(), t_option["percent"].getInt());
					}
					else if(t_abil["type"].getInt() == kIC_widePerfect)
					{
						JsonBox::Object t_option = t_abil["option"].getObject();
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_widePerfect_option_percent", t_card["no"].getInt())->getCString(), t_option["percent"].getInt());
					}
				}
				
				JsonBox::Object t_imgInfo = t_card["imgInfo"].getObject();
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_imgInfo_size", t_card["no"].getInt())->getCString(), t_imgInfo["size"].getInt());
				
				if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_imgInfo_img", t_card["no"].getInt())->getCString()) != t_imgInfo["img"].getString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_imgInfo["size"].getInt();
					t_df.img = t_imgInfo["img"].getString().c_str();
					t_df.filename = CCSTR_CWF("stage%d_level%d_visible.png", mySD->getSilType(), i+1)->getCString();
					t_df.key = CCSTR_CWF("%d_imgInfo_img", t_card["no"].getInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				JsonBox::Object t_thumbnailInfo = t_card["thumbnailInfo"].getObject();
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_thumbnailInfo_size", t_card["no"].getInt())->getCString(), t_thumbnailInfo["size"].getInt());
				
				if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_thumbnailInfo_img", t_card["no"].getInt())->getCString()) != t_thumbnailInfo["img"].getString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_thumbnailInfo["size"].getInt();
					t_df.img = t_thumbnailInfo["img"].getString().c_str();
					t_df.filename = CCSTR_CWF("stage%d_level%d_thumbnail.png", mySD->getSilType(), i+1)->getCString();
					t_df.key = CCSTR_CWF("%d_thumbnailInfo_img", t_card["no"].getInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				JsonBox::Object t_aniInfo = t_card["aniInfo"].getObject();
				SDS_SB(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_isAni", t_card["no"].getInt())->getCString(), t_aniInfo["isAni"].getBoolean());
				if(t_aniInfo["isAni"].getBoolean())
				{
					JsonBox::Object t_detail = t_aniInfo["detail"].getObject();
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_loopLength", t_card["no"].getInt())->getCString(), t_detail["loopLength"].getInt());
					
					JsonBox::Array t_loopSeq = t_detail["loopSeq"].getArray();
					for(int j=0;j<t_loopSeq.size();j++)
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_loopSeq_%d", t_card["no"].getInt(), j)->getCString(), t_loopSeq[j].getInt());
					
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_cutWidth", t_card["no"].getInt())->getCString(), t_detail["cutWidth"].getInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_cutHeight", t_card["no"].getInt())->getCString(), t_detail["cutHeight"].getInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_cutLength", t_card["no"].getInt())->getCString(), t_detail["cutLength"].getInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_positionX", t_card["no"].getInt())->getCString(), t_detail["positionX"].getInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_positionY", t_card["no"].getInt())->getCString(), t_detail["positionY"].getInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_size", t_card["no"].getInt())->getCString(), t_detail["size"].getInt());
					
					if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].getInt())->getCString()) != t_detail["img"].getString())
					{
						// check, after download ----------
						DownloadFile t_df;
						t_df.size = t_detail["size"].getInt();
						t_df.img = t_detail["img"].getString().c_str();
						t_df.filename = CCSTR_CWF("stage%d_level%d_animation.png", mySD->getSilType(), i+1)->getCString();
						t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].getInt())->getCString();
						df_list.push_back(t_df);
						// ================================
					}
				}
				
				SDS_SS(kSDF_cardInfo, CCSTR_CWF("%d_script", t_card["no"].getInt())->getCString(), t_card["script"].getString());
				
				JsonBox::Object t_silImgInfo = t_card["silImgInfo"].getObject();
				SDS_SB(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_isSil", t_card["no"].getInt())->getCString(), t_silImgInfo["isSil"].getBoolean());
				if(t_silImgInfo["isSil"].getBoolean())
				{
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_size", t_card["no"].getInt())->getCString(), t_silImgInfo["size"].getInt());
					if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_img", t_card["no"].getInt())->getCString()) != t_silImgInfo["img"].getString())
					{
						// check, after download ----------
						DownloadFile t_df;
						t_df.size = t_silImgInfo["size"].getInt();
						t_df.img = t_silImgInfo["img"].getString().c_str();
						t_df.filename = CCSTR_CWF("stage%d_level%d_invisible.png", mySD->getSilType(), i+1)->getCString();
						t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].getInt())->getCString();
						df_list.push_back(t_df);
						// ================================
					}
					SDS_SS(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_silData", t_card["no"].getInt())->getCString(), t_silImgInfo["silData"].getString());
				}
			}
			
			SDS_SS(kSDF_stageInfo, mySD->getSilType(), "boss", result_data["boss"].getString());
			SDS_SS(kSDF_stageInfo, mySD->getSilType(), "junior", result_data["junior"].getString());
			
//			JsonBox::Array bosss = result_data["boss"].getArray();
//			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "boss_cnt", bosss.size());
//			for(int i=0;i<bosss.size();i++)
//			{
//				JsonBox::Object t_boss = bosss[i].getObject();
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_type", i)->getCString(), t_boss["type"].getInt());
//				
//				JsonBox::Object t_pattern = t_boss["pattern"].getObject();
//				JsonBox::Array t_missile = t_pattern["missile"].getArray();
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_missile_cnt", i)->getCString(), t_missile.size());
//				for(int j=0;j<t_missile.size();j++)
//				{
//					JsonBox::Object t_m = t_missile[j].getObject();
//					SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_missile_%d_type", i, j)->getCString(), t_m["type"].getInt());
//					SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_missile_%d_percent", i, j)->getCString(), t_m["percent"].getInt());
//					
//					if(t_m["type"] == 0) // enum
//					{
//						// option
//					}
//				}
//				
//				JsonBox::Array t_destroy = t_pattern["destroy"].getArray();
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_destroy_cnt", i)->getCString(), t_destroy.size());
//				for(int j=0;j<t_destroy.size();j++)
//				{
//					JsonBox::Object t_d = t_destroy[j].getObject();
//					SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_destroy_%d_type", i, j)->getCString(), t_d["type"].getInt());
//					SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_destroy_%d_percent", i, j)->getCString(), t_d["percent"].getInt());
//					
//					if(t_d["type"] == 0) // enum
//					{
//						// option
//					}
//				}
//				
//				JsonBox::Array t_special = t_pattern["special"].getArray();
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_spacial_cnt", i)->getCString(), t_special.size());
//				for(int j=0;j<t_special.size();j++)
//				{
//					JsonBox::Object t_s = t_destroy[j].getObject();
//					SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_special_%d_type", i, j)->getCString(), t_s["type"].getInt());
//					SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_pattern_special_%d_percent", i, j)->getCString(), t_s["percent"].getInt());
//					
//					if(t_s["type"] == 0) // enum
//					{
//						// option
//					}
//				}
//				
//				JsonBox::Object t_speed = t_boss["speed"].getObject();
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_speed_max", i)->getCString(), t_speed["max"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_speed_start", i)->getCString(), t_speed["start"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_speed_min", i)->getCString(), t_speed["min"].getDouble());
//				
//				JsonBox::Object t_scale = t_boss["scale"].getObject();
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_scale_max", i)->getCString(), t_scale["max"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_scale_start", i)->getCString(), t_scale["start"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_scale_min", i)->getCString(), t_scale["min"].getDouble());
//				
//				JsonBox::Object t_movement = t_boss["movement"].getObject();
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_movement_normal", i)->getCString(), t_movement["normal"].getInt());
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_movement_draw", i)->getCString(), t_movement["draw"].getInt());
//				
//				
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_attackTerm", i)->getCString(), t_boss["attackTerm"].getInt());
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("boss_%d_hp", i)->getCString(), t_boss["hp"].getInt());
//			}
			
//			JsonBox::Array juniors = result_data["junior"].getArray();
//			for(int i=0;i<juniors.size();i++)
//			{
//				JsonBox::Object t_junior = juniors[i].getObject();
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_type", i)->getCString(), t_junior["type"].getInt());
//				
//				JsonBox::Object t_speed = t_junior["speed"].getObject();
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_speed_max", i)->getCString(), t_speed["max"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_speed_start", i)->getCString(), t_speed["start"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_speed_min", i)->getCString(), t_speed["min"].getDouble());
//				
//				JsonBox::Object t_scale = t_junior["scale"].getObject();
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_scale_max", i)->getCString(), t_scale["max"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_scale_start", i)->getCString(), t_scale["start"].getDouble());
//				SDS_SD(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_scale_min", i)->getCString(), t_scale["min"].getDouble());
//				
//				JsonBox::Object t_movement = t_junior["movement"].getObject();
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_movement_normal", i)->getCString(), t_movement["normal"].getInt());
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_movement_draw", i)->getCString(), t_movement["draw"].getInt());
//				
//				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("junior_%d_hp", i)->getCString(), t_junior["hp"].getInt());
//			}
			
			if(df_list.size() > 0) // need download
			{
				download_version = result_data["version"].getInt();
				state_ment->setString("이미지 정보를 다운로드 합니다.");
				ing_download_cnt = 1;
				ing_download_per = 0;
				download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString(), "etc_font.fnt");
				download_state->setPosition(ccp(240,130));
				addChild(download_state, kSID_Z_content);
				is_downloading = true;
				startDownload();
			}
			else
			{
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "version", result_data["version"].getInt());
				state_ment->setString("스테이지 정보 확인 완료.");
				
				CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
			}
		}
		else
		{
			state_ment->setString("스테이지 정보 확인 완료.");
			
			CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
		}
	}
	else
	{
		state_ment->setString("스테이지 정보를 받아오는데 실패하였습니다.");
	}
}

void StageInfoDown::successAction()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
	
	if(ing_download_cnt >= df_list.size())
	{
		SDS_SI(kSDF_stageInfo, mySD->getSilType(), "version", download_version);
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		state_ment->setString("이미지 정보 다운로드 완료.");
		
		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
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
	CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(StageInfoDown::successAction), this, callfunc_selector(StageInfoDown::failAction));
	
	schedule(schedule_selector(StageInfoDown::downloadingAction));
}


bool StageInfoDown::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(touch_number != 0)	return true;
	
	if(cancel_menu->ccTouchBegan(pTouch, pEvent))	touch_number = kSID_MT_cancel;
	
	return true;
}
void StageInfoDown::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(touch_number == kSID_MT_cancel)		cancel_menu->ccTouchMoved(pTouch, pEvent);
}
void StageInfoDown::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(touch_number == kSID_MT_cancel)
	{
		cancel_menu->ccTouchEnded(pTouch, pEvent);
		touch_number = 0;
	}
}
void StageInfoDown::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(touch_number == kSID_MT_cancel)
	{
		cancel_menu->ccTouchCancelled(pTouch, pEvent);
		touch_number = 0;
	}
}

void StageInfoDown::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
}