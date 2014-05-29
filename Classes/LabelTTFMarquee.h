//
//  CCLabelTTFMarquee.h
//  DGproto
//
//  Created by LitQoo on 2014. 1. 21..
//
//

#ifndef __DGproto__CCLabelTTFMarquee__
#define __DGproto__CCLabelTTFMarquee__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "StarGoldData.h"
#include "StyledLabelTTF.h"

using namespace std;
using namespace cocos2d;

class LabelTTFMarquee : public CCNode {
		
public:
	
	StyledLabelTTF* m_text1;
	StyledLabelTTF* m_text2;
	CCSprite* m_back;
	vector<string> m_texts;
	int m_strIndex;
	int m_stopIndex;
	int m_isMoving;
	bool init(const ccColor4B &color, GLfloat width, GLfloat height, string text){
		if(!CCNode::init()){
			return false;
		}
		m_isMoving = false;
		m_strIndex = 0;
		m_stopIndex= -1;
		m_texts.push_back(text);
		
		this->setContentSize(CCSizeMake(width, height));
//		m_back = CCLayerColor::create(color,width,height);
//		m_back->setOpacity(color.a);
//		m_back->setAnchorPoint(ccp(0.5,0.5));
//		m_back->setPosition(ccp(0,0));

		m_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, width, height));
		//m_back->setOpacity(color.a);
		m_back->setAnchorPoint(ccp(0,0));
		m_back->setPosition(ccp(0,0));
		//this->addChild(m_back, 1);
		
		
		
		CCClippingNode* cNode = CCClippingNode::create();
		cNode->setContentSize(CCSizeMake(width,height));
		cNode->setAnchorPoint(ccp(0,0));
		cNode->setPosition(ccp(0,0));
		cNode->setStencil(m_back);
		cNode->setInverted(false);
		this->addChild(cNode,1);
		
		m_text1 = StyledLabelTTF::create("<font color=999 size=12>load..</font>", mySGD->getFont().c_str(),12,999, StyledAlignment::kLeftAlignment);
		
		m_text1->setAnchorPoint(ccp(0,0.5));
		m_text1->setPosition(ccp(m_text1->getContentSize().width*-1,height/2+6));
		m_text1->setTag(-2);
		
		
		m_text2 = StyledLabelTTF::create("<font color=999 size=12>load..</font>", mySGD->getFont().c_str(),12,999, StyledAlignment::kLeftAlignment);
		m_text2->setAnchorPoint(ccp(0,0.5));
		m_text2->setTag(0);
		m_text2->setPosition(ccp(0,height/2+6));
		
		cNode->addChild(m_text1,4);
		cNode->addChild(m_text2,4);
				
		
		return true;
	}


	
	void setFontSize(int size){
		//m_text1->setFontSize(size);
		//m_text2->setFontSize(size);
	}
	
	StyledLabelTTF* getFirstLabel(){
		if(m_text1->getPositionX()<m_text2->getPositionX())return m_text1;
		else return m_text2;
	}
	
	StyledLabelTTF* getLastLabel(){
		if(m_text1->getPositionX()<m_text2->getPositionX())return m_text2;
		else return m_text1;
	}
	
	int addText(string text){
		this->m_texts.push_back(text);
		return m_texts.size()-1;
	}
	
	bool setTextAtIndex(string text, int index){
		
		if(this->m_texts.size()<=index) return false;
		
		this->m_texts[index]=text;
		
		//지금현재 보여지고있는건 바로 수정
		StyledLabelTTF *fl = this->getFirstLabel();
		if(fl->getTag()==index){
			fl->setStringByTag(text.c_str());
		}
		
		StyledLabelTTF *ll = this->getLastLabel();
		if(ll->getTag()==index){
			ll->setStringByTag(text.c_str());
		}
		
		return true;
	}
	
	//해당 인덱스 스트링에서 멈추기
	void stopMarqueeAtIndex(int index){
		m_stopIndex=index;
	}
	
	//지금인덱스에서멈추기
	void stopMarqueeAtNowIndex(){
		this->stopMarqueeAtIndex(this->getLastLabel()->getTag());
	}
	
	void startMarquee(){
		m_stopIndex=-1;
		this->schedule(schedule_selector(LabelTTFMarquee::marquee));
	}
	
	void stopMarquee(){
		this->unschedule(schedule_selector(LabelTTFMarquee::marquee));
	}
	
	
	void marquee(float dt){
		setTextPosition();
	}
	
	static LabelTTFMarquee* create(const ccColor4B &color, GLfloat width, GLfloat height, string text){
		LabelTTFMarquee* obj = new LabelTTFMarquee();
		obj->init(color,width,height,text);
		obj->autorelease();
		return obj;
	}
	
	int getNextTextIndex(){
		m_strIndex++;
		if(m_texts.size()<=m_strIndex){
			m_strIndex=0;
		}
		
		return m_strIndex;
	}
	
	void setTextPosition(){
		StyledLabelTTF* firstLabel = this->getFirstLabel();
		StyledLabelTTF* lastLabel = this->getLastLabel();
		
		float labelTerm =firstLabel->getContentSize().width + 100;
		if(firstLabel->getContentSize().width>labelTerm){
			labelTerm = firstLabel->getContentSize().width + 100;
		}
		
		firstLabel->setPositionX(firstLabel->getPositionX()-1);
		lastLabel->setPositionX(lastLabel->getPositionX()-1);
		if(firstLabel->getTag()==m_stopIndex && firstLabel->getPositionX()<5 && firstLabel->getPositionX()>=0){
				m_stopIndex=-1;
				this->stopMarquee();
				return;
		}
		
		if(firstLabel->getPositionX()<firstLabel->getContentSize().width*-1){

			float nextP = lastLabel->getPositionX() + lastLabel->getContentSize().width + 100;
			if(nextP < this->getContentSize().width)nextP = this->getContentSize().width;
			firstLabel->setPositionX(nextP);
			int strIndex = this->getNextTextIndex();
			firstLabel->setTag(strIndex);
			firstLabel->setStringByTag(m_texts[strIndex].c_str());
		}
	}
};

#endif /* defined(__DGproto__CCLabelTTFMarquee__) */
