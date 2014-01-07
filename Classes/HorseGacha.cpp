#include "HorseGacha.h"
#include "KSUtil.h"
#include "StarGoldData.h"
#include "ProbSelector.h"

bool HorseGachaSub::init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm)
{
	CCLayer::init();
	m_rewards = rs;
	
	// 시작하기도 전에 미리 등수를 지정함.
	// 슬프다... ㅜ.ㅜ
	int totalWeight = 0;
	ProbSelector ps; // {1,1,1,3,3,3,10} 으로 입력됐다면,
	for(auto i : m_rewards)
	{
		ps.pushProb(i->m_weight);
	}
	m_alreadyDeterminantOrder = ps.getResult(); // 등수를 미리 지정.
	CCLog("my order %d", m_alreadyDeterminantOrder);
	m_gachaMode = gsm;
	setTouchEnabled(true);
	CCSprite* back = CCSprite::create("table12.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	m_trackPosition = ccp(240, 188);
	m_horseBoardNode = CCNode::create();
	addChild(m_horseBoardNode);
	
	m_horseBoard = CCSprite::create("gacha3_horse_map.png");
	m_horseBoard->setPosition(m_trackPosition + ccp(0, 300));
	m_horseBoardNode->addChild(m_horseBoard);
	
	m_horsePositions = {ccp(20, 100), ccp(20, 100+40*1), ccp(20, 100+40*2),
		ccp(20, 100 + 40*3), ccp(20, 100 + 40*4), ccp(20, 100 + 40*5), ccp(20, 100 + 40*6)};
	std::vector<std::string> horseFiles = {"gacha3_horse1.png", "gacha3_horse2.png", "gacha3_horse3.png",
		"gacha3_horse4.png", "gacha3_horse5.png", "gacha3_horse6.png", "gacha3_horse7.png"};
	std::vector<CCPoint> rewardPositions = {ccp(30, 30), ccp(30 + 60*1, 30), ccp(30 + 60*2, 30), ccp(30 + 60*3, 30),
		ccp(30 + 60*4, 30), ccp(30 + 60*5, 30), ccp(30 + 60*6, 30)};
	CCPoint horseToReward = ccp(450, 0);

	int i=0;
	for(auto& reward : m_rewards)
	{
		reward->setPosition(rewardPositions[i]);
		reward->setAnchorPoint(ccp(0.5f, 0.0f));
		CCLabelBMFont* value = CCLabelBMFont::create(CCString::createWithFormat("%d", reward->m_value)->getCString(), "mb_white_font.fnt");
		reward->addChild(value);
//		m_rewards.push_back(hrs);
		addChild(reward);
		i++;
	}
	
	for(int i=0; i<horseFiles.size(); i++)
	{
		HorseSprite* horse = new HorseSprite();
		horse->init();
		horse->autorelease();
		horse->initWithFile( horseFiles[i].c_str() );
		
		horse->setPosition(m_horsePositions[i] + ccp(0, 300));
		horse->getCamera()->setEyeXYZ(0, 0.4f, 0.5f);
		
		horse->m_horseSpeed = m_well512.GetFloatValue(1.f, 1.f);
		horse->m_totalDistance = 0.f;
		m_horses.push_back(horse);
		m_horseBoardNode->addChild(horse, 8 - i);
	}
	m_horseBoardNode->setScaleY(1.2f);
//	m_horseBoardNode->setScaleX(1.4f);
	m_horseBoardNode->setScaleX(1.08f);
	m_horseBoardNode->getCamera()->setEyeXYZ(0, -1.1f, 1.6f);
	m_horseBoardNode->setPosition(ccp(-25.f, 0.f));
	// y = 0.4f, z = 0.5f 말의 카메라.
	
	m_parent = av;
	m_callback = callback;
	//		setContentSize(av->getViewSize());
	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setTouchPriority(INT_MIN);
	m_menu->setTouchEnabled(true);
	addChild(m_menu);
	
	CCMenuItemImageLambda* startBtn = CCMenuItemImageLambda::create("gacha_start.png", "gacha_start.png");
	startBtn->setPosition(ccp(240, 40));
	startBtn->setVisible(false);
	startBtn->setTarget([=](CCObject*)
											{
												normal_distribution<> distribution(1.3f, 1.0f);       // 생성 범위
												auto generator = bind(distribution, m_rEngine);
												for(int frame=0; ;frame++) // 가상 시뮬레이션 프레임
												{
													for(int i=0; i<m_horses.size(); i++)
													{
														if(std::find(m_arriveOrder.begin(), m_arriveOrder.end(), i) != m_arriveOrder.end())
															continue;
														ProbSelector ps = {1, 50};
														
														if(ps.getResult() == 0)
														{
															auto randomValue = generator();
															
															m_horses[i]->m_horseSpeed = MAX(1.0, randomValue);
														}
														m_horses[i]->m_speedPerFrame.push_back(m_horses[i]->m_horseSpeed);
														m_horses[i]->m_totalDistance += m_horses[i]->m_horseSpeed;
														if(m_horses[i]->m_totalDistance >= 450)
														{
															m_arriveOrder.push_back(i);
														}
													}
													
													// 전부 도착했으면 달림.
													if(m_arriveOrder.size() >= 7)
													{
														m_state = HorseSceneState::kRun;
														break;
													}
												}
												
//												m_arriveOrder[m_alreadyDeterminantOrder] = m_selectedHorseIndex;
												
												// 미리 정해진 등수가 몇 번째 인덱스인지
												int determintOrderIndex = m_arriveOrder[m_alreadyDeterminantOrder];
												for(int i=0; i<m_arriveOrder.size(); i++)
												{
													if(m_arriveOrder[i] == m_selectedHorseIndex)
													{
														std::swap(m_arriveOrder[i], m_arriveOrder[m_alreadyDeterminantOrder]);
														break;
													}
												}
												m_horses[m_selectedHorseIndex]->m_speedPerFrame.swap(m_horses[determintOrderIndex]->m_speedPerFrame);
												
												m_state = HorseSceneState::kRun;
												startBtn->setVisible(false);
											});
	m_menu->addChild(startBtn);
	//		HorseRewardSprite*
	addChild(KSGradualValue<CCPoint>::create(m_horseBoard->getPosition(), m_trackPosition, 0.6f,
																					 [=](CCPoint t)
																					 {
																						 m_horseBoard->setPosition(t);
//																						 CCLog("%f %f", t.x, t.y);
																					 },
																					 [=](CCPoint t)
																					 {
																						 m_state = HorseSceneState::kUnjiHorseReward;
																						 for(int i=0; i<m_horses.size(); i++)
																						 {
																							 addChild(KSGradualValue<CCPoint>::create(m_horses[i]->getPosition(), m_horsePositions[i], 0.6f,
																																												[=](CCPoint t)
																																												{
																																													m_horses[i]->setPosition(t);
																																												},
																																												[=](CCPoint t)
																																												{
																																												}));
																						 }
																						 
																						 addChild(KSTimer::create(2.6f, [=](){
																							 // 말 고르는 창이 뜸. 리턴으로 선택된 말의 인덱스가 넘어옴.
																							 // horseFiles[i].c_str() 으로 생성하면 됨.
																							 // 선택이 끝났으면 m_state = HorseSceneState::kRun; 달려!
																							 
																							 CCLayerColor* thiz = CCLayerColor::create(ccc4(0, 0, 0, 0));
																							 CCSprite* thiz0 = CCSprite::create();
																							 thiz0->setTextureRect(CCRectMake(0, 0, 520, 320));
																							 thiz0->setColor(ccc3(0, 0, 0));
																							 thiz0->setOpacity(150);
																							 thiz0->setPosition(ccp(240, 160));
																							 thiz->addChild(thiz0);
																							 addChild(thiz);
																							 
																							 CCSprite* desc = CCSprite::create("gacha3_horse_select1.png");
																							 desc->setPosition(ccp(240, 270));
																							 thiz->addChild(desc);
																							 vector<CCPoint> horseSelectPositions =
																								{ccp(90, 206), ccp(190, 206), ccp(288, 206), ccp(388, 206),
																									ccp(143, 98), ccp(248, 98), ccp(353, 98)};
																							 auto retFunction = [=](int index)
																							 {
																								 CCLog("mal index = %d", index);
																								 m_selectedHorseIndex = index;
																								 thiz->removeFromParent();
																								 startBtn->setVisible(true);
																								 
																							 };
																							 CCMenuLambda* _menu = CCMenuLambda::create();
																							 _menu->setTouchPriority(INT_MIN);
																							 _menu->setPosition(ccp(0, 0));
																							 thiz->addChild(_menu);
																							 for(int i=0; i<7; i++)
																							 {
																								 
																								 CCMenuItemLambda* m0 =
																								 CCMenuItemImageLambda::create
																								 ("gacha3_horse_selectbox.png", "gacha3_horse_selectbox.png",
																									[=](CCObject* t)
																									{
																										retFunction(i);
																									});

																								 m0->setPosition(horseSelectPositions[i]);
																								 CCSprite* horse = CCSprite::create(horseFiles[i].c_str());
																								 horse->setPosition(ccp(m0->getContentSize().width / 2.f, m0->getContentSize().height / 2.f));
																								 m0->addChild(horse);
																								 _menu->addChild(m0);
																							 }
																						 
																							 //
																						 }));
																						 CCLog("reward start");
																					 }));
	
	scheduleUpdate();
	return true;

}

void HorseGachaSub::update(float dt)
{
	if(m_state == HorseSceneState::kRun)
	{
		bool allArrive = true;
		for(auto horse : m_horses)
		{
			if(horse->m_speedPerFrame.empty() == false)
			{
				float speed = horse->m_speedPerFrame.front();
				horse->m_speedPerFrame.pop_front();
				horse->setPositionX(horse->getPositionX() + speed);
				allArrive = false;
			}
			else
			{
				
			}
		}
		
		if(allArrive) // 전부 도착했으면
		{
			m_state = HorseSceneState::kShowReward1;
		}
//		m_horses[0]->setPosition(ccp()
	}
	else if(m_state == HorseSceneState::kShowReward1)
	{
		m_state = HorseSceneState::kFinish;
		KSAlertView* av = KSAlertView::create();
		
		CCNode* contentParent = CCNode::create();
		
		KSNode* content = new KSNode();
		content->init();
		content->autorelease();
		contentParent->addChild(content);
		av->setContentNode(contentParent);
		av->setCloseOnPress(true);
		av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
		av->setContentBorder(CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0,0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
		av->setWidth(446 / 2.f);
		av->setHeight(466 / 2.f + 10);
		av->setButtonOffsetY(7);
		av->setBorderScale(0.8f);
		av->setCenterY(150);
		av->setContentNode(
											 contentParent
											 );
		av->setTitleOffsetY(-20);
		av->setTitleStr("획득");
		
		content->addChild(CCSprite::create(m_rewards[ m_alreadyDeterminantOrder ]->m_spriteStr.c_str()));
		int selectedHorseOrder = -1;
		for(int i=0; i<m_arriveOrder.size(); i++)
		{
			if(m_arriveOrder[i] == m_selectedHorseIndex)
			{
				selectedHorseOrder = i;
				break;
			}
		}
		content->addChild(CCLabelTTF::create
											(CCString::createWithFormat
											 ("+%d",
												
												m_rewards[m_alreadyDeterminantOrder]->m_value)->getCString(), mySGD->getFont().c_str(), 25));
		
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
						 rewards.push_back(RewardSprite::create(i->m_kind, i->m_value, i->m_spriteStr, i->m_weight));
					 }
					 getParent()->addChild(HorseGachaSub::create(m_callback, rewards, m_gachaMode),
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
			don->setPosition(ccp(25, 17));
			CCLabelTTF* value = CCLabelTTF::create("500", mySGD->getFont().c_str(), 14);
			value->setPosition(ccp(34, 12));
			don->addChild(value);
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
										 RewardKind kind = m_rewards[ m_alreadyDeterminantOrder ]->m_kind;
										 int selectedItemValue = m_rewards[ m_alreadyDeterminantOrder ]->m_value;
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
		
	}
}

HorseGacha::HorseGacha()
{
	
}
HorseGacha::~HorseGacha()
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

bool HorseGacha::init(std::function<void(void)> closeCallback)
{
	CCLayer::init();
	KSAlertView* av = KSAlertView::create();
	
	HorseGachaSub* gs = HorseGachaSub::create(av, {
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


