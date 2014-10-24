//
//  GameItemManager.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#ifndef __galsprototype__GameItemManager__
#define __galsprototype__GameItemManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <deque>
#include "SelectorDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum GameItemType{
	kGIT_speedup = 1,
	kGIT_attack,
	kGIT_fast,
	kGIT_bomb
};

enum GameItemAttackType{
	kGIAT_empty = 0,
	kGIAT_life,
	kGIAT_fire,
	kGIAT_water,
	kGIAT_wind,
	kGIAT_lightning,
	kGIAT_plasma
};

enum GameItemBombType{
	kGIBT_life = 1,
	kGIBT_fire,
	kGIBT_water,
	kGIBT_wind,
	kGIBT_lightning,
	kGIBT_plasma
};

class GameItemBase : public CCNode
{
public:
	virtual void selfRemove();
	virtual void startFraming();
	void setTakeEffectFunc(CCObject* t_effect, SEL_CallFuncCCp d_effect);
	GameItemBase();
	
protected:
	CCObject* target_effect;
	SEL_CallFuncCCp delegate_effect;
	virtual void acting();
	
	float m_absorb_distance;
	
	int holding_time;
	int frame_cnt;
	int starting_side_cnt;
	
	CCSprite* item_img;
	IntPoint myPoint;
	
	virtual void framing();
	int getSideCount();
	
	float keep_scale_value;
	
	void startTraceCharacter();
	void traceCharacter();
	void stopTraceCharacter();
	
	void startHide();
	void setMyPoint(bool is_near);
};

class GameItemAddTime : public GameItemBase
{
public:
	static GameItemAddTime* create(bool is_near);
	virtual void selfRemove();
	
private:
	
	virtual void acting();
	void myInit(bool is_near);
};

class GameItemSpeedUp : public GameItemBase
{
public:
	static GameItemSpeedUp* create(bool is_near);
	
private:
	
	virtual void acting();
	void myInit(bool is_near);
};

class GameItemFast : public GameItemBase
{
public:
	static GameItemFast* create(bool is_near);
	
private:
	
	virtual void acting();
	void ending();
	void myInit(bool is_near);
};

class GameItemAttack : public GameItemBase
{
public:
	static GameItemAttack* create(bool is_near);
	
private:
	int my_elemental;
	
	virtual void acting();
	void myInit(bool is_near);
};

class GameItemSubOneDie : public GameItemBase
{
public:
	static GameItemSubOneDie* create(bool is_near);
	
private:
	
	virtual void acting();
	void myInit(bool is_near);
};

class GameItemSilence : public GameItemBase
{
public:
	static GameItemSilence* create(bool is_near);
	
private:
	
	virtual void acting();
	void finalAction();
	void myInit(bool is_near);
};

class GameItemFire : public GameItemBase
{
public:
	static GameItemFire* create(bool is_near);
	
private:
	int my_elemental;
	int damage_frame;
	int ing_frame;
	
	float damage;
	float total_damage;
	float sum_damage;
	
	virtual void acting();
	void attack(float t_damage);
	void resetChecking();
	
	void myInit(bool is_near);
};

class ExchangeCoin : public CCNode
{
public:
	static ExchangeCoin* create(int t_type, CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin);
	void showCoin();
	void stopMoving();
	void smallScaleHiding();
	bool isLocked();
	
private:
	int myType;
	CCObject* target_ui;
	SEL_CallFuncCCpI delegate_takeExchangeCoin;
	CCSprite* coin_img;
	CCSprite* back_img;
	
	bool is_hiding;
	bool is_cution;
	void cutionAction();
	void endCutionAction();
	
	IntPoint myPoint;
	int directionAngle;
	float move_speed;
	
	void startTraceCharacter();
	void traceCharacter();
	void stopTraceCharacter();
	
	void startMoving();
	void endTakeAction();
	void changeBack();
	void changeFront();
	void moving();
	void myInit(int t_type, CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin);
};

class FeverCoin : public CCSprite
{
public:
	static FeverCoin* create(IntPoint t_point, CCObject* t_add, SEL_CallFuncO d_add);
	void startRemove();
	
	bool is_stan_by;
	
private:
	IntPoint my_point;
	CCObject* target_add;
	SEL_CallFuncO delegate_add;
	int remove_frame;
	
	void removing();
	void startCheck();
	void checking();
	void stopCheck();
	void myInit(IntPoint t_point, CCObject* t_add, SEL_CallFuncO d_add);
};

class FeverCoinParent : public CCSpriteBatchNode
{
public:
	static FeverCoinParent* create();
	void startFever();
	void stopFever();
	bool is_fevering;
	
private:
	bool is_removing;
	deque<CCObject*> remove_target_list;
	float weight_value;
	
	void addGetCoinList(CCObject* t_coin);
	void startRemove();
	void removing();
	void stopRemove();
	void myInit();
};

class FloatingCoin : public CCSprite
{
public:
	static FloatingCoin* create(function<void(CCPoint)> t_take_func, int t_gold, CCPoint t_start_point, bool t_auto_take = false);
	
	void hideAction();
	
	FloatingCoin();
	
private:
	
	float m_absorb_distance;
	
	int m_gold;
	float moving_direction; // -180 <= ~ < 180
	float moving_speed; // 2 <= ~ < 5
	
	bool auto_take;
	int auto_take_frame;
	
	int ing_frame;
	bool is_locked;
	IntPoint locked_point;
	int absorb_frame;
	
	int keeping_count;
	
//	CCSprite* coin_img;
	
	float start_speed;
	
	void onLock();
	void startAsLonging();
	void asLonging();
	
	bool isAsLong();
	
	void startTraceCharacter();
	void traceCharacter();
	void stopTraceCharacter();
	
	void startAbsorbChecking();
	void absorbChecking();
	void startFloating();
	void floating();
	
	void startTing();
	void ting();
	
	float ting_y;
	
	function<void(CCPoint)> take_func;
	void takeIt();
	
	void myInit(function<void(CCPoint)> t_take_func, int t_gold, CCPoint t_start_point, bool t_auto_take);
};

class FloatingCoinCreator : public CCNode
{
public:
	static FloatingCoinCreator* create(CCSpriteBatchNode* t_add_parent, function<void(CCPoint)> t_take_func, int t_frame, int t_count, int t_gold, CCPoint t_start_point, bool t_auto_take = false);
	
private:
	int m_frame;
	int m_count;
	int m_gold;
	CCPoint start_point;
	int ing_frame;
	int ing_count;
	CCNode* add_parent;
	function<void(CCPoint)> take_func;
	bool auto_take;
	
	void startCreate();
	void creating();
	void myInit(CCSpriteBatchNode* t_add_parent, function<void(CCPoint)> t_take_func, int t_frame, int t_count, int t_gold, CCPoint t_start_point, bool t_auto_take);
};

class FloatingCoinParent : public CCNode
{
public:
	static FloatingCoinParent* create(function<void(CCPoint)> t_take_func);
	
	void showPercentFloatingCoin(float t_percent);
	void showAttackFloatingCoin(CCPoint t_target_point, int t_coin_count);
	void hideAllFloatingCoin();
	void startClearFloatCoin(float t_percent);
	
private:
	CCSpriteBatchNode* coin_node;
	CCNode* creator_node;
	
	function<void(CCPoint)> take_func;
	
	void myInit(function<void(CCPoint)> t_take_func);
};

class TakeEffectManager : public CCNode
{
public:
	static TakeEffectManager* create();
	CCSprite* t_4_1;
	CCSprite* t_4_2;
	CCSprite* t_1_1;
	CCSprite* t_2;
	CCSprite* t_5;
	CCSprite* t_1_2;
	CCSprite* t_3;
	CCSprite* t_7;
	
	void removeMyAction();
	void startAnimation();
private:
};

class GameItemManager : public CCNode, public CCBAnimationManagerDelegate
{
public:
	static GameItemManager* create();
	void startItemSetting();
	void startCounting();
	void stopCounting();
	void dieCreateItem();
	void showCoin(CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin);
	void stopCoin();
	
	virtual void completedAnimationSequenceNamed (char const * name);
	
	bool getIsFevering();
	
	void showBeautyStone();
	void removeBeautyStone();
	
	void showTakeItemEffect(CCPoint t_p);
	
private:
	CCSpriteBatchNode* take_effect_node;
	
	int counting_value;
	int create_counting_value;
	CLEAR_CONDITION clr_cdt_type;
	
	CCNode* coin_parent;//change coin
	FeverCoinParent* fever_coin_parent;
//	CCSpriteBatchNode* take_item_effects;
	FloatingCoinParent* floating_coin_parent;
	deque<CCSprite*> effect_que;
	deque<CCBAnimationManager*> effect_animation_manager;
	
	bool is_on_addTime;
	
	bool isChangeAllInner();
	
	vector<ITEM_CODE> creatable_list;
	vector<CCNode*> beauty_stone_list;
	int selected_item_cnt;
	
	int double_item_cnt;
	int child_base_cnt;
	
	void counting();
	void addItem();
	void myInit();
};

#endif /* defined(__galsprototype__GameItemManager__) */
