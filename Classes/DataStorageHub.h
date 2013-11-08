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
	kDSH_Key_savedStar,
	kDSH_Key_savedGold,
	kDSH_Key_notFirstExe,
	kDSH_Key_bgmOff,
	kDSH_Key_effectOff,
	kDSH_Key_heartCnt,
	kDSH_Key_heartTime,
	kDSH_Key_haveItemCnt_int1,
	kDSH_Key_selectedCard,
	kDSH_Key_cardDurability_int1,
	kDSH_Key_hasGottenCard_int1,
	kDSH_Key_inputTextCard_int1,
	kDSH_Key_cardTakeCnt,
	kDSH_Key_cardSortType,
	kDSH_Key_lastSelectedStage,
	kDSH_Key_uiType,
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
		string bool_string = myDefault->getValue(kSDF_myDSH, getKey(t_key), "false");
		if(bool_string == "false")		return false;
		else							return true;
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
	float gold_get_rate;
	
	string getKey(DSH_Key t_name)
	{
		string return_value;
		if(t_name == kDSH_Key_savedStar)								return_value = "ss";
		else if(t_name == kDSH_Key_savedGold)							return_value = "sg";
		else if(t_name == kDSH_Key_notFirstExe)							return_value = "nfe";
		else if(t_name == kDSH_Key_bgmOff)								return_value = "bgmoff";
		else if(t_name == kDSH_Key_effectOff)							return_value = "effoff";
		else if(t_name == kDSH_Key_heartCnt)							return_value = "hc";
		else if(t_name == kDSH_Key_heartTime)							return_value = "ht";
		else if(t_name == kDSH_Key_haveItemCnt_int1)					return_value = "hic%d";
		else if(t_name == kDSH_Key_selectedCard)						return_value = "scard";
		else if(t_name == kDSH_Key_cardDurability_int1)					return_value = "cd%d";
		else if(t_name == kDSH_Key_hasGottenCard_int1)					return_value = "hgcard%d";
		else if(t_name == kDSH_Key_inputTextCard_int1)					return_value = "itc%d";
		else if(t_name == kDSH_Key_cardTakeCnt)							return_value = "ctc";
		else if(t_name == kDSH_Key_cardSortType)						return_value = "cst";
		else if(t_name == kDSH_Key_lastSelectedStage)					return_value = "lss";
		else if(t_name == kDSH_Key_uiType)								return_value = "uitype";
		else if(t_name == kDSH_Key_puzzleMode)							return_value = "pm";
		else if(t_name == kDSH_Key_selectedPuzzleNumber)				return_value = "spn";
		else if(t_name == kDSH_Key_allHighScore)						return_value = "ahs";
		
		return return_value;
	}
	
	void myInit()
	{
		myDefault = SaveData::sharedObject();
	}
};

#endif
