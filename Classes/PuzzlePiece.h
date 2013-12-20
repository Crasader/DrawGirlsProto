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
#include "GraySprite.h"
#include "DataStorageHub.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "StarGoldData.h"

USING_NS_CC;
using namespace std;

enum PieceZorder{
	kPieceZorder_level = 1,
	kPieceZorder_menu,
	kPieceZorder_stroke,
	kPieceZorder_haveInfo
};

enum PieceMode{
	kPieceMode_default = 0,
	kPieceMode_thumb
};

enum PieceType{
	kPieceType_empty = 0,
	kPieceType_grayStroke,
	kPieceType_colorStroke,
	kPieceType_color,
	kPieceType_buy,
	kPieceType_lock
};

class PuzzlePiece : public CCNode
{
public:
	static PuzzlePiece* create(int t_stage_number, int t_stage_level, CCObject* t_clicked, SEL_CallFuncI d_clicked)
	{
		PuzzlePiece* n_pp = new PuzzlePiece();
		n_pp->myInit(t_stage_number, t_stage_level, t_clicked, d_clicked);
		n_pp->autorelease();
		return n_pp;
	}
	
	bool turnPiece(PieceMode t_pm)
	{
		if(!is_turnable || piece_mode == t_pm)
			return false;
		
		piece_mode = t_pm;
		
		setPieceImg();
		
		is_menu_enable = false;
		endTurnPiece();
		return true;
	}
	
	void showTakeAnimation()
	{
		
	}
	
	string getWorH()
	{
		return WorH;
	}
	
	void setTurnInfo(bool have_card1, bool have_card2, bool have_card3)
	{
		is_have_card[0] = have_card1;
		is_have_card[1] = have_card2;
		is_have_card[2] = have_card3;
		is_turnable = true;
	}
	
	void initWithPieceInfo(PieceMode t_pm, PieceType t_pt, string t_WorH)
	{
		piece_mode = t_pm;
		piece_type = t_pt;
		WorH = t_WorH;
		
		setPieceImg();
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
	string WorH;
	
	bool is_have_card[3];
	
	bool is_menu_enable;
	bool is_turnable;
	
	bool is_clicked;
	
	PieceMode piece_mode;
	PieceType piece_type;
	
	CCMenu* piece_menu;
	
	void setPieceImg()
	{
		if(piece_menu)
			piece_menu->removeFromParent();
		
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		
		if(piece_type == kPieceType_color)
		{
			createBasicPieceMenu(false);
		}
		else if(piece_type == kPieceType_colorStroke)
		{
			createBasicPieceMenu(false);
			
			CCSprite* stroke_img = CCSprite::create(("piece_stroke_" + WorH + ".png").c_str());
			stroke_img->setPosition(CCPointZero);
			addChild(stroke_img, kPieceZorder_stroke);
		}
		else if(piece_type == kPieceType_grayStroke)
		{
			createBasicPieceMenu(true);
			
			CCSprite* stroke_img = CCSprite::create(("piece_stroke_" + WorH + ".png").c_str());
			stroke_img->setPosition(CCPointZero);
			addChild(stroke_img, kPieceZorder_stroke);
		}
		else if(piece_type == kPieceType_empty)
		{
			CCSprite* n_piece = CCSprite::create(("piece_stroke_" + WorH + ".png").c_str());
			setStageLevel(n_piece, false);
			CCSprite* s_piece = CCSprite::create(("piece_stroke_" + WorH + ".png").c_str());
			setStageLevel(s_piece, true);
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
			
			piece_menu = CCMenu::createWithItem(piece_item);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu);
		}
		else if(piece_type == kPieceType_buy)
		{
			CCSprite* n_piece = CCSprite::create(("piece_buy_" + WorH + ".png").c_str());
			CCSprite* n_type = CCSprite::create("price_gold_img.png");
			n_type->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f));
			n_piece->addChild(n_type);
			CCLabelTTF* n_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 20);
			n_quantity_label->setAnchorPoint(ccp(0.5,0.5));
			n_quantity_label->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f-10));
			n_piece->addChild(n_quantity_label);
			
			CCSprite* s_piece = CCSprite::create(("piece_buy_" + WorH + ".png").c_str());
			s_piece->setColor(ccGRAY);
			CCSprite* s_type = CCSprite::create("price_gold_img.png");
			s_type->setColor(ccGRAY);
			s_type->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f));
			s_piece->addChild(s_type);
			CCLabelTTF* s_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 20);
			s_quantity_label->setColor(ccGRAY);
			s_quantity_label->setAnchorPoint(ccp(0.5,0.5));
			s_quantity_label->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f-10));
			s_piece->addChild(s_quantity_label);
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
			
			piece_menu = CCMenu::createWithItem(piece_item);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu);
		}
		else if(piece_type == kPieceType_lock)
		{
			CCSprite* n_piece = CCSprite::create(("piece_lock_" + WorH + ".png").c_str());
			CCSprite* s_piece = CCSprite::create(("piece_lock_" + WorH + ".png").c_str());
			s_piece->setColor(ccGRAY);
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
			
			piece_menu = CCMenu::createWithItem(piece_item);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu);
		}
	}
	
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
	
	void myInit(int t_stage_number, int t_stage_level, CCObject* t_clicked, SEL_CallFuncI d_clicked)
	{
		stage_number = t_stage_number;
		stage_level = t_stage_level;
		target_clicked = t_clicked;
		delegate_clicked = d_clicked;
		
		
		
		is_menu_enable = true;
	}
	
	void setStageLevel(CCSprite* piece_img, bool is_gray)
	{
		CCSprite* level_label = CCSprite::create("level_label.png");
		level_label->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f-6));
		piece_img->addChild(level_label);
		
		CCLabelBMFont* level_count = CCLabelBMFont::create(CCString::createWithFormat("%d", stage_level)->getCString(), "levelcount.fnt");
		level_count->setAnchorPoint(ccp(0.5,0.5));
		level_count->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f+6));
		piece_img->addChild(level_count);
		
		if(!is_gray)
		{
			ccColor3B font_color = ccc3(0,0,0);
			if(stage_level <= 1)
				font_color = ccc3(255,255,255);
			else if(stage_level == 2)
				font_color = ccc3(255,255,102);
			else if(stage_level == 3)
				font_color = ccc3(255,204,102);
			else if(stage_level == 4)
				font_color = ccc3(204,255,102);
			else if(stage_level == 5)
				font_color = ccc3(102,255,204);
			else if(stage_level == 6)
				font_color = ccc3(102,204,255);
			else if(stage_level == 7)
				font_color = ccc3(204,102,255);
			else if(stage_level == 8)
				font_color = ccc3(255,111,207);
			else if(stage_level == 9)
				font_color = ccc3(255,0,128);
			else if(stage_level == 10)
				font_color = ccc3(128,0,128);
			else if(stage_level == 11)
				font_color = ccc3(128,0,0);
			else if(stage_level == 12)
				font_color = ccc3(128,64,0);
			else if(stage_level == 13)
				font_color = ccc3(128,128,0);
			else if(stage_level == 14)
				font_color = ccc3(0,128,128);
			else if(stage_level == 15)
				font_color = ccc3(0,64,128);
			else if(stage_level == 16)
				font_color = ccc3(64,0,128);
			else if(stage_level >= 17)
				font_color = ccc3(0,0,0);
			
			level_label->setColor(font_color);
			level_count->setColor(font_color);
		}
		else
		{
			ccColor3B font_color = ccc3(100,100,100);
			level_label->setColor(font_color);
			level_count->setColor(font_color);
		}
	}
	
	void setHaveCardBase(CCSprite* piece_img, bool is_gray)
	{
		CCSprite* card_base = CCSprite::create("test_map_card_base.png");
		if(is_gray)
			card_base->setColor(ccGRAY);
		card_base->setPosition(ccp(piece_img->getContentSize().width*2.f/7.f, piece_img->getContentSize().height/2.f));
		piece_img->addChild(card_base);
		for(int i=0;i<3;i++)
		{
			if(is_have_card[i])
			{
				CCSprite* card_on = CCSprite::create(CCString::createWithFormat("test_map_card%d.png", i+1)->getCString());
				if(is_gray)
					card_on->setColor(ccGRAY);
				card_on->setPosition(ccpAdd(ccp(card_base->getContentSize().width/2.f, card_base->getContentSize().height/2.f), ccp(0,18.f+i*(-18.f))));
				card_base->addChild(card_on);
			}
		}
	}
	
	void createBasicPieceMenu(bool is_gray)
	{
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		if(piece_mode == kPieceMode_default)
		{
			GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number,
																										   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
			GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number,
																										   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
			if(is_gray)
			{
				n_piece->setGray(true);
				s_piece->setColor(ccc3(100,100,100));
			}
			else
				s_piece->setColor(ccGRAY);
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
			piece_menu = CCMenu::createWithItem(piece_item);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu, kPieceZorder_menu);
		}
		else if(piece_mode == kPieceMode_thumb)
		{
			GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																										   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
			setHaveCardBase(n_piece, false);
			GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																										   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
			s_piece->setColor(ccGRAY);
			setHaveCardBase(s_piece, true);
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
			piece_menu = CCMenu::createWithItem(piece_item);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu, kPieceZorder_menu);
		}
	}
};

#endif /* defined(__DGproto__PuzzlePiece__) */
