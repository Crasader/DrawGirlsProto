//
//  NewPuzzlePiece.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 2. 8..
//
//

#include "NewPuzzlePiece.h"

#include "ScrollMenu.h"
#include "ServerDataSave.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "hspConnector.h"
#include "GDWebSprite.h"
#include "StageImgLoader.h"
#include "NewPieceManager.h"

NewPuzzlePiece::~NewPuzzlePiece()
{
	NewPieceManager::sharedInstance()->removeNewPiece(stage_number, this);
}

void NewPuzzlePiece::myInit(int t_stage_number, function<void(int)> t_clicked_func, NewPuzzlePieceMode t_init_mode, bool t_is_buy, bool t_is_lock)
{
	is_menu_enable = false;
	stage_number = t_stage_number;
	clicked_func = t_clicked_func;
	
	int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
	if(puzzle_number == 1 || mySGD->getOpenPuzzleCount() >= puzzle_number)
	{
		is_open_puzzle = true;
		recent_mode = t_init_mode; // kNewPuzzlePieceMode_default or kNewPuzzlePieceMode_thumbnail or kNewPuzzlePieceMode_ranker
	}
	else
	{
		is_open_puzzle = false;
		recent_mode = kNewPuzzlePieceMode_default;//ranker;
	}
	
	is_buy = t_is_buy;
	is_lock = t_is_lock;
	
	setPieceImg(recent_mode);
	is_menu_enable = true;
	
	NewPieceManager::sharedInstance()->addNewPiece(stage_number, this);
}

void NewPuzzlePiece::setPieceImg(NewPuzzlePieceMode t_mode)
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
//		if(t_mode == kNewPuzzlePieceMode_default)
//		{
			CCSprite* n_piece = CCSprite::create(("temp_piece_buy_" + piece_direction + ".png").c_str());
			CCSprite* n_type = CCSprite::create("price_gold_img.png");
			n_type->setScale(0.6f);
			n_type->setPosition(ccp(n_piece->getContentSize().width/2.f-13, n_piece->getContentSize().height/2.f-12));
			n_piece->addChild(n_type);
			CCLabelTTF* n_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 8);
			n_quantity_label->setAnchorPoint(ccp(0.5,0.5));
			n_quantity_label->setPosition(ccp(n_piece->getContentSize().width/2.f+6, n_piece->getContentSize().height/2.f-13));
			n_piece->addChild(n_quantity_label);
		n_piece->setPosition(ccp(-12.f,-12.f));
			
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
		s_piece->setPosition(ccp(-12.f,-12.f));
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
		piece_item->setContentSize(CCSizeMake(50, 50));
			
			piece_menu = ScrollMenu::create(piece_item, NULL);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu);
//		}
//		else if(t_mode == kNewPuzzlePieceMode_thumbnail)
//		{
//			CCSprite* n_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "face", piece_no)->getCString());
//			n_piece->setColor(ccGRAY);
//			CCSprite* s_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "face", piece_no)->getCString());
//			s_piece->setColor(ccc3(100, 100, 100));
//			
//			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
//			
//			piece_menu = ScrollMenu::create(piece_item, NULL);
//			piece_menu->setPosition(CCPointZero);
//			addChild(piece_menu);
//		}
//		else
//		{
//			createRanker();
//		}
	}
	else if(is_lock)
	{
//		if(t_mode == kNewPuzzlePieceMode_default)
//		{
			CCSprite* n_piece = CCSprite::create(("temp_piece_gray_" + piece_direction + ".png").c_str());
		CCLabelTTF* n_stage = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 12);
		n_stage->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f+5));
		n_piece->addChild(n_stage);
		n_piece->setPosition(ccp(-12.f,-12.f));
			CCSprite* s_piece = CCSprite::create(("temp_piece_gray_" + piece_direction + ".png").c_str());
			s_piece->setColor(ccGRAY);
		CCLabelTTF* s_stage = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 12);
		s_stage->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f+5));
		s_piece->addChild(s_stage);
		s_piece->setPosition(ccp(-12.f,-12.f));
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
		piece_item->setContentSize(CCSizeMake(50, 50));
			
			piece_menu = ScrollMenu::create(piece_item, NULL);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu);
//		}
//		else if(t_mode == kNewPuzzlePieceMode_thumbnail)
//		{
//			CCSprite* n_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "face", piece_no)->getCString());
//			n_piece->setColor(ccGRAY);
//			CCSprite* s_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "face", piece_no)->getCString());
//			s_piece->setColor(ccc3(100, 100, 100));
//			
//			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
//			
//			piece_menu = ScrollMenu::create(piece_item, NULL);
//			piece_menu->setPosition(CCPointZero);
//			addChild(piece_menu);
//		}
//		else
//		{
//			createRanker();
//		}
	}
	else
	{
		if(stage_number == mySGD->suitable_stage)
		{
			CCSprite* n_piece = CCSprite::create(CCString::createWithFormat("temp_piece_suitable_%s.png", piece_direction.c_str())->getCString());
			CCSprite* s_piece = CCSprite::create(CCString::createWithFormat("temp_piece_suitable_%s.png", piece_direction.c_str())->getCString());
			s_piece->setColor(ccGRAY);

			n_piece->setPosition(ccp(-(n_piece->getContentSize().width-50)/2.f,-(n_piece->getContentSize().height-50)/2.f));
			s_piece->setPosition(ccp(-(s_piece->getContentSize().width-50)/2.f,-(s_piece->getContentSize().height-50)/2.f));
			
			CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
			piece_item->setContentSize(CCSizeMake(50, 50));
			
			piece_menu = ScrollMenu::create(piece_item, NULL);
			piece_menu->setPosition(CCPointZero);
			addChild(piece_menu);
		}
		else
		{
			bool is_have[3] = {false,};
			is_have[0] = mySGD->isHasGottenCards(stage_number, 1) > 0;
			is_have[1] = mySGD->isHasGottenCards(stage_number, 2) > 0;
			is_have[2] = mySGD->isHasGottenCards(stage_number, 3) > 0;
			
			if(t_mode == kNewPuzzlePieceMode_default)
			{
				string piece_filename;
				if(is_have[2])
					piece_filename = "temp_piece_gold_" + piece_direction + ".png";
				else if(is_have[1])
					piece_filename = "temp_piece_silver_" + piece_direction + ".png";
				else if(is_have[0])
					piece_filename = "temp_piece_bronze_" + piece_direction + ".png";
				else
					piece_filename = "temp_piece_normal_" + piece_direction + ".png";
				
				CCSprite* n_piece = CCSprite::create(piece_filename.c_str());
				CCLabelTTF* n_stage = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 12);
				n_stage->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f+5));
				n_piece->addChild(n_stage);
				setStageLevel(n_piece);
				n_piece->setPosition(ccp(-12.f,-12.f));
				
				CCSprite* s_piece = CCSprite::create(piece_filename.c_str());
				s_piece->setColor(ccGRAY);
				CCLabelTTF* s_stage = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 12);
				s_stage->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f+5));
				s_piece->addChild(s_stage);
				setStageLevel(s_piece);
				s_piece->setPosition(ccp(-12.f,-12.f));
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				piece_item->setContentSize(CCSizeMake(50, 50));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(t_mode == kNewPuzzlePieceMode_thumbnail)
			{
				CCSprite* n_piece;
				CCSprite* s_piece;
				
				if(mySGD->isClearPiece(stage_number))
				{
					n_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "original", piece_no)->getCString());
					s_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "original", piece_no)->getCString());
					s_piece->setColor(ccGRAY);
				}
				else
				{
					n_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "center", piece_no)->getCString());
					s_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "center", piece_no)->getCString());
					s_piece->setColor(ccGRAY);
				}
				n_piece->setPosition(ccp(-(n_piece->getContentSize().width-50)/2.f,-(n_piece->getContentSize().height-50)/2.f));
				s_piece->setPosition(ccp(-(s_piece->getContentSize().width-50)/2.f,-(s_piece->getContentSize().height-50)/2.f));
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(NewPuzzlePiece::menuAction));
				piece_item->setContentSize(CCSizeMake(50, 50));
				
				piece_menu = ScrollMenu::create(piece_item, NULL);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
		}
//		else if(t_mode == kNewPuzzlePieceMode_ranker)
//		{
//			createRanker();
//		}
	}
}

//void NewPuzzlePiece::createRanker()
//{
//	if(mySGD->temp_stage_ranker_list["result"]["code"].asInt() == GDSUCCESS)
//	{
//		string stage_ranker_user_id;
//		for(int i=0;i<mySGD->temp_stage_ranker_list["list"].size();i++)
//		{
//			if(mySGD->temp_stage_ranker_list["list"][i][0]["stageNo"].asInt() == stage_number)
//			{
//				stage_ranker_user_id = mySGD->temp_stage_ranker_list["list"][i][0]["memberID"].asString();
//				break;
//			}
//		}
//		
//		if(!stage_ranker_user_id.empty())
//		{
//			bool is_found = false;
//			string found_nick;
//			string found_url;
//			
//			if(stage_ranker_user_id == hspConnector::get()->myKakaoInfo["user_id"].asString())
//			{
//				is_found = true;
//				found_nick = hspConnector::get()->myKakaoInfo["nickname"].asString();
//				found_url = hspConnector::get()->myKakaoInfo["profile_image_url"].asString();
//			}
//			
//			for(auto i : KnownFriends::getInstance()->getFriends())
//			{
//				if(!is_found)
//				{
//					if(i.userId == stage_ranker_user_id)
//					{
//						is_found = true;
//						found_nick = i.nick;
//						found_url = i.profileUrl;
//						break;
//					}
//				}
//				else
//					break;
//			}
//			
//			for(auto i : UnknownFriends::getInstance()->getFriends())
//			{
//				if(!is_found)
//				{
//					if(i.userId == stage_ranker_user_id)
//					{
//						is_found = true;
//						found_nick = i.nick;
//						found_url = i.profileUrl;
//						break;
//					}
//				}
//				else
//					break;
//			}
//			
//			if(is_found)
//			{
//				CCSprite* n_piece = GDWebSprite::create(found_url.c_str(), "piece_noimg.png");
//				n_piece->setAnchorPoint(ccp(0.5,0.5));
//				//						CCSprite* s_piece = GDWebSprite::create(found_url.c_str(), "piece_ranker_noimg.png");
//				//						s_piece->setScale(58.f / s_piece->getContentSize().width);
//				//						s_piece->setColor(ccGRAY);
//				
//				
//				CCNode *abc = CCNode::create();
//				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, abc, this, menu_selector(NewPuzzlePiece::menuAction));
//				piece_menu = ScrollMenu::create(piece_item, NULL);
//				piece_menu->setPosition(CCPointZero);
//				addChild(piece_menu);
//				
//				CCLabelTTF* nick_label = CCLabelTTF::create(found_nick.c_str(), mySGD->getFont().c_str(), 15);
//				nick_label->setColor(ccBLACK);
//				nick_label->setPosition(CCPointZero);
//				addChild(nick_label);
//			}
//		}
//	}
//}

void NewPuzzlePiece::setStageLevel(CCSprite* piece_img)
{
	CCNode* t_node = CCNode::create();
	t_node->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
	piece_img->addChild(t_node);
	
//	CCSprite* level_label = CCSprite::create("temp_level_label.png");
//	level_label->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
//	piece_img->addChild(level_label);
//	
//	int stage_level = NSDS_GI(stage_number, kSDS_SI_level_i);
//	
//	CCLabelBMFont* level_count = CCLabelBMFont::create(CCString::createWithFormat("%d", stage_level)->getCString(), "levelcount.fnt");
//	level_count->setAnchorPoint(ccp(0.5,0.5));
//	level_count->setPosition(ccp(piece_img->getContentSize().width/2.f-9, piece_img->getContentSize().height/2.f-10));
//	piece_img->addChild(level_count);
//	
//	ccColor3B font_color = ccc3(0,0,0);
//	if(stage_level <= 1)
//		font_color = ccc3(255,255,255);
//	else if(stage_level == 2)
//		font_color = ccc3(255,255,102);
//	else if(stage_level == 3)
//		font_color = ccc3(255,204,102);
//	else if(stage_level == 4)
//		font_color = ccc3(204,255,102);
//	else if(stage_level == 5)
//		font_color = ccc3(102,255,204);
//	else if(stage_level == 6)
//		font_color = ccc3(102,204,255);
//	else if(stage_level == 7)
//		font_color = ccc3(204,102,255);
//	else if(stage_level == 8)
//		font_color = ccc3(255,111,207);
//	else if(stage_level == 9)
//		font_color = ccc3(255,0,128);
//	else if(stage_level == 10)
//		font_color = ccc3(128,0,128);
//	else if(stage_level == 11)
//		font_color = ccc3(128,0,0);
//	else if(stage_level == 12)
//		font_color = ccc3(128,64,0);
//	else if(stage_level == 13)
//		font_color = ccc3(128,128,0);
//	else if(stage_level == 14)
//		font_color = ccc3(0,128,128);
//	else if(stage_level == 15)
//		font_color = ccc3(0,64,128);
//	else if(stage_level == 16)
//		font_color = ccc3(64,0,128);
//	else if(stage_level >= 17)
//		font_color = ccc3(0,0,0);
//	
//	//			level_label->setColor(font_color);
//	level_count->setColor(font_color);
	
	int star_cnt = 0;
	
	int card_durability[3] = {0,};
	card_durability[0] = mySGD->isHasGottenCards(stage_number, 1);
	card_durability[1] = mySGD->isHasGottenCards(stage_number, 2);
	card_durability[2] = mySGD->isHasGottenCards(stage_number, 3);
	
	if(card_durability[0])		star_cnt++;
	if(card_durability[1])		star_cnt++;
	if(card_durability[2])		star_cnt++;
	
	
	if(card_durability[0] > 0)
	{
		star1 = CCSprite::create("temp_piece_star_bronze.png");
		star1->setScale(0.7f);
		CCPoint star_position;
		if(star_cnt == 1)
			star_position = ccp(0,-5);
		else if(star_cnt == 2)
			star_position = ccp(-5.5f,-5);
		else if(star_cnt == 3)
			star_position = ccp(-11,-5);
		star1->setPosition(star_position);
		t_node->addChild(star1);
//		star1->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(-13,4)));
//		level_label->addChild(star1);
	}
	if(card_durability[1] > 0)
	{
		star2 = CCSprite::create("temp_piece_star_silver.png");
		star2->setScale(0.7f);
		CCPoint star_position;
		if(star_cnt == 1)
			star_position = ccp(0,-5);
		else if(star_cnt == 2)
		{
			if(card_durability[0] > 0)
				star_position = ccp(5.5f,-5);
			else
				star_position = ccp(-5.5f,-5);
		}
		else if(star_cnt == 3)
			star_position = ccp(0,-5);
		star2->setPosition(star_position);
		t_node->addChild(star2);
//		star2->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(13,4)));
//		level_label->addChild(star2);
	}
	if(card_durability[2] > 0)
	{
		star3 = CCSprite::create("temp_piece_star_gold.png");
		star3->setScale(0.7f);
		CCPoint star_position;
		if(star_cnt == 1)
			star_position = ccp(0,-5);
		else if(star_cnt == 2)
		{
			star_position = ccp(5.5f,-5);
		}
		else if(star_cnt == 3)
			star_position = ccp(11,-5);
		star3->setPosition(star_position);
		t_node->addChild(star3);
//		star3->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(0,8)));
//		level_label->addChild(star3);
	}
}