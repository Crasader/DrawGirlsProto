//
//  TutorialScene.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 30..
//
//

#ifndef __DGproto__TutorialScene__
#define __DGproto__TutorialScene__

#include "cocos2d.h"

USING_NS_CC;

class TutorialScene : public CCLayer
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(TutorialScene);
	
private:
	
	bool is_menu_enable;
	
	void menuAction(CCObject* sender);
	
	CCPoint getContentPosition(int t_tag);
};

#endif /* defined(__DGproto__TutorialScene__) */
