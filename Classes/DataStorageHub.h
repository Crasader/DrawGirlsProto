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
	kDSH_Key_savedFriendPoint,
	kDSH_Key_notFirstExe,
	kDSH_Key_bgmOff,
	kDSH_Key_effectOff,
	kDSH_Key_heartCnt,
	kDSH_Key_heartTime,
	kDSH_Key_haveItemCnt_int1,
	kDSH_Key_selectedCard,
	
	kDSH_Key_cardLevel_int1,
	kDSH_Key_cardMaxDurability_int1,
	kDSH_Key_cardPassive_int1,
	
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
	kDSH_Key_haveTicketCnt,
	kDSH_Key_ticketUserId_int1,
	kDSH_Key_openStageCnt,
	kDSH_Key_openStageNumber_int1,
	kDSH_Key_isOpenStage_int1,
	kDSH_Key_clearStageCnt,
	kDSH_Key_clearStageNumber_int1,
	kDSH_Key_isClearStage_int1,
	kDSH_Key_nick,
	kDSH_Key_wasTutorialPopupShockWave,
	kDSH_Key_wasTutorialPopupMissileTrace,
	kDSH_Key_wasTutorialPopupCrashArea,
	kDSH_Key_controlJoystickDirection,
	kDSH_Key_isControlJoystickFixed,
	kDSH_Key_hasShowTutorial_int1,
	kDSH_Key_selectedCharacter,
	kDSH_Key_isCharacterUnlocked_int1
}DSH_Key;

enum SpecialTutorialCode{
	kSpecialTutorialCode_control = 100000
};

enum ControlJoystickDirection{
	kControlJoystickDirection_right = 0,
	kControlJoystickDirection_left
};

enum SaveUserData_Key{
	kSaveUserData_Key_star = 1,
	kSaveUserData_Key_gold,
	kSaveUserData_Key_friendPoint,
	kSaveUserData_Key_item,
	kSaveUserData_Key_cardsInfo,
	kSaveUserData_Key_highScore,
	kSaveUserData_Key_selectedCard,
	kSaveUserData_Key_openPuzzle,
	kSaveUserData_Key_haveTicket,
	kSaveUserData_Key_openStage,
	kSaveUserData_Key_nick,
	kSaveUserData_Key_character
};

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
	
	string getStringForKey(DSH_Key t_key)
	{
		return myDefault->getValue(kSDF_myDSH, getKey(t_key), "");
	}
	void setStringForKey(DSH_Key t_key, string val1)
	{
		myDefault->setKeyValue(kSDF_myDSH, getKey(t_key), val1.c_str());
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
		else if(t_name == kDSH_Key_savedFriendPoint)					return_value = "sfp";
		else if(t_name == kDSH_Key_haveItemCnt_int1)					return_value = "hic%d";
		
		else if(t_name == kDSH_Key_heartCnt)							return_value = "hc";
		else if(t_name == kDSH_Key_heartTime)							return_value = "ht";
		
		else if(t_name == kDSH_Key_puzzleMode)							return_value = "pm";
		else if(t_name == kDSH_Key_selectedPuzzleNumber)				return_value = "spn";
		else if(t_name == kDSH_Key_lastSelectedStage)					return_value = "lss";
		
		else if(t_name == kDSH_Key_selectedCard)						return_value = "scard";
		else if(t_name == kDSH_Key_cardSortType)						return_value = "cst";
		else if(t_name == kDSH_Key_inputTextCard_int1)					return_value = "itc%d";
		
		else if(t_name == kDSH_Key_cardLevel_int1)						return_value = "cl%d";
		else if(t_name == kDSH_Key_cardMaxDurability_int1)				return_value = "cmd%d";
		else if(t_name == kDSH_Key_cardPassive_int1)					return_value = "cp%d";
		
		else if(t_name == kDSH_Key_cardDurability_int1)					return_value = "cd%d";	// arg int(card_number) // return usable durability
		else if(t_name == kDSH_Key_hasGottenCard_int1)					return_value = "hgcard%d";	// arg int(card_number) // return cardTakeCnt_number
		else if(t_name == kDSH_Key_cardTakeCnt)							return_value = "ctc";	// hasGottenCards count
		else if(t_name == kDSH_Key_takeCardNumber_int1)					return_value = "tcn%d"; // arg int(loop start 1 ~ cardTakeCnt) // return card_number
		
		else if(t_name == kDSH_Key_allHighScore)						return_value = "ahs";
		
		else if(t_name == kDSH_Key_jackBaseSpeed)						return_value = "jbs";
		
		else if(t_name == kDSH_Key_openPuzzleCnt)						return_value = "opc";
		else if(t_name == kDSH_Key_isClearedPuzzle_int1)				return_value = "icp%d";
		
		else if(t_name == kDSH_Key_haveTicketCnt)						return_value = "htc";
		else if(t_name == kDSH_Key_ticketUserId_int1)					return_value = "tui%d";
		
		else if(t_name == kDSH_Key_openStageCnt)						return_value = "osc";
		else if(t_name == kDSH_Key_openStageNumber_int1)				return_value = "osn%d";
		else if(t_name == kDSH_Key_isOpenStage_int1)					return_value = "ios%d";
		else if(t_name == kDSH_Key_clearStageCnt)						return_value = "csc";
		else if(t_name == kDSH_Key_clearStageNumber_int1)				return_value = "csn%d";
		else if(t_name == kDSH_Key_isClearStage_int1)					return_value = "ics%d";
		
		else if(t_name == kDSH_Key_nick)								return_value = "nick";
		
		else if(t_name == kDSH_Key_wasTutorialPopupShockWave)			return_value = "wtpsw";
		else if(t_name == kDSH_Key_wasTutorialPopupMissileTrace)		return_value = "wtpmt";
		else if(t_name == kDSH_Key_wasTutorialPopupCrashArea)			return_value = "wtpca";
		
		else if(t_name == kDSH_Key_controlJoystickDirection)			return_value = "cjd";
		
		else if(t_name == kDSH_Key_hasShowTutorial_int1)				return_value = "hst%d";
		else if(t_name == kDSH_Key_isControlJoystickFixed)				return_value = "icjf";
		
		else if(t_name == kDSH_Key_selectedCharacter)					return_value = "scharacter";
		else if(t_name == kDSH_Key_isCharacterUnlocked_int1)			return_value = "icu%d";
		
		return return_value;
	}
	
	Json::Value getSaveAllUserDataParam()
	{
		Json::Value param;
		param["memberID"] = hspConnector::get()->getKakaoID();
		
		Json::Value data;
		
		for(int i = kSaveUserData_Key_star;i<=kSaveUserData_Key_character;i++)
		{
			writeParamForKey(data, SaveUserData_Key(i));
		}
		
		Json::FastWriter writer;
		param["data"] = writer.write(data);
		param["nick"] = getStringForKey(kDSH_Key_nick);
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
		setIntegerForKey(kDSH_Key_savedFriendPoint, data[getKey(kDSH_Key_savedFriendPoint)].asInt());
		
		for(int i=kIC_attack;i<=kIC_rentCard;i++)
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
			
			setIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number, data[getKey(kDSH_Key_cardLevel_int1)].get(i, 1).asInt());
			setIntegerForKey(kDSH_Key_cardMaxDurability_int1, take_card_number, data[getKey(kDSH_Key_cardMaxDurability_int1)].get(i, 5).asInt());
			setStringForKey(kDSH_Key_cardPassive_int1, take_card_number, data[getKey(kDSH_Key_cardPassive_int1)].get(i, "").asString());
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, take_card_number) == "")
				card_data_load_list.push_back(take_card_number);
		}
		
		setIntegerForKey(kDSH_Key_allHighScore, data[getKey(kDSH_Key_allHighScore)].asInt());
		setIntegerForKey(kDSH_Key_selectedCard, data[getKey(kDSH_Key_selectedCard)].asInt());
		
		int open_puzzle_cnt = data[getKey(kDSH_Key_openPuzzleCnt)].asInt();
		
		setIntegerForKey(kDSH_Key_openPuzzleCnt, open_puzzle_cnt);
		for(int i=1;i<=open_puzzle_cnt+2 && i < data[getKey(kDSH_Key_isClearedPuzzle_int1)].size();i++)
			setBoolForKey(kDSH_Key_isClearedPuzzle_int1, i, data[getKey(kDSH_Key_isClearedPuzzle_int1)][i].asBool());
		
		int have_ticket_cnt = data[getKey(kDSH_Key_haveTicketCnt)].asInt();
		setIntegerForKey(kDSH_Key_haveTicketCnt, have_ticket_cnt);
		for(int i=1;i<=have_ticket_cnt;i++)
			setStringForKey(kDSH_Key_ticketUserId_int1, i, data[getKey(kDSH_Key_ticketUserId_int1)][i].asString());
		
		int open_stage_cnt = data[getKey(kDSH_Key_openStageCnt)].asInt();
		setIntegerForKey(kDSH_Key_openStageCnt, open_stage_cnt);
		for(int i=1;i<=open_stage_cnt;i++)
		{
			int t_stage_number = data[getKey(kDSH_Key_openStageNumber_int1)][i].asInt();
			setIntegerForKey(kDSH_Key_openStageNumber_int1, i, t_stage_number);
			setBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number, data[getKey(kDSH_Key_isOpenStage_int1)][i].asBool());
		}
		
		int clear_stage_cnt = data[getKey(kDSH_Key_clearStageCnt)].asInt();
		setIntegerForKey(kDSH_Key_clearStageCnt, clear_stage_cnt);
		for(int i=1;i<=clear_stage_cnt;i++)
		{
			int t_stage_number = data[getKey(kDSH_Key_clearStageNumber_int1)][i].asInt();
			setIntegerForKey(kDSH_Key_clearStageNumber_int1, i, t_stage_number);
			setBoolForKey(kDSH_Key_isClearStage_int1, t_stage_number, data[getKey(kDSH_Key_isClearStage_int1)][i].asBool());
		}
		
		setStringForKey(kDSH_Key_nick, data[getKey(kDSH_Key_nick)].asString().c_str());
		
		setIntegerForKey(kDSH_Key_selectedCharacter, data[getKey(kDSH_Key_selectedCharacter)].asInt());
		for(int i=1;i<NSDS_GI(kSDS_GI_characterCount_i);i++)
		{
			bool t_unlocked = data[getKey(kDSH_Key_isCharacterUnlocked_int1)][i].asBool();
			setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, t_unlocked);
		}
	}
	
	void writeParamForKey(Json::Value& data, SaveUserData_Key t_key)
	{
		if(t_key == kSaveUserData_Key_star)
			data[getKey(kDSH_Key_savedStar)] = getIntegerForKey(kDSH_Key_savedStar);
		else if(t_key == kSaveUserData_Key_gold)
			data[getKey(kDSH_Key_savedGold)] = getIntegerForKey(kDSH_Key_savedGold);
		else if(t_key == kSaveUserData_Key_friendPoint)
			data[getKey(kDSH_Key_savedFriendPoint)] = getIntegerForKey(kDSH_Key_savedFriendPoint);
		else if(t_key == kSaveUserData_Key_item)
		{
			for(int i=kIC_attack;i<=kIC_rentCard;i++)
				data[getKey(kDSH_Key_haveItemCnt_int1)][i] = getIntegerForKey(kDSH_Key_haveItemCnt_int1, i); // 0
		}
		else if(t_key == kSaveUserData_Key_cardsInfo)
		{
			data[getKey(kDSH_Key_cardTakeCnt)] = getIntegerForKey(kDSH_Key_cardTakeCnt);
			int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt);
			for(int i=1;i<=card_take_cnt;i++)
			{
				int take_card_number = getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
				data[getKey(kDSH_Key_takeCardNumber_int1)][i] = take_card_number;
				data[getKey(kDSH_Key_hasGottenCard_int1)][i] = getIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number);
				data[getKey(kDSH_Key_cardDurability_int1)][i] = getIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number);
				data[getKey(kDSH_Key_inputTextCard_int1)][i] = getStringForKey(kDSH_Key_inputTextCard_int1, take_card_number);
				
				data[getKey(kDSH_Key_cardLevel_int1)][i] = getIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number);
				data[getKey(kDSH_Key_cardMaxDurability_int1)][i] = getIntegerForKey(kDSH_Key_cardMaxDurability_int1, take_card_number);
				data[getKey(kDSH_Key_cardPassive_int1)][i] = getStringForKey(kDSH_Key_cardPassive_int1, take_card_number);
			}
		}
		else if(t_key == kSaveUserData_Key_highScore)
			data[getKey(kDSH_Key_allHighScore)] = getIntegerForKey(kDSH_Key_allHighScore);
		else if(t_key == kSaveUserData_Key_selectedCard)
			data[getKey(kDSH_Key_selectedCard)] = getIntegerForKey(kDSH_Key_selectedCard);
		else if(t_key == kSaveUserData_Key_openPuzzle)
		{
			int open_puzzle_cnt = getIntegerForKey(kDSH_Key_openPuzzleCnt);
			data[getKey(kDSH_Key_openPuzzleCnt)] = open_puzzle_cnt;
			for(int i=1;i<=open_puzzle_cnt+2;i++)
				data[getKey(kDSH_Key_isClearedPuzzle_int1)][i] = getBoolForKey(kDSH_Key_isClearedPuzzle_int1, i);
		}
		else if(t_key == kSaveUserData_Key_haveTicket)
		{
			int have_ticket_cnt = getIntegerForKey(kDSH_Key_haveTicketCnt);
			data[getKey(kDSH_Key_haveTicketCnt)] = have_ticket_cnt;
			for(int i=1;i<=have_ticket_cnt;i++)
				data[getKey(kDSH_Key_ticketUserId_int1)][i] = getStringForKey(kDSH_Key_ticketUserId_int1, i);
		}
		else if(t_key == kSaveUserData_Key_openStage)
		{
			int open_stage_cnt = getIntegerForKey(kDSH_Key_openStageCnt);
			data[getKey(kDSH_Key_openStageCnt)] = open_stage_cnt;
			for(int i=1;i<=open_stage_cnt;i++)
			{
				int t_stage_number = getIntegerForKey(kDSH_Key_openStageNumber_int1, i);
				data[getKey(kDSH_Key_openStageNumber_int1)][i] = t_stage_number;
				data[getKey(kDSH_Key_isOpenStage_int1)][i] = getBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number);
			}
			
			int clear_stage_cnt = getIntegerForKey(kDSH_Key_clearStageCnt);
			data[getKey(kDSH_Key_clearStageCnt)] = clear_stage_cnt;
			for(int i=1;i<=clear_stage_cnt;i++)
			{
				int t_stage_number = getIntegerForKey(kDSH_Key_clearStageNumber_int1, i);
				data[getKey(kDSH_Key_clearStageNumber_int1)][i] = t_stage_number;
				data[getKey(kDSH_Key_isClearStage_int1)][i] = getBoolForKey(kDSH_Key_isClearStage_int1, t_stage_number);
			}
		}
		else if(t_key == kSaveUserData_Key_nick)
		{
			data[getKey(kDSH_Key_nick)] = getStringForKey(kDSH_Key_nick);
		}
		else if(t_key == kSaveUserData_Key_character)
		{
			data[getKey(kDSH_Key_selectedCharacter)] = getIntegerForKey(kDSH_Key_selectedCharacter);
			for(int i=2;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
				data[getKey(kDSH_Key_isCharacterUnlocked_int1)][i] = getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i);
		}
	}
	
	void saveUserData(const vector<SaveUserData_Key>& key_list, function<void(Json::Value)> t_selector)
	{
		Json::Value param;
		param["memberID"] = hspConnector::get()->getKakaoID();
		
		Json::Value data;
		
		for(int i=0;i<key_list.size();i++)
		{
			writeParamForKey(data, key_list[i]);
		}
		
		Json::FastWriter writer;
		param["data"] = writer.write(data);
		param["nick"] = getStringForKey(kDSH_Key_nick);
		
		hspConnector::get()->command("updateUserData", param, t_selector);
	}
	
	void saveAllUserData(jsonSelType t_saved)
	{
		hspConnector::get()->command("updateUserData", getSaveAllUserDataParam(), t_saved);
	}
	
	void resetDSH()
	{
		setIntegerForKey(kDSH_Key_savedStar, 1);
		setIntegerForKey(kDSH_Key_savedGold, 10000);
		setIntegerForKey(kDSH_Key_savedFriendPoint, 0);
		
		for(int i=kIC_attack;i<=kIC_rentCard;i++)
			setIntegerForKey(kDSH_Key_haveItemCnt_int1, i, 0);
		
		int card_take_cnt = getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=card_take_cnt;i++)
		{
			int take_card_number = getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			setIntegerForKey(kDSH_Key_takeCardNumber_int1, i, 0);
			setIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number, 0);
			setIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number, 0);
			setStringForKey(kDSH_Key_inputTextCard_int1, take_card_number, "");
			
			
			setIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number, 1);
			setIntegerForKey(kDSH_Key_cardMaxDurability_int1, take_card_number, 0);
			setStringForKey(kDSH_Key_cardPassive_int1, take_card_number, "");
		}
		setIntegerForKey(kDSH_Key_cardTakeCnt, 0);
		
		setIntegerForKey(kDSH_Key_allHighScore, 0);
		
		int opened_puzzle_cnt = getIntegerForKey(kDSH_Key_openPuzzleCnt);
		for(int i=1;i<=opened_puzzle_cnt+2;i++)
			setBoolForKey(kDSH_Key_isClearedPuzzle_int1, i, false);
		setIntegerForKey(kDSH_Key_openPuzzleCnt, 0);
		
		int have_ticket_cnt = getIntegerForKey(kDSH_Key_haveTicketCnt);
		for(int i=1;i<=have_ticket_cnt;i++)
			setStringForKey(kDSH_Key_ticketUserId_int1, i, "");
		setIntegerForKey(kDSH_Key_haveTicketCnt, 0);
		
		int open_stage_cnt = getIntegerForKey(kDSH_Key_openStageCnt);
		for(int i=1;i<=open_stage_cnt;i++)
		{
			setBoolForKey(kDSH_Key_isOpenStage_int1, getIntegerForKey(kDSH_Key_openStageNumber_int1, i), false);
			setIntegerForKey(kDSH_Key_openStageNumber_int1, i, 0);
		}
		setIntegerForKey(kDSH_Key_openStageCnt, 1);
		
		
		int clear_stage_cnt = getIntegerForKey(kDSH_Key_clearStageCnt);
		for(int i=1;i<=clear_stage_cnt;i++)
		{
			setBoolForKey(kDSH_Key_isClearStage_int1, getIntegerForKey(kDSH_Key_clearStageNumber_int1, i), false);
			setIntegerForKey(kDSH_Key_clearStageNumber_int1, i, 0);
		}
		setIntegerForKey(kDSH_Key_clearStageCnt, 0);
		
		setStringForKey(kDSH_Key_nick, "");
		
		setIntegerForKey(kDSH_Key_selectedCharacter, 0);
		for(int i=2;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
			setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i, true);
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
