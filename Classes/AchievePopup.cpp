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
#include "TypingBox.h"
#include "CCMenuLambda.h"
#include "GraySprite.h"
#include "ConvexGraph.h"
#include "StoryLayer.h"

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
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kAchievePopupZorder_back);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achievement), mySGD->getFont().c_str(), 15);
	title_label->disableOuterStroke();
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 194));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f + 5);
	main_case->addChild(main_inner);
	
	
	recent_code = kAchievePopupListCode_all;
	
	tab_menu = CCMenu::create();
	tab_menu->setPosition(CCPointZero);
	main_case->addChild(tab_menu, kAchievePopupZorder_menu);
	
	tab_menu->setTouchPriority(-190);
	
	
	
	CCSprite* n_allReward_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_allReward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allRewardGet), mySGD->getFont().c_str(), 12.5f);
	n_allReward_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_allReward_label->setPosition(ccpFromSize(n_allReward_img->getContentSize()/2.f) + ccp(0,-1));
	n_allReward_img->addChild(n_allReward_label);
	
	CCSprite* s_allReward_img = CCSprite::create("subbutton_pink.png");
	s_allReward_img->setColor(ccGRAY);
	KSLabelTTF* s_allReward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allRewardGet), mySGD->getFont().c_str(), 12.5f);
	s_allReward_label->setColor(ccGRAY);
	s_allReward_label->disableOuterStroke();
	s_allReward_label->setPosition(ccpFromSize(s_allReward_img->getContentSize()/2.f) + ccp(0,-1));
	s_allReward_img->addChild(s_allReward_label);
	
	CCSprite* d_allReward_img = GraySprite::create("subbutton_pink.png");
	((GraySprite*)d_allReward_img)->setGray(true);
	KSLabelTTF* d_allReward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allRewardGet), mySGD->getFont().c_str(), 12.5f);
	d_allReward_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_allReward_label->setPosition(ccpFromSize(d_allReward_img->getContentSize()/2.f) + ccp(0,-1));
	d_allReward_img->addChild(d_allReward_label);
	
	
	all_reward_menu = CCMenuItemSprite::create(n_allReward_img, s_allReward_img, d_allReward_img, this, menu_selector(AchievePopup::takeAllReward));
	all_reward_menu->setTag(kAchievePopupMenuTag_allReward);
	all_reward_menu->setPosition(ccp(395,16));
	tab_menu->addChild(all_reward_menu);
	
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
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kAchievePopupMenuTag_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kAchievePopupZorder_menu);
	
	achieve_table = NULL;
//	m_scrollBar = NULL;
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
	
	if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_achieve))
	{
		myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_achieve, true);
				StoryLayer::startStory(this, "menu_archivement");
	}
	
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
	checkLogRewardCount();
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
				empty_ment->setString(myLoc->getLocalForKey(LK::kMyLocalKey_nothingSuccessAchieve));
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
				empty_ment->setString(myLoc->getLocalForKey(LK::kMyLocalKey_nothingRewardAchieve));
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
					if(myAchieve->getCondition(AchievementCode(i)) > 0 && !isHaveAchieveGroup(another_list, myAchieve->getAchieveGroup((AchievementCode)i)))
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
					if(myAchieve->getCondition(AchievementCode(i)) > 0 && !isHaveAchieveGroup(another_list, myAchieve->getAchieveGroup((AchievementCode)i)))
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
			if(myAchieve->getCondition(AchievementCode(i)) > 0 && myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
			{
				if(!myAchieve->isCompleted((AchievementCode)i) &&
				   !myAchieve->isAchieve((AchievementCode)i))
					if(!isHaveAchieveGroup(achieve_list, myAchieve->getAchieveGroup((AchievementCode)i)))
						achieve_list.push_back(myAchieve->getAchieveGroup((AchievementCode)i));
			}
		}
		
		for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
		{
			if(myAchieve->getCondition(AchievementCode(i)) > 0 && myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
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
			if(myAchieve->getCondition(AchievementCode(i)) > 0 && myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
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
			if(myAchieve->getCondition(AchievementCode(i)) > 0 && myAchieve->getRecentCodeFromGroup((AchievementCode)i) == (AchievementCode)i)
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
	
	CCSize table_size = CCSizeMake(416, 192);
	CCPoint table_position = ccp(32, 35);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setOpacity(100);
//	temp_back->setPosition(table_position);
//	main_case->addChild(temp_back, kAchievePopupZorder_table);
	
//	CCScale9Sprite* scroll_back = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
//	scroll_back->setContentSize(CCSizeMake(7, table_size.height-20));
//	scroll_back->setPosition(ccpAdd(table_position, ccp(table_size.width-23, table_size.height/2.f)));
//	main_case->addChild(scroll_back, kAchievePopupZorder_table);
	
	achieve_table = CCTableView::create(this, table_size);
	achieve_table->setAnchorPoint(CCPointZero);
	achieve_table->setDirection(kCCScrollViewDirectionVertical);
	achieve_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	achieve_table->setPosition(table_position);
	
	achieve_table->setDelegate(this);
	main_case->addChild(achieve_table, kAchievePopupZorder_table);
	achieve_table->setTouchPriority(-190);
	
	achieve_table->reloadData();
	
//	CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
//																										 CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
//	m_scrollBar = ScrollBar::createScrollbar(achieve_table, -18, NULL, scrollBar, -191);
//	m_scrollBar->setDynamicScrollSize(true);
//	m_scrollBar->setVisible(true);
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-189);
	t_suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	t_suction->setTouchEnabled(true);
	main_case->addChild(t_suction);
	
	
	all_reward_menu->setEnabled(is_reward);
	
	checkLogRewardCount();
}

void AchievePopup::checkLogRewardCount()
{
	int reward_count = 0;
	
	for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
	{
		if(!myAchieve->isCompleted((AchievementCode)i) &&
		   myAchieve->isAchieve((AchievementCode)i))
		{
			reward_count++;
		}
	}
	
	for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
	{
		if(!myAchieve->isCompleted((AchievementCode)i) &&
		   myAchieve->isAchieve((AchievementCode)i))
		{
			reward_count++;
		}
	}
	
	CCLOG("achieve reward count : %d", reward_count);
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
		mySGD->addChangeGoods(CCString::createWithFormat("ac_%d", keep_tag)->getCString(), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "업적완료");
		
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
		
		mySGD->network_check_cnt = 0;
		
		setAchieveTable();
		
		loading_layer->removeFromParent();
		is_menu_enable = true;
	}
	else
	{
		CCLOG("reward get fail!!");
		
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultSaveUserData));
				mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
			{
				vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultSaveUserData));
				mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
			}));
		}
		
//		mySGD->clearChangeGoods();
		
//		myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value);
	}
}

CCTableViewCell* AchievePopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int state_value = -1; // complete
	
	AchievementCode recent_code;
	
	for(int i=0;i<achieve_list[idx].achieve_list.size();i++)
	{
		state_value = 0;
		
		recent_code = achieve_list[idx].achieve_list[i];
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
	
	if(achieve_list[idx].achieve_list.size() <= 0)
	{
		return cell;
	}
	
	string cell_back_filename;
	if(state_value == -1)
		cell_back_filename = "rank_normal1.png";
	else if(state_value == 1)
		cell_back_filename = "rank_normal1.png";
	else if(recent_code > kAchievementCode_hidden_base && recent_code < kAchievementCode_hidden_end)
	{
		state_value = 2;
		cell_back_filename = "achievement_cell_hidden.png";
	}
	else
		cell_back_filename = "rank_normal2.png";
	
	CCScale9Sprite* cell_back;
	if(state_value == 2)
	{
		cell_back = CCScale9Sprite::create(cell_back_filename.c_str());
	}
	else
	{
		cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
		cell_back->setContentSize(CCSizeMake(304, 41));
	}
	
	
	
	cell_back->setAnchorPoint(CCPointZero);
	cell_back->setPosition(ccp(3,0));
	cell->addChild(cell_back);
	
	if(state_value == 2)
	{
		KSLabelTTF* cell_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveHiddenTitle), mySGD->getFont().c_str(), 13);
		cell_title->setColor(ccc3(170, 225, 255));
		cell_title->setAnchorPoint(ccp(0.5f,0.5f));
		cell_title->setPosition(ccpFromSize(cell_back->getContentSize()/2.f) + ccp(0,10));
		cell_back->addChild(cell_title);
		
		KSLabelTTF* cell_content = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveHiddenContent), mySGD->getFont().c_str(), 9);
		cell_content->setColor(ccc3(195, 195, 195));
		cell_content->setAnchorPoint(ccp(0.5f,0.5f));
		cell_content->setPosition(ccpFromSize(cell_back->getContentSize()/2.f) + ccp(0,-10));
		cell_back->addChild(cell_content);
	}
	else
	{
		CCSprite* center_line = CCSprite::create("common_line.png");
		center_line->setScaleX(178/center_line->getContentSize().width);
		center_line->setPosition(ccpFromSize(cell_back->getContentSize()/2.f) - ccp(55,2));
		cell_back->addChild(center_line);
		
		if(achieve_list[idx].achieve_list.size() == 3)
		{
			if(myAchieve->isCompleted(achieve_list[idx].achieve_list[2]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
				t_crown->setPosition(ccp(cell_back->getContentSize().width-18, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-18, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
			
			if(myAchieve->isCompleted(achieve_list[idx].achieve_list[1]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_silver.png");
				t_crown->setPosition(ccp(cell_back->getContentSize().width-18-21, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-18-21, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
			
			if(myAchieve->isCompleted(achieve_list[idx].achieve_list[0]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_bronze.png");
				t_crown->setPosition(ccp(cell_back->getContentSize().width-18-42, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-18-42, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
		}
		else if(achieve_list[idx].achieve_list.size() == 2)
		{
			if(myAchieve->isCompleted(achieve_list[idx].achieve_list[1]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
				t_crown->setPosition(ccp(cell_back->getContentSize().width-18, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-18, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
			
			if(myAchieve->isCompleted(achieve_list[idx].achieve_list[0]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_silver.png");
				t_crown->setPosition(ccp(cell_back->getContentSize().width-18-21, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-18-21, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
		}
		else if(achieve_list[idx].achieve_list.size() == 1)
		{
			if(myAchieve->isCompleted(achieve_list[idx].achieve_list[0]))
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
				t_crown->setPosition(ccp(cell_back->getContentSize().width-18, cell_back->getContentSize().height-12));
				cell_back->addChild(t_crown);
			}
			else
			{
				CCSprite* t_back = CCSprite::create("achievement_crown_down.png");
				t_back->setPosition(ccp(cell_back->getContentSize().width-18, cell_back->getContentSize().height-12));
				cell_back->addChild(t_back);
			}
		}
		
		KSLabelTTF* cell_title = KSLabelTTF::create(CCString::createWithFormat("%s",
																			   myAchieve->getTitle(recent_code).c_str())->getCString(),
													mySGD->getFont().c_str(), 13);
		if(state_value == 1 || state_value == -1)
		{
			cell_title->setColor(ccc3(255, 170, 20));
			cell_title->disableOuterStroke();
		}
		else
		{
			cell_title->enableOuterStroke(ccBLACK, 0.3f, 50, true);
			
		}
		cell_title->setAnchorPoint(ccp(0,0.5));
		cell_title->setPosition(ccp(10,cell_back->getContentSize().height/2.f + 9));
		cell_back->addChild(cell_title);
		
		KSLabelTTF* cell_content = KSLabelTTF::create(myAchieve->getContent(recent_code).c_str(), mySGD->getFont().c_str(), 9);
		if(state_value != 1 && state_value != -1)
		{
			cell_content->disableOuterStroke();
			cell_content->setColor(ccc3(0, 80, 130));
		}
		else
		{
			cell_content->enableOuterStroke(ccBLACK, 0.3f, 50, true);
			
		}
		cell_content->setAnchorPoint(ccp(0,0.5));
		cell_content->setPosition(ccp(10,cell_back->getContentSize().height/2.f - 10));
		cell_back->addChild(cell_content);
		
		CCPoint img_position = ccp(170,24);
		if(myAchieve->isCompleted(recent_code))
		{
			ConvexGraph* graph_img = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(126, 13), ConvexGraphType::width);
			graph_img->setPosition(ccp(cell_back->getContentSize().width-60, 10));
			cell_back->addChild(graph_img);
			
			graph_img->setCover("loading_progress_front1.png", "loading_progress_mask.png");
			graph_img->setBack("loading_progress_back.png");
			
			graph_img->setScale(0.846f);
			graph_img->setPercentage(100.f);
			
			
			KSLabelTTF* progress_label = KSLabelTTF::create((KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getPresentationCondition(recent_code)/*myAchieve->getRecentValue(recent_code)*/)->getCString()) + "/" + KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getPresentationCondition(recent_code))->getCString())).c_str(), mySGD->getFont().c_str(), 8);
			progress_label->enableOuterStroke(ccBLACK, 0.5f, 255*0.6f, true);
			progress_label->setAnchorPoint(ccp(0,0.5f));
			progress_label->setPosition(graph_img->getPosition() + ccp(5-53.5f, 0));
			cell_back->addChild(progress_label);
			
			
			CCSprite* n_success = CCSprite::create("achievement_button_success.png");
			KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveSuccess2), mySGD->getFont().c_str(), 12.5f);
			n_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
			n_label->setPosition(ccpFromSize(n_success->getContentSize()/2.f) + ccp(13,0));
			n_success->addChild(n_label);
			n_success->setPosition(ccp(360,cell_back->getContentSize().height/2.f));
			cell_back->addChild(n_success);
			
			CCSprite* complete_img = CCSprite::create("achievement_success.png");
			complete_img->setPosition(ccp(cell_back->getContentSize().width+33, cell_back->getContentSize().height/2.f));
			cell_back->addChild(complete_img);
		}
		else if(myAchieve->isAchieve(recent_code))
		{
			ConvexGraph* graph_img = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(126, 13), ConvexGraphType::width);
			graph_img->setPosition(ccp(cell_back->getContentSize().width-60, 10));
			cell_back->addChild(graph_img);
			
			graph_img->setCover("loading_progress_front1.png", "loading_progress_mask.png");
			graph_img->setBack("loading_progress_back.png");
			
			graph_img->setScale(0.846f);
			graph_img->setPercentage(100.f);
			
			KSLabelTTF* progress_label = KSLabelTTF::create((KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getPresentationCondition(recent_code)/*myAchieve->getRecentValue(recent_code)*/)->getCString()) + "/" + KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getPresentationCondition(recent_code))->getCString())).c_str(), mySGD->getFont().c_str(), 8);
			progress_label->enableOuterStroke(ccBLACK, 0.5f, 255*0.6f, true);
			progress_label->setAnchorPoint(ccp(0,0.5f));
			progress_label->setPosition(graph_img->getPosition() + ccp(5-53.5f, 0));
			cell_back->addChild(progress_label);
			
			
			CCSprite* n_get = CCSprite::create("achievement_button_reward.png");
			KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_getReward), mySGD->getFont().c_str(), 12.5f);
			n_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
			n_label->setPosition(ccpFromSize(n_get->getContentSize()/2.f) + ccp(13,0));
			n_get->addChild(n_label);
			
			CCSprite* s_get = CCSprite::create("achievement_button_reward.png");
			s_get->setColor(ccGRAY);
			KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_getReward), mySGD->getFont().c_str(), 12.5f);
			s_label->setColor(ccGRAY);
			s_label->disableOuterStroke();
			s_label->setPosition(ccpFromSize(s_get->getContentSize()/2.f) + ccp(13,0));
			s_get->addChild(s_label);
			
			CCMenuItem* get_item = CCMenuItemSprite::create(n_get, s_get, this, menu_selector(AchievePopup::cellAction));
			get_item->setTag(recent_code);
			
			ScrollMenu* get_menu = ScrollMenu::create(get_item, NULL);
			get_menu->setPosition(ccp(360,cell_back->getContentSize().height/2.f));
			cell_back->addChild(get_menu);
			get_menu->setTouchPriority(-188);
			
			string reward_type_str;
			AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(recent_code);
			if(reward_type == kAchieveRewardType_ruby)
				reward_type_str = "price_ruby_img.png";
			else if(reward_type == kAchieveRewardType_gold)
				reward_type_str = "price_gold_img.png";
			else if(reward_type == kAchieveRewardType_package)
				reward_type_str = "price_package_img.png";
			
			CCSprite* reward_value_back = CCSprite::create("achievement_reward_label_back.png");
			reward_value_back->setPosition(ccp(cell_back->getContentSize().width+33, cell_back->getContentSize().height/2.f - 7));
			cell_back->addChild(reward_value_back);
			
			KSLabelTTF* reward_value = KSLabelTTF::create(CCString::createWithFormat("%d", myAchieve->getRewardValue(recent_code))->getCString(),
														  mySGD->getFont().c_str(), 10);
			reward_value->setPosition(ccpFromSize(reward_value_back->getContentSize()/2.f) + ccp(0,-1));
			reward_value_back->addChild(reward_value);
			
			CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
			reward_type_img->setPosition(ccp(cell_back->getContentSize().width+33, cell_back->getContentSize().height/2.f + 5));
			cell_back->addChild(reward_type_img);
		}
		else
		{
			float rate = 1.f*myAchieve->getRecentValue(recent_code)/myAchieve->getCondition(recent_code);
			
			ConvexGraph* graph_img = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(126, 13), ConvexGraphType::width);
			graph_img->setPosition(ccp(cell_back->getContentSize().width-60, 10));
			cell_back->addChild(graph_img);
			
			graph_img->setCover("loading_progress_front1.png", "loading_progress_mask.png");
			graph_img->setBack("loading_progress_back.png");
			
			graph_img->setScale(0.846f);
			graph_img->setPercentage(rate*100.f);
			
			KSLabelTTF* progress_label = KSLabelTTF::create((KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getRecentValue(recent_code))->getCString()) + "/" + KS::insert_separator(CCString::createWithFormat("%d", myAchieve->getPresentationCondition(recent_code))->getCString())).c_str(), mySGD->getFont().c_str(), 8);
			progress_label->enableOuterStroke(ccBLACK, 0.5f, 255*0.6f, true);
			progress_label->setAnchorPoint(ccp(0,0.5f));
			progress_label->setPosition(graph_img->getPosition() + ccp(5-53.5f, 0));
			cell_back->addChild(progress_label);
			
			
			CCSprite* n_success = CCSprite::create("achievement_button_ing.png");
			KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_reward), mySGD->getFont().c_str(), 12.5f);
			n_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
			n_label->setPosition(ccpFromSize(n_success->getContentSize()/2.f) + ccp(13,0));
			n_success->addChild(n_label);
			n_success->setPosition(ccp(360,cell_back->getContentSize().height/2.f));
			cell_back->addChild(n_success);
			
			
			string reward_type_str;
			AchieveRewardType reward_type = AchieveConditionReward::sharedInstance()->getRewardType(recent_code);
			if(reward_type == kAchieveRewardType_ruby)
				reward_type_str = "price_ruby_img.png";
			else if(reward_type == kAchieveRewardType_gold)
				reward_type_str = "price_gold_img.png";
			else if(reward_type == kAchieveRewardType_package)
				reward_type_str = "price_package_img.png";
			
			CCSprite* reward_value_back = CCSprite::create("achievement_reward_label_back.png");
			reward_value_back->setPosition(ccp(cell_back->getContentSize().width+33, cell_back->getContentSize().height/2.f - 7));
			cell_back->addChild(reward_value_back);
			
			KSLabelTTF* reward_value = KSLabelTTF::create(CCString::createWithFormat("%d", myAchieve->getRewardValue(recent_code))->getCString(),
														  mySGD->getFont().c_str(), 10);
			reward_value->setPosition(ccpFromSize(reward_value_back->getContentSize()/2.f) + ccp(0,-1));
			reward_value_back->addChild(reward_value);
			
			CCSprite* reward_type_img = CCSprite::create(reward_type_str.c_str());
			reward_type_img->setPosition(ccp(cell_back->getContentSize().width+33, cell_back->getContentSize().height/2.f + 5));
			cell_back->addChild(reward_type_img);
		}
	}
	
	return cell;
}

void AchievePopup::scrollViewDidScroll( CCScrollView* view )
{
//	if(m_scrollBar)
//	{
//		m_scrollBar->setBarRefresh();
//	}
}

void AchievePopup::scrollViewDidZoom( CCScrollView* view ){}

void AchievePopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	//		CCLOG("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize AchievePopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(416, 45);
}

unsigned int AchievePopup::numberOfCellsInTableView( CCTableView *table )
{
	return achieve_list.size();
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
		CCSprite* n_all_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_all_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allView), mySGD->getFont().c_str(), 12.5f);
		n_all_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		n_all_label->setPosition(ccpFromSize(n_all_img->getContentSize()/2.f) + ccp(0,2));
		n_all_img->addChild(n_all_label);
		
		CCSprite* s_all_img = CCSprite::create("tabbutton_down.png");
		s_all_img->setColor(ccGRAY);
		KSLabelTTF* s_all_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allView), mySGD->getFont().c_str(), 12.5f);
		s_all_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		s_all_label->setColor(ccGRAY);
		s_all_label->setPosition(ccpFromSize(s_all_img->getContentSize()/2.f) + ccp(0,2));
		s_all_img->addChild(s_all_label);
		
		CCSprite* d_all_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_all_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_allView), mySGD->getFont().c_str(), 12.5f);
		d_all_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_all_label->setPosition(ccpFromSize(d_all_img->getContentSize()/2.f) + ccp(0,2));
		d_all_img->addChild(d_all_label);
		
		
		all_menu = CCMenuItemSprite::create(n_all_img, s_all_img, d_all_img, this, menu_selector(AchievePopup::menuAction));
		all_menu->setTag(kAchievePopupMenuTag_all);
		all_menu->setPosition(ccp(169,256.5f));
		tab_menu->addChild(all_menu);
	}
	
	all_menu->setEnabled(recent_code != kAchievePopupListCode_all);
}
void AchievePopup::setSuccessMenu()
{
	if(!success_menu)
	{
		CCSprite* n_success_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_success_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveSuccess), mySGD->getFont().c_str(), 12.5f);
		n_success_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		n_success_label->setPosition(ccpFromSize(n_success_img->getContentSize()/2.f) + ccp(0,2));
		n_success_img->addChild(n_success_label);
		
		CCSprite* s_success_img = CCSprite::create("tabbutton_down.png");
		s_success_img->setColor(ccGRAY);
		KSLabelTTF* s_success_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveSuccess), mySGD->getFont().c_str(), 12.5f);
		s_success_label->setColor(ccGRAY);
		s_success_label->disableOuterStroke();
		s_success_label->setPosition(ccpFromSize(s_success_img->getContentSize()/2.f) + ccp(0,2));
		s_success_img->addChild(s_success_label);
		
		CCSprite* d_success_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_success_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveSuccess), mySGD->getFont().c_str(), 12.5f);
		d_success_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_success_label->setPosition(ccpFromSize(d_success_img->getContentSize()/2.f) + ccp(0,2));
		d_success_img->addChild(d_success_label);
		
		
		success_menu = CCMenuItemSprite::create(n_success_img, s_success_img, d_success_img, this, menu_selector(AchievePopup::menuAction));
		success_menu->setTag(kAchievePopupMenuTag_success);
		success_menu->setPosition(ccp(244,256.5f));
		tab_menu->addChild(success_menu);
	}
	
	success_menu->setEnabled(recent_code != kAchievePopupListCode_success);
}
void AchievePopup::setIngMenu()
{
	if(!ing_menu)
	{
		CCSprite* n_ing_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_ing_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveNotSuccess), mySGD->getFont().c_str(), 12.5f);
		n_ing_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		n_ing_label->setPosition(ccpFromSize(n_ing_img->getContentSize()/2.f) + ccp(0,2));
		n_ing_img->addChild(n_ing_label);
		
		CCSprite* s_ing_img = CCSprite::create("tabbutton_down.png");
		s_ing_img->setColor(ccGRAY);
		KSLabelTTF* s_ing_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveNotSuccess), mySGD->getFont().c_str(), 12.5f);
		s_ing_label->setColor(ccGRAY);
		s_ing_label->disableOuterStroke();
		s_ing_label->setPosition(ccpFromSize(s_ing_img->getContentSize()/2.f) + ccp(0,2));
		s_ing_img->addChild(s_ing_label);
		
		CCSprite* d_ing_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_ing_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveNotSuccess), mySGD->getFont().c_str(), 12.5f);
		d_ing_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_ing_label->setPosition(ccpFromSize(d_ing_img->getContentSize()/2.f) + ccp(0,2));
		d_ing_img->addChild(d_ing_label);
		
		
		ing_menu = CCMenuItemSprite::create(n_ing_img, s_ing_img, d_ing_img, this, menu_selector(AchievePopup::menuAction));
		ing_menu->setTag(kAchievePopupMenuTag_ing);
		ing_menu->setPosition(ccp(319,256.5f));
		tab_menu->addChild(ing_menu);
	}
	
	ing_menu->setEnabled(recent_code != kAchievePopupListCode_ing);
}
void AchievePopup::setRewardMenu()
{
	if(!reward_menu)
	{
		CCSprite* n_reward_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveReward), mySGD->getFont().c_str(), 12.5f);
		n_reward_label->disableOuterStroke();
		n_reward_label->setPosition(ccpFromSize(n_reward_img->getContentSize()/2.f) + ccp(0,2));
		n_reward_img->addChild(n_reward_label);
		
		CCSprite* s_reward_img = CCSprite::create("tabbutton_down.png");
		s_reward_img->setColor(ccGRAY);
		KSLabelTTF* s_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveReward), mySGD->getFont().c_str(), 12.5f);
		s_reward_label->setColor(ccGRAY);
		s_reward_label->disableOuterStroke();
		s_reward_label->setPosition(ccpFromSize(s_reward_img->getContentSize()/2.f) + ccp(0,2));
		s_reward_img->addChild(s_reward_label);
		
		CCSprite* d_reward_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_achieveReward), mySGD->getFont().c_str(), 12.5f);
		d_reward_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_reward_label->setPosition(ccpFromSize(d_reward_img->getContentSize()/2.f) + ccp(0,2));
		d_reward_img->addChild(d_reward_label);
		
		
		reward_menu = CCMenuItemSprite::create(n_reward_img, s_reward_img, d_reward_img, this, menu_selector(AchievePopup::menuAction));
		reward_menu->setTag(kAchievePopupMenuTag_reward);
		reward_menu->setPosition(ccp(394,256.5f));
		tab_menu->addChild(reward_menu);
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
			mySGD->addChangeGoods(CCString::createWithFormat("ac_%d", i)->getCString(), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "업적완료");
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
			mySGD->addChangeGoods(CCString::createWithFormat("ac_%d", i)->getCString(), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "업적완료");
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
		CCLOG("resultAllTakeSaveUserData");
		
		mySGD->network_check_cnt = 0;
		
		addChild(KSTimer::create(0.1f, [=]()
								 {
									 setAchieveTable();
									 
									 loading_layer->removeFromParent();
									 is_menu_enable = true;
								 }));
	}
	else
	{
		CCLOG("reward get fail!!");
		
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultAllTakeSaveUserData));
				mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 vector<CommandParam> t_command_achieve = myAchieve->updateAchieveHistoryVectorParam(json_selector(this, AchievePopup::resultAllTakeSaveUserData));
										 mySGD->changeGoodsTransaction(t_command_achieve, nullptr);
									 }));
		}
		
//		mySGD->clearChangeGoods();
		
//		for(int i=0;i<keep_value_list.size();i++)
//			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, keep_value_list[i], false);
//		myDSH->fFlush();
	}
}