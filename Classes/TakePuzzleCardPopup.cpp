//
//  TakePuzzleCardPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 6. 15..
//
//

#include "TakePuzzleCardPopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "FormSetter.h"
#include "StarGoldData.h"
#include "StyledLabelTTF.h"
#include "CommonAnimation.h"
#include "StageImgLoader.h"

TakePuzzleCardPopup* TakePuzzleCardPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	TakePuzzleCardPopup* t_mup = new TakePuzzleCardPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void TakePuzzleCardPopup::myInit(int t_touch_priority, function<void()> t_end_func)
{
	startFormSetter(this);
	
	mySGD->is_new_puzzle_card = false;
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_large_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	setFormSetter(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251,185));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0,-15));
	back_case->addChild(back_in);
	setFormSetter(back_in);
	
	bool is_perfect = mySGD->getPuzzleHistory(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)).is_perfect.getV();
	
	int new_card_number = mySGD->new_puzzle_card_info["cardNo"].asInt();
	
	string title_str, content_str;
	if(is_perfect)
	{
		title_str = myLoc->getLocalForKey(LK::kMyLocalKey_puzzlePerfectCardTitle);
		content_str = myLoc->getLocalForKey(LK::kMyLocalKey_puzzlePerfectCardContent);
	}
	else
	{
		title_str = myLoc->getLocalForKey(LK::kMyLocalKey_puzzleClearCardTitle);
		content_str = myLoc->getLocalForKey(LK::kMyLocalKey_puzzleClearCardContent);
	}
	
	KSLabelTTF* title_label = KSLabelTTF::create(title_str.c_str(), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(-85, back_case->getContentSize().height/2.f-35));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
								  
								  CommonAnimation::closePopup(this, m_container, gray, [=](){
									  
								  }, [=](){
									  end_func();
									  removeFromParent();
									  
								  });
							  });
	back_case->addChild(close_button);
	
	
	CCSprite* card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", new_card_number));// mySIL->getUnsafeLoadedImg(ccsf("card%d_thumbnail.png", new_card_number));
	card_img->setScale(0.4f*0.5f);
	
	string t_card_type = NSDS_GS(kSDS_CI_int1_category_s, new_card_number);
	if(t_card_type == "")
		t_card_type = "normal";
	
	CCSprite* card_case = CCSprite::create(ccsf("cardsetting_minicase_%s.png", t_card_type.c_str()));
	card_case->setPosition(ccpFromSize(card_img->getContentSize()/2.f));
	card_case->setScale(2.f/0.4f);
	card_img->addChild(card_case);
	
	CCSprite* light_img = CCSprite::create("newitem_back.png");
	light_img->setScale(2.f/0.4f);
	light_img->setPosition(card_case->getPosition());
	card_img->addChild(light_img, -1);
	
	light_img->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 90)));
	
	card_img->setPosition(ccpFromSize(back_in->getContentSize()/2.f) + ccp(0, 25));
	back_in->addChild(card_img);
	setFormSetter(card_img);
	
	
	
	StyledLabelTTF* content_label = StyledLabelTTF::create(content_str.c_str(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
	content_label->setPosition(ccp(back_in->getContentSize().width / 2.f, 50));
	back_in->addChild(content_label);
		
	if(!is_perfect)
	{
		this->setVisible(false);
		addChild(KSTimer::create(1.f, [=]()
								 {
									 this->setVisible(true);
									 CommonAnimation::openPopup(this, m_container, gray, [=](){
									 }, [=](){
										 is_menu_enable = true;
									 });
								 }));
	}
	else
	{
		CommonAnimation::openPopup(this, m_container, gray, [=](){
		}, [=](){
			is_menu_enable = true;
		});
	}
}