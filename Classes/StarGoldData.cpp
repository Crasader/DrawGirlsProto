//
//  StarGoldData.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 3. 12..
//
//

#include "StarGoldData.h"
#include "MyLocalization.h"
#include "DataStorageHub.h"
#include "AchieveNoti.h"
#include "HeartTime.h"
#include "FiveRocksCpp.h"

void StarGoldData::withdraw()
{
	is_option_tutorial = false;
	total_card_cnt = 0;
	
	loading_tip_back_number = 1;
	
	is_on_accountLinkLead = false;
	
	is_unlock_puzzle = 0;
	is_perfect_puzzle = 0;
	
	time_event_list.clear();
	
	setIntroducerID(0);
	
	endless_my_victory_on = false;
	is_morphing_noti = true;
	is_new_puzzle_card = false;
	
	star_label = NULL;
	gold_label = NULL;
	
	heart_time = NULL;
	
	has_gotten_cards.clear();
	puzzle_historys.clear();
	piece_historys.clear();
	goods_data.clear();
	userdata_storage.clear();
	
	at_time_show_firstPurchase = 0;
	at_time_show_emptyItem = 0;
	at_time_show_stupidNpuHelp = 0;
	at_time_show_eventRubyShop = 0;
	at_time_show_levelupGuide = 0;
	
	myDSH->setIntegerForKey(kDSH_Key_atTimeShowFirstPurchase, 0, false);
	myDSH->setIntegerForKey(kDSH_Key_atTimeShowEmptyItem, 0, false);
	myDSH->setIntegerForKey(kDSH_Key_atTimeShowStupidNpuHelp, 0, false);
	myDSH->setIntegerForKey(kDSH_Key_atTimeShowEventRubyShop, 0, false);
	myDSH->setIntegerForKey(kDSH_Key_atTimeShowLevelupGuide, 0, false);
	
	myDSH->fFlush();
}

string StarGoldData::getReplayKey(ReplayKey t_key)
{
	string return_value;
	if(t_key == kReplayKey_timeStamp)									return_value = "ts";
	else if(t_key == kReplayKey_timeStamp_characterPositionX)			return_value = "cpx";
	else if(t_key == kReplayKey_timeStamp_characterPositionY)			return_value = "cpy";
	else if(t_key == kReplayKey_timeStamp_boss)							return_value = "boss";
	else if(t_key == kReplayKey_timeStamp_boss_x)						return_value = "bpx";
	else if(t_key == kReplayKey_timeStamp_boss_y)						return_value = "bpy";
	else if(t_key == kReplayKey_timeStamp_monster)						return_value = "mob";
	else if(t_key == kReplayKey_timeStamp_monster_x)					return_value = "x";
	else if(t_key == kReplayKey_timeStamp_monster_y)					return_value = "y";
	else if(t_key == kReplayKey_timeStamp_mapIndex)						return_value = "mi";
	else if(t_key == kReplayKey_timeStamp_scoreIndex)					return_value = "si";
	else if(t_key == kReplayKey_timeStamp_isDie)						return_value = "id";
	else if(t_key == kReplayKey_timeStamp_isContinue)					return_value = "ic";
	else if(t_key == kReplayKey_timeStamp_isImageChange)				return_value = "iic";
	else if(t_key == kReplayKey_timeStamp_gameInfo)						return_value = "gi";
	else if(t_key == kReplayKey_timeStamp_scoreAttackDamage)			return_value = "sad";
	else if(t_key == kReplayKey_mapTime)								return_value = "mt";
	else if(t_key == kReplayKey_mapData)								return_value = "md";
	else if(t_key == kReplayKey_scoreTime)								return_value = "st";
	else if(t_key == kReplayKey_scoreData)								return_value = "sd";
	else if(t_key == kReplayKey_isChangedMap)							return_value = "icm";
	else if(t_key == kReplayKey_isChangedScore)							return_value = "ics";
	else if(t_key == kReplayKey_playIndex)								return_value = "pi";
	else if(t_key == kReplayKey_clearGrade)								return_value = "cg";
	else if(t_key == kReplayKey_gameTime)								return_value = "gt";
	else if(t_key == kReplayKey_takeGold)								return_value = "tg";
	else if(t_key == kReplayKey_totalScore)								return_value = "tts";
	else if(t_key == kReplayKey_originalScore)							return_value = "os";
	else if(t_key == kReplayKey_win)									return_value = "win";
	else if(t_key == kReplayKey_lose)									return_value = "lose";
	else if(t_key == kReplayKey_useItemCnt)								return_value = "uic";
	else if(t_key == kReplayKey_useItem_int1_itemCode)					return_value = "ui%dic";
	else if(t_key == kReplayKey_areaScore)								return_value = "as";
	else if(t_key == kReplayKey_damageScore)							return_value = "ds";
	else if(t_key == kReplayKey_comboScore)								return_value = "cs";
	else if(t_key == kReplayKey_lifeBonusCnt)							return_value = "lbc";
	else if(t_key == kReplayKey_takeArea)								return_value = "ta";
	else if(t_key == kReplayKey_scoreAttackedValue)						return_value = "sav";
	else if(t_key == kReplayKey_stageNo)								return_value = "srn";
	
	return return_value;
}

CCSprite* StarGoldData::getLoadingImg()
{
	if(after_loading == kImgType_Empty)
	{
		after_loading = getRandomImgType();
	}
	
	string filename;
	string ments;
//	if(after_loading == kImgType_specialMap)
//	{
//		filename = "loadingimg_specialMap.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading1);
//	}
//	else if(after_loading == kImgType_control1)
//	{
//		filename = "loadingimg_control1.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading2);
//	}
//	else if(after_loading == kImgType_control2)
//	{
//		filename = "loadingimg_control2.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading3);
//	}
//	else if(after_loading == kImgType_facebook1)
//	{
//		filename = "loadingimg_facebook1.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading4);
//	}
//	else if(after_loading == kImgType_facebook2)
//	{
//		filename = "loadingimg_facebook2.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading5);
//	}
//	else if(after_loading == kImgType_facebook3)
//	{
//		filename = "loadingimg_facebook3.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading6);
//	}
//	else if(after_loading == kImgType_gallery)
//	{
//		filename = "loadingimg_gallery.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading21);
//	}
//	else if(after_loading == kImgType_gallery1)
//	{
//		filename = "loadingimg_gallery1.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading22);
//	}
//	else if(after_loading == kImgType_option)
//	{
//		filename = "loadingimg_option.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading23);
//	}
//	else if(after_loading == kImgType_pet)
//	{
//		filename = "loadingimg_pet.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading24);
//	}
//	else if(after_loading == kImgType_petbuff)
//	{
//		filename = "loadingimg_petbuff.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading25);
//	}
//	else if(after_loading == kImgType_elementalPresentation1)
//	{
//		filename = "loadingimg_elementalPresentation1.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading17);
//	}
//	else if(after_loading == kImgType_elemental)
//	{
//		filename = "loadingimg_elemental.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading19);
//	}
//	else if(after_loading == kImgType_facebookLoginReward)
//	{
//		filename = "loadingimg_facebookLoginReward.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_loading20);
//	}
	
	after_loading = kImgType_Empty;
	
	CCSprite* t_loadingImg = CCSprite::create(filename.c_str());
	CCLabelTTF* t_loadingMents = CCLabelTTF::create(ments.c_str(), getFont().c_str(), 14, CCSizeMake(260, 300), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	t_loadingMents->setAnchorPoint(ccp(0,1));
	t_loadingMents->setPosition(ccp(30,117));
	t_loadingImg->addChild(t_loadingMents);
	
	return t_loadingImg;
}

string StarGoldData::getFont()
{
	string font_name;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//	font_name = "RixGoEB";
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	font_name = "fonts/RixGoEB.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB우왕
//#endif
//	

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	font_name = "jrNaver";
	//font_name = "RixGoEB";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "fonts/jrNaver.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
	
	
	return font_name;
}

string StarGoldData::getFont2() // Jrnaver
{

	string font_name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	font_name = "RixGoEB";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "fonts/RixGoEB.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
	
//	string font_name;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//	font_name = "jrNaver";
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	font_name = "fonts/jrNaver.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
//#endif
	
	return font_name;
}

StarGoldData* StarGoldData::sharedInstance()
{
	static StarGoldData* t_sg = NULL;
	if(t_sg == NULL)
	{
		t_sg = new StarGoldData();
		t_sg->myInit();
	}
	return t_sg;
}

void StarGoldData::resetLabels()
{
	star_label = NULL;
	gold_label = NULL;
	friend_point_label = NULL;
	ingame_gold_label = NULL;
}

void StarGoldData::setStarLabel( CCLabelBMFont* t_label )
{
	star_label = t_label;
}

void StarGoldData::setIngameGoldLabel( CCLabelBMFont* t_label )
{
	ingame_gold_label = t_label;
}

//int StarGoldData::getStar()
//{
//	return myDSH->getIntegerForKey(kDSH_Key_savedStar);
//}
//
//void StarGoldData::setStar( int t_star )
//{
//	if(myDSH->getIntegerForKey(kDSH_Key_savedStar) < t_star)
//	{
//		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
//		
//		for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
//		{
//			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
//			   t_star >= shared_acr->getCondition((AchievementCode)i))
//			{
//				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
//				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
//				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
//			}
//		}
//	}
//
//	if(star_label && myDSH->getIntegerForKey(kDSH_Key_savedStar) != t_star)
//		star_label->setString(CCString::createWithFormat("%d", t_star)->getCString());
//	
//	myDSH->setIntegerForKey(kDSH_Key_savedStar, t_star);
//}

void StarGoldData::setGoldLabel( CCLabelBMFont* t_label )
{
	gold_label = t_label;
}

void StarGoldData::setHeartTime(HeartTime *t_heart)
{
	heart_time = t_heart;
}

HeartTime* StarGoldData::getHeartTime()
{
	return heart_time;
}

//int StarGoldData::getGold()
//{
//	return myDSH->getIntegerForKey(kDSH_Key_savedGold);
//}
//void StarGoldData::setGold( int t_gold , bool is_write/* = true */)
//{
//	if(myDSH->getIntegerForKey(kDSH_Key_savedGold) < t_gold)
//	{
//		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
//		
//		for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
//		{
//			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
//			   t_gold >= shared_acr->getCondition((AchievementCode)i))
//			{
//				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
//				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
//				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
//			}
//		}
//	}
//
//	if(gold_label && myDSH->getIntegerForKey(kDSH_Key_savedGold) != t_gold)
//		gold_label->setString(CCString::createWithFormat("%d", t_gold)->getCString());
//	
//	myDSH->setIntegerForKey(kDSH_Key_savedGold, t_gold, is_write);
//}

int StarGoldData::getKeepGold()
{
	return keep_gold.getV();
}

void StarGoldData::setKeepGold( int t_gold )
{
	keep_gold = t_gold;
}

void StarGoldData::increaseCatchCumber()
{
	catch_cumber_count = catch_cumber_count.getV() + 1;
}
int StarGoldData::getCatchCumberCount()
{
	return catch_cumber_count.getV();
}

void StarGoldData::resetIngameDetailScore()
{
	area_score = 0;
	damage_score = 0;
	combo_score = 0;
}

void StarGoldData::setGameStart()
{
	gacha_item = kIC_emptyEnd;
	
	catch_cumber_count = 0;
	hunt_value = 0;
	
	is_clear_diary = false;
	is_safety_mode = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
	
	hell_play_time = 0.f;
	
	stage_attack_count = 0;
	
	ingame_gold = 0;
	
//	if(myDSH->getIntegerForKey(kDSH_Key_endPlayedStage) < mySD->getSilType())
//		myDSH->setIntegerForKey(kDSH_Key_endPlayedStage, mySD->getSilType());
	
	is_write_replay = true;
	
	replay_write_info.clear();
	replay_write_info[getReplayKey(kReplayKey_isChangedMap)] = true;
	replay_write_info[getReplayKey(kReplayKey_isChangedScore)] = true;
	
	int use_item_cnt = 0;
	for(int i=0;i<is_using_item.size();i++)
	{
		if(is_using_item[i])
		{
			use_item_cnt++;
			replay_write_info[CCString::createWithFormat(getReplayKey(kReplayKey_useItem_int1_itemCode).c_str(), use_item_cnt)->getCString()] = i;
		}
	}
	
	replay_write_info[getReplayKey(kReplayKey_useItemCnt)] = use_item_cnt;
	
	
//	ingame_before_stage_rank = myDSH->getIntegerForKey(kDSH_Key_stageClearRank_int1, mySD->getSilType());
	is_not_cleared_stage = !mySGD->isClearPiece(mySD->getSilType());
	
	mySD->startSetting();
	
	if(mySGD->isHasGottenCards(mySD->getSilType(), 1) > 0 || myDSH->getIntegerForKey(kDSH_Key_selectedCard) == NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))
		is_ingame_before_have_stage_cards[0] = true;
	else
		is_ingame_before_have_stage_cards[0] = false;
	if(mySGD->isHasGottenCards(mySD->getSilType(), 2) > 0 || myDSH->getIntegerForKey(kDSH_Key_selectedCard) == NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 2))
		is_ingame_before_have_stage_cards[1] = true;
	else
		is_ingame_before_have_stage_cards[1] = false;
	if(mySGD->isHasGottenCards(mySD->getSilType(), 3) > 0 || myDSH->getIntegerForKey(kDSH_Key_selectedCard) == NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3))
		is_ingame_before_have_stage_cards[2] = true;
	else
		is_ingame_before_have_stage_cards[2] = false;
	if(mySGD->isHasGottenCards(mySD->getSilType(), 4) > 0 || myDSH->getIntegerForKey(kDSH_Key_selectedCard) == NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 4))
		is_ingame_before_have_stage_cards[3] = true;
	else
		is_ingame_before_have_stage_cards[3] = false;
	
	is_showtime = false;
	is_exchanged = false;
	is_cleared = false;
	score = 0.f;
	base_score = 0.f;
	percentage = 0.f;
	keep_gold = myDSH->getIntegerForKey(kDSH_Key_savedGold);
	stage_grade = 0;
	before_rankup_stage_grade = 0;
	game_time = 0;
	game_use_time = 0;
	start_map_gacha_cnt = 0;
	start_map_gacha_use_gold_cnt = 0;
	clear_reward_gold = 0;
	
	damaged_score = 0;

	deque<int> card_options;
	deque<int>::iterator iter;
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	int ability_cnt = NSDS_GI(kSDS_CI_int1_abilityCnt_i, selected_card_number);
	
	for(int i=0;i<ability_cnt;i++)
		card_options.push_back(NSDS_GI(kSDS_CI_int1_ability_int2_type_i, selected_card_number, i));

	doubleItem_value = 0;
	if(isUsingItem(kIC_doubleItem))		doubleItem_value += mySD->getDoubleItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_doubleItem);
	if(iter != card_options.end())		doubleItem_value += NSDS_GI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, selected_card_number);

	longTime_value = 0;
	if(isUsingItem(kIC_longTime))		longTime_value += mySD->getLongTimeItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_longTime);
	if(iter != card_options.end())		longTime_value += NSDS_GI(kSDS_CI_int1_abilityLongTimeOptionSec_i, selected_card_number);

	baseSpeedUp_value = 0;
	if(isUsingItem(kIC_baseSpeedUp))	baseSpeedUp_value += mySD->getBaseSpeedUpItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_baseSpeedUp);
	if(iter != card_options.end())			baseSpeedUp_value += NSDS_GI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, selected_card_number);
}

void StarGoldData::gameClear( int t_grade, float t_score, float t_percentage, int t_game_time, int t_use_time, int t_total_time )
{
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		before_use_item[i] = is_using_item[i];
		is_using_item[i] = false;
	}

	is_cleared = true;
	stage_grade = t_grade;
	before_rankup_stage_grade = t_grade;
	percentage = t_percentage;

	base_score = t_score;
	
	float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
	
	if(mySD->getClearCondition() == kCLEAR_timeLimit)
	{
		play_limit_time -= mySD->getClearConditionTimeLimit();
	}
	
	score = t_score + ((play_limit_time-t_game_time)*500*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d));
	score = score.getV()*stage_grade.getV();

	game_time = t_game_time;
	game_use_time = t_use_time;
	
	if(is_write_replay)
	{
		replay_write_info[getReplayKey(kReplayKey_areaScore)] = area_score.getV();
		replay_write_info[getReplayKey(kReplayKey_damageScore)] = damage_score.getV();
		replay_write_info[getReplayKey(kReplayKey_comboScore)] = combo_score.getV();
		replay_write_info[getReplayKey(kReplayKey_takeArea)] = t_percentage;
	}
	
//	if(!mySGD->isClearPiece(mySD->getSilType()))
//	{
//		myDSH->setIntegerForKey(kDSH_Key_clearStageCnt, myDSH->getIntegerForKey(kDSH_Key_clearStageCnt)+1);
//		myDSH->setIntegerForKey(kDSH_Key_clearStageNumber_int1, myDSH->getIntegerForKey(kDSH_Key_clearStageCnt), mySD->getSilType());
//		myDSH->setBoolForKey(kDSH_Key_isClearStage_int1, mySD->getSilType(), true);
//		
//		PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
//		t_history.is_clear[t_grade-1] = true;
//		mySGD->setPieceHistory(t_history, nullptr);
//	}
	
	myGD->setIsGameover(true);
}

void StarGoldData::gameOver( float t_score, float t_percentage, int t_game_time )
{
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		before_use_item[i] = is_using_item[i];
		is_using_item[i] = false;
	}

	is_cleared = false;
	stage_grade = 0;
	before_rankup_stage_grade = 0;
	
	base_score = t_score;
	score = t_score;
	percentage = t_percentage;
//	game_time = t_game_time;
	
	float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
	
	if(mySD->getClearCondition() == kCLEAR_timeLimit)
	{
		play_limit_time -= mySD->getClearConditionTimeLimit();
	}
	
	game_time = play_limit_time;
	
	if(mySD->getClearCondition() != kCLEAR_hellMode)
		base_score = base_score.getV()*0;
	
	score = score.getV()*0;
	
	area_score = area_score.getV()*0;
	damage_score = damage_score.getV()*0;
	combo_score = combo_score.getV()*0;
	
	if(is_write_replay)
	{
		replay_write_info[getReplayKey(kReplayKey_areaScore)] = area_score.getV();
		replay_write_info[getReplayKey(kReplayKey_damageScore)] = damage_score.getV();
		replay_write_info[getReplayKey(kReplayKey_comboScore)] = combo_score.getV();
		replay_write_info[getReplayKey(kReplayKey_takeArea)] = t_percentage;
	}
	
	myGD->setIsGameover(true);
}

bool StarGoldData::getIsCleared()
{
	return is_cleared.getV();
}

float StarGoldData::getScore()
{
	return score.getV();
}

float StarGoldData::getBaseScore()
{
	return base_score.getV();
}

void StarGoldData::setPercentage(float t_f)
{
	percentage = t_f;
}
float StarGoldData::getPercentage()
{
	return percentage.getV();
}

void StarGoldData::setStageGrade(int t_i)
{
	if(stage_grade.getV() != 0)
		score = score.getV()/stage_grade.getV();
	
	stage_grade = t_i;
	score = score.getV()*stage_grade.getV();
}
int StarGoldData::getStageGrade()
{
	return stage_grade.getV();
}

void StarGoldData::setBeforeRankUpStageGrade(int t_i)
{
	before_rankup_stage_grade = t_i;
}
int StarGoldData::getBeforeRankUpStageGrade()
{
	return before_rankup_stage_grade.getV();
}

int StarGoldData::getStageGold()
{
	return ingame_gold.getV();
}

bool StarGoldData::getIsAfterSceneChapter()
{
	return is_after_scene_chapter;
}

void StarGoldData::setIsAfterSceneChapter( bool t_b )
{
	is_after_scene_chapter = t_b;
}

bool StarGoldData::getTutorialCleared()
{
	return is_tutorial_cleared;
}

ImgType StarGoldData::getRandomImgType()
{
	//		if(!FBConnector::get()->isUsed() && rand()%5 == 0)
	//		{
	//			return kImgType_facebookLoginReward;
	//		}
	//		else
	//		{
	int t_rand = rand()%kImgType_elemental + kImgType_specialMap;
	return ImgType(t_rand);
	//		}
}

void StarGoldData::setLoadingImgType( ImgType t_t )
{
	after_loading = t_t;
}

void StarGoldData::setOpenShopTD( CCObject* t_target, SEL_CallFuncI t_delegate )
{
	shop_opener = t_target;
	open_shop_delegate = t_delegate;
}

void StarGoldData::openShop( int t_code )
{
	(shop_opener->*open_shop_delegate)(t_code);
}

void StarGoldData::setAfterScene( SceneCode t_s )
{
	after_scene = t_s;
}

SceneCode StarGoldData::getAfterScene()
{
	return after_scene;
}

void StarGoldData::setTargetDelegate( CCObject* t_t, SEL_CallFuncB t_d )
{
	graphDog_target = t_t;
	graphDog_delegate = t_d;
}

int StarGoldData::getGameTime()
{
	return game_time.getV();
}

int StarGoldData::getGameUseTime()
{
	return game_use_time.getV();
}

void StarGoldData::setStartRequestsData( Json::Value result_data )
{
	startRequestsData = result_data;
}

Json::Value StarGoldData::getStartRequestsData()
{
	return startRequestsData;
}

void StarGoldData::setCollectionStarter( CollectionStarterType t_type )
{
	collection_starter = t_type;
}

CollectionStarterType StarGoldData::getCollectionStarter()
{
	CollectionStarterType r_value = collection_starter;
	collection_starter = kCST_basic;
	return r_value;
}

bool StarGoldData::isBeforeUseItem( ITEM_CODE t_i )
{
	return before_use_item[t_i];
}

void StarGoldData::setBeforeUseItem(ITEM_CODE t_i, bool t_b)
{
	before_use_item[t_i] = t_b;
}

bool StarGoldData::isUsingItem( ITEM_CODE t_i )
{
	return is_using_item[t_i];
}

void StarGoldData::setIsUsingItem( ITEM_CODE t_i, bool t_b )
{
	is_using_item[t_i] = t_b;
}

void StarGoldData::resetUsingItem()
{
	is_using_item.clear();
}

int StarGoldData::getNextCardNumber( int recent_card_number )
{
	int find_index;
	bool is_normal = false;
	bool is_event = false;
	bool is_special = false;
	
	string card_type = NSDS_GS(kSDS_CI_int1_category_s, recent_card_number);
	
	function<void(vector<CollectionCardInfo>&, function<void(int)>)> find_func = [&](vector<CollectionCardInfo>& target_vector, function<void(int)> found_func)
	{
		bool is_found = false;
		
		for(int i=0;!is_found && i<target_vector.size();i++)
		{
			if(target_vector[i].grade1_card_number.getV() == recent_card_number ||
			   target_vector[i].grade2_card_number.getV() == recent_card_number ||
			   target_vector[i].grade3_card_number.getV() == recent_card_number ||
			   target_vector[i].grade4_card_number.getV() == recent_card_number)
			{
				is_found = true;
				found_func(i);
			}
		}
	};
	
	if(card_type == "gift")
	{
		find_func(special_cards, [&](int t_index)
				  {
					  is_special = true;
					  find_index = t_index;
				  });
	}
	else if(card_type == "ePuzzle" || card_type == "event")
	{
		find_func(event_puzzle_cards, [&](int t_index)
				  {
					  is_event = true;
					  find_index = t_index;
				  });
	}
	else if(card_type == "nPuzzle" || card_type == "normal" || card_type == "special")
	{
		find_func(normal_puzzle_cards, [&](int t_index)
				  {
					  is_normal = true;
					  find_index = t_index;
				  });
	}
	
	if(is_normal)
	{
		if(normal_puzzle_cards.size() == 1)
			return -1;
		else
		{
			if(find_index == normal_puzzle_cards.size()-1)
				return normal_puzzle_cards[0].getHighCardNumber();
			else
				return normal_puzzle_cards[find_index+1].getHighCardNumber();
		}
	}
	else
	{
		if(event_puzzle_cards.size() + special_cards.size() == 1)
			return -1;
		else
		{
			if(is_event)
			{
				if(find_index == event_puzzle_cards.size()-1)
				{
					if(!special_cards.empty())
						return special_cards[0].getHighCardNumber();
					else
						return event_puzzle_cards[0].getHighCardNumber();
				}
				else
					return event_puzzle_cards[find_index+1].getHighCardNumber();
			}
			else
			{
				if(find_index == special_cards.size()-1)
				{
					if(!event_puzzle_cards.empty())
						return event_puzzle_cards[0].getHighCardNumber();
					else
						return special_cards[0].getHighCardNumber();
				}
				else
					return special_cards[find_index+1].getHighCardNumber();
			}
		}
	}
	
//	int t_size = has_gotten_cards.size();
//
//	if(t_size == 1)
//		return -1;
//
//	int found_number = -1;
//	for(int i=0;i<t_size;i++)
//	{
//		if(recent_card_number == has_gotten_cards[i].card_number.getV())
//		{
//			found_number = i;
//			break;
//		}
//	}
//
//	if(found_number == -1) // not found
//		return -1;
//
//	if(found_number >= t_size-1)
//		return has_gotten_cards[0].card_number.getV();
//	else
//		return has_gotten_cards[found_number+1].card_number.getV();
}

int StarGoldData::getNextStageCardNumber( int recent_card_number )
{
//	int find_index = -1;
//	bool is_normal = false;
//	bool is_event = false;
//	bool is_special = false;
//	
//	string card_type = NSDS_GS(kSDS_CI_int1_category_s, recent_card_number);
//	
//	function<void(vector<CollectionCardInfo>&, function<void(int)>)> find_func = [&](vector<CollectionCardInfo>& target_vector, function<void(int)> found_func)
//	{
//		bool is_found = false;
//		
//		for(int i=0;!is_found && i<target_vector.size();i++)
//		{
//			if(target_vector[i].grade1_card_number.getV() == recent_card_number ||
//			   target_vector[i].grade2_card_number.getV() == recent_card_number ||
//			   target_vector[i].grade3_card_number.getV() == recent_card_number ||
//			   target_vector[i].grade4_card_number.getV() == recent_card_number)
//			{
//				is_found = true;
//				found_func(i);
//			}
//		}
//	};
//	
//	if(card_type == "gift")
//	{
//		find_func(special_cards, [&](int t_index)
//				  {
//					  is_special = true;
//					  find_index = t_index;
//				  });
//	}
//	else if(card_type == "ePuzzle" || card_type == "event")
//	{
//		find_func(event_puzzle_cards, [&](int t_index)
//				  {
//					  is_event = true;
//					  find_index = t_index;
//				  });
//	}
//	else if(card_type == "nPuzzle" || card_type == "normal" || card_type == "special")
//	{
//		find_func(normal_puzzle_cards, [&](int t_index)
//				  {
//					  is_normal = true;
//					  find_index = t_index;
//				  });
//	}
//	
//	if(find_index == -1)
//		return -1;
//	
//	if(is_normal)
//	{
//		if(normal_puzzle_cards.size() == 1)
//			return -1;
//		else
//		{
//			if(find_index == normal_puzzle_cards.size()-1)
//				return normal_puzzle_cards[0].getHighCardNumber();
//			else
//				return normal_puzzle_cards[find_index+1].getHighCardNumber();
//		}
//	}
//	else
//	{
//		if(event_puzzle_cards.size() + special_cards.size() == 1)
//			return -1;
//		else
//		{
//			if(is_event)
//			{
//				if(find_index == event_puzzle_cards.size()-1)
//				{
//					if(!special_cards.empty())
//						return special_cards[0].getHighCardNumber();
//					else
//						return event_puzzle_cards[0].getHighCardNumber();
//				}
//				else
//					return event_puzzle_cards[find_index+1].getHighCardNumber();
//			}
//			else
//			{
//				if(find_index == special_cards.size()-1)
//				{
//					if(!event_puzzle_cards.empty())
//						return event_puzzle_cards[0].getHighCardNumber();
//					else
//						return special_cards[0].getHighCardNumber();
//				}
//				else
//					return special_cards[find_index+1].getHighCardNumber();
//			}
//		}
//	}
	
	deque<CardSortInfo> t_card_list = has_gotten_cards;
	
	struct t_CardSortDefault{
		bool operator() (const CardSortInfo& a, const CardSortInfo& b)
		{
			return a.card_number.getV() < b.card_number.getV();
		}
	} pred;
	
	sort(t_card_list.begin(), t_card_list.end(), pred);
	
	for(int i=0;i<t_card_list.size();i++)
	{
		if(t_card_list[i].card_number.getV() == recent_card_number)
		{
			if(i == t_card_list.size()-1)
			{
				// last
				if(t_card_list.size() == 1)
					return -1;
				else
					return t_card_list[0].card_number.getV();
			}
			else
			{
				return t_card_list[i+1].card_number.getV();
			}
		}
	}
}

int StarGoldData::getPreCardNumber( int recent_card_number )
{
	int find_index;
	bool is_normal = false;
	bool is_event = false;
	bool is_special = false;
	
	string card_type = NSDS_GS(kSDS_CI_int1_category_s, recent_card_number);
	
	function<void(vector<CollectionCardInfo>&, function<void(int)>)> find_func = [&](vector<CollectionCardInfo>& target_vector, function<void(int)> found_func)
	{
		bool is_found = false;
		
		for(int i=0;!is_found && i<target_vector.size();i++)
		{
			if(target_vector[i].grade1_card_number.getV() == recent_card_number ||
			   target_vector[i].grade2_card_number.getV() == recent_card_number ||
			   target_vector[i].grade3_card_number.getV() == recent_card_number ||
			   target_vector[i].grade4_card_number.getV() == recent_card_number)
			{
				is_found = true;
				found_func(i);
			}
		}
	};
	
	if(card_type == "gift")
	{
		find_func(special_cards, [&](int t_index)
				  {
					  is_special = true;
					  find_index = t_index;
				  });
	}
	else if(card_type == "ePuzzle" || card_type == "event")
	{
		find_func(event_puzzle_cards, [&](int t_index)
				  {
					  is_event = true;
					  find_index = t_index;
				  });
	}
	else if(card_type == "nPuzzle" || card_type == "normal" || card_type == "special")
	{
		find_func(normal_puzzle_cards, [&](int t_index)
				  {
					  is_normal = true;
					  find_index = t_index;
				  });
	}
	
	if(is_normal)
	{
		if(normal_puzzle_cards.size() == 1)
			return -1;
		else
		{
			if(find_index == 0)
				return normal_puzzle_cards[normal_puzzle_cards.size()-1].getHighCardNumber();
			else
				return normal_puzzle_cards[find_index-1].getHighCardNumber();
		}
	}
	else
	{
		if(event_puzzle_cards.size() + special_cards.size() == 1)
			return -1;
		else
		{
			if(is_event)
			{
				if(find_index == 0)
				{
					if(!special_cards.empty())
						return special_cards[special_cards.size()-1].getHighCardNumber();
					else
						return event_puzzle_cards[event_puzzle_cards.size()-1].getHighCardNumber();
				}
				else
					return event_puzzle_cards[find_index-1].getHighCardNumber();
			}
			else
			{
				if(find_index == 0)
				{
					if(!event_puzzle_cards.empty())
						return event_puzzle_cards[event_puzzle_cards.size()-1].getHighCardNumber();
					else
						return special_cards[special_cards.size()-1].getHighCardNumber();
				}
				else
					return special_cards[find_index-1].getHighCardNumber();
			}
		}
	}
	
	
	
	
	
//	int t_size = has_gotten_cards.size();
//
//	if(t_size == 1)
//		return -1;
//
//	int found_number = -1;
//	for(int i=0;i<t_size;i++)
//	{
//		if(recent_card_number == has_gotten_cards[i].card_number.getV())
//		{
//			found_number = i;
//			break;
//		}
//	}
//
//	if(found_number == -1) // not found
//		return -1;
//
//	if(found_number <= 0)
//		return has_gotten_cards[t_size-1].card_number.getV();
//	else
//		return has_gotten_cards[found_number-1].card_number.getV();
}

vector<int> StarGoldData::getCollectionCardSet(int recent_card_number)
{
	int find_index;
	bool is_normal = false;
	bool is_event = false;
	bool is_special = false;
	
	string card_type = NSDS_GS(kSDS_CI_int1_category_s, recent_card_number);
	
	function<void(vector<CollectionCardInfo>&, function<void(int)>)> find_func = [&](vector<CollectionCardInfo>& target_vector, function<void(int)> found_func)
	{
		bool is_found = false;
		
		for(int i=0;!is_found && i<target_vector.size();i++)
		{
			if(target_vector[i].grade1_card_number.getV() == recent_card_number ||
			   target_vector[i].grade2_card_number.getV() == recent_card_number ||
			   target_vector[i].grade3_card_number.getV() == recent_card_number ||
			   target_vector[i].grade4_card_number.getV() == recent_card_number)
			{
				is_found = true;
				found_func(i);
			}
		}
	};
	
	if(card_type == "gift")
	{
		find_func(special_cards, [&](int t_index)
				  {
					  is_special = true;
					  find_index = t_index;
				  });
	}
	else if(card_type == "ePuzzle" || card_type == "event")
	{
		find_func(event_puzzle_cards, [&](int t_index)
				  {
					  is_event = true;
					  find_index = t_index;
				  });
	}
	else if(card_type == "nPuzzle" || card_type == "normal" || card_type == "special")
	{
		find_func(normal_puzzle_cards, [&](int t_index)
				  {
					  is_normal = true;
					  find_index = t_index;
				  });
	}
	
	if(is_normal)
		return normal_puzzle_cards[find_index].getCardSet();
	else if(is_event)
		return event_puzzle_cards[find_index].getCardSet();
	else if(is_special)
		return special_cards[find_index].getCardSet();
}

int StarGoldData::getPreStageCardNumber( int recent_card_number )
{
//	int find_index;
//	bool is_normal = false;
//	bool is_event = false;
//	bool is_special = false;
//	
//	string card_type = NSDS_GS(kSDS_CI_int1_category_s, recent_card_number);
//	
//	function<void(vector<CollectionCardInfo>&, function<void(int)>)> find_func = [&](vector<CollectionCardInfo>& target_vector, function<void(int)> found_func)
//	{
//		bool is_found = false;
//		
//		for(int i=0;!is_found && i<target_vector.size();i++)
//		{
//			if(target_vector[i].grade1_card_number.getV() == recent_card_number ||
//			   target_vector[i].grade2_card_number.getV() == recent_card_number ||
//			   target_vector[i].grade3_card_number.getV() == recent_card_number ||
//			   target_vector[i].grade4_card_number.getV() == recent_card_number)
//			{
//				is_found = true;
//				found_func(i);
//			}
//		}
//	};
//	
//	if(card_type == "gift")
//	{
//		find_func(special_cards, [&](int t_index)
//				  {
//					  is_special = true;
//					  find_index = t_index;
//				  });
//	}
//	else if(card_type == "ePuzzle" || card_type == "event")
//	{
//		find_func(event_puzzle_cards, [&](int t_index)
//				  {
//					  is_event = true;
//					  find_index = t_index;
//				  });
//	}
//	else if(card_type == "nPuzzle" || card_type == "normal" || card_type == "special")
//	{
//		find_func(normal_puzzle_cards, [&](int t_index)
//				  {
//					  is_normal = true;
//					  find_index = t_index;
//				  });
//	}
//	
//	if(is_normal)
//	{
//		if(normal_puzzle_cards.size() == 1)
//			return -1;
//		else
//		{
//			if(find_index == 0)
//				return normal_puzzle_cards[normal_puzzle_cards.size()-1].getHighCardNumber();
//			else
//				return normal_puzzle_cards[find_index-1].getHighCardNumber();
//		}
//	}
//	else
//	{
//		if(event_puzzle_cards.size() + special_cards.size() == 1)
//			return -1;
//		else
//		{
//			if(is_event)
//			{
//				if(find_index == 0)
//				{
//					if(!special_cards.empty())
//						return special_cards[special_cards.size()-1].getHighCardNumber();
//					else
//						return event_puzzle_cards[event_puzzle_cards.size()-1].getHighCardNumber();
//				}
//				else
//					return event_puzzle_cards[find_index-1].getHighCardNumber();
//			}
//			else
//			{
//				if(find_index == 0)
//				{
//					if(!event_puzzle_cards.empty())
//						return event_puzzle_cards[event_puzzle_cards.size()-1].getHighCardNumber();
//					else
//						return special_cards[special_cards.size()-1].getHighCardNumber();
//				}
//				else
//					return special_cards[find_index-1].getHighCardNumber();
//			}
//		}
//	}
	
	
	
	deque<CardSortInfo> t_card_list = has_gotten_cards;
	
	struct t_CardSortDefault{
		bool operator() (const CardSortInfo& a, const CardSortInfo& b)
		{
			return a.card_number.getV() < b.card_number.getV();
		}
	} pred;
	
	sort(t_card_list.begin(), t_card_list.end(), pred);
	
	for(int i=0;i<t_card_list.size();i++)
	{
		if(t_card_list[i].card_number.getV() == recent_card_number)
		{
			if(i == 0)
			{
				// first
				if(t_card_list.size() == 1)
					return -1;
				else
					return t_card_list[t_card_list.size()-1].card_number.getV();
			}
			else
			{
				return t_card_list[i-1].card_number.getV();
			}
		}
	}
}

void StarGoldData::changeSortType( CardSortType t_type )
{
	myDSH->setIntegerForKey(kDSH_Key_cardSortType, t_type);

	if(t_type == kCST_default)
	{
		struct t_CardSortDefault{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.card_number.getV() < b.card_number.getV();
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() > b.take_number.getV();
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number.getV() < b.take_number.getV();
			}
		} pred;
		
		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_gradeUp) // rank
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() > b.rank.getV();
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_gradeDown) // rank
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank.getV() < b.rank.getV();
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
}

void StarGoldData::addHasGottenCardNumber( int card_number )
{
//	int take_number = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number);
	
	bool is_found = false;
	for(int i=0;!is_found && i<has_gotten_cards.size();i++)
	{
		CardSortInfo t_info = has_gotten_cards[i];
		if(t_info.card_number.getV() == card_number)
		{
			has_gotten_cards[i].count = t_info.count.getV() + getClearTakeCardCnt();
			is_found = true;
		}
	}
	
	if(!is_found)
	{
		CardSortInfo t_info;
		t_info.card_number = card_number;
		t_info.take_number = has_gotten_cards.size()+1;
		t_info.grade = NSDS_GI(kSDS_CI_int1_grade_i, t_info.card_number.getV());
		t_info.rank = NSDS_GI(kSDS_CI_int1_rank_i, t_info.card_number.getV());
		t_info.count = getClearTakeCardCnt();
		
		has_gotten_cards.push_back(t_info);
		
		changeSortType(CardSortType(myDSH->getIntegerForKey(kDSH_Key_cardSortType)));
		
		CCLOG("input %d, sort", card_number);
		for(int i=0;i<has_gotten_cards.size();i++)
		{
			CCLOG("%d", has_gotten_cards[i].card_number.getV());
		}
	}
}

int StarGoldData::getHasGottenCardsDataCardNumber( int index )
{
	return has_gotten_cards[index].card_number.getV();
}

CardSortInfo StarGoldData::getHasGottenCardDataForCardNumber(int t_card_number)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number.getV() == t_card_number)
			return *i;
	}
	
	return CardSortInfo();
}

CardSortInfo StarGoldData::getHasGottenCardData(int index)
{
	return has_gotten_cards[index];
}

int StarGoldData::getHasGottenCardsSize()
{
	return int(has_gotten_cards.size());
}

int StarGoldData::getDoubleItemValue()
{
	return doubleItem_value;
}

int StarGoldData::getLongTimeValue()
{
	return longTime_value;
}

int StarGoldData::getBaseSpeedUpValue()
{
	return baseSpeedUp_value;
}

//int StarGoldData::getSubSmallSizeValue()
//{
//	return subSmallSize_value;
//}
//
//int StarGoldData::getSmallAreaValue()
//{
//	return smallArea_value;
//}
//
//int StarGoldData::getWidePerfectValue()
//{
//	return widePerfect_value;
//}

int StarGoldData::getStartMapGachaCnt()
{
	return start_map_gacha_cnt;
}

void StarGoldData::startMapGachaOn()
{
	start_map_gacha_cnt++;
	keep_gold = myDSH->getIntegerForKey(kDSH_Key_savedGold);
}

string StarGoldData::getCardComment(int t_card_number)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number.getV() == t_card_number)
			return i->user_ment.getV();
	}
	
	return "";
}

void StarGoldData::setCardComment(int t_card_number, string comment)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number.getV() == t_card_number)
			i->user_ment = comment;
	}
}

bool StarGoldData::isCardMorphing(int card_number)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number.getV() == card_number)
			return i->is_morphing.getV();
	}
	
	return false;
}

void StarGoldData::onCardMorphing(int card_number)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number.getV() == card_number)
			i->is_morphing = true;
	}
}

int StarGoldData::isHasGottenCards(int t_card_number)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number.getV() == t_card_number && i->count.getV() > 0)
			return i->card_number.getV();
	}
	
	return 0;
}

int StarGoldData::isHasGottenCardsToIndex(int t_card_number)
{
	for(int i = 0;i<has_gotten_cards.size();i++)
	{
		if(has_gotten_cards[i].card_number.getV() == t_card_number && has_gotten_cards[i].count.getV() > 0)
			return i;
	}
	
	return -1;
}

int StarGoldData::isHasGottenCards( int t_stage, int t_grade )
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(NSDS_GI(kSDS_CI_int1_stage_i, i->card_number.getV()) == t_stage && i->grade.getV() == t_grade && i->count.getV() > 0)
			return i->card_number.getV();
	}

	return 0;
}

void StarGoldData::resetHasGottenCards()
{
	for(int i=0;i<has_gotten_cards.size();i++)
	{
		int card_number = has_gotten_cards[i].card_number.getV();
		has_gotten_cards[i].grade = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
		has_gotten_cards[i].rank = NSDS_GI(kSDS_CI_int1_rank_i, card_number);
	}
	
	changeSortType(CardSortType(myDSH->getIntegerForKey(kDSH_Key_cardSortType)));
}

void StarGoldData::initCollectionBook()
{
	normal_puzzle_cards.clear();
	event_puzzle_cards.clear();
	special_cards.clear();
	
	map<int, IntPoint> puzzle_stage_info;
	puzzle_stage_info.clear();
	
	int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	for(int i=0;i<puzzle_count;i++)
	{
		int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
		puzzle_stage_info[puzzle_number] = IntPoint(NSDS_GI(puzzle_number, kSDS_PZ_startStage_i), NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i));
	}
	
	function<void(int)> after_log = [&](int t_type)
	{
		CCLOG("=============================[after lists]===========================");
		if(t_type == 0 || t_type == 1)
		{
			CCLOG("normal_puzzle_cards");
			for(int i=0;i<normal_puzzle_cards.size();i++)
			{
				CCLOG("puzzle_number : %d , stage_number : %d, is_event_puzzle : %d, card1 : %d, card2 : %d, card3 : %d, card4 : %d", normal_puzzle_cards[i].puzzle_number.getV(), normal_puzzle_cards[i].stage_number.getV(), normal_puzzle_cards[i].is_puzzle_card.getV(), normal_puzzle_cards[i].grade1_card_number.getV(), normal_puzzle_cards[i].grade2_card_number.getV(), normal_puzzle_cards[i].grade3_card_number.getV(), normal_puzzle_cards[i].grade4_card_number.getV());
			}
		}
		
		if(t_type == 0 || t_type == 2)
		{
			CCLOG("event_puzzle_cards");
			for(int i=0;i<event_puzzle_cards.size();i++)
			{
				CCLOG("puzzle_number : %d , stage_number : %d, is_event_puzzle : %d, card1 : %d, card2 : %d, card3 : %d, card4 : %d", event_puzzle_cards[i].puzzle_number.getV(), event_puzzle_cards[i].stage_number.getV(), event_puzzle_cards[i].is_puzzle_card.getV(), event_puzzle_cards[i].grade1_card_number.getV(), event_puzzle_cards[i].grade2_card_number.getV(), event_puzzle_cards[i].grade3_card_number.getV(), event_puzzle_cards[i].grade4_card_number.getV());
			}
		}
		
		if(t_type == 0 || t_type == 3)
		{
			CCLOG("special_cards");
			for(int i=0;i<special_cards.size();i++)
			{
				CCLOG("puzzle_number : %d , stage_number : %d, is_event_puzzle : %d, card1 : %d, card2 : %d, card3 : %d, card4 : %d", special_cards[i].puzzle_number.getV(), special_cards[i].stage_number.getV(), special_cards[i].is_puzzle_card.getV(), special_cards[i].grade1_card_number.getV(), special_cards[i].grade2_card_number.getV(), special_cards[i].grade3_card_number.getV(), special_cards[i].grade4_card_number.getV());
			}
		}
		
		CCLOG("=============================[end]===========================");
	};
	
	function<void(vector<CollectionCardInfo>&, int, int, function<void()>)> t_puzzle_func = [](vector<CollectionCardInfo>& target_vector, int t_puzzle_number, int t_card_number, function<void()> log_func)
	{
		bool is_insert = false;
		bool is_found = false;
		for(int j=0;!is_found && j<target_vector.size();j++)
		{
			if(target_vector[j].puzzle_number.getV() == t_puzzle_number && target_vector[j].is_puzzle_card.getV())
			{
				is_found = true;
				
				if(t_card_number == NSDS_GI(t_puzzle_number, kSDS_PZ_clearCard_i))
					target_vector[j].grade3_card_number = t_card_number;
				else if(t_card_number == NSDS_GI(t_puzzle_number, kSDS_PZ_perfectCard_i))
					target_vector[j].grade4_card_number = t_card_number;
				is_insert = true;
				log_func();
			}
		}
		
		if(!is_found)
		{
			// find insert point
			for(auto iter = target_vector.begin();iter!=target_vector.end();iter++)
			{
				if(iter->puzzle_number.getV() > t_puzzle_number)
				{
					CollectionCardInfo t_info;
					t_info.puzzle_number = t_puzzle_number;
					t_info.grade1_card_number = -2;
					t_info.grade2_card_number = -2;
					
					if(t_card_number == NSDS_GI(t_puzzle_number, kSDS_PZ_clearCard_i))
						t_info.grade3_card_number = t_card_number;
					else if(t_card_number == NSDS_GI(t_puzzle_number, kSDS_PZ_perfectCard_i))
						t_info.grade4_card_number = t_card_number;
					
					target_vector.insert(iter, t_info);
					is_insert = true;
					log_func();
					break;
				}
			}
		}
		
		if(!is_insert)
		{
			CollectionCardInfo t_info;
			t_info.puzzle_number = t_puzzle_number;
			t_info.grade1_card_number = -2;
			t_info.grade2_card_number = -2;
			
			if(t_card_number == NSDS_GI(t_puzzle_number, kSDS_PZ_clearCard_i))
				t_info.grade3_card_number = t_card_number;
			else if(t_card_number == NSDS_GI(t_puzzle_number, kSDS_PZ_perfectCard_i))
				t_info.grade4_card_number = t_card_number;
			
			target_vector.push_back(t_info);
			is_insert = true;
			log_func();
		}
	};
	
	function<void(vector<CollectionCardInfo>&, vector<CollectionCardInfo>::iterator&, int, int, int, function<void()>)> insert_func = [](vector<CollectionCardInfo>& target_vector, vector<CollectionCardInfo>::iterator& iter, int t_puzzle_number, int t_stage_number, int t_card_number, function<void()> log_func)
	{
		CollectionCardInfo t_info;
		t_info.puzzle_number = t_puzzle_number;
		t_info.stage_number = t_stage_number;
		
		int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
		
		if(card_grade == 1)
			t_info.grade1_card_number = t_card_number;
		else if(card_grade == 2)
			t_info.grade2_card_number = t_card_number;
		else if(card_grade == 3)
			t_info.grade3_card_number = t_card_number;
		else if(card_grade == 4)
			t_info.grade4_card_number = t_card_number;
		
		target_vector.insert(iter, t_info);
		log_func();
	};
	
	
	function<void(vector<CollectionCardInfo>&, int, int, int, function<void(vector<CollectionCardInfo>&, vector<CollectionCardInfo>::iterator&, int, int, int, function<void()>)>, function<void()>)> puzzle_find_func = [](vector<CollectionCardInfo>& target_vector, int t_puzzle_number, int t_stage_number, int t_card_number, function<void(vector<CollectionCardInfo>&, vector<CollectionCardInfo>::iterator&, int, int, int, function<void()>)> insert_func, function<void()> log_func)
	{
		bool is_insert = false;
		bool is_found = false;
		for(int j=0;!is_found && j<target_vector.size();j++)
		{
			if(target_vector[j].puzzle_number.getV() == t_puzzle_number && !target_vector[j].is_puzzle_card.getV() && target_vector[j].stage_number.getV() == t_stage_number)
			{
				is_found = true;
				
				int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
				
				if(card_grade == 1)
					target_vector[j].grade1_card_number = t_card_number;
				else if(card_grade == 2)
					target_vector[j].grade2_card_number = t_card_number;
				else if(card_grade == 3)
					target_vector[j].grade3_card_number = t_card_number;
				else if(card_grade == 4)
					target_vector[j].grade4_card_number = t_card_number;
				is_insert = true;
				log_func();
			}
		}
		
		if(!is_found)
		{
			// find insert point
			for(auto iter = target_vector.begin();iter!=target_vector.end();iter++)
			{
				if(iter->puzzle_number.getV() > t_puzzle_number)
				{
					insert_func(target_vector, iter, t_puzzle_number, t_stage_number, t_card_number, log_func);
					is_insert = true;
					break;
				}
				else if(iter->puzzle_number.getV() == t_puzzle_number)
				{
					int iter_stage_number = iter->stage_number.getV();
					if(iter_stage_number == -1)
					{
						insert_func(target_vector, iter, t_puzzle_number, t_stage_number, t_card_number, log_func);
						is_insert = true;
						break;
					}
					else if(iter_stage_number > t_stage_number)
					{
						insert_func(target_vector, iter, t_puzzle_number, t_stage_number, t_card_number, log_func);
						is_insert = true;
						break;
					}
				}
				else
				{
					
				}
			}
		}
		
		if(!is_insert)
		{
			CollectionCardInfo t_info;
			t_info.puzzle_number = t_puzzle_number;
			t_info.stage_number = t_stage_number;
			
			int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
			
			if(card_grade == 1)
				t_info.grade1_card_number = t_card_number;
			else if(card_grade == 2)
				t_info.grade2_card_number = t_card_number;
			else if(card_grade == 3)
				t_info.grade3_card_number = t_card_number;
			else if(card_grade == 4)
				t_info.grade4_card_number = t_card_number;
			
			target_vector.push_back(t_info);
			log_func();
		}
	};
	
	for(int i=0;i<has_gotten_cards.size();i++)
	{
		int card_number = has_gotten_cards[i].card_number.getV();
		
		string card_type = NSDS_GS(kSDS_CI_int1_category_s, card_number);
		CCLOG("card_type : %s, card_number : %d", card_type.c_str(), card_number);
		if(card_type == "gift")
		{
			CollectionCardInfo t_info;
			t_info.grade4_card_number = card_number;
			
			special_cards.push_back(t_info);
			after_log(3);
		}
		else
		{
			int puzzle_number = -1;
			
			if(card_type == "nPuzzle" || card_type == "ePuzzle")
			{
				for(auto iter = puzzle_stage_info.begin();puzzle_number == -1 && iter!=puzzle_stage_info.end();iter++)
				{
					if(NSDS_GI(iter->first, kSDS_PZ_clearCard_i) == card_number || NSDS_GI(iter->first, kSDS_PZ_perfectCard_i) == card_number)
					{
						puzzle_number = iter->first;
					}
				}
			}
			
			int stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
			for(auto iter = puzzle_stage_info.begin();puzzle_number == -1 && iter!=puzzle_stage_info.end();iter++)
			{
				if(iter->second.x <= stage_number && iter->second.x + iter->second.y > stage_number)
				{
					puzzle_number = iter->first;
				}
			}
			
			if(puzzle_number != -1)
			{
				if(card_type == "nPuzzle")
				{
					t_puzzle_func(normal_puzzle_cards, puzzle_number, card_number, [=](){after_log(1);});
				}
				else if(card_type == "ePuzzle")
				{
					t_puzzle_func(event_puzzle_cards, puzzle_number, card_number, [=](){after_log(2);});
				}
				else if(card_type == "event")
				{
					puzzle_find_func(event_puzzle_cards, puzzle_number, stage_number, card_number, insert_func, [=](){after_log(2);});
				}
				else if(card_type == "special")
				{
					puzzle_find_func(normal_puzzle_cards, puzzle_number, stage_number, card_number, insert_func, [=](){after_log(1);});
				}
				else if(card_type == "normal")
				{
					puzzle_find_func(normal_puzzle_cards, puzzle_number, stage_number, card_number, insert_func, [=](){after_log(1);});
				}
				else // nothing
				{
					
				}
				
//				bool is_event_puzzle = NSDS_GB(puzzle_number, kSDS_PZ_isEvent_b);
			}
			else
			{
				CCLOG("not found puzzle | card_number : %d, stage_number : %d", card_number, stage_number);
			}
		}
	}
	
	CCLOG("total card : %d", int(has_gotten_cards.size()));
}

void StarGoldData::initTakeCardInfo(Json::Value card_list, vector<int>& card_data_load_list)
{
	has_gotten_cards.clear();
	
	int card_cnt = card_list.size();
	
	for(int i=0;i<card_cnt;i++)
	{
		Json::Value card_info = card_list[i];
		
		int card_number = card_info["cardNo"].asInt();
		
		CardSortInfo t_info;
		t_info.card_number = card_number;
		t_info.take_number = i+1;
		t_info.grade = 0;
		t_info.rank = 0;
		t_info.user_ment = card_info["comment"].asString();
		t_info.is_morphing = card_info["isMorphing"].asBool();
		t_info.count = card_info["count"].asInt();
		
		if(t_info.count.getV() > 0)
			has_gotten_cards.push_back(t_info);
		
		if(NSDS_GS(kSDS_CI_int1_imgInfo_s, card_number) == "")
			card_data_load_list.push_back(card_number);
	}
}

void StarGoldData::heartRefreshSuccess(Json::Value result_data)
{
	CCLOG("heart cnt : %d", result_data["heart"].asInt());
	myDSH->setIntegerForKey(kDSH_Key_heartCnt, result_data["heart"].asInt());
	
	chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
	chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
	int recent_time_second = recent_time_value.count();
	
	CCLOG("left time : %d", result_data["leftTime"].asInt());
	myDSH->setIntegerForKey(kDSH_Key_heartTime, recent_time_second-mySGD->getHeartCoolTime()+result_data["leftTime"].asInt());
}

int StarGoldData::getOpenPuzzleCount()
{
	int history_count = getPuzzleHistorySize();
	int open_puzzle_count = 0;
	for(int i=0;i<history_count;i++)
		if(getPuzzleHistoryForIndex(i).is_open.getV())
			open_puzzle_count++;
	return open_puzzle_count;
}

int StarGoldData::getPuzzleHistorySize()
{
	return int(puzzle_historys.size());
}

PuzzleHistory StarGoldData::getPuzzleHistoryForIndex(int t_index)
{
	return puzzle_historys[t_index];
}

PuzzleHistory StarGoldData::getPuzzleHistory(int puzzle_number)
{
	for(int i=0;i<puzzle_historys.size();i++)
	{
		if(puzzle_historys[i].puzzle_number == puzzle_number)
			return puzzle_historys[i];
	}
	
	PuzzleHistory t_empty;
	t_empty.puzzle_number = puzzle_number;
	t_empty.is_open = false;
	t_empty.is_clear = false;
	t_empty.is_perfect = false;
	t_empty.open_type = "";
	return t_empty;
}

CommandParam StarGoldData::getUpdatePuzzleHistoryParam(PuzzleHistory t_history, jsonSelType call_back)
{
	bool is_found = false;
	for(int i=0;i<puzzle_historys.size() && !is_found;i++)
	{
		if(puzzle_historys[i].puzzle_number.getV() == t_history.puzzle_number.getV())
		{
			is_found = true;
			puzzle_historys[i].is_open = t_history.is_open.getV();
			puzzle_historys[i].is_clear = t_history.is_clear.getV();
			puzzle_historys[i].is_perfect = t_history.is_perfect.getV();
			puzzle_historys[i].open_type = t_history.open_type.getV();
		}
	}
	
	if(!is_found)
		puzzle_historys.push_back(t_history);
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["puzzleNo"] = t_history.puzzle_number.getV();
	param["updateOpenDate"] = t_history.is_open.getV();
	param["updateClearDate"] = t_history.is_clear.getV();
	param["updatePerfectDate"] = t_history.is_perfect.getV();
	param["openType"] = t_history.open_type.getV();
	
	return CommandParam("updatePuzzleHistory", param, call_back);
}

void StarGoldData::setPuzzleHistory(PuzzleHistory t_history, jsonSelType call_back)
{
	setPuzzleHistoryForNotSave(t_history);
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["puzzleNo"] = t_history.puzzle_number.getV();
	param["updateOpenDate"] = t_history.is_open.getV();
	param["updateClearDate"] = t_history.is_clear.getV();
	param["updatePerfectDate"] = t_history.is_perfect.getV();
	param["openType"] = t_history.open_type.getV();
	hspConnector::get()->command("updatePuzzleHistory", param, call_back);
}

void StarGoldData::setPuzzleHistoryForNotSave(PuzzleHistory t_history)
{
	bool is_found = false;
	for(int i=0;i<puzzle_historys.size() && !is_found;i++)
	{
		if(puzzle_historys[i].puzzle_number.getV() == t_history.puzzle_number.getV())
		{
			is_found = true;
			puzzle_historys[i].is_open = t_history.is_open.getV();
			puzzle_historys[i].is_clear = t_history.is_clear.getV();
			puzzle_historys[i].is_perfect = t_history.is_perfect.getV();
			puzzle_historys[i].open_type = t_history.open_type.getV();
		}
	}
	
	if(!is_found)
		puzzle_historys.push_back(t_history);
}

void StarGoldData::resultUpdatePuzzleHistory(Json::Value result_data)
{
	Json::Value history_data = result_data["data"];
	int puzzle_no = history_data["puzzleNo"].asInt();
	
	PuzzleHistory t_history = getPuzzleHistory(puzzle_no);
	t_history.is_open = history_data["openDate"].asInt64() != 0;
	t_history.is_clear = history_data["clearDate"].asInt64() != 0;
	t_history.is_perfect = history_data["perfectDate"].asInt64() != 0;
	
	setPuzzleHistoryForNotSave(t_history);
}

void StarGoldData::initPuzzleHistory(Json::Value history_list)
{
	puzzle_historys.clear();
	
	int history_cnt = history_list.size();
	
	for(int i=0;i<history_cnt;i++)
	{
		Json::Value history_info = history_list[i];
		
		PuzzleHistory t_history;
		t_history.puzzle_number = history_info["puzzleNo"].asInt();
		t_history.is_open = history_info["openDate"].asInt64() != 0;
		t_history.is_clear = history_info["clearDate"].asInt64() != 0;
		t_history.is_perfect = history_info["perfectDate"].asInt64() != 0;
		t_history.open_type = history_info["openType"].asString();
		puzzle_historys.push_back(t_history);
	}
}

bool StarGoldData::isClearPiece(int stage_number)
{
	for(int i=0;i<piece_historys.size();i++)
		if(piece_historys[i].stage_number.getV() == stage_number)
			return piece_historys[i].is_clear[0].getV() || piece_historys[i].is_clear[1].getV() || piece_historys[i].is_clear[2].getV() || piece_historys[i].is_clear[3].getV();
	
	return false;
}

int StarGoldData::getPieceHistorySize()
{
	return int(piece_historys.size());
}

PieceHistory StarGoldData::getPieceHistoryForIndex(int t_index)
{
	return piece_historys[t_index];
}

PieceHistory StarGoldData::getPieceHistory(int stage_number)
{
	for(int i=0;i<piece_historys.size();i++)
	{
		if(piece_historys[i].stage_number.getV() == stage_number)
			return piece_historys[i];
	}
	
	PieceHistory t_empty;
	t_empty.stage_number = stage_number;
	t_empty.is_open = false;
	for(int j=0;j<4;j++)
		t_empty.is_clear[j] = false;
	t_empty.try_count = 0;
	t_empty.clear_count = 0;
	t_empty.open_type = "";
	return t_empty;
}

CommandParam StarGoldData::getUpdatePieceHistoryParam(PieceHistory t_history, jsonSelType call_back)
{
	setPieceHistoryForNotSave(t_history);
	
	return CommandParam("updatePieceHistory", getSavePieceHistoryParam(t_history), call_back);
}

Json::Value StarGoldData::getSavePieceHistoryParam(PieceHistory t_history)
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["pieceNo"] = t_history.stage_number.getV();
	param["openDate"] = t_history.is_open.getV();
	for(int j=0;j<4;j++)
		param["clearDateList"][j] = t_history.is_clear[j].getV();
	param["tryCount"] = t_history.try_count.getV();
	param["clearCount"] = t_history.clear_count.getV();
	param["openType"] = t_history.open_type.getV();
	return param;
}

void StarGoldData::setPieceHistoryForNotSave(PieceHistory t_history)
{
	bool is_found = false;
	for(int i=0;i<piece_historys.size() && !is_found;i++)
	{
		if(piece_historys[i].stage_number.getV() == t_history.stage_number.getV())
		{
			is_found = true;
			piece_historys[i].stage_number = t_history.stage_number.getV();
			piece_historys[i].is_open = t_history.is_open.getV();
			
			for(int j=0;j<4;j++)
			{
				piece_historys[i].is_clear[j] = t_history.is_clear[j].getV();
			}
			
			piece_historys[i].try_count = t_history.try_count.getV();
			piece_historys[i].clear_count = t_history.clear_count.getV();
			piece_historys[i].open_type = t_history.open_type.getV();
		}
	}
	
	if(!is_found)
	{
		piece_historys.push_back(t_history);
	}
}

void StarGoldData::resultUpdatePieceHistory(Json::Value result_data)
{
	Json::Value history_data = result_data["data"];
	int piece_no = history_data["pieceNo"].asInt();
	
	PieceHistory t_history = getPieceHistory(piece_no);
	t_history.is_open = history_data["openDate"].asInt64() != 0;
	if(!history_data["clearDateList"].isNull())
	{
		for(int i=0;i<4;i++)
		{
			t_history.is_clear[i] = history_data["clearDateList"][i].asInt64() != 0;
		}
	}
	else
	{
		for(int i=0;i<4;i++)
		{
			t_history.is_clear[i] = false;
		}
	}
	t_history.open_type = history_data["openType"].asString();
	t_history.try_count = history_data.get("tryCount", Json::Value()).asInt();
	t_history.clear_count = history_data.get("clearCount", Json::Value()).asInt();
	
	setPieceHistoryForNotSave(t_history);
}

void StarGoldData::setPieceHistory(PieceHistory t_history, jsonSelType call_back)
{
	setPieceHistoryForNotSave(t_history);
	
	hspConnector::get()->command("updatePieceHistory", getSavePieceHistoryParam(t_history), call_back);
}

void StarGoldData::initPieceHistory(Json::Value history_list)
{
	piece_historys.clear();
	
	int history_cnt = history_list.size();
	
	for(int i=0;i<history_cnt;i++)
	{
		Json::Value history_info = history_list[i];
		
		PieceHistory t_history;
		t_history.stage_number = history_info["pieceNo"].asInt();
		t_history.is_open = history_info["openDate"].asInt64() != 0;
		
		for(int j=0;j<4;j++)
			t_history.is_clear[j] = history_info["clearDateList"][j].asInt64() != 0;
		
		t_history.try_count = history_info["tryCount"].asInt();
		t_history.clear_count = history_info["clearCount"].asInt();
		t_history.open_type = history_info["openType"].asString();
		piece_historys.push_back(t_history);
	}
}

void StarGoldData::initCharacterHistory(Json::Value history_list)
{
	character_historys.clear();
	for(int i=0;i<history_list.size();i++)
	{
		Json::Value t_data = history_list[i];
		CharacterHistory t_history;
		t_history.characterNo = t_data["characterNo"].asInt();
		t_history.level = t_data["level"].asInt();
		t_history.nextPrice = t_data["nextPrice"].asInt();
		t_history.power = t_data["power"].asInt();
		t_history.nextPower = t_data["nextPower"].asInt();
		t_history.prevPower = t_data["prevPower"].asInt();
		t_history.isMaxLevel = t_data["isMaxLevel"].asBool();
		
		character_historys.push_back(t_history);
	}
}
void StarGoldData::initSelectedCharacterNo(int t_i)
{
	bool is_found = false;
	for(int i=0;!is_found && i<character_historys.size();i++)
	{
		if(character_historys[i].characterNo.getV() == t_i)
		{
			selected_character_index = i;
			is_found = true;
		}
	}
	if(!is_found)
	{
		selected_character_index = 0;
	}
}
CharacterHistory StarGoldData::getSelectedCharacterHistory()
{
	return character_historys[selected_character_index.getV()];
}
int StarGoldData::getCharacterHistorySize()
{
	return character_historys.size();
}
CharacterHistory StarGoldData::getCharacterHistory(int t_index)
{
	return character_historys[t_index];
}
CommandParam StarGoldData::getUpdateCharacterHistoryParam(CharacterHistory t_history, jsonSelType call_back)
{
	keep_character_history_callback = call_back;
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	
	param["characterNo"] = t_history.characterNo.getV();
	param["level"] = t_history.level.getV();
	
	return CommandParam("updatecharacterhistory", param, json_selector(this, StarGoldData::resultUpdateCharacterHistory));
	
}
void StarGoldData::setCharacterHistory(CharacterHistory t_history, jsonSelType call_back)
{
	keep_character_history_callback = call_back;
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	
	param["characterNo"] = t_history.characterNo.getV();
	param["level"] = t_history.level.getV();
	
	hspConnector::get()->command("updatecharacterhistory", param, json_selector(this, StarGoldData::resultUpdateCharacterHistory));
}

void StarGoldData::resultUpdateCharacterHistory(Json::Value result_data)
{
	if(result_data["result"]["code"] == GDSUCCESS)
	{
		int characterNo = result_data["characterNo"].asInt();
		bool is_found = false;
		for(int i=0;!is_found && i<getCharacterHistorySize();i++)
		{
			if(character_historys[i].characterNo.getV() == characterNo)
			{
				character_historys[i].level = result_data["level"].asInt();
				character_historys[i].nextPrice = result_data["nextPrice"].asInt();
				character_historys[i].power = result_data["power"].asInt();
				character_historys[i].nextPower = result_data["nextPower"].asInt();
				character_historys[i].prevPower = result_data["prevPower"].asInt();
				character_historys[i].isMaxLevel = result_data["isMaxLevel"].asBool();
				is_found = true;
			}
		}
		
		if(!is_found)
		{
			CharacterHistory t_history;
			t_history.characterNo = characterNo;
			t_history.level = result_data["level"].asInt();
			t_history.nextPrice = result_data["nextPrice"].asInt();
			t_history.power = result_data["power"].asInt();
			t_history.nextPower = result_data["nextPower"].asInt();
			t_history.prevPower = result_data["prevPower"].asInt();
			t_history.isMaxLevel = result_data["isMaxLevel"].asBool();
			
			character_historys.push_back(t_history);
		}
	}
	
	if(keep_character_history_callback != nullptr)
		keep_character_history_callback(result_data);
}

CommandParam StarGoldData::getUpdateTodayMissionParam(jsonSelType t_callback)
{
	update_today_mission_callback = t_callback;
	
	TodayMissionType t_type = (TodayMissionType)today_mission_info.mission_type.getV();
	
	Json::Value param;
	param["memberID"] = myHSP->getSocialID();
	param["date"] = today_mission_info.today_date.getV();
	
	if(t_type == kTodayMissionType_totalPercent)
	{
		param["count"] = int(getPercentage()*100.f);
	}
	else if(t_type == kTodayMissionType_totalScore)
	{
		param["count"] = int(getScore());
	}
	else if(t_type == kTodayMissionType_totalTakeGold)
	{
		param["count"] = int(getStageGold());
	}
	else if(t_type == kTodayMissionType_totalCatch)
	{
		param["count"] = int(getCatchCumberCount());
	}
	
	return CommandParam("updatetodaymission", param, json_selector(this, StarGoldData::resultUpdateTodayMission));
}

void StarGoldData::resultUpdateTodayMission(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		initTodayMission(result_data);
		
		if(result_data["isFirstCheck"].asBool())
		{
			GoodsType t_type = getGoodsKeyToType(today_mission_info.reward_type.getV());
			int t_count = result_data["rewardCount"].asInt();
			int before_value = goods_data[t_type].getV();
			goods_data[t_type] = t_count;
			
			if(t_type == kGoodsType_ruby && star_label)
			{
				star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
				
				if(before_value < t_count)
				{
					for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
					{
						if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
						   t_count >= myAchieve->getCondition((AchievementCode)i))
						{
							myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
							AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
							CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
						}
					}
				}
			}
			else if(t_type == kGoodsType_gold && gold_label)
			{
				gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
				
				if(before_value < t_count)
				{
					for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
					{
						if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
						   t_count >= myAchieve->getCondition((AchievementCode)i))
						{
							myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
							AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
							CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
						}
					}
					myAchieve->updateAchieve(nullptr);
				}
			}
			else if(t_type == kGoodsType_heart && heart_time)
			{
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, t_count);
				heart_time->refreshHeartTime();
			}
		}
	}
	
	if(update_today_mission_callback != nullptr)
		update_today_mission_callback(result_data);
}

int StarGoldData::getClearStarCount()
{
	int return_value = 0;
	for(int i=0;i<piece_historys.size();i++)
	{
		for(int j=0;j<4;j++)
		{
			if(piece_historys[i].is_clear[j].getV())
				return_value += j+1;
		}
	}
	
	return return_value;
}

bool StarGoldData::isEmptyAchieveNotiQueue()
{
	return achieve_noti_que.empty();
}

void StarGoldData::pushAchieveNotiQueue(AchievementCode t_code)
{
	achieve_noti_que.push_back(t_code);
}

void StarGoldData::popAchieveNotiQueue()
{
	achieve_noti_que.pop_front();
}

AchievementCode StarGoldData::frontAchieveNotiQueue()
{
	return achieve_noti_que.front();
}

void StarGoldData::setNextSceneName(string scene_name)
{
	next_scene_name = scene_name;
}

string StarGoldData::getNextSceneName()
{
	string return_value = next_scene_name;
	next_scene_name = "";
	return return_value;
}

void StarGoldData::resetNoticeList(Json::Value t_notice_list)
{
	must_be_show_notice = false;
	//		notice_list.clear();
	
	Json::Value not_encode_notice_list;
	
	
	
	int64_t now_value = GraphDog::get()->getTime();
	
	for(int i=0;i<t_notice_list.size();i++)
	{
		string notice_time = myDSH->getStringForKey(kDSH_Key_noticeViewDate_int1, t_notice_list[i]["no"].asInt());
		int64_t notice_value = atoll(notice_time.c_str());
		if(notice_time == "" || notice_value <= now_value)
			not_encode_notice_list.append(t_notice_list[i]);
	}
	if(!not_encode_notice_list.empty())
	{
		must_be_show_notice = true;
		
		Json::FastWriter writer;
		notice_list = KSProtectStr(writer.write(not_encode_notice_list));
	}
	else
	{
		notice_list = KSProtectStr();
	}
}

string StarGoldData::getGoodsTypeToKey(GoodsType t_type)
{
	string return_value;
	
	if(t_type == kGoodsType_money)
		return_value = "m";
	else if(t_type == kGoodsType_ruby)
		return_value = "r";
	else if(t_type == kGoodsType_gold)
		return_value = "g";
	else if(t_type == kGoodsType_item9)
		return_value = "i9";
	else if(t_type == kGoodsType_item6)
		return_value = "i6";
	else if(t_type == kGoodsType_item11)
		return_value = "i11";
	else if(t_type == kGoodsType_pass1)
		return_value = "p1";
	else if(t_type == kGoodsType_pass2)
		return_value = "p2";
	else if(t_type == kGoodsType_pass3)
		return_value = "p3";
	else if(t_type == kGoodsType_pass4)
		return_value = "p4";
	else if(t_type == kGoodsType_pass5)
		return_value = "p5";
	else if(t_type == kGoodsType_pass6)
		return_value = "p6";
	else if(t_type == kGoodsType_heart)
		return_value = "h";
	else if(t_type == kGoodsType_pz)
		return_value = "pz";
	else if(t_type == kGoodsType_pc)
		return_value = "pc";
	else if(t_type == kGoodsType_cu)
		return_value = "cu";
	
	return return_value;
}

GoodsType StarGoldData::getGoodsKeyToType(string t_key)
{
	GoodsType return_value = kGoodsType_end;
	
	for(int i=kGoodsType_begin+1;i<kGoodsType_end;i++)
	{
		if(t_key == getGoodsTypeToKey((GoodsType)i))
			return (GoodsType)i;
	}
	
	return return_value;
}

GoodsType StarGoldData::getItemCodeToGoodsType(ITEM_CODE t_code)
{
	GoodsType return_value;
	if(t_code == kIC_baseSpeedUp)
		return_value = kGoodsType_item9;
	else if(t_code == kIC_doubleItem)
		return_value = kGoodsType_item6;
	else if(t_code == kIC_magnet)
		return_value = kGoodsType_item11;
	return return_value;
}

void StarGoldData::initProperties(Json::Value t_list)
{
	for(int i=0;i<t_list.size();i++)
	{
		GoodsType t_type = getGoodsKeyToType(t_list[i]["type"].asString());
		int t_count = t_list[i]["count"].asInt();
		
		goods_data[t_type] = t_count;
		
		if(t_type == kGoodsType_ruby && star_label)
			star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
		else if(t_type == kGoodsType_gold && gold_label)
			gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
		else if(t_type == kGoodsType_heart && heart_time)
		{
			myDSH->setIntegerForKey(kDSH_Key_heartCnt, t_count);
			heart_time->refreshHeartTime();
		}
	}
}

int StarGoldData::getGoodsValue(GoodsType t_type)
{
	map<GoodsType, KSProtectVar<int>>::iterator iter = goods_data.find(t_type);
	if(iter == goods_data.end())
		return 0;
	else
		return iter->second.getV();
}

void StarGoldData::addChangeGoodsIngameGold(int t_value)
{
	ingame_gold = ingame_gold.getV() + t_value;
	
	if(ingame_gold_label)
		ingame_gold_label->setString(CCString::createWithFormat("%d", ingame_gold.getV())->getCString());
}

void StarGoldData::addChangeGoods(string t_exchangeID, GoodsType t_type/* = kGoodsType_begin*/, int t_value/* = 0*/, string t_statsID/* = ""*/, string t_statsValue/* = ""*/, string t_content/* = ""*/, bool t_isPurchase/* = false*/)
{
	ChangeGoodsData t_data;
	t_data.is_renewal = true;
	t_data.m_exchangeID = t_exchangeID;
	
	ChangeGoodsDataDetail t_detail;
	t_detail.m_type = t_type;
	t_detail.m_value = t_value;
	t_detail.m_statsID = t_statsID;
	t_detail.m_statsValue = t_statsValue;
	t_detail.m_content = t_content;
	t_detail.m_isPurchase = t_isPurchase;
	t_data.detail_list.push_back(t_detail);
	
	change_goods_list.push_back(t_data);
}

void StarGoldData::addChangeGoods(string t_exchangeID, vector<ChangeGoodsDataDetail> t_detail_list)
{
	ChangeGoodsData t_data;
	t_data.is_renewal = true;
	t_data.m_exchangeID = t_exchangeID;
	
	t_data.detail_list = t_detail_list;
	
	change_goods_list.push_back(t_data);
}

//void StarGoldData::addChangeGoods(GoodsType t_type, int t_value, string t_statsID/* = ""*/, string t_statsValue/* = ""*/, string t_content/* = ""*/, bool t_isPurchase/* = false*/)
//{
//	// add
//	for(int i=0;i<change_goods_list.size();i++)
//	{
//		if(change_goods_list[i].m_type == t_type)
//		{
//			change_goods_list[i].m_value = change_goods_list[i].m_value.getV() + t_value;
//			if(t_statsID != "")
//				change_goods_list[i].m_statsID += " | " + t_statsID;
//			if(t_statsValue != "")
//				change_goods_list[i].m_statsValue += " | " + t_statsValue;
//			if(t_content != "")
//				change_goods_list[i].m_content += " | " + t_content;
//			change_goods_list[i].m_isPurchase = t_isPurchase;
//			return;
//		}
//	}
//	
//	// insert
//	ChangeGoodsData t_data;
//	t_data.m_type = t_type;
//	t_data.m_value = t_value;
//	t_data.m_statsID = t_statsID;
//	t_data.m_statsValue = t_statsValue;
//	t_data.m_content = t_content;
//	t_data.m_isPurchase = t_isPurchase;
//	
//	change_goods_list.push_back(t_data);
//}
//
//void StarGoldData::updateChangeGoods(GoodsType t_type, int t_value, string t_statsID/* = ""*/, string t_statsValue/* = ""*/, string t_content/* = ""*/, bool t_isPurchase/* = false*/)
//{
//	// modify
//	for(int i=0;i<change_goods_list.size();i++)
//	{
//		if(change_goods_list[i].m_type == t_type)
//		{
//			change_goods_list[i].m_value = t_value;
//			change_goods_list[i].m_statsID = t_statsID;
//			change_goods_list[i].m_statsValue = t_statsValue;
//			change_goods_list[i].m_content = t_content;
//			change_goods_list[i].m_isPurchase = t_isPurchase;
//			return;
//		}
//	}
//	
//	// insert
//	ChangeGoodsData t_data;
//	t_data.m_type = t_type;
//	t_data.m_value = t_value;
//	t_data.m_statsID = t_statsID;
//	t_data.m_statsValue = t_statsValue;
//	t_data.m_content = t_content;
//	t_data.m_isPurchase = t_isPurchase;
//	
//	change_goods_list.push_back(t_data);
//}

void StarGoldData::clearChangeGoods()
{
	change_goods_list.clear();
}

void StarGoldData::changeGoods(jsonSelType t_callback)
{
	change_goods_callback = t_callback;
	retryChangeGoods();
}

void StarGoldData::changeGoodsTransaction(vector<CommandParam> command_list, jsonSelType t_callback)
{
	if(ingame_gold.getV() > 0)
	{
		is_ingame_gold = true;
		int t_ingame_gold = ingame_gold.getV();
		if(isTimeEvent(kTimeEventType_gold))
			t_ingame_gold *= getTimeEventFloatValue(kTimeEventType_gold);
		
		addChangeGoods("stageGold", kGoodsType_gold, t_ingame_gold, "stage", CCString::createWithFormat("%d", mySD->getSilType())->getCString(), "골드획득");
	}
	
	vector<CommandParam> transaction_list;
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getSocialID();
	
	transaction_list.push_back(CommandParam("starttransaction", transaction_param, t_callback));
	if(isChangedGoods())
	{
		vector<CommandParam> t_list = getChangeGoodsParam(json_selector(this, StarGoldData::saveChangeGoodsTransaction));
		for(int i=0;i<t_list.size();i++)
		{
			transaction_list.push_back(t_list[i]);
		}
	}
	
	for(int i=0;i<command_list.size();i++)
	{
		transaction_list.push_back(command_list[i]);
	}
	
	hspConnector::get()->command(transaction_list, -1);
}

void StarGoldData::linkChangeHistory(Json::Value result_data)
{
	string t_type = result_data["type"].asString();
	if(t_type == "pc")
	{
		resultUpdatePieceHistory(result_data["result"]);
	}
	else if(t_type == "pz")
	{
		resultUpdatePuzzleHistory(result_data["result"]);
	}
	else if(t_type == "cu")
	{
		resultUpdateCharacterHistory(result_data["result"]);
	}
}

void StarGoldData::saveChangeGoodsTransaction(Json::Value result_data)
{
	GraphDogLib::JsonToLog("transaction changeuserproperties", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		if(is_ingame_gold)
		{
			int t_missile_level = getSelectedCharacterHistory().level.getV();
			string fiverocks_param2;
			if(t_missile_level <= 5)
				fiverocks_param2 = "UserLv01~Lv05";
			else if(t_missile_level <= 10)
				fiverocks_param2 = "UserLv06~Lv10";
			else if(t_missile_level <= 15)
				fiverocks_param2 = "UserLv11~Lv15";
			else if(t_missile_level <= 20)
				fiverocks_param2 = "UserLv16~Lv20";
			else if(t_missile_level <= 25)
				fiverocks_param2 = "UserLv21~Lv25";
			else if(t_missile_level <= 30)
				fiverocks_param2 = "UserLv26~Lv30";
			else
				fiverocks_param2 = "UserLv31~";
			
			
			fiverocks::FiveRocksBridge::trackEvent("GetGold", "Get_Event", "Ingame", fiverocks_param2.c_str(), ingame_gold.getV());
			is_ingame_gold = false;
		}
		
		change_goods_list.clear();
		
		Json::Value result_list = result_data["list"];
		
		for(int i=0;i<result_list.size();i++)
		{
			GoodsType t_type = getGoodsKeyToType(result_list[i]["type"].asString());
			
			if(t_type != kGoodsType_end)
			{
				int t_count = result_list[i]["count"].asInt();
				int before_value = goods_data[t_type].getV();
				goods_data[t_type] = t_count;
				
				if(t_type == kGoodsType_ruby && star_label)
				{
					star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
					
					if(before_value < t_count)
					{
						for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
						{
							if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
							   t_count >= myAchieve->getCondition((AchievementCode)i))
							{
								myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
								AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
								CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
							}
						}
						myAchieve->updateAchieve(nullptr);
					}
				}
				else if(t_type == kGoodsType_gold && gold_label)
				{
					gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
					
					if(before_value < t_count)
					{
						for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
						{
							if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
							   t_count >= myAchieve->getCondition((AchievementCode)i))
							{
								myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
								AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
								CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
							}
						}
						myAchieve->updateAchieve(nullptr);
					}
				}
				else if(t_type == kGoodsType_heart && heart_time)
				{
					myDSH->setIntegerForKey(kDSH_Key_heartCnt, t_count);
					heart_time->refreshHeartTime();
				}
			}
			else
			{
				linkChangeHistory(result_list[i]);
			}
		}
		
		ingame_gold = 0;
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		GoodsType t_type = getGoodsKeyToType(result_data["minusType"].asString());
		if(t_type != kGoodsType_end)
		{
			int t_count = result_data["minusCount"].asInt();
			int before_value = goods_data[t_type].getV();
			goods_data[t_type] = t_count;
			
			if(t_type == kGoodsType_ruby && star_label)
			{
				star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
				
				if(before_value < t_count)
				{
					for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
					{
						if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
						   t_count >= myAchieve->getCondition((AchievementCode)i))
						{
							myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
							AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
							CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
						}
					}
					myAchieve->updateAchieve(nullptr);
				}
			}
			else if(t_type == kGoodsType_gold && gold_label)
			{
				gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
				
				if(before_value < t_count)
				{
					for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
					{
						if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
						   t_count >= myAchieve->getCondition((AchievementCode)i))
						{
							myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
							AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
							CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
						}
					}
					myAchieve->updateAchieve(nullptr);
				}
			}
			else if(t_type == kGoodsType_heart && heart_time)
			{
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, t_count);
				heart_time->refreshHeartTime();
			}
		}
		else
		{
			linkChangeHistory(result_data);
		}
	}
	else
	{
		
	}
}

void StarGoldData::refreshGoodsData(string t_key, int t_count)
{
	GoodsType t_type = getGoodsKeyToType(t_key);
	if(t_type != kGoodsType_end)
	{
		int before_value = goods_data[t_type].getV();
		goods_data[t_type] = t_count;
		
		if(t_type == kGoodsType_ruby && star_label)
		{
			star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
			
			if(before_value < t_count)
			{
				for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
				{
					if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
					   t_count >= myAchieve->getCondition((AchievementCode)i))
					{
						myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
						AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
						CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
					}
				}
				myAchieve->updateAchieve(nullptr);
			}
		}
		else if(t_type == kGoodsType_gold && gold_label)
		{
			gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
			
			if(before_value < t_count)
			{
				for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
				{
					if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
					   t_count >= myAchieve->getCondition((AchievementCode)i))
					{
						myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
						AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
						CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
					}
				}
				myAchieve->updateAchieve(nullptr);
			}
		}
		else if(t_type == kGoodsType_heart && heart_time)
		{
			myDSH->setIntegerForKey(kDSH_Key_heartCnt, t_count);
			heart_time->refreshHeartTime();
		}
	}
	else
	{
		// ?
	}
}

bool StarGoldData::isChangedGoods()
{
	return change_goods_list.size() > 0;
}

vector<CommandParam> StarGoldData::getChangeGoodsParam(jsonSelType t_callback)
{
	vector<CommandParam> command_list;
	command_list.clear();
	
	for(int i=0;i<change_goods_list.size();i++)
	{
		Json::Value param;
		param["memberID"] = hspConnector::get()->getSocialID();
		param["exchangeID"] = change_goods_list[i].m_exchangeID.getV();
		
		if(change_goods_list[i].detail_list.size() > 0 && change_goods_list[i].detail_list[0].m_value.getV() != 0)
		{
			Json::Value p_list;
			for(int j=0;j<change_goods_list[i].detail_list.size();j++)
			{
				p_list[j]["type"] = getGoodsTypeToKey(change_goods_list[i].detail_list[j].m_type);
				if(change_goods_list[i].detail_list[j].m_value.getV() != 0)
					p_list[j]["count"] = change_goods_list[i].detail_list[j].m_value.getV();
				if(change_goods_list[i].detail_list[j].m_statsID != "")
					p_list[j]["statsID"] = change_goods_list[i].detail_list[j].m_statsID;
				if(change_goods_list[i].detail_list[j].m_statsValue != "")
					p_list[j]["statsValue"] = change_goods_list[i].detail_list[j].m_statsValue;
				if(change_goods_list[i].detail_list[j].m_content != "")
					p_list[j]["content"] = change_goods_list[i].detail_list[j].m_content;
				if(change_goods_list[i].detail_list[j].m_type == kGoodsType_ruby)
					p_list[j]["isPurchase"] = change_goods_list[i].detail_list[j].m_isPurchase;
			}
			param["list"] = p_list;
		}
		
		command_list.push_back(CommandParam("exchange", param, t_callback));
	}
	
	CCLOG("change goods size : %d", int(command_list.size()));
	
	return command_list;
//
//	return CommandParam("changeuserproperties", param, t_callback);
}

void StarGoldData::retryChangeGoods()
{
	if(!isChangedGoods())
	{
		Json::Value result_data;
		result_data["result"]["code"] = GDSUCCESS;
		change_goods_callback(result_data);
	}
	else
	{
		myHSP->command(getChangeGoodsParam(json_selector(this, StarGoldData::resultChangeGoods)));
//		Json::Value param;
//		param["memberID"] = hspConnector::get()->getSocialID();
//		
//		Json::Value p_list;
//		
//		for(int i=0;i<change_goods_list.size();i++)
//		{
//			p_list[i]["type"] = getGoodsTypeToKey(change_goods_list[i].m_type);
//			p_list[i]["count"] = change_goods_list[i].m_value.getV();
//			if(change_goods_list[i].m_statsID != "")
//				p_list[i]["statsID"] = change_goods_list[i].m_statsID;
//			if(change_goods_list[i].m_statsValue != "")
//				p_list[i]["statsValue"] = change_goods_list[i].m_statsValue;
//			if(change_goods_list[i].m_content != "")
//				p_list[i]["content"] = change_goods_list[i].m_content;
//			if(change_goods_list[i].m_type == kGoodsType_ruby)
//				p_list[i]["isPurchase"] = change_goods_list[i].m_isPurchase;
//		}
//		
//		param["list"] = p_list;
//		
//		hspConnector::get()->command("changeuserproperties", param, json_selector(this, StarGoldData::resultChangeGoods));
	}
}

void StarGoldData::resultChangeGoods(Json::Value result_data)
{
	GraphDogLib::JsonToLog("result changeuserproperties", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		change_goods_list.clear();
		
		Json::Value result_list = result_data["list"];
		
		for(int i=0;i<result_list.size();i++)
		{
			GoodsType t_type = getGoodsKeyToType(result_list[i]["type"].asString());
			if(t_type != kGoodsType_end)
			{
				int t_count = result_list[i]["count"].asInt();
				int before_value = goods_data[t_type].getV();
				goods_data[t_type] = t_count;
				
				if(t_type == kGoodsType_ruby && star_label)
				{
					star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
					
					if(before_value < t_count)
					{
						for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
						{
							if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
							   t_count >= myAchieve->getCondition((AchievementCode)i))
							{
								myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
								AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
								CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
							}
						}
						myAchieve->updateAchieve(nullptr);
					}
				}
				else if(t_type == kGoodsType_gold && gold_label)
				{
					gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
					
					if(before_value < t_count)
					{
						for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
						{
							if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
							   t_count >= myAchieve->getCondition((AchievementCode)i))
							{
								myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
								AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
								CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
							}
						}
						myAchieve->updateAchieve(nullptr);
					}
				}
				else if(t_type == kGoodsType_heart && heart_time)
				{
					myDSH->setIntegerForKey(kDSH_Key_heartCnt, t_count);
					heart_time->refreshHeartTime();
				}
			}
			else
			{
				linkChangeHistory(result_list[i]);
			}
		}
		
		change_goods_callback(result_data);
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		GoodsType t_type = getGoodsKeyToType(result_data["minusType"].asString());
		if(t_type != kGoodsType_end)
		{
			int before_value = goods_data[t_type].getV();
			int t_count = result_data["minusCount"].asInt();
			
			goods_data[t_type] = t_count;
			
			if(t_type == kGoodsType_ruby && star_label)
			{
				star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
				
				if(before_value < t_count)
				{
					for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
					{
						if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
						   t_count >= myAchieve->getCondition((AchievementCode)i))
						{
							myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
							AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
							CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
						}
					}
					myAchieve->updateAchieve(nullptr);
				}
			}
			else if(t_type == kGoodsType_gold && gold_label)
			{
				gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
				
				if(before_value < t_count)
				{
					for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
					{
						if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted(AchievementCode(i)) &&
						   t_count >= myAchieve->getCondition((AchievementCode)i))
						{
							myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
							AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
							CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
						}
					}
					myAchieve->updateAchieve(nullptr);
				}
			}
			else if(t_type == kGoodsType_heart && heart_time)
			{
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, t_count);
				heart_time->refreshHeartTime();
			}
		}
		else
		{
			linkChangeHistory(result_data);
		}
		
		change_goods_callback(result_data);
	}
	else
	{
		retryChangeGoods();
	}
}

UserdataType StarGoldData::getUserdataKeyToType(string t_key)
{
	for(int i=kUserdataType_begin+1;i<kUserdataType_end;i++)
	{
		if(t_key == getUserdataTypeToKey((UserdataType)i))
			return (UserdataType)i;
	}
	
	return kUserdataType_end;
}
string StarGoldData::getUserdataTypeToKey(UserdataType t_type)
{
	string return_value = "";
	
	if(t_type == kUserdataType_isVIP)
		return_value = "isVIP";
	else if(t_type == kUserdataType_isFirstBuy)
		return_value = "isFirstBuy";
	else if(t_type == kUserdataType_totalPlayCount)
		return_value = "totalPlayCount";
	else if(t_type == kUserdataType_failCount)
		return_value = "failCount";
	else if(t_type == kUserdataType_autoLevel)
		return_value = "autoLevel";
	else if(t_type == kUserdataType_selectedCharNO)
		return_value = "selectedCharNO";
	else if(t_type == kUserdataType_highScore)
		return_value = "highScore";
	else if(t_type == kUserdataType_highPiece)
		return_value = "highPiece";
	else if(t_type == kUserdataType_onlyOneBuyPack)
		return_value = "onlyOneBuyPack";
	
	else if(t_type == kUserdataType_endlessData_ingWin)
		return_value = "ing_win";
	else if(t_type == kUserdataType_endlessData_ingWeek)
		return_value = "ing_week";
	else if(t_type == kUserdataType_endlessData_score)
		return_value = "score";
	
	else if(t_type == kUserdataType_achieve_mapGacha)
		return_value = "aMapGacha";
	else if(t_type == kUserdataType_achieve_noFail)
		return_value = "aNoFail";
	else if(t_type == kUserdataType_achieve_hunter)
		return_value = "aHunter";
	else if(t_type == kUserdataType_achieve_changeMania)
		return_value = "aChange";
	else if(t_type == kUserdataType_achieve_fail)
		return_value = "aFail";
	else if(t_type == kUserdataType_achieve_perfect)
		return_value = "aPerfect";
	else if(t_type == kUserdataType_achieve_itemBuyCount)
		return_value = "aItByCnt";
	else if(t_type == kUserdataType_achieve_seqAttendance) // last
		return_value = "aSeqAtd";
	
	return return_value;
}

void StarGoldData::clearChangeUserdata()
{
	changed_userdata_list.clear();
	is_changed_userdata = false;
}

void StarGoldData::changeUserdata(jsonSelType t_callback)
{
	if(!is_changed_userdata)
	{
		Json::Value param;
		param["result"]["code"] = GDSUCCESS;
		
		t_callback(param);
		return;
	}
	
	change_userdata_callback = t_callback;
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getMemberID();
	for(int i=0;i<changed_userdata_list.size();i++)
		param[getUserdataTypeToKey(changed_userdata_list[i].m_type)] = changed_userdata_list[i].m_value.getV();
	
	hspConnector::get()->command("updateuserdata", param, json_selector(this, StarGoldData::resultChangeUserdata));
}

void StarGoldData::resultChangeUserdata(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		for(int i=0;i<changed_userdata_list.size();i++)
			userdata_storage[changed_userdata_list[i].m_type] = changed_userdata_list[i].m_value.getV();
		
		changed_userdata_list.clear();
		
		initUserdata(result_data);
		
		is_changed_userdata = false;
	}
	
	if(change_userdata_callback != nullptr)
		change_userdata_callback(result_data);
}

CommandParam StarGoldData::getChangeUserdataParam(jsonSelType t_callback)
{
	change_userdata_callback = t_callback;
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getMemberID();
	for(int i=0;i<changed_userdata_list.size();i++)
		param[getUserdataTypeToKey(changed_userdata_list[i].m_type)] = changed_userdata_list[i].m_value.getV();
	
	return CommandParam("updateuserdata", param, json_selector(this, StarGoldData::resultChangeUserdata));
}

void StarGoldData::initUserdata(Json::Value result_data)
{
	userdata_storage.clear();
	
	for(int i=kUserdataType_begin+1;i<kUserdataType_end;i++)
	{
		if(i >= kUserdataType_achieve_mapGacha && i <= kUserdataType_achieve_seqAttendance) // HS가 유저데이터의 업적관련한 정보는 따로 묶겠다고 했음. 그래서 분기
		{
			userdata_storage[(UserdataType)i] = result_data["archiveData"].get(getUserdataTypeToKey((UserdataType)i), Json::Value()).asInt();
		}
		else if(i == kUserdataType_endlessData_ingWin || i == kUserdataType_endlessData_ingWeek || i == kUserdataType_endlessData_score)
		{
			userdata_storage[(UserdataType)i] = result_data["endlessData"].get(getUserdataTypeToKey((UserdataType)i), Json::Value()).asInt();
			if(i == kUserdataType_endlessData_ingWin)
			{
				endless_my_victory = userdata_storage[(UserdataType)i].getV();
			}
		}
		else
		{
			userdata_storage[(UserdataType)i] = result_data[getUserdataTypeToKey((UserdataType)i)].asInt();
		}
		
		if(i == kUserdataType_selectedCharNO)
			initSelectedCharacterNo(userdata_storage[(UserdataType)i].getV());
	}
	
	setIntroducerID(result_data.get("introducerID", 0).asInt64());
}

bool StarGoldData::isPossibleShowPurchasePopup(PurchaseGuideType t_type)
{
	bool return_value = true;
	if(t_type == kPurchaseGuideType_firstPurchase)
	{
		if(at_time_show_firstPurchase.getV() > 0 && at_time_show_firstPurchase.getV() + getFirstPurchaseReviewSecond() >= graphdog->getTime())
			return_value = false;
	}
	else if(t_type == kPurchaseGuideType_emptyItem)
	{
		if(empty_item_is_on.getV() == 0 || (at_time_show_emptyItem.getV() > 0 && at_time_show_emptyItem.getV() + getEmptyItemReviewSecond() >= graphdog->getTime()))
			return_value = false;
	}
	else if(t_type == kPurchaseGuideType_stupidNpuHelp)
	{
		if(stupid_npu_help_is_on.getV() == 0 || (at_time_show_stupidNpuHelp.getV() > 0 && at_time_show_stupidNpuHelp.getV() + getStupidNpuHelpReviewSecond() >= graphdog->getTime()))
			return_value = false;
	}
	else if(t_type == kPurchaseGuideType_eventRubyShop)
	{
		if(at_time_show_eventRubyShop.getV() > 0 && at_time_show_eventRubyShop.getV() + getEventRubyShopReviewSecond() >= graphdog->getTime())
			return_value = false;
	}
	else if(t_type == kPurchaseGuideType_levelupGuide)
	{
		if(levelup_guide_is_on.getV() == 0 || (at_time_show_levelupGuide.getV() > 0 && at_time_show_levelupGuide.getV() + getLevelupGuideReviewSecond() >= graphdog->getTime()))
			return_value = false;
	}
	return return_value;
}

void StarGoldData::showPurchasePopup(PurchaseGuideType t_type)
{
	if(t_type == kPurchaseGuideType_firstPurchase)
	{
		at_time_show_firstPurchase = graphdog->getTime();
		myDSH->setIntegerForKey(kDSH_Key_atTimeShowFirstPurchase, at_time_show_firstPurchase.getV());
	}
	else if(t_type == kPurchaseGuideType_emptyItem)
	{
		at_time_show_emptyItem = graphdog->getTime();
		myDSH->setIntegerForKey(kDSH_Key_atTimeShowEmptyItem, at_time_show_emptyItem.getV());
	}
	else if(t_type == kPurchaseGuideType_stupidNpuHelp)
	{
		at_time_show_stupidNpuHelp = graphdog->getTime();
		myDSH->setIntegerForKey(kDSH_Key_atTimeShowStupidNpuHelp, at_time_show_stupidNpuHelp.getV());
	}
	else if(t_type == kPurchaseGuideType_eventRubyShop)
	{
		at_time_show_eventRubyShop = graphdog->getTime();
		myDSH->setIntegerForKey(kDSH_Key_atTimeShowEventRubyShop, at_time_show_eventRubyShop.getV());
	}
	else if(t_type == kPurchaseGuideType_levelupGuide)
	{
		at_time_show_levelupGuide = graphdog->getTime();
		myDSH->setIntegerForKey(kDSH_Key_atTimeShowLevelupGuide, at_time_show_levelupGuide.getV());
	}
}

void StarGoldData::initTodayMission(Json::Value t_info)
{
	today_mission_info.today_date = t_info["date"].asInt();
	today_mission_info.mission_type = t_info["type"].asInt();
	today_mission_info.ing_count = t_info["count"].asInt();
	today_mission_info.reward_type = t_info["reward"][0]["type"].asString();
	today_mission_info.reward_count = t_info["reward"][0]["count"].asInt();
	today_mission_info.goal_count = t_info["goal"].asInt();
	today_mission_info.is_success = t_info["isSuccess"].asBool();
	today_mission_info.resetTimestamp = t_info["resetTimestamp"].asInt64();
	
	is_today_mission_first = t_info["isFirstCheck"].asBool();
}

void StarGoldData::initAttendance(Json::Value result_data)
{
//	GraphDogLib::JsonToLog("attendance data", result_data);
	
	attendance_data.clear();
	attendance_data = result_data;
	if(result_data["sendGift"].asBool())
	{
		is_on_attendance = true;
	}
	else
	{
		is_on_attendance = false;
	}
}

void StarGoldData::resetAttendance()
{
	is_on_attendance = false;
//	attendance_data.clear();
}

Json::Value StarGoldData::getProductInfo(string t_id)
{
    Json::Value t_infos = product_infos["info"];
    int loop_cnt = t_infos.size();
    for(int i=0;i<loop_cnt;i++)
    {
        Json::Value t_info = t_infos[i];
        if(t_info["productid"].asString() == t_id)
        {
            return t_info;
        }
    }
    
    return Json::Value();
}

void StarGoldData::initRankReward(Json::Value result_data)
{
	if(result_data["sendGift"].asBool())
	{
		is_on_rank_reward = true;
		rank_reward_data = result_data;
	}
	else
	{
		is_on_rank_reward = false;
	}
}

void StarGoldData::resetRankReward()
{
	is_on_rank_reward = false;
	rank_reward_data.clear();
}

void StarGoldData::initTimeEventList(Json::Value t_list)
{
	time_event_list.clear();
	int list_size = t_list.size();
	
	for(int i=0;i<list_size;i++)
	{
		Json::Value t_time_event = t_list[i];
		
		string t_type = t_time_event["type"].asString();
		
		TimeEventInfo t_info;
		t_info.event_type = t_type;
		t_info.startDate = t_time_event["startDate"].asInt64();
		t_info.endDate = t_time_event["endDate"].asInt64();
		t_info.startTime = t_time_event["startTime"].asInt();
		t_info.endTime = t_time_event["endTime"].asInt();
		
		if(t_type == getTimeEventTypeString(kTimeEventType_gold))
		{
			t_info.float_value = t_time_event["value"].asFloat();
		}
		else if(t_type == getTimeEventTypeString(kTimeEventType_heart))
		{
			
		}
		else if(t_type == getTimeEventTypeString(kTimeEventType_card))
		{
			t_info.int_value = t_time_event["value"].asInt();
		}
		else if(t_type == getTimeEventTypeString(kTimeEventType_clear))
		{
			t_info.float_value = t_time_event["value"].asFloat();
		}
		
		time_event_list.push_back(t_info);
	}
	
	refreshTimeEvent();
}

string StarGoldData::getTimeEventTypeString(TimeEventType t_type)
{
	string return_value;
	
	if(t_type == kTimeEventType_gold)
		return_value = "gold";
	else if(t_type == kTimeEventType_heart)
		return_value = "heart";
	else if(t_type == kTimeEventType_card)
		return_value = "card";
	else if(t_type == kTimeEventType_clear)
		return_value = "clear";
	return return_value;
}

bool StarGoldData::isTimeEvent(TimeEventType t_type)
{
	bool is_found = false;
	string t_type_string = getTimeEventTypeString(t_type);
	for(int i=0;!is_found && i<time_event_list.size();i++)
	{
		if(time_event_list[i].event_type.getV() == t_type_string)
		{
			is_found = true;
		}
	}
	
	return is_found;
}

int StarGoldData::getTimeEventIntValue(TimeEventType t_type)
{
	string t_type_string = getTimeEventTypeString(t_type);
	for(int i=0;i<time_event_list.size();i++)
	{
		if(time_event_list[i].event_type.getV() == t_type_string)
		{
			return time_event_list[i].int_value.getV();
		}
	}
	
	return 0;
}
float StarGoldData::getTimeEventFloatValue(TimeEventType t_type)
{
	string t_type_string = getTimeEventTypeString(t_type);
	for(int i=0;i<time_event_list.size();i++)
	{
		if(time_event_list[i].event_type.getV() == t_type_string)
		{
			return time_event_list[i].float_value.getV();
		}
	}
	
	return 0.f;
}

void StarGoldData::refreshTimeEvent()
{
	long long recent_date = graphdog->getDate();
	
	struct Eraser
	{
		Eraser(long long t_date) : m_date(t_date) {}
		long long m_date;
		bool operator()(TimeEventInfo i) const
		{
			return i.startDate.getV() > m_date || i.endDate.getV() <= m_date;
		}
	};
	
	time_event_list.erase(std::remove_if(time_event_list.begin(), time_event_list.end(), Eraser(recent_date)), time_event_list.end());
}

string StarGoldData::getAppType()
{
	return app_type;
}
int StarGoldData::getAppVersion()
{
	return app_version;
}

void StarGoldData::myInit()
{
	is_hell_mode_enabled = false;
	
	is_option_tutorial = false;
	
	goods_data.clear();
	
	app_type = "light1";
	app_version = 4; // 커몬세팅값과 비교해서 앱 종료시키는 버전.
	
	client_version = 5; // 이 것도 아마 커몬세팅과 비교해서 암튼 클라 저장된 기록들 날려서 새로 다운받게 하게끔 하는 의도.
	
	is_ingame_gold = false;
	total_card_cnt = 0;
	
	loading_tip_back_number = 1;
	
	max_network_check_cnt = 6; // 0.5초 단위로
	network_check_cnt = 0;
	
	time_event_list.clear();
	
	heart_time = NULL;
	
	setIntroducerID(0);
	
	is_new_puzzle_card = false;
	
	is_on_attendance = false;
	is_on_rank_reward = false;
	is_endless_mode = false;
	is_hell_mode = false;
	is_on_accountLinkLead = false;
	endless_my_victory_on = false;
	is_morphing_noti = true;
	
	suitable_stage = -1;
	is_on_maingame = false;
	gacha_item = kIC_emptyEnd;
	
	rank_up_add_rate = 0;
	keep_time_info.is_loaded = false;
	
	at_time_show_firstPurchase = myDSH->getIntegerForKey(kDSH_Key_atTimeShowFirstPurchase);
	at_time_show_emptyItem = myDSH->getIntegerForKey(kDSH_Key_atTimeShowEmptyItem);
	at_time_show_stupidNpuHelp = myDSH->getIntegerForKey(kDSH_Key_atTimeShowStupidNpuHelp);
	at_time_show_eventRubyShop = myDSH->getIntegerForKey(kDSH_Key_atTimeShowEventRubyShop);
	at_time_show_levelupGuide = myDSH->getIntegerForKey(kDSH_Key_atTimeShowLevelupGuide);
	
	goods_data.clear();
	change_goods_list.clear();
	
	changed_userdata_list.clear();
	
	replay_write_info.clear();
	replay_playing_info.clear();
	is_write_replay = false;
	is_play_replay = false;
	
	is_before_stage_img_download = false;
	
	is_unlock_puzzle = 0;
	is_perfect_puzzle = 0;
	strength_target_card_number = 0;
	is_ingame_before_have_stage_cards.push_back(false);
	is_ingame_before_have_stage_cards.push_back(false);
	is_ingame_before_have_stage_cards.push_back(false);
	is_ingame_before_have_stage_cards.push_back(false);
	
	is_before_selected_event_stage = false;
	is_paused = false;
	login_getted = false;
	is_before_title = true;
	
	clear_reward_gold = 0;

	setTargetDelegate(NULL, NULL);

	after_loading = kImgType_Empty;
	is_after_scene_chapter = false;
	resetLabels();

	before_use_item.push_back(false);
	is_using_item.push_back(false);
	
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		before_use_item.push_back(false);
		is_using_item.push_back(false);
	}
	
	bonus_item_cnt.push_back(KSProtectVar<int>(0)); // empty
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
		bonus_item_cnt.push_back(KSProtectVar<int>(0));


	if(myDSH->getIntegerForKey(kDSH_Key_heartTime) == 0)
	{
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, 5);
		chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
		chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
		int recent_time_second = recent_time_value.count();
		myDSH->setIntegerForKey(kDSH_Key_heartTime, recent_time_second);
	}


	if(!myDSH->getBoolForKey(kDSH_Key_notFirstExe))
	{
//		myDSH->setBoolForKey(kDSH_Key_isDisableDrawButton, true);
		
		myDSH->setBoolForKey(kDSH_Key_notFirstExe, true);
		is_tutorial_cleared = false;
		myDSH->setBoolForKey(kDSH_Key_bgmOff, false);
		myDSH->setBoolForKey(kDSH_Key_effectOff, false);
		AudioEngine::sharedInstance()->setSoundOnOff(true);
		AudioEngine::sharedInstance()->setEffectOnOff(true);

		myDSH->resetDSH();
	}
	else
	{
		is_tutorial_cleared = true;

		AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
		AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
	}
	
	rewind_cnt_per_frame = 2;
}

long long StarGoldData::getIntroducerID()
{
	return introducerID.getV();
}
void StarGoldData::setIntroducerID(long long t_id)
{
	introducerID = t_id;
}

bool StarGoldData::getIsNotClearedStage(){	return is_not_cleared_stage;}
int StarGoldData::getIsUnlockPuzzle(){	return is_unlock_puzzle;}
void StarGoldData::setIsUnlockPuzzle(int t_i)
{
	is_unlock_puzzle = t_i;
}
int StarGoldData::getIsPerfectPuzzle(){	return is_perfect_puzzle;}
void StarGoldData::setIsPerfectPuzzle(int t_i){	is_perfect_puzzle = t_i;}
void StarGoldData::setStrengthTargetCardNumber(int t_card_number){	strength_target_card_number = t_card_number;}
int StarGoldData::getStrengthTargetCardNumber(){	return strength_target_card_number;}
CardStrengthBefore StarGoldData::getCardStrengthBefore(){	return card_strength_before;}
void StarGoldData::setCardStrengthBefore(CardStrengthBefore t_before){	card_strength_before = t_before;}
void StarGoldData::setHeartMax(int t_data){	heart_max = t_data;}
void StarGoldData::setHeartCoolTime(int t_data){	heart_cool_time = t_data;}
void StarGoldData::setGameFriendMax(int t_data){	game_friend_max = t_data;}
void StarGoldData::setHelpCoolTime(int t_data){	help_cool_time = t_data;}
void StarGoldData::setChallengeCoolTime(int t_data){	challenge_cool_time = t_data;}
void StarGoldData::setMsgRemoveDay(int t_data){	msg_remove_day = t_data;}
void StarGoldData::setGachaGoldFee(int t_data){	gacha_gold_fee = t_data;}
void StarGoldData::setGachaRubyFee(int t_data){	gacha_ruby_fee = t_data;}
void StarGoldData::setGachaSocialFee(int t_data){	gacha_social_fee = t_data;}
void StarGoldData::setGachaGoldFeeRetry(int t_data){	gacha_gold_fee_retry = t_data;}
void StarGoldData::setGachaRubyFeeRetry(int t_data){	gacha_ruby_fee_retry = t_data;}
void StarGoldData::setGachaSocialFeeRetry(int t_data){	gacha_social_fee_retry = t_data;}
void StarGoldData::setCardUpgradeGoldFee(int t_data){	card_upgrade_gold_fee = t_data;}
void StarGoldData::setCardUpgradeRubyFee(int t_data){	card_upgrade_ruby_fee = t_data;}
void StarGoldData::setHeartSendCoolTime(int t_data){	heart_send_cool_time = t_data;}
void StarGoldData::setInviteMaxADay(int t_data){	invite_max_a_day = t_data;}
void StarGoldData::setInviteCoolDay(int t_data){	invite_cool_day = t_data;}
void StarGoldData::setPlayContinueFee(int t_data){	play_continue_fee = t_data;}
void StarGoldData::setPlayContinueFeeEndless(int t_data){	play_continue_fee_endless = t_data;	}
void StarGoldData::setCardDurabilityUpFee(int t_data){	card_durability_up_fee = t_data;}
void StarGoldData::setGachaMapFee(int t_data){	gacha_map_fee = t_data;}
void StarGoldData::setRemoveFriendCoolTime(int t_data){	remove_friend_cool_time = t_data;}
void StarGoldData::setSPSendHeart(int t_data){	SP_send_heart = t_data;}
void StarGoldData::setSPSendTicket(int t_data){	SP_send_ticket = t_data;}
void StarGoldData::setSPFinishedChallenge(int t_data){	SP_finished_challenge = t_data;}
void StarGoldData::setSPHelpChallenge(int t_data){	SP_help_challenge = t_data;}
void StarGoldData::setSPSendBoast(int t_data){	SP_send_boast = t_data;}
void StarGoldData::setSPGetTime(int t_data){	SP_get_time = t_data;}
void StarGoldData::setSPGetHeart(int t_data){	SP_get_heart = t_data;}
void StarGoldData::setGachaOnePercentFee(int t_data){	gacha_one_percent_fee = t_data;}
void StarGoldData::setAiAdderOnDrewOrDamaged(float t_data){	ai_adder_on_drew_or_damaged = t_data;}
void StarGoldData::setFuryPercent(float t_data){	fury_percent = t_data;}
void StarGoldData::setSPRentCardThanks(int t_data){	SP_rent_card_thanks = t_data;}
int StarGoldData::getHeartMax(){	return heart_max.getV();}
int StarGoldData::getHeartCoolTime(){	return heart_cool_time.getV();}
int StarGoldData::getGameFriendMax(){	return game_friend_max.getV();}
int StarGoldData::getHelpCoolTime(){	return help_cool_time.getV();}
int StarGoldData::getChallengeCoolTime(){	return challenge_cool_time.getV();}
int StarGoldData::getMsgRemoveDay(){	return msg_remove_day.getV();}
int StarGoldData::getGachaGoldFee(){	return gacha_gold_fee.getV();}
int StarGoldData::getGachaRubyFee(){	return gacha_ruby_fee.getV();}
int StarGoldData::getGachaSocialFee(){	return gacha_social_fee.getV();}
int StarGoldData::getGachaGoldFeeRetry(){	return gacha_gold_fee_retry.getV();}
int StarGoldData::getGachaRubyFeeRetry(){	return gacha_ruby_fee_retry.getV();}
int StarGoldData::getGachaSocialFeeRetry(){	return gacha_social_fee_retry.getV();}
int StarGoldData::getCardUpgradeGoldFee(){	return card_upgrade_gold_fee.getV();}
int StarGoldData::getCardUpgradeRubyFee(){	return card_upgrade_ruby_fee.getV();}
int StarGoldData::getHeartSendCoolTime(){	return heart_send_cool_time.getV();}
int StarGoldData::getInviteMaxADay(){	return invite_max_a_day.getV();}
int StarGoldData::getInviteCoolDay(){	return invite_cool_day.getV();}
int StarGoldData::getPlayContinueFee(){	return play_continue_fee.getV();}
int StarGoldData::getPlayContinueFeeEndless(){	return play_continue_fee_endless.getV();	}
int StarGoldData::getCardDurabilityUpFee(){	return card_durability_up_fee.getV();}
int StarGoldData::getGachaMapFee(){	return gacha_map_fee.getV();}
int StarGoldData::getRemoveFriendCoolTime(){	return remove_friend_cool_time.getV();}
int StarGoldData::getSPSendHeart(){	return SP_send_heart.getV();}
int StarGoldData::getSPSendTicket(){	return SP_send_ticket.getV();}
int StarGoldData::getSPFinishedChallenge(){	return SP_finished_challenge.getV();}
int StarGoldData::getSPHelpChallenge(){	return SP_help_challenge.getV();}
int StarGoldData::getSPSendBoast(){	return SP_send_boast.getV();}
int StarGoldData::getSPGetTime(){	return SP_get_time.getV();}
int StarGoldData::getSPGetHeart(){	return SP_get_heart.getV();}
int StarGoldData::getGachaOnePercentFee(){	return gacha_one_percent_fee.getV();}
float StarGoldData::getAiAdderOnDrewOrDamaged(){	return ai_adder_on_drew_or_damaged.getV();}
float StarGoldData::getFuryPercent(){	return fury_percent.getV();}
int StarGoldData::getSPRentCardThanks(){	return SP_rent_card_thanks.getV();}

void StarGoldData::setItemGachaGoldFee(int t_i){	item_gacha_gold_fee = t_i;}
int StarGoldData::getItemGachaGoldFee(){	return item_gacha_gold_fee.getV();}
void StarGoldData::setItemGachaReplayGoldFee(int t_i){	item_gacha_replay_gold_fee = t_i;}
int StarGoldData::getItemGachaReplayGoldFee(){	return item_gacha_replay_gold_fee.getV();}

void StarGoldData::setUpgradeGoldFee(int t_i){	upgrade_gold_fee = t_i;}
int StarGoldData::getUpgradeGoldFee(){	return upgrade_gold_fee.getV();}

void StarGoldData::setIngameTutorialRewardGold(int t_i){	ingame_tutorial_reward_gold = t_i;}
int StarGoldData::getIngameTutorialRewardGold(){	return ingame_tutorial_reward_gold.getV();}

string StarGoldData::getInappProduct(int t_index){	return NSDS_GS(kSDS_GI_shopRuby_int1_pID_s, t_index);}
string StarGoldData::getEventInappProduct(int t_index){	return NSDS_GS(kSDS_GI_shopEventRuby_int1_pID_s, t_index);}

void StarGoldData::setRankUpConditionCount(int t_i){	rank_up_condition_count = t_i;}
int StarGoldData::getRankUpConditionCount(){	return rank_up_condition_count.getV();}
void StarGoldData::setRankUpBaseRate(float t_f){	rank_up_base_rate = t_f;}
float StarGoldData::getRankUpBaseRate(){	return rank_up_base_rate.getV();}
void StarGoldData::setRankUpRateDistance(float t_f){	rank_up_rate_distance = t_f;}
float StarGoldData::getRankUpRateDistance(){	return rank_up_rate_distance.getV();}
void StarGoldData::setRankUpRubyFee(int t_i){	rank_up_ruby_fee = t_i;}
int StarGoldData::getRankUpRubyFee(){	return rank_up_ruby_fee.getV();}
void StarGoldData::setRankUpAddRate(float t_f){	rank_up_add_rate = t_f;}
float StarGoldData::getRankUpAddRate(){	return rank_up_add_rate.getV();}
void StarGoldData::setSessionTime(int t_i){	sessionTime = t_i;}
int StarGoldData::getSessionTime(){	return sessionTime.getV();}

void StarGoldData::setFirstPurchasePlayCount(int t_i){	first_purchase_play_count = t_i;	}
int StarGoldData::getFirstPurchasePlayCount(){	return first_purchase_play_count.getV();	}
void StarGoldData::setFirstPurchaseReviewSecond(long long t_i){	first_purchase_review_second = t_i;	}
long long StarGoldData::getFirstPurchaseReviewSecond(){	return first_purchase_review_second.getV();	}
void StarGoldData::setEmptyItemReviewSecond(long long t_i){	empty_item_review_second = t_i;}
long long StarGoldData::getEmptyItemReviewSecond(){	return empty_item_review_second.getV();}
void StarGoldData::setStupidNpuHelpReviewSecond(long long t_i){	stupid_npu_help_review_second = t_i;	}
long long StarGoldData::getStupidNpuHelpReviewSecond(){	return stupid_npu_help_review_second.getV();	}
void StarGoldData::setStupidNpuHelpPlayCount(int t_i){	stupid_npu_help_play_count = t_i;	}
int StarGoldData::getStupidNpuHelpPlayCount(){	return stupid_npu_help_play_count.getV();	}
void StarGoldData::setStupidNpuHelpFailCount(int t_i){	stupid_npu_help_fail_count = t_i;	}
int StarGoldData::getStupidNpuHelpFailCount(){	return stupid_npu_help_fail_count.getV();	}
void StarGoldData::setEventRubyShopReviewSecond(long long t_i){	event_ruby_shop_review_second = t_i;	}
long long StarGoldData::getEventRubyShopReviewSecond(){	return event_ruby_shop_review_second.getV();	}
void StarGoldData::setPlayCountHighValue(int t_i){	play_count_high_value = t_i;	}
int StarGoldData::getPlayCountHighValue(){	return play_count_high_value.getV();	}

void StarGoldData::setEmptyItemIsOn(int t_i){	empty_item_is_on = t_i;	}
int StarGoldData::getEmptyItemIsOn(){	return empty_item_is_on.getV();	}
void StarGoldData::setStupidNpuHelpIsOn(int t_i){	stupid_npu_help_is_on = t_i;	}
int StarGoldData::getStupidNpuHelpIsOn(){	return stupid_npu_help_is_on.getV();	}
void StarGoldData::setPlayCountHighIsOn(int t_i){	play_count_high_is_on = t_i;	}
int StarGoldData::getPlayCountHighIsOn(){	return play_count_high_is_on.getV();	}

void StarGoldData::setLevelupGuideReviewSecond(long long t_i){	levelup_guide_review_second = t_i;	}
long long StarGoldData::getLevelupGuideReviewSecond(){	return levelup_guide_review_second.getV();	}
void StarGoldData::setLevelupGuidePlayCount(int t_i){	levelup_guide_play_count = t_i;	}
int StarGoldData::getLevelupGuidePlayCount(){	return levelup_guide_play_count.getV();	}
void StarGoldData::setLevelupGuideConditionLevel(int t_i){	levelup_guide_condition_level = t_i;	}
int StarGoldData::getLevelupGuideConditionLevel(){	return levelup_guide_condition_level.getV();	}
void StarGoldData::setLevelupGuideIsOn(int t_i){	levelup_guide_is_on = t_i;	}
int StarGoldData::getLevelupGuideIsOn(){	return levelup_guide_is_on.getV();	}

void StarGoldData::setItem9OpenStage(int t_i){	item9_open_stage = t_i;	}
int StarGoldData::getItem9OpenStage(){	return item9_open_stage.getV();	}
void StarGoldData::setItem6OpenStage(int t_i){	item6_open_stage = t_i;	}
int StarGoldData::getItem6OpenStage(){	return item6_open_stage.getV();	}
void StarGoldData::setItem11OpenStage(int t_i){	item11_open_stage = t_i;	}
int StarGoldData::getItem11OpenStage(){	return item11_open_stage.getV();	}
void StarGoldData::setItemGachaOpenStage(int t_i){	itemGacha_open_stage = t_i;	}
int StarGoldData::getItemGachaOpenStage(){	return itemGacha_open_stage.getV();	}

void StarGoldData::setPuzzlePerfectRewardRuby(int t_i){	puzzle_perfect_reward_ruby = t_i;	}
int StarGoldData::getPuzzlePerfectRewardRuby(){	return puzzle_perfect_reward_ruby.getV();	}

void StarGoldData::setEndlessMinPiece(int t_i){	endless_min_piece = t_i;	}
int StarGoldData::getEndlessMinPiece(){	return endless_min_piece.getV();	}

void StarGoldData::setEventString(string t_str){	event_string = t_str;	}
string StarGoldData::getEventString(){	return event_string;	}

void StarGoldData::setIsAlwaysSavePlaydata(int t_i){	is_always_save_playdata = t_i;	}
int StarGoldData::getIsAlwaysSavePlaydata(){	return is_always_save_playdata;	}

void StarGoldData::setSendPvpPlayDataRate(int t_i){	send_pvp_play_data_rate = t_i;	}
int StarGoldData::getSendPvpPlayDataRate(){	return send_pvp_play_data_rate;	}

void StarGoldData::setIntroduceStage(int t_i){	introduce_stage = t_i;	}
int StarGoldData::getIntroduceStage(){	return introduce_stage.getV();	}

string StarGoldData::getAllClearReward(){	return all_clear_reward.getV();	}
void StarGoldData::setAllClearReward(string t_str){	all_clear_reward = t_str;	}

void StarGoldData::setClearTakeCardCnt(int t_i){	clear_take_card_cnt = t_i;	}
int StarGoldData::getClearTakeCardCnt(){	return clear_take_card_cnt.getV();	}

void StarGoldData::setUnlockFrameCnt(int t_i){	unlock_frame_cnt = t_i;	}
int StarGoldData::getUnlockFrameCnt(){	return unlock_frame_cnt.getV();	}

void StarGoldData::setSpecialEventPuzzleNumber(int t_i){	special_event_puzzle_number = t_i;	}
int StarGoldData::getSpecialEventPuzzleNumber(){	return special_event_puzzle_number.getV();	}


string StarGoldData::getKakaoMsg(){	return kakao_msg.getV();	}
void StarGoldData::setKakaoMsg(string t_str){	kakao_msg = t_str;	}



//void StarGoldData::setUserdataPGuide(string t_s){	userdata_pGuide = t_s;}
//string StarGoldData::getUserdataPGuide(){	return userdata_pGuide.getV();}
void StarGoldData::setUserdataIsVIP(int t_i)
{
	if(userdata_storage[kUserdataType_isVIP].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_isVIP;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataIsVIP(){	return userdata_storage[kUserdataType_isVIP].getV();}
void StarGoldData::setUserdataIsFirstBuy(int t_i)
{
	if(userdata_storage[kUserdataType_isFirstBuy].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_isFirstBuy;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataIsFirstBuy(){	return userdata_storage[kUserdataType_isFirstBuy].getV();	}
void StarGoldData::setUserdataTotalPlayCount(int t_i)
{
	if(userdata_storage[kUserdataType_totalPlayCount].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_totalPlayCount;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataTotalPlayCount(){	return userdata_storage[kUserdataType_totalPlayCount].getV();	}
void StarGoldData::setUserdataFailCount(int t_i)
{
	if(userdata_storage[kUserdataType_failCount].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_failCount;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataFailCount(){	return userdata_storage[kUserdataType_failCount].getV();	}
void StarGoldData::setUserdataAutoLevel(int t_i)
{
	if(userdata_storage[kUserdataType_autoLevel].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_autoLevel;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAutoLevel(){	return userdata_storage[kUserdataType_autoLevel].getV();	}
void StarGoldData::setUserdataHighScore(int t_i)
{
	if(userdata_storage[kUserdataType_highScore].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_highScore;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataHighScore(){	return userdata_storage[kUserdataType_highScore].getV();	}

void StarGoldData::setUserdataHighPiece(int t_i)
{
	if(userdata_storage[kUserdataType_highPiece].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_highPiece;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataHighPiece(){	return userdata_storage[kUserdataType_highPiece].getV();	}

void StarGoldData::setUserdataOnlyOneBuyPack(int t_i)
{
	if(userdata_storage[kUserdataType_onlyOneBuyPack].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_onlyOneBuyPack;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataOnlyOneBuyPack(){	return userdata_storage[kUserdataType_onlyOneBuyPack].getV();	}

void StarGoldData::setUserdataSelectedCharNO(int t_i)
{
	if(userdata_storage[kUserdataType_selectedCharNO].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_selectedCharNO;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataSelectedCharNO(){	return userdata_storage[kUserdataType_selectedCharNO].getV();	}

void StarGoldData::setUserdataEndlessIngWin(int t_i)
{
	userdata_storage[kUserdataType_endlessData_ingWin] = t_i;
//	if(userdata_storage[kUserdataType_endlessData_ingWin].getV() != t_i)
//	{
//		is_changed_userdata = true;
//		ChangeUserdataValue t_change;
//		t_change.m_type = kUserdataType_endlessData_ingWin;
//		t_change.m_value = t_i;
//		changed_userdata_list.push_back(t_change);
//	}
}
int StarGoldData::getUserdataEndlessIngWin(){	return userdata_storage[kUserdataType_endlessData_ingWin].getV();	}

void StarGoldData::setUserdataEndlessIngWeek(int t_i)
{
	userdata_storage[kUserdataType_endlessData_ingWeek] = t_i;
//	if(userdata_storage[kUserdataType_endlessData_ingWeek].getV() != t_i)
//	{
//		is_changed_userdata = true;
//		ChangeUserdataValue t_change;
//		t_change.m_type = kUserdataType_endlessData_ingWeek;
//		t_change.m_value = t_i;
//		changed_userdata_list.push_back(t_change);
//	}
}
int StarGoldData::getUserdataEndlessIngWeek(){	return userdata_storage[kUserdataType_endlessData_ingWeek].getV();	}
int StarGoldData::getUserdataEndlessScore(){	return userdata_storage[kUserdataType_endlessData_score].getV();	}

void StarGoldData::setUserdataAchieveMapGacha(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_mapGacha].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_mapGacha;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchieveMapGacha(){	return userdata_storage[kUserdataType_achieve_mapGacha].getV();	}
void StarGoldData::setUserdataAchieveNoFail(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_noFail].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_noFail;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchieveNoFail(){	return userdata_storage[kUserdataType_achieve_noFail].getV();	}
void StarGoldData::setUserdataAchieveHunter(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_hunter].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_hunter;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchieveHunter(){	return userdata_storage[kUserdataType_achieve_hunter].getV();	}
void StarGoldData::setUserdataAchieveChangeMania(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_changeMania].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_changeMania;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchieveChangeMania(){	return userdata_storage[kUserdataType_achieve_changeMania].getV();	}
void StarGoldData::setUserdataAchieveFail(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_fail].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_fail;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchieveFail(){	return userdata_storage[kUserdataType_achieve_fail].getV();	}
void StarGoldData::setUserdataAchievePerfect(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_perfect].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_perfect;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchievePerfect(){	return userdata_storage[kUserdataType_achieve_perfect].getV();	}
void StarGoldData::setUserdataAchieveSeqAttendance(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_seqAttendance].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_seqAttendance;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchieveSeqAttendance(){	return userdata_storage[kUserdataType_achieve_seqAttendance].getV();	}
void StarGoldData::setUserdataAchieveItemBuyCount(int t_i)
{
	if(userdata_storage[kUserdataType_achieve_itemBuyCount].getV() != t_i)
	{
		is_changed_userdata = true;
		ChangeUserdataValue t_change;
		t_change.m_type = kUserdataType_achieve_itemBuyCount;
		t_change.m_value = t_i;
		changed_userdata_list.push_back(t_change);
	}
}
int StarGoldData::getUserdataAchieveItemBuyCount(){	return userdata_storage[kUserdataType_achieve_itemBuyCount].getV();	}



void StarGoldData::setDiaryDownUrl(string t_str){	diaryDownUrl = t_str.c_str();	}
string StarGoldData::getDiaryDownUrl(){	return diaryDownUrl;	}

void StarGoldData::setIosHideVer(string t_str){	iosHideVer = t_str.c_str();	}
string StarGoldData::getIosHideVer(){	return iosHideVer;	}