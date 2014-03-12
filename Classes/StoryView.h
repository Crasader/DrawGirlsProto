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
#include "KSLabelTTF.h"
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
	string nextAni;
	CommonButton *nextBtn;
	KSLabelTTF *scriptLbl;
public:
	//CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(StoryView, create);
	
	bool init(){
		if(!CCLayer::init())return false;
		
		nextAni="";
		
		
		//load ani
		auto ret = KS::loadCCBI<CCLayer*>(this, "openning1.ccbi");
		mAnimationManager1 = ret.second;
		opLayer1 = ret.first;
		addChild(opLayer1,2);
		mAnimationManager1->setDelegate(this);

		
		//make button
		nextBtn =CommonButton::create("다음", 15, CCSizeMake(60, 40), CommonButtonOrange, -300);
		nextBtn->setPosition(ccp(440,20));
		this->addChild(nextBtn,3);
		
		nextBtn->setFunction([=](CCObject *btn){
			
			if(nextAni=="op1"){
				scriptLbl->setString("책에는 아름다움을 시기하는 마녀에 대한 이야기가 쓰여져 있었습니다.");
			}else if(nextAni=="op2"){
				scriptLbl->setString("재미있게 글을 읽던 케이가 마지막장을 펼치자\n 책에서 이상한 빛이 새어 나왔습니다.");
			}else if(nextAni=="op5"){
				scriptLbl->setString("마녀가 케이의 머리를 쓰다듬자\n 케이은 자신도 모르게 깊은 잠에 빠져버렸습니다.");
			}else if(nextAni=="op6"){
				scriptLbl->setString("그렇게 세상으로 나온 마녀는 시공간을 뛰어넘어\n 과거와 현재, 현실과 책속을 넘나들며\n 자신보다 아름다운 미녀들을 그림카드에 봉인했습니다.");
			}else if(nextAni=="op7"){
				scriptLbl->setString("'이제 내가 가장 아름다운 사람이지 않을까? 호호호'");
				this->addChild(KSTimer::create(3.f, [=](){
					scriptLbl->setString("그리고 책 앞으로 돌아온 마녀는 다시 고서적으로 사라졌습니다.");
				}));
			}else if(nextAni=="op8"){
				scriptLbl->setString("그때 잠들었던 케이도 차원의 소용돌이에 휘말려 \n 책속 세상으로 빨려들어가고 말았습니다.");
			}else if(nextAni=="end"){
				scriptLbl->setString("케이는 어떻게 되는걸까요?");
			}
			
			
			
			nextBtn->setVisible(false);
				if(nextAni=="end"){
					this->addChild(KSTimer::create(5.f, [=](){
						if(mFunc)mFunc();
						this->removeFromParentAndCleanup(true);
					}));
					return;
				}
				CCLog("play next %s",nextAni.c_str());
				mAnimationManager1->runAnimationsForSequenceNamed(nextAni.c_str());
				this->nextBtn->setVisible(false);
			
		});
		
		
		scriptLbl = KSLabelTTF::create("케이는 다락방에서 오래된 책을 한 권 발견했습니다.", mySGD->getFont().c_str(), 15, CCSizeMake(450, 50), kCCTextAlignmentCenter);
		scriptLbl->enableOuterStroke(ccBLACK, 1.f);
		scriptLbl->setPosition(ccp(240,40));
		this->addChild(scriptLbl,3);
		
		
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
		if(sn=="op0"){
			nextBtn->setVisible(true);
			nextAni = "op1";
		}if(sn=="op1"){
			nextBtn->setVisible(true);
			nextAni = "op2";
		}else if(sn=="op2"){
			nextBtn->setVisible(true);
			nextAni = "op3";
		}else if(sn=="op3"){
			nextBtn->setVisible(false);
			
			auto ret = KS::loadCCBI<CCLayer*>(this, "openning2.ccbi");
			mAnimationManager1 = ret.second;
			mAnimationManager1->setDelegate(this);
			opLayer2 = ret.first;
			addChild(opLayer2,1);
			this->addChild(KSTimer::create(1.f, [=](){
				mAnimationManager1->runAnimationsForSequenceNamed("op4");
				
				scriptLbl->setString("'호호호 이제야 좀 살것 같네.' \n 눈부신 빛과 함께 마녀가 실제로 나타났습니다.");
			}));
			
			
			opLayer1->removeFromParentAndCleanup(true);
		}else if(sn=="op4"){
			nextBtn->setVisible(true);
			nextAni="op5";
			//mAnimationManager1->runAnimationsForSequenceNamed("op5");
		}else if(sn=="op5"){
			nextBtn->setVisible(true);
			nextAni="op6";
			//mAnimationManager1->runAnimationsForSequenceNamed("op6");
		}else if(sn=="op6"){
			nextBtn->setVisible(true);
			nextAni="op7";
//			this->addChild(KSTimer::create(3.f, [=](){
//				mAnimationManager1->runAnimationsForSequenceNamed("op7");
//			}));
		}else if(sn=="op7"){
			nextBtn->setVisible(true);
			nextAni="op8";
		}else if(sn=="op8"){
			nextBtn->setVisible(true);
			nextAni="end";
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
