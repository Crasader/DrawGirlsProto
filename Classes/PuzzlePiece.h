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
#include "hspConnector.h"
#include "GDWebSprite.h"
#include "KSLabelTTF.h"

USING_NS_CC;
using namespace std;

enum PieceZorder{
	kPieceZorder_level = 1,
	kPieceZorder_menu,
	kPieceZorder_stroke,
	kPieceZorder_haveInfo,
	kPieceZorder_nick
};

enum PieceMode{
	kPieceMode_thumb = 0,
	kPieceMode_default
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
		if(!is_turnable && t_pm == kPieceMode_thumb)
			return false;
		
		is_menu_enable = false;
		
		piece_mode = t_pm;
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.2f, 1.f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::setPieceImg));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.2f, 1.f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::endTurnPiece));
		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		runAction(t_seq);
		
		if(selected_img)
		{
			if(piece_mode == kPieceMode_default)
			{
				CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.2f, 1.f, 0, 0, 90, 0, 0);
				CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::selectedImgInvisible));
				CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, NULL);
				selected_img->runAction(t_seq);
			}
			else // kPieceMode_thumb
			{
				CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.2f, 1.f, 0, 0, 90, 0, 0);
				CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::selectedImgVisible));
				CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.2f, 1.f, 0, -90, 90, 0, 0);
				CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::resetCamera));
				CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, NULL);
				selected_img->runAction(t_seq);
			}
		}
		
		return true;
	}
	
	void regSelectImg(CCNode* t_img)
	{
		selected_img = t_img;
		
		if(piece_mode == kPieceMode_default)
			selectedImgInvisible();
	}
	
	void unRegSelectImg()
	{
		selected_img = NULL;
	}
	
	void simpleView()
	{
		is_menu_enable = false;
		
		piece_mode = kPieceMode_default;
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.2f, 1.f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::setPieceImgSimple));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.2f, 1.f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::endTurnPiece));
		CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		runAction(t_seq);
	}
	
	void setPieceImgSimple()
	{
		setPieceImg(true);
	}
	
	void startGetPieceAnimation (CCObject * t_create_particle, SEL_CallFuncCCp d_create_particle)
	{
		target_create_particle = t_create_particle;
		delegate_create_particle = d_create_particle;
		get_animation_mode = (PieceMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode);
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.05f, 1.f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::myChangeAction));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.05f, 1.f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::myChangeAction));
		CCSequence* t_seq1 = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		CCRepeat* t_repeat1 = CCRepeat::create(t_seq1, 10);
		CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::originalMode));
		CCSequence* t_seq4 = CCSequence::createWithTwoActions(t_repeat1, t_call4);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::showParticle));
		CCSequence* t_seq2 = CCSequence::create(t_delay1, t_call3, NULL);
		CCRepeat* t_repeat2 = CCRepeat::create(t_seq2, 5);
		CCSequence* t_seq3 = CCSequence::create(t_seq4, t_repeat2, NULL);
		runAction(t_seq3);
	}
	
	void startGetStarAnimation(int t_star, CCObject* t_end, SEL_CallFunc d_end)
	{
		(t_end->*d_end)();
		return;
		
		if(t_star == 1)
		{
			if(star1)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(t_end, d_end);
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star1->runAction(t_seq);
			}
			else
				(t_end->*d_end)();
		}
		else if(t_star == 2)
		{
			if(star2)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(t_end, d_end);
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star2->runAction(t_seq);
			}
			else
				(t_end->*d_end)();
		}
		else if(t_star == 3)
		{
			if(star3)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(t_end, d_end);
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star3->runAction(t_seq);
			}
			else
				(t_end->*d_end)();
		}
		else
			(t_end->*d_end)();
	}
	
	string getWorH()
	{
		return WorH;
	}
	
	int getLevel()
	{
		return stage_level;
	}
	
	void setTurnInfo(bool have_card1, bool have_card2, bool have_card3, bool have_card4)
	{
		is_have_card[0] = have_card1;
		is_have_card[1] = have_card2;
		is_have_card[2] = have_card3;
		is_have_card[3] = have_card4;
		is_turnable = true;
	}
	
	void initWithPieceInfo(PieceMode t_pm, PieceType t_pt, string t_WorH)
	{
		piece_mode = t_pm;
		piece_type = t_pt;
		WorH = t_WorH;
		
		setPieceImg();
	}
	
	bool is_simple;
	
	PieceMode piece_mode;
	
private:
	
	CCObject* target_create_particle;
	SEL_CallFuncCCp delegate_create_particle;
	PieceMode get_animation_mode;
	
	CCNode* selected_img;
	
	CCSprite* star1;
	CCSprite* star2;
	CCSprite* star3;
	CCSprite* star4;
	
	void resetCamera()
	{
		if(selected_img)
		{
			selected_img->getCamera()->restore();
		}
	}
	
	void selectedImgVisible()
	{
		if(selected_img)
			selected_img->setVisible(true);
	}
	
	void selectedImgInvisible()
	{
		if(selected_img)
			selected_img->setVisible(false);
	}
	
	void originalMode ()
	{
//		setPuzzleMode((PieceMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode));
	}
	void showParticle ()
	{
		(target_create_particle->*delegate_create_particle)(getPosition());
	}
	void myChangeAction ()
	{
		get_animation_mode = (PieceMode)((get_animation_mode + 1)%(kPieceMode_thumb+1));
//		setPuzzleMode(get_animation_mode);
	}
	
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
	
	bool is_have_card[4];
	
	bool is_menu_enable;
	bool is_turnable;
	
	bool is_clicked;
	
	PieceType piece_type;
	
	CCMenu* piece_menu;
	
	void setPieceImg(bool t_simple = false)
	{
		is_simple = t_simple;
		
		removeAllChildren();
		star1 = NULL;
		star2 = NULL;
		star3 = NULL;
//		if(piece_menu)
//			piece_menu->removeFromParent();
		
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		
		if(piece_type == kPieceType_color)
		{
			if(piece_mode == kPieceMode_default)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
//				if(!is_simple)
//					setStar(n_piece);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
//				if(!is_simple)
//					setStar(s_piece);
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				setStar(n_piece);
				setStageLevel(n_piece, false);
//				CCSprite* n_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
//				n_embo->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f));
//				n_piece->addChild(n_embo);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccGRAY);
				setStar(s_piece);
				setStageLevel(s_piece, false);
//				CCSprite* s_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
//				s_embo->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f));
//				s_piece->addChild(s_embo);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
		}
		else if(piece_type == kPieceType_empty)
		{
			if(piece_mode == kPieceMode_default)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_center_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
//				setStageLevel(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_center_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
//				setStageLevel(s_piece, true);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				n_piece->setGray(true);
				setStar(n_piece);
				setStageLevel(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccc3(80, 80, 80));
				setStar(s_piece);
				setStageLevel(s_piece, false);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
		}
		else if(piece_type == kPieceType_buy)
		{
			if(piece_mode == kPieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("piece_buy_" + WorH + ".png").c_str());
				CCSprite* n_type = CCSprite::create("price_gold_img.png");
				n_type->setScale(0.6f);
				n_type->setPosition(ccp(n_piece->getContentSize().width/2.f-13, n_piece->getContentSize().height/2.f-12));
				n_piece->addChild(n_type);
				CCLabelTTF* n_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 12); // 8
				n_quantity_label->setAnchorPoint(ccp(0.5,0.5));
				n_quantity_label->setPosition(ccp(n_piece->getContentSize().width/2.f+6, n_piece->getContentSize().height/2.f-12));
				n_piece->addChild(n_quantity_label);
				
				CCSprite* s_piece = CCSprite::create(("piece_buy_" + WorH + ".png").c_str());
				s_piece->setColor(ccGRAY);
				CCSprite* s_type = CCSprite::create("price_gold_img.png");
				s_type->setScale(0.6f);
				s_type->setColor(ccGRAY);
				s_type->setPosition(ccp(s_piece->getContentSize().width/2.f-13, s_piece->getContentSize().height/2.f-12));
				s_piece->addChild(s_type);
				CCLabelTTF* s_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 12); // 8
				s_quantity_label->setColor(ccGRAY);
				s_quantity_label->setAnchorPoint(ccp(0.5,0.5));
				s_quantity_label->setPosition(ccp(s_piece->getContentSize().width/2.f+6, s_piece->getContentSize().height/2.f-12));
				s_piece->addChild(s_quantity_label);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				n_piece->setGray(true);
				setStar(n_piece);
				setStageLevel(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccc3(80, 80, 80));
				setStar(s_piece);
				setStageLevel(s_piece, false);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
		}
		else if(piece_type == kPieceType_lock)
		{
			if(piece_mode == kPieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("piece_lock_" + WorH + ".png").c_str());
				CCSprite* s_piece = CCSprite::create(("piece_lock_" + WorH + ".png").c_str());
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				n_piece->setGray(true);
				setStar(n_piece);
				setStageLevel(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addNakedImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccc3(80, 80, 80));
				setStar(s_piece);
				setStageLevel(s_piece, false);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				CCSize original_size = piece_item->getContentSize();
				piece_item->setContentSize(CCSizeMake(60, 60));
				piece_item->setPosition(ccp(-(original_size.width-60)/2.f, -(original_size.height-60)/2.f));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
		}
	}
	
	void endTurnPiece()
	{
		getCamera()->restore();
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
	
	bool is_mission_stage;
	bool is_special_stage; // 과금 스테이지
	
	void myInit(int t_stage_number, int t_stage_level, CCObject* t_clicked, SEL_CallFuncI d_clicked)
	{
		star1 = NULL;
		star2 = NULL;
		star3 = NULL;
		star4 = NULL;
		
		selected_img = NULL;
		
		stage_number = t_stage_number;
		stage_level = t_stage_level;
		target_clicked = t_clicked;
		delegate_clicked = d_clicked;
		
		is_mission_stage = NSDS_GI(stage_number, kSDS_SI_missionType_i) != kCLEAR_default;
		is_special_stage = NSDS_GI(NSDS_GI(stage_number, kSDS_SI_puzzle_i), kSDS_PZ_stage_int1_condition_gold_i, stage_number) > 0;
		
		is_menu_enable = true;
	}
	
	void setStageLevel(CCSprite* piece_img, bool is_gray)
	{
		if(is_special_stage)
		{
			CCSprite* special_mark = CCSprite::create("puzzle_special.png");
			special_mark->setPosition(ccp(piece_img->getContentSize().width/2.f-20, piece_img->getContentSize().height/2.f+20));
			piece_img->addChild(special_mark);
		}
		
		if(is_mission_stage)
		{
			CCSprite* mission_mark = CCSprite::create("puzzle_mission.png");
			mission_mark->setPosition(ccp(piece_img->getContentSize().width/2.f+20, piece_img->getContentSize().height/2.f+20));
			piece_img->addChild(mission_mark);
		}
		
		KSLabelTTF* stage_number_label = KSLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 13);
		stage_number_label->setColor(ccc3(195, 255, 50));
		stage_number_label->enableOuterStroke(ccBLACK, 1.f);
		stage_number_label->setPosition(ccp(piece_img->getContentSize().width/2.f-17, piece_img->getContentSize().height/2.f-20));
		piece_img->addChild(stage_number_label);
		
//		KSLabelTTF* stage_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_stage), mySGD->getFont().c_str(), 9);
//		stage_label->setColor(ccWHITE);
//		stage_label->enableOuterStroke(ccBLACK, 1.f);
//		stage_label->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f - 7));
//		piece_img->addChild(stage_label);
	}
	
	void setStar(CCSprite* piece_img);
};

#endif /* defined(__DGproto__PuzzlePiece__) */
