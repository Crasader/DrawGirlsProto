//
//  PuzzleListShadow.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 22..
//
//

#ifndef __DGproto__PuzzleListShadow__
#define __DGproto__PuzzleListShadow__

#include "cocos2d.h"

USING_NS_CC;

class PuzzleListShadow : public CCNode
{
public:
	static PuzzleListShadow* create(CCNode* t_base_node, CCNode* t_target_node, CCPoint t_base_position, CCPoint t_center_position, CCPoint t_distance, CCPoint t_move_position)
	{
		PuzzleListShadow* t_pls = new PuzzleListShadow();
		t_pls->myInit(t_base_node, t_target_node, t_base_position, t_center_position, t_distance, t_move_position);
		t_pls->autorelease();
		return t_pls;
	}
	
	void startAction()
	{
		schedule(schedule_selector(PuzzleListShadow::myAction));
	}
	
private:
	
	CCPoint base_position;
	CCPoint center_position;
	CCPoint distance;
	CCPoint move_position;
	CCNode* base_node;
	CCNode* target_node;
	
	void myAction()
	{
		CCPoint distance_position = convertToWorldSpace(ccp(-center_position.x, -center_position.y));
		distance_position = base_node->convertToNodeSpace(distance_position);
		distance_position = ccpSub(distance_position, base_position); // 테이블의 중심 위치
//		distance_position = ccpAdd(distance_position, center_position); // 셀의 중심 위치
		
		if(distance.x != 0)
			distance_position.x = distance_position.x/distance.x;
		else
			distance_position.x = 0;
		if(distance.y != 0)
			distance_position.y = distance_position.y/distance.y;
		else
			distance_position.y = 0;
		
		distance_position.x = distance_position.x*move_position.x + 60.f*move_position.x;
		distance_position.y = distance_position.y*move_position.y;// + 60.f*move_position.y;
		
		setPosition(ccpAdd(center_position, distance_position));
	}
	
	void myInit(CCNode* t_base_node, CCNode* t_target_node, CCPoint t_base_position, CCPoint t_center_position, CCPoint t_distance, CCPoint t_move_position)
	{
		base_node = t_base_node;
		target_node = t_target_node;
		base_position = t_base_position;
		center_position = t_center_position;
		distance = t_distance;
		move_position = t_move_position;
	}
};

#endif /* defined(__DGproto__PuzzleListShadow__) */
