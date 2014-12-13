// ServerDataSave.h
//

#ifndef LZZ_ServerDataSave_h
#define LZZ_ServerDataSave_h
#include "cocos2d.h"
#include "SaveData.h"
#include <map>
#include "KSProtect.h"
#include "jsoncpp/json.h"

USING_NS_CC;

using namespace std;

#define mySDS ServerDataSave::sharedInstance()

#define SDS_SB mySDS->setBoolForKey
#define SDS_SI mySDS->setIntegerForKey
#define SDS_SD mySDS->setDoubleForKey
#define SDS_SS mySDS->setStringForKey
#define SDS_GB mySDS->getBoolForKey
#define SDS_GI mySDS->getIntegerForKey
#define SDS_GS mySDS->getStringForKey
#define SDS_GD mySDS->getDoubleForKey

#define NSDS_SB mySDS->sbfk
#define NSDS_SI mySDS->sifk
#define NSDS_SD mySDS->sdfk
#define NSDS_SS mySDS->ssfk
#define NSDS_GB mySDS->gbfk
#define NSDS_GI mySDS->gifk
#define NSDS_GS mySDS->gsfk
#define NSDS_GD mySDS->gdfk
#define LZZ_INLINE inline
enum SDS_KEY
{
	kSDS_PZ_base = 0,
	kSDS_PZ_version_i,
	kSDS_PZ_title_s,
	kSDS_PZ_condition_s,
	kSDS_PZ_ticket_i,
	kSDS_PZ_point_i,
	kSDS_PZ_center_s,
	kSDS_PZ_original_s,
	kSDS_PZ_face_s,
	kSDS_PZ_map_s,
	kSDS_PZ_bottom_s,
	kSDS_PZ_top_s,
	kSDS_PZ_left_s,
	kSDS_PZ_right_s,
	kSDS_PZ_startStage_i,
	kSDS_PZ_stageCount_i,
	kSDS_PZ_stage_int1_level_i,
	kSDS_PZ_stage_int1_pieceType_s,
	kSDS_PZ_stage_int1_pieceNo_i,
	kSDS_PZ_stage_int1_condition_stage_i,
	kSDS_PZ_stage_int1_condition_gold_i,
	kSDS_PZ_stage_int1_x_d,
	kSDS_PZ_stage_int1_y_d,
	kSDS_PZ_stage_int1_piece_s,
	kSDS_PZ_stage_int1_thumbnail_s,
	kSDS_PZ_startWarp_x_d,
	kSDS_PZ_startWarp_y_d,
	kSDS_PZ_lastWarp_x_d,
	kSDS_PZ_lastWarp_y_d,
	kSDS_PZ_color_r_d,
	kSDS_PZ_color_g_d,
	kSDS_PZ_color_b_d,
	kSDS_PZ_isEvent_b,
	kSDS_PZ_clearCard_i,
	kSDS_PZ_perfectCard_i,
	kSDS_PZ_end = 999,
	kSDS_GI_base = 1000,
	kSDS_GI_puzzleListVersion_i,
	kSDS_GI_puzzleListCount_i,
	kSDS_GI_puzzleList_int1_no_i,
	kSDS_GI_puzzleList_int1_title_s,
	kSDS_GI_puzzleList_int1_version_i,
	kSDS_GI_puzzleList_int1_thumbnail_s,
	kSDS_GI_eventListVersion_i,
	kSDS_GI_eventListCount_i,
	kSDS_GI_eventList_int1_no_i,
	kSDS_GI_eventList_int1_title_s,
	kSDS_GI_eventList_int1_version_i,
	kSDS_GI_eventList_int1_thumbnail_s,
	kSDS_GI_characterVersion_i,
	kSDS_GI_characterCount_i, // 1 ~ count
	kSDS_GI_characterInfo_int1_no_i,
	kSDS_GI_characterInfo_int1_name_s,
	kSDS_GI_characterInfo_int1_purchaseInfo_type_s,
	kSDS_GI_characterInfo_int1_purchaseInfo_value_i,
	kSDS_GI_characterInfo_int1_maxLevel_i,
	kSDS_GI_characterInfo_int1_statInfo_int2_gold_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_percent_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_speed_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_life_i,
	kSDS_GI_characterInfo_int1_statInfo_int2_lineColor_i, // 000000~FFFFFF를 10진수로 바꾼값
	kSDS_GI_characterInfo_int1_statInfo_int2_rewindSpd_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_monsterWave_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_missileWave_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_magnetic_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_power_d,
	kSDS_GI_characterInfo_int1_statInfo_int2_score_d,
	kSDS_GI_characterInfo_int1_patternInfo_int2_s,
	kSDS_GI_characterInfo_int1_missionInfo_int2_s,
	kSDS_GI_characterInfo_int1_missileInfo_int2_s,
	kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s,
	kSDS_GI_characterInfo_int1_resourceInfo_ccbi_s,
	kSDS_GI_characterInfo_int1_resourceInfo_imageID_s,
	kSDS_GI_characterInfo_int1_resourceInfo_plist_s,
	kSDS_GI_characterInfo_int1_resourceInfo_pvrccz_s,
	kSDS_GI_characterInfo_int1_resourceInfo_size_i,
	kSDS_GI_characterInfo_int1_comment_int2_s,
	kSDS_GI_characterInfo_int1_scriptInfo_s,
	kSDS_GI_monsterVersion_i,
	kSDS_GI_monsterCount_i, // 1 ~ count
	kSDS_GI_monsterInfo_int1_no_i,
	kSDS_GI_monsterInfo_int1_name_s,
	kSDS_GI_monsterInfo_int1_isBoss_b,
	kSDS_GI_monsterInfo_int1_resourceInfo_ccbiID_s,
	kSDS_GI_monsterInfo_int1_resourceInfo_ccbi_s,
	kSDS_GI_monsterInfo_int1_resourceInfo_imageID_s,
	kSDS_GI_monsterInfo_int1_resourceInfo_plist_s,
	kSDS_GI_monsterInfo_int1_resourceInfo_pvrccz_s,
	kSDS_GI_monsterInfo_int1_resourceInfo_size_i,
	kSDS_GI_monsterInfo_int1_script_start_s,
	kSDS_GI_monsterInfo_int1_script_clear_s,
	kSDS_GI_monsterInfo_int1_script_fail_s,
	kSDS_GI_shopVersion_i,
//	kSDS_GI_shopRuby_int1_count_i,
	kSDS_GI_shopRuby_int1_countName_s,
//	kSDS_GI_shopRuby_int1_price_i,
//	kSDS_GI_shopRuby_int1_priceType_s,
	kSDS_GI_shopRuby_int1_priceName_s,
	kSDS_GI_shopRuby_int1_sale_s,
	kSDS_GI_shopRuby_int1_pID_s,
	kSDS_GI_shopRuby_int1_exchangeID_s,
//	kSDS_GI_shopGold_int1_count_i,
	kSDS_GI_shopGold_int1_countName_s,
//	kSDS_GI_shopGold_int1_price_i,
//	kSDS_GI_shopGold_int1_priceType_s,
	kSDS_GI_shopGold_int1_priceName_s,
	kSDS_GI_shopGold_int1_sale_s,
	kSDS_GI_shopGold_int1_exchangeID_s,
//	kSDS_GI_shopCoin_int1_count_i,
	kSDS_GI_shopCoin_int1_countName_s,
//	kSDS_GI_shopCoin_int1_price_i,
//	kSDS_GI_shopCoin_int1_priceType_s,
	kSDS_GI_shopCoin_int1_priceName_s,
	kSDS_GI_shopCoin_int1_sale_s,
	kSDS_GI_shopCoin_int1_exchangeID_s,
//	kSDS_GI_shopEventRuby_int1_count_i,
	kSDS_GI_shopEventRuby_int1_countName_s,
//	kSDS_GI_shopEventRuby_int1_price_i,
//	kSDS_GI_shopEventRuby_int1_priceType_s,
	kSDS_GI_shopEventRuby_int1_priceName_s,
	kSDS_GI_shopEventRuby_int1_sale_s,
	kSDS_GI_shopEventRuby_int1_pID_s,
	kSDS_GI_shopEventRuby_int1_exchangeID_s,
//	kSDS_GI_shopPurchaseGuide_int1_count_i,
	kSDS_GI_shopPurchaseGuide_int1_countName_s,
//	kSDS_GI_shopPurchaseGuide_int1_price_i,
//	kSDS_GI_shopPurchaseGuide_int1_priceType_s,
	kSDS_GI_shopPurchaseGuide_int1_priceName_s,
	kSDS_GI_shopPurchaseGuide_int1_sale_s,
	kSDS_GI_shopPurchaseGuide_int1_data_s,
	kSDS_GI_shopPurchaseGuide_int1_pID_s,
	kSDS_GI_shopPurchaseGuide_int1_exchangeID_s,
	kSDS_GI_shopGachaCardOnce_countName_s,
	kSDS_GI_shopGachaCardOnce_priceName_s,
	kSDS_GI_shopGachaCardOnce_sale_s,
	kSDS_GI_shopGachaCardOnce_data_s,
	kSDS_GI_shopGachaCardOnce_exchangeID_s,
	kSDS_GI_shopGachaCardOnce_reward_s,
	kSDS_GI_shopGachaCardDozen_countName_s,
	kSDS_GI_shopGachaCardDozen_priceName_s,
	kSDS_GI_shopGachaCardDozen_sale_s,
	kSDS_GI_shopGachaCardDozen_data_s,
	kSDS_GI_shopGachaCardDozen_exchangeID_s,
	kSDS_GI_shopGachaCardDozen_reward_s,
	kSDS_GI_shopGachaCardPass_countName_s,
	kSDS_GI_shopGachaCardPass_priceName_s,
	kSDS_GI_shopGachaCardPass_sale_s,
	kSDS_GI_shopGachaCardPass_data_s,
	kSDS_GI_shopGachaCardPass_exchangeID_s,
	kSDS_GI_shopGachaCardPass_reward_s,
	kSDS_GI_shopComposeCardStone_countName_s,
	kSDS_GI_shopComposeCardStone_priceName_s,
	kSDS_GI_shopComposeCardStone_sale_s,
	kSDS_GI_shopComposeCardStone_data_s,
	kSDS_GI_shopComposeCardStone_exchangeID_s,
	kSDS_GI_shopComposeCardStone_reward_s,
	kSDS_GI_shopComposeCardPass_countName_s,
	kSDS_GI_shopComposeCardPass_priceName_s,
	kSDS_GI_shopComposeCardPass_sale_s,
	kSDS_GI_shopComposeCardPass_data_s,
	kSDS_GI_shopComposeCardPass_exchangeID_s,
	kSDS_GI_shopComposeCardPass_reward_s,
	kSDS_GI_shopItem_int1_countName_s,
//	kSDS_GI_shopItem_int1_price_i,
//	kSDS_GI_shopItem_int1_priceType_s,
	kSDS_GI_shopItem_int1_priceName_s,
	kSDS_GI_shopItem_int1_sale_s,
	kSDS_GI_shopItem_int1_exchangeID_s,
	kSDS_GI_shopP1_int1_countName_s,
	kSDS_GI_shopP1_int1_priceName_s,
	kSDS_GI_shopP1_int1_sale_s,
	kSDS_GI_shopP1_int1_exchangeID_s,
	kSDS_GI_shop_isEvent_b,
	kSDS_GI_shopStartPack_no_i,
	kSDS_GI_shopStartPack_pID_s,
	kSDS_GI_shopStartPack_exchangeID_s,
	kSDS_GI_shopStartPack_expireSec_i,
	kSDS_GI_shopStartPack_img_s,
	kSDS_GI_shopEventPack_isHave_b,
	kSDS_GI_shopEventPack_no_i,
	kSDS_GI_shopEventPack_pID_s,
	kSDS_GI_shopEventPack_exchangeID_s,
	kSDS_GI_shopEventPack_startDate_s,
	kSDS_GI_shopEventPack_endDate_s,
	kSDS_GI_shopEventPack_startTime_i,
	kSDS_GI_shopEventPack_endTime_i,
	kSDS_GI_shopEventPack_isJustOne_b,
	kSDS_GI_shopEventPack_img_s,
	kSDS_GI_serial_int1_cardNumber_i,
	kSDS_GI_hellMode_version_i,
	kSDS_GI_hellMode_listCount_i,
	kSDS_GI_hellMode_int1_openPieceNo_i,
	kSDS_GI_hellMode_int1_cellImgInfo_s,
	kSDS_GI_hellMode_int1_pieceNo_i,
	kSDS_GI_hellMode_int1_title_s,
	kSDS_GI_hellMode_int1_content_s,
	kSDS_GI_hellMode_int1_characterNo_i,
	kSDS_GI_hellMode_int1_balance_s,
	kSDS_GI_realTimeMessageVersion_i,
	kSDS_GI_card_version_i,
	kSDS_GI_end = 1999,
	kSDS_SI_base = 2000,
	kSDS_SI_version_i,
	kSDS_SI_puzzle_i,
	kSDS_SI_playtime_i,
	kSDS_SI_scoreRate_d,
	kSDS_SI_scale_d,
	kSDS_SI_missionType_i,
	kSDS_SI_missionOptionEnergy_i,
	kSDS_SI_missionOptionCount_i,
	kSDS_SI_missionOptionPercent_i,
	kSDS_SI_missionOptionSec_i,
	kSDS_SI_shopItemsCnt_i,
	kSDS_SI_shopItems_int1_currency_s,
	kSDS_SI_shopItems_int1_type_i,
	kSDS_SI_shopItems_int1_price_i,
	kSDS_SI_defItemsCnt_i,
	kSDS_SI_defItems_int1_type_i,
	kSDS_SI_itemOptionAttackPower_i,
	kSDS_SI_itemOptionAddTimeSec_i,
	kSDS_SI_itemOptionFastSec_i,
	kSDS_SI_itemOptionSilenceSec_i,
	kSDS_SI_itemOptionDoubleItemPercent_i,
	kSDS_SI_itemOptionLongTimeSec_i,
	kSDS_SI_itemOptionBaseSpeedUpUnit_i,
	kSDS_SI_cardCount_i,
	kSDS_SI_level_int1_card_i,
	kSDS_SI_level_i,
	kSDS_SI_boss_s,
	kSDS_SI_junior_s,
	kSDS_SI_autoBalanceTry_i,
	kSDS_SI_minigame_b,
	kSDS_SI_type_s,
	kSDS_SI_end = 2999,
	kSDS_CI_base = 3000,
	kSDS_CI_int1_rank_i, // 1,2,3,4 or 3,4,5,6 etc..
	kSDS_CI_int1_grade_i, // 1,2,3,4
	kSDS_CI_int1_durability_i,
	kSDS_CI_int1_theme_i,
	kSDS_CI_int1_stage_i,
	kSDS_CI_int1_reward_i,
	kSDS_CI_int1_missile_type_s,
	kSDS_CI_int1_missile_power_i,
	kSDS_CI_int1_missile_dex_i,
	kSDS_CI_int1_missile_speed_d,
	kSDS_CI_int1_passive_s,
	kSDS_CI_int1_abilityCnt_i,
	kSDS_CI_int1_ability_int2_type_i,
	kSDS_CI_int1_abilityAttackOptionPower_i,
	kSDS_CI_int1_abilityAddTimeOptionSec_i,
	kSDS_CI_int1_abilityFastOptionSec_i,
	kSDS_CI_int1_abilitySilenceOptionSec_i,
	kSDS_CI_int1_abilityDoubleItemOptionPercent_i,
	kSDS_CI_int1_abilityLongTimeOptionSec_i,
	kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i,
	kSDS_CI_int1_imgInfo_s,
	kSDS_CI_int1_thumbnailInfo_s,
	kSDS_CI_int1_aniInfoIsAni_b,
	kSDS_CI_int1_aniInfoDetailLoopLength_i,
	kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i,
	kSDS_CI_int1_aniInfoDetailCutWidth_i,
	kSDS_CI_int1_aniInfoDetailCutHeight_i,
	kSDS_CI_int1_aniInfoDetailCutLength_i,
	kSDS_CI_int1_aniInfoDetailPositionX_i,
	kSDS_CI_int1_aniInfoDetailPositionY_i,
	kSDS_CI_int1_aniInfoDetailImg_s,
	kSDS_CI_int1_script_s,
	kSDS_CI_int1_silImgInfoIsSil_b,
	kSDS_CI_int1_silImgInfoImg_s,
	kSDS_CI_int1_profile_s,
	kSDS_CI_int1_name_s,
	kSDS_CI_int1_mPrice_ruby_i,
	kSDS_CI_int1_mPrice_pass_i,
	kSDS_CI_int1_type_i,
	kSDS_CI_int1_category_s,
	kSDS_CI_int1_level_i,
	kSDS_CI_int1_soundCnt_i,
	kSDS_CI_int1_soundType_int1_s,
	kSDS_CI_int1_characterNo_i,
	kSDS_CI_int1_serial_i,
	kSDS_CI_int1_haveFaceInfo_b,
	kSDS_CI_int1_faceInfo_s,
	kSDS_CI_int1_faceInfoCcbi_s,
	kSDS_CI_int1_faceInfoPvrccz_s,
	kSDS_CI_int1_faceInfoPlist_s,
	kSDS_CI_int1_haveAdult_b,
	kSDS_CI_int1_exp_i,
	kSDS_CI_int1_version_i,
	kSDS_CI_end = 3999,
	kSDS_AI_base = 4000,
	kSDS_AI_version_i,
	kSDS_AI_count_i,
	kSDS_AI_int1_id_i,
	kSDS_AI_int1_title_s,
	kSDS_AI_int1_content_s,
	kSDS_AI_int1_goal_i,
	kSDS_AI_int1_reward_count_i,
	kSDS_AI_int1_reward_int2_type_s,
	kSDS_AI_int1_reward_int2_count_i,
	kSDS_AI_int1_exchangeID_s,
	kSDS_AI_int1_groupNo_i,
	kSDS_AI_end = 4999
};
class SDS_SET
{
public:
	SaveDataFile f_key;
	string r_key;
};
enum SaveType
{
	kST_int = 0,
	kST_string,
	kST_double,
	kST_bool
};
class ServerDataSave : public CCObject
{
public:
	static ServerDataSave * sharedInstance ();
	bool getBoolForKey (SaveDataFile f_key, string r_key);
	bool gbfk (SDS_KEY fr_key);
	bool getBoolForKey (SaveDataFile f_key, string t_key, int key_val1);
	bool gbfk (SDS_KEY fr_key, int key_val1);
	bool getBoolForKey (SaveDataFile f_key, int i1, string r_key);
	bool gbfk (int i1, SDS_KEY fr_key);
	bool getBoolForKey (SaveDataFile f_key, int i1, string t_key, int key_val1);
	bool gbfk (int i1, SDS_KEY fr_key, int key_val1);
	void setBoolForKey (SaveDataFile f_key, string r_key, bool t_b, bool diskWrite = true);
	void sbfk (SDS_KEY fr_key, bool t_b, bool diskWrite = true);
	void setBoolForKey (SaveDataFile f_key, string r_key, int key_val1, bool t_b, bool diskWrite = true);
	void sbfk (SDS_KEY fr_key, int key_val1, bool t_b, bool diskWrite = true);
	void setBoolForKey (SaveDataFile f_key, int i1, string r_key, bool t_b, bool diskWrite = true);
	void sbfk (int i1, SDS_KEY fr_key, bool t_b, bool diskWrite = true);
	void setBoolForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, bool t_b, bool diskWrite = true);
	void sbfk (int i1, SDS_KEY fr_key, int key_val1, bool t_b, bool diskWrite = true);
	int getIntegerForKey (SaveDataFile f_key, string r_key);
	int gifk (SDS_KEY fr_key);
	int getIntegerForKey (SaveDataFile f_key, string r_key, int key_val1);
	int gifk (SDS_KEY fr_key, int key_val1);
	int getIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2);
	int gifk (SDS_KEY fr_key, int key_val1, int key_val2);
	int getIntegerForKey (SaveDataFile f_key, int i1, string r_key);
	int gifk (int i1, SDS_KEY fr_key);
	int getIntegerForKey (SaveDataFile f_key, int i1, string r_key, int key_val1);
	int gifk (int i1, SDS_KEY fr_key, int key_val1);
	void setIntegerForKey (SaveDataFile f_key, string r_key, int val1, bool diskWrite = true);
	void sifk (SDS_KEY fr_key, int val1, bool diskWrite = true);
	void setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int val1, bool diskWrite = true);
	void sifk (SDS_KEY fr_key, int key_val1, int val1, bool diskWrite = true);
	void setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, int val1, bool diskWrite = true);
	void sifk (SDS_KEY fr_key, int key_val1, int key_val2, int val1, bool diskWrite = true);
	void setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int val1, bool diskWrite = true);
	void sifk (int i1, SDS_KEY fr_key, int val1, bool diskWrite = true);
	void setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, int val1, bool diskWrite = true);
	void sifk (int i1, SDS_KEY fr_key, int key_val1, int val1, bool diskWrite = true);
	double getDoubleForKey (SaveDataFile f_key, string r_key);
	double gdfk (SDS_KEY fr_key);
	double getDoubleForKey (SaveDataFile f_key, string r_key, int key_val1);
	double gdfk (SDS_KEY fr_key, int key_val1);
	double getDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2);
	double gdfk (SDS_KEY fr_key, int key_val1, int key_val2);
	double getDoubleForKey (SaveDataFile f_key, int i1, string r_key);
	double gdfk (int i1, SDS_KEY fr_key);
	double getDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1);
	double gdfk (int i1, SDS_KEY fr_key, int key_val1);
	void setDoubleForKey (SaveDataFile f_key, string r_key, double val1, bool diskWrite = true);
	void sdfk (SDS_KEY fr_key, double val1, bool diskWrite = true);
	void setDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, double val1, bool diskWrite = true);
	void sdfk (SDS_KEY fr_key, int key_val1, double val1, bool diskWrite = true);
	void setDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, double val1, bool diskWrite = true);
	void sdfk (SDS_KEY fr_key, int key_val1, int key_val2, double val1, bool diskWrite = true);
	void setDoubleForKey (SaveDataFile f_key, int i1, string r_key, double val1, bool diskWrite = true);
	void sdfk (int i1, SDS_KEY fr_key, double val1, bool diskWrite = true);
	void setDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, double val1, bool diskWrite = true);
	void sdfk (int i1, SDS_KEY fr_key, int key_val1, double val1, bool diskWrite = true);
	string getStringForKey (SaveDataFile f_key, string r_key);
	string gsfk (SDS_KEY fr_key);
	string getStringForKey (SaveDataFile f_key, string r_key, int key_val1);
	string gsfk (SDS_KEY fr_key, int key_val1);
	string getStringForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2);
	string gsfk (SDS_KEY fr_key, int key_val1, int key_val2);
	string getStringForKey (SaveDataFile f_key, int i1, string r_key);
	string gsfk (int i1, SDS_KEY fr_key);
	string getStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1);
	string gsfk (int i1, SDS_KEY fr_key, int key_val1);
	void setStringForKey (SaveDataFile f_key, string r_key, string val1, bool diskWrite = true);
	void ssfk (SDS_KEY fr_key, string val1, bool diskWrite = true);
	void setStringForKey (SaveDataFile f_key, string r_key, int key_val1, string val1, bool diskWrite = true);
	void ssfk (SDS_KEY fr_key, int key_val1, string val1, bool diskWrite = true);
	void setStringForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, string val1, bool diskWrite = true);
	void ssfk (SDS_KEY fr_key, int key_val1, int key_val2, string val1, bool diskWrite = true);
	void setStringForKey (SaveDataFile f_key, int i1, string r_key, string val1, bool diskWrite = true);
	void ssfk (int i1, SDS_KEY fr_key, string val1, bool diskWrite = true);
	void setStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, string val1, bool diskWrite = true);
	void ssfk (int i1, SDS_KEY fr_key, int key_val1, string val1, bool diskWrite = true);
	
	void fFlush(SaveDataFile f_key);
	void fFlush(SaveDataFile f_key, int i1);
	void fFlush(SDS_KEY fr_key);
	void fFlush(int i1, SDS_KEY fr_key);
	
	void removeCache();
	
	string getRKey (SDS_KEY t_key);
	
	Json::Value getSavedServerData(Json::Value t_key_list);
	Json::Value getSavedServerDataFile(string t_filename);
	void saveServerData(Json::Value t_data_list);
	void saveServerDataFile(string t_filename, Json::Value t_data);
	
private:
	map<string, KSProtectVar<bool>> sds_cache_bool;
	map<string, KSProtectVar<bool>>::iterator iter_bool;
	map<string, KSProtectVar<int>> sds_cache_int;
	map<string, KSProtectVar<int>>::iterator iter_int;
	map<string, KSProtectVar<float>> sds_cache_float;
	map<string, KSProtectVar<float>>::iterator iter_float;
	map<string, KSProtectVar<double>> sds_cache_double;
	map<string, KSProtectVar<double>>::iterator iter_double;
	map<string, KSProtectStr> sds_cache_string;
	map<string, KSProtectStr>::iterator iter_string;
	
//	map<string, bool> sds_cache_bool;
//	map<string, bool>::iterator iter_bool;
//	map<string, int> sds_cache_int;
//	map<string, int>::iterator iter_int;
//	map<string, float> sds_cache_float;
//	map<string, float>::iterator iter_float;
//	map<string, double> sds_cache_double;
//	map<string, double>::iterator iter_double;
//	map<string, string> sds_cache_string;
//	map<string, string>::iterator iter_string;
	
	SaveData * myDefault;
	SDS_SET getKeySet (SDS_KEY t_key);
	SaveDataFile getSDF (SDS_KEY t_key);
	void myInit ();
};
#undef LZZ_INLINE
#endif
