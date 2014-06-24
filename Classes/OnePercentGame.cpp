#include "OnePercentGame.h"

#include "MyLocalization.h"
#include "CommonButton.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "ShopPopup.h"
#include "StyledLabelTTF.h"
#include <boost/format.hpp>
#include "FormSetter.h"
#include "EasingAction.h"
#include "StageImgLoader.h"
#include "FormSetter.h"
#include "CommonAnimation.h"

OnePercentGame::OnePercentGame()
{
	m_cursorDirection = 1;
	m_99State = 1; // 결제하기 직전.
	
	m_validSize = 50;
	m_touchEnable = true;
}
OnePercentGame::~OnePercentGame()
{
}
void OnePercentGame::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}

bool OnePercentGame::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	
	return true;
}

bool OnePercentGame::init()
{
	CCLayer::init();
	
	return true;
}


bool OnePercentGame::init(float originalPercent, std::function<void(void)> cancelGacha,
													std::function<void(float)> tryGacha, bool openEffect)
{
	CCLayer::init();
	startFormSetter(this);
	setTouchEnabled(true);
	m_validSize = 50;
	m_cancelGacha = cancelGacha;
	m_resultGacha = tryGacha;
	
	m_totalPercent = recent_percent = originalPercent;
	
	setTouchEnabled(true);
	
	m_grayBack = CCSprite::create("back_gray.png");
	m_grayBack->setPosition(ccp(240,myDSH->ui_center_y));
	m_grayBack->setScaleY(myDSH->ui_top/320.f);
	addChild(m_grayBack, kOnePercentGame_Z_gray);
	m_grayBack->setOpacity(0);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(m_container, kOnePercentGame_Z_back);
	
	back_img = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_img->setContentSize(CCSizeMake(330, 270));
	back_img->setPosition(ccp(0,0));
	m_container->addChild(back_img, kOnePercentGame_Z_back);
	
	KSLabelTTF* titleLbl = KSLabelTTF::create("보너스 영역 뽑기", mySGD->getFont().c_str(), 15.f);
	titleLbl->setPosition(ccp(.0, 112.5)); 			// dt (6.0, 112.5)
	titleLbl->setColor(ccc3(255, 170, 20));
	m_container->addChild(titleLbl, kOnePercentGame_Z_content);
//	titleLbl->setPosition(ccp(0, 40));
	setFormSetter(titleLbl);
	
	CCScale9Sprite* inner_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	inner_back->setContentSize(CCSizeMake(295, 180));
	inner_back->setPosition(ccp(0,10));
	m_container->addChild(inner_back, kOnePercentGame_Z_back);
	
	StyledLabelTTF* desc = StyledLabelTTF::create("<font color=999 size=11 newline=20>뽑기 기회는 단 3번!!!</font><font color=#FFAA14 size=11>부족한 영역을 채워 100% 에 도전하세요.</font>",
																								mySGD->getFont().c_str(), 99, 0, StyledAlignment::kCenterAlignment);
	desc->setOldAnchorPoint();
	m_container->addChild(desc, kOnePercentGame_Z_content);
	m_desc = desc;
	desc->setPosition(0, 40);

	StyledLabelTTF* desc3 = StyledLabelTTF::create("<font color=#FFAA14 size=13 newline=12>100% 에 성공시</font><font color=#FFAA14 size=13 tag=1>한단계 높은 카드를 획득할 수 있어요.</font>",
																								mySGD->getFont().c_str(), 99, 0, StyledAlignment::kCenterAlignment);
	m_container->addChild(desc3, kOnePercentGame_Z_content);
	setFormSetter(desc3);
	m_desc3 = desc3;
	
	StyledLabelTTF* currentGainArea = StyledLabelTTF::create("<font color=999 size=18>현재 획득영역  </font><font color=990 size=30 strokecolor=000 strokesize=2 tag=1>99.9%</font>", mySGD->getFont().c_str(), 99, 0, StyledAlignment::kCenterAlignment);
//	StyledLabelTTF* currentGainArea = StyledLabelTTF::create("<font color=#FFAA14 size=13 newline=12>100% 에 성공시</font><font color=#FFAA14 size=13 tag=1>한단계 높은 카드를 획득할 수 있어요.</font>", mySGD->getFont().c_str(), 99, 0, StyledAlignment::kCenterAlignment);
	currentGainArea->setOldAnchorPoint();
	m_currentGainArea = currentGainArea;
	currentGainArea->getLabelByTag(1)->setString(boost::str(boost::format("%|4.3|%%") % (recent_percent * 100.f)).c_str());
	currentGainArea->setPosition(ccp(0, 80));
	m_container->addChild(currentGainArea, kOnePercentGame_Z_content);

	CCSprite* tutorialBox = CCSprite::create("one_percent_gacha_tutorialbox.png");
	tutorialBox->setPosition(ccp(-0.5, -20.0)); 			// dt (-0.5, -20.0)
	m_tutorialBox = tutorialBox;
	m_toDown = [=]{
		addChild(KSGradualValue<CCPoint>::create(tutorialBox->getPosition(), tutorialBox->getPosition() + ccp(0, -10), 0.5f,
																					 [=](CCPoint t){
																						 tutorialBox->setPosition(t);
																					 }, [=](CCPoint t){
																						 tutorialBox->setPosition(t);
																						 m_toUp();
																					 }));
	};
	m_toUp = [=]{
		addChild(KSGradualValue<CCPoint>::create(tutorialBox->getPosition(), tutorialBox->getPosition() + ccp(0, 10), 0.5f,
																						 [=](CCPoint t){
																							 tutorialBox->setPosition(t);
																						 }, [=](CCPoint t){
																							 tutorialBox->setPosition(t);
																							 m_toDown();
																						 }));
	};
	
	m_toDown(); // 최초로 내림.
	
	setFormSetter(tutorialBox);
	StyledLabelTTF* command1 = StyledLabelTTF::create("<font size=12 color=#FFFFFF newline=12>분홍색 영역에 맞게</font>"
																										"<font size=12 color=#FFAA14>멈춰 버튼 터치</font><font size=12 color=#FFFFFF>하세요!</font>",
																										mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	command1->setPosition(ccpFromSize(tutorialBox->getContentSize()) / 2.f + ccp(0, 17));
	
	tutorialBox->addChild(command1);
	m_container->addChild(tutorialBox, kOnePercentGame_Z_content + 1);
	setFormSetter(command1);
	StyledLabelTTF* desc2 = StyledLabelTTF::create(
			CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_onePercentDesc2), 3, 0.3f)->getCString(),
			mySGD->getFont().c_str(), 12.f, 0, StyledAlignment::kCenterAlignment);
	m_container->addChild(desc2, kOnePercentGame_Z_content);
	desc2->setPosition(ccp(0.0,-36.5)); 			// dt (0.0,-36.5)
	m_desc2 = desc2;
	setFormSetter(desc2);
	
		
	CCSprite* progress_back = CCSprite::create("one_percent_gacha_graph_back.png");
	progress_back->setPosition(ccp(0,10));
	m_container->addChild(progress_back, kOnePercentGame_Z_content);
	progress_back->setPosition(ccp(0, -60));
	CCSprite* validArea = CCSprite::create();
	m_validArea = validArea;
	validArea->setTextureRect(CCRectMake(0, 0, m_validSize, 15));
	validArea->setPosition(ccp(0, -60));
	m_container->addChild(validArea, kOnePercentGame_Z_content);
	validArea->setColor(ccc3(232, 0, 192));
	
	
	m_cursor = CCSprite::create("one_percent_gacha_02.png");
	m_cursor->setPosition(ccp(0, -60));
	m_container->addChild(m_cursor, kOnePercentGame_Z_content);
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_go100percent), mySGD->getFont().c_str(), 13);
	gacha_label->setColor(ccBLACK);
	gacha_label->setPosition(ccp(0,15));
	t_label->addChild(gacha_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("normal_yellow_down.png", CCRectMake(0,0,34,34), CCRectMake(16,16,2,2));
	price_back->setContentSize(CCSizeMake(90, 34));
	price_back->setPosition(ccp(0, -10));
	t_label->addChild(price_back);
	
	if(mySGD->getGoodsValue(kGoodsType_pass5) > 0)
	{
		CCSprite* pass_ticket = CCSprite::create("pass_ticket5.png");
		pass_ticket->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
		price_back->addChild(pass_ticket);
		KSLabelTTF* free_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_free), mySGD->getFont().c_str(), 20);
		free_label->setColor(ccWHITE);
		free_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(free_label);
	}
	else
	{
		CCSprite* price_type = CCSprite::create("price_ruby_img.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGachaOnePercentFee())->getCString(), mySGD->getFont().c_str(), 20);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	
	
	CCScale9Sprite* gacha_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	CCNode* gachaNode = CCNode::create();
	gacha_button = CCControlButton::create(t_label, gacha_back);
	gachaNode->setScale(0.7f);
	gachaNode->addChild(gacha_button); // 스케일을 위해 하나 더 만듬.
	gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(OnePercentGame::gachaAction), CCControlEventTouchUpInside);
	gacha_button->setPreferredSize(CCSizeMake(170,65));
	gacha_button->setPosition(ccp(0, -145));
	m_container->addChild(gachaNode, kOnePercentGame_Z_content);
	gacha_button->setTouchPriority(-180);
	
	
	
	cancel_menu = CommonButton::createCloseButton();
	
	cancel_menu->setTouchPriority(-180);
	cancel_menu->setFunction([=](CCObject* sender) {
		if(m_cancelGacha) {
			removeFromParent();
			m_cancelGacha();
		}
		//CCNode* t_node = CCNode::create();
		//t_node->setTag(kOnePercentGame_MT_cancel);
		//menuAction(t_node);
	});
	cancel_menu->setPosition(ccp(143, 112));
	m_container->addChild(cancel_menu, kOnePercentGame_Z_content);
	CCFadeTo* gray_fade = CCFadeTo::create(0.25f, 255);
	m_grayBack->runAction(gray_fade);
	
	if(openEffect)
	{
		CommonAnimation::openPopup(this, m_container, nullptr);
	}
	
//	m_container->setScaleY(0.f);
//	
//	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
//		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
//			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
//	
//	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t)
//																			 {
//																				 KS::setOpacity(m_container, 255);
//																				 //endShow();
//																			 }));
	
	m_lblPercent = CCLabelTTF::create(boost::str(boost::format("%|4.3|") % m_totalPercent).c_str(), mySGD->getFont().c_str(),
																		14.f);
	m_lblPercent->setPosition(ccp(0, 0));
	m_container->addChild(m_lblPercent, kOnePercentGame_Z_content);
	
//	{
//		auto stencil = CCScale9Sprite::create("one_percent_gacha_06.png", CCRectMake(0, 0, 13, 176), CCRectMake(5, 5, 3, 165));
//		stencil->setContentSize(CCSizeMake(13, 100));
//		
//		m_container->addChild(stencil, kOnePercentGame_Z_content);
//	}
	CCNode* tempNode = CCNode::create();
	CCSprite* graphBack = CCSprite::create("one_percent_gacha_04.png");
	tempNode->addChild(graphBack);
	graphBack->setPosition(ccp(0.0, 0));
	CCClippingNode* cNode = CCClippingNode::create();
	m_closer = cNode;
	auto stencil = CCScale9Sprite::create("one_percent_gacha_06.png", CCRectMake(0, 0, 13, graphHeight), CCRectMake(5, 5, 3, 165));
	stencil->setContentSize(CCSizeMake(13, graphHeight));
	stencil->setAnchorPoint(ccp(0.5f, 0.0f));

	cNode->setStencil(stencil);
	m_stencil = stencil;
	CCSprite* gradient = CCSprite::create("one_percent_gacha_05.png");
	cNode->addChild(gradient);
	m_gradient = gradient;
	stencil->setPositionY(-gradient->getContentSize().height / 2.f);
	
	cNode->setAlphaThreshold(0.1f);
	tempNode->addChild(cNode, kOnePercentGame_Z_content);
	m_container->addChild(tempNode, kOnePercentGame_Z_content);
	tempNode->setPosition(ccp(61.5,22.5)); 			// dt (-33.5,0.0)
	
	KSLabelTTF* _100 = KSLabelTTF::create("100.0%", mySGD->getFont().c_str(), 14.f);
	_100->setColor(ccc3(255, 0, 0));
	tempNode->addChild(_100, kOnePercentGame_Z_content);
	KSLabelTTF* _99 = KSLabelTTF::create("99.0%", mySGD->getFont().c_str(), 14.f);
	_99->setColor(ccc3(255, 255, 0));
	tempNode->addChild(_99, kOnePercentGame_Z_content);
	_100->setPosition(ccp(32.0,60.0)); 			// dt (-61.0,-21.5)Cocos2d:
	_99->setPosition(ccp(32.0,-44.0)); 			// dt (-61.5,-22.0)Cocos2d:
	stencil->setContentSize(CCSizeMake(13, graphHeight * (m_totalPercent - 0.99f) * 100.f));
	m_graphNode = tempNode;
	m_graphNode->setVisible(false);
	tempNode->setPosition(ccp(59.5,30.5)); 			// dt (-2.0,8.0)
	tempNode->setScaleX(0.9); 			// dt -0.1
	tempNode->setScaleY(0.7); 			// dt -0.3
	_100->setPosition(ccp(32.0,73.5)); 			// dt (0.0,13.5)
	_100->setScaleX(1.0); 			// dt 0.0
	_100->setScaleY(1.4); 			// dt 0.4
	_99->setPosition(ccp(31.0,-75.0)); 			// dt (-1.0,-31.0)
	_99->setScaleX(1.0); 			// dt -0.0
	_99->setScaleY(1.4); 			// dt 0.4
	setFormSetter(_100);
	setFormSetter(_99);
	setFormSetter(tempNode);
	setFormSetter(gradient);
	setFormSetter(cNode);
	setFormSetter(graphBack);
	setFormSetter(stencil);
	
	this->scheduleUpdate();
	return true;
}
void OnePercentGame::gameUISetting()
{
	cancel_menu->removeFromParent();
	// 현재 획득 영역, 설명글 지우고
	addChild(KSGradualValue<float>::create(255, 0, 0.3f, [=](float t){
		KS::setOpacity(m_currentGainArea, t);
		KS::setOpacity(m_desc, t);
		KS::setOpacity(m_tutorialBox, t);
		KS::setOpacity(m_desc3, t);
	},
																				 [=](float t){
																					 m_currentGainArea->removeFromParent();
																					 m_desc->removeFromParent();
																				 }));
	
	CCNode* tempNode = CCNode::create();
	CCClippingNode* cNode = CCClippingNode::create();
	cNode->setStencil(CCSprite::create("one_percent_gacha_color.png"));
	
	int t_grade = 3;
	if(mySGD->is_exchanged)
		t_grade = 4;
	CCSprite* girl = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, t_grade))->getCString());
	if(girl == nullptr)
		girl = CCSprite::create();
	cNode->addChild(girl);
	girl->setScale(130 / girl->getContentSize().width);
	//		setFormSetter(girl);
	cNode->setAlphaThreshold(0.1f);
	tempNode->addChild(cNode, kOnePercentGame_Z_content);
	
	CCSprite* shutter = CCSprite::create("one_percent_gacha_box_02.png");
	m_shutter = shutter;
	cNode->addChild(shutter, kOnePercentGame_Z_content);
	
	CCSprite* imageBack = CCSprite::create("one_percent_gacha_box_01.png");
	tempNode->addChild(imageBack, kOnePercentGame_Z_content);
	
	CCScale9Sprite* gacha_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	CCLabelTTF* stopFnt = CCLabelTTF::create("멈춰!", mySGD->getFont().c_str(), 20.f);
	stopFnt->setColor(ccc3(22, 11, 0));
	//gacha_button->removeFromParent();
	gacha_button->getTitleLabel()->removeAllChildren();
	
	gacha_button->getTitleLabel()->addChild(stopFnt);
	m_container->addChild(tempNode, kOnePercentGame_Z_content);
	tempNode->setPosition(ccp(0, 30));
	tempNode->setScale(0.7f);
}
void OnePercentGame::gachaAction(CCObject* sender, CCControlEvent t_event)
{
	if(m_touchEnable == false)
		return;
	if(m_99State == 1)
	{
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		
		if(mySGD->getGoodsValue(kGoodsType_pass5) > 0)
		{
			myLog->addLog(kLOG_gacha_onePercent, -1);
			
			LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
			addChild(t_loading, 9999);
			
			mySGD->addChangeGoods("g_99_p");
			
			mySGD->changeGoods([=](Json::Value result_data){
				t_loading->removeFromParent();
				if(result_data["result"]["code"] == GDSUCCESS)
				{
					// 무료 아템으로 구매.
					gameUISetting();
					m_99State = 2;
					m_graphNode->setVisible(true);
				}
				else
				{
					mySGD->clearChangeGoods();
					addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
					//is_menu_enable = true;
				}
			});
		}
		else if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getGachaOnePercentFee())
		{
			myLog->addLog(kLOG_gacha_onePercent, -1);
			
			LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
			addChild(t_loading, 9999);
			
			mySGD->addChangeGoods("g_99_r");
			
			mySGD->changeGoods([=](Json::Value result_data){
				t_loading->removeFromParent();
				if(result_data["result"]["code"] == GDSUCCESS)
				{
					// 잼로 구매.
					gameUISetting();
					m_99State = 2;
					m_graphNode->setVisible(true);
				}
				else
				{
					mySGD->clearChangeGoods();
					addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
					//is_menu_enable = true;
				}
			});
		}
		else
		{
			ShopPopup* t_popup = ShopPopup::create();
			t_popup->setScale(myDSH->screen_convert_rate);
			t_popup->setShopCode(kSC_ruby);
			t_popup->setCloseFunc([=]()
														{
															//																is_menu_enable = true;
														});
			addChild(t_popup, kOnePercentGame_Z_shop);
			
		}
	}
	else
	{
		/*
		 멈춰! 버튼 클릭! 승리시 셔터가 올라감 2~3초 그리고 그사이에 하트가생성! 그후 1~2초 멈춘다음 다시 게임 플레이~ 플레이 동시에 2.3번째 게임시 포인트 영역이 작아져있음 (속도는 동일)
		 */
		if(m_99State == 2) // 첫번 째 시도.
		{
			int pos = m_cursor->getPositionX();
			unscheduleUpdate();
			m_99State = 3; // 두번 째 시도로 세팅.
			m_touchEnable = false;
			
			addChild(KSGradualValue<float>::create(m_validSize, m_validSize - 20, 1.f, [=](float t){
				m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
			}, [=](float t){
				
				m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
			}));
			
			addChild(KSTimer::create(1.5f, [=](){
				m_touchEnable = true;
			}));
			
			if(fabsf(pos) <= m_validSize / 2.f)
			{
				AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3", false);
				m_totalPercent += 0.003f;
				m_totalPercent = MIN(1.f, m_totalPercent);
//				auto stencil = CCScale9Sprite::create("one_percent_gacha_06.png", CCRectMake(0, 0, 13, graphHeight), CCRectMake(5, 5, 3, 165));
//				stencil->setContentSize(CCSizeMake(13, graphHeight * (m_totalPercent - 0.99f) * 100.f));
//				stencil->setAnchorPoint(ccp(0.5f, 0.0f));
//				stencil->setPositionY(-m_gradient->getContentSize().height / 2.f);
//				m_closer->setStencil(stencil);
				
				m_stencil->setContentSize(CCSizeMake(13, graphHeight * (m_totalPercent - 0.99f) * 100.f));
				if(m_totalPercent >= 1.f)
				{
					unscheduleUpdate();
					m_touchEnable = false;
					m_99State = 999;
					addChild(KSTimer::create(1.5f, [=](){
						m_touchEnable = true;
						float upLimit = 175;
						addChild(KSGradualValue<float>::create(m_shutter->getPositionY(), upLimit, 1.f, [=](float t){
							m_shutter->setPositionY(t);
						}, [=](float t){
							m_shutter->setPositionY(t);
						}));
						showSuccess();
					}));
				}
				else
				{
					addChild(KSTimer::create(0.f, [=](){
						m_desc2->removeFromParent();
						StyledLabelTTF* desc2 = StyledLabelTTF::create(
																													 CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_onePercentDesc2), 2, 0.3f)->getCString(),
																													 mySGD->getFont().c_str(), 12.f, 0, StyledAlignment::kCenterAlignment);
						m_container->addChild(desc2, kOnePercentGame_Z_content);
						m_desc2 = desc2;
						desc2->setPosition(ccp(0.0,-36.5)); 			// dt (0.0,-36.5)
					}));
				}
				
				
			}
			else
			{
				// 실패했을 경우.
				AudioEngine::sharedInstance()->playEffect("se_fail.mp3", false);
			}
			if(m_99State != 999)
			{
				addChild(KSTimer::create(1.0f, [=](){
					scheduleUpdate();
				}));
			}
			m_validSize -= 20;
		}
		else if(m_99State == 3) // 2nd try
		{
			
			unscheduleUpdate();
			int pos = m_cursor->getPositionX();
			m_99State = 4; // 세번 째 시도로 세팅.
			m_touchEnable = false;
			
			addChild(KSGradualValue<float>::create(m_validSize, m_validSize - 18, 1.f, [=](float t){
				m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
			}, [=](float t){
				
				m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
			}));
			
			addChild(KSTimer::create(1.5f, [=](){
				m_touchEnable = true;
			}));
			
			
			if(fabsf(pos) <= m_validSize / 2.f)
			{
				AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3", false);
				m_totalPercent += 0.003f;
				m_totalPercent = MIN(1.f, m_totalPercent);
				
//				auto stencil = CCScale9Sprite::create("one_percent_gacha_06.png", CCRectMake(0, 0, 13, graphHeight), CCRectMake(5, 5, 3, 165));
//				stencil->setContentSize(CCSizeMake(13, graphHeight * (m_totalPercent - 0.99f) * 100.f));
//				stencil->setAnchorPoint(ccp(0.5f, 0.0f));
//				stencil->setPositionY(-m_gradient->getContentSize().height / 2.f);
//				m_closer->setStencil(stencil);
				m_stencil->setContentSize(CCSizeMake(13, graphHeight * (m_totalPercent - 0.99f) * 100.f));
				if(m_totalPercent >= 1.f)
				{
					unscheduleUpdate();
					m_touchEnable = false;
					m_99State = 999;
					addChild(KSTimer::create(1.5f, [=](){
						m_touchEnable = true;
						float upLimit = 175;
						addChild(KSGradualValue<float>::create(m_shutter->getPositionY(), upLimit, 1.f, [=](float t){
							m_shutter->setPositionY(t);
						}, [=](float t){
							m_shutter->setPositionY(t);
						}));
						
						showSuccess();
					}));
				}
				else
				{
					addChild(KSTimer::create(0.f, [=](){
						m_desc2->removeFromParent();
						StyledLabelTTF* desc2 = StyledLabelTTF::create(
																													 CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_onePercentDesc2), 1, 0.4f)->getCString(),
																													 mySGD->getFont().c_str(), 12.f, 0, StyledAlignment::kCenterAlignment);
						m_container->addChild(desc2, kOnePercentGame_Z_content);
						desc2->setPosition(ccp(0.0,-36.5)); 			// dt (0.0,-36.5)
						m_desc2 = desc2;
					}));
				}
					
				
				
				
			}
			else
			{
				// 실패했을 경우.
				AudioEngine::sharedInstance()->playEffect("se_fail.mp3", false);
			}
			if(m_99State != 999)
			{
				addChild(KSTimer::create(1.0f, [=](){
					scheduleUpdate();
				}));
			}
			m_validSize -= 18;
		}
		else if(m_99State == 4) // 3rd try
		{
			unscheduleUpdate();
			int pos = m_cursor->getPositionX();
			m_touchEnable = false;
			m_99State = 999;
			addChild(KSGradualValue<float>::create(m_validSize, 0, 1.f, [=](float t){
				m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
			}, [=](float t){
				
				m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
			}));
			
			if(fabsf(pos) <= m_validSize / 2.f)
			{
				AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3", false);
				m_totalPercent += 0.004f;
				m_totalPercent = MIN(1.f, m_totalPercent);
//				auto stencil = CCScale9Sprite::create("one_percent_gacha_06.png", CCRectMake(0, 0, 13, graphHeight), CCRectMake(5, 5, 3, 165));
//				stencil->setContentSize(CCSizeMake(13, graphHeight * (m_totalPercent - 0.99f) * 100.f));
//				stencil->setAnchorPoint(ccp(0.5f, 0.0f));
//				stencil->setPositionY(-m_gradient->getContentSize().height / 2.f);
//				m_closer->setStencil(stencil);
				m_stencil->setContentSize(CCSizeMake(13, graphHeight * (m_totalPercent - 0.99f) * 100.f));
				//					m_99State = 777; // 성공
				float upLimit;
				if(m_totalPercent >= 1.f)
					upLimit = 175;
				else
					upLimit = m_shutter->getPositionY();
				
				addChild(KSGradualValue<float>::create(m_shutter->getPositionY(), upLimit, 1.f, [=](float t){
					m_shutter->setPositionY(t);
				}, [=](float t){
					m_shutter->setPositionY(t);
				}
																							 ));
				addChild(KSTimer::create(0.f, [=](){
				}));
			}
			else
			{
				// 실패했을 경우.
				AudioEngine::sharedInstance()->playEffect("se_fail.mp3", false);
			}
			
			addChild(KSTimer::create(1.5f, [=](){
				m_touchEnable = true;
				if(m_totalPercent >= 1.f)
				{
					float upLimit = 175;
					addChild(KSGradualValue<float>::create(m_shutter->getPositionY(), upLimit, 1.f, [=](float t){
						m_shutter->setPositionY(t);
					}, [=](float t){
						m_shutter->setPositionY(t);
					}));
					
					showSuccess();
				}
				else
				{
					showFail();
				}
			}));
		}
		else if(m_99State == 4444) // 닫기
		{
			if(m_resultGacha)
			{
				//					m_resultGacha(recent_percent); // 원래 가차 수치로 ~!
				m_resultGacha(recent_percent); // 원래 가차 수치로 ~!
				
				removeFromParent();
			}
		}
		else if(m_99State == 777)
		{
			if(m_resultGacha)
			{
				m_resultGacha(1.f); // 100% 로 설정함!
				removeFromParent();
			}
		}
		else if(m_99State == 999) // 마지막 결과
		{
			if(m_resultGacha)
			{
				m_resultGacha(MIN(1.f, m_totalPercent)); // 100% 로 설정함!
				removeFromParent();
			}
		}
	}
	//one_percent_gacha_color.png
}
void OnePercentGame::showFail()
{
	CCSprite* failBox = CCSprite::create("one_percent_gacha_failbox.png");
	m_container->addChild(failBox, kOnePercentGame_Z_content);
	KSLabelTTF* failTitle = KSLabelTTF::create("100% 달성 실패", mySGD->getFont().c_str(), 20.f);
	AudioEngine::sharedInstance()->playEffect("ment_gameover1.mp3", false);
	failTitle->setPosition(ccp(0.0, 14.5)); 			// dt (0.0, 14.5)
	failTitle->setColor(ccc3(255, 0, 0));
	setFormSetter(failTitle);
	m_container->addChild(failTitle, kOnePercentGame_Z_content);
	
	StyledLabelTTF* failContent = StyledLabelTTF::create(
																 ccsf("<font color=#FFFFFF size=12>최종 획득 영역</font>"
																 "<font color=#FFAA14 size=30>%.1f%%</font>", m_totalPercent * 100.f),
																	 mySGD->getFont().c_str(),
																					 12.f,
																					 999,
																				 StyledAlignment::kCenterAlignment);
	failContent->setPosition(ccp(0.0, -5.0)); 			// dt (0.0, -5.0)
	setFormSetter(failContent);
	m_container->addChild(failContent, kOnePercentGame_Z_content);
	
//	label->setScale(0);
//	addChild(KSGradualValue<float>::create(0, 1.f, 0.3f, [=](float t){
//		label->setScale(t);
//	}, [=](float t){
//		label->setScale(t);
//	}, elasticOut));
	
	m_99State = 999;
	CCLabelTTF* stopFnt = CCLabelTTF::create("닫기", mySGD->getFont().c_str(), 20.f);
	stopFnt->setColor(ccc3(22, 11, 0));
	gacha_button->getTitleLabel()->removeAllChildren();
	
	gacha_button->getTitleLabel()->addChild(stopFnt);
}
void OnePercentGame::showSuccess()
{
	KSLabelTTF* label = KSLabelTTF::create("100% 달성!!", mySGD->getFont().c_str(), 30.f);
	AudioEngine::sharedInstance()->playEffect("ment_rankup.mp3", false);
	m_container->addChild(label, kOnePercentGame_Z_content);
	label->setScale(0);
	addChild(KSGradualValue<float>::create(0, 1.f, 0.3f, [=](float t){
		label->setScale(t);
	}, [=](float t){
		label->setScale(t);
	}, elasticOut));
	
	label->enableOuterStroke(ccc3(19, 9, 0), 1.f);
	label->setColor(ccc3(255, 155, 0));
	
	m_99State = 999;
	CCLabelTTF* stopFnt = CCLabelTTF::create("카드 받기", mySGD->getFont().c_str(), 20.f);
	stopFnt->setColor(ccc3(22, 11, 0));
	gacha_button->getTitleLabel()->removeAllChildren();
	
	gacha_button->getTitleLabel()->addChild(stopFnt);
}
void OnePercentGame::update(float dt)
{
	if(m_cursorDirection > 0)
	{
		m_cursor->setPositionX(m_cursor->getPositionX() + 2.5f);
		if(m_cursor->getPositionX() > rightLimit)
		{
			m_cursorDirection = -1;
		}
	}
	else
	{
		m_cursor->setPositionX(m_cursor->getPositionX() - 2.5f);
		if(m_cursor->getPositionX() < leftLimit)
		{
			m_cursorDirection = 1;
		}
	}
}
