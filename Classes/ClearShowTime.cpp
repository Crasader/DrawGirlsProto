//
//  ClearShowTime.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 26..
//
//

#include "ClearShowTime.h"

#include "GameData.h"
#include "AudioEngine.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#include "StageImgLoader.h"
#include "DataStorageHub.h"
#include "EffectSprite.h"

ClearShowTime* ClearShowTime::create( bool t_exchanged, bool t_is, CCNode* t_game_node, CCObject* t_changeScene, SEL_CallFunc d_changeScene )
{
	ClearShowTime* t_cst = new ClearShowTime();
	t_cst->myInit(t_exchanged, t_is, t_game_node, t_changeScene, d_changeScene);
	t_cst->autorelease();
	return t_cst;
}

void ClearShowTime::visit()
{
	if(!is_end_schedule)
	{
		glEnable(GL_SCISSOR_TEST);
		
		int viewport [4];
		glGetIntegerv (GL_VIEWPORT, viewport);
		CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
		float wScale = viewport[2] / rSize.width;
		float hScale = viewport[3] / rSize.height;
		float x = view_rect.origin.x*wScale + viewport[0];
		float y = view_rect.origin.y*hScale + viewport[1];
		float w = view_rect.size.width*wScale;
		float h = view_rect.size.height*hScale;
		glScissor(x,y,w,h);
		
		CCNode::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	else
	{
		CCNode::visit();
	}
}

void ClearShowTime::startClearView(CCPoint t_p)
{
	side_back->setPosition(getPosition() - t_p);
	
	frame_cnt = 0;
	schedule(schedule_selector(ClearShowTime::addViewHeight));
}

void ClearShowTime::changeScene()
{
	(target_changeScene->*delegate_changeScene)();
}

void ClearShowTime::addViewHeight()
{
	frame_cnt++;
	view_rect.origin.y -= myDSH->ui_top/40.f;
	view_rect.size.height += myDSH->ui_top/20.f;

	if(frame_cnt >= 20)
	{
		unschedule(schedule_selector(ClearShowTime::addViewHeight));

//		int remove_target = 0;
//		while(game_node->getChildrenCount() <= 1)
//		{
//			CCNode* t_target = (CCNode*)game_node->getChildren()->objectAtIndex(remove_target);
//			if(t_target == this)
//			{
//				remove_target++;
//			}
//			else
//			{
//				game_node->removeChild(t_target, true);
//			}
//		}

		changeScene();
		is_end_schedule = true;
//		CCDelayTime* t_delay = CCDelayTime::create(1.5f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearShowTime::changeScene));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//		runAction(t_seq);
	}
}

void ClearShowTime::myInit( bool t_exchanged, bool t_is, CCNode* t_game_node, CCObject* t_changeScene, SEL_CallFunc d_changeScene )
{
	is_end_schedule = false;
	is_showtime = t_is;
	game_node = t_game_node;
	target_changeScene = t_changeScene;
	delegate_changeScene = d_changeScene;

	mySGD->is_exchanged = t_exchanged;
	mySGD->is_showtime = is_showtime;

	view_rect = CCRectMake(0, myDSH->ui_center_y, 480, 0);

	silType = mySD->getSilType();

	string ttt, ttt2;

	CCSprite* ccb_img = NULL;
	
	if(t_exchanged)
	{
		ttt = CCString::createWithFormat("card%d_visible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3))->getCString();
		ttt2 = CCString::createWithFormat("card%d_invisible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3))->getCString();
		
//		if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3)))
//		{
//			ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, NSDS_GI(silType, kSDS_SI_level_int1_card_i, 3))).first;
//			ccb_img->setPosition(ccp(160,215));
//		}
	}
	else
	{
		ttt = CCString::createWithFormat("card%d_visible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1))->getCString();
		ttt2 = CCString::createWithFormat("card%d_invisible.png", NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1))->getCString();
		
//		if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1)))
//		{
//			ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, NSDS_GI(silType, kSDS_SI_level_int1_card_i, 1))).first;
//			ccb_img->setPosition(ccp(160,215));
//		}
	}

	clear_img = mySIL->getLoadedImg(ttt.c_str());
	clear_img->setPosition(ccp(160,215));
	addChild(clear_img, kCST_Z_clear);
	
	if(ccb_img)
	{
		clear_img->addChild(ccb_img);
	}
	
	if(mySGD->is_safety_mode)
	{
		EffectSprite* safety_img = EffectSprite::createWithTexture(mySIL->addImage(ttt2.c_str()));
		safety_img->setSilhouetteConvert(0);
		safety_img->setPosition(ccp(160, 215));
		addChild(safety_img, kCST_Z_clear);
	}

//	CCSpriteBatchNode* side_back = CCSpriteBatchNode::create("ingame_side_pattern.png");
//	addChild(side_back);
//	
//	CCSize pattern_size = side_back->getTexture()->getContentSize();
//	
//	for(int i=0;i*pattern_size.width < 480;i++)
//	{
//		for(int j=0;j*pattern_size.height < myDSH->ui_top;j++)
//		{
//			CCSprite* t_pattern = CCSprite::createWithTexture(side_back->getTexture());
//			t_pattern->setAnchorPoint(ccp(0,0));
//			t_pattern->setPosition(ccp(i*pattern_size.width,j*pattern_size.height));
//			side_back->addChild(t_pattern);
//		}
//	}
	
	side_back = CCSprite::create("ingame_outback.png");
	side_back->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(side_back);
	
//	CCSprite* top_back = CCSprite::create("top_back.png");
//	top_back->setAnchorPoint(ccp(0.5,0));
//	top_back->setPosition(ccp(160,430));
//	top_back->setScaleX((320.f+myGD->boarder_value*2.f)/480.f);
//	addChild(top_back);
//
//	CCSprite* bottom_back = CCSprite::create("bottom_back.png");
//	bottom_back->setAnchorPoint(ccp(0.5,1));
//	bottom_back->setPosition(ccp(160,0));
//	bottom_back->setScaleX((320.f+myGD->boarder_value*2.f)/480.f);
//	addChild(bottom_back);

	CCSprite* top_boarder = CCSprite::create("frame_top.png");
	top_boarder->setAnchorPoint(ccp(0.5,0));
	top_boarder->setPosition(ccp(160,430));
	//		top_boarder->setScaleX(34.f);
	addChild(top_boarder);

	CCSprite* bottom_boarder = CCSprite::create("frame_bottom.png");
	bottom_boarder->setAnchorPoint(ccp(0.5,1));
	bottom_boarder->setPosition(ccp(160,0));
	//		bottom_boarder->setScaleX(34.f);
	addChild(bottom_boarder);

	CCSprite* left_boarder = CCSprite::create("frame_left.png");
	left_boarder->setAnchorPoint(ccp(1,0.5));
	left_boarder->setPosition(ccp(0,215));
	//		left_boarder->setScaleY(45.f);
	addChild(left_boarder);

	CCSprite* right_boarder = CCSprite::create("frame_right.png");
	right_boarder->setAnchorPoint(ccp(0,0.5));
	right_boarder->setPosition(ccp(320,215));
	//		right_boarder->setScaleY(45.f);
	addChild(right_boarder);
}
