#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "Lua_extensions_CCB.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "Lua_web_socket.h"
#endif

#include "GameData.h"
//#include "StartingScene.h"
#include "WorldMapScene.h"

using namespace CocosDenshion;

USING_NS_CC;

#include <functional>
#include <map>
AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

//static char sharedResourceDirectory[100];
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);

bool AppDelegate::applicationDidFinishLaunching()
{

	
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
//	pDirector->setProjection(kCCDirectorProjection2D);
	
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
//	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
//	CCFileUtils::sharedFileUtils()->setResourceDirectory(sharedResourceDirectory);
	
	CCSize screen_size = pEGLView->getFrameSize();
	float base_value = 320.f-480.f*screen_size.height/screen_size.width;
	
	float upper_limit = base_value/1.5f; // height wide
	DataStorageHub::sharedInstance()->upper_limit = upper_limit > 0.f ? upper_limit : 0.f;
	float bottom_base = -base_value/2.f; // bottom up
	DataStorageHub::sharedInstance()->bottom_base = bottom_base > 0.f ? bottom_base : 0.f;
	float ui_top_control = -base_value; // ui top up
	DataStorageHub::sharedInstance()->ui_top_control = ui_top_control;
	
	float ui_height_center_control;
	if(screen_size.width/screen_size.height > 1.5f)
		ui_height_center_control = -base_value/4.f;
	else
		ui_height_center_control = -base_value/2.f;
	DataStorageHub::sharedInstance()->ui_height_center_control = ui_height_center_control;
	
	float ui_jack_center_control;
	if(screen_size.width/screen_size.height > 1.5f)
		ui_jack_center_control = base_value/1.5f;
	else
		ui_jack_center_control = -base_value/4.f;
	DataStorageHub::sharedInstance()->ui_jack_center_control = ui_jack_center_control;
	
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_flow");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_jack");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_jmissile");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_pattern");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_pause");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_skill");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_stage");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_ui");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_sound/sound_bgm");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_sound/sound_effect");
	CCFileUtils::sharedFileUtils()->addSearchPath("fonts");
	
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//	CCSize frameSize = pEGLView->getFrameSize();
//	
//	float basicRate = 1.5; // iphone, ipod lower than version 5
//	//	float upperThan5 = 1.775;
//	//	float ipadRate = 1024.f/768.f;
//	float frameRate = frameSize.width/frameSize.height;
//	
//	CCSize calcFrameSize;
//	
//	if(frameRate == basicRate) // iphone, ipod lower than version 5
//	{
//		calcFrameSize = designResolutionSize;
//		DataStorageHub::sharedInstance()->device_rate = 1.f;
//	}
//	else if(frameRate > basicRate) // iphone, ipod upper than version 5 // upperThan5
//	{
//		float screen_rate = frameSize.height/designResolutionSize.height;
//		calcFrameSize = CCSizeMake(frameSize.width/screen_rate, frameSize.height/screen_rate);
//		DataStorageHub::sharedInstance()->device_rate = 1.f;
//		DataStorageHub::sharedInstance()->device_margine = CCSizeMake(fabsf(frameSize.width-(960.f*DataStorageHub::sharedInstance()->device_rate))/2.f, 0);
//	}
//	else if(frameRate < basicRate) // ipad // ipadRate
//	{
//		calcFrameSize = CCSizeMake(1024, 768);
//		DataStorageHub::sharedInstance()->device_rate = frameSize.width/960.f;
//		DataStorageHub::sharedInstance()->device_margine = CCSizeMake(0, fabsf(frameSize.height-(640.f*DataStorageHub::sharedInstance()->device_rate))/2.f);
//		if(frameSize.height == calcFrameSize.height)
//		{
//			DataStorageHub::sharedInstance()->setIsNotRetinaIpad(true);
//		}
//	}
//	else
//	{
//		calcFrameSize = designResolutionSize;
//		DataStorageHub::sharedInstance()->device_rate = 1.f;
//	}
//	
//	CCSize imgSize = CCSizeMake(960, 640);
//	
//	if(basicRate < frameRate) // height long, calc : width
//		pDirector->setContentScaleFactor(imgSize.height/calcFrameSize.height);
//	else if(basicRate > frameRate) // width long, calc : height
//		pDirector->setContentScaleFactor(2.0); // only ipad
//	else
//		pDirector->setContentScaleFactor(imgSize.height/calcFrameSize.height);
//	
//	
//	
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	pDirector->setContentScaleFactor(2.0);
//	
//	CCSize frameSize = pEGLView->getFrameSize();
//	
//	float basicRate = 1.5; // iphone, ipod lower than version 5
//	float frameRate = frameSize.width/frameSize.height;
//	
//	CCSize imgSize = CCSizeMake(960, 640);
//	
//	DataStorageHub* myDSH = DataStorageHub::sharedInstance();
//	if(basicRate < frameRate) // height long, calc : width
//	{
//		myDSH->device_rate = frameSize.height/imgSize.height;
//		myDSH->device_margine = CCSizeMake(fabsf(frameSize.width-(imgSize.width*myDSH->device_rate))/2.f, 0);
//	}
//	else if(basicRate > frameRate) // width long, calc : height
//	{
//		myDSH->device_rate = frameSize.width/imgSize.width;
//		myDSH->device_margine = CCSizeMake(0, fabsf(frameSize.height-(imgSize.height*myDSH->device_rate))/2.f);
//	}
//	else
//	{
//		myDSH->device_rate = frameSize.width/imgSize.width;
//		myDSH->device_margine = CCSizeMake(0, 0);
//	}
//	
//#endif
	
	pDirector->setContentScaleFactor(2.0);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // register lua engine
//    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
//    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
//
//    CCLuaStack *pStack = pEngine->getLuaStack();
//    lua_State *tolua_s = pStack->getLuaState();
//    tolua_extensions_ccb_open(tolua_s);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    pStack = pEngine->getLuaStack();
//    tolua_s = pStack->getLuaState();
//    tolua_web_socket_open(tolua_s);
//#endif
//    
//    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("hello.lua");
//    pEngine->executeScriptFile(path.c_str());
	
	
	
//	CCScene *pScene = StartingScene::scene();
	CCScene* pScene = WorldMapScene::scene();
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
