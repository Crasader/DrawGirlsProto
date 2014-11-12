/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
//#include "StarGoldData.h"

#import "RootViewController.h"
#import "hspConnector.h"
#import "FiveRocks.h"
#import "HSPCore.h"
#import <IgaworksAD/IgaworksAD.h>
#include "DataStorageHub.h"
#import "HSPSDKPackageInfo.h"
#import "HSPCore.h"
#import "HSPUiLauncher.h"
#ifdef HSPSDK_SUPPORTS_UI
#import "HSPUiReference.h"
#endif // HSPSDK_SUPPORTS_UI
#import "HSPMyProfile.h"
#import "HSPMessage.h"

//#define FIVEROCKS_APP_ID @"YOUR_APP_ID"
//#define FIVEROCKS_APP_KEY @"YOUR_APP_KEY"
AdXTracking *tracker;
@implementation AppController

//@synthesize window;
@synthesize viewController;
@synthesize window = _window;
#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;
- (void)log:(NSString*)log
{
	NSLog(@"%@",log);
	
//	self.currentLog.text = [self.currentLog.text stringByAppendingString:[log stringByAppendingString:@"\n"]];
//	
//	if ( self.currentLog.contentSize.height > self.currentLog.frame.size.height )
//		[self.currentLog setContentOffset:CGPointMake(0, self.currentLog.contentSize.height - self.currentLog.frame.size.height ) animated:YES];
}
- (void)onBeforeSessionExpire
{
	/* A method to be called when logging out or initializing account */
//	[self.viewController log:@"onBeforeSessionExpire – session will be expired."];
//	
//	/* Preparing for user change since session will be soon expired. Personal information used in game should be stored. */
//	[self.viewController log:@"save user data because session will be expired."];
}

-(void) didReceivedMessage:(NSNotification*)notification
{
//	[self.viewController log:[NSString stringWithFormat:@"Received new message. %@", notification.object]];
}

-(void) didReceivedPacket:(NSNotification*)notification
{
//	[self.viewController log:[NSString stringWithFormat:@"Received binary data. %@", notification.object]];
}

-(void) didChangedProfile:(NSNotification*)notification
{
//	[self.viewController log:[NSString stringWithFormat:@"User information has chaned. %@", notification.object]];
}

- (void)onSessionExpireComplete:(NSNotification*)notification
{
//	/* It will be logged out or account will be initialized. */
//	[self.viewController log:@"Session has expired."];
//	
//	/* User information that has been used is initialized and the page is moved to first screen. */
//	[self.viewController log:@"User information has initialized"];
}

-(void) onSessionExpiredByExternalFactors:(NSNotification*)notification
{
//	/* Authentication session has expired caused from outside source. */
//	[self.viewController log:@"Session has expired by outside source."];
//	
//	/* Storing an existing user information and initializing game. */
//	[self.viewController log:@"User information has initialized"];
}
-(void) onPushMessageReceive:(NSNotification*)notification
{
	NSDictionary* dic = [notification userInfo];
 
	NSLog(@"받은 노티피케이션 정보 : %@", dic);
}
- (void)registerHSPEventObserver
{
	/* Registering the method to be called in HSPCore before authentication session expiration. */
	[[HSPCore sharedHSPCore] addBeforeLogoutListener:^{
		[self onBeforeSessionExpire];
	}];
	
	[[HSPCore sharedHSPCore] addBeforeResetAccountListener:^{
		[self onBeforeSessionExpire];
	}];

	
	/* Registering Notification to obtain authentication session expired event of HSPCore.*/
	[[NSNotificationCenter defaultCenter] addObserver:self
																					selector:@selector(onPushMessageReceive:)
																							name:HSPPushNotificationName
																						object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(onSessionExpireComplete:)
																							 name:HSPDidLogoutNotificationName
																						 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(onSessionExpireComplete:)
																							 name:HSPDidResetAccountNotificationName
																						 object:nil];
	
	/* Registering Notification to obtain authentication session expired event caused from outside source. */
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(onSessionExpiredByExternalFactors:)
																							 name:HSPSessionExpiredNotificationName
																						 object:nil];
	// Handling event occurred in HSP
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(didReceivedMessage:)
																							 name:HSPDidReceiveMessageNotificationName
																						 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(didReceivedPacket:)
																							 name:HSPDidReceivePacketNotificationName
																						 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(didChangedProfile:)
																							 name:HSPMyProfileAllPropertiesDidChangeNotificationName
																						 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(didChangedProfile:)
																							 name:HSPMyProfileNicknameDidChangeNotificationName
																						 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(didChangedProfile:)
																							 name:HSPMyProfileTodayWordsDidChangeNotificationName
																						 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(didChangedProfile:)
																							 name:HSPMyProfileImageDidChangeNotificationName
																						 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
																					 selector:@selector(didChangedProfile:)
																							 name:HSPMyProfilePhoneNoDidChangeNotificationName
																						 object:nil];
}

- (void)unregisterHSPEventObserver
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}
void uncaughtExceptionHandler(NSException *exception) {
	NSLog(@"CRASH: %@", exception);
	NSLog(@"Stack Trace: %@", [exception callStackSymbols]);
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	NSSetUncaughtExceptionHandler(&uncaughtExceptionHandler);
	// Override point for customization after application launch.

	[IgaworksAD igaworksADWithAppKey:@"58229468"
												andHashKey:@"03c0b0579dd4470b"
			andIsUseIgaworksRewardServer:NO];
	[self reportAppOpen];
	
	
	hspConnector::get()->setupHSPonIOS(10331, "SKSUMRAN", "1.0.0", launchOptions);
	
	
	[FiveRocks startSession:@"53c4918600821d86a400000e" withAppKey:@"bSYyKMPM-oSSEMEvOPMW"];
	[FiveRocks setDebugEnabled:true];
	
	
	// Add the view controller's view to the window and display.
	window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
	EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
																	 pixelFormat: kEAGLColorFormatRGBA8
																	 depthFormat: GL_DEPTH24_STENCIL8_OES//GL_DEPTH_COMPONENT16
														preserveBackbuffer: NO
																		sharegroup: nil
																 multiSampling: NO
															 numberOfSamples: 0 ];
	
	[__glView setMultipleTouchEnabled:YES];
	// Use RootViewController manage EAGLView
	viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
	viewController.wantsFullScreenLayout = YES;
	viewController.view = __glView;
	
	// Set RootViewController to window
	if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
	{
		// warning: addSubView doesn't work on iOS6
		[window addSubview: viewController.view];
	}
	else
	{
		// use this method on ios6
		[window setRootViewController:viewController];
	}
	
	[window makeKeyAndVisible];
	
	[[UIApplication sharedApplication] setStatusBarHidden: YES];
	[UIApplication sharedApplication].idleTimerDisabled = YES; // screen out disable
	
	////////////////////////////////////////////
	/* HSP로부터 발생하는 이벤트를 처리하기 위한 핸들러를 등록한다. "HSP에서 발생하는 이벤트" 참조 */
	[self registerHSPEventObserver];
	/*************************************************************************/
 
	/*************************************************************************/
	/* HSPUiLauncher에 게임 윈도우와 게임 뷰를 등록한다. */
	[HSPUiLauncher sharedLauncher].gameWindow = self.window;
	[HSPUiLauncher sharedLauncher].gameView = self.viewController.view;
	/////////////////////////////////////////////
	cocos2d::CCApplication::sharedApplication()->run();
	return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
	/*
	 Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	 Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
	 */
	[FiveRocks endSession];
	cocos2d::CCDirector::sharedDirector()->pause();
	cocos2d::CCDirector::sharedDirector()->stopAnimation();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	/*
	 Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
	 */
	[FiveRocks startSession:@"53c4918600821d86a400000e" withAppKey:@"bSYyKMPM-oSSEMEvOPMW"];
	

	cocos2d::CCDirector::sharedDirector()->startAnimation();
	//	if(!StarGoldData::sharedInstance()->is_paused)
	cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	/*
	 Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	 If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
	 */
	
	cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	/*
	 Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
	 */
	
	[self reportAppOpen];
	if ( [HSPCore sharedHSPCore].state != HSP_STATE_INIT )
	{
		
		HSPOAuthProvider lType = (HSPOAuthProvider)myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSP_OAUTHPROVIDER_GAMECENTER);
		CCLOG("AUTO LOGIN TYPE == %d", lType);
//		[[HSPCore sharedHSPCore] loginWithManualLogin:NO completionHandler:^(BOOL playable, HSPError* error) {
//			// 로그인 응답 처리
//		}];
		[[HSPCore sharedHSPCore] loginWithOAuthProvider:lType completionHandler:^(BOOL playable, HSPError *error) {
			//
		}];
	}
		cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
	
	
}

- (void)applicationWillTerminate:(UIApplication *)application {
	/*
	 Called when the application is about to terminate.
	 See also applicationDidEnterBackground:.
	 */
}


- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation{
	
//	NSLog(@"url %@ sourceApplication %@",url, sourceApplication);
//	
//	NSString *str = [NSString stringWithFormat:@"url:%@\n source:%@",[url absoluteString],sourceApplication];
//	
//	//	UIAlertView *alert =
//	//
//	//	[[[UIAlertView alloc]initWithTitle:@"call From A" message:str delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil]autorelease];
//	//
//	//	[alert show];
//	
//	return YES;
	
	NSDictionary *dict = [self parseQueryString:[url query]];
	NSString *ADXID = [dict objectForKey:@"ADXID"];
	if (ADXID) {
		[tracker sendEvent:@"DeepLinkLaunch" withData:ADXID];
	}
	
	
	return [tracker handleOpenURL:url];
	
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
	/*
	 Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
	 */
	cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc {
	[self unregisterHSPEventObserver];
	
	[_window release];
	[viewController release];
	[super dealloc];
}

- (void)reportAppOpen
{
	tracker = [[AdXTracking alloc] init];
	[tracker setURLScheme:@"ADX41051"];
	[tracker setClientId:@"nhntrt809531jdr"];
	[tracker setAppleId:@"924830126"];
	[tracker setBundleID:@"com.nhnent.SKSUMRAN"];
	[tracker reportAppOpen];
}

- (NSDictionary *)parseQueryString:(NSString *)query {
	NSMutableDictionary *dict = [[[NSMutableDictionary alloc] initWithCapacity:16] autorelease];
	NSArray *pairs = [query componentsSeparatedByString:@"&"];
	
	for (NSString *pair in pairs) {
		NSArray *elements = [pair componentsSeparatedByString:@"="];
		NSString *key = [[elements objectAtIndex:0] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
		NSString *val = [[elements objectAtIndex:1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
		
		[dict setObject:val forKey:key];
	}
	return dict;
}
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
	NSDictionary *dict = [self parseQueryString:[url query]];
	NSString *ADXID = [dict objectForKey:@"ADXID"];
	if (ADXID) {
		[tracker sendEvent:@"DeepLinkLaunch" withData:ADXID];
	}
	
	return [tracker handleOpenURL:url];
}

@end

