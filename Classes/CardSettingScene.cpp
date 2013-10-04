//
//  CardSettingScene.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 18..
//
//

#include "CardSettingScene.h"
#include "ScreenSide.h"
#include "MyLocalization.h"
#include "StageSettingScene.h"
#include "StageImgLoader.h"
#include "CollectionListScene.h"
#include "CollectionBook.h"
#include "WorldMapScene.h"

CCScene* CardSettingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CardSettingScene *layer = CardSettingScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

//enum CSS_Zorder{
//	kCSS_Z_back = 1,
//	kCSS_Z_selectedImg,
//	kCSS_Z_content,
//	kCSS_Z_check
//};
//
//enum CSS_MenuTag{
//	kCSS_MT_close = 1,
//	kCSS_MT_align = 2,
//	kCSS_MT_diary = 3,
//	kCSS_MT_selectedCard = 4,
//	kCSS_MT_selectedCheck = 5,
//	kCSS_MT_checkMark = 6,
//	kCSS_MT_cardBase = 1000,
//	kCSS_MT_cardMenuBase = 2000,
//	kCSS_MT_noCardBase = 3000
//};

// on "init" you need to initialize your instance
bool CardSettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	setKeypadEnabled(true);
	
	CCSprite* cardsetting_back = CCSprite::create("cardsetting_back.png");
	cardsetting_back->setPosition(ccp(240,160));
	addChild(cardsetting_back, kCSS_Z_back);
	
	my_clv = CardListViewer::create();
	addChild(my_clv, kCSS_Z_content);
	
	int sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(selected_card_number > 0)
	{
		int card_stage = selected_card_number/10;
		int card_level = selected_card_number%10 + 1;
		
		mountingCard(card_stage, card_level);
		
		check_img = CCSprite::create("card_check.png");
		check_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((card_level-1)*65, -(card_stage-1)*82)));
		my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
	}
	else
	{
		selected_card_img = NULL;
		selected_img = NULL;
		star_parent = NULL;
		card_option_case = NULL;
		card_option_script = NULL;
		selected_card_menu = NULL;
		check_img = NULL;
	}
	
	recent_mounted_number = selected_card_number;
	
	for(int i=1;i<=mySD->getLastUpdateStageNumber();i++)
	{
		for(int j=1;j<=3;j++)
		{
			int card_stage = i;
			int card_level = j;
			
			CLV_Node* t_node = CLV_Node::create(card_stage, card_level, this, menu_selector(CardSettingScene::menuAction),
												ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((j-1)*65, -(i-1)*82)), my_clv->getViewRect());
			my_clv->addChild(t_node, kCSS_Z_content, t_node->getMyTag());
		}
	}
	
	my_clv->setMaxPositionY();
	
	ListViewerScroll* t_lvs = ListViewerScroll::create(CCRectMake(428, 27, 27, 187), my_clv, "card_scroll.png", ccp(440,35), ccp(440,208));
	t_lvs->setTouchEnabled(true);
	addChild(t_lvs, kCSS_Z_content);
	
	my_clv->setScroll(t_lvs);
	my_clv->setTouchEnabled(true);
	
	CCSprite* n_close = CCSprite::create("ui_common_close.png");
	CCSprite* s_close = CCSprite::create("ui_common_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(CardSettingScene::menuAction));
	close_item->setTag(kCSS_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kCSS_MT_close));
	addChild(close_menu, kCSS_Z_content);
	
	
	CCSprite* n_align = CCSprite::create("cardsetting_align.png");
	CCSprite* s_align = CCSprite::create("cardsetting_align.png");
	s_align->setColor(ccGRAY);
	
	CCMenuItem* align_item = CCMenuItemSprite::create(n_align, s_align, this, menu_selector(CardSettingScene::menuAction));
	align_item->setTag(kCSS_MT_align);
	
	CCMenu* align_menu = CCMenu::createWithItem(align_item);
	align_menu->setPosition(getContentPosition(kCSS_MT_align));
	addChild(align_menu, kCSS_Z_content);
	
	
	CCSprite* n_diary = CCSprite::create("cardsetting_diary.png");
	CCSprite* s_diary = CCSprite::create("cardsetting_diary.png");
	s_diary->setColor(ccGRAY);
	
	CCMenuItem* diary_item = CCMenuItemSprite::create(n_diary, s_diary, this, menu_selector(CardSettingScene::menuAction));
	diary_item->setTag(kCSS_MT_diary);
	
	CCMenu* diary_menu = CCMenu::createWithItem(diary_item);
	diary_menu->setPosition(getContentPosition(kCSS_MT_diary));
	addChild(diary_menu, kCSS_Z_content);
	
	align_list_img = NULL;
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
    return true;
}

CCPoint CardSettingScene::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCSS_MT_close)			return_value = ccp(450,295);
	else if(t_tag == kCSS_MT_align)		return_value = ccp(290,250);
	else if(t_tag == kCSS_MT_diary)		return_value = ccp(403,250);
	else if(t_tag == kCSS_MT_cardBase)	return_value = ccp(267,180);
	
	return return_value;
}

void CardSettingScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kCSS_MT_close)
	{
		if(mySGD->before_cardsetting == kSceneCode_WorldMapScene)
			CCDirector::sharedDirector()->replaceScene(WorldMapScene::scene());
		else if(mySGD->before_cardsetting == kSceneCode_StageSetting)
			CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
	else if(tag == kCSS_MT_align)
	{
		if(align_list_img)
		{
			align_list_img->removeFromParent();
			align_list_img = NULL;
		}
		else
		{
			align_list_img = CCSprite::create("cardsetting_alignback.png");
			align_list_img->setAnchorPoint(ccp(0.5f,1.f));
			align_list_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_align), ccp(0,-15)));
			addChild(align_list_img, kCSS_Z_alignList);
		}
		
		is_menu_enable = true;
	}
	else if(tag == kCSS_MT_diary)
	{
		if(recent_mounted_number != 0)
		{
			mySGD->selected_collectionbook = recent_mounted_number;
			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
		}
		else
		{
			is_menu_enable = true;
		}
	}
	else if(tag == kCSS_MT_selectedCard)
	{
		if(recent_mounted_number != 0)
		{
			mySGD->selected_collectionbook = recent_mounted_number;
			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
		}
		else
		{
			is_menu_enable = true;
		}
	}
	else if(tag >= kCSS_MT_cardMenuBase && tag < kCSS_MT_noCardBase)
	{
		int clicked_card_number = tag-kCSS_MT_cardMenuBase;
		
		if(clicked_card_number != recent_mounted_number)
		{
			removeMountingCard();
			recent_mounted_number = clicked_card_number;
			int card_stage = recent_mounted_number/10;
			int card_level = recent_mounted_number%10 + 1;
			mountingCard(card_stage, card_level);
		}
		else if(myDSH->getIntegerForKey(kDSH_Key_selectedCard) != clicked_card_number && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, clicked_card_number) > 0)
		{
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, clicked_card_number);
			
			if(check_img)
			{
				check_img->removeFromParent();
				check_img = NULL;
			}
			
			int card_stage = clicked_card_number/10;
			int card_level = clicked_card_number%10 + 1;
			
			check_img = CCSprite::create("card_check.png");
			check_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((card_level-1)*65, -(card_stage-1)*82)));
			my_clv->addChild(check_img, kCSS_Z_check, kCSS_MT_checkMark);
		}
		
		is_menu_enable = true;
	}
	else if(tag >= kCSS_MT_noCardBase)
	{
		if(recent_mounted_number != 0)
			removeMountingCard();
		else
		{
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
			if(check_img)
			{
				check_img->removeFromParent();
				check_img = NULL;
			}
		}
		is_menu_enable = true;
	}
}

void CardSettingScene::removeMountingCard()
{
	if(selected_card_img)		selected_card_img->removeFromParent();
	selected_card_img = NULL;
	if(star_parent)				star_parent->removeFromParent();
	star_parent = NULL;
	
	if(card_option_case)		card_option_case->removeFromParent();
	card_option_case = NULL;
	if(card_option_script)		card_option_script->removeFromParent();
	card_option_script = NULL;
	
	if(selected_card_menu)		selected_card_menu->removeFromParent();
	selected_card_menu = NULL;
	
	if(selected_img)			selected_img->removeFromParent();
	selected_img = NULL;
	
	recent_mounted_number = 0;
}

void CardSettingScene::mountingCard(int card_stage, int card_level)
{
	selected_card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_level)->getCString());
	selected_card_img->setScale(0.58);
	selected_card_img->setPosition(ccp(117,144));
	addChild(selected_card_img, kCSS_Z_content);
	
	if(card_level == 3 && mySD->isAnimationStage(card_stage))
	{
		CCSize ani_size = mySD->getAnimationCutSize(card_stage);
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_level)->getCString(),
										   CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(mySD->getAnimationPosition(card_stage));
		selected_card_img->addChild(t_ani);
	}
	
	star_parent = CCNode::create();
	star_parent->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(80,110)));
	addChild(star_parent, kCSS_Z_content);
	
	for(int i=card_level-1;i>=0;i--)
	{
		CCSprite* card_star = CCSprite::create("card_star.png");
		card_star->setPosition(ccp(i*(-20),0));
		star_parent->addChild(card_star);
	}
	
	card_option_case = CCSprite::create("card_option_case.png");
	card_option_case->setPosition(ccpAdd(selected_card_img->getPosition(), ccp(0,-110)));
	addChild(card_option_case, kCSS_Z_content);
	
	card_option_script = CCLabelTTF::create(CCString::createWithFormat("%d/%d | %s",
																	   myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_stage*10 + card_level-1),
																	   mySD->getCardDurability(card_stage, card_level),
																	   mySD->getCardOptionScript(card_stage, card_level).c_str())->getCString(),
											mySGD->getFont().c_str(), 12);
	card_option_script->setColor(ccBLACK);
	card_option_script->setHorizontalAlignment(kCCTextAlignmentCenter);
	card_option_script->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	card_option_script->setPosition(card_option_case->getPosition());
	addChild(card_option_script, kCSS_Z_content);
	
	
	CCMenuItem* selected_card_item = CCMenuItemImage::create("cardsetting_cardmenu_big.png", "cardsetting_cardmenu_big.png", this, menu_selector(CardSettingScene::menuAction));
	selected_card_item->setTag(kCSS_MT_selectedCard);
	
	selected_card_menu = CCMenu::createWithItem(selected_card_item);
	selected_card_menu->setPosition(selected_card_img->getPosition());
	addChild(selected_card_menu, kCSS_Z_content);
	
	selected_img = CCSprite::create("card_selected.png");
	selected_img->setPosition(ccpAdd(getContentPosition(kCSS_MT_cardBase), ccp((card_level-1)*65, -(card_stage-1)*82)));
	my_clv->addChild(selected_img, kCSS_Z_selectedImg, kCSS_MT_selectedCheck);
}

void CardSettingScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void CardSettingScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(CardSettingScene::alertAction));
}