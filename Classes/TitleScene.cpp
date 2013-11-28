//
//  TitleScene.cpp
//  DGproto
//
//  Created by LitQoo on 13. 10. 22..
//
//

#include "TitleScene.h"
#include "StarGoldData.h"
#include "TakeCardPopup.h"
#include "utf8.h"

CCScene* TitleScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	TitleScene *layer = TitleScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	is_menu_enable = false;
	successed_get_puzzle_list = false;
	
	state_label = CCLabelTTF::create("까똑 로그인 ing...", mySGD->getFont().c_str(), 20);
	state_label->setPosition(ccp(240,290));
	addChild(state_label);

	//startGetPuzzleList();
	
	Json::Value param;
	param["ManualLogin"] = true;
	
	hspConnector::get()->login(param, param, std::bind(&TitleScene::resultLogin, this, std::placeholders::_1));
	
	return true;
}

void TitleScene::startGetPuzzleList()
{
	myLog->addLog(kLOG_network_getPuzzleEventList, -1);
	
	Json::Value param;
	param["puzzlelistversion"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
	param["eventstagelistversion"] = NSDS_GI(kSDS_GI_eventListVersion_i);
	
	hspConnector::get()->command("getpuzzlelist", param, json_selector(this, TitleScene::resultGetPuzzleList));
}

void TitleScene::startGetCommonSetting()
{
	hspConnector::get()->command("getcommonsetting", Json::Value(), json_selector(this, TitleScene::resultGetCommonSetting));
}

void TitleScene::resultGetCommonSetting(Json::Value result_data)
{
	CCLog("resultGetCommonSetting data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	if(result_data["state"].asString() == "ok")
	{
		mySGD->setHeartMax(result_data["heartMax"].asInt());
		mySGD->setHeartCoolTime(result_data["heartCoolTime"].asInt());
		
		startGetUserData();
	}
	else
	{
		save_target = this;
		save_delegate = callfunc_selector(TitleScene::startGetCommonSetting);
		
		state_label->setString("게임 정보를 가져오는데 실패하였습니다.");
		
		CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
		CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
		s_replay->setColor(ccGRAY);
		
		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
		replay_item->setTag(kTitle_MT_replay);
		
		CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setPosition(ccp(240,160));
		addChild(replay_menu, 0, kTitle_MT_replay);
		
		is_menu_enable = true;
	}
}

void TitleScene::resultLogin( Json::Value result_data )
{
	CCLog("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());

	if(result_data["error"]["isSuccess"].asBool())
	{
		if(myLog->getLogCount() > 0)
		{
			myLog->sendLog(CCString::createWithFormat("ting_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
		}

		startGetCommonSetting();
	}
	else
	{
		Json::Value param;
		param["ManualLogin"] = true;

		hspConnector::get()->login(param, param, std::bind(&TitleScene::resultLogin, this, std::placeholders::_1));
	}
}

void TitleScene::startGetUserData()
{
	myLog->addLog(kLOG_network_getUserData, -1);

	state_label->setString("유저 정보를 가져오는 ing...");

	Json::Value param;
	param["memberID"] = hspConnector::get()->getKakaoID();
	hspConnector::get()->command("getUserData", param, json_selector(this, TitleScene::resultGetUserData));
}

void TitleScene::resultGetUserData( Json::Value result_data )
{
	CCLog("resultGetUserData data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());

	if(result_data["state"].asString() == "ok")
	{
		if(result_data["data"].isNull())
			startSaveUserData();
		else
			resultSaveUserData(result_data);
	}
	else
	{
		save_target = this;
		save_delegate = callfunc_selector(TitleScene::startGetUserData);

		state_label->setString("유저 정보를 가져오는데 실패하였습니다.");

		CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
		CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
		s_replay->setColor(ccGRAY);

		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
		replay_item->setTag(kTitle_MT_replay);

		CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setPosition(ccp(240,160));
		addChild(replay_menu, 0, kTitle_MT_replay);

		is_menu_enable = true;
	}
}

void TitleScene::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLog("edit begin");
}
void TitleScene::editBoxEditingDidEnd(CCEditBox* editBox)
{
	CCLog("edit end");
}
void TitleScene::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLog("edit changed : %s", text.c_str());
}
void TitleScene::editBoxReturn(CCEditBox* editBox)
{
	CCLog("edit return");
}

void TitleScene::resultSaveUserData( Json::Value result_data )
{
	CCLog("resultSaveUserData data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());

	if(result_data["state"].asString() == "ok")
	{
		myDSH->resetDSH();
		
		myDSH->loadAllUserData(result_data, card_data_load_list);

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
			
			CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(TitleScene::menuAction));
			ok_item->setTag(kTitle_MT_nick);
			
			CCMenu* ok_menu = CCMenu::createWithItem(ok_item);
			ok_menu->setPosition(ccp(370,130));
			addChild(ok_menu, 0, kTitle_MT_nick);
			
			is_menu_enable = true;
		}
		else
		{
			mySGD->resetHasGottenCards();
			
			if(card_data_load_list.size() > 0)
			{
				startGetCardsInfo();
			}
			else
			{
				state_label->setString("퍼즐 목록을 받아오는 ing...");
				startGetPuzzleList();
			}
		}
	}
	else
	{
		save_target = this;
		save_delegate = callfunc_selector(TitleScene::startSaveUserData);

		state_label->setString("유저 정보를 초기화 하는데 실패하였습니다.");

		CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
		CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
		s_replay->setColor(ccGRAY);

		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
		replay_item->setTag(kTitle_MT_replay);

		CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setPosition(ccp(240,160));
		addChild(replay_menu, 0, kTitle_MT_replay);

		is_menu_enable = true;
	}
}

void TitleScene::startGetCardsInfo()
{
	myLog->addLog(kLOG_network_loadCardData, -1);

	state_label->setString("카드 정보를 받아오는 ing...");
	Json::Value param;
	for(int i=0;i<card_data_load_list.size();i++)
		param["noList"][i] = card_data_load_list[i];
	hspConnector::get()->command("getcardlist", param, json_selector(this, TitleScene::resultLoadedCardData));
}

void TitleScene::resultLoadedCardData( Json::Value result_data )
{
	CCLog("resultLoadedCardData data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());

	if(result_data["state"].asString() == "ok")
	{
		Json::Value cards = result_data["list"];
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
			}
		}

		if(df_list.size() > 0) // need download
		{
			download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString(), "etc_font.fnt");
			download_state->setPosition(ccp(240,130));
			addChild(download_state);
			startDownloadCardImage();
		}
		else
		{
			state_label->setString("퍼즐 목록을 받아오는 ing...");
			startGetPuzzleList();
		}
	}
	else
	{
		save_target = this;
		save_delegate = callfunc_selector(TitleScene::startGetCardsInfo);

		state_label->setString("카드 정보를 가져오는데 실패하였습니다.");

		CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
		CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
		s_replay->setColor(ccGRAY);

		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
		replay_item->setTag(kTitle_MT_replay);

		CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setPosition(ccp(240,160));
		addChild(replay_menu, 0, kTitle_MT_replay);

		is_menu_enable = true;
	}
}

void TitleScene::successAction2()
{
	unschedule(schedule_selector(TitleScene::downloadingAction2));
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);

	if(ing_download_cnt >= df_list.size())
	{
		state_label->setString("카드 섬네일 만드는 중...");
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

		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());

		df_list.clear();
		cf_list.clear();
		download_state->removeFromParent();


		state_label->setString("퍼즐 목록을 받아오는 ing...");
		startGetPuzzleList();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		startDownload2();
	}
}

void TitleScene::failAction2()
{
	unschedule(schedule_selector(TitleScene::downloadingAction2));
	state_label->setString("이미지 정보 다운로드에 실패하였습니다.");

	save_target = this;
	save_delegate = callfunc_selector(TitleScene::startDownloadCardImage);

	CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
	CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
	s_replay->setColor(ccGRAY);

	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
	replay_item->setTag(kTitle_MT_replay);

	CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setPosition(ccp(240,160));
	addChild(replay_menu, 0, kTitle_MT_replay);

	is_menu_enable = true;
}

void TitleScene::startDownloadCardImage()
{
	myLog->addLog(kLOG_network_downloadCardImg, -1);

	state_label->setString("이미지 정보를 다운로드 합니다.");
	ing_download_cnt = 1;
	ing_download_per = 0;
	is_downloading = true;
	startDownload2();
}

void TitleScene::downloadingAction2()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();

	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;

	ing_download_per = t_per;

	download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
}

void TitleScene::startDownload2()
{
	CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());

	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(TitleScene::successAction2), this, callfunc_selector(TitleScene::failAction2));

	schedule(schedule_selector(TitleScene::downloadingAction2));
}

void TitleScene::startSaveUserData()
{
	myLog->addLog(kLOG_network_setUserData, -1);

	state_label->setString("유저 데이터를 초기화 ing...");

	myDSH->saveAllUserData(json_selector(this, TitleScene::resultSaveUserData));
}

void TitleScene::changeScene()
{
	CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
}

void TitleScene::resultGetPuzzleList( Json::Value result_data )
{
	CCLog("resultGetPuzzleList data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());

	if(result_data["state"].asString() == "ok")
	{
		successed_get_puzzle_list = true;

		if(result_data["puzzlelistversion"] > NSDS_GI(kSDS_GI_puzzleListVersion_i))
		{
			Json::Value puzzle_list = result_data["puzzlelist"];

			NSDS_SI(kSDS_GI_puzzleListCount_i, puzzle_list.size());

			int puzzle_cnt = puzzle_list.size();
			for(int i=0;i<puzzle_cnt;i++)
			{
				NSDS_SI(kSDS_GI_puzzleList_int1_no_i, i+1, puzzle_list[i]["no"].asInt());
				NSDS_SS(kSDS_GI_puzzleList_int1_title_s, i+1, puzzle_list[i]["title"].asString().c_str());
				NSDS_SI(kSDS_GI_puzzleList_int1_version_i, i+1, puzzle_list[i]["version"].asInt());
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_startStage_i, puzzle_list[i]["startStage"].asInt());
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_stageCount_i, puzzle_list[i]["stageCount"].asInt());
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_point_i, puzzle_list[i]["point"].asInt());
				NSDS_SI(puzzle_list[i]["no"].asInt(), kSDS_PZ_ticket_i, puzzle_list[i]["ticket"].asInt());

				Json::Value thumbnail = puzzle_list[i]["thumbnail"];
				if(NSDS_GS(kSDS_GI_puzzleList_int1_thumbnail_s, i+1) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("puzzleList%d_thumbnail.png", i+1)->getCString();
					t_df.key = CCSTR_CWF("puzzleList%d_thumbnail", i+1)->getCString();
					df_list.push_back(t_df);
					// ================================
				}
			}

			if(df_list.size() > 0)
				puzzlelist_download_version = result_data["puzzlelistversion"].asInt();
			else
				NSDS_SI(kSDS_GI_puzzleListVersion_i, result_data["puzzlelistversion"].asInt());
		}

		if(result_data["eventstagelistversion"] > NSDS_GI(kSDS_GI_eventListVersion_i))
		{
			Json::Value event_list = result_data["eventstagelist"];
			int el_length = event_list.size();
			NSDS_SI(kSDS_GI_eventCount_i, el_length);
			for(int i=0;i<el_length;i++)
			{
				int event_code = event_list[i]["no"].asInt();
				NSDS_SI(kSDS_GI_event_int1_code_i, i, event_code);
				Json::Value thumbnail = event_list[i]["thumbnail"];
				if(NSDS_GS(kSDS_GI_event_int1_thumbnail_s, i) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("event%d_thumbnail.png", i)->getCString();
					t_df.key = CCSTR_CWF("event%d_thumbnail", i)->getCString();
					ef_list.push_back(t_df);
					// ================================
				}
			}

			if(ef_list.size() > 0) // need download
				eventstagelist_download_version = result_data["eventstagelistversion"].asInt();
			else
				NSDS_SI(kSDS_GI_eventListVersion_i, result_data["eventstagelistversion"].asInt());
		}


		if(df_list.size() + ef_list.size() > 0)
		{
			download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString(), "etc_font.fnt");
			download_state->setPosition(ccp(240,130));
			addChild(download_state);
			startDownloadGameInfo();
		}
		else
			endingAction();

	}
	else
	{
		state_label->setString("퍼즐 목록을 받아오는데 실패하였습니다. 재시도 해주세요.");

		save_target = this;
		save_delegate = callfunc_selector(TitleScene::startGetPuzzleList);

		CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
		CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
		s_replay->setColor(ccGRAY);

		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
		replay_item->setTag(kTitle_MT_replay);

		CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setPosition(ccp(240,160));
		addChild(replay_menu, 0, kTitle_MT_replay);

		is_menu_enable = true;
	}
}

void TitleScene::startDownloadGameInfo()
{
	myLog->addLog(kLOG_network_downloadPuzzleEventImg, -1);

	state_label->setString("게임 정보를 받아옵니다.");
	ing_download_cnt = 1;
	ing_download_per = 0;
	is_downloading = true;
	startDownload();
}

void TitleScene::menuAction( CCObject* sender )
{
	if(!is_menu_enable)		return;

	is_menu_enable = false;

	int tag = ((CCNode*)sender)->getTag();

	if(tag == kTitle_MT_replay)
	{
		removeChildByTag(kTitle_MT_replay);
		(save_target->*save_delegate)();
	}
	else if(tag == kTitle_MT_nick)
	{
		string comp_not_ok = "";
		if(input_text->getText() != comp_not_ok)
		{
			myDSH->setStringForKey(kDSH_Key_nick, input_text->getText());
			setTouchEnabled(false);
			is_menu_enable = false;
			nick_back->removeFromParent();
			removeChildByTag(kTitle_MT_nick);
			input_text->removeFromParent();
			
			myDSH->saveUserData({kSaveUserData_Key_nick}, nullptr);
			
			mySGD->resetHasGottenCards();
			
			if(card_data_load_list.size() > 0)
			{
				startGetCardsInfo();
			}
			else
			{
				state_label->setString("퍼즐 목록을 받아오는 ing...");
				startGetPuzzleList();
			}
		}
		else
		{
			is_menu_enable = true;
		}
	}
	else if(tag >= kTitle_MT_puzzleBase)
	{
		tag -= kTitle_MT_puzzleBase;

		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, tag);
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
}

void TitleScene::successAction()
{
	unschedule(schedule_selector(TitleScene::downloadingAction));
	if(ing_download_cnt <= df_list.size())
		SDS_SS(kSDF_gameInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
	else
		SDS_SS(kSDF_gameInfo, ef_list[ing_download_cnt-df_list.size()-1].key, ef_list[ing_download_cnt-df_list.size()-1].img);

	if(ing_download_cnt >= df_list.size() + ef_list.size())
	{
		if(df_list.size() > 0)
			NSDS_SI(kSDS_GI_puzzleListVersion_i, puzzlelist_download_version);
		if(ef_list.size() > 0)
			NSDS_SI(kSDS_GI_eventListVersion_i, eventstagelist_download_version);
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString());
		state_label->setString("이벤트 정보 다운로드 완료.");

		endingAction();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString());
		startDownload();
	}
}

void TitleScene::failAction()
{
	unschedule(schedule_selector(TitleScene::downloadingAction));
	state_label->setString("게임 정보를 받아오는데 실패하였습니다.");

	save_target = this;
	save_delegate = callfunc_selector(TitleScene::startDownloadGameInfo);

	CCSprite* n_replay = CCSprite::create("cardsetting_zoom.png");
	CCSprite* s_replay = CCSprite::create("cardsetting_zoom.png");
	s_replay->setColor(ccGRAY);

	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(TitleScene::menuAction));
	replay_item->setTag(kTitle_MT_replay);

	CCMenu* replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setPosition(ccp(240,160));
	addChild(replay_menu, 0, kTitle_MT_replay);

	is_menu_enable = true;
}

void TitleScene::downloadingAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();

	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;

	ing_download_per = t_per;

	download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString());
}

void TitleScene::startDownload()
{
	if(ing_download_cnt <= df_list.size())
	{
		CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size,
			df_list[ing_download_cnt-1].filename, this, callfunc_selector(TitleScene::successAction), this, callfunc_selector(TitleScene::failAction));
	}
	else
	{
		CCLog("%d : %s", ing_download_cnt, ef_list[ing_download_cnt-df_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(ef_list[ing_download_cnt-df_list.size()-1].img, ef_list[ing_download_cnt-df_list.size()-1].size,
			ef_list[ing_download_cnt-df_list.size()-1].filename, this, callfunc_selector(TitleScene::successAction), this, callfunc_selector(TitleScene::failAction));
	}

	schedule(schedule_selector(TitleScene::downloadingAction));
}

void TitleScene::endingAction()
{
	CCDelayTime* t_delay = CCDelayTime::create(0.2f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleScene::changeScene));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
}
