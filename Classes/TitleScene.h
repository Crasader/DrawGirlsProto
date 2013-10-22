//
//  TitleScene.h
//  DGproto
//
//  Created by LitQoo on 13. 10. 22..
//
//

#ifndef __DGproto__TitleScene__
#define __DGproto__TitleScene__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "PuzzleMapScene.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

class TitleScene : public cocos2d::CCLayer
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(TitleScene);
	
private:
	
	void resultLogin(Json::Value result_data)
	{
		CCLog("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
	
};

#endif /* defined(__DGproto__TitleScene__) */
