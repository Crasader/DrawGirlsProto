// SearchEye.cpp
//

#include "SearchEye.h"
#define LZZ_INLINE inline
SearchEye * SearchEye::create ()
        {
		SearchEye* t_se = new SearchEye();
		t_se->myInit();
		t_se->autorelease();
		return t_se;
	}
void SearchEye::startSearch ()
        {
		ing_frame = 0;
		schedule(schedule_selector(SearchEye::mainCumberSearching));
	}
void SearchEye::mainCumberSearching ()
        {
		ing_frame++;
		
		if(ing_frame%6 == 0)
		{
			CCPoint main_cumber_position = myGD->getMainCumberPoint().convertToCCP();
			
			CCPoint position_to_game_node;
			position_to_game_node.x = main_cumber_position.x/320.f*(480.f-myGD->boarder_value*2.f) + myGD->boarder_value;
			if(myGD->gamescreen_type == kGT_leftUI)			position_to_game_node.x += 50.f;
			position_to_game_node.y = main_cumber_position.y/320.f*(480.f-myGD->boarder_value*2.f);
			position_to_game_node.y += myGD->getCommunicationCCPoint("Main_getGameNodePosition").y;
			
			if(position_to_game_node.y < -5 || position_to_game_node.y > frame_size.height + 5)
			{
				search_eye_img_top->setVisible(true);
				search_eye_img_bottom->setVisible(true);
			}
			else
			{
				search_eye_img_top->setVisible(false);
				search_eye_img_bottom->setVisible(false);
			}
			
			if(position_to_game_node.y > frame_size.height)
				position_to_game_node.y = frame_size.height;
			else if(position_to_game_node.y < 0)
				position_to_game_node.y = 0;
			
			CCMoveTo* t_move = CCMoveTo::create(0.1f, position_to_game_node);
			runAction(t_move);
		}
	}
void SearchEye::myInit ()
        {
		CCTexture2D* search_eye_texture = CCTextureCache::sharedTextureCache()->addImage("search_eye.png");
		
		search_eye_img_top = CCSprite::createWithTexture(search_eye_texture, CCRectMake(0, 0, 45, 30));
		search_eye_img_top->setAnchorPoint(ccp(0.5,0));
		search_eye_img_top->setPosition(CCPointZero);
		search_eye_img_top->setVisible(false);
		addChild(search_eye_img_top);
		
		search_eye_img_bottom = CCSprite::createWithTexture(search_eye_texture, CCRectMake(0, 30, 45, 30));
		search_eye_img_bottom->setAnchorPoint(ccp(0.5,1));
		search_eye_img_bottom->setPosition(CCPointZero);
		search_eye_img_bottom->setVisible(false);
		addChild(search_eye_img_bottom);
		
		CCMoveTo* up_move_top = CCMoveTo::create(0.3f, ccp(0,15));
		CCMoveTo* down_move_top = CCMoveTo::create(0.3f, ccp(0,0));
		CCSequence* t_seq_top = CCSequence::createWithTwoActions(up_move_top, down_move_top);
		CCRepeatForever* t_repeat_top = CCRepeatForever::create(t_seq_top);
		
		search_eye_img_top->runAction(t_repeat_top);
		
		CCMoveTo* down_move_bottom = CCMoveTo::create(0.3f, ccp(0,-15));
		CCMoveTo* up_move_bottom = CCMoveTo::create(0.3f, ccp(0,0));
		CCSequence* t_seq_bottom = CCSequence::createWithTwoActions(down_move_bottom, up_move_bottom);
		CCRepeatForever* t_repeat_bottom = CCRepeatForever::create(t_seq_bottom);
		
		search_eye_img_bottom->runAction(t_repeat_bottom);
		
		frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		frame_size = CCSizeMake(480, myDSH->ui_top);
	}
#undef LZZ_INLINE
