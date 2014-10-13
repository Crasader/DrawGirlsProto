//
//  NoticeContent.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 16..
//
//

#include "NoticeContent.h"
#include "GDWebSprite.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "KsLocal.h"

void NoticeContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = true;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kNoticeContentMenuTag_check)
	{
		is_not_show_three_day = true;
	}
	
	if(is_not_show_three_day)
	{
		myDSH->setStringForKey(kDSH_Key_noticeViewDate_int1, notice_list[ing_close_cnt]["no"].asInt(), CCString::createWithFormat("%lld",GraphDog::get()->getTime()+18*60*60)->getCString());
	}
	
	ing_close_cnt++;
	
	if(ing_close_cnt >= notice_list.size())
	{
		end_selector(NULL);
	}
	else
	{
		is_not_show_three_day = false;
		
		loadNotice();
		
		//			case_back->setContentSize(CCSizeMake(notice_list[ing_close_cnt]["imgInfo"]["w"].asInt() + 17, notice_list[ing_close_cnt]["imgInfo"]["h"].asInt() + 66));
	}
}

void NoticeContent::loadNotice(){
	
	day3Btn->setVisible(false);
	closeBtn->setVisible(false);
	loading_circle->setVisible(true);
	
	is_not_show_three_day = false;
	
	isBreak = notice_list[ing_close_cnt].get("isBreak",false).asBool();
	if(notice_list[ing_close_cnt].get("content", "").asString()!=""){
		//텍스트모드

		
		
		//<font|rgb|size|tag|newline>
		if(stTxt)stTxt->removeFromParent();
		string str = notice_list[ing_close_cnt].get("content", "").asString();
		
		stTxt = StyledLabelTTF::create(str.c_str(),mySGD->getFont().c_str(),13,999, StyledAlignment::kCenterAlignment);
		stTxt->setOldAnchorPoint();
		stTxt->setPosition(ccp(0, 110));
		addChild(stTxt,10000);
		
		
		
		//		content_label->setString(notice_list[ing_close_cnt]["content"].asString().c_str());
//		title_label->setString(notice_list[ing_close_cnt]["title"].asString().c_str());
		
		if(show_content)show_content->setVisible(false);
		
		content_label->setVisible(true);
		title_label->setVisible(true);
		is_menu_enable=true;
		
		loadedAction();
		
	}else if(show_content==NULL){
		//그림모드
		is_menu_enable=false;
		CCSprite* default_node = CCSprite::create("whitePaper.png");
		GDWebSprite::removeCache();
		show_content=GDWebSprite::create(notice_list[ing_close_cnt]["imgInfo"]["img"].asString(), default_node, this, callfunc_selector(NoticeContent::loadedAction));
		show_content->setAnchorPoint(ccp(0.5,0.5));
		show_content->setPosition(ccp(0,0));
		addChild(show_content,1);
		if(show_content)show_content->setVisible(true);
		
		content_label->setVisible(false);
		title_label->setVisible(false);
	}else if(show_content!=NULL){
		show_content->removeFromParent();
		is_menu_enable=false;
		CCSprite* default_node = CCSprite::create("whitePaper.png");
		show_content=GDWebSprite::create(notice_list[ing_close_cnt]["imgInfo"]["img"].asString(), default_node, this, callfunc_selector(NoticeContent::loadedAction));
		show_content->setPosition(ccp(0,0));
		addChild(show_content,1);
		if(show_content)show_content->setVisible(true);
		
		content_label->setVisible(false);
		title_label->setVisible(false);
	}
}
void NoticeContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector, Json::Value t_noti_list)
{
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	notice_list = t_noti_list;
	isBreak = false;
	
	CCSprite* white_paper = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 480, 320));
	white_paper->setScale(2);
	white_paper->setPosition(ccp(0,0));
	white_paper->setColor(ccc3(0, 0, 0));
	addChild(white_paper, 1);
	
	
	//	case_back = CCScale9Sprite::create("popup3_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
	//	case_back->setPosition(CCPointZero);
	//	addChild(case_back);
	
	ing_close_cnt = 0;
	
	//	case_back->setContentSize(CCSizeMake(notice_list[ing_close_cnt]["imgInfo"]["w"].asInt() + 17, notice_list[ing_close_cnt]["imgInfo"]["h"].asInt() + 66)); // 333 + 17 , 199 + 66
	
	
	closeBtn = CommonButton::create(getLocal(LK::kMyLocalKey_close), 12, CCSizeMake(80, 40), CommonButtonLightPupple, touch_priority-2);
	addChild(closeBtn,100);
	closeBtn->setFunction(json_selector(this, NoticeContent::menuAction));
	closeBtn->setAnchorPoint(ccp(1,0.5));
	closeBtn->setPosition(ccp(230,-140));
	closeBtn->setTag(kNoticeContentMenuTag_ok);
	
	
	
	day3Btn = CommonButton::create(getLocal(LK::kMyLocalKey_doNotOpenTodoay), 12, CCSizeMake(120,40), CommonButtonGray, touch_priority-2);
	addChild(day3Btn,100);
	day3Btn->setTag(kNoticeContentMenuTag_check);
	day3Btn->setAnchorPoint(ccp(0,0.5));
	day3Btn->setPosition(ccp(-230,-140));
	day3Btn->setFunction(json_selector(this, NoticeContent::menuAction));
	
	title_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 15);
	title_label->setPosition(ccp(0,150));
	title_label->setColor(ccc3(255, 255, 0));
	addChild(title_label,10);
	
	content_label= CCLabelTTF::create("", mySGD->getFont().c_str(), 12, CCSizeMake(450, 280), kCCTextAlignmentCenter);
	content_label->setColor(ccc3(255, 255, 255));
	content_label->setPosition(ccp(0,-20));
	addChild(content_label,10);
	
	show_content = NULL;
	
	
	loading_circle = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_circle->setPosition(ccp(this->getContentSize().width/2.f, this->getContentSize().height/2.f));
	this->addChild(loading_circle,9999999);
	
	
	loadNotice();
	
	
	
	CommonButton* back = CommonButton::create("", 10, CCSizeMake(480, 320), CommonButtonGray, touch_priority-1);
	addChild(back,0);
	back->setFunction([=](CCObject* btn){
		//CCLog("testtest");
		if(loading_circle->isVisible()==false && notice_list[ing_close_cnt].get("linkURL", "").asString()!=""){
			hspConnector::get()->openHSPUrl(notice_list[ing_close_cnt]["linkURL"].asString().c_str());
			//CCLog("openurl %s",notice_list[ing_close_cnt]["linkURL"].asString().c_str());
		}
		
	});
	
}