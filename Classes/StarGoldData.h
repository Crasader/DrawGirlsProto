//
//  StarGoldData.h
//  DrawingJack
//
//  Created by 사원3 on 13. 1. 30..
//
//

#ifndef DrawingJack_StarGoldData_h
#define DrawingJack_StarGoldData_h

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "AudioEngine.h"
#include "GraphDog.h"
#include "GameData.h"
#include "JsonBox.h"
#include "EnumDefine.h"
#include "SilhouetteData.h"
#include <deque>
#include <algorithm>
#include <chrono>

using namespace cocos2d;
using namespace std;

typedef enum tImgType{
	kImgType_Empty = 0,
	kImgType_specialMap, // 각 챕터마다 5, 10스테이지는 특별 스테이지로 특별한 이미지가 나옵니다.
	kImgType_control1, // 얇게 그리고 싶다면 이동중에 반대방향으로 제스쳐 해보세요.
	kImgType_control2, // 모서리부터 그리려면 모서리에 가까워졌을때 해당 방향으로 한번더 제스쳐 하세요.
	kImgType_facebook1, // 페이스북 로그인을 하시면 친구들의 점수를 볼수 있습니다.
	kImgType_facebook2, // 친구에게 (초대or함께)하기 를 보내면 붓을 얻을 수 있습니다. 한 사람에게 하루에 한번씩 보낼 수 있습니다.
	kImgType_facebook3, // 친구들이 내게 보낸 붓들은 여기서 확인할 수 있습니다.
	kImgType_gallery,
	kImgType_gallery1,
	kImgType_option,
	kImgType_pet,
	kImgType_petbuff,
	kImgType_elementalPresentation1, // 속성을 선택하면 해당 속성에 맞는 공격 스킬이 게임중에 나옵니다.
	kImgType_elemental, // 속성상관관계
	kImgType_facebookLoginReward
}ImgType;

typedef enum tSceneCode{
	kSceneCode_WorldMapScene = 1,
	kSceneCode_PuzzleMapScene,
	kSceneCode_StageSetting
}SceneCode;

enum FailCode{
	kFC_gameover = 0,
	kFC_timeover,
	kFC_missionfail
};

enum CardSortType{
	kCST_default = 0,
	kCST_take,
	kCST_gradeUp, // rank
	kCST_gradeDown // rank
};

class CardSortInfo{
public:
	int card_number;
	int take_number;
	int grade;
	int rank;
};

#define SGD_KEY	0xD9
#define mySGD StarGoldData::sharedInstance()

class StarGoldData : public CCObject
{
public:
	
	static StarGoldData* sharedInstance()
	{
		static StarGoldData* t_sg = NULL;
		if(t_sg == NULL)
		{
			t_sg = new StarGoldData();
			t_sg->myInit();
		}
		return t_sg;
	}
	
	void resetLabels()
	{
		star_label = NULL;
		gold_label = NULL;
	}
	
	void setStarLabel(CCLabelBMFont* t_label)
	{
		star_label = t_label;
	}
	
	int getStar()
	{
		return myDSH->getIntegerForKey(kDSH_Key_savedStar);
	}
	
	void setStar(int t_star)
	{
		myDSH->setIntegerForKey(kDSH_Key_savedStar, t_star);
		
		if(star_label)
			star_label->setString(CCString::createWithFormat("%d", t_star)->getCString());
	}
	
	void setGoldLabel(CCLabelBMFont* t_label)
	{
		gold_label = t_label;
	}
	
	int getGold()
	{
		return myDSH->getIntegerForKey(kDSH_Key_savedGold);
	}
	
	void setGold(int t_gold)
	{
		myDSH->setIntegerForKey(kDSH_Key_savedGold, t_gold);
		
		if(gold_label)
			gold_label->setString(CCString::createWithFormat("%d", t_gold)->getCString());
	}
	
	int getKeepGold()
	{
		return keep_gold;
	}
	
	void setKeepGold(int t_gold)
	{
		keep_gold = t_gold;
	}
	
	void setGameStart()
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
	
	void gameClear(int t_grade, float t_score, float t_percentage, int t_game_time, int t_use_time, int t_total_time)
	{
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
		{
			before_use_item[i] = is_using_item[i];
			is_using_item[i] = false;
		}
		
		is_cleared = true;
		stage_grade = t_grade;
		percentage = t_percentage;
		
		score = t_score + t_score*(stage_grade-1.f)*0.5f + t_score*(1.f-(t_use_time*1.f)/t_total_time);
		
		if(score < 0.f)
			CCLog("t_score : %.1f, stage_grade : %d, use_time : %d, total_time : %d", t_score, stage_grade, t_use_time, t_total_time);
		
		
		game_time = t_game_time;
		myGD->setIsGameover(true);
	}
	
	void gameOver(float t_score, float t_percentage, int t_game_time)
	{
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
		{
			before_use_item[i] = is_using_item[i];
			is_using_item[i] = false;
		}
		
		score = t_score;
		percentage = t_percentage;
		game_time = t_game_time;
		myGD->setIsGameover(true);
	}
	
	bool getIsCleared()
	{
		return is_cleared;
	}
	
	float getScore()
	{
		return score;
	}
	
	float getPercentage()
	{
		return percentage;
	}
	
	int getStageGrade()
	{
		return stage_grade;
	}
	
	int getStageGold()
	{
		return myDSH->getIntegerForKey(kDSH_Key_savedGold)-keep_gold;
	}
	
	bool getIsAfterSceneChapter()
	{
		return is_after_scene_chapter;
	}
	
	void setIsAfterSceneChapter(bool t_b)
	{
		is_after_scene_chapter = t_b;
	}
	
	bool getTutorialCleared()
	{
		return is_tutorial_cleared;
	}
	
	ImgType getRandomImgType()
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
	
	void setLoadingImgType(ImgType t_t)
	{
		after_loading = t_t;
	}
	
	CCSprite* getLoadingImg();
	
	void setOpenShopTD(CCObject* t_target, SEL_CallFuncI t_delegate)
	{
		shop_opener = t_target;
		open_shop_delegate = t_delegate;
	}
	
	void openShop(int t_code)
	{
		(shop_opener->*open_shop_delegate)(t_code);
	}
	
	void setAfterScene(SceneCode t_s)
	{
		after_scene = t_s;
	}
	
	SceneCode getAfterScene(){	return after_scene;	}
	
	void setTargetDelegate(CCObject* t_t, SEL_CallFuncB t_d)
	{
		graphDog_target = t_t;
		graphDog_delegate = t_d;
	}
	
	int getGameTime()
	{
		return game_time;
	}
	
	void setStartRequestsData(JsonBox::Object result_data)
	{
		startRequestsData = result_data;
	}
	
	JsonBox::Object getStartRequestsData()
	{
		return startRequestsData;
	}
	
	bool is_paused;
	
	string getFont();
	
	void setCollectionStarter(CollectionStarterType t_type)
	{
		collection_starter = t_type;
	}
	
	CollectionStarterType getCollectionStarter()
	{
		CollectionStarterType r_value = collection_starter;
		collection_starter = kCST_basic;
		return r_value;
	}
	
	bool is_before_title;
	
	bool is_showtime;
	bool is_exchanged;
	
	FailCode fail_code;
	
	bool isBeforeUseItem(ITEM_CODE t_i)
	{
		return before_use_item[t_i];
	}
	bool isUsingItem(ITEM_CODE t_i)
	{
		return is_using_item[t_i];
	}
	
	void setIsUsingItem(ITEM_CODE t_i, bool t_b)
	{
		is_using_item[t_i] = t_b;
	}
	void resetUsingItem()
	{
		is_using_item.clear();
	}
	
	int selected_collectionbook;
	SceneCode before_cardsetting;
	
	int getNextCardNumber(int recent_card_number)
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
	
	int getNextStageCardNumber(int recent_card_number)
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
	
	int getPreCardNumber(int recent_card_number)
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
	
	int getPreStageCardNumber(int recent_card_number)
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
	
	void changeSortType(CardSortType t_type)
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
	
	void addHasGottenCardNumber(int card_number)
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
	
	int getHasGottenCardsDataCardNumber(int index)
	{
		return has_gotten_cards[index].card_number;
	}
	
	int getHasGottenCardsSize()
	{
		return int(has_gotten_cards.size());
	}
	
	int getDoubleItemValue(){	return doubleItem_value;	}
	int getLongTimeValue(){		return longTime_value;		}
	int getBossLittleEnergyValue(){		return bossLittleEnergy_value;	}
	int getSubSmallSizeValue(){	return subSmallSize_value;	}
	int getSmallAreaValue(){	return smallArea_value;		}
	int getWidePerfectValue(){	return widePerfect_value;	}
	int getStartMapGachaCnt(){	return start_map_gacha_cnt;	}
	
	void startMapGachaOn()
	{
		start_map_gacha_cnt++;
		keep_gold = myDSH->getIntegerForKey(kDSH_Key_savedGold);
	}
	
	int isHasGottenCards(int t_stage, int t_grade)
	{
		for(auto i = has_gotten_cards.begin();i!=has_gotten_cards.end();i++)
		{
			if(NSDS_GI(kSDS_CI_int1_stage_i, (*i).card_number) == t_stage && (*i).grade == t_grade)
				return (*i).card_number;
		}
		
		return 0;
	}
	
	void resetHasGottenCards()
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
	
private:
	CCLabelBMFont* star_label;
	CCLabelBMFont* gold_label;
	
	bool is_after_scene_chapter;
	bool is_cleared;
	float score;
	float percentage;
	int stage_grade;
	int keep_gold;
	int game_time;
	
	int start_map_gacha_cnt;
		   
	int doubleItem_value;
	int longTime_value;
	int bossLittleEnergy_value;
	int subSmallSize_value;
	int smallArea_value;
	int widePerfect_value;
	
	deque<bool> before_use_item;
	deque<bool> is_using_item;
	deque<CardSortInfo> has_gotten_cards;
	
	bool is_tutorial_cleared;
	ImgType after_loading;
	SceneCode after_scene;
	
	CollectionStarterType collection_starter;
	
	CCObject* graphDog_target;
	SEL_CallFuncB graphDog_delegate;
	bool graphDog_logined;
	bool login_getted;
	
	CCObject* shop_opener;
	SEL_CallFuncI open_shop_delegate;
	
	JsonBox::Object startRequestsData;
	
	void myInit()
	{
		is_paused = false;
		login_getted = false;
		is_before_title = true;
		
		setTargetDelegate(NULL, NULL);
		
		after_loading = kImgType_Empty;
		is_after_scene_chapter = false;
		resetLabels();
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
		{
			before_use_item.push_back(false);
			is_using_item.push_back(false);
		}
		
		
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
};

#endif
