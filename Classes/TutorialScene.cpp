//
//  TutorialScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 30..
//
//

#include "TutorialScene.h"
#include "DataStorageHub.h"
#include "PuzzleMapScene.h"
#include "PuzzleScene.h"

CCScene* TutorialScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	TutorialScene *layer = TutorialScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

enum TutorialZorder{
	kTutorialZorder_back = 0,
	kTutorialZorder_menus
};

enum TutorialMenuTag{
	kTutorialMenuTag_cancel = 0,
	kTutorialMenuTag_next,
	kTutorialMenuTag_main1,
	kTutorialMenuTag_main2,
	kTutorialMenuTag_startsetting,
	kTutorialMenuTag_diary,
	kTutorialMenuTag_gameprocess,
	kTutorialMenuTag_control
};

bool TutorialScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	
	is_menu_enable = false;
	
	CCSprite* back_img = CCSprite::create("tutorial_back.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kTutorialZorder_back);
	
	
	CCSprite* n_cancel = CCSprite::create("tutorial_common_cancel.png");
	CCSprite* s_cancel = CCSprite::create("tutorial_common_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(TutorialScene::menuAction));
	cancel_item->setTag(kTutorialMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(getContentPosition(kTutorialMenuTag_cancel));
	back_img->addChild(cancel_menu, kTutorialZorder_menus);
	
	
	CCSprite* n_next = CCSprite::create("tutorial_common_next.png");
	CCSprite* s_next = CCSprite::create("tutorial_common_next.png");
	s_next->setColor(ccGRAY);
	
	CCMenuItem* next_item = CCMenuItemSprite::create(n_next, s_next, this, menu_selector(TutorialScene::menuAction));
	next_item->setTag(kTutorialMenuTag_next);
	
	next_menu = CCMenu::createWithItem(next_item);
	next_menu->setPosition(getContentPosition(kTutorialMenuTag_next));
	back_img->addChild(next_menu, kTutorialMenuTag_next);
	next_menu->setVisible(false);
	
	loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
	loading_label->setPosition(ccp(200,140));
	back_img->addChild(loading_label, kTutorialZorder_menus);
	loading_label->setVisible(false);
	
	title_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 30);
	title_label->setPosition(ccp(200,286));
	back_img->addChild(title_label, kTutorialZorder_menus);
	
	tutorial_container = CCNode::create();
	tutorial_container->setPosition(CCPointZero);
	back_img->addChild(tutorial_container, kTutorialZorder_menus);
	
	recent_tutorial_state = kTutorialState_list;
	title_label->setString(getTitle(recent_tutorial_state).c_str());
	setTutorialContainer(kTutorialState_list);
	
	loading_cnt = 0;
	
	is_menu_enable = true;

	return true;
}

void TutorialScene::setTutorialContainer(TutorialState t_state)
{
	if(t_state == kTutorialState_list)
	{
		if(loading_label->isVisible())
			loading_label->setVisible(false);
		next_menu->setVisible(false);
		
		for(int i=kTutorialMenuTag_main1;i<=kTutorialMenuTag_control;i++)
		{
			CCSprite* n_button = CCSprite::create(CCString::createWithFormat("tutorial_button%d.png", i-kTutorialMenuTag_main1+1)->getCString());
			CCSprite* s_button = CCSprite::create(CCString::createWithFormat("tutorial_button%d.png", i-kTutorialMenuTag_main1+1)->getCString());
			s_button->setColor(ccGRAY);
			
			CCMenuItem* button_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(TutorialScene::menuAction));
			button_item->setTag(i);
			
			CCMenu* button_menu = CCMenu::createWithItem(button_item);
			button_menu->setPosition(getContentPosition(i));
			tutorial_container->addChild(button_menu, kTutorialZorder_menus);
		}
	}
	else
	{
		if(loading_label->isVisible())
			loading_label->setVisible(false);
		
		CCSprite* content = CCSprite::create(getFilename(t_state).c_str());
		content->setPosition(ccp(200,140));
		tutorial_container->addChild(content, kTutorialZorder_menus);
	}
//	else if(t_state == kTutorialState_main1_2)
//	{
//		
//	}
//	else if(t_state == kTutorialState_main1_3)
//	{
//		
//	}
//	else if(t_state == kTutorialState_main2_1)
//	{
//		
//	}
//	else if(t_state == kTutorialState_main2_2)
//	{
//		
//	}
//	else if(t_state == kTutorialState_startsetting_1)
//	{
//		
//	}
//	else if(t_state == kTutorialState_startsetting_2)
//	{
//		
//	}
//	else if(t_state == kTutorialState_diary_1)
//	{
//		
//	}
//	else if(t_state == kTutorialState_diary_2)
//	{
//		
//	}
//	else if(t_state == kTutorialState_diary_3)
//	{
//		
//	}
//	else if(t_state == kTutorialState_gameprocess_1)
//	{
//		
//	}
//	else if(t_state == kTutorialState_gameprocess_2)
//	{
//		
//	}
//	else if(t_state == kTutorialState_gameprocess_3)
//	{
//		
//	}
//	else if(t_state == kTutorialState_control_1)
//	{
//		
//	}
//	else if(t_state == kTutorialState_control_2)
//	{
//		
//	}
}

void TutorialScene::loadedImg(CCObject *sender)
{
	loading_cnt--;
	
	if(recent_tutorial_state != kTutorialState_list && loading_cnt == 0)
	{
		setTutorialContainer(recent_tutorial_state);
	}
}

void TutorialScene::menuAction(CCObject *sender)
{
	if(!is_menu_enable)		return;
	
//	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kTutorialMenuTag_cancel)
	{
		if(recent_tutorial_state == kTutorialState_list)
		{
			is_menu_enable = false;
//			CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
			CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
		}
		else
		{
			tutorial_container->removeAllChildren();
			recent_tutorial_state = kTutorialState_list;
			title_label->setString(getTitle(recent_tutorial_state).c_str());
			setTutorialContainer(kTutorialState_list);
		}
	}
	else if(tag == kTutorialMenuTag_next)
	{
		tutorial_container->removeAllChildren();
		
		recent_tutorial_state = getNextState(recent_tutorial_state);
		loading_label->setVisible(recent_tutorial_state != kTutorialState_list);
		next_menu->setVisible(recent_tutorial_state != kTutorialState_list);

		title_label->setString(getTitle(recent_tutorial_state).c_str());
		
		if(recent_tutorial_state == kTutorialState_list)
		{
			setTutorialContainer(recent_tutorial_state);
		}
		else
		{
			loading_cnt++;
			CCTextureCache::sharedTextureCache()->addImageAsync(getFilename(recent_tutorial_state).c_str(), this, callfuncO_selector(TutorialScene::loadedImg));
		}
		
	}
	else if(tag == kTutorialMenuTag_main1)
	{
		tutorial_container->removeAllChildren();
		loading_label->setVisible(true);
		next_menu->setVisible(true);
		
		recent_tutorial_state = kTutorialState_main1_1;
		title_label->setString(getTitle(recent_tutorial_state).c_str());
		
		loading_cnt++;
		CCTextureCache::sharedTextureCache()->addImageAsync(getFilename(recent_tutorial_state).c_str(), this, callfuncO_selector(TutorialScene::loadedImg));
	}
	else if(tag == kTutorialMenuTag_main2)
	{
		tutorial_container->removeAllChildren();
		loading_label->setVisible(true);
		next_menu->setVisible(true);
		
		recent_tutorial_state = kTutorialState_main2_1;
		title_label->setString(getTitle(recent_tutorial_state).c_str());
		
		loading_cnt++;
		CCTextureCache::sharedTextureCache()->addImageAsync(getFilename(recent_tutorial_state).c_str(), this, callfuncO_selector(TutorialScene::loadedImg));
	}
	else if(tag == kTutorialMenuTag_startsetting)
	{
		tutorial_container->removeAllChildren();
		loading_label->setVisible(true);
		next_menu->setVisible(true);
		
		recent_tutorial_state = kTutorialState_startsetting_1;
		title_label->setString(getTitle(recent_tutorial_state).c_str());
		
		loading_cnt++;
		CCTextureCache::sharedTextureCache()->addImageAsync(getFilename(recent_tutorial_state).c_str(), this, callfuncO_selector(TutorialScene::loadedImg));
	}
	else if(tag == kTutorialMenuTag_diary)
	{
		tutorial_container->removeAllChildren();
		loading_label->setVisible(true);
		next_menu->setVisible(true);
		
		recent_tutorial_state = kTutorialState_diary_1;
		title_label->setString(getTitle(recent_tutorial_state).c_str());
		
		loading_cnt++;
		CCTextureCache::sharedTextureCache()->addImageAsync(getFilename(recent_tutorial_state).c_str(), this, callfuncO_selector(TutorialScene::loadedImg));
	}
	else if(tag == kTutorialMenuTag_gameprocess)
	{
		tutorial_container->removeAllChildren();
		loading_label->setVisible(true);
		next_menu->setVisible(true);
		
		recent_tutorial_state = kTutorialState_gameprocess_1;
		title_label->setString(getTitle(recent_tutorial_state).c_str());
		
		loading_cnt++;
		CCTextureCache::sharedTextureCache()->addImageAsync(getFilename(recent_tutorial_state).c_str(), this, callfuncO_selector(TutorialScene::loadedImg));
	}
	else if(tag == kTutorialMenuTag_control)
	{
		tutorial_container->removeAllChildren();
		loading_label->setVisible(true);
		next_menu->setVisible(true);
		
		recent_tutorial_state = kTutorialState_control_1;
		title_label->setString(getTitle(recent_tutorial_state).c_str());
		
		loading_cnt++;
		CCTextureCache::sharedTextureCache()->addImageAsync(getFilename(recent_tutorial_state).c_str(), this, callfuncO_selector(TutorialScene::loadedImg));
	}
}

CCPoint TutorialScene::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kTutorialMenuTag_cancel)
		return_value = ccp(28,290);
	else if(t_tag == kTutorialMenuTag_next)
		return_value = ccp(372,290);
	else if(t_tag == kTutorialMenuTag_main1)
		return_value = ccp(115,215);
	else if(t_tag == kTutorialMenuTag_main2)
		return_value = ccp(115,145);
	else if(t_tag == kTutorialMenuTag_startsetting)
		return_value = ccp(115,75);
	else if(t_tag == kTutorialMenuTag_diary)
		return_value = ccp(285,215);
	else if(t_tag == kTutorialMenuTag_gameprocess)
		return_value = ccp(285,145);
	else if(t_tag == kTutorialMenuTag_control)
		return_value = ccp(285,75);
	
	return return_value;
}

string TutorialScene::getTitle(TutorialState t_state)
{
	string return_value;
	
	if(t_state == kTutorialState_list)
		return_value = "게임 설명";
	else if(t_state == kTutorialState_main1_1)
		return_value = "메인 화면 1 ( 1 / 3 )";
	else if(t_state == kTutorialState_main1_2)
		return_value = "메인 화면 1 ( 2 / 3 )";
	else if(t_state == kTutorialState_main1_3)
		return_value = "메인 화면 1 ( 3 / 3 )";
	else if(t_state == kTutorialState_main2_1)
		return_value = "메인 화면 2 ( 1 / 2 )";
	else if(t_state == kTutorialState_main2_2)
		return_value = "메인 화면 2 ( 2 / 2 )";
	else if(t_state == kTutorialState_startsetting_1)
		return_value = "카드/아이템 설정 ( 1 / 2 )";
	else if(t_state == kTutorialState_startsetting_2)
		return_value = "카드/아이템 설정 ( 2 / 2 )";
	else if(t_state == kTutorialState_diary_1)
		return_value = "다이어리 ( 1 / 3 )";
	else if(t_state == kTutorialState_diary_2)
		return_value = "다이어리 ( 2 / 3 )";
	else if(t_state == kTutorialState_diary_3)
		return_value = "다이어리 ( 3 / 3 )";
	else if(t_state == kTutorialState_gameprocess_1)
		return_value = "게임진행방법 ( 1 / 3 )";
	else if(t_state == kTutorialState_gameprocess_2)
		return_value = "게임진행방법 ( 2 / 3 )";
	else if(t_state == kTutorialState_gameprocess_3)
		return_value = "게임진행방법 ( 3 / 3 )";
	else if(t_state == kTutorialState_control_1)
		return_value = "컨트롤 방법 ( 1 / 2 )";
	else if(t_state == kTutorialState_control_2)
		return_value = "컨트롤 방법 ( 2 / 2 )";
	
	return return_value.c_str();
}

string TutorialScene::getFilename(TutorialState t_state)
{
	string return_value;
	
	if(t_state == kTutorialState_list)
		return_value = "";
	else if(t_state == kTutorialState_main1_1)
		return_value = "tutorial_main1_1.png";
	else if(t_state == kTutorialState_main1_2)
		return_value = "tutorial_main1_2.png";
	else if(t_state == kTutorialState_main1_3)
		return_value = "tutorial_main1_3.png";
	else if(t_state == kTutorialState_main2_1)
		return_value = "tutorial_main2_1.png";
	else if(t_state == kTutorialState_main2_2)
		return_value = "tutorial_main2_2.png";
	else if(t_state == kTutorialState_startsetting_1)
		return_value = "tutorial_startsetting_1.png";
	else if(t_state == kTutorialState_startsetting_2)
		return_value = "tutorial_startsetting_2.png";
	else if(t_state == kTutorialState_diary_1)
		return_value = "tutorial_diary_1.png";
	else if(t_state == kTutorialState_diary_2)
		return_value = "tutorial_diary_2.png";
	else if(t_state == kTutorialState_diary_3)
		return_value = "tutorial_diary_3.png";
	else if(t_state == kTutorialState_gameprocess_1)
		return_value = "tutorial_main1_1.png";
	else if(t_state == kTutorialState_gameprocess_2)
		return_value = "tutorial_gameprocess_2.png";
	else if(t_state == kTutorialState_gameprocess_3)
		return_value = "tutorial_gameprocess_3.png";
	else if(t_state == kTutorialState_control_1)
		return_value = "tutorial_control_1.png";
	else if(t_state == kTutorialState_control_2)
		return_value = "tutorial_control_2.png";
	
	return return_value.c_str();
}

TutorialState TutorialScene::getNextState(TutorialState t_state)
{
	TutorialState return_value;
	
	if(t_state == kTutorialState_list)
		return_value = kTutorialState_list;
	else if(t_state == kTutorialState_main1_1)
		return_value = kTutorialState_main1_2;
	else if(t_state == kTutorialState_main1_2)
		return_value = kTutorialState_main1_3;
	else if(t_state == kTutorialState_main1_3)
		return_value = kTutorialState_list;
	else if(t_state == kTutorialState_main2_1)
		return_value = kTutorialState_main2_2;
	else if(t_state == kTutorialState_main2_2)
		return_value = kTutorialState_list;
	else if(t_state == kTutorialState_startsetting_1)
		return_value = kTutorialState_startsetting_2;
	else if(t_state == kTutorialState_startsetting_2)
		return_value = kTutorialState_list;
	else if(t_state == kTutorialState_diary_1)
		return_value = kTutorialState_diary_2;
	else if(t_state == kTutorialState_diary_2)
		return_value = kTutorialState_diary_3;
	else if(t_state == kTutorialState_diary_3)
		return_value = kTutorialState_list;
	else if(t_state == kTutorialState_gameprocess_1)
		return_value = kTutorialState_gameprocess_2;
	else if(t_state == kTutorialState_gameprocess_2)
		return_value = kTutorialState_gameprocess_3;
	else if(t_state == kTutorialState_gameprocess_3)
		return_value = kTutorialState_list;
	else if(t_state == kTutorialState_control_1)
		return_value = kTutorialState_control_2;
	else if(t_state == kTutorialState_control_2)
		return_value = kTutorialState_list;
	
	return return_value;
}