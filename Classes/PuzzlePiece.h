//
//  PuzzlePiece.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 19..
//
//

#ifndef __DGproto__PuzzlePiece__
#define __DGproto__PuzzlePiece__

#include "cocos2d.h"
#include "SelectorDefine.h"

USING_NS_CC;
using namespace std;

enum PieceZorder{
	kPieceZorder_level = 1,
	kPieceZorder_menu,
	kPieceZorder_stroke,
	kPieceZorder_haveInfo
};

class PuzzlePiece : public CCNode
{
public:
	static PuzzlePiece* create(int t_stage_number, int t_stage_level, CCObject* t_clicked, SEL_CallFuncI d_clicked, bool t_turnable)
	{
		PuzzlePiece* n_pp = new PuzzlePiece();
		n_pp->myInit(t_stage_number, t_stage_level, t_clicked, d_clicked, t_turnable);
		n_pp->autorelease();
		return n_pp;
	}
	
	bool turnPiece()
	{
		if(!is_turnable)
			return false;
		
		//
		
		is_menu_enable = false;
		return true;
	}
	
	void showTakeAnimation()
	{
		
	}
	
	void setChangeInfo()
	{
		
	}
	
private:
	
	/*
	 3단계 -> 클릭 가능, 턴 가능 / stage number
	 2단계 -> 클릭 가능, 턴 가능 / stage number
	 1단계 -> 클릭 가능, 턴 가능 / stage number
	 없음  -> 클릭 가능, 턴 불가 / stage number, stage level
	 buy  -> 클릭 가능(buy 함수로 연결), 턴 불가 / stage number, stage level(open시 필요)
	 lock -> 클릭 가능(lock 함수로 연결), 턴 불가 / stage number
	 */
	
	int stage_number; // menu enable
	int stage_level; // empty
	
	int is_have_card[3];
	
	bool is_menu_enable;
	bool is_turnable;
	
	void endTurnPiece()
	{
		is_menu_enable = true;
	}
	
	CCObject* target_clicked;
	SEL_CallFuncI delegate_clicked;
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		is_menu_enable = false;
		
		(target_clicked->*delegate_clicked)(stage_number);
		
		is_menu_enable = true;
	}
	
	void myInit(int t_stage_number, int t_stage_level, CCObject* t_clicked, SEL_CallFuncI d_clicked, bool t_turnable)
	{
		stage_number = t_stage_number;
		stage_level = t_stage_level;
		target_clicked = t_clicked;
		delegate_clicked = d_clicked;
		is_turnable = t_turnable;
		
		
		
	}
};

#endif /* defined(__DGproto__PuzzlePiece__) */
