//
//  CardGiftPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 7. 7..
//
//

#include "CardGiftPopup.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "TouchSuctionLayer.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "CommonButton.h"
#include "StyledLabelTTF.h"
#include "KSUtil.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"

CardGiftPopup* CardGiftPopup::create(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func)
{
	CardGiftPopup* t_cgp = new CardGiftPopup();
	t_cgp->myInit(t_touch_priority, t_gift_card, t_end_func, t_close_func);
	t_cgp->autorelease();
	return t_cgp;
}

void CardGiftPopup::myInit(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func)
{
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	close_func = t_close_func;
	gift_card_number = t_gift_card;
	
	is_menu_enable = false;
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(touch_priority+1);
	t_suction->setTouchEnabled(true);
	addChild(t_suction);
	
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
	
	main_case = CCSprite::create("popup_small_back.png");
	main_case->setPosition(ccp(240,160));
	addChild(main_case);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cardGiftTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(main_case->getContentSize().width/2.f-85,main_case->getContentSize().height-35));
	main_case->addChild(title_label);
	
	main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(250, 110.f));
	main_inner->setPosition(main_case->getContentSize().width/2.f, 82.f);
	main_case->addChild(main_inner);
	
	CommonButton* close_menu = CommonButton::createCloseButton(touch_priority);
	close_menu->setPosition(ccp(main_case->getContentSize().width-25,main_case->getContentSize().height-22));
	close_menu->setFunction([=](CCObject* sender)
							{
								if(!is_menu_enable)
									return;
								
								is_menu_enable = false;
								
								input_text->setEnabled(false);
								input_text->setVisible(false);
								
								input_text->removeFromParent();
								
								CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
								{
									end_func();
									removeFromParent();
								});
							});
	main_case->addChild(close_menu);
	
	StyledLabelTTF* my_id_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(kMyLocalKey_cardGiftMyID), KS::longLongToStrForDG(mySGD->user_index).c_str()), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	my_id_label->setAnchorPoint(ccp(0.5f,0.5f));
	my_id_label->setPosition(ccp(main_inner->getContentSize().width/2.f, 95));
	main_inner->addChild(my_id_label);
	
	CCPoint text_position = ccp(207,157);
	
	CCScale9Sprite* text_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(5, 5, 8, 8));
	text_back->setContentSize(CCSizeMake(160, 30));
	text_back->setPosition(text_position - (main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + main_inner->getPosition() + ccpFromSize(main_inner->getContentSize()/(-2.f))));
	main_inner->addChild(text_back, 999);
	
	CCScale9Sprite* t_back1 = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(5, 5, 8, 8));
	t_back1->setOpacity(0);
	input_text = CCEditBox::create(CCSizeMake(160, 30), t_back1);
	input_text->setAnchorPoint(ccp(0.5f, 0.5f));
	input_text->setPosition(text_position); 			// dt (10.0,-1.0)
	
	input_text->setPlaceHolder(myLoc->getLocalForKey(kMyLocalKey_cardGiftInputText));
	input_text->setReturnType(kKeyboardReturnTypeDone);
	input_text->setFont(mySGD->getFont().c_str(), 13);
	input_text->setInputMode(kEditBoxInputModeSingleLine);
//	input_text->setInputFlag(cocos2d::extension::EditBoxInputFlag::kEditBoxInputFlagInitialCapsAllCharacters);
	input_text->setDelegate(this);
	input_text->setTouchPriority(touch_priority);
	input_text->setEnabled(false);
	input_text->setVisible(false);
	
	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text, 99999);
	
	found_back = NULL;
	
	CommonButton* search_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_cardGiftSearch), 12, CCSizeMake(60, 35), CCScale9Sprite::create("subbutton_purple2.png", CCRectMake(0, 0, 62, 32), CCRectMake(30, 15, 2, 2)), touch_priority-1);
	search_button->setPosition(text_position + ccp(118,0) - (main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + main_inner->getPosition() + ccpFromSize(main_inner->getContentSize()/(-2.f))));
	search_button->setFunction([=](CCObject* sender)
							   {
								   if(!is_menu_enable)
									   return;
								   
								   is_menu_enable = false;
								   
								   if(this->found_back)
									{
									   this->found_back->removeFromParent();
										this->found_back = NULL;
									}
								   this->result_label->setVisible(false);
								   
								   t_loading = LoadingLayer::create(touch_priority-100);
								   addChild(t_loading, 9999);
								   
								   string input_data = input_text->getText();
								   
								   Json::Value t_param;
								   t_param["userIndex"] = KS::strToLongLongForDG(input_data);
								   
								   myHSP->command("getuserdata", t_param, json_selector(this, CardGiftPopup::resultGetUserData));
							   });
	main_inner->addChild(search_button, 1000);
	
	result_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_cardGiftNotFound), mySGD->getFont().c_str(), 12);
	result_label->setPosition(ccp(240,117) - (main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + main_inner->getPosition() + ccpFromSize(main_inner->getContentSize()/(-2.f))));
	main_inner->addChild(result_label, 1001);
		
	CommonAnimation::openPopup(this, main_case, gray,
							   [=](){}, [=]()
	{
		is_menu_enable = true;
		input_text->setEnabled(true);
		input_text->setVisible(true);
	});
}

void CardGiftPopup::resultSendAction(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		vector<int> card_data_load_list;
		card_data_load_list.clear();
		
		mySGD->initTakeCardInfo(result_data["list"], card_data_load_list);
		
//		if(card_data_load_list.size() > 0)
//		{
//			Json::Value card_param;
//			for(int i=0;i<card_data_load_list.size();i++)
//				card_param["noList"][i] = card_data_load_list[i];
//			command_list.push_back(CommandParam("getcardlist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
//		}
//		else
//		{
			mySGD->resetHasGottenCards();
//		}
		
		t_loading->removeFromParent();
		
		if(mySGD->isHasGottenCards(gift_card_number.getV()) > 0)
		{
			addChild(ASPopupView::getCommonNoti(touch_priority-100, myLoc->getLocalForKey(kMyLocalKey_cardGiftSuccessTitle), myLoc->getLocalForKey(kMyLocalKey_cardGiftSuccessContent), [=]()
			{
				input_text->setVisible(true);
				is_menu_enable = true;
			}), 9999);
		}
		else
		{
			addChild(ASPopupView::getCommonNoti(touch_priority-100, myLoc->getLocalForKey(kMyLocalKey_cardGiftSuccessTitle), myLoc->getLocalForKey(kMyLocalKey_cardGiftSuccessContent), [=]()
												{
													input_text->setEnabled(false);
													input_text->setVisible(false);
													
													input_text->removeFromParent();
													
													CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
																				{
																					close_func();
																					removeFromParent();
																				});
												}), 9999);
		}
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
				myHSP->command(command_list);
			});
			((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 myHSP->command(command_list);
									 }));
		}
	}
}

void CardGiftPopup::resultGetUserData(Json::Value result_data)
{
	GraphDogLib::JsonToLog("result getuserdata", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		found_back = CCScale9Sprite::create("rank_normal2.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
		found_back->setContentSize(CCSizeMake(224, 35));
		found_back->setPosition(ccp(240,117) - (main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + main_inner->getPosition() + ccpFromSize(main_inner->getContentSize()/(-2.f))));
		main_inner->addChild(found_back, 1001);
		
		KSLabelTTF* nick_label = KSLabelTTF::create(result_data["nick"].asString().c_str(), mySGD->getFont().c_str(), 12);
		nick_label->setAnchorPoint(ccp(0,0.5f));
		nick_label->setPosition(ccp(10, found_back->getContentSize().height/2.f));
		found_back->addChild(nick_label);
		
		CommonButton* send_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_cardGiftSend), 12, CCSizeMake(60, 35), CCScale9Sprite::create("tip.png", CCRectMake(0, 0, 55, 32), CCRectMake(27, 15, 1, 2)), touch_priority-2);
		send_button->setPosition(ccp(found_back->getContentSize().width-31, found_back->getContentSize().height/2.f));
		send_button->setFunction([=](CCObject* sender)
								 {
									 if(!is_menu_enable)
										 return;
									 
									 is_menu_enable = false;
									 input_text->setVisible(false);
									 t_loading = LoadingLayer::create(touch_priority-100);
									 addChild(t_loading, 9999);
									 
									 command_list.clear();
									 
									 Json::Value transaction_param;
									 transaction_param["memberID"] = hspConnector::get()->getMemberID();
									 command_list.push_back(CommandParam("starttransaction", transaction_param, nullptr));
									 
									 Json::Value send_card_param;
									 send_card_param["memberID"] = myHSP->getMemberID();
									 send_card_param["toMemberID"] = result_data["memberID"].asInt64();
									 send_card_param["cardNo"] = gift_card_number.getV();
									 command_list.push_back(CommandParam("sendcard", send_card_param, nullptr));
									 
									 Json::Value card_history_param;
									 card_history_param["memberID"] = myHSP->getMemberID();
									 command_list.push_back(CommandParam("getCardHistory", card_history_param, json_selector(this, CardGiftPopup::resultSendAction)));
									 
									 myHSP->command(command_list);
								 });
		found_back->addChild(send_button);
		
		t_loading->removeFromParent();
		is_menu_enable = true;
	}
	else if(result_data["result"]["code"].asInt() == GDDONTFIND)
	{
		mySGD->network_check_cnt = 0;
		
		this->result_label->setVisible(true);
		
		t_loading->removeFromParent();
		is_menu_enable = true;
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
				string input_data = input_text->getText();
				
				Json::Value t_param;
				t_param["userIndex"] = KS::strToLongLongForDG(input_data);
				
				myHSP->command("getuserdata", t_param, json_selector(this, CardGiftPopup::resultGetUserData));
			});
			((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 string input_data = input_text->getText();
										 
										 Json::Value t_param;
										 t_param["userIndex"] = KS::strToLongLongForDG(input_data);
										 
										 myHSP->command("getuserdata", t_param, json_selector(this, CardGiftPopup::resultGetUserData));
									 }));
		}
	}
}

void CardGiftPopup::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLOG("edit begin");
}
void CardGiftPopup::editBoxEditingDidEnd(CCEditBox* editBox)
{
	CCLOG("edit end");
}
void CardGiftPopup::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLOG("edit changed : %s", text.c_str());
//	if(text.size() == 4)
//	{
//		if(editBox == input_text1)
//		{
//			input_text2->touchDownAction(nullptr, CCControlEvent());
//		}
//		else if(editBox == input_text2)
//		{
//			input_text3->touchDownAction(nullptr, CCControlEvent());
//		}
//	}
}
void CardGiftPopup::editBoxReturn(CCEditBox* editBox)
{
	CCLOG("edit return");
}