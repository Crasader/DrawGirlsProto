//
//  StageInfoDown.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 24..
//
//

#include "StageInfoDown.h"
#include "DataStorageHub.h"


void StageInfoDown::resultGetStageInfo(Json::Value result_data)
{
	CCLog("result data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["state"].asString() == "ok")
	{
		CCLog("saved version : %d", NSDS_GI(mySD->getSilType(), kSDS_SI_version_i));
		if(NSDS_GI(mySD->getSilType(), kSDS_SI_version_i) < result_data["version"].asInt())
		{
			NSDS_SI(mySD->getSilType(), kSDS_SI_puzzle_i, result_data["puzzle"].asInt());
			NSDS_SI(mySD->getSilType(), kSDS_SI_playtime_i, result_data["playtime"].asInt());
			NSDS_SD(mySD->getSilType(), kSDS_SI_scoreRate_d, result_data["scoreRate"].asDouble());
			NSDS_SI(mySD->getSilType(), kSDS_SI_condGold_i, result_data["condGold"].asInt());
			NSDS_SI(mySD->getSilType(), kSDS_SI_condStageNo_i, result_data["condStageNo"].asInt());
			
			Json::Value t_mission = result_data["mission"];
			NSDS_SI(mySD->getSilType(), kSDS_SI_missionType_i, t_mission["type"].asInt());
			
			Json::Value t_option;
			if(!t_mission["option"].isObject())			t_option["key"]="value";
			else										t_option =t_mission["option"];
			
			if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
				NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionEnergy_i, t_option["energy"].asInt());
			else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
				NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["count"].asInt());
			else if(t_mission["type"].asInt() == kCLEAR_bigArea)
			{
				NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt());
				NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["count"].asInt());
			}
			else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
				NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionCount_i, t_option["count"].asInt());
			else if(t_mission["type"].asInt() == kCLEAR_perfect)
				NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt());
			else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
				NSDS_SI(mySD->getSilType(), kSDS_SI_missionOptionSec_i, t_option["sec"].asInt());
			
			
			Json::Value shopItems = result_data["shopItems"];
			NSDS_SI(mySD->getSilType(), kSDS_SI_shopItemsCnt_i, shopItems.size());
			for(int i=0;i<shopItems.size();i++)
			{
				Json::Value t_item = shopItems[i];
				NSDS_SI(mySD->getSilType(), kSDS_SI_shopItems_int1_type_i, i, t_item["type"].asInt());
				NSDS_SI(mySD->getSilType(), kSDS_SI_shopItems_int1_price_i, i, t_item["price"].asInt());
				
                
                Json::Value t_option;
                if(!t_item["option"].isObject())				t_option["key"]="value";
                else											t_option =t_item["option"];
                
				if(t_item["type"].asInt() == kIC_attack)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAttackPower_i, t_option.get("power",0).asInt());
				else if(t_item["type"].asInt() == kIC_addTime)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAddTimeSec_i, t_option.get("sec",0).asInt());
				else if(t_item["type"].asInt() == kIC_fast)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionFastSec_i, t_option.get("sec",0).asInt());
				else if(t_item["type"].asInt() == kIC_silence)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSilenceSec_i, t_option.get("sec",0).asInt());
				else if(t_item["type"].asInt() == kIC_doubleItem)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_longTime)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt());
				else if(t_item["type"].asInt() == kIC_bossLittleEnergy)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionBossLittleEnergyPercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_subSmallSize)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSubSmallSizePercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_smallArea)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSmallAreaPercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_widePerfect)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionWidePerfectPercent_i, t_option["percent"].asInt());
			}
			
			Json::Value defItems = result_data["defItems"];
			NSDS_SI(mySD->getSilType(), kSDS_SI_defItemsCnt_i, defItems.size());
			for(int i=0;i<defItems.size();i++)
			{
				Json::Value t_item = defItems[i];
				NSDS_SI(mySD->getSilType(), kSDS_SI_defItems_int1_type_i, i, t_item["type"].asInt());
				
                Json::Value t_option;
                if(!t_item["option"].isObject())				t_option["key"]="value";
                else											t_option =t_item["option"];
                
				if(t_item["type"].asInt() == kIC_attack)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAttackPower_i, t_option["power"].asInt());
				else if(t_item["type"].asInt() == kIC_addTime)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionAddTimeSec_i, t_option["sec"].asInt());
				else if(t_item["type"].asInt() == kIC_fast)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionFastSec_i, t_option["sec"].asInt());
				else if(t_item["type"].asInt() == kIC_silence)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSilenceSec_i, t_option["sec"].asInt());
				else if(t_item["type"].asInt() == kIC_doubleItem)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_longTime)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt());
				else if(t_item["type"].asInt() == kIC_bossLittleEnergy)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionBossLittleEnergyPercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_subSmallSize)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSubSmallSizePercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_smallArea)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionSmallAreaPercent_i, t_option["percent"].asInt());
				else if(t_item["type"].asInt() == kIC_widePerfect)
					NSDS_SI(mySD->getSilType(), kSDS_SI_itemOptionWidePerfectPercent_i, t_option["percent"].asInt());
			}
			
			Json::Value cards = result_data["cards"];
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
					DownloadFile t_df;
					t_df.size = t_imgInfo["size"].asInt();
					t_df.img = t_imgInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("stage%d_level%d_visible.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
					
					CopyFile t_cf;
					t_cf.from_filename = t_df.filename.c_str();
					t_cf.to_filename = CCSTR_CWF("stage%d_level%d_thumbnail.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					cf_list.push_back(t_cf);
					
					is_add_cf = true;
				}
				
//				Json::Value t_thumbnailInfo = t_card["thumbnailInfo"];
//				
//				if(NSDS_GS(kSDS_CI_int1_thumbnailInfo_s, t_card["no"].asInt()) != t_thumbnailInfo["img"].asString())
//				{
//					// check, after download ----------
//					DownloadFile t_df;
//					t_df.size = t_thumbnailInfo["size"].asInt();
//					t_df.img = t_thumbnailInfo["img"].asString().c_str();
//					t_df.filename = CCSTR_CWF("stage%d_level%d_thumbnail.png", mySD->getSilType(), i+1)->getCString();
//					t_df.key = CCSTR_CWF("%d_thumbnailInfo", t_card["no"].asInt())->getCString();
//					df_list.push_back(t_df);
//					// ================================
//				}
				
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
						DownloadFile t_df;
						t_df.size = t_detail["size"].asInt();
						t_df.img = t_detail["img"].asString().c_str();
						t_df.filename = CCSTR_CWF("stage%d_level%d_animation.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
						t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
						df_list.push_back(t_df);
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
						DownloadFile t_df;
						t_df.size = t_silImgInfo["size"].asInt();
						t_df.img = t_silImgInfo["img"].asString().c_str();
						t_df.filename = CCSTR_CWF("stage%d_level%d_invisible.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
						t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
						df_list.push_back(t_df);
						// ================================
					}
//					NSDS_SS(kSDS_CI_int1_silImgInfoSilData_s, t_card["no"].asInt(), t_silImgInfo["silData"].asString());
				}
			}
			
			NSDS_SI(mySD->getSilType(), kSDS_SI_level_i, result_data["level"].asInt());
			
			NSDS_SS(mySD->getSilType(), kSDS_SI_boss_s, result_data["boss"].asString());
			NSDS_SS(mySD->getSilType(), kSDS_SI_junior_s, result_data["junior"].asString());
			NSDS_SI(mySD->getSilType(), kSDS_SI_autoBalanceTry_i, result_data["autoBalanceTry"].asInt());
			
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
				removeFromParent();
				(target_success->*delegate_success)();
				
			}
		}
		else
		{
			state_ment->setString("스테이지 정보 확인 완료.");
			removeFromParent();
			(target_success->*delegate_success)();
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
	unschedule(schedule_selector(StageInfoDown::downloadingAction));
	
	if(ing_download_cnt >= df_list.size())
	{
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
		
		NSDS_SI(mySD->getSilType(), kSDS_SI_version_i, download_version);
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
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -180, true);
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

	CCSprite* t_back = CCSprite::create("back_gray.png");
	t_back->setPosition(ccp(240,160));
	addChild(t_back, kSID_Z_back);

	CCMenuItem* cancel_item = CCMenuItemImage::create("sspl_cancel.png", "sspl_cancel.png", this, menu_selector(StageInfoDown::menuAction));
	cancel_item->setTag(kSID_MT_cancel);

	cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(350, 240));
	addChild(cancel_menu, kSID_Z_content);

	state_ment = CCLabelTTF::create("스테이지 정보를 받아오는 ing...", mySGD->getFont().c_str(), 20);
	state_ment->setAnchorPoint(ccp(0.5,0.5));
	state_ment->setPosition(ccp(240,160));
	state_ment->setHorizontalAlignment(kCCTextAlignmentCenter);
	state_ment->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	addChild(state_ment, kSID_Z_content);

	is_downloading = false;

	startGetStageInfo();

	touch_number = 0;
	is_menu_enable = true;
	setTouchEnabled(true);
}

void StageInfoDown::startGetStageInfo()
{
	int stage_number = mySD->getSilType();

	myLog->addLog(kLOG_getStageInfo_i, -1, stage_number);

	if(stage_number < 10000)
	{
		Json::Value param;
		param["no"] = stage_number;
		param["version"] = NSDS_GI(stage_number, kSDS_SI_version_i);
		hspConnector::get()->command("getstageinfo", param, json_selector(this, StageInfoDown::resultGetStageInfo));
	}
	else // event stage
	{
		Json::Value param;
		param["no"] = stage_number;
		param["version"] = NSDS_GI(stage_number, kSDS_SI_version_i);
		hspConnector::get()->command("geteventstageinfo", param, json_selector(this, StageInfoDown::resultGetStageInfo));
	}
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
