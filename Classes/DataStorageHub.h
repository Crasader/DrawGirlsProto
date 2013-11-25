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
#include "EnumDefine.h"
#include "hspConnector.h"
#include "ServerDataSave.h"

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
	kDSH_Key_takeCardNumber_int1,
	kDSH_Key_cardSortType,
	kDSH_Key_lastSelectedStage,
	kDSH_Key_uiType,
	kDSH_Key_puzzleMode,
	kDSH_Key_selectedPuzzleNumber,
	kDSH_Key_allHighScore,
	kDSH_Key_jackBaseSpeed,
	kDSH_Key_openPuzzleCnt,
	kDSH_Key_isClearedPuzzle_int1,
	kDSH_Key_haveTicketCnt
}DSH_Key;

#define myDSH DataStorageHub::sharedInstance()

enum PuzzleMapSceneShowType{
	kPuzzleMapSceneShowType_init = 0,
	kPuzzleMapSceneShowType_stage,
	kPuzzleMapSceneShowType_clear,
	kPuzzleMapSceneShowType_fail,
	kPuzzleMapSceneShowType_stageSetting
};

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
	
	int getUserIntForStr(string t_key, int t_default)
	{
		return myDefault->getValue(kSDF_myDSH, t_key, t_default);
	}
	void setUserIntForStr(string t_key, int t_val1)
	{
		myDefault->setKeyValue(kSDF_myDSH, t_key, t_val1);
	}
	
	string getUserStrForStr(string t_key, string t_default)
	{
		return myDefault->getValue(kSDF_myDSH, t_key, t_default);
	}
	void setUserStrForStr(string t_key, string t_val1)
	{
		myDefault->setKeyValue(kSDF_myDSH, t_key, t_val1);
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
	
	string getKey(DSH_Key t_name)
	{
		string return_value;
		if(t_name == kDSH_Key_notFirstExe)								return_value = "nfe";
		
		else if(t_name == kDSH_Key_bgmOff)								return_value = "bgmoff";
		else if(t_name == kDSH_Key_effectOff)							return_value = "effoff";
		else if(t_name == kDSH_Key_uiType)								return_value = "uitype";
		
		else if(t_name == kDSH_Key_savedStar)							return_value = "ss";
		else if(t_name == kDSH_Key_savedGold)							return_value = "sg";
		else if(t_name == kDSH_Key_haveItemCnt_int1)					return_value = "hic%d";
		
		else if(t_name == kDSH_Key_heartCnt)							return_value = "hc";
		else if(t_name == kDSH_Key_heartTime)							return_value = "ht";
		
		else if(t_name == kDSH_Key_puzzleMode)							return_value = "pm";
		else if(t_name == kDSH_Key_selectedPuzzleNumber)				return_value = "spn";
		else if(t_name == kDSH_Key_lastSelectedStage)					return_value = "lss";
		
		else if(t_name == kDSH_Key_selectedCard)						return_value = "scard";
		else if(t_name == kDSH_Key_cardSortType)						return_value = "cst";
		else if(t_name == kDSH_Key_inputTextCard_int1)					return_value = "itc%d";
		
		else if(t_name == kDSH_Key_cardDurability_int1)					return_value = "cd%d";	// arg int(card_number) // return usable durability
		else if(t_name == kDSH_Key_hasGottenCard_int1)					return_value = "hgcard%d";	// arg int(card_number) // return cardTakeCnt_number
		else if(t_name == kDSH_Key_cardTakeCnt)							return_value = "ctc";	// hasGottenCards count
		else if(t_name == kDSH_Key_takeCardNumber_int1)					return_value = "tcn%d"; // arg int(loop start 1 ~ cardTakeCnt) // return card_number
		
		else if(t_name == kDSH_Key_allHighScore)						return_value = "ahs";
		
		else if(t_name == kDSH_Key_jackBaseSpeed)						return_value = "jbs";
		
		else if(t_name == kDSH_Key_openPuzzleCnt)						return_value = "opc";
		else if(t_name == kDSH_Key_isClearedPuzzle_int1)				return_value = "icp%d";
		
		else if(t_name == kDSH_Key_haveTicketCnt)						return_value = "htc";
		
		return return_value;
	}
	
	Json::Value getSaveAllUserDataParam()
	{
		Json::Value param;
		param["memberID"] = hspConnector::get()->getKakaoID();
		
		Json::Value data;
		data[getKey(kDSH_Key_savedStar)] = getIntegerForKey(kDSH_Key_savedStar); // 1
		data[getKey(kDSH_Key_savedGold)] = getIntegerForKey(kDSH_Key_savedGold); // 1000
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
			data[getKey(kDSH_Key_haveItemCnt_int1)][i] = getIntegerForKey(kDSH_Key_haveItemCnt_int1, i); // 0
		
		data[getKey(kDSH_Key_cardTakeCnt)] = getIntegerForKey(kDSH_Key_cardTakeCnt); // 0
		int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt); /////////////////////////////
		for(int i=1;i<=card_take_cnt;i++)
		{
			int take_card_number = getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			data[getKey(kDSH_Key_takeCardNumber_int1)][i] = take_card_number;
			data[getKey(kDSH_Key_hasGottenCard_int1)][i] = getIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number);
			data[getKey(kDSH_Key_cardDurability_int1)][i] = getIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number);
			data[getKey(kDSH_Key_inputTextCard_int1)][i] = getStringForKey(kDSH_Key_inputTextCard_int1, take_card_number);
		} /////////////////////////////////////////
		
		data[getKey(kDSH_Key_allHighScore)] = getIntegerForKey(kDSH_Key_allHighScore); // 0
		data[getKey(kDSH_Key_selectedCard)] = getIntegerForKey(kDSH_Key_selectedCard); // 0
		
		int open_puzzle_cnt = getIntegerForKey(kDSH_Key_openPuzzleCnt);
		
		data[getKey(kDSH_Key_openPuzzleCnt)] = open_puzzle_cnt;
		
		for(int i=1;i<=open_puzzle_cnt+2;i++)
			data[getKey(kDSH_Key_isClearedPuzzle_int1)][i] = getBoolForKey(kDSH_Key_isClearedPuzzle_int1, i);
		
		data[getKey(kDSH_Key_haveTicketCnt)] = getIntegerForKey(kDSH_Key_haveTicketCnt);
		
		Json::FastWriter writer;
		param["data"] = writer.write(data);
		return param;
	}
	
	void loadAllUserData(Json::Value result_data, vector<int>& card_data_load_list)
	{
		Json::Value data;
		Json::Reader reader;
		reader.parse(result_data["data"].asString(), data);
		
		CCLog("parse data : %s", GraphDogLib::JsonObjectToString(data).c_str());
		
		setIntegerForKey(kDSH_Key_savedStar, data[getKey(kDSH_Key_savedStar)].asInt());
		setIntegerForKey(kDSH_Key_savedGold, data[getKey(kDSH_Key_savedGold)].asInt());
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
			setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, data[getKey(kDSH_Key_haveItemCnt_int1)][i].asInt());
		
		setIntegerForKey(kDSH_Key_cardTakeCnt, data[getKey(kDSH_Key_cardTakeCnt)].asInt());
		int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt);
		
		for(int i=1;i<=card_take_cnt;i++)
		{
			int take_card_number = data[getKey(kDSH_Key_takeCardNumber_int1)][i].asInt();
			setIntegerForKey(kDSH_Key_takeCardNumber_int1, i, take_card_number);
			setStringForKey(kDSH_Key_inputTextCard_int1, take_card_number, data[getKey(kDSH_Key_inputTextCard_int1)][i].asString());
			setIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number, data[getKey(kDSH_Key_cardDurability_int1)][i].asInt());
			setIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number, data[getKey(kDSH_Key_hasGottenCard_int1)][i].asInt());
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, take_card_number) == "")
				card_data_load_list.push_back(take_card_number);
		}
		
		setIntegerForKey(kDSH_Key_allHighScore, data[getKey(kDSH_Key_allHighScore)].asInt());
		setIntegerForKey(kDSH_Key_selectedCard, data[getKey(kDSH_Key_selectedCard)].asInt());
		
		int open_puzzle_cnt = data[getKey(kDSH_Key_openPuzzleCnt)].asInt();
		
		setIntegerForKey(kDSH_Key_openPuzzleCnt, open_puzzle_cnt);
		for(int i=1;i<=open_puzzle_cnt+2;i++)
			setBoolForKey(kDSH_Key_isClearedPuzzle_int1, i, data[getKey(kDSH_Key_openPuzzleCnt)][i].asInt());
		
		setIntegerForKey(kDSH_Key_haveTicketCnt, data[getKey(kDSH_Key_haveTicketCnt)].asInt());
	}
	
	void saveAllUserData(jsonSelType t_saved)
	{
		hspConnector::get()->command("updateUserData", getSaveAllUserDataParam(), t_saved);
	}
	
	void resetDSH()
	{
		setIntegerForKey(kDSH_Key_savedStar, 1);
		setIntegerForKey(kDSH_Key_savedGold, 10000);
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
			setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, 0);
		
		int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=card_take_cnt;i++)
		{
			int take_card_number = getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			setIntegerForKey(kDSH_Key_takeCardNumber_int1, i, 0);
			setIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number, 0);
			setIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number, 0);
			setStringForKey(kDSH_Key_inputTextCard_int1, take_card_number, "");
		}
		setIntegerForKey(kDSH_Key_cardTakeCnt, 0);
		
		setIntegerForKey(kDSH_Key_allHighScore, 0);
		
		int opened_puzzle_cnt = getIntegerForKey(kDSH_Key_openPuzzleCnt);
		for(int i=1;i<=opened_puzzle_cnt+2;i++)
			setIntegerForKey(kDSH_Key_isClearedPuzzle_int1, i, false);
		
		setIntegerForKey(kDSH_Key_openPuzzleCnt, 0);
		setIntegerForKey(kDSH_Key_haveTicketCnt, 0);
	}
	
	bool isCheatKeyEnable()
	{
		return is_cheat_key_enabled;
	}
	
	int getPuzzleMapSceneShowType()
	{
		return puzzle_map_scene_show_type;
	}
	
	void setPuzzleMapSceneShowType(int t_type)
	{
		puzzle_map_scene_show_type = t_type;
	}
	
private:
	SaveData* myDefault;
	float gold_get_rate;
	bool is_cheat_key_enabled;
	int puzzle_map_scene_show_type;
	
	void myInit()
	{
		myDefault = SaveData::sharedObject();
		is_cheat_key_enabled = true;
		puzzle_map_scene_show_type = kPuzzleMapSceneShowType_init;
	}
};

#endif
