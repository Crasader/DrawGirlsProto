//
//  EnumDefine.h
//  DrawingJack
//
//  Created by 사원3 on 13. 2. 4..
//
//

#ifndef DrawingJack_EnumDefine_h
#define DrawingJack_EnumDefine_h

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
	kWMS_MTB_stage = 1000
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

typedef enum t_ShopCode{
	kShopCode_ruby = 1,
	kShopCode_gold,
	kShopCode_brush
}ShopCode;

typedef enum t_CLEAR_CONDITION{
	kCLEAR_default = 0,
	kCLEAR_bossLifeZero,
	kCLEAR_subCumberCatch,
	kCLEAR_bigArea,
	kCLEAR_itemCollect,
	kCLEAR_perfect,
	kCLEAR_sequenceChange,
	kCLEAR_timeLimit
}CLEAR_CONDITION;

typedef enum t_ITEM_CODE{
	kIC_attack = 0,
	kIC_speedUp,
	kIC_addTime,
	kIC_fast,
	kIC_critical,
	kIC_subOneDie,
	kIC_doubleItem,
	kIC_silence,
	kIC_timeStop,
	kIC_subNothing,
	kIC_longTime,
	kIC_bossLittleEnergy,
	kIC_subSmallSize,
	kIC_smallArea,
	kIC_widePerfect,
	kIC_randomChange,
	kIC_removeMission
}ITEM_CODE;

#define SHOP_KEY	0xD9

#endif
