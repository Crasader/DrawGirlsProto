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
	kSceneCode_ChapterSetting = 1,
	kSceneCode_Maingame,
	kSceneCode_StageSetting,
	kSceneCode_Gallery,
	kSceneCode_WorldMapScene
}SceneCode;

enum FailCode{
	kFC_gameover = 0,
	kFC_timeover,
	kFC_missionfail
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
	
	void setGameStart()
	{
		mySD->startSetting();
		is_showtime = false;
		is_exchanged = false;
		is_cleared = false;
		score = 0.f;
		percentage = 0.f;
		keep_gold = myDSH->getIntegerForKey(kDSH_Key_savedGold);
		stage_star = 0;
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
	
	void gameClear(int t_star, float t_score, float t_percentage, int t_game_time)
	{
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
		{
			before_use_item[i] = is_using_item[i];
			is_using_item[i] = false;
		}
		
		is_cleared = true;
		stage_star = t_star;
		percentage = t_percentage;
		score = t_score*(1.f+percentage)*(1.f+stage_star/10.f);
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
	
	int getStageStar()
	{
		return stage_star;
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
	
	int getBrushCnt(){	return myDSH->getIntegerForKey(kDSH_Key_brushCnt);	}
	void setBrushCnt(int t_t)
	{
		myDSH->setIntegerForKey(kDSH_Key_brushCnt, t_t);
	}
	int getBrushTime(){	return brush_time;	}
	void setBrushTime(int t_t)
	{
		brush_time = t_t;
		myDSH->setIntegerForKey(kDSH_Key_brushTime, brush_time);
	}
	int getSavedServerTime(){	return server_time;	}
	void refreshServerTime()
	{
		if(is_credit_servertime)
		{
			time_t recent_client_time = time(NULL);
			server_time += recent_client_time - client_time;
			client_time = recent_client_time;
			(graphDog_target->*graphDog_delegate)(is_credit_servertime);
		}
		else
		{
//			GraphDog::get()->command("timestamp", NULL, this, gd_selector(StarGoldData::graphDogRefreshServerTime));
		}
	}
	
	void setTargetDelegate(CCObject* t_t, SEL_CallFuncB t_d)
	{
		graphDog_target = t_t;
		graphDog_delegate = t_d;
	}
	
	void removeGraphDogTarget()
	{
//		GraphDog::get()->removeCommand(this);
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
	
	void notBelieveServerTime()
	{
		is_credit_servertime = false;
	}
	
	bool is_paused;
	
	string getFont();
	
	bool getIsCreditServerTime()
	{
		return is_credit_servertime;
	}
	
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
	
	void resetData()
	{
		myDSH->setIntegerForKey(kDSH_Key_elementLevelEmpty, 1);
		myDSH->setIntegerForKey(kDSH_Key_elementLevelLife, 0);
		myDSH->setIntegerForKey(kDSH_Key_elementLevelFire, 0);
		myDSH->setIntegerForKey(kDSH_Key_elementLevelWater, 0);
		myDSH->setIntegerForKey(kDSH_Key_elementLevelWind, 0);
		myDSH->setIntegerForKey(kDSH_Key_elementLevelLightning, 0);
		myDSH->setIntegerForKey(kDSH_Key_elementLevelPlasma, 0);
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedElement, kElementCode_empty);
		myDSH->setIntegerForKey(kDSH_Key_savedStar, 0);
		myDSH->setIntegerForKey(kDSH_Key_savedGold, 0);
		myDSH->setIntegerForKey(kDSH_Key_brushCnt, 5);
		myDSH->setIntegerForKey(kDSH_Key_brushTime, -1);
		myDSH->setBoolForKey(kDSH_Key_isBrushInf, false);
		myDSH->setBoolForKey(kDSH_Key_isRemoveAD, false);
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedPet, kPetCode_empty_ladybug);
		myDSH->setIntegerForKey(kDSH_Key_totalSelfPetCount, 1);
		myDSH->setIntegerForKey(kDSH_Key_selfPetCode_int1, kPetCode_empty_ladybug);
		myDSH->setIntegerForKey(kDSH_Key_openSlotCount, 1);
		for(int i=1;i<=20;i++)
		{
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i, 0, false);
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i, 1, false);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i, 0, 0);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i, 1, 0);
		}
		for(int i=20;i<=26;i++)
		{
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i, 0, false);
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i, 1, false);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i, 0, 0);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i, 1, 0);
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i+10, 0, false);
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i+10, 1, false);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i+10, 0, 0);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i+10, 1, 0);
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i+20, 0, false);
			myDSH->setBoolForKey(kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2, i+20, 1, false);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i+20, 0, 0);
			myDSH->setIntegerForKey(kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2, i+20, 1, 0);
		}
		for(int i=kPetCode_attack_genie;i<=kPetCode_gold_frog;i++)
			myDSH->setBoolForKey(kDSH_Key_hasGottenPet_int1, i, false);
	}
	
	void setBrushTimeInstance(CCObject* t_o)
	{
		brush_time_instance = t_o;
	}
	
	CCObject* getBrushTimeInstance()
	{
		return brush_time_instance;
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
	
	int selected_collectionbook;
	
	int getNextCardNumber(int recent_card_number)
	{
		int t_size = has_gotten_cards.size();
		
		if(t_size == 1)
			return -1;
		
		int found_number = -1;
		for(int i=0;i<t_size;i++)
		{
			if(recent_card_number == has_gotten_cards[i])
			{
				found_number = i;
				break;
			}
		}
		
		if(found_number == -1) // not found
			return -1;

		if(found_number >= t_size-1)
			return has_gotten_cards[0];
		else
			return has_gotten_cards[found_number+1];
	}
	
	int getPreCardNumber(int recent_card_number)
	{
		int t_size = has_gotten_cards.size();
		
		if(t_size == 1)
			return -1;
		
		int found_number = -1;
		for(int i=0;i<t_size;i++)
		{
			if(recent_card_number == has_gotten_cards[i])
			{
				found_number = i;
				break;
			}
		}
		
		if(found_number == -1) // not found
			return -1;
		
		if(found_number <= 0)
			return has_gotten_cards[t_size-1];
		else
			return has_gotten_cards[found_number-1];
	}
	
	void addHasGottenCardNumber(int card_number)
	{
		has_gotten_cards.push_back(card_number);
		sort(has_gotten_cards.begin(), has_gotten_cards.end());
		CCLog("input %d, sort", card_number);
		for(int i=0;i<has_gotten_cards.size();i++)
		{
			CCLog("%d", has_gotten_cards[i]);
		}
	}
	
	int getDoubleItemValue(){	return doubleItem_value;	}
	int getLongTimeValue(){		return longTime_value;		}
	int getBossLittleEnergyValue(){		return bossLittleEnergy_value;	}
	int getSubSmallSizeValue(){	return subSmallSize_value;	}
	int getSmallAreaValue(){	return smallArea_value;		}
	int getWidePerfectValue(){	return widePerfect_value;	}
	int getStartMapGachaCnt(){	return start_map_gacha_cnt;	}
	
	void startMapGachaOn(){	start_map_gacha_cnt++;	}
	
private:
	CCLabelBMFont* star_label;
	CCLabelBMFont* gold_label;
	
	bool is_after_scene_chapter;
	bool is_cleared;
	float score;
	float percentage;
	int stage_star;
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
	deque<int> has_gotten_cards;
	
	bool is_tutorial_cleared;
	ImgType after_loading;
	SceneCode after_scene;
//	int brush_cnt;
	int brush_time;
	int server_time;
	bool is_credit_servertime;
	time_t client_time;
	
	
	CollectionStarterType collection_starter;
	
	CCObject* graphDog_target;
	SEL_CallFuncB graphDog_delegate;
	bool graphDog_logined;
	bool login_getted;
	bool serverTime_getted;
	
	CCObject* shop_opener;
	SEL_CallFuncI open_shop_delegate;
	
	CCObject* brush_time_instance;
	
	JsonBox::Object startRequestsData;
	
	void graphDogRefreshServerTime(JsonBox::Object data)
	{
		if(data["state"].getString() == "ok")
		{
			server_time = data["timestamp"].getInt();
			myDSH->setIntegerForKey(kDSH_Key_serverTime, server_time);
			is_credit_servertime = true;
			client_time = time(NULL);
		}
		else
			is_credit_servertime = false;
		
		(graphDog_target->*graphDog_delegate)(is_credit_servertime);
	}
	
	void getServerTime()
	{
//		GraphDog::get()->command("timestamp", NULL, this, gd_selector(StarGoldData::graphDogGetServerTime));
	}
	
	void loadServerTime()
	{
		server_time = myDSH->getIntegerForKey(kDSH_Key_serverTime);
		serverTime_getted = true;
		
		if(login_getted && serverTime_getted)
		{
			(graphDog_target->*graphDog_delegate)(is_credit_servertime && graphDog_logined);
		}
	}
	
	void graphDogGetServerTime(JsonBox::Object data)
	{
		if(data["state"].getString() == "ok")
		{
			server_time = data["timestamp"].getInt();
			myDSH->setIntegerForKey(kDSH_Key_serverTime, server_time);
			myDSH->setBoolForKey(kDSH_Key_notFirstExe, true);
			is_credit_servertime = true;
			client_time = time(NULL);
		}
		else
			is_credit_servertime = false;
		serverTime_getted = true;
		
		if(login_getted && serverTime_getted)
		{
			(graphDog_target->*graphDog_delegate)(is_credit_servertime && graphDog_logined);
		}
	}
	
	void myInit()
	{
		is_paused = false;
		is_credit_servertime = false;
		server_time = -1;
		login_getted = false;
		serverTime_getted = false;
		is_before_title = true;
		
		for(int i=1;i<=mySD->getLastUpdateStageNumber();i++)
		{
			for(int j=0;j<3;j++)
			{
				if(myDSH->getBoolForKey(kDSH_Key_hasGottenCard_int1, i*10+j))
					has_gotten_cards.push_back(i*10+j);
			}
		}
		
		collection_starter = kCST_basic;//kCST_basic;
		
		setTargetDelegate(NULL, NULL);
		
		after_loading = kImgType_Empty;
		is_after_scene_chapter = false;
		resetLabels();
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
		{
			before_use_item.push_back(false);
			is_using_item.push_back(false);
		}
		
		if(myDSH->getIntegerForKey(kDSH_Key_chapter_int1_Stage_int2_Rating,10,5) > 0 && !myDSH->getBoolForKey(kDSH_Key_isOpendChapter_int1, 21))
			myDSH->setBoolForKey(kDSH_Key_isOpendChapter_int1, 21, true);
		
		if(myDSH->getIntegerForKey(kDSH_Key_totalSelfPetCount) <= 0)
		{
			myDSH->setIntegerForKey(kDSH_Key_openSlotCount, 1);
			myDSH->setIntegerForKey(kDSH_Key_totalSelfPetCount, 1);
			myDSH->setIntegerForKey(kDSH_Key_selfPetCode_int1, 1, kPetCode_empty_ladybug);
			myDSH->setIntegerForKey(kDSH_Key_lastSelectedPet, 1);
		}
		
		if(!myDSH->getBoolForKey(kDSH_Key_hasGottenPet_int1, kPetCode_empty_ladybug))
		{
			int total_self_pet_cnt = myDSH->getIntegerForKey(kDSH_Key_totalSelfPetCount);
			for(int i=1;i<=total_self_pet_cnt;i++)
			{
				int pet_code = myDSH->getIntegerForKey(kDSH_Key_selfPetCode_int1, i);
				myDSH->setBoolForKey(kDSH_Key_hasGottenPet_int1, pet_code, true);
			}
		}
		
//		GraphDog::get()->setup("drawingjack", "GDSK8052", "com.litqoo.lib.FBConnectorBase", 20);
//		graphdog->setGraphDogVersion(5);
		
		if(!myDSH->getBoolForKey(kDSH_Key_notFirstExe))
		{
			myDSH->setBoolForKey(kDSH_Key_notFirstExe, true);
			is_tutorial_cleared = false;
			myDSH->setBoolForKey(kDSH_Key_bgmOff, false);
			myDSH->setBoolForKey(kDSH_Key_effectOff, false);
			AudioEngine::sharedInstance()->setSoundOnOff(true);
			AudioEngine::sharedInstance()->setEffectOnOff(true);
			
			int cmp_value1 = 0xD8;
			int cmp_value2 = 0x331;
			int cmp_value3 = 0xDC;
			
			myDSH->setIntegerForKey(kDSH_Key_savedStar, (cmp_value1^SGD_KEY));
			myDSH->setIntegerForKey(kDSH_Key_savedGold, (cmp_value2^SGD_KEY));
			
			myDSH->setIntegerForKey(kDSH_Key_brushCnt, (cmp_value3^SGD_KEY));
			brush_time = -1;
			myDSH->setIntegerForKey(kDSH_Key_brushTime, brush_time);
			getServerTime();
			
			myDSH->setIntegerForKey(kDSH_Key_lastSelectedChapter, 1);
		}
		else
		{
			is_tutorial_cleared = true;
			
//			AudioEngine::sharedInstance()->initMusicOnOff(myDSH->getIntegerForKey(kDSH_Key_musicOn));
			AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
			AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
			
			brush_time = myDSH->getIntegerForKey(kDSH_Key_brushTime);
			
			loadServerTime();
		}
	}
};

#endif
