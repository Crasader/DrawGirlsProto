//
//  ControlTipContent.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 16..
//
//

#include "ControlTipContent.h"
#include "CCMenuLambda.h"
#include "KSUtil.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonButton.h"

ControlTipContent* ControlTipContent::create(int t_touch_priority, function<void(CCObject*)> t_selector, int t_code)
{
	ControlTipContent* t_ctc = new ControlTipContent();
	t_ctc->myInit(t_touch_priority, t_selector, t_code);
	t_ctc->autorelease();
	return t_ctc;
}

void ControlTipContent::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	
	if(t_name == "Default Timeline")
	{
		if(!close_button->isVisible())
		{
			close_button->setVisible(true);
			is_menu_enable = true;
		}
		
		addChild(KSTimer::create(0.5f, [=]()
						{
							ccb_manager->runAnimationsForSequenceNamed("Default Timeline");
						}));
	}
}

void ControlTipContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
	{
//		CCSprite* t_stencil = (CCSprite*)area_take_sample->getStencil();
//		int before_opacity = t_stencil->getOpacity();
//		KS::setOpacity(this, t);
//		t_stencil->setOpacity(before_opacity);
	}, [=](int t)
	{
//		CCSprite* t_stencil = (CCSprite*)area_take_sample->getStencil();
//		int before_opacity = t_stencil->getOpacity();
//		KS::setOpacity(this, 0);
//		t_stencil->setOpacity(before_opacity);
		end_selector(NULL);
		getParent()->removeFromParent();
	}));
}

void ControlTipContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector, int t_code)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	auto t_ccb = KS::loadCCBI<CCSprite*>(this, getCCBfilenameForTipNumber(t_code).c_str());
	
	area_take_sample = CCClippingNode::create(CCSprite::create("tutorial_ccb_mask.png"));
	area_take_sample->setAlphaThreshold(0.1f);
	t_ccb.first->setPosition(ccp(0,0));
	area_take_sample->addChild(t_ccb.first);
	area_take_sample->setPosition(ccp(0,0));
	addChild(area_take_sample);
	
	t_ccb.second->setDelegate(this);
	ccb_manager = t_ccb.second;
	
	close_button = KS::loadCCBI<CCSprite*>(this, "tutorial_nextbutton.ccbi").first;
	close_button->setPosition(ccp(160, -120));
	close_button->setVisible(false);
	addChild(close_button);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(getTitleLocalKeyForTipNumber(t_code))), mySGD->getFont().c_str(), 15.f); // 17.5f는 너무 큼
	title_label->setColor(ccYELLOW);
	title_label->setPosition(ccp(0, 80));
	addChild(title_label);
	
	KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(getContentLocalKeyForTipNumber(t_code))), mySGD->getFont().c_str(), 12);
	content_label->setPosition(ccp(0, 60));
	addChild(content_label);
	
	
	CCSprite* n_close = CCSprite::create("whitePaper.png");
	n_close->setOpacity(0);
	CCSprite* s_close = CCSprite::create("whitePaper.png");
	s_close->setOpacity(0);
	
	CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ControlTipContent::menuAction));
	
	close_menu = CCMenu::createWithItem(close_item);
	close_menu->setTouchPriority(touch_priority-1);
	close_menu->setPosition(ccp(0,0));
	addChild(close_menu);
	
	setScaleY(0);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){setScaleY(t);}, [=](float t)
			{
				setScaleY(1.f);
				t_ccb.second->runAnimationsForSequenceNamed("Default Timeline");
				
			}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
	{
//		CCSprite* t_stencil = (CCSprite*)area_take_sample->getStencil();
//		int before_opacity = t_stencil->getOpacity();
//		KS::setOpacity(this, t);
//		t_stencil->setOpacity(before_opacity);
	}, [=](int t)
	{
//		CCSprite* t_stencil = (CCSprite*)area_take_sample->getStencil();
//		int before_opacity = t_stencil->getOpacity();
//		KS::setOpacity(this, 255);
//		t_stencil->setOpacity(before_opacity);
	}));
}

string ControlTipContent::getCCBfilenameForTipNumber(int t_code)
{
	string return_value = "";
	if(t_code == kSpecialTutorialCode_lineTangle)
		return_value = "tutorial_tip2.ccbi";
	else if(t_code == kSpecialTutorialCode_slimLine)
		return_value = "tutorial_tip1.ccbi";
	
	return return_value;
}

int ControlTipContent::getTitleLocalKeyForTipNumber(int t_code)
{
	int return_value = 0;
	if(t_code == kSpecialTutorialCode_lineTangle)
		return_value = kMyLocalKey_controlTip_lineTangle_title;
	else if(t_code == kSpecialTutorialCode_slimLine)
		return_value = kMyLocalKey_controlTip_slimLine_content;
	
	return return_value;
}

int ControlTipContent::getContentLocalKeyForTipNumber(int t_code)
{
	int return_value = 0;
	if(t_code == kSpecialTutorialCode_lineTangle)
		return_value = kMyLocalKey_controlTip_lineTangle_content;
	else if(t_code == kSpecialTutorialCode_slimLine)
		return_value = kMyLocalKey_controlTip_slimLine_content;
	
	return return_value;
}