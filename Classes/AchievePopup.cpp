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
	kAchievePopupMenuTag_reward,
	kAchievePopupMenuTag_allReward
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
	main_case->setAnchorPoint(CCPointZero);
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kAchievePopupZorder_back);
	
	recent_code = kAchievePopupListCode_all;
	
	CCSprite* n_all_reward = CCSprite::create("achievement_allreward.png");
	CCSprite* s_all_reward = CCSprite::create("achievement_allreward.png");
	s_all_reward->setColor(ccGRAY);
	
	CCMenuItem* all_reward_item = CCMenuItemSprite::create(n_all_reward, s_all_reward, this, menu_selector(AchievePopup::takeAllReward));
	all_reward_item->setTag(kAchievePopupMenuTag_allReward);
	
	all_reward_menu = CCMenu::createWithItem(all_reward_item);
	all_reward_menu->setVisible(recent_code == kAchievePopupListCode_all || recent_code == kAchievePopupListCode_reward);
	all_reward_menu->setPosition(ccp(390,33));
	main_case->addChild(all_reward_menu, kAchievePopupZorder_menu);
	all_reward_menu->setTouchPriority(-190);
	
	all_menu = NULL;
	setAllMenu();
	
	success_menu = NULL;
	setSuccessMenu();
	
	ing_menu = NULL;
	setIngMenu();
	
	reward_menu = NULL;
	setRewardMenu();
	
	CCSprite* n_close = CCSprite::create("cardchange_cancel.png");
	CCSprite* s_close = CCSprite::create("cardchange_cancel.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(AchievePopup::menuAction));
	close_item->setTag(kAchievePopupMenuTag_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(ccp(452,259));
	main_case->addChild(close_menu, kAchievePopupZorder_menu);
	close_menu->setTouchPriority(-190);
	
	achieve_table = NULL;
	setAchieveTable();
	
    return true;
}

void AchievePopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
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
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
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
			AchievePopupListCode keep_code = recent_code;
			recent_code = kAchievePopupListCode_all;
			
			if(keep_code == kAchievePopupListCode_success)
				setSuccessMenu();
			else if(keep_code == kAchievePopupListCode_ing)
				setIngMenu();
			else if(keep_code == kAchievePopupListCode_reward)
				setRewardMenu();
			
			setAllMenu();
			setAchieveTable();
			all_reward_menu->setVisible(true);
		}
		is_menu_enable = true;
	}
	else if(tag == kAchievePopupMenuTag_success)
	{
		if(recent_code != kAchievePopupListCode_success)
		{
			AchievePopupListCode keep_code = recent_code;
			recent_code = kAchievePopupListCode_success;
			
			if(keep_code == kAchievePopupListCode_all)
				setAllMenu();
			else if(keep_code == kAchievePopupListCode_ing)
				setIngMenu();
			else if(keep_code == kAchievePopupListCode_reward)
				setRewardMenu();
			
			setSuccessMenu();
			setAchieveTable();
			all_reward_menu->setVisible(false);
		}
		is_menu_enable = true;
	}
	else if(tag == kAchievePopupMenuTag_ing)
	{
		if(recent_code != kAchievePopupListCode_ing)
		{
			AchievePopupListCode keep_code = recent_code;
			recent_code = kAchievePopupListCode_ing;
			
			if(keep_code == kAchievePopupListCode_all)
				setAllMenu();
			else if(keep_code == kAchievePopupListCode_success)
				setSuccessMenu();
			else if(keep_code == kAchievePopupListCode_reward)
				setRewardMenu();
			
			setIngMenu();
			setAchieveTable();
			all_reward_menu->setVisible(false);
		}
		is_menu_enable = true;
	}
	else if(tag == kAchievePopupMenuTag_reward)
	{
		if(recent_code != kAchievePopupListCode_reward)
		{
			AchievePopupListCode keep_code = recent_code;
			recent_code = kAchievePopupListCode_reward;
			
			if(keep_code == kAchievePopupListCode_all)
				setAllMenu();
			else if(keep_code == kAchievePopupListCode_success)
				setSuccessMenu();
			else if(keep_code == kAchievePopupListCode_ing)
				setIngMenu();
			
			setRewardMenu();
			setAchieveTable();
			all_reward_menu->setVisible(true);
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
			   AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
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
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
			   !AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
	}
	else if(recent_code == kAchievePopupListCode_reward)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
			   AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
			   AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
	}
	
	CCSize table_size = CCSizeMake(385, 183);
	CCPoint table_position = ccp(39, 52);
	
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
	
	CCSprite* cell_back = CCSprite::create("achievement_cell.png");
	cell_back->setAnchorPoint(CCPointZero);
	cell_back->setPosition(ccp(0,0));
	cell->addChild(cell_back);
	
	CCLabelTTF* cell_title = CCLabelTTF::create(CCString::createWithFormat("%s(%d/%d)",
																		   AchieveTitleContent::getAchievementScript(achieve_list[idx*2]).title.c_str(),
																		   AchieveConditionReward::sharedInstance()->getRecentValue(achieve_list[idx*2]),
																		   AchieveConditionReward::sharedInstance()->getCondition(achieve_list[idx*2]))->getCString(),
																		   mySGD->getFont().c_str(), 10);
	cell_title->setColor(ccBLACK);
	cell_title->setAnchorPoint(ccp(0,0.5));
	cell_title->setPosition(ccp(6,26));
	cell_back->addChild(cell_title);
	
	CCLabelTTF* cell_content = CCLabelTTF::create(AchieveTitleContent::getAchievementScript(achieve_list[idx*2]).content.c_str(), mySGD->getFont().c_str(), 8);
	cell_content->setColor(ccBLACK);
	cell_content->setAnchorPoint(ccp(0,0.5));
	cell_content->setPosition(ccp(6,13));
	cell_back->addChild(cell_content);
	
	if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, achieve_list[idx*2]) == -1)
	{
		CCSprite* success_img = CCSprite::create("achievement_cell_success.png");
		success_img->setPosition(ccp(157,21));
		cell_back->addChild(success_img);
	}
	else if(AchieveConditionReward::sharedInstance()->isAchieve(achieve_list[idx*2]))
	{
		CCSprite* n_reward_img = CCSprite::create("achievement_cell_reward_get.png");
		CCSprite* s_reward_img = CCSprite::create("achievement_cell_reward_get.png");
		s_reward_img->setColor(ccGRAY);
		
		CCMenuItem* reward_get_item = CCMenuItemSprite::create(n_reward_img, s_reward_img, this, menu_selector(AchievePopup::cellAction));
		reward_get_item->setTag(idx*2);
		
		ScrollMenu* reward_get_menu = ScrollMenu::create(reward_get_item, NULL);
		reward_get_menu->setPosition(ccp(157,21));
		cell_back->addChild(reward_get_menu);
		reward_get_menu->setTouchPriority(-191);
	}
	else
	{
		string reward_type_str;
		AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[idx*2]);
		if(reward_type == kAchieveRewardType_ruby)
			reward_type_str = "price_ruby_img.png";
		else if(reward_type == kAchieveRewardType_gold)
			reward_type_str = "price_gold_img.png";
		
		CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
		ing_back->setPosition(ccp(157,21));
		cell_back->addChild(ing_back);
		CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
		reward_type_img->setPosition(ccp(ing_back->getContentSize().width/2.f-12, ing_back->getContentSize().height/2.f-7));
		ing_back->addChild(reward_type_img);
		CCLabelTTF* reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[idx*2]))->getCString(),
														mySGD->getFont().c_str(), 10);
		reward_value->setPosition(ccp(ing_back->getContentSize().width/2.f+9, ing_back->getContentSize().height/2.f-8));
		ing_back->addChild(reward_value);
	}
	
	if(idx*2+1 < achieve_list.size())
	{
		CCSprite* cell_back = CCSprite::create("achievement_cell.png");
		cell_back->setAnchorPoint(CCPointZero);
		cell_back->setPosition(ccp(385-188,0));
		cell->addChild(cell_back);
		
		CCLabelTTF* cell_title = CCLabelTTF::create(CCString::createWithFormat("%s(%d/%d)",
																			   AchieveTitleContent::getAchievementScript(achieve_list[idx*2+1]).title.c_str(),
																			   AchieveConditionReward::sharedInstance()->getRecentValue(achieve_list[idx*2+1]),
																			   AchieveConditionReward::sharedInstance()->getCondition(achieve_list[idx*2+1]))->getCString(),
													mySGD->getFont().c_str(), 10);
		cell_title->setColor(ccBLACK);
		cell_title->setAnchorPoint(ccp(0,0.5));
		cell_title->setPosition(ccp(6,26));
		cell_back->addChild(cell_title);
		
		CCLabelTTF* cell_content = CCLabelTTF::create(AchieveTitleContent::getAchievementScript(achieve_list[idx*2+1]).content.c_str(), mySGD->getFont().c_str(), 8);
		cell_content->setColor(ccBLACK);
		cell_content->setAnchorPoint(ccp(0,0.5));
		cell_content->setPosition(ccp(6,13));
		cell_back->addChild(cell_content);
		
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, achieve_list[idx*2+1]) == -1)
		{
			CCSprite* success_img = CCSprite::create("achievement_cell_success.png");
			success_img->setPosition(ccp(157,21));
			cell_back->addChild(success_img);
		}
		else if(AchieveConditionReward::sharedInstance()->isAchieve(achieve_list[idx*2+1]))
		{
			CCSprite* n_reward_img = CCSprite::create("achievement_cell_reward_get.png");
			CCSprite* s_reward_img = CCSprite::create("achievement_cell_reward_get.png");
			s_reward_img->setColor(ccGRAY);
			
			CCMenuItem* reward_get_item = CCMenuItemSprite::create(n_reward_img, s_reward_img, this, menu_selector(AchievePopup::cellAction));
			reward_get_item->setTag(idx*2+1);
			
			ScrollMenu* reward_get_menu = ScrollMenu::create(reward_get_item, NULL);
			reward_get_menu->setPosition(ccp(157,21));
			cell_back->addChild(reward_get_menu);
			reward_get_menu->setTouchPriority(-191);
		}
		else
		{
			string reward_type_str;
			AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[idx*2+1]);
			if(reward_type == kAchieveRewardType_ruby)
				reward_type_str = "price_ruby_img.png";
			else if(reward_type == kAchieveRewardType_gold)
				reward_type_str = "price_gold_img.png";
			
			CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
			ing_back->setPosition(ccp(157,21));
			cell_back->addChild(ing_back);
			CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
			reward_type_img->setPosition(ccp(ing_back->getContentSize().width/2.f-12, ing_back->getContentSize().height/2.f-7));
			ing_back->addChild(reward_type_img);
			CCLabelTTF* reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[idx*2+1]))->getCString(),
														  mySGD->getFont().c_str(), 10);
			reward_value->setPosition(ccp(ing_back->getContentSize().width/2.f+9, ing_back->getContentSize().height/2.f-8));
			ing_back->addChild(reward_value);
		}
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
	return CCSizeMake(385, 42);
}

unsigned int AchievePopup::numberOfCellsInTableView( CCTableView *table )
{
	if(achieve_list.size() <= 0)
		return 0;
	else
		return (achieve_list.size()-1)/2 + 1;
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

void AchievePopup::setAllMenu()
{
	if(all_menu)
	{
		all_menu->removeFromParent();
		all_menu = NULL;
	}
	
	if(recent_code == kAchievePopupListCode_all)
	{
		all_menu = CCSprite::create("achievement_all_on.png");
		all_menu->setPosition(ccp(70,259));
		main_case->addChild(all_menu, kAchievePopupZorder_menu);
	}
	else
	{
		CCSprite* n_all = CCSprite::create("achievement_all_off.png");
		CCSprite* s_all = CCSprite::create("achievement_all_off.png");
		s_all->setColor(ccGRAY);
		
		CCMenuItem* all_item = CCMenuItemSprite::create(n_all, s_all, this, menu_selector(AchievePopup::menuAction));
		all_item->setTag(kAchievePopupMenuTag_all);
		
		all_menu = CCMenu::createWithItem(all_item);
		all_menu->setPosition(ccp(70,259));
		main_case->addChild(all_menu, kAchievePopupZorder_menu);
		((CCMenu*)all_menu)->setTouchPriority(-190);
	}
}
void AchievePopup::setSuccessMenu()
{
	if(success_menu)
	{
		success_menu->removeFromParent();
		success_menu = NULL;
	}
	
	if(recent_code == kAchievePopupListCode_success)
	{
		success_menu = CCSprite::create("achievement_success_on.png");
		success_menu->setPosition(ccp(172,259));
		main_case->addChild(success_menu, kAchievePopupZorder_menu);
	}
	else
	{
		CCSprite* n_success = CCSprite::create("achievement_success_off.png");
		CCSprite* s_success = CCSprite::create("achievement_success_off.png");
		s_success->setColor(ccGRAY);
		
		CCMenuItem* success_item = CCMenuItemSprite::create(n_success, s_success, this, menu_selector(AchievePopup::menuAction));
		success_item->setTag(kAchievePopupMenuTag_success);
		
		success_menu = CCMenu::createWithItem(success_item);
		success_menu->setPosition(ccp(172,259));
		main_case->addChild(success_menu, kAchievePopupZorder_menu);
		((CCMenu*)success_menu)->setTouchPriority(-190);
	}
}
void AchievePopup::setIngMenu()
{
	if(ing_menu)
	{
		ing_menu->removeFromParent();
		ing_menu = NULL;
	}
	
	if(recent_code == kAchievePopupListCode_ing)
	{
		ing_menu = CCSprite::create("achievement_ing_on.png");
		ing_menu->setPosition(ccp(274,259));
		main_case->addChild(ing_menu, kAchievePopupZorder_menu);
	}
	else
	{
		CCSprite* n_ing = CCSprite::create("achievement_ing_off.png");
		CCSprite* s_ing = CCSprite::create("achievement_ing_off.png");
		s_ing->setColor(ccGRAY);
		
		CCMenuItem* ing_item = CCMenuItemSprite::create(n_ing, s_ing, this, menu_selector(AchievePopup::menuAction));
		ing_item->setTag(kAchievePopupMenuTag_ing);
		
		ing_menu = CCMenu::createWithItem(ing_item);
		ing_menu->setPosition(ccp(274,259));
		main_case->addChild(ing_menu, kAchievePopupZorder_menu);
		((CCMenu*)ing_menu)->setTouchPriority(-190);
	}
}
void AchievePopup::setRewardMenu()
{
	if(reward_menu)
	{
		reward_menu->removeFromParent();
		reward_menu = NULL;
	}
	
	if(recent_code == kAchievePopupListCode_reward)
	{
		reward_menu = CCSprite::create("achievement_reward_on.png");
		reward_menu->setPosition(ccp(376,259));
		main_case->addChild(reward_menu, kAchievePopupZorder_menu);
	}
	else
	{
		CCSprite* n_reward = CCSprite::create("achievement_reward_off.png");
		CCSprite* s_reward = CCSprite::create("achievement_reward_off.png");
		s_reward->setColor(ccGRAY);
		
		CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(AchievePopup::menuAction));
		reward_item->setTag(kAchievePopupMenuTag_reward);
		
		reward_menu = CCMenu::createWithItem(reward_item);
		reward_menu->setPosition(ccp(376,259));
		main_case->addChild(reward_menu, kAchievePopupZorder_menu);
		((CCMenu*)reward_menu)->setTouchPriority(-190);
	}
}

void AchievePopup::takeAllReward(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	loading_layer = LoadingLayer::create();
	addChild(loading_layer, kAchievePopupZorder_popup);
	
	keep_value_list.clear();
	
	keep_take_gold = 0;
	keep_take_ruby = 0;
	
	for(int i=0;i<achieve_list.size();i++)
	{
		if(AchieveConditionReward::sharedInstance()->isAchieve(achieve_list[i]))
		{
			AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[i]);
			int reward_value = AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[i]);
			
			if(reward_type == kAchieveRewardType_ruby)
				keep_take_ruby += reward_value;
			else if(reward_type == kAchieveRewardType_gold)
				keep_take_gold += reward_value;
			
			keep_value_list.push_back(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, achieve_list[i]));
			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, achieve_list[i], -1, false);
		}
	}
	myDSH->fFlush();
	
	mySGD->setStar(mySGD->getStar() + keep_take_ruby);
	mySGD->setGold(mySGD->getGold() + keep_take_gold);
	
	myDSH->saveUserData({kSaveUserData_Key_achieve, kSaveUserData_Key_gold, kSaveUserData_Key_star}, json_selector(this, AchievePopup::resultSaveUserData));
}

void AchievePopup::resultAllTakeSaveUserData(Json::Value result_data)
{
	CCLog("resultAllTakeSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLog("reward get success!!");
		
		setAchieveTable();
	}
	else
	{
		CCLog("reward get fail!!");
		
		for(int i=0;i<keep_value_list.size();i++)
			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value_list[i], false);
		myDSH->fFlush();
		
		mySGD->setStar(mySGD->getStar() - keep_take_ruby);
		mySGD->setGold(mySGD->getGold() - keep_take_gold);
	}
	loading_layer->removeFromParent();
	is_menu_enable = true;
}