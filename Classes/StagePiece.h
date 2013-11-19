//
//  StagePiece.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 22..
//
//

#ifndef __DGproto__StagePiece__
#define __DGproto__StagePiece__

#include "cocos2d.h"
#include "StageImgLoader.h"
#include "DataStorageHub.h"
#include "GraySprite.h"
#include "ScrollMenu.h"
#include "PuzzleCache.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"

USING_NS_CC;
using namespace std;

enum PuzzleMode{
	kPM_default = 0,
	kPM_thumb
};

class StagePiece : public CCNode
{
public:
	static StagePiece* create(string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder, string t_type, CCObject* t_t, SEL_MenuHandler t_d)
	{
		StagePiece* t_sp = new StagePiece();
		t_sp->myInit(t_piece, t_number, t_level, t_p, t_rect, t_gray, t_boarder, t_type, t_t, t_d);
		t_sp->autorelease();
		return t_sp;
	}
	
	bool isInnerRect(CCPoint t_p)
	{
		CCPoint location = convertToNodeSpace(t_p);
		
		return touch_rect.containsPoint(location);
	}
	
	void setChangable(string t_thumb, bool t_card1, bool t_card2, bool t_card3)
	{
		is_changable = true;
		thumb_name = t_thumb.c_str();
		is_have_card[0] = t_card1;
		is_have_card[1] = t_card2;
		is_have_card[2] = t_card3;
	}
	
	bool isChangable(){		return is_changable;	}
	void setBackPuzzle()
	{
		piece_img = GraySprite::createWithTexture(PuzzleCache::getInstance()->getImage(piece_name.c_str())->makeTexture());
		piece_img->setGray(is_gray);
		addChild(piece_img);
		
		if(is_boarder)
		{
			string boarder_filename;
			bool is_long = (piece_type == "h");
			
			if(is_long)			boarder_filename = "test_map_boarder_long.png";
			else				boarder_filename = "test_map_boarder_wide.png";
			
			CCSprite* boarder = CCSprite::create(boarder_filename.c_str());
			boarder->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
			piece_img->addChild(boarder);
		}
	}
	void setPuzzleMode(PuzzleMode t_mode)
	{
		if(!is_changable)		return;
		if(piece_img)
		{
			piece_img->removeFromParent();
			piece_img = NULL;
		}
		
		if(t_mode == kPM_default)
		{
			piece_img = GraySprite::createWithTexture(PuzzleCache::getInstance()->getImage(piece_name.c_str())->makeTexture());
			piece_img->setGray(is_gray);
			addChild(piece_img);
			
			if(is_boarder)
			{
				string boarder_filename;
				bool is_long = (piece_type == "h");
				
				if(is_long)			boarder_filename = "test_map_boarder_long.png";
				else				boarder_filename = "test_map_boarder_wide.png";
				
				CCSprite* boarder = CCSprite::create(boarder_filename.c_str());
				boarder->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
				piece_img->addChild(boarder);
			}
		}
		else if(t_mode == kPM_thumb)
		{
			piece_img = GraySprite::createWithTexture(PuzzleCache::getInstance()->getImage(thumb_name.c_str())->makeTexture());
			addChild(piece_img);
			
			//			if(is_boarder)
			//			{
			string boarder_filename;
			bool is_long = (piece_type == "h");
			
			if(is_long)			boarder_filename = "test_map_boarder_long.png";
			else				boarder_filename = "test_map_boarder_wide.png";
			
			CCSprite* boarder = CCSprite::create(boarder_filename.c_str());
			boarder->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
			piece_img->addChild(boarder);
			//			}
			
			CCSprite* card_base = CCSprite::create("test_map_card_base.png");
			card_base->setPosition(ccp(piece_img->getContentSize().width*2.f/7.f, piece_img->getContentSize().height/2.f));
			piece_img->addChild(card_base);
			
			for(int i=0;i<3;i++)
			{
				if(is_have_card[i])
				{
					CCSprite* card_on = CCSprite::create(CCString::createWithFormat("test_map_card%d.png", i+1)->getCString());
					card_on->setPosition(ccpAdd(card_base->getPosition(), ccp(0,18.f+i*(-18.f))));
					piece_img->addChild(card_on);
				}
			}
		}
	}
	
	void mySetTouchEnable(bool t_b)
	{
		s_menu->setTouchEnabled(t_b);
		if(piece_img)
			setTouchCancel();
//		if(t_b)
//			s_menu->setHandlerPriority(kCCMenuHandlerPriority+2);
	}
	
	bool touchBegan(CCTouch* touch, CCEvent* event)
	{
		return s_menu->ccTouchBegan(touch, event);
	}
	
	void touchMoved(CCTouch* touch, CCEvent* event)
	{
		s_menu->ccTouchMoved(touch, event);
	}
	
	void touchEnded(CCTouch* touch, CCEvent* event)
	{
		s_menu->ccTouchEnded(touch, event);
	}
	
	void touchCancelled(CCTouch* touch, CCEvent* event)
	{
		s_menu->ccTouchCancelled(touch, event);
		setTouchCancel();
	}
	
	int getStageNumber(){	return stage_number;	}
	
	void setTouchBegin(){	piece_img->setColor(ccGRAY);	}
	void setTouchCancel(){	piece_img->setColor(ccWHITE);	}
	
	bool isBoarder(){	return is_boarder;	}
	
	void startGetPieceAnimation(CCObject* t_create_particle, SEL_CallFuncCCp d_create_particle)
	{
		target_create_particle = t_create_particle;
		delegate_create_particle = d_create_particle;
		get_animation_mode = (PuzzleMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode);
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.05f, 1.f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(StagePiece::myChangeAction));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.05f, 1.f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(StagePiece::myChangeAction));
		CCSequence* t_seq1 = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		CCRepeat* t_repeat1 = CCRepeat::create(t_seq1, 10);
		CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(StagePiece::originalMode));
		CCSequence* t_seq4 = CCSequence::createWithTwoActions(t_repeat1, t_call4);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(StagePiece::showParticle));
		CCSequence* t_seq2 = CCSequence::create(t_delay1, t_call3, NULL);
		CCRepeat* t_repeat2 = CCRepeat::create(t_seq2, 5);
		CCSequence* t_seq3 = CCSequence::create(t_seq4, t_repeat2, NULL);
		runAction(t_seq3);
	}
	
	CCSprite* shadow_node;
	
private:
	PuzzleMode get_animation_mode;
	
	GraySprite* piece_img;
	
	CCObject* target_create_particle;
	SEL_CallFuncCCp delegate_create_particle;
	
	string piece_name;
	int stage_number;
	int stage_level;
	CCRect touch_rect;
	bool is_gray;
	bool is_boarder;
	string piece_type;
	
	bool is_changable;
	string thumb_name;
	bool is_have_card[3];
	
	ScrollMenu* s_menu;
	
	void originalMode()
	{
		setPuzzleMode((PuzzleMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode));
	}
	
	void showParticle()
	{
		(target_create_particle->*delegate_create_particle)(getPosition());
	}
	
	void myChangeAction()
	{
		get_animation_mode = (PuzzleMode)((get_animation_mode + 1)%(kPM_thumb+1));
		setPuzzleMode(get_animation_mode);
	}
	
	void myInit(string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder, string t_type, CCObject* t_t, SEL_MenuHandler t_d)
	{
		piece_name = t_piece.c_str();
		stage_number = t_number;
		stage_level = t_level;
		touch_rect = t_rect;
		is_gray = t_gray;
		is_boarder = t_boarder;
		is_changable = false;
		piece_type = t_type.c_str();
		
		setPosition(t_p);
		
		piece_img = NULL;
		
		if(piece_name == "test_puzzle_empty.png")
		{
			piece_img = GraySprite::create(piece_name.c_str());
			piece_img->setGray(is_gray);
			addChild(piece_img);
			
			CCSprite* level_label = CCSprite::create("level_label.png");
			level_label->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f-6));
			piece_img->addChild(level_label);
			
			CCLabelBMFont* level_count = CCLabelBMFont::create(CCString::createWithFormat("%d", stage_level)->getCString(), "levelcount.fnt");
			level_count->setAnchorPoint(ccp(0.5,0.5));
			level_count->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f+6));
			piece_img->addChild(((CCLabelBMFont*)level_count));
			
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
			
//			CCSprite* level_img = CCSprite::create(CCString::createWithFormat("test_map_level%d.png", stage_level)->getCString());
//			level_img->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
//			piece_img->addChild(level_img);
		}

		CCSprite* n_touch = CCSprite::create("whitePaper.png", CCRectMake(0, 0, touch_rect.size.width, touch_rect.size.height));
		n_touch->setOpacity(0);
		CCSprite* s_touch = CCSprite::create("whitePaper.png", CCRectMake(0, 0, touch_rect.size.width, touch_rect.size.height));
		s_touch->setOpacity(0);
		s_touch->setColor(ccRED);

		CCMenuItem* s_item = CCMenuItemSprite::create(n_touch, s_touch, t_t, t_d);
		s_item->setTag(stage_number);

		s_menu = ScrollMenu::create(s_item, NULL);
		s_menu->setPosition(CCPointZero);
		addChild(s_menu);
	}
};

#endif /* defined(__DGproto__StagePiece__) */
