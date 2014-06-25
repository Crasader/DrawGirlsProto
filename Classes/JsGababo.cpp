#include "JsGababo.h"
#include "StyledLabelTTF.h"
#include "FormSetter.h"
#include "CommonButton.h"
#include "ks19937.h"
#include "ProbSelector.h"
#include "KSLabelTTF.h"
#include <boost/format.hpp>
#include "KSUtil.h"
#include "StyledLabelTTF.h"
#include "FormSetter.h"
#include "StageImgLoader.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
static int 	kAttackGa = 1;
static int	kAttackBa = 2;
static int	kAttackBo = 3;

static CCPoint _0winPosition = ccp(83.f - 5, 40.5);
static CCPoint _1winPosition = ccp(83.f - 5 + 74, 40.5);
static CCPoint _2winPosition = ccp(83.f - 5 + 74 * 2, 40.5);
static CCPoint _3winPosition = ccp(83.f - 5 + 74 * 3, 40.5);
JsGababo::JsGababo()
{
	m_stepSprite = nullptr;
//	m_gameStep = 1;
	m_winCount = 0;
	m_resultStamp = nullptr;
	m_tutorialStep = 1;
}
JsGababo::~JsGababo()
{
}
//void JsGababo::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool JsGababo::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool JsGababo::init()
{
	CCLayer::init();
	
	return true;
}


bool JsGababo::init(int touchPriority, const std::vector<BonusGameReward>& rewards, std::function<void(int)> endFunction)
{
	startFormSetter(this);
	m_touchPriority = touchPriority;
	m_endFunction = endFunction;
	m_rewards = rewards;
	
	CCSprite* bg = CCSprite::create("gababo_bg.png");
	bg->setPosition(ccp(240, 160));
	
	addChild(bg);
	
	auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back->setContentSize(CCSizeMake(357, 308));
	back->setPosition(ccp(240, 160));
	m_back = back;
	addChild(back);
	
	auto front = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	front->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -95 - 12.5));
	front->setContentSize(CCSizeMake(330 + 14, 100 - 19.5));
	back->addChild(front);
	m_front1 = front;
	auto tuto1 = StyledLabelTTF::create("<font color=#FFFFFF size=12 newline=14>가위 바위 보 중</font>"
																			"<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>",
																			mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	tuto1->setPosition(ccp(57.0, 54.5)); 			// dt (52.0, 25.5)
	front->addChild(tuto1);
	
	auto onSelection = [=]() // 바위 가위 보 셋중 하나 눌렀을 때~
	{
		contextSwitching(m_front1, m_front2, bind(&JsGababo::showHandsMotionWrapper, this), nullptr);
	};
	auto baBox = CommonButton::create(CCSprite::create("gababo_box.png"), touchPriority);
	baBox->setPosition(ccp(143.5, 41.0)); 			// dt (10.0, -3.0)
	auto ba = CCSprite::create("ba.png");
	ba->setRotation(-90);
	ba->setPosition(ccpFromSize(baBox->getContentSize()) / 2.f + ccp(0, 10.5f - 9.5f + 8));
	ba->setScale(0.5f);
	baBox->addChild(ba, 1);
	baBox->setFunction([=](CCObject*){
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackBa;
		onSelection();
	});
	front->addChild(baBox);
	auto gaBox = CommonButton::create(CCSprite::create("gababo_box.png"), touchPriority);
	gaBox->setPosition(ccp(220.0, 41.0)); 			// dt (2.0, -4.5)
	auto ga = CCSprite::create("ga.png");
	ga->setRotation(-90);
	ga->setPosition(ccpFromSize(gaBox->getContentSize()) / 2.f + ccp(0, 10.5f - 6.5f - 4));
	ga->setScale(0.5f);
	gaBox->addChild(ga, 1);
	gaBox->setFunction([=](CCObject*){
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackGa;
		onSelection();
	});
	front->addChild(gaBox);
	auto boBox = CommonButton::create(CCSprite::create("gababo_box.png"), touchPriority);
	boBox->setPosition(ccp(296.5, 41.0)); 			// dt (6.0, -3.0)
	auto bo = CCSprite::create("bo.png");
	bo->setRotation(-90);
	bo->setPosition(ccpFromSize(boBox->getContentSize()) / 2.f + ccp(0, 10.5f - 6.5f));
	bo->setScale(0.5f);
	boBox->addChild(bo, 1);
	boBox->setFunction([=](CCObject*){
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackBo;
		onSelection();
	});
	front->addChild(boBox);
	
	
	CCSprite* stepFrame = CCSprite::create("gababo_frame.png");
	m_stepFrame = stepFrame;
	back->addChild(stepFrame, 2);
	//////////////////////
	stepFrame->setScale(1.f);
	//////////////////
	stepFrame->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, 39));
	setFormSetter(back);
	setFormSetter(front);
	setFormSetter(tuto1);
	setFormSetter(baBox);
	setFormSetter(gaBox);
	setFormSetter(boBox);
	setFormSetter(stepFrame);
	setFormSetter(ba);
	setFormSetter(ga);
	setFormSetter(bo);
	
	///////////////
//	front->setVisible(false);
	////////////////
	
	
//	m_front1->setScaleY(0.f);
	
	m_front1->setScaleY(0.f);
	
	setupReward();
	setupCongMessage();
	setupTutorial();
	setupHands();
	loadImage(1);
	if(myDSH->getIntegerForKey(kDSH_Key_isShowGababoTutorial) || 1)
	{
		m_front1->setScaleY(1.f);
	}
	else
	{
		myDSH->setIntegerForKey(kDSH_Key_isShowGababoTutorial, 1);
		m_front4->setScaleY(1.f);
	}
	return true;
}

void JsGababo::setupReward()
{
	
	auto front2 = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	front2->setPosition(ccpFromSize(m_back->getContentSize()) / 2.f + ccp(0, -95 - 12.5));
	front2->setContentSize(CCSizeMake(330 + 14, 100 - 19.5));
	m_back->addChild(front2);
	m_front2 = front2;
	
	CCSprite* rewardTitleBack = CCSprite::create("gababo_lefttop.png");
	rewardTitleBack->setPosition(ccp(28.0 - 3.5, 40.5)); 			// dt (4.0, -10.0)
	
	m_front2->addChild(rewardTitleBack);
	auto rewardTitleLbl = StyledLabelTTF::create("<font size=12 newline=13>연</font>"
																							 "<font size=12 newline=13>승</font>"
																							 "<font size=12 newline=13>보</font>"
																							 "<font size=12 newline=13>상</font>",
																							 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	rewardTitleLbl->setAnchorPoint(ccp(0.5f, 0.5f));
	rewardTitleLbl->setPosition(ccpFromSize(rewardTitleBack->getContentSize()) / 2.f + ccp(0, 0));
	rewardTitleBack->addChild(rewardTitleLbl);

	
	auto addReward = [=](const std::string& lbl, int rewardIndex)->pair<CCNode*, CCNode*>
	{
		
		CCSprite* _1winBox = CCSprite::create("gababo_box.png");
		front2->addChild(_1winBox);
		
		std::map<int, std::string> tagImageStr;
		tagImageStr[0] = "gababo_normal.png";
		tagImageStr[1] = "gababo_bronze.png";
		tagImageStr[2] = "gababo_silver.png";
		tagImageStr[3] = "gababo_gold.png";
		
		CCSprite* _1winTag = CCSprite::create(tagImageStr[rewardIndex].c_str());
		_1winTag->setPosition(ccp(37.5, 39.0)); 			// dt (0.0, 1.5)
		_1winTag->setPosition(ccpFromSize(_1winBox->getContentSize()) / 2.f + ccp(0, 0));
		
		_1winBox->addChild(_1winTag);
		KSLabelTTF* _1winTagLbl = KSLabelTTF::create(lbl.c_str(), mySGD->getFont().c_str(), 12.f);
		_1winTagLbl->setPosition(ccpFromSize(_1winTag->getContentSize()) / 2.f + ccp(0, 25));
		front2->addChild(_1winTagLbl, 1);
		
		auto _1winReward = CCSprite::create(m_rewards[rewardIndex].spriteName.c_str());
		_1winReward->setPosition(ccpFromSize(_1winBox->getContentSize()) / 2.f + ccp(0, -13.0));
		_1winBox->addChild(_1winReward);
		auto _1winRewardLbl = KSLabelTTF::create(m_rewards[rewardIndex].desc.c_str(), mySGD->getFont().c_str(), 11.f);
		_1winRewardLbl->setPosition(ccpFromSize(_1winReward->getContentSize()) / 2.f + ccp(0, -6.0));
		_1winReward->addChild(_1winRewardLbl);
		return make_pair(_1winBox, _1winTagLbl);
	};
	auto _0 = addReward("보상", 0);
	auto _1 = addReward("1승", 1);
	auto _2 = addReward("3승", 2);
	auto _3 = addReward("4승", 3);
	CCNode* _0winBox = _0.first;
	CCNode* _1winBox = _1.first;
	CCNode* _2winBox = _2.first;
	CCNode* _3winBox = _3.first;
	_0winBox->setPosition(_0winPosition); 			// dt (140.5, 45.0)
	_1winBox->setPosition(_1winPosition);
	_2winBox->setPosition(_2winPosition);
	_3winBox->setPosition(_3winPosition);
	
	CCNode* _0winTagLbl = _0.second;
	_0winTagLbl->setPosition(ccp(78.0, 65.5)); 			// dt (40.5, 3.0)
	CCNode* _1winTagLbl = _1.second;
	_1winTagLbl->setPosition(ccp(78 + 74*1, 65.5)); 			// dt (114.5, 3.0)
	CCNode* _2winTagLbl = _2.second;
	_2winTagLbl->setPosition(ccp(78 + 74*2, 65.5)); 			// dt (114.5, 3.0)
	CCNode* _3winTagLbl = _3.second;
	_3winTagLbl->setPosition(ccp(78 + 74*3, 65.5)); 			// dt (114.5, 3.0)
	setFormSetter(_0winTagLbl);
	setFormSetter(_1winTagLbl);
	setFormSetter(_2winTagLbl);
	setFormSetter(_3winTagLbl);
	m_currentRewardCursor = CCSprite::create("gababo_select.png");
	m_currentRewardCursor->setPosition(_0winPosition);
	front2->addChild(m_currentRewardCursor);
	setFormSetter(rewardTitleBack);
	setFormSetter(front2);
	setFormSetter(_1winBox);
	setFormSetter(_2winBox);
	setFormSetter(_3winBox);
	front2->setScaleY(0.f);
}
void JsGababo::loadImage(int step)
{
	if(m_stepSprite != nullptr)
		m_stepSprite->removeFromParent();
	m_stepSprite = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",
																																NSDS_GI(1, kSDS_SI_level_int1_card_i, step))->getCString());
	// CCSprite::create(boost::str(boost::format("ga%||.png") % step).c_str());
	m_stepSprite->setScale(m_stepFrame->getContentSize().height / m_stepSprite->getContentSize().height);
	m_stepSprite->setScale(m_stepSprite->getScale());
	m_stepSprite->setPosition(m_stepFrame->getPosition());
	m_back->addChild(m_stepSprite, 1);
	
	setFormSetter(m_stepSprite);
}

void JsGababo::contextSwitching(CCNode* from, CCNode* to, std::function<void(void)> mid, std::function<void(void)> end)
{
	to->setScaleY(0.f);
	addChild(KSGradualValue<float>::create(1.f, 0, 0.2f, [=](float t){
		from->setScaleY(t);
	}, [=](float t){
		from->setScaleY(t);
		if(mid)
		{
			mid();
		}
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t){
			to->setScaleY(t);
		}, [=](float t){
			to->setScaleY(t);
			if(end)
			{
				end();
			}
		}));
	}));
}
void JsGababo::showReward()
{
//	addChild(KSGradualValue<float>::create(1.f, 0, 0.2f, [=](float t){
//		m_front1->setScaleY(t);
//	}, [=](float t){
//		m_front1->setScaleY(t);
//		showHandsMotion([=](){
//			auto t1 = m_leftHand->getPosition();
//			m_leftHand->removeFromParent();
//			if(m_mySelection == kAttackBa)
//			{
//				m_leftHand = CCSprite::create("ba.png");
//			}
//			else if(m_mySelection == kAttackGa)
//			{
//				m_leftHand = CCSprite::create("ga.png");
//			}
//			else
//			{
//				m_leftHand = CCSprite::create("bo.png");
//			}
//			m_leftHandContainer->addChild(m_leftHand);
//			m_leftHand->setPosition(t1);
//		}, [=](){
//			int computer = ks19937::getIntValue(1, 3);
//			int D = m_mySelection - computer % 3;
//			auto t1 = m_rightHand->getPosition();
//			if(computer == kAttackBa)
//			{
//				m_rightHand = CCSprite::create("ba.png");
//			}
//			else if(computer == kAttackGa)
//			{
//				m_rightHand = CCSprite::create("ga.png");
//			}
//			else
//			{
//				m_rightHand = CCSprite::create("bo.png");
//			}
//			m_rightHandContainer->addChild(m_rightHand);
//			m_rightHand->setPosition(t1);
//			if(m_mySelection == computer) // Draw
//			{
//				CCLOG("draw");
//				m_currentJudge = "draw";
//			}
//			else if(D == 1) // Win
//			{
//				CCLOG("win");
//				m_currentJudge = "win";
//				
//			}
//			else // Lose
//			{
//				CCLOG("lose~");
//				m_currentJudge = "lose";
//			}
//		});
//		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t){
//			m_front2->setScaleY(t);
//		}, [=](float t){
//			m_front2->setScaleY(t);
//		}));
//	}));
}

void JsGababo::showHands()
{
	addChild(KSGradualValue<float>::create(1.f, 0, 0.2f, [=](float t){
		m_front2->setScaleY(t);
	}, [=](float t){
		m_front2->setScaleY(t);
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t){
			m_front1->setScaleY(t);
		}, [=](float t){
			m_front1->setScaleY(t);
		}));
	}));
}
void JsGababo::showHandsMotion(std::function<void(void)> endLeft, std::function<void(void)> endRight)
{
	auto p1 = m_leftHand->getPosition();
	auto j1 = m_leftHand->getAnchorPoint();
	m_leftHand->removeFromParent();
	m_leftHand = CCSprite::create("ba.png");
	m_leftHand->setPosition(p1);
	m_leftHand->setAnchorPoint(j1);
	m_leftHandContainer->addChild(m_leftHand);
	
	auto p2 = m_rightHand->getPosition();
	auto j2 = m_rightHand->getAnchorPoint();
	m_rightHand->removeFromParent();
	m_rightHand = CCSprite::create("ba.png");
	m_rightHand->setPosition(p2);
	m_rightHand->setAnchorPoint(j2);
	m_rightHandContainer->addChild(m_rightHand);
	
	
	float timeMul = 2.f;
	{
		CCNode* handContainer = m_leftHandContainer;
		addChild(KSGradualValue<float>::create(handContainer->getRotation(), handContainer->getRotation() + 120, 0.2 * timeMul,
																					 [=](float t){
																						 handContainer->setRotation(t);
																					 }, [=](float t){
																						 handContainer->setRotation(t);
																						 addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																										handContainer->getRotation() - 40, 0.23 * timeMul,
																																										[=](float t){
																																											handContainer->setRotation(t);
																																										}, [=](float t){
																																											handContainer->setRotation(t);
																																											addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																																														 handContainer->getRotation() + 40, 0.13 * timeMul,
																																																														 [=](float t){
																																																															 handContainer->setRotation(t);
																																																														 }, [=](float t){
																																																															 handContainer->setRotation(t);
																																																															 addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																																																																			handContainer->getRotation() - 40, 0.23 * timeMul,
																																																																																			[=](float t){
																																																																																				handContainer->setRotation(t);
																																																																																			}, [=](float t){
																																																																																				handContainer->setRotation(t);
																																																																																				addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																																																																																							 handContainer->getRotation() + 40, 0.2 * timeMul,
																																																																																																							 [=](float t){
																																																																																																								 handContainer->setRotation(t);
																																																																																																							 }, [=](float t){
																																																																																																								 handContainer->setRotation(t);
																																																																																																								 if(endLeft)
																																																																																																									 endLeft();
																																																																																																							 }));
																																																																																			}));
																																																														 }));
																																										}));
																					 }));
	}
	{
		
		CCNode* handContainer = m_rightHandContainer;
		addChild(KSGradualValue<float>::create(handContainer->getRotation(), handContainer->getRotation() - 120, 0.2 * timeMul,
																					 [=](float t){
																						 handContainer->setRotation(t);
																					 }, [=](float t){
																						 handContainer->setRotation(t);
																						 addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																										handContainer->getRotation() + 40, 0.23 * timeMul,
																																										[=](float t){
																																											handContainer->setRotation(t);
																																										}, [=](float t){
																																											handContainer->setRotation(t);
																																											addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																																														 handContainer->getRotation() - 40, 0.13 * timeMul,
																																																														 [=](float t){
																																																															 handContainer->setRotation(t);
																																																														 }, [=](float t){
																																																															 handContainer->setRotation(t);
																																																															 addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																																																																			handContainer->getRotation() + 40, 0.23 * timeMul,
																																																																																			[=](float t){
																																																																																				handContainer->setRotation(t);
																																																																																			}, [=](float t){
																																																																																				handContainer->setRotation(t);
																																																																																				addChild(KSGradualValue<float>::create(handContainer->getRotation(),
																																																																																																							 handContainer->getRotation() - 40, 0.2 * timeMul,
																																																																																																							 [=](float t){
																																																																																																								 handContainer->setRotation(t);
																																																																																																							 }, [=](float t){
																																																																																																								 handContainer->setRotation(t);
																																																																																																								 if(endRight)
																																																																																																									 endRight();
																																																																																																							 }));
																																																																																			}));
																																																														 }));
																																										}));
																					 }));
		handContainer->setScaleX(-1);
	}
	setFormSetter(m_leftHandContainer);
	setFormSetter(m_rightHandContainer);
}
void JsGababo::setupCongMessage()
{
	auto front = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	front->setPosition(ccpFromSize(m_back->getContentSize()) / 2.f + ccp(0, -95 - 12.5));
	front->setContentSize(CCSizeMake(330 + 14, 100 - 19.5));
	m_back->addChild(front);
	m_front3 = front;
	
	StyledLabelTTF* message = StyledLabelTTF::create("<font color=#FFFFFF size=14 newline=14>와우! 이겼어요!</font>"
																									 "<font color=#FFAA14 size=14>더 좋은 보상을 향해 한번 더!</font>",
																									 mySGD->getFont().c_str(),
																									 12.f, 999, StyledAlignment::kCenterAlignment);
	m_message = message;
	message->setAnchorPoint(ccp(0.5f, 0.5f));
	message->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(-29, 0));
	front->addChild(message);
	
	CommonButton* button = CommonButton::create("확 인", 12.f, CCSizeMake(60, 36), CommonButtonYellow, 0);
	button->setTouchPriority(m_touchPriority - 1);
	button->setFunction(bind(&JsGababo::onPressConfirm, this, std::placeholders::_1));
	button->getTitleLabel()->setColor(ccc3(37, 15, 0));
	button->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(128, 0));
	front->addChild(button);
	
	setFormSetter(message);
	setFormSetter(button);
	m_front3->setScaleY(0.f);
}

void JsGababo::setupHands()
{
	CCSprite* me = CCSprite::create("one_percent_gacha_me.png");
	m_back->addChild(me);
	me->setPosition(ccp(53, 254));
	KSLabelTTF* meLbl = KSLabelTTF::create("나", mySGD->getFont().c_str(), 14.f);
	meLbl->setColor(ccc3(255, 170, 20));
	me->addChild(meLbl);
	meLbl->setPosition(ccpFromSize(me->getContentSize()) / 2.f + ccp(0, 10));
	
	setFormSetter(me);
	float timeMul = 2.f;
	{
		CCNode* handContainer = CCNode::create();
		m_leftHandContainer = handContainer;
		m_back->addChild(handContainer, 2);
		handContainer->setPosition(ccp(-208.5, 181.5)); 			// dt (-208.5, 181.5)
		float x = 0;
		for(int i=0; i<50; i++)
		{
			CCSprite* stick = CCSprite::create("gababo_stick.png");
			stick->setAnchorPoint(ccp(0.f, 0.5f));
			handContainer->addChild(stick);
			stick->setPosition(ccp(x, 0));
			x += stick->getContentSize().width;
		}
		CCSprite* ba = CCSprite::create("ba.png");
		m_leftHand = ba;
		ba->setAnchorPoint(ccp(0.f, 0.5f));
		handContainer->addChild(ba);
		ba->setPosition(ccp(x - 20, 0));
		handContainer->setRotation(-120);
	}
	{
		CCNode* handContainer = CCNode::create();
		m_rightHandContainer = handContainer;
		m_back->addChild(handContainer, 2);
		handContainer->setPosition(ccp(567.f, 181.5)); 			// dt (-208.5, 181.5)
		float x = 0;
		for(int i=0; i<50; i++)
		{
			CCSprite* stick = CCSprite::create("gababo_stick.png");
			stick->setAnchorPoint(ccp(0.f, 0.5f));
			handContainer->addChild(stick);
			stick->setPosition(ccp(x, 0));
			x += stick->getContentSize().width;
		}
		CCSprite* ba = CCSprite::create("ba.png");
		m_rightHand = ba;
		ba->setAnchorPoint(ccp(0.f, 0.5f));
		handContainer->addChild(ba);
		ba->setPosition(ccp(x - 20, 0));
		handContainer->setRotation(120);
	}
}
void JsGababo::onPressConfirm(CCObject* t)
{
	CCLOG("%s", m_currentJudge.c_str());
	if(m_front3->getScaleY() <= 0.5f)
		return;
	
	auto rollBack = [=](){
		auto r1 = m_leftHandContainer->getRotation();
		auto r2 = m_rightHandContainer->getRotation();
		addChild(KSGradualValue<float>::create(0, 1.f, 1.f, [=](float t){
			float y0 = (- 120) * t + r1;
			float y1 = (120) * t + r2;
			m_leftHandContainer->setRotation(y0);
			m_rightHandContainer->setRotation(y1);
			
		}, [=](float t){
			float y0 = (- 120) * t + r1;
			float y1 = (120) * t + r2;
			m_leftHandContainer->setRotation(y0);
			m_rightHandContainer->setRotation(y1);
		}));
		
	};
	if(m_currentJudge == "draw")
	{
		contextSwitching(m_front3, m_front1, rollBack, nullptr);
//		contextSwitching(m_front1, m_front2, bind(&JsGababo::showHandsMotionWrapper, this), nullptr);
//		auto onSelection = [=]() // 바위 가위 보 셋중 하나 눌렀을 때~
//		{
//			contextSwitching(m_front1, m_front2, bind(&JsGababo::showHandsMotionWrapper, this), nullptr);
//		};

	}
	
	else if(m_currentJudge == "win")
	{
		if(m_winCount < 3)
		{
			contextSwitching(m_front3, m_front1, [=](){
				rollBack();
				auto lightPair = KS::loadCCBI<CCSprite*>(this, "gababo_change.ccbi");
				CCSprite* light = lightPair.first;
				lightPair.second->setAnimationCompletedCallbackLambda(this, [=](const char* seqName){
					light->removeFromParent();
				});
				light->setPosition(ccp(240, 160));
				addChild(light, 100);
				addChild(KSTimer::create(2.2f, [=](){
					loadImage(m_winCount);
				}));
			}, nullptr);
		}
		else
		{
			showResult();
		}
	}
	else if(m_currentJudge == "lose")
	{
		rollBack();
		showResult();
	}
	if(m_resultStamp)
	{
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.16f, [=](float t){
			float y0 = (1.5f - 1.f) * t + 1.f; // scale
			float y1 = ((0 - 255.f) * t + 255.f); // opacity
			KS::setOpacity(m_resultStamp, y1);
			m_resultStamp->setScale(y0);
		}, [=](float t){
			
			float y0 = (1.5f - 1.f) * t + 1.f; // scale
			float y1 = ((0 - 255.f) * t + 255.f); // opacity
			KS::setOpacity(m_resultStamp, y1);
			m_resultStamp->setScale(y0);
			m_resultStamp->removeFromParent();
			m_resultStamp = nullptr;
		}));
		
	}
}

void JsGababo::showHandsMotionWrapper()
{
	showHandsMotion([=](){
		auto t1 = m_leftHand->getPosition();
		m_leftHand->removeFromParent();
		if(m_mySelection == kAttackBa)
		{
			m_leftHand = CCSprite::create("ba.png");
		}
		else if(m_mySelection == kAttackGa)
		{
			m_leftHand = CCSprite::create("ga.png");
		}
		else
		{
			m_leftHand = CCSprite::create("bo.png");
		}
		m_leftHand->setAnchorPoint(ccp(0.f, 0.5f));
		m_leftHandContainer->addChild(m_leftHand);
		m_leftHand->setPosition(t1);
	}, [=](){
		// m_winCount : 0, 1, 2
		int computer = 1;
		std::map<int, std::function<int(int)>> functor;
		functor[0] = bind(&JsGababo::loseSkill, this, std::placeholders::_1);
		functor[1] = bind(&JsGababo::winSkill, this, std::placeholders::_1);
		functor[2] = bind(&JsGababo::drawSkill, this, std::placeholders::_1);
		if(m_winCount == 0)
		{
			// 이길 확률 70 % 질 확률 15% 비길 확률 15%
			ProbSelector ps = {70.f, 15.f, 15.f};
			computer = functor[ps.getResult()](m_mySelection);
		}
		else if(m_winCount == 1)
		{
			// 이길 확률 60% 질 확률 20% 비길 확률 20%
			ProbSelector ps = {60.f, 20.f, 20.f};
			computer = functor[ps.getResult()](m_mySelection);
		}
		else if(m_winCount == 2)
		{
			// 이길 확률 50 % 질 확률 25% 비길 확률 25%
			ProbSelector ps = {50.f, 25.f, 25.f};
			computer = functor[ps.getResult()](m_mySelection);
		}
		
		int D = m_mySelection - computer % 3;
		auto t1 = m_rightHand->getPosition();
		m_rightHand->removeFromParent();
		if(computer == kAttackBa)
		{
			m_rightHand = CCSprite::create("ba.png");
		}
		else if(computer == kAttackGa)
		{
			m_rightHand = CCSprite::create("ga.png");
		}
		else
		{
			m_rightHand = CCSprite::create("bo.png");
		}
		m_rightHand->setAnchorPoint(ccp(0.f, 0.5f));
		m_rightHandContainer->addChild(m_rightHand);
		m_rightHand->setPosition(t1);
		
		if(m_mySelection == computer) // Draw
		{
			
		}
		else if(D == 1) // Win
		{
			m_winCount++;
			CCPoint targetPosition = CCPointZero;
			
			if(m_winCount == 1)
			{
				targetPosition = _1winPosition;
			}
			else if(m_winCount == 2)
			{
				targetPosition = _2winPosition;
			}
			else if(m_winCount == 3)
			{
				targetPosition = _3winPosition;
			}
			
			addChild(KSGradualValue<CCPoint>::create(m_currentRewardCursor->getPosition(), targetPosition,
																							 0.5f, [=](CCPoint t){
																								 m_currentRewardCursor->setPosition(t);
																							 }, [=](CCPoint t){
																								 m_currentRewardCursor->setPosition(t);
																							 }));
		}
		else // Lose
		{
			
		}
		addChild(KSTimer::create(2.f, [=](){
			auto showEffect = [=](CCNode* result_stamp)
			{
				addChild(KSGradualValue<float>::create(0.f, 1.f, 0.26f, [=](float t){
					float y0 = 255.f * t;
					float y1 = -1*t + 2;
					KS::setOpacity(result_stamp, y0);
					result_stamp->setScale(y1);
				}, [=](float t){
					float y0 = 255.f * t;
					float y1 = -1*t + 2;
					KS::setOpacity(result_stamp, y0);
					result_stamp->setScale(y1);
					
				}));
			};
			
			if(m_mySelection == computer) // Draw
			{
				m_message->setStringByTag("<font color=#FFFFFF size=14 newline=14>이런, 무승부네요!</font>"
																	"<font color=#FFAA14 size=14>한번 더!</font>");
				m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 0));
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					CCSprite* result_stamp = CCSprite::create("gababo_draw.png");
					m_resultStamp = result_stamp;
					result_stamp->setRotation(-15);
					m_back->addChild(result_stamp, 3);
					result_stamp->setPosition(ccp(m_back->getContentSize().width / 2.f, 160));
					showEffect(result_stamp);
				});
				m_currentJudge = "draw";
				CCLOG("draw");
			}
			else if(D == 1) // Win
			{
				CCLOG("win");
				
				if(m_winCount != 3)
				{
					m_message->setStringByTag("<font color=#FFFFFF size=14 newline=14>와우! 이겼어요!</font>"
																		"<font color=#FFAA14 size=14>더 좋은 보상을 향해 한번 더!</font>");
					m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 0));
				}
				else
				{
					m_message->setStringByTag("<font color=#FFFFFF size=14 newline=14>세상에, 4판모두 이긴 사람은 당신이 처음이에요.</font>"
																		"<font color=#FFAA14 size=14 newline=14>최고의 보상을 드릴게요!</font>"
																		"<font color=#FFAA14 size=14 newline=14>보너스 게임은 퍼펙트 클리어시마다 할 수 있어요!</font>"
																		"<font color=#FFAA14 size=14 newline=14>그럼 다음에 또 만나요!</font>"
																		);
					m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 0));
					
				}
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					CCSprite* result_stamp = CCSprite::create("endless_winner.png");
					
					m_resultStamp = result_stamp;
					CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", m_winCount)->getCString(), "winfont.fnt");
					win_label->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f+10));
					result_stamp->addChild(win_label);
					
					CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
					win_ment->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f-25));
					result_stamp->addChild(win_ment);
					
					result_stamp->setRotation(-15);
					m_back->addChild(result_stamp, 3);
					result_stamp->setPosition(ccp(m_back->getContentSize().width / 2.f, 160));
					
					showEffect(result_stamp);
				});
				m_currentJudge = "win";
				
			}
			else // Lose
			{
				CCLOG("lose~");
				m_message->setStringByTag("<font color=#FFFFFF size=14 newline=14>이런이런... 지고 말았네요. 너무 상심마세요!</font>"
																	"<font color=#FFAA14 size=14>보너스게임은 퍼펙트 클리어시마다 할 수 있어요!</font>");
				m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 0));
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					CCSprite* result_stamp = CCSprite::create("endless_loser.png");
					m_resultStamp = result_stamp;
					result_stamp->setRotation(-15);
					m_back->addChild(result_stamp, 3);
					result_stamp->setPosition(ccp(m_back->getContentSize().width / 2.f, 160));
					showEffect(result_stamp);
				});
				m_currentJudge = "lose";
				
			}
		}));
	});
	
}
void JsGababo::showResult()
{
	BonusGameReward gr1;
	gr1.spriteName = "shop_ruby2.png";
	gr1.desc = "루우비~!";
	auto tempEndFunction = m_endFunction;
	CurtainNodeForBonusGame* curtain = CurtainNodeForBonusGame::createForEnding((int)Curtain::kTouchPriority, gr1,
																																							[=](){
																																								removeFromParent();
																																							},
																																							[=](){
																																								if(tempEndFunction)
																																								{
																																									tempEndFunction(m_winCount);
																																								}
																																							});
	getParent()->addChild(curtain, (int)Curtain::kCurtain);
	
}
void JsGababo::setupTutorial()
{
	auto front = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	front->setPosition(ccpFromSize(m_back->getContentSize()) / 2.f + ccp(0, -95 - 12.5));
	front->setContentSize(CCSizeMake(330 + 14, 100 - 19.5));
	m_back->addChild(front);
	m_front4 = front;
	
	StyledLabelTTF* message = StyledLabelTTF::create("<font color=#FFFFFF size=14 newline=14>보너스 게임에 오신 것을 환영합니다!</font>"
																									 "<font color=#FFFFFF size=14 newline=14>벌써 퍼즐을 퍼펙트 클리어 하시다니 정말 대단해요!</font>"
																									 "<font color=#FFAA14 size=14>게임 설명은 최초 1회만 해드리니 잘 봐주세요!</font>",
																									 mySGD->getFont().c_str(),
																									 12.f, 999, StyledAlignment::kCenterAlignment);
//	m_message = message;
	message->setAnchorPoint(ccp(0.5f, 0.5f));
	message->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(-29, 0));
	front->addChild(message);
	
	CommonButton* button = CommonButton::create("다 음", 12.f, CCSizeMake(60, 36), CommonButtonYellow, 0);
	button->setTouchPriority(m_touchPriority - 1);
	button->setFunction([=](CCObject*){
		
		if(m_front4->getScaleY() <= 0.5f)
			return;
		if(m_tutorialStep == 1)
		{
			message->setStringByTag("<font color=#FFFFFF size=14 newline=14>게임은 가위바위보 입니다!</font>"
															"<font color=#FFAA14 size=14 newline=14>가위바위보 중 하나를 선택하여 저를 이기시면 됩니다.</font>"
															"<font color=#FFAA14 size=14 newline=14>연승 횟수에 따라 더 큰 보상이 있어요!</font>"
															"<font color=#FFFFFF size=14 newline=14>그럼 행운을 빌어요!</font>"
															);
		}
		else
		{
			this->contextSwitching(m_front4, m_front1, nullptr, nullptr);
		}
		m_tutorialStep++;
	});
	button->getTitleLabel()->setColor(ccc3(37, 15, 0));
	button->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(128, 0));
	front->addChild(button);
	
	setFormSetter(message);
	setFormSetter(button);
	m_front4->setScaleY(0.f);
}
int JsGababo::winSkill(int i)
{
	if(i == 1)
		return 2;
	else if(i == 2)
		return 3;
	else if(i == 3)
		return 1;
}
int JsGababo::loseSkill(int i)
{
	if(i == 1)
		return 3;
	else if(i == 2)
		return 1;
	else if(i == 3)
		return 2;
}
int JsGababo::drawSkill(int i)
{
	return i;
}