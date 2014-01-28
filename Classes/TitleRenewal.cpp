//
//  TitleRenewal.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 9..
//
//

#include "TitleRenewal.h"
#include "StarGoldData.h"
#include "TakeCardPopup.h"
#include "utf8.h"
#include "UnknownFriends.h"
#include "KnownFriend.h"
#include "KSUtil.h"
#include <chrono>
#include "MainFlowScene.h"
#include "TutorialFlowStep.h"

CCScene* TitleRenewalScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	TitleRenewalScene *layer = TitleRenewalScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TitleRenewalScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	is_menu_enable = false;
	
	state_label = CCLabelTTF::create("까똑 로그인 ing...", mySGD->getFont().c_str(), 20);
	state_label->setPosition(ccp(240,290));
	addChild(state_label);
	
//	Json::Value param;
//	param["ManualLogin"] = true;
//	
//	hspConnector::get()->login(param, param, std::bind(&TitleRenewalScene::resultLogin, this, std::placeholders::_1));
	
	Json::Value t_result_data;
	hspConnector::get()->myKakaoInfo["user_id"] = 88741857374149376L;
	hspConnector::get()->myKakaoInfo["nickname"] = "YH";
	graphdog->setKakaoMemberID(hspConnector::get()->getKakaoID());
	t_result_data["error"]["isSuccess"] = true;
	resultLogin(t_result_data);
	
	return true;
}

void TitleRenewalScene::resultLogin( Json::Value result_data )
{
	CCLog("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	if(result_data["error"]["isSuccess"].asBool())
	{
		if(myLog->getLogCount() > 0)
		{
			myLog->sendLog(CCString::createWithFormat("ting_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
		}
		
		receive_cnt = 0;
		
		CCLabelTTF* common_setting_label = CCLabelTTF::create("start getcommonsetting", mySGD->getFont().c_str(), 10);
		common_setting_label->setPosition(ccp(40, myDSH->ui_top-30));
		addChild(common_setting_label);
		command_list.push_back(CommandParam("getcommonsetting", Json::Value(), json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
		
		CCLabelTTF* noticelist_label = CCLabelTTF::create("start getnoticelist", mySGD->getFont().c_str(), 10);
		noticelist_label->setPosition(ccp(440, myDSH->ui_top-30));
		addChild(noticelist_label);
		command_list.push_back(CommandParam("getnoticelist", Json::Value(), json_selector(this, TitleRenewalScene::resultGetNoticeList)));
		
		CCLabelTTF* character_label = CCLabelTTF::create("start getcharacterlist", mySGD->getFont().c_str(), 10);
		character_label->setPosition(ccp(120, myDSH->ui_top-30));
		addChild(character_label);
		Json::Value character_param;
		character_param["version"] = NSDS_GI(kSDS_GI_characterVersion_i);
		command_list.push_back(CommandParam("getcharacterlist", character_param, json_selector(this, TitleRenewalScene::resultGetCharacterInfo)));
		
		CCLabelTTF* monster_label = CCLabelTTF::create("start getmonsterlist", mySGD->getFont().c_str(), 10);
		monster_label->setPosition(ccp(240, myDSH->ui_top-45));
		addChild(monster_label);
		Json::Value monster_param;
		monster_param["version"] = NSDS_GI(kSDS_GI_monsterVersion_i);
		command_list.push_back(CommandParam("getmonsterlist", monster_param, json_selector(this, TitleRenewalScene::resultGetMonsterList)));
		
		CCLabelTTF* userdata_label = CCLabelTTF::create("start getuserdata", mySGD->getFont().c_str(), 10);
		userdata_label->setPosition(ccp(200, myDSH->ui_top-30));
		addChild(userdata_label);
		Json::Value userdata_param;
		userdata_param["memberID"] = hspConnector::get()->getKakaoID();
		command_list.push_back(CommandParam("getUserData", userdata_param, json_selector(this, TitleRenewalScene::resultGetUserData)));
		
		CCLabelTTF* puzzlelist_label = CCLabelTTF::create("start getpuzzlelist", mySGD->getFont().c_str(), 10);
		puzzlelist_label->setPosition(ccp(280, myDSH->ui_top-30));
		addChild(puzzlelist_label);
		Json::Value puzzlelist_param;
		puzzlelist_param["puzzlelistversion"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
		puzzlelist_param["eventstagelistversion"] = NSDS_GI(kSDS_GI_eventListVersion_i);
		command_list.push_back(CommandParam("getpuzzlelist", puzzlelist_param, json_selector(this, TitleRenewalScene::resultGetPuzzleList)));
		
		CCLabelTTF* loadfriends_label = CCLabelTTF::create("start kLoadFriends", mySGD->getFont().c_str(), 10);
		loadfriends_label->setPosition(ccp(360, myDSH->ui_top-30));
		addChild(loadfriends_label);
		must_be_load_friends = true;
		
		startCommand();
	}
	else
	{
		Json::Value param;
		param["ManualLogin"] = true;
		
		hspConnector::get()->login(param, param, std::bind(&TitleRenewalScene::resultLogin, this, std::placeholders::_1));
	}
}

void TitleRenewalScene::startCommand()
{
	is_receive_fail = false;
	receive_cnt += command_list.size();
	if(must_be_load_friends)
		receive_cnt++;
	hspConnector::get()->command(command_list);
	command_list.clear();
	
	if(must_be_load_friends)
	{
		Json::Value t_result_data;
		
		t_result_data["status"] = 0;
		
		resultGetKnownFriendList(t_result_data);
		
//		hspConnector::get()->kLoadFriends(Json::Value(),
//										  bind(&ThisClassType::resultGetKnownFriendList, this, std::placeholders::_1));
		must_be_load_friends = false;
	}
}

void TitleRenewalScene::checkReceive()
{
	CCLog("receive_cnt : %d", receive_cnt);
	if(receive_cnt == 0)
	{
		if(command_list.empty() && !must_be_load_friends)
		{
			state_label->setString("이미지 정보를 받아옵니다.");
			ing_download_cnt = 1;
			ing_download_per = 0;
			is_downloading = true;
			
			if(!download_state)
			{
				download_state = CCLabelBMFont::create("", "etc_font.fnt");
				download_state->setPosition(ccp(240,130));
				addChild(download_state);
			}
			
			startFileDownload();
		}
		else if(is_receive_fail)
		{
			state_label->setString("정보 불러오기 실패");
			
			CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
			CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
			s_replay->setColor(ccGRAY);
			
			CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleRenewalScene::menuAction));
			replay_item->setTag(kTitleRenewal_MT_replay);
			
			CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
			replay_menu->setPosition(ccp(240,160));
			addChild(replay_menu, 0, kTitleRenewal_MT_replay);
			
			is_menu_enable = true;
		}
		else
		{
			startCommand();
		}
	}
}

void TitleRenewalScene::resultGetCommonSetting(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->setHeartMax(result_data["heartMax"].asInt());
		mySGD->setHeartCoolTime(result_data["heartCoolTime"].asInt());
		mySGD->setGameFriendMax(result_data["gameFriendMax"].asInt());
		mySGD->setHelpCoolTime(result_data["helpCoolTime"].asInt());
		mySGD->setChallengeCoolTime(result_data["challengeCoolTime"].asInt());
		mySGD->setMsgRemoveDay(result_data["msgRemoveDay"].asInt());
		mySGD->setGachaGoldFee(result_data["gachaGoldFee"].asInt());
		mySGD->setGachaRubyFee(result_data["gachaRubyFee"].asInt());
		mySGD->setGachaSocialFee(result_data["gachaSocialFee"].asInt());
		mySGD->setCardUpgradeGoldFee(result_data["cardUpgradeGoldFee"].asInt());
		mySGD->setCardUpgradeRubyFee(result_data["cardUpgradeRubyFee"].asInt());
		mySGD->setHeartSendCoolTime(result_data["heartSendCoolTime"].asInt());
		mySGD->setInviteMaxADay(result_data["inviteMaxADay"].asInt());
		mySGD->setInviteCoolDay(result_data["inviteCoolDay"].asInt());
		mySGD->setPlayContinueFee(result_data["playContinueFee"].asInt());
		mySGD->setCardDurabilityUpFee(result_data["cardDurabilityUpFee"].asInt());
		mySGD->setGachaMapFee(result_data["gachaMapFee"].asInt());
		mySGD->setRemoveFriendCoolTime(result_data["removeFriendCoolTime"].asInt());
		myDSH->setDefaultGold(result_data["defaultGold"].asInt());
		myDSH->setDefaultRuby(result_data["defaultRuby"].asInt());
		mySGD->setSPSendHeart(result_data["SPSendHeart"].asInt());
		mySGD->setSPSendTicket(result_data["SPSendTicket"].asInt());
		mySGD->setSPFinishedChallenge(result_data["SPFinishedChallenge"].asInt());
		mySGD->setSPHelpChallenge(result_data["SPHelpChallenge"].asInt());
		mySGD->setSPSendBoast(result_data["SPSendBoast"].asInt());
		mySGD->setSPGetTime(result_data["SPGetTime"].asInt());
		mySGD->setSPGetHeart(result_data["SPGetHeart"].asInt());
		mySGD->setGachaOnePercentFee(result_data["gachaOnePercentFee"].asInt());
		myDSH->setDefaultSocial(result_data["defaultSocial"].asInt());
		
		mySGD->setBonusItemCnt(kIC_fast, result_data["bonusItemCntFast"].asInt());
		mySGD->setBonusItemCnt(kIC_critical, result_data["bonusItemCntCritical"].asInt());
		mySGD->setBonusItemCnt(kIC_subOneDie, result_data["bonusItemCntSubOneDie"].asInt());
		mySGD->setBonusItemCnt(kIC_doubleItem, result_data["bonusItemCntDoubleItem"].asInt());
		mySGD->setBonusItemCnt(kIC_silence, result_data["bonusItemCntSilence"].asInt());
		mySGD->setBonusItemCnt(kIC_subNothing, result_data["bonusItemCntSubNothing"].asInt());
		mySGD->setBonusItemCnt(kIC_longTime, result_data["bonusItemCntLongTime"].asInt());
		mySGD->setBonusItemCnt(kIC_bossLittleEnergy, result_data["bonusItemCntBossLittleEnergy"].asInt());
		mySGD->setBonusItemCnt(kIC_subSmallSize, result_data["bonusItemCntSubSmallSize"].asInt());
		mySGD->setBonusItemCnt(kIC_smallArea, result_data["bonusItemCntSmallArea"].asInt());
		mySGD->setBonusItemCnt(kIC_widePerfect, result_data["bonusItemCntWidePerfect"].asInt());
		mySGD->setBonusItemCnt(kIC_randomChange, result_data["bonusItemCntRandomChange"].asInt());
		mySGD->setBonusItemCnt(kIC_rentCard, result_data["bonusItemCntRentCard"].asInt());
		mySGD->setAiAdderOnDrewOrDamaged(result_data["aiAdderOnDrewOrDamaged"].asFloat());
		mySGD->setFuryPercent(result_data["furyPercent"].asFloat());
	}
	else
	{
		is_receive_fail = true;
		CCLabelTTF* common_setting_label = CCLabelTTF::create("fail getcommonsetting", mySGD->getFont().c_str(), 10);
		common_setting_label->setPosition(ccp(40, myDSH->ui_top-60));
		addChild(common_setting_label);
		command_list.push_back(CommandParam("getcommonsetting", Json::Value(), json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetNoticeList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->resetNoticeList(result_data["list"]);
	}
	else
	{
		is_receive_fail = true;
		CCLabelTTF* common_setting_label = CCLabelTTF::create("fail getnoticelist", mySGD->getFont().c_str(), 10);
		common_setting_label->setPosition(ccp(440, myDSH->ui_top-60));
		addChild(common_setting_label);
		command_list.push_back(CommandParam("getnoticelist", Json::Value(), json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetCharacterInfo(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value character_list = result_data["list"];
		int character_count = character_list.size();
		NSDS_SI(kSDS_GI_characterCount_i, character_count, false);
		for(int i=1;i<=character_count;i++)
		{
			NSDS_SI(kSDS_GI_characterInfo_int1_no_i, i, character_list[i-1]["no"].asInt(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_name_s, i, character_list[i-1]["name"].asString(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_purchaseInfo_type_s, i, character_list[i-1]["purchaseInfo"]["type"].asString(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_purchaseInfo_value_i, i, character_list[i-1]["purchaseInfo"]["value"].asInt(), false);
			NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_gold_d, i, character_list[i-1]["statInfo"]["gold"].asDouble(), false);
			NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_percent_d, i, character_list[i-1]["statInfo"]["percent"].asDouble(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_feverTime_i, i, character_list[i-1]["statInfo"]["feverTime"].asInt(), false);
			NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_speed_d, i, character_list[i-1]["statInfo"]["speed"].asDouble(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_life_i, i, character_list[i-1]["statInfo"]["life"].asInt(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, i, character_list[i-1]["resourceInfo"]["ccbiID"].asString(), false);
			
			if(NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbi_s, i) != character_list[i-1]["resourceInfo"]["ccbi"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 100;
				t_df.img = character_list[i-1]["resourceInfo"]["ccbi"].asString().c_str();
				t_df.filename = character_list[i-1]["resourceInfo"]["ccbiID"].asString() + ".ccbi";
				t_df.key = CCSTR_CWF("ci%d_ri_ccbi", i)->getCString();
				character_download_list.push_back(t_df);
				// ================================
			}
			
			NSDS_SS(kSDS_GI_characterInfo_int1_resourceInfo_imageID_s, i, character_list[i-1]["resourceInfo"]["imageID"].asString(), false);
			
			if(NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_plist_s, i) != character_list[i-1]["resourceInfo"]["plist"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 100;
				t_df.img = character_list[i-1]["resourceInfo"]["plist"].asString().c_str();
				t_df.filename = character_list[i-1]["resourceInfo"]["imageID"].asString() + ".plist";
				t_df.key = CCSTR_CWF("ci%d_ri_plist", i)->getCString();
				character_download_list.push_back(t_df);
				// ================================
			}
			
			
			if(NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_pvrccz_s, i) != character_list[i-1]["resourceInfo"]["pvrccz"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 100;
				t_df.img = character_list[i-1]["resourceInfo"]["pvrccz"].asString().c_str();
				t_df.filename = character_list[i-1]["resourceInfo"]["imageID"].asString() + ".pvr.ccz";
				t_df.key = CCSTR_CWF("ci%d_ri_pvrccz", i)->getCString();
				character_download_list.push_back(t_df);
				// ================================
			}
			
			NSDS_SI(kSDS_GI_characterInfo_int1_resourceInfo_size_i, i, character_list[i-1]["resourceInfo"]["size"].asInt(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_comment_s, i, character_list[i-1]["comment"].asString(), false);
		}
		
		if(character_download_list.size() > 0)
			character_download_version = result_data["version"].asInt();
		else
			NSDS_SI(kSDS_GI_characterVersion_i, result_data["version"].asInt(), false);
		
		mySDS->fFlush(kSDS_GI_characterCount_i);
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		
	}
	else
	{
		is_receive_fail = true;
		CCLabelTTF* character_label = CCLabelTTF::create("fail getcharacterlist", mySGD->getFont().c_str(), 10);
		character_label->setPosition(ccp(120, myDSH->ui_top-60));
		addChild(character_label);
		Json::Value character_param;
		character_param["version"] = NSDS_GI(kSDS_GI_characterVersion_i);
		command_list.push_back(CommandParam("getcharacterlist", character_param, json_selector(this, TitleRenewalScene::resultGetCharacterInfo)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetMonsterList(Json::Value result_data)
{
	CCLog("result getmonsterlist : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value monster_list = result_data["list"];
		int monster_count = monster_list.size();
		NSDS_SI(kSDS_GI_monsterCount_i, monster_count, false);
		for(int i=1;i<=monster_count;i++)
		{
			NSDS_SI(kSDS_GI_monsterInfo_int1_no_i, i, monster_list[i-1]["no"].asInt(), false);
			NSDS_SS(kSDS_GI_monsterInfo_int1_name_s, i, monster_list[i-1]["name"].asString(), false);
			NSDS_SB(kSDS_GI_monsterInfo_int1_isBoss_b, i, monster_list[i-1]["isBoss"].asBool(), false);
			NSDS_SS(kSDS_GI_monsterInfo_int1_resourceInfo_ccbiID_s, i, monster_list[i-1]["resourceInfo"]["ccbiID"].asString(), false);
			
			string monster_type = monster_list[i-1]["type"].asString();
			if(monster_type == "snake")
			{
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_ccbi_s, i) != monster_list[i-1]["resourceInfo"]["ccbi"].asString())
				{
					// check, after download ----------
					DownloadFile t_df1;
					t_df1.size = 100;
					t_df1.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + "_head.ccbi";
					t_df1.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_head.ccbi";
					t_df1.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					monster_download_list.push_back(t_df1);
					// ================================
					
					// check, after download ----------
					DownloadFile t_df2;
					t_df2.size = 100;
					t_df2.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + "_body.ccbi";
					t_df2.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_body.ccbi";
					t_df2.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					monster_download_list.push_back(t_df2);
					// ================================
					
					// check, after download ----------
					DownloadFile t_df3;
					t_df3.size = 100;
					t_df3.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + "_tail.ccbi";
					t_df3.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_tail.ccbi";
					t_df3.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					monster_download_list.push_back(t_df3);
					// ================================
				}
				
				NSDS_SS(kSDS_GI_monsterInfo_int1_resourceInfo_imageID_s, i, monster_list[i-1]["resourceInfo"]["imageID"].asString(), false);
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_plist_s, i) != monster_list[i-1]["resourceInfo"]["plist"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["plist"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".plist";
					t_df.key = CCSTR_CWF("mi%d_ri_plist", i)->getCString();
					monster_download_list.push_back(t_df);
					// ================================
				}
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_pvrccz_s, i) != monster_list[i-1]["resourceInfo"]["pvrccz"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["pvrccz"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".pvr.ccz";
					t_df.key = CCSTR_CWF("mi%d_ri_pvrccz", i)->getCString();
					monster_download_list.push_back(t_df);
					// ================================
				}
				
				NSDS_SI(kSDS_GI_monsterInfo_int1_resourceInfo_size_i, i, monster_list[i-1]["resourceInfo"]["size"].asInt(), false);
			}
			else
			{
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_ccbi_s, i) != monster_list[i-1]["resourceInfo"]["ccbi"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + ".ccbi";
					t_df.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + ".ccbi";
					t_df.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					monster_download_list.push_back(t_df);
					// ================================
				}
				
				NSDS_SS(kSDS_GI_monsterInfo_int1_resourceInfo_imageID_s, i, monster_list[i-1]["resourceInfo"]["imageID"].asString(), false);
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_plist_s, i) != monster_list[i-1]["resourceInfo"]["plist"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["plist"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".plist";
					t_df.key = CCSTR_CWF("mi%d_ri_plist", i)->getCString();
					monster_download_list.push_back(t_df);
					// ================================
				}
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_pvrccz_s, i) != monster_list[i-1]["resourceInfo"]["pvrccz"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["pvrccz"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".pvr.ccz";
					t_df.key = CCSTR_CWF("mi%d_ri_pvrccz", i)->getCString();
					monster_download_list.push_back(t_df);
					// ================================
				}
				
				NSDS_SI(kSDS_GI_monsterInfo_int1_resourceInfo_size_i, i, monster_list[i-1]["resourceInfo"]["size"].asInt(), false);
			}
		}
		
		if(monster_download_list.size() > 0)
			monster_download_version = result_data["version"].asInt();
		else
			NSDS_SI(kSDS_GI_monsterVersion_i, result_data["version"].asInt(), false);
		
		mySDS->fFlush(kSDS_GI_characterCount_i);
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		
	}
	else
	{
		is_receive_fail = true;
		CCLabelTTF* monster_label = CCLabelTTF::create("fail getmonsterlist", mySGD->getFont().c_str(), 10);
		monster_label->setPosition(ccp(240, myDSH->ui_top-90));
		addChild(monster_label);
		Json::Value monster_param;
		monster_param["version"] = NSDS_GI(kSDS_GI_monsterVersion_i);
		command_list.push_back(CommandParam("getmonsterlist", monster_param, json_selector(this, TitleRenewalScene::resultGetMonsterList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetUserData( Json::Value result_data )
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS || result_data["result"]["code"].asInt() == GDDONTFIND)
	{
		myDSH->resetDSH();
		card_data_load_list.clear();
		myDSH->loadAllUserData(result_data, card_data_load_list);
		
		if(myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep) != kTutorialFlowStep_puzzleClick)
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_end);
		
		mySGD->resetHasGottenCards();
		
		if(card_data_load_list.size() > 0)
		{
			Json::Value card_param;
			for(int i=0;i<card_data_load_list.size();i++)
				card_param["noList"][i] = card_data_load_list[i];
			command_list.push_back(CommandParam("getcardlist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
		}
		
		Json::Value memberIDList;
		Json::Reader reader;
		reader.parse(result_data["friendList"].asString(), friendList);
		
		for(int i = 0; i<friendList.size(); i++)
			memberIDList["memberIDList"].append(friendList[i].asString());
		
		command_list.push_back(CommandParam("getuserdatalist", memberIDList, bind(&ThisClassType::resultGetUnknownFriendUserData, this,	std::placeholders::_1)));
	}
	else
	{
		is_receive_fail = true;
		CCLabelTTF* userdata_label = CCLabelTTF::create("fail getuserdata", mySGD->getFont().c_str(), 10);
		userdata_label->setPosition(ccp(200, myDSH->ui_top-30));
		addChild(userdata_label);
		Json::Value userdata_param;
		userdata_param["memberID"] = hspConnector::get()->getKakaoID();
		command_list.push_back(CommandParam("getUserData", userdata_param, json_selector(this, TitleRenewalScene::resultGetUserData)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultLoadedCardData( Json::Value result_data )
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["stage"].asInt(), false);
			NSDS_SI(t_card["stage"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
			Json::Value t_card_missile = t_card["missile"];
			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
			
			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
			
			Json::Value t_ability = t_card["ability"];
			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
			for(int j=0;j<t_ability.size();j++)
			{
				Json::Value t_abil = t_ability[j];
				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
				
				Json::Value t_option;
				if(!t_abil["option"].isObject())                    t_option["key"]="value";
				else												t_option =t_abil["option"];
				
				if(t_abil["type"].asInt() == kIC_attack)
					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_addTime)
					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_fast)
					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_silence)
					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_doubleItem)
					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_longTime)
					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_bossLittleEnergy)
					NSDS_SI(kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_subSmallSize)
					NSDS_SI(kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_smallArea)
					NSDS_SI(kSDS_CI_int1_abilitySmallAreaOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_widePerfect)
					NSDS_SI(kSDS_CI_int1_abilityWidePerfectOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
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
				card_download_list.push_back(t_df);
				// ================================
				
				CopyFile t_cf;
				t_cf.from_filename = t_df.filename.c_str();
				t_cf.to_filename = CCSTR_CWF("stage%d_level%d_thumbnail.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
				card_reduction_list.push_back(t_cf);
				
				is_add_cf = true;
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
					t_df.filename = CCSTR_CWF("stage%d_level%d_animation.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					card_download_list.push_back(t_df);
					// ================================
				}
				
				if(is_add_cf)
				{
					CopyFile t_cf = card_reduction_list.back();
					card_reduction_list.pop_back();
					t_cf.is_ani = true;
					t_cf.cut_width = t_detail["cutWidth"].asInt();
					t_cf.cut_height = t_detail["cutHeight"].asInt();
					t_cf.position_x = t_detail["positionX"].asInt();
					t_cf.position_y = t_detail["positionY"].asInt();
					t_cf.ani_filename = CCSTR_CWF("stage%d_level%d_animation.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					card_reduction_list.push_back(t_cf);
				}
			}
			
			NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
			
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
					t_df.filename = CCSTR_CWF("stage%d_level%d_invisible.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					card_download_list.push_back(t_df);
					// ================================
				}
			}
		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
	}
	else
	{
		is_receive_fail = true;
		Json::Value card_param;
		for(int i=0;i<card_data_load_list.size();i++)
			card_param["noList"][i] = card_data_load_list[i];
		command_list.push_back(CommandParam("getcardlist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetKnownFriendList(Json::Value fInfo)
{
	if(fInfo["status"].asInt() == 0)
	{
		Json::Value appFriends = fInfo["app_friends_info"];
		
		for(int i=0; i<appFriends.size(); i++)
		{
			FriendData kfd;
			kfd.nick = appFriends[i]["nickname"].asString();
			kfd.messageBlocked = appFriends[i]["message_blocked"].asInt();
			kfd.profileUrl = appFriends[i]["profile_image_url"].asString();
			kfd.userId = appFriends[i]["user_id"].asString();
			kfd.hashedTalkUserId = appFriends[i]["hashed_talk_user_id"].asString();
			kfd.unknownFriend = false;
			KnownFriends::getInstance()->add(kfd);
		}
		
		Json::Value memberIDList;
		for(auto i : KnownFriends::getInstance()->getFriends())
		{
			memberIDList["memberIDList"].append(i.userId);
		}
		
//		CCAssert(memberIDList.size() > 0, "friend count is zero?");
		if(memberIDList.size() > 0)
			command_list.push_back(CommandParam("getuserdatalist", memberIDList,
												bind(&ThisClassType::resultGetKnownFriendUserData,
													 this,	std::placeholders::_1)));
	}
	else
	{
		is_receive_fail = true;
		must_be_load_friends = true;
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetKnownFriendUserData(Json::Value v)
{
	if(v["result"]["code"].asInt() == GDSUCCESS)
	{
		for(int i=0; i<v["list"].size(); i++)
		{
			Json::Reader reader;
			Json::Value userData;
			reader.parse(v["list"][i]["data"].asString(), userData);
			KnownFriends::getInstance()->putUserData(i, userData);
			KnownFriends::getInstance()->putLastDate(i, v["list"][i]["lastDate"].asInt64());
			KnownFriends::getInstance()->putJoinDate(i, v["list"][i]["joinDate"].asInt64());
		}
	}
	else
	{
		is_receive_fail = true;
		Json::Value memberIDList;
		for(auto i : KnownFriends::getInstance()->getFriends())
		{
			memberIDList["memberIDList"].append(i.userId);
		}
		
		command_list.push_back(CommandParam("getuserdatalist", memberIDList,
											bind(&ThisClassType::resultGetKnownFriendUserData,
												 this,	std::placeholders::_1)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetUnknownFriendUserData(Json::Value v)
{
	if(v["result"]["code"].asInt() == GDSUCCESS || 1)
	{
		for(int i=0; i<v["list"].size(); i++)
		{
			
			Json::Reader reader;
			Json::Value userData;
			reader.parse(v["list"][i]["data"].asString(), userData);
			
			FriendData ufd;
			
			ufd.userData = userData;
			ufd.joinDate = v["list"][i]["joinDate"].asInt64();
			ufd.lastDate = v["list"][i]["lastDate"].asInt64();
			ufd.userId = v["list"][i]["memberID"].asString();
			ufd.nick = v["list"][i]["nick"].asString();
			ufd.unknownFriend = true;
			
			UnknownFriends::getInstance()->add(ufd);
		}
	}
	else
	{
		is_receive_fail = true;
		Json::Value memberIDList;
		for(int i = 0; i<friendList.size(); i++)
			memberIDList["memberIDList"].append(friendList[i].asString());
		
		command_list.push_back(CommandParam("getuserdatalist", memberIDList, bind(&ThisClassType::resultGetUnknownFriendUserData, this,	std::placeholders::_1)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetPuzzleList( Json::Value result_data )
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		if(result_data["puzzlelistversion"] > NSDS_GI(kSDS_GI_puzzleListVersion_i))
		{
			Json::Value puzzle_list = result_data["puzzlelist"];
			
			NSDS_SI(kSDS_GI_puzzleListCount_i, puzzle_list.size(), false);
			
			int puzzle_cnt = puzzle_list.size();
			for(int i=0;i<puzzle_cnt;i++)
			{
				NSDS_SI(kSDS_GI_puzzleList_int1_no_i, i+1, puzzle_list[i]["no"].asInt(), false);
				NSDS_SS(kSDS_GI_puzzleList_int1_title_s, i+1, puzzle_list[i]["title"].asString().c_str(), false);
				NSDS_SS(puzzle_list[i]["no"].asInt(), kSDS_PZ_title_s, puzzle_list[i]["title"].asString().c_str(), false);
				NSDS_SI(kSDS_GI_puzzleList_int1_version_i, i+1, puzzle_list[i]["version"].asInt(), false);
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_startStage_i, puzzle_list[i]["startStage"].asInt(), false);
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_stageCount_i, puzzle_list[i]["stageCount"].asInt(), false);
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_point_i, puzzle_list[i]["point"].asInt(), false);
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_ticket_i, puzzle_list[i]["ticket"].asInt(), false);
				
				mySDS->fFlush(puzzle_list[i]["no"].asInt(), kSDS_PZ_base);
				
				Json::Value thumbnail = puzzle_list[i]["thumbnail"];
				if(NSDS_GS(kSDS_GI_puzzleList_int1_thumbnail_s, i+1) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("puzzleList%d_thumbnail.png", i+1)->getCString();
					t_df.key = CCSTR_CWF("puzzleList%d_thumbnail", i+1)->getCString();
					puzzle_download_list.push_back(t_df);
					// ================================
				}
			}
			
			if(puzzle_download_list.size() > 0)
				puzzlelist_download_version = result_data["puzzlelistversion"].asInt();
			else
				NSDS_SI(kSDS_GI_puzzleListVersion_i, result_data["puzzlelistversion"].asInt(), false);
		}
		
		if(result_data["eventstagelistversion"] > NSDS_GI(kSDS_GI_eventListVersion_i))
		{
			Json::Value event_list = result_data["eventstagelist"];
			int el_length = event_list.size();
			NSDS_SI(kSDS_GI_eventCount_i, el_length, false);
			for(int i=0;i<el_length;i++)
			{
				int event_code = event_list[i]["no"].asInt();
				NSDS_SI(kSDS_GI_event_int1_code_i, i, event_code, false);
				Json::Value thumbnail = event_list[i]["thumbnail"];
				if(NSDS_GS(kSDS_GI_event_int1_thumbnail_s, i) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("event%d_thumbnail.png", i)->getCString();
					t_df.key = CCSTR_CWF("event%d_thumbnail", i)->getCString();
					event_download_list.push_back(t_df);
					// ================================
				}
			}
			
			if(event_download_list.size() > 0) // need download
				eventstagelist_download_version = result_data["eventstagelistversion"].asInt();
			else
				NSDS_SI(kSDS_GI_eventListVersion_i, result_data["eventstagelistversion"].asInt(), false);
		}
		
		mySDS->fFlush(kSDS_GI_characterCount_i);
	}
	else
	{
		is_receive_fail = true;
		Json::Value puzzlelist_param;
		puzzlelist_param["puzzlelistversion"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
		puzzlelist_param["eventstagelistversion"] = NSDS_GI(kSDS_GI_eventListVersion_i);
		command_list.push_back(CommandParam("getpuzzlelist", puzzlelist_param, json_selector(this, TitleRenewalScene::resultGetPuzzleList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::endingAction()
{
	mySGD->selectFriendCard();
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	
	CCDelayTime* t_delay = CCDelayTime::create(0.2f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleRenewalScene::changeScene));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
}

void TitleRenewalScene::changeScene()
{
	CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
//	CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
}

void TitleRenewalScene::startFileDownload()
{
	if(character_download_list.size() > 0 && ing_download_cnt <= character_download_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, character_download_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(character_download_list[ing_download_cnt-1].img,
													  character_download_list[ing_download_cnt-1].size,
													  character_download_list[ing_download_cnt-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
	}
	else if(monster_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, monster_download_list[ing_download_cnt-character_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(monster_download_list[ing_download_cnt-character_download_list.size()-1].img,
													  monster_download_list[ing_download_cnt-character_download_list.size()-1].size,
													  monster_download_list[ing_download_cnt-character_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
	}
	else if(card_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img,
													  card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].size,
													  card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
		
		schedule(schedule_selector(TitleRenewalScene::downloadingFileAction));
	}
	else if(puzzle_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img,
													  puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].size,
													  puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
		
		schedule(schedule_selector(TitleRenewalScene::downloadingFileAction));
	}
	else if(event_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].img,
													  event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].size,
													  event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
		
		schedule(schedule_selector(TitleRenewalScene::downloadingFileAction));
	}
	else
	{
		endingCheck();
	}
}

void TitleRenewalScene::successDownloadAction()
{
	unschedule(schedule_selector(TitleRenewalScene::downloadingFileAction));
	if(ing_download_cnt < character_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, character_download_list[ing_download_cnt-1].key, character_download_list[ing_download_cnt-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, character_download_list[ing_download_cnt-1].key, character_download_list[ing_download_cnt-1].img, false);
		NSDS_SI(kSDS_GI_characterVersion_i, character_download_version, false);
		mySDS->fFlush(kSDS_GI_characterCount_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, monster_download_list[ing_download_cnt-character_download_list.size()-1].key, monster_download_list[ing_download_cnt-character_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, monster_download_list[ing_download_cnt-character_download_list.size()-1].key, monster_download_list[ing_download_cnt-character_download_list.size()-1].img, false);
		NSDS_SI(kSDS_GI_monsterVersion_i, monster_download_version, false);
		mySDS->fFlush(kSDS_GI_monsterCount_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].key,
			   card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].key,
			   card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img, false);
		state_label->setString("카드 섬네일 만드는 중...");
		for(int i=0;i<card_reduction_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(card_reduction_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
																CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
				ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].key,
			   puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].key,
			   event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].key,
			   event_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-puzzle_download_list.size()-1].img, false);
		
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt,
											int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
		
		endingCheck();
	}
}

void TitleRenewalScene::endingCheck()
{
	if(puzzle_download_list.size() > 0)
		NSDS_SI(kSDS_GI_puzzleListVersion_i, puzzlelist_download_version, false);
	if(event_download_list.size() > 0)
		NSDS_SI(kSDS_GI_eventListVersion_i, eventstagelist_download_version, false);
	
	mySDS->fFlush(kSDS_GI_characterCount_i);
	
	string nick = myDSH->getStringForKey(kDSH_Key_nick);
	
	if(nick == "")
	{
		state_label->setString("닉네임을 입력해주세요.");
		
		nick_back = CCSprite::create("nickname_back.png");
		nick_back->setPosition(ccp(240,130));
		addChild(nick_back);
		
		input_text = CCEditBox::create(CCSizeMake(210, 30), CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
		input_text->setPosition(ccp(197,113));
		input_text->setPlaceHolder("입력해주세요.");
		input_text->setReturnType(kKeyboardReturnTypeDone);
		input_text->setFont(mySGD->getFont().c_str(), 20);
		input_text->setInputMode(kEditBoxInputModeSingleLine);
		input_text->setDelegate(this);
		addChild(input_text);
		
		CCSprite* n_ok = CCSprite::create("nickname_ok.png");
		CCSprite* s_ok = CCSprite::create("nickname_ok.png");
		s_ok->setColor(ccGRAY);
		
		CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(TitleRenewalScene::menuAction));
		ok_item->setTag(kTitleRenewal_MT_nick);
		
		CCMenu* ok_menu = CCMenu::createWithItem(ok_item);
		ok_menu->setPosition(ccp(370,130));
		addChild(ok_menu, 0, kTitleRenewal_MT_nick);
		
		is_menu_enable = true;
	}
	else
		endingAction();
}

void TitleRenewalScene::downloadingFileAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	
	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;
	
	ing_download_per = t_per;
	
	download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt,
										int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() + event_download_list.size()))->getCString());
}

void TitleRenewalScene::failDownloadAction()
{
	state_label->setString("이미지 받아오기 실패");
	
	CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
	CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
	s_replay->setColor(ccGRAY);
	
	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleRenewalScene::menuAction));
	replay_item->setTag(kTitleRenewal_MT_redown);
	
	CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setPosition(ccp(240,160));
	addChild(replay_menu, 0, kTitleRenewal_MT_redown);
	
	is_menu_enable = true;
}

void TitleRenewalScene::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLog("edit begin");
}
void TitleRenewalScene::editBoxEditingDidEnd(CCEditBox* editBox)
{
	CCLog("edit end");
}
void TitleRenewalScene::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLog("edit changed : %s", text.c_str());
}
void TitleRenewalScene::editBoxReturn(CCEditBox* editBox)
{
	CCLog("edit return");
}

void TitleRenewalScene::menuAction( CCObject* sender )
{
	if(!is_menu_enable)		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kTitleRenewal_MT_replay)
	{
		removeChildByTag(kTitleRenewal_MT_replay);
		//		(save_target->*save_delegate)();
		startCommand();
	}
	else if(tag == kTitleRenewal_MT_redown)
	{
		removeChildByTag(kTitleRenewal_MT_redown);
		state_label->setString("이미지 정보를 받아옵니다.");
		ing_download_cnt = 1;
		ing_download_per = 0;
		is_downloading = true;
		startFileDownload();
	}
	else if(tag == kTitleRenewal_MT_nick)
	{
		string comp_not_ok = "";
		if(input_text->getText() != comp_not_ok)
		{
			myDSH->setStringForKey(kDSH_Key_nick, input_text->getText());
			setTouchEnabled(false);
			is_menu_enable = false;
			nick_back->removeFromParent();
			removeChildByTag(kTitleRenewal_MT_nick);
			input_text->removeFromParent();
			
			myDSH->saveUserData({kSaveUserData_Key_nick}, nullptr);
			
			endingAction();
		}
		else
		{
			is_menu_enable = true;
		}
	}
	else if(tag >= kTitleRenewal_MT_puzzleBase)
	{
		tag -= kTitleRenewal_MT_puzzleBase;
		
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, tag);
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
}