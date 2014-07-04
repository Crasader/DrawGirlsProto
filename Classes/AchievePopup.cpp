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
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "LoadingLayer.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "ScrollBar.h"
#include "CommonAnimation.h"
#include "ASPopupView.h"

enum AchievePopupMenuTag{
	kAchievePopupMenuTag_close = 1,
	kAchievePopupMenuTag_all,
	kAchievePopupMenuTag_success,
	kAchievePopupMenuTag_ing,
	kAchievePopupMenuTag_reward,
	kAchievePopupMenuTag_allReward,
	kAchievePopupMenuTag_tip
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
	
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kAchievePopupZorder_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_achievement), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(40,256));
	main_case->addChild(title_label);
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner->setContentSize(CCSizeMake(460, 232));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.44f);
	main_case->addChild(main_inner);
	
	
	recent_code = kAchievePopupListCode_all;
	
	all_reward_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_allRewardGet), 13, CCSizeMake(100,35), CommonButtonLightPupple, -190);
	all_reward_menu->setPosition(ccp(395,32));
	all_reward_menu->setBackgroundTypeForDisabled(CommonButtonGray);
	all_reward_menu->setTitleColorForDisable(ccc3(90, 60, 30));
	main_case->addChild(all_reward_menu, kAchievePopupZorder_menu);
	all_reward_menu->setFunction([=](CCObject* sender)
						  {
							  CCNode* t_node = CCNode::create();
							  t_node->setTag(kAchievePopupMenuTag_allReward);
							  takeAllReward(t_node);
						  });
	
	all_reward_menu->setEnabled(recent_code == kAchievePopupListCode_all || recent_code == kAchievePopupListCode_reward);
	
	int is_found = myAchieve->isHaveRewardCount();
	
	if(is_found > 0)
	{
		all_reward_menu->setEnabled(true);
	}
	else
	{
		all_reward_menu->setEnabled(false);
	}
	
		
	all_menu = NULL;
	setAllMenu();
	
	success_menu = NULL;
	setSuccessMenu();
	
	ing_menu = NULL;
	setIngMenu();
	
	reward_menu = NULL;
	setRewardMenu();
	
	CommonButton* close_menu = CommonButton::createCloseButton(-200);
	close_menu->setPosition(ccp(452,257));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kAchievePopupMenuTag_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kAchievePopupZorder_menu);
	
	achieve_table = NULL;
	m_scrollBar = NULL;
	setAchieveTable();
	
	empty_ment = CCLabelTTF::create("", mySGD->getFont().c_str(), 15);
	empty_ment->setPosition(ccp(25+210,52+90));
	main_case->addChild(empty_ment, kAchievePopupZorder_menu);
	
//	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
//	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
//	s_tip->setColor(ccGRAY);
//	
//	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(AchievePopup::menuAction));
//	tip_item->setTag(kAchievePopupMenuTag_tip);
//	
//	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
//	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
//	addChild(tip_menu, kAchievePopupZorder_menu);
//	
//	tip_menu->setTouchPriority(-190);
	
    return true;
}

void AchievePopup::showPopup()
{
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&AchievePopup::endShowPopup, this));
}

void AchievePopup::endShowPopup()
{
//	if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_achievement))
//	{
//		myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_achievement, true);
//		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//		t_suction->target_touch_began = t_suction;
//		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//		t_suction->setTouchEnabled(true);
//		
//		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_achievement.png");
//		dimed_tip->setPosition(ccp(240,160));
//		t_suction->addChild(dimed_tip);
//		
//		addChild(t_suction, kAchievePopupZorder_popup);
//	}
	
	is_menu_enable = true;
}

void AchievePopup::hidePopup()
{
	is_menu_enable = false;
	achieve_table->setTouchEnabled(false);
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&AchievePopup::endHidePopup, this));
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
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	
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
			
			empty_ment->setString("");
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
			if(achieve_list.size() <= 0)
				empty_ment->setString(myLoc->getLocalForKey(kMyLocalKey_nothingSuccessAchieve));
			else
				empty_ment->setString("");
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
			empty_ment->setString("");
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
			if(achieve_list.size() <= 0)
			{
				empty_ment->setString(myLoc->getLocalForKey(kMyLocalKey_nothingRewardAchieve));
			}
			else
			{
				empty_ment->setString("");
			}
		}
		is_menu_enable = true;
	}
	else if(tag == kAchievePopupMenuTag_tip)
	{
		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
		t_suction->target_touch_began = t_suction;
		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
		t_suction->setTouchEnabled(true);
		
		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_achievement.png");
		dimed_tip->setPosition(ccp(240,160));
		t_suction->addChild(dimed_tip);
		
		addChild(t_suction, kAchievePopupZorder_popup);
		
		is_menu_enable = true;
	}
}

bool AchievePopup::isHaveAchieveGroup(vector<AchieveGroup>& t_vector, AchieveGroup t_group)
{
	for(int i=0;i<t_vector.size();i++)
	{
		bool is_false = true;
		if(t_vector[i].achieve_list.size() != t_group.achieve_list.size())
			is_false = false;
		
		if(is_false)
		{
			for(int j=0;is_false && j<t_vector[i].achieve_list.size();j++)
			{
				if(t_group.achieve_list[j] != t_vector[i].achieve_list[j])
					is_false = false;
			}
		}
		
		if(is_false)
			return true;
	}
	return false;
}

void AchievePopup::setAchieveTable()
{
	if(achieve_table)
	{
		achieve_table->removeFromParent();
		achieve_table = NULL;
	}
	
	achieve_list.clear();
	
	bool is_reward = false;
	
	if(recent_code == kAchievePopupListCode_all)
	{
		vector<AchieveGroup> another_list;
		another_list.clear();
		
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(!myAchieve->isCompleted((AchievementCode)i) &&
				   myAchieve->isAchieve((AchievementCode)i))
				{
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
					is_reward = true;
				}
				else
				{
					if(!isHaveAchieveGroup(another_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						another_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
				}
			}
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(!myAchieve->isCompleted((AchievementCode)i) &&
				   myAchieve->isAchieve((AchievementCode)i))
				{
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
					is_reward = true;
				}
				else// if(myAchieve->isCompleted((AchievementCode)i) ||
//						myAchieve->isAchieve((AchievementCode)i))
				{
					if(!isHaveAchieveGroup(another_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						another_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
				}
			}
		}
		
		for(int i=0;i<another_list.size();i++)
		{
			if(!isHaveAchieveGroup(achieve_list, another_list[i]))
				achieve_list.push_back(another_list[i]);
		}
	}
	else if(recent_code == kAchievePopupListCode_success)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(myAchieve->isCompleted((AchievementCode)i))
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
			}
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(myAchieve->isCompleted((AchievementCode)i))
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
			}
		}
	}
	else if(recent_code == kAchievePopupListCode_ing)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(!myAchieve->isCompleted((AchievementCode)i) &&
				   !myAchieve->isAchieve((AchievementCode)i))
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
			}
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(!myAchieve->isCompleted((AchievementCode)i) &&
				   !myAchieve->isAchieve((AchievementCode)i))
				{
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
				}
			}
		}
	}
	else if(recent_code == kAchievePopupListCode_reward)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(!myAchieve->isCompleted((AchievementCode)i) &&
				   myAchieve->isAchieve((AchievementCode)i))
				{
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
					is_reward = true;
				}
			}
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(!myAchieve->isCompleted((AchievementCode)i) &&
				   myAchieve->isAchieve((AchievementCode)i))
				{
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
					is_reward = true;
				}
			}
		}
	}
	
	CCSize table_size = CCSizeMake(445, 180);
	CCPoint table_position = ccp(25, 52);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setOpacity(100);
//	temp_back->setPosition(table_position);
//	main_case->addChild(temp_back, kAchievePopupZorder_table);
	
	CCScale9Sprite* scroll_back = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
	scroll_back->setContentSize(CCSizeMake(7, table_size.height-20));
	scroll_back->setPosition(ccpAdd(table_position, ccp(table_size.width-23, table_size.height/2.f)));
	main_case->addChild(scroll_back, kAchievePopupZorder_table);
	
	achieve_table = CCTableView::create(this, table_size);
	achieve_table->setAnchorPoint(CCPointZero);
	achieve_table->setDirection(kCCScrollViewDirectionVertical);
	achieve_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	achieve_table->setPosition(table_position);
	
	achieve_table->setDelegate(this);
	main_case->addChild(achieve_table, kAchievePopupZorder_table);
	achieve_table->setTouchPriority(-190);
	
	CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
																										 CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
	m_scrollBar = ScrollBar::createScrollbar(achieve_table, -18, NULL, scrollBar, -191);
	m_scrollBar->setDynamicScrollSize(true);
	m_scrollBar->setVisible(true);
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-189);
	t_suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	t_suction->setTouchEnabled(true);
	main_case->addChild(t_suction);
	
	
	all_reward_menu->setEnabled(is_reward);
}

void AchievePopup::cellAction( CCObject* sender )
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	loading_layer = LoadingLayer::create();
	addChild(loading_layer, kAchievePopupZorder_popup);
	
	keep_tag = tag;
	
	myAchieve->changeComplete(AchievementCode(keep_tag));
	
	if(myAchieve->isChangedAchieve())
	{
		mySGD->addChangeGoods(CCString::createWithFormat("ac_%d", keep_tag)->getCString());
		
		vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultSaveUserData));
		mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
	}
	else
	{
		loading_layer->removeFromParent();
		is_menu_enable = true;
	}
}

void AchievePopup::resultSaveUserData(Json::Value result_data)
{
	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("reward get success!!");
		
		setAchieveTable();
		
		loading_layer->removeFromParent();
		is_menu_enable = true;
	}
	else
	{
		CCLOG("reward get fail!!");
		
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
			vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultSaveUserData));
			mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		
//		mySGD->clearChangeGoods();
		
//		myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value);
	}
}

CCTableViewCell* AchievePopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int state_value = -1; // complete
	
	AchievementCode recent_code;
	
	for(int i=0;i<achieve_list[idx*2].achieve_list.size();i++)
	{
		state_value = 0;
		
		recent_code = achieve_list[idx*2].achieve_list[i];
		if(myAchieve->isCompleted(recent_code))
			state_value = -1;
		else if(myAchieve->isAchieve(recent_code))
		{
			state_value = 1;
			break;
		}
		else
			break;
	}
	
	if(achieve_list[idx*2].achieve_list.size() <= 0)
	{
		return cell;
	}
	
	string cell_back_filename;
	if(state_value == -1)
		cell_back_filename = "achievement_cell_success.png";
	else if(state_value == 1)
		cell_back_filename = "achievement_cellback_success.png";
	else if(recent_code > kAchievementCode_hidden_base && recent_code < kAchievementCode_hidden_end)
	{
		state_value = 2;
		cell_back_filename = "achievement_cell_hidden.png";
	}
	else
		cell_back_filename = "achievement_cellback_normal.png";
	
	CCScale9Sprite* cell_back;
	if(state_value == -1)
	{
		cell_back = CCScale9Sprite::create(cell_back_filename.c_str()); // , CCRectMake(0, 0, 47, 47), CCRectMake(23, 23, 1, 1));
	}
	else if(state_value == 2)
	{
		cell_back = CCScale9Sprite::create(cell_back_filename.c_str());
	}
	else
	{
		cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 47, 47), CCRectMake(23, 23, 1, 1));
		cell_back->setContentSize(CCSizeMake(206, 62));
	}
	
	
	
	cell_back->setAnchorPoint(CCPointZero);
	cell_back->setPosition(ccp(0,0));
	cell->addChild(cell_back);
	
	if(state_value == 2)
	{
		KSLabelTTF* cell_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_achieveHiddenTitle), mySGD->getFont().c_str(), 12);
		cell_title->setColor(ccc3(210, 190, 160));
		cell_title->setAnchorPoint(ccp(0.5f,0.5f));
		cell_title->setPosition(ccp(cell_back->getContentSize().width/2.f,40));
		cell_back->addChild(cell_title);
		
		KSLabelTTF* cell_content = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_achieveHiddenContent), mySGD->getFont().c_str(), 9);
		cell_content->setColor(ccc3(160, 130, 90));
		cell_content->setAnchorPoint(ccp(0.5f,0.5f));
		cell_content->setPosition(ccp(cell_back->getContentSize().width/2.f,20));
		cell_back->addChild(cell_content);
	}
	else
	{
		if(achieve_list[idx*2].achieve_list.size() == 3)
		{
			if(myAchieve->isCompleted(achieve_list[idx*2].achieve_list[2]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
				t_crown->setScale(0.7f);
				t_crown->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
			
			if(myAchieve->isCompleted(achieve_list[idx*2].achieve_list[1]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_silver.png");
				t_crown->setScale(0.7f);
				t_crown->setPosition(ccp(cell_back->getContentSize().width-15-21, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-15-21, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
			
			if(myAchieve->isCompleted(achieve_list[idx*2].achieve_list[0]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_bronze.png");
				t_crown->setScale(0.7f);
				t_crown->setPosition(ccp(cell_back->getContentSize().width-15-42, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-15-42, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
		}
		else if(achieve_list[idx*2].achieve_list.size() == 1)
		{
			if(myAchieve->isCompleted(achieve_list[idx*2].achieve_list[0]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
				t_crown->setScale(0.7f);
				t_crown->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
		}
		
		KSLabelTTF* cell_title = KSLabelTTF::create(CCString::createWithFormat("%s",
																			   myAchieve->getTitle(recent_code).c_str())->getCString(),
													//																		   myAchieve->getRecentValue(recent_code),
													//																		   myAchieve->getCondition(recent_code))->getCString(),
													mySGD->getFont().c_str(), 12);
		cell_title->setAnchorPoint(ccp(0,0.5));
		cell_title->setPosition(ccp(8,48));
		cell_back->addChild(cell_title);
		
		KSLabelTTF* cell_content = KSLabelTTF::create(myAchieve->getContent(recent_code).c_str(), mySGD->getFont().c_str(), 9);
		cell_content->setAnchorPoint(ccp(0,0.5));
		cell_content->setPosition(ccp(8,32));
		cell_back->addChild(cell_content);
		
		CCPoint img_position = ccp(170,24);
		if(myAchieve->isCompleted(recent_code))
		{
			//		cell_title->enableOuterStroke(ccc3(60, 0, 100), 1);
			cell_title->enableOuterStroke(ccc3(0, 0, 0), 1);
			
			CCSprite* success_img = CCSprite::create("achievement_cell_success.png");
			success_img->setOpacity(0);
			success_img->setPosition(ccp(cell_back->getContentSize().width/2.f, 15));
			cell_back->addChild(success_img);
			
			KSLabelTTF* success_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_achieveSuccess2), mySGD->getFont().c_str(), 10);
			success_label->enableOuterStroke(ccBLACK, 1.f);
			success_label->setPosition(ccpFromSize(success_img->getContentSize()/2.f));
			success_img->addChild(success_label);
		}
		else if(myAchieve->isAchieve(recent_code))
		{
			cell_title->enableOuterStroke(ccc3(70, 0, 40), 1);
			
			CCSprite* n_get = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, cell_back->getContentSize().width, cell_back->getContentSize().height));
			CCSprite* s_get = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, cell_back->getContentSize().width, cell_back->getContentSize().height));
			
			CCMenuItem* get_item = CCMenuItemSprite::create(n_get, s_get, this, menu_selector(AchievePopup::cellAction));
			get_item->setTag(recent_code);
			
			ScrollMenu* get_menu = ScrollMenu::create(get_item, NULL);
			get_menu->setPosition(ccpFromSize(cell_back->getContentSize()/2.f));
			cell_back->addChild(get_menu);
			get_menu->setTouchPriority(-200);
			
			
			CCScale9Sprite* graph_back = CCScale9Sprite::create("achievement_graph_back.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
			graph_back->setContentSize(CCSizeMake(195, 18));
			graph_back->setPosition(ccp(cell_back->getContentSize().width/2.f, 15));
			cell_back->addChild(graph_back);
			
			CCScale9Sprite* graph_complete = CCScale9Sprite::create("achievement_graph_complete.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
			graph_complete->setAnchorPoint(ccp(0,0));
			graph_complete->setPosition(ccp(0,0));
			graph_back->addChild(graph_complete);
			
			graph_complete->setContentSize(CCSizeMake(25+170*1, 18));
			
			KSLabelTTF* progress_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont().c_str(), 10);
			progress_label->enableOuterStroke(ccBLACK, 1.f);
			progress_label->setPosition(ccpFromSize(graph_back->getContentSize()/2.f));
			graph_back->addChild(progress_label);
			
			string reward_type_str;
			AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(recent_code);
			if(reward_type == kAchieveRewardType_ruby)
				reward_type_str = "price_ruby_img.png";
			else if(reward_type == kAchieveRewardType_gold)
				reward_type_str = "price_gold_img.png";
			else if(reward_type == kAchieveRewardType_package)
				reward_type_str = "price_package_img.png";
			
			//		CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
			//
			//		KSLabelTTF* ing_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_reward), mySGD->getFont2().c_str(), 11);
			//		ing_label->enableOuterStroke(ccc3(60, 45, 25), 1.f);
			//		ing_label->setPosition(ccp(ing_back->getContentSize().width/2.f, ing_back->getContentSize().height*0.65f));
			//		ing_back->addChild(ing_label);
			//
			//		ing_back->setPosition(img_position);
			//		cell_back->addChild(ing_back);
			
			CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
			reward_type_img->setScale(0.8f);
			reward_type_img->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
			graph_back->addChild(reward_type_img);
			
			KSLabelTTF* reward_value = KSLabelTTF::create(CCString::createWithFormat("+%d", myAchieve->getRewardValue(recent_code))->getCString(),
														  mySGD->getFont().c_str(), 10);
			reward_value->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
			graph_back->addChild(reward_value);
			
			
			
			//		string reward_type_str;
			//		AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(recent_code);
			//		if(reward_type == kAchieveRewardType_ruby)
			//			reward_type_str = "price_ruby_img.png";
			//		else if(reward_type == kAchieveRewardType_gold)
			//			reward_type_str = "price_gold_img.png";
			//		else if(reward_type == kAchieveRewardType_package)
			//			reward_type_str = "price_package_img.png";
			//
			//		CCSprite* n_reward_img = CCSprite::create("achievement_cell_reward_get.png");
			//
			//		KSLabelTTF* n_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
			//		n_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
			//		n_reward_label->setPosition(ccp(n_reward_img->getContentSize().width/2.f, n_reward_img->getContentSize().height*0.65f));
			//		n_reward_img->addChild(n_reward_label);
			//
			//		CCSprite* n_reward_type_img = CCSprite::create(reward_type_str.c_str());
			//		n_reward_type_img->setScale(0.5f);
			//		n_reward_type_img->setPosition(ccp(n_reward_img->getContentSize().width/2.f-12, n_reward_img->getContentSize().height/2.f-6.5f));
			//		n_reward_img->addChild(n_reward_type_img);
			//		CCLabelTTF* n_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(recent_code))->getCString(),
			//													  mySGD->getFont().c_str(), 10);
			//		n_reward_value->setPosition(ccp(n_reward_img->getContentSize().width/2.f+7, n_reward_img->getContentSize().height/2.f-7));
			//		n_reward_img->addChild(n_reward_value);
			//
			//		CCSprite* s_reward_img = CCSprite::create("achievement_cell_reward_get.png");
			//		s_reward_img->setColor(ccGRAY);
			//
			//		KSLabelTTF* s_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
			//		s_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
			//		s_reward_label->setPosition(ccp(s_reward_img->getContentSize().width/2.f, s_reward_img->getContentSize().height*0.65f));
			//		s_reward_img->addChild(s_reward_label);
			//
			//		CCSprite* s_reward_type_img = CCSprite::create(reward_type_str.c_str());
			//		s_reward_type_img->setScale(0.5f);
			//		s_reward_type_img->setPosition(ccp(s_reward_img->getContentSize().width/2.f-12, s_reward_img->getContentSize().height/2.f-6.5f));
			//		s_reward_img->addChild(s_reward_type_img);
			//		CCLabelTTF* s_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(recent_code))->getCString(),
			//														mySGD->getFont().c_str(), 10);
			//		s_reward_value->setPosition(ccp(s_reward_img->getContentSize().width/2.f+7, s_reward_img->getContentSize().height/2.f-7));
			//		s_reward_img->addChild(s_reward_value);
			//
			//		CCMenuItem* reward_get_item = CCMenuItemSprite::create(n_reward_img, s_reward_img, this, menu_selector(AchievePopup::cellAction));
			//		reward_get_item->setTag(recent_code);
			//
			//		ScrollMenu* reward_get_menu = ScrollMenu::create(reward_get_item, NULL);
			//		reward_get_menu->setPosition(img_position);
			//		cell_back->addChild(reward_get_menu);
			//		reward_get_menu->setTouchPriority(-188);
		}
		else
		{
			cell_title->enableOuterStroke(ccc3(70, 40, 0), 1);
			
			float rate = 1.f*myAchieve->getRecentValue(recent_code)/myAchieve->getCondition(recent_code);
			
			CCScale9Sprite* graph_back = CCScale9Sprite::create("achievement_graph_back.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
			graph_back->setContentSize(CCSizeMake(195, 18));
			graph_back->setPosition(ccp(cell_back->getContentSize().width/2.f, 15));
			cell_back->addChild(graph_back);
			
			CCScale9Sprite* graph_normal = CCScale9Sprite::create("achievement_graph_normal.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
			graph_normal->setAnchorPoint(ccp(0,0));
			graph_normal->setPosition(ccp(0,0));
			graph_back->addChild(graph_normal);
			
			if(myAchieve->getRecentValue(recent_code) == 0)
			{
				graph_normal->setContentSize(CCSizeMake(0, 18));
				graph_normal->setVisible(false);
			}
			else
				graph_normal->setContentSize(CCSizeMake(25+170*rate, 18));
			
			KSLabelTTF* progress_label = KSLabelTTF::create((KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getRecentValue(recent_code))->getCString()) + "/" + KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getPresentationCondition(recent_code))->getCString())).c_str(), mySGD->getFont().c_str(), 10);
			progress_label->enableOuterStroke(ccBLACK, 1.f);
			progress_label->setPosition(ccpFromSize(graph_back->getContentSize()/2.f));
			graph_back->addChild(progress_label);
			
			string reward_type_str;
			AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(recent_code);
			if(reward_type == kAchieveRewardType_ruby)
				reward_type_str = "price_ruby_img.png";
			else if(reward_type == kAchieveRewardType_gold)
				reward_type_str = "price_gold_img.png";
			else if(reward_type == kAchieveRewardType_package)
				reward_type_str = "price_package_img.png";
			
			//		CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
			//
			//		KSLabelTTF* ing_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_reward), mySGD->getFont2().c_str(), 11);
			//		ing_label->enableOuterStroke(ccc3(60, 45, 25), 1.f);
			//		ing_label->setPosition(ccp(ing_back->getContentSize().width/2.f, ing_back->getContentSize().height*0.65f));
			//		ing_back->addChild(ing_label);
			//
			//		ing_back->setPosition(img_position);
			//		cell_back->addChild(ing_back);
			
			CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
			reward_type_img->setScale(0.8f);
			reward_type_img->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
			graph_back->addChild(reward_type_img);
			
			KSLabelTTF* reward_value = KSLabelTTF::create(CCString::createWithFormat("+%d", myAchieve->getRewardValue(recent_code))->getCString(),
														  mySGD->getFont().c_str(), 10);
			reward_value->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
			graph_back->addChild(reward_value);
		}
	}
	
	if(idx*2+1 < achieve_list.size())
	{
		int state_value = -1; // complete
		
		AchievementCode recent_code;
		
		for(int i=0;i<achieve_list[idx*2+1].achieve_list.size();i++)
		{
			state_value = 0;
			
			recent_code = achieve_list[idx*2+1].achieve_list[i];
			if(myAchieve->isCompleted(recent_code))
				state_value = -1;
			else if(myAchieve->isAchieve(recent_code))
			{
				state_value = 1;
				break;
			}
			else
				break;
		}
		
		if(achieve_list[idx*2+1].achieve_list.size() <= 0)
		{
			return cell;
		}
		
		string cell_back_filename;
		if(myAchieve->isCompleted(recent_code))
			cell_back_filename = "achievement_cell_success.png";
		else if(myAchieve->isAchieve(recent_code))
			cell_back_filename = "achievement_cellback_success.png";
		else if(recent_code > kAchievementCode_hidden_base && recent_code < kAchievementCode_hidden_end)
		{
			state_value = 2;
			cell_back_filename = "achievement_cell_hidden.png";
		}
		else
			cell_back_filename = "achievement_cellback_normal.png";
		
			
		CCScale9Sprite* cell_back;
		if(state_value == -1)
		{
			cell_back = CCScale9Sprite::create(cell_back_filename.c_str()); // , CCRectMake(0, 0, 47, 47), CCRectMake(23, 23, 1, 1));
		}
		else if(state_value == 2)
		{
			cell_back = CCScale9Sprite::create(cell_back_filename.c_str());
		}
		else
		{
			cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 47, 47), CCRectMake(23, 23, 1, 1));
			cell_back->setContentSize(CCSizeMake(206, 62));
		}
		
		cell_back->setContentSize(CCSizeMake(206, 62));
		cell_back->setAnchorPoint(CCPointZero);
		cell_back->setPosition(ccp(385-180,0));
		cell->addChild(cell_back);
		
		if(state_value == 2)
		{
			KSLabelTTF* cell_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_achieveHiddenTitle), mySGD->getFont().c_str(), 12);
			cell_title->setColor(ccc3(210, 190, 160));
			cell_title->setAnchorPoint(ccp(0.5f,0.5f));
			cell_title->setPosition(ccp(cell_back->getContentSize().width/2.f,40));
			cell_back->addChild(cell_title);
			
			KSLabelTTF* cell_content = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_achieveHiddenContent), mySGD->getFont().c_str(), 9);
			cell_content->setColor(ccc3(160, 130, 90));
			cell_content->setAnchorPoint(ccp(0.5f,0.5f));
			cell_content->setPosition(ccp(cell_back->getContentSize().width/2.f,20));
			cell_back->addChild(cell_content);
		}
		else
		{
			if(achieve_list[idx*2+1].achieve_list.size() == 3)
			{
				if(myAchieve->isCompleted(achieve_list[idx*2+1].achieve_list[2]))
				{
					CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
					t_crown->setScale(0.7f);
					t_crown->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
					cell_back->addChild(t_crown);
				}
				else
				{
					CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
					t_back->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
					cell_back->addChild(t_back);
				}
				
				if(myAchieve->isCompleted(achieve_list[idx*2+1].achieve_list[1]))
				{
					CCSprite* t_crown = CCSprite::create("achievement_crown_silver.png");
					t_crown->setScale(0.7f);
					t_crown->setPosition(ccp(cell_back->getContentSize().width-15-21, cell_back->getContentSize().height-12));
					cell_back->addChild(t_crown);
				}
				else
				{
					CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
					t_back->setPosition(ccp(cell_back->getContentSize().width-15-21, cell_back->getContentSize().height-12));
					cell_back->addChild(t_back);
				}
				
				if(myAchieve->isCompleted(achieve_list[idx*2+1].achieve_list[0]))
				{
					CCSprite* t_crown = CCSprite::create("achievement_crown_bronze.png");
					t_crown->setScale(0.7f);
					t_crown->setPosition(ccp(cell_back->getContentSize().width-15-42, cell_back->getContentSize().height-12));
					cell_back->addChild(t_crown);
				}
				else
				{
					CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
					t_back->setPosition(ccp(cell_back->getContentSize().width-15-42, cell_back->getContentSize().height-12));
					cell_back->addChild(t_back);
				}
			}
			else if(achieve_list[idx*2+1].achieve_list.size() == 1)
			{
				if(myAchieve->isCompleted(achieve_list[idx*2+1].achieve_list[0]))
				{
					CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
					t_crown->setScale(0.7f);
					t_crown->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
					cell_back->addChild(t_crown);
				}
				else
				{
					CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
					t_back->setPosition(ccp(cell_back->getContentSize().width-15, cell_back->getContentSize().height-12));
					cell_back->addChild(t_back);
				}
			}
			
			KSLabelTTF* cell_title = KSLabelTTF::create(CCString::createWithFormat("%s",
																				   myAchieve->getTitle(recent_code).c_str())->getCString(),
														//																			   myAchieve->getRecentValue(recent_code),
														//																			   myAchieve->getCondition(recent_code))->getCString(),
														mySGD->getFont().c_str(), 12);
			cell_title->setAnchorPoint(ccp(0,0.5));
			cell_title->setPosition(ccp(8,48));
			cell_back->addChild(cell_title);
			
			KSLabelTTF* cell_content = KSLabelTTF::create(myAchieve->getContent(recent_code).c_str(), mySGD->getFont().c_str(), 9);
			cell_content->setAnchorPoint(ccp(0,0.5));
			cell_content->setPosition(ccp(8,32));
			cell_back->addChild(cell_content);
			
			CCPoint img_position = ccp(170,24);
			
			if(myAchieve->isCompleted(recent_code))
			{
				//			cell_title->enableOuterStroke(ccc3(60, 0, 100), 1);
				cell_title->enableOuterStroke(ccc3(0, 0, 0), 1);
				CCSprite* success_img = CCSprite::create("achievement_cell_success.png");
				success_img->setOpacity(0);
				success_img->setPosition(ccp(cell_back->getContentSize().width/2.f, 15));
				cell_back->addChild(success_img);
				
				KSLabelTTF* success_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_achieveSuccess2), mySGD->getFont().c_str(), 10);
				success_label->enableOuterStroke(ccBLACK, 1.f);
				success_label->setPosition(ccpFromSize(success_img->getContentSize()/2.f));
				success_img->addChild(success_label);
			}
			else if(myAchieve->isAchieve(recent_code))
			{
				cell_title->enableOuterStroke(ccc3(70, 0, 40), 1);
				
				CCSprite* n_get = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, cell_back->getContentSize().width, cell_back->getContentSize().height));
				CCSprite* s_get = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, cell_back->getContentSize().width, cell_back->getContentSize().height));
				
				CCMenuItem* get_item = CCMenuItemSprite::create(n_get, s_get, this, menu_selector(AchievePopup::cellAction));
				get_item->setTag(recent_code);
				
				ScrollMenu* get_menu = ScrollMenu::create(get_item, NULL);
				get_menu->setPosition(ccpFromSize(cell_back->getContentSize()/2.f));
				cell_back->addChild(get_menu);
				get_menu->setTouchPriority(-200);
				
				CCScale9Sprite* graph_back = CCScale9Sprite::create("achievement_graph_back.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
				graph_back->setContentSize(CCSizeMake(195, 18));
				graph_back->setPosition(ccp(cell_back->getContentSize().width/2.f, 15));
				cell_back->addChild(graph_back);
				
				CCScale9Sprite* graph_complete = CCScale9Sprite::create("achievement_graph_complete.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
				graph_complete->setAnchorPoint(ccp(0,0));
				graph_complete->setPosition(ccp(0,0));
				graph_back->addChild(graph_complete);
				
				graph_complete->setContentSize(CCSizeMake(25+170*1, 18));
				
				KSLabelTTF* progress_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont().c_str(), 10);
				progress_label->enableOuterStroke(ccBLACK, 1.f);
				progress_label->setPosition(ccpFromSize(graph_back->getContentSize()/2.f));
				graph_back->addChild(progress_label);
				
				string reward_type_str;
				AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(recent_code);
				if(reward_type == kAchieveRewardType_ruby)
					reward_type_str = "price_ruby_img.png";
				else if(reward_type == kAchieveRewardType_gold)
					reward_type_str = "price_gold_img.png";
				else if(reward_type == kAchieveRewardType_package)
					reward_type_str = "price_package_img.png";
				
				//		CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
				//
				//		KSLabelTTF* ing_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_reward), mySGD->getFont2().c_str(), 11);
				//		ing_label->enableOuterStroke(ccc3(60, 45, 25), 1.f);
				//		ing_label->setPosition(ccp(ing_back->getContentSize().width/2.f, ing_back->getContentSize().height*0.65f));
				//		ing_back->addChild(ing_label);
				//
				//		ing_back->setPosition(img_position);
				//		cell_back->addChild(ing_back);
				
				CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
				reward_type_img->setScale(0.8f);
				reward_type_img->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
				graph_back->addChild(reward_type_img);
				
				KSLabelTTF* reward_value = KSLabelTTF::create(CCString::createWithFormat("+%d", myAchieve->getRewardValue(recent_code))->getCString(),
															  mySGD->getFont().c_str(), 10);
				reward_value->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
				graph_back->addChild(reward_value);
				
				//			string reward_type_str;
				//			AchieveRewardType reward_type = myAchieve->getRewardType(recent_code);
				//			if(reward_type == kAchieveRewardType_ruby)
				//				reward_type_str = "price_ruby_img.png";
				//			else if(reward_type == kAchieveRewardType_gold)
				//				reward_type_str = "price_gold_img.png";
				//			else if(reward_type == kAchieveRewardType_package)
				//				reward_type_str = "price_package_img.png";
				//
				//			CCSprite* n_reward_img = CCSprite::create("achievement_cell_reward_get.png");
				//
				//			KSLabelTTF* n_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
				//			n_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
				//			n_reward_label->setPosition(ccp(n_reward_img->getContentSize().width/2.f, n_reward_img->getContentSize().height*0.65f));
				//			n_reward_img->addChild(n_reward_label);
				//
				//			CCSprite* n_reward_type_img = CCSprite::create(reward_type_str.c_str());
				//			n_reward_type_img->setScale(0.5f);
				//			n_reward_type_img->setPosition(ccp(n_reward_img->getContentSize().width/2.f-12, n_reward_img->getContentSize().height/2.f-6.5f));
				//			n_reward_img->addChild(n_reward_type_img);
				//			CCLabelTTF* n_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(recent_code))->getCString(),
				//															mySGD->getFont().c_str(), 10);
				//			n_reward_value->setPosition(ccp(n_reward_img->getContentSize().width/2.f+7, n_reward_img->getContentSize().height/2.f-7));
				//			n_reward_img->addChild(n_reward_value);
				//
				//			CCSprite* s_reward_img = CCSprite::create("achievement_cell_reward_get.png");
				//			s_reward_img->setColor(ccGRAY);
				//
				//			KSLabelTTF* s_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
				//			s_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
				//			s_reward_label->setPosition(ccp(s_reward_img->getContentSize().width/2.f, s_reward_img->getContentSize().height*0.65f));
				//			s_reward_img->addChild(s_reward_label);
				//
				//			CCSprite* s_reward_type_img = CCSprite::create(reward_type_str.c_str());
				//			s_reward_type_img->setScale(0.5f);
				//			s_reward_type_img->setPosition(ccp(s_reward_img->getContentSize().width/2.f-12, s_reward_img->getContentSize().height/2.f-6.5f));
				//			s_reward_img->addChild(s_reward_type_img);
				//			CCLabelTTF* s_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", myAchieve->getRewardValue(recent_code))->getCString(),
				//															mySGD->getFont().c_str(), 10);
				//			s_reward_value->setPosition(ccp(s_reward_img->getContentSize().width/2.f+7, s_reward_img->getContentSize().height/2.f-7));
				//			s_reward_img->addChild(s_reward_value);
				//
				//			CCMenuItem* reward_get_item = CCMenuItemSprite::create(n_reward_img, s_reward_img, this, menu_selector(AchievePopup::cellAction));
				//			reward_get_item->setTag(recent_code);
				//
				//			ScrollMenu* reward_get_menu = ScrollMenu::create(reward_get_item, NULL);
				//			reward_get_menu->setPosition(img_position);
				//			cell_back->addChild(reward_get_menu);
				//			reward_get_menu->setTouchPriority(-188);
			}
			else
			{
				cell_title->enableOuterStroke(ccc3(70, 40, 0), 1);
				
				float rate = 1.f*myAchieve->getRecentValue(recent_code)/myAchieve->getCondition(recent_code);
				
				CCScale9Sprite* graph_back = CCScale9Sprite::create("achievement_graph_back.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
				graph_back->setContentSize(CCSizeMake(195, 18));
				graph_back->setPosition(ccp(cell_back->getContentSize().width/2.f, 15));
				cell_back->addChild(graph_back);
				
				CCScale9Sprite* graph_normal = CCScale9Sprite::create("achievement_graph_normal.png", CCRectMake(0, 0, 25, 18), CCRectMake(12, 8, 1, 2));
				graph_normal->setAnchorPoint(ccp(0,0));
				graph_normal->setPosition(ccp(0,0));
				graph_back->addChild(graph_normal);
				
				if(myAchieve->getRecentValue(recent_code) == 0)
				{
					graph_normal->setContentSize(CCSizeMake(0, 18));
					graph_normal->setVisible(false);
				}
				else
					graph_normal->setContentSize(CCSizeMake(25+170*rate, 18));
				
				KSLabelTTF* progress_label = KSLabelTTF::create((KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getRecentValue(recent_code))->getCString()) + "/" + KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getPresentationCondition(recent_code))->getCString())).c_str(), mySGD->getFont().c_str(), 10);
				progress_label->enableOuterStroke(ccBLACK, 1.f);
				progress_label->setPosition(ccpFromSize(graph_back->getContentSize()/2.f));
				graph_back->addChild(progress_label);
				
				string reward_type_str;
				AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(recent_code);
				if(reward_type == kAchieveRewardType_ruby)
					reward_type_str = "price_ruby_img.png";
				else if(reward_type == kAchieveRewardType_gold)
					reward_type_str = "price_gold_img.png";
				else if(reward_type == kAchieveRewardType_package)
					reward_type_str = "price_package_img.png";
				
				//		CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
				//
				//		KSLabelTTF* ing_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_reward), mySGD->getFont2().c_str(), 11);
				//		ing_label->enableOuterStroke(ccc3(60, 45, 25), 1.f);
				//		ing_label->setPosition(ccp(ing_back->getContentSize().width/2.f, ing_back->getContentSize().height*0.65f));
				//		ing_back->addChild(ing_label);
				//
				//		ing_back->setPosition(img_position);
				//		cell_back->addChild(ing_back);
				
				CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
				reward_type_img->setScale(0.8f);
				reward_type_img->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
				graph_back->addChild(reward_type_img);
				
				KSLabelTTF* reward_value = KSLabelTTF::create(CCString::createWithFormat("+%d", myAchieve->getRewardValue(recent_code))->getCString(),
															  mySGD->getFont().c_str(), 10);
				reward_value->setPosition(ccp(graph_back->getContentSize().width-12, graph_back->getContentSize().height/2.f));
				graph_back->addChild(reward_value);
			}
		}
	}
	
	return cell;
}

void AchievePopup::scrollViewDidScroll( CCScrollView* view )
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
}

void AchievePopup::scrollViewDidZoom( CCScrollView* view ){}

void AchievePopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	//		CCLOG("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize AchievePopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(385, 62);
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
	CCLOG("touch swallow!!!");
	
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
	if(!all_menu)
	{
		all_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_allView), 13, CCSizeMake(80,38), CommonButtonYellowDown, -190);
		all_menu->setPosition(ccp(169,256));
		main_case->addChild(all_menu, kAchievePopupZorder_menu);
		all_menu->setFunction([=](CCObject* sender)
							  {
								  CCNode* t_node = CCNode::create();
								  t_node->setTag(kAchievePopupMenuTag_all);
								  menuAction(t_node);
							  });
		
		all_menu->setBackgroundTypeForDisabled(CommonButtonYellowUp);
		all_menu->setTitleColorForDisable(ccc3(50, 20, 0));
	}
	
	all_menu->setEnabled(recent_code != kAchievePopupListCode_all);
}
void AchievePopup::setSuccessMenu()
{
	if(!success_menu)
	{
		success_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_achieveSuccess), 13, CCSizeMake(80,38), CommonButtonYellowDown, -190);
//		success_menu->setTitleColor(ccWHITE);
		success_menu->setPosition(ccp(244,256));
		main_case->addChild(success_menu, kAchievePopupZorder_menu);
		success_menu->setFunction([=](CCObject* sender)
							  {
								  CCNode* t_node = CCNode::create();
								  t_node->setTag(kAchievePopupMenuTag_success);
								  menuAction(t_node);
							  });
		
		success_menu->setBackgroundTypeForDisabled(CommonButtonYellowUp);
		success_menu->setTitleColorForDisable(ccc3(50, 20, 0));
	}
	
	success_menu->setEnabled(recent_code != kAchievePopupListCode_success);
}
void AchievePopup::setIngMenu()
{
	if(!ing_menu)
	{
		ing_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_achieveNotSuccess), 13, CCSizeMake(80,38), CommonButtonYellowDown, -190);
//		ing_menu->setTitleColor(ccWHITE);
		ing_menu->setPosition(ccp(319,256));
		main_case->addChild(ing_menu, kAchievePopupZorder_menu);
		ing_menu->setFunction([=](CCObject* sender)
								  {
									  CCNode* t_node = CCNode::create();
									  t_node->setTag(kAchievePopupMenuTag_ing);
									  menuAction(t_node);
								  });
		
		ing_menu->setBackgroundTypeForDisabled(CommonButtonYellowUp);
		ing_menu->setTitleColorForDisable(ccc3(50, 20, 0));
	}
	
	ing_menu->setEnabled(recent_code != kAchievePopupListCode_ing);
}
void AchievePopup::setRewardMenu()
{
	if(!reward_menu)
	{
		reward_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_achieveReward), 13, CCSizeMake(80,38), CommonButtonYellowDown, -190);
//		reward_menu->setTitleColor(ccWHITE);
		reward_menu->setPosition(ccp(394,256));
		main_case->addChild(reward_menu, kAchievePopupZorder_menu);
		reward_menu->setFunction([=](CCObject* sender)
							  {
								  CCNode* t_node = CCNode::create();
								  t_node->setTag(kAchievePopupMenuTag_reward);
								  menuAction(t_node);
							  });
		
		reward_menu->setBackgroundTypeForDisabled(CommonButtonYellowUp);
		reward_menu->setTitleColorForDisable(ccc3(50, 20, 0));
	}
	
	reward_menu->setEnabled(recent_code != kAchievePopupListCode_reward);
}

void AchievePopup::takeAllReward(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	bool is_have = false;
	
	vector<AchievementCode> one_list;
	one_list.clear();
	
	for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
	{
		if(!myAchieve->isCompleted((AchievementCode)i) &&
		   myAchieve->isAchieve((AchievementCode)i))
		{
			mySGD->addChangeGoods(CCString::createWithFormat("ac_%d", i)->getCString());
			myAchieve->changeComplete((AchievementCode)i);
			
			one_list.push_back((AchievementCode)i);
			is_have = true;
		}
	}
	
	for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
	{
		if(!myAchieve->isCompleted((AchievementCode)i) &&
		   myAchieve->isAchieve((AchievementCode)i))
		{
			mySGD->addChangeGoods(CCString::createWithFormat("ac_%d", i)->getCString());
			myAchieve->changeComplete((AchievementCode)i);
			
			one_list.push_back((AchievementCode)i);
			is_have = true;
		}
	}
	
	if(is_have)
	{
		loading_layer = LoadingLayer::create();
		addChild(loading_layer, kAchievePopupZorder_popup);
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		if(myAchieve->isChangedAchieve())
		{
			vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultAllTakeSaveUserData));
			mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
		}
		else
		{
			loading_layer->removeFromParent();
			is_menu_enable = true;
		}
	}
	else
	{
		all_reward_menu->setEnabled(false);
		is_menu_enable = true;
	}
}

void AchievePopup::resultAllTakeSaveUserData(Json::Value result_data)
{
	CCLOG("resultAllTakeSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("reward get success!!");
		
		setAchieveTable();
		
		loading_layer->removeFromParent();
		is_menu_enable = true;
	}
	else
	{
		CCLOG("reward get fail!!");
		
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
			vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(nullptr);
			mySGD->changeGoodsTransaction(t_command_achieve, json_selector(this, AchievePopup::resultSaveUserData));
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		
//		mySGD->clearChangeGoods();
		
//		for(int i=0;i<keep_value_list.size();i++)
//			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value_list[i], false);
//		myDSH->fFlush();
	}
}