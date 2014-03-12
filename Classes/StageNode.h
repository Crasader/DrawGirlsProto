//
//  StageNode.h
//  DGproto
//
//  Created by 사원3 on 2014. 3. 12..
//
//

#ifndef __DGproto__StageNode__
#define __DGproto__StageNode__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum StageNodeViewMode
{
	kStageNodeViewMode_open = 0,
	kStageNodeViewMode_buy,
	kStageNodeViewMode_lock
};

class StageNode : public CCNode
{
public:
	static StageNode* create(int t_puzzle_number, int t_stage_number, CCObject* t_target, SEL_MenuHandler t_delegate)
	{
		StageNode* t_sn = new StageNode();
		t_sn->myInit(t_puzzle_number, t_stage_number, t_target, t_delegate);
		t_sn->autorelease();
		return t_sn;
	}
	
	void setViewMode(StageNodeViewMode t_mode)
	{
		recent_mode = t_mode;
		
		removeAllChildren();
		
		
	}
	
private:
	
	int puzzle_number;
	int stage_number;
	CCObject* menu_target;
	SEL_MenuHandler menu_delegate;
	StageNodeViewMode recent_mode;
	
	void myInit(int t_puzzle_number, int t_stage_number, CCObject* t_target, SEL_MenuHandler t_delegate)
	{
		puzzle_number = t_puzzle_number;
		stage_number = t_stage_number;
		menu_target = t_target;
		menu_delegate = t_delegate;
	}
};

#endif /* defined(__DGproto__StageNode__) */
