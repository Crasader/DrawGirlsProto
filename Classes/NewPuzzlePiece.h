//
//  NewPuzzlePiece.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 8..
//
//

#ifndef __DGproto__NewPuzzlePiece__
#define __DGproto__NewPuzzlePiece__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum NewPuzzlePieceMode
{
	kNewPuzzlePieceMode_default = 0,
	kNewPuzzlePieceMode_thumbnail
//	kNewPuzzlePieceMode_ranker
};

class ScrollMenu;
class NewPuzzlePiece : public CCNode
{
public:
	static NewPuzzlePiece* create(int t_stage_number, function<void(int)> t_clicked_func, NewPuzzlePieceMode t_init_mode, bool t_is_buy, bool t_is_lock)
	{
		NewPuzzlePiece* t_npp = new NewPuzzlePiece();
		t_npp->myInit(t_stage_number, t_clicked_func, t_init_mode, t_is_buy, t_is_lock);
		t_npp->autorelease();
		return t_npp;
	}
	
	void turnPiece(NewPuzzlePieceMode t_mode)
	{
		recent_mode = t_mode;
		
//		setPieceImg(recent_mode);
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.1f, 1.f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::originalMode));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.1f, 1.f, 0, -90, 90, 0, 0);
		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, NULL);
		
		runAction(t_seq);
	}
	
	void startGetPieceAnimation(function<void(CCPoint)> t_create_particle_func)
	{
		create_particle_func = t_create_particle_func;
		
		turn_mode = kNewPuzzlePieceMode_default;
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.05f, 1.f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::myChangeAction));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.05f, 1.f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::myChangeAction));
		CCSequence* t_seq1 = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		CCRepeat* t_repeat1 = CCRepeat::create(t_seq1, 10);
		CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::originalMode));
		CCSequence* t_seq4 = CCSequence::createWithTwoActions(t_repeat1, t_call4);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::showParticle));
		CCSequence* t_seq2 = CCSequence::create(t_delay1, t_call3, NULL);
		CCRepeat* t_repeat2 = CCRepeat::create(t_seq2, 5);
		CCSequence* t_seq3 = CCSequence::create(t_seq4, t_repeat2, NULL);
		runAction(t_seq3);
	}
	
	void startGetStarAnimation(int t_star, function<void(void)> t_end_func)
	{
		star_animation_end_func = t_end_func;
		
		if(t_star == 1)
		{
			if(star1)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::endStarAnimation));
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star1->runAction(t_seq);
			}
			else
				t_end_func();
		}
		else if(t_star == 2)
		{
			if(star2)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::endStarAnimation));
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star2->runAction(t_seq);
			}
			else
				t_end_func();
		}
		else if(t_star == 3)
		{
			if(star3)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(NewPuzzlePiece::endStarAnimation));
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star3->runAction(t_seq);
			}
			else
				t_end_func();
		}
		else
			t_end_func();
	}
	
private:
	int stage_number;
	function<void(int)> clicked_func;
	function<void(CCPoint)> create_particle_func;
	function<void(void)> star_animation_end_func;
	NewPuzzlePieceMode recent_mode;
	NewPuzzlePieceMode turn_mode;
	
	bool is_open_puzzle;
	bool is_buy;
	bool is_lock;
	
	CCSprite* star1;
	CCSprite* star2;
	CCSprite* star3;
	
	ScrollMenu* piece_menu;
	
	bool is_menu_enable;
	
	void myInit(int t_stage_number, function<void(int)> t_clicked_func, NewPuzzlePieceMode t_init_mode, bool t_is_buy, bool t_is_lock);
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		clicked_func(stage_number);
	}
	
	void endStarAnimation()
	{
		star_animation_end_func();
	}
	
	void showParticle()
	{
		create_particle_func(getPosition());
	}
	
	void originalMode()
	{
		setPieceImg(recent_mode);
	}
	
	void myChangeAction()
	{
		if(turn_mode == kNewPuzzlePieceMode_default)
			turn_mode = kNewPuzzlePieceMode_thumbnail;
		else if(turn_mode == kNewPuzzlePieceMode_thumbnail)
			turn_mode = kNewPuzzlePieceMode_default;
		
		setPieceImg(turn_mode);
	}
	
	void setPieceImg(NewPuzzlePieceMode t_mode);
//	void createRanker();
	void setStageLevel(CCSprite* piece_img);
};


#endif /* defined(__DGproto__NewPuzzlePiece__) */
