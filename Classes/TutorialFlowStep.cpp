//
//  TutorialFlowStep.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 5..
//
//

#include "TutorialFlowStep.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "GraySprite.h"
#include "StageImgLoader.h"
#include "PuzzleScene.h"

bool TutorialFlowStepLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	return true;
}

void TutorialFlowStepLayer::initStep(TutorialFlowStep t_step)
{
	recent_step = t_step;
	if(t_step == kTutorialFlowStep_puzzleClick)
	{
		setTouchEnabled(true);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		gray->setPosition(ccp(240,160));
		addChild(gray);
		
		CCSprite* girl_img = CCSprite::create("tutorial_girl.png");
		girl_img->setAnchorPoint(ccp(0.5,0));
		girl_img->setPosition(ccp(170, -(screen_scale_y-1.f)*320.f/2.f));
		addChild(girl_img);
		
		CCSprite* toke_box = CCSprite::create("tutorial_girlbox.png");
		toke_box->setAnchorPoint(ccp(0,3.f/4.f));
		toke_box->setPosition(ccp(140, 150));
		girl_img->addChild(toke_box);
		
		CCLabelTTF* start_ment = CCLabelTTF::create("반가워요!\n지금부터 튜토리얼을 시작합니다.", mySGD->getFont().c_str(), 13);
		start_ment->setColor(ccBLACK);
		start_ment->setPosition(ccp(120,43));
		toke_box->addChild(start_ment);
	}
	else if(t_step == kTutorialFlowStep_pieceClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.48f);
		stencil_node->setPosition(ccp(-12,71));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(55,0))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("피스를 선택해주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width/2.f+2, 0)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_readyClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.45f);
		stencil_node->setPosition(ccp(167,-130));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setRotation(-90);
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(0,45))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("READY를 눌러주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0.5,0));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(0, arrow->getContentSize().height/2.f)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_startClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCNode* stencil_node = CCNode::create();
		
		CCSprite* stencil_node1 = CCSprite::create("tutorial_clipping.png");
		stencil_node1->setScale(0.3f);
		stencil_node1->setPosition(ccp(-60,98));
		stencil_node->addChild(stencil_node1);
		
		CCSprite* stencil_node2 = CCSprite::create("tutorial_clipping.png");
		stencil_node2->setScale(0.45f);
		stencil_node2->setPosition(ccp(82,-115));
		stencil_node->addChild(stencil_node2);
		
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		
		CCSprite* arrow1 = CCSprite::create("tutorial_arrow.png");
		arrow1->setRotation(90);
		arrow1->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node1->getPosition(), ccp(0,-40))));
		addChild(arrow1);
		
		CCLabelTTF* script1 = CCLabelTTF::create("클리어 조건을 확인하세요.", mySGD->getFont().c_str(), 13);
		script1->setAnchorPoint(ccp(0.5,1));
		script1->setPosition(ccpAdd(arrow1->getPosition(), ccp(0, -arrow1->getContentSize().height/2.f)));
		addChild(script1);
		
		CCSprite* arrow2 = CCSprite::create("tutorial_arrow.png");
		arrow2->setRotation(-90);
		arrow2->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node2->getPosition(), ccp(0,45))));
		addChild(arrow2);
		
		CCLabelTTF* script2 = CCLabelTTF::create("START를 누르면 게임이 시작됩니다.", mySGD->getFont().c_str(), 13);
		script2->setAnchorPoint(ccp(0.5,0));
		script2->setPosition(ccpAdd(arrow2->getPosition(), ccp(0, arrow2->getContentSize().height/2.f)));
		addChild(script2);
	}
	else if(t_step == kTutorialFlowStep_ingame)
	{
		
	}
	else if(t_step == kTutorialFlowStep_homeClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.45f);
		stencil_node->setPosition(ccp(90,-128));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setRotation(-90);
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(0,45))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("HOME을 눌러주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0.5,0));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(0, arrow->getContentSize().height/2.f)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_pieceType)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		gray->setPosition(ccp(240,160));
		addChild(gray);
		
		int puzzle_number = 1;
		int piece_no = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(puzzle_number, kSDS_PZ_startStage_i))-1;
		string piece_type;
		if(piece_no%2 == 0)
			piece_type = "h";
		else
			piece_type = "w";
		
		GraySprite* gray_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number, piece_no)->getCString()));
		gray_piece->setGray(true);
		gray_piece->setPosition(ccpAdd(ccp(240,160), ccp(-140, 20)));
		addChild(gray_piece);
		
		CCSprite* gray_stroke = CCSprite::create(("piece_stroke_" + piece_type + ".png").c_str());
		gray_stroke->setPosition(ccp(gray_piece->getContentSize().width/2.f, gray_piece->getContentSize().height/2.f));
		gray_piece->addChild(gray_stroke);
		
		CCLabelTTF* step1_title = CCLabelTTF::create("1단계", mySGD->getFont().c_str(), 10);
		step1_title->setAnchorPoint(ccp(0.5,0));
		step1_title->setPosition(ccp(gray_piece->getContentSize().width/2.f, gray_piece->getContentSize().height+2));
		gray_piece->addChild(step1_title);
		
		CCLabelTTF* step1_condition = CCLabelTTF::create("85%이상 달성", mySGD->getFont().c_str(), 10);
		step1_condition->setAnchorPoint(ccp(0.5,1));
		step1_condition->setPosition(ccp(gray_piece->getContentSize().width/2.f, -2));
		gray_piece->addChild(step1_condition);
		
		
		CCSprite* stroke_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number, piece_no)->getCString());
		stroke_piece->setPosition(ccpAdd(ccp(240,160), ccp(0, 20)));
		addChild(stroke_piece);
		
		CCSprite* stroke_stroke = CCSprite::create(("piece_stroke_" + piece_type + ".png").c_str());
		stroke_stroke->setPosition(ccp(stroke_piece->getContentSize().width/2.f, stroke_piece->getContentSize().height/2.f));
		stroke_piece->addChild(stroke_stroke);
		
		CCLabelTTF* step2_title = CCLabelTTF::create("2단계", mySGD->getFont().c_str(), 10);
		step2_title->setAnchorPoint(ccp(0.5,0));
		step2_title->setPosition(ccp(stroke_piece->getContentSize().width/2.f, stroke_piece->getContentSize().height+2));
		stroke_piece->addChild(step2_title);
		
		CCLabelTTF* step2_condition = CCLabelTTF::create("체인지 성공 또는 100% 달성", mySGD->getFont().c_str(), 10);
		step2_condition->setAnchorPoint(ccp(0.5,1));
		step2_condition->setPosition(ccp(stroke_piece->getContentSize().width/2.f, -2));
		stroke_piece->addChild(step2_condition);
		
		
		CCSprite* color_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number, piece_no)->getCString());
		color_piece->setPosition(ccpAdd(ccp(240,160), ccp(140, 20)));
		addChild(color_piece);
		
		CCLabelTTF* step3_title = CCLabelTTF::create("3단계", mySGD->getFont().c_str(), 10);
		step3_title->setAnchorPoint(ccp(0.5,0));
		step3_title->setPosition(ccp(color_piece->getContentSize().width/2.f, color_piece->getContentSize().height+2));
		color_piece->addChild(step3_title);
		
		CCLabelTTF* step3_condition = CCLabelTTF::create("체인지 성공 후 100% 달성", mySGD->getFont().c_str(), 10);
		step3_condition->setAnchorPoint(ccp(0.5,1));
		step3_condition->setPosition(ccp(color_piece->getContentSize().width/2.f, -2));
		color_piece->addChild(step3_condition);
		
		
		CCLabelTTF* main_script = CCLabelTTF::create("스테이지를 클리어 하면 피스를 획득할 수 있어요.\n클리어 단계에 따라 피스가 달라요.", mySGD->getFont().c_str(), 15);
		main_script->setPosition(ccp(240,75));
		addChild(main_script);
		
		setTouchEnabled(true);
	}
	else if(t_step == kTutorialFlowStep_pieceClick2)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.48f);
		stencil_node->setPosition(ccp(-69,13));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(55,0))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("피스를 선택해주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width/2.f+2, 0)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_readyClick2)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.45f);
		stencil_node->setPosition(ccp(167,-130));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setRotation(-90);
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(0,45))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("READY를 눌러주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0.5,0));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(0, arrow->getContentSize().height/2.f)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_emptyCardClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(1.f);
		stencil_node->setPosition(ccp(-124,-25));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(65,-25))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("카드를 장착해볼까요?", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width/2.f+2, 0)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_targetCardClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.35f);
		stencil_node->setPosition(ccp(-10,-97));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(45,0))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("장착할 카드를 선택해주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width/2.f+2, 0)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_mountClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.35f);
		stencil_node->setPosition(ccp(-135,-116));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(45,0))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("장착해주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width/2.f+2, 0)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_closeClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCNode* stencil_node = CCNode::create();
		
		CCSprite* stencil_node1 = CCSprite::create("tutorial_clipping.png");
		stencil_node1->setScale(1.f);
		stencil_node1->setPosition(ccp(-135,-20));
		stencil_node->addChild(stencil_node1);
		
		CCSprite* stencil_node2 = CCSprite::create("tutorial_clipping.png");
		stencil_node2->setScale(0.3f);
		stencil_node2->setPosition(ccp(200,100));
		stencil_node->addChild(stencil_node2);
		
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		
		CCLabelTTF* mount_complete = CCLabelTTF::create("장착 완료!", mySGD->getFont().c_str(), 13);
		mount_complete->setPosition(ccpAdd(ccp(240,160), ccp(-135,-20)));
		addChild(mount_complete);
		
		CCSprite* mount_complete_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, mount_complete->getContentSize().width, mount_complete->getContentSize().height));
		mount_complete_back->setColor(ccBLACK);
		mount_complete_back->setOpacity(100);
		mount_complete_back->setAnchorPoint(CCPointZero);
		mount_complete->addChild(mount_complete_back, -1);
		
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setRotation(180);
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node2->getPosition(), ccp(-45,0))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("장착이 완료되었습니다.\n닫기버튼을 누른 후 게임을 진행하세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(1,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(-arrow->getContentSize().width/2.f-2, 0)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_backClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.3f);
		stencil_node->setPosition(ccp(-205,320*screen_scale_y/2.f-20));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(45,0))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("카드 강화를 위해 메인으로 가실께요!", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width/2.f+2, 0)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_cardCollectionClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.33f);
		stencil_node->setPosition(ccp(-73,-320*screen_scale_y/2.f+17));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setRotation(-90);
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(0,42))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("카드를 강화해볼까요?", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0.5,0));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(0, arrow->getContentSize().height/2.f+2)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_upgradeClick)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.36f);
		stencil_node->setPosition(ccp(-168,-113));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setRotation(-90);
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(0,42))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("카드가 장착된 상태에서 강화를 눌러주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0.2,0));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(0, arrow->getContentSize().height/2.f+2)));
		addChild(script);
	}
	else if(t_step == kTutorialFlowStep_upgradeScript)
	{
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.4f);
		stencil_node->setPosition(ccp(168,-10));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setRotation(90);
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(0,-37))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("재료카드를 선택하고 강화를 눌러주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0.8,1));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(0, -arrow->getContentSize().height/2.f)));
		addChild(script);
		
		CCSprite* arrow2 = CCSprite::create("tutorial_arrow.png");
		arrow2->setRotation(180);
		arrow2->setPosition(ccpAdd(ccp(240,160), ccp(-170,-100)));
		addChild(arrow2);
		
		CCLabelTTF* script2 = CCLabelTTF::create("재료 카드 목록", mySGD->getFont().c_str(), 15);
		script2->setAnchorPoint(ccp(0.2,0));
		script2->setPosition(ccpAdd(arrow2->getPosition(), ccp(0,arrow2->getContentSize().height/2.f+4)));
		addChild(script2);
		
		setTouchEnabled(true);
	}
}

bool TutorialFlowStepLayer::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	return true;
}
void TutorialFlowStepLayer::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void TutorialFlowStepLayer::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	if(recent_step == kTutorialFlowStep_puzzleClick)
	{
		setTouchEnabled(false);
		removeAllChildren();
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		
		CCSprite* stencil_node = CCSprite::create("tutorial_clipping.png");
		stencil_node->setScale(0.8f);
		stencil_node->setPosition(ccp(-170+(-480.f*screen_scale_x+480.f)/2.f,10));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
		float change_scale = 1.f;
		CCPoint change_origin = ccp(0,0);
		if(screen_scale_x > 1.f)
		{
			change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
			change_scale = screen_scale_x;
		}
		if(screen_scale_y > 1.f)
			change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
		CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->addChild(gray);
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setInverted(true);
		cliping_node->setPosition(ccp(240,160));
		addChild(cliping_node);
		
		CCSprite* arrow = CCSprite::create("tutorial_arrow.png");
		arrow->setPosition(ccpAdd(cliping_node->getPosition(), ccpAdd(stencil_node->getPosition(), ccp(70,0))));
		addChild(arrow);
		
		CCLabelTTF* script = CCLabelTTF::create("퍼즐테마를 선택해주세요.", mySGD->getFont().c_str(), 13);
		script->setAnchorPoint(ccp(0,0.5));
		script->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width/2.f+2, 0)));
		addChild(script);
	}
	else if(recent_step == kTutorialFlowStep_pieceType)
	{
		myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_pieceClick2);
		
		CCNode* parent_node = getParent();
		
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_pieceClick2);
		parent_node->addChild(t_tutorial, kPuzzleZorder_popup);
		
		((PuzzleScene*)parent_node)->tutorial_node = t_tutorial;
		removeFromParent();
	}
	else if(recent_step == kTutorialFlowStep_upgradeScript)
	{
		myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_end);
		recent_step = kTutorialFlowStep_end;
		
		removeAllChildren();
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(screen_scale_y);
		gray->setPosition(ccp(240,160));
		addChild(gray);
		
		CCSprite* girl_img = CCSprite::create("tutorial_girl.png");
		girl_img->setAnchorPoint(ccp(0.5,0));
		girl_img->setPosition(ccp(170, -(screen_scale_y-1.f)*320.f/2.f));
		addChild(girl_img);
		
		CCSprite* toke_box = CCSprite::create("tutorial_girlbox.png");
		toke_box->setAnchorPoint(ccp(0,3.f/4.f));
		toke_box->setPosition(ccp(140, 150));
		girl_img->addChild(toke_box);
		
		CCLabelTTF* start_ment = CCLabelTTF::create("수고하셨어요!\n이제 친구들과 경쟁도 하면서\n즐거운 게임되세요~!", mySGD->getFont().c_str(), 13);
		start_ment->setColor(ccBLACK);
		start_ment->setPosition(ccp(120,43));
		toke_box->addChild(start_ment);
	}
	else if(recent_step == kTutorialFlowStep_end)
	{
		removeFromParent();
	}
}
void TutorialFlowStepLayer::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void TutorialFlowStepLayer::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -200, true);
}
