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

static CCPoint _0winPosition = ccp(40, 32.5);
static CCPoint _1winPosition = ccp(40.f + 190.f / 3.f * 1, 32.5);
static CCPoint _2winPosition = ccp(40.f + 190.f / 3.f * 2, 32.5);
static CCPoint _3winPosition = ccp(40.f + 190.f / 3.f * 3, 32.5);
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
	
	CCSpriteBatchNode* back_batch = CCSpriteBatchNode::create("ingame_side_pattern.png");
	back_batch->setPosition(ccp(240,160));
	addChild(back_batch);
	
	CCPoint base_position = ccp(-284,-180);
	
	for(int i=0;i*26 < 360;i++)
	{
		for(int j=0;j*48 < 568;j++)
		{
			CCSprite* t_back = CCSprite::createWithTexture(back_batch->getTexture());
			t_back->setAnchorPoint(ccp(0,0));
			t_back->setPosition(base_position + ccp(j*48, i*26));
			back_batch->addChild(t_back);
		}
	}
	
//	auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	auto back = CCSprite::create("ingame_back2.png");
//	back->setContentSize(CCSizeMake(357, 308));
	back->setPosition(ccp(240, 160));
	m_back = back;
	addChild(back);

	KSLabelTTF* titleLbl = KSLabelTTF::create("보너스 게임", mySGD->getFont().c_str(), 14);
	titleLbl->setPosition(ccp(84.0, 301.0));
	back->addChild(titleLbl);
	setFormSetter(titleLbl);
	
	auto realFront = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	m_realFront = realFront;
	realFront->setContentSize(CCSizeMake(548 / 2.f, 506 / 2.f));
	realFront->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -3.0));
	back->addChild(realFront);
	setFormSetter(realFront);

	auto front = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	front->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -95 - 12.5 + 19.5f));
	front->setContentSize(CCSizeMake(538 / 2.f, 156 / 2.f));
	back->addChild(front);
	m_front1 = front;
	auto tuto1 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_gababoContent1),
										mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	setFormSetter(tuto1);
	tuto1->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 37.5));
	front->addChild(tuto1);
	
	auto meChar = KS::loadCCBI<CCSprite*>(this, "gababo_me.ccbi");
	meChar.first->setPosition(ccp(50.5, 173.5));
	m_meCharSprite = meChar.first;
	meManager = meChar.second;
	back->addChild(meChar.first, 3);
	setFormSetter(meChar.first);
	
	auto npcChar = KS::loadCCBI<CCSprite*>(this, "gababo_you.ccbi");
	back->addChild(npcChar.first, 3);
	m_npcCharSprite = npcChar.first;
	npcManager = npcChar.second;
	npcChar.first->setPosition(ccp(261.0, 173.5));
	setFormSetter(npcChar.first);
	
	auto onSelection = [=]() // 바위 가위 보 셋중 하나 눌렀을 때~
	{
		contextSwitching(m_front1, m_front2, bind(&JsGababo::showHandsMotionWrapper, this), nullptr);
	};
	auto baBox = CommonButton::create("", 1.f, CCSizeMake(166 / 2.f, 124 / 2.f), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), touchPriority);
	
	m_ba = baBox;
	baBox->setPosition(ccp(49.0, 33.0));
	auto ba = CCSprite::create("ba.png");
	ba->setRotation(-90);
	ba->setPosition(ccpFromSize(baBox->getContentSize()) / 2.f + ccp(0, 10.5f - 9.5f + 8));
	ba->setScale(0.5f);
	baBox->addChild(ba, 10);
	baBox->setFunction([=](CCObject*){
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackBa;
		onSelection();
	});
	front->addChild(baBox);
	auto gaBox = CommonButton::create("", 1.f, CCSizeMake(166 / 2.f, 124 / 2.f), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), touchPriority);
	m_ga = gaBox;
	
	gaBox->setPosition(ccp(141.5, 33.0));
	auto ga = CCSprite::create("ga.png");
	ga->setRotation(-90);
	ga->setPosition(ccpFromSize(gaBox->getContentSize()) / 2.f + ccp(0, 10.5f - 6.5f - 4));
	ga->setScale(0.5f);
	gaBox->addChild(ga, 10);
	gaBox->setFunction([=](CCObject*){
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackGa;
		onSelection();
	});
	front->addChild(gaBox);
	auto boBox = CommonButton::create("", 1.f, CCSizeMake(166 / 2.f, 124 / 2.f), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), touchPriority);
	boBox->setPosition(ccp(229.5, 31.0));
	auto bo = CCSprite::create("bo.png");
	m_bo = boBox;
	bo->setRotation(-90);
	bo->setPosition(ccpFromSize(boBox->getContentSize()) / 2.f + ccp(0, 10.5f - 6.5f));
	bo->setScale(0.5f);
	boBox->addChild(bo, 10);
	boBox->setFunction([=](CCObject*){
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackBo;
		onSelection();
	});
	front->addChild(boBox);
	
	setFormSetter(m_ba);
	setFormSetter(m_ga);
	setFormSetter(m_bo);
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
	
	setupHands();
	loadImage(1);
	
	// 튜토 보여줬다면
	if(myDSH->getIntegerForKey(kDSH_Key_isShowGababoTutorial) && false)
	{
		m_front1->setScaleY(1.f);

	}
	else
	{
		setupTutorial();
		myDSH->setIntegerForKey(kDSH_Key_isShowGababoTutorial, 1);
	}
	return true;
}

void JsGababo::setupReward()
{

	auto front2 = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	front2->setPosition(ccpFromSize(m_back->getContentSize()) / 2.f + ccp(0, -95 - 12.5 + 19.5f));
	front2->setContentSize(CCSizeMake(538 / 2.f, 156 / 2.f));
	m_back->addChild(front2);
	
	m_front2 = front2;
	
	
	auto rewardTitleLbl = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_gababoContent2),
												 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
//	rewardTitleLbl->setAnchorPoint(ccp(0.5f, 0.5f));
	
	rewardTitleLbl->setPosition(ccpFromSize(m_front2->getContentSize()) / 2.f + ccp(0, 37.5f));
	m_front2->addChild(rewardTitleLbl);
	
	setFormSetter(rewardTitleLbl);
	auto addReward = [=](const std::string& lbl, int rewardIndex)->pair<CCNode*, CCNode*>
	{
		
		CCSprite* _1winBox = CCSprite::create("gababo_box.png");
		front2->addChild(_1winBox);
		
//		std::map<int, std::string> tagImageStr;
//		tagImageStr[0] = "gababo_normal.png";
//		tagImageStr[1] = "gababo_bronze.png";
//		tagImageStr[2] = "gababo_silver.png";
//		tagImageStr[3] = "gababo_gold.png";
//		
//		CCSprite* _1winTag = CCSprite::create(tagImageStr[rewardIndex].c_str());
//		_1winTag->setPosition(ccp(37.5, 39.0)); 			// dt (0.0, 1.5)
//		_1winTag->setPosition(ccpFromSize(_1winBox->getContentSize()) / 2.f + ccp(0, 0));
//		
//		_1winBox->addChild(_1winTag);
		KSLabelTTF* _1winTagLbl = KSLabelTTF::create(lbl.c_str(), mySGD->getFont().c_str(), 12.f);
		_1winTagLbl->setPosition(ccpFromSize(_1winBox->getContentSize()) / 2.f + ccp(0, 25));
		front2->addChild(_1winTagLbl, 1);
		
		auto _1winReward = CCSprite::create(m_rewards[rewardIndex].spriteName.c_str());
		_1winReward->setScale(0.7f);
		_1winReward->setPosition(ccpFromSize(_1winBox->getContentSize()) / 2.f + ccp(0, -4.0));
		_1winBox->addChild(_1winReward);
		auto _1winRewardLbl = KSLabelTTF::create(m_rewards[rewardIndex].desc.c_str(), mySGD->getFont().c_str(), 11.f);
		_1winRewardLbl->setScale(1.f/0.7f);
		_1winRewardLbl->setPosition(ccpFromSize(_1winReward->getContentSize()) / 2.f + ccp(0, -6.0));
		_1winReward->addChild(_1winRewardLbl);
		return make_pair(_1winBox, _1winTagLbl);
	};
	auto _0 = addReward(myLoc->getLocalForKey(kMyLocalKey_gababoContent3), 0);
	auto _1 = addReward(myLoc->getLocalForKey(kMyLocalKey_gababoContent4), 1);
	auto _2 = addReward(myLoc->getLocalForKey(kMyLocalKey_gababoContent5), 2);
	auto _3 = addReward(myLoc->getLocalForKey(kMyLocalKey_gababoContent6), 3);
	CCNode* _0winBox = _0.first;
	CCNode* _1winBox = _1.first;
	CCNode* _2winBox = _2.first;
	CCNode* _3winBox = _3.first;
	_0winBox->setPosition(_0winPosition); 			// dt (140.5, 45.0)
	_1winBox->setPosition(_1winPosition);
	_2winBox->setPosition(_2winPosition);
	_3winBox->setPosition(_3winPosition);
	
	CCNode* _0winTagLbl = _0.second;
	_0winTagLbl->setPosition(_0winPosition + ccp(0, 20)); 			// dt (40.5, 3.0)
	CCNode* _1winTagLbl = _1.second;
	_1winTagLbl->setPosition(_1winPosition + ccp(0, 20)); 			// dt (114.5, 3.0)
	CCNode* _2winTagLbl = _2.second;
	_2winTagLbl->setPosition(_2winPosition + ccp(0, 20)); 			// dt (114.5, 3.0)
	CCNode* _3winTagLbl = _3.second;
	_3winTagLbl->setPosition(_3winPosition + ccp(0, 20)); 			// dt (114.5, 3.0)
	setFormSetter(_0winTagLbl);
	setFormSetter(_1winTagLbl);
	setFormSetter(_2winTagLbl);
	setFormSetter(_3winTagLbl);
	m_currentRewardCursor = CCSprite::create("gababo_select.png");
	m_currentRewardCursor->setPosition(_0winPosition);
	front2->addChild(m_currentRewardCursor);
	setFormSetter(rewardTitleLbl);
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
		if(from)
		{
			from->setScaleY(t);
		}
	}, [=](float t){
		if(from)
		{
			from->setScaleY(t);
		}
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
	meManager->runAnimationsForSequenceNamed("gababo_start");
	meManager->setAnimationCompletedCallbackLambda(this, [=](const char* str){
		if(std::string("gababo_start") == str) {
			if(endLeft) {
				endLeft();
			}
		}
	});
	npcManager->runAnimationsForSequenceNamed("gababo_start");
	
	npcManager->setAnimationCompletedCallbackLambda(this, [=](const char* str){
		if(std::string("gababo_start") == str) {
			if(endRight) {
				endRight();
			}
		}
	});
	return;
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
	auto front = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	front->setPosition(ccpFromSize(m_back->getContentSize()) / 2.f + ccp(0, -95 - 12.5 + 19.5f));
	front->setContentSize(CCSizeMake(538 / 2.f, 156 / 2.f));
	m_back->addChild(front);

	m_front3 = front;
	
	StyledLabelTTF* message = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_gababoContent7),
													 mySGD->getFont().c_str(),
													 12.f, 999, StyledAlignment::kCenterAlignment);
	m_message = message;
	
	message->setAnchorPoint(ccp(0.5f, 0.5f));
	message->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(-29, 0));
	front->addChild(message);
	
	CommonButton* button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_gababoContent8), 12.f, CCSizeMake(69, 46), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), m_touchPriority - 1);
	
	m_confirmButton = button;
	button->setFunction(bind(&JsGababo::onPressConfirm, this, std::placeholders::_1));
//	button->setTitleColor(ccc3(37, 15, 0));
//	button->setTitleColorForDisable(ccc3(37, 15, 0));
	button->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(128 - 34, 0));
	front->addChild(button);
	
	setFormSetter(message);
	setFormSetter(button);
	m_front3->setScaleY(0.f);
}

void JsGababo::setupHands()
{
	float timeMul = 2.f;
}
void JsGababo::onPressConfirm(CCObject* t)
{
	if(
	((CommonButton*)t)->isEnabled() == false)
		return;
	
	CCLOG("%s", m_currentJudge.c_str());
	((CommonButton*)t)->setEnabled(false);
	if(m_front3->getScaleY() <= 0.5f)
		return;
	
	meManager->runAnimationsForSequenceNamed("Default Timeline");
	npcManager->runAnimationsForSequenceNamed("Default Timeline");
	m_leftHand->removeFromParent();
	m_rightHand->removeFromParent();
	meManager->runAnimationsForSequenceNamed("Default Timeline");
	auto rollBack = [=](){
		addChild(KSGradualValue<float>::create(0, 1.f, 1.f, [=](float t){
			
		}, [=](float t){
			if(m_currentJudge == "draw")
			{
				m_ba->setEnabled(true);
				m_ga->setEnabled(true);
				m_bo->setEnabled(true);
			}
		}));
		
	};
	if(m_currentJudge == "draw")
	{
		m_ba->setEnabled(false);
		m_ga->setEnabled(false);
		m_bo->setEnabled(false);
		//addChild(KSTimer::create(2.0f, [=](){
			contextSwitching(m_front3, m_front1, rollBack, [=](){
			});
		//}));
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
			m_ba->setEnabled(false);
			m_ga->setEnabled(false);
			m_bo->setEnabled(false);
			contextSwitching(m_front3, m_front1, [=](){
				rollBack();
				auto lightPair = KS::loadCCBI<CCSprite*>(this, "gababo_change.ccbi");
				lightPair.first->setScale(0.8f);
				CCSprite* light = lightPair.first;
				lightPair.second->setAnimationCompletedCallbackLambda(this, [=](const char* seqName){
					light->removeFromParent();
					m_ba->setEnabled(true);
					m_ga->setEnabled(true);
					m_bo->setEnabled(true);
				});
				//				light->setPosition(ccp(240, 195));
				light->setPosition(m_stepSprite->getPosition());
				m_back->addChild(light, 100);
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
		m_leftHand->setPosition(ccp(56.0, 9.5));
		m_meCharSprite->addChild(m_leftHand, 3);
//		m_leftHand->setPosition(t1);
		setFormSetter(m_leftHand);
	}, [=](){
		// m_winCount : 0, 1, 2
		int computer = 1;
		std::map<int, std::function<int(int)>> functor;
		functor[0] = bind(&JsGababo::loseSkill, this, std::placeholders::_1);
		functor[1] = bind(&JsGababo::winSkill, this, std::placeholders::_1);
		functor[2] = bind(&JsGababo::drawSkill, this, std::placeholders::_1);
		if(m_winCount == 0)
		{
			// 이길 확률 70 % 질 확률 15% 비길 확률 50%
//			ProbSelector ps = {70.f, 15.f, 50.f};
			ProbSelector ps = {70.f, 100000.f, 50.f};
			computer = functor[ps.getResult()](m_mySelection);
		}
		else if(m_winCount == 1)
		{
			// 이길 확률 60% 질 확률 20% 비길 확률 60%
			ProbSelector ps = {60.f, 20.f, 60.f};
			computer = functor[ps.getResult()](m_mySelection);
		}
		else if(m_winCount == 2)
		{
			// 이길 확률 50 % 질 확률 25% 비길 확률 30%
			ProbSelector ps = {50.f, 25.f, 30.f};
			computer = functor[ps.getResult()](m_mySelection);
		}
		
		int D = m_mySelection - computer % 3;
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
		m_npcCharSprite->addChild(m_rightHand, 3);
		m_rightHand->setScaleX(-1.0);
		m_rightHand->setPosition(ccp(-54.5, 10.0));
//		m_rightHand->setPosition(t1);
		setFormSetter(m_rightHand);
		
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
				m_message->setStringByTag(myLoc->getLocalForKey(kMyLocalKey_gababoContent10));
//				m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29 - 6, 10 + 6.5 - 3));
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					m_confirmButton->setEnabled(true);
					CCSprite* result_stamp = CCSprite::create("gababo_draw.png");
					m_resultStamp = result_stamp;
					//					result_stamp->setRotation(-15);
					m_back->addChild(result_stamp, 3);
					result_stamp->setPosition(ccp(m_back->getContentSize().width / 2.f, 190));
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
					m_message->setStringByTag(myLoc->getLocalForKey(kMyLocalKey_gababoContent11));
//					m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 8.f - 6));
				}
				else
				{
					m_message->setStringByTag(myLoc->getLocalForKey(kMyLocalKey_gababoContent12)
											  );
//					m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29 - 4.5, 15.f + 21.f));
				}
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					m_confirmButton->setEnabled(true);
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
					result_stamp->setPosition(ccp(m_back->getContentSize().width / 2.f, 190));
					
					showEffect(result_stamp);
				});
				m_currentJudge = "win";
				
			}
			else // Lose
			{
				CCLOG("lose~");
				m_message->setStringByTag(myLoc->getLocalForKey(kMyLocalKey_gababoContent13));
//				m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 8.f));
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					m_confirmButton->setEnabled(true);
					CCSprite* result_stamp = CCSprite::create("endless_loser.png");
					m_resultStamp = result_stamp;
//					result_stamp->setRotation(-15);
					m_back->addChild(result_stamp, 3);
					result_stamp->setPosition(ccp(m_back->getContentSize().width / 2.f, 190));
					showEffect(result_stamp);
				});
				m_currentJudge = "lose";
				
			}
		}));
	});
	
}
void JsGababo::showResult()
{
	BonusGameReward gr1 = m_rewards[m_winCount];
	//	gr1.spriteName = "shop_ruby2.png";
	//	gr1.desc = myLoc->getLocalForKey(kMyLocalKey_gababoContent14);
	auto tempEndFunction = m_endFunction;
	auto winCount = m_winCount;
	CurtainNodeForBonusGame* curtain = CurtainNodeForBonusGame::createForEnding((int)Curtain::kTouchPriority, gr1,
																				[=](){
																					removeFromParent();
																				},
																				[=](){
																					if(tempEndFunction)
																					{
																						tempEndFunction(winCount);
																					}
																				});
	getParent()->addChild(curtain, (int)Curtain::kCurtain);
	
}
void JsGababo::setupTutorial()
{
	CCSprite* helper = CCSprite::create("kt_cha_hibari_1.png");
	helper->setPosition(ccp(79.0, 166.5));
	m_back->addChild(helper, 5);
	setFormSetter(helper);
	
	auto puppleInner = CCScale9Sprite::create("kt_talkbox_purple.png",
																						CCRectMake(0, 0, 170.f/2.f, 230/2.f),
																						CCRectMake(30, 115 - 30, 2, 2));
	
	setFormSetter(puppleInner);
	puppleInner->setPosition(ccp(199.5, 189.0));
	puppleInner->setScaleX(-1.0); 			// dt -2.0
	puppleInner->setContentSize(CCSizeMake(205.0, 177.5));
	m_back->addChild(puppleInner, 5);
	
	StyledLabelTTF* message1 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_gababoContent15),
													 mySGD->getFont().c_str(),
													 12.f, 999, StyledAlignment::kCenterAlignment);
	//	m_message = message;
	message1->setAnchorPoint(ccp(0.5f, 0.5f));
	message1->setPosition(ccp(207.5, 189.2));
	m_back->addChild(message1, 6);

	CommonButton* button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_gababoContent16), 12.f, CCSizeMake(69, 46), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), m_touchPriority - 1);
	button->setTouchPriority(m_touchPriority - 1);
	button->setFunction([=](CCObject*){
		if(button->isEnabled() == false)
			return;
		
		CCSprite* tutoGababo;
		if(m_tutorialStep == 1)
		{
			message1->setStringByTag(myLoc->getLocalForKey(kMyLocalKey_gababoContent17)
									);
			CCSprite* tutoGababo;
			tutoGababo = CCSprite::create("gababo_sum.png");
			tutoGababo->setPosition(ccp(209.5, 217.0));
			m_tutoGababo = tutoGababo;
			m_back->addChild(tutoGababo, 6);
			setFormSetter(tutoGababo);
		}
		else
		{
			button->setEnabled(false);
			// 여기
			addChild(KSGradualValue<float>::create(255.f, 0.f, 1.f, [=](float t){
				helper->setOpacity(t);
				message1->setVisible(false);
				button->setOpacity(t);
				puppleInner->setOpacity(t);
				m_tutoGababo->setOpacity(t);
			}, [=](float t){
				helper->setVisible(false);
				button->setVisible(false);
				puppleInner->setVisible(false);
				m_tutoGababo->setOpacity(t);
			}));
			this->contextSwitching(nullptr, m_front1, nullptr, nullptr);
		}
		m_tutorialStep++;
	});
//	button->getTitleLabel()->setColor(ccc3(37, 15, 0));
	button->setPosition(ccp(237.0, 54.0));
	m_realFront->addChild(button);
	
	setFormSetter(message1);
	setFormSetter(button);
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
