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

USING_NS_CC;
using namespace std;

enum PuzzleMode{
	kPM_default = 0,
	kPM_thumb
};

class StagePiece : public CCNode
{
public:
	static StagePiece* create(string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder, string t_type)
	{
		StagePiece* t_sp = new StagePiece();
		t_sp->myInit(t_piece, t_number, t_level, t_p, t_rect, t_gray, t_boarder, t_type);
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
			piece_img = GraySprite::createWithTexture(mySIL->addImage(piece_name.c_str()));
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
			piece_img = GraySprite::createWithTexture(mySIL->addImage(thumb_name.c_str()));
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
	
	int getStageNumber(){	return stage_number;	}
	
	void setTouchBegin(){	piece_img->setColor(ccGRAY);	}
	void setTouchCancel(){	piece_img->setColor(ccWHITE);	}
	
	bool isBoarder(){	return is_boarder;	}
	
	CCSprite* shadow_node;
	
private:
	GraySprite* piece_img;
	
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
	
	void myInit(string t_piece, int t_number, int t_level, CCPoint t_p, CCRect t_rect, bool t_gray, bool t_boarder, string t_type)
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
			
			CCSprite* level_img = CCSprite::create(CCString::createWithFormat("test_map_level%d.png", stage_level)->getCString());
			level_img->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
			piece_img->addChild(level_img);
		}
		
	}
};

#endif /* defined(__DGproto__StagePiece__) */
