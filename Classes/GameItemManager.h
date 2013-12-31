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
#include "GameData.h"
#include <deque>

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
	
protected:
	CCObject* target_effect;
	SEL_CallFuncCCp delegate_effect;
	virtual void acting() = 0;
	
	int holding_time;
	int frame_cnt;
	int starting_side_cnt;
	
	CCSprite* item_img;
	IntPoint myPoint;
	
	virtual void framing();
	int getSideCount();
	
	float keep_scale_value;
	
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
	
private:
	int myType;
	CCObject* target_ui;
	SEL_CallFuncCCpI delegate_takeExchangeCoin;
	CCSprite* coin_img;
	CCSprite* back_img;
	
	IntPoint myPoint;
	int directionAngle;
	float move_speed;
	
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
	
private:
	bool is_removing;
	deque<CCObject*> remove_target_list;
	
	void addGetCoinList(CCObject* t_coin);
	void startRemove();
	void removing();
	void stopRemove();
	void myInit();
};

class GameItemManager : public CCNode, public CCBAnimationManagerDelegate
{
public:
	static GameItemManager* create();
	void startItemSetting();
	void startCounting();
	void dieCreateItem();
	void showCoin(CCObject* t_ui, SEL_CallFuncCCpI d_takeExchangeCoin);
	void stopCoin();
	
	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	
	int counting_value;
	int create_counting_value;
	CLEAR_CONDITION clr_cdt_type;
	
	CCNode* coin_parent;//change coin
	FeverCoinParent* fever_coin_parent;
//	CCSpriteBatchNode* take_item_effects;
	deque<CCSprite*> effect_que;
	
	void showTakeItemEffect(CCPoint t_p);
	
	vector<ITEM_CODE> creatable_list;
	int selected_item_cnt;
	
	int double_item_cnt;
	int child_base_cnt;
	
	void counting();
	void addItem();
	void myInit();
};

#endif /* defined(__galsprototype__GameItemManager__) */
