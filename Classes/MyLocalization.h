#ifndef EscapeTheSpace_MyLocalization_h
#define EscapeTheSpace_MyLocalization_h

#include "cocos2d.h"
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

using namespace cocos2d;
using namespace std;
/*
 
 myLoc->getLocalForKey(kMyLocalKey_tutorial7)
 
 */


typedef enum MyLocalKey_ {
	kMyLocalKey_canNotConnectedServer = 1,
	kMyLocalKey_touchPlease,
	kMyLocalKey_tutorial1,
	kMyLocalKey_tutorial2,
	kMyLocalKey_tutorial3,
	kMyLocalKey_tutorial4,
	kMyLocalKey_tutorial5,
	kMyLocalKey_tutorial6,
	kMyLocalKey_tutorial7,
	kMyLocalKey_tutorial8,
	kMyLocalKey_tutorial9,
	kMyLocalKey_tutorial10,
	kMyLocalKey_tutorial11,
	kMyLocalKey_tutorial12,
	kMyLocalKey_tutorial13,
	kMyLocalKey_tutorial14,
	kMyLocalKey_tutorial15,
	kMyLocalKey_tutorial16,
	kMyLocalKey_tutorial17,
	kMyLocalKey_tutorial17_1,
	kMyLocalKey_tutorial18,
	kMyLocalKey_tutorial19,
	kMyLocalKey_tutorial20,
	kMyLocalKey_tutorial21,
	kMyLocalKey_tutorial22,
	kMyLocalKey_tutorial23,
	kMyLocalKey_tutorial24,
	kMyLocalKey_tutorial25,
	kMyLocalKey_tutorial26,
	kMyLocalKey_loading1,
	kMyLocalKey_loading2,
	kMyLocalKey_loading3,
	kMyLocalKey_loading4,
	kMyLocalKey_loading5,
	kMyLocalKey_loading6,
	kMyLocalKey_loading11,
	kMyLocalKey_loading12,
	kMyLocalKey_loading13,
	kMyLocalKey_loading14,
	kMyLocalKey_loading15,
	kMyLocalKey_loading16,
	kMyLocalKey_loading17,
	kMyLocalKey_loading18,
	kMyLocalKey_loading19,
	kMyLocalKey_loading20,
	kMyLocalKey_loading21,
	kMyLocalKey_loading22,
	kMyLocalKey_loading23,
	kMyLocalKey_loading24,
	kMyLocalKey_loading25,
	kMyLocalKey_notSelectedItem,
	kMyLocalKey_boughtItem,
	kMyLocalKey_multiSelect,
	kMyLocalKey_notEnoughGold,
	kMyLocalKey_goShop,
	kMyLocalKey_notEnoughBrush,
	kMyLocalKey_notEnoughRuby,
	kMyLocalKey_review,
	kMyLocalKey_regiGallery,
	kMyLocalKey_openHard,
	kMyLocalKey_shieldCancel,
	kMyLocalKey_item1,
	kMyLocalKey_item2,
	kMyLocalKey_item3,
	kMyLocalKey_item4,
	kMyLocalKey_item5,
	kMyLocalKey_item6,
	kMyLocalKey_exeAppReward,
	kMyLocalKey_giftAHour,
	kMyLocalKey_giftInvalid,
	kMyLocalKey_giftUsed,
	kMyLocalKey_giftLitqoo,
	kMyLocalKey_goHome,
	kMyLocalKey_star3clear,
	kMyLocalKey_itemEnable,
	kMyLocalKey_needUpdate,
	kMyLocalKey_newVersion,
	kMyLocalKey_facebookInvite,
	kMyLocalKey_facebookWith,
	kMyLocalKey_petEmpty,
	kMyLocalKey_petAttack,
	kMyLocalKey_petGold,
	kMyLocalKey_petFast,
	kMyLocalKey_petSilence,
	kMyLocalKey_petSlow,
	kMyLocalKey_getMonster,
	
	
	kMyLocalKey_exit,
	
	kMyLocalKey_connectingServer,
	kMyLocalKey_inputNick,
	kMyLocalKey_inputPlease,
	kMyLocalKey_ok,
	kMyLocalKey_downImgInfo,
	kMyLocalKey_failLoadInfo,
	kMyLocalKey_replay,
	kMyLocalKey_downloadingProgress,
	kMyLocalKey_downImgFail,
	kMyLocalKey_successLogin,
	kMyLocalKey_sameNick,
	kMyLocalKey_invalidNick,
	kMyLocalKey_longNick,
	kMyLocalKey_shortNick,
	kMyLocalKey_openRightNow,
	kMyLocalKey_openRubyValue,
	kMyLocalKey_openGoldValue,
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
	kMyLocalKey_stageOpenTitle,
	kMyLocalKey_stageOpenContent,
	kMyLocalKey_view,
	kMyLocalKey_condition1,
	kMyLocalKey_condition2,
	kMyLocalKey_condition3,
	kMyLocalKey_condition4,
	kMyLocalKey_conditionTwoLine1,
	kMyLocalKey_conditionTwoLine2,
	kMyLocalKey_conditionTwoLine3,
	kMyLocalKey_conditionTwoLine4,
	kMyLocalKey_stageValue,
	kMyLocalKey_myrankValue,
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
	kMyLocalKey_goToShop,
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
	kMyLocalKey_checkBuy,
	kMyLocalKey_realBuy,
	kMyLocalKey_option,
	kMyLocalKey_regCoupon,
	kMyLocalKey_community,
	kMyLocalKey_gametip,
	kMyLocalKey_tutorial,
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
	kMyLocalKey_blocked,
	kMyLocalKey_blockedMsg,
	kMyLocalKey_noti,
	kMyLocalKey_pushNoti,
	kMyLocalKey_messageNoti,
	kMyLocalKey_giftboxAlert,
	kMyLocalKey_getgift,
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
	kMyLocalKey_giftbox,
	kMyLocalKey_allAccept,
	kMyLocalKey_puzzleOpenTitle,
	kMyLocalKey_puzzleOpenContent,
	kMyLocalKey_ticketBox,
	kMyLocalKey_helpBox,
	kMyLocalKey_challengeBox,
	kMyLocalKey_heartBox,
	kMyLocalKey_allView,
	kMyLocalKey_arriveHeart,
	kMyLocalKey_arriveChallenge,
	kMyLocalKey_checkAcceptChallenge,
	kMyLocalKey_ttt,
	kMyLocalKey_tttContent,
	kMyLocalKey_resultChallenge,
	kMyLocalKey_loseContent,
	kMyLocalKey_arriveHelp,
	kMyLocalKey_acceptHelp,
	kMyLocalKey_deny,
	kMyLocalKey_accept,
	kMyLocalKey_arriveHelped,
	kMyLocalKey_cardInfoLoading,
	kMyLocalKey_iHelped,
	kMyLocalKey_arriveNeedTicket,
	kMyLocalKey_arriveNeedTicketContent,
	kMyLocalKey_send,
	kMyLocalKey_arriveTicket,
	kMyLocalKey_arriveTicketContent,
	kMyLocalKey_takeTicket,
	kMyLocalKey_arriveAddFriend,
	kMyLocalKey_whatError,
	kMyLocalKey_arriveWhatError,
	kMyLocalKey_defaultSort,
	kMyLocalKey_takeOrder,
	kMyLocalKey_gradeOrder,
	kMyLocalKey_rotation,
	kMyLocalKey_upgradeSubMent,
	kMyLocalKey_itemRegacha,
	kMyLocalKey_itemRegachaDiscountValue,
	kMyLocalKey_thisItemUse,
	kMyLocalKey_time,
	kMyLocalKey_gold,
	kMyLocalKey_score,
	kMyLocalKey_totalScore,
	kMyLocalKey_nextStage,
	kMyLocalKey_regame,
	kMyLocalKey_toMain,
	kMyLocalKey_beforeNotClearPuzzle,
	kMyLocalKey_waitForUpdate,
	kMyLocalKey_diaryNoImg,
	kMyLocalKey_reward,
	kMyLocalKey_speed,
	kMyLocalKey_ready,
	kMyLocalKey_rankA,
	kMyLocalKey_rankB,
	kMyLocalKey_rankC,
	kMyLocalKey_rankD,
	kMyLocalKey_buy,
	kMyLocalKey_controlClick,
	kMyLocalKey_locked,
	kMyLocalKey_selectClearBox,
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
//	kMyLocalKey_detailConditionPopupContent2,
//	kMyLocalKey_detailConditionPopupContent3,
//	kMyLocalKey_detailConditionPopupContent4,
//	kMyLocalKey_detailConditionPopupContent5,
	kMyLocalKey_failPurchase,
	kMyLocalKey_enoughtGoodsContent,
	kMyLocalKey_giveup,
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
	kMyLocalKey_firstPurchaseMent2,
	kMyLocalKey_firstPurchaseMent3,
	kMyLocalKey_packageItemSale,
	kMyLocalKey_emptyItemSaleMent1,
	kMyLocalKey_emptyItemSaleMent2,
	kMyLocalKey_stupidNpuHelpMent1,
	kMyLocalKey_stupidNpuHelpMent2,
	kMyLocalKey_eventShopTitle,
	kMyLocalKey_eventShopMent1,
	kMyLocalKey_eventShopMent2,
	kMyLocalKey_mileageMent1,
	kMyLocalKey_mileageMent2,
	kMyLocalKey_mileageMent3,
	kMyLocalKey_levelupGuideMent1,
	kMyLocalKey_levelupGuideMent2,
	kMyLocalKey_levelupGuideMent3,
	kMyLocalKey_levelupGuideMent4,
	kMyLocalKey_next,
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
	kMyLocalKey_todaymissionSuccess,
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
	kMyLocalKey_todaymissionSuccessPass1,
	kMyLocalKey_todaymissionSuccessPass2,
	kMyLocalKey_todaymissionSuccessPass3,
	kMyLocalKey_todaymissionSuccessPass4,
	kMyLocalKey_todaymissionSuccessPass5,
	kMyLocalKey_todaymissionSuccessCommon,
	kMyLocalKey_free,
	kMyLocalKey_areaReGacha,
	kMyLocalKey_pleaseClickButton,
	kMyLocalKey_stop,
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
	kMyLocalKey_controlTip_lineTangle_title,
	kMyLocalKey_controlTip_lineTangle_content,
	kMyLocalKey_controlTip_slimLine_title,
	kMyLocalKey_controlTip_slimLine_content,
	kMyLocalKey_mission,
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
	kMyLocalKey_endlessMyInfo,
	kMyLocalKey_endlessInfoScore,
	kMyLocalKey_endlessInfoScoreValue,
	kMyLocalKey_endlessInfoScoreValue2,
	kMyLocalKey_endlessHighScore,
	kMyLocalKey_endlessHighStraight,
	kMyLocalKey_endlessHighStraightValue1,
	kMyLocalKey_endlessHighStraightValue2,
	kMyLocalKey_endlessServerError,
	kMyLocalKey_endlessDownloadFail,
	kMyLocalKey_endlessCheckGiveup,
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
	kMyLocalKey_endlessSeqWinRewardGoodsTypeMany,
	
	kMyLocalKey_cardSettingMyCardContent,
	kMyLocalKey_cardSettingMyCardToSpecial,
	kMyLocalKey_cardSettingClearCardMent,
	kMyLocalKey_cardSettingPerfectCardMent,
	
	kMyLocalKey_cardSettingSpecialCardTitle,
	kMyLocalKey_cardSettingSpecialCardContent,
	kMyLocalKey_cardSettingSpecialCardToMyCard,

	kMyLocalKey_endlessReady
}MyLocalKey;





#define myLoc MyLocal::sharedInstance()

class MyLocal : public CCDictionary
{
	
public:
	
	static MyLocal* sharedInstance();
	
	~MyLocal()
	{
		
	}
	
	const char* getLocalForKey(MyLocalKey key);
	const char* getSupportLocalCode();
	CCString* getLocalCode();
	
private:
	MyLocal() : CCDictionary()
	{
		languageType = NULL;
		this->initLocal();
	}
	void initLocal();
	void setLocal();
	void ko();
	void en();
	void ja();
	CCString* languageType;
};



#endif
