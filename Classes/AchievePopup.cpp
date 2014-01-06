//
//  AchievePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 4..
//
//

#include "AchievePopup.h"
#include "DataStorageHub.h"
#include "ScrollMenu.h"

enum AchievePopupMenuTag{
	kAchievePopupMenuTag_close = 1,
	kAchievePopupMenuTag_all,
	kAchievePopupMenuTag_success,
	kAchievePopupMenuTag_ing,
	kAchievePopupMenuTag_reward
};

enum AchievePopupZorder{
	kAchievePopupZorder_gray = 1,
	kAchievePopupZorder_back,
	kAchievePopupZorder_table,
	kAchievePopupZorder_menu,
	kAchievePopupZorder_popup
};

void AchievePopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool AchievePopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_menu_enable = false;
	setTouchEnabled(true);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kAchievePopupZorder_gray);
	
	main_case = CCSprite::create("achievement_back.png");
	main_case->setPosition(ccp(240,150-320));
	addChild(main_case, kAchievePopupZorder_back);
	
	CCSprite* n_all = CCSprite::create("achievement_all.png");
	CCSprite* s_all = CCSprite::create("achievement_all.png");
	s_all->setColor(ccGRAY);
	
	CCMenuItem* all_item = CCMenuItemSprite::create(n_all, s_all, this, menu_selector(AchievePopup::menuAction));
	all_item->setTag(kAchievePopupMenuTag_all);
	
	all_menu = CCMenu::createWithItem(all_item);
	all_menu->setPosition(ccp(99,252));
	main_case->addChild(all_menu, kAchievePopupZorder_menu);
	all_menu->setTouchPriority(-190);
	
	CCSprite* n_success = CCSprite::create("achievement_success.png");
	CCSprite* s_success = CCSprite::create("achievement_success.png");
	s_success->setColor(ccGRAY);
	
	CCMenuItem* success_item = CCMenuItemSprite::create(n_success, s_success, this, menu_selector(AchievePopup::menuAction));
	success_item->setTag(kAchievePopupMenuTag_success);
	
	success_menu = CCMenu::createWithItem(success_item);
	success_menu->setPosition(ccp(157,252));
	main_case->addChild(success_menu, kAchievePopupZorder_menu);
	success_menu->setTouchPriority(-190);
	
	CCSprite* n_ing = CCSprite::create("achievement_ing.png");
	CCSprite* s_ing = CCSprite::create("achievement_ing.png");
	s_ing->setColor(ccGRAY);
	
	CCMenuItem* ing_item = CCMenuItemSprite::create(n_ing, s_ing, this, menu_selector(AchievePopup::menuAction));
	ing_item->setTag(kAchievePopupMenuTag_ing);
	
	ing_menu = CCMenu::createWithItem(ing_item);
	ing_menu->setPosition(ccp(214,252));
	main_case->addChild(ing_menu, kAchievePopupZorder_menu);
	ing_menu->setTouchPriority(-190);
	
	CCSprite* n_reward = CCSprite::create("achievement_reward.png");
	CCSprite* s_reward = CCSprite::create("achievement_reward.png");
	s_reward->setColor(ccGRAY);
	
	CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(AchievePopup::menuAction));
	reward_item->setTag(kAchievePopupMenuTag_reward);
	
	reward_menu = CCMenu::createWithItem(reward_item);
	reward_menu->setPosition(ccp(272,252));
	main_case->addChild(reward_menu, kAchievePopupZorder_menu);
	reward_menu->setTouchPriority(-190);
	
	CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(AchievePopup::menuAction));
	close_item->setTag(kAchievePopupMenuTag_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(ccp(325,253));
	main_case->addChild(close_menu, kAchievePopupZorder_menu);
	close_menu->setTouchPriority(-190);
	
	
	achieve_table = NULL;
	recent_code = kAchievePopupListCode_all;
	setAchieveTable();
	
    return true;
}

void AchievePopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,150));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(AchievePopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void AchievePopup::endShowPopup()
{
	is_menu_enable = true;
}

void AchievePopup::hidePopup()
{
	is_menu_enable = false;
	achieve_table->setTouchEnabled(false);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,150-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(AchievePopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void AchievePopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void AchievePopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void AchievePopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	is_menu_enable = false;
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kAchievePopupMenuTag_close)
	{
		hidePopup();
	}
	else if(tag == kAchievePopupMenuTag_all)
	{
		if(recent_code != kAchievePopupListCode_all)
		{
			recent_code = kAchievePopupListCode_all;
			setAchieveTable();
		}
		is_menu_enable = true;
	}
	else if(tag == kAchievePopupMenuTag_success)
	{
		if(recent_code != kAchievePopupListCode_success)
		{
			recent_code = kAchievePopupListCode_success;
			setAchieveTable();
		}
		is_menu_enable = true;
	}
	else if(tag == kAchievePopupMenuTag_ing)
	{
		if(recent_code != kAchievePopupListCode_ing)
		{
			recent_code = kAchievePopupListCode_ing;
			setAchieveTable();
		}
		is_menu_enable = true;
	}
	else if(tag == kAchievePopupMenuTag_reward)
	{
		if(recent_code != kAchievePopupListCode_reward)
		{
			recent_code = kAchievePopupListCode_reward;
			setAchieveTable();
		}
		is_menu_enable = true;
	}
}

void AchievePopup::setAchieveTable()
{
	if(achieve_table)
	{
		achieve_table->removeFromParent();
		achieve_table = NULL;
	}
	
	achieve_list.clear();
	
	if(recent_code == kAchievePopupListCode_all)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
			achieve_list.push_back((AchievementCode)i);
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == -1 ||
			   AchieveConditionReward::sharedInstance()->getRecentValue((AchievementCode)i) >= AchieveConditionReward::sharedInstance()->getCondition((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
	}
	else if(recent_code == kAchievePopupListCode_success)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == -1)
				achieve_list.push_back((AchievementCode)i);
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == -1)
				achieve_list.push_back((AchievementCode)i);
		}
	}
	else if(recent_code == kAchievePopupListCode_ing)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
			   AchieveConditionReward::sharedInstance()->getRecentValue((AchievementCode)i) < AchieveConditionReward::sharedInstance()->getCondition((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
	}
	else if(recent_code == kAchievePopupListCode_reward)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
			   AchieveConditionReward::sharedInstance()->getRecentValue((AchievementCode)i) >= AchieveConditionReward::sharedInstance()->getCondition((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
			   AchieveConditionReward::sharedInstance()->getRecentValue((AchievementCode)i) >= AchieveConditionReward::sharedInstance()->getCondition((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
	}
	
	CCSize table_size = CCSizeMake(320, 208);
	CCPoint table_position = ccp(15, 17);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setOpacity(100);
//	temp_back->setPosition(table_position);
//	main_case->addChild(temp_back, kAchievePopupZorder_table);
	
	achieve_table = CCTableView::create(this, table_size);
	achieve_table->setAnchorPoint(CCPointZero);
	achieve_table->setDirection(kCCScrollViewDirectionVertical);
	achieve_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	achieve_table->setPosition(table_position);
	
	achieve_table->setDelegate(this);
	main_case->addChild(achieve_table, kAchievePopupZorder_table);
	achieve_table->setTouchPriority(-190);
	
	all_menu->setVisible(recent_code != kAchievePopupListCode_all);
	success_menu->setVisible(recent_code != kAchievePopupListCode_success);
	ing_menu->setVisible(recent_code != kAchievePopupListCode_ing);
	reward_menu->setVisible(recent_code != kAchievePopupListCode_reward);
}

void AchievePopup::cellAction( CCObject* sender )
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	loading_layer = LoadingLayer::create();
	addChild(loading_layer, kAchievePopupZorder_popup);
	
	keep_tag = tag;
	AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[keep_tag]);
	int reward_value = AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[keep_tag]);
	
	if(reward_type == kAchieveRewardType_ruby)
		mySGD->setStar(mySGD->getStar() + reward_value);
	else if(reward_type == kAchieveRewardType_gold)
		mySGD->setGold(mySGD->getGold() + reward_value);
	
	keep_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, achieve_list[tag]);
	myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, achieve_list[tag], -1);
	myDSH->saveUserData({kSaveUserData_Key_achieve, kSaveUserData_Key_gold, kSaveUserData_Key_star}, json_selector(this, AchievePopup::resultSaveUserData));
}

void AchievePopup::resultSaveUserData(Json::Value result_data)
{
	CCLog("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLog("reward get success!!");
		
		setAchieveTable();
	}
	else
	{
		CCLog("reward get fail!!");
		
		AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[keep_tag]);
		int reward_value = AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[keep_tag]);
		
		if(reward_type == kAchieveRewardType_ruby)
			mySGD->setStar(mySGD->getStar() - reward_value);
		else if(reward_type == kAchieveRewardType_gold)
			mySGD->setGold(mySGD->getGold() - reward_value);
		
		myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value);
	}
	loading_layer->removeFromParent();
	is_menu_enable = true;
}

CCTableViewCell* AchievePopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSprite* cell_back = CCSprite::create("achievement_cell_back.png");
	cell_back->setAnchorPoint(CCPointZero);
	cell_back->setPosition(ccp(5,0));
	cell->addChild(cell_back);
	
	CCLabelTTF* cell_title = CCLabelTTF::create(AchieveTitleContent::getAchievementScript(achieve_list[idx]).title.c_str(), mySGD->getFont().c_str(), 15);
	cell_title->setAnchorPoint(ccp(0,0.5));
	cell_title->setPosition(ccp(12,35));
	cell_back->addChild(cell_title);
	
	CCLabelTTF* cell_content = CCLabelTTF::create(AchieveTitleContent::getAchievementScript(achieve_list[idx]).content.c_str(), mySGD->getFont().c_str(), 12);
	cell_content->setAnchorPoint(ccp(0,0.5));
	cell_content->setPosition(ccp(12,12));
	cell_back->addChild(cell_content);
	
	if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, achieve_list[idx]) == -1)
	{
		CCSprite* success_img = CCSprite::create("achievement_cell_success.png");
		success_img->setPosition(ccp(270,25));
		cell_back->addChild(success_img);
	}
	else if(AchieveConditionReward::sharedInstance()->getRecentValue(achieve_list[idx]) >= AchieveConditionReward::sharedInstance()->getCondition(achieve_list[idx]))
	{
		CCSprite* n_reward_img = CCSprite::create("achievement_cell_reward_get.png");
		CCSprite* s_reward_img = CCSprite::create("achievement_cell_reward_get.png");
		s_reward_img->setColor(ccGRAY);
		
		CCMenuItem* reward_get_item = CCMenuItemSprite::create(n_reward_img, s_reward_img, this, menu_selector(AchievePopup::cellAction));
		reward_get_item->setTag(idx);
		
		ScrollMenu* reward_get_menu = ScrollMenu::create(reward_get_item, NULL);
		reward_get_menu->setPosition(ccp(270,25));
		cell_back->addChild(reward_get_menu);
		reward_get_menu->setTouchPriority(-191);
	}
	else
	{
		string reward_type_str;
		AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[idx]);
		if(reward_type == kAchieveRewardType_ruby)
			reward_type_str = "price_ruby_img.png";
		else if(reward_type == kAchieveRewardType_gold)
			reward_type_str = "price_gold_img.png";
		
		CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
		ing_back->setPosition(ccp(270,25));
		cell_back->addChild(ing_back);
		CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
		reward_type_img->setPosition(ccp(ing_back->getContentSize().width/2.f-12, ing_back->getContentSize().height/2.f-7));
		ing_back->addChild(reward_type_img);
		CCLabelTTF* reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[idx]))->getCString(),
														mySGD->getFont().c_str(), 10);
		reward_value->setPosition(ccp(ing_back->getContentSize().width/2.f+9, ing_back->getContentSize().height/2.f-8));
		ing_back->addChild(reward_value);
	}
	
	return cell;
}

void AchievePopup::scrollViewDidScroll( CCScrollView* view ){}

void AchievePopup::scrollViewDidZoom( CCScrollView* view ){}

void AchievePopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	//		CCLog("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize AchievePopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(310, 52);
}

unsigned int AchievePopup::numberOfCellsInTableView( CCTableView *table )
{
	return achieve_list.size();
}

bool AchievePopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLog("touch swallow!!!");
	
	return true;
}
void AchievePopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void AchievePopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void AchievePopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void AchievePopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}