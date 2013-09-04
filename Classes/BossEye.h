//
//  BossEye.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 29..
//
//

#ifndef __DrawingJack__BossEye__
#define __DrawingJack__BossEye__

#include "cocos2d.h"
#include "SelectedMapData.h"

using namespace cocos2d;

/*
 step 1 : 1
 step 2 : 1 - 2
 step 3 : 2
 step 4 : 2 - 3
 step 5 : 3
 step 6 : 3 - 4
 step 7 : 4
 */

class BossEye : public CCSprite
{
public:
	static BossEye* create(int t_type);
	void setStep(int t_step);
	
private:
	int frame_cnt;
	int my_step;
	CCSize frame_size;
	
	void myAnimation();
	void myInit(int t_type);
};

#endif /* defined(__DrawingJack__BossEye__) */
