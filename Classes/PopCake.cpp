#include "PopCake.h"
#include "bustMorphing.h"
PopCake::PopCake()
{
	
}
PopCake::~PopCake()
{
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
	MyNode* n = MyNode::create(); // CCTextureCache::sharedTextureCache()->addImage("bmTest.png"));
	n->setPosition(ccp(0, 0));
	n->setContentSize(CCSizeMake(640, 860));
	n->setScale(0.5);
	n->setAnchorPoint(ccp(0.5, 0.5));
	addChild(n);
	
	CommonButton* cb = CommonButton::create("load", 20, CCSizeMake(100, 100), CommonButtonOrange,
																					0);
	addChild(cb, 1);
	cb->setPosition(ccp( 300, 200));
	cb->setFunction([=](CCObject* obj){
		n->init(CCTextureCache::sharedTextureCache()->addImage("bmTest.png"));
		cb->setVisible(false);
	});
	
	CommonButton* cb2 = CommonButton::create("load RGB", 20, CCSizeMake(100, 100), CommonButtonOrange,
																					 0);
	addChild(cb2, 1);
	cb2->setPosition(ccp( 300, 100));
	cb2->setFunction([=](CCObject* obj){
		n->putBasicInfomation();	// 기본정보 들어가게.
		n->loadRGB(CCFileUtils::sharedFileUtils()->fullPathForFilename("bmTest2.png").c_str()); // 실루엣 z 정보 넣는 곳.
		n->triangulationWithPoints();
	});
	return true;
}


