//
//  StageInfoDown.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 24..
//
//

#include "StageInfoDown.h"
#include "StageSettingScene.h"


void StageInfoDown::resultGetStageInfo(Json::Value result_data)
{
	CCLog("result data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["state"].asString() == "ok")
	{
		CCLog("saved version : %d", SDS_GI(kSDF_stageInfo, mySD->getSilType(), "version"));
		if(SDS_GI(kSDF_stageInfo, mySD->getSilType(), "version") < result_data["version"].asInt())
		{
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "playtime", result_data["playtime"].asInt());
			
			Json::Value t_mission = result_data["mission"];
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_type", t_mission["type"].asInt());
			
			if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
			{
				Json::Value t_option = t_mission["option"];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_energy", t_option["energy"].asInt());
			}
			else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
			{
				Json::Value t_option = t_mission["option"];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_count", t_option["count"].asInt());
			}
			else if(t_mission["type"].asInt() == kCLEAR_bigArea)
			{
				Json::Value t_option = t_mission["option"];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_percent", t_option["percent"].asInt());
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_count", t_option["count"].asInt());
			}
			else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
			{
				Json::Value t_option = t_mission["option"];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_count", t_option["count"].asInt());
			}
			else if(t_mission["type"].asInt() == kCLEAR_perfect)
			{
				Json::Value t_option = t_mission["option"];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_percent", t_option["percent"].asInt());
			}
			else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
			{
				Json::Value t_option = t_mission["option"];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "mission_option_sec", t_option["sec"].asInt());
			}
			
			
			Json::Value shopItems = result_data["shopItems"];
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "shopItems_cnt", shopItems.size());
			for(int i=0;i<shopItems.size();i++)
			{
				Json::Value t_item = shopItems[i];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("shopItems_%d_type", i)->getCString(), t_item["type"].asInt());
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("shopItems_%d_price", i)->getCString(), t_item["price"].asInt());
				
                
                Json::Value t_option;
                if(!t_item["option"].isObject()){
                    t_option["key"]="value";
                }else{
                    t_option =t_item["option"];
                }
                
				if(t_item["type"].asInt() == kIC_attack)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_attack_power", t_option.get("power",0).asInt());
				}
				else if(t_item["type"].asInt() == kIC_addTime)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_addTime_sec", t_option.get("sec",0).asInt());
				}
				else if(t_item["type"].asInt() == kIC_fast)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_fast_sec", t_option.get("sec",0).asInt());
				}
				else if(t_item["type"].asInt() == kIC_silence)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_silence_sec", t_option.get("sec",0).asInt());
				}
				else if(t_item["type"].asInt() == kIC_doubleItem)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_doubleItem_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_longTime)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_longTime_sec", t_option["sec"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_bossLittleEnergy)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_bossLittleEnergy_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_subSmallSize)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_subSmallSize_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_smallArea)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_smallArea_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_widePerfect)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_widePerfect_percent", t_option["percent"].asInt());
				}
			}
			
			Json::Value defItems = result_data["defItems"];
			SDS_SI(kSDF_stageInfo, mySD->getSilType(), "defItems_cnt", defItems.size());
			for(int i=0;i<defItems.size();i++)
			{
				Json::Value t_item = defItems[i];
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), CCSTR_CWF("defItems_%d_type", i)->getCString(), t_item["type"].asInt());
				
                
                Json::Value t_option;
                if(!t_item["option"].isObject()){
                    t_option["key"]="value";
                }else{
                    t_option =t_item["option"];
                }
                
				if(t_item["type"].asInt() == kIC_attack)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_attack_power", t_option["power"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_addTime)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_addTime_sec", t_option["sec"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_fast)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_fast_sec", t_option["sec"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_silence)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_silence_sec", t_option["sec"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_doubleItem)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_doubleItem_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_longTime)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_longTime_sec", t_option["sec"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_bossLittleEnergy)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_bossLittleEnergy_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_subSmallSize)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_subSmallSize_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_smallArea)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_smallArea_percent", t_option["percent"].asInt());
				}
				else if(t_item["type"].asInt() == kIC_widePerfect)
				{
					SDS_SI(kSDF_stageInfo, mySD->getSilType(), "itemOption_widePerfect_percent", t_option["percent"].asInt());
				}
			}
			
			Json::Value cards = result_data["cards"];
			for(int i=0;i<cards.size();i++)
			{
				Json::Value t_card = cards[i];
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_rank", t_card["no"].asInt())->getCString(), t_card["rank"].asInt());
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_durability", t_card["no"].asInt())->getCString(), t_card["durability"].asInt());
				
				Json::Value t_ability = t_card["ability"];
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_cnt", t_card["no"].asInt())->getCString(), t_ability.size());
				for(int j=0;j<t_ability.size();j++)
				{
					Json::Value t_abil = t_ability[j];
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_%d_type", t_card["no"].asInt(), t_abil["type"].asInt())->getCString(), t_abil["type"].asInt());
					
                    
                    Json::Value t_option;
                    if(!t_abil["option"].isObject()){
                        t_option["key"]="value";
                    }else{
                        t_option =t_abil["option"];
                    }
                    
					if(t_abil["type"].asInt() == kIC_attack)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_attack_option_power", t_card["no"].asInt())->getCString(), t_option["power"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_addTime)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_addTime_option_sec", t_card["no"].asInt())->getCString(), t_option["sec"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_fast)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_fast_option_sec", t_card["no"].asInt())->getCString(), t_option["sec"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_silence)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_silence_option_sec", t_card["no"].asInt())->getCString(), t_option["sec"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_doubleItem)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_doubleItem_option_percent", t_card["no"].asInt())->getCString(), t_option["percent"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_longTime)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_longTime_option_sec", t_card["no"].asInt())->getCString(), t_option["sec"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_bossLittleEnergy)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_bossLittleEnergy_option_percent", t_card["no"].asInt())->getCString(), t_option["percent"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_subSmallSize)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_subSmallSize_option_percent", t_card["no"].asInt())->getCString(), t_option["percent"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_smallArea)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_smallArea_option_percent", t_card["no"].asInt())->getCString(), t_option["percent"].asInt());
					}
					else if(t_abil["type"].asInt() == kIC_widePerfect)
					{
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_ability_widePerfect_option_percent", t_card["no"].asInt())->getCString(), t_option["percent"].asInt());
					}
				}
				
				Json::Value t_imgInfo = t_card["imgInfo"];
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_imgInfo_size", t_card["no"].asInt())->getCString(), t_imgInfo["size"].asInt());
				
				if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_imgInfo_img", t_card["no"].asInt())->getCString()) != t_imgInfo["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_imgInfo["size"].asInt();
					t_df.img = t_imgInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("stage%d_level%d_visible.png", mySD->getSilType(), i+1)->getCString();
					t_df.key = CCSTR_CWF("%d_imgInfo_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				Json::Value t_thumbnailInfo = t_card["thumbnailInfo"];
				SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_thumbnailInfo_size", t_card["no"].asInt())->getCString(), t_thumbnailInfo["size"].asInt());
				
				if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_thumbnailInfo_img", t_card["no"].asInt())->getCString()) != t_thumbnailInfo["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_thumbnailInfo["size"].asInt();
					t_df.img = t_thumbnailInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("stage%d_level%d_thumbnail.png", mySD->getSilType(), i+1)->getCString();
					t_df.key = CCSTR_CWF("%d_thumbnailInfo_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				Json::Value t_aniInfo = t_card["aniInfo"];
				SDS_SB(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_isAni", t_card["no"].asInt())->getCString(), t_aniInfo["isAni"].asBool());
				if(t_aniInfo["isAni"].asBool())
				{
					Json::Value t_detail = t_aniInfo["detail"];
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_loopLength", t_card["no"].asInt())->getCString(), t_detail["loopLength"].asInt());
					
					Json::Value t_loopSeq = t_detail["loopSeq"];
					for(int j=0;j<t_loopSeq.size();j++)
						SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_loopSeq_%d", t_card["no"].asInt(), j)->getCString(), t_loopSeq[j].asInt());
					
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_cutWidth", t_card["no"].asInt())->getCString(), t_detail["cutWidth"].asInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_cutHeight", t_card["no"].asInt())->getCString(), t_detail["cutHeight"].asInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_cutLength", t_card["no"].asInt())->getCString(), t_detail["cutLength"].asInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_positionX", t_card["no"].asInt())->getCString(), t_detail["positionX"].asInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_positionY", t_card["no"].asInt())->getCString(), t_detail["positionY"].asInt());
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_size", t_card["no"].asInt())->getCString(), t_detail["size"].asInt());
					
					if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString()) != t_detail["img"].asString())
					{
						// check, after download ----------
						DownloadFile t_df;
						t_df.size = t_detail["size"].asInt();
						t_df.img = t_detail["img"].asString().c_str();
						t_df.filename = CCSTR_CWF("stage%d_level%d_animation.png", mySD->getSilType(), i+1)->getCString();
						t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
						df_list.push_back(t_df);
						// ================================
					}
				}
				
				SDS_SS(kSDF_cardInfo, CCSTR_CWF("%d_script", t_card["no"].asInt())->getCString(), t_card["script"].asString());
				
				Json::Value t_silImgInfo = t_card["silImgInfo"];
				SDS_SB(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_isSil", t_card["no"].asInt())->getCString(), t_silImgInfo["isSil"].asBool());
				if(t_silImgInfo["isSil"].asBool())
				{
					SDS_SI(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_size", t_card["no"].asInt())->getCString(), t_silImgInfo["size"].asInt());
					if(SDS_GS(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString()) != t_silImgInfo["img"].asString())
					{
						// check, after download ----------
						DownloadFile t_df;
						t_df.size = t_silImgInfo["size"].asInt();
						t_df.img = t_silImgInfo["img"].asString().c_str();
						t_df.filename = CCSTR_CWF("stage%d_level%d_invisible.png", mySD->getSilType(), i+1)->getCString();
						t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
						df_list.push_back(t_df);
						// ================================
					}
					SDS_SS(kSDF_cardInfo, CCSTR_CWF("%d_silImgInfo_silData", t_card["no"].asInt())->getCString(), t_silImgInfo["silData"].asString());
				}
			}
			
			SDS_SS(kSDF_stageInfo, mySD->getSilType(), "boss", result_data["boss"].asString());
			SDS_SS(kSDF_stageInfo, mySD->getSilType(), "junior", result_data["junior"].asString());
			
			if(df_list.size() > 0) // need download
			{
				download_version = result_data["version"].asInt();
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
				SDS_SI(kSDF_stageInfo, mySD->getSilType(), "version", result_data["version"].asInt());
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