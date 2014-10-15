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
#include "FlagSelector.h"
#include "TouchSuctionLayer.h"

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
	cardNo=mySD->getSilType();
	m_resultStamp = nullptr;
	m_resultParticle = nullptr;
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
	auto suction = TouchSuctionLayer::create(touchPriority);
	addChild(suction);
	suction->setTouchEnabled(true);

	
	startFormSetter(this);
	m_touchPriority = touchPriority;
	m_endFunction = endFunction;
	m_rewards = rewards;
	
	float height_value = 320.f;
	if(myDSH->screen_convert_rate < 1.f)
		height_value = 320.f/myDSH->screen_convert_rate;
	
	if(height_value < myDSH->ui_top)
		height_value = myDSH->ui_top;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	CCSprite* dimmed_sprite = CCSprite::create("whitePaper.png");
	dimmed_sprite->setPosition(ccp(240, myDSH->ui_center_y));
	dimmed_sprite->setColor(ccc3(0, 0, 0));
	dimmed_sprite->setOpacity(100);
	dimmed_sprite->setScaleX(screen_scale_x*480.f);
	dimmed_sprite->setScaleY(height_value);// /myDSH->screen_convert_rate));
//	t_popup->addChild(dimmed_sprite, -1);
//	C
//	CCSprite* back_img = CCSprite::create("main_back.png");
//	back_img->setPosition(ccp(240,160));
	addChild(dimmed_sprite, 0);
	
	
//	auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	auto back = CCSprite::create("ingame_back2.png");
//	back->setContentSize(CCSizeMake(357, 308));
	back->setPosition(ccp(240, 160));
	m_back = back;
	addChild(back);

	KSLabelTTF* titleLbl = KSLabelTTF::create("보너스 게임", mySGD->getFont().c_str(), 15);
	titleLbl->disableOuterStroke();
	titleLbl->setPosition(ccp(84.0, 301.0));
	back->addChild(titleLbl);
	setFormSetter(titleLbl);
	
	CommonAnimation::applyShadow(titleLbl);

	
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
	auto tuto1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent1),
										mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	setFormSetter(tuto1);
	tuto1->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 37.5 - 2.f));
	front->addChild(tuto1);
	
	KSLabelTTF* playerLbl = KSLabelTTF::create("PLAYER", mySGD->getFont().c_str(), 12.f);
	playerLbl->setColor(ccc3(255, 255, 0));
	realFront->addChild(playerLbl);
	playerLbl->setPosition(ccp(40.0, 205.0)); 			// dt (40.0, 205.0)
	
	string flag = myDSH->getStringForKey(kDSH_Key_flag);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
	realFront->addChild(selectedFlagSpr);
	selectedFlagSpr->setPosition(ccp(40.0, 228.0)); 			// dt (40.0, 228.0)
	
	KSLabelTTF* npcLbl = KSLabelTTF::create("NPC", mySGD->getFont().c_str(), 12.f);
	npcLbl->setColor(ccc3(255, 255, 0));
	realFront->addChild(npcLbl);
	npcLbl->setPosition(ccp(235.0, 205.0)); 			// dt (235.0, 205.0)
	
	CCSprite* npcFlag = CCSprite::create("gababo_npc.png");
	realFront->addChild(npcFlag);
	npcFlag->setPosition(ccp(235.0, 228.0)); 			// dt (235.0, 228.0)

	CCSprite* nameBar1 = CCSprite::create("gababo_namebar.png");
	realFront->addChild(nameBar1);
	nameBar1->setPosition(ccp(39.0, 96.0));

	KSLabelTTF* nick1 = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 11.f);
	nameBar1->addChild(nick1);
	nick1->setPosition(ccpFromSize(nameBar1->getContentSize()) / 2.f);
	
	
	CCSprite* nameBar2 = CCSprite::create("gababo_namebar.png");
	realFront->addChild(nameBar2);
	nameBar2->setPosition(ccp(234.5, 96.0));
	
	KSLabelTTF* nick2 = KSLabelTTF::create("묵지빠마왕", mySGD->getFont().c_str(), 11.f);
	nameBar2->addChild(nick2);
	nick2->setPosition(ccpFromSize(nameBar2->getContentSize()) / 2.f);
	
	setFormSetter(nameBar1);
	setFormSetter(nameBar2);
	setFormSetter(playerLbl);
	setFormSetter(selectedFlagSpr);
	setFormSetter(npcLbl);
	setFormSetter(npcFlag);
	
	m_willToggleObjects.push_back(nameBar1);
	m_willToggleObjects.push_back(nick1);
	m_willToggleObjects.push_back(playerLbl);
	m_willToggleObjects.push_back(selectedFlagSpr);
	m_willToggleObjects.push_back(nameBar2);
	m_willToggleObjects.push_back(nick2);
	m_willToggleObjects.push_back(npcLbl);
	m_willToggleObjects.push_back(npcFlag);
	
	auto meChar = KS::loadCCBI<CCSprite*>(this, "gababo_me.ccbi");
	meChar.first->setPosition(ccp(50.5, 173.5));
	m_meCharSprite = meChar.first;
	meManager = meChar.second;
	back->addChild(meChar.first, 3);
	setFormSetter(meChar.first);
	m_willToggleObjects.push_back(meChar.first);
	
	auto npcChar = KS::loadCCBI<CCSprite*>(this, "gababo_you.ccbi");
	back->addChild(npcChar.first, 3);
	m_npcCharSprite = npcChar.first;
	npcManager = npcChar.second;
	npcChar.first->setPosition(ccp(261.0, 173.5));
	setFormSetter(npcChar.first);
	m_willToggleObjects.push_back(npcChar.first);
	
	auto onSelection = [=]() // 바위 가위 보 셋중 하나 눌렀을 때~
	{
		contextSwitching(m_front1, m_front2, bind(&JsGababo::showHandsMotionWrapper, this), nullptr);
	};
	auto baBox = CommonButton::create("", 1.f, CCSizeMake(83, 58), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), touchPriority);
	
	m_ba = baBox;
	baBox->setPosition(ccp(49.0, 33.0 - 2.5));
	auto ba = CCSprite::create("ba.png");
	ba->setRotation(-90);
	ba->setPosition(ccpFromSize(baBox->getContentSize()) / 2.f + ccp(0, 10.5f - 9.5f + 8 - 3.f));
	ba->setScale(0.75f);
	baBox->addChild(ba, 10);
	baBox->setFunction([=](CCObject*){
		if(baBox->isEnabled() == false)
			return;
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		baBox->setEnabled(false);
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackBa;
		onSelection();
	});
	front->addChild(baBox);
	auto gaBox = CommonButton::create("", 1.f, CCSizeMake(83, 58), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), touchPriority);
	m_ga = gaBox;
	
//	gaBox->setPosition(ccp(141.5, 33.0));
	gaBox->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, -8.5f));
	auto ga = CCSprite::create("ga.png");
	ga->setRotation(-90);
	ga->setPosition(ccpFromSize(gaBox->getContentSize()) / 2.f + ccp(0, -6.5f + 5.f));
	ga->setScale(0.75f);
	gaBox->addChild(ga, 10);
	gaBox->setFunction([=](CCObject*){
		if(gaBox->isEnabled() == false)
			return;
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		gaBox->setEnabled(false);
		if(m_front1->getScaleY() <= 0.5f)
			return;
		m_mySelection = kAttackGa;
		onSelection();
	});
	front->addChild(gaBox);
	auto boBox = CommonButton::create("", 1.f, CCSizeMake(83, 58), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), touchPriority);
	boBox->setPosition(ccp(219.5, 30.5));
	auto bo = CCSprite::create("bo.png");
	m_bo = boBox;
	bo->setRotation(-90);
	bo->setPosition(ccpFromSize(boBox->getContentSize()) / 2.f + ccp(0, 10.5f - 6.5f - 3.5f));
	bo->setScale(0.75f);
	boBox->addChild(bo, 10);
	boBox->setFunction([=](CCObject*){
		if(boBox->isEnabled() == false)
			return;
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		boBox->setEnabled(false);
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
	m_willToggleObjects.push_back(stepFrame);
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
	loadImage(0);
	
	// 튜토 보여줬다면
	if(myDSH->getIntegerForKey(kDSH_Key_isShowGababoTutorial))
	{
		m_front1->setScaleY(1.f);

	}
	else
	{
		setupTutorial();
		myDSH->setIntegerForKey(kDSH_Key_isShowGababoTutorial, 1);
		
		for(auto i : m_willToggleObjects)
		{
			i->setVisible(false);
			i->setOpacity(0);
		}
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
	
	
	auto rewardTitleLbl = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent2),
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
	auto _0 = addReward(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent3), 0);
	auto _1 = addReward(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent4), 1);
	auto _2 = addReward(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent5), 2);
	auto _3 = addReward(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent6), 3);
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
	step+=1;
	if(!cardNo)cardNo=mySD->getSilType();
	if(m_stepSprite != nullptr)
		m_stepSprite->removeFromParent();
	m_stepSprite = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",
																  NSDS_GI(cardNo, kSDS_SI_level_int1_card_i, step))->getCString());
	
	m_willToggleObjects.push_back(m_stepSprite);
	// CCSprite::create(boost::str(boost::format("ga%||.png") % step).c_str());
	m_stepSprite->setScale(m_stepFrame->getContentSize().height / m_stepSprite->getContentSize().height);
	m_stepSprite->setScale(m_stepSprite->getScale());
	m_stepSprite->setPosition(m_stepFrame->getPosition());
	m_back->addChild(m_stepSprite, 1);
	
//	if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, NSDS_GI(cardNo, kSDS_SI_level_int1_card_i, step)))
//	{
//		CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, NSDS_GI(cardNo, kSDS_SI_level_int1_card_i, step))).first;
//		ccb_img->setPosition(ccp(160,215));
//		m_stepSprite->addChild(ccb_img);
//	}
	
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
	
	StyledLabelTTF* message = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent7),
													 mySGD->getFont().c_str(),
													 12.f, 999, StyledAlignment::kCenterAlignment);
	m_message = message;
	
	message->setAnchorPoint(ccp(0.5f, 0.5f));
	message->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(-29, 0));
	front->addChild(message);
	
	CommonButton* button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent8), 12.f, CCSizeMake(69, 46), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0,0,62,32), CCRectMake(30, 15, 2, 2)), m_touchPriority - 1);
	
	m_confirmButton = button;
	m_confirmButton->setEnabled(false);

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
	m_confirmButton->setEnabled(false);

	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
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
				m_ga->setEnabled(true);
				m_ba->setEnabled(true);
				m_bo->setEnabled(true);
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
				
				AudioEngine::sharedInstance()->playEffect("sg_mg_light.mp3");
				auto lightPair = KS::loadCCBI<CCSprite*>(this, "gababo_change.ccbi");
//				lightPair.first->setScale(0.8f);
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
			}, [=](){
//				m_ga->setEnabled(true);
//				m_ba->setEnabled(true);
//				m_bo->setEnabled(true);
			});
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
	if(m_resultParticle)
	{
		m_resultParticle->removeFromParent();
		m_resultParticle = nullptr;
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
			ProbSelector ps = {40, 30, 30};
//			ProbSelector ps = {70.f, 100000.f, 50.f};
			computer = functor[ps.getResult()](m_mySelection);
		}
		else if(m_winCount == 1)
		{
			// 이길 확률 60% 질 확률 20% 비길 확률 60%
			ProbSelector ps =  {30,40,30};
			computer = functor[ps.getResult()](m_mySelection);
		}
		else if(m_winCount == 2)
		{
			// 이길 확률 50 % 질 확률 25% 비길 확률 30%
			ProbSelector ps = {20, 60, 20};
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
					if(m_resultStamp)
					{
						KS::setOpacity(result_stamp, y0);
						result_stamp->setScale(y1);
					}

				}, [=](float t){
					float y0 = 255.f * t;
					float y1 = -1*t + 2;
					if(m_resultStamp)
					{
						KS::setOpacity(result_stamp, y0);
						result_stamp->setScale(y1);
					}

					
				}));
			};
			
			if(m_mySelection == computer) // Draw
			{
				m_message->setStringByTag(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent10));
//				m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29 - 6, 10 + 6.5 - 3));
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					m_confirmButton->setEnabled(true);
					CCSprite* result_stamp = CCSprite::create("gababo_draw.png");
					AudioEngine::sharedInstance()->playEffect("sg_mg_draw.mp3");
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
					m_message->setStringByTag(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent11));
//					m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 8.f - 6));
				}
				else
				{
					m_message->setStringByTag(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent12)
											  );
//					m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29 - 4.5, 15.f + 21.f));
				}
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					m_confirmButton->setEnabled(true);
					
					CCSprite* result_stamp = CCSprite::create("endless_winner.png");
					int random_value = rand()%3 + 1;
					AudioEngine::sharedInstance()->playEffect(CCString::createWithFormat("sg_mg_win%d.mp3", random_value)->getCString(),false);
					
					//AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3");
					// YH 코드.
					addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																								 {
																									 if(m_resultStamp)
																									 {
																										 KS::setOpacity(result_stamp, t*255);
																										 result_stamp->setScale(2.5f-t*1.5f);
																									 }

																								 }, [=](float t)
																								 {
																									 if(m_resultStamp)
																									 {
																										 KS::setOpacity(result_stamp, 255);
																										 result_stamp->setScale(1.f);
																									 }

																								 }));
					m_resultStamp = result_stamp;
					CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", m_winCount)->getCString(), "winfont.fnt");
					win_label->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f+10));
					result_stamp->addChild(win_label);
					
					CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode().c_str())->getCString());
					win_ment->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f-25));
					result_stamp->addChild(win_ment);
					
					result_stamp->setRotation(-15);
					m_back->addChild(result_stamp, 3);
					result_stamp->setPosition(ccp(m_back->getContentSize().width / 2.f, 190));
					{ // 처음에 강조하는 파티클, 좀 있다가 사라질 것.
						CCParticleSystemQuad* particle1 = CCParticleSystemQuad::createWithTotalParticles(100);
						particle1->setPositionType(kCCPositionTypeRelative);
						particle1->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
						particle1->setEmissionRate(300);
						particle1->setAngle(180.0);
						particle1->setAngleVar(180.0);
						ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
						particle1->setBlendFunc(blendFunc);
						particle1->setDuration(0.3f);
						particle1->setEmitterMode(kCCParticleModeGravity);
						particle1->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
						particle1->setStartColorVar(ccc4f(0,0,0,0.f));
						particle1->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
						particle1->setEndColorVar(ccc4f(0, 0, 0, 0.f));
						particle1->setStartSize(40.0);
						particle1->setStartSizeVar(10.0);
						particle1->setEndSize(0.0);
						particle1->setEndSizeVar(0.0);
						particle1->setGravity(ccp(0,-100));
						particle1->setRadialAccel(50.0);
						particle1->setRadialAccelVar(20.0);
						particle1->setSpeed(50);
						particle1->setSpeedVar(30.0);
						particle1->setTangentialAccel(0);
						particle1->setTangentialAccelVar(0);
						particle1->setTotalParticles(100);
						particle1->setLife(2.0);
						particle1->setLifeVar(0.5);
						particle1->setStartSpin(0.0);
						particle1->setStartSpinVar(0.f);
						particle1->setEndSpin(0.0);
						particle1->setEndSpinVar(0.f);
						particle1->setPosVar(ccp(90,90));
						particle1->setPosition(result_stamp->getPosition());
						particle1->setAutoRemoveOnFinish(true);
						m_back->addChild(particle1);
					}
					CCParticleSystemQuad* particle2;
					{ // 유지되는 파티클.
						particle2 = CCParticleSystemQuad::createWithTotalParticles(10);
						m_resultParticle = particle2;
						particle2->setPositionType(kCCPositionTypeRelative);
						particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
						particle2->setEmissionRate(80);
						particle2->setAngle(360.0);
						particle2->setAngleVar(0.0);
						ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
						particle2->setBlendFunc(blendFunc);
						particle2->setDuration(-1.0);
						particle2->setEmitterMode(kCCParticleModeGravity);
						particle2->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
						particle2->setStartColorVar(ccc4f(0,0,0,0.f));
						particle2->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
						particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
						particle2->setStartSize(25.0);
						particle2->setStartSizeVar(10.0);
						particle2->setEndSize(0.0);
						particle2->setEndSizeVar(0.0);
						particle2->setGravity(ccp(0,0));
						particle2->setRadialAccel(3.0);
						particle2->setRadialAccelVar(0.0);
						particle2->setSpeed(0);
						particle2->setSpeedVar(0.0);
						particle2->setTangentialAccel(0);
						particle2->setTangentialAccelVar(0);
						particle2->setTotalParticles(10);
						particle2->setLife(0.8);
						particle2->setLifeVar(0.25);
						particle2->setStartSpin(0.0);
						particle2->setStartSpinVar(50.f);
						particle2->setEndSpin(0.0);
						particle2->setEndSpinVar(60.f);
						particle2->setPosVar(ccp(80,80));
						particle2->setPosition(result_stamp->getPosition());
						m_back->addChild(particle2);
					}
	
					
					showEffect(result_stamp);
//					particle2->removeFromParent();
				});
				m_currentJudge = "win";
				
			}
			else // Lose
			{
				CCLOG("lose~");
				m_message->setStringByTag(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent13));
//				m_message->setPosition(ccpFromSize(m_front3->getContentSize()) / 2.f + ccp(-29, 8.f));
				this->contextSwitching(m_front2, m_front3, nullptr, [=](){
					m_confirmButton->setEnabled(true);
					CCSprite* result_stamp = CCSprite::create("endless_loser.png");
					AudioEngine::sharedInstance()->playEffect("sg_mg_fail.mp3");
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
	//	gr1.desc = myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent14);
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
	helper->setPosition(ccp(70.0, 160.5));
	helper->setScale(0.9f);
	m_back->addChild(helper, 5);
	setFormSetter(helper);
	
	auto puppleInner = CCScale9Sprite::create("kt_talkbox_purple.png",
																						CCRectMake(0, 0, 170.f/2.f, 230/2.f),
																						CCRectMake(30, 115 - 30, 2, 2));
	
	setFormSetter(puppleInner);
	puppleInner->setPosition(ccp(193.0, 193.0));
	puppleInner->setScaleX(-1.0); 			// dt -2.0
	puppleInner->setContentSize(CCSizeMake(205.0, 177.5));
	m_back->addChild(puppleInner, 5);
	
	StyledLabelTTF* message1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent15),
													 mySGD->getFont().c_str(),
													 12.f, 999, StyledAlignment::kCenterAlignment);
	//	m_message = message;
	message1->setAnchorPoint(ccp(0.5f, 0.5f));
	message1->setPosition(ccp(202.0, 195.7));
	m_back->addChild(message1, 6);

	CCScale9Sprite* button9Scale = nullptr;
	CommonButton* button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent16), 15.f, CCSizeMake(114.5, 63.5),
							button9Scale = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0,0,92,45), CCRectMake(45, 21, 2, 2)), m_touchPriority - 1);
//	button->setContentSize(CCSizeMake(92, 65));
//	button->setContentSize(CCSizeMake(120, 80));
	button->setContentSize(CCSizeMake(132 / 2.f, 100 / 2.f));
	// 92 x 45
	button->setTouchPriority(m_touchPriority - 1);
	button->setFunction([=](CCObject*){
		if(button->isEnabled() == false)
			return;
	
		
			
			
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
		CCSprite* tutoGababo;
		if(m_tutorialStep == 1)
		{
			message1->setStringByTag(myLoc->getLocalForKey(LK::kMyLocalKey_gababoContent17)
									);
			CCSprite* tutoGababo;
			tutoGababo = CCSprite::create("gababo_sum.png");
			tutoGababo->setPosition(ccp(202.0, 224.0));
			m_tutoGababo = tutoGababo;
			m_back->addChild(tutoGababo, 6);
			setFormSetter(tutoGababo);
		}
		else
		{
			button->setEnabled(false);
			// 여기
			addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t0){
				float t = 255 + -255.f * t0 / 1.f;
				float t2 = t0 * 255.f;
				helper->setOpacity(t);
				message1->setVisible(false);
				button->setOpacity(t);
				puppleInner->setOpacity(t);
				m_tutoGababo->setOpacity(t);
				
				for(auto iter : m_willToggleObjects)
				{
					iter->setOpacity(t2);
					iter->setVisible(true);
				}
			}, [=](float t0){
				float t = 255 + -255.f * t0 / 1.f;
				float t2 = t0 * 255.f;
				helper->setVisible(false);
				button->setVisible(false);
				puppleInner->setVisible(false);
				m_tutoGababo->setOpacity(t);
				for(auto iter : m_willToggleObjects)
				{
					iter->setOpacity(t2);
					iter->setVisible(true);
				}
			}));
			this->contextSwitching(nullptr, m_front1, nullptr, nullptr);
		}
		m_tutorialStep++;
	});
//	button->getTitleLabel()->setColor(ccc3(37, 15, 0));
	button->setPosition(ccp(185.0, 37.5));
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
SEL_CallFuncN JsGababo::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "firstsound", JsGababo::firstSound);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "secondsound", JsGababo::secondSound);
	return NULL;
}

void JsGababo::firstSound()
{
	CCLog("first Sound");
}
void JsGababo::secondSound()
{
	CCLog("second Sound"); 
}