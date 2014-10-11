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
#include "StageImgLoader.h"

//#include "CCBReader.h"
//#include "cocos-ext.h"
//USING_NS_CC_EXT;

bool CumberShowWindow::init()
{
	boss_node = CCNode::create();
	boss_node->setPosition(CCPointZero);
	addChild(boss_node);
	
	std::string bossShape = "circle";
	
	CCPoint boss_position = ccp(0,0);
	
	if(bossShape == "circle")
	{
		std::string ccbiName = "boss_bee";
		std::string ccbiname2 = ccbiName;
		if(ccbiName.length()<3) {
			ccbiname2="boss_bee";
		}
		////////////////////////////////////////////
		
		//		m_directionAngleDegree = m_well512.GetValue(0, 360);
		
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
		
		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		//CCNode* p = reader->readNodeGraphFromFile(("boss_" + ccbiname2 + ".ccbi").c_str(), this);
		CCNode* p = reader->readNodeGraphFromFile((ccbiname2 + ".ccbi").c_str(), this);// readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+ccbiname2+".ccbi").c_str(), this);
		//		auto ret = KS::loadCCBIForFullPath<CircleBossCCB*>(this, (mySIL->getDocumentPath()+ccbiname2+".ccbi"));
		
		m_circleSprite = dynamic_cast<CircleBossCCB*>(p);//ret.first;
		m_circleAnimation = reader->getAnimationManager();// ret.second;
		m_circleAnimation->runAnimationsForSequenceNamed("Default Timeline");
//		m_circleAnimation->setDelegate(this);
		reader->release();
		
		if(m_circleSprite != NULL)
		{
			boss_node->addChild(m_circleSprite);
			m_circleSprite->setScale(0.7f);
			m_circleSprite->setPosition(boss_position);
		}
	}
//	else if(bossShape == "snake")
//	{
//		std::string ccbiName = bossJson["type"].asString();
//		std::string ccbiname2 = ccbiName;
//		if(ccbiName.length()<3) {
//			ccbiname2="apple2";
//		}
//		m_snakeNode = KSSnakeBase::create(ccbiname2, false);
//		boss_node->addChild(m_snakeNode, 1000);
//		m_snakeNode->setScale(0.7f);
//		m_snakeNode->setPosition(boss_position);
//		
//		m_snakeNode->startAnimationNoDirection();
//	}
	
	return true;
}

bool CumberShowWindow::init(int ss, CumberShowWindowSceneCode t_code)
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
	
	boss_node = CCNode::create();
	boss_node->setPosition(CCPointZero);
	addChild(boss_node);
	
	std::string bossShape = bossJson.get("shape", "circle").asString();
	
	CCPoint boss_position;
	if(t_code == kCumberShowWindowSceneCode_puzzle)
		boss_position = ccp(-15,45);
	else if(t_code == kCumberShowWindowSceneCode_cardChange)
		boss_position = ccp(0,0);
	
	std::string ccbiName = bossJson["type"].asString();
	std::string ccbiname2 = ccbiName;
	
	if(bossShape == "circle")
	{
		if(ccbiName.length()<3) {
			ccbiname2="boss_snake";
		}
		////////////////////////////////////////////
		
//		m_directionAngleDegree = m_well512.GetValue(0, 360);
		
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
		
		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		//CCNode* p = reader->readNodeGraphFromFile(("boss_" + ccbiname2 + ".ccbi").c_str(), this);
		CCNode* p = reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+ccbiname2+".ccbi").c_str(), this);
//		auto ret = KS::loadCCBIForFullPath<CircleBossCCB*>(this, (mySIL->getDocumentPath()+ccbiname2+".ccbi"));

		m_circleSprite = dynamic_cast<CircleBossCCB*>(p);//ret.first;
		m_circleAnimation = reader->getAnimationManager();// ret.second;
		m_circleAnimation->runAnimationsForSequenceNamed("Default Timeline");
//		m_circleAnimation->setDelegate(this);
		reader->release();
		
		if(m_circleSprite != NULL)
		{
			boss_node->addChild(m_circleSprite);
			m_circleSprite->setScale(0.7f);
			m_circleSprite->setPosition(boss_position);
		}
	}
	else if(bossShape == "snake")
	{
		if(ccbiName.length()<3) {
			ccbiname2="apple2";
		}
		m_snakeNode = KSSnakeBase::create(ccbiname2, false);
		boss_node->addChild(m_snakeNode, 1000);
		m_snakeNode->setScale(0.7f);
		m_snakeNode->setPosition(boss_position);
		
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
	
	start_ment = "";
	clear_ment = "";
	fail_ment = "";
	
	// 몬스터별 로 가 아닌 스테이지별 로 대사가 바뀔일은 없다고 HS가 자신있게 말했음. 낭비라 함. 그래서 굳이 루프 돌림
	int monster_count = NSDS_GI(kSDS_GI_monsterCount_i);
	bool is_found = false;
	for(int i=0;!is_found && i<monster_count;i++)
	{
		if(ccbiname2 == NSDS_GS(kSDS_GI_monsterInfo_int1_name_s, i))
		{
			start_ment = NSDS_GS(kSDS_GI_monsterInfo_int1_script_start_s, i);
			clear_ment = NSDS_GS(kSDS_GI_monsterInfo_int1_script_clear_s, i);
			fail_ment = NSDS_GS(kSDS_GI_monsterInfo_int1_script_fail_s, i);
			is_found = true;
		}
	}
	
	boss_hp = bossJson["hp"].asInt();
	boss_speed = bossJson["speed"]["start"].asDouble();
	boss_agi = bossJson.get("agi", 0).asDouble();
	
//	CCLabelTTF* boss_hp_label = CCLabelTTF::create(CCString::createWithFormat("%d", boss_hp)->getCString(), mySGD->getFont().c_str(), 10);
//	addChild(boss_hp_label);
//	
//	CCLabelTTF* boss_speed_label = CCLabelTTF::create(CCString::createWithFormat("%.1f", boss_speed)->getCString(), mySGD->getFont().c_str(), 10);
//	addChild(boss_speed_label);
//	
//	CCLabelTTF* boss_agi_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", boss_agi)->getCString(), mySGD->getFont().c_str(), 10);
//	addChild(boss_agi_label);
//	
//	if(t_code == kCumberShowWindowSceneCode_puzzle)
//	{
//		boss_hp_label->setPosition(ccp(40,70));
//		boss_speed_label->setPosition(ccp(40,42));
//		boss_agi_label->setPosition(ccp(40,14));
//	}
//	else if(t_code == kCumberShowWindowSceneCode_cardChange)
//	{
//		boss_hp_label->setPosition(ccp(120,6));
//		boss_speed_label->setPosition(ccp(120,-13));
//		boss_agi_label->setPosition(ccp(120,-32));
//	}
	
	
	if(t_code == kCumberShowWindowSceneCode_puzzle && juniorJson.size() > 0)
	{
		int junior_cnt = juniorJson.size();
		int juniorIndex = m_well512.GetValue(0, juniorJson.size() - 1);
		juniorJson = juniorJson[juniorIndex];
		{
			std::string juniorType = juniorJson["type"].asString();
			std::string ccbiName = juniorType;
			std::string ccbiname2 = ccbiName;
			
			if(ccbiname2.substr(0,1)=="1") {
				ccbiname2="bear";
			}
			////////////////////////////////////////////
			
			//		m_directionAngleDegree = m_well512.GetValue(0, 360);
			
			//		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			//		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
			
			//		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			
			
			std::string _ccbiName = (ccbiname2 + ".ccbi").c_str();
			CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(nodeLoader);
			CCNode* p = reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+_ccbiName).c_str(), this);
			
			m_juniorSprite = dynamic_cast<CCSprite*>(p);
			m_juniorAnimation = reader->getAnimationManager();
//			m_juniorAnimation->setDelegate(this);
			reader->release();
			
			if(m_juniorSprite != NULL)
			{
				this->addChild(m_juniorSprite, 1001);
				m_juniorSprite->setScale(0.7f);
				m_juniorSprite->setPosition(ccp(-15,-45));
				if(junior_cnt > 1)
				{
					CCLabelTTF* junior_count_label = CCLabelTTF::create(CCString::createWithFormat("x%d", junior_cnt)->getCString(), mySGD->getFont().c_str(), 20);
					junior_count_label->setAnchorPoint(ccp(0,0.5));
					junior_count_label->setPosition(ccp(0,-65));
					addChild(junior_count_label, 1002);
				}
			}
		}
		int junior_hp = juniorJson["hp"].asInt();
		double junior_speed = juniorJson["speed"]["start"].asDouble();
		double junior_agi = juniorJson.get("agi", 0).asDouble();
		
		CCLabelTTF* junior_hp_label = CCLabelTTF::create(CCString::createWithFormat("%d", junior_hp)->getCString(), mySGD->getFont().c_str(), 10);
		junior_hp_label->setPosition(ccp(40,-20));
		addChild(junior_hp_label);
		
		CCLabelTTF* junior_speed_label = CCLabelTTF::create(CCString::createWithFormat("%.1f", junior_speed)->getCString(), mySGD->getFont().c_str(), 10);
		junior_speed_label->setPosition(ccp(40,-48));
		addChild(junior_speed_label);
		
		CCLabelTTF* junior_agi_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", junior_agi)->getCString(), mySGD->getFont().c_str(), 10);
		junior_agi_label->setPosition(ccp(40,-76));
		addChild(junior_agi_label);
	}
	
//	std::string juniorType = juniorJson["type"].asString();

	
	return true;
}

void CumberShowWindow::startFloatMoving()
{
	base_position = boss_node->getPosition() + ccp(0,11);
	a_y = 0.2f;
	v_y = 0;
	schedule(schedule_selector(CumberShowWindow::floatMoving));
}

void CumberShowWindow::floatMoving()
{
	v_y += a_y;
	boss_node->setPosition(boss_node->getPosition() + ccp(0,v_y));
	
	if(boss_node->getPositionY() > base_position.y)
		a_y = -0.2f;
	else if(boss_node->getPositionY() < base_position.y)
		a_y = 0.2f;
	else
		a_y = -a_y;
}