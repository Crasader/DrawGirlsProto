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
#include "AudioEngine.h"
#include "GraphDog.h"
#include "GameData.h"
#include "jsoncpp/json.h"
#include "EnumDefine.h"
#include "SilhouetteData.h"
#include <deque>
#include <algorithm>
#include <chrono>
#include <random>
#include <map>
#include "SelectorDefine.h"
#include "KSProtect.h"

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

enum CardStrengthBefore{
	kCardStrengthBefore_cardSetting = 0,
	kCardStrengthBefore_diary,
	kCardStrengthBefore_cardChange
};

enum FailCode{
	kFC_gameover = 0,
	kFC_timeover,
	kFC_missionfail
};

enum CardSortType{
	kCST_default = 0,
	kCST_take,
	kCST_takeReverse,
	kCST_gradeUp, // rank
	kCST_gradeDown // rank
};

class CardSortInfo{
public:
	int card_number;
	long long int take_number;
	int grade;
	int rank;
	string user_ment;
};

class PuzzleHistory{
public:
	int puzzle_number;
	bool is_open;
	bool is_clear;
	bool is_perfect;
	string open_type;
};

class PieceHistory{
public:
	int stage_number;
	bool is_open;
	bool is_clear[4];
	int try_count;
	int clear_count;
	string open_type;
};

class TimeInfo{
public:
	bool is_loaded;
	KSProtectVar<unsigned int> timestamp;
	KSProtectVar<unsigned int> weekNo;
	KSProtectVar<int> weekday;
	KSProtectVar<long long int> date;
	KSProtectVar<int> hour;
};

enum ReplayKey
{
	kReplayKey_timeStamp = 0,
	kReplayKey_timeStamp_characterPositionX,
	kReplayKey_timeStamp_characterPositionY,
	kReplayKey_timeStamp_boss,
	kReplayKey_timeStamp_boss_x,
	kReplayKey_timeStamp_boss_y,
	kReplayKey_timeStamp_monster,
	kReplayKey_timeStamp_monster_x,
	kReplayKey_timeStamp_monster_y,
	kReplayKey_timeStamp_mapIndex,
	kReplayKey_timeStamp_scoreIndex,
	kReplayKey_timeStamp_isDie,
	kReplayKey_timeStamp_isContinue,
	kReplayKey_timeStamp_isImageChange,
	kReplayKey_timeStamp_gameInfo,
	kReplayKey_mapTime,
	kReplayKey_mapData,
	kReplayKey_scoreTime,
	kReplayKey_scoreData,
	kReplayKey_isChangedMap,
	kReplayKey_isChangedScore,
	kReplayKey_playIndex
};

enum GoodsType
{
	kGoodsType_begin = 0,
	kGoodsType_money,
	kGoodsType_ruby,
	kGoodsType_gold,
	kGoodsType_item9,
	kGoodsType_item6,
	kGoodsType_item8,
	kGoodsType_pass1,
	kGoodsType_pass2,
	kGoodsType_pass3,
	kGoodsType_pass4,
	kGoodsType_pass5,
	kGoodsType_end
};

class ChangeGoodsData
{
public:
	GoodsType m_type;
	KSProtectVar<int> m_value;
	string m_statsID;
	string m_statsValue;
	string m_content;
	bool m_isPurchase;
};

enum UserdataType
{
	kUserdataType_begin = 0,
	kUserdataType_isVIP,
	kUserdataType_isFirstBuy,
	kUserdataType_totalPlayCount,
	kUserdataType_failCount,
	kUserdataType_autoLevel,
	kUserdataType_selectedCharNO,
	kUserdataType_highScore,
	kUserdataType_end
};

class ChangeUserdataValue
{
public:
	UserdataType m_type;
	KSProtectVar<int> m_value;
};

enum PurchaseGuideType
{
	kPurchaseGuideType_begin = 0,
	kPurchaseGuideType_firstPurchase,
	kPurchaseGuideType_emptyItem,
	kPurchaseGuideType_stupidNpuHelp,
	kPurchaseGuideType_eventRubyShop,
	kPurchaseGuideType_levelupGuide,
	kPurchaseGuideType_end
};

class CharacterHistory
{
public:
	KSProtectVar<int> characterNo;
	KSProtectVar<int> level;
	KSProtectVar<int> nextPrice;
	KSProtectVar<int> power;
	KSProtectVar<int> nextPower;
	KSProtectVar<int> prevPower;
	KSProtectVar<bool> isMaxLevel;
};

class TodayMission
{
public:
	KSProtectVar<int> today_date;
	KSProtectVar<int> mission_type;
	KSProtectVar<int> ing_count;
	KSProtectVar<int> goal_count;
	KSProtectStr reward_type;
	KSProtectVar<int> reward_count;
	KSProtectVar<bool> is_success;
};

enum TodayMissionType
{
	kTodayMissionType_begin = 0,
	kTodayMissionType_totalPercent,
	kTodayMissionType_totalScore,
	kTodayMissionType_totalTakeGold,
	kTodayMissionType_totalCatch,
	kTodayMissionType_end
};

#define SGD_KEY	0xD9
#define mySGD StarGoldData::sharedInstance()

class StarGoldData : public CCObject
{
public:
	
	static StarGoldData* sharedInstance();
	
	void resetLabels();
	
	void setStarLabel(CCLabelBMFont* t_label);
	void setIngameGoldLabel( CCLabelBMFont* t_label );
//	int getStar();
//	void setStar(int t_star);
	
	void setGoldLabel(CCLabelBMFont* t_label);
//	int getGold();
//	void setGold(int t_gold, bool is_write = true);
	
	int getKeepGold();
	
	void setKeepGold(int t_gold);
	
	void setGameStart();
	
	void gameClear(int t_grade, float t_score, float t_percentage, int t_game_time, int t_use_time, int t_total_time);
	
	void gameOver(float t_score, float t_percentage, int t_game_time);
	
	bool getIsCleared();
	
	float getScore();
	float getBaseScore();
	void setPercentage(float t_f);
	float getPercentage();
	
	void setStageGrade(int t_i);
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
	
	void setStartRequestsData(Json::Value result_data);
	
	Json::Value getStartRequestsData();
	
	bool is_paused;
	bool is_on_maingame;
	
	string getFont();
	string getFont2();
	
	void setCollectionStarter(CollectionStarterType t_type);
	
	CollectionStarterType getCollectionStarter();
	
	bool is_before_title;
	
	bool is_showtime;
	bool is_exchanged;
	
	FailCode fail_code;
	
	bool isBeforeUseItem(ITEM_CODE t_i);
	void setBeforeUseItem(ITEM_CODE t_i, bool t_b);
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
	CardSortInfo getHasGottenCardData(int index);
	
	int getHasGottenCardsSize();
	
	int getDoubleItemValue();
	int getLongTimeValue();
	int getBaseSpeedUpValue();
//	int getBossLittleEnergyValue();
//	int getSubSmallSizeValue();
//	int getSmallAreaValue();
//	int getWidePerfectValue();
	int getStartMapGachaCnt();
	
	void startMapGachaOn();
	
	string getCardComment(int t_card_number);
	void setCardComment(int t_card_number, string comment);
	int isHasGottenCards(int t_card_number);
	int isHasGottenCards(int t_stage, int t_grade);
	
	void resetHasGottenCards();
	void initTakeCardInfo(Json::Value card_list, vector<int>& card_data_load_list);
	
	CommandParam getUpdatePuzzleHistoryParam(PuzzleHistory t_history, jsonSelType call_back);
	int getOpenPuzzleCount();
	int getPuzzleHistorySize();
	PuzzleHistory getPuzzleHistoryForIndex(int t_index);
	PuzzleHistory getPuzzleHistory(int puzzle_number);
	void setPuzzleHistory(PuzzleHistory t_history, jsonSelType call_back);
	void setPuzzleHistoryForNotSave(PuzzleHistory t_history);
	void initPuzzleHistory(Json::Value history_list);
	
	CommandParam getUpdatePieceHistoryParam(PieceHistory t_history, jsonSelType call_back);
	Json::Value getSavePieceHistoryParam(PieceHistory t_history);
	bool isClearPiece(int stage_number);
	int getPieceHistorySize();
	PieceHistory getPieceHistoryForIndex(int t_index);
	PieceHistory getPieceHistory(int stage_number);
	void setPieceHistoryForNotSave(PieceHistory t_history);
	void setPieceHistory(PieceHistory t_history, jsonSelType call_back);
	void initPieceHistory(Json::Value history_list);
	
	void initSelectedCharacterNo(int t_i);
	CharacterHistory getSelectedCharacterHistory();
	int getCharacterHistorySize();
	CharacterHistory getCharacterHistory(int t_index);
	jsonSelType keep_character_history_callback;
	CommandParam getUpdateCharacterHistoryParam(CharacterHistory t_history, jsonSelType call_back);
	void setCharacterHistory(CharacterHistory t_history, jsonSelType call_back);
	void initCharacterHistory(Json::Value history_list);
	void resultUpdateCharacterHistory(Json::Value result_data);
	
	int getClearStarCount();
	
	void setClearRewardGold(int t_reward)
	{
		clear_reward_gold = t_reward;
	}
	int getClearRewardGold()
	{
		return clear_reward_gold;
	}
	
	void setBonusItemCnt(ITEM_CODE t_code, int t_cnt)
	{
		bonus_item_cnt[t_code] = t_cnt;
	}
	int getBonusItemCnt(ITEM_CODE t_code)
	{
		return bonus_item_cnt[t_code].getV();
	}
	
	void resetNoticeList(Json::Value t_notice_list);
	
	bool getMustBeShowNotice()
	{
		return must_be_show_notice;
	}
	
	Json::Value getNoticeList()
	{
		must_be_show_notice = false;
		Json::Reader reader;
		Json::Value return_value;
		string decode_data = notice_list.getV();
		reader.parse(decode_data, return_value);
		return return_value;
	}
	
	void setHeartMax(int t_data);
	void setHeartCoolTime(int t_data);
	void setGameFriendMax(int t_data);
	void setHelpCoolTime(int t_data);
	void setChallengeCoolTime(int t_data);
	void setMsgRemoveDay(int t_data);
	void setGachaGoldFee(int t_data);
	void setGachaRubyFee(int t_data);
	void setGachaSocialFee(int t_data);
	void setGachaGoldFeeRetry(int t_data);
	void setGachaRubyFeeRetry(int t_data);
	void setGachaSocialFeeRetry(int t_data);
	void setCardUpgradeGoldFee(int t_data);
	void setCardUpgradeRubyFee(int t_data);
	void setHeartSendCoolTime(int t_data);
	void setInviteMaxADay(int t_data);
	void setInviteCoolDay(int t_data);
	void setPlayContinueFee(int t_data);
	void setCardDurabilityUpFee(int t_data);
	void setGachaMapFee(int t_data);
	void setRemoveFriendCoolTime(int t_data);
	void setSPSendHeart(int t_data);
	void setSPSendTicket(int t_data);
	void setSPFinishedChallenge(int t_data);
	void setSPHelpChallenge(int t_data);
	void setSPSendBoast(int t_data);
	void setSPGetTime(int t_data);
	void setSPGetHeart(int t_data);
	void setGachaOnePercentFee(int t_data);
	void setAiAdderOnDrewOrDamaged(float t_data);
	void setFuryPercent(float t_data);
	void setSPRentCardThanks(int t_data);
	
	int getHeartMax();
	int getHeartCoolTime();
	int getGameFriendMax();
	int getHelpCoolTime();
	int getChallengeCoolTime();
	int getMsgRemoveDay();
	int getGachaGoldFee();
	int getGachaRubyFee();
	int getGachaSocialFee();
	int getGachaGoldFeeRetry();
	int getGachaRubyFeeRetry();
	int getGachaSocialFeeRetry();
	int getCardUpgradeGoldFee();
	int getCardUpgradeRubyFee();
	int getHeartSendCoolTime();
	int getInviteMaxADay();
	int getInviteCoolDay();
	int getPlayContinueFee();
	int getCardDurabilityUpFee();
	int getGachaMapFee();
	int getRemoveFriendCoolTime();
	int getSPSendHeart();
	int getSPSendTicket();
	int getSPFinishedChallenge();
	int getSPHelpChallenge();
	int getSPSendBoast();
	int getSPGetTime();
	int getSPGetHeart();
	int getGachaOnePercentFee();
	float getAiAdderOnDrewOrDamaged();
	float getFuryPercent();
	int getSPRentCardThanks();
	
	void setItemGachaGoldFee(int t_i);
	int getItemGachaGoldFee();
	void setItemGachaReplayGoldFee(int t_i);
	int getItemGachaReplayGoldFee();
	void setUpgradeGoldFee(int t_i);
	int getUpgradeGoldFee();
	void setIngameTutorialRewardGold(int t_i);
	int getIngameTutorialRewardGold();
	
	void setRankUpConditionCount(int t_i);
	int getRankUpConditionCount();
	void setRankUpBaseRate(float t_f);
	float getRankUpBaseRate();
	void setRankUpRateDistance(float t_f);
	float getRankUpRateDistance();
	void setRankUpRubyFee(int t_i);
	int getRankUpRubyFee();
	void setRankUpAddRate(float t_f);
	float getRankUpAddRate();
	
	void setFirstPurchasePlayCount(int t_i);
	int getFirstPurchasePlayCount();
	void setFirstPurchaseReviewSecond(long long t_i);
	long long getFirstPurchaseReviewSecond();
	
//	void setUserdataPGuide(string t_s);
//	string getUserdataPGuide();
	void setUserdataIsVIP(int t_i);
	int getUserdataIsVIP();
	void setUserdataIsFirstBuy(int t_i);
	int getUserdataIsFirstBuy();
	void setUserdataTotalPlayCount(int t_i);
	int getUserdataTotalPlayCount();
	void setUserdataFailCount(int t_i);
	int getUserdataFailCount();
	void setUserdataAutoLevel(int t_i);
	int getUserdataAutoLevel();
	void setUserdataHighScore(int t_i);
	int getUserdataHighScore();
	
	string getInappProduct(int t_index);
	void initInappProduct(int t_index, string t_product);
	string getEventInappProduct(int t_index);
	void initEventInappProduct(int t_index, string t_product);
	
	void setEmptyItemReviewSecond(long long t_i);
	long long getEmptyItemReviewSecond();
	void setStupidNpuHelpReviewSecond(long long t_i);
	long long getStupidNpuHelpReviewSecond();
	void setStupidNpuHelpPlayCount(int t_i);
	int getStupidNpuHelpPlayCount();
	void setStupidNpuHelpFailCount(int t_i);
	int getStupidNpuHelpFailCount();
	void setEventRubyShopReviewSecond(long long t_i);
	long long getEventRubyShopReviewSecond();
	void setPlayCountHighValue(int t_i);
	int getPlayCountHighValue();
	
	void setEmptyItemIsOn(int t_i);
	int getEmptyItemIsOn();
	void setStupidNpuHelpIsOn(int t_i);
	int getStupidNpuHelpIsOn();
	void setPlayCountHighIsOn(int t_i);
	int getPlayCountHighIsOn();
	
	void setLevelupGuideReviewSecond(long long t_i);
	long long getLevelupGuideReviewSecond();
	void setLevelupGuidePlayCount(int t_i);
	int getLevelupGuidePlayCount();
	void setLevelupGuideConditionLevel(int t_i);
	int getLevelupGuideConditionLevel();
	void setLevelupGuideIsOn(int t_i);
	int getLevelupGuideIsOn();
	
	void setItem9OpenStage(int t_i);
	int getItem9OpenStage();
	void setItem6OpenStage(int t_i);
	int getItem6OpenStage();
	void setItem8OpenStage(int t_i);
	int getItem8OpenStage();
	void setItemGachaOpenStage(int t_i);
	int getItemGachaOpenStage();
	
	
	bool is_before_selected_event_stage;
	
	StarGoldData() : heart_max(0), heart_cool_time(0), game_friend_max(0), help_cool_time(0), challenge_cool_time(0), msg_remove_day(0),
	gacha_gold_fee(0), gacha_ruby_fee(0), gacha_social_fee(0), card_upgrade_gold_fee(0), card_upgrade_ruby_fee(0), heart_send_cool_time(0),
	invite_max_a_day(0), invite_cool_day(0), play_continue_fee(0), card_durability_up_fee(0), gacha_map_fee(0), remove_friend_cool_time(0),
	SP_send_heart(0), SP_send_ticket(0), SP_finished_challenge(0), SP_help_challenge(0), SP_send_boast(0), SP_get_time(0), SP_get_heart(0),
	gacha_one_percent_fee(0), ai_adder_on_drew_or_damaged(0.f), fury_percent(0.f)
	{
		
	}
	
	bool isEmptyAchieveNotiQueue();
	void pushAchieveNotiQueue(AchievementCode t_code);
	void popAchieveNotiQueue();
	AchievementCode frontAchieveNotiQueue();
	void setNextSceneName(string scene_name);
	string getNextSceneName();
	
	deque<bool> is_ingame_before_have_stage_cards;
//	int ingame_before_stage_rank;
	bool getIsNotClearedStage();
	int getIsUnlockPuzzle();
	void setIsUnlockPuzzle(int t_i);
	
	void setStrengthTargetCardNumber(int t_card_number);
	int getStrengthTargetCardNumber();
	
	CardStrengthBefore getCardStrengthBefore();
	void setCardStrengthBefore(CardStrengthBefore t_before);
	
	bool is_draw_button_tutorial;
	int draw_button_tutorial_ing;
	
	bool is_write_replay;
	Json::Value replay_write_info;
	bool is_play_replay;
	Json::Value replay_playing_info;
	
	void setReplayPlayingInfo(Json::Value t_data)
	{
		is_play_replay = true;
		replay_playing_info = t_data;
		replay_playing_info[getReplayKey(kReplayKey_playIndex)] = 0;
	}
	void resetReplayPlayingInfo()
	{
		is_play_replay = false;
		replay_playing_info.clear();
	}
	
	string getReplayKey(ReplayKey t_key);
	
	int suitable_stage;
	
	string getAppType();
	int getAppVersion();
	
	int before_curtain_tip_type;
	bool is_clear_diary;
	bool is_safety_mode;
	bool is_before_stage_img_download;
	
	void withdraw();
	Json::Value cgp_data;
	
	void initProperties(Json::Value t_list);
	string getGoodsTypeToKey(GoodsType t_type);
	GoodsType getGoodsKeyToType(string t_key);
	GoodsType getItemCodeToGoodsType(ITEM_CODE t_code);
	void addChangeGoodsIngameGold(int t_value);
	void addChangeGoods(GoodsType t_type, int t_value, string t_statsID = "", string t_statsValue = "", string t_content = "", bool t_isPurchase = false);
	void updateChangeGoods(GoodsType t_type, int t_value, string t_statsID = "", string t_statsValue = "", string t_content = "", bool t_isPurchase = false);
	void clearChangeGoods();
	void changeGoods(jsonSelType t_callback);
	void changeGoodsTransaction(vector<CommandParam> command_list, jsonSelType t_callback);
	void refreshGoodsData(string t_key, int t_count);
	CommandParam getChangeGoodsParam(jsonSelType t_callback);
	void saveChangeGoodsTransaction(Json::Value result_data);
	bool isChangedGoods();
	
	int getGoodsValue(GoodsType t_type);
	
	TimeInfo keep_time_info;
	
	bool is_changed_userdata;
	UserdataType getUserdataKeyToType(string t_key);
	string getUserdataTypeToKey(UserdataType t_type);
	void changeUserdata(jsonSelType t_callback);
	CommandParam getChangeUserdataParam(jsonSelType t_callback);
	void initUserdata(Json::Value result_data);
	void clearChangeUserdata();
	
	bool isPossibleShowPurchasePopup(PurchaseGuideType t_type);
	void showPurchasePopup(PurchaseGuideType t_type);
	
	KSProtectVar<int> gacha_item;
	
	void initTodayMission(Json::Value t_info);
	TodayMission today_mission_info;
	bool is_today_mission_first;
	
	void increaseCatchCumber();
	int getCatchCumberCount();
	
	CommandParam getUpdateTodayMissionParam(jsonSelType t_callback);
	
private:
	bool is_not_cleared_stage;
	int is_unlock_puzzle;
	
	CardStrengthBefore card_strength_before;
	
	int strength_target_card_number;
	
	deque<AchievementCode> achieve_noti_que;
	
	CCLabelBMFont* star_label;
	CCLabelBMFont* gold_label;
	CCLabelBMFont* friend_point_label;
	CCLabelBMFont* ingame_gold_label;
	
	vector<KSProtectVar<int>> bonus_item_cnt;

	int clear_reward_gold;
	
	bool is_after_scene_chapter;
	
	KSProtectVar<bool> is_cleared;
	KSProtectVar<float> base_score;
	KSProtectVar<float> score;
	KSProtectVar<float> percentage;
	KSProtectVar<int> stage_grade;
	KSProtectVar<int> keep_gold;
	KSProtectVar<int> game_time;
	KSProtectVar<int> ingame_gold;
	
	int start_map_gacha_cnt;
		   
	int doubleItem_value;
	int longTime_value;
	int baseSpeedUp_value;
	
	deque<bool> before_use_item;
	deque<bool> is_using_item;
	deque<CardSortInfo> has_gotten_cards;
	deque<PuzzleHistory> puzzle_historys;
	deque<PieceHistory> piece_historys;
	
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
	
	Json::Value startRequestsData;
	
	bool must_be_show_notice;
	
	KSProtectStr notice_list;
	
	string next_scene_name;
	
//	Json::Value notice_list;
	
	KSProtectVar<int> heart_max; // 최대 보유할 수 있는 하트 수
	KSProtectVar<int> heart_cool_time; // 몇 초 뒤에 하트가 새로 생길 것 인가
	KSProtectVar<int> game_friend_max; // 게임친구 인원제한
	KSProtectVar<int> help_cool_time; // 도움요청 시간제한
	KSProtectVar<int> challenge_cool_time; // 도전요청 시간제한
	KSProtectVar<int> msg_remove_day; // 메세지 보관일 수
	KSProtectVar<int> gacha_gold_fee; // 골드 가챠 가격(골드)
	KSProtectVar<int> gacha_ruby_fee; // 루비 가챠 가격(루비)
	KSProtectVar<int> gacha_social_fee; // 소셜 가챠 가격(소셜포인트)
	KSProtectVar<int> gacha_gold_fee_retry; // 골드 가챠 가격(골드)
	KSProtectVar<int> gacha_ruby_fee_retry; // 루비 가챠 가격(루비)
	KSProtectVar<int> gacha_social_fee_retry; // 소셜 가챠 가격(소셜포인트)
	KSProtectVar<int> card_upgrade_gold_fee; // 카드 일반강화 가격(골드)
	KSProtectVar<int> card_upgrade_ruby_fee; // 카드 고급강화 가격(루비)
	KSProtectVar<int> heart_send_cool_time; // 하트 보내기 쿨타임
	KSProtectVar<int> invite_max_a_day; // 하루 초대 인원 제한
	KSProtectVar<int> invite_cool_day; // 한 친구당 친구초대 주기 일수
	KSProtectVar<int> play_continue_fee; // 이어하기 가격(루비)
	KSProtectVar<int> card_durability_up_fee; // 카드 내구도 회복 가격(루비)
	KSProtectVar<int> gacha_map_fee; // 맵 가챠 가격(골드)
	KSProtectVar<int> remove_friend_cool_time; // 친구 삭제 간격(초)
	KSProtectVar<int> SP_send_heart; // 소셜포인트 획득량(하트 보낼때)
	KSProtectVar<int> SP_send_ticket; // 소셜포인트 획득량(티켓 보낼때)
	KSProtectVar<int> SP_finished_challenge; // 소셜포인트 획득량(도전하기 끝나서 결과 나왔을때 요청자, 대상자)
	KSProtectVar<int> SP_help_challenge; // 소셜포인트 획득량(도움요청 끝나서 결과 나왔을때 요청자, 대상자)
	KSProtectVar<int> SP_send_boast; // 소셜포인트 획득량(자랑하기 보낼때)
	KSProtectVar<int> SP_get_time; // 받은 메세지 몇초 안에 확인해야 소셜포인트 줄건지 설정
	KSProtectVar<int> SP_get_heart; // 소셜포인트 획득량(하트온지 SPGetTime초 안에 수락하면 주는 포인트)
	KSProtectVar<int> gacha_one_percent_fee; // 1프로 가챠 가격(루비)
	KSProtectVar<float> ai_adder_on_drew_or_damaged; //
	KSProtectVar<float> fury_percent; //
	KSProtectVar<int> SP_rent_card_thanks; // 소셜포인트 획득량-카드빌리고 보답할때
	
	KSProtectVar<int> item_gacha_gold_fee; // 아이템 가챠 가격
	KSProtectVar<int> item_gacha_replay_gold_fee; // 아이템 가챠 다시뽑기 가격
	
	KSProtectVar<int> upgrade_gold_fee;
	KSProtectVar<int> ingame_tutorial_reward_gold;
	
	KSProtectVar<int> item9_open_stage;
	KSProtectVar<int> item6_open_stage;
	KSProtectVar<int> item8_open_stage;
	KSProtectVar<int> itemGacha_open_stage;
	
	KSProtectVar<int> rank_up_condition_count;
	KSProtectVar<float> rank_up_base_rate;
	KSProtectVar<float> rank_up_rate_distance;
	KSProtectVar<int> rank_up_ruby_fee;
	KSProtectVar<float> rank_up_add_rate;
	
	KSProtectVar<int> first_purchase_play_count;
	KSProtectVar<long long> first_purchase_review_second;
	KSProtectVar<long long> empty_item_review_second;
	KSProtectVar<long long> stupid_npu_help_review_second;
	KSProtectVar<int> stupid_npu_help_play_count;
	KSProtectVar<int> stupid_npu_help_fail_count;
	KSProtectVar<long long> event_ruby_shop_review_second;
	KSProtectVar<int> play_count_high_value;
	KSProtectVar<long long> levelup_guide_review_second;
	KSProtectVar<int> levelup_guide_play_count;
	KSProtectVar<int> levelup_guide_condition_level;
	
	KSProtectVar<int> empty_item_is_on;
	KSProtectVar<int> stupid_npu_help_is_on;
	KSProtectVar<int> play_count_high_is_on;
	KSProtectVar<int> levelup_guide_is_on;
	
	vector<ChangeUserdataValue> changed_userdata_list;
	jsonSelType change_userdata_callback;
	map<UserdataType, KSProtectVar<int>> userdata_storage;
	void resultChangeUserdata(Json::Value result_data);
	
	KSProtectStr inapp_products[6];
	KSProtectStr event_inapp_products[6];
	
	map<GoodsType, KSProtectVar<int>> goods_data;
	vector<ChangeGoodsData> change_goods_list;
	jsonSelType change_goods_callback;
	void resultChangeGoods(Json::Value result_data);
	void retryChangeGoods();
	
	KSProtectVar<long long> at_time_show_firstPurchase;
	KSProtectVar<long long> at_time_show_emptyItem;
	KSProtectVar<long long> at_time_show_stupidNpuHelp;
	KSProtectVar<long long> at_time_show_eventRubyShop;
	KSProtectVar<long long> at_time_show_levelupGuide;
	
	KSProtectVar<int> selected_character_index;
	deque<CharacterHistory> character_historys;
	
	KSProtectVar<int> catch_cumber_count;
	jsonSelType update_today_mission_callback;
	void resultUpdateTodayMission(Json::Value result_data);
	
	string app_type;
	int app_version;
	
	void myInit();
	
	
	CC_SYNTHESIZE(long long, remove_message_member_id, RemoveMessageMemberId);
	CC_SYNTHESIZE(int, remove_message_mail_no, RemoveMessageMailNo);
	
};

#endif
