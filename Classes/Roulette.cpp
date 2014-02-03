
#include "Roulette.h"
#include <random>
#include <limits>
#include "GachaShowReward.h"
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
bool RouletteSub::init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, GachaCategory gc)
{
	CCLayer::init();
	m_gachaCategory = gc;
	setTouchEnabled(true);

	vector<int> ttt;
	CCSprite* dimed = CCSprite::create();
	dimed->setTextureRect(CCRectMake(0, 0, 520, 320));
	dimed->setColor(ccc3(0, 0, 0));
	dimed->setOpacity(180);
	dimed->setPosition(ccp(240, 160));
	addChild(dimed, 0);

	CCSprite* commonBack = CCSprite::create("gacha_1_stage.png");
	commonBack->setPosition(ccp(240, 140));
	addChild(commonBack, 0);
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

	//CCSprite* ground = CCSprite::create("gacha1_back.png");
	//ground->setPosition(ccp(240, 195));
	//addChild(ground);
	m_gachaMode = gsm;


	m_parentDialog = av;
	m_callback = callback;
	//		setContentSize(av->getViewSize());

	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setTouchPriority(INT_MIN);
	m_menu->setTouchEnabled(true);
	addChild(m_menu, 1);


	CCSprite* m_guide = CCSprite::create("gacha1_tip.png");
	m_guide->setPosition(ccp(240, 142));
	addChild(m_guide, 1);


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
	m_rotationBoard->setPosition(ccp(240, 150));
	addChild(m_rotationBoard);

	m_circleBoard = CCSprite::create("gacha1_roulette.png");
	m_rotationBoard->addChild(m_circleBoard);
	//	circleBoard->setPosition(ccp(240, 190));

	CCSprite* arrow = CCSprite::create("gacha1_arrow.png");
	addChild(arrow);
	arrow->setPosition(ccp(240, 245));
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

		if(i->m_kind == RewardKind::kGold || i->m_kind == RewardKind::kRuby)
		{
			CCLabelBMFont* m_value = CCLabelBMFont::create(CCString::createWithFormat("%d", i->m_value)->getCString(), "etc_font.fnt");
			float degree = 360 / 7.f * degreeCount;
			float rad = deg2Rad(-degree) + M_PI / 2.f;
			m_value->setRotation(360 / 7.f * degreeCount);
			m_value->setPosition(ccp(50 * cos(rad), 50 * sin(rad)));
			m_rotationBoard->addChild(m_value);
			m_value->setAnchorPoint(ccp(0.5f, 0.f));
		}
		degreeCount++;
	}

	CCLog("goal Degree : %f, %s", m_rewards[m_alreadyDeterminantOrder].first, m_rewards[m_alreadyDeterminantOrder].second->m_spriteStr.c_str());
	m_state = RouletteState::kRotation;

	stopBtn->setVisible(false);
	CCMenuItemImageLambda* startBtn = CCMenuItemImageLambda::create("gacha1_start.png", "gacha1_start.png");
	startBtn->setTarget([=](CCObject*)
			{
				m_guide->setVisible(false);
				stopBtn->setVisible(true);
				stopBtn->runAction(CCRepeatForever::create((CCSequence::createWithTwoActions(
								CCEaseBackOut::create(CCScaleTo::create(0.8f, 1.1f)), CCEaseBackOut::create(CCScaleTo::create(0.7f, 1.0f))))));
				startBtn->removeFromParent();
				addChild(KSGradualValue<float>::create(0, 15, 0.3f, [=](float t)
						{
							m_rotationBoard->setRotation(m_rotationBoard->getRotation() + t);
						},
						[=](float t)
						{
							scheduleUpdate();
						}));

			});

	startBtn->setPosition(ccp(240, 150));
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
		addChild(KSTimer::create(2.f, [=]() {
			float tempDegree = fmod(m_rotationBoard->getRotation(), 360.f);
			CCLog("degree = %f", tempDegree);

			m_rotationBoard->setRotation(tempDegree);
			float minDiffDegree = std::numeric_limits<float>::max();
			RewardSprite* resultReward = nullptr;
			for(auto i : m_rewards) {
				if(minDiffDegree > diffDegree(360 - m_rotationBoard->getRotation(), i.first)) {
					minDiffDegree = diffDegree(360 - m_rotationBoard->getRotation(), i.first);
					resultReward = i.second;
				}
			}



			RewardKind kind = resultReward->m_kind;
			int selectedItemValue = resultReward->m_value;
			std::function<void(void)> replayFunction;
			CCNode* parentPointer = getParent();
			if(m_gachaMode == kGachaPurchaseStartMode_select){ // 선택으로 들어온 거라면 다시 하기가 가능함.
				replayFunction = [=]() {
					// 선택으로 들어온 거라면 다시 하기가 가능함.
					if(mySGD->getGold() >= 500) {
						mySGD->setGold(mySGD->getGold() - 500);
						myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v) {

						});


						std::vector<RewardSprite*> rewards;
						for(auto i : m_rewards) {
							rewards.push_back(RewardSprite::create(i.second->m_kind, i.second->m_value, i.second->m_spriteStr, i.second->m_weight));
						}
						parentPointer->addChild(RouletteSub::create(m_callback, rewards, m_gachaMode, m_gachaCategory),
								this->getZOrder());
						this->removeFromParent();
					}
					else {
						CCLog("돈 없음");
					}

				};
			}else{
				replayFunction = nullptr;
			}
			GachaShowReward* gachaShowReward = GachaShowReward::create(replayFunction,
				m_callback,
					CCSprite::create(m_rewards[ m_alreadyDeterminantOrder ].second->m_spriteStr.c_str()),
					CCString::createWithFormat("%d", m_rewards[m_alreadyDeterminantOrder].second->m_value)->getCString(),
					kind,
					selectedItemValue
					);
			addChild(gachaShowReward, 3);
		}));
	}
}

float RouletteSub::diffDegree(float a, float b) //
{
	a = fmod(a, 360.0);
	b = fmod(b, 360.0);

	return fmin(abs(a-b), abs(abs(a-b) - 360));
}

//Roulette::Roulette()
//{

//}

//Roulette::~Roulette()
//{
//CCLog("~hatgacha");
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

//bool Roulette::init(std::function<void(void)> closeCallback)
//{
//CCLayer::init();
//KSAlertView* av = KSAlertView::create();

//RouletteSub* gs = RouletteSub::create(av, {
//RewardSprite::create(RewardKind::kRuby, 20, "price_ruby_img.png", 1),
//RewardSprite::create(RewardKind::kGold, 500, "price_gold_img.png", 2),
//RewardSprite::create(RewardKind::kSpecialAttack, 1, "item1.png", 5),
//RewardSprite::create(RewardKind::kDash, 1, "item4.png", 5),
//RewardSprite::create(RewardKind::kSlience, 1, "item8.png", 5),
//RewardSprite::create(RewardKind::kRentCard, 1, "item16.png", 5),
//RewardSprite::create(RewardKind::kSubMonsterOneKill, 1, "item9.png", 5)
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


