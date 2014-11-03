//
//  AchievePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 4..
//
//

#include "FriendPopup.h"
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
#include "KsLocal.h"
#include "FormSetter.h"
#include "FlagSelector.h"
#include "StyledLabelTTF.h"
#include <boost/lexical_cast.hpp>
#include "SendMessageUtil.h"
#include "GDWebSprite.h"
#include "StoryLayer.h"

enum FriendPopupZorder{
	kFriendPopupZorder_gray = 1,
	kFriendPopupZorder_back,
	kFriendPopupZorder_table,
	kFriendPopupZorder_menu,
	kFriendPopupZorder_popup
};

void FriendPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool FriendPopup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	m_introduced = false;
	m_touchPriority = -190;
	startFormSetter(this);
	input_text1 = nullptr;
	m_voteInputText = nullptr;
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
	addChild(gray, kFriendPopupZorder_gray);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kFriendPopupZorder_back);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(getLocal(LK::kFriendListTitle), mySGD->getFont().c_str(), 15);
	title_label->disableOuterStroke();
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	m_front = main_inner;
	main_inner->setContentSize(CCSizeMake(424, 194));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f + 5);
	main_case->addChild(main_inner);
	
	
	
	tab_menu = CCMenuLambda::create();
	tab_menu->setPosition(CCPointZero);
	main_case->addChild(tab_menu, kFriendPopupZorder_menu);
	
	tab_menu->setTouchPriority(m_touchPriority);
	
	
	m_friendAddContainer = CCNode::create();
	main_case->addChild(m_friendAddContainer, kFriendPopupZorder_popup);
	m_friendManageContainer = CCNode::create();
	main_case->addChild(m_friendManageContainer, kFriendPopupZorder_popup);
	m_friendListContainer = CCNode::create();
	main_case->addChild(m_friendListContainer, kFriendPopupZorder_popup);
	m_friendVoteContainer = CCNode::create();
	main_case->addChild(m_friendVoteContainer, kFriendPopupZorder_popup);
	friend_table = NULL;
	//	m_scrollBar = NULL;
	setFriendTable();
	
	setListMenu();
	setAddMenu();
	setManageMenu();
	setVoteFriendMenu();
	
	list_menu->setEnabled(false);
	recent_code = FriendTabCode::kList;
	
	CommonButton* close_menu = CommonButton::createCloseButton(m_touchPriority);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
													{
														AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
														hidePopup();
													});
	main_case->addChild(close_menu, kFriendPopupZorder_menu);
	
		
	empty_ment = CCLabelTTF::create("", mySGD->getFont().c_str(), 15);
	empty_ment->setPosition(ccp(25+210,52+90));
	main_case->addChild(empty_ment, kFriendPopupZorder_menu);
	
	//	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	//	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	//	s_tip->setColor(ccGRAY);
	//
	//	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(FriendPopup::menuAction));
	//	tip_item->setTag(kAchievePopupMenuTag_tip);
	//
	//	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	//	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
	//	addChild(tip_menu, kAchievePopupZorder_menu);
	//
	//	tip_menu->setTouchPriority(-190);
	
	
	m_loadingCCBI = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	addChild(m_loadingCCBI, INT_MAX);
	m_loadingCCBI->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 0));
	m_loadingCCBI->setVisible(false);
	
	Json::Value param;
	param["memberID"] = myHSP->getMemberID();
	
	myHSP->command("getuserdata", param, this, [=](Json::Value v){
		if(v["result"]["code"] != GDSUCCESS)
		{
		
			m_listButtonCallback(0);
			return;
			
		}
		// 추천인이 있으면
		if(v["introducerID"].asInt64() != 0)
		{
			m_listButtonCallback(0);
		}
		else
		{
			m_voteFriendButtonCallback(0);
		}
	});
//	if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_achieve))
//	{
//		myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_achieve, true);
//		
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
//		CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//		ikaruga->setAnchorPoint(ccp(0,0));
//		ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//		scenario_node->addChild(ikaruga, 1);
//		
//		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
//		typing_box->setHide();
//		scenario_node->addChild(typing_box, 2);
//		
//		CCSprite* n_skip = CCSprite::create("kt_skip.png");
//		CCSprite* s_skip = CCSprite::create("kt_skip.png");
//		s_skip->setColor(ccGRAY);
//		
//		CCMenuLambda* skip_menu = CCMenuLambda::create();
//		skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150));
//		scenario_node->addChild(skip_menu, 3);
//		skip_menu->setTouchPriority(-19999);
//		skip_menu->setEnabled(false);
//		
//		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
//																																 {
////																																	 typing_box->setBackKeyEnabled(false);
//																																	 skip_menu->setEnabled(false);
//																																	 
//																																	 addChild(KSTimer::create(0.1f, [=]()
//																																														{
//																																															scenario_node->removeFromParent();
//																																														}));
//																																 });
//		skip_menu->addChild(skip_item);
//		
//		typing_box->showAnimation(0.3f);
//		
//		function<void()> end_func2 = [=]()
//		{
////			typing_box->setBackKeyEnabled(false);
//			skip_menu->setEnabled(false);
//			
//			addChild(KSTimer::create(0.1f, [=]()
//															 {
//																 scenario_node->removeFromParent();
//															 }));
//		};
//		
//		function<void()> end_func1 = [=]()
//		{
//			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial2), end_func2);
//		};
//		
//		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																													{
//																														ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																														skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
//																													}, [=](float t)
//																													{
//																														ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																														skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
//																														skip_menu->setEnabled(true);
//																														
//																														typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial1), end_func1);
//																														
////																														typing_box->setBackKeyFunc([=](){
////																															skip_menu->setEnabled(false);
////																															
////																															addChild(KSTimer::create(0.1f, [=]()
////																																					 {
////																																						 scenario_node->removeFromParent();
////																																					 }));
////																														});
////																														typing_box->setBackKeyEnabled(true);
////																														typing_box->setKeypadEnabled(true);
//																													}));
//	}
	
	if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_hellmode))
	{
		myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kUI_hellmode, true);
		StoryLayer::startStory(this, "menu_friend");
	}
	
	return true;
}

void FriendPopup::showPopup()
{
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, bind(&FriendPopup::endShowPopup, this));
}

void FriendPopup::endShowPopup()
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
	
//	setBackKeyFunc([=]()
//				   {
//					   hidePopup();
//				   });
//	setBackKeyEnabled(true);
//	setKeypadEnabled(true);
}

void FriendPopup::hidePopup()
{
//	setBackKeyEnabled(false);
	
	is_menu_enable = false;
	if(friend_table)
	{
		friend_table->setTouchEnabled(false);
	}
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){
		
	}, bind(&FriendPopup::endHidePopup, this));
}

void FriendPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void FriendPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void FriendPopup::menuAction(CCObject* pSender)
{
	
}


void FriendPopup::setFriendTable()
{
	if(friend_table)
	{
		friend_table->removeFromParent();
		friend_table = NULL;
	}
	CCSize table_size;
	switch(recent_code)
	{
		case FriendTabCode::kList:
			table_size = CCSizeMake(416, 165);
			break;
		case FriendTabCode::kAdd:
			table_size = CCSizeMake(416, 150);
			break;
		case FriendTabCode::kManage:
			table_size = CCSizeMake(416, 192);
			break;
		default:
			table_size = CCSizeMake(416, 192);
			break;
	}
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
	
	friend_table = CCTableView::create(this, table_size);
	friend_table->setAnchorPoint(CCPointZero);
	friend_table->setDirection(kCCScrollViewDirectionVertical);
	friend_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	friend_table->setPosition(table_position);
	
	friend_table->setDelegate(this);
	main_case->addChild(friend_table, kFriendPopupZorder_table);
	friend_table->setTouchPriority(m_touchPriority);
	
	friend_table->reloadData();
	
	//	CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
	//																										 CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
	//	m_scrollBar = ScrollBar::createScrollbar(friend_table, -18, NULL, scrollBar, -191);
	//	m_scrollBar->setDynamicScrollSize(true);
	//	m_scrollBar->setVisible(true);
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-189);
	t_suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	t_suction->setTouchEnabled(true);
	main_case->addChild(t_suction);
}



CCTableViewCell* FriendPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	Json::Value memberInfo;
	
	if(recent_code == FriendTabCode::kAdd)
	{
		memberInfo = m_votedFriendList[idx];
	}
	else if(recent_code == FriendTabCode::kList || recent_code == FriendTabCode::kManage)
	{
		memberInfo = m_friendList[idx];
	}
	auto userAttacher = [=](CCNode* cell_back, Json::Value memberInfo)
	{
		KSLabelTTF* nick = KSLabelTTF::create(memberInfo["nick"].asString().c_str(), mySGD->getFont().c_str(), 12.5f);
		cell_back->addChild(nick, 1);
		nick->setAnchorPoint(ccp(0.f, 0.5f));
		nick->setPosition(ccp(51.0, 21.5));

		nick->enableOuterStroke(ccc3(0, 0, 0), 1.f, 200, true);
		setFormSetter(nick);
		
		KSLabelTTF* lastConnection = KSLabelTTF::create(ccsf(getLocal(LK::kFriendLastConnection), GraphDogLib::dateFormat("m/d H:i", memberInfo["lastDate"].asLargestInt()).c_str()),
																										mySGD->getFont().c_str(), 10.f);
		lastConnection->setAnchorPoint(ccp(0.f, 0.5f));
		lastConnection->setColor(ccc3(50, 40, 150));
		lastConnection->setPosition(ccp(51.0, 8.0));
		cell_back->addChild(lastConnection, 1);
		setFormSetter(lastConnection);
		
		KSLabelTTF* currentStage = KSLabelTTF::create(ccsf(getLocal(LK::kFriendCurrentStage), memberInfo["highPiece"].asInt()),
																									mySGD->getFont().c_str(), 15.f);
		currentStage->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
		currentStage->setAnchorPoint(ccp(1.f, 0.5f));
		currentStage->enableOuterStroke(ccc3(0, 0, 0), 0.3f, 50, true);
		cell_back->addChild(currentStage, 1);
		
		setFormSetter(currentStage);
		currentStage->setPosition(ccp(238.0, 17.5f));

		
		CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(memberInfo["flag"].asString()).c_str());
		setFormSetter(selectedFlagSpr);
		cell_back->addChild(selectedFlagSpr, 1);
		selectedFlagSpr->setPosition(ccp(25.5, 17.0)); 			// dt (25.5, 17.0)
		
	};
	
	
	auto removeQ = [=](CCObject*){
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		//			memberID	string or int	memberID
		//			friendID	string or int	friendID
		//			nick	string	nick
		//			content	string	내용
		//			data	string or dict	data
		//			exchangeID	string or dict	exchangeID
		
		// 삭제할거냐 팝업 띄우고 삭제.
		
		ASPopupView* warningPopup = ASPopupView::createDimmed(m_touchPriority - 1);
		
		warningPopup->getDimmedSprite()->setVisible(false);
		addChild(warningPopup);
		
		auto back = CCSprite::create("popup_small_back.png");
		
		CommonAnimation::openPopup(warningPopup, back, warningPopup->getDimmedSprite(), nullptr, nullptr);
		
		auto front = CCScale9Sprite::create("common_grayblue.png",
																				CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		
		//	front->setVisible(false);
		warningPopup->setContainerNode(back);
		//	back->setContentSize(CCSizeMake(550 / 2.f, 506 / 2.f));
		//	back->setContentSize(CCSizeMake(200, 200));
		front->setContentSize(CCSizeMake(251, 68));
		front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0,9));
		
		
		back->addChild(front);
		
		//				back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-12));
		//				content_back->setPosition(ccp(0.0,-12)); 			// dt (0.0,-4.5)
		
		//				front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0, 15));
		setFormSetter(front);
		
		KSLabelTTF* titleLbl = KSLabelTTF::create(getLocal(LK::kWarningDesc), mySGD->getFont().c_str(), 12.f);
		//	titleLbl->setColor(ccc3(255, 170, 20));
		titleLbl->setAnchorPoint(ccp(0.5f,0.5f));
		titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));
		back->addChild(titleLbl);
		
		
		CommonButton* closeButton = CommonButton::createCloseButton(m_touchPriority - 2);
		closeButton->setFunction([=](CCObject*)
														 {
															 AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
															 CommonAnimation::closePopup(this, back, warningPopup->getDimmedSprite(), nullptr,
																													 [=]()
																													 {
																														 warningPopup->removeFromParent();
																													 });
														 });
		back->addChild(closeButton);
		closeButton->setPosition(ccp(back->getContentSize().width-25, back->getContentSize().height-22));
		
		StyledLabelTTF* content_label = StyledLabelTTF::create(ccsf(getLocal(LK::kFriendRemoveFriendQ),
																																memberInfo["nick"].asString().c_str()), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		content_label->setAnchorPoint(ccp(0.5f,0.5f));
		content_label->setPosition(ccpFromSize(back->getContentSize() / 2.f) + ccp(0, 10));
		setFormSetter(content_label);
		back->addChild(content_label);
		
		CommonButton* confirm = CommonButton::create("확인", 12.f, CCSizeMake(101, 44), CommonButtonAchievement, m_touchPriority - 2);
		back->addChild(confirm);
		confirm->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -50));
		setFormSetter(confirm);
		confirm->setFunction([=](CCObject*){
			
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			Json::Value param;
			Json::Value param2;
			param["memberID"] = myHSP->getMemberID();
			param["friendID"] = memberInfo["memberID"].asString();
			LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
			addChild(ll, INT_MAX);

			myHSP->command("removefriend", param, [=](Json::Value v){
				ll->removeFromParent();
				CommonAnimation::closePopup(this, back, warningPopup->getDimmedSprite(), nullptr,
																		[=]()
																		{
																			warningPopup->removeFromParent();
																		});
				
				
				if(v["result"]["code"] != GDSUCCESS)
				{
					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																							getLocal(LK::kFriendError), [=]()
																							{
																								
																							}));
					return;
				}
				
				// 성공 적으로 삭제했다.
				m_afterActionFunc(0);
				//							m_manageButtonCallback(0);
			});
		});
		
		
		
		
		
		CCLOG("remove friend");
	};
	if(recent_code == FriendTabCode::kAdd)
	{
		// m_votedFriendList 가지고 루프 돌면서 뿌림.
		/*
		 [
		 {
		 "memberID":88899626759652880,
		 "nick":"테스트다",
		 "lastDate":20140907143313,
		 "flag":"kr",
		 "highPiece":1,
		 "highScore":711652
		 },
		 {
		 "memberID":88899626759647900,
		 "nick":"랄랄라라",
		 "lastDate":20140911145125,
		 "flag":"kr",
		 "highPiece":1,
		 "highScore":567132
		 },
		 {
		 "memberID":1000798,
		 "nick":"dum798",
		 "lastDate":20140828143121,
		 "flag":"kr",
		 "highPiece":0,
		 "highScore":0
		 },
		 {
		 "memberID":88899626759652560,
		 "nick":"니코짱",
		 "lastDate":20140905172043,
		 "flag":"tw",
		 "highPiece":1,
		 "highScore":0
		 },
		 {
		 "memberID":1000769,
		 "nick":"dum769",
		 "lastDate":20140828143115,
		 "flag":"kr",
		 "highPiece":0,
		 "highScore":0
		 }
		 
		 ]
		 */
		
		std::string	cell_back_filename = "rank_normal2.png";
		
		CCScale9Sprite* cell_back;
		cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
		cell_back->setContentSize(CCSizeMake(401, 34));
		
		cell_back->setAnchorPoint(CCPointZero);
		cell_back->setPosition(ccp(7.0, 0.0)); 			// dt (4.0, -2.0)
		cell->addChild(cell_back);
		setFormSetter(cell_back);
		
		userAttacher(cell_back, memberInfo);
		
		
		if(std::find(m_sendList.begin(), m_sendList.end(), memberInfo["memberID"].asString()) == m_sendList.end() &&
			 std::find(m_sendList.begin(), m_sendList.end(), boost::lexical_cast<std::string>(myHSP->getMemberID())) == m_sendList.end() )
			 
		{
			CommonButton* addFriend = CommonButton::create(getLocal(LK::kFriendAddFriend), 13.f, CCSizeMake(100, 30),
																										 CCScale9Sprite::create("friend_bt.png"),
																										 m_touchPriority);
			cell_back->addChild(addFriend, 1);
			addFriend->setPosition(ccp(349.0, 17.0));
			setFormSetter(addFriend);
			addFriend->setFunction([=](CCObject*){
				//			memberID	string or int	memberID
				//			friendID	string or int	friendID
				//			nick	string	nick
				//			content	string	내용
				//			data	string or dict	data
				//			exchangeID	string or dict	exchangeID
				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
				Json::Value param;
				Json::Value param2;
				param2["friendreq"] = 1;
				param2["from"] = myHSP->getMemberID();
				param["memberID"] = myHSP->getMemberID();
				param["friendID"] = memberInfo["memberID"].asString();
				param["nick"] = memberInfo["nick"].asString();
				param["template"] = "msgRquestFriend";
				param["custom"] = Json::Value(Json::arrayValue);
				param["custom"][0] = myDSH->getStringForKey(kDSH_Key_nick);
				//param["content"] = myDSH->getStringForKey(kDSH_Key_nick) + "님이 친구 요청 보냈습니다.";
				param["data"] = param2;
				LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
				addChild(ll, INT_MAX);

				myHSP->command("sendmessage", param, [=](Json::Value v){
					ll->removeFromParent();
					if(v["result"]["code"] != GDSUCCESS)
					{
						addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																								getLocal(LK::kFriendError), [=]()
																								{
																									
																								}));
						return;
					}
					
					if(input_text1)
					{
						input_text1->setVisible(false);
					}
					m_sendList.push_back(memberInfo["memberID"].asString());
					// 성공 적으로 보냈다.
//					m_votedFriendList[idx]["reqexist"] = true;
					auto successPopup = ASPopupView::getCommonNoti(m_touchPriority - 2, "성공", "성공적으로 요청했습니다.", [=](){
						input_text1->setVisible(true);
						
						
					});
					successPopup->getDimmedSprite()->setVisible(false);
					addChild(successPopup);
					
					addFriend->setTitle(getLocal(LK::kFriendAddingFriend));
					addFriend->setEnabled(false);
				});
				CCLOG("add friend");
			});
		}
		else
		{
			CommonButton* addFriend = CommonButton::create(getLocal(LK::kFriendAddingFriend), 13.f, CCSizeMake(100, 30),
																										 CCScale9Sprite::create("friend_bt.png"),
																										 m_touchPriority);
			cell_back->addChild(addFriend, 1);
			addFriend->setPosition(ccp(349.0, 17.0));
			addFriend->setEnabled(false);
		}
		
	}
	else if(recent_code == FriendTabCode::kList)
	{
		
		std::string	cell_back_filename = "rank_normal2.png";
		
		CCScale9Sprite* cell_back;
		cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
		cell_back->setContentSize(CCSizeMake(401, 34));
		
		cell_back->setAnchorPoint(CCPointZero);
		cell_back->setPosition(ccp(7.0, 0.0)); 			// dt (4.0, -2.0)
		cell->addChild(cell_back);
		setFormSetter(cell_back);
		
		KS::KSLog("%", memberInfo);
		userAttacher(cell_back, memberInfo);
		
		int remainTime = getHeartSendingRemainTime(memberInfo["memberID"].asString(), mySGD->getHeartSendCoolTime());
		if(remainTime == 0 && memberInfo["memberID"].asInt64() != myHSP->getMemberID())
		{
			CommonButton* miniRemoveFriend = CommonButton::create( CCSprite::create("friend_delete.png"),
																														m_touchPriority);
			miniRemoveFriend->setTitleSize(13.f);
//			CommonButton* miniRemoveFriend = CommonButton::create(getLocal(LK::kFriendRemoveFriend), 13.f, CCSizeMake(100, 30),
//																														CCScale9Sprite::create("friend_delete.png"),
//																														m_touchPriority);
			cell_back->addChild(miniRemoveFriend, 1);
			miniRemoveFriend->setPosition(ccp(378.5, 17.0));
			setFormSetter(miniRemoveFriend);
			m_afterActionFunc = m_listButtonCallback;
			miniRemoveFriend->setFunction(removeQ);
			

			
			CommonButton* sendHeart = CommonButton::create(getLocal(LK::kFriendHeartSend), 13.f, CCSizeMake(100, 30),
																										 CCScale9Sprite::create("friend_bt.png"),
																										 m_touchPriority);
			CCSprite* rubySprite = CCSprite::create("shop_coin1.png");
			rubySprite->setPosition(ccp(26.0, 6.0));
			rubySprite->setScale(0.6);
			sendHeart->getBackgroundSprite()->addChild(rubySprite);
			setFormSetter(rubySprite);
			cell_back->addChild(sendHeart, 1);
			sendHeart->setPosition(ccp(310.5, 17.0));
			setFormSetter(sendHeart);
			sendHeart->setFunction([=](CCObject*){
				//			memberID	string or int	memberID
				//			friendID	string or int	friendID
				//			nick	string	nick
				//			content	string	내용
				//			data	string or dict	data
				//			exchangeID	string or dict	exchangeID
				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
				Json::Value param;
				Json::Value param2;
				//			param2["friendreq"] = 1;
				//			param2["from"] = myHSP->getMemberID();
				param["memberID"] = myHSP->getMemberID();
				param["friendID"] = memberInfo["memberID"].asString();
				param["nick"] = memberInfo["nick"].asString();
				param["exchangeID"] = "friendHeart";
				param["template"] = "msgSendHeart";
				param["custom"] = Json::Value(Json::arrayValue);
				param["custom"][0] = myDSH->getStringForKey(kDSH_Key_nick);
				
				//param["content"] = myDSH->getStringForKey(kDSH_Key_nick) + "님이 하트를 보냈습니다.";
				//			param["data"] = param2;
				LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
				addChild(ll, INT_MAX);

				myHSP->command("sendmessage", param, [=](Json::Value v){
					ll->removeFromParent();
					if(v["result"]["code"] != GDSUCCESS)
					{
						addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																								getLocal(LK::kFriendError), [=]()
																								{
																									
																								}));
						return;
					}
					
					if(input_text1)
					{
						input_text1->setVisible(false);
					}
					m_sendList.push_back(memberInfo["memberID"].asString());
					// 성공 적으로 보냈다.
					auto successPopup = ASPopupView::getCommonNoti(m_touchPriority - 2, "성공", "성공적으로 하트를 보냈습니다.", [=](){
						if(input_text1)
						{
							input_text1->setVisible(true);
						}
						
					});
					addChild(successPopup);
					setHeartSendTime(memberInfo["memberID"].asString());
					sendHeart->setVisible(false);
					
					CCScale9Sprite* remainTimeBack = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
					remainTimeBack->setContentSize(CCSizeMake(91.0, 25.5));
					remainTimeBack->setPosition(ccp(310.5f, 18.0));
					cell_back->addChild(remainTimeBack);
					setFormSetter(remainTimeBack);
					
//					std::string remainTimeStr = getRemainTimeMsg(mySGD->getHeartSendCoolTime());
					int d, h, i, s;
					timeSpliter(mySGD->getHeartSendCoolTime(), 0, 0, &d, &h, &i, &s);
					h += d * 24;
					KSLabelTTF* remainTimeFnt = KSLabelTTF::create(ccsf("%02d:%02d:%02d", h, i, s), mySGD->getFont().c_str(), 13.f);
					remainTimeBack->addChild(remainTimeFnt);
					remainTimeFnt->setPosition(ccpFromSize(remainTimeBack->getContentSize()) / 2.f + ccp(0, -2));
					
				});
				
				
								
				
				CCLOG("add friend");
			});
		}
		else // 남은 시간이 있다면.
		{
			CCScale9Sprite* remainTimeBack = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
			remainTimeBack->setContentSize(CCSizeMake(91.0, 25.5));
			remainTimeBack->setPosition(ccp(310.5f, 18.0));
			cell_back->addChild(remainTimeBack);
			setFormSetter(remainTimeBack);
			
			std::string remainTimeStr = getRemainTimeMsg(remainTime);
			int h, i, s;
			timeSpliter(remainTime, 0, 0, 0, &h, &i, &s);
			KSLabelTTF* remainTimeFnt = KSLabelTTF::create(ccsf("%02d:%02d:%02d", h, i, s), mySGD->getFont().c_str(), 13.f);
			remainTimeBack->addChild(remainTimeFnt);
			remainTimeFnt->setPosition(ccpFromSize(remainTimeBack->getContentSize()) / 2.f + ccp(0, -2));
			
			CommonButton* miniRemoveFriend = CommonButton::create( CCSprite::create("friend_delete.png"),
																														m_touchPriority);
			miniRemoveFriend->setTitleSize(13.f);
			//			CommonButton* miniRemoveFriend = CommonButton::create(getLocal(LK::kFriendRemoveFriend), 13.f, CCSizeMake(100, 30),
			//																														CCScale9Sprite::create("friend_delete.png"),
			//																														m_touchPriority);
			cell_back->addChild(miniRemoveFriend, 1);
			miniRemoveFriend->setPosition(ccp(378.5, 17.0));
			setFormSetter(miniRemoveFriend);
			m_afterActionFunc = m_listButtonCallback;
			miniRemoveFriend->setFunction(removeQ);
		}

//		CommonButton* addFriend = CommonButton::create(getLocal(LK::kFriendAddFriend), 13.f, CCSizeMake(100, 30),
//																									 CCScale9Sprite::create("subbutton_purple3.png" , CCRectMake(0,0, 100, 39), CCRectMake(57, 22, 2, 2)),
//																									 -190);
//		cell_back->addChild(addFriend, 1);
//		addFriend->setPosition(ccp(349.0, 20.0));
//		setFormSetter(addFriend);
//		addFriend->setFunction([=](CCObject*){
//			
//		});
	}
	else if(recent_code == FriendTabCode::kManage)
	{
		{
			
			std::string	cell_back_filename = "rank_normal2.png";
			
			CCScale9Sprite* cell_back;
			cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			cell_back->setContentSize(CCSizeMake(401, 34));
			
			cell_back->setAnchorPoint(CCPointZero);
			cell_back->setPosition(ccp(7.0, 0.0)); 			// dt (4.0, -2.0)
			cell->addChild(cell_back);
			setFormSetter(cell_back);
			
			userAttacher(cell_back, memberInfo);
			
			if(memberInfo["memberID"].asString() != boost::lexical_cast<std::string>(myHSP->getMemberID()))
			{
				CommonButton* removeFriend = CommonButton::create(getLocal(LK::kFriendRemoveFriend), 13.f, CCSizeMake(100, 30),
																													CCScale9Sprite::create("friend_bt.png"),
																													m_touchPriority);
				cell_back->addChild(removeFriend, 1);
				removeFriend->setPosition(ccp(349.0, 17.0));
				setFormSetter(removeFriend);
				m_afterActionFunc = m_manageButtonCallback;
				removeFriend->setFunction(removeQ);
			}
			
			//		CommonButton* addFriend = CommonButton::create(getLocal(LK::kFriendAddFriend), 13.f, CCSizeMake(100, 30),
			//																									 CCScale9Sprite::create("subbutton_purple3.png" , CCRectMake(0,0, 100, 39), CCRectMake(57, 22, 2, 2)),
			//																									 -190);
			//		cell_back->addChild(addFriend, 1);
			//		addFriend->setPosition(ccp(349.0, 20.0));
			//		setFormSetter(addFriend);
			//		addFriend->setFunction([=](CCObject*){
			//			
			//		});
		}

	}
	return cell;
}

void FriendPopup::scrollViewDidScroll( CCScrollView* view )
{
	//	if(m_scrollBar)
	//	{
	//		m_scrollBar->setBarRefresh();
	//	}
}

void FriendPopup::scrollViewDidZoom( CCScrollView* view ){}

void FriendPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	//		CCLOG("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize FriendPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(401, 37);
}

unsigned int FriendPopup::numberOfCellsInTableView( CCTableView *table )
{
	if(recent_code == FriendTabCode::kAdd)
	{
		return m_votedFriendList.size();
	}
	else if(recent_code == FriendTabCode::kList)
	{
		return m_friendList.size();
	}
	else if(recent_code == FriendTabCode::kManage)
	{
		return m_friendList.size();
	}
	else
	{
		return 0;
	}
}

bool FriendPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLOG("touch swallow!!!");
	
	return true;
}
void FriendPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void FriendPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void FriendPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void FriendPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}

void FriendPopup::setListMenu()
{
	if(!list_menu)
	{
		CCSprite* n_all_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_all_label = KSLabelTTF::create(getLocal(LK::kFriendListMenu), mySGD->getFont().c_str(), 12.5f);
		n_all_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		n_all_label->setPosition(ccpFromSize(n_all_img->getContentSize()/2.f) + ccp(0,2));
		n_all_img->addChild(n_all_label);
		
		CCSprite* s_all_img = CCSprite::create("tabbutton_down.png");
		s_all_img->setColor(ccGRAY);
		KSLabelTTF* s_all_label = KSLabelTTF::create(getLocal(LK::kFriendListMenu), mySGD->getFont().c_str(), 12.5f);
		s_all_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		s_all_label->setColor(ccGRAY);
		s_all_label->setPosition(ccpFromSize(s_all_img->getContentSize()/2.f) + ccp(0,2));
		s_all_img->addChild(s_all_label);
		
		CCSprite* d_all_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_all_label = KSLabelTTF::create(getLocal(LK::kFriendListMenu), mySGD->getFont().c_str(), 12.5f);
		d_all_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_all_label->setPosition(ccpFromSize(d_all_img->getContentSize()/2.f) + ccp(0,2));
		d_all_img->addChild(d_all_label);
		
		
		m_listButtonCallback = [=](CCObject* sound)
		{
			m_friendListContainer->removeAllChildren();
			m_friendAddContainer->removeAllChildren();
			m_friendManageContainer->removeAllChildren();
			m_friendVoteContainer->removeAllChildren();
			if(sound)
			{
				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			}
			if(friend_table)
			{
				friend_table->removeFromParent();
				friend_table = NULL;
			}
			if(input_text1)
			{
				input_text1->removeFromParent();
				input_text1 = nullptr;
			}
			if(m_voteInputText)
			{
				m_voteInputText->removeFromParent();
				m_voteInputText = nullptr;
			}
			add_menu->setEnabled(true);
			list_menu->setEnabled(false);
			manage_menu->setEnabled(true);
			vote_friend_menu->setEnabled(true);
			Json::Value param;
			param["memberID"] = myHSP->getMemberID();
			
//			CCSprite* loadingCCBI = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
//			m_friendListContainer->addChild(loadingCCBI);
//			loadingCCBI->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 0));
			m_loadingCCBI->setVisible(true);

			myHSP->command("getfriendlist", param, this, [=](Json::Value v){
				if(list_menu->isEnabled() == true)
					return;
				m_loadingCCBI->setVisible(false);
//				loadingCCBI->removeFromParent();
				/*
				 {
				 "result":{
				 "code":1
				 },
				 "list":[
				 {
				 "memberID":88899626759652560,
				 "nick":"니코짱",
				 "lastDate":20140911202136,
				 "flag":"tw",
				 "highPiece":1,
				 "highScore":0
				 }
				 ],
				 "log":[
				 ]
				 }
				 */
				
				KS::KSLog("%", v);
				if(v["result"]["code"] != GDSUCCESS)
				{
					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																							getLocal(LK::kFriendError), [=]()
																							{
																								
																							}));
					return;
				}
				
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
				recent_code = FriendTabCode::kList;
				setFriendTable();
				m_friendList = v["list"];
				friend_table->reloadData();
				
				if(m_friendList.empty())
				{
					
					KSLabelTTF* nothing = KSLabelTTF::create(getLocal(LK::kFriendNothingExist), mySGD->getFont().c_str(), 13.f);
					m_friendListContainer->addChild(nothing, 100);
					nothing->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 0));
				}
				if(input_text1)
				{
					input_text1->removeFromParent();
					input_text1 = nullptr;
				}
				if(m_voteInputText)
				{
					m_voteInputText->removeFromParent();
					m_voteInputText = nullptr;
				}

				
				KSLabelTTF* leftMent = KSLabelTTF::create(ccsf("친구는 %d명까지 등록가능합니다.", mySGD->getGameFriendMax()), mySGD->getFont().c_str(), 11.f);
				leftMent->setAnchorPoint(ccp(0.f, 0.5f));
				m_friendListContainer->addChild(leftMent);
				setFormSetter(leftMent);
				leftMent->setPosition(ccp(38.0, 212.0)); 			//
				
				
				CCScale9Sprite* rightMentBack = CCScale9Sprite::create("common_gray.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
				rightMentBack->setContentSize(CCSizeMake(100, 30));
				m_friendListContainer->addChild(rightMentBack);
				setFormSetter(rightMentBack);
				
				KSLabelTTF* rightMent = KSLabelTTF::create(ccsf("내 친구 수 %d", m_friendList.size()), mySGD->getFont().c_str(), 11.f);
				rightMent->setAnchorPoint(ccp(0.f, 0.5f));
				m_friendListContainer->addChild(rightMent);
				setFormSetter(rightMent);
				rightMentBack->setPosition(ccp(390.0, 212.0));
				rightMentBack->setContentSize(CCSizeMake(100.0, 24.0));
				rightMent->setPosition(ccp(364.5, 212.0));
			});
			
			
			
		};
		list_menu = CCMenuItemSpriteLambda::create(n_all_img, s_all_img, d_all_img, m_listButtonCallback);
		list_menu->setPosition(ccp(244 + 75*(0),256.5f));
		tab_menu->addChild(list_menu);
	}
}
void FriendPopup::setAddMenu()
{
	if(!add_menu)
	{
		CCSprite* n_success_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_success_label = KSLabelTTF::create(getLocal(LK::kFriendAddMenu), mySGD->getFont().c_str(), 12.5f);
		n_success_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		n_success_label->setPosition(ccpFromSize(n_success_img->getContentSize()/2.f) + ccp(0,2));
		n_success_img->addChild(n_success_label);
		
		CCSprite* s_success_img = CCSprite::create("tabbutton_down.png");
		s_success_img->setColor(ccGRAY);
		KSLabelTTF* s_success_label = KSLabelTTF::create(getLocal(LK::kFriendAddMenu), mySGD->getFont().c_str(), 12.5f);
		s_success_label->setColor(ccGRAY);
		s_success_label->disableOuterStroke();
		s_success_label->setPosition(ccpFromSize(s_success_img->getContentSize()/2.f) + ccp(0,2));
		s_success_img->addChild(s_success_label);
		
		CCSprite* d_success_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_success_label = KSLabelTTF::create(getLocal(LK::kFriendAddMenu), mySGD->getFont().c_str(), 12.5f);
		d_success_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_success_label->setPosition(ccpFromSize(d_success_img->getContentSize()/2.f) + ccp(0,2));
		d_success_img->addChild(d_success_label);
		
		add_menu = CCMenuItemSpriteLambda::create(n_success_img, s_success_img, d_success_img, [=](CCObject*){
			add_menu->setEnabled(false);
			list_menu->setEnabled(true);
			manage_menu->setEnabled(true);
			vote_friend_menu->setEnabled(true);
			m_friendListContainer->removeAllChildren();
			m_friendAddContainer->removeAllChildren();
			m_friendManageContainer->removeAllChildren();
			m_friendVoteContainer->removeAllChildren();
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			if(friend_table)
			{
				friend_table->removeFromParent();
				friend_table = NULL;
			}
			if(input_text1)
			{
				input_text1->removeFromParent();
				input_text1 = nullptr;
			}
			if(m_voteInputText)
			{
				m_voteInputText->removeFromParent();
				m_voteInputText = nullptr;
			}
			Json::Value param;
			param["memberID"] = myHSP->getMemberID();
//			CCSprite* loadingCCBI = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
//			m_friendAddContainer->addChild(loadingCCBI);
//			loadingCCBI->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 0));
			m_loadingCCBI->setVisible(true);
			myHSP->command("getuserlistbyrandom", param, this, [=](Json::Value v){
				if(add_menu->isEnabled() == true)
					return;

				m_loadingCCBI->setVisible(false);
//				ll->removeFromParent();
//				loadingCCBI->removeFromParent();
				/*
				 {
				 "result":{
				 "code":1
				 },
				 "list":[
				 {
				 "memberID":88899626759652880,
				 "nick":"테스트다",
				 "lastDate":20140907143313,
				 "flag":"kr",
				 "highPiece":1,
				 "highScore":711652
				 },
				 {
				 "memberID":88899626759647900,
				 "nick":"랄랄라라",
				 "lastDate":20140911145125,
				 "flag":"kr",
				 "highPiece":1,
				 "highScore":567132
				 },
				 {
				 "memberID":1000798,
				 "nick":"dum798",
				 "lastDate":20140828143121,
				 "flag":"kr",
				 "highPiece":0,
				 "highScore":0
				 },
				 {
				 "memberID":88899626759652560,
				 "nick":"니코짱",
				 "lastDate":20140905172043,
				 "flag":"tw",
				 "highPiece":1,
				 "highScore":0
				 },
				 {
				 "memberID":1000769,
				 "nick":"dum769",
				 "lastDate":20140828143115,
				 "flag":"kr",
				 "highPiece":0,
				 "highScore":0
				 }
				 
				 ],
				 "log":[
				 ]
				 }
				 */
					
				if(v["result"]["code"] != GDSUCCESS)
				{
					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																							getLocal(LK::kFriendError), [=]()
																							{
																								
																							}));
					return;
				}
				
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
				recent_code = FriendTabCode::kAdd;
				setFriendTable();
				m_votedFriendList = v["list"];
				friend_table->reloadData();
				
				if(input_text1)
				{
					input_text1->removeFromParent();
					input_text1 = nullptr;
				}
				if(m_voteInputText)
				{
					m_voteInputText->removeFromParent();
					m_voteInputText = nullptr;
				}


				
				CCScale9Sprite* t_back1 = CCScale9Sprite::create("friend_search.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
//				t_back1->setOpacity(0);
				input_text1 = CCEditBox::create(CCSizeMake(241.f, 34), t_back1);
				input_text1->setPosition(ccp(63.0, 209.5));
				CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text1, 99999);
				CCEditBox* editbox = input_text1;
				editbox->setAnchorPoint(ccp(0.0f, 0.5f));
				editbox->setPlaceHolder(getLocal(LK::kFriendInputNickName));
				editbox->setReturnType(kKeyboardReturnTypeDone);
				editbox->setFont(mySGD->getFont().c_str(), 15);
				editbox->setInputMode(kEditBoxInputModeSingleLine);
				editbox->setInputFlag(cocos2d::extension::EditBoxInputFlag::kEditBoxInputFlagInitialCapsAllCharacters);
				editbox->setDelegate(this);
				//	m_container->addChild(input_text);
				editbox->setTouchPriority(m_touchPriority);
				editbox->setMaxLength(20);
//				editbox->setEnabled(false);
//				editbox->setVisible(false);

				setFormSetter(input_text1);
				
				CCScale9Sprite* searchButton = CCScale9Sprite::create("subbutton_purple3.png");
				CommonButton* searchBtn = CommonButton::create(CCSprite::create("subbutton_purple3.png"), m_touchPriority);
				searchBtn->setTitle(getLocal(LK::kFriendSearch));
				searchBtn->setTitleSize(13.f);
				searchBtn->setPosition(ccp(363.0, 203.0));
				m_friendAddContainer->addChild(searchBtn);
				setFormSetter(searchBtn);
				searchBtn->setFunction([=](CCObject*){
					// getuserdata 로 사람 찾아내어서 테이블에 띄움.
					
					AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
					if(input_text1)
					{
						input_text1->setVisible(false);
					}
					Json::Value param;
					if(editbox->getText() == std::string(""))
					{
						auto popup = ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti), getLocal(LK::kFriendNickInputPlz), [=](){
							if(input_text1)
							{
								input_text1->setVisible(true);
							}
							
						});
						popup->getDimmedSprite()->setVisible(false);
						addChild(popup);
						
						return;
					}
					else if( editbox->getText() == std::string(myDSH->getStringForKey(kDSH_Key_nick)))
					{
						auto popup = ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																										getLocal(LK::kFriendSelfNickname), [=](){
							if(input_text1)
							{
								input_text1->setVisible(true);
							}
							
						});
						popup->getDimmedSprite()->setVisible(false);
						addChild(popup);
						
						return;
					}
					param["nick"] = editbox->getText();
					param["isPublic"] = true;
					LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
					addChild(ll, INT_MAX); 
					myHSP->command("getuserdata", param, [=](Json::Value v){
						ll->removeFromParent();
						KS::KSLog("%", v);
						if(v["result"]["code"] != GDSUCCESS)
						{
							if(input_text1)
							{
								input_text1->setVisible(false);
							}
							auto popup = ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti), getLocal(LK::kFriendNotFountFriend), [=](){
								if(input_text1)
								{
									input_text1->setVisible(true);
								}
	
							});
							popup->getDimmedSprite()->setVisible(false);
							addChild(popup);
							return;
						}
						if(input_text1)
						{
							input_text1->setVisible(true);
						}
						m_votedFriendList = Json::Value(Json::arrayValue);
						Json::Value temp;
						temp = v;
//						temp["memberID"] = v["memberID"].asString();
//						temp["nick"] = v["nick"].asString();
//						temp["lastDate"] = v["lastDate"].asInt64();
//						temp["flag"] = v["flag"].asString();
//						temp["highpiece"] = v["]
						
						/*
						 "memberID":1000769,
						 "nick":"dum769",
						 "lastDate":20140828143115,
						 "flag":"kr",
						 "highPiece":0,
						 "highScore":0
						 */
						m_votedFriendList.append(temp);
						friend_table->reloadData();
						
						
					});
					
					CCLOG("SEARCH");
				});
			});
			
		});
		
		add_menu->setPosition(ccp(244 + 75*1,256.5f));
		tab_menu->addChild(add_menu);
	}
}

void FriendPopup::editBoxReturn(CCEditBox* editBox)
{
	
}
void FriendPopup::setManageMenu()
{
	if(!manage_menu)
	{
		CCSprite* n_ing_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_ing_label = KSLabelTTF::create(getLocal(LK::kFriendManageMenu), mySGD->getFont().c_str(), 12.5f);
		n_ing_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		n_ing_label->setPosition(ccpFromSize(n_ing_img->getContentSize()/2.f) + ccp(0,2));
		n_ing_img->addChild(n_ing_label);
		
		CCSprite* s_ing_img = CCSprite::create("tabbutton_down.png");
		s_ing_img->setColor(ccGRAY);
		KSLabelTTF* s_ing_label = KSLabelTTF::create(getLocal(LK::kFriendManageMenu), mySGD->getFont().c_str(), 12.5f);
		s_ing_label->setColor(ccGRAY);
		s_ing_label->disableOuterStroke();
		s_ing_label->setPosition(ccpFromSize(s_ing_img->getContentSize()/2.f) + ccp(0,2));
		s_ing_img->addChild(s_ing_label);
		
		CCSprite* d_ing_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_ing_label = KSLabelTTF::create(getLocal(LK::kFriendManageMenu), mySGD->getFont().c_str(), 12.5f);
		d_ing_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_ing_label->setPosition(ccpFromSize(d_ing_img->getContentSize()/2.f) + ccp(0,2));
		d_ing_img->addChild(d_ing_label);
		
		m_manageButtonCallback = [=](CCObject*){
			
			
			add_menu->setEnabled(true);
			list_menu->setEnabled(true);
			manage_menu->setEnabled(false);
			vote_friend_menu->setEnabled(true);
			m_friendListContainer->removeAllChildren();
			m_friendAddContainer->removeAllChildren();
			m_friendManageContainer->removeAllChildren();
			m_friendVoteContainer->removeAllChildren();
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			if(friend_table)
			{
				friend_table->removeFromParent();
				friend_table = NULL;
			}
			if(input_text1)
			{
				input_text1->removeFromParent();
				input_text1 = nullptr;
			}
			if(m_voteInputText)
			{
				m_voteInputText->removeFromParent();
				m_voteInputText = nullptr;
			}
			Json::Value param;
			param["memberID"] = myHSP->getMemberID();
			m_loadingCCBI->setVisible(true);

			myHSP->command("getfriendlist", param, this, [=](Json::Value v){
				if(manage_menu->isEnabled() == true)
					return;
				m_loadingCCBI->setVisible(false);
				/*
				 {
				 "result":{
				 "code":1
				 },
				 "list":[
				 {
				 "memberID":88899626759652560,
				 "nick":"니코짱",
				 "lastDate":20140911202136,
				 "flag":"tw",
				 "highPiece":1,
				 "highScore":0
				 }
				 ],
				 "log":[
				 ]
				 }
				 */
				
				KS::KSLog("%", v);
				if(v["result"]["code"] != GDSUCCESS)
				{
					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																							getLocal(LK::kFriendError), [=]()
																							{
																								
																							}));
					return;
				}
				
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
				recent_code = FriendTabCode::kManage;
				setFriendTable();
				m_friendList = v["list"];
				friend_table->reloadData();
				
				if(m_friendList.empty())
				{
					
					KSLabelTTF* nothing = KSLabelTTF::create(getLocal(LK::kFriendNothingExist), mySGD->getFont().c_str(), 13.f);
					m_friendListContainer->addChild(nothing, 100);
					nothing->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 0));
				}
	
				

			});
		};
		manage_menu = CCMenuItemSpriteLambda::create(n_ing_img, s_ing_img, d_ing_img, m_manageButtonCallback);
		
		manage_menu->setPosition(ccp(244 + 75*1,256.5f));
		tab_menu->addChild(manage_menu);
		manage_menu->setVisible(false);
		manage_menu->setEnabled(false);
	}
}

void FriendPopup::setVoteFriendMenu()
{
	if(!vote_friend_menu)
	{
		CCSprite* n_ing_img = CCSprite::create("tabbutton_down.png");
		KSLabelTTF* n_ing_label = KSLabelTTF::create(getLocal(LK::kFriendRecommend), mySGD->getFont().c_str(), 12.5f);
		n_ing_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		n_ing_label->setPosition(ccpFromSize(n_ing_img->getContentSize()/2.f) + ccp(0,2));
		n_ing_img->addChild(n_ing_label);
		
		CCSprite* s_ing_img = CCSprite::create("tabbutton_down.png");
		s_ing_img->setColor(ccGRAY);
		KSLabelTTF* s_ing_label = KSLabelTTF::create(getLocal(LK::kFriendRecommend), mySGD->getFont().c_str(), 12.5f);
		s_ing_label->setColor(ccGRAY);
		s_ing_label->disableOuterStroke();
		s_ing_label->setPosition(ccpFromSize(s_ing_img->getContentSize()/2.f) + ccp(0,2));
		s_ing_img->addChild(s_ing_label);
		
		CCSprite* d_ing_img = CCSprite::create("tabbutton_up.png");
		KSLabelTTF* d_ing_label = KSLabelTTF::create(getLocal(LK::kFriendRecommend), mySGD->getFont().c_str(), 12.5f);
		d_ing_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		d_ing_label->setPosition(ccpFromSize(d_ing_img->getContentSize()/2.f) + ccp(0,2));
		d_ing_img->addChild(d_ing_label);
		// 추천 두번 째 팝업
		m_voteFriendButtonCallbackSecond = [=](CCObject*){
			TRACE();
			add_menu->setEnabled(true);
			list_menu->setEnabled(true);
			manage_menu->setEnabled(true);
			vote_friend_menu->setEnabled(false);
			m_friendListContainer->removeAllChildren();
			m_friendAddContainer->removeAllChildren();
			m_friendManageContainer->removeAllChildren();
			m_friendVoteContainer->removeAllChildren();
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			TRACE();
			if(input_text1)
			{
				input_text1->removeFromParent();
				input_text1 = nullptr;
			}
			if(m_voteInputText)
			{
				m_voteInputText->removeFromParent();
				m_voteInputText = nullptr;
			}
			TRACE();
			if(friend_table)
			{
				friend_table->removeFromParent();
				friend_table = NULL;
			}
			TRACE();
			Json::Value param1;
			param1["memberID"] = myHSP->getMemberID();
			TRACE();
//			CCSprite* loadingCCBI = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
//			m_friendVoteContainer->addChild(loadingCCBI);
//			loadingCCBI->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 0));
			m_loadingCCBI->setVisible(true);
//			LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
//			addChild(ll, INT_MAX);



			TRACE();
			myHSP->command("getintroducereward", param1, this, [=](Json::Value v){
				if(vote_friend_menu->isEnabled() == true)
					return;
				m_loadingCCBI->setVisible(false);
				//				ll->removeFromParent();
				TRACE();
				KS::KSLog("%", v);
				if(v["result"]["code"].asInt() != GDSUCCESS)
					return;
				//				v["introduceCnt"] = 10;
				StyledLabelTTF* promotion1 = StyledLabelTTF::create(ccsf(getLocal(LK::kFriendVotePromotion1), v["introduceCnt"].asInt()), mySGD->getFont().c_str(),
																														20.f, 0, StyledAlignment::kCenterAlignment);
				TRACE();
				m_friendVoteContainer->addChild(promotion1);
				setFormSetter(promotion1);
				promotion1->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 82.5));
				
				StyledLabelTTF* promotion2 = StyledLabelTTF::create(ccsf(getLocal(LK::kFriendVotePromotion2), mySGD->getIntroduceStage()), mySGD->getFont().c_str(),
																														12.f, 0, StyledAlignment::kCenterAlignment);
				m_friendVoteContainer->addChild(promotion2);
				promotion2->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 59.0));
				setFormSetter(promotion2);
				
				TRACE();
				Json::Value reward = v["reward"];
				CCSprite* rewardImage = GDWebSprite::create(reward["image"].asString(), CCNode::create(), CCSizeMake(v["reward"]["size"]["w"].asInt(), v["reward"]["size"]["h"].asInt()));
				rewardImage->setAnchorPoint(ccp(0.5f, 0.5f));
				//				rewardImage->setContentSize(CCSizeMake(v["reward"]["size"]["w"].asInt(), v["reward"]["size"]["h"].asInt()));
				m_friendVoteContainer->addChild(rewardImage);
				rewardImage->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, -18));
				setFormSetter(rewardImage);
				
				TRACE();
				vector<int> rewardCounts;
				int stampCount = 0;
				for(Json::ValueIterator iter = v["reward"]["count"].begin(); iter != v["reward"]["count"].end(); ++iter)
				{
					if((*iter).asInt() <= v["introduceCnt"].asInt())
					{
						stampCount++;
					}
				}
				
				TRACE();
				int x = v["reward"]["position"]["x"].asInt();
				int y = v["reward"]["position"]["y"].asInt();
				int term = v["reward"]["position"]["term"].asInt();
				for(int i=0; i<stampCount; i++)
				{
					int newX = x + i * term;
					int newY = y;
					
					CCSprite* stampSprite = CCSprite::create("friend_gain.png");
					m_friendVoteContainer->addChild(stampSprite);
					stampSprite->setPosition(ccp(newX, newY));
					
				}
				
				
				TRACE();
				//				CCScale9Sprite* giftBack = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
				//				giftBack->setContentSize(CCSizeMake(411, 97));
				//				m_friendVoteContainer->addChild(giftBack);
				//				giftBack->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, -21));
				//				setFormSetter(giftBack);
				
				
				
				
				// {80, 119} {160, 119} {240, 119} {320, 119}, {400, 119} 가 상품 좌표
				
				TRACE();
				if(m_introduced == false)
				{
					CommonButton* voterInput = CommonButton::create(CCSprite::create("endless_bt_up.png"), m_touchPriority);
					voterInput->setPosition(ccp(303.0, 52.5));
					voterInput->setTitle(getLocal(LK::kFriendVoterInput));
					voterInput->setTitleSize(13.f);
					m_friendVoteContainer->addChild(voterInput);
					voterInput->setFunction([=](CCObject*){
						AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
						TRACE();
						m_voteFriendButtonCallback(0);  // 첫번 째 팝업으로 돌아감.
					});
					setFormSetter(voterInput);
				}
				
				CommonButton* kakaoTalkInvite = CommonButton::create(CCSprite::create("friend_kakao.png"), m_touchPriority);
				kakaoTalkInvite->setPosition(ccp(400.5, 53.5));
				//			kakaoTalkInvite->setTitle(getLocal(LK::kFriendVoterInput));
				//			kakaoTalkInvite->setTitleSize(13.f);
				m_friendVoteContainer->addChild(kakaoTalkInvite);
				kakaoTalkInvite->setFunction([=](CCObject*){
					
					Json::Value msgInfo = mySGD->getKakaoMsg();
					string msg = msgInfo["msg"].asString();
					GraphDogLib::ReplaceString(msg,"[p1]",myDSH->getStringForKey(kDSH_Key_nick).c_str());
					CCLOG("%s",msg.c_str());
					int ret = hspConnector::get()->sendKakaoMsg(msgInfo["title"].asString(),msg.c_str(),msgInfo["url"].asString());
					
					if(ret == 0) {
						auto ment = StyledLabelTTF::create(getLocal(LK::kFriendNeedKakao),
																							 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
						ment->setAnchorPoint(ccp(0.5f, 0.5f));
						ASPopupView* as = ASPopupView::getCommonNoti2(-9999999, "에러",
																													ment, nullptr, ccp(0, 0), true);
						addChild(as, 9999999);
					}
					

					
				});
				setFormSetter(kakaoTalkInvite);
				
			});
			
//			TRACE();
			
			
	
		};
		
		// 추천 첫번 째 팝업
		/////////////////////////////////////////////////////////여 기 가 시 작 //////////////////////
		m_voteFriendButtonCallback = [=](CCObject*){
			

			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			add_menu->setEnabled(true);
			list_menu->setEnabled(true);
			manage_menu->setEnabled(true);
			vote_friend_menu->setEnabled(false);
			m_friendListContainer->removeAllChildren();
			m_friendAddContainer->removeAllChildren();
			m_friendManageContainer->removeAllChildren();
			m_friendVoteContainer->removeAllChildren();
			if(input_text1)
			{
				input_text1->removeFromParent();
				input_text1 = nullptr;
			}
			if(m_voteInputText)
			{
				m_voteInputText->removeFromParent();
				m_voteInputText = nullptr;
			}
			if(friend_table)
			{
				friend_table->removeFromParent();
				friend_table = NULL;
			}
			

			TRACE();
			Json::Value param;
			param["memberID"] = myHSP->getMemberID();
//			LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
//			addChild(ll, INT_MAX);
//			LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
//			addChild(ll, INT_MAX);

			m_loadingCCBI->setVisible(true);
//			CCSprite* loadingCCBI = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
//			m_friendVoteContainer->addChild(loadingCCBI);
//			loadingCCBI->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 0));

			myHSP->command("getuserdata", param, this, [=](Json::Value v){
				if(vote_friend_menu->isEnabled() == true)
					return;
				m_loadingCCBI->setVisible(false);
//				ll->removeFromParent();
//				loadingCCBI->removeFromParent();
				TRACE();
				if(v["result"]["code"] != GDSUCCESS)
				{
					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																							getLocal(LK::kFriendError), [=]()
																							{
																								
																							}));
					return;
				}
				
				// 추천인이 있으면 두번 째 팝업으로 넘어감.
				if(v["introducerID"].asInt64() != 0)
				{
					m_introduced = true;
					m_voteFriendButtonCallbackSecond(0);
					
				}
				// 추천인이 없으면 진짜 첫번 째 팝업 코드 실행됨.
				else
				{
					KSLabelTTF* voteDesc1 = KSLabelTTF::create(getLocal(LK::kFriendVote1), mySGD->getFont().c_str(), 18.f);
					voteDesc1->setGradientColor(ccc4(255, 230, 10, 255), ccc4(255, 130, 7, 255), ccp(0,-1));
					m_friendVoteContainer->addChild(voteDesc1);
					voteDesc1->setPosition (ccpFromSize(main_case->getContentSize()) / 2 + ccp(0, 54));
					setFormSetter(voteDesc1);
					
					StyledLabelTTF* voteDesc2 = StyledLabelTTF::create(ccsf(getLocal(LK::kFriendVote2), mySGD->getAddGemReward()), mySGD->getFont().c_str(),
																														 13.f, 0, StyledAlignment::kCenterAlignment);
					setFormSetter(voteDesc2);
					m_friendVoteContainer->addChild(voteDesc2);
					voteDesc2->setPosition (ccpFromSize(main_case->getContentSize()) / 2 + ccp(0, 36));
					
					CCScale9Sprite* t_back1 = CCScale9Sprite::create("friend_search.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
					//				t_back1->setOpacity(0);
					m_voteInputText = CCEditBox::create(CCSizeMake(241.f, 34), t_back1);
					m_voteInputText->setPosition(ccp(63.0, 126));
					CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(m_voteInputText, 99999);
					CCEditBox* editbox = m_voteInputText;
					editbox->setAnchorPoint(ccp(0.0f, 0.5f));
					editbox->setPlaceHolder(getLocal(LK::kFriendInputNickName));
					editbox->setReturnType(kKeyboardReturnTypeDone);
					editbox->setFont(mySGD->getFont().c_str(), 15);
					editbox->setInputMode(kEditBoxInputModeSingleLine);
					editbox->setInputFlag(cocos2d::extension::EditBoxInputFlag::kEditBoxInputFlagInitialCapsAllCharacters);
					editbox->setDelegate(this);
					//	m_container->addChild(input_text);
					editbox->setTouchPriority(m_touchPriority);
					editbox->setMaxLength(20);
					//				editbox->setEnabled(false);
					//				editbox->setVisible(false);
					TRACE();
					setFormSetter(m_voteInputText);
					TRACE();
					CCScale9Sprite* searchButton = CCScale9Sprite::create("subbutton_purple3.png");
					CommonButton* searchBtn = CommonButton::create(CCSprite::create("subbutton_purple3.png"), m_touchPriority);
					searchBtn->setTitle(getLocal(LK::kFriendConfirm));
					searchBtn->setTitleSize(13.f);
					searchBtn->setPosition(ccp(363.0, 120));
					m_friendAddContainer->addChild(searchBtn);
					setFormSetter(searchBtn);
					
					CommonButton* skipButton = CommonButton::create(CCSprite::create("endless_bt_up.png"), m_touchPriority);
					skipButton->setTitleSize(13.f);
					skipButton->setTitle(getLocal(LK::kFriendInputSkip));
					skipButton->setPosition(ccp(406.5, 55.0));
					m_friendVoteContainer->addChild(skipButton);
					setFormSetter(skipButton);
					
					skipButton->setFunction([=](CCObject*){
						TRACE();
						AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
						TRACE();
						m_voteFriendButtonCallbackSecond(0);
						TRACE();
//						CCLog("m_voteInputText %x", m_voteInputText);
//						if(m_voteInputText)
//						{
//							m_voteInputText->setVisible(false);
//						}
					});
					
					// 첫번 째 팝업에서 검색을 놀렀을 때.
					searchBtn->setFunction([=](CCObject*)
																 {
																	 {
																		 AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		 Json::Value param;
																		 if(m_voteInputText)
																		 {
																			 m_voteInputText->setVisible(false);
																		 }
 
																		 if(m_voteInputText->getText() == std::string(""))
																		 {
																			 auto popup = ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti), getLocal(LK::kFriendNickInputPlz), [=](){
																				 if(m_voteInputText)
																				 {
																					 m_voteInputText->setVisible(true);
																				 }
																			 });
																			 popup->getDimmedSprite()->setVisible(false);
																			 addChild(popup);
																			 return;
																		 }
																		 param["memberID"] = myHSP->getSocialID();
																		 param["nick"] = m_voteInputText->getText();
																		 param["content"] = myLoc->getLocalForKey(LK::kMyLocalKey_introducerInputReward);
																		 LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
																		 addChild(ll, INT_MAX);

																		 myHSP->command("saveIntroducer", param, [=](Json::Value v2){
																			 ll->removeFromParent();
																			 if(v2["result"]["code"] == GDSUCCESS)
																			 {
                                                                                 mySGD->setIntroducerID(v2["introducerID"].asInt64());
																				 m_voteInputText->setVisible(false);
																				 addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																																						 getLocal(LK::kFriendSuccessfullyRecommend), [=]()
																																						 {
																																							 if(m_voteInputText)
																																							 {
																																								 m_voteInputText->setVisible(true);
																																							 }
																																						 }));
																				 m_voteFriendButtonCallbackSecond(0);
																				 
																				 
																			 }
																			 else if(v2["result"]["code"] == GDDONTFIND)
																			 {
																				 m_voteInputText->setVisible(false);
																				 addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																																						 getLocal(LK::kFriendDontFind), [=]()
																																						 {
																																							 if(m_voteInputText)
																																							 {
																																								 m_voteInputText->setVisible(true);
																																							 }

																																						 }));
																				 
																			 }
																			 //																	 else if(v2["result"]["code"] == GDALREADY)
																			 //																	 {
																			 //
																			 //																	 }
																			 else
																			 {
																				 m_voteInputText->setVisible(false);
																				 addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																																						 getLocal(LK::kFriendError), [=]()
																																						 {
																																							 if(m_voteInputText)
																																							 {
																																								 m_voteInputText->setVisible(true);
																																							 }
																																						 }));
																				 
																			 }
																		 });
																	 }
																	 
																 });
					
				}

			});
			
			
			
			
		};
		TRACE();
		vote_friend_menu = CCMenuItemSpriteLambda::create(n_ing_img, s_ing_img, d_ing_img, m_voteFriendButtonCallback);
		TRACE();
		vote_friend_menu->setPosition(ccp(244 + 75*2,256.5f));
		TRACE();
		tab_menu->addChild(vote_friend_menu);
		
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		vote_friend_menu->setVisible(mySGD->getIosMenuVisible() && graphdog->getAppVersionString() != mySGD->getIosHideVer());
#endif
		TRACE();
		/////////////////////////////// 탭버튼 추가 끝
		
		
	}
}
