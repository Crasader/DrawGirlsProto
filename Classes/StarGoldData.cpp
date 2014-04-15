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
	string font_name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	font_name = "RixJGoB";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "fonts/RixJGoB.ttf"; //RixHeadEB.ttf //RixMGoB.ttf //RixJGoB
#endif
	
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
}

void StarGoldData::setStarLabel( CCLabelBMFont* t_label )
{
	star_label = t_label;
}

int StarGoldData::getStar()
{
	return myDSH->getIntegerForKey(kDSH_Key_savedStar);
}

void StarGoldData::setStar( int t_star )
{
	if(myDSH->getIntegerForKey(kDSH_Key_savedStar) < t_star)
	{
		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
		
		for(int i=kAchievementCode_ruby1;i<=kAchievementCode_ruby3;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
			   t_star >= shared_acr->getCondition((AchievementCode)i))
			{
				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
	}
	
	if(star_label && myDSH->getIntegerForKey(kDSH_Key_savedStar) != t_star)
		star_label->setString(CCString::createWithFormat("%d", t_star)->getCString());
	
	myDSH->setIntegerForKey(kDSH_Key_savedStar, t_star);
}

void StarGoldData::setGoldLabel( CCLabelBMFont* t_label )
{
	gold_label = t_label;
}
int StarGoldData::getGold()
{
	return myDSH->getIntegerForKey(kDSH_Key_savedGold);
}
void StarGoldData::setGold( int t_gold , bool is_write/* = true */)
{
	if(myDSH->getIntegerForKey(kDSH_Key_savedGold) < t_gold)
	{
		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
		
		for(int i=kAchievementCode_gold1;i<=kAchievementCode_gold3;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
			   t_gold >= shared_acr->getCondition((AchievementCode)i))
			{
				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
	}
	
	if(gold_label && myDSH->getIntegerForKey(kDSH_Key_savedGold) != t_gold)
		gold_label->setString(CCString::createWithFormat("%d", t_gold)->getCString());
	
	myDSH->setIntegerForKey(kDSH_Key_savedGold, t_gold, is_write);
}

int StarGoldData::getKeepGold()
{
	return keep_gold;
}

void StarGoldData::setKeepGold( int t_gold )
{
	keep_gold = t_gold;
}

void StarGoldData::setGameStart()
{
	if(myDSH->getIntegerForKey(kDSH_Key_endPlayedStage) < mySD->getSilType())
		myDSH->setIntegerForKey(kDSH_Key_endPlayedStage, mySD->getSilType());
	
	is_write_replay = true;
	
	replay_write_info.clear();
	replay_write_info[getReplayKey(kReplayKey_isChangedMap)] = true;
	replay_write_info[getReplayKey(kReplayKey_isChangedScore)] = true;
	
	ingame_before_stage_rank = myDSH->getIntegerForKey(kDSH_Key_stageClearRank_int1, mySD->getSilType());
	is_not_cleared_stage = !myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, mySD->getSilType());
	
	mySD->startSetting();
	
	if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1)) > 0 || myDSH->getIntegerForKey(kDSH_Key_selectedCard) == NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))
		is_ingame_before_have_stage_cards[0] = true;
	else
		is_ingame_before_have_stage_cards[0] = false;
	if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 2)) > 0 || myDSH->getIntegerForKey(kDSH_Key_selectedCard) == NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 2))
		is_ingame_before_have_stage_cards[1] = true;
	else
		is_ingame_before_have_stage_cards[1] = false;
	if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3)) > 0 || myDSH->getIntegerForKey(kDSH_Key_selectedCard) == NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3))
		is_ingame_before_have_stage_cards[2] = true;
	else
		is_ingame_before_have_stage_cards[2] = false;
	
	is_showtime = false;
	is_exchanged = false;
	is_cleared = false;
	score = 0.f;
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

	score = t_score + t_score*(stage_grade-1.f)*0.5f + t_score*(1.f-(t_use_time*1.f)/t_total_time);

	game_time = t_game_time;
	
	int before_clear_rank = myDSH->getIntegerForKey(kDSH_Key_stageClearRank_int1, mySD->getSilType());
	int recent_clear_rank;
	if(percentage == 1.f)
		recent_clear_rank = 3;
	else if(percentage >= 0.95f)
		recent_clear_rank = 2;
	else
		recent_clear_rank = 1;
	
	if(before_clear_rank < recent_clear_rank)
		myDSH->setIntegerForKey(kDSH_Key_stageClearRank_int1, mySD->getSilType(), recent_clear_rank);
	
	if(!myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, mySD->getSilType()))
	{
		myDSH->setIntegerForKey(kDSH_Key_clearStageCnt, myDSH->getIntegerForKey(kDSH_Key_clearStageCnt)+1);
		myDSH->setIntegerForKey(kDSH_Key_clearStageNumber_int1, myDSH->getIntegerForKey(kDSH_Key_clearStageCnt), mySD->getSilType());
		myDSH->setBoolForKey(kDSH_Key_isClearStage_int1, mySD->getSilType(), true);
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

	score = t_score;
	percentage = t_percentage;
	game_time = t_game_time;
	myGD->setIsGameover(true);
}

bool StarGoldData::getIsCleared()
{
	return is_cleared;
}

float StarGoldData::getScore()
{
	return score;
}

float StarGoldData::getPercentage()
{
	return percentage;
}

int StarGoldData::getStageGrade()
{
	return stage_grade;
}

int StarGoldData::getStageGold()
{
	return myDSH->getIntegerForKey(kDSH_Key_savedGold)-keep_gold;
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
	return game_time;
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
		if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 4)) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 4);
		else if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3)) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3);
		else if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 2)) > 0)
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
		if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 4)) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 4);
		else if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3)) > 0)
			return NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3);
		else if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 2)) > 0)
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

	CCLog("input %d, sort", card_number);
	for(int i=0;i<has_gotten_cards.size();i++)
	{
		CCLog("%d", has_gotten_cards[i].card_number);
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

int StarGoldData::isHasGottenCards( int t_stage, int t_grade )
{
	for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
	{
		if(NSDS_GI(kSDS_CI_int1_stage_i, (*i).card_number) == t_stage && (*i).grade == t_grade)
			return (*i).card_number;
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
		
		myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, card_number, 100, false);
		myDSH->setIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number, i+1, false);
		myDSH->setIntegerForKey(kDSH_Key_takeCardNumber_int1, i+1, card_number, false);
		
		if(NSDS_GS(kSDS_CI_int1_imgInfo_s, card_number) == "")
			card_data_load_list.push_back(card_number);
	}
	
	myDSH->setIntegerForKey(kDSH_Key_cardTakeCnt, card_cnt, false);
	myDSH->fFlush();
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
	
	chrono::time_point<chrono::system_clock> chrono_now_time = chrono::system_clock::now();
	time_t now_time = chrono::system_clock::to_time_t(chrono_now_time);
	struct tm* struct_time = localtime(&now_time);
	string time_string = "";
	time_string += CCString::createWithFormat("%04d", struct_time->tm_year+1900)->getCString();
	time_string += CCString::createWithFormat("%02d", struct_time->tm_mon+1)->getCString();
	time_string += CCString::createWithFormat("%02d", struct_time->tm_mday)->getCString();
	time_string += CCString::createWithFormat("%02d", struct_time->tm_hour)->getCString();
	time_string += CCString::createWithFormat("%02d", struct_time->tm_min)->getCString();
	time_string += CCString::createWithFormat("%02d", struct_time->tm_sec)->getCString();
	
	int64_t now_value = atoll(time_string.c_str());
	
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
	
	replay_write_info.clear();
	replay_playing_info.clear();
	is_write_replay = false;
	is_play_replay = false;
	
	is_unlock_puzzle = 0;
	strength_target_card_number = 0;
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

bool StarGoldData::getIsNotClearedStage()
{
	return is_not_cleared_stage;
}

int StarGoldData::getIsUnlockPuzzle()
{
	return is_unlock_puzzle;
}
void StarGoldData::setIsUnlockPuzzle(int t_i)
{
	is_unlock_puzzle = t_i;
}

void StarGoldData::setStrengthTargetCardNumber(int t_card_number)
{
	strength_target_card_number = t_card_number;
}

int StarGoldData::getStrengthTargetCardNumber()
{
	return strength_target_card_number;
}

CardStrengthBefore StarGoldData::getCardStrengthBefore()
{
	return card_strength_before;
}

void StarGoldData::setCardStrengthBefore(CardStrengthBefore t_before)
{
	card_strength_before = t_before;
}

void StarGoldData::setHeartMax(int t_data)
{
	heart_max = t_data;
}
void StarGoldData::setHeartCoolTime(int t_data)
{
	heart_cool_time = t_data;
}
void StarGoldData::setGameFriendMax(int t_data)
{
	game_friend_max = t_data;
}
void StarGoldData::setHelpCoolTime(int t_data)
{
	help_cool_time = t_data;
}
void StarGoldData::setChallengeCoolTime(int t_data)
{
	challenge_cool_time = t_data;
}
void StarGoldData::setMsgRemoveDay(int t_data)
{
	msg_remove_day = t_data;
}
void StarGoldData::setGachaGoldFee(int t_data)
{
	gacha_gold_fee = t_data;
}
void StarGoldData::setGachaRubyFee(int t_data)
{
	gacha_ruby_fee = t_data;
}
void StarGoldData::setGachaSocialFee(int t_data)
{
	gacha_social_fee = t_data;
}
void StarGoldData::setGachaGoldFeeRetry(int t_data)
{
	gacha_gold_fee_retry = t_data;
}
void StarGoldData::setGachaRubyFeeRetry(int t_data)
{
	gacha_ruby_fee_retry = t_data;
}
void StarGoldData::setGachaSocialFeeRetry(int t_data)
{
	gacha_social_fee_retry = t_data;
}
void StarGoldData::setCardUpgradeGoldFee(int t_data)
{
	card_upgrade_gold_fee = t_data;
}
void StarGoldData::setCardUpgradeRubyFee(int t_data)
{
	card_upgrade_ruby_fee = t_data;
}
void StarGoldData::setHeartSendCoolTime(int t_data)
{
	heart_send_cool_time = t_data;
}
void StarGoldData::setInviteMaxADay(int t_data)
{
	invite_max_a_day = t_data;
}
void StarGoldData::setInviteCoolDay(int t_data)
{
	invite_cool_day = t_data;
}
void StarGoldData::setPlayContinueFee(int t_data)
{
	play_continue_fee = t_data;
}
void StarGoldData::setCardDurabilityUpFee(int t_data)
{
	card_durability_up_fee = t_data;
}
void StarGoldData::setGachaMapFee(int t_data)
{
	gacha_map_fee = t_data;
}
void StarGoldData::setRemoveFriendCoolTime(int t_data)
{
	remove_friend_cool_time = t_data;
}
void StarGoldData::setSPSendHeart(int t_data)
{
	SP_send_heart = t_data;
}
void StarGoldData::setSPSendTicket(int t_data)
{
	SP_send_ticket = t_data;
}
void StarGoldData::setSPFinishedChallenge(int t_data)
{
	SP_finished_challenge = t_data;
}
void StarGoldData::setSPHelpChallenge(int t_data)
{
	SP_help_challenge = t_data;
}
void StarGoldData::setSPSendBoast(int t_data)
{
	SP_send_boast = t_data;
}
void StarGoldData::setSPGetTime(int t_data)
{
	SP_get_time = t_data;
}
void StarGoldData::setSPGetHeart(int t_data)
{
	SP_get_heart = t_data;
}
void StarGoldData::setGachaOnePercentFee(int t_data)
{
	gacha_one_percent_fee = t_data;
}
void StarGoldData::setAiAdderOnDrewOrDamaged(float t_data)
{
	ai_adder_on_drew_or_damaged = t_data;
}
void StarGoldData::setFuryPercent(float t_data)
{
	fury_percent = t_data;
}
void StarGoldData::setSPRentCardThanks(int t_data)
{
	SP_rent_card_thanks = t_data;
}

int StarGoldData::getHeartMax()
{
	return heart_max.getV();
}
int StarGoldData::getHeartCoolTime()
{
	return heart_cool_time.getV();
}
int StarGoldData::getGameFriendMax()
{
	return game_friend_max.getV();
}
int StarGoldData::getHelpCoolTime()
{
	return help_cool_time.getV();
}
int StarGoldData::getChallengeCoolTime()
{
	return challenge_cool_time.getV();
}
int StarGoldData::getMsgRemoveDay()
{
	return msg_remove_day.getV();
}
int StarGoldData::getGachaGoldFee()
{
	return gacha_gold_fee.getV();
}
int StarGoldData::getGachaRubyFee()
{
	return gacha_ruby_fee.getV();
}
int StarGoldData::getGachaSocialFee()
{
	return gacha_social_fee.getV();
}
int StarGoldData::getGachaGoldFeeRetry()
{
	return gacha_gold_fee_retry.getV();
}
int StarGoldData::getGachaRubyFeeRetry()
{
	return gacha_ruby_fee_retry.getV();
}
int StarGoldData::getGachaSocialFeeRetry()
{
	return gacha_social_fee_retry.getV();
}
int StarGoldData::getCardUpgradeGoldFee()
{
	return card_upgrade_gold_fee.getV();
}
int StarGoldData::getCardUpgradeRubyFee()
{
	return card_upgrade_ruby_fee.getV();
}
int StarGoldData::getHeartSendCoolTime()
{
	return heart_send_cool_time.getV();
}
int StarGoldData::getInviteMaxADay()
{
	return invite_max_a_day.getV();
}
int StarGoldData::getInviteCoolDay()
{
	return invite_cool_day.getV();
}
int StarGoldData::getPlayContinueFee()
{
	return play_continue_fee.getV();
}
int StarGoldData::getCardDurabilityUpFee()
{
	return card_durability_up_fee.getV();
}
int StarGoldData::getGachaMapFee()
{
	return gacha_map_fee.getV();
}
int StarGoldData::getRemoveFriendCoolTime()
{
	return remove_friend_cool_time.getV();
}
int StarGoldData::getSPSendHeart()
{
	return SP_send_heart.getV();
}
int StarGoldData::getSPSendTicket()
{
	return SP_send_ticket.getV();
}
int StarGoldData::getSPFinishedChallenge()
{
	return SP_finished_challenge.getV();
}
int StarGoldData::getSPHelpChallenge()
{
	return SP_help_challenge.getV();
}
int StarGoldData::getSPSendBoast()
{
	return SP_send_boast.getV();
}
int StarGoldData::getSPGetTime()
{
	return SP_get_time.getV();
}
int StarGoldData::getSPGetHeart()
{
	return SP_get_heart.getV();
}
int StarGoldData::getGachaOnePercentFee()
{
	return gacha_one_percent_fee.getV();
}
float StarGoldData::getAiAdderOnDrewOrDamaged()
{
	return ai_adder_on_drew_or_damaged.getV();
}
float StarGoldData::getFuryPercent()
{
	return fury_percent.getV();
}
int StarGoldData::getSPRentCardThanks()
{
	return SP_rent_card_thanks.getV();
}
