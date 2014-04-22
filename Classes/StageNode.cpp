//
//  StageNode.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 12..
//
//

#include "StageNode.h"
#include "StageImgLoader.h"
#include "ServerDataSave.h"
#include "ScrollMenu.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "GraySprite.h"

void StageNode::setViewMode(StageNodeViewMode t_mode)
{
	recent_mode = t_mode;
	
	star_vector.clear();
	removeAllChildren();
	
	if(recent_mode == kStageNodeViewMode_open)
	{
		CCSprite* n_stage = CCSprite::create("stage_icon_back.png");
		
		CCSprite* n_stage_img = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number, piece_no)->getCString());
		n_stage_img->setPosition(ccp(n_stage->getContentSize().width/2.f-7, n_stage->getContentSize().height/2.f+1));
		n_stage->addChild(n_stage_img);
		
		CCLabelTTF* n_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 10);
		n_label->setPosition(ccp(n_stage->getContentSize().width/2.f+17, n_stage->getContentSize().height/2.f-8));
		n_stage->addChild(n_label);
		CCSprite* s_stage = CCSprite::create("stage_icon_back.png");
		
		CCSprite* s_stage_img = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number, piece_no)->getCString());
		s_stage_img->setPosition(ccp(s_stage->getContentSize().width/2.f-7, s_stage->getContentSize().height/2.f+1));
		s_stage->addChild(s_stage_img);
		
		CCLabelTTF* s_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 10);
		s_label->setPosition(ccp(s_stage->getContentSize().width/2.f+17, s_stage->getContentSize().height/2.f-8));
		s_stage->addChild(s_label);
		s_stage->setColor(ccGRAY);
		
		CCMenuItem* stage_item = CCMenuItemSprite::create(n_stage, s_stage, menu_target, menu_delegate);
		stage_item->setTag(stage_number);
		
		ScrollMenu* stage_menu = ScrollMenu::create(stage_item, NULL);
		stage_menu->setPosition(ccp(0, 0));
		addChild(stage_menu);
		
		CCSprite* star_back = CCSprite::create("stage_icon_star_back.png");
		star_back->setPosition(ccpAdd(stage_menu->getPosition(), ccp(-6.5f,16)));
		addChild(star_back);
		
//		int stage_clear_rank = myDSH->getIntegerForKey(kDSH_Key_stageClearRank_int1, stage_number);
//		if(stage_clear_rank >= 1)
//		{
//			CCSprite* star1 = CCSprite::create("stage_icon_star.png");
//			star1->setPosition(ccp(6,4));
//			star_back->addChild(star1);
//			star_vector.push_back(star1);
//		}
//		if(stage_clear_rank >= 2)
//		{
//			CCSprite* star2 = CCSprite::create("stage_icon_star.png");
//			star2->setPosition(ccp(15,7));
//			star_back->addChild(star2);
//			star_vector.push_back(star2);
//		}
//		if(stage_clear_rank >= 3)
//		{
//			CCSprite* star3 = CCSprite::create("stage_icon_star.png");
//			star3->setPosition(ccp(24,4));
//			star_back->addChild(star3);
//			star_vector.push_back(star3);
//		}
	}
	else if(recent_mode == kStageNodeViewMode_buy)
	{
		CCSprite* n_buy = CCSprite::create("stage_icon_back.png");
		
		GraySprite* n_stage_img = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number, piece_no)->getCString()));
		n_stage_img->setGray(true);
		n_stage_img->setPosition(ccp(n_buy->getContentSize().width/2.f-7, n_buy->getContentSize().height/2.f+1));
		n_buy->addChild(n_stage_img);
		
		CCSprite* n_lock = CCSprite::create("stage_icon_lock.png");
		n_lock->setPosition(ccp(n_buy->getContentSize().width/2.f-6, n_buy->getContentSize().height/2.f+5));
		n_buy->addChild(n_lock);
		CCLabelTTF* n_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 10);
		n_label->setPosition(ccp(n_buy->getContentSize().width/2.f+17, n_buy->getContentSize().height/2.f-8));
		n_buy->addChild(n_label);
		CCSprite* s_buy = CCSprite::create("stage_icon_back.png");
		
		GraySprite* s_stage_img = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number, piece_no)->getCString()));
		s_stage_img->setGray(true);
		s_stage_img->setPosition(ccp(s_buy->getContentSize().width/2.f-7, s_buy->getContentSize().height/2.f+1));
		s_buy->addChild(s_stage_img);
		
		CCSprite* s_lock = CCSprite::create("stage_icon_lock.png");
		s_lock->setPosition(ccp(s_buy->getContentSize().width/2.f-6, s_buy->getContentSize().height/2.f+5));
		s_buy->addChild(s_lock);
		CCLabelTTF* s_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 10);
		s_label->setPosition(ccp(s_buy->getContentSize().width/2.f+17, s_buy->getContentSize().height/2.f-8));
		s_buy->addChild(s_label);
		s_buy->setColor(ccGRAY);
		
		CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, menu_target, menu_delegate);
		buy_item->setTag(stage_number);
		
		ScrollMenu* buy_menu = ScrollMenu::create(buy_item, NULL);
		buy_menu->setPosition(ccp(0, 0));
		addChild(buy_menu);
	}
	else if(recent_mode == kStageNodeViewMode_lock)
	{
		CCSprite* n_buy = CCSprite::create("stage_icon_back.png");
		
		GraySprite* n_stage_img = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number, piece_no)->getCString()));
		n_stage_img->setGray(true);
		n_stage_img->setPosition(ccp(n_buy->getContentSize().width/2.f-7, n_buy->getContentSize().height/2.f+1));
		n_buy->addChild(n_stage_img);
		
		CCSprite* n_lock = CCSprite::create("stage_icon_lock.png");
		n_lock->setPosition(ccp(n_buy->getContentSize().width/2.f-6, n_buy->getContentSize().height/2.f+5));
		n_buy->addChild(n_lock);
		CCLabelTTF* n_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 10);
		n_label->setPosition(ccp(n_buy->getContentSize().width/2.f+17, n_buy->getContentSize().height/2.f-8));
		n_buy->addChild(n_label);
		CCSprite* s_buy = CCSprite::create("stage_icon_back.png");
		
		GraySprite* s_stage_img = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number, piece_no)->getCString()));
		s_stage_img->setGray(true);
		s_stage_img->setPosition(ccp(s_buy->getContentSize().width/2.f-7, s_buy->getContentSize().height/2.f+1));
		s_buy->addChild(s_stage_img);
		
		CCSprite* s_lock = CCSprite::create("stage_icon_lock.png");
		s_lock->setPosition(ccp(s_buy->getContentSize().width/2.f-6, s_buy->getContentSize().height/2.f+5));
		s_buy->addChild(s_lock);
		CCLabelTTF* s_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(), mySGD->getFont().c_str(), 10);
		s_label->setPosition(ccp(s_buy->getContentSize().width/2.f+17, s_buy->getContentSize().height/2.f-8));
		s_buy->addChild(s_label);
		s_buy->setColor(ccGRAY);
		
		CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, menu_target, menu_delegate);
		buy_item->setTag(stage_number);
		
		ScrollMenu* buy_menu = ScrollMenu::create(buy_item, NULL);
		buy_menu->setPosition(ccp(0, 0));
		addChild(buy_menu);
	}
}

void StageNode::myInit(int t_puzzle_number, int t_stage_number, CCObject* t_target, SEL_MenuHandler t_delegate, function<void(int)> t_destroy_func)
{
	star_vector.clear();
	puzzle_number = t_puzzle_number;
	stage_number = t_stage_number;
	menu_target = t_target;
	menu_delegate = t_delegate;
	destroy_func = t_destroy_func;
	piece_no = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
	setPosition(ccp(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_x_d, stage_number), NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_y_d, stage_number)));
}