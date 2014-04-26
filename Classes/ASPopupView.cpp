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
#include "CommonButton.h"
#include "AudioEngine.h"

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment)
{
	return getCommonNoti(t_touch_priority, t_comment, [](){});
}

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment, function<void()> close_func)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	float height_value = 320.f;
	if(myDSH->screen_convert_rate < 1.f)
		height_value = 320.f/myDSH->screen_convert_rate;
	
	if(height_value < myDSH->ui_top)
		height_value = myDSH->ui_top;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, 160));
	t_popup->setBasePosition(ccp(240, 160));
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	content_back->setPosition(CCPointZero);
	t_container->addChild(content_back);
	
	CCLabelTTF* ment_label = CCLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 15);
	ment_label->setPosition(ccp(0,0));
	t_container->addChild(ment_label);
	
	case_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+60, ment_label->getContentSize().height + 40));
	content_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+40, ment_label->getContentSize().height + 20));
	
	CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	close_button->setPosition(ccp(case_back->getContentSize().width/2.f-10,
								  case_back->getContentSize().height/2.f-10));
	close_button->setFunction([=](CCObject* sender)
							  {
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  close_func();
								  t_popup->removeFromParent();
							  });
	t_container->addChild(close_button);
	
	return t_popup;
}