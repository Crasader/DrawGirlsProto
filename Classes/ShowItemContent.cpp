//
//  ShowItemContent.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 9..
//
//

#include "ShowItemContent.h"

#include "MyLocalization.h"
#include "FormSetter.h"
#include "CommonAnimation.h"
#include "TypingBox.h"
#include "DataStorageHub.h"
#include "StoryLayer.h"

ShowItemContent* ShowItemContent::create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
{
	ShowItemContent* t_ctc = new ShowItemContent();
	t_ctc->myInit(t_touch_priority, t_selector, t_item_list);
	t_ctc->autorelease();
	return t_ctc;
}

void ShowItemContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	ing_close_cnt++;
	
	if(ing_close_cnt >= item_list.size())
	{
		startHideAnimation();
	}
	else
	{
		show_content->removeFromParent();
		show_content = CCSprite::create(CCString::createWithFormat("item%d.png", item_list[ing_close_cnt])->getCString());
		show_content->setPosition(ccp(0, 40 - 28));
		addChild(show_content);
		
		item_title->setString(convertToItemCodeToItemName((ITEM_CODE)item_list[ing_close_cnt]).c_str());
		
		item_ment->setString(mySD->getItemScript((ITEM_CODE)item_list[ing_close_cnt]).c_str());
		
		addKindTutorial(this);
	}
}

void ShowItemContent::startShowAnimation()
{
	setScale(0);
	CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowItemContent::endShowAnimation));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	runAction(t_seq);
}

void ShowItemContent::endShowAnimation()
{
	is_menu_enable = true;
}

void ShowItemContent::startHideAnimation()
{
	is_menu_enable = false;
	hide_func([=]()
	{
		this->endHideAnimation();
	});
//	CCScaleTo* t_scale = CCScaleTo::create(0.2f, 0.f);
//	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowItemContent::endHideAnimation));
//	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
//	runAction(t_seq);
}
void ShowItemContent::endHideAnimation()
{
	end_selector(NULL);
}

void ShowItemContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	for(int i=0;i<t_item_list.size();i++)
		item_list.push_back(t_item_list[i]);
	
	
	CCSprite* case_back = CCSprite::create("popup_small_back.png");
	case_back->setStringData("itemPannel");
	case_back->setPosition(ccp(0,0));
	addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	content_back->setContentSize(CCSizeMake(251, 113));
	content_back->setPosition(ccp(case_back->getContentSize().width/2.f,case_back->getContentSize().height/2.f-12));
	case_back->addChild(content_back);
	
	
	ing_close_cnt = 0;
	
	show_content = CCSprite::create(CCString::createWithFormat("item%d.png", item_list[ing_close_cnt])->getCString());
	show_content->setPosition(ccp(0, 40 - 28));
	addChild(show_content);
	
	item_title = KSLabelTTF::create(convertToItemCodeToItemName((ITEM_CODE)item_list[ing_close_cnt]).c_str(), mySGD->getFont().c_str(), 12);
	item_title->disableOuterStroke();
	item_title->setColor(ccc3(255, 170, 20));
	item_title->setPosition(ccp(0,-24));
	addChild(item_title);
	
	item_ment = KSLabelTTF::create(mySD->getItemScript((ITEM_CODE)item_list[ing_close_cnt]).c_str(), mySGD->getFont().c_str(), 12);
	item_ment->disableOuterStroke();
	item_ment->setPosition(ccp(0,-32 - 9));
	addChild(item_ment);
	
//	if(item_ment->getContentSize().width > 200)
//	{
//		case_back->setContentSize(CCSizeMake(item_ment->getContentSize().width+40,case_back->getContentSize().height));
//		content_back->setContentSize(CCSizeMake(item_ment->getContentSize().width+20,content_back->getContentSize().height));
//	}
	
	
	KSLabelTTF* title_img = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_newItemTitle), mySGD->getFont().c_str(), 12);
	title_img->disableOuterStroke();
	title_img->setAnchorPoint(ccp(0.5f, 0.5f));
	title_img->setPosition(ccp(-85,case_back->getContentSize().height/2.f-35));
	addChild(title_img);
	
	KSLabelTTF* bonus_ment_img = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_newItemMent), mySGD->getFont().c_str(), 12);
	bonus_ment_img->disableOuterStroke();
	bonus_ment_img->setColor(ccc3(255,170,20));
	bonus_ment_img->setPosition(ccp(0,-57));
	addChild(bonus_ment_img);
	
	//		CCSprite* t_tab = CCSprite::create("shop_tab.png");
	//		t_tab->setPosition(ccp(30,71));
	//		addChild(t_tab, 2);
	//
	//		CCLabelTTF* new_label = CCLabelTTF::create("new", mySGD->getFont().c_str(), 10);
	//		new_label->setPosition(ccp(t_tab->getContentSize().width/2.f, t_tab->getContentSize().height/2.f+5));
	//		t_tab->addChild(new_label);
	
	
	close_menu = CommonButton::createCloseButton(touch_priority-1);
//	close_menu->setPosition(ccp(0,-85));
	close_menu->setFunction([=](CCObject* sender)
							{
								menuAction(sender);
							});
	addChild(close_menu);
	close_menu->setPosition(ccpFromSize(case_back->getContentSize()) / 2.f - ccp(24, 24));
	
	
	addKindTutorial = [=](CCNode* obj)
	{
		
				if(item_list[ing_close_cnt] == kIC_baseSpeedUp)
				{
					StoryLayer::startStory(nullptr, "item_9");
				}
				else if(item_list[ing_close_cnt] == kIC_doubleItem)
				{
					StoryLayer::startStory(nullptr, "item_6");
				}
				else if(item_list[ing_close_cnt] == kIC_magnet)
				{
					StoryLayer::startStory(nullptr, "item_11");
					//					kt_string1 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial21);
//					kt_string2 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial22);
				}
		
		
//		CCNode* scenario_node = CCNode::create();
//		addChild(scenario_node, 9999);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//		
//		
//		CCNode* t_stencil_node = CCNode::create();
//		
//		
//		CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
//		t_clipping->setAlphaThreshold(0.1f);
//		
//		float change_scale = 1.f;
//		CCPoint change_origin = ccp(0,0);
//		if(screen_scale_x > 1.f)
//		{
//			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
//			change_scale = screen_scale_x;
//		}
//		if(screen_scale_y > 1.f)
//			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
//		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
//		
//		t_clipping->setInverted(true);
//		scenario_node->addChild(t_clipping, 0);
//		
//		
//		CCSprite* asuka = CCSprite::create("kt_cha_asuka_1.png");
//		asuka->setAnchorPoint(ccp(0,0));
//		asuka->setPosition(ccp(-240*screen_scale_x-asuka->getContentSize().width, -160*screen_scale_y));
//		scenario_node->addChild(asuka, 1);
//		
//		CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//		ikaruga->setAnchorPoint(ccp(1,0));
//		ikaruga->setPosition(ccp(240*screen_scale_x+ikaruga->getContentSize().width, -160*screen_scale_y));
//		ikaruga->setVisible(false);
//		scenario_node->addChild(ikaruga, 1);
//		
//		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50)-ccp(240,160), ccp(425,25)-ccp(240,160));
//		typing_box->setHide();
//		scenario_node->addChild(typing_box, 2);
//		
//		TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(255, 60)-ccp(240,160), ccp(425,25)-ccp(240,160));
//		scenario_node->addChild(typing_box2, 2);
//		
//		typing_box2->setTouchOffScrollAndButton();
//		typing_box2->setVisible(false);
//		typing_box2->setTouchSuction(false);
//		
//		typing_box->showAnimation(0.3f);
//		
//		
//		if(item_list[ing_close_cnt] == kIC_baseSpeedUp)
//		{
//			kt_string1 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial17);
//			kt_string2 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial18);
//		}
//		else if(item_list[ing_close_cnt] == kIC_doubleItem)
//		{
//			kt_string1 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial19);
//			kt_string2 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial20);
//		}
//		else if(item_list[ing_close_cnt] == kIC_magnet)
//		{
//			kt_string1 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial21);
//			kt_string2 = myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial22);
//		}
//		
//		function<void()> end_func2 = [=]()
//		{
//			addChild(KSTimer::create(0.1f, [=]()
//									 {
//										 scenario_node->removeFromParent();
//									 }));
//		};
//		
//		function<void()> end_func1 = [=]()
//		{
//			asuka->setVisible(false);
//			ikaruga->setVisible(true);
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240*screen_scale_x+ikaruga->getContentSize().width - ikaruga->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240*screen_scale_x+ikaruga->getContentSize().width - ikaruga->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box2->setVisible(true);
//																	  typing_box2->setTouchSuction(true);
//																	  
//																	  typing_box->setTouchSuction(false);
//																	  
//																	  typing_box2->startTyping(kt_string2.c_str(), end_func2);
//																  }));
//			typing_box->setTouchOffScrollAndButton();
//			typing_box->setVisible(false);
//		};
//		
//		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//															  {
//																  asuka->setPositionX(-240*screen_scale_x-asuka->getContentSize().width + asuka->getContentSize().width*2.f/3.f*t);
//															  }, [=](float t)
//															  {
//																  asuka->setPositionX(-240*screen_scale_x-asuka->getContentSize().width + asuka->getContentSize().width*2.f/3.f*t);
//																  
//																  typing_box->startTyping(kt_string1.c_str(), end_func1);
//															  }));
	};
	
	addKindTutorial(this);
	
//	startShowAnimation();
}

string ShowItemContent::convertToItemCodeToItemName(ITEM_CODE t_code)
{
	string return_value;
	if(t_code == kIC_attack)				return_value = "Attack";
	else if(t_code == kIC_speedUp)			return_value = "SpeedUp";
	else if(t_code == kIC_addTime)			return_value = "AddTime";
	else if(t_code == kIC_fast)				return_value = "Fast";
	else if(t_code == kIC_subOneDie)		return_value = "SubOneDie";
	else if(t_code == kIC_doubleItem)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_doubleItemTitle);
	else if(t_code == kIC_silence)			return_value = "Silence";
	else if(t_code == kIC_longTime)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_longTimeTitle);
	else if(t_code == kIC_baseSpeedUp)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_baseSpeedUpTitle);
	else if(t_code == kIC_magnet)			return_value = myLoc->getLocalForKey(LK::kMyLocalKey_magnetTitle);
	else if(t_code == kIC_itemGacha)		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_itemGachaTitle);
	
	return return_value.c_str();
}

