//
//  DataStorageHub.h
//  DrawingJack
//
//  Created by 사원3 on 13. 2. 4..
//
//

#ifndef DrawingJack_DataStorageHub_h
#define DrawingJack_DataStorageHub_h

#include "cocos2d.h"
#include "SaveData.h"

using namespace cocos2d;
using namespace std;

typedef enum t_DSH_Key{
	kDSH_Key_lastSelectedChapter = 0,
	kDSH_Key_isOpendChapter_int1,
	kDSH_Key_chapter_int1_Stage_int2_Rating,
	kDSH_Key_chapter_int1_Stage_int2_Highscore,
	kDSH_Key_chapter_int1_LastSelectedStage,
	kDSH_Key_isOpendChapter_int1_Stage_int2,
	kDSH_Key_elementLevelEmpty,
	kDSH_Key_elementLevelLife,
	kDSH_Key_elementLevelFire,
	kDSH_Key_elementLevelWater,
	kDSH_Key_elementLevelWind,
	kDSH_Key_elementLevelLightning,
	kDSH_Key_elementLevelPlasma,
	kDSH_Key_lastSelectedElement,
	kDSH_Key_savedStar,
	kDSH_Key_savedGold,
	kDSH_Key_notFirstExe,
	kDSH_Key_musicOn,
	kDSH_Key_bgmOff,
	kDSH_Key_effectOff,
	kDSH_Key_serverTime,
	kDSH_Key_heartCnt,
	kDSH_Key_heartTime,
	kDSH_Key_FBMHEldestNumber, // FBMH : Facebook Message History
	kDSH_Key_FBMHId,
	kDSH_Key_FBMHTime,
	kDSH_Key_FBMHAfterLink,
	kDSH_Key_isBrushInf,
	kDSH_Key_isRemoveAD,
	kDSH_Key_isReview,
	kDSH_Key_isFacebookLogined,
	kDSH_Key_lastSelectedPet,
	kDSH_Key_totalSelfPetCount,
	kDSH_Key_selfPetCode_int1,
	kDSH_Key_openSlotCount,
	kDSH_Key_lastSelectedControler,
	kDSH_Key_controlOriginX,
	kDSH_Key_controlOriginY,
	kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2,
	kDSH_Key_hasGottenPet_int1,
	kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2,
	kDSH_Key_checkedNewControlJoystick,
	kDSH_Key_theme_int1_clearednumber,
	kDSH_Key_haveItemCnt_int1,
	kDSH_Key_selectedCard,
	kDSH_Key_cardDurability_int1,
	kDSH_Key_hasGottenCard_int1,
	kDSH_Key_inputTextCard_int1,
	kDSH_Key_cardTakeCnt,
	kDSH_Key_cardSortType,
	kDSH_Key_lastSelectedStage,
	kDSH_Key_isPassCoupon_int1,
	kDSH_Key_uiType,
//	kDSH_Key_isOpenPuzzle_int1_Stage_int2,
	kDSH_Key_puzzleMode,
	kDSH_Key_selectedPuzzleNumber,
	kDSH_Key_allHighScore
}DSH_Key;

#define myDSH DataStorageHub::sharedInstance()

class DataStorageHub : public CCObject
{
public:
	static DataStorageHub* sharedInstance()
	{
		static DataStorageHub* t_dsh = NULL;
		if(t_dsh == NULL)
		{
			t_dsh = new DataStorageHub();
			t_dsh->myInit();
		}
		return t_dsh;
	}
	
	bool getBoolForKey(DSH_Key t_key)
	{
//		if(t_key == kDSH_Key_isRemoveAD)
//			return true;
//		if(t_key == kDSH_Key_isBrushInf)
//			return true;
//		else
//		{
			string bool_string = myDefault->getValue(kSDF_myDSH, getKey(t_key), "false");
			if(bool_string == "false")		return false;
			else							return true;
//		}
	}
	bool getBoolForKey(DSH_Key t_key, int key_val1)
	{
		string bool_string = myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), "false");
		if(bool_string == "false")		return false;
		else							return true;
	}
	bool getBoolForKey(DSH_Key t_key, int key_val1, int key_val2)
	{
		string bool_string = myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), "false");
		if(bool_string == "false")		return false;
		else							return true;
	}
	
	void setBoolForKey(DSH_Key t_key, bool t_b)
	{
		string bool_string;
		if(t_b)			bool_string = "true";
		else			bool_string = "false";
		myDefault->setKeyValue(kSDF_myDSH, getKey(t_key), bool_string);
	}
	void setBoolForKey(DSH_Key t_key, int key_val1, bool t_b)
	{
		string bool_string;
		if(t_b)			bool_string = "true";
		else			bool_string = "false";
		myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), bool_string);
	}
	void setBoolForKey(DSH_Key t_key, int key_val1, int key_val2, bool t_b)
	{
		string bool_string;
		if(t_b)			bool_string = "true";
		else			bool_string = "false";
		myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), bool_string);
	}
	
	int getIntegerForKey(DSH_Key t_key)
	{
		if(t_key == kDSH_Key_elementLevelEmpty)
			return myDefault->getValue(kSDF_myDSH, getKey(t_key), 1);
		
		return myDefault->getValue(kSDF_myDSH, getKey(t_key), 0);
	}
	int getIntegerForKey(DSH_Key t_key, int key_val1)
	{
		return myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), 0);
	}
	int getIntegerForKey(DSH_Key t_key, int key_val1, int key_val2)
	{
		return myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), 0);
	}
	
	void setIntegerForKey(DSH_Key t_key, int val1)
	{
		myDefault->setKeyValue(kSDF_myDSH, getKey(t_key), val1);
	}
	void setIntegerForKey(DSH_Key t_key, int key_val1, int val1)
	{
		myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), val1);
	}
	
	string getStringForKey(DSH_Key t_key, int key_val1)
	{
		return myDefault->getValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), "");
	}
	void setStringForKey(DSH_Key t_key, int key_val1, string val1)
	{
		myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1)->getCString(), val1.c_str());
	}
	
	void setIntegerForKey(DSH_Key t_key, int key_val1, int key_val2, int val1)
	{
		myDefault->setKeyValue(kSDF_myDSH, CCString::createWithFormat(getKey(t_key).c_str(), key_val1, key_val2)->getCString(), val1);
	}
	
	void setGoldGetRate(float t_rate)
	{
		gold_get_rate = t_rate;
	}
	
	float getGoldGetRate()
	{
		return gold_get_rate;
	}
	
	CCSize getDesignResolutionSize()
	{
		return CCSizeMake(480, 320);
	}
	
	float ui_top;
	float puzzle_ui_top;
	float ui_center_y;
	float ui_bottom;
	CCPoint ui_zero_point;
	float screen_convert_rate;
	CCPoint ui_touch_convert;
	
	CCPoint mult_point;
	CCPoint add_point;
	CCPoint wideWidthFixTouch(CCPoint t_p){	return ccp(t_p.x*mult_point.x + add_point.x, t_p.y*mult_point.y + add_point.y);	}
	
private:
	SaveData* myDefault;
//	bool is_notRetinaIpad;
	float gold_get_rate;
	
	string getKey(DSH_Key t_name)
	{
		string return_value;
		if(t_name == kDSH_Key_lastSelectedChapter)						return_value = "lsc";
		else if(t_name == kDSH_Key_isOpendChapter_int1)					return_value = "oc%d";
		else if(t_name == kDSH_Key_chapter_int1_Stage_int2_Rating)		return_value = "c%ds%dr";
		else if(t_name == kDSH_Key_chapter_int1_Stage_int2_Highscore)	return_value = "c%ds%dhs";
		else if(t_name == kDSH_Key_chapter_int1_LastSelectedStage)		return_value = "c%dlss";
		else if(t_name == kDSH_Key_isOpendChapter_int1_Stage_int2)		return_value = "oc%ds%d";
		else if(t_name == kDSH_Key_elementLevelEmpty)					return_value = "ele";
		else if(t_name == kDSH_Key_elementLevelLife)					return_value = "ell";
		else if(t_name == kDSH_Key_elementLevelFire)					return_value = "elf";
		else if(t_name == kDSH_Key_elementLevelWater)					return_value = "elw";
		else if(t_name == kDSH_Key_elementLevelWind)					return_value = "eln";
		else if(t_name == kDSH_Key_elementLevelLightning)				return_value = "elt";
		else if(t_name == kDSH_Key_elementLevelPlasma)					return_value = "elp";
		else if(t_name == kDSH_Key_lastSelectedElement)					return_value = "lse";
		else if(t_name == kDSH_Key_savedStar)							return_value = "ss";
		else if(t_name == kDSH_Key_savedGold)							return_value = "sg";
		else if(t_name == kDSH_Key_notFirstExe)							return_value = "nfe";
		else if(t_name == kDSH_Key_musicOn)								return_value = "mo";
		else if(t_name == kDSH_Key_bgmOff)								return_value = "bgmoff";
		else if(t_name == kDSH_Key_effectOff)							return_value = "effoff";
		else if(t_name == kDSH_Key_serverTime)							return_value = "st";
		else if(t_name == kDSH_Key_heartCnt)							return_value = "hc";
		else if(t_name == kDSH_Key_heartTime)							return_value = "ht";
		else if(t_name == kDSH_Key_FBMHEldestNumber)					return_value = "fmhen";
		else if(t_name == kDSH_Key_FBMHId)								return_value = "fmhi%d";
		else if(t_name == kDSH_Key_FBMHTime)							return_value = "fmht%d";
		else if(t_name == kDSH_Key_FBMHAfterLink)						return_value = "fmhal%d";
		else if(t_name == kDSH_Key_isBrushInf)							return_value = "ibi";
		else if(t_name == kDSH_Key_isRemoveAD)							return_value = "irad";
		else if(t_name == kDSH_Key_isReview)							return_value = "review";
		else if(t_name == kDSH_Key_isFacebookLogined)					return_value = "ifl";
		else if(t_name == kDSH_Key_lastSelectedPet)						return_value = "lsp";
		else if(t_name == kDSH_Key_totalSelfPetCount)					return_value = "tspc";
		else if(t_name == kDSH_Key_selfPetCode_int1)					return_value = "spc%d";
		else if(t_name == kDSH_Key_openSlotCount)						return_value = "osc";
		else if(t_name == kDSH_Key_lastSelectedControler)				return_value = "lscontrol";
		else if(t_name == kDSH_Key_controlOriginX)						return_value = "cox";
		else if(t_name == kDSH_Key_controlOriginY)						return_value = "coy";
		else if(t_name == kDSH_Key_catchedMonsterChapter_int1_IsBoss_int2)		return_value = "cmc%db%d";
		else if(t_name == kDSH_Key_hasGottenPet_int1)							return_value = "hgp%d";
		else if(t_name == kDSH_Key_hasCaughtMonsterChapter_int1_IsBoss_int2)	return_value = "hcmc%db%d";
		else if(t_name == kDSH_Key_checkedNewControlJoystick)					return_value = "cncj";
		else if(t_name == kDSH_Key_theme_int1_clearednumber)			return_value = "theme%dclrn";
		else if(t_name == kDSH_Key_haveItemCnt_int1)					return_value = "hic%d";
		else if(t_name == kDSH_Key_selectedCard)						return_value = "scard";
		else if(t_name == kDSH_Key_cardDurability_int1)					return_value = "cd%d";
		else if(t_name == kDSH_Key_hasGottenCard_int1)					return_value = "hgcard%d";
		else if(t_name == kDSH_Key_inputTextCard_int1)					return_value = "itc%d";
		else if(t_name == kDSH_Key_cardTakeCnt)							return_value = "ctc";
		else if(t_name == kDSH_Key_cardSortType)						return_value = "cst";
		else if(t_name == kDSH_Key_lastSelectedStage)					return_value = "lss";
		else if(t_name == kDSH_Key_isPassCoupon_int1)					return_value = "ipcp%d";
		else if(t_name == kDSH_Key_uiType)								return_value = "uitype";
//		else if(t_name == kDSH_Key_isOpenTheme_int1_Stage_int2)			return_value = "iot%ds%d";
		else if(t_name == kDSH_Key_puzzleMode)							return_value = "pm";
		else if(t_name == kDSH_Key_selectedPuzzleNumber)				return_value = "spn";
		else if(t_name == kDSH_Key_allHighScore)						return_value = "ahs";
		
		return return_value;
	}
	
	void myInit()
	{
		myDefault = SaveData::sharedObject();
		
		
//		is_notRetinaIpad = false;
	}
};

#endif
