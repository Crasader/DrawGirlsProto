//
//  PatternTutorialContent.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 3..
//
//

#include "PatternTutorialContent.h"
#include "CCMenuLambda.h"
#include "KSUtil.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "AudioEngine.h"

PatternTutorialContent* PatternTutorialContent::create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_pattern_list)
{
	PatternTutorialContent* t_ctc = new PatternTutorialContent();
	t_ctc->myInit(t_touch_priority, t_selector, t_pattern_list);
	t_ctc->autorelease();
	return t_ctc;
}

void PatternTutorialContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	ing_close_cnt++;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	if(ing_close_cnt >= pattern_list.size())
	{
		is_menu_enable = false;
		end_selector(NULL);
		
		addChild(KSTimer::create(0.2f, [=](){
			getParent()->removeFromParent();
		}));
		
		CommonAnimation::closePopup(this, show_node, nullptr, [=](){
			
		}, [=](){
//			end_func(); removeFromParent();
		});
//		addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){show_node->setScaleY(t);}, [=](float t){show_node->setScaleY(1.2f);
//			addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){show_node->setScaleY(t);}, [=](float t){show_node->setScaleY(0.f);}));}));
//		
//		addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(show_node, t);}, [=](int t){KS::setOpacity(show_node, 0);}));
	}
	else
	{
		if(show_content)
		{
			CCNode* add_parent = show_content->getParent();
			CCPoint add_position = show_content->getPosition();
			
			show_content->removeFromParent();
			show_content = CCSprite::create(CCString::createWithFormat("pattern%d_tutorial.png", pattern_list[ing_close_cnt])->getCString());
			show_content->setPosition(add_position);
			add_parent->addChild(show_content);
			
			pattern_title->setString(myLoc->getLocalForKey(LK(getTitleLocalKeyForPatternNumber(pattern_list[ing_close_cnt]))));
			pattern_content->setString(myLoc->getLocalForKey(LK(getContentLocalKeyForPatternNumber(pattern_list[ing_close_cnt]))));
			
		}
		addChild(KSTimer::create(0.5f, [=](){is_menu_enable = true;}));
	}
}

void PatternTutorialContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_pattern_list)
{
	is_menu_enable = false;
	
	show_node = CCNode::create();
	show_node->setPosition(CCPointZero);
	addChild(show_node);
	
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	for(int i=0;i<t_pattern_list.size();i++)
	{
		if(t_pattern_list[i] == 0)
			continue;
		pattern_list.push_back(t_pattern_list[i]);
	}
	CCSprite* case_back = CCSprite::create("ingame_back.png");
	case_back->setPosition(CCPointZero);
	show_node->addChild(case_back);
	
	CCSprite* title_back = CCSprite::create("title_tag.png");
	title_back->setPosition(ccp(65,case_back->getContentSize().height-30));
	case_back->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_newBossPattern), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(-3,0));
	title_back->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(0);
	close_button->setPosition(ccp(case_back->getContentSize().width/2.f-22, case_back->getContentSize().height/2.f-22));
	close_button->setEnabled(false);
	show_node->addChild(close_button);
	
	CCScale9Sprite* case_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	case_in->setContentSize(CCSizeMake(251, 97));
	case_in->setPosition(ccp(case_back->getContentSize().width/2.f,case_back->getContentSize().height/2.f+10));
	case_back->addChild(case_in);
	
	ing_close_cnt = 0;
	
	pattern_title = KSLabelTTF::create(myLoc->getLocalForKey(LK(getTitleLocalKeyForPatternNumber(pattern_list[ing_close_cnt]))), mySGD->getFont().c_str(), 14.f);
	pattern_title->setColor(ccc3(255, 170, 20));
	pattern_title->enableOuterStroke(ccBLACK, 1, 190, true);
	pattern_title->setAnchorPoint(ccp(0.5f,0.5f));
	pattern_title->setPosition(ccp(case_back->getContentSize().width/2.f, 50));
	case_back->addChild(pattern_title);
	
	pattern_content = KSLabelTTF::create(myLoc->getLocalForKey(LK(getContentLocalKeyForPatternNumber(pattern_list[ing_close_cnt]))), mySGD->getFont().c_str(), 12);
	pattern_content->setColor(ccc3(0, 45, 100));
	pattern_content->disableOuterStroke();
	pattern_content->setAnchorPoint(ccp(0.5f,0.5f));
	pattern_content->setPosition(ccp(case_back->getContentSize().width/2.f, 28));
	case_back->addChild(pattern_content);
	
	
	show_content = CCSprite::create(CCString::createWithFormat("pattern%d_tutorial.png", pattern_list[ing_close_cnt])->getCString());
	if(show_content)
	{
		show_content->setPosition(ccp(case_in->getContentSize().width/2.f, case_in->getContentSize().height/2.f));
		case_in->addChild(show_content);
	}
	
	CCSprite* n_close = CCSprite::create("whitePaper.png");
	n_close->setOpacity(0);
	CCSprite* s_close = CCSprite::create("whitePaper.png");
	s_close->setOpacity(0);
	
	CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PatternTutorialContent::menuAction));
	
	close_menu = CCMenu::createWithItem(close_item);
	close_menu->setTouchPriority(touch_priority-1);
	close_menu->setPosition(ccp(0,0));
	addChild(close_menu);
	
	show_node->setScaleY(0.f);
	
	CommonAnimation::openPopup(this, show_node, nullptr, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

int PatternTutorialContent::getTitleLocalKeyForPatternNumber(int t_pattern)
{
	int return_value = 0;
	if(t_pattern == 9)
		return_value = int(LK::kMyLocalKey_warning9);
	else if(t_pattern == 105)
		return_value = int(LK::kMyLocalKey_warning105);
	else if(t_pattern == 106)
		return_value = int(LK::kMyLocalKey_warning106);
	else if(t_pattern == 107)
		return_value = int(LK::kMyLocalKey_warning107);
	else if(t_pattern == 108)
		return_value = int(LK::kMyLocalKey_warning108);
	else if(t_pattern == 109)
		return_value = int(LK::kMyLocalKey_warning109);
	else if(t_pattern == 110)
		return_value = int(LK::kMyLocalKey_warning110);
	else if(t_pattern == 111)
		return_value = int(LK::kMyLocalKey_warning111);
	else if(t_pattern == 112)
		return_value = int(LK::kMyLocalKey_warning112);
	else if(t_pattern == 113)
		return_value = int(LK::kMyLocalKey_warning113);
	else if(t_pattern == 1001)
		return_value = int(LK::kMyLocalKey_warning1001);
	else if(t_pattern == 1002)
		return_value = int(LK::kMyLocalKey_warning1002);
	else if(t_pattern == 1003)
		return_value = int(LK::kMyLocalKey_warning1003);
	else if(t_pattern == 1004)
		return_value = int(LK::kMyLocalKey_warning1004);
	else if(t_pattern == 1005)
		return_value = int(LK::kMyLocalKey_warning1005);
	else if(t_pattern == 1006)
		return_value = int(LK::kMyLocalKey_warning1006);
	else if(t_pattern == 1007)
		return_value = int(LK::kMyLocalKey_warning1007);
	else if(t_pattern == 1008)
		return_value = int(LK::kMyLocalKey_warning1008);
	else if(t_pattern == 1009)
		return_value = int(LK::kMyLocalKey_warning1009);
	else if(t_pattern == 1010)
		return_value = int(LK::kMyLocalKey_warning1010);
	else if(t_pattern == 1011)
		return_value = int(LK::kMyLocalKey_warning1011);
	else if(t_pattern == 1012)
		return_value = int(LK::kMyLocalKey_warning1012);
	else if(t_pattern == 1013)
		return_value = int(LK::kMyLocalKey_warning1013);
	else if(t_pattern == 1014)
		return_value = int(LK::kMyLocalKey_warning1014);
	else if(t_pattern == 1015)
		return_value = int(LK::kMyLocalKey_warning1015);
	else if(t_pattern == 1016)
		return_value = int(LK::kMyLocalKey_warning1016);
	else if(t_pattern == 1017)
		return_value = int(LK::kMyLocalKey_warning1017);
	else if(t_pattern == 1018)
		return_value = int(LK::kMyLocalKey_warning1018);
	else if(t_pattern == 1019)
		return_value = int(LK::kMyLocalKey_warning1019);
	else if(t_pattern == 1020)
		return_value = int(LK::kMyLocalKey_warning1020);
	
	return return_value;
}

int PatternTutorialContent::getContentLocalKeyForPatternNumber(int t_pattern)
{
	int return_value = 0;
	if(t_pattern == 9)
		return_value = int(LK::kMyLocalKey_patternContent9);
	else if(t_pattern == 105)
		return_value = int(LK::kMyLocalKey_patternContent105);
	else if(t_pattern == 106)
		return_value = int(LK::kMyLocalKey_patternContent106);
	else if(t_pattern == 107)
		return_value = int(LK::kMyLocalKey_patternContent107);
	else if(t_pattern == 108)
		return_value = int(LK::kMyLocalKey_patternContent108);
	else if(t_pattern == 109)
		return_value = int(LK::kMyLocalKey_patternContent109);
	else if(t_pattern == 110)
		return_value = int(LK::kMyLocalKey_patternContent110);
	else if(t_pattern == 111)
		return_value = int(LK::kMyLocalKey_patternContent111);
	else if(t_pattern == 112)
		return_value = int(LK::kMyLocalKey_patternContent112);
	else if(t_pattern == 113)
		return_value = int(LK::kMyLocalKey_patternContent113);
	else if(t_pattern == 1001)
		return_value = int(LK::kMyLocalKey_patternContent1001);
	else if(t_pattern == 1002)
		return_value = int(LK::kMyLocalKey_patternContent1002);
	else if(t_pattern == 1003)
		return_value = int(LK::kMyLocalKey_patternContent1003);
	else if(t_pattern == 1004)
		return_value = int(LK::kMyLocalKey_patternContent1004);
	else if(t_pattern == 1005)
		return_value = int(LK::kMyLocalKey_patternContent1005);
	else if(t_pattern == 1006)
		return_value = int(LK::kMyLocalKey_patternContent1006);
	else if(t_pattern == 1007)
		return_value = int(LK::kMyLocalKey_patternContent1007);
	else if(t_pattern == 1008)
		return_value = int(LK::kMyLocalKey_patternContent1008);
	else if(t_pattern == 1009)
		return_value = int(LK::kMyLocalKey_patternContent1009);
	else if(t_pattern == 1010)
		return_value = int(LK::kMyLocalKey_patternContent1010);
	else if(t_pattern == 1011)
		return_value = int(LK::kMyLocalKey_patternContent1011);
	else if(t_pattern == 1012)
		return_value = int(LK::kMyLocalKey_patternContent1012);
	else if(t_pattern == 1013)
		return_value = int(LK::kMyLocalKey_patternContent1013);
	else if(t_pattern == 1014)
		return_value = int(LK::kMyLocalKey_patternContent1014);
	else if(t_pattern == 1015)
		return_value = int(LK::kMyLocalKey_patternContent1015);
	else if(t_pattern == 1016)
		return_value = int(LK::kMyLocalKey_patternContent1016);
	else if(t_pattern == 1017)
		return_value = int(LK::kMyLocalKey_patternContent1017);
	else if(t_pattern == 1018)
		return_value = int(LK::kMyLocalKey_patternContent1018);
	else if(t_pattern == 1019)
		return_value = int(LK::kMyLocalKey_patternContent1019);
	else if(t_pattern == 1020)
		return_value = int(LK::kMyLocalKey_patternContent1020);
	else if(t_pattern == 1022)
		return_value = int(LK::kMyLocalKey_patternContent1022);
	else if(t_pattern == 1023)
		return_value = int(LK::kMyLocalKey_patternContent1023);
	else if(t_pattern == 1024)
		return_value = int(LK::kMyLocalKey_patternContent1024);
	
	return return_value;
}
