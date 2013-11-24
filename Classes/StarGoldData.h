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
	
	static StarGoldData* sharedInstance();
	
	void resetLabels();
	
	void setStarLabel(CCLabelBMFont* t_label);
	
	int getStar();
	
	void setStar(int t_star);
	
	void setGoldLabel(CCLabelBMFont* t_label);
	
	int getGold();
	
	void setGold(int t_gold);
	
	int getKeepGold();
	
	void setKeepGold(int t_gold);
	
	void setGameStart();
	
	void gameClear(int t_grade, float t_score, float t_percentage, int t_game_time, int t_use_time, int t_total_time);
	
	void gameOver(float t_score, float t_percentage, int t_game_time);
	
	bool getIsCleared();
	
	float getScore();
	
	float getPercentage();
	
	int getStageGrade();
	
	int getStageGold();
	
	bool getIsAfterSceneChapter();
	
	void setIsAfterSceneChapter(bool t_b);
	
	bool getTutorialCleared();
	
	ImgType getRandomImgType();
	
	void setLoadingImgType(ImgType t_t);
	
	CCSprite* getLoadingImg();
	
	void setOpenShopTD(CCObject* t_target, SEL_CallFuncI t_delegate);
	
	void openShop(int t_code);
	
	void setAfterScene(SceneCode t_s);
	
	SceneCode getAfterScene();
	
	void setTargetDelegate(CCObject* t_t, SEL_CallFuncB t_d);
	
	int getGameTime();
	
	void setStartRequestsData(JsonBox::Object result_data);
	
	JsonBox::Object getStartRequestsData();
	
	bool is_paused;
	
	string getFont();
	
	void setCollectionStarter(CollectionStarterType t_type);
	
	CollectionStarterType getCollectionStarter();
	
	bool is_before_title;
	
	bool is_showtime;
	bool is_exchanged;
	
	FailCode fail_code;
	
	bool isBeforeUseItem(ITEM_CODE t_i);
	bool isUsingItem(ITEM_CODE t_i);
	
	void setIsUsingItem(ITEM_CODE t_i, bool t_b);
	void resetUsingItem();
	
	int selected_collectionbook;
	SceneCode before_cardsetting;
	
	int getNextCardNumber(int recent_card_number);
	
	int getNextStageCardNumber(int recent_card_number);
	
	int getPreCardNumber(int recent_card_number);
	
	int getPreStageCardNumber(int recent_card_number);
	
	void changeSortType(CardSortType t_type);
	
	void addHasGottenCardNumber(int card_number);
	
	int getHasGottenCardsDataCardNumber(int index);
	
	int getHasGottenCardsSize();
	
	int getDoubleItemValue();
	int getLongTimeValue();
	int getBossLittleEnergyValue();
	int getSubSmallSizeValue();
	int getSmallAreaValue();
	int getWidePerfectValue();
	int getStartMapGachaCnt();
	
	void startMapGachaOn();
	
	int isHasGottenCards(int t_stage, int t_grade);
	
	void resetHasGottenCards();
	
	bool getIsMeChallenge()
	{
		return is_me_challenge;
	}
	void setIsMeChallenge(bool t_b)
	{
		is_me_challenge = t_b;
		if(!is_me_challenge)
		{
			me_challenge_target_user_id = "";
			me_challenge_target_user_nick = "";
		}
	}
	void setMeChallengeTarget(string t_target, const std::string& t_nickname)
	{
		me_challenge_target_user_id = t_target.c_str();
		me_challenge_target_user_nick = t_nickname;
	}
	string getMeChallengeTarget()
	{
		return me_challenge_target_user_id;
	}
	string getMeChallengeTargetNick()
	{
		return me_challenge_target_user_nick;
	}
	
	bool getIsAcceptChallenge()
	{
		return is_accept_challenge;
	}
	void setIsAcceptChallenge(bool t_b)
	{
		is_accept_challenge = t_b;
		if(!is_accept_challenge)
		{
			accept_challenge_target_user_id = "";
			accept_challenge_target_user_nick = "";
			accept_challenge_target_score = 0.f;
		}
	}
	void setAcceptChallengeTarget(string t_id, string t_nick, float t_score)
	{
		accept_challenge_target_user_id = t_id.c_str();
		accept_challenge_target_user_nick = t_nick.c_str();
		accept_challenge_target_score = t_score;
	}
	string getAcceptChallengeId()
	{
		return accept_challenge_target_user_id;
	}
	string getAcceptChallengeNick()
	{
		return accept_challenge_target_user_nick;
	}
	float getAcceptChallengeScore()
	{
		return accept_challenge_target_score;
	}
	
	bool getIsAcceptHelp()
	{
		return is_accept_help;
	}
	void setIsAcceptHelp(bool t_b)
	{
		is_accept_help = t_b;
		if(!is_accept_help)
		{
			accept_help_target_user_id = "";
			accept_help_target_user_nick = "";
		}
	}
	void setAcceptHelpTarget(string t_id, string t_nick)
	{
		accept_help_target_user_id = t_id.c_str();
		accept_help_target_user_nick = t_nick.c_str();
	}
	string getAcceptHelpId()
	{
		return accept_help_target_user_id;
	}
	string getAcceptHelpNick()
	{
		return accept_help_target_user_nick;
	}
	
	void setClearRewardGold(int t_reward)
	{
		clear_reward_gold = t_reward;
	}
	int getClearRewardGold()
	{
		return clear_reward_gold;
	}
	
private:
	CCLabelBMFont* star_label;
	CCLabelBMFont* gold_label;
	
	bool is_me_challenge;
	string me_challenge_target_user_id;
	string me_challenge_target_user_nick;
	
	bool is_accept_challenge;
	string accept_challenge_target_user_id;
	string accept_challenge_target_user_nick;
	float accept_challenge_target_score;
	
	bool is_accept_help;
	string accept_help_target_user_id;
	string accept_help_target_user_nick;
	
	int clear_reward_gold;
	
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
	
	void myInit();
};

#endif
