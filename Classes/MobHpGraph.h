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

USING_NS_CC;

class MobHpGraph : public CCNode
{
public:
	static MobHpGraph* create()
	{
		MobHpGraph* t_graph = new MobHpGraph();
		t_graph->myInit();
		t_graph->autorelease();
		return t_graph;
	}
	
private:
	
	void myInit()
	{
		
	}
};

#endif /* defined(__DGproto__MobHpGraph__) */
