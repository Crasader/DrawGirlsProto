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
#include "TitleRenewal.h"
#include "TextureReloader.h"

using namespace CocosDenshion;

USING_NS_CC;

#include <functional>
#include <map>
#include "KSUtil.h"
#include "Well512.h"
#include "Dodge.h"
#include "RandomDistribution.h"
#include "CountingGame.h"


#include "SlidingPuzzle.h"
#include "CardMatching.h"
#include "KSProtect.h"
#include "ks19937.h"


#include "ASPopupView.h"
#include "GraphDog.h"
#include "KSUtil.h"
#include "GaBaBo.h"
#include "StarGoldData.h"
#include "FormSetter.h"
#include "MyLocalization.h"
#include <chrono>
//#include <boost/graph/graphviz.hpp>
/*
 메테오 : AP_Missile16
 AP_Missile15 불지르기
 AP_Missile34 인비지블
 
 AP_Missile12 위성빔
 AP_Missile6  레이저 스캔
 AP_Missile22 방사능.
 
 */
AppDelegate::AppDelegate()
{
	CCLOG("!!!");

//	std::chrono::time_point<std::chrono::system_clock> recent;
//    recent = std::chrono::system_clock::now();
//	std::time_t recent_time = std::chrono::system_clock::to_time_t(recent);
//	CCLOG("start : %d", int(recent_time));
//	KSProtectVar<float> testVar(15.f);
//	KSProtectVar<float> testVar2(20.f);
//	testVar += 3.f;
//	testVar -= 5.f;
//	
//	CCLOG("%f", testVar.getV()); // 값 얻기.
//	CCLOG("%f", testVar + testVar2); // 보호된 변수끼리 연산가능.
//	CCLOG("%d", testVar < testVar2); // 비교가능.
//	CCLOG("%d", testVar < 3.f); // 상수값 비교 가능.
//	
//	KSProtectStr testStr("testtest");
//	CCLOG("%s", testStr.getV().c_str());
	
	
	//	std::random_device rd;
	//	std::mt19937 rEngine(rd());
	//	uniform_int_distribution<long long> dist(1000000000000000000L, 2000000000000000000L);
	//
	//	for(int i=0; i<20; i++)
	//	{
	//		long long number = dist(rEngine);
	//		CCLOG("%lld", number);
	//		CCLOG("%s", KS::longLongToStr(number).c_str());
	//		CCLOG("\t%lld", KS::strToLongLong(KS::longLongToStr(number)));
	//		CCLOG("%s", KS::longLongToStr(number).c_str());
	//		CCLOG("\t%lld", KS::strToLongLong(KS::longLongToStr(number)));
	//		CCLOG("");
	//	}
	//	CCLOG("f");
}

AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::end();
}

//static char sharedResourceDirectory[100];
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);

struct AA
{
	int a;
	int b;
};
bool AppDelegate::applicationDidFinishLaunching()
{
	CCLog("finish l");
//	string plainText = "AAABBVCBCVBVV";
//	string encText = CipherUtils::encryptAESBASE64(encryptChars("nonevoidmodebase").c_str(), plainText.c_str());
//	CCLog("asdasdsa %s", encText.c_str());
//	CCLog("encText length : %d", encText.size());
//	encText = "e9EeE1UV7v+UwZS0z0wzlg==";
//	string re = CipherUtils::decryptAESBASE64(encryptChars("nonevoidmodebase").c_str(),
//																						encText.c_str());
//	CCLog("re %s", re.c_str());
//	return false;
	
//	CCLOG("TEST RESULT : %s", re.c_str());
//	string resultStr = "es1hmm2BeF8wfgnRfKN7D3zgX78TbuzEj4GjeKaQG6tqqxmeHmCbagw6OfUjLnROF4XhcTl8gertaZppPtb8FoNFk5dBCot7D81iEnWfiEd1OsX2mqM8aRsHsbR+DMl8YRkz+LgWpFS+rfyMSet2ntVF7Mu2UOb2DIm6wvJoF5fsjf4y3y3EOlRkyUziccR/6P1jAaLij8bYyFWu45tpDuih2xQKb+Zh100Ep7eBJpBFjAWFn5JpBFjAWFn5iCa9t5dL/swLeVJ6oTyVqKvW/wXfau+krKvdqskiJ23Dk2SqndpUFBD7RNKDq9MbZKH7MRQthfxmiG";
//	
//	string rawResultStr = CipherUtils::decryptAESBASE64(encryptChars("nonevoidmodebase").c_str(), resultStr.c_str());
//	CCLOG("%s", rawResultStr.c_str());
//	return false;
//	std::chrono::time_point<std::chrono::system_clock> recent;
//    recent = std::chrono::system_clock::now();
//	std::time_t recent_time = std::chrono::system_clock::to_time_t(recent);
//	CCLOG("finish start : %d", int(recent_time));
//	
//	std::chrono::time_point<std::chrono::system_clock> start, end;
//    start = std::chrono::system_clock::now();
	
	if(myDSH->getIntegerForKey(kDSH_Key_clientVersion) < mySGD->client_version)
	{
		SaveData::sharedObject()->resetAllData();
		myDSH->removeCache();
		mySDS->removeCache();
		myDSH->setIntegerForKey(kDSH_Key_clientVersion, mySGD->client_version);
	}
	
	
	graphdog = GraphDog::get();
	ks19937::seed(std::random_device()());
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setProjection(kCCDirectorProjection2D); // 바스트 모핑 측면을 보려면 주석 처리 필요함.
	
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	//	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionFixedWidth);// kResolutionNoBorder);
    //	CCFileUtils::sharedFileUtils()->setResourceDirectory(sharedResourceDirectory);
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
	
	CCSize screen_size = pEGLView->getFrameSize();
	myDSH->ui_top = 480.f*screen_size.height/screen_size.width;
	myDSH->ui_bottom = 0;
	myDSH->ui_center_y = myDSH->ui_top/2.f;
	myDSH->puzzle_ui_top = 320.f;
	
	float screen_rate = screen_size.width/screen_size.height;
	if(screen_rate > 1.5f)
	{
		myDSH->screen_convert_rate = (designResolutionSize.width/designResolutionSize.height)/screen_rate;
		myDSH->ui_zero_point = CCPointZero;
		
		float t_value = screen_size.height/designResolutionSize.height;
		myDSH->mult_point.x = screen_size.width/t_value/designResolutionSize.width;
		myDSH->add_point.x = -(screen_size.width-designResolutionSize.width*t_value)/2.f/t_value;
		
		myDSH->mult_point.y = screen_size.width/designResolutionSize.width/t_value;
		myDSH->add_point.y = 0.f;
	}
	else if(screen_rate < 1.5f)
	{
		myDSH->screen_convert_rate = 1.f;
		myDSH->ui_zero_point = ccp(0,(screen_size.height*(designResolutionSize.width/screen_size.width) - designResolutionSize.height)/2.f);
		myDSH->puzzle_ui_top = myDSH->ui_top;
		
		myDSH->mult_point.x = 1.f;
		myDSH->add_point.x = 0.f;
		
		myDSH->mult_point.y = 1.f;
		myDSH->add_point.y = -(screen_size.height*(designResolutionSize.width/screen_size.width) - designResolutionSize.height)/2.f;
	}
	else
	{
		myDSH->screen_convert_rate = 1.f;
		myDSH->ui_zero_point = CCPointZero;
		
		myDSH->mult_point.x = 1.f;
		myDSH->add_point.x = 0.f;
		
		myDSH->mult_point.y = 1.f;
		myDSH->add_point.y = 0.f;
	}
	
	CCPoint ui_touch_convert = ccp(0,screen_size.height*(designResolutionSize.width/screen_size.width) - designResolutionSize.height);
	myDSH->ui_touch_convert = ui_touch_convert;
    
	
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb/ccbResources");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ksoo");
	//	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb_test");
	//	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb_test/resources-iphonehd");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb/resources-iphonehd");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_flow");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_tutorial");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_jack");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_jmissile");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_pattern");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_pause");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_skill");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_stage");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_ui");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_sound/sound_bgm");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_sound/sound_effect");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_sound/sound_girls");
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
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	pDirector->setDisplayStats(true);
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	pDirector->setDisplayStats(false);

#endif
	
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
	
	
	GraphDog::get()->setDuplicateLoginFunc([](){
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert1),[](){
			mySGD->resetLabels();
			AudioEngine::sharedInstance()->stopAllEffects();
			CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
	});
	
	
	GraphDog::get()->setCmdNoErrorFunc([](){
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert2),[](){
			mySGD->resetLabels();
			AudioEngine::sharedInstance()->stopAllEffects();
			CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
	});
	
	GraphDog::get()->setLongTimeErrorFunc([](){
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert3),[](){
			mySGD->resetLabels();
			AudioEngine::sharedInstance()->stopAllEffects();
			CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
	});
	
	GraphDog::get()->setCommandRetryFunc([](std::vector<CommandParam> vcp){
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
			GraphDog::get()->command(vcp);
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
	});
	
	myHSP->getAdXConnectEventInstance("Launch", "", "");
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	FormSetter::get()->setEnabledRemocon(true);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	FormSetter::get()->setEnabledRemocon(false);
#endif
	CCScene* pScene = TitleRenewalScene::scene();
//	CCScene* pScene = TestScene::scene();

	//	pEGLView->setDesignResolutionSize(480, 480, kResolutionFixedWidth);// kResolutionNoBorder);
	pDirector->runWithScene(pScene);
	
//	end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_seconds = end-start;
//	CCLOG("AppDelegate Finish time : %f", elapsed_seconds.count());
//	
//    recent = std::chrono::system_clock::now();
//	recent_time = std::chrono::system_clock::to_time_t(recent);
//	CCLOG("finish end : %d", int(recent_time));
	
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	lastTime = GraphDog::get()->getTime();
	if(!mySGD->is_paused && mySGD->is_on_maingame)
		myGD->communication("Main_showPause");
	
	CCDirector::sharedDirector()->stopAnimation();
	
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->startAnimation();
	
	myTR->reloadTexture();
	
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	
	long long int nowTime = GraphDog::get()->getTime();
	int time = mySGD->getSessionTime();
	if(!time)time=180;
	
	
	if(nowTime-lastTime>time){
		
		CCDirector::sharedDirector()->getRunningScene()->addChild(KSTimer::create(0.1f, [=]()
																				  {
																					  mySGD->resetLabels();
																					  AudioEngine::sharedInstance()->stopAllEffects();
																					  CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																				  }));
	}
	
	
	
}
