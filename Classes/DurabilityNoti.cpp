//
//  DurabilityNoti.cpp
//  DGproto
//
//  Created by 사원3 on 13. 11. 11..
//
//

#include "DurabilityNoti.h"

DurabilityNoti* DurabilityNoti::create( CCObject* t_cancel, SEL_MenuHandler d_cancel, CCObject* t_ok, SEL_MenuHandler d_ok )
{
	DurabilityNoti* t_tnp = new DurabilityNoti();
	t_tnp->myInit(t_cancel, d_cancel, t_ok, d_ok);
	t_tnp->autorelease();
	return t_tnp;
}

void DurabilityNoti::myInit( CCObject* t_cancel, SEL_MenuHandler d_cancel, CCObject* t_ok, SEL_MenuHandler d_ok )
{
	target_cancel = t_cancel;
	delegate_cancel = d_cancel;
	target_ok = t_ok;
	delegate_ok = d_ok;

	back_img = NULL;
	cancel_menu = NULL;
	ok_menu = NULL;

	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;

	CCSprite* gray = CCSprite::create("back_gray.png");
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kDN_Z_gray);

	setBack();

	is_menu_enable = true;

	touched_number = 0;

	setTouchEnabled(true);
}

void DurabilityNoti::setBack()
{
	back_img = CCSprite::create("durability_noti_case.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kDN_Z_back);

	CCSprite* n_ok = CCSprite::create("stagesetting_start.png");
	CCSprite* s_ok = CCSprite::create("stagesetting_start.png");
	s_ok->setColor(ccGRAY);

	CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, target_ok, delegate_ok);
	ok_item->setTag(kSSP_MT_noti_ok);
	ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setPosition(ccp(296, 115));
	addChild(ok_menu, kDN_Z_content);


	CCSprite* n_cancel = CCSprite::create("durability_noti_cancel.png");
	CCSprite* s_cancel = CCSprite::create("durability_noti_cancel.png");
	s_cancel->setColor(ccGRAY);

	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, target_cancel, delegate_cancel);
	cancel_item->setTag(kSSP_MT_noti_cancel);
	cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(175, 115));
	addChild(cancel_menu, kDN_Z_content);
}

CCPoint DurabilityNoti::getContentPosition( int t_tag )
{
	CCPoint return_value;



	return return_value;
}

bool DurabilityNoti::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number != 0)		return true;
	if(cancel_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kDN_MT_cancel;
	else if(ok_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kDN_MT_ok;
	return true;
}

void DurabilityNoti::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kDN_MT_cancel)							cancel_menu->ccTouchMoved(pTouch, pEvent);
	else if(touched_number == kDN_MT_ok)						ok_menu->ccTouchMoved(pTouch, pEvent);
}

void DurabilityNoti::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kDN_MT_cancel){			cancel_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	else if(touched_number == kDN_MT_ok){			ok_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;	}
}

void DurabilityNoti::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	if(touched_number == kDN_MT_cancel){			cancel_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	else if(touched_number == kDN_MT_ok){			ok_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
}

void DurabilityNoti::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
