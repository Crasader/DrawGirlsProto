//
//  OptionScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 29..
//
//

#ifndef __DGproto__OptionScene__
#define __DGproto__OptionScene__

#include "cocos2d.h"
#include "AlertEngine.h"
#include "GameData.h"
#include "DataStorageHub.h"

USING_NS_CC;
using namespace std;

class OptionScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(OptionScene);
	
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);

	CCSprite* bgm_img;
	CCSprite* effect_img;
	
	CCMenu* bgm_menu;
	CCMenu* effect_menu;
	
	CCPoint getContentPosition(int t_tag);
	
	void resetBgmMenu();
	void resetEffectMenu();

	void menuAction(CCObject* sender);
	void popupClose();
};

#endif /* defined(__DGproto__OptionScene__) */
