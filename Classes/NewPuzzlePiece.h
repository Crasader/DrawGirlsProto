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
#include "ScrollMenu.h"
#include "ServerDataSave.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "hspConnector.h"
#include "GDWebSprite.h"

USING_NS_CC;
using namespace std;

enum NewPuzzlePieceMode
{
	kNewPuzzlePieceMode_default = 0,
	kNewPuzzlePieceMode_thumbnail,
	kNewPuzzlePieceMode_ranker
};

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
		
		setPieceImg(recent_mode);
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
	
	void myInit(int t_stage_number, function<void(int)> t_clicked_func, NewPuzzlePieceMode t_init_mode, bool t_is_buy, bool t_is_lock)
	{
		is_menu_enable = false;
		stage_number = t_stage_number;
		clicked_func = t_clicked_func;
		
		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
		if(puzzle_number == 1 || myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1 >= puzzle_number)
		{
			is_open_puzzle = true;
			recent_mode = t_init_mode; // kNewPuzzlePieceMode_default or kNewPuzzlePieceMode_thumbnail or kNewPuzzlePieceMode_ranker
		}
		else
		{
			is_open_puzzle = false;
			recent_mode = kNewPuzzlePieceMode_ranker;
		}
		
		is_buy = t_is_buy;
		is_lock = t_is_lock;
		
		setPieceImg(recent_mode);
		is_menu_enable = true;
	}
	
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
	
	void setPieceImg(NewPuzzlePieceMode t_mode)
	{
		removeAllChildren();
		star1 = NULL;
		star2 = NULL;
		star3 = NULL;
		
		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
		int piece_no = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		int x,y;
		
		x = (piece_no-1)%6;
		y = (piece_no-1)/6;
		
		string piece_direction;
		if((x+y)%2 == 0)
			piece_direction = "h";
		else
			piece_direction = "w";
		
		if(is_buy)
		{
			if(t_mode == kNewPuzzlePieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("temp_piece_buy_" + piece_direction + ".png").c_str());
				CCSprite* n_type = CCSprite::create("price_gold_img.png");
				n_type->setScale(0.6f);
				n_type->setPosition(ccp(n_piece->getContentSize().width/2.f-13, n_piece->getContentSize().height/2.f-12));
				n_piece->addChild(n_type);
				CCLabelTTF* n_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 8);
				n_quantity_label->setAnchorPoint(ccp(0.5,0.5));
				n_quantity_label->setPosition(ccp(n_piece->getContentSize().width/2.f+6, n_piece->getContentSize().height/2.f-13));
				n_piece->addChild(n_quantity_label);
				
				CCSprite* s_piece = CCSprite::create(("temp_piece_buy_" + piece_direction + ".png").c_str());
				s_piece->setColor(ccGRAY);
				CCSprite* s_type = CCSprite::create("price_gold_img.png");
				s_type->setScale(0.6f);
				s_type->setColor(ccGRAY);
				s_type->setPosition(ccp(s_piece->getContentSize().width/2.f-13, s_piece->getContentSize().height/2.f-12));
				s_piece->addChild(s_type);
				CCLabelTTF* s_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 8);
				s_quantity_label->setColor(ccGRAY);
				s_quantity_label->setAnchorPoint(ccp(0.5,0.5));
				s_quantity_label->setPosition(ccp(s_piece->getContentSize().width/2.f+6, s_piece->getContentSize().height/2.f-13));
				s_piece->addChild(s_quantity_label);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(t_mode == kNewPuzzlePieceMode_thumbnail)
			{
				CCSprite* n_piece = CCSprite::create(("temp_puzzle_back_p" + piece_direction + ".png").c_str());
				n_piece->setColor(ccGREEN);
				CCSprite* s_piece = CCSprite::create(("temp_puzzle_back_p" + piece_direction + ".png").c_str());
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else
			{
				createRanker();
			}
		}
		else if(is_lock)
		{
			if(t_mode == kNewPuzzlePieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("temp_piece_lock_" + piece_direction + ".png").c_str());
				CCSprite* s_piece = CCSprite::create(("temp_piece_lock_" + piece_direction + ".png").c_str());
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(t_mode == kNewPuzzlePieceMode_thumbnail)
			{
				CCSprite* n_piece = CCSprite::create(("temp_puzzle_back_p" + piece_direction + ".png").c_str());
				n_piece->setColor(ccGREEN);
				CCSprite* s_piece = CCSprite::create(("temp_puzzle_back_p" + piece_direction + ".png").c_str());
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else
			{
				createRanker();
			}
		}
		else
		{
			if(t_mode == kNewPuzzlePieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("temp_puzzle_stencil_p" + piece_direction + ".png").c_str());
				setStageLevel(n_piece);
				CCSprite* s_piece = CCSprite::create(("temp_puzzle_stencil_p" + piece_direction + ".png").c_str());
				s_piece->setColor(ccGRAY);
				setStageLevel(s_piece);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(t_mode == kNewPuzzlePieceMode_thumbnail)
			{
				CCSprite* n_piece = CCSprite::create(("temp_puzzle_stencil_p" + piece_direction + ".png").c_str());
				n_piece->setColor(ccGREEN);
				CCSprite* s_piece = CCSprite::create(("temp_puzzle_stencil_p" + piece_direction + ".png").c_str());
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(t_mode == kNewPuzzlePieceMode_ranker)
			{
				createRanker();
			}
		}
	}
	
	void createRanker()
	{
		if(mySGD->temp_stage_ranker_list["result"]["code"].asInt() == GDSUCCESS)
		{
			string stage_ranker_user_id;
			for(int i=0;i<mySGD->temp_stage_ranker_list["list"].size();i++)
			{
				if(mySGD->temp_stage_ranker_list["list"][i][0]["stageNo"].asInt() == stage_number)
				{
					stage_ranker_user_id = mySGD->temp_stage_ranker_list["list"][i][0]["memberID"].asString();
					break;
				}
			}
			
			if(!stage_ranker_user_id.empty())
			{
				bool is_found = false;
				string found_nick;
				string found_url;
				
				if(stage_ranker_user_id == hspConnector::get()->myKakaoInfo["user_id"].asString())
				{
					is_found = true;
					found_nick = hspConnector::get()->myKakaoInfo["nickname"].asString();
					found_url = hspConnector::get()->myKakaoInfo["profile_image_url"].asString();
				}
				
				for(auto i : KnownFriends::getInstance()->getFriends())
				{
					if(!is_found)
					{
						if(i.userId == stage_ranker_user_id)
						{
							is_found = true;
							found_nick = i.nick;
							found_url = i.profileUrl;
							break;
						}
					}
					else
						break;
				}
				
				for(auto i : UnknownFriends::getInstance()->getFriends())
				{
					if(!is_found)
					{
						if(i.userId == stage_ranker_user_id)
						{
							is_found = true;
							found_nick = i.nick;
							found_url = i.profileUrl;
							break;
						}
					}
					else
						break;
				}
				
				if(is_found)
				{
					CCSprite* n_piece = GDWebSprite::create(found_url.c_str(), "piece_noimg.png");
					n_piece->setAnchorPoint(ccp(0.5,0.5));
					//						CCSprite* s_piece = GDWebSprite::create(found_url.c_str(), "piece_ranker_noimg.png");
					//						s_piece->setScale(58.f / s_piece->getContentSize().width);
					//						s_piece->setColor(ccGRAY);
					
					
					CCNode *abc = CCNode::create();
					CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, abc, this, menu_selector(NewPuzzlePiece::menuAction));
					piece_menu = ScrollMenu::create(piece_item, NULL);
					piece_menu->setPosition(CCPointZero);
					addChild(piece_menu);
					
					CCLabelTTF* nick_label = CCLabelTTF::create(found_nick.c_str(), mySGD->getFont().c_str(), 15);
					nick_label->setColor(ccBLACK);
					nick_label->setPosition(CCPointZero);
					addChild(nick_label);
				}
			}
		}
	}
	
	void setStageLevel(CCSprite* piece_img)
	{
		CCSprite* level_label = CCSprite::create("temp_level_label.png");
		level_label->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
		piece_img->addChild(level_label);
		
		int stage_level = NSDS_GI(stage_number, kSDS_SI_level_i);
		
		CCLabelBMFont* level_count = CCLabelBMFont::create(CCString::createWithFormat("%d", stage_level)->getCString(), "levelcount.fnt");
		level_count->setAnchorPoint(ccp(0.5,0.5));
		level_count->setPosition(ccp(piece_img->getContentSize().width/2.f-9, piece_img->getContentSize().height/2.f-10));
		piece_img->addChild(level_count);
		
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
		
		//			level_label->setColor(font_color);
		level_count->setColor(font_color);
		
		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 1)) > 0)
		{
			star1 = CCSprite::create("temp_piece_star_bronze.png");
			star1->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(-13,4)));
			level_label->addChild(star1);
		}
		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 1)) > 1)
		{
			star2 = CCSprite::create("temp_piece_star_silver.png");
			star2->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(13,4)));
			level_label->addChild(star2);
		}
		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, 1)) > 3)
		{
			star3 = CCSprite::create("temp_piece_star_gold.png");
			star3->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(0,8)));
			level_label->addChild(star3);
		}
	}
};


#endif /* defined(__DGproto__NewPuzzlePiece__) */
