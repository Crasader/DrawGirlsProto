#include "HatGacha.h"
#include <random>
const int kFakeItemTag = 0x43534;

class CCNodeFrames : public CCNode
{
protected:
	std::vector<CCNode*> m_nodes;
	int m_currentIndex;
	float m_delay;
	float m_timer;
public:
	void runAnimation(float delay)
	{
		m_timer = 0;
		m_delay = delay;
		for(auto i : m_nodes)
		{
			i->setVisible(false);
		}
		scheduleUpdate();
	}
	void update(float dt)
	{
		m_timer += dt;
		if(m_timer > m_delay)
		{
			m_nodes[m_currentIndex]->setVisible(false);
			
			m_currentIndex++;
			if(m_nodes.size() == m_currentIndex)
			{
				m_currentIndex = 0;
			}
			
			m_nodes[m_currentIndex]->setVisible(true);
			while(m_timer > m_delay)
			{
				m_timer -= m_delay;
			}
		}
	}
	void appendNode(CCNode* n)
	{
		m_nodes.push_back(n);
		addChild(n);
	}
	static CCNodeFrames* create()
	{
		CCNodeFrames* nf = new CCNodeFrames();
		nf->init();
		nf->autorelease();
		return nf;
	}
};
bool HatGachaSub::init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm)
{
	CCLayer::init();
	m_gachaMode = gsm;
	m_fakeRewards = rs;
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
																																						content->addChild(CCSprite::create(i.first->m_reward->m_spriteStr.c_str()));
																																						
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
																																									 std::vector<RewardSprite*> rewards;
																																									 for(auto i : m_rewards)
																																									 {
																																										 rewards.push_back(RewardSprite::create(i->m_kind, i->m_value, i->m_spriteStr, i->m_weight));
																																									 }
																																									
																																									 getParent()->addChild(HatGachaSub::create(m_callback, rewards, m_gachaMode),
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
																																														 RewardKind kind = i.first->m_reward->m_kind;
																																														 int selectedItemValue = i.first->m_reward->m_value;
																																														 switch(kind)
																																														 {
																																															 case RewardKind::kRuby:
																																																 mySGD->setStar(mySGD->getStar() + selectedItemValue);
																																																 myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
																																																										 {
																																																											 
																																																										 });
																																																 break;
																																															 case RewardKind::kGold:
																																																 mySGD->setGold(mySGD->getGold() + selectedItemValue);
																																																 myDSH->saveUserData({kSaveUserData_Key_gold}, [=](Json::Value v)
																																																										 {
																																																											 
																																																										 });
																																																 break;
																																															 case RewardKind::kSpecialAttack:
																																															 {
																																																 int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_attack);
																																																 myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
																																															 }
																																																 break;
																																															 case RewardKind::kDash:
																																															 {
																																																 int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_fast);
																																																 myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
																																															 }
																																																 break;
																																															 case RewardKind::kSlience:
																																															 {
																																																 int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_silence);
																																																 myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
																																															 }
																																																 break;
																																															 case RewardKind::kRentCard:
																																															 {
																																																 int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_rentCard);
																																																 myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
																																															 }
																																																 break;
																																															 case RewardKind::kSubMonsterOneKill:
																																															 {
																																																 int currentValue = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_subOneDie);
																																																 myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, ITEM_CODE::kIC_critical, currentValue + selectedItemValue);
																																															 }
																																																 break;
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
	
	ProbSelector ps;
	for(auto i : m_fakeRewards)
	{
		ps.pushProb(i->m_weight);
	}
	
	for(int i=0; i<8; i++)
	{
		int index = ps.getResult();
		RewardSprite* temp_rs = m_fakeRewards[index];
		RewardSprite* rs = RewardSprite::create(temp_rs->m_kind, temp_rs->m_value, temp_rs->m_spriteStr, temp_rs->m_weight);
		
		
		std::string valueStr = CCString::createWithFormat("%d", temp_rs->m_value)->getCString();
		valueStr = std::string("+") + KS::insert_separator(valueStr);
		CCLabelBMFont* value = CCLabelBMFont::create(valueStr.c_str(), "mb_white_font.fnt");
		rs->addChild(value);
		value->setPosition(ccp(rs->getContentSize().width, rs->getContentSize().height) / 2.f);
		
//		rs->setColor(ccc3(255, 0, 0));
		KS::setOpacity(rs, 0);
//		rs->setOpacity(0);
		addChild(rs, 20);
		m_rewards.push_back(rs);
		
		
		CCNodeFrames* nf = CCNodeFrames::create();
		
		std::vector<int> randomIndex;
		for(int i=0; i<m_fakeRewards.size(); i++)
		{
			randomIndex.push_back(i);
		}
		random_device rd;
		mt19937 rEngine(rd());
		
		random_shuffle(randomIndex.begin(), randomIndex.end(), [&rEngine](int n)
									 {
										 uniform_int_distribution<> distribution(0, n-1);
										 return distribution(rEngine);
									 });
		// 페이크 에니메이션 돌림.
		for(auto iter : randomIndex)
		{
			RewardSprite* temp_rs = m_fakeRewards[iter];
			RewardSprite* fakeItem = RewardSprite::create(temp_rs->m_kind, temp_rs->m_value, temp_rs->m_spriteStr, temp_rs->m_weight);
			
			
			std::string valueStr = CCString::createWithFormat("%d", temp_rs->m_value)->getCString();
			valueStr = std::string("+") + KS::insert_separator(valueStr);
			CCLabelBMFont* value = CCLabelBMFont::create(valueStr.c_str(), "mb_white_font.fnt");
			fakeItem->addChild(value);
			value->setPosition(ccp(rs->getContentSize().width, fakeItem->getContentSize().height) / 2.f);
			
			nf->appendNode(fakeItem);
		}
		nf->runAnimation(0.05f);
		nf->setTag(kFakeItemTag);
		rs->addChild(nf);
//		nf->setPosition(rs->getPosition());
		nf->setPosition(ccp(rs->getContentSize().width, rs->getContentSize().height) / 2.f);
	}
	
	
	// 모자와 똑같은 위치에 상품 넣음.
	
	
	
	{ // 상품 연결
		std::random_shuffle(m_rewards.begin(), m_rewards.end(), [=](int i)
												{
													return m_well512.GetValue(0, i-1);
												});
		for(int i=0; i<m_hats.size(); i++)
		{
			m_hats[i].first->m_reward = m_rewards[i];
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
												for(auto i : m_rewards)
												{
													KS::setOpacity(i, 255);
													i->getChildByTag(kFakeItemTag)->removeFromParent();
												}
												
												addChild(KSTimer::create(2.f, [=]()
												{
													m_state = SceneState::kCoveringHat;
												}));
												
												
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
	
	HatGachaSub* gs = HatGachaSub::create(av, {
		RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
		RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
		RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
		RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
		RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
		RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
		RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
		RewardSprite::create(RewardKind::kGold, 1000, "price_gold_img.png", 5)
	});
	
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


