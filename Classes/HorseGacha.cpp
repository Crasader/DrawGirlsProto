#include "HorseGacha.h"
#include "KSUtil.h"
bool HorseGachaSub::init(KSAlertView* av, std::function<void(void)> callback)
{
	CCLayer::init();

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
	for(int i=0; i<7; i++)
	{
		HorseRewardSprite* hrs = new HorseRewardSprite();
		hrs->initWithFile("ui_gold_img.png");
		hrs->autorelease();
		hrs->m_kind = HorseRewardKind::kGold;
		hrs->m_value = m_well512.GetValue(100, 300);
		hrs->setPosition(rewardPositions[i]);
//		hrs->getCamera()->setEyeXYZ(0, 0.4f, 0.5f);
		hrs->setAnchorPoint(ccp(0.5f, 0.0f));
		CCLabelBMFont* value = CCLabelBMFont::create(CCString::createWithFormat("%d", hrs->m_value)->getCString(), "mb_white_font.fnt");
		hrs->addChild(value);
		m_rewards.push_back(hrs);
		addChild(hrs);
//		m_horseBoardNode->addChild(hrs);
		
	}
	
	for(int i=0; i<horseFiles.size(); i++)
	{
		CCSprite* horse = CCSprite::create(horseFiles[i].c_str());
		horse->setPosition(m_horsePositions[i] + ccp(0, 300));
		horse->getCamera()->setEyeXYZ(0, 0.4f, 0.5f);
		m_horses.push_back(horse);
		m_horseBoardNode->addChild(horse, 8 - i);
	}
	m_horseBoardNode->setScaleY(1.2f);
	m_horseBoardNode->setScaleX(1.4f);
	m_horseBoardNode->getCamera()->setEyeXYZ(0, -1.1f, 1.6f);
	m_horseBoardNode->setPosition(ccp(-100.f, 0.f));
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
												m_state = HorseSceneState::kRun;
												startBtn->setVisible(false);
											});
	m_menu->addChild(startBtn);
	//		HorseRewardSprite*
	addChild(KSGradualValue<CCPoint>::create(m_horseBoard->getPosition(), m_trackPosition, 0.6f,
																					 [=](CCPoint t)
																					 {
																						 m_horseBoard->setPosition(t);
																						 CCLog("%f %f", t.x, t.y);
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
																																													CCLog("%f", t.y);
																																												},
																																												[=](CCPoint t)
																																												{
																																												}));
																						 }
																						 
																						 addChild(KSTimer::create(2.6f, [=](){
																							 // 말 고르는 창이 뜸. 리턴으로 선택된 말의 인덱스가 넘어옴.
																							 // horseFiles[i].c_str() 으로 생성하면 됨.
																							 // 선택이 끝났으면 m_state = HorseSceneState::kRun; 달려!
																							 
																							 CCLayerColor* thiz = CCLayerColor::create(ccc4(166, 166, 166, 0));
																							 CCSprite* thiz0 = CCSprite::create();
																							 thiz0->setTextureRect(CCRectMake(0, 0, 520, 320));
																							 thiz0->setColor(ccc3(166, 166, 166));
																							 thiz0->setOpacity(100);
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
//		m_horses[0]->setPosition(ccp()
		for(int i=0; i<m_horses.size(); i++)
		{
			if(std::find(m_arriveOrder.begin(), m_arriveOrder.end(), i) != m_arriveOrder.end())
				continue;
			
			switch(i)
			{
				case 0:
					m_horses[i]->setPosition(ccp(m_horses[i]->getPosition().x + m_well512.GetFloatValue(2, 3.f), m_horses[i]->getPosition().y ));
					break;
				case 1:
					m_horses[i]->setPosition(ccp(m_horses[i]->getPosition().x + m_well512.GetFloatValue(0, 4.f), m_horses[i]->getPosition().y ));
					break;
				default:
					m_horses[i]->setPosition(ccp(m_horses[i]->getPosition().x + m_well512.GetFloatValue(2, 4.f), m_horses[i]->getPosition().y ));
					break;
			}
			
				
			if(m_horses[i]->getPosition().x >= 450)
			{
				m_arriveOrder.push_back(i);
			}
		}
		
		if(m_arriveOrder.size() >= 7)
		{
			m_state = HorseSceneState::kShowReward1;
		}
	}
	else if(m_state == HorseSceneState::kShowReward1)
	{
		m_state = HorseSceneState::kFinish;
		KSAlertView* av = KSAlertView::create();
		av->setCloseOnPress(true);
		av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
		
		av->setBorderScale(0.9f);
		av->setCenterY(150);
		
		auto ttf = CCLabelTTF::create("축하!!", "", 12.f);
		ttf->setColor(ccc3(0, 0, 0));
		
		av->setContentNode(
											 ttf
											 );
		av->setContentSize(ttf->getDimensions());
		
		av->addButton(CCMenuItemImageLambda::create
									(
									 "gacha_replay.png",
									 "gacha_replay.png",
									 [=](CCObject* e){
										 // 다시 해야되는데, 커튼이 닫히는거부터... 시작함.
									 }
									 ));
		
		av->addButton(CCMenuItemImageLambda::create
									(
									 "gacha_ok.png",
									 "gacha_ok.png",
									 [=](CCObject* e)
									 {
										 CCLog("%d %d", m_rewards[ m_arriveOrder[m_selectedHorseIndex] ]->m_kind, m_rewards[ m_arriveOrder[m_selectedHorseIndex] ]->m_value);
//										 removeFromParent();
										 
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
		addChild(av);
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
	
	HorseGachaSub* gs = HorseGachaSub::create(av);
	
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


