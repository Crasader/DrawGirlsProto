//
//  ASPopupView.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 26..
//
//

#include "ASPopupView.h"
#include "StarGoldData.h"
#include "CCMenuLambda.h"
#include "DataStorageHub.h"

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
	t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCLabelTTF* ment_label = CCLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 15);
	ment_label->setColor(ccBLACK);
	ment_label->setPosition(ccp(0,-17));
	t_container->addChild(ment_label);
	
	case_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+40, ment_label->getContentSize().height + 70));
	
	CCLabelTTF* title_img = CCLabelTTF::create("알림", mySGD->getFont().c_str(), 17);
	title_img->setPosition(ccp(0, case_back->getContentSize().height/2.f-22));
	t_container->addChild(title_img);
	
	CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
																		{
																			t_popup->removeFromParent();
																		});
	
	CCMenuLambda* close_menu = CCMenuLambda::createWithItem(close_item);
	close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
	close_menu->setPosition(ccp(case_back->getContentSize().width/2.f-n_close->getContentSize().width/2.f+1,
								case_back->getContentSize().height/2.f-n_close->getContentSize().height/2.f+2));
	t_container->addChild(close_menu);
	
	return t_popup;
}