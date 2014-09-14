//
//  IntroducerPopup.h
//  DGproto
//
//  Created by ksoo k on 2014. 9. 13..
//
//

#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LoadingLayer;
class TouchSuctionLayer;
class KSLabelTTF;
class IntroducerPopup : public CCLayer, public CCEditBoxDelegate
{
public:
	static IntroducerPopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_success_func);
	
private:
	int m_touchPriority;
	function<void()> endFunctor;
	function<void()> successFunctor;
	
	TouchSuctionLayer* suction;
	
	CCEditBox* input_text1;
	//	CCEditBox* input_text2;
	//	CCEditBox* input_text3;
	
	CCSprite* back, *front;
};


