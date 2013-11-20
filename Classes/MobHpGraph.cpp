// MobHpGraph.cpp
//

#include "MobHpGraph.h"
#define LZZ_INLINE inline
MobHpGraph * MobHpGraph::create (CCObject * t_target)
        {
		MobHpGraph* t_graph = new MobHpGraph();
		t_graph->myInit(t_target);
		t_graph->autorelease();
		return t_graph;
	}
CCObject * MobHpGraph::getTargetNode ()
        {
		return target_node;
	}
void MobHpGraph::setGameover ()
        {
		unschedule(schedule_selector(MobHpGraph::graphing));
		t_case->stopAllActions();
		hp_progress->stopAllActions();
		
		t_case->setVisible(true);
		hp_progress->setVisible(true);
		
		CCProgressFromTo* progress_to = CCProgressFromTo::create(0.3f, last_life/max_life*100.f, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MobHpGraph::realHidingAction));
		hp_progress->runAction(CCSequence::createWithTwoActions(progress_to, t_call));
	}
void MobHpGraph::startGraph ()
        {
		schedule(schedule_selector(MobHpGraph::graphing));
	}
void MobHpGraph::graphing ()
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
void MobHpGraph::realHidingAction ()
        {
		CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
		CCHide* t_hide1 = CCHide::create();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.5f);
		CCHide* t_hide2 = CCHide::create();
		
		t_case->runAction(CCSequence::createWithTwoActions(t_delay1, t_hide1));
		hp_progress->runAction(CCSequence::createWithTwoActions(t_delay2, t_hide2));
	}
void MobHpGraph::hidingAction ()
        {
		if(last_life/max_life > 0.3f)
		{
			realHidingAction();
		}
		else
		{
			CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
			CCHide* t_hide1 = CCHide::create();
			CCDelayTime* t_delay2 = CCDelayTime::create(0.2f);
			CCShow* t_show1 = CCShow::create();
			CCSequence* t_seq1 = CCSequence::create(t_delay1, t_hide1, t_delay2, t_show1, NULL);
			
			CCDelayTime* t_delay3 = CCDelayTime::create(0.5f);
			CCHide* t_hide2 = CCHide::create();
			CCDelayTime* t_delay4 = CCDelayTime::create(0.2f);
			CCShow* t_show2 = CCShow::create();
			CCSequence* t_seq2 = CCSequence::create(t_delay3, t_hide2, t_delay4, t_show2, NULL);
			
			t_case->runAction(t_seq1);
			hp_progress->runAction(t_seq2);
		}
	}
void MobHpGraph::myInit (CCObject * t_target)
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
		
		target_node = (KSCumberBase*)t_target;
		last_life = target_node->getLife();
		max_life = target_node->getTotalLife();
		
		hidingAction();
		
		setPosition(ccpAdd(target_node->getPosition(), ccp(0,-20)));
		
		startGraph();
	}
#undef LZZ_INLINE
