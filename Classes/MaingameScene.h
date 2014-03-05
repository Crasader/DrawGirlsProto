//
//  MaingameScene.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#ifndef DrawingJack_MaingameScene_h
#define DrawingJack_MaingameScene_h

#include "cocos2d.h"
#include "GameData.h"
#include "Jack.h"
#include "MapScanner.h"
#include "PathManager.h"
#include "Cumber.h"
#include "PlayUI.h"
#include "AttackItem.h"
#include "ControlType.h"
#include "ClearShowTime.h"
#include "GameItemManager.h"
#include "ZoomScriptScene.h"
#include "MissileDamageLabel.h"

using namespace cocos2d;

enum Mainzorder{
	topBottomZorder = 1,
	myMSZorder,
	myPMZorder,
	attackItemZorder,
	myCPZorder,
	myJackZorder,
	controlImgZorder,
	countingLabelZorder,
	conditionLabelZorder,
	goldZorder,
	searchEyeZorder,
	clearshowtimeZorder,
	myUIZorder,
	screenNodeZorder,
	startGachaZorder,
	mControlZorder,
	particleZorder,
	shutterZorder
};

enum MainInitState{
	kMIS_beforeInit = 1,
	kMIS_movingGameNode,
	kMIS_randomRectView,
	kMIS_startGame
};

enum GameNodeMovingDirection{
	kGNMD_up = -1,
	kGNMD_down = 1
};

class SearchEye;
class Maingame : public CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(Maingame);
	
	void touchEnd();
	
	void touchOn();
	
	virtual void onEnter();
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	
	void controlStunOff();
	virtual ~Maingame()
	{
		sub_thumbs->release();
	}
		
private:
	
	PlayUI* myUI;
	
	CCNode* game_node;
	
	Jack* myJack;
	MapScanner* myMS;
	PathManager* myPM;
	CumberParent* myCP;
	
	CCParticleSystemQuad* line_particle;
	
	CCSprite* condition_spr;
	CCSprite* touch_img;
	
	GameItemManager* myGIM;
	
	CCSprite* bottom_shutter;
	CCSprite* top_shutter;
	
	ControlCommon* mControl;
	
	MainInitState init_state;
	GameNodeMovingDirection gamenode_moving_direction;
	
	void setLineParticle(bool t_b);
	
	void movingGameNode();
	int ignore_cnt;
	void randomingRectView();
	void finalSetting();
	void startScene();
	void checkAcceptChallenge();
	void checkFriendCard();
	
	void setControlGesture();
	void setControlButton();
	void setControlJoystick();
	void setControlJoystickButton();
	
	void startControl();
	
	void removeConditionLabel();
	
	int countingCnt;
	
	void startCounting();
	void counting();
	
	void gachaOn();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
	
	virtual void keyBackClicked();
	void alertAction(int t1, int t2);
	
	bool isCheckingBacking;
	
	void checkingBacking();
	
	void stunBackTracking();
	void stunBacking();
	
	void readyBackTracking();
	void startBackTracking();
	void stopBackTracking();
	void backTracking();
	
	void pauseBackTracking();
	
	void allStopSchedule();
	
	void startSpecialAttack();
	
	void stopSpecialAttack()
	{
//		mControl->isStun = false;
//		((ControlJoystickButton*)mControl)->resetTouch();
//		mControl->setTouchEnabled(true);
	}
	
	void gameover();
	
	void closeShutter();
	
	void shutterClosedSound()
	{
//		AudioEngine::sharedInstance()->playEffect("sound_shutter_closed.m4a", false);
	}
	
	void endCloseShutter();
	
	int move_to_boss_position_frame;
	CCPoint move_to_boss_position_ds;
	void startMoveToBossPosition();
	void startMoveToCCPoint(CCPoint t_point);
	void moveToBossPosition();
	
	CCPoint getObjectToGameNodePosition(CCPoint t_p);
	CCPoint getObjectToGameNodePositionCoin(CCPoint t_p);
	CCPoint getGameNodePosition();
	CCPoint getGameNodeToObjectPosition(CCPoint t_p);
	
	void moveGamePosition(CCPoint t_p);
	
	void goldGettingEffect(CCPoint t_p, int t_i);
	void percentageGettingEffect(float t_f, bool t_b, CCPoint t_p);
	void takeSpeedUpEffect(int t_step);
	int recent_take_speed_up_value;
	CCNode* save_take_speed_up_effect;
	void endTakeSpeedUpEffect();
	
	void showMissMissile(CCPoint t_position);
	void showDamageMissile(CCPoint t_position, int t_damage);
	CCSprite* combo_string_img;
	CCLabelBMFont* combo_value_img;
	void showComboImage(CCPoint t_position, int t_combo_value);
	void removeComboImage(CCObject* t_node);
	
	
	bool is_line_die;
	
	void showLineDiePosition(IntPoint t_p);
	
	void removeLineDieEffect(CCObject* t_obj);
	
	void resetIsLineDie();
	
	void showWarning(int t1);
	void showDetailMessage(const std::string& fileName);
	void showThumbWarning(CCPoint t_point);
	int warning_count;
	CCNode* screen_side_warning_node;
	void showScreenSideWarning();
	void hideScreenSideWarning();
	void showTextMessage(const std::string& msg);	
	void showTakeCoin();
	CCNode* gameNodePointer();
	void showChangeCard();
	
	void showCoin();
	
	void takeExchangeCoin(CCPoint t_start_position, int t_coin_number);
	
	void startExchange();
	
	void setLimitMap();
	
	CCPoint limitted_map_position;
	int changing_game_step_frame;
	void setUnlimitMap();
	
	void changingGameStep();
	
	void gameNodeChangingGameStep(CCPoint t_p, int t_step);
	
	void showAreaScroll();
	
	int shake_frame;
	void startShake(float t_direction_angle);
	
	void shaking();
	void stopShake();
	
	vector<SearchEye*> search_eye_vector;
//	SearchEye* search_eye;
	
	CCNode* screen_node;
	
	CCSprite* character_thumb;
	CCArray* boss_thumbs;
	CCArray* sub_thumbs;
	CCPoint thumb_base_position;
	CCRenderTexture* thumb_texture;
	CCSprite* thumb_case_top;
	CCSprite* thumb_case_down;
	CCSprite* thumb_case_left;
	CCSprite* thumb_case_right;
	void refreshThumb();
	
	CCRenderTexture* replay_thumb_texture;
	void refreshReplayThumb(int temp_time);
	
	CCLabelTTF* replay_score;
	void refreshReplayScore(int temp_time);
	
	CCSprite* replay_character;
	CCArray* replay_boss;
	CCArray* replay_sub;
	void refreshReplayPosition(int temp_time);
	
	CCNode* replay_all_node;
	
	int replay_continue_count;
	CCLabelTTF* replay_continue_label;
	
	void hideThumb();
	
	void showDrawButtonTutorial();
	void hideDrawButtonTutorial();
	
	bool is_gohome;
	void showPause();
	void goHome ();
	void goReplay ();
	void cancelHome ();
	
	void showContinue(CCObject * t_end, SEL_CallFunc d_end, CCObject * t_continue, SEL_CallFunc d_continue);
	void continueAction();
	
};

#endif
