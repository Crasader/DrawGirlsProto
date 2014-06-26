#include "Terms.h"
#include "ASPopupView.h"
#include "CommonButton.h"
#include "FormSetter.h"
#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"
#include "StarGoldData.h"
#include "CommonAnimation.h"
#include "MyLocalization.h"


Terms::Terms()
{
	m_leftChecked = m_rightChecked = false;
	m_leftCheckSprite = m_rightCheckSprite = nullptr;
}
Terms::~Terms()
{
}
//void Terms::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool Terms::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool Terms::init(int touchP, std::function<void(void)> finalFunctor)
{
	CCLayer::init();
	
	startFormSetter(this);
	
	ASPopupView* t_popup = ASPopupView::create(touchP);
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;

	
	float height_value = 320.f;
	if(myDSH->screen_convert_rate < 1.f)
		height_value = 320.f/myDSH->screen_convert_rate;
	
	if(height_value < myDSH->ui_top)
		height_value = myDSH->ui_top;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, 160));
	t_popup->setBasePosition(ccp(240, 160));

	CCScale9Sprite* container = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	container->setContentSize(CCSizeMake(946/2.f, 558/2.f));
	
	t_popup->setContainerNode(container);
	
	KSLabelTTF* termsLbl = KSLabelTTF::create("이용약관", mySGD->getFont().c_str(), 15.f);
	termsLbl->setColor(ccc3(255, 170, 20));
	termsLbl->setPosition(ccpFromSize(container->getContentSize()) / 2.f + ccp(0, 116.0));
	setFormSetter(termsLbl);
	container->addChild(termsLbl);
	auto skeleton = [=](int order)->tuple<CCNode*, CCNode*, CCNode*>{
		CCScale9Sprite* Front = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		Front->setContentSize(CCSizeMake(458/2.f, 410/2.f));
		container->addChild(Front);
		
//	std::get<1>(ret1)->setPosition(ccpFromSize(std::get<0>(ret1)->getContentSize()) / 2.f);
		
		if(order == 0)
		{
			Front->setPosition(ccp(123.0, 140.0)); 			// dt (123.0, 140.0)
			setFormSetter(Front);
		}
		else if(order == 1)
		{
			Front->setPosition(ccp(350.f, 140.0)); 			// dt (123.0, 140.0)
			setFormSetter(Front);
		}
		CCScale9Sprite* InnerFront = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		InnerFront->setContentSize(CCSizeMake(426/2.f, 304/2.f));
		InnerFront->setPosition(ccpFromSize(Front->getContentSize()) / 2.f);
		Front->addChild(InnerFront);
		
		if(order == 0)
		{
			CCScrollView* terms = CCScrollView::create(CCSizeMake(426/2.f - 15, 304/2.f - 25));
			terms->setTouchPriority(touchP - 1);
			unsigned long fileSize;
			unsigned char* termContent = CCFileUtils::sharedFileUtils()->getFileData("hello.lua", "rt", &fileSize);
//			KSLabelTTF* termLbl = KSLabelTTF::create((char*)termContent, mySGD->getFont().c_str(), 8.f);
			KSLabelTTF* termLbl = KSLabelTTF::create((char*)termContent, mySGD->getFont().c_str(),
																							 8.f, CCSizeZero, kCCTextAlignmentLeft);
//			termLbl->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
//			termLbl->updateTexture();
//			KSLabelTTF* termLbl = KSLabelTTF::create((char*)termContent, mySGD->getFont().c_str(), 8.f);
			Front->addChild(terms, 1);
			terms->setContainer(termLbl);
			setFormSetter(terms);
//			terms->setPosition(InnerFront->getPosition());
			terms->setPosition(ccp(18.5, 23.0));
			
		}
		else if(order == 1)
		{
			CCScrollView* terms = CCScrollView::create(CCSizeMake(426/2.f - 15, 304/2.f - 25));
			terms->setTouchPriority(touchP - 1);
			unsigned long fileSize;
			unsigned char* termContent = CCFileUtils::sharedFileUtils()->getFileData("hello.lua", "rt", &fileSize);
			//			KSLabelTTF* termLbl = KSLabelTTF::create((char*)termContent, mySGD->getFont().c_str(), 8.f);
			KSLabelTTF* termLbl = KSLabelTTF::create((char*)termContent, mySGD->getFont().c_str(),
																							 8.f, CCSizeZero, kCCTextAlignmentLeft);
			//			termLbl->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
			//			termLbl->updateTexture();
			//			KSLabelTTF* termLbl = KSLabelTTF::create((char*)termContent, mySGD->getFont().c_str(), 8.f);
			Front->addChild(terms, 1);
			terms->setContainer(termLbl);
			setFormSetter(terms);
			//			terms->setPosition(InnerFront->getPosition());
			terms->setPosition(ccp(243, 23.0));
	
		}
		
		
		KSLabelTTF* TitleLbl = KSLabelTTF::create(ccsf("이용약관%d", order + 1), mySGD->getFont().c_str(), 13.f);
		Front->addChild(TitleLbl);
		setFormSetter(TitleLbl);
		TitleLbl->setPosition(ccpFromSize(Front->getContentSize()) / 2.f + ccp(0, 86.f));
		
		
		CCNode* checkBox = CCNode::create();
		
		Front->addChild(checkBox);
		CommonButton* check = CommonButton::create(CCSprite::create("popup_check_back.png"), -99999);
		check->setTouchPriority(touchP - 1);
		check->setAnchorPoint(ccp(0, 0.5f));
		checkBox->addChild(check);
		
		KSLabelTTF* agree = KSLabelTTF::create("동의합니다.", mySGD->getFont().c_str(), 13.f);
		agree->setAnchorPoint(ccp(0.f, 0.5f));
		checkBox->addChild(agree);
		
		agree->setPosition(ccp(check->getContentSize().width, check->getPosition().y));
		
		checkBox->setPosition(ccpFromSize(Front->getContentSize()) / 2.f +
													ccp(-(check->getContentSize().width + agree->getContentSize().width) / 2.f,0) +
													ccp(0, -77 - 8));
		check->setFunction([=](CCObject*){
			if(order == 0)
			{
				if(m_leftCheckSprite)
				{
					m_leftCheckSprite->removeFromParent();
					m_leftCheckSprite = nullptr;
				}
				else
				{
					m_leftCheckSprite = CCSprite::create("popup_check.png");
					check->addChild(m_leftCheckSprite, 1);
					m_leftCheckSprite->setPosition(ccpFromSize(check->getContentSize()) / 2.f);
				}
			}
			else if(order == 1)
			{
				if(m_rightCheckSprite)
				{
					m_rightCheckSprite->removeFromParent();
					m_rightCheckSprite = nullptr;
				}
				else
				{
					m_rightCheckSprite = CCSprite::create("popup_check.png");
					check->addChild(m_rightCheckSprite, 1);
					m_rightCheckSprite->setPosition(ccpFromSize(check->getContentSize()) / 2.f);
				}
			}
//			setFormSetter(Front);
//			setFormSetter(container);
//			setFormSetter(InnerFront);
			setFormSetter(checkBox);
//			setFormSetter(check);
//			setFormSetter(TitleLbl);
			
		
		});
		return make_tuple(Front, InnerFront, check);
	};
	
	auto ret1 = skeleton(0);
	setFormSetter(std::get<0>(ret1));
	std::get<0>(ret1)->setPosition(ccp(123.0, 140.0)); 			// dt (123.0, 140.0)
//	std::get<1>(ret1)->setPosition(ccpFromSize(std::get<0>(ret1)->getContentSize()) / 2.f);
	auto ret2 = skeleton(1);
	setFormSetter(std::get<0>(ret2));
	std::get<0>(ret2)->setPosition(ccp(350.f, 140.0)); 			// dt (123.0, 140.0)
//	std::get<1>(ret2)->setPosition(ccpFromSize(std::get<0>(ret2)->getContentSize()) / 2.f);
	addChild(t_popup);

	CommonButton* agreeButton = CommonButton::create("확인", 12.f, CCSizeMake(87, 32),
																									 CommonButtonType::CommonButtonLightPupple,
																									 touchP - 1);
	container->addChild(agreeButton);
	agreeButton->setPosition(ccpFromSize(container->getContentSize()) / 2.f + ccp(0, -160 + 44));
	
	setFormSetter(agreeButton);
	CommonAnimation::openPopup(this, container, t_popup->getDimmedSprite());
	
	agreeButton->setFunction([=](CCObject*){
		if(m_leftCheckSprite && m_rightCheckSprite)
		{
			CommonAnimation::closePopup(this, container, t_popup->getDimmedSprite(), nullptr,
																	[=](){
																		removeFromParent();
																	}); 
		}
	});
	return true;
}


