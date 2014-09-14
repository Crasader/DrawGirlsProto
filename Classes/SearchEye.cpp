// SearchEye.cpp
//

#include "SearchEye.h"
#include "SilhouetteData.h"
#define LZZ_INLINE inline
SearchEye * SearchEye::create (CCNode* t_boss, vector<SearchEye*>* sv)
{
	SearchEye* t_se = new SearchEye();
	t_se->myInit(t_boss, sv);
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
		if(!myGD->isValidMainCumber(target_boss))
		{
			unschedule(schedule_selector(SearchEye::mainCumberSearching));
			removeFromParent();
			search_eye_vector->erase(std::find(search_eye_vector->begin(), search_eye_vector->end(), this));
			return;
		}
		
		CCPoint main_cumber_position = target_boss->getPosition();
		float scale_value = NSDS_GD(mySD->getSilType(), kSDS_SI_scale_d);
		if(scale_value < 0.1f)
			scale_value = 1.f;
		CCPoint position_to_game_node;
		position_to_game_node.x = main_cumber_position.x/320.f*(720.f*scale_value-myGD->boarder_value*2.f);
		position_to_game_node.y = main_cumber_position.y/320.f*(720.f*scale_value-myGD->boarder_value*2.f);
		position_to_game_node.x += myGD->getCommunicationCCPoint("Main_getGameNodePosition").x;
		position_to_game_node.y += myGD->getCommunicationCCPoint("Main_getGameNodePosition").y;
		
		if(position_to_game_node.y < 0 || position_to_game_node.y > frame_size.height || position_to_game_node.x < 0 || position_to_game_node.x > frame_size.width)
		{
			search_eye_arrow->setVisible(true);
			search_eye_boss->setVisible(true);
		}
		else
		{
			search_eye_arrow->setVisible(false);
			search_eye_boss->setVisible(false);
		}
		
		if(position_to_game_node.y > frame_size.height)
			position_to_game_node.y = frame_size.height;
		else if(position_to_game_node.y < 0)
			position_to_game_node.y = 0;
		if(position_to_game_node.x > frame_size.width)
			position_to_game_node.x = frame_size.width;
		else if(position_to_game_node.x < 0)
			position_to_game_node.x = 0;
		
		float angle = atan2f(position_to_game_node.y - myDSH->ui_center_y, position_to_game_node.x - 240.f);
		search_eye_arrow->setRotation(-(angle/M_PI*180.f-90.f));
		
		CCPoint boss_position;
		boss_position.x = -cosf(angle)*30.f;
		boss_position.y = -sinf(angle)*30.f;
		search_eye_boss->setPosition(boss_position);
		
		CCMoveTo* t_move = CCMoveTo::create(0.1f, position_to_game_node);
		runAction(t_move);
	}
}
void SearchEye::myInit (CCNode* t_boss, vector<SearchEye*>* sv)
{
	target_boss = t_boss;
	search_eye_vector = sv;
	search_eye_arrow = CCSprite::create("search_eye_arrow.png");
	search_eye_arrow->setAnchorPoint(ccp(0.5,1));
	search_eye_arrow->setPosition(CCPointZero);
	search_eye_arrow->setVisible(false);
	addChild(search_eye_arrow);
	
	search_eye_boss = CCSprite::create("search_eye_boss.png");
	search_eye_boss->setPosition(CCPointZero);
	search_eye_boss->setVisible(false);
	addChild(search_eye_boss);
	
	frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	frame_size = CCSizeMake(480, myDSH->ui_top);
}
#undef LZZ_INLINE
