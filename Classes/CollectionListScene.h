//
//  CollectionListScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 20..
//
//

#ifndef __DGproto__CollectionListScene__
#define __DGproto__CollectionListScene__

#include "cocos2d.h"
#include "AlertEngine.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CollectionView.h"

USING_NS_CC;

class CollectionListScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CollectionListScene);
	
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	CCMenu* close_menu;
	CollectionView* collection_view;
};

#endif /* defined(__DGproto__CollectionListScene__) */
