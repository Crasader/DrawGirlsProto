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
//	else if(after_loading == kImgType_item1)
//	{
//		filename = "loadingimg_item1.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading11);
//	}
//	else if(after_loading == kImgType_item2)
//	{
//		filename = "loadingimg_item2.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading12);
//	}
//	else if(after_loading == kImgType_item3)
//	{
//		filename = "loadingimg_item3.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading13);
//	}
//	else if(after_loading == kImgType_item4)
//	{
//		filename = "loadingimg_item4.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading14);
//	}
//	else if(after_loading == kImgType_item5)
//	{
//		filename = "loadingimg_item5.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading15);
//	}
//	else if(after_loading == kImgType_item6)
//	{
//		filename = "loadingimg_item6.png";
//		ments = MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_loading16);
//	}
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
	font_name = "AppleGothic";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	font_name = "";
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
	myDSH->setIntegerForKey(kDSH_Key_savedStar, t_star);

	if(star_label)
		star_label->setString(CCString::createWithFormat("%d", t_star)->getCString());
}

void StarGoldData::setGoldLabel( CCLabelBMFont* t_label )
{
	gold_label = t_label;
}
int StarGoldData::getGold()
{
	return myDSH->getIntegerForKey(kDSH_Key_savedGold);
}
void StarGoldData::setGold( int t_gold )
{
	myDSH->setIntegerForKey(kDSH_Key_savedGold, t_gold);

	if(gold_label)
		gold_label->setString(CCString::createWithFormat("%d", t_gold)->getCString());
}

void StarGoldData::setFriendPointLabel(CCLabelBMFont* t_label)
{
	friend_point_label = t_label;
}
int StarGoldData::getFriendPoint()
{
	return myDSH->getIntegerForKey(kDSH_Key_savedFriendPoint);
}
void StarGoldData::setFriendPoint(int t_point)
{
	myDSH->setIntegerForKey(kDSH_Key_savedFriendPoint, t_point);
	
	if(friend_point_label)
		friend_point_label->setString(CCString::createWithFormat("%d", t_point)->getCString());
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
	mySD->startSetting();
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
	mySD->setCardOptions(card_options, selected_card_number);

	doubleItem_value = 0;
	if(isUsingItem(kIC_doubleItem))		doubleItem_value += mySD->getDoubleItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_doubleItem);
	if(iter != card_options.end())		doubleItem_value += mySD->getCardDoubleItemOption(selected_card_number);

	longTime_value = 0;
	if(isUsingItem(kIC_longTime))		longTime_value += mySD->getLongTimeItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_longTime);
	if(iter != card_options.end())		longTime_value += mySD->getCardLongTimeItemOption(selected_card_number);

	bossLittleEnergy_value = 0;
	if(isUsingItem(kIC_bossLittleEnergy))	bossLittleEnergy_value += mySD->getBossLittleEnergyItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_bossLittleEnergy);
	if(iter != card_options.end())			bossLittleEnergy_value += mySD->getCardBossLittleEnergyItemOption(selected_card_number);

	subSmallSize_value = 0;
	if(isUsingItem(kIC_subSmallSize))	subSmallSize_value += mySD->getSubSmallSizeItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_subSmallSize);
	if(iter != card_options.end())		subSmallSize_value += mySD->getCardSubSmallSizeItemOption(selected_card_number);

	smallArea_value = 0;
	if(isUsingItem(kIC_smallArea))		smallArea_value += mySD->getSmallAreaItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_smallArea);
	if(iter != card_options.end())		smallArea_value += mySD->getCardSmallAreaItemOption(selected_card_number);

	widePerfect_value = 0;
	if(isUsingItem(kIC_widePerfect))	widePerfect_value += mySD->getWidePerfectItemOption();
	iter = find(card_options.begin(), card_options.end(), kIC_widePerfect);
	if(iter != card_options.end())		widePerfect_value += mySD->getCardWidePerfectItemOption(selected_card_number);
}

void StarGoldData::gameClear( int t_grade, float t_score, float t_percentage, int t_game_time, int t_use_time, int t_total_time )
{
	was_used_friend_card = is_using_item[kIC_rentCard];
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
	{
		before_use_item[i] = is_using_item[i];
		is_using_item[i] = false;
	}

	is_cleared = true;
	stage_grade = t_grade;
	percentage = t_percentage;

	score = t_score + t_score*(stage_grade-1.f)*0.5f + t_score*(1.f-(t_use_time*1.f)/t_total_time);

	game_time = t_game_time;
	
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
	was_used_friend_card = is_using_item[kIC_rentCard];
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
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

void StarGoldData::setStartRequestsData( JsonBox::Object result_data )
{
	startRequestsData = result_data;
}

JsonBox::Object StarGoldData::getStartRequestsData()
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
		if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3)) > 0)
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
		if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(ing_card_stage, kSDS_SI_level_int1_card_i, 3)) > 0)
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
	int take_number = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number);
	CardSortInfo t_info;
	t_info.card_number = card_number;
	t_info.take_number = take_number;
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

int StarGoldData::getBossLittleEnergyValue()
{
	return bossLittleEnergy_value;
}

int StarGoldData::getSubSmallSizeValue()
{
	return subSmallSize_value;
}

int StarGoldData::getSmallAreaValue()
{
	return smallArea_value;
}

int StarGoldData::getWidePerfectValue()
{
	return widePerfect_value;
}

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
	has_gotten_cards.clear();
	int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
	for(int i=1;i<=card_take_cnt;i++)
	{
		int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
		int take_number = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, card_number);
		if(take_number != 0)
		{
			CardSortInfo t_info;
			t_info.card_number = card_number;
			t_info.take_number = take_number;
			t_info.grade = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
			t_info.rank = NSDS_GI(kSDS_CI_int1_rank_i, card_number);
			has_gotten_cards.push_back(t_info);
		}
	}
	changeSortType(CardSortType(myDSH->getIntegerForKey(kDSH_Key_cardSortType)));
}

void StarGoldData::selectFriendCard()
{
	vector<FriendData> known_list = KnownFriends::getInstance()->getFriends();
	vector<FriendData> unknown_list = UnknownFriends::getInstance()->getFriends();
	
	
	vector<FriendCardData> friends_card_data_list;
	
	for(int i=0;i<known_list.size();i++)
	{
		int t_card_number = known_list[i].userData[myDSH->getKey(kDSH_Key_selectedCard)].asInt();
		if(t_card_number != 0)
		{
			FriendCardData t_data;
			
			t_data.userId = known_list[i].userId;
			t_data.nick = known_list[i].nick;
			t_data.profileUrl = known_list[i].profileUrl;
			t_data.messageBlocked = known_list[i].messageBlocked;
			
			t_data.card_number = t_card_number;
			t_data.card_level = known_list[i].userData.get(myDSH->getKey(kDSH_Key_selectedCardLevel), 1).asInt();
			t_data.card_passive = known_list[i].userData.get(myDSH->getKey(kDSH_Key_selectedCardPassive), "").asString();
			
			friends_card_data_list.push_back(t_data);
		}
	}
	
	for(int i=0;i<unknown_list.size();i++)
	{
		int t_card_number = unknown_list[i].userData[myDSH->getKey(kDSH_Key_selectedCard)].asInt();
		if(t_card_number != 0)
		{
			FriendCardData t_data;
			
			t_data.userId = unknown_list[i].userId;
			t_data.nick = unknown_list[i].nick;
			t_data.profileUrl = unknown_list[i].profileUrl;
			t_data.messageBlocked = unknown_list[i].messageBlocked;
			
			t_data.card_number = t_card_number;
			t_data.card_level = unknown_list[i].userData.get(myDSH->getKey(kDSH_Key_selectedCardLevel), 1).asInt();
			t_data.card_passive = unknown_list[i].userData.get(myDSH->getKey(kDSH_Key_selectedCardPassive), "").asString();
			
			friends_card_data_list.push_back(t_data);
		}
	}
	
	if(!friends_card_data_list.empty())
	{
		random_device rd;
		default_random_engine e1(rd());
		uniform_int_distribution<int> uniform_dist(0, friends_card_data_list.size()-1);
		
		int selected_idx = uniform_dist(e1);
		selected_friend_card_data.card_number = friends_card_data_list[selected_idx].card_number;
		selected_friend_card_data.card_level = friends_card_data_list[selected_idx].card_level;
		selected_friend_card_data.card_passive = friends_card_data_list[selected_idx].card_passive;
		selected_friend_card_data.userId = friends_card_data_list[selected_idx].userId;
		selected_friend_card_data.nick = friends_card_data_list[selected_idx].nick;
		selected_friend_card_data.profileUrl = friends_card_data_list[selected_idx].profileUrl;
		selected_friend_card_data.messageBlocked = friends_card_data_list[selected_idx].messageBlocked;
	}
	else
	{
		selected_friend_card_data.card_number = 0;
	}
}

void StarGoldData::myInit()
{
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
	
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
	{
		before_use_item.push_back(false);
		is_using_item.push_back(false);
	}
	
	bonus_item_cnt.push_back(0); // empty
	for(int i=kIC_attack;i<=kIC_rentCard;i++)
		bonus_item_cnt.push_back(0);


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
