// ServerDataSave.h
//

#ifndef LZZ_ServerDataSave_h
#define LZZ_ServerDataSave_h
#include "cocos2d.h"
#include "SaveData.h"

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
	kSDS_PZ_ticket_i,
	kSDS_PZ_point_i,
	kSDS_PZ_center_s,
	kSDS_PZ_original_s,
	kSDS_PZ_face_s,
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
	kSDS_PZ_end = 999,
	kSDS_GI_base = 1000,
	kSDS_GI_puzzleListVersion_i,
	kSDS_GI_puzzleListCount_i,
	kSDS_GI_puzzleList_int1_no_i,
	kSDS_GI_puzzleList_int1_title_s,
	kSDS_GI_puzzleList_int1_version_i,
	kSDS_GI_puzzleList_int1_thumbnail_s,
	kSDS_GI_eventListVersion_i,
	kSDS_GI_eventCount_i,
	kSDS_GI_event_int1_code_i,
	kSDS_GI_event_int1_thumbnail_s,
	kSDS_GI_end = 1999,
	kSDS_SI_base = 2000,
	kSDS_SI_version_i,
	kSDS_SI_puzzle_i,
	kSDS_SI_playtime_i,
	kSDS_SI_scoreRate_d,
	kSDS_SI_missionType_i,
	kSDS_SI_missionOptionEnergy_i,
	kSDS_SI_missionOptionCount_i,
	kSDS_SI_missionOptionPercent_i,
	kSDS_SI_missionOptionSec_i,
	kSDS_SI_shopItemsCnt_i,
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
	kSDS_SI_itemOptionBossLittleEnergyPercent_i,
	kSDS_SI_itemOptionSubSmallSizePercent_i,
	kSDS_SI_itemOptionSmallAreaPercent_i,
	kSDS_SI_itemOptionWidePerfectPercent_i,
	kSDS_SI_level_int1_card_i,
	kSDS_SI_level_i,
	kSDS_SI_boss_s,
	kSDS_SI_junior_s,
	kSDS_SI_autoBalanceTry_i,
	kSDS_SI_end = 2999,
	kSDS_CI_base = 3000,
	kSDS_CI_int1_rank_i,
	kSDS_CI_int1_grade_i,
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
	kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i,
	kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i,
	kSDS_CI_int1_abilitySmallAreaOptionPercent_i,
	kSDS_CI_int1_abilityWidePerfectOptionPercent_i,
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
	kSDS_CI_end = 3999
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
	void setBoolForKey (SaveDataFile f_key, string r_key, bool t_b);
	void sbfk (SDS_KEY fr_key, bool t_b);
	void setBoolForKey (SaveDataFile f_key, string r_key, int key_val1, bool t_b);
	void sbfk (SDS_KEY fr_key, int key_val1, bool t_b);
	void setBoolForKey (SaveDataFile f_key, int i1, string r_key, bool t_b);
	void sbfk (int i1, SDS_KEY fr_key, bool t_b);
	void setBoolForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, bool t_b);
	void sbfk (int i1, SDS_KEY fr_key, int key_val1, bool t_b);
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
	void setIntegerForKey (SaveDataFile f_key, string r_key, int val1);
	void sifk (SDS_KEY fr_key, int val1);
	void setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int val1);
	void sifk (SDS_KEY fr_key, int key_val1, int val1);
	void setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, int val1);
	void sifk (SDS_KEY fr_key, int key_val1, int key_val2, int val1);
	void setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int val1);
	void sifk (int i1, SDS_KEY fr_key, int val1);
	void setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, int val1);
	void sifk (int i1, SDS_KEY fr_key, int key_val1, int val1);
	double getDoubleForKey (SaveDataFile f_key, string r_key);
	double gdfk (SDS_KEY fr_key);
	double getDoubleForKey (SaveDataFile f_key, string r_key, int key_val1);
	double gdfk (SDS_KEY fr_key, int key_val1);
	double getDoubleForKey (SaveDataFile f_key, int i1, string r_key);
	double gdfk (int i1, SDS_KEY fr_key);
	double getDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1);
	double gdfk (int i1, SDS_KEY fr_key, int key_val1);
	void setDoubleForKey (SaveDataFile f_key, string r_key, double val1);
	void sdfk (SDS_KEY fr_key, double val1);
	void setDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, double val1);
	void sdfk (SDS_KEY fr_key, int key_val1, double val1);
	void setDoubleForKey (SaveDataFile f_key, int i1, string r_key, double val1);
	void sdfk (int i1, SDS_KEY fr_key, double val1);
	void setDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, double val1);
	void sdfk (int i1, SDS_KEY fr_key, int key_val1, double val1);
	string getStringForKey (SaveDataFile f_key, string r_key);
	string gsfk (SDS_KEY fr_key);
	string getStringForKey (SaveDataFile f_key, string r_key, int key_val1);
	string gsfk (SDS_KEY fr_key, int key_val1);
	string getStringForKey (SaveDataFile f_key, int i1, string r_key);
	string gsfk (int i1, SDS_KEY fr_key);
	string getStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1);
	string gsfk (int i1, SDS_KEY fr_key, int key_val1);
	void setStringForKey (SaveDataFile f_key, string r_key, string val1);
	void ssfk (SDS_KEY fr_key, string val1);
	void setStringForKey (SaveDataFile f_key, string r_key, int key_val1, string val1);
	void ssfk (SDS_KEY fr_key, int key_val1, string val1);
	void setStringForKey (SaveDataFile f_key, int i1, string r_key, string val1);
	void ssfk (int i1, SDS_KEY fr_key, string val1);
	void setStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, string val1);
	void ssfk (int i1, SDS_KEY fr_key, int key_val1, string val1);
private:
	SaveData * myDefault;
	SDS_SET getKeySet (SDS_KEY t_key);
	string getRKey (SDS_KEY t_key);
	SaveDataFile getSDF (SDS_KEY t_key);
	void myInit ();
};
#undef LZZ_INLINE
#endif
