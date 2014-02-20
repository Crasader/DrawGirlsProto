//// LogoutPopup.cpp
////
//
//#include "LogoutPopup.h"
//#include "TitleRenewal.h"
//#define LZZ_INLINE inline
//using namespace std;
//LogoutPopup * LogoutPopup::create (CCObject * t_close, SEL_CallFunc d_close)
//{
//	LogoutPopup* t_lp = new LogoutPopup();
//	t_lp->myInit(t_close, d_close);
//	t_lp->autorelease();
//	return t_lp;
//}
//void LogoutPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
//{
//	target_close = t_close;
//	delegate_close = d_close;
//	
//	CCSprite* gray = CCSprite::create("back_gray.png");
//	gray->setPosition(ccp(240,160));
//	addChild(gray, kLP_Z_gray);
//	
//	CCSprite* back = CCSprite::create("option_logout_back.png");
//	back->setPosition(ccp(240,160));
//	addChild(back, kLP_Z_back);
//	
//	CCSprite* n_logout = CCSprite::create("option_real_logout.png");
//	CCSprite* s_logout = CCSprite::create("option_real_logout.png");
//	s_logout->setColor(ccGRAY);
//	
//	CCMenuItem* logout_item = CCMenuItemSprite::create(n_logout, s_logout, this, menu_selector(LogoutPopup::menuAction));
//	logout_item->setTag(kLP_MT_logout);
//	
//	logout_menu = CCMenu::createWithItem(logout_item);
//	logout_menu->setPosition(getContentPosition(kLP_MT_logout));
//	addChild(logout_menu, kLP_Z_content);
//	
//	
//	CCSprite* n_close = CCSprite::create("option_long_close.png");
//	CCSprite* s_close = CCSprite::create("option_long_close.png");
//	s_close->setColor(ccGRAY);
//	
//	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(LogoutPopup::menuAction));
//	close_item->setTag(kLP_MT_close);
//	
//	close_menu = CCMenu::createWithItem(close_item);
//	close_menu->setPosition(getContentPosition(kLP_MT_close));
//	addChild(close_menu, kLP_Z_content);
//	
//	is_menu_enable = true;
//	
//	touched_number = 0;
//	
//	setTouchEnabled(true);
//}
//CCPoint LogoutPopup::getContentPosition (int t_tag)
//{
//	CCPoint return_value;
//	
//	if(t_tag == kLP_MT_close)	return_value = ccp(241,97);
//	else if(t_tag == kLP_MT_logout)	return_value = ccp(241,160);
//	
//	return return_value;
//}
//void LogoutPopup::menuAction (CCObject * sender)
//{
//	if(!is_menu_enable)
//		return;
//	
//	is_menu_enable = false;
//	
//	int tag = ((CCNode*)sender)->getTag();
//	
//	if(tag == kLP_MT_close)
//	{
//		(target_close->*delegate_close)();
//		removeFromParent();
//	}
//	else if(tag == kLP_MT_logout)
//	{
//		hspConnector::get()->logout(json_selector(this, LogoutPopup::resultLogoutAction));
//	}
//}
//void LogoutPopup::resultLogoutAction (Json::Value result_data)
//{
//	CCLog("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
//	if(result_data["error"]["isSuccess"].asBool())
//	{
//		myDSH->clear();
//		myDSH->resetDSH();
//		CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
//	}
//	else
//	{
//		is_menu_enable = true;
//	}
//}
//bool LogoutPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
//{
//	if(touched_number != 0)		return true;
//	if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kLP_MT_close;
//	else if(logout_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kLP_MT_logout;
//	return true;
//}
//void LogoutPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
//{
//	if(touched_number == kLP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
//	else if(touched_number == kLP_MT_logout)					logout_menu->ccTouchMoved(pTouch, pEvent);
//}
//void LogoutPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
//{
//	if(touched_number == kLP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
//	else if(touched_number == kLP_MT_logout){	logout_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
//}
//void LogoutPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
//{
//	if(touched_number == kLP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
//	else if(touched_number == kLP_MT_logout){	logout_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
//}
//void LogoutPopup::registerWithTouchDispatcher ()
//{
//	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//	pDispatcher->addTargetedDelegate(this, -170, true);
//}
//#undef LZZ_INLINE
