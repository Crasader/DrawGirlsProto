//
//  StoryManager.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 11..
//
//

#include "StoryManager.h"
#include "StarGoldData.h"
#include "utf8.h"

void StoryManager::addMent(bool is_left, string t_name, string t_namefile, string t_ment, function<void(void)> t_end_func)
{
	ment_node->removeAllChildren();
	
	CCScale9Sprite* ment_box = CCScale9Sprite::create("talk_mentbox.png", CCRectMake(0, 0, 35, 35), CCRectMake(12, 12, 23-12, 23-12));
	ment_box->setContentSize(CCSizeMake(350, 100));
	ment_box->setPosition(ccp(0,-110));
	ment_node->addChild(ment_box);
	
	recent_ment = t_ment;
	
	ment_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 12);
	ment_label->setAnchorPoint(ccp(0,1));
	ment_label->setPosition(ccp(12, 100-12));
	ment_label->setHorizontalAlignment(kCCTextAlignmentLeft);
	ment_box->addChild(ment_label);
	
	
	CCLabelTTF* name_label = CCLabelTTF::create(t_name.c_str(), mySGD->getFont().c_str(), 14);
	name_label->setAnchorPoint(ccp(0,1));
	
	CCScale9Sprite* name_tag = CCScale9Sprite::create(t_namefile.c_str(), CCRectMake(0, 0, 35, 35), CCRectMake(12, 12, 23-12, 23-12));
	name_tag->setContentSize(CCSizeMake(name_label->getContentSize().width+24, name_label->getContentSize().height+24));
	if(is_left)
	{
		name_tag->setAnchorPoint(ccp(0,0.5));
		name_tag->setPosition(ccp(-175, -45));
	}
	else
	{
		name_tag->setAnchorPoint(ccp(1,0.5));
		name_tag->setPosition(ccp(175, -45));
	}
	name_label->setPosition(ccp(12,35-8));
	name_tag->addChild(name_label);
	ment_node->addChild(name_tag);
	
	end_func = t_end_func;
	
	startMent();
}

void StoryManager::mentAction()
{
	ing_ment_cnt++;
	
	if(ing_ment_cnt%6 == 0)
	{
		basic_string<wchar_t> result;
		utf8::utf8to16(recent_ment.begin(), recent_ment.end(), back_inserter(result));
		int ment_length = result.length();
		result = result.substr(0, ing_ment_cnt/6);
		string conver;
		utf8::utf16to8(result.begin(), result.end(), back_inserter(conver));
		ment_label->setString(conver.c_str());
		
		if(ing_ment_cnt/6 == ment_length)
		{
			unschedule(schedule_selector(StoryManager::mentAction));
			is_menting = false;
		}
	}
}


bool StoryManager::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!is_menting && !is_delaying)
		end_func();
	
	return true;
}

void StoryManager::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void StoryManager::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void StoryManager::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void StoryManager::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, m_touch_priority, true);
}