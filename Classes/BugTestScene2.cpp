//
//  BugTestScene2.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 8..
//
//

#include "BugTestScene2.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "CommonButton.h"
#include "MainFlowScene.h"

CCScene* BugTestScene2::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	BugTestScene2 *layer = BugTestScene2::create();
//	layer->setAnchorPoint(ccp(0.5,0));
//	layer->setScale(myDSH->screen_convert_rate);
//	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

bool BugTestScene2::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	CommonButton* return_button = CommonButton::createCloseButton();
	return_button->setPosition(ccp(450,280));
	addChild(return_button);
	return_button->setFunction([=](CCObject* sender)
							   {
								   CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
							   });
	
	CCLabelTTF* character_label = CCLabelTTF::create("character", mySGD->getFont().c_str(), 12);
	character_label->setPosition(ccp(30,230));
	addChild(character_label);
	
	CCLabelTTF* load_ccbi_label = CCLabelTTF::create("loadCCBIForFullPath", mySGD->getFont().c_str(), 12);
	load_ccbi_label->setPosition(ccp(70,280));
	addChild(load_ccbi_label);
	
	CCLabelTTF* load_file_label = CCLabelTTF::create("loadFileFullPath", mySGD->getFont().c_str(), 12);
	load_file_label->setPosition(ccp(110,280));
	addChild(load_file_label);
	
	CCLabelTTF* load_data_label = CCLabelTTF::create("loadDataFullPath", mySGD->getFont().c_str(), 12);
	load_data_label->setPosition(ccp(110,280));
	addChild(load_data_label);
	
	
	
	auto t_pair = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterNo.getV()) + ".ccbi");
	
	CCSprite* load_ccbi_char = t_pair.first;
	load_ccbi_char->setPosition(ccp(70,230));
	addChild(load_ccbi_char);
	
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* t_reader = new CCBReader(nodeLoader);
	CCSprite* load_file_char = (CCSprite*)t_reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterNo.getV()) + ".ccbi").c_str(), this);
	load_file_char->setPosition(ccp(110,230));
	addChild(load_file_char);
	
	
	unsigned long size = 0;
    unsigned char * pBytes = CCFileUtils::sharedFileUtils()->getFileData((mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, mySGD->getSelectedCharacterHistory().characterNo.getV()) + ".ccbi").c_str(), "rb", &size);
    CCData *data = new CCData(pBytes, size);
    CC_SAFE_DELETE_ARRAY(pBytes);
	
	CCNodeLoaderLibrary* nodeLoader2 = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* t_reader2 = new CCBReader(nodeLoader2);
	
    CCSprite* load_data_char = (CCSprite*)t_reader2->readNodeGraphFromDataForFullPath(data, this, CCDirector::sharedDirector()->getWinSize());
    load_data_char->setPosition(ccp(110,280));
	addChild(load_data_char);
    data->release();
	
	
	
	
	return true;
}