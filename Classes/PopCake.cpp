#include "PopCake.h"
#include "bustMorphing.h"
#include "FormSetter.h"

#include "StageImgLoader.h"
#include "StarGoldData.h"
#include "FormSetter.h"

PopCake::PopCake()
{
	
}
PopCake::~PopCake()
{
	mySIL->removeTD();
}
//void PopCake::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool PopCake::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool PopCake::init()
{
	CCLayer::init();
	
//	MyNode* n = MyNode::create(CCTextureCache::sharedTextureCache()->addImage("bmTest.png"));
//	n->setPosition(ccp(0, 0));
//	n->setContentSize(CCSizeMake(640, 860));
//	n->setScale(0.5);
//	n->setAnchorPoint(ccp(0.5, 0.5));
//	addChild(n);
	
	n = MyNode::create(); // CCTextureCache::sharedTextureCache()->addImage("bmTest.png"));
	n->setPosition(ccp(240, 160));
	//n->setContentSize(CCSizeMake(640, 860));
//	n->setScale(0.5);
	n->setAnchorPoint(ccp(0.5f, .5f));
	addChild(n);
	FormSetter::get()->addObject("bobj",n);
	
	cb = CommonButton::create("load", 20, CCSizeMake(100, 30), CommonButtonOrange,
																					0);
	addChild(cb, 1);
	cb->setPosition(ccp( 420, 300));
	cb->setFunction([=](CCObject* obj){
		
		FormSetter::get()->requestFormDataOnce([=](){
			Json::Value v =  FormSetter::get()->getFormData("bustmorphing");
			
			mySIL->downloadImg(v["original"].asString(), 999, "morphing_img.png", this, callfunc_selector(PopCake::successOriginalAction), this, callfunc_selector(PopCake::failOriginalAction));
		});
		
		cb->setVisible(false);
	});
	
	CCLog("cb2");
	cb2 = CommonButton::create("load RGB 11 ", 20, CCSizeMake(100, 35), CommonButtonOrange,
																					 0);
	addChild(cb2, 1);
	cb2->setPosition(ccp( 420, 260));
	cb2->setFunction([=](CCObject* obj){
		
		FormSetter::get()->requestFormDataOnce([=](){
			Json::Value v =  FormSetter::get()->getFormData("bustmorphing");
			
			mySIL->downloadImg(v["rgb"].asString(), 999, "morphing_rgb.png", this, callfunc_selector(PopCake::successRgbAction), this, callfunc_selector(PopCake::failRgbAction));
		});
		
		cb2->setVisible(false);
	});
	
	{
		CommonButton* cb = CommonButton::create("left", 20, CCSizeMake(100, 35), CommonButtonOrange,
																						0);
		addChild(cb, 1);
		cb->setPosition(ccp( 420, 220));
		cb->setFunction([=](CCObject* obj){
			n->setImageRotationDegree(n->getImageRotationDegree() - 10);
		});
		
		CommonButton* cb2 = CommonButton::create("right", 20, CCSizeMake(100, 35), CommonButtonOrange,
																						 0);
		addChild(cb2, 1);
		cb2->setPosition(ccp( 420, 180));
		cb2->setFunction([=](CCObject* obj){
			n->setImageRotationDegree(n->getImageRotationDegree() + 10);
		});
	}
	{
		{
			CommonButton* cb = CommonButton::create("up", 20, CCSizeMake(100, 35), CommonButtonOrange,
																							0);
			addChild(cb, 1);
			cb->setPosition(ccp( 420, 140));
			cb->setFunction([=](CCObject* obj){
				n->setImageRotationDegreeX(n->getImageRotationDegreeX() - 10);
			});
			
			CommonButton* cb2 = CommonButton::create("down", 20, CCSizeMake(100, 35), CommonButtonOrange,
																							 0);
			addChild(cb2, 1);
			cb2->setPosition(ccp( 420, 100));
			cb2->setFunction([=](CCObject* obj){
				n->setImageRotationDegreeX(n->getImageRotationDegreeX() + 10);
			});
		}
	}
	{
		{
			CommonButton* cb = CommonButton::create("zoom in", 20, CCSizeMake(100, 35), CommonButtonOrange,
																							0);
			addChild(cb, 1);
			cb->setPosition(ccp( 420, 60));
			cb->setFunction([=](CCObject* obj){
				n->setScale(n->getScale() + 0.1f);
			});
			
			CommonButton* cb2 = CommonButton::create("zoom out", 20, CCSizeMake(100, 30), CommonButtonOrange,
																							 0);
			addChild(cb2, 1);
			cb2->setPosition(ccp( 420, 20));
			cb2->setFunction([=](CCObject* obj){
				n->setScale(n->getScale() - 0.1f);

			});
		}
	}
	return true;
}

void PopCake::successOriginalAction()
{
	n->init(mySIL->addImage("morphing_img.png"));
}



void PopCake::failOriginalAction()
{
	cb->setVisible(true);
	
	CCLabelTTF* fail_label = CCLabelTTF::create("오리지날 다운로드 실패", mySGD->getFont().c_str(), 12);
	fail_label->setPosition(ccp(240,160));
	addChild(fail_label);
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(fail_label, callfunc_selector(CCLabelTTF::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	fail_label->runAction(t_seq);
}

void PopCake::successRgbAction()
{
	cb2->setVisible(true);
	
	n->putBasicInfomation();	// 기본정보 들어가게.
	n->loadRGB(CCFileUtils::sharedFileUtils()->fullPathForFilename("bmTest2.png").c_str()); // 실루엣 z 정보 넣는 곳.
//	n->loadRGB(mySIL->getDocumentPath() + "morphing_rgb.png"); // 실루엣 z 정보 넣는 곳.
	n->triangulationWithPoints();
}

void PopCake::failRgbAction()
{
	cb2->setVisible(true);
	
	CCLabelTTF* fail_label = CCLabelTTF::create("알지비 다운로드 실패", mySGD->getFont().c_str(), 12);
	fail_label->setPosition(ccp(240,160));
	addChild(fail_label);
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(fail_label, callfunc_selector(CCLabelTTF::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	fail_label->runAction(t_seq);
}

