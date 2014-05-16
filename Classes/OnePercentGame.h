#ifndef ONEPERCENTGAME_H
#define ONEPERCENTGAME_H
#include "DataStorageHub.h"
#include "cocos2d.h"
#include "CountingBMLabel.h"
#include "LogData.h"
#include "StarGoldData.h"
USING_NS_CC;

#include "cocos-ext.h"

USING_NS_CC_EXT;
#include "CommonButton.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "ShopPopup.h"
#include "StyledLabelTTF.h"
#include <boost/format.hpp>
#include "FormSetter.h"
#include "EasingAction.h"
enum OnePercentGameZorder
{
	kOnePercentGame_Z_gray = 1,
	kOnePercentGame_Z_back,
	kOnePercentGame_Z_content,
	kOnePercentGame_Z_shop
};
class OnePercentGame : public CCLayer
{
public:
	OnePercentGame();
	virtual ~OnePercentGame();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static OnePercentGame* create()
	{
		OnePercentGame* t = new OnePercentGame();
		t->init();
		t->autorelease();
		return t;
	}
	static OnePercentGame* create(float originalPercent, std::function<void(void)> cancelGacha, std::function<void(float)> tryGacha)
	{

		OnePercentGame* t = new OnePercentGame();
		t->init(originalPercent, cancelGacha, tryGacha);
		t->autorelease();
		return t;
	}
	bool init(float originalPercent, std::function<void(void)> cancelGacha, std::function<void(float)> tryGacha)
	{
		CCLayer::init();
		setTouchEnabled(true);
		m_cancelGacha = cancelGacha;
		m_resultGacha = tryGacha;

		recent_percent = originalPercent;
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

		CCScale9Sprite* inner_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		inner_back->setContentSize(CCSizeMake(295, 180));
		inner_back->setPosition(ccp(0,10));
		m_container->addChild(inner_back, kOnePercentGame_Z_back);
		
		StyledLabelTTF* desc = StyledLabelTTF::create("<font color=999 size=11 newline=20>영역뽑기를 통해</font><font color=999 size=11>부족한 영역을 채워 </font><font color=900 size=11>100%에 도전 하세요.</font>",
																									mySGD->getFont().c_str(), 99, 0, StyledAlignment::kCenterAlignment);
		m_container->addChild(desc, kOnePercentGame_Z_content);
		m_desc = desc;
		desc->setPosition(0, 40);
		
		StyledLabelTTF* currentGainArea = StyledLabelTTF::create("<font color=999 size=18>현재 획득영역  </font><font color=990 size=30 strokecolor=000 strokesize=2 tag=1>99.9%</font>", mySGD->getFont().c_str(), 99, 0, StyledAlignment::kCenterAlignment);
		m_currentGainArea = currentGainArea;
		currentGainArea->getLabelByTag(1)->setString(boost::str(boost::format("%|4.3|%%") % (recent_percent * 100.f)).c_str());
		currentGainArea->setPosition(ccp(0, 80));
		m_container->addChild(currentGainArea, kOnePercentGame_Z_content);

		CCSprite* heart1 = CCSprite::create("one_percent_gacha_heart_0.png");
		m_container->addChild(heart1, kOnePercentGame_Z_content);

		CCSprite* heart2 = CCSprite::create("one_percent_gacha_heart_0.png");
		m_container->addChild(heart2, kOnePercentGame_Z_content);

		CCSprite* heart3 = CCSprite::create("one_percent_gacha_heart_0.png");
		m_container->addChild(heart3, kOnePercentGame_Z_content);
		
		heart1->setPosition(ccp(-50, -40));
		heart2->setPosition(ccp(0, -40));
		heart3->setPosition(ccp(50, -40));

		m_heart1 = heart1;
		m_heart2 = heart2;
		m_heart3 = heart3;
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

		m_container->setScaleY(0.f);

		addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
			addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
				addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));

		addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t)
																				 {
																					 KS::setOpacity(m_container, 255);
																					 //endShow();
																				 }));

		this->scheduleUpdate();
		return true;
	}
	void gameUISetting()
	{
		cancel_menu->removeFromParent();				
		// 현재 획득 영역, 설명글 지우고 
		addChild(KSGradualValue<float>::create(255, 0, 0.3f, [=](float t){
			KS::setOpacity(m_currentGainArea, t);
			KS::setOpacity(m_desc, t);
		},
		[=](float t){
			m_currentGainArea->removeFromParent();
			m_desc->removeFromParent();
		}));
		
		CCNode* tempNode = CCNode::create();
		CCClippingNode* cNode = CCClippingNode::create();
		cNode->setStencil(CCSprite::create("one_percent_gacha_color.png"));
		CCSprite* girl = CCSprite::create("ga1.png");
		cNode->addChild(girl);
		cNode->setAlphaThreshold(0.1f);
		tempNode->addChild(cNode, kOnePercentGame_Z_content);

		CCSprite* shutter = CCSprite::create("one_percent_gacha_box_02.png");
		m_shutter = shutter;
		cNode->addChild(shutter, kOnePercentGame_Z_content);

		CCSprite* imageBack = CCSprite::create("one_percent_gacha_box_01.png");
		tempNode->addChild(imageBack, kOnePercentGame_Z_content);

		CCScale9Sprite* gacha_back = CCScale9Sprite::create("common_button_yellowup.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
		CCLabelTTF* stopFnt = CCLabelTTF::create("멈춰!", mySGD->getFont().c_str(), 20.f);
		
		//gacha_button->removeFromParent();
		gacha_button->getTitleLabel()->removeAllChildren();

		gacha_button->getTitleLabel()->addChild(stopFnt);
		//gacha_button = CCControlButton::create(stopFnt, gacha_back);
		//gacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(OnePercentGame::gachaAction), CCControlEventTouchUpInside);
		//gacha_button->setPreferredSize(CCSizeMake(170,65));
		//gacha_button->setPosition(ccp(0, -100));
		//m_container->addChild(gacha_button, kOnePercentGame_Z_content);
		//gacha_button->setTouchPriority(-180);
		
		m_container->addChild(tempNode, kOnePercentGame_Z_content);
		tempNode->setPosition(ccp(0, 30));
		tempNode->setScale(0.7f);
	}
	void gachaAction(CCObject* sender, CCControlEvent t_event)
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

				mySGD->addChangeGoods(kGoodsType_pass5, -1, "99프로가챠");

				mySGD->changeGoods([=](Json::Value result_data){
					t_loading->removeFromParent();
					if(result_data["result"]["code"] == GDSUCCESS)
				{
					// 무료 아템으로 구매.
					gameUISetting();
					m_99State = 2;		
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

				mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getGachaOnePercentFee(), "99프로가챠");

				mySGD->changeGoods([=](Json::Value result_data){
					t_loading->removeFromParent();
					if(result_data["result"]["code"] == GDSUCCESS)
				{
					// 루비로 구매.
					gameUISetting();
					m_99State = 2;		
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
				if(fabsf(pos) <= m_validSize / 2.f)
				{
					m_99State = 3; // 두번 째 시도로 세팅.
					m_touchEnable = false;
					addChild(KSGradualValue<float>::create(0, 30, 1.f, [=](float t){
						m_shutter->setPositionY(t);
					}, [=](float t){
						m_shutter->setPositionY(t);
						scheduleUpdate();
						addChild(KSTimer::create(0.5f, [=](){
							m_touchEnable = true;
						}));
					}
					));
					addChild(KSTimer::create(0.f, [=](){
						CCSprite* heart = CCSprite::create("one_percent_gacha_heart_1.png");
						heart->setPosition(m_heart1->getPosition());
						m_container->addChild(heart, kOnePercentGame_Z_content);
					}));
					
					m_validSize = 30;
					addChild(KSGradualValue<float>::create(50, 30, 1.f, [=](float t){
						m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
					}, [=](float t){

						m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
					}));

				}
				else 
				{
					// 실패했을 경우.
					showFail();
				}
			}
			else if(m_99State == 3) // 2nd try
			{

				unscheduleUpdate();
				int pos = m_cursor->getPositionX();
				if(fabsf(pos) <= m_validSize / 2.f)
				{
					m_99State = 4; // 세번 째 시도로 세팅.
					m_touchEnable = false;
					addChild(KSGradualValue<float>::create(30, 90, 1.f, [=](float t){
						m_shutter->setPositionY(t);
					}, [=](float t){
						m_shutter->setPositionY(t);
						scheduleUpdate();
						addChild(KSTimer::create(0.5f, [=](){
							m_touchEnable = true;
						}));
					}
					));
					addChild(KSTimer::create(0.f, [=](){
						CCSprite* heart = CCSprite::create("one_percent_gacha_heart_1.png");
						heart->setPosition(m_heart2->getPosition());
						m_container->addChild(heart, kOnePercentGame_Z_content);
					}));
					
					m_validSize = 12;
					addChild(KSGradualValue<float>::create(30, 12, 1.f, [=](float t){
						m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
					}, [=](float t){

						m_validArea->setTextureRect(CCRectMake(0, 0, t, 15));
					}));

				}
				else 
				{
					// 실패했을 경우.
					showFail();
				}
			}
			else if(m_99State == 4) // 3rd try
			{
				unscheduleUpdate();
				int pos = m_cursor->getPositionX();
				if(fabsf(pos) <= m_validSize / 2.f)
				{
					m_99State = 777; // 성공
					m_touchEnable = false;
					addChild(KSGradualValue<float>::create(90, 175, 1.f, [=](float t){
						m_shutter->setPositionY(t);
					}, [=](float t){
						m_shutter->setPositionY(t);
						addChild(KSTimer::create(0.5f, [=](){
							m_touchEnable = true;
							showSuccess();
						}));
					}
					));
					addChild(KSTimer::create(0.f, [=](){
						CCSprite* heart = CCSprite::create("one_percent_gacha_heart_1.png");
						heart->setPosition(m_heart3->getPosition());
						m_container->addChild(heart, kOnePercentGame_Z_content);
					}));
				}
				else 
				{
					// 실패했을 경우.
					showFail();
				}
			}
			else if(m_99State == 4444) // 닫기
			{
				if(m_resultGacha)
				{
//					m_resultGacha(recent_percent); // 원래 가차 수치로 ~!
					m_resultGacha(1.f); // 원래 가차 수치로 ~!

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
		}
		//one_percent_gacha_color.png
	}
	void showFail()
	{
		KSLabelTTF* label = KSLabelTTF::create(boost::str(boost::format("%|4.3|%% 실패") % (recent_percent * 100.f)).c_str(), mySGD->getFont().c_str(), 30.f);
		label->enableStroke(ccc3(19, 9, 0), 2.f);
		label->setColor(ccc3(255, 155, 0));
		m_container->addChild(label, kOnePercentGame_Z_content);
		label->setScale(0);
		addChild(KSGradualValue<float>::create(0, 1.f, 0.3f, [=](float t){
			label->setScale(t);
		}, [=](float t){
			label->setScale(t);
		}, elasticOut));

		m_99State = 4444;	
		CCLabelTTF* stopFnt = CCLabelTTF::create("닫기", mySGD->getFont().c_str(), 20.f);
		stopFnt->setColor(ccc3(22, 11, 0));
		gacha_button->getTitleLabel()->removeAllChildren();

		gacha_button->getTitleLabel()->addChild(stopFnt);
	}
	void showSuccess()
	{
		KSLabelTTF* label = KSLabelTTF::create("100% 달성!!", mySGD->getFont().c_str(), 30.f);
		m_container->addChild(label, kOnePercentGame_Z_content);
		label->setScale(0);
		addChild(KSGradualValue<float>::create(0, 1.f, 0.3f, [=](float t){
			label->setScale(t);
		}, [=](float t){
			label->setScale(t);
		}, elasticOut));

		label->enableStroke(ccc3(19, 9, 0), 2.f);
		label->setColor(ccc3(255, 155, 0));

		m_99State = 777;	
		CCLabelTTF* stopFnt = CCLabelTTF::create("100% 달성!!", mySGD->getFont().c_str(), 20.f);
		stopFnt->setColor(ccc3(22, 11, 0));
		gacha_button->getTitleLabel()->removeAllChildren();

		gacha_button->getTitleLabel()->addChild(stopFnt);
	}
	void update(float dt)
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
	virtual void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
protected:
	CCSprite* m_grayBack;
	CCNode* m_container;

	CommonButton * cancel_menu;
	CCScale9Sprite* back_img;
	float recent_percent;
	CountingBMLabel * percent_label;
	CCProgressTimer * progress_img;
	CCSprite * arrow_img;
	CCControlButton* gacha_button;
	CCSprite* m_cursor;
	int m_cursorDirection;
	static const int leftLimit = -115;
	static const int rightLimit = 115;
	int m_99State;
	CCNode* m_currentGainArea, *m_desc;
	int m_validSize;
	CCSprite* m_validArea;
	CCSprite* m_heart1;
	CCSprite* m_heart2;
	CCSprite* m_heart3;
	CCSprite* m_shutter;
	bool m_touchEnable;
	std::function<void(void)> m_cancelGacha;
	std::function<void(float)> m_resultGacha;
};
#endif
