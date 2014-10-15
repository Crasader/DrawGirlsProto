//
//  ASPopupView.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 26..
//
//

#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "DataStorageHub.h"
#include "CommonButton.h"
#include "AudioEngine.h"
#include "MyLocalization.h"
#include "FormSetter.h"
#include "CommonAnimation.h"


bool CommonNotiDuplicateManager::isOnTag(int t_tag)
{
	auto iter = on_tag_list.find(t_tag);
	if(iter == on_tag_list.end())
		return false;
	else
		return true;
}

void CommonNotiDuplicateManager::onTagAction(int t_tag)
{
	auto iter = on_tag_list.find(t_tag);
	if(iter != on_tag_list.end())
	{
		// vector<function<void()>> iter->second
		int loop_cnt = iter->second.size();
		for(int i=0;i<loop_cnt;i++)
		{
			iter->second[i]();
		}
		on_tag_list.erase(iter);
	}
}

void CommonNotiDuplicateManager::addTagFunction(int t_tag, function<void()> t_func)
{
	auto iter = on_tag_list.find(t_tag);
	if(iter != on_tag_list.end())
	{
		iter->second.push_back(t_func);
	}
	else
	{
		vector<function<void()>> func_list;
		func_list.clear();
		func_list.push_back(t_func);
		on_tag_list[t_tag] = func_list;
	}
}

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment)
{
	return getCommonNoti(t_touch_priority, t_comment, [](){}, CCPointZero);
}

//ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, const char* t_title, const char* t_comment)
//{
//	return getCommonNoti(t_touch_priority, t_title, t_comment, [](){});
//}


ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_title, string t_comment)
{
	return getCommonNoti(t_touch_priority, t_title, t_comment, [](){});
}


ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_title, CCNode* ment_label,
																				function<void()> close_func,
																				float titleFontSize, CCPoint t_position, bool Xbutton)
{
	return getCommonNoti(t_touch_priority, t_title,ment_label,close_func,titleFontSize,t_position,Xbutton,false,nullptr);
}

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_title, string t_comment,
																				function<void()> close_func,
																				float titleFontSize, CCPoint t_position, bool Xbutton, bool towButton,function<void()> close_func2){

	KSLabelTTF* ment_label = KSLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 12);
	ment_label->disableOuterStroke();

	return getCommonNoti(t_touch_priority, t_title,  ment_label,
																								  close_func,
																								  titleFontSize,  t_position,  Xbutton,  towButton,close_func2);
}
ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_title, CCNode* ment_label,
																				function<void()> close_func,
																				float titleFontSize, CCPoint t_position, bool Xbutton, bool towButton,function<void()> close_func2){
	

	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	startFormSetter(t_popup);
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
	
	if(t_position.equals(CCPointZero))
	{
		if(mySGD->is_on_maingame)
		{
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		}
		else
		{
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
		}
	}
	else
	{
		t_popup->setDimmedPosition(t_position);
		t_popup->setBasePosition(t_position);
	}
	
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCSprite* case_back = CCSprite::create("popup_small_back.png");
	setFormSetter(case_back);
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	setFormSetter(content_back);
	content_back->setContentSize(CCSizeMake(251, 113));
	content_back->setPosition(ccp(0.0,-12)); 			// dt (0.0,-4.5)
	t_container->addChild(content_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(t_title.c_str(), mySGD->getFont().c_str(), titleFontSize);
	setFormSetter(title_label);
//	title_label->setColor(ccc3(255, 170, 20));
	title_label->disableOuterStroke();
	title_label->setPosition(ccp(-85,case_back->getContentSize().height/2.f-35));
	t_container->addChild(title_label);
	
//	setFormSetter(ment_label);
//	CCSize pSize = CCSizeMake(150, 20);
//	
//	if(ment_label->getContentSize().width>pSize.width)pSize.width = ment_label->getContentSize().width;
//	if(ment_label->getContentSize().height>pSize.height)pSize.height = ment_label->getContentSize().height;
	ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
//	ment_label->setPosition(ccp(0,5));
	content_back->addChild(ment_label);
	
//	case_back->setContentSize(CCSizeMake(pSize.width+50, pSize.height + 40+80 - 40));
//	content_back->setContentSize(CCSizeMake(pSize.width+39, pSize.height+76+2.5 - 40));
	
	setFormSetter(case_back);
	setFormSetter(content_back);
	
	if(Xbutton)
	{
		CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
		setFormSetter(close_button);
		close_button->setPosition(ccp(case_back->getContentSize().width/2.f - 25, case_back->getContentSize().height/2.f - 22));
		close_button->setFunction([=](CCObject* sender)
															{
//																t_popup->setBackKeyEnabled(false);
																AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																
																if(t_popup->duplicate_tag == -1)
																{
																	CommonAnimation::closePopup(t_popup, t_container,
																								t_popup->getDimmedSprite(), nullptr,
																								[=](){
																									if(close_func && !towButton)
																										close_func();
																									
																									if(close_func && towButton)
																										close_func2();
																									
																									t_popup->removeFromParent();
																								});
																}
																else
																{
																	CommonNotiDuplicateManager::sharedInstance()->onTagAction(t_popup->duplicate_tag);
																	CommonAnimation::closePopup(t_popup, t_container,
																								t_popup->getDimmedSprite(), nullptr,
																								[=](){
																									t_popup->removeFromParent();
																								});
																}
															});
		t_container->addChild(close_button);
		
	}
	
	if((Xbutton && towButton) || !Xbutton){
		CommonButton* close_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), 12, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), t_popup->getTouchPriority()-5);
		setFormSetter(close_button);
		close_button->setPosition(ccp(0,case_back->getContentSize().height/2.f*-1+45));
		close_button->setFunction([=](CCObject* sender)
															{
																AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																
																if(t_popup->duplicate_tag == -1)
																{
																	CommonAnimation::closePopup(t_popup, t_container,
																								t_popup->getDimmedSprite(), nullptr,
																								[=](){
																									if(close_func)
																										close_func();
																									t_popup->removeFromParent();
																								});
																}
																else
																{
																	CommonNotiDuplicateManager::sharedInstance()->onTagAction(t_popup->duplicate_tag);
																	CommonAnimation::closePopup(t_popup, t_container,
																								t_popup->getDimmedSprite(), nullptr,
																								[=](){
																									t_popup->removeFromParent();
																								});
																}
															});
		t_container->addChild(close_button);
		
		
		content_back->setContentSize(content_back->getContentSize() + CCSizeMake(0, -53));
		content_back->setPositionY(content_back->getPositionY() + 22.5f);
//		case_back->setContentSize(case_back->getContentSize() + CCSizeMake(0, 30));
	}
	
//	title_label->setPosition(ccp(0,case_back->getContentSize().height/2-22));
	ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
	
	t_popup->getDimmedSprite()->setOpacity(0);
	CommonAnimation::openPopup(t_popup, t_container, t_popup->getDimmedSprite(), nullptr, [=]()
							   {
//								   if(Xbutton)
//									{
//										t_popup->setBackKeyFunc([=]()
//																{
//																	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
//																	
//																	if(t_popup->duplicate_tag == -1)
//																	{
//																		CommonAnimation::closePopup(t_popup, t_container,
//																									t_popup->getDimmedSprite(), nullptr,
//																									[=](){
//																										if(close_func && !towButton)
//																											close_func();
//																										
//																										if(close_func && towButton)
//																											close_func2();
//																										
//																										t_popup->removeFromParent();
//																									});
//																	}
//																	else
//																	{
//																		CommonNotiDuplicateManager::sharedInstance()->onTagAction(t_popup->duplicate_tag);
//																		CommonAnimation::closePopup(t_popup, t_container,
//																									t_popup->getDimmedSprite(), nullptr,
//																									[=](){
//																										t_popup->removeFromParent();
//																									});
//																	}
//																});
//										t_popup->setBackKeyEnabled(true);
//										t_popup->setKeypadEnabled(true);
//									}
							   });
	return t_popup;
}

ASPopupView* ASPopupView::getCommonNoti2(int t_touch_priority, string t_title, CCNode* ment_label,
																				function<void()> close_func, CCPoint t_position, bool Xbutton)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	startFormSetter(t_popup);
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
	
	if(t_position.equals(CCPointZero))
	{
		if(mySGD->is_on_maingame)
		{
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		}
		else
		{
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
		}
	}
	else
	{
		t_popup->setDimmedPosition(t_position);
		t_popup->setBasePosition(t_position);
	}
	
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCSprite* case_back = CCSprite::create("popup_small_back.png");
	setFormSetter(case_back);
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	setFormSetter(content_back);
	content_back->setContentSize(CCSizeMake(251, 113));
	content_back->setPosition(ccp(0.0,-12)); 			// dt (0.0,-4.5)
	t_container->addChild(content_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(t_title.c_str(), mySGD->getFont().c_str(), 12);
	setFormSetter(title_label);
//	title_label->setColor(ccc3(255, 170, 20));
	title_label->disableOuterStroke();
	title_label->setPosition(ccp(-85,case_back->getContentSize().height/2.f-35));
	t_container->addChild(title_label);
	
//	setFormSetter(ment_label);
//	CCSize pSize = CCSizeMake(150, 20);
//	
//	if(ment_label->getContentSize().width>pSize.width)pSize.width = ment_label->getContentSize().width;
//	if(ment_label->getContentSize().height>pSize.height)pSize.height = ment_label->getContentSize().height;
	ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
//	ment_label->setPosition(ccp(0,5));
	content_back->addChild(ment_label);
	
//	case_back->setContentSize(CCSizeMake(pSize.width+50, pSize.height + 40+80 - 40));
//	content_back->setContentSize(CCSizeMake(pSize.width+39, pSize.height+76+2.5 - 40));
	
	setFormSetter(case_back);
	setFormSetter(content_back);
	
	if(Xbutton)
	{
		CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
		setFormSetter(close_button);
		close_button->setPosition(ccp(case_back->getContentSize().width/2.f - 25, case_back->getContentSize().height/2.f - 22));
		close_button->setFunction([=](CCObject* sender)
															{
//																t_popup->setBackKeyEnabled(false);
																AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																CommonAnimation::closePopup(t_popup, t_container,
																														t_popup->getDimmedSprite(), nullptr,
																														[=](){
																															if(close_func)
																																close_func();
																															t_popup->removeFromParent();
																														});
																
															});
		t_container->addChild(close_button);
		
	}
	else
	{
		CommonButton* close_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), 12, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), t_popup->getTouchPriority()-5);
		setFormSetter(close_button);
		close_button->setPosition(ccp(0,case_back->getContentSize().height/2.f*-1+45));
		close_button->setFunction([=](CCObject* sender)
															{
																AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																CommonAnimation::closePopup(t_popup, t_container,
																														t_popup->getDimmedSprite(), nullptr,
																														[=](){
																															if(close_func)
																																close_func();
																															t_popup->removeFromParent();
																														});
															});
		t_container->addChild(close_button);
		
		
		content_back->setContentSize(content_back->getContentSize() + CCSizeMake(0, -50));
		content_back->setPositionY(content_back->getPositionY() + 21.5f);
//		case_back->setContentSize(case_back->getContentSize() + CCSizeMake(0, 30));
	}
	
//	title_label->setPosition(ccp(0,case_back->getContentSize().height/2-22));
	ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
	
	t_popup->getDimmedSprite()->setOpacity(0);
	CommonAnimation::openPopup(t_popup, t_container, t_popup->getDimmedSprite(), nullptr, [=]()
							   {
//								   if(Xbutton)
//								   {
//									   t_popup->setBackKeyFunc([=]()
//															   {
//																   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
//																   CommonAnimation::closePopup(t_popup, t_container,
//																							   t_popup->getDimmedSprite(), nullptr,
//																							   [=](){
//																								   if(close_func)
//																									   close_func();
//																								   t_popup->removeFromParent();
//																							   });
//															   });
//									   t_popup->setBackKeyEnabled(true);
//									   t_popup->setKeypadEnabled(true);
//								   }
							   });
	return t_popup;
}

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_title, string t_comment, function<void()> close_func, CCPoint t_position, bool XButton)
{
	KSLabelTTF* ment_label = KSLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 12);
	ment_label->disableOuterStroke();
	return ASPopupView::getCommonNoti(t_touch_priority, t_title,ment_label, close_func,
																		12.f, t_position, XButton);
}

ASPopupView* ASPopupView::getCommonNotiTag(int t_touch_priority, string t_title, string t_comment, function<void()> close_func, int t_tag)
{
	CommonNotiDuplicateManager* t_cndm = CommonNotiDuplicateManager::sharedInstance();
	
	bool is_on_tag = t_cndm->isOnTag(t_tag);
	
	ASPopupView* t_popup = NULL;
	
	if(!is_on_tag)
	{
		t_popup = getCommonNoti(t_touch_priority, t_title, t_comment, close_func);
		t_popup->duplicate_tag = t_tag;
	}
	
	t_cndm->addTagFunction(t_tag, close_func);
	
	return t_popup;
}


ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment, function<void()> close_func,
																				CCPoint t_position, bool Xbutton)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	startFormSetter(t_popup);
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
	
	if(t_position.equals(CCPointZero))
	{
		if(mySGD->is_on_maingame)
		{
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		}
		else
		{
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
		}
	}
	else
	{
		t_popup->setDimmedPosition(t_position);
		t_popup->setBasePosition(t_position);
	}
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCSprite* case_back = CCSprite::create("popup_small_back.png");
	setFormSetter(case_back);
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	setFormSetter(content_back);
	content_back->setContentSize(CCSizeMake(251, 113));
	content_back->setPosition(CCPointZero);
	t_container->addChild(content_back);

	
	KSLabelTTF* ment_label = KSLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 12);
	setFormSetter(ment_label);
	ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
	content_back->addChild(ment_label);
	
	if(Xbutton)
	{
		CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
		setFormSetter(close_button);
		close_button->setPosition(ccp(case_back->getContentSize().width/2.f-22, case_back->getContentSize().height/2.f-25));
		close_button->setFunction([=](CCObject* sender)
															{
//																t_popup->setBackKeyEnabled(false);
																AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																close_func();
																t_popup->removeFromParent();
															});
		t_container->addChild(close_button);
		
//		t_popup->setBackKeyFunc([=]()
//								{
//									AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
//									close_func();
//									t_popup->removeFromParent();
//								});
//		t_popup->setBackKeyEnabled(true);
//		t_popup->setKeypadEnabled(true);
	}
	else
	{
		CommonButton* close_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), 12, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), t_popup->getTouchPriority()-5);
		setFormSetter(close_button);
		close_button->setPosition(ccp(0, -case_back->getContentSize().height/2.f+45));
		close_button->setFunction([=](CCObject* sender)
															{
																AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																close_func();
																t_popup->removeFromParent();
															});
		t_container->addChild(close_button);
		
		content_back->setContentSize(content_back->getContentSize() + CCSizeMake(0, -50));
		content_back->setPosition(content_back->getPosition() + ccp(0,6.5f));
		ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
	}
	
	return t_popup;
}

ASPopupView* ASPopupView::getNotEnoughtGoodsGoShopPopup(int t_touch_priority, GoodsType t_type, function<void()> goshop_func, function<void()> cancel_func)
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
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	t_popup->setBasePosition(ccp(240, 160));
	
	CCSprite* back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	t_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-45, back_case->getContentSize().height-140));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f+16.5f));
	back_case->addChild(back_in);
	
	string title_string;
	if(t_type == kGoodsType_ruby)
		title_string = myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought);
	else if(t_type == kGoodsType_gold)
		title_string = myLoc->getLocalForKey(LK::kMyLocalKey_goldNotEnought);
	else if(t_type == kGoodsType_money)
		title_string = myLoc->getLocalForKey(LK::kMyLocalKey_heartNotEnought);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title_string.c_str(), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	t_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_enoughtGoodsContent), mySGD->getFont().c_str(), 12);
	sub_label->disableOuterStroke();
	sub_label->setAnchorPoint(ccp(0.5f,0.5f));
	sub_label->setPosition(ccp(0,16));
	t_container->addChild(sub_label);
	
	CCSprite* gray = t_popup->getDimmedSprite();
	
	CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!t_popup->is_menu_enable)
									  return;
								  
//								  t_popup->setBackKeyEnabled(false);
								  
								  t_popup->is_menu_enable = false;
									AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
									CommonAnimation::closePopup(t_popup, t_container, gray, [=](){
										
									}, [=](){
									  if(cancel_func != nullptr)
										  cancel_func();
									  t_popup->removeFromParent();
									});
							  });
	t_container->addChild(close_button);
	
	t_popup->button_func_list.clear();
	
	t_popup->button_func_list.push_back([=](){
		if(!t_popup->is_menu_enable)
			return;
		
		t_popup->is_menu_enable = false;
		
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		CommonAnimation::closePopup(t_popup, t_container, gray, [=](){
			
		}, [=](){
			goshop_func(); t_popup->removeFromParent();
		});
		
	});
	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	
	KSLabelTTF* goshop_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_goShopButton), mySGD->getFont().c_str(), 13);
	goshop_label->disableOuterStroke();
	goshop_label->setPosition(ccp(0,0));
	t2_label->addChild(goshop_label);
	
	CCScale9Sprite* goshop_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	CCControlButton* goshop_button = CCControlButton::create(t2_label, goshop_back);
	goshop_button->addTargetWithActionForControlEvents(t_popup, cccontrol_selector(ASPopupView::buttonAction), CCControlEventTouchUpInside);
	goshop_button->setTag(0);
	goshop_button->setPreferredSize(CCSizeMake(150,50));
	goshop_button->setPosition(ccp(0,-42));
	t_container->addChild(goshop_button);
	
	goshop_button->setTouchPriority(t_popup->getTouchPriority()-5);
	
	
	CommonAnimation::openPopup(t_popup, t_container, gray, [=](){
		
	}, [=](){
		t_popup->is_menu_enable = true;
//		t_popup->setBackKeyFunc([=]()
//								{
//									if(!t_popup->is_menu_enable)
//										return;
//									
//									t_popup->is_menu_enable = false;
//									CommonAnimation::closePopup(t_popup, t_container, gray, [=](){
//										
//									}, [=](){
//										if(cancel_func != nullptr)
//											cancel_func();
//										t_popup->removeFromParent();
//									});
//								});
//		t_popup->setBackKeyEnabled(true);
//		t_popup->setKeypadEnabled(true);
	});
	
	
	return t_popup;
}

void ASPopupView::buttonAction(CCObject* sender, CCControlEvent t_event)
{
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	int tag = ((CCNode*)sender)->getTag();
	
	button_func_list[tag]();
}

void ASPopupView::keyBackClicked(void)
{
//	onBackKeyAction();
}