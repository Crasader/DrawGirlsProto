//
//  StoryView.h
//  DGproto
//
//  Created by LitQoo on 2014. 3. 10..
//
//

#ifndef __DGproto__StoryView__
#define __DGproto__StoryView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "GDWebSprite.h"
#include "CommonButton.h"
#include <vector>

using namespace cocos2d::extension;
using namespace std;

/*
 
 */


class StoryView : public CCLayer, public CCBAnimationManagerDelegate{
	CCLayer *opLayer1;
	CCLayer *opLayer2;
	CCBAnimationManager *mAnimationManager1;
	CCBAnimationManager *mAnimationManager2;
	std::function<void(void)> mFunc;
public:
	//CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(StoryView, create);
	
	bool init(){
		if(!CCLayer::init())return false;
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "openning1.ccbi");
		mAnimationManager1 = ret.second;
		opLayer1 = ret.first;
		addChild(opLayer1,10000);
		mAnimationManager1->setDelegate(this);
		this->addChild(KSTimer::create(1.f, [=](){
				mAnimationManager1->runAnimationsForSequenceNamed("op1");
		}));
		
		return true;
	}
//
	static StoryView* create(){
		StoryView *ob = new StoryView();
		ob->init();
		ob->autorelease();
		return ob;
	}
	void setFunc(std::function<void(void)>func){
		mFunc=func;
	}
	void step(CCObject* pSender){

	}
	//ccb 연결
	virtual void completedAnimationSequenceNamed(const char *name)
	{
		string sn = name;
		if(sn=="op1"){
			mAnimationManager1->runAnimationsForSequenceNamed("op2");
		}else if(sn=="op2"){
			mAnimationManager1->runAnimationsForSequenceNamed("op3");
		}else if(sn=="op3"){
			
			opLayer1->removeFromParentAndCleanup(true);
			
			auto ret = KS::loadCCBI<CCLayer*>(this, "openning2.ccbi");
			mAnimationManager2 = ret.second;
			mAnimationManager2->setDelegate(this);
			opLayer2 = ret.first;
			addChild(opLayer2,10000);
			this->addChild(KSTimer::create(1.f, [=](){
				mAnimationManager2->runAnimationsForSequenceNamed("op4");
			}));
		}else if(sn=="op4"){
			mAnimationManager2->runAnimationsForSequenceNamed("op5");
		}else if(sn=="op5"){
			mAnimationManager2->runAnimationsForSequenceNamed("op6");
		}else if(sn=="op6"){
			this->addChild(KSTimer::create(3.f, [=](){
				mAnimationManager2->runAnimationsForSequenceNamed("op7");
			}));
		}else if(sn=="op7"){
			this->addChild(KSTimer::create(2.f, [=](){
				if(mFunc)mFunc();
				this->removeFromParentAndCleanup(true);
			}));
		}
		
		CCLog("completedAnimationSequenceNamed %s",name);
	}
	
//	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
//	{
//		CCLog("SEL_MenuHandler");
//		return NULL;
//	}
//	virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
//	{
//		CCLog("SEL_CallFuncN");
//		CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "step", StoryView::step);
//		return NULL;
//	}
//	
//	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
//	{
//		CCLog("SEL_CCControlHandler");
//		return NULL;
//	}
	
	//ccb 메뉴버튼콜벡
	void onStartClicked(CCObject* pSender, CCControlEvent pCCControlEvent);
	
	//ccb 애니메이션 콜백
	void onEndShowEditButton(CCObject* pSender);
	//ccb 애니메이션 호출
	
};


#endif /* defined(__DGproto__StoryView__) */
