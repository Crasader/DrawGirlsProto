//
//  TitleScene.h
//  DGproto
//
//  Created by LitQoo on 13. 10. 22..
//
//

#ifndef __DGproto__TitleScene__
#define __DGproto__TitleScene__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "PuzzleMapScene.h"
#include "hspConnector.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"
#include "LogData.h"
#include "StageImgLoader.h"

USING_NS_CC;
using namespace std;

enum TitleMenuTag
{
	kTitle_MT_replay = 1,
	kTitle_MT_puzzleBase = 10000
};

class TitleScene : public cocos2d::CCLayer
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(TitleScene);
	
private:
	
	CCLabelTTF* state_label;
	CCLabelBMFont* download_state;
	
	int ing_download_cnt;
	float ing_download_per;
	
	bool is_downloading;
	int puzzlelist_download_version;
	int eventstagelist_download_version;
	
	bool successed_get_puzzle_list;
	
	vector<int> card_data_load_list;
	vector<DownloadFile> df_list;
	vector<CopyFile> cf_list;
	vector<DownloadFile> ef_list;
	
	CCObject* save_target;
	SEL_CallFunc save_delegate;
	
	void startGetPuzzleList()
	{
		Json::Value param;
		param["puzzlelistversion"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
		param["eventstagelistversion"] = NSDS_GI(kSDS_GI_eventListVersion_i);
		hspConnector::get()->command("getpuzzlelist", param, json_selector(this, TitleScene::resultGetPuzzleList));
	}
	
	void resultLogin(Json::Value result_data)
	{
		CCLog("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		
		if(result_data["error"]["isSuccess"].asBool())
		{
			if(myLog->getLogCount() > 0)
			{
				myLog->sendLog(CCString::createWithFormat("ting_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
			}
			
			startGetUserData();
		}
		else
		{
			Json::Value param;
			param["ManualLogin"] = true;
			
			hspConnector::get()->login(param, param, std::bind(&TitleScene::resultLogin, this, std::placeholders::_1));
		}
	}
	
	void startGetUserData()
	{
		state_label->setString("유저 정보를 가져오는 ing...");
		
		Json::Value param;
		param["memberID"] = hspConnector::get()->getKakaoID();
		hspConnector::get()->command("getUserData", param, json_selector(this, TitleScene::resultGetUserData));
	}
	
	void resultGetUserData(Json::Value result_data)
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
	
	void resultSaveUserData(Json::Value result_data)
	{
		CCLog("resultSaveUserData data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		
		if(result_data["state"].asString() == "ok")
		{
			Json::Value data;
			Json::Reader reader;
			reader.parse(result_data["data"].asString(), data);
			
			CCLog("parse data : %s", GraphDogLib::JsonObjectToString(data).c_str());
			
			myDSH->setIntegerForKey(kDSH_Key_savedStar, data[myDSH->getKey(kDSH_Key_savedStar)].asInt());
			myDSH->setIntegerForKey(kDSH_Key_savedGold, data[myDSH->getKey(kDSH_Key_savedGold)].asInt());
			
			for(int i=kIC_attack;i<=kIC_randomChange;i++)
				myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, data[myDSH->getKey(kDSH_Key_haveItemCnt_int1)][i].asInt());
			
			
			
			/////// 카드 수집 정보 초기화 ////////////
			int before_card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
			myDSH->setIntegerForKey(kDSH_Key_cardTakeCnt, 0);
			for(int i=1;i<=before_card_take_cnt;i++)
			{
				int take_card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
				myDSH->setIntegerForKey(kDSH_Key_takeCardNumber_int1, i, 0);
				myDSH->setStringForKey(kDSH_Key_inputTextCard_int1, take_card_number, "");
				myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number, 0);
				myDSH->setIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number, 0);
			}
			////////// 카드 수집 정보 초기화 //////////
			
			
			
			myDSH->setIntegerForKey(kDSH_Key_cardTakeCnt, data[myDSH->getKey(kDSH_Key_cardTakeCnt)].asInt());
			int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
			
			for(int i=1;i<=card_take_cnt;i++)
			{
				int take_card_number = data[myDSH->getKey(kDSH_Key_takeCardNumber_int1)][i].asInt();
				myDSH->setIntegerForKey(kDSH_Key_takeCardNumber_int1, i, take_card_number);
				myDSH->setStringForKey(kDSH_Key_inputTextCard_int1, take_card_number, data[myDSH->getKey(kDSH_Key_inputTextCard_int1)][i].asString());
				myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number, data[myDSH->getKey(kDSH_Key_cardDurability_int1)][i].asInt());
				myDSH->setIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number, data[myDSH->getKey(kDSH_Key_hasGottenCard_int1)][i].asInt());
				
				if(NSDS_GS(kSDS_CI_int1_imgInfo_s, take_card_number) == "")
					card_data_load_list.push_back(take_card_number);
			}
			
			myDSH->setIntegerForKey(kDSH_Key_allHighScore, data[myDSH->getKey(kDSH_Key_allHighScore)].asInt());
			
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
	
	void startGetCardsInfo()
	{
		state_label->setString("카드 정보를 받아오는 ing...");
		Json::Value param;
		for(int i=0;i<card_data_load_list.size();i++)
			param["noList"][i] = card_data_load_list[i];
		hspConnector::get()->command("getcardlist", param, json_selector(this, TitleScene::resultLoadedCardData));
	}
	
	void resultLoadedCardData(Json::Value result_data)
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
				
				NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1);
				NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["stage"].asInt());
				NSDS_SI(t_card["stage"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
				
				Json::Value t_card_missile = t_card["missile"];
				NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str());
				NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt());
				NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt());
				NSDS_SI(kSDS_CI_int1_missile_speed_i, t_card["no"].asInt(), t_card_missile["speed"].asInt());
				
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
	
	void successAction2()
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
	
	void failAction2()
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
	
	void startDownloadCardImage()
	{
		state_label->setString("이미지 정보를 다운로드 합니다.");
		ing_download_cnt = 1;
		ing_download_per = 0;
		is_downloading = true;
		startDownload2();
	}
	
	void downloadingAction2()
	{
		float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
		
		if(t_per < 0.f)			t_per = 0.f;
		else if(t_per > 1.f)	t_per = 1.f;
		
		ing_download_per = t_per;
		
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
	}
	
	void startDownload2()
	{
		CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(TitleScene::successAction2), this, callfunc_selector(TitleScene::failAction2));
		
		schedule(schedule_selector(TitleScene::downloadingAction2));
	}
	
	void startSaveUserData()
	{
		state_label->setString("유저 데이터를 초기화 ing...");
		
		Json::Value param;
		param["memberID"] = hspConnector::get()->getKakaoID();
		
		Json::Value data;
		data[myDSH->getKey(kDSH_Key_savedStar)] = myDSH->getIntegerForKey(kDSH_Key_savedStar); // 1
		data[myDSH->getKey(kDSH_Key_savedGold)] = myDSH->getIntegerForKey(kDSH_Key_savedGold); // 1000
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
			data[myDSH->getKey(kDSH_Key_haveItemCnt_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, i); // 0
		
		data[myDSH->getKey(kDSH_Key_cardTakeCnt)] = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt); // 0
		int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt); /////////////////////////////
		for(int i=1;i<=card_take_cnt;i++)
		{
			int take_card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			data[myDSH->getKey(kDSH_Key_takeCardNumber_int1)][i] = take_card_number;
			data[myDSH->getKey(kDSH_Key_hasGottenCard_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number);
			data[myDSH->getKey(kDSH_Key_cardDurability_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number);
			data[myDSH->getKey(kDSH_Key_inputTextCard_int1)][i] = myDSH->getStringForKey(kDSH_Key_inputTextCard_int1, take_card_number);
		} /////////////////////////////////////////
		
		data[myDSH->getKey(kDSH_Key_allHighScore)] = myDSH->getIntegerForKey(kDSH_Key_allHighScore); // 0
		
		Json::FastWriter writer;
		param["data"] = writer.write(data);
		hspConnector::get()->command("updateUserData", param, json_selector(this, TitleScene::resultSaveUserData));
	}
	
	void changeScene()
	{
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}

	void resultGetPuzzleList(Json::Value result_data)
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
	
	void startDownloadGameInfo()
	{
		state_label->setString("게임 정보를 받아옵니다.");
		ing_download_cnt = 1;
		ing_download_per = 0;
		is_downloading = true;
		startDownload();
	}
	
	bool is_menu_enable;
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)		return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kTitle_MT_replay)
		{
			removeChildByTag(kTitle_MT_replay);
			(save_target->*save_delegate)();
		}
		else if(tag >= kTitle_MT_puzzleBase)
		{
			tag -= kTitle_MT_puzzleBase;
			
			myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, tag);
			CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
		}
	}
	
	
	void successAction()
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
	
	void failAction()
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
	
	void downloadingAction()
	{
		float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
		
		if(t_per < 0.f)			t_per = 0.f;
		else if(t_per > 1.f)	t_per = 1.f;
		
		ing_download_per = t_per;
		
		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size() + ef_list.size()))->getCString());
	}
	
	void startDownload()
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
	
	void endingAction()
	{
		CCDelayTime* t_delay = CCDelayTime::create(0.2f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleScene::changeScene));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
};

#endif /* defined(__DGproto__TitleScene__) */
