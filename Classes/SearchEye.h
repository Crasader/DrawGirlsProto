//
//  SearchEye.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 8..
//
//

#ifndef __DGproto__SearchEye__
#define __DGproto__SearchEye__

#include "cocos2d.h"
#include "GameData.h"
#include "DataStorageHub.h"

USING_NS_CC;

class SearchEye : public CCNode
{
public:
	static SearchEye* create()
	{
		SearchEye* t_se = new SearchEye();
		t_se->myInit();
		t_se->autorelease();
		return t_se;
	}
	
	void startSearch()
	{
		ing_frame = 0;
		schedule(schedule_selector(SearchEye::mainCumberSearching));
	}
	
private:
	CCSprite* search_eye_img;
	CCSize frame_size;
	int ing_frame;
	
	void mainCumberSearching()
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
				search_eye_img->setVisible(true);
			else
				search_eye_img->setVisible(false);
			
			if(position_to_game_node.y > frame_size.height)
				position_to_game_node.y = frame_size.height;
			else if(position_to_game_node.y < 0)
				position_to_game_node.y = 0;
			
			CCMoveTo* t_move = CCMoveTo::create(0.1f, position_to_game_node);
			search_eye_img->runAction(t_move);
		}
	}
	
	void myInit()
	{
		search_eye_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 20, 20));
		search_eye_img->setPosition(CCPointZero);
		search_eye_img->setVisible(false);
		addChild(search_eye_img);
		
		frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		frame_size = CCSizeMake(480, myDSH->ui_top);
	}
};

#endif /* defined(__DGproto__SearchEye__) */
