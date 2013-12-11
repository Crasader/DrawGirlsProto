#include "HatGacha.h"


bool HatGachaSub::init(KSAlertView* av, std::function<void(void)> callback, GachaPurchaseStartMode gsm)
{
	CCLayer::init();
	m_gachaMode = gsm;
	setTouchEnabled(true);
	m_parent = av;
	m_callback = callback;
	//		setContentSize(av->getViewSize());
	m_internalMenu = CCMenuLambda::create();
	m_internalMenu->setPosition(ccp(0, 0));
	m_internalMenu->setTouchPriority(INT_MIN);
	m_internalMenu->setTouchEnabled(true);
	addChild(m_internalMenu);
	
	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setTouchPriority(INT_MIN);
	m_menu->setTouchEnabled(false);
	addChild(m_menu);
	m_disableMenu = CCMenuLambda::create();
	m_disableMenu->setPosition(ccp(0, 0));
	addChild(m_disableMenu);
	
	for(int i=0; i<360; i+=45)
	{
		// 모자 추가
		CCMenuItemImageLambda* hatTopOpen = CCMenuItemImageLambda::create("hat_open_01.png", "hat_open_01.png", nullptr);
		CCMenuItemImageLambda* hatBottomOpen = CCMenuItemImageLambda::create("hat_open_02.png", "hat_open_02.png", nullptr);
		CCMenuItemImageLambda* hatTopClose = CCMenuItemImageLambda::create("hat_close_01.png", "hat_close_01.png", nullptr);
		CCMenuItemImageLambda* hatBottomClose = CCMenuItemImageLambda::create("hat_close_02.png", "hat_close_02.png", nullptr);
		((CCSprite*)hatTopOpen->getNormalImage())->getTexture()->setAliasTexParameters();
		((CCSprite*)hatBottomOpen->getNormalImage())->getTexture()->setAliasTexParameters();
		((CCSprite*)hatTopClose->getNormalImage())->getTexture()->setAliasTexParameters();
		((CCSprite*)hatBottomClose->getNormalImage())->getTexture()->setAliasTexParameters();
		//			CCSprite* t;
		//			t->getTexture()->set
		
		
		CCMenuItemToggleWithTopHatLambda* hatBottom = CCMenuItemToggleWithTopHatLambda::createWithTarget
		(
		 [=](CCObject* _item)
		 {
			 m_menu->setTouchEnabled(false);
			 CCMenuItemToggleWithTopHatLambda* item = dynamic_cast<CCMenuItemToggleWithTopHatLambda*>(_item);
			 CCLog("%d", item->getSelectedIndex());
			 if(m_state == SceneState::kStopHat)
			 {
				 m_selectComment->removeFromParent();
				 item->m_hatTop->setSelectedIndex(0);
				 item->setSelectedIndex(0);
				 m_state = SceneState::kSelectedHatMoving;
				 m_selectedHat = item;
				 CCPoint centerPosition;
				 if(m_parent)
					 centerPosition = ccp(m_parent->getViewSize().width / 2.f, m_parent->getViewSize().height / 2.f) +
					 ccp(0, 50);
				 else
					 centerPosition = ccp(240, 160) + ccp(0, 30);
				 this->addChild(KSGradualValue<CCPoint>::create
												(m_selectedHat->getPosition(),
												 centerPosition,
												 0.8f,
												 [=](CCPoint pt)
												 {
													 m_selectedHat->setPosition(pt);
													 topFollowBottom(); // 모자 위가 모자 밑둥을 따라감
												 },
												 [=](CCPoint pt)
												 {
													 
													 addChild(KSTimer::create(1.f, [=]()
																										// 상품을 모자에 맞추고 모자를 열고 상품을 보여줌.
																										{
																											for(auto i : m_hats)
																											{
																												i.first->m_reward->setScale(i.first->getScale());
																												i.first->m_reward->setPosition(i.first->getPosition());
																												i.first->setSelectedIndex(1);
																												i.first->m_hatTop->setSelectedIndex(1);
																												i.first->m_reward->setVisible(true);
																												if(i.first == m_selectedHat)
																												{
																													//																									 KS::KSLog("kind % value %", (int)i.first->m_reward->m_kind, );
																													
																													m_state = SceneState::kShowReward1;
																													this->addChild(KSGradualValue<float>::create
																																				 (1.f, 2.f, 1.f,
																																					[=](float t)
																																					{
																																						i.first->m_reward->setScale(t);
																																					},
																																					[=](float t)
																																					{
																																						CCLog("end!!");
																																						
																																						m_state = SceneState::kShowReward2;
																																						
																																						/*
																																						 
																																						 */
																																						// 다시 할꺼냐 받을거냐 물어보는 팝업.
																																						// 가운데 정렬 예제
																																						KSAlertView* av = KSAlertView::create();
																																						av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
																																						
																																						av->setBorderScale(0.9f);
																																						av->setCenterY(150);
																																						CCNode* contentParent = CCNode::create();
																																						
																																						KSNode* content = new KSNode();
																																						content->init();
																																						content->autorelease();
																																						contentParent->addChild(content);
																																						av->setContentNode(contentParent);
																																						
																																						// 내용에 루비인지 골드인지 아템인지 표시
																																						if(i.first->m_reward->m_kind == RewardKind::kRuby)
																																						{
																																							content->addChild(CCSprite::create("price_ruby_img.png"));
																																						}
																																						else if(i.first->m_reward->m_kind == RewardKind::kGold)
																																						{
																																							content->addChild(CCSprite::create("price_gold_img.png"));
																																						}
																																						// 가격 표시
																																						content->addChild(CCLabelTTF::create
																																															(CCString::createWithFormat
																																															 ("+%d",
																																																i.first->m_reward->m_value)->getCString(), mySGD->getFont().c_str(), 25));
																																						
																																						if(m_gachaMode == kGachaPurchaseStartMode_select)
																																						{
																																							// 다시하기 버튼
																																							auto retryBtn = CCMenuItemImageLambda::create
																																							(
																																							 "gacha_replay.png",
																																							 "gacha_replay.png",
																																							 [=](CCObject* e){
																																								 // 다시 해야되는데, 커튼이 닫히는거부터... 시작함.
																																								 
																																								 if(mySGD->getGold() >= 500)
																																								 {
																																									 mySGD->setStar(mySGD->getStar() - 500);
																																									 myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
																																																			 {
																																																				 
																																																			 });
																																									 getParent()->addChild(HatGachaSub::create(m_callback, m_gachaMode),
																																																				 this->getZOrder());
																																									 this->removeFromParent();
																																								 }
																																								 else
																																								 {
																																									 CCLog("돈 없음");
																																								 }
																																							 }
																																							 );
																																							auto don = CCSprite::create("price_gold_img.png");
																																							don->setPosition(ccp(50, 20));
																																							don->addChild(CCLabelTTF::create("-500", mySGD->getFont().c_str(), 25));
																																							retryBtn->addChild(don);
																																							av->addButton(retryBtn);
																																						}
																																						
																																						
																																						// 확인 버튼. (그냥 받음)
																																						av->addButton(CCMenuItemImageLambda::create
																																													(
																																													 "gacha_ok.png",
																																													 "gacha_ok.png",
																																													 [=](CCObject* e)
																																													 {
																																														 if(i.first->m_reward->m_kind == RewardKind::kRuby)
																																														 {
																																															 mySGD->setStar(mySGD->getStar() + i.first->m_reward->m_value);
																																															 myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
																																																									 {
																																																										 
																																																									 });
																																															 

																																														 }
																																														 else if(i.first->m_reward->m_kind == RewardKind::kGold)
																																														 {
																																															 mySGD->setGold(mySGD->getGold() + i.first->m_reward->m_value);
																																															 myDSH->saveUserData({kSaveUserData_Key_gold}, [=](Json::Value v)
																																																									 {
																																																										 
																																																									 });
																																														 }
																																														 
																																														 if(m_parent)
																																														 {
																																															 m_parent->removeFromParent();
																																															 m_parent->m_customCloseFunction();
																																														 }
																																														 
																																														 if(m_callback != nullptr)
																																														 {
																																															 m_callback();
																																															 removeFromParent();
																																														 }
																																														 
																																													 }
																																													 ));
																																						
																																						
																																						//	con->setAnchorPoint(ccp(0, 0));
																																						
																																						
																																						
																																						
																																						av->setBorderScale(0.9f);
																																						//	av->setTitleStr("지금 열기");
																																						
																																						
																																						
																																						content->setPosition(ccp((av->getViewSize() / 2.f).width,
																																																		 -(av->getViewSize() / 2.f).height));
																																						content->alignItemsVerticallyWithPadding(30);
																																						addChild(av, 30);
																																						av->show();
																																						
																																					}));
																													CCSprite* hatBack = CCSprite::create("hat_back.png");
																													hatBack->setScale(2.f);
																													CCPoint centerPosition;
																													if(m_parent)
																														centerPosition = ccp(m_parent->getViewSize().width / 2.f, m_parent->getViewSize().height / 2.f) +
																														ccp(0, 50);
																													else
																														centerPosition = ccp(240, 160);
																													hatBack->setPosition(centerPosition);
																													this->addChild(hatBack, 0);
																													this->addChild(KSGradualValue<float>::create
																																				 (0, 180.f * 99999.f, 99999.f,
																																					[=](float t)
																																					{
																																						hatBack->setRotation(t);
																																					},
																																					[=](float t)
																																					{
																																						CCLog("qq");
																																					}));
																												}
																												topFollowBottom(); // 모자 위가 모자 밑둥을 따라감
																											}
																										}));
													 
													 
													 
													 
												 }));
			 }
			 
			 
			 //				 if(m_state == SceneState::kStopHat)
			 //				 {
			 //					 item->m_reward->setVisible(true);
			 //				 }
			 
			 CCLog("open!!");
		 },
		 hatBottomClose,
		 hatBottomOpen, 0);
		
		CCMenuItemToggleLambda* hatTop = CCMenuItemToggleLambda::createWithTarget
		(nullptr,
		 hatTopClose,
		 hatTopOpen, 0);
		
		m_menu->addChild(hatBottom);
		m_disableMenu->addChild(hatTop);
		////		m_menu
		hatBottom->setPosition(retOnTheta(i * M_PI / 180.f));
		hatBottom->m_hatTop = hatTop;
		hatTop->setAnchorPoint(ccp(0.5f, 0.0f));
		hatTop->setPosition(ccp(hatBottom->getPositionX(),
														hatBottom->getPositionY() + hatBottom->getContentSize().height / 2.f));
		m_hats.push_back(std::make_pair(hatBottom, i));
		
		
		
	}
	// 모자와 똑같은 위치에 상품 넣음.
	{ // gold 넣기
		for(int n=0; n<m_hats.size()-1; n++)
		{
			RewardSprite* rs = new RewardSprite();
			rs->initWithFile("hat_gold.png");
			rs->autorelease();
			
			//			rs->setPosition(retOnTheta(i * M_PI / 180.f));
			rs->m_kind = RewardKind::kGold;
			rs->m_value = (int)(m_well512.GetValue(1000, 2000) / 100) * 100;
			std::string valueStr = CCString::createWithFormat("%d", rs->m_value)->getCString();
			valueStr = std::string("+") + KS::insert_separator(valueStr);
			CCLabelBMFont* value = CCLabelBMFont::create(valueStr.c_str(), "mb_white_font.fnt");
			rs->addChild(value);
			value->setPosition(ccp(rs->getContentSize().width, rs->getContentSize().height) / 2.f);
			addChild(rs, 20);
			m_rewards.push_back(std::make_pair(rs, 0));
		}
	}
	{ // ruby 넣기
		RewardSprite* rs = new RewardSprite();
		rs->initWithFile("hat_ruby.png");
		rs->autorelease();
		
		
		
		
		//			rs->setPosition(retOnTheta(i * M_PI / 180.f));
		rs->m_kind = RewardKind::kRuby;
		rs->m_value = (m_well512.GetValue(1, 2));
		std::string valueStr = CCString::createWithFormat("%d", rs->m_value)->getCString();
		valueStr = std::string("+") + KS::insert_separator(valueStr);
		CCLabelBMFont* value = CCLabelBMFont::create(valueStr.c_str(), "mb_white_font.fnt");
		rs->addChild(value);
		value->setPosition(ccp(rs->getContentSize().width, rs->getContentSize().height) / 2.f);
		addChild(rs);
		m_rewards.push_back(std::make_pair(rs, 0));
		
		
		
	}
	{ // 상품 연결
		std::random_shuffle(m_rewards.begin(), m_rewards.end(), [=](int i)
												{
													return m_well512.GetValue(0, i-1);
												});
		for(int i=0; i<m_hats.size(); i++)
		{
			m_hats[i].first->m_reward = m_rewards[i].first;
		}
	}
	repositionHat();
	
	for(auto i : m_hats)
	{
		i.first->setSelectedIndex(1); // 다 열음
		i.first->m_hatTop->setSelectedIndex(1);
	}
	//		m_rewardFollowHat = false;
	CCMenuItemImageLambda* startBtn = CCMenuItemImageLambda::create("gacha_start.png", "gacha_start.png");
	startBtn->setPosition(ccp(240, 40));
	//		startBtn->setVisible(false);
	startBtn->setTarget([=](CCObject*)
											{
												//													m_state = SceneState::kRun;
												startBtn->setVisible(false);
												m_state = SceneState::kCoveringHat;
												
											});
	
	m_state = SceneState::kBeforeCoveringHat;
	m_internalMenu->addChild(startBtn);
	scheduleUpdate();
	return true;
}

HatGacha::HatGacha()
{
	
}
HatGacha::~HatGacha()
{
	CCLog("~hatgacha");
}
//void HatGacha::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
//}

//bool HatGacha::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool HatGacha::init(std::function<void(void)> closeCallback)
{
	CCLayer::init();
	KSAlertView* av = KSAlertView::create();
	
	HatGachaSub* gs = HatGachaSub::create(av);
	
	av->setContentNode(gs);
	av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
//	av->setContentBorder(CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0,0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	av->setBorderScale(0.9f);
	av->setButtonHeight(0);
	av->setCloseOnPress(false);
	//	av->setTitleStr("지금 열기");
	addChild(av, 1);
	
//	con2->alignItemsVerticallyWithPadding(30);
	av->show(closeCallback);
	av->getContainerScrollView()->setTouchEnabled(false);

	
	return true;
}


