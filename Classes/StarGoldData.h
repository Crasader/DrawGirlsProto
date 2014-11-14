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

#define COMMON_VAR(varType, varName, funcName)\
protected: KSProtectVar<varType> varName;\
public: virtual varType get##funcName(void){return varName.getV(); }\
public: virtual void set##funcName(varType var){ varName = var; }

#define COMMON_VAR_STR(varName, funcName)\
protected: KSProtectStr varName;\
public: virtual std::string get##funcName(void){return varName.getV(); }\
public: virtual void set##funcName(const std::string& var){ varName = var; }


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

enum UISceneCode{
	kUISceneCode_empty = 0,
	kUISceneCode_mainFlow,
	kUISceneCode_puzzle
};

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
	kCST_gradeDown, // rank
	kCST_event
};

class CardSortInfo{
public:
	KSProtectVar<int> card_number;
	KSProtectVar<long long int> take_number;
	KSProtectVar<int> grade;
	KSProtectVar<int> rank;
	KSProtectStr user_ment;
	KSProtectVar<bool> is_morphing;
	KSProtectVar<int> count;
	
	CardSortInfo() : card_number(0){}
};

class PuzzleHistory{
public:
	KSProtectVar<int> puzzle_number;
	KSProtectVar<bool> is_open;
	KSProtectVar<bool> is_clear;
	KSProtectVar<bool> is_perfect;
	KSProtectStr open_type;
};

class PieceHistory{
public:
	KSProtectVar<int> stage_number;
	KSProtectVar<bool> is_open;
	KSProtectVar<bool> is_clear[4];
	KSProtectVar<int> try_count;
	KSProtectVar<int> clear_count;
	KSProtectStr open_type;
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
	kReplayKey_timeStamp_scoreAttackDamage,
	kReplayKey_mapTime,
	kReplayKey_mapData,
	kReplayKey_scoreTime,
	kReplayKey_scoreData,
	kReplayKey_isChangedMap,
	kReplayKey_isChangedScore,
	kReplayKey_playIndex,
	kReplayKey_clearGrade,
	kReplayKey_gameTime,
	kReplayKey_takeGold,
	kReplayKey_totalScore,
	kReplayKey_originalScore,
	kReplayKey_win,
	kReplayKey_lose,
	kReplayKey_useItemCnt,
	kReplayKey_useItem_int1_itemCode,
	kReplayKey_areaScore,
	kReplayKey_damageScore,
	kReplayKey_comboScore,
	kReplayKey_lifeBonusCnt,
	kReplayKey_takeArea,
	kReplayKey_scoreAttackedValue,
	kReplayKey_stageNo
};

enum GoodsType
{
	kGoodsType_begin = 0,
	kGoodsType_money,
	kGoodsType_ruby,
	kGoodsType_gold,
	kGoodsType_item9,
	kGoodsType_item6,
	kGoodsType_item11,
	kGoodsType_pass1,
	kGoodsType_pass2,
	kGoodsType_pass3,
	kGoodsType_pass4,
	kGoodsType_pass5,
	kGoodsType_pass6,
	kGoodsType_pass7,
	kGoodsType_pass8,
	kGoodsType_heart,
	kGoodsType_end,
	kGoodsType_pz,
	kGoodsType_pc,
	kGoodsType_cu
};

class ChangeGoodsDataDetail
{
public:
	GoodsType m_type;
	KSProtectVar<int> m_value;
	string m_statsID;
	string m_statsValue;
	string m_content;
	bool m_isPurchase;
};

class ChangeGoodsData
{
public:
	vector<ChangeGoodsDataDetail> detail_list;
	
	bool is_renewal;
	KSProtectStr m_exchangeID;
	
	ChangeGoodsData()
	{
		is_renewal = false;
		detail_list.clear();
	}
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
	kUserdataType_highPiece,
	kUserdataType_onlyOneBuyPack,
	kUserdataType_characterLevel,
	
	kUserdataType_endlessData_ingWin,
	kUserdataType_endlessData_ingWeek,
	kUserdataType_endlessData_score,
	
	kUserdataType_achieve_mapGacha,
	kUserdataType_achieve_noFail,
	kUserdataType_achieve_hunter,
	kUserdataType_achieve_changeMania,
	kUserdataType_achieve_fail,
	kUserdataType_achieve_perfect,
	kUserdataType_achieve_itemBuyCount,
	kUserdataType_achieve_seqAttendance,
	
	kUserdataType_missileInfo_nextPrice,
	kUserdataType_missileInfo_power,
	kUserdataType_missileInfo_nextPower,
	kUserdataType_missileInfo_prevPower,
	kUserdataType_missileInfo_isMaxLevel,
	
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
	KSProtectVar<int> characterLevel;
	KSProtectVar<int> characterExp;
	KSProtectVar<int> characterCurrentLevelExp; // 현재 레벨의 min 값
	KSProtectVar<int> characterNextLevelExp; // 현재 레벨의 max 값, 다음 레벨의 min 값
	KSProtectVar<int> characterNextLevelUpExp; // 다음 레벨의 max 값
	KSProtectVar<int> characterIndex;
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
	KSProtectVar<long long> resetTimestamp;
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

class TimeEventInfo
{
public:
	KSProtectVar<long long> startDate;
	KSProtectVar<long long> endDate;
	KSProtectVar<int> startTime;
	KSProtectVar<int> endTime;
	KSProtectStr event_type;
	KSProtectVar<int> int_value;
	KSProtectVar<float> float_value;
};

enum TimeEventType
{
	kTimeEventType_gold = 1,
	kTimeEventType_heart,
	kTimeEventType_card,
	kTimeEventType_clear
};

class CollectionCardInfo
{
public:
	KSProtectVar<int> puzzle_number;
	KSProtectVar<int> stage_number;
	KSProtectVar<bool> is_puzzle_card;
	KSProtectVar<int> grade1_card_number;
	KSProtectVar<int> grade2_card_number;
	KSProtectVar<int> grade3_card_number; // or clear_card
	KSProtectVar<int> grade4_card_number; // or perfect_card
	
	int getHighCardNumber()
	{
		int return_value = grade4_card_number.getV();
		if(return_value < 0)
			return_value = grade3_card_number.getV();
		else
			return return_value;
		if(return_value < 0)
			return_value = grade2_card_number.getV();
		else
			return return_value;
		if(return_value < 0)
			return_value = grade1_card_number.getV();
		else
			return return_value;
		return return_value;
	}
	
	vector<int> getCardSet()
	{
		vector<int> return_value;
		return_value.clear();
		return_value.push_back(grade1_card_number.getV());
		return_value.push_back(grade2_card_number.getV());
		return_value.push_back(grade3_card_number.getV());
		return_value.push_back(grade4_card_number.getV());
		return return_value;
	}
	
	CollectionCardInfo() :
	stage_number(-1),
	is_puzzle_card(false),
	grade1_card_number(-1),
	grade2_card_number(-1),
	grade3_card_number(-1),
	grade4_card_number(-1)
	{
		
	}
};

class HeartTime;

#define SGD_KEY	0xD9
#define mySGD StarGoldData::sharedInstance()
class KSLabelTTF;
class StarGoldData : public CCObject
{
public:
	
	static StarGoldData* sharedInstance();
	
	int client_version;
	
	void resetLabels();
	
	void setStarLabel(CCLabelBMFont* t_label);
	void setIngameGoldLabel( CCLabelBMFont* t_label, KSLabelTTF* t_label2 );
//	int getStar();
//	void setStar(int t_star);
	
	void setGoldLabel(CCLabelBMFont* t_label);
//	int getGold();
//	void setGold(int t_gold, bool is_write = true);
	
	void setHeartTime(HeartTime* t_heart);
	HeartTime* getHeartTime();
	
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
	
	void setBeforeRankUpStageGrade(int t_i);
	int getBeforeRankUpStageGrade();
	
	int getStageGold();
	int getStageBaseGold();
	
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
	int getGameUseTime();
	
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
	
	vector<int> getCollectionCardSet(int recent_card_number);
	
	void changeSortType(CardSortType t_type);
	
	void addHasGottenCardNumber(int card_number);
	
	int getHasGottenCardsDataCardNumber(int index);
	CardSortInfo getHasGottenCardData(int index);
	CardSortInfo getHasGottenCardDataForCardNumber(int t_card_number);
	bool isCardMorphing(int card_number);
	void onCardMorphing(int card_number);
	
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
	int isHasGottenCardsToIndex(int t_card_number);
	
	void resetHasGottenCards();
	void initTakeCardInfo(Json::Value card_list, vector<int>& card_data_load_list);
	
	CommandParam getUpdatePuzzleHistoryParam(PuzzleHistory t_history, jsonSelType call_back);
	int getOpenPuzzleCount();
	int getPuzzleHistorySize();
	PuzzleHistory getPuzzleHistoryForIndex(int t_index);
	PuzzleHistory getPuzzleHistory(int puzzle_number);
	void setPuzzleHistory(PuzzleHistory t_history, jsonSelType call_back);
	void setPuzzleHistoryForNotSave(PuzzleHistory t_history);
	void resultUpdatePuzzleHistory(Json::Value result_data);
	void initPuzzleHistory(Json::Value history_list);
	
	CommandParam getUpdatePieceHistoryParam(PieceHistory t_history, jsonSelType call_back);
	Json::Value getSavePieceHistoryParam(PieceHistory t_history);
	bool isClearPiece(int stage_number);
	int getPieceHistorySize();
	PieceHistory getPieceHistoryForIndex(int t_index);
	PieceHistory getPieceHistory(int stage_number);
	void setPieceHistoryForNotSave(PieceHistory t_history);
	void setPieceHistory(PieceHistory t_history, jsonSelType call_back);
	void resultUpdatePieceHistory(Json::Value result_data);
	void initPieceHistory(Json::Value history_list);
	
	void initSelectedCharacterNo(int t_i);
	void initCharacterLevel(int t_i);
	void initCharacterNextPrice(int t_i);
	void initCharacterPower(int t_i);
	void initCharacterNextPower(int t_i);
	void initCharacterPrevPower(int t_i);
	void initCharacterIsMaxLevel(int t_i);
	CharacterHistory getSelectedCharacterHistory();
	CharacterHistory getSelectedCharacterHistoryOriginal();
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
	void setPlayContinueFeeEndless(int t_data);
	void setSessionTime(int t_i);
	int getSessionTime();
	
	void setClearTakeCardCnt(int t_i);
	int getClearTakeCardCnt();
	
	void setUnlockFrameCnt(int t_i);
	int getUnlockFrameCnt();
	
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
	int getPlayContinueFeeEndless();
	
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
	
	string getAllClearReward();
	void setAllClearReward(string t_str);
	
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
	void setUserdataHighPiece(int t_i);
	int getUserdataHighPiece();
	void setUserdataOnlyOneBuyPack(int t_i);
	int getUserdataOnlyOneBuyPack();
	void setUserdataSelectedCharNO(int t_i);
	int getUserdataSelectedCharNO();
	void setUserdataCharLevel(int t_i);
	int getUserdataCharLevel();
	
	void setUserdataEndlessIngWin(int t_i);
	int getUserdataEndlessIngWin();
	void setUserdataEndlessIngWeek(int t_i);
	int getUserdataEndlessIngWeek();
	int getUserdataEndlessScore();
	
	void setUserdataAchieveMapGacha(int t_i);
	int getUserdataAchieveMapGacha();
	void setUserdataAchieveNoFail(int t_i);
	int getUserdataAchieveNoFail();
	void setUserdataAchieveHunter(int t_i);
	int getUserdataAchieveHunter();
	void setUserdataAchieveChangeMania(int t_i);
	int getUserdataAchieveChangeMania();
	void setUserdataAchieveFail(int t_i);
	int getUserdataAchieveFail();
	void setUserdataAchievePerfect(int t_i);
	int getUserdataAchievePerfect();
	void setUserdataAchieveSeqAttendance(int t_i);
	int getUserdataAchieveSeqAttendance();
	void setUserdataAchieveItemBuyCount(int t_i);
	int getUserdataAchieveItemBuyCount();
	
	void setUserdataMissileInfoNextPrice(int t_i);
	int getUserdataMissileInfoNextPrice();
	void setUserdataMissileInfoPower(int t_i);
	int getUserdataMissileInfoPower();
	void setUserdataMissileInfoNextPower(int t_i);
	int getUserdataMissileInfoNextPower();
	void setUserdataMissileInfoPrevPower(int t_i);
	int getUserdataMissileInfoPrevPower();
	void setUserdataMissileInfoIsMaxLevel(int t_i);
	int getUserdataMissileInfoIsMaxLevel();
	
	string getInappProduct(int t_index);
	string getEventInappProduct(int t_index);
	
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
	void setItem11OpenStage(int t_i);
	int getItem11OpenStage();
	void setItemGachaOpenStage(int t_i);
	int getItemGachaOpenStage();
	
	void setPuzzlePerfectRewardRuby(int t_i);
	int getPuzzlePerfectRewardRuby();
	void setEndlessMinPiece(int t_i);
	int getEndlessMinPiece();
	
	void setSpecialEventPuzzleNumber(int t_i);
	int getSpecialEventPuzzleNumber();

	
	string getKakaoMsg();
	void setKakaoMsg(string t_str);

	
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
	
	deque<string> real_time_message_que;
	bool isEmptyRealTimeMessageQueue();
	void pushRealTimeMessageQueue(string t_message);
	void popRealTimeMessageQueue();
	string frontRealTimeMessageQueue();
	
	void setNextSceneName(string scene_name);
	string getNextSceneName();
	
	deque<bool> is_ingame_before_have_stage_cards;
//	int ingame_before_stage_rank;
	bool getIsNotClearedStage();
	int getIsUnlockPuzzle();
	void setIsUnlockPuzzle(int t_i);
	int getIsPerfectPuzzle();
	void setIsPerfectPuzzle(int t_i);
	
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
	
	bool is_endless_mode;
	bool is_hell_mode;
	KSProtectVar<float> hell_play_time;
	KSProtectVar<bool> endless_my_victory_on;
	KSProtectVar<int> endless_my_victory;
//	KSProtectVar<int> endless_my_total_score;
	KSProtectVar<int> endless_my_level;
	KSProtectVar<int> endless_my_high_victory;
	KSProtectVar<int> endless_my_lose;
	KSProtectVar<int> endless_my_high_score;
	KSProtectVar<int> endless_my_win;
	
	KSProtectVar<int> endless_my_ing_win;
	KSProtectVar<int> endless_my_ing_score;
	
	KSProtectVar<int> endless_memberID;
	KSProtectStr endless_nick;
	KSProtectStr endless_flag;
	KSProtectVar<int> endless_victory;
	KSProtectVar<int> endless_autoLevel;
	KSProtectVar<int> endless_level;
	KSProtectVar<int> endless_score;
	KSProtectVar<long long> endless_regDate;
	KSProtectVar<int> endless_lose;
	KSProtectVar<int> endless_win;
	
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
	void addChangeGoodsIngameGold(int t_value, float t_value2);
	void addChangeGoods(string t_exchangeID, GoodsType t_type = kGoodsType_begin, int t_value = 0, string t_statsID = "", string t_statsValue = "", string t_content = "", bool t_isPurchase = false);
	void addChangeGoods(string t_exchangeID, vector<ChangeGoodsDataDetail> t_detail_list);
//	void addChangeGoods(GoodsType t_type, int t_value, string t_statsID = "", string t_statsValue = "", string t_content = "", bool t_isPurchase = false);
//	void updateChangeGoods(GoodsType t_type, int t_value, string t_statsID = "", string t_statsValue = "", string t_content = "", bool t_isPurchase = false);
	void clearChangeGoods();
	void changeGoods(jsonSelType t_callback);
	void changeGoodsTransaction(vector<CommandParam> command_list, jsonSelType t_callback, bool is_end_game = true);
	void refreshGoodsData(string t_key, int t_count);
	vector<CommandParam> getChangeGoodsParam(jsonSelType t_callback);
	void saveChangeGoodsTransaction(Json::Value result_data);
	bool isChangedGoods();
	
	void linkChangeHistory(Json::Value result_data);
	
	int getGoodsValue(GoodsType t_type);
	
	TimeInfo keep_time_info;
	
	bool is_changed_userdata;
	UserdataType getUserdataKeyToType(string t_key);
	string getUserdataTypeToKey(UserdataType t_type);
	void changeUserdata(jsonSelType t_callback);
	CommandParam getChangeUserdataParam(jsonSelType t_callback);
	void initUserdata(Json::Value result_data);
	void clearChangeUserdata();
	void refreshUserdata(UserdataType t_type, int t_i);
	
	bool isPossibleShowPurchasePopup(PurchaseGuideType t_type);
	void showPurchasePopup(PurchaseGuideType t_type);
	
	KSProtectVar<int> gacha_item;
	
	void initTodayMission(Json::Value t_info);
	TodayMission today_mission_info;
	bool is_today_mission_first;
	
	void increaseCatchCumber();
	int getCatchCumberCount();
	
	CommandParam getUpdateTodayMissionParam(jsonSelType t_callback);
	
	bool is_on_attendance;
	Json::Value attendance_data;
	void initAttendance(Json::Value result_data);
	void resetAttendance();
	
	bool is_on_rank_reward;
	Json::Value rank_reward_data;
	void initRankReward(Json::Value result_data);
	void resetRankReward();
	
	KSProtectVar<int> area_score;
	KSProtectVar<int> damage_score;
	KSProtectVar<int> combo_score;
	void resetIngameDetailScore();
	
	KSProtectVar<int> damaged_score;
	
	KSProtectVar<float> temp_endless_play_limit_time;
	Json::Value temp_replay_data;
	KSProtectStr temp_endless_nick;
	KSProtectStr temp_endless_flag;
	KSProtectVar<float> temp_endless_score;
	
	KSProtectVar<int> recent_week_no;
	
	void setEventString(string t_str);
	string getEventString();
	
	void setIsAlwaysSavePlaydata(int t_i);
	int getIsAlwaysSavePlaydata();
	void setSendPvpPlayDataRate(int t_i);
	int getSendPvpPlayDataRate();
	
	
	bool is_morphing_noti;
	
	void initTimeEventList(Json::Value t_list);
	string getTimeEventTypeString(TimeEventType t_type);
	bool isTimeEvent(TimeEventType t_type);
	int getTimeEventIntValue(TimeEventType t_type);
	float getTimeEventFloatValue(TimeEventType t_type);
	void refreshTimeEvent();
	
	Json::Value dummy_list;
	
	KSProtectVar<int> stage_attack_count;
	KSProtectVar<int> hunt_value;
	
	KSProtectVar<bool> is_new_puzzle_card;
	Json::Value new_puzzle_card_info;
	
	void initCollectionBook();
	void heartRefreshSuccess(Json::Value result_data);
	
	int network_check_cnt;
	int max_network_check_cnt;
	
	long long user_index;
	int total_card_cnt;
	
	int loading_tip_back_number;
	
	int pvp_continue_cnt;
	int ingame_continue_cnt;
	int start_map_gacha_use_gold_cnt;
	
	bool is_on_accountLinkLead;
	
	long long getIntroducerID();
	void setIntroducerID(long long t_id);
	
	void setIntroduceStage(int t_i);
	int getIntroduceStage();
    
    Json::Value product_infos;
    Json::Value getProductInfo(string t_id);
	
	bool is_hell_mode_enabled;
	
	void setDiaryDownUrl(string t_str);
	string getDiaryDownUrl();
	
	void setIosHideVer(string t_str);
	string getIosHideVer();
	
	KSProtectVar<double> rewind_cnt_per_frame;
	KSProtectVar<double> character_magnetic;
	
	bool is_option_tutorial;
	
	void addCharacterHistoryForGacha(Json::Value result_data);
	
	vector<int> is_cleared_grade;
	
	UISceneCode ui_scene_code;
	KSProtectVar<int> add_time_value;
	Json::Value hell_balance;
	
private:
	
	vector<CollectionCardInfo> normal_puzzle_cards;
	vector<CollectionCardInfo> event_puzzle_cards;
	vector<CollectionCardInfo> special_cards;
	
	bool is_ingame_gold;
	bool is_ingame_sub_gold;
	
	bool is_not_cleared_stage;
	int is_unlock_puzzle;
	int is_perfect_puzzle;
	
	CardStrengthBefore card_strength_before;
	
	int strength_target_card_number;
	
	deque<AchievementCode> achieve_noti_que;
	
	CCLabelBMFont* star_label;
	CCLabelBMFont* gold_label;
	CCLabelBMFont* friend_point_label;
	CCLabelBMFont* ingame_gold_label;
	KSLabelTTF* ingame_sub_gold_label;
	HeartTime* heart_time;
	
	vector<KSProtectVar<int>> bonus_item_cnt;

	int clear_reward_gold;
	
	bool is_after_scene_chapter;
	
	KSProtectVar<bool> is_cleared;
	KSProtectVar<float> base_score;
	KSProtectVar<float> score;
	KSProtectVar<float> percentage;
	KSProtectVar<int> stage_grade;
	KSProtectVar<int> before_rankup_stage_grade;
	KSProtectVar<int> keep_gold;
	KSProtectVar<int> game_time;
	KSProtectVar<int> game_use_time;
	KSProtectVar<int> ingame_gold;
	KSProtectVar<float> ingame_sub_gold;
	
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
	KSProtectVar<int> gacha_ruby_fee; // 젬 가챠 가격(젬)
	KSProtectVar<int> gacha_social_fee; // 소셜 가챠 가격(소셜포인트)
	KSProtectVar<int> gacha_gold_fee_retry; // 골드 가챠 가격(골드)
	KSProtectVar<int> gacha_ruby_fee_retry; // 젬 가챠 가격(젬)
	KSProtectVar<int> gacha_social_fee_retry; // 소셜 가챠 가격(소셜포인트)
	KSProtectVar<int> card_upgrade_gold_fee; // 카드 일반강화 가격(골드)
	KSProtectVar<int> card_upgrade_ruby_fee; // 카드 고급강화 가격(젬)
	KSProtectVar<int> heart_send_cool_time; // 하트 보내기 쿨타임
	KSProtectVar<int> invite_max_a_day; // 하루 초대 인원 제한
	KSProtectVar<int> invite_cool_day; // 한 친구당 친구초대 주기 일수
	KSProtectVar<int> play_continue_fee; // 이어하기 가격(젬)
	KSProtectVar<int> card_durability_up_fee; // 카드 내구도 회복 가격(젬)
	KSProtectVar<int> gacha_map_fee; // 맵 가챠 가격(골드)
	KSProtectVar<int> remove_friend_cool_time; // 친구 삭제 간격(초)
	KSProtectVar<int> SP_send_heart; // 소셜포인트 획득량(하트 보낼때)
	KSProtectVar<int> SP_send_ticket; // 소셜포인트 획득량(티켓 보낼때)
	KSProtectVar<int> SP_finished_challenge; // 소셜포인트 획득량(도전하기 끝나서 결과 나왔을때 요청자, 대상자)
	KSProtectVar<int> SP_help_challenge; // 소셜포인트 획득량(도움요청 끝나서 결과 나왔을때 요청자, 대상자)
	KSProtectVar<int> SP_send_boast; // 소셜포인트 획득량(자랑하기 보낼때)
	KSProtectVar<int> SP_get_time; // 받은 메세지 몇초 안에 확인해야 소셜포인트 줄건지 설정
	KSProtectVar<int> SP_get_heart; // 소셜포인트 획득량(하트온지 SPGetTime초 안에 수락하면 주는 포인트)
	KSProtectVar<int> gacha_one_percent_fee; // 1프로 가챠 가격(젬)
	KSProtectVar<float> ai_adder_on_drew_or_damaged; //
	KSProtectVar<float> fury_percent; //
	KSProtectVar<int> SP_rent_card_thanks; // 소셜포인트 획득량-카드빌리고 보답할때
	KSProtectVar<int> sessionTime; // 세션타임
	
	KSProtectVar<int> item_gacha_gold_fee; // 아이템 가챠 가격
	KSProtectVar<int> item_gacha_replay_gold_fee; // 아이템 가챠 다시뽑기 가격
	
	KSProtectVar<int> upgrade_gold_fee;
	KSProtectVar<int> ingame_tutorial_reward_gold;
	
	KSProtectVar<int> item9_open_stage;
	KSProtectVar<int> item6_open_stage;
	KSProtectVar<int> item11_open_stage;
	KSProtectVar<int> itemGacha_open_stage;
	
	KSProtectVar<int> puzzle_perfect_reward_ruby;
	
	KSProtectVar<int> endless_min_piece;
	KSProtectVar<int> play_continue_fee_endless;
	
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
	
	KSProtectVar<int> clear_take_card_cnt;
	KSProtectVar<int> unlock_frame_cnt;
	
	KSProtectVar<int> special_event_puzzle_number;
	
	KSProtectStr all_clear_reward;
	KSProtectStr kakao_msg;
	
	vector<ChangeUserdataValue> changed_userdata_list;
	jsonSelType change_userdata_callback;
	map<UserdataType, KSProtectVar<int>> userdata_storage;
	void resultChangeUserdata(Json::Value result_data);
	
	
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
	
	KSProtectVar<long long> introducerID;
	
	vector<TimeEventInfo> time_event_list;
	
	string event_string;
	int is_always_save_playdata;
	int send_pvp_play_data_rate;
	
	KSProtectVar<int> introduce_stage;
	
	string diaryDownUrl;
	string iosHideVer;
	
	string app_type;
	int app_version;
	
	void myInit();
	
	
	CC_SYNTHESIZE(long long, remove_message_member_id, RemoveMessageMemberId);
	CC_SYNTHESIZE(int, remove_message_mail_no, RemoveMessageMailNo);
	
	COMMON_VAR(int, testInt, TestInt);
	
	COMMON_VAR(int, addGemReward, AddGemReward);
	COMMON_VAR(int, iosMenuVisible, IosMenuVisible);
	
	COMMON_VAR_STR(nmlGc, NmlGc);
	COMMON_VAR_STR(prmGc, PrmGc);
	COMMON_VAR(int, gachaRefreshTime, GachaRefreshTime);
	
	COMMON_VAR(int, pvpWinExp, PvpWinExp);
	COMMON_VAR(int, pvpLoseExp, PvpLoseExp);
	COMMON_VAR(int, gachaCharExp, GachaCharExp);
	COMMON_VAR(int, gachaRefreshGem, GachaRefreshGem);
	COMMON_VAR(int, addTimeItemLimit, AddTimeItemLimit);
	COMMON_VAR(int, isOnGiftForIOS, IsOnGiftForIOS);
	
	COMMON_VAR_STR(iosMenu, IosMenu);
	COMMON_VAR_STR(goldBalance, GoldBalance);
};

#endif
