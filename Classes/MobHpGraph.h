//
//  MobHpGraph.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#ifndef __DGproto__MobHpGraph__
#define __DGproto__MobHpGraph__

#include "cocos2d.h"
#include "KSCumberBase.h"

USING_NS_CC;

class MobHpGraph : public CCNode
{
public:
	static MobHpGraph* create(CCObject* t_target)
	{
		MobHpGraph* t_graph = new MobHpGraph();
		t_graph->myInit(t_target);
		t_graph->autorelease();
		return t_graph;
	}
	
	CCObject* getTargetNode()
	{
		return target_node;
	}
	
private:
	KSCumberBase* target_node;
	float max_life;
	float last_life;
	CCProgressTimer* hp_progress;
	CCSprite* t_case;
	
	void startGraph()
	{
		schedule(schedule_selector(MobHpGraph::graphing));
	}
	
	void graphing()
	{
		if(target_node->getLife() != last_life)
		{
			t_case->stopAllActions();
			hp_progress->stopAllActions();
			
			t_case->setVisible(true);
			hp_progress->setVisible(true);
			
			CCProgressFromTo* progress_to = CCProgressFromTo::create(0.3f, last_life/max_life*100.f, target_node->getLife()/max_life*100.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MobHpGraph::hidingAction));
			hp_progress->runAction(CCSequence::createWithTwoActions(progress_to, t_call));
			last_life = target_node->getLife();
		}
		
		setPosition(ccpAdd(target_node->getPosition(), ccp(0,-20)));
	}
	
	void hidingAction()
	{
		CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
		CCHide* t_hide1 = CCHide::create();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.5f);
		CCHide* t_hide2 = CCHide::create();
		
		t_case->runAction(CCSequence::createWithTwoActions(t_delay1, t_hide1));
		hp_progress->runAction(CCSequence::createWithTwoActions(t_delay2, t_hide2));
	}
	
	void myInit(CCObject* t_target)
	{
		t_case = CCSprite::create("monster_hp_back.png");
		t_case->setPosition(CCPointZero);
		addChild(t_case);
		
		CCSprite* t_graph = CCSprite::create("monster_hp_bar.png");
		
		hp_progress = CCProgressTimer::create(t_graph);
		hp_progress->setType(kCCProgressTimerTypeBar);
		hp_progress->setMidpoint(ccp(0,0));
		hp_progress->setBarChangeRate(ccp(1,0));
		hp_progress->setPosition(CCPointZero);
		hp_progress->setPercentage(100.f);
		addChild(hp_progress);
		
		hidingAction();
		
		target_node = (KSCumberBase*)t_target;
		last_life = 100.f;
		max_life = target_node->getTotalLife();
		
		setPosition(ccpAdd(target_node->getPosition(), ccp(0,-20)));
		
		startGraph();
	}
};

#endif /* defined(__DGproto__MobHpGraph__) */
