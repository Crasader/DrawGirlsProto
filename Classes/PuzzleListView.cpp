//
//  PuzzleListView.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 29..
//
//

#include "PuzzleListView.h"

void PuzzleListView::visit()
{
	glEnable(GL_SCISSOR_TEST);
	
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize rSize = myDSH->getDesignResolutionSize(); // getSize
	
	float wScale = frame_size.width / rSize.width;
	float hScale = frame_size.height / rSize.height;
	
	float xMargine = 0;
	float yMargine = 0;
	
	if(wScale >= hScale)
	{
		wScale = hScale;
		xMargine = (frame_size.width - rSize.width*wScale)/2.f;
	}
	else
	{
		hScale = wScale;
		yMargine = (frame_size.height - rSize.height*hScale)/2.f;
	}
	
	CCNode* parent_node = getParent();
	CCRect with_parent_view_rect = CCRectMake(parent_node->getPositionX()+view_rect.origin.x, parent_node->getPositionY()+view_rect.origin.y, view_rect.size.width, view_rect.size.height);
	
	float x = with_parent_view_rect.origin.x*wScale + xMargine;
	float y = with_parent_view_rect.origin.y*hScale + yMargine;
	float w = with_parent_view_rect.size.width*wScale;
	float h = with_parent_view_rect.size.height*hScale;
	
	glScissor(x,y,w,h);
	
	CCLayer::visit();
	
	glDisable(GL_SCISSOR_TEST);
}

void PuzzleListView::setPositionX(float t_x)
{
	if(t_x < min_positionX)
	{
		t_x = min_positionX;
		unschedule(schedule_selector(PuzzleListView::moveAnimation));
	}
	else if(t_x > 0)
	{
		t_x = 0;
		unschedule(schedule_selector(PuzzleListView::moveAnimation));
	}
	
	CCLayer::setPositionX(t_x);
	
	for(int i=0;i<getChildrenCount();i++)
	{
		CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
		((PLV_Node*)t_child)->viewCheck();
	}
}

void PuzzleListView::setMinPositionX()
{
	int cnt = getChildrenCount()/2;
	
	float t_x = (cnt-3)*136 + 5;
	if(cnt > 3)
		min_positionX = -t_x;
	else
		min_positionX = 0;
}

bool PuzzleListView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	bool return_value = false;
	
	CCNode* parent_node = getParent();
	CCRect with_parent_view_rect = CCRectMake(parent_node->getPositionX()+view_rect.origin.x, parent_node->getPositionY()+view_rect.origin.y, view_rect.size.width, view_rect.size.height);
	
	if(isVisible() && with_parent_view_rect.containsPoint(location))
	{
		touch_begin_point = location;
		first_touch_point = touch_begin_point;
		is_touching = true;
		
		touchx=location.x;
		
		timeval time;
		gettimeofday(&time, NULL);
		touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
		touchStartX=location.x;
		
		for(int i=0;i<getChildrenCount() && touched_index == 0;i++)
		{
			CCNode* item_parent = (CCNode*)getChildren()->objectAtIndex(i);
			if(!((PLV_Node*)item_parent)->isSetted())		continue;
			CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(item_parent->getTag());
			if(item_menu->ccTouchBegan(pTouch, pEvent))
				touched_index = item_parent->getTag();
		}
		
		return_value = true;
	}
	
	unschedule(schedule_selector(PuzzleListView::moveAnimation));
	isAnimated=false;
	
	return return_value;
}

void PuzzleListView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	CCNode* parent_node = getParent();
	CCRect with_parent_view_rect = CCRectMake(parent_node->getPositionX()+view_rect.origin.x, parent_node->getPositionY()+view_rect.origin.y, view_rect.size.width, view_rect.size.height);
	
	if(isVisible() && with_parent_view_rect.containsPoint(location))
	{
		float distance = location.x - touch_begin_point.x;
		setPositionX(getPositionX() + distance);
		touch_begin_point = location;
		
		if(fabsf(first_touch_point.x - location.x) > 5.f)
		{
			if(touched_index != 0)
			{
				CCNode* item_parent = getChildByTag(touched_index);
				CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
				item_menu->ccTouchCancelled(pTouch, pEvent);
				touched_index = 0;
			}
		}
	}
	else
	{
		timeval time;
		gettimeofday(&time, NULL);
		long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
		float _x = location.x-touchStartX;
		float _spd = _x/_time*10000;
		
		
		if(isAnimated == false && abs((int)_spd)>2){
			if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
			else			moveSpeed = ((int)_spd*10)/10.f+2;
			schedule(schedule_selector(PuzzleListView::moveAnimation));
		}
		
		is_touching = false;
		
		if(touched_index != 0)
		{
			CCNode* item_parent = getChildByTag(touched_index);
			CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
			item_menu->ccTouchCancelled(pTouch, pEvent);
			touched_index = 0;
		}
	}
}

void PuzzleListView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	CCNode* parent_node = getParent();
	CCRect with_parent_view_rect = CCRectMake(parent_node->getPositionX()+view_rect.origin.x, parent_node->getPositionY()+view_rect.origin.y, view_rect.size.width, view_rect.size.height);
	
	if(isVisible() && with_parent_view_rect.containsPoint(location))
	{
		float distance = location.x - touch_begin_point.x;
		setPositionX(getPositionX() + distance);
		touch_begin_point = location;
		
		timeval time;
		gettimeofday(&time, NULL);
		long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
		float _x = location.x-touchStartX;
		float _spd = _x/_time*10000;
		
		if(isAnimated == false && abs((int)_spd)>2){
			if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
			else			moveSpeed = ((int)_spd*10)/10.f+2;
			schedule(schedule_selector(PuzzleListView::moveAnimation));
		}
	}
	
	if(touched_index != 0)
	{
		CCNode* item_parent = getChildByTag(touched_index);
		CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
		item_menu->ccTouchEnded(pTouch, pEvent);
		touched_index = 0;
	}
	
	is_touching = false;
}

void PuzzleListView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
	CCTouch* touch = pTouch;
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = myDSH->wideWidthFixTouch(location);
	
	CCNode* parent_node = getParent();
	CCRect with_parent_view_rect = CCRectMake(parent_node->getPositionX()+view_rect.origin.x, parent_node->getPositionY()+view_rect.origin.y, view_rect.size.width, view_rect.size.height);
	
	if(isVisible() && with_parent_view_rect.containsPoint(location))
	{
		float distance = location.x - touch_begin_point.x;
		setPositionX(getPositionX() + distance);
		touch_begin_point = location;
		
		timeval time;
		gettimeofday(&time, NULL);
		long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
		float _x = location.x-touchStartX;
		float _spd = _x/_time*10000;
		
		if(isAnimated == false && abs((int)_spd)>2){
			if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
			else			moveSpeed = ((int)_spd*10)/10.f+2;
			schedule(schedule_selector(PuzzleListView::moveAnimation));
		}
	}
	
	if(touched_index != 0)
	{
		CCNode* item_parent = getChildByTag(touched_index);
		CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
		item_menu->ccTouchCancelled(pTouch, pEvent);
		touched_index = 0;
	}
	
	is_touching = false;
}

void PuzzleListView::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
}

void PuzzleListView::moveList(float dx)
{
	if(dx > 10)			dx = 10;
	else if(dx < -10)	dx = -10;
	
	setPositionX(getPositionX()+dx);
}

void PuzzleListView::moveAnimation()
{
	isAnimated=true;
	
	if(moveSpeed > 0)		moveSpeed -= 0.1;
	else if(moveSpeed < 0)	moveSpeed += 0.1;
	
	if((moveSpeed < 0.1 && moveSpeed > -0.1) || isAnimated == false)
	{
		unschedule(schedule_selector(PuzzleListView::moveAnimation));
		moveSpeed = 0;
		isAnimated = false;
	}
	else
		moveList(moveSpeed);
}

void PuzzleListView::myInit()
{
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	view_rect = CCRectMake(0, 340, 480, 480.f*screen_size.height/screen_size.width);
	
	is_touching = false;
	touched_index = 0;
	
	CCLayer::init();
}

PuzzleListView* PuzzleListView::create()
{
	PuzzleListView* t_clv = new PuzzleListView();
	t_clv->myInit();
	t_clv->autorelease();
	return t_clv;
}

void PuzzleListView::setPercentage( float t_p )
{
	setPosition(ccp(min_positionX*t_p,getPositionY()));
	for(int i=0;i<getChildrenCount();i++)
	{
		CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
		((PLV_Node*)t_child)->viewCheck();
	}
}

void PuzzleListView::touchCancel()
{
	if(touched_index != 0)
	{
		CCNode* t_child = (CCNode*)getChildByTag(touched_index);
		((PLV_Node*)t_child)->touchCancel();
		touched_index = 0;
	}
}

void PuzzleListView::startViewCheck()
{
	schedule(schedule_selector(PuzzleListView::viewChecking));
}

void PuzzleListView::stopViewCheck()
{
	unschedule(schedule_selector(PuzzleListView::viewChecking));
}

CCRect PuzzleListView::getViewRect()
{
	return view_rect;
}

void PuzzleListView::viewChecking()
{
	for(int i=0;i<getChildrenCount();i++)
	{
		CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
		((PLV_Node*)t_child)->viewCheck();
	}
}








PLV_Node* PLV_Node::create( int t_puzzle_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect )
{
	PLV_Node* t_n = new PLV_Node();
	t_n->myInit(t_puzzle_number, t_menu, d_menu, t_position, t_rect);
	t_n->autorelease();
	return t_n;
}

void PLV_Node::viewCheck()
{
	CCPoint parent_position = getParent()->getPosition();
	CCRect tt_rect = CCRectMake(parent_position.x+my_position.x-my_size.width/2.f, parent_position.y+my_position.y-my_size.height/2.f, my_size.width, my_size.height);
	if(parent_view_rect.intersectsRect(tt_rect))
	{
		if(!is_setted)
			setChild();
	}
	else
	{
		is_setted = false;
		removeAllChildren();
	}
}

void PLV_Node::touchCancel()
{
	((CCMenu*)getChildByTag(puzzle_number))->ccTouchCancelled(NULL, NULL);
}

bool PLV_Node::isSetted()
{
	return is_setted;
}

int PLV_Node::getPuzzleNumber()
{
	return puzzle_number;
}

void PLV_Node::setChild()
{
	int frame_type = 0;
	if(puzzle_number > 1 && puzzle_number > mySGD->getOpenPuzzleCount())
	{
		if(mySGD->getPuzzleHistory(puzzle_number-1).is_clear.getV())
		{
			if(NSDS_GI(puzzle_number, kSDS_PZ_point_i) > 0 && NSDS_GI(puzzle_number, kSDS_PZ_ticket_i) > 0)
				frame_type = 1;
		}
		else
			frame_type = 2;
	}
	
	if(frame_type == 0)
	{
		int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
		int found_index = 0;
		for(int i=1;i<=puzzle_count && found_index == 0;i++)
		{
			int puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
			if(puzzle_no == puzzle_number)
				found_index = i;
		}
		
		CCSprite* n_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", found_index)->getCString());
		CCSprite* s_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", found_index)->getCString());
		s_back->setColor(ccGRAY);
		
		CCMenuItem* back_item = CCMenuItemSprite::create(n_back, s_back, target_menu, delegate_menu);
		back_item->setTag(puzzle_number);
		
		CCMenu* back_menu = CCMenu::createWithItem(back_item);
		back_menu->setPosition(CCPointZero);
		back_menu->setTouchEnabled(false);
		addChild(back_menu, 0, puzzle_number);
	}
	else if(frame_type == 1)
	{
		CCSprite* n_back = CCSprite::create("gallery_needticket.png");
		CCSprite* s_back = CCSprite::create("gallery_needticket.png");
		s_back->setColor(ccGRAY);
		
		CCMenuItem* back_item = CCMenuItemSprite::create(n_back, s_back, target_menu, delegate_menu);
		back_item->setTag(puzzle_number);
		
		CCMenu* back_menu = CCMenu::createWithItem(back_item);
		back_menu->setPosition(CCPointZero);
		back_menu->setTouchEnabled(false);
		addChild(back_menu, 0, puzzle_number);
	}
	else if(frame_type == 2)
	{
		CCSprite* n_back = CCSprite::create("gallery_lock.png");
		CCSprite* s_back = CCSprite::create("gallery_lock.png");
		s_back->setColor(ccGRAY);
		
		CCMenuItem* back_item = CCMenuItemSprite::create(n_back, s_back, target_menu, delegate_menu);
		back_item->setTag(puzzle_number);
		
		CCMenu* back_menu = CCMenu::createWithItem(back_item);
		back_menu->setPosition(CCPointZero);
		back_menu->setTouchEnabled(false);
		addChild(back_menu, 0, puzzle_number);
	}
	is_setted = true;
}

void PLV_Node::myInit( int t_puzzle_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect )
{
	puzzle_number = t_puzzle_number;
	target_menu = t_menu;
	delegate_menu = d_menu;
	my_position = t_position;
	parent_view_rect = t_rect;

	setPosition(my_position);

	my_size = CCSizeMake(136, 115);
	is_setted = false;

	CCRect tt_rect = CCRectMake(my_position.x-my_size.width/2.f, my_position.y-my_size.height/2.f, my_size.width, my_size.height);
	if(parent_view_rect.intersectsRect(tt_rect))
	{
		setChild();
	}
}
