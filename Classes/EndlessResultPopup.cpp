#include "EndlessResultPopup.h"
#include "DataStorageHub.h"
#include "FormSetter.h"

EndlessResultPopup::EndlessResultPopup()
{
	
}
EndlessResultPopup::~EndlessResultPopup()
{
}
//void EndlessResultPopup::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool EndlessResultPopup::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool EndlessResultPopup::init()
{
	CCLayer::init();

	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kEndlessResultPopupZorder_main);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));

main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-22.f));
	addChild(main_case, kEndlessResultPopupZorder_main);
	
	main_case->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t)
	{
		KS::setOpacity(main_case, 255);
		
	}));
	
	CCScale9Sprite* inner_left = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	inner_left->setContentSize(CCSizeMake(230, 215));
	inner_left->setPosition(main_case->getContentSize().width*0.26f, main_case->getContentSize().height*0.58f);
	main_case->addChild(inner_left);
	
	
	CCScale9Sprite* inner_right = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	inner_right->setContentSize(CCSizeMake(230, 215));
	inner_right->setPosition(main_case->getContentSize().width*0.74f, main_case->getContentSize().height*0.58f);
	main_case->addChild(inner_right);
	
	
	CCScale9Sprite* star_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	star_back->setContentSize(CCSizeMake(215, 60));
	star_back->setPosition(ccp(115,140));
	inner_left->addChild(star_back);
	
	FormSetter::get()->addObject("left", inner_left, [=](Json::Value t){
		inner_left->setContentSize(CCSizeMake(t["cx"].asFloat(), t["cy"].asFloat()));
	});
	
	FormSetter::get()->addObject("right", inner_right, [=](Json::Value t){
		inner_right->setContentSize(CCSizeMake(t["cx"].asFloat(), t["cy"].asFloat()));
	});
	
	FormSetter::get()->addObject("info", star_back, [=](Json::Value t){
		star_back->setContentSize(CCSizeMake(t["cx"].asFloat(), t["cy"].asFloat()));
	});
	
//
//	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
//	main_case->setContentSize(CCSizeMake(480, 280));
//	main_case->setPosition(ccp(240,160-450));
//	addChild(main_case, kAchievePopupZorder_back);
		
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake(310, 232));
	right_back->setPosition(main_case->getContentSize().width*0.66f, main_case->getContentSize().height*0.44f);
	main_case->addChild(right_back);
	
	return true;
}



