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
	main_case->setPosition(ccp(240,160-450));
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
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(AchievePopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
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
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
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
			
			int is_found = myAchieve->isHaveRewardCount();
			
			if(is_found > 0)
			{
				all_reward_menu->setEnabled(true);
			}
			else
			{
				all_reward_menu->setEnabled(false);
			}
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
			all_reward_menu->setEnabled(false);
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
			all_reward_menu->setEnabled(false);
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
				all_reward_menu->setEnabled(false);
				empty_ment->setString(myLoc->getLocalForKey(kMyLocalKey_nothingRewardAchieve));
			}
			else
			{
				all_reward_menu->setEnabled(true);
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
		vector<AchievementCode> another_list;
		another_list.clear();
		
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(!myAchieve->isCompleted((AchievementCode)i) &&
			   myAchieve->isAchieve((AchievementCode)i))
			{
				achieve_list.push_back((AchievementCode)i);
				is_reward = true;
			}
			else
				another_list.push_back((AchievementCode)i);
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(!myAchieve->isCompleted((AchievementCode)i) &&
			   myAchieve->isAchieve((AchievementCode)i))
			{
				achieve_list.push_back((AchievementCode)i);
				is_reward = true;
			}
			else if(myAchieve->isCompleted((AchievementCode)i) ||
					myAchieve->isAchieve((AchievementCode)i))
				another_list.push_back((AchievementCode)i);
		}
		
		for(int i=0;i<another_list.size();i++)
		{
			achieve_list.push_back(another_list[i]);
		}
	}
	else if(recent_code == kAchievePopupListCode_success)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(myAchieve->isCompleted((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myAchieve->isCompleted((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
	}
	else if(recent_code == kAchievePopupListCode_ing)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(!myAchieve->isCompleted((AchievementCode)i) &&
			   !myAchieve->isAchieve((AchievementCode)i))
				achieve_list.push_back((AchievementCode)i);
		}
	}
	else if(recent_code == kAchievePopupListCode_reward)
	{
		for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
		{
			if(!myAchieve->isCompleted((AchievementCode)i) &&
			   myAchieve->isAchieve((AchievementCode)i))
			{
				achieve_list.push_back((AchievementCode)i);
				is_reward = true;
			}
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(!myAchieve->isCompleted((AchievementCode)i) &&
			   myAchieve->isAchieve((AchievementCode)i))
			{
				achieve_list.push_back((AchievementCode)i);
				is_reward = true;
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
	
	m_scrollBar = ScrollBar::createScrollbar(achieve_table, -2 - 10, NULL, CCScale9Sprite::create("cardsetting_scrollbutton.png"), -191);
	m_scrollBar->setDynamicScrollSize(false);
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
	AchieveRewardType reward_type = myAchieve->getRewardType(achieve_list[keep_tag]);
	int reward_value = myAchieve->getRewardValue(achieve_list[keep_tag]);
	
	if(reward_type == kAchieveRewardType_ruby)
		mySGD->addChangeGoods(kGoodsType_ruby, reward_value, "업적", CCString::createWithFormat("%d", achieve_list[keep_tag])->getCString(), "", false);
	else if(reward_type == kAchieveRewardType_gold)
		mySGD->addChangeGoods(kGoodsType_gold, reward_value, "업적", CCString::createWithFormat("%d", achieve_list[keep_tag])->getCString());
	else if(reward_type == kAchieveRewardType_package)
	{
		for(int i=0;i<reward_value;i++)
		{
			AchieveRewardType t_type = myAchieve->getRewardTypeForIndex(achieve_list[keep_tag], i);
			int t_value = myAchieve->getRewardValueForIndex(achieve_list[keep_tag], i);
			
			if(t_type == kAchieveRewardType_ruby)
				mySGD->addChangeGoods(kGoodsType_ruby, t_value, "업적", CCString::createWithFormat("%d", achieve_list[keep_tag])->getCString(), "", false);
			else if(t_type == kAchieveRewardType_gold)
				mySGD->addChangeGoods(kGoodsType_gold, t_value, "업적", CCString::createWithFormat("%d", achieve_list[keep_tag])->getCString());
		}
	}
	
	myAchieve->changeComplete(achieve_list[keep_tag]);
	
	vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultSaveUserData));
	mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
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
		
		addChild(KSTimer::create(0.1f, [=](){
			vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(nullptr);
			mySGD->changeGoodsTransaction(t_command_achieve, json_selector(this, AchievePopup::resultSaveUserData));
		}));
		
//		mySGD->clearChangeGoods();
		
//		myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value);
	}
}

CCTableViewCell* AchievePopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	string cell_back_filename;
	if(myAchieve->isCompleted(achieve_list[idx*2]))
		cell_back_filename = "achievement_cellback_success.png";
	else if(myAchieve->isAchieve(achieve_list[idx*2]))
		cell_back_filename = "achievement_cellback_reward.png";
	else
		cell_back_filename = "achievement_cellback_normal.png";
	
	CCScale9Sprite* cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 47, 47), CCRectMake(23, 23, 1, 1));
	cell_back->setContentSize(CCSizeMake(206, 47));
	cell_back->setAnchorPoint(CCPointZero);
	cell_back->setPosition(ccp(0,0));
	cell->addChild(cell_back);
	
	KSLabelTTF* cell_title = KSLabelTTF::create(CCString::createWithFormat("%s(%d/%d)",
																		   myAchieve->getTitle(achieve_list[idx*2]).c_str(),
																		   myAchieve->getRecentValue(achieve_list[idx*2]),
																		   myAchieve->getCondition(achieve_list[idx*2]))->getCString(),
																		   mySGD->getFont().c_str(), 11);
	cell_title->setAnchorPoint(ccp(0,0.5));
	cell_title->setPosition(ccp(8,30));
	cell_back->addChild(cell_title);
	
	CCLabelTTF* cell_content = CCLabelTTF::create(myAchieve->getContent(achieve_list[idx*2]).c_str(), mySGD->getFont().c_str(), 8);
	cell_content->setAnchorPoint(ccp(0,0.5));
	cell_content->setPosition(ccp(8,14));
	cell_back->addChild(cell_content);
	
	CCPoint img_position = ccp(170,24);
	if(myAchieve->isCompleted(achieve_list[idx*2]))
	{
		cell_title->enableOuterStroke(ccc3(60, 0, 100), 1);
		
		CCSprite* success_img = CCSprite::create("achievement_cell_success.png");
		success_img->setPosition(img_position);
		cell_back->addChild(success_img);
	}
	else if(myAchieve->isAchieve(achieve_list[idx*2]))
	{
		cell_title->enableOuterStroke(ccc3(70, 0, 40), 1);
		
		string reward_type_str;
		AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[idx*2]);
		if(reward_type == kAchieveRewardType_ruby)
			reward_type_str = "price_ruby_img.png";
		else if(reward_type == kAchieveRewardType_gold)
			reward_type_str = "price_gold_img.png";
		else if(reward_type == kAchieveRewardType_package)
			reward_type_str = "price_package_img.png";
		
		CCSprite* n_reward_img = CCSprite::create("achievement_cell_reward_get.png");
		
		KSLabelTTF* n_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
		n_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
		n_reward_label->setPosition(ccp(n_reward_img->getContentSize().width/2.f, n_reward_img->getContentSize().height*0.65f));
		n_reward_img->addChild(n_reward_label);
		
		CCSprite* n_reward_type_img = CCSprite::create(reward_type_str.c_str());
		n_reward_type_img->setScale(0.5f);
		n_reward_type_img->setPosition(ccp(n_reward_img->getContentSize().width/2.f-12, n_reward_img->getContentSize().height/2.f-6.5f));
		n_reward_img->addChild(n_reward_type_img);
		CCLabelTTF* n_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[idx*2]))->getCString(),
													  mySGD->getFont().c_str(), 10);
		n_reward_value->setPosition(ccp(n_reward_img->getContentSize().width/2.f+7, n_reward_img->getContentSize().height/2.f-7));
		n_reward_img->addChild(n_reward_value);
		
		CCSprite* s_reward_img = CCSprite::create("achievement_cell_reward_get.png");
		s_reward_img->setColor(ccGRAY);
		
		KSLabelTTF* s_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
		s_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
		s_reward_label->setPosition(ccp(s_reward_img->getContentSize().width/2.f, s_reward_img->getContentSize().height*0.65f));
		s_reward_img->addChild(s_reward_label);
		
		CCSprite* s_reward_type_img = CCSprite::create(reward_type_str.c_str());
		s_reward_type_img->setScale(0.5f);
		s_reward_type_img->setPosition(ccp(s_reward_img->getContentSize().width/2.f-12, s_reward_img->getContentSize().height/2.f-6.5f));
		s_reward_img->addChild(s_reward_type_img);
		CCLabelTTF* s_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[idx*2]))->getCString(),
														mySGD->getFont().c_str(), 10);
		s_reward_value->setPosition(ccp(s_reward_img->getContentSize().width/2.f+7, s_reward_img->getContentSize().height/2.f-7));
		s_reward_img->addChild(s_reward_value);
		
		CCMenuItem* reward_get_item = CCMenuItemSprite::create(n_reward_img, s_reward_img, this, menu_selector(AchievePopup::cellAction));
		reward_get_item->setTag(idx*2);
		
		ScrollMenu* reward_get_menu = ScrollMenu::create(reward_get_item, NULL);
		reward_get_menu->setPosition(img_position);
		cell_back->addChild(reward_get_menu);
		reward_get_menu->setTouchPriority(-188);
	}
	else
	{
		cell_title->enableOuterStroke(ccc3(70, 40, 0), 1);
		
		string reward_type_str;
		AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(achieve_list[idx*2]);
		if(reward_type == kAchieveRewardType_ruby)
			reward_type_str = "price_ruby_img.png";
		else if(reward_type == kAchieveRewardType_gold)
			reward_type_str = "price_gold_img.png";
		else if(reward_type == kAchieveRewardType_package)
			reward_type_str = "price_package_img.png";
		
		CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
		
		KSLabelTTF* ing_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_reward), mySGD->getFont2().c_str(), 11);
		ing_label->enableOuterStroke(ccc3(60, 45, 25), 1.f);
		ing_label->setPosition(ccp(ing_back->getContentSize().width/2.f, ing_back->getContentSize().height*0.65f));
		ing_back->addChild(ing_label);
		
		ing_back->setPosition(img_position);
		cell_back->addChild(ing_back);
		CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
		reward_type_img->setScale(0.5f);
		reward_type_img->setPosition(ccp(ing_back->getContentSize().width/2.f-12, ing_back->getContentSize().height/2.f-6.5f));
		ing_back->addChild(reward_type_img);
		CCLabelTTF* reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", myAchieve->getRewardValue(achieve_list[idx*2]))->getCString(),
														mySGD->getFont().c_str(), 10);
		reward_value->setPosition(ccp(ing_back->getContentSize().width/2.f+7, ing_back->getContentSize().height/2.f-7));
		ing_back->addChild(reward_value);
	}
	
	if(idx*2+1 < achieve_list.size())
	{
		string cell_back_filename;
		if(myAchieve->isCompleted(achieve_list[idx*2+1]))
			cell_back_filename = "achievement_cellback_success.png";
		else if(myAchieve->isAchieve(achieve_list[idx*2+1]))
			cell_back_filename = "achievement_cellback_reward.png";
		else
			cell_back_filename = "achievement_cellback_normal.png";
		
		CCScale9Sprite* cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 47, 47), CCRectMake(23, 23, 1, 1));
		cell_back->setContentSize(CCSizeMake(206, 47));
		cell_back->setAnchorPoint(CCPointZero);
		cell_back->setPosition(ccp(385-180,0));
		cell->addChild(cell_back);
		
		KSLabelTTF* cell_title = KSLabelTTF::create(CCString::createWithFormat("%s(%d/%d)",
																			   myAchieve->getTitle(achieve_list[idx*2+1]).c_str(),
																			   myAchieve->getRecentValue(achieve_list[idx*2+1]),
																			   myAchieve->getCondition(achieve_list[idx*2+1]))->getCString(),
													mySGD->getFont().c_str(), 11);
		cell_title->setAnchorPoint(ccp(0,0.5));
		cell_title->setPosition(ccp(8,30));
		cell_back->addChild(cell_title);
		
		CCLabelTTF* cell_content = CCLabelTTF::create(myAchieve->getContent(achieve_list[idx*2+1]).c_str(), mySGD->getFont().c_str(), 8);
		cell_content->setAnchorPoint(ccp(0,0.5));
		cell_content->setPosition(ccp(8,14));
		cell_back->addChild(cell_content);
		
		CCPoint img_position = ccp(170,24);
		
		if(myAchieve->isCompleted(achieve_list[idx*2+1]))
		{
			cell_title->enableOuterStroke(ccc3(60, 0, 100), 1);
			CCSprite* success_img = CCSprite::create("achievement_cell_success.png");
			success_img->setPosition(img_position);
			cell_back->addChild(success_img);
		}
		else if(myAchieve->isAchieve(achieve_list[idx*2+1]))
		{
			cell_title->enableOuterStroke(ccc3(70, 0, 40), 1);
			string reward_type_str;
			AchieveRewardType reward_type = myAchieve->getRewardType(achieve_list[idx*2+1]);
			if(reward_type == kAchieveRewardType_ruby)
				reward_type_str = "price_ruby_img.png";
			else if(reward_type == kAchieveRewardType_gold)
				reward_type_str = "price_gold_img.png";
			else if(reward_type == kAchieveRewardType_package)
				reward_type_str = "price_package_img.png";
			
			CCSprite* n_reward_img = CCSprite::create("achievement_cell_reward_get.png");
			
			KSLabelTTF* n_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
			n_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
			n_reward_label->setPosition(ccp(n_reward_img->getContentSize().width/2.f, n_reward_img->getContentSize().height*0.65f));
			n_reward_img->addChild(n_reward_label);
			
			CCSprite* n_reward_type_img = CCSprite::create(reward_type_str.c_str());
			n_reward_type_img->setScale(0.5f);
			n_reward_type_img->setPosition(ccp(n_reward_img->getContentSize().width/2.f-12, n_reward_img->getContentSize().height/2.f-6.5f));
			n_reward_img->addChild(n_reward_type_img);
			CCLabelTTF* n_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", AchieveConditionReward::sharedInstance()->getRewardValue(achieve_list[idx*2+1]))->getCString(),
															mySGD->getFont().c_str(), 10);
			n_reward_value->setPosition(ccp(n_reward_img->getContentSize().width/2.f+7, n_reward_img->getContentSize().height/2.f-7));
			n_reward_img->addChild(n_reward_value);
			
			CCSprite* s_reward_img = CCSprite::create("achievement_cell_reward_get.png");
			s_reward_img->setColor(ccGRAY);
			
			KSLabelTTF* s_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_getReward), mySGD->getFont2().c_str(), 11);
			s_reward_label->enableOuterStroke(ccc3(65, 45, 90), 1.f);
			s_reward_label->setPosition(ccp(s_reward_img->getContentSize().width/2.f, s_reward_img->getContentSize().height*0.65f));
			s_reward_img->addChild(s_reward_label);
			
			CCSprite* s_reward_type_img = CCSprite::create(reward_type_str.c_str());
			s_reward_type_img->setScale(0.5f);
			s_reward_type_img->setPosition(ccp(s_reward_img->getContentSize().width/2.f-12, s_reward_img->getContentSize().height/2.f-6.5f));
			s_reward_img->addChild(s_reward_type_img);
			CCLabelTTF* s_reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", myAchieve->getRewardValue(achieve_list[idx*2+1]))->getCString(),
															mySGD->getFont().c_str(), 10);
			s_reward_value->setPosition(ccp(s_reward_img->getContentSize().width/2.f+7, s_reward_img->getContentSize().height/2.f-7));
			s_reward_img->addChild(s_reward_value);
			
			CCMenuItem* reward_get_item = CCMenuItemSprite::create(n_reward_img, s_reward_img, this, menu_selector(AchievePopup::cellAction));
			reward_get_item->setTag(idx*2+1);
			
			ScrollMenu* reward_get_menu = ScrollMenu::create(reward_get_item, NULL);
			reward_get_menu->setPosition(img_position);
			cell_back->addChild(reward_get_menu);
			reward_get_menu->setTouchPriority(-188);
		}
		else
		{
			cell_title->enableOuterStroke(ccc3(70, 40, 0), 1);
			string reward_type_str;
			AchieveRewardType reward_type = myAchieve->getRewardType(achieve_list[idx*2+1]);
			if(reward_type == kAchieveRewardType_ruby)
				reward_type_str = "price_ruby_img.png";
			else if(reward_type == kAchieveRewardType_gold)
				reward_type_str = "price_gold_img.png";
			else if(reward_type == kAchieveRewardType_package)
				reward_type_str = "price_package_img.png";
			
			CCSprite* ing_back = CCSprite::create("achievement_cell_reward_view.png");
			
			KSLabelTTF* ing_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_reward), mySGD->getFont2().c_str(), 11);
			ing_label->enableOuterStroke(ccc3(60, 45, 25), 1.f);
			ing_label->setPosition(ccp(ing_back->getContentSize().width/2.f, ing_back->getContentSize().height*0.65f));
			ing_back->addChild(ing_label);
			
			ing_back->setPosition(img_position);
			cell_back->addChild(ing_back);
			CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
			reward_type_img->setScale(0.5f);
			reward_type_img->setPosition(ccp(ing_back->getContentSize().width/2.f-12, ing_back->getContentSize().height/2.f-6.5f));
			ing_back->addChild(reward_type_img);
			CCLabelTTF* reward_value = CCLabelTTF::create(CCString::createWithFormat("%d", myAchieve->getRewardValue(achieve_list[idx*2+1]))->getCString(),
														  mySGD->getFont().c_str(), 10);
			reward_value->setPosition(ccp(ing_back->getContentSize().width/2.f+7, ing_back->getContentSize().height/2.f-7));
			ing_back->addChild(reward_value);
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
	return CCSizeMake(385, 47);
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
		all_menu->setPosition(ccp(150,256));
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
		success_menu->setPosition(ccp(225,256));
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
		ing_menu->setPosition(ccp(300,256));
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
		reward_menu->setPosition(ccp(375,256));
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
	
	keep_value_list.clear();
	
	keep_take_gold = 0;
	keep_take_ruby = 0;
	
	string ruby_stats_value = "";
	string gold_stats_value = "";
	
	for(int i=0;i<achieve_list.size();i++)
	{
		if(myAchieve->isAchieve(achieve_list[i]))
		{
			AchieveRewardType reward_type = myAchieve->getRewardType(achieve_list[i]);
			int reward_value = myAchieve->getRewardValue(achieve_list[i]);
			
			if(reward_type == kAchieveRewardType_ruby)
			{
				keep_take_ruby += reward_value;
				ruby_stats_value += CCString::createWithFormat(" | %d", achieve_list[i])->getCString();
			}
			else if(reward_type == kAchieveRewardType_gold)
			{
				keep_take_gold += reward_value;
				gold_stats_value += CCString::createWithFormat(" | %d", achieve_list[i])->getCString();
			}
			else if(reward_type == kAchieveRewardType_package)
			{
				for(int i=0;i<reward_value;i++)
				{
					AchieveRewardType t_type = myAchieve->getRewardTypeForIndex(achieve_list[keep_tag], i);
					int t_value = myAchieve->getRewardValueForIndex(achieve_list[keep_tag], i);
					
					if(t_type == kAchieveRewardType_ruby)
					{
						keep_take_ruby += t_value;
						ruby_stats_value += CCString::createWithFormat(" | %d", achieve_list[i])->getCString();
					}
					else if(t_type == kAchieveRewardType_gold)
					{
						keep_take_gold += t_value;
						gold_stats_value += CCString::createWithFormat(" | %d", achieve_list[i])->getCString();
					}
				}
			}
			
			myAchieve->changeComplete(achieve_list[i]);
		}
	}
	
	if(keep_take_ruby > 0)
		mySGD->addChangeGoods(kGoodsType_ruby, keep_take_ruby, "업적", ruby_stats_value);
	if(keep_take_gold > 0)
		mySGD->addChangeGoods(kGoodsType_gold, keep_take_gold, "업적", gold_stats_value);
	
	if(keep_take_ruby > 0 || keep_take_gold > 0)
	{
		loading_layer = LoadingLayer::create();
		addChild(loading_layer, kAchievePopupZorder_popup);
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultAllTakeSaveUserData));
		mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
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
		
		addChild(KSTimer::create(0.1f, [=](){
			vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(nullptr);
			mySGD->changeGoodsTransaction(t_command_achieve, json_selector(this, AchievePopup::resultSaveUserData));
		}));
		
//		mySGD->clearChangeGoods();
		
//		for(int i=0;i<keep_value_list.size();i++)
//			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value_list[i], false);
//		myDSH->fFlush();
	}
}