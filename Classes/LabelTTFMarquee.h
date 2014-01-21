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


using namespace std;
using namespace cocos2d;

class LabelTTFMarquee : public CCLayer {
		
public:
	
	CCLabelTTF *m_text1;
	CCLabelTTF *m_text2;
	
	vector<string> m_texts;
	
	bool init(){
		if(!CCLayer::init()){
			return false;
		}
		
		
		m_text1 = CCLabelTTF::create("부하몬스터 5마리를 잡으세요!!", "", 10);
		m_text1->setPosition(ccp(0,0));
		
		m_text2 = CCLabelTTF::create("지금까지 3마리 포획 성공!", "", 10);
		m_text2->setPosition(ccp(0,0));
		
		this->addChild(m_text1,4);
		this->addChild(m_text2,4);
				
		this->schedule(schedule_selector(LabelTTFMarquee::marquee));
		return true;
	}
	
	void marquee(float dt){
		setTextPosition(true);
	}
	
	static CCLabelTTFMarquee* create(){
		LabelTTFMarquee* obj = new LabelTTFMarquee();
		obj->init();
		obj->autorelease();
		return obj;
	}
	
	void setTextPosition(bool move){
		if(m_text1->getPositionX()<m_text2->getPositionX()){
			
			CCSize m1Size = m_text1->getContentSize();
			CCPoint m1Point = m_text1->getPosition();
			
			m_text2->setPositionX(m1Point.x + 200);
			if(move){
				m_text1->setPositionX(m_text1->getPositionX()-1);
				if(m_text1->getPositionX()<0){
					m_text1->setPositionX(m_text2->getPositionX()+200);
				}
			}
		}else{
			CCSize m2Size = m_text2->getContentSize();
			CCPoint m2Point = m_text2->getPosition();
			
			m_text1->setPositionX(m2Point.x + 200);
			
			if(move){
				m_text2->setPositionX(m_text2->getPositionX()-1);
				if(m_text2->getPositionX()<0){
					m_text2->setPositionX(m_text1->getPositionX()+200);
				}
			}
		}
	}
	
};

#endif /* defined(__DGproto__CCLabelTTFMarquee__) */
