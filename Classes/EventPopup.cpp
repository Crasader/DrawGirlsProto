// EventPopup.cpp
//

#include "EventPopup.h"
#define LZZ_INLINE inline
using namespace std;
EventPopup * EventPopup::create ()
        {
		EventPopup* t_ep = new EventPopup();
		t_ep->myInit();
		t_ep->autorelease();
		return t_ep;
	}
void EventPopup::setHideFinalAction (CCObject * t_final, SEL_CallFunc d_final)
        {
		target_final = t_final;
		delegate_final = d_final;
	}
void EventPopup::myInit ()
        {
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, kEP_Z_gray);
		
		main_case = CCSprite::create("event_back.png");
		main_case->setPosition(ccp(240,-160));
		addChild(main_case, kEP_Z_back);
		
		
		CCSprite* n_close = CCSprite::create("ui_common_close.png");
		CCSprite* s_close = CCSprite::create("ui_common_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(EventPopup::menuAction));
		close_item->setTag(kEP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kEP_MT_close));
		main_case->addChild(close_menu, kEP_Z_content);
		
		
		CCSprite* n_start = CCSprite::create("event_start.png");
		CCSprite* s_start = CCSprite::create("event_start.png");
		s_start->setColor(ccGRAY);
		
		CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(EventPopup::menuAction));
		start_item->setTag(kEP_MT_start);
		
		start_menu = CCMenu::createWithItem(start_item);
		start_menu->setPosition(getContentPosition(kEP_MT_start));
		main_case->addChild(start_menu, kEP_Z_content);

		el_length = NSDS_GI(kSDS_GI_eventCount_i);
		for(int i=0;i<el_length;i++)
		{
			int event_code = SDS_GI(kSDF_gameInfo, CCString::createWithFormat("event%d_code", i)->getCString());
			CCSprite* n_event = mySIL->getLoadedImg(CCString::createWithFormat("event%d_thumbnail.png", i)->getCString());
			CCSprite* s_event = mySIL->getLoadedImg(CCString::createWithFormat("event%d_thumbnail.png", i)->getCString());
			s_event->setColor(ccGRAY);
			
			CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(EventPopup::menuAction));
			event_item->setTag(event_code);
			
			CCMenu* event_menu = CCMenu::createWithItem(event_item);
//			event_menu->setEnabled(false);
			event_menu->setPosition(getContentPosition(event_code));
			main_case->addChild(event_menu, kEP_Z_content, kEP_MT_eventStageBase + i);
		}
		
		is_menu_enable = false;
		selected_event_code = 0;
		touched_number = 0;
		
		selected_event_button = NULL;
	}
void EventPopup::onEnter ()
        {
		CCLayer::onEnter();
		showPopup();
	}
void EventPopup::showPopup ()
        {
		CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
		gray->runAction(gray_fade);
		
		CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
		CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(EventPopup::endShowPopup));
		CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
		main_case->runAction(main_seq);
	}
void EventPopup::endShowPopup ()
        {
		is_menu_enable = true;
//		setTouchEnabled(true);
	}
void EventPopup::hidePopup ()
        {
		is_menu_enable = false;
//		setTouchEnabled(false);
		
		CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
		gray->runAction(gray_fade);
		
		CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,-160));
		CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(EventPopup::endHidePopup));
		CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
		main_case->runAction(main_seq);
	}
void EventPopup::endHidePopup ()
        {
		if(target_final)
			(target_final->*delegate_final)();
		removeFromParent();
	}
CCPoint EventPopup::getContentPosition (int t_tag)
        {
		CCPoint return_value;
		
		if(t_tag == kEP_MT_close)						return_value = ccp(430,280);
		else if(t_tag == kEP_MT_start)					return_value = ccp(385,48);
		else if(t_tag >= kEP_MT_eventStageBase)			return_value = ccp(128,234 - (t_tag - kEP_MT_eventStageBase - 1)*45);
		
		return_value = ccpSub(return_value, ccp(240,160));
		return_value = ccpAdd(return_value, ccp(main_case->getContentSize().width/2.f, main_case->getContentSize().height/2.f));
		
		return return_value;
	}
void EventPopup::startCancel ()
        {
		is_menu_enable = true;
	}
void EventPopup::menuAction (CCObject * sender)
        {
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kEP_MT_close)
		{
			hidePopup();
		}
		else if(tag == kEP_MT_start)
		{
			if(selected_event_code == 0)
			{
				is_menu_enable = true;
			}
			else
			{
				mySD->setSilType(selected_event_code);
				
				StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(EventPopup::successStageInfoDown), this, callfunc_selector(EventPopup::startCancel));
				addChild(t_sid, kEP_Z_popup);
			}
		}
		else if(tag >= kEP_MT_eventStageBase)
		{
			if(tag != selected_event_code)
			{
				selected_event_code = tag;
				
				if(!selected_event_button)
				{
					selected_event_button = CCSprite::create("event_button_selected.png");
					main_case->addChild(selected_event_button, kEP_Z_selected);
				}
				selected_event_button->setPosition(getContentPosition(selected_event_code));
				is_menu_enable = true;
			}
			else
			{
				mySD->setSilType(selected_event_code);
				
				StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(EventPopup::successStageInfoDown), this, callfunc_selector(EventPopup::startCancel));
				addChild(t_sid, kEP_Z_popup);
			}
		}
	}
void EventPopup::successStageInfoDown ()
        {
		StageSettingPopup* t_popup = StageSettingPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
		
//		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
#undef LZZ_INLINE
