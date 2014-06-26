//
//  FormSetter.cpp
//  DGproto
//
//  Created by LitQoo on 2014. 3. 24..
//
//

#include "FormSetter.h"



void FormSetter::start(string filename, string startName){
	if(!m_isEnabledRemocon)return;
	m_startObjName=startName;
	m_filename = filename;
	this->retain();
	//		CCNode* front = (CCNode *)(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0));
	auto front2 = (CCDirector::sharedDirector()->getRunningScene());
	//remocon->setVisible(true);
	if(this->getParent()){
		this->removeFromParent();
		this->setParent(nullptr);
	}
	//		front->addChild(this,INT_MAX-10);
	front2->addChild(this,INT_MAX-10);
	this->setTouchEnabled(true);
	
	m_objList.clear();
	//this->schedule(schedule_selector(FormSetter::sch));
	
	
}
bool FormSetter::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	
	m_startPosition = pTouch->getLocation();
	
	if(!remocon->isVisible()){
		if(m_startPosition.x>475 && m_startPosition.y<15){
			
			if(this->getParent()){
				this->removeFromParent();
				this->setParent(nullptr);
			}
			CCNode* front = (CCNode *)(CCDirector::sharedDirector()->getRunningScene()); // ->getChildren()->objectAtIndex(0));
			front->addChild(this,100000);
			
			remocon->setVisible(true);
			m_swLayer->setTouchEnabled(true);
			m_guideLine->setVisible(true);
			if(m_guideImage)m_guideImage->setVisible(true);
			
			//m_objList.clear();
			findObject(front);
			if(m_objList.size()>0){
				for(int i=0;i<m_objList.size();i++){
					CCNode* obj = (CCNode*)m_objList[i].obj;
					if(obj->getStringData()==m_startObjName){
						m_selectedObjNumber=i;
					}
				}
				if(m_selectedObjNumber<=0)m_selectedObjNumber = m_objList.size()-1;
				CCNode* selectedObj = (CCNode*)m_objList[m_selectedObjNumber].obj;
				selectedObj->runAction(CCBlink::create(1, 5));
				setInfomation();
			}else{
				m_selectedObjNumber=-1;
				m_objName->setString("don't find");
				m_objInfo->setString("don't find");
			}
			
		}
	}
	return true;
}