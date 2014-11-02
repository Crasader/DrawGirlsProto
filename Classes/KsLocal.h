#ifndef EscapeTheSpace_KsLocalization_h
#define EscapeTheSpace_KsLocalization_h

#include "cocos2d.h"
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

using namespace cocos2d;
using namespace std;
#include <map>
#include <string>
/*
 
 myLoc->getLocalForKey(kKsLocalKey_tutorial7)
 
 */

#define myLoc KsLocal::sharedInstance()
#define getLocal(X) (KsLocal::sharedInstance()->getLocalForKey(X))
enum class LK
{
	kItem1,
	kReGacha,
	kRewardConfirm,
	kRewardTitle,
	kBonusGame,
	kBonusGameStart,
	kBonusGameDesc,
	kBonusGameGababo,
	kGababoDesc1,
	kGababoDesc2,
	kBonusGameTouch,
	kTouchDesc1,
	kTouchDesc2,
	kBonusCong,
	kAccountConf,
	kLinkingGoogle,
	kLinkingGameCenter,
	kLinkingFacebook,
	kLinkingX,
	kLinkingUnknown,
	kLinkingAccount,
	kMemberNo,
	kSaveDesc1,
	kGoogleButton,
	kSaveDesc2,
	kFacebookButton,
	kWarningDesc,
	kAnotherHistory,
	kLoadPreviousData,
	kSaveCurrentData,
	kNetworkError,
	kCantLinking,
	kRetryLinking,
	kAccountDesc,
	kLinkingCommand,
	kLinked,
	kForeverDeleteAccount1,
	kForeverDeleteAccount2,
	kUnlinkAccount1,
	kLinkAccount1,
	kFriendList,
	kFriendListTitle,
	kFriendListMenu,
	kFriendAddMenu,
	kFriendManageMenu,
	kFriendLimitDesc,
	kFriendMyCount,
	kFriendLastConnection,
	kFriendCurrentStage,
	kFriendHeartSend,
	kFriendHeartReSend,
	kFriendInputNickName,
	kFriendAddFriend,
	kFriendAddingFriend,
	kFriendSearch,
	kFriendMailAccept,
	kFriendMailReject,
	kFriendRemoveFriend,
	kFriendConfirm,
	kFriendRemoveFriendQ,
	kFriendAddLimitMe,
	kFriendAddLimitYou,
	kFriendNotFountFriend,
	kFriendVote1,
	kFriendVote2,
	kFriendInputSkip,
	kFriendVoterInput,
	kFriendKakaoTalkFriend,
	kFriendNeedKakao,
	kFriendDontFind,
	kFriendNoti,
	kFriendError,
	kFriendSuccessfullyRecommend,
	kFriendCurrentValue,
	kFriendVotePromotion1,
	kFriendVotePromotion1_1,
	kFriendVotePromotion2,
	kFriendRecommend,
	kFriendGiftDesc,
	kFriendGiftSend,
	kFriendGiftTitle,
	kFriendGiftQ,
	kFriendGiftYes,
	kFriendNothingExist,
	kFriendCardGiftErrorMessage,
	kFriendNickInputPlz,

	kMyLocalKey_tutorial1,
	kMyLocalKey_tutorial2,
	kMyLocalKey_tutorial3,
	kMyLocalKey_tutorial4,
	kMyLocalKey_tutorial5,
	kMyLocalKey_tutorial6,
	kMyLocalKey_tutorial7,
	kMyLocalKey_tutorial8,
	kMyLocalKey_exit,
	kMyLocalKey_connectingServer,
	kMyLocalKey_inputPlease,
	kMyLocalKey_ok,
	kMyLocalKey_downImgInfo,
	kMyLocalKey_failLoadInfo,
	kMyLocalKey_replay,
	kMyLocalKey_downImgFail,
	kMyLocalKey_sameNick,
	kMyLocalKey_invalidNick,
	kMyLocalKey_longNick,
	kMyLocalKey_shortNick,
	kMyLocalKey_openRightNow,
	kMyLocalKey_openRubyValue,
	kMyLocalKey_cgpNormalTitle,
	kMyLocalKey_cgpNormalContent,
	kMyLocalKey_getReward,
	kMyLocalKey_take,
	kMyLocalKey_cgpAllPopupTitle,
	kMyLocalKey_shop,
	kMyLocalKey_ranking,
	kMyLocalKey_mycard,
	kMyLocalKey_todaymission,
	kMyLocalKey_event,
	kMyLocalKey_openStage,
	kMyLocalKey_openStageContent,
	kMyLocalKey_doOpen,
	kMyLocalKey_view,
	kMyLocalKey_condition1,
	kMyLocalKey_condition2,
	kMyLocalKey_condition3,
	kMyLocalKey_condition4,
	kMyLocalKey_stageValue,
	kMyLocalKey_failCheckRanking,
	kMyLocalKey_stage,
	kMyLocalKey_selectUseItem,
	kMyLocalKey_levelValue,
	kMyLocalKey_powerValue,
	kMyLocalKey_upgradeLevelValue,
	kMyLocalKey_totalItemSelectCount3,
	kMyLocalKey_goldNotEnought,
	kMyLocalKey_rubyNotEnought,
	kMyLocalKey_heartNotEnought,
	kMyLocalKey_baseSpeedUpTitle,
	kMyLocalKey_baseSpeedUpContent,
	kMyLocalKey_doubleItemTitle,
	kMyLocalKey_doubleItemContent,
	kMyLocalKey_magnetTitle,
	kMyLocalKey_magnetContent,
	kMyLocalKey_longTimeTitle,
	kMyLocalKey_longTimeContent,
	kMyLocalKey_itemGachaTitle,
	kMyLocalKey_itemGachaDefaultContent1,
	kMyLocalKey_itemGachaDefaultContent2,
	kMyLocalKey_itemGachaContent,
	kMyLocalKey_rubyShop,
	kMyLocalKey_goldShop,
	kMyLocalKey_heartShop,
	kMyLocalKey_realBuy,
	kMyLocalKey_option,
	kMyLocalKey_regCoupon,
	kMyLocalKey_community,
	kMyLocalKey_system,
	kMyLocalKey_bgm,
	kMyLocalKey_effect,
	kMyLocalKey_joystickPosition,
	kMyLocalKey_joystickControl,
	kMyLocalKey_safetyMode,
	kMyLocalKey_reConnect,
	kMyLocalKey_reConnectAlert1,
	kMyLocalKey_reConnectAlert2,
	kMyLocalKey_reConnectAlert3,
	kMyLocalKey_reConnectAlert4,
	kMyLocalKey_blocked,
	kMyLocalKey_blockedMsg,
	kMyLocalKey_noti,
	kMyLocalKey_pushNoti,
	kMyLocalKey_messageNoti,
	kMyLocalKey_giftboxAlert,
	kMyLocalKey_nogift,
	kMyLocalKey_memberID,
	kMyLocalKey_gameversion,
	kMyLocalKey_withdraw,
	kMyLocalKey_callCenter,
	kMyLocalKey_checkLogout,
	kMyLocalKey_cancel,
	kMyLocalKey_withdrawTitle,
	kMyLocalKey_withdrawContent1,
	kMyLocalKey_withdrawContent2,
	kMyLocalKey_withdrawContent3,
	kMyLocalKey_doWithdraw,
	kMyLocalKey_lightOff,
	kMyLocalKey_lightOn,
	kMyLocalKey_right,
	kMyLocalKey_left,
	kMyLocalKey_fix,
	kMyLocalKey_move,
	kMyLocalKey_weeklyranking,
	kMyLocalKey_dayAfterReset,
	kMyLocalKey_hourAfterReset,
	kMyLocalKey_secondAfterReset,
	kMyLocalKey_confirmGift,
	kMyLocalKey_failedConnect,
	kMyLocalKey_giftbox,
	kMyLocalKey_giftboxContent,
	kMyLocalKey_allAccept,
	kMyLocalKey_ticketBox,
	kMyLocalKey_helpBox,
	kMyLocalKey_challengeBox,
	kMyLocalKey_heartBox,
	kMyLocalKey_allView,
	kMyLocalKey_defaultSort,
	kMyLocalKey_takeOrder,
	kMyLocalKey_gradeOrder,
	kMyLocalKey_upgradeSubMent,
	kMyLocalKey_itemRegacha,
	kMyLocalKey_itemRegachaDiscountValue,
	kMyLocalKey_thisItemUse,
	kMyLocalKey_totalScore,
	kMyLocalKey_nextStage,
	kMyLocalKey_regame,
	kMyLocalKey_toMain,
	kMyLocalKey_beforeNotClearPuzzle,
	kMyLocalKey_waitForUpdate,
	kMyLocalKey_reward,
	kMyLocalKey_speed,
	kMyLocalKey_ready,
	kMyLocalKey_controlClick,
	kMyLocalKey_locked,
	kMyLocalKey_titleLoadingBegin,
	kMyLocalKey_titleLoading1,
	kMyLocalKey_titleLoading2,
	kMyLocalKey_titleLoading3,
	kMyLocalKey_titleLoading4,
	kMyLocalKey_titleLoading5,
	kMyLocalKey_titleLoading6,
	kMyLocalKey_titleLoading7,
	kMyLocalKey_titleLoading8,
	kMyLocalKey_titleLoading9,
	kMyLocalKey_titleLoading10,
	kMyLocalKey_titleLoading11,
	kMyLocalKey_titleLoading12,
	kMyLocalKey_titleLoading13,
	kMyLocalKey_titleLoading14,
	kMyLocalKey_titleLoading15,
	kMyLocalKey_titleLoading16,
	kMyLocalKey_titleLoading17,
	kMyLocalKey_titleLoading18,
	kMyLocalKey_titleLoading19,
	kMyLocalKey_titleLoading20,
	kMyLocalKey_titleLoading21,
	kMyLocalKey_titleLoading22,
	kMyLocalKey_titleLoading23,
	kMyLocalKey_titleLoading24,
	kMyLocalKey_titleLoading25,
	kMyLocalKey_titleLoading26,
	kMyLocalKey_titleLoading27,
	kMyLocalKey_titleLoading28,
	kMyLocalKey_titleLoading29,
	kMyLocalKey_titleLoading30,
	kMyLocalKey_titleLoading31,
	kMyLocalKey_titleLoading32,
	kMyLocalKey_titleLoading33,
	kMyLocalKey_titleLoading34,
	kMyLocalKey_titleLoading35,
	kMyLocalKey_titleLoading36,
	kMyLocalKey_titleLoading37,
	kMyLocalKey_titleLoading38,
	kMyLocalKey_titleLoading39,
	kMyLocalKey_titleLoading40,
	kMyLocalKey_titleLoading41,
	kMyLocalKey_titleLoading42,
	kMyLocalKey_titleLoading43,
	kMyLocalKey_titleLoading44,
	kMyLocalKey_titleLoading45,
	kMyLocalKey_titleLoading46,
	kMyLocalKey_titleLoading47,
	kMyLocalKey_titleLoading48,
	kMyLocalKey_titleLoading49,
//	kMyLocalKey_titleLoading50,
//	kMyLocalKey_titleLoading51,
	kMyLocalKey_titleLoadingEnd,
	kMyLocalKey_gamestart,
	kMyLocalKey_battleStart,
	kMyLocalKey_mymissile,
	kMyLocalKey_power,
	kMyLocalKey_gacha,
	kMyLocalKey_stageListDown,
	kMyLocalKey_frameOpenConditionTitle,
	kMyLocalKey_frameOpenConditionContent,
	kMyLocalKey_detailView,
	kMyLocalKey_detailConditionPopupTitle,
	kMyLocalKey_detailConditionPopupContent,
	kMyLocalKey_successPurchase,
	kMyLocalKey_failPurchase,
	kMyLocalKey_enoughtGoodsContent,
	kMyLocalKey_goShopButton,
	kMyLocalKey_timeOutFrame,
	kMyLocalKey_frameOpenConditionContentRuby,
	kMyLocalKey_frameOpenConditionContentTimeWeek,
	kMyLocalKey_frameOpenConditionContentTimeDate,
	kMyLocalKey_directEnter,
	kMyLocalKey_rankUpTitle,
	kMyLocalKey_rankUpSubTitle,
	kMyLocalKey_recentTakeCard,
	kMyLocalKey_rankUpRate,
	kMyLocalKey_rankup,
	kMyLocalKey_purchase,
	kMyLocalKey_firstPurchaseSale,
	kMyLocalKey_firstPurchaseMent1,
//	kMyLocalKey_firstPurchaseMent2,
//	kMyLocalKey_firstPurchaseMent3,
	kMyLocalKey_packageItemSale,
	kMyLocalKey_emptyItemSaleMent1,
	kMyLocalKey_emptyItemSaleMent2,
	kMyLocalKey_stupidNpuHelpMent1,
	kMyLocalKey_stupidNpuHelpMent2,
	kMyLocalKey_eventShopTitle,
	kMyLocalKey_eventShopMent1,
	kMyLocalKey_eventShopMent2,
	kMyLocalKey_eventShopMent3,
	kMyLocalKey_mileageMent1,
	kMyLocalKey_mileageMent2,
	kMyLocalKey_mileageMent3,
	kMyLocalKey_levelupGuideMent1,
	kMyLocalKey_levelupGuideMent2,
	kMyLocalKey_levelupGuideMent3,
	kMyLocalKey_levelupGuideMent4,
	kMyLocalKey_levelupGo,
	kMyLocalKey_allRewardGet,
	kMyLocalKey_achievement,
	kMyLocalKey_itemPackageChance,
	kMyLocalKey_emptyItemSaleTitle,
	kMyLocalKey_firstPurchaseSaleTitle,
	kMyLocalKey_mileageTitle,
	kMyLocalKey_missileUpgrade,
	kMyLocalKey_todaymissionTotalPercent1,
	kMyLocalKey_todaymissionTotalPercent2,
	kMyLocalKey_todaymissionTotalPercent3,
	kMyLocalKey_todaymissionTotalPercent4,
	kMyLocalKey_todaymissionTotalScore1,
	kMyLocalKey_todaymissionTotalScore2,
	kMyLocalKey_todaymissionTotalScore3,
	kMyLocalKey_todaymissionTotalScore4,
	kMyLocalKey_todaymissionTotalTakeGold1,
	kMyLocalKey_todaymissionTotalTakeGold2,
	kMyLocalKey_todaymissionTotalTakeGold3,
	kMyLocalKey_todaymissionTotalCatch1,
	kMyLocalKey_todaymissionTotalCatch2,
	kMyLocalKey_todaymissionTotalCatch3,
	kMyLocalKey_todaymissionTotalCatch4,
	kMyLocalKey_missionTitle0,
	kMyLocalKey_missionTitle1,
	kMyLocalKey_missionTitle2,
	kMyLocalKey_missionTitle3,
	kMyLocalKey_missionTitle4,
	kMyLocalKey_missionTitle5,
	kMyLocalKey_missionTitle6,
	kMyLocalKey_missionTitle7,
	kMyLocalKey_missionDiscription0,
	kMyLocalKey_missionDiscription1,
	kMyLocalKey_missionDiscription2,
	kMyLocalKey_missionDiscription3,
	kMyLocalKey_missionDiscription4,
	kMyLocalKey_missionDiscription5,
	kMyLocalKey_missionDiscription6,
	kMyLocalKey_missionDiscription7,
	kMyLocalKey_complete,
	kMyLocalKey_achieveSuccess,
	kMyLocalKey_achieveNotSuccess,
	kMyLocalKey_achieveReward,
	kMyLocalKey_nothingSuccessAchieve,
	kMyLocalKey_nothingRewardAchieve,
	kMyLocalKey_item4title,
	kMyLocalKey_item4ment,
	kMyLocalKey_item5title,
	kMyLocalKey_item5ment,
	kMyLocalKey_item7title,
	kMyLocalKey_item7ment,
	kMyLocalKey_item10title,
	kMyLocalKey_item10ment,
	kMyLocalKey_item8title,
	kMyLocalKey_item8ment,
	kMyLocalKey_pause,
	kMyLocalKey_continue,
	kMyLocalKey_ingameOut,
	kMyLocalKey_ingameReplay,
	kMyLocalKey_publicPlaceMode,
	kMyLocalKey_todaymissionSuccessCommon,
	kMyLocalKey_free,
	kMyLocalKey_areaReGacha,
	kMyLocalKey_go100percent,
	kMyLocalKey_accountLink,
	kMyLocalKey_maxLevel,
	kMyLocalKey_endUpgrade,
	kMyLocalKey_cardLockedTitle,
	kMyLocalKey_cardLockedMent1,
	kMyLocalKey_cardLockedMent2,
	kMyLocalKey_cardLockedMent3,
	kMyLocalKey_cardLockedMent4,
	kMyLocalKey_todaymissionRemainTime,
	kMyLocalKey_todaymissionRemainTime2,
	kMyLocalKey_todaymissionRemainTimeMinute,
	kMyLocalKey_todaymissionRemainTimeMinute2,
	kMyLocalKey_puzzleSuccessTitle,
	kMyLocalKey_puzzleSuccessMent,
	kMyLocalKey_puzzlePerfectTitle,
	kMyLocalKey_puzzlePerfectMent,
	kMyLocalKey_doYouWantToContinue,
	kMyLocalKey_onContinue,
	kMyLocalKey_offContinue,
	kMyLocalKey_notUseItem,
	kMyLocalKey_newItemTitle,
	kMyLocalKey_newItemMent,
	kMyLocalKey_dieTutorial1,
	kMyLocalKey_dieTutorial2,
	kMyLocalKey_dieTutorial3,
	kMyLocalKey_restTime,
	kMyLocalKey_restTimeDay,
	kMyLocalKey_restTimeHour,
	kMyLocalKey_restTimeMinute,
	kMyLocalKey_restTimeSecond,
	kMyLocalKey_warning9,
	kMyLocalKey_warning105,
	kMyLocalKey_warning106,
	kMyLocalKey_warning107,
	kMyLocalKey_warning108,
	kMyLocalKey_warning109,
	kMyLocalKey_warning110,
	kMyLocalKey_warning111,
	kMyLocalKey_warning112,
	kMyLocalKey_warning113,
	kMyLocalKey_warning1001,
	kMyLocalKey_warning1002,
	kMyLocalKey_warning1003,
	kMyLocalKey_warning1004,
	kMyLocalKey_warning1005,
	kMyLocalKey_warning1006,
	kMyLocalKey_warning1007,
	kMyLocalKey_warning1008,
	kMyLocalKey_warning1009,
	kMyLocalKey_warning1010,
	kMyLocalKey_warning1011,
	kMyLocalKey_warning1012,
	kMyLocalKey_warning1013,
	kMyLocalKey_warning1014,
	kMyLocalKey_warning1015,
	kMyLocalKey_warning1016,
	kMyLocalKey_warning1017,
	kMyLocalKey_warning1018,
	kMyLocalKey_warning1019,
	kMyLocalKey_warning1020,
	kMyLocalKey_warningBossSuccess,
	kMyLocalKey_warningLastLife,
	kMyLocalKey_newBossPattern,
	kMyLocalKey_patternContent9,
	kMyLocalKey_patternContent105,
	kMyLocalKey_patternContent106,
	kMyLocalKey_patternContent107,
	kMyLocalKey_patternContent108,
	kMyLocalKey_patternContent109,
	kMyLocalKey_patternContent110,
	kMyLocalKey_patternContent111,
	kMyLocalKey_patternContent112,
	kMyLocalKey_patternContent113,
	kMyLocalKey_patternContent1001,
	kMyLocalKey_patternContent1002,
	kMyLocalKey_patternContent1003,
	kMyLocalKey_patternContent1004,
	kMyLocalKey_patternContent1005,
	kMyLocalKey_patternContent1006,
	kMyLocalKey_patternContent1007,
	kMyLocalKey_patternContent1008,
	kMyLocalKey_patternContent1009,
	kMyLocalKey_patternContent1010,
	kMyLocalKey_patternContent1011,
	kMyLocalKey_patternContent1012,
	kMyLocalKey_patternContent1013,
	kMyLocalKey_patternContent1014,
	kMyLocalKey_patternContent1015,
	kMyLocalKey_patternContent1016,
	kMyLocalKey_patternContent1017,
	kMyLocalKey_patternContent1018,
	kMyLocalKey_patternContent1019,
	kMyLocalKey_patternContent1020,
	kMyLocalKey_controlTip_lineTangle_title,
	kMyLocalKey_controlTip_lineTangle_content,
	kMyLocalKey_controlTip_slimLine_title,
	kMyLocalKey_controlTip_slimLine_content,
	kMyLocalKey_missionCondition0,
	kMyLocalKey_missionCondition1,
	kMyLocalKey_missionCondition2,
	kMyLocalKey_missionCondition3,
	kMyLocalKey_missionCondition4,
	kMyLocalKey_missionCondition5,
	kMyLocalKey_missionCondition6,
	kMyLocalKey_missionCondition7,
	kMyLocalKey_endlessMode,
	kMyLocalKey_endlessWeeklyranking,
	kMyLocalKey_endlessInfoScore,
	kMyLocalKey_endlessInfoScoreValue2,
	kMyLocalKey_endlessHighScore,
	kMyLocalKey_endlessHighStraight,
	kMyLocalKey_endlessHighStraightValue1,
	kMyLocalKey_endlessHighStraightValue2,
	kMyLocalKey_endlessServerError,
	kMyLocalKey_endlessDownloadFail,
	kMyLocalKey_attendanceSpecialTitle,
	kMyLocalKey_attendanceSpecialContent,
	kMyLocalKey_attendanceSpecialGoodsTypeGold,
	kMyLocalKey_attendanceSpecialGoodsTypeRuby,
	kMyLocalKey_attendanceSpecialGoodsTypeItem9,
	kMyLocalKey_attendanceSpecialGoodsTypeItem6,
	kMyLocalKey_attendanceSpecialGoodsTypeItem11,
	kMyLocalKey_attendanceSpecialGoodsTypePass1,
	kMyLocalKey_attendanceSpecialGoodsTypePass2,
	kMyLocalKey_attendanceSpecialGoodsTypePass3,
	kMyLocalKey_attendanceSpecialGoodsTypePass4,
	kMyLocalKey_attendanceSpecialGoodsTypePass5,
	kMyLocalKey_attendanceSpecialGoodsTypePass6,
	kMyLocalKey_attendanceSpecialGoodsTypeMany,
	kMyLocalKey_attendanceGoodsTypeGold,
	kMyLocalKey_attendanceGoodsTypeRuby,
	kMyLocalKey_attendanceGoodsTypeItem9,
	kMyLocalKey_attendanceGoodsTypeItem6,
	kMyLocalKey_attendanceGoodsTypeItem11,
	kMyLocalKey_attendanceGoodsTypePass1,
	kMyLocalKey_attendanceGoodsTypePass2,
	kMyLocalKey_attendanceGoodsTypePass3,
	kMyLocalKey_attendanceGoodsTypePass4,
	kMyLocalKey_attendanceGoodsTypePass5,
	kMyLocalKey_attendanceGoodsTypePass6,
	kMyLocalKey_attendanceGoodsTypePass7,
	kMyLocalKey_attendanceGoodsTypePass8,
	kMyLocalKey_attendanceGoodsTypeMany,
	kMyLocalKey_attendanceTitle,
	kMyLocalKey_attendanceContent,
	kMyLocalKey_attendanceSeqTitle,
	kMyLocalKey_attendanceDay,
	kMyLocalKey_buyMorphingTitle,
	kMyLocalKey_buyMorphingContent,
	kMyLocalKey_buyMorphingTake,
	kMyLocalKey_buyMorphingTakeValue,
	kMyLocalKey_buyMorphingBuy,
	kMyLocalKey_notEnoughtMorphingTitle,
	kMyLocalKey_notEnoughtMorphingContent,
	kMyLocalKey_couponTitle,
	kMyLocalKey_couponContent,
	kMyLocalKey_couponSuccess,
	kMyLocalKey_couponGiftbox,
	kMyLocalKey_couponFail,
	kMyLocalKey_couponAlready,
	kMyLocalKey_couponExpire,
	kMyLocalKey_couponOserror,
	kMyLocalKey_couponDontfind,
	kMyLocalKey_couponOtherError,
	kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle,
	kMyLocalKey_notOpenPuzzleNotEnoughtStarContent,
	kMyLocalKey_endlessStartSettingTitle,
	kMyLocalKey_endlessCalcTitleAreaScore,
	kMyLocalKey_endlessCalcTitleDamageScore,
	kMyLocalKey_endlessCalcTitleComboScore,
	kMyLocalKey_endlessCalcTitleLifeScore,
	kMyLocalKey_endlessCalcTitleTimeScore,
	kMyLocalKey_endlessCalcTitleGradeScore,
	kMyLocalKey_endlessCalcTitleTakeGold,
	kMyLocalKey_endlessCalcTitleTakeArea,
	kMyLocalKey_endlessCalcTitleDamagedScore,
	kMyLocalKey_win,
	kMyLocalKey_gameResult,
	kMyLocalKey_fail,
	kMyLocalKey_achieve,
	kMyLocalKey_close,
	kMyLocalKey_endlessIngWin,
	kMyLocalKey_endlessOpenConditionTitle,
	kMyLocalKey_endlessOpenConditionContent,
	kMyLocalKey_endlessKeepWin,
	kMyLocalKey_endlessKeepWinTitle,
	kMyLocalKey_endlessKeepWinContent,
	kMyLocalKey_endlessReadyIngWin,
	kMyLocalKey_nicknameError,
	kMyLocalKey_onePercentDesc2,
	kMyLocalKey_rivalSearch,
	kMyLocalKey_rivalLevel,
	kMyLocalKey_rivalIngWin,
	kMyLocalKey_rivalSelect,
	kMyLocalKey_achieveSuccess2,
	kMyLocalKey_titleTempScript1,
	kMyLocalKey_titleTempScript2,
	kMyLocalKey_titleTempScript3,
	kMyLocalKey_heartFree,
	kMyLocalKey_rankRewardTitle,
	kMyLocalKey_rankRewardContent,
	kMyLocalKey_rankRewardStageTitle,
	kMyLocalKey_rankRewardStageScore,
	kMyLocalKey_rankRewardStageRank,
	kMyLocalKey_rankRewardStagePlayCount,
	kMyLocalKey_rankRewardEndlessTitle,
	kMyLocalKey_rankRewardEndlessScore,
	kMyLocalKey_rankRewardEndlessRank,
	kMyLocalKey_rankRewardEndlessVictory,
	kMyLocalKey_rankRewardStageReward,
	kMyLocalKey_rankRewardEndlessReward,
	kMyLocalKey_rankRewardStageRankValue,
	kMyLocalKey_rankRewardEndlessRankValue,
	kMyLocalKey_rankRewardEndlessVictoryScript,
	kMyLocalKey_rankRewardRankValue,
	kMyLocalKey_rankRewardGoodsTypeGold,
	kMyLocalKey_rankRewardGoodsTypeRuby,
	kMyLocalKey_rankRewardGoodsTypeItem9,
	kMyLocalKey_rankRewardGoodsTypeItem6,
	kMyLocalKey_rankRewardGoodsTypeItem11,
	kMyLocalKey_rankRewardGoodsTypePass1,
	kMyLocalKey_rankRewardGoodsTypePass2,
	kMyLocalKey_rankRewardGoodsTypePass3,
	kMyLocalKey_rankRewardGoodsTypePass4,
	kMyLocalKey_rankRewardGoodsTypePass5,
	kMyLocalKey_rankRewardGoodsTypePass6,
	kMyLocalKey_rankRewardGoodsTypePass7,
	kMyLocalKey_rankRewardGoodsTypePass8,
	kMyLocalKey_rankRewardGoodsTypeMany,
	kMyLocalKey_rankUpEventTitle,
	kMyLocalKey_rankUpEventContent,
	kMyLocalKey_no,
	kMyLocalKey_yes,
	kMyLocalKey_clearTimeEventMent,
	kMyLocalKey_clearTimeEventTitle,
	kMyLocalKey_achieveHiddenTitle,
	kMyLocalKey_achieveHiddenContent,
	kMyLocalKey_mainflowDimmed1,
	kMyLocalKey_mainflowDimmed2,
	kMyLocalKey_mainflowDimmed3,
	kMyLocalKey_mainflowDimmed4,
	kMyLocalKey_mainflowDimmed5,
	kMyLocalKey_mainflowDimmed6,
	kMyLocalKey_mainflowDimmed7,
	kMyLocalKey_mainflowDimmed8,
	kMyLocalKey_mainflowDimmed9,
	kMyLocalKey_mainflowDimmed10,
	kMyLocalKey_puzzleClearCardTitle,
	kMyLocalKey_puzzleClearCardContent,
	kMyLocalKey_puzzlePerfectCardTitle,
	kMyLocalKey_puzzlePerfectCardContent,
	kMyLocalKey_endlessSeqWinRewardTitle,
	kMyLocalKey_endlessSeqWinRewardContent,
	kMyLocalKey_endlessSeqWinRewardGoodsTypeGold,
	kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby,
	kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9,
	kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6,
	kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass1,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass2,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass3,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass4,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass5,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass6,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass7,
	kMyLocalKey_endlessSeqWinRewardGoodsTypePass8,
	kMyLocalKey_endlessSeqWinRewardGoodsTypeMany,
	kMyLocalKey_cardSettingMyCardContent,
	kMyLocalKey_cardSettingMyCardToSpecial,
	kMyLocalKey_cardSettingClearCardMent,
	kMyLocalKey_cardSettingPerfectCardMent,
	kMyLocalKey_cardSettingSpecialCardTitle,
	kMyLocalKey_cardSettingSpecialCardContent,
	kMyLocalKey_cardSettingSpecialCardToMyCard,
	kMyLocalKey_endlessReady,
	kMyLocalKey_endlessTutorialMent1,
	kMyLocalKey_endlessTutorialMent2,
	kMyLocalKey_endlessTutorialMent3,
	kMyLocalKey_endlessTutorialMent4,
	kMyLocalKey_endlessTutorialMent5,
	kMyLocalKey_endlessTutorialMent6,
	kMyLocalKey_endlessTutorialMent7,
	kMyLocalKey_endlessTutorialMent8,
	kMyLocalKey_endlessTutorialMent9,
	kMyLocalKey_endlessTutorialMent10,
	kMyLocalKey_endlessTutorialMent11,
	kMyLocalKey_endlessTutorialMent12,
	kMyLocalKey_endlessTutorialMent13,
	kMyLocalKey_endlessTutorialMent14,
	kMyLocalKey_endlessTutorialMent15,
	kMyLocalKey_endlessTutorialMent16,
	kMyLocalKey_endlessTutorialMent17,
	kMyLocalKey_endlessTutorialMent18,
	kMyLocalKey_puzzleValue,
	kMyLocalKey_onePercentTutorial1,
	kMyLocalKey_onePercentTutorial2,
	kMyLocalKey_onePercentTutorial3,
	kMyLocalKey_onePercentTutorial4,
	kMyLocalKey_onePercentGame1,
	kMyLocalKey_onePercentGame2,
	kMyLocalKey_onePercentGame3,
	kMyLocalKey_onePercentGame4,
	kMyLocalKey_onePercentGame5,
	kMyLocalKey_onePercentGame6,
	kMyLocalKey_onePercentGame7,
	kMyLocalKey_onePercentGame8,
	kMyLocalKey_onePercentGame9,
	kMyLocalKey_onePercentGame10,
	kMyLocalKey_endlessRewardInfo,
	kMyLocalKey_gababoContent1,
	kMyLocalKey_gababoContent2,
	kMyLocalKey_gababoContent3,
	kMyLocalKey_gababoContent4,
	kMyLocalKey_gababoContent5,
	kMyLocalKey_gababoContent6,
	kMyLocalKey_gababoContent7,
	kMyLocalKey_gababoContent8,
	kMyLocalKey_gababoContent9,
	kMyLocalKey_gababoContent10,
	kMyLocalKey_gababoContent11,
	kMyLocalKey_gababoContent12,
	kMyLocalKey_gababoContent13,
	kMyLocalKey_gababoContent14,
	kMyLocalKey_gababoContent15,
	kMyLocalKey_gababoContent16,
	kMyLocalKey_gababoContent17,
	kMyLocalKey_gababoReward,
	kMyLocalKey_notGohomeEndlessTutorialMent,
	kMyLocalKey_notReplayEndlessMent,
	kMyLocalKey_optionSecretTitle,
	kMyLocalKey_optionSecretOn,
	kMyLocalKey_optionSecretOff,
	kMyLocalKey_cardGiftTitle,
	kMyLocalKey_cardGiftMyID,
	kMyLocalKey_cardGiftInputText,
	kMyLocalKey_cardGiftSearch,
	kMyLocalKey_cardGiftSend,
	kMyLocalKey_cardGiftNotFound,
	kMyLocalKey_rankUpFailRateUp,
	kMyLocalKey_rankUpEventSubMent,
	kMyLocalKey_failTitleTimeover,
	kMyLocalKey_failTitleMissionfail,
	kMyLocalKey_failTitleGameover,
	kMyLocalKey_clearTitle,
	kMyLocalKey_allPuzzleClearTitle,
	kMyLocalKey_allPuzzleClearMent,
	kMyLocalKey_warningDie,
	kMyLocalKey_couponMent,
	kMyLocalKey_upgradeLevelValue2,
	kMyLocalKey_nickTitle,
	kMyLocalKey_plusGive,
	kMyLocalKey_pvpInfo,
	kMyLocalKey_pvpReward,
	kMyLocalKey_choice,
	kMyLocalKey_stageImgLoadFail,
	kMyLocalKey_cardGiftSuccessTitle,
	kMyLocalKey_cardGiftSuccessContent,
	kMyLocalKey_profile,
	kMyLocalKey_cardElemental,
	kMyLocalKey_touch,
	kMyLocalKey_sound,
	kMyLocalKey_elementalValue,
	kMyLocalKey_myCardCount,
	kMyLocalKey_diaryView,
	kMyLocalKey_cardTakeCnt,
	kMyLocalKey_buttonTutorialMent,
	kMyLocalKey_nickBottomMent,
	kMyLocalKey_pleaseInputID,
	kMyLocalKey_invalidID,
	kMyLocalKey_tutorialNextButton,
	kMyLocalKey_puzzleDimmed1,
	kMyLocalKey_puzzleDimmed2,
	kMyLocalKey_puzzleDimmed3,
	kMyLocalKey_puzzleDimmed4,
	kMyLocalKey_stageSettingDimmed1,
	kMyLocalKey_stageSettingDimmed2,
	kMyLocalKey_noReview,
	kMyLocalKey_gem100,
	kMyLocalKey_endlessOpeningMarquee1,
	kMyLocalKey_endlessOpeningMarquee2,
	kMyLocalKey_endlessOpeningMarquee3,
	kMyLocalKey_endlessOpeningMarquee4,
	kMyLocalKey_endlessOpeningMarquee5,
	kMyLocalKey_rankNewMarquee1,
	kMyLocalKey_rankNewMarquee2,
	kMyLocalKey_rankNewMarquee3,
	kMyLocalKey_scenarioMent1,
	kMyLocalKey_scenarioMent2,
	kMyLocalKey_scenarioMent3,
	kMyLocalKey_scenarioMent4,
	kMyLocalKey_scenarioMent5,
	kMyLocalKey_scenarioMent6,
	kMyLocalKey_scenarioMent7,
	kMyLocalKey_scenarioMent8,
	kMyLocalKey_scenarioMent9,
	kMyLocalKey_scenarioMent10,
	kMyLocalKey_scenarioMent11,
	kMyLocalKey_scenarioMent12,
	kMyLocalKey_scenarioMent13,
	kMyLocalKey_scenarioMent14,
	kMyLocalKey_scenarioMent15,
	kMyLocalKey_scenarioMent16,
	kMyLocalKey_scenarioMent17,
	kMyLocalKey_scenarioMent18,
	kMyLocalKey_scenarioMent19,
	kMyLocalKey_scenarioMent20,
	kMyLocalKey_scenarioMent21,
	kMyLocalKey_scenarioMent22,
	kMyLocalKey_scenarioMent23,
	kMyLocalKey_scenarioMent24,
	kMyLocalKey_scenarioMent25,
	kMyLocalKey_scenarioMent26,
	kMyLocalKey_scenarioMent27,
	kMyLocalKey_scenarioMent28,
	kMyLocalKey_scenarioMent29,
	kMyLocalKey_scenarioMent30,
	kMyLocalKey_scenarioMent31,
	kMyLocalKey_scenarioMent32,
	kMyLocalKey_scenarioMent33,
	kMyLocalKey_scenarioMent34,
	kMyLocalKey_scenarioMent35,
	kMyLocalKey_scenarioMent36,
	kMyLocalKey_scenarioMent37,
	kMyLocalKey_scenarioMent38,
	kMyLocalKey_scenarioMent39,
	kMyLocalKey_scenarioMent40,
	kMyLocalKey_scenarioMent41,
	kMyLocalKey_scenarioMent42,
	kMyLocalKey_scenarioMent43,
	kMyLocalKey_scenarioMent44,
	kMyLocalKey_scenarioMent45,
	kMyLocalKey_scenarioMent46,
	kMyLocalKey_scenarioMent47,
	kMyLocalKey_scenarioMent48,
	kMyLocalKey_scenarioMent49,
	kMyLocalKey_scenarioMent50,
	kMyLocalKey_scenarioMent51,
	kMyLocalKey_scenarioMent52,
	kMyLocalKey_scenarioMent53,
	kMyLocalKey_scenarioMent54,
	kMyLocalKey_scenarioMent55,
	kMyLocalKey_scenarioMent56,
	kMyLocalKey_scenarioMent57,
	kMyLocalKey_scenarioMent58,
	kMyLocalKey_scenarioMent59,
	kMyLocalKey_scenarioMent60,
	kMyLocalKey_scenarioMent61,
	kMyLocalKey_scenarioMent62,
	kMyLocalKey_kindTutorial1,
	kMyLocalKey_kindTutorial2,
	kMyLocalKey_kindTutorial3,
	kMyLocalKey_kindTutorial4,
	kMyLocalKey_kindTutorial5,
	kMyLocalKey_kindTutorial6,
	kMyLocalKey_kindTutorial7,
	kMyLocalKey_kindTutorial8,
	kMyLocalKey_kindTutorial9,
	kMyLocalKey_kindTutorial10,
	kMyLocalKey_kindTutorial11,
	kMyLocalKey_kindTutorial12,
	kMyLocalKey_kindTutorial13,
	kMyLocalKey_kindTutorial14,
	kMyLocalKey_kindTutorial15,
	kMyLocalKey_kindTutorial16,
	kMyLocalKey_kindTutorial17,
	kMyLocalKey_kindTutorial18,
	kMyLocalKey_kindTutorial19,
	kMyLocalKey_kindTutorial20,
	kMyLocalKey_kindTutorial21,
	kMyLocalKey_kindTutorial22,
	kMyLocalKey_kindTutorial23,
	kMyLocalKey_kindTutorial24,
	kMyLocalKey_kindTutorial25,
	kMyLocalKey_kindTutorial26,
	kMyLocalKey_kindTutorial27,
	kMyLocalKey_kindTutorial28,
	kMyLocalKey_kindTutorial29,
	kMyLocalKey_kindTutorial30,
	kMyLocalKey_kindTutorial31,
	kMyLocalKey_kindTutorial32,
	kMyLocalKey_kindTutorial33,
	kMyLocalKey_kindTutorial34,
	kMyLocalKey_kindTutorial35,
	kMyLocalKey_kindTutorial36,
	kMyLocalKey_kindTutorial37,
	kMyLocalKey_kindTutorial38,
	kMyLocalKey_kindTutorial39,
	kMyLocalKey_maxSpeed,
	kMyLocalKey_showtime,
	kMyLocalKey_miss,
	kMyLocalKey_combo,
	kMyLocalKey_unlockedAreaScroll,
	kMyLocalKey_missionComplete,
	kMyLocalKey_collectChange,
	kMyLocalKey_afterOpenCBT,
	kMyLocalKey_cardChange,
	kMyLocalKey_showtimeMorphingTouch,
	kMyLocalKey_cardStrength,
	kMyLocalKey_updateTitle,
	kMyLocalKey_updateContent,
	kMyLocalKey_communityNotOpenTitle,
	kMyLocalKey_communityNotOpenContent,
	kMyLocalKey_joystickSize,
	kMyLocalKey_useSideDirection,
	kMyLocalKey_toDiary19,
	kMyLocalKey_toDiary19Title,
	kMyLocalKey_toDiary19Content,
	kMyLocalKey_toDiary19Button,
	kMyLocalKey_joystickVib,
	kMyLocalKey_joystickSizeQuestionTitle,
	kMyLocalKey_joystickSizeQuestionContent,
	kMyLocalKey_missionTitle8,
	kMyLocalKey_missionTitle9,
	kMyLocalKey_missionTitle10,
	kMyLocalKey_missionTitle11,
	kMyLocalKey_missionTitle12,
	kMyLocalKey_missionTitle13,
	kMyLocalKey_missionDiscription8,
	kMyLocalKey_missionDiscription9,
	kMyLocalKey_missionDiscription10,
	kMyLocalKey_missionDiscription11,
	kMyLocalKey_missionDiscription12,
	kMyLocalKey_missionDiscription13,
	kMyLocalKey_missionCondition8,
	kMyLocalKey_missionCondition9,
	kMyLocalKey_missionCondition10,
	kMyLocalKey_missionCondition11,
	kMyLocalKey_missionCondition12,
	kMyLocalKey_missionCondition13,
	kMyLocalKey_mission10Label,
	kMyLocalKey_mission11Label,
	kMyLocalKey_mission12Label,
	kMyLocalKey_mission13Label,
	kMyLocalKey_recommenderReg,
	kMyLocalKey_realContinue,
	kMyLocalKey_accountLinkLeadTitle,
	kMyLocalKey_accountLinkLeadContent,
	kMyLocalKey_accountLinkLeadButton,
	kMyLocalKey_introducerTitle,
	kMyLocalKey_introducerContent,
	kMyLocalKey_introducerPlaceHolder,
	kMyLocalKey_introducerOk,
	kMyLocalKey_introducerAfter,
	kMyLocalKey_introducerPleaseInputNick,
	kMyLocalKey_introducerDonFindNick,
	kMyLocalKey_introducerSuccess,
	kMyLocalKey_introducerSelfNotIntroduce,
	kMyLocalKey_p1Shop,
	kMyLocalKey_introducerInputReward,
	kMyLocalKey_introducerCompleteReward,
	kMyLocalKey_onContinueGem,
	kMyLocalKey_onContinueP1,
	kMyLocalKey_eventCard,
	kMyLocalKey_puzzleOpenTitle,
	kMyLocalKey_puzzleOpenContent,
	kMyLocalKey_puzzleOpenButton,
	kMyLocalKey_takeCount,
	kMyLocalKey_joystickPositionSelectTitle,
	kMyLocalKey_joystickPositionSelectContent,
	kMyLocalKey_joystickPositionSelectRecommend,
	kMyLocalKey_missionFailContextCombo,
	kMyLocalKey_missionFailContextGold,
	kMyLocalKey_missionFailContextItemCollect,
	kMyLocalKey_missionFailContextScore,
	kMyLocalKey_missionFailContextSubCumberCatch,
	kMyLocalKey_missionFailContextTurns,
	kMyLocalKey_hellMode,
	kMyLocalKey_hellMarquee,
	kMyLocalKey_hellNotOpen,
	kMyLocalKey_mycharacter,
	kMyLocalKey_missileUpgrade2,
	kMyLocalKey_changeCharacter,
	kMyLocalKey_selectCharacter,
	kMyLocalKey_characterMarquee,
	kMyLocalKey_characterSelected,
	kMyLocalKey_characterSelect,
	kMyLocalKey_takeCharacterCard,
	kMyLocalKey_haveCharacterCard,
	kMyLocalKey_hellTitleGold,
	kMyLocalKey_hellTitleTime,
	kMyLocalKey_hellContentTime,
	kMyLocalKey_hellTakeCharacter,
	kMyLocalKey_failHellMode,
	kMyLocalKey_needHaveCharacterCard,
	kMyLocalKey_doNotOpenTodoay,
	kMyInfo,
	kFriendEvent,
	kRequestFriend,
	
	kStory_1_1,
	kStory_1_2,
	kStory_1_3,
	kStory_1_4,
	kStory_1_5,
	kStory_1_6,
	kStory_1_7,
	
	kStory_2_1,
	kStory_2_2,
	kStory_2_3,
	kStory_2_4,
	kStory_2_5,
	kStory_2_6,
	kStory_2_7,
	kStory_2_8,
	kStory_2_9,
	
	kStory_3_1,
	kStory_3_2,
	kStory_3_3,
	kStory_3_4,
	kStory_3_5,
	kStory_3_6,
	kStory_3_7,
	kStory_3_8,
	kStory_3_9,
	kStory_3_10,
	kStory_3_11,
	
	kStory_4_1,
	kStory_4_2,
	kStory_4_3,
	kStory_4_4,
	kStory_4_5,
	
	kStory_5_1,
	kStory_5_2,
	kStory_5_3,
	kStory_5_4,
	kStory_5_5,
	kStory_5_6,
	kStory_5_7,
	kStory_5_8,
	kStory_5_9,
	
	kStory_6_1,
	kStory_6_2,
	kStory_6_3,
	kStory_6_4,
	kStory_6_5,
	kStory_6_6,
	
	kStory_7_1,
	kStory_7_2,
	kStory_7_3,
	kStory_7_4,
	kStory_7_5,
	
	kStory_8_1,
	kStory_8_2,
	kStory_8_3,
	kStory_8_4,
	kStory_8_5,
	kStory_8_6,
	kStory_8_7,
	kStory_8_8,
	
	kStory_9_1,
	kStory_9_2,
	kStory_9_3,
	kStory_9_4,
	kStory_9_5,
	kStory_9_6,
	kStory_9_7,
	
	kStory_10_1,
	kStory_10_2,
	kStory_10_3,
	kStory_10_4,
	kStory_10_5,
	
	kStory_11_1,
	kStory_11_2,
	kStory_11_3,
	
	kStory_12_1,
	kStory_12_2,
	kStory_12_3,
	kStory_12_4,
	
	kStory_13_1,
	kStory_13_2,
	kStory_13_3,
	kStory_13_4,
	kStory_13_5,
	kStory_13_6,
	
	kStory_14_1,
	kStory_14_2,
	kStory_14_3,
	kStory_14_4,
	
	kStory_15_1,
	kStory_15_2,
	kStory_15_3,
	kStory_15_4,
	
	kStory_16_1,
	kStory_16_2,
	kStory_16_3,
	
	kStory_17_1,
	kStory_17_2,
	kStory_17_3,
	kStory_17_4,
	
	kStory_18_1,
	kStory_18_2,
	kStory_18_3,
	kStory_18_4,
	kStory_18_5,
	
	kStory_19_1,
	kStory_19_2,
	
	kStory_20_1,
	kStory_20_2,
	kFriendSelfNickname,
	kAttendanceCheck,
	kTutorialReplay,
	kMyLocalKey_frameOpenConditionContentCard,
	kMyLocalKey_frameOpenConditionContentCardGold,
	kMyLocalKey_detailConditionPopupTitleCard,
	kMyLocalKey_detailConditionPopupContentCard,
	kMyLocalKey_notOpenPuzzleNotEnoughtCardContent,
	kMyLocalKey_gachaListAt4HourTimeAutoRefresh,
	kMyLocalKey_goodLuckAyameaFighting,
	kMyLocalKey_brotherJustOne,
	kMyLocalKey_goodsTouchDetailScript,
	kMyLocalKey_listRefreshRestTime,
	kMyLocalKey_nowRefresh,
	kMyLocalKey_normalGacha,
	kMyLocalKey_premiumGacha,
	kMyLocalKey_congratulation,
	kMyLocalKey_characterName,
	
	kMyLocalKey_gemTake,
	kMyLocalKey_goldTake,
	kMyLocalKey_heartTake,
	kMyLocalKey_doubleItemTake,
	kMyLocalKey_baseSpeedUpItemTake,
	kMyLocalKey_magneticItemTake,
	kMyLocalKey_p1Take,
	kMyLocalKey_p2Take,
	kMyLocalKey_p3Take,
	kMyLocalKey_p4Take,
	kMyLocalKey_p5Take,
	kMyLocalKey_p6Take,
	kMyLocalKey_p7Take,
	kMyLocalKey_p8Take,
	
	kMyLocalKey_gemCount,
	kMyLocalKey_goldCount,
	kMyLocalKey_propertyCount,
	kMyLocalKey_nGradeCard,
	kMyLocalKey_cardTake,
	kMyLocalKey_goodsScript,
	
	kMyLocalKey_gem,
	kMyLocalKey_gold,
	kMyLocalKey_heart,
	kMyLocalKey_doubleItem,
	kMyLocalKey_baseSpeedUpItem,
	kMyLocalKey_magneticItem,
	kMyLocalKey_p1,
	kMyLocalKey_p2,
	kMyLocalKey_p3,
	kMyLocalKey_p4,
	kMyLocalKey_p5,
	kMyLocalKey_p6,
	kMyLocalKey_p7,
	kMyLocalKey_p8,
	
	kMyLocalKey_controlTip_100percent_title,
	kMyLocalKey_controlTip_100percent_content,
	
	kMyLocalKey_expTake,
	kMyLocalKey_expN_M,
	kMyLocalKey_equip,
	
	kMyLocalKey_magnetEffect,
	kMyLocalKey_missileCreateValue,
	kMyLocalKey_cancelSpeed,
	kMyLocalKey_goldBonus,
	kMyLocalKey_scoreBonus,
	kMyLocalKey_shockRegi,
	kMyLocalKey_missileFireRegi,
	kMyLocalKey_nowRefreshNeedGem
};
class KsLocal
{
	
public:
	
	static KsLocal* sharedInstance();
	
	~KsLocal()
	{
		
	}
	
	const char* getLocalForKey(LK key);
	const char* getSupportLocalCode();
	std::string getLocalCode();
private:
	KsLocal()
	{
		languageType = "";
		this->initLocal();
	}
	void initLocal();
	void setLocal();
	void ko();
	void en();
	void ja();
	std::string languageType;
	std::map<std::string, std::map<LK, std::string>> m_mapper;
};



#endif
