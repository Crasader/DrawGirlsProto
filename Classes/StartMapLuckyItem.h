//
//  StartMapLuckyItem.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#ifndef __DGproto__StartMapLuckyItem__
#define __DGproto__StartMapLuckyItem__

#include "cocos2d.h"
#include "GameData.h"

USING_NS_CC;

class StartMapLuckyItem : public CCSprite
{
public:
	static StartMapLuckyItem* create(IntPoint t_point)
	{
		StartMapLuckyItem* t_smli = new StartMapLuckyItem();
		t_smli->myInit(t_point);
		t_smli->autorelease();
		return t_smli;
	}
	
	void checkInnerRect()
	{
		if(myGD->mapState[my_point.x][my_point.y] == mapOldget || myGD->mapState[my_point.x][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y] == mapOldget || myGD->mapState[my_point.x-1][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x-1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x-1][my_point.y+1] == mapOldline ||
		   myGD->mapState[my_point.x][my_point.y-1] == mapOldget || myGD->mapState[my_point.x][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x][my_point.y+1] == mapOldget || myGD->mapState[my_point.x][my_point.y+1] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y-1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y-1] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y] == mapOldget || myGD->mapState[my_point.x+1][my_point.y] == mapOldline ||
		   myGD->mapState[my_point.x+1][my_point.y+1] == mapOldget || myGD->mapState[my_point.x+1][my_point.y+1] == mapOldline)
		{
			speedUpAction();
		}
		else
		{
			startRemoveAction();
		}
	}
	
private:
	IntPoint my_point;
	
	void startMyAction()
	{
		runAction(CCScaleTo::create(1.f, 0.7f));
	}
	
	void speedUpAction()
	{
		stopAllActions();
		
		CCScaleTo* t_scale1 = CCScaleTo::create(0.5f, 1.f);
		CCFadeTo* t_fade1 = CCFadeTo::create(0.5f, 195);
		CCSpawn* t_spawn1 = CCSpawn::createWithTwoActions(t_scale1, t_fade1);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
		
		CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 1.3f);
		CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 130);
		CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_scale2, t_fade2);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
		
		CCScaleTo* t_scale3 = CCScaleTo::create(0.5f, 1.6f);
		CCFadeTo* t_fade3 = CCFadeTo::create(0.5f, 65);
		CCSpawn* t_spawn3 = CCSpawn::createWithTwoActions(t_scale3, t_fade3);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::speedUp));
		
		CCScaleTo* t_scale4 = CCScaleTo::create(0.5f, 1.9f);
		CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
		CCSpawn* t_spawn4 = CCSpawn::createWithTwoActions(t_scale4, t_fade4);
		CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::removeFromParent));
		
		runAction(CCSequence::create(t_spawn1, t_call1, t_spawn2, t_call2, t_spawn3, t_call3, t_spawn4, t_call4, NULL));
	}
	
	void speedUp()
	{
		myGD->communication("Jack_takeSpeedUpItem");
	}
	
	void startRemoveAction()
	{
		stopAllActions();
		runAction(CCSequence::createWithTwoActions(CCScaleTo::create(1.f, 0.f), CCCallFunc::create(this, callfunc_selector(StartMapLuckyItem::removeFromParent))));
	}
	
	void myInit(IntPoint t_point)
	{
		my_point = IntPoint(t_point.x, t_point.y);
		
		initWithFile("item_lucky.png");
		
		setScale(0.f);
		
		setPosition(my_point.convertToCCP());
		startMyAction();
	}
};

#endif /* defined(__DGproto__StartMapLuckyItem__) */
