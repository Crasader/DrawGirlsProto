//
//  CumberShowWindow.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 21..
//
//

#include "CumberShowWindow.h"
#include "jsoncpp/json.h"
#include "KSCircleBase.h"
#include "KSSnakeBase.h"
#include "KSJuniorBase.h"

//#include "CCBReader.h"
//#include "cocos-ext.h"
//USING_NS_CC_EXT;

bool CumberShowWindow::init(int ss)
{
	
//	std::string bossInfo = R"([{"shape":"snake","type":"apple","pattern":[{"speed":200,"numberperframe":5,"percent":1,"target":"no","atype":"normal","pattern":"101","color":1}],"speed":{"max":3,"start":2,"min":0.3},"scale":{"max":0.8,"start":0.7,"min":0.4},"movement":{"normal":2,"draw":2},"attackpercent":0.5,"hp":100,"agi":1,"ai":50}])";
//	
//	std::string juniorInfo = R"([{"type":"cow","speed":{"max":1,"start":0.5,"min":0.2},"scale":{"max":1,"start":1,"min":1},"movement":{"normal":1,"draw":1},"hp":15,"agi":0,"ai":0},{"type":"1004","speed":{"max":0,"start":0,"min":0},"scale":{"max":0,"start":0,"min":0},"movement":{"normal":0,"draw":0},"hp":0,"agi":0,"ai":0}])";
	std::string bossInfo = mySDS->getStringForKey(kSDF_stageInfo, ss, "boss");
	std::string juniorInfo = mySDS->getStringForKey(kSDF_stageInfo, ss, "junior");

	Json::Reader reader;
	Json::Value bossJson;
	Json::Value juniorJson;
	reader.parse(bossInfo, bossJson);
	bossJson = bossJson[0u];
	reader.parse(juniorInfo, juniorJson);
	
	std::string bossShape = bossJson.get("shape", "circle").asString();
	
	if(bossShape == "circle")
	{
		std::string ccbiName = bossJson["type"].asString();
		std::string ccbiname2 = ccbiName;
		if(ccbiName.length()<3) {
			ccbiname2="cherry";
		}
		////////////////////////////////////////////
		
//		m_directionAngleDegree = m_well512.GetValue(0, 360);
		
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
		
		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		CCNode* p = reader->readNodeGraphFromFile(("boss_" + ccbiname2 + ".ccbi").c_str(), this);
		m_circleSprite = dynamic_cast<CircleBossCCB*>(p);
		m_circleAnimation = reader->getAnimationManager();
		m_circleAnimation->setDelegate(this);
		reader->release();
		
		if(m_circleSprite != NULL)
		{
			this->addChild(m_circleSprite);
			m_circleSprite->setScale(0.7f);
			
			if(juniorJson.size() > 0)
			{
				m_circleSprite->setPosition(ccp(-50,0));
			}
		}
	}
	else if(bossShape == "snake")
	{
		std::string ccbiName = bossJson["type"].asString();
		std::string ccbiname2 = ccbiName;
		if(ccbiName.length()<3) {
			ccbiname2="apple2";
		}
		m_snakeNode = KSSnakeBase::create(ccbiname2, false);
		addChild(m_snakeNode, 1000);
		m_snakeNode->setScale(0.7f);
		if(juniorJson.size() > 0)
		{
			m_snakeNode->setPosition(ccp(-50,0));
		}
		
		m_snakeNode->startAnimationNoDirection();
//		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//		{
//			CCBReader* reader = new CCBReader(nodeLoader);
//			auto fileName = "boss_"+ccbiFile+"_"+"head.ccbi";
//			m_headImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(fileName.c_str(),this));
//			m_headAnimationManager = reader->getAnimationManager();
//			this->addChild(m_headImg, 10);
//			reader->release();
//		}
//		int lastZ=-1;
//		{
//			
//			for(int i=0; i<7; i++)
//			{
//				CCBReader* reader = new CCBReader(nodeLoader);
//				auto fileName = "boss_"+ccbiFile+"_"+"body.ccbi";
//				CCSprite* body = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(fileName.c_str(),this));
//				m_bodyAnimationManagers.push_back(reader->getAnimationManager());
//				addChild(body, 9 - i);
//				lastZ = 9 - i;
//				m_Bodies.push_back(body);
//				reader->release();
//			}
//		}
//		
//		
//		{
//			CCBReader* reader = new CCBReader(nodeLoader);
//			auto fileName = "boss_"+ccbiFile+"_"+"tail.ccbi";
//			m_tailImg = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(fileName.c_str(),this));
//			m_tailAnimationManager = reader->getAnimationManager();
//			this->addChild(m_tailImg, lastZ - 1);
//			reader->release();
//		}
		
		
	}
	
	if(juniorJson.size() > 0)
	{
		int juniorIndex = m_well512.GetValue(0, juniorJson.size() - 1);
		juniorJson = juniorJson[juniorIndex];
		{
			std::string juniorType = juniorJson["type"].asString();
			std::string ccbiName = juniorType;
			std::string ccbiname2 = ccbiName;
			if(ccbiName.length()<3) {
				ccbiname2="bear";
			}
			////////////////////////////////////////////
			
			//		m_directionAngleDegree = m_well512.GetValue(0, 360);
			
			//		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			//		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
			
			//		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			
			std::string _ccbiName = ("mob_" + ccbiname2 + ".ccbi").c_str();
			CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(nodeLoader);
			CCNode* p = reader->readNodeGraphFromFile(_ccbiName.c_str(),this);
			
			m_juniorSprite = dynamic_cast<CCSprite*>(p);
			m_juniorAnimation = reader->getAnimationManager();
			m_juniorAnimation->setDelegate(this);
			reader->release();
			
			if(m_juniorSprite != NULL)
			{
				this->addChild(m_juniorSprite, 1001);
				m_juniorSprite->setScale(0.7f);
				m_juniorSprite->setPosition(ccp(50, 0));
				
				if(juniorJson.size() > 1)
				{
					CCLabelTTF* junior_cnt_label = CCLabelTTF::create(CCString::createWithFormat("x %d", juniorJson.size())->getCString(), mySGD->getFont().c_str(), 30);
					junior_cnt_label->setAnchorPoint(ccp(0,0.5));
					junior_cnt_label->setPosition(ccp(70,-20));
					addChild(junior_cnt_label, 1002);
				}
			}
		}
	}
	
//	std::string juniorType = juniorJson["type"].asString();

	
	return true;
}