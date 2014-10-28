//
//  EnumDefine.h
//  DrawingJack
//
//  Created by 사원3 on 13. 2. 4..
//
//

#ifndef DrawingJack_EnumDefine_h
#define DrawingJack_EnumDefine_h

enum mapLoopRange{
	mapWidthOutlineBegin = 0, // 실제 맵이 아니고 맵 가장자리에 임시로 틀 하나 놔둔거지
	mapWidthOutlineEnd = 162, // 마찬가지 실제 맵이 아님
	mapHeightOutlineBegin = 0, // 실제 맵이 아님
	mapHeightOutlineEnd = 217, // 실제 맵이 아님
	// 이 까지는 루프를 돌 때 신경 ㄴ ㄴ
	
	mapWidthInnerBegin = 1, // 실제 공간
	mapWidthInnerEnd = 161, // 실제 공간
	mapHeightInnerBegin = 1, // 실제 공간
	mapHeightInnerEnd = 216, // 실제 공간
};

enum DieType{
	kDieType_other = 0,
	kDieType_shockwave,
	kDieType_missileToLine,
	kDieType_timeover
};

typedef enum t_ElementCode{
	kElementCode_empty = 0,
	kElementCode_life,
	kElementCode_fire,
	kElementCode_water,
	kElementCode_wind,
	kElementCode_lightning,
	kElementCode_plasma
}ElementCode;

typedef enum tIntDirection{
	directionStop = 0,
	directionLeftUp,		// 1
	directionLeft,			// 2
	directionLeftDown,		// 3
	directionDown,			// 4
	directionRightDown,		// 5
	directionRight,			// 6
	directionRightUp,		// 7
	directionUp				// 8
}IntDirection;

typedef enum t_PetRank{
	kPetRank_W = 5,
	kPetRank_D = 4,
	kPetRank_C = 3,
	kPetRank_B = 2,
	kPetRank_A = 1,
	kPetRank_S = 0
}PetRank;

typedef enum t_PetCode{
	kPetCode_empty_ladybug = 0,	// w rank
	
	kPetCode_attack_genie,		// d rank
	kPetCode_silence_pierrot,
	kPetCode_fast_fairy,
	kPetCode_slow_mud,
	kPetCode_gold_bird,
	
	kPetCode_attack_hedgehog,	// c rank
	kPetCode_silence_eggbird,
	kPetCode_fast_peacock,
	kPetCode_slow_racat, // racoon + cat
	kPetCode_gold_frog,
	
	kPetCode_fast_beefly = 3000 // event rank
}PetCode;

typedef enum t_PSPL_Type{
	kPSPL_Type_addslot = 1,
	kPSPL_Type_buypet,
	kPSPL_Type_delpet
}PSPL_Type;

typedef enum t_ControlType{
	kControlType_unsetted = 0,
	kControlType_gesture,
	kControlType_button,
	kControlType_joystick,
	kControlType_joystick_button
}ControlType;

typedef enum t_cumberState{
	cumberStateStop = 0,
	cumberStateMoving,		// 1
	cumberStateAttackReady,	// 2
	cumberStateAttack,		// 3
	cumberStateCrashReady,	// 4
	cumberStateCrash		// 5
}cumberState;

typedef enum tCollectionStarterType{
	kCST_basic = 0,
	kCST_gallery_target,
	kCST_pet,
	kCST_monster
}CollectionStarterType;

typedef enum tWMS_MenuTagBase{
	kWMS_MTB_ui = 0,
	kWMS_MTB_stage = 10000
}WMS_MenuTagBase;

enum MainUI_MenuTag{
	kMUI_MT_option = 1,
	kMUI_MT_friend,
	kMUI_MT_info,
//	kMUI_MT_elemental,
//	kMUI_MT_pet,
	kMUI_MT_rubyShop,
	kMUI_MT_goldShop,
	kMUI_MT_brushShop,
	kMUI_MT_collection
};

enum MUI_SHOP_MenuTag{
	kMUI_SHOP_MT_rubyTab = 1,
	kMUI_SHOP_MT_goldTab,
	kMUI_SHOP_MT_brushTab,
	kMUI_SHOP_MT_close,
	kMUI_SHOP_MT_ruby10,
	kMUI_SHOP_MT_ruby20,
	kMUI_SHOP_MT_ruby30,
	kMUI_SHOP_MT_ruby50,
	kMUI_SHOP_MT_ruby100,
	kMUI_SHOP_MT_gold15000,
	kMUI_SHOP_MT_gold37000,
	kMUI_SHOP_MT_gold60000,
	kMUI_SHOP_MT_gold120000,
	kMUI_SHOP_MT_gold300000,
	kMUI_SHOP_MT_brush1,
	kMUI_SHOP_MT_brush5,
	kMUI_SHOP_MT_brush20,
	kMUI_SHOP_MT_brush100,
	kMUI_SHOP_MT_brush500,
};

enum MUI_COLLECTION_MenuTag{
	kMUI_COLLECTION_MT_close = 0,
	kMUI_COLLECTION_MT_galleryTab,
	kMUI_COLLECTION_MT_petTab,
	kMUI_COLLECTION_MT_monsterTab
};

enum MUI_OPTION_MenuTag{
	kMUI_OPTION_MT_close = 0,
	kMUI_OPTION_MT_controlGesture,
	kMUI_OPTION_MT_controlButton,
	kMUI_OPTION_MT_effectOn,
	kMUI_OPTION_MT_effectOff,
	kMUI_OPTION_MT_bgmOn,
	kMUI_OPTION_MT_bgmOff,
	kMUI_OPTION_MT_giftcodeEnter,
	kMUI_OPTION_MT_logout
};

enum MUI_FRIEND_MenuTag{
	kMUI_FRIEND_MT_close = 0,
	kMUI_FRIEND_MT_rankTab,
	kMUI_FRIEND_MT_inviteTab,
	kMUI_FRIEND_MT_postboxTab
};

enum MUI_ELEMENTAL_MenuTag{
	kMUI_ELEMENTAL_MT_close = 0,
	kMUI_ELEMENTAL_MT_empty,
	kMUI_ELEMENTAL_MT_fire,
	kMUI_ELEMENTAL_MT_water,
	kMUI_ELEMENTAL_MT_life,
	kMUI_ELEMENTAL_MT_wind,
	kMUI_ELEMENTAL_MT_lightning,
	kMUI_ELEMENTAL_MT_plasma
};

enum MUI_PET_MenuTag{
	kMUI_PET_MT_close = 0,
	kMUI_PET_MT_buy,
	kMUI_PET_MT_mix,
	kMUI_PET_MT_del
};

//typedef enum t_ShopCode{
//	kShopCode_ruby = 1,
//	kShopCode_gold,
//	kShopCode_brush
//}ShopCode;

typedef enum t_CLEAR_CONDITION{
	kCLEAR_default = 0,
	kCLEAR_bossLifeZero,	// 1
	kCLEAR_subCumberCatch,	// 2
	kCLEAR_bigArea,			// 3
	kCLEAR_itemCollect,		// 4
	kCLEAR_perfect,			// 5
	kCLEAR_sequenceChange,	// 6
	kCLEAR_timeLimit,		// 7
	kCLEAR_hellMode,		// 8
	kCLEAR_percentage,		// 9
	kCLEAR_score,			// 10
	kCLEAR_combo,			// 11
	kCLEAR_gold,			// 12
	kCLEAR_turns			// 13
}CLEAR_CONDITION;

typedef enum t_ITEM_CODE{
	kIC_emptyBegin = 0,
	kIC_attack,		// 1
	kIC_speedUp,	// 2
	kIC_addTime,	// 3
	kIC_fast,		// 4
//	kIC_critical,
	kIC_subOneDie,	// 5
	kIC_doubleItem,	// 6
	kIC_silence,	// 7
//	kIC_subNothing,
	kIC_longTime,	// 8
//	kIC_bossLittleEnergy,
//	kIC_subSmallSize,
//	kIC_smallArea,
//	kIC_widePerfect,
//	kIC_randomChange
	kIC_baseSpeedUp, // 9
	kIC_heartUp,	 // 10
	kIC_magnet,		// 11
	kIC_emptyEnd,
	kIC_itemGacha
}ITEM_CODE;


enum MessageRecvType
{
	kHeart = 1,
	kChallengeRequest,
	kChallengeResult,
	kHelpRequest = 4,
	kHelpResult,
	kTicketRequest,
	kTicketResult = 7,
	kInvite,
	kNews,
	kUnknownFriendRequest,
	kGift
};
#define SHOP_KEY	0xD9

enum AchievementCode
{
	kAchievementCode_base = 0,
	
	kAchievementCode_tutorial,//	초보자 : 튜토리얼다 보기 완							1
	
	kAchievementCode_luckySeven1,//	럭키세븐1 : 한번에 7% 먹기 완							2
	kAchievementCode_luckySeven2,//	럭키세븐2 : 한번에 7.7% 먹기 완						3
	kAchievementCode_luckySeven3,//	럭키세븐3 : 한번에 77.7% 먹기 완						4
	
	kAchievementCode_noFail1,//	실패란없다1 : 5회연속 클리어 완							5
	kAchievementCode_noFail2,//	실패란없다2 : 20회연속클리어 완							6
	kAchievementCode_noFail3,//	실패란없다3 : 50회연속클리어 완							7
	
	kAchievementCode_fastClear1,//	초스피드클리어1 : 30초안에 클리어 완					8
	kAchievementCode_fastClear2,//	초스피드클리어2 : 10초안에 클리어 완					9
	kAchievementCode_fastClear3,//	초스피드클리어3 : 5초안에 클리어 완						10
	
	kAchievementCode_fail1,//	실패는 성공의 어머니1 : 게임실패 10회 완						11
	kAchievementCode_fail2,//	실패는 성공의 어머니2 : 게임실패 50회 완						12
	kAchievementCode_fail3,//	실패는 성공의 어머니3 : 게임실패 200회 완					13
	
	kAchievementCode_changeMania1,//	체인지1 : 10회 체인지 완							14
	kAchievementCode_changeMania2,//	체인지2 : 50회 체인지 완							15
	kAchievementCode_changeMania3,//	체인지3 : 200회 체인지 완						16
	
	kAchievementCode_perfect1,//	대단한성과1 : 100%클리어 10회 완						17
	kAchievementCode_perfect2,//	대단한성과2 : 100%클리어 50회 완						18
	kAchievementCode_perfect3,//	대단한성과3 : 100%클리어 200회 완						19
	
	kAchievementCode_luckyGuy,//	럭키가이 : 시작행운아이템 30회성공 완					20
	
	kAchievementCode_cardCollection1,//	카드컬렉터1: 카드 50장모으기 완					21
	kAchievementCode_cardCollection2,//	카드컬렉터2: 카드 100장모으기 완					22
	kAchievementCode_cardCollection3,//	카드컬렉터3: 카드 500장모으기 완					23
	
	kAchievementCode_cardSet,//	셋트완성 : 한스테이지의 1,2,3,4단계 카드셋트 완성 완			24
	
	kAchievementCode_hunter1,//	사냥꾼1 : 부하몹 100마리 가두어잡기 완						25
	kAchievementCode_hunter2,//	사냥꾼2 : 부하몹 200마리 가두어잡기 완						26
	kAchievementCode_hunter3,//	사냥꾼3 : 부하몹 500마리 가두어잡기 완						27
	
	kAchievementCode_attacker1,//	닥치고공격1 : 한판에 50회공격 성공 완					28
	kAchievementCode_attacker2,//	닥치고공격2 : 한판에 200회공격 성공 완					29
	kAchievementCode_attacker3,//	닥치고공격3 : 한판에 400회공격 성공 완					30
	
	kAchievementCode_comboMania1,//	콤보매니아1 : 50콤보달성 완							31
	kAchievementCode_comboMania2,//	콤보매니아2 : 500콤보달성 완							32
	kAchievementCode_comboMania3,//	콤보매니아3 : 5,000콤보달성 완						33
	
	kAchievementCode_mapGacha1,//	맵가챠맨1 : 맵가챠 20회 돌리기 완						34
	kAchievementCode_mapGacha2,//	맵가챠맨2 : 맵가챠 50회 돌리기 완						35
	kAchievementCode_mapGacha3,//	맵가챠맨3 : 맵가챠 100회 돌리기 완						36
	
	kAchievementCode_scoreHigh1,//	경쟁의신1 : 500,000점 이상 획득 완					37
	kAchievementCode_scoreHigh2,//	경쟁의신2 : 1,000,000점 이상 획득 완					38
	kAchievementCode_scoreHigh3,//	경쟁의신3 : 2,000,000점 이상 획득 완					39
	
	kAchievementCode_gold1,//	골드서민 : 50,000골드 모으기. 완							40
	kAchievementCode_gold2,//	골드부자 : 500,000골드 모으기. 완							41
	kAchievementCode_gold3,//	골드재벌 : 1,000,000골드 모으기. 완						42
	
	kAchievementCode_ruby1,//	젬서민 : 50골드모으기. 완									43
	kAchievementCode_ruby2,//	젬부자 : 300젬모으기. 완									44
	kAchievementCode_ruby3,//	젬재벌 : 1,000젬모으기. 완								45
	
	kAchievementCode_end,//															46
	kAchievementCode_hidden_base,//													47
	
	kAchievementCode_hidden_bigHand1,//	(히든) 욕심쟁이 : 한번에 먹은 영역이 50%이상일때 완	48
	kAchievementCode_hidden_bigHand2,//	(히든) 욕심쟁이2 : 한번에 먹은 영역이 90%이상일때 완	49
	
	kAchievementCode_hidden_holder,//	(히든) 야메떼 : 카드 터치 100회					50
	
	kAchievementCode_hidden_noSound,//	(히든) 사운드따윈필요없어 : 설정에 사운드 끄고 플레이후 클리어 완	51
	
	kAchievementCode_hidden_shopper1,//	(히든) 쇼핑홀릭 : 상점메뉴 100회 진입시. 완			52
	kAchievementCode_hidden_shopper2,//	(히든) 쇼핑홀릭2 : 아이템 50번째 구매 완			53
	kAchievementCode_hidden_shopper3,//	(히든) 쇼핑홀릭3 : 아이템 100번째 구매 완			54
	
	kAchievementCode_hidden_breathtaking1,//	(히든)아슬아슬 : 5초이하로 남기고 클리어 완	55
	kAchievementCode_hidden_breathtaking2,//	(히든)아슬아슬2 : 1초이하로 남기고 클리어 완	56
	
	kAchievementCode_hidden_dieEasy,//	(히든)다이이지 : 게임시작 5초안에 죽기 완				57
	
	kAchievementCode_hidden_speedMania,//	(히든)스피드매니아 : 스피드 max찍기 완			58
	
//	kAchievementCode_dieHard1,//	다이하드1 : 1~3level 에서 한목숨도 안죽고 클리어
//	kAchievementCode_dieHard2,//	다이하드2 : 4~9level 에서 한목숨도 안죽고 클리어
//	kAchievementCode_dieHard3,//	다이하드3 : 10level 이상에서 한목숨도 안죽고 클리어
	
//	kAchievementCode_seqAttendance1,//	개근1 : 7일 연속출첵
//	kAchievementCode_seqAttendance2,//	개근2 : 14일 연속출첵
//	kAchievementCode_seqAttendance3,//	개근3 : 21일 연속출첵
	
//	kAchievementCode_reviewer,//	리뷰어 : 리뷰남겨주기	
	
//	kAchievementCode_hidden_returnee,//	(히든) 오랜만이야 : 일주일만에 접속.
	
	kAchievementCode_hidden_end
};

enum class GachaCategory
{
	kRubyGacha = 1,
	kGoldGacha = 2
};

enum StoneType
{
	kStoneType_guided = 0,
	kStoneType_spread,
	kStoneType_laser,
	kStoneType_mine,
	kStoneType_spirit,
	kStoneType_range,
	kStoneType_global = 6,
	kStoneType_protector
};

enum class Curtain
{
	kCurtain = 999,
	kBonusGame = 990,
	kTouchPriority = -500
};
#endif
