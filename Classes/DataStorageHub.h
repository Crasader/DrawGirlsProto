// DataStorageHub.h
//

#ifndef LZZ_DataStorageHub_h
#define LZZ_DataStorageHub_h
#include "cocos2d.h"
#include "SaveData.h"
#include "EnumDefine.h"
#include "hspConnector.h"
#include "ServerDataSave.h"
#include <map>
#include "KSProtect.h"

using namespace cocos2d;
using namespace std;

typedef enum t_DSH_Key{
	kDSH_Key_savedStar,
	kDSH_Key_savedGold,
	kDSH_Key_notFirstExe,
	kDSH_Key_bgmOff,
	kDSH_Key_effectOff,
	kDSH_Key_heartCnt,
	kDSH_Key_heartTime,
	kDSH_Key_haveItemCnt_int1,
	kDSH_Key_isShowItem_int1,
	kDSH_Key_selectedCard,
	
	kDSH_Key_selectedCharacter_int1_weaponSlot_int2, // 0~(n-1)번째 캐릭터의 1~n번째 슬롯에 장착된 뷰티스톤의 id
	kDSH_Key_selfBeautyStoneID, // 1~n 의 뷰티스톤의 id
	kDSH_Key_haveBeautyStoneCnt, // 0~n 의 뷰티스톤 개수
	kDSH_Key_haveBeautyStoneID_int1, // 1~haveBeautyStoneCnt 의 뷰티스톤 id
	kDSH_Key_beautyStoneType_int1, // 1~selfBeautyStoneID 의 뷰티스톤 type
	kDSH_Key_beautyStoneRank_int1, // 1~selfBeautyStoneID 의 뷰티스톤 rank
	kDSH_Key_beautyStoneLevel_int1, // 1~selfBeautyStoneID 의 뷰티스톤 level
	
//	kDSH_Key_endPlayedStage,
//	kDSH_Key_stageClearRank_int1,
	
	kDSH_Key_cardSortType,
	kDSH_Key_lastSelectedStageForPuzzle_int1,
	kDSH_Key_uiType,
	kDSH_Key_puzzleMode,
	kDSH_Key_selectedPuzzleNumber,
	kDSH_Key_allHighScore,
	kDSH_Key_jackBaseSpeed,
	
	kDSH_Key_haveTicketCnt,
	kDSH_Key_ticketUserId_int1,
//	kDSH_Key_openStageCnt,
//	kDSH_Key_openStageNumber_int1,
//	kDSH_Key_isOpenStage_int1,
//	kDSH_Key_clearStageCnt,
//	kDSH_Key_clearStageNumber_int1,
//	kDSH_Key_isClearStage_int1,
	kDSH_Key_nick,
	kDSH_Key_flag,
	kDSH_Key_wasTutorialPopupShockWave,
	kDSH_Key_wasTutorialPopupMissileTrace,
	kDSH_Key_wasTutorialPopupCrashArea,
	kDSH_Key_controlJoystickDirection,
	kDSH_Key_isControlJoystickNotFixed,
	kDSH_Key_isEnableIrregularDirection,
	kDSH_Key_isDisableDrawButton,
	kDSH_Key_isAlwaysInvisibleJoystick,
	kDSH_Key_isAlwaysCenterCharacter,
	kDSH_Key_isDisableLineOver,
	kDSH_Key_isJoystickCenterNotFixed,
	
	kDSH_Key_hasShowTutorial_int1,
	kDSH_Key_selectedCharacter,
	kDSH_Key_weaponLevelForCharacter_int1,
	kDSH_Key_isCharacterUnlocked_int1,
	
	kDSH_Key_noticeViewDate_int1,
	
	kDSH_Key_achieveDataCnt,
	kDSH_Key_achieveData_int1_code, // 1 ~ cnt
	kDSH_Key_achieveData_int1_value, // code
	
	kDSH_Key_achieve_base,
	kDSH_Key_achieve_sendHeartCnt,
	kDSH_Key_achieve_playBonusGameCnt,
	kDSH_Key_achieve_mapGachaCnt,
	kDSH_Key_achieve_totalFeverCnt,
	kDSH_Key_achieve_seqNoFailCnt,
	kDSH_Key_achieve_catchMonsterCnt,
	kDSH_Key_achieve_continueCnt,
	kDSH_Key_achieve_attendanceCnt,
	kDSH_Key_achieve_changeCnt,
	kDSH_Key_achieve_failCnt,
	kDSH_Key_achieve_inviteCnt,
	kDSH_Key_achieve_challengeCnt,
	kDSH_Key_achieve_helpCnt,
	kDSH_Key_achieve_helpAcceptCnt,
	kDSH_Key_achieve_challengeAcceptCnt,
	kDSH_Key_achieve_upgradeSuccessCnt,
	kDSH_Key_achieve_perfectClearCnt,
	kDSH_Key_achieve_seqAttendanceCnt,
	kDSH_Key_achieve_gacha1Cnt,
	kDSH_Key_achieve_gacha2Cnt,
	kDSH_Key_achieve_gacha3Cnt,
	kDSH_Key_achieve_gacha4Cnt,
	kDSH_Key_achieve_weeklyTopCnt,
	kDSH_Key_achieve_startLuckyCnt,
	kDSH_Key_achieve_enterShopCnt,
	kDSH_Key_achieve_itemBuyCnt,
	kDSH_Key_achieve_end,
	
	kDSH_Key_tutorial_flowStep,
	
	kDSH_Key_minigame_playedCnt,
	kDSH_Key_minigame_int1_stageNumber, // int1 : playedCnt loop number 1 ~ playedCnt
	kDSH_Key_minigame_int1_isPlayed, // int1 : stage_number
	
	kDSH_Key_was_opened_tutorial_dimed_achievement,
	kDSH_Key_was_opened_tutorial_dimed_cardchange,
	kDSH_Key_was_opened_tutorial_dimed_cardsetting,
	kDSH_Key_was_opened_tutorial_dimed_cardstrength,
	kDSH_Key_was_opened_tutorial_dimed_friendoption_invite,
	kDSH_Key_was_opened_tutorial_dimed_friendoption_list,
	kDSH_Key_was_opened_tutorial_dimed_friendoption_surch,
	kDSH_Key_was_opened_tutorial_dimed_main,
	kDSH_Key_was_opened_tutorial_dimed_rank,
	kDSH_Key_was_opened_tutorial_dimed_startsetting,
	
	kDSH_Key_mission_willNeverWatch_bossLifeZero,
	kDSH_Key_mission_willNeverWatch_subCumberCatch,
	kDSH_Key_mission_willNeverWatch_bigArea,
	kDSH_Key_mission_willNeverWatch_itemCollect,
	kDSH_Key_mission_willNeverWatch_perfect,
	kDSH_Key_mission_willNeverWatch_timeLimit,
	kDSH_Key_mission_willNeverWatch_sequenceChange,
	
	kDSH_Key_storyReadPoint,
	
	kDSH_Key_isSafetyMode,
	kDSH_Key_isPushOff,
	kDSH_Key_isMessageOff,
	
	kDSH_Key_atTimeShowFirstPurchase,
	kDSH_Key_atTimeShowEmptyItem,
	kDSH_Key_atTimeShowStupidNpuHelp,
	kDSH_Key_atTimeShowEventRubyShop,
	kDSH_Key_atTimeShowLevelupGuide,
	
	kDSH_Key_stageHighRank_int1,
	
	kDSH_Key_showedScenario,
	kDSH_Key_isShowEndlessModeTutorial,
	
	
	kSDH_Key_isShowOnePercentTutorial,
	kDSH_Key_isShowGababoTutorial,
	kDSH_Key_accountType,
	
	kDSH_Key_clientVersion,
	kDSH_Key_dataFileCnt,
	kDSH_Key_dataFileName_int1,
	
	kDSH_Key_isCheckTerms,
	kDSH_Key_savedMemberID,
	kDSH_Key_timeZone,
	
	kDSH_Key_showedKindTutorial_int1,
	kDSH_Key_showedEmptyItemSale1,
	kDSH_Key_showedEmptyItemSale2,
	kDSH_Key_showedCardRankUp,
	
	kDSH_Key_isOffCardMorphingSound,
	
	kDSH_Key_joystickSize,
	kDSH_Key_isOffJoystickVib,
	kDSH_Key_isNotFirstItemGacha,
	
	kDSH_Key_cardSettingTableOffsetDefault,
	kDSH_Key_cardSettingTableOffsetTake,
	kDSH_Key_cardSettingTableOffsetTakeReverse,
	kDSH_Key_cardSettingTableOffsetGrade,
	kDSH_Key_cardSettingTableOffsetGradeReverse,
	kDSH_Key_cardSettingTableOffsetEvent,
	
	kDSH_Key_savedStartPackFirstTime,
	
	kDSH_Key_normalGachaList,
	kDSH_Key_premiumGachaList,
	kDSH_Key_normalGachaDataTime,
	kDSH_Key_premiumGachaDataTime,
	
	kDSH_Key_cardGachaTryCnt_int1,
	kDSH_Key_composeListOffsetX
}DSH_Key;

enum SpecialTutorialCode{
	kSpecialTutorialCode_control = 100000,
	kSpecialTutorialCode_slimLine,
	kSpecialTutorialCode_100percent,
	kSpecialTutorialCode_bossPrison,
	kSpecialTutorialCode_lineTangle
};

enum ControlJoystickDirection{
	kControlJoystickDirection_left = 0,
	kControlJoystickDirection_right
};

enum SaveUserData_Key{
	kSaveUserData_Key_base = 0,
	kSaveUserData_Key_star,
	kSaveUserData_Key_gold,
	kSaveUserData_Key_heart,
	kSaveUserData_Key_item,
	kSaveUserData_Key_beautyStone,
	kSaveUserData_Key_cardsInfo,
	kSaveUserData_Key_highScore,
//	kSaveUserData_Key_stageRank,
	kSaveUserData_Key_selectedCard,
	kSaveUserData_Key_haveTicket,
//	kSaveUserData_Key_openStage,
	kSaveUserData_Key_nick,
	kSaveUserData_Key_character,
	kSaveUserData_Key_achieve,
	kSaveUserData_Key_minigame,
	kSaveUserData_Key_tutorial,
	kSaveUserData_Key_end
};

#define myDSH DataStorageHub::sharedInstance()

enum PuzzleMapSceneShowType{
	kPuzzleMapSceneShowType_init = 0,
	kPuzzleMapSceneShowType_stage,
	kPuzzleMapSceneShowType_clear,
	kPuzzleMapSceneShowType_fail,
	kPuzzleMapSceneShowType_stageSetting
};

enum MainFlowSceneShowType
{
	kMainFlowSceneShowType_init = 0,
	kMainFlowSceneShowType_hellReplay
};

#define LZZ_INLINE inline
class DataStorageHub : public CCObject
{
public:
	static DataStorageHub * sharedInstance ();
	bool getBoolForKey (DSH_Key t_key);
	bool getBoolForKey (DSH_Key t_key, int key_val1);
	bool getBoolForKey (DSH_Key t_key, int key_val1, int key_val2);
	void setBoolForKey (DSH_Key t_key, bool t_b, bool diskWrite = true);
	void setBoolForKey (DSH_Key t_key, int key_val1, bool t_b, bool diskWrite = true);
	void setBoolForKey (DSH_Key t_key, int key_val1, int key_val2, bool t_b, bool diskWrite = true);
	int getUserIntForStr (string t_key, int t_default);
	void setUserIntForStr (string t_key, int t_val1, bool diskWrite = true);
	string getUserStrForStr (string t_key, string t_default);
	void setUserStrForStr (string t_key, string t_val1, bool diskWrite = true);
	int getIntegerForKey (DSH_Key t_key);
	int getIntegerForKeyDefault (DSH_Key t_key, int def);
	int getIntegerForKey (DSH_Key t_key, int key_val1);
	int getIntegerForKey (DSH_Key t_key, int key_val1, int key_val2);
	void setIntegerForKey (DSH_Key t_key, int val1, bool diskWrite = true);
	void setIntegerForKey (DSH_Key t_key, int key_val1, int val1, bool diskWrite = true);
	string getStringForKey (DSH_Key t_key);
	void setStringForKey (DSH_Key t_key, string val1, bool diskWrite = true);
	string getStringForKey (DSH_Key t_key, int key_val1);
	void setStringForKey (DSH_Key t_key, int key_val1, string val1, bool diskWrite = true);
	void setIntegerForKey (DSH_Key t_key, int key_val1, int key_val2, int val1, bool diskWrite = true);
	CCSize getDesignResolutionSize ();
	float ui_top;
	float puzzle_ui_top;
	float ui_center_y;
	float ui_bottom;
	CCPoint ui_zero_point;
	float screen_convert_rate;
	CCPoint ui_touch_convert;
	CCPoint mult_point;
	CCPoint add_point;
	CCPoint wideWidthFixTouch (CCPoint t_p);
	string getKey (DSH_Key t_name);
//	Json::Value getSaveAllUserDataParam ();
//	void loadAllUserData (Json::Value result_data);
	void writeParamForKey (Json::Value & data, SaveUserData_Key t_key);
//	void saveUserData (vector <SaveUserData_Key> const & key_list, function <void(Json::Value)> t_selector);
//	void saveAllUserData (jsonSelType t_saved);
	void clear();
	void resetDSH ();
	bool isCheatKeyEnable ();
	int getPuzzleMapSceneShowType ();
	void setPuzzleMapSceneShowType (int t_type);
	int getMainFlowSceneShowType();
	void setMainFlowSceneShowType(int t_type);
	void fFlush ();
	void removeCache();
	void initReturnPair();
	
//	void changedCcbAnimationManager();
//	int ccb_animation_manager_count;
//	void regiCcbAnimationManager(CCBAnimationManager* t_manager);
//	void unregiCcbAnimationManager(CCBAnimationManager* t_manager);
//	void unregiAllCcbAnimationManagers();
private:
	
//	vector<CCBAnimationManager*> manager_list;
//	vector<CCBAnimationManager*> check_manager_list;
	
	map<string, KSProtectVar<bool>> dsh_cache_bool;
	map<string, KSProtectVar<bool>>::iterator iter_bool;
	map<string, KSProtectVar<int>> dsh_cache_int;
	map<string, KSProtectVar<int>>::iterator iter_int;
	map<string, KSProtectStr> dsh_cache_string;
	map<string, KSProtectStr>::iterator iter_string;
	map<DSH_Key, string> return_value;
//	map<string, bool> dsh_cache_bool;
//	map<string, bool>::iterator iter_bool;
//	map<string, int> dsh_cache_int;
//	map<string, int>::iterator iter_int;
//	map<string, string> dsh_cache_string;
//	map<string, string>::iterator iter_string;
	
	SaveData * myDefault;
	float gold_get_rate;
	bool is_cheat_key_enabled;
	int mainflow_scene_show_type;
	int puzzle_map_scene_show_type;
	void myInit ();
	
	CC_SYNTHESIZE(int, default_gold, DefaultGold); // 기본 골드
	CC_SYNTHESIZE(int, default_ruby, DefaultRuby); // 기본 젬
	
};
#undef LZZ_INLINE
#endif
