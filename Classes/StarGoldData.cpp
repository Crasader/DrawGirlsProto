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

void StarGoldData::withdraw()
{
	has_gotten_cards.clear();
	puzzle_historys.clear();
	piece_historys.clear();
	goods_data.clear();
	userdata_storage.clear();
	
	is_show_firstPurchase = false;
	at_time_show_emptyItem = 0;
	at_time_show_stupidNpuHelp = 0;
	at_time_show_eventRubyShop = 0;
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
	else if(t_key == kReplayKey_mapTime)								return_value = "mt";
	else if(t_key == kReplayKey_mapData)								return_value = "md";
	else if(t_key == kReplayKey_scoreTime)								return_value = "st";
	else if(t_key == kReplayKey_scoreData)								return_value = "sd";
	else if(t_key == kReplayKey_isChangedMap)							return_value = "icm";
	else if(t_key == kReplayKey_isChangedScore)							return_value = "ics";
	else if(t_key == kReplayKey_playIndex)								return_value = "pi";
	
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
	if(after_loading == kImgType_specialMap)
	{
		filename = "loadingimg_specialMap.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading1);
	}
	else if(after_loading == kImgType_control1)
	{
		filename = "loadingimg_control1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading2);
	}
	else if(after_loading == kImgType_control2)
	{
		filename = "loadingimg_control2.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading3);
	}
	else if(after_loading == kImgType_facebook1)
	{
		filename = "loadingimg_facebook1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading4);
	}
	else if(after_loading == kImgType_facebook2)
	{
		filename = "loadingimg_facebook2.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading5);
	}
	else if(after_loading == kImgType_facebook3)
	{
		filename = "loadingimg_facebook3.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading6);
	}
	else if(after_loading == kImgType_gallery)
	{
		filename = "loadingimg_gallery.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading21);
	}
	else if(after_loading == kImgType_gallery1)
	{
		filename = "loadingimg_gallery1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading22);
	}
	else if(after_loading == kImgType_option)
	{
		filename = "loadingimg_option.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading23);
	}
	else if(after_loading == kImgType_pet)
	{
		filename = "loadingimg_pet.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading24);
	}
	else if(after_loading == kImgType_petbuff)
	{
		filename = "loadingimg_petbuff.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading25);
	}
	else if(after_loading == kImgType_elementalPresentation1)
	{
		filename = "loadingimg_elementalPresentation1.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading17);
	}
	else if(after_loading == kImgType_elemental)
	{
		filename = "loadingimg_elemental.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading19);
	}
	else if(after_loading == kImgType_facebookLoginReward)
	{
		filename = "loadingimg_facebookLoginReward.png";
		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading20);
	}
	
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
	
	if(myLoc->getSupportLocalCode() == "ja")
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		return "meiryo";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		return "fonts/meiryo.ttc"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
	}
	
	string font_name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	font_name = "RixJGoB";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "fonts/RixJGoB.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
	
	return font_name;
}

string StarGoldData::getFont2() // Jrnaver
{
	if(myLoc->getSupportLocalCode() == "ja")
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		return "meiryo";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		return "fonts/meiryo.ttc"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
	}

	string font_name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	font_name = "RixJGoB";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "fonts/RixJGoB.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
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

void StarGoldData::setGameStart()
{
	gacha_item = kIC_emptyEnd;
	
	is_clear_diary = false;
	is_safety_mode = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
	
	ingame_gold = 0;
	
//	if(myDSH->getIntegerForKey(kDSH_Key_endPlayedStage) < mySD->getSilType())
//		myDSH->setIntegerForKey(kDSH_Key_endPlayedStage, mySD->getSilType());
	
	is_write_replay = true;
	
	replay_write_info.clear();
	replay_write_info[getReplayKey(kReplayKey_isChangedMap)] = true;
	replay_write_info[getReplayKey(kReplayKey_isChangedScore)] = true;
	
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
	game_time = 0;
	start_map_gacha_cnt = 0;
	clear_reward_gold = 0;

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
	percentage = t_percentage;

	base_score = t_score;
	
	float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
	
	score = t_score + t_score*stage_grade.getV()*0.5f + t_score*((play_limit_time-t_game_time)/play_limit_time);

	game_time = t_game_time;
	
	if(!mySGD->isClearPiece(mySD->getSilType()))
	{
//		myDSH->setIntegerForKey(kDSH_Key_clearStageCnt, myDSH->getIntegerForKey(kDSH_Key_clearStageCnt)+1);
//		myDSH->setIntegerForKey(kDSH_Key_clearStageNumber_int1, myDSH->getIntegerForKey(kDSH_Key_clearStageCnt), mySD->getSilType());
//		myDSH->setBoolForKey(kDSH_Key_isClearStage_int1, mySD->getSilType(), true);
		
		PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
		t_history.is_clear[t_grade-1] = true;
		mySGD->setPieceHistory(t_history, nullptr);
	}
	
	myGD->setIsGameover(true);
}

void StarGoldData::gameOver( float t_score, float t_percentage, int t_game_time )
{
	for(int i=kIC_emptyBegin+1;i<kIC_emptyEnd;i++)
	{
		before_use_item[i] = is_using_item[i];
		is_using_item[i] = false;
	}

	base_score = t_score;
	score = t_score;
	percentage = t_percentage;
	game_time = t_game_time;
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
	stage_grade = t_i;
}
int StarGoldData::getStageGrade()
{
	return stage_grade.getV();
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
	int t_size = has_gotten_cards.size();

	if(t_size == 1)
		return -1;

	int found_number = -1;
	for(int i=0;i<t_size;i++)
	{
		if(recent_card_number == has_gotten_cards[i].card_number)
		{
			found_number = i;
			break;
		}
	}

	if(found_number == -1) // not found
		return -1;

	if(found_number >= t_size-1)
		return has_gotten_cards[0].card_number;
	else
		return has_gotten_cards[found_number+1].card_number;
}

int StarGoldData::getNextStageCardNumber( int recent_card_number )
{
	int ing_card_number = recent_card_number;
	bool is_found = false;
	do{
		ing_card_number = getNextCardNumber(ing_card_number);
		if(ing_card_number == -1)		break;
		if(NSDS_GI(kSDS_CI_int1_stage_i, ing_card_number) != NSDS_GI(kSDS_CI_int1_stage_i, recent_card_number))
			is_found = true;
	}while(!is_found && ing_card_number != recent_card_number);

	if(!is_found)
		return -1;
	else
	{
		int ing_card_stage = NSDS_GI(kSDS_CI_int1_stage_i, ing_card_number);
		if(mySGD->isHasGottenCards(ing_card_stage, 4) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 4);
		else if(mySGD->isHasGottenCards(ing_card_stage, 3) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3);
		else if(mySGD->isHasGottenCards(ing_card_stage, 2) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 2);
		else
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 1);
	}
}

int StarGoldData::getPreCardNumber( int recent_card_number )
{
	int t_size = has_gotten_cards.size();

	if(t_size == 1)
		return -1;

	int found_number = -1;
	for(int i=0;i<t_size;i++)
	{
		if(recent_card_number == has_gotten_cards[i].card_number)
		{
			found_number = i;
			break;
		}
	}

	if(found_number == -1) // not found
		return -1;

	if(found_number <= 0)
		return has_gotten_cards[t_size-1].card_number;
	else
		return has_gotten_cards[found_number-1].card_number;
}

int StarGoldData::getPreStageCardNumber( int recent_card_number )
{
	int ing_card_number = recent_card_number;
	bool is_found = false;
	do{
		ing_card_number = getPreCardNumber(ing_card_number);
		if(ing_card_number == -1)		break;
		if(NSDS_GI(kSDS_CI_int1_stage_i, ing_card_number) != NSDS_GI(kSDS_CI_int1_stage_i, recent_card_number))
			is_found = true;
	}while(!is_found && ing_card_number != recent_card_number);

	if(!is_found)
		return -1;
	else
	{
		int ing_card_stage = NSDS_GI(kSDS_CI_int1_stage_i, ing_card_number);
		if(mySGD->isHasGottenCards(ing_card_stage, 4) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 4);
		else if(mySGD->isHasGottenCards(ing_card_stage, 3) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3);
		else if(mySGD->isHasGottenCards(ing_card_stage, 2) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 2);
		else
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 1);
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
				return a.card_number < b.card_number;
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number > b.take_number;
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number < b.take_number;
			}
		} pred;
		
		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_gradeUp) // rank
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank > b.rank;
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
	else if(t_type == kCST_gradeDown) // rank
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank < b.rank;
			}
		} pred;

		sort(has_gotten_cards.begin(), has_gotten_cards.end(), pred);
	}
}

void StarGoldData::addHasGottenCardNumber( int card_number )
{
//	int take_number = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number);
	
	CardSortInfo t_info;
	t_info.card_number = card_number;
	t_info.take_number = has_gotten_cards.size()+1;
	t_info.grade = NSDS_GI(kSDS_CI_int1_grade_i, t_info.card_number);
	t_info.rank = NSDS_GI(kSDS_CI_int1_rank_i, t_info.card_number);
	has_gotten_cards.push_back(t_info);

	changeSortType(CardSortType(myDSH->getIntegerForKey(kDSH_Key_cardSortType)));

	CCLOG("input %d, sort", card_number);
	for(int i=0;i<has_gotten_cards.size();i++)
	{
		CCLOG("%d", has_gotten_cards[i].card_number);
	}
}

int StarGoldData::getHasGottenCardsDataCardNumber( int index )
{
	return has_gotten_cards[index].card_number;
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
		if(i->card_number == t_card_number)
			return i->user_ment;
	}
	
	return "";
}

void StarGoldData::setCardComment(int t_card_number, string comment)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number == t_card_number)
			i->user_ment = comment;
	}
}

int StarGoldData::isHasGottenCards(int t_card_number)
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(i->card_number == t_card_number)
			return i->card_number;
	}
	
	return 0;
}

int StarGoldData::isHasGottenCards( int t_stage, int t_grade )
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(NSDS_GI(kSDS_CI_int1_stage_i, i->card_number) == t_stage && i->grade == t_grade)
			return i->card_number;
	}

	return 0;
}

void StarGoldData::resetHasGottenCards()
{
	for(int i=0;i<has_gotten_cards.size();i++)
	{
		int card_number = has_gotten_cards[i].card_number;
		has_gotten_cards[i].grade = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
		has_gotten_cards[i].rank = NSDS_GI(kSDS_CI_int1_rank_i, card_number);
	}
	
	changeSortType(CardSortType(myDSH->getIntegerForKey(kDSH_Key_cardSortType)));
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
		has_gotten_cards.push_back(t_info);
		
		if(NSDS_GS(kSDS_CI_int1_imgInfo_s, card_number) == "")
			card_data_load_list.push_back(card_number);
	}
}

int StarGoldData::getOpenPuzzleCount()
{
	int history_count = getPuzzleHistorySize();
	int open_puzzle_count = 0;
	for(int i=0;i<history_count;i++)
		if(getPuzzleHistoryForIndex(i).is_open)
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
		if(puzzle_historys[i].puzzle_number == t_history.puzzle_number)
		{
			is_found = true;
			puzzle_historys[i].is_open = t_history.is_open;
			puzzle_historys[i].is_clear = t_history.is_clear;
			puzzle_historys[i].is_perfect = t_history.is_perfect;
			puzzle_historys[i].open_type = t_history.open_type;
		}
	}
	
	if(!is_found)
		puzzle_historys.push_back(t_history);
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["puzzleNo"] = t_history.puzzle_number;
	param["updateOpenDate"] = t_history.is_open;
	param["updateClearDate"] = t_history.is_clear;
	param["updatePerfectDate"] = t_history.is_perfect;
	param["openType"] = t_history.open_type;
	
	return CommandParam("updatePuzzleHistory", param, call_back);
}

void StarGoldData::setPuzzleHistory(PuzzleHistory t_history, jsonSelType call_back)
{
	setPuzzleHistoryForNotSave(t_history);
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["puzzleNo"] = t_history.puzzle_number;
	param["updateOpenDate"] = t_history.is_open;
	param["updateClearDate"] = t_history.is_clear;
	param["updatePerfectDate"] = t_history.is_perfect;
	param["openType"] = t_history.open_type;
	hspConnector::get()->command("updatePuzzleHistory", param, call_back);
}

void StarGoldData::setPuzzleHistoryForNotSave(PuzzleHistory t_history)
{
	bool is_found = false;
	for(int i=0;i<puzzle_historys.size() && !is_found;i++)
	{
		if(puzzle_historys[i].puzzle_number == t_history.puzzle_number)
		{
			is_found = true;
			puzzle_historys[i].is_open = t_history.is_open;
			puzzle_historys[i].is_clear = t_history.is_clear;
			puzzle_historys[i].is_perfect = t_history.is_perfect;
			puzzle_historys[i].open_type = t_history.open_type;
		}
	}
	
	if(!is_found)
		puzzle_historys.push_back(t_history);
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
		if(piece_historys[i].stage_number == stage_number)
			return piece_historys[i].is_clear[0] || piece_historys[i].is_clear[1] || piece_historys[i].is_clear[2] || piece_historys[i].is_clear[3];
	
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
		if(piece_historys[i].stage_number == stage_number)
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
	param["pieceNo"] = t_history.stage_number;
	param["openDate"] = t_history.is_open;
	for(int j=0;j<4;j++)
		param["clearDateList"][j] = t_history.is_clear[j];
	param["tryCount"] = t_history.try_count;
	param["clearCount"] = t_history.clear_count;
	param["openType"] = t_history.open_type;
	return param;
}

void StarGoldData::setPieceHistoryForNotSave(PieceHistory t_history)
{
	bool is_found = false;
	for(int i=0;i<piece_historys.size() && !is_found;i++)
	{
		if(piece_historys[i].stage_number == t_history.stage_number)
		{
			is_found = true;
			piece_historys[i].stage_number = t_history.stage_number;
			piece_historys[i].is_open = t_history.is_open;
			
			for(int j=0;j<4;j++)
			{
				piece_historys[i].is_clear[j] = t_history.is_clear[j];
			}
			
			piece_historys[i].try_count = t_history.try_count;
			piece_historys[i].clear_count = t_history.clear_count;
			piece_historys[i].open_type = t_history.open_type;
		}
	}
	
	if(!is_found)
	{
		piece_historys.push_back(t_history);
	}
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
	for(int i=0;i<history_list.size();i++)
	{
		Json::Value t_data = history_list[i];
		CharacterHistory t_history;
		t_history.characterNo = t_data["characterNo"].asInt();
		t_history.level = t_data["level"].asInt();
		t_history.nextPrice = t_data["nextPrice"].asInt();
		
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
			CharacterHistory t_history = getCharacterHistory(i);
			if(t_history.characterNo.getV() == characterNo)
			{
				t_history.level = result_data["level"].asInt();
				t_history.nextPrice = result_data["nextPrice"].asInt();
				is_found = true;
			}
		}
		
		if(!is_found)
		{
			CharacterHistory t_history;
			t_history.characterNo = characterNo;
			t_history.level = result_data["level"].asInt();
			t_history.nextPrice = result_data["nextPrice"].asInt();
			
			character_historys.push_back(t_history);
		}
	}
	
	if(keep_character_history_callback != nullptr)
		keep_character_history_callback(result_data);
}

int StarGoldData::getClearStarCount()
{
	int return_value = 0;
	for(int i=0;i<piece_historys.size();i++)
	{
		for(int j=0;j<4;j++)
		{
			if(piece_historys[i].is_clear[j])
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
	else if(t_type == kGoodsType_item8)
		return_value = "i8";
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
	else if(t_code == kIC_longTime)
		return_value = kGoodsType_item8;
	return return_value;
}

void StarGoldData::initProperties(Json::Value t_list)
{
	goods_data.clear();
	for(int i=0;i<t_list.size();i++)
	{
		GoodsType t_type = getGoodsKeyToType(t_list[i]["type"].asString());
		int t_count = t_list[i]["count"].asInt();
		
		goods_data[t_type] = t_count;
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

void StarGoldData::addChangeGoods(GoodsType t_type, int t_value, string t_statsID/* = ""*/, string t_statsValue/* = ""*/, string t_content/* = ""*/, bool t_isPurchase/* = false*/)
{
	// add
	for(int i=0;i<change_goods_list.size();i++)
	{
		if(change_goods_list[i].m_type == t_type)
		{
			change_goods_list[i].m_value = change_goods_list[i].m_value.getV() + t_value;
			if(t_statsID != "")
				change_goods_list[i].m_statsID += " | " + t_statsID;
			if(t_statsValue != "")
				change_goods_list[i].m_statsValue += " | " + t_statsValue;
			if(t_content != "")
				change_goods_list[i].m_content += " | " + t_content;
			change_goods_list[i].m_isPurchase = t_isPurchase;
			return;
		}
	}
	
	// insert
	ChangeGoodsData t_data;
	t_data.m_type = t_type;
	t_data.m_value = t_value;
	t_data.m_statsID = t_statsID;
	t_data.m_statsValue = t_statsValue;
	t_data.m_content = t_content;
	t_data.m_isPurchase = t_isPurchase;
	
	change_goods_list.push_back(t_data);
}

void StarGoldData::updateChangeGoods(GoodsType t_type, int t_value, string t_statsID/* = ""*/, string t_statsValue/* = ""*/, string t_content/* = ""*/, bool t_isPurchase/* = false*/)
{
	// modify
	for(int i=0;i<change_goods_list.size();i++)
	{
		if(change_goods_list[i].m_type == t_type)
		{
			change_goods_list[i].m_value = t_value;
			change_goods_list[i].m_statsID = t_statsID;
			change_goods_list[i].m_statsValue = t_statsValue;
			change_goods_list[i].m_content = t_content;
			change_goods_list[i].m_isPurchase = t_isPurchase;
			return;
		}
	}
	
	// insert
	ChangeGoodsData t_data;
	t_data.m_type = t_type;
	t_data.m_value = t_value;
	t_data.m_statsID = t_statsID;
	t_data.m_statsValue = t_statsValue;
	t_data.m_content = t_content;
	t_data.m_isPurchase = t_isPurchase;
	
	change_goods_list.push_back(t_data);
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
		userdata_storage[(UserdataType)i] = result_data[getUserdataTypeToKey((UserdataType)i)].asInt();
		if(i == kUserdataType_selectedCharNO)
			initSelectedCharacterNo(userdata_storage[(UserdataType)i].getV());
	}
}

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
	addChangeGoods(kGoodsType_gold, ingame_gold.getV(), "인게임", CCString::createWithFormat("%d", mySD->getSilType())->getCString());
	
	vector<CommandParam> transaction_list;
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getSocialID();
	
	transaction_list.push_back(CommandParam("starttransaction", transaction_param, t_callback));
	if(isChangedGoods())
		transaction_list.push_back(getChangeGoodsParam(json_selector(this, StarGoldData::saveChangeGoodsTransaction)));
	
	for(int i=0;i<command_list.size();i++)
	{
		transaction_list.push_back(command_list[i]);
	}
	
	hspConnector::get()->command(transaction_list);
}

void StarGoldData::saveChangeGoodsTransaction(Json::Value result_data)
{
	GraphDogLib::JsonToLog("transaction changeuserproperties", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		change_goods_list.clear();
		
		Json::Value result_list = result_data["list"];
		
		for(int i=0;i<result_list.size();i++)
		{
			GoodsType t_type = getGoodsKeyToType(result_list[i]["type"].asString());
			int t_count = result_list[i]["count"].asInt();
			goods_data[t_type] = t_count;
			
			if(t_type == kGoodsType_ruby && star_label)
				star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
			else if(t_type == kGoodsType_gold && gold_label)
				gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
		}
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		GoodsType t_type = getGoodsKeyToType(result_data["minusType"].asString());
		int t_count = result_data["minusCount"].asInt();
		
		goods_data[t_type] = t_count;
		
		if(t_type == kGoodsType_ruby && star_label)
			star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
		else if(t_type == kGoodsType_gold && gold_label)
			gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
	}
	else
	{
		
	}
}

void StarGoldData::refreshGoodsData(string t_key, int t_count)
{
	GoodsType t_type = getGoodsKeyToType(t_key);
	goods_data[t_type] = t_count;
	
	if(t_type == kGoodsType_ruby && star_label)
		star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
	else if(t_type == kGoodsType_gold && gold_label)
		gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
}

bool StarGoldData::isChangedGoods()
{
	return change_goods_list.size() > 0;
}

CommandParam StarGoldData::getChangeGoodsParam(jsonSelType t_callback)
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	
	Json::Value p_list;
	
	for(int i=0;i<change_goods_list.size();i++)
	{
		p_list[i]["type"] = getGoodsTypeToKey(change_goods_list[i].m_type);
		p_list[i]["count"] = change_goods_list[i].m_value.getV();
		if(change_goods_list[i].m_statsID != "")
			p_list[i]["statsID"] = change_goods_list[i].m_statsID;
		if(change_goods_list[i].m_statsValue != "")
			p_list[i]["statsValue"] = change_goods_list[i].m_statsValue;
		if(change_goods_list[i].m_content != "")
			p_list[i]["content"] = change_goods_list[i].m_content;
		if(change_goods_list[i].m_type == kGoodsType_ruby)
			p_list[i]["isPurchase"] = change_goods_list[i].m_isPurchase;
	}
	
	param["list"] = p_list;
	
	return CommandParam("changeuserproperties", param, t_callback);
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
		Json::Value param;
		param["memberID"] = hspConnector::get()->getSocialID();
		
		Json::Value p_list;
		
		for(int i=0;i<change_goods_list.size();i++)
		{
			p_list[i]["type"] = getGoodsTypeToKey(change_goods_list[i].m_type);
			p_list[i]["count"] = change_goods_list[i].m_value.getV();
			if(change_goods_list[i].m_statsID != "")
				p_list[i]["statsID"] = change_goods_list[i].m_statsID;
			if(change_goods_list[i].m_statsValue != "")
				p_list[i]["statsValue"] = change_goods_list[i].m_statsValue;
			if(change_goods_list[i].m_content != "")
				p_list[i]["content"] = change_goods_list[i].m_content;
			if(change_goods_list[i].m_type == kGoodsType_ruby)
				p_list[i]["isPurchase"] = change_goods_list[i].m_isPurchase;
		}
		
		param["list"] = p_list;
		
		hspConnector::get()->command("changeuserproperties", param, json_selector(this, StarGoldData::resultChangeGoods));
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
			int t_count = result_list[i]["count"].asInt();
			goods_data[t_type] = t_count;
			
			if(t_type == kGoodsType_ruby && star_label)
				star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
			else if(t_type == kGoodsType_gold && gold_label)
				gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
		}
		
		change_goods_callback(result_data);
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		GoodsType t_type = getGoodsKeyToType(result_data["minusType"].asString());
		int t_count = result_data["minusCount"].asInt();
		
		goods_data[t_type] = t_count;
		
		if(t_type == kGoodsType_ruby && star_label)
			star_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
		else if(t_type == kGoodsType_gold && gold_label)
			gold_label->setString(CCString::createWithFormat("%d", t_count)->getCString());
		
		change_goods_callback(result_data);
	}
	else
	{
		retryChangeGoods();
	}
}

bool StarGoldData::isPossibleShowPurchasePopup(PurchaseGuideType t_type)
{
	bool return_value = true;
	if(t_type == kPurchaseGuideType_firstPurchase)
		return_value = !is_show_firstPurchase;
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
	return return_value;
}

void StarGoldData::showPurchasePopup(PurchaseGuideType t_type)
{
	if(t_type == kPurchaseGuideType_firstPurchase)
		is_show_firstPurchase = true;
	else if(t_type == kPurchaseGuideType_emptyItem)
		at_time_show_emptyItem = graphdog->getTime();
	else if(t_type == kPurchaseGuideType_stupidNpuHelp)
		at_time_show_stupidNpuHelp = graphdog->getTime();
	else if(t_type == kPurchaseGuideType_eventRubyShop)
		at_time_show_eventRubyShop = graphdog->getTime();
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
	app_type = "light1";
	app_version = 1;
	
	suitable_stage = -1;
	
	gacha_item = kIC_emptyEnd;
	
	rank_up_add_rate = 0;
	keep_time_info.is_loaded = false;
	
	is_show_firstPurchase = false;
	at_time_show_emptyItem = 0;
	at_time_show_stupidNpuHelp = 0;
	at_time_show_eventRubyShop = 0;
	
	goods_data.clear();
	change_goods_list.clear();
	
	changed_userdata_list.clear();
	
	replay_write_info.clear();
	replay_playing_info.clear();
	is_write_replay = false;
	is_play_replay = false;
	
	is_before_stage_img_download = false;
	
	is_unlock_puzzle = 0;
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
}

bool StarGoldData::getIsNotClearedStage(){	return is_not_cleared_stage;}
int StarGoldData::getIsUnlockPuzzle(){	return is_unlock_puzzle;}
void StarGoldData::setIsUnlockPuzzle(int t_i){	is_unlock_puzzle = t_i;}
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

void StarGoldData::initInappProduct(int t_index, string t_product){	inapp_products[t_index] = t_product;}
string StarGoldData::getInappProduct(int t_index){	return inapp_products[t_index].getV();}

void StarGoldData::initEventInappProduct(int t_index, string t_product){	event_inapp_products[t_index] = t_product;}
string StarGoldData::getEventInappProduct(int t_index){	return event_inapp_products[t_index].getV();}

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

void StarGoldData::setFirstPurchasePlayCount(int t_i){	first_purchase_play_count = t_i;	}
int StarGoldData::getFirstPurchasePlayCount(){	return first_purchase_play_count.getV();	}
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

