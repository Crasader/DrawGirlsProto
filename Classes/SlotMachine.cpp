#include "SlotMachine.h"
#include <random>
#include <limits>
#include "GachaShowReward.h"


const int marginPerItem = 60;
void SlotMachineSub::update(float dt)
{
	if(m_state == SlotMachineState::kScrolling1)
	{
		rotationSlot({0,1,2}, -15);
	}
	else if(m_state == SlotMachineState::kScrolling2)
	{
		rotationSlot({1,2}, -15);
	}
	else if(m_state == SlotMachineState::kScrolling3)
	{
		rotationSlot({2}, -15);
	}
	else if(m_state == SlotMachineState::kShow)
	{
		//		rotationSlot({2}, -15);

		m_state = SlotMachineState::kQuestion;

		RewardKind kind = m_rewards[ m_alreadyDeterminantOrder ]->m_kind;
		int selectedItemValue = m_rewards[ m_alreadyDeterminantOrder ]->m_value;
		std::function<void(void)> replayFunction;
		CCNode* parentPointer = getParent();
		if(m_gachaMode == kGachaPurchaseStartMode_select){ // 선택으로 들어온 거라면 다시 하기가 가능함.
			replayFunction = [=]() {
				auto retryGame = [=](){

					std::vector<RewardSprite*> rewards;
					for(auto i : m_rewards) {
						rewards.push_back(RewardSprite::create(i->m_kind, i->m_value, i->m_spriteStr, i->m_weight));
					}
					parentPointer->addChild(SlotMachineSub::create(m_callback, rewards, m_gachaMode, m_gachaCategory),
																	this->getZOrder());
					this->removeFromParent();
				};
				 // 선택으로 들어온 거라면 다시 하기가 가능함.
				if(m_gachaCategory == GachaCategory::kRubyGacha)
				{
					if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getGachaRubyFeeRetry()) {
//						mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - mySGD->getGachaRubyFeeRetry());
//						myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v) {
//
//						});
						retryGame();
					}
					else {
						CCLOG("돈 없음");
					}
				}
				else if(m_gachaCategory == GachaCategory::kGoldGacha)
				{
					if(mySGD->getGoodsValue(kGoodsType_gold) >= mySGD->getGachaGoldFeeRetry()) {
//						mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) - mySGD->getGachaGoldFeeRetry());
//						myDSH->saveUserData({kSaveUserData_Key_gold}, [=](Json::Value v) {
//
//						});
						retryGame();
					}
					else {
						CCLOG("돈 없음");
					}
				}

			};
		}else{
			replayFunction = nullptr;
		}
		std::string againFileName;
		if(m_gachaCategory == GachaCategory::kRubyGacha)
		{
			againFileName = "Ruby";
		}
		else if(m_gachaCategory == GachaCategory::kGoldGacha)
		{
			againFileName = "Gold";
		}
		GachaShowReward* gachaShowReward = GachaShowReward::create(replayFunction,
			m_callback,
			CCSprite::create(m_rewards[ m_alreadyDeterminantOrder ]->m_spriteStr.c_str()),
			CCString::createWithFormat("%d", m_rewards[m_alreadyDeterminantOrder]->m_value)->getCString(),
			kind,
			selectedItemValue, againFileName, m_gachaCategory
				);
		addChild(gachaShowReward, 3);

	}
}

bool SlotMachineSub::init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm,
		GachaCategory gc)
{
	CCLayer::init();
	m_gachaCategory = gc;
	CCSprite* dimed = CCSprite::create();
	dimed->setTextureRect(CCRectMake(0, 0, 600, 400));
	dimed->setColor(ccc3(0, 0, 0));
	dimed->setOpacity(180);
	dimed->setPosition(ccp(240, 160));
	addChild(dimed, 0);

	CCSprite* commonBack = CCSprite::create("gacha_1_stage.png");
	commonBack->setPosition(ccp(240, 140));
	addChild(commonBack, 0);
	setTouchEnabled(true);
	m_rewards = rs;
	m_firstSlotStopFirst = m_firstSlotStopSecond = m_firstSlotStopThird = true;
	random_device rd;
	mt19937 rEngine(rd());
	default_random_engine rE;

	// 시작하기도 전에 미리 등수를 지정함.
	// 슬프다... ㅜ.ㅜ
	ProbSelector ps; // {1,1,1,3,3,3,10} 으로 입력됐다면,
	for(auto i : rs)
	{
		ps.pushProb(i->m_weight);
		addChild(i);
		i->setVisible(false);
	}
	m_alreadyDeterminantOrder = ps.getResult(); // 순서
	CCLOG("my order %d", m_alreadyDeterminantOrder);

	uniform_int_distribution<> dist(0, 2);
	switch(m_alreadyDeterminantOrder)
	{
		case 0: // 제일 조은거 3개가 나오도록 주작
			m_determinantSlotItems.push_back(0);
			m_determinantSlotItems.push_back(0);
			m_determinantSlotItems.push_back(0);
			break;
		case 1:
			{
				m_determinantSlotItems.push_back(0);
				m_determinantSlotItems.push_back(0);

				std::vector<int> enabledList = {1,2,3};
				m_determinantSlotItems.push_back(enabledList[dist(rEngine)]); // 0~3중에 0 제외
			}
			break;
		case 2:
			{
				m_determinantSlotItems.push_back(1);
				m_determinantSlotItems.push_back(1);
				m_determinantSlotItems.push_back(1);
			}
			break;
		case 3:
			{
				m_determinantSlotItems.push_back(1);
				m_determinantSlotItems.push_back(1);
				std::vector<int> enabledList = {0,2,3};
				m_determinantSlotItems.push_back(enabledList[dist(rEngine)]); // 0~3 중에 1 제외
			}
			break;
		case 4:
			{
				m_determinantSlotItems.push_back(2);
				m_determinantSlotItems.push_back(2);
				m_determinantSlotItems.push_back(2);
			}
			break;
		case 5:
			{
				m_determinantSlotItems.push_back(2);

				m_determinantSlotItems.push_back(2);
				std::vector<int> enabledList = {0,2,3};
				m_determinantSlotItems.push_back(enabledList[dist(rEngine)]); // 0~3 중에 2 제외한것.
			}
			break;
		case 6:
			{
				m_determinantSlotItems.push_back(3);
				m_determinantSlotItems.push_back(3);
				m_determinantSlotItems.push_back(3);
			}
			break;
		case 7:
			{
				uniform_int_distribution<> dist(0, 3);
				std::vector<int> enabledList = {0,1,2,3};
				enabledList.erase(enabledList.begin() + dist(rEngine));

				m_determinantSlotItems = enabledList;
			}
			break;
	}

	random_shuffle(m_determinantSlotItems.begin(), m_determinantSlotItems.end(),
			[&rEngine](int n)->int{
				uniform_int_distribution<> dist(0, n-1);
				return dist(rEngine);
			});
	//	m_determinantSlotItems[0] = 0;
	//	m_determinantSlotItems[1] = 1;
	//	m_determinantSlotItems[2] = 2;
	for(auto i : m_determinantSlotItems)
	{
		CCLOG("-- %d", i);
	}
	m_gachaMode = gsm;


	m_parent = av;
	m_callback = callback;
	//		setContentSize(av->getViewSize());

	CCSprite* back = CCSprite::create("gacha4_back.png");
	back->setPosition(ccp(240, 150));
	addChild(back);

	CCSprite* back2 = CCSprite::create("gacha4_back_2.png");
	back2->setPosition(ccp(240, 135));
	addChild(back2, 3);


	for(int slot=0; slot<3; slot++)
	{
		CCDrawNode* shape = CCDrawNode::create();

		CCPoint pts[4];
		pts[0] = ccp( 0, 3);
		pts[1] = ccp( 0, 114);
		pts[2] = ccp( 179 - 113, 114);
		pts[3] = ccp( 179 - 113, 3);

		shape->drawPolygon(pts, 4, ccc4f(1, 1, 1, 1), 0, ccc4f(1, 0, 0, 1));

		// add shape in stencil
		m_clipSlot[slot] = CCClippingNode::create();
		m_clipSlot[slot]->setAnchorPoint(ccp(0.5, 0.5));

		m_clipSlot[slot]->setStencil(shape);
		this->addChild(m_clipSlot[slot]);
		m_nodeOfItems[slot] = CCNode::create();
		m_clipSlot[slot]->addChild(m_nodeOfItems[slot]);

		int cnt = 0;
		std::vector<std::string> slotItems = {"gacha4_item1.png", "gacha4_item2.png", "gacha4_item3.png",
			"gacha4_item4.png"};


		for(auto i : slotItems)
		{
			uniform_int_distribution<> dist(-marginPerItem*3, 0);
			CCSprite* pSprite = CCSprite::create(i.c_str());
			//			pSprite->setScale(7.f);
			pSprite->setPosition(ccp(33, 0 + marginPerItem * (cnt - 1) ));
			m_nodeOfItems[slot]->addChild(pSprite);
			cnt++;
		}

		CCSprite* topGray = CCSprite::create("gacha4_shadow.png");
		topGray->setAnchorPoint(ccp(0.5f, 1.f));
		topGray->setPosition(ccp(33, 121));
		m_clipSlot[slot]->addChild(topGray);

		CCSprite* bottomGray = CCSprite::create("gacha4_shadow.png");
		bottomGray->setFlipY(true);
		bottomGray->setAnchorPoint(ccp(0.5f, 0.f));
		bottomGray->setPosition(ccp(33, -1));
		m_clipSlot[slot]->addChild(bottomGray);
	}
	m_clipSlot[0]->setPosition(ccp(138, 72));
	m_clipSlot[1]->setPosition(ccp(209, 72));
	m_clipSlot[2]->setPosition(ccp(279, 72));

	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setTouchPriority(INT_MIN);
	m_menu->setTouchEnabled(true);
	addChild(m_menu);

	CCSprite* s1 = CCSprite::create("gacha4_stop.png");
	CCSprite* s2 = CCSprite::create("gacha4_stop.png");
	s2->setColor(ccc3(166, 166, 166));
	CCMenuItemSpriteLambda* stopBtn = CCMenuItemSpriteLambda::create(s1, s2);

	stopBtn->setPosition(ccp(240, 40));
	//		startBtn->setVisible(false);
	stopBtn->setTarget([=](CCObject*)
			{
				if(m_state == SlotMachineState::kScrolling1)
	{
		m_state = SlotMachineState::kScrolling2;
		int currentSlot = 0;
		addChild(KSSchedule::create([=](float dt)
				{
					int prevSlotIndex = m_prevSlotIndex[currentSlot];
					auto indexs = this->rotationSlot({currentSlot}, -15);
					CCLOG("current Index : %d", indexs[currentSlot]);
					int ii = (indexs[currentSlot] + 1) % 4 ;

					if(prevSlotIndex != indexs[currentSlot] && m_determinantSlotItems[currentSlot] == ii && !m_firstSlotStopFirst)
		{
			addChild(KSGradualValue<float>::create
				(-15, 0, 0.7f,
				 [=](float t)
				 {
					 this->rotationSlot({currentSlot}, t);
				 }
				 ,
				 [=](float t)
				 {

				 }));
			return false;
		}
		m_firstSlotStopFirst = false;
		return true;
				}));
	}
				else if(m_state == SlotMachineState::kScrolling2)
				{
					m_state = SlotMachineState::kScrolling3;
					int currentSlot = 1;
					addChild(KSSchedule::create([=](float dt)
								{
									int prevSlotIndex = m_prevSlotIndex[currentSlot];
									auto indexs = this->rotationSlot({currentSlot}, -15);
									CCLOG("current Index : %d", indexs[currentSlot]);
									int ii = (indexs[currentSlot] + 1) % 4 ;

									if(prevSlotIndex != indexs[currentSlot] && m_determinantSlotItems[currentSlot] == ii && !m_firstSlotStopSecond)
					{
						addChild(KSGradualValue<float>::create
							(-15, 0, 0.7f,
							 [=](float t)
							 {
								 this->rotationSlot({currentSlot}, t);
							 }
							 ,
							 [=](float t)
							 {

							 }));
						return false;
					}
					m_firstSlotStopSecond = false;
					return true;
								}));
				}
				else if(m_state == SlotMachineState::kScrolling3)
				{
					m_state = SlotMachineState::kFinishing;
					int currentSlot = 2;
					addChild(KSSchedule::create([=](float dt)
								{
									int prevSlotIndex = m_prevSlotIndex[currentSlot];
									auto indexs = this->rotationSlot({currentSlot}, -15);
									CCLOG("current Index : %d", indexs[currentSlot]);
									int ii = (indexs[currentSlot] + 1) % 4 ;

									if(prevSlotIndex != indexs[currentSlot] && m_determinantSlotItems[currentSlot] == ii && !m_firstSlotStopThird)
					{
						addChild(KSGradualValue<float>::create
							(-15, 0, 0.7f,
							 [=](float t)
							 {
								 this->rotationSlot({currentSlot}, t);
							 }
							 ,
							 [=](float t)
							 {
								 m_state = SlotMachineState::kShow;
							 }));
						return false;
					}
					m_firstSlotStopThird = false;
					return true;
								}));
				}

			});
	m_menu->addChild(stopBtn);

	m_state = SlotMachineState::kScrolling1;
	scheduleUpdate();

	return true;
}

std::vector<int> SlotMachineSub::rotationSlot(const std::vector<int>& slots, float scrollSpeed)
{
	std::vector<int> nearestCenterPositionIndex = {numeric_limits<int>::max(), numeric_limits<int>::max(), numeric_limits<int>::max()};
	std::vector<int> nearestDistance = {numeric_limits<int>::max(), numeric_limits<int>::max(), numeric_limits<int>::max()};
	for(int slot=0; slot<3; slot++)
	{
		CCNode* minYNode;
		float minY = std::numeric_limits<float>::max();
		float topY = std::numeric_limits<float>::min();
		for(int i=0; i<m_nodeOfItems[slot]->getChildrenCount(); i++)
		{
			CCNode* child = dynamic_cast<CCNode*>(m_nodeOfItems[slot]->getChildren()->objectAtIndex(i));
			if(std::find(slots.begin(), slots.end(), slot) != slots.end()) // 움직일 슬롯은 움직임
				child->setPositionY(child->getPositionY() + scrollSpeed);

			if(child->getPositionY() < minY)
			{
				minY = child->getPositionY();
				minYNode = child;
			}
			if(child->getPositionY() > topY)
			{
				topY = child->getPositionY();
			}
			if(abs(child->getPositionY() - 66) < nearestDistance[slot])
			{
				nearestDistance[slot] = abs(child->getPositionY() - 66);
				nearestCenterPositionIndex[slot] = i;
				m_prevSlotIndex[slot] = i;
			}
		}

		if(minY < -marginPerItem)
		{
			minYNode->setPositionY(topY + marginPerItem);
		}
	}

	return nearestCenterPositionIndex;
}
//SlotMachine::SlotMachine()
//{

//}

//SlotMachine::~SlotMachine()
//{
//CCLOG("~hatgacha");
//}
////void HatGacha::registerWithTouchDispatcher()
////{
////	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
////}

////bool HatGacha::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
////{
////	CCTouch* touch = pTouch;
////
////	CCPoint location(ccp(0, 0));
////	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
////
////	return true;
////}

//bool SlotMachine::init(std::function<void(void)> closeCallback)
//{
//CCLayer::init();
//KSAlertView* av = KSAlertView::create();

//SlotMachineSub* gs = SlotMachineSub::create(av, {
//RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
//RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
//RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
//RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
//RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
//RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
//RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5),
//RewardSprite::create(RewardKind::kGold, 1000, "price_gold_img.png", 5)
//});

//av->setContentNode(gs);
//av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
////	av->setContentBorder(CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0,0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
//av->setBorderScale(0.9f);
//av->setButtonHeight(0);
//av->setCloseOnPress(false);
////	av->setTitleStr("지금 열기");
//addChild(av, 1);

////	con2->alignItemsVerticallyWithPadding(30);
//av->show(closeCallback);
//av->getContainerScrollView()->setTouchEnabled(false);


//return true;
//}


