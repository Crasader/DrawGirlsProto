//
//  GachaBase.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 5..
//
//

#ifndef __DGproto__GachaBase__
#define __DGproto__GachaBase__

#include "cocos2d.h"

USING_NS_CC;

class GachaBase : public CCLayer
{
private:
	CCObject* target_fianl;
	SEL_CallFunc delegate_final;
public:
	void setFinalAction(CCObject* t_final, SEL_CallFunc d_final)
	{
		target_fianl = t_final;
		delegate_final = d_final;
	}
	
	void finalAction()
	{
		(target_fianl->*delegate_final)();
	}
	

};

#endif /* defined(__DGproto__GachaBase__) */
