#pragma once

#include "cocos2d.h"
USING_NS_CC;

#define ReplaceScene(X) (CCDirector::sharedDirector()->replaceScene(LoadingScene<X>::scene()))
#define PushScene(X) (CCDirector::sharedDirector()->pushScene(LoadingScene<X>::scene()))

template<typename T>
class LoadingScene : public CCLayer
{
private:
	virtual void onEnterTransitionDidFinish()
	{
		CCLayer::onEnterTransitionDidFinish();
		CCDirector::sharedDirector()->replaceScene(T::scene());
	}
public:
	static CCScene* scene()
	{
		CCScene * scene = CCScene::create();
		LoadingScene* layer = LoadingScene::create();
		scene->addChild(layer);
		return scene;
	}
	
	// implement the "static node()" method manually
	CREATE_FUNC(LoadingScene);
};
