//
//  CollectionView.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 20..
//
//

#include "CollectionView.h"
#include "SilhouetteData.h"
#include "CollectionBook.h"
#include "StarGoldData.h"

CollectionInnerLine* CollectionInnerLine::create(int t_line_number)
{
	CollectionInnerLine* t_ig = new CollectionInnerLine();
	t_ig->myInit(t_line_number);
	t_ig->autorelease();
	return t_ig;
}

void CollectionInnerLine::visit()
{
	real_position_y = getParent()->getPositionY() + getPositionY();
	if(real_position_y > -31 && real_position_y < 297)			setVisible(true);
	else														setVisible(false);
	
	CCNode::visit();
}

void CollectionInnerLine::menuAction(CCObject* sender)
{
	if(!is_touch_enable)	return;
	if(!isVisible())	return;
	
	is_touch_enable = false;
	int tag = ((CCNode*)sender)->getTag();
	
	int stage_number = line_number*2 + 1 + tag/3;
	int level_number = tag%3;
	
	mySGD->selected_collectionbook = stage_number*10 + level_number;
	
	CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
	
//	GalleryZoom* t_gz = GalleryZoom::create(zoom_point, chapter_number, stage_number, collection_gallery, cg_setTouchEnable);
//	getParent()->getParent()->getParent()->addChild(t_gz, 20);
//	t_gz->showGalleryZoom();
}

void CollectionInnerLine::myInit(int t_line_number)
{
	menus_array = new CCArray(1);
	is_touch_enable = true;
	line_number = t_line_number;
	setPosition(ccp(0,line_number*(-103)));
	
	int index = 0;
	for(int i=line_number*2+1;i<=line_number*2+2;i++)
	{
		for(int j=1;j<=3;j++)
		{
			if(myDSH->getBoolForKey(kDSH_Key_hasGottenCard_int1, i*10+j-1))
				createCard(i, j, index++);
			else
				createUncover(index++);
		}
	}
}

void CollectionInnerLine::createCard(int t_stage, int t_level, int index)
{
	CCSprite* card_img = CCSprite::create(CCString::createWithFormat("stage%d_level%d_visible.png", t_stage, t_level)->getCString());
	card_img->setScale(0.17);
	card_img->setPosition(ccp(index*69,0));
	addChild(card_img);
	
	if(t_level == 3 && mySD->isAnimationStage(t_stage))
	{
		CCSize ani_size = mySD->getAnimationCutSize(t_stage);
		CCSprite* ani_img = CCSprite::create(CCString::createWithFormat("stage%d_level%d_animation.png", t_stage, t_level)->getCString(), CCRectMake(0, 0, ani_size.width, ani_size.height));
		ani_img->setPosition(mySD->getAnimationPosition(t_stage));
		card_img->addChild(ani_img);
	}
	
	CCMenuItem* card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CollectionInnerLine::menuAction));
	card_item->setTag(index);
	
	CCMenu* card_menu = CCMenu::createWithItem(card_item);
	card_menu->setPosition(card_img->getPosition());
	addChild(card_menu);
	
	menus_array->addObject(card_menu);
}

void CollectionInnerLine::createUncover(int index)
{
	CCSprite* uncover_img = CCSprite::create("collection_uncover.png");
	uncover_img->setPosition(ccp(index*69,0));
	addChild(uncover_img);
}

CollectionView* CollectionView::create()
{
	CollectionView* t_cg = new CollectionView();
	t_cg->myInit();
	t_cg->autorelease();
	return t_cg;
}

void CollectionView::visit()
{
	glEnable(GL_SCISSOR_TEST);
	
	int viewport [4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
	float wScale = viewport[2] / rSize.width;
	float hScale = viewport[3] / rSize.height;
	float x = view_rect.origin.x*wScale + viewport[0];
	float y = view_rect.origin.y*hScale + viewport[1];
	float w = view_rect.size.width*wScale;
	float h = view_rect.size.height*hScale;
	
	glScissor(x,y,w,h);
	
	CCLayer::visit();
	
	glDisable(GL_SCISSOR_TEST);
}

void CollectionView::myInit()
{
	view_rect = CCRectMake(40, 43, 400, 197);
	
	inner_view = CCNode::create();
	inner_view->setPosition(ccp(70,190));
	addChild(inner_view);
	
	setInnerView();
	
	setTouchEnabled(true);
}

void CollectionView::setInnerView()
{
	for(int i=0;i<(mySD->getLastUpdateStageNumber()+1)/2;i++)
	{
		CollectionInnerLine* t_gil = CollectionInnerLine::create(i);
		inner_view->addChild(t_gil);
	}
}

void CollectionView::moveList(float dy)
{
    if(dy > 10)			dy = 10;
    else if(dy < -10)	dy = -10;
	
//	float after_y = inner_view->getPositionY()+dy;
//	if(after_y < 200)	after_y = 200;
//	else if(after_y > 200 + ((mySD->getLastUpdateStageNumber()+1)/2-1)*103)
//		after_y = 200 + ((mySD->getLastUpdateStageNumber()+1)/2-1)*103;
	
	inner_view->setPositionY(inner_view->getPositionY()+dy);//after_y);
}

void CollectionView::moveAnimation()
{
	if(inner_view->getPositionY() < 190)
	{
		unschedule(schedule_selector(CollectionView::moveAnimation));
		schedule(schedule_selector(CollectionView::moveAnimationUp));
		return;
	}
	
	if(inner_view->getPositionY() > 190 + ((mySD->getLastUpdateStageNumber()+1)/2-2)*103)
	{
		unschedule(schedule_selector(CollectionView::moveAnimation));
		schedule(schedule_selector(CollectionView::moveAnimationDown));
		return;
	}
	
    isAnimated=true;
	
    if(moveSpeed > 0)		moveSpeed -= 0.1;
    else if(moveSpeed < 0)	moveSpeed += 0.1;
	
    if((moveSpeed < 0.1 && moveSpeed > -0.1) || isAnimated == false)
	{
        unschedule(schedule_selector(CollectionView::moveAnimation));
        moveSpeed = 0;
        isAnimated = false;
    }
	else
		moveList(moveSpeed);
}

void CollectionView::moveAnimationUp()
{
    isAnimated = true;
	
	if(inner_view->getPositionY() < 190)
	{
		float dy = 190 - inner_view->getPositionY();
		if(dy < 10)
		{
			moveList(dy);
			unschedule(schedule_selector(CollectionView::moveAnimationUp));
			isAnimated = false;
		}
		else
			moveList(10);
	}
	else
	{
		unschedule(schedule_selector(CollectionView::moveAnimationUp));
		isAnimated = false;
	}
}

void CollectionView::moveAnimationDown()
{
    isAnimated = true;
	
	if(inner_view->getPositionY() > 190 + ((mySD->getLastUpdateStageNumber()+1)/2-2)*103)
	{
		float dy = 190 + ((mySD->getLastUpdateStageNumber()+1)/2-2)*103 - inner_view->getPositionY();
		if(dy > -10)
		{
			moveList(dy);
			unschedule(schedule_selector(CollectionView::moveAnimationDown));
			isAnimated = false;
		}
		else
			moveList(-10);
	}
	else
	{
		unschedule(schedule_selector(CollectionView::moveAnimationDown));
		isAnimated = false;
	}
}

bool CollectionView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	touched_index = -1;
	int loop_cnt = inner_view->getChildrenCount();
	CCArray* iv_children = inner_view->getChildren();
	for(int i=0;i<loop_cnt;i++)
	{
		CollectionInnerLine* t_gil = (CollectionInnerLine*)iv_children->objectAtIndex(i);
		if(t_gil->ccTouchBegan(pTouch, pEvent))
		{
			touched_index = i;
			break;
		}
	}
	
    CCTouch* touch = pTouch;
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	
	bool return_value;
	if(view_rect.containsPoint(location) && isVisible())
	{
		touchy=location.y;
		
		timeval time;
		gettimeofday(&time, NULL);
		touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
		touchStartY=location.y;
		return_value = true;
	}
	else
	{
		return_value = false;
	}
    
    unschedule(schedule_selector(CollectionView::moveAnimation));
    unschedule(schedule_selector(CollectionView::moveAnimationUp));
    unschedule(schedule_selector(CollectionView::moveAnimationDown));
    isAnimated=false;
	return return_value;
}
void CollectionView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(touched_index > -1)
	{
		CCArray* iv_children = inner_view->getChildren();
		CollectionInnerLine* t_gil = (CollectionInnerLine*)iv_children->objectAtIndex(touched_index);
		t_gil->ccTouchMoved(pTouch, pEvent);
	}
	
    CCTouch* touch = pTouch;
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    
	if(view_rect.containsPoint(location) && isVisible())
	{
		float now = location.y;
		moveList(now-touchy);
		touchy=now;
	}
	else
	{
		if(inner_view->getPositionY() < 190)
		{
			if(isAnimated == false)
			{
				schedule(schedule_selector(CollectionView::moveAnimationUp));
				return;
			}
		}
        
		if(inner_view->getPositionY() > 190 + ((mySD->getLastUpdateStageNumber()+1)/2-2)*103)
		{
			if(isAnimated == false)
			{
				schedule(schedule_selector(CollectionView::moveAnimationDown));
				return;
			}
		}
        
        timeval time;
        gettimeofday(&time, NULL);
        long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
        float _y = location.y-touchStartY;
        float _spd = _y/_time*10000;
        
        
        if(isAnimated == false && abs((int)_spd)>2){
            if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
            else			moveSpeed = ((int)_spd*10)/10.f+2;
            schedule(schedule_selector(CollectionView::moveAnimation));
        }
    }
}
void CollectionView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(touched_index > -1)
	{
		CCArray* iv_children = inner_view->getChildren();
		CollectionInnerLine* t_gil = (CollectionInnerLine*)iv_children->objectAtIndex(touched_index);
		t_gil->ccTouchEnded(pTouch, pEvent);
	}
	
    CCTouch* touch = pTouch;
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    
	if(view_rect.containsPoint(location) && isVisible())
	{
		float now = location.y;
		moveList(now-touchy);
		touchy=now;
		
		if(inner_view->getPositionY() < 190)
		{
			if(isAnimated == false)
			{
				schedule(schedule_selector(CollectionView::moveAnimationUp));
				return;
			}
		}
        
		if(inner_view->getPositionY() > 190 + ((mySD->getLastUpdateStageNumber()+1)/2-2)*103)
		{
			if(isAnimated == false)
			{
				schedule(schedule_selector(CollectionView::moveAnimationDown));
				return;
			}
		}
        
        timeval time;
        gettimeofday(&time, NULL);
        long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
        float _y = location.y-touchStartY;
        float _spd = _y/_time*10000;
        
        
        if(isAnimated == false && abs((int)_spd)>2){
            if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
            else			moveSpeed = ((int)_spd*10)/10.f+2;
            schedule(schedule_selector(CollectionView::moveAnimation));
        }
	}
}
void CollectionView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	ccTouchEnded(pTouch, pEvent);
}

void CollectionView::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
}