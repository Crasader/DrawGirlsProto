//
//  SnowBackNode.h
//  DGproto
//
//  Created by 사원3 on 2014. 12. 16..
//
//

#ifndef __DGproto__SnowBackNode__
#define __DGproto__SnowBackNode__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class SnowNode : public CCSprite
{
public:
	static SnowNode* create();
	void myInit(function<void(int)> t_func);
	
private:
	function<void(int)> action_func;
	int ing_frame;
	void mySchedule();
};

class SnowBackNode : public CCSpriteBatchNode
{
public:
	void myInit();
	float wind_value;
	
private:
	int ing_frame_created;
	int frame_created;
	float screen_width;
	float screen_height;
	
	void snowingSchedule();
	virtual void onEnter();
};

#endif /* defined(__DGproto__SnowBackNode__) */
