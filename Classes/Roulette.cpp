#include "Roulette.h"
#include <random>
#include <limits>
const int marginPerItem = 60;

std::map<RewardKind, std::string> kind2File =
{ {RewardKind::kGold, "gacha1_gold.png"},
	{RewardKind::kRuby, "gacha1_ruby.png"},
	{RewardKind::kSpecialAttack, ""},
	{RewardKind::kDash, "gacha1_item1.png"},
	{RewardKind::kSlience, "gacha1_item5.png"},
	{RewardKind::kRentCard, "gacha1_item13.png"},
	{RewardKind::kSubMonsterOneKill, "gacha1_item3.png"},
	{RewardKind::kPower, "gacha1_item2.png"},
	{RewardKind::kMobLess, "gacha1_item6.png"},
	{RewardKind::kAddTime, "gacha1_item7.png"},
	{RewardKind::kDecreaseBossHp, "gacha1_item8.png"},
	{RewardKind::kDecreaseMobScale, "gacha1_item9.png"},
	{RewardKind::kPerfectLess, "gacha1_item11.png"},
	{RewardKind::kChangeOrderForce, "gacha1_item12.png"},
};

bool RouletteSub::init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm)
{
	CCLayer::init();
	setTouchEnabled(true);
	random_device rd;
	m_rEngine.seed(rd());

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
	CCLog("my order %d", m_alreadyDeterminantOrder);
	
	uniform_int_distribution<> dist(0, 2);
	
	CCSprite* ground = CCSprite::create("gacha1_back.png");
	ground->setPosition(ccp(240, 195));
	addChild(ground);
	m_gachaMode = gsm;
	
	
	m_parent = av;
	m_callback = callback;
	//		setContentSize(av->getViewSize());
	
	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setTouchPriority(INT_MIN);
	m_menu->setTouchEnabled(true);
	addChild(m_menu, 1);
	
	
	
	
	CCMenuItemImageLambda* stopBtn = CCMenuItemImageLambda::create("gacha4_stop.png", "gacha4_stop.png");
	stopBtn->setPosition(ccp(240, 40));
	//		startBtn->setVisible(false);
	stopBtn->setTarget([=](CCObject*)
										 {
											 if(m_state == RouletteState::kRotation)
											 {
												 stopBtn->setVisible(false);
												 m_state = RouletteState::kStopping;
												 addChild(KSSchedule::create([=](float dt)
																										 {
																											 float regexDegree = fmod(m_rotationBoard->getRotation(), 360.f);
																											 CCLog("goal %f, cur %f", fmod(m_rewards[m_alreadyDeterminantOrder].first, 360.f),
																														 regexDegree);
																											 uniform_int_distribution<> dist(-120, -68);
																											 if(this->diffDegree(360 - m_rewards[m_alreadyDeterminantOrder].first + dist(m_rEngine), regexDegree) < 3.f)
																											 {
																												 addChild(KSGradualValue<float>::create(15, 0, 5.f,
																																																[=](float t)
																																																{
																																																	m_rotationBoard->setRotation(m_rotationBoard->getRotation() + t);
																																																},
																																																[=](float t)
																																																{
																																																	m_state = RouletteState::kStoped;
																																																}));
																												 return false;
																											 }
																											 else
																											 {
																												 m_rotationBoard->setRotation(m_rotationBoard->getRotation() + 15.f);
																											 }
																											 return true;
																										 }));
												 
											 }
										 });
	m_menu->addChild(stopBtn);
	
	m_rotationBoard = CCNode::create();
	m_rotationBoard->setPosition(ccp(240, 190));
	addChild(m_rotationBoard);
	
	m_circleBoard = CCSprite::create("gacha1_roulette.png");
	m_rotationBoard->addChild(m_circleBoard);
//	circleBoard->setPosition(ccp(240, 190));
	
	CCSprite* arrow = CCSprite::create("gacha1_arrow.png");
	addChild(arrow);
	arrow->setPosition(ccp(240, 292));
	float sourceDegree = 0;
	int degreeCount = 0;
	for(auto i : rs)
	{
		RewardSprite* item = RewardSprite::create(i->m_kind, i->m_value, kind2File[i->m_kind], i->m_weight);
		item->setAnchorPoint(ccp(0.5f, 0.0f));
//		item->setPosition(ccp(240, 190));
		m_rotationBoard->addChild(item);
		item->setRotation(360 / 7.f * degreeCount);
		m_rewards.push_back(std::make_pair(360 / 7.f * degreeCount, item));
		degreeCount++;
	}
	
	CCLog("goal Degree : %f, %s", m_rewards[m_alreadyDeterminantOrder].first, m_rewards[m_alreadyDeterminantOrder].second->m_spriteStr.c_str());
	m_state = RouletteState::kRotation;
	
	stopBtn->setVisible(false);
	CCMenuItemImageLambda* startBtn = CCMenuItemImageLambda::create("gacha1_start.png", "gacha1_start.png");
	startBtn->setTarget([=](CCObject*)
											{
												stopBtn->setVisible(true);
												startBtn->removeFromParent();
												scheduleUpdate();
											});
	
	startBtn->setPosition(ccp(240, 190));
	m_menu->addChild(startBtn);
	return true;
}
void RouletteSub::update(float dt)
{
	if(m_state == RouletteState::kRotation)
	{
		m_rotationBoard->setRotation(m_rotationBoard->getRotation() + 15.f);
		
		float tempDegree = m_rotationBoard->getRotation();
		while(tempDegree >= 0)
		{
			tempDegree -= 360;
		}
		tempDegree += 360;
//		CCLog("degree = %f", tempDegree);
	}
	else if(m_state == RouletteState::kStopping)
	{
//		m_rotationBoard->setRotation(m_rotationBoard->getRotation() + 15.f);
	}
	else if(m_state == RouletteState::kStoped)
	{
		m_state = RouletteState::kQuestion;
		addChild(KSTimer::create(2.f, [=]()
														 {
															 float tempDegree = fmod(m_rotationBoard->getRotation(), 360.f);
															 CCLog("degree = %f", tempDegree);
															 
															 m_rotationBoard->setRotation(tempDegree);
															 float minDiffDegree = std::numeric_limits<float>::max();
															 RewardSprite* resultReward = nullptr;
															 for(auto i : m_rewards)
															 {
																 if(minDiffDegree > diffDegree(360 - m_rotationBoard->getRotation(), i.first))
																 {
																	 minDiffDegree = diffDegree(360 - m_rotationBoard->getRotation(), i.first);
																	 resultReward = i.second;
																 }
															 }
															 
															 KSAlertView* av = KSAlertView::create();
															 
															 CCNode* contentParent = CCNode::create();
															 
															 KSNode* content = new KSNode();
															 content->init();
															 content->autorelease();
															 contentParent->addChild(content);
															 av->setContentNode(contentParent);
															 av->setCloseOnPress(true);
															 av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
															 
															 av->setBorderScale(0.9f);
															 av->setCenterY(150);
															 
															 av->setContentNode(
																									contentParent
																									);
															 
															 content->addChild(CCSprite::create(resultReward->m_spriteStr.c_str()));
															 //		int selectedHorseOrder = -1;
															 //		for(int i=0; i<m_arriveOrder.size(); i++)
															 //		{
															 //			if(m_arriveOrder[i] == m_selectedHorseIndex)
															 //			{
															 //				selectedHorseOrder = i;
															 //				break;
															 //			}
															 //		}
															 content->addChild(CCLabelTTF::create
																								 (CCString::createWithFormat
																									("+%d",
																									 
																									 resultReward->m_value)->getCString(), mySGD->getFont().c_str(), 25));
															 
															 if(m_gachaMode == kGachaPurchaseStartMode_select)
															 {
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
																				rewards.push_back(RewardSprite::create(i.second->m_kind, i.second->m_value, i.second->m_spriteStr, i.second->m_weight));
																			}
																			getParent()->addChild(RouletteSub::create(m_callback, rewards, m_gachaMode),
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
															 av->addButton(CCMenuItemImageLambda::create
																						 (
																							"gacha_ok.png",
																							"gacha_ok.png",
																							[=](CCObject* e)
																							{
																								//										 removeFromParent();
																								RewardKind kind = resultReward->m_kind;
																								int selectedItemValue = resultReward->m_value;
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
															 content->setPosition(ccp((av->getViewSize() / 2.f).width,
																												-(av->getViewSize() / 2.f).height));
															 content->alignItemsVerticallyWithPadding(30);
															 addChild(av, 3);
															 av->show();
														 }));
	}
}

float RouletteSub::diffDegree(float a, float b) //
{
	a = fmod(a, 360.0);
	b = fmod(b, 360.0);
	
	return fmin(abs(a-b), abs(abs(a-b) - 360));
}

Roulette::Roulette()
{
	
}

Roulette::~Roulette()
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

bool Roulette::init(std::function<void(void)> closeCallback)
{
	CCLayer::init();
	KSAlertView* av = KSAlertView::create();
	
	RouletteSub* gs = RouletteSub::create(av, {
		RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
		RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
		RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
		RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
		RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
		RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
		RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
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


