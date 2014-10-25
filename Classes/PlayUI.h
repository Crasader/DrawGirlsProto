// PlayUI.h
//

#ifndef LZZ_PlayUI_h
#define LZZ_PlayUI_h
#include "cocos2d.h"
#include "StarGoldData.h"
#include "EnumDefine.h"
#include "DataStorageHub.h"
#include "AudioEngine.h"
#include "AlertEngine.h"
#include "CountingBMLabel.h"
#include "BossLifeGage.h"
#include "AreaGage.h"
#include "PuzzleMapScene.h"
#include "ConditionPopup.h"
#include "ServerDataSave.h"
#include "LogData.h"
#include "OnePercentGacha.h"
#include "hspConnector.h"
#include "MissileDamageData.h"
#include "KSProtect.h"
#include "KSLabelTTF.h"
#include <map>

using namespace std;
USING_NS_CC;

#define JM_CONDITION	0.02

#define MY_GRAVITY	-0.5
#define t_tta 0xD9
#define LZZ_INLINE inline

class MissionOper
{
public:
	int m_type;
	string prop;
	string oper;
	double value;
};

class KSLabelTTF;
class ComboView : public CCNode
{
public:
	static ComboView * create (int combo);
	void changeCombo (int combo);
	void setPercentage (float t_percent);
private:
	int hide_frame;
	KSLabelTTF * combo_label;
	KSLabelTTF* shadow_label;
//	CCSprite* combo_str;
//	CCSprite* combo_front;
//	CCProgressTimer * combo_timer;
	void myInit (int combo);
};
class ComboParent : public CCNode
{
public:
	static ComboParent * create (CCNode* t_score_label);
	void showCombo (int t_combo);
	void stopKeep ();
private:
	int keeping_frame;
	bool is_keeping;
	void startKeep ();
	void keeping ();
	CCNode* score_label;
	void myInit (CCNode* t_score_label);
};
class FeverParent : public CCNode
{
public:
	static FeverParent * create (CCNode* t_counting_label);
	void addFeverGage (int count);
	void endFever ();
private:
	CCParticleSystemQuad * fever_particle;
	CCProgressTimer * fever_top;
	CCSprite* fever_back;
	int recent_count;
	bool ing_fever;
	int keeping_count;
	bool is_keeping;
	int entered_fever_cnt;
	void startKeep ();
	void keeping ();
	void stopKeep ();
	CCNode* counting_label;
	void myInit (CCNode* t_counting_label);
};
class GoldLabel : public CCLabelBMFont
{
public:
	static GoldLabel * create ();
	virtual void setString (char const * after_gold);
private:
	bool is_incresing;
	float keep_gold;
	float base_gold;
	float decrease_gold;
	float increase_gold;
	string keep_gold_string;
	void startIncreasing ();
	void increasing (float dt);
	void stopIncreasing ();
	void myInit ();
};
class MyGold : public CCSprite
{
public:
	static MyGold * create ();
	void startMoving ();
private:
	float i_dy;
	float r_dy;
	CCPoint t_dv;
	float dscale;
	int z_order;
	CCPoint init_position;
	int ing_frame;
	CCPoint after_position;
	void moving ();
	void myInit ();
};
class GetGold : public CCNode
{
public:
	static GetGold * create (CCPoint t_sp, int t_duration_frame);
	void startMyAction ();
private:
	CCSpriteBatchNode * batch_node;
	int duration_frame;
	int ing_frame;
	int create_frame;
	void myAction ();
	void selfRemove ();
	void myInit (CCPoint t_sp, int t_duration_frame);
};
enum ZorderGetPercentage
{
	kZorderGetPercentage_backImg = 1,
	kZorderGetPercentage_label
};
class GetPercentage : public CCNode
{
public:
	static GetPercentage * create (float t_gp, bool is_item);
private:
	KSLabelTTF* my_label;
//	CCLabelBMFont * my_label;
//	CCSprite * backImg;
	int t_value;
	void startFadeOut ();
	void selfRemove ();
	void myInit (float t_gp, bool is_item);
};
class TakeSpeedUp : public CCNode
{
public:
	static TakeSpeedUp * create (int t_step, std::function<void()> t_end_func);
private:
//	CCSprite * backImg;
	std::function<void()> end_function;
	void startFadeOut ();
	void selfRemove ();
	void myInit (int t_step, std::function<void()> t_end_func);
};
class DetailWarning : public CCNode
{
	public:
		static DetailWarning * create (const string& fileName);
		void startAction ();
	private:
		void selfRemove ();
		void myInit (const string& fileName);
};
class Warning : public CCSprite
{
public:
	static Warning * create (int t1);
	void startAction ();
private:
	void selfRemove ();
	void myInit (int t1);
};
class TakeCoin : public CCNode
{
public:
	static TakeCoin * create ();
private:
	void startMyAction ();
	void myInit ();
};
class AreaScroll : public CCNode
{
public:
	static AreaScroll * create ();
	void startAction ();
private:
	void selfRemove ();
	void showArrow ();
	void myInit ();
};
class ChangeCard : public CCNode
{
public:
	static ChangeCard * create ();
private:
	void startMyAction();
	void myInit ();
};
enum MenuTagUI
{
	kMenuTagUI_home = 1,
	kMenuTagUI_condition
};
enum AlertTagUI
{
	kAlertTagUI_home = 1
};
enum ChildTagInPlayUI
{
	kCT_UI_clrCdtLabel = 100,
	kCT_UI_clrCdtIcon,
	kCT_UI_clrCdtPopup
};
class RollingButton;
class GraySprite;
class ConvexGraph;
class PlayUI : public CCNode
{
public:
	static PlayUI * create ();
	virtual ~ PlayUI ();
	void addScore (int t_score);
	void decreasePercentage ();
	void decreasePercentages(int t_cnt);
	float getScore ();
	float getPercentage ();
	void setPercentage (float t_p, bool t_b);
	void conditionClear ();
	void conditionFail ();
	void takeExchangeCoin (CCPoint t_start_position, int t_coin_number);
	void subBossLife (float t_life);
	void setMaxBossLife (float t_life);
	void setClearPercentage (float t_p);
	void startCounting ();
	void resumeCounting ();
	void stopCounting ();
	void keepBossLife ();
	void checkBossLife ();
	int getGameTime ();
//	void setControlTD (CCObject * t_main, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick, SEL_CallFunc d_startControl);
	void showPause ();
	void showContinuePopup (CCObject * t_end, SEL_CallFunc d_end, CCObject * t_continue, SEL_CallFunc d_continue);
	void addGameTime30Sec ();
	bool beRevivedJack ();
	void takeAddTimeItem ();
	bool getIsExchanged ();
	int getComboCnt ();
	void setComboCnt (int t_combo);
	int getUseTime ();
	void takeSilenceItem();
	void hideThumb();
	void addScoreAttack(int t_damage);
	void checkStageGoldMission(int t_gold);
	void addTurnCnt();
	void checkScoreMission();
	void hellModeResult();
	
	function<void(function<void()>, function<void()>)> clear_time_event_func;
	
	bool isGameover;
	
private:
	bool is_on_clear_time_event;
	KSProtectVar<int> score_attack_damage;
	KSProtectVar<float> score_value;
	KSProtectVar<int> damaged_score;
	int percentage_decrease_cnt;
//	CCObject * target_main;
//	SEL_CallFunc delegate_gesture;
//	SEL_CallFunc delegate_button;
//	SEL_CallFunc delegate_joystick;
//	SEL_CallFunc delegate_startControl;
	CCObject * target_continue;
	SEL_CallFunc delegate_continue;
	float bossLife;
	float keepLife;
	float maxBossLife;
	KSProtectVar<float> clearPercentage;
//	CCSprite * sand_clock;
//	BossLifeGage * m_bossLifeGage;
	AreaGage * m_areaGage;
	FeverParent * my_fp;
	ComboParent * my_combo;
	KSProtectVar<int> combo_cnt;
	int high_combo_cnt;
	KSProtectVar<int> turn_cnt;
	GoldLabel * gold_label;
	CCNode* top_center_node;
	CCLabelBMFont * score_label;
	CCLabelBMFont * percentageLabel;
	CCLabelBMFont * countingLabel;
	CCSprite * result_sprite;
	CCDictionary * exchange_dic;
	bool is_exchanged;
	bool is_show_exchange_coin;
	int taked_coin_cnt;
	bool isFirst;
	bool is_hard;
	int beforePercentage;
	KSProtectVar<int> playtime_limit;
	int detail_counting_cnt;
	bool is_urgent;
	KSProtectVar<int> countingCnt;
	int use_time;
	int total_time;
	KSProtectVar<int> jack_life;
	int jack_life_hide_count;
	CCNode* jack_life_node;
	CCArray * jack_array;
	bool is_cleared_cdt;
	CLEAR_CONDITION clr_cdt_type;
	bool is_show_condition;
	KSProtectVar<int> clr_cdt_cnt;
	KSProtectVar<int> ing_cdt_cnt;
	float clr_cdt_per;
	float clr_cdt_range;
	int save_counting_value;
	RollingButton* mission_button;
	void takeCoinModeOn ();
	void counting ();
	void lifeBonus ();
	void removeParticle (CCObject * sender);
	void createBonusScore ();
	void endGame (bool is_show_reason);
	void showGachaOnePercent ();
	void gachaOnOnePercent (float t_percent);
	void searchEmptyPosition ();
	void cancelOnePercentGacha ();
	void nextScene ();
	void catchSubCumber ();
	void takeBigArea ();
	void takeItemCollect ();
	void myInit ();
	bool isExchanged ();
	void continueAction ();
	void menuAction (CCObject * sender);
	void showCondition ();
	void closeCondition ();
	void closeShutter ();
	void endCloseShutter ();
//	void goHome ();
//	void goReplay ();
//	void cancelHome ();
	void alertAction (int t1, int t2);
	void addResultClearCCB();
	void addResultCCB(string ccb_filename);
	KSProtectVar<float> keep_percentage;
	
	ConvexGraph* progress_timer;
	
	
	CCNode* thumb_node;
	
	CCNode* endless_me_bomb_node;
	
	CCSprite* mission_back;
	vector<CCNode*> mission_clear_remove_nodes;
	
	bool is_used_heartUpItem;
	bool is_used_longTimeItem;
	
	map<int, GraySprite*> using_item_sprites;
	
	CCMenu* home_menu;
	
	int score_attack_keep_frame;
	void scoreAttackKeep();
	void scoreAttackMissile(int t_damage);
	
	void writeMap();
	void checkMapTimeVector();
	
	void writeScore();
	void checkScoreTimeVector();
	
	void writePosition();
	
	void writeDie();
	void writeImageChange();
	void writeGameOver(int t_i);
	void writeContinue();
	
	int draw_button_tutorial_show;
	CCSprite* bomb_img;
	int ing_bomb_value;
	
	map<int, MissionOper> mission_oper_list;
};
#undef LZZ_INLINE
#endif
