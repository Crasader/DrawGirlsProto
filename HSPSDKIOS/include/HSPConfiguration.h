//
//  HSPConfiguration.h
//  HSPCore
//
//  Created by  on 11. 10. 19..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPTypes.h"

/**
 * @brief A key of Launching Zone value stored in the dictionary of HSPConfiguration.
 */
#define HSP_LAUNCHING_ZONE					(@"HSP_LAUNCHING_ZONE")

/**
 * @brief A key of addressLaunching value stored in the dictionary of HSPConfiguration.
 */
#define HSP_ADDRESS_LAUNCHING				(@"HSP_ADDRESS_LAUNCHING")

/**
 * @brief A key of isRealZone value stored in the dictionary of HSPConfiguration.
 */
#define HSP_IS_REALZONE						(@"HSP_IS_REALZONE")

/**
 * @brief A key of lncRefreshTimeInterval value stored in the dictionary of HSPConfiguration.
 */
#define HSP_LAUNCHING_REFRESH_TIMEINTERVAL	(@"HSP_LAUNCHING_REFRESH_TIMEINTERVAL")

/**
 * @brief A key of heartbeatInterval value stored in the dictionary of HSPConfiguration.
 */
#define HSP_HEARTBEAT_TIMEINTERVAL			(@"HSP_HEARTBEAT_TIMEINTERVAL")

/**
 * @brief A key of debugLevel value stored in the dictionary of HSPConfiguration.
 */
#define HSP_DEBUG_LEVEL						(@"HSP_DEBUG_LEVEL")

/**
 * @brief A key of enableAssertion value stored in the dictionary of HSPConfiguration.
 */
#define HSP_ENABLE_ASSERTION				(@"HSP_ENABLE_ASSERTION")

/**
 * @brief A key of locale value stored in the dictionary of HSPConfiguration.
 */
#define HSP_LOCALE							(@"HSP_LOCALE")

/**
 * @brief A key of service domain value stored in the dictionary of HSPConfiguration.
 */
#define HSP_SERVICE_DOMAIN					(@"HSP_SERVICE_DOMAIN")

/**
 * @brief A key of marketCode value stored in the dictionary of HSPConfiguration.
 */
#define HSP_MARKET							(@"HSP_MARKET")

/**
 * @brief A key of saveImageFormat value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_IMAGE_FORMAT					(@"HSP_IMAGE_FORMAT")

/**
 * @brief A key of timeout TCP value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_TIMEOUT_TCP						(@"HSP_TIMEOUT_TCP")

/**
 * @brief A key of timeout HTTP value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_TIMEOUT_HTTP					(@"HSP_TIMEOUT_HTTP")

/**
 * @brief A key of isShowUiWelcomeMessage value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_SHOW_UI_WELCOMEMESSAGE			(@"HSP_SHOW_UI_WELCOMEMESSAGE")

/**
 * @brief A key of isShowUiVersionCheck value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_SHOW_UI_VERSIONCHECK			(@"HSP_SHOW_UI_VERSIONCHECK")

/**
 * @brief A key of isShowUiMaintenance value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_SHOW_UI_MAINTENANCE				(@"HSP_SHOW_UI_MAINTENANCE")

/**
 * @brief A key of isShowUiRecommendGame value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_SHOW_UI_RECOMMENDGAME			(@"HSP_SHOW_UI_RECOMMENDGAME")

/**
 * @brief A key of isUsePush value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_USE_PUSH						(@"HSP_USE_PUSH")

/**
 * @brief A key, stored in the dictionary of HSPConfiguration, representing whether to display HSP as popover on iPad.
 */
#define HSP_STYLE_POPOVER					(@"HSP_STYLE_POPOVER")

/**
 * @brief A key of isResendFailedData value stored in the dictionary of HSPConfiguration.
 */
#define HSP_RESEND_FAILED_DATA				(@"HSP_RESEND_FAILED_DATA")

/**
 * @brief A key of isUseAutoSuspend value stored in the dictionary of HSPConfiguration.
 */
#define HSP_USE_AUTOSUSPEND					(@"HSP_USE_AUTOSUSPEND")

/**
 * @brief A key of isEnforedGuestLogin value stored in the dictionary of HSPConfiguration.
 */
#define HSP_ENFORCED_GUEST_LOGIN			(@"HSP_ENFORCED_GUEST_LOGIN")

/**
 * @brief A key of isShowUiPunishment value stored in the dictionary of HSPConfiguration. 
 */
#define HSP_SHOW_UI_PUNISHMENT				(@"HSP_SHOW_UI_PUNISHMENT")

/**
 * @brief A key of enableLoginUIAnimation value stored in the dictionary of HSPConfiguration.
 */
#define HSP_ENABLE_LOGIN_UI_ANIMATION		(@"HSP_ENABLE_LOGIN_UI_ANIMATION")

/**
 * @brief A key of enableLineEmailAccount value stored in the dictionary of HSPConfiguration.
 */
#define HSP_ENABLE_LINE_EMAIL_LOGIN			(@"HSP_ENABLE_LINE_EMAIL_LOGIN")

/**
 * @brief A key of nelo server address value stored in the dictionary of HSPConfiguration.
 */
#define HSP_NELO_SERVER_ADDRESS				(@"HSP_NELO_SERVER_ADDRESS")

/**
 * @brief A key of nelo debug level value stored in the dictionary of HSPConfiguration.
 */
#define HSP_NELO_DEBUG_LEVEL				(@"HSP_NELO_DEBUG_LEVEL")

/**
 * @brief A key of isUseNelo value stored in the dictionary of HSPConfiguration.
 */
#define HSP_USE_NELO						(@"HSP_USE_NELO")

/**
 * @brief A key of Login idp value stored in the dictionary of HSPConfiguration.
 */
#define HSP_LOGIN_IDP                       (@"HSP_LOGIN_IDP")

/**
 * @brief A key of isUseXcode5Later value stored in the dictionary of HSPConfiguration.
 */
#define HSP_USE_XCODE5_LATER				(@"HSP_USE_XCODE5_LATER")

/**
 * @brief A key of isUseJoga value stored in the dictionary of HSPConfiguration.
 */
#define HSP_USE_JOGA						(@"HSP_USE_JOGA")

/**
 * @brief A key of Login retry times value stored in the dictionary of HSPConfiguration.
 */
#define HSP_LOGIN_RETRY_TIMES				(@"HSP_LOGIN_RETRY_TIMES")

/**
 * @brief A key for Facebook URL scheme suffix used while in Facebook App-to-App authentication.
 * The suffix can be registered on Facebook developer's settings page for the app
 */
#define HSP_FACEBOOK_URL_SCHEME_SUFFIX		(@"HSP_FACEBOOK_URL_SCHEME_SUFFIX")

/**
 * Information that can be returned by the debug method.
 */
#define HSP_DEBUG_LOW						(@"LOW")
#define HSP_DEBUG_MIDDLE					(@"MIDDLE")
#define HSP_DEBUG_HIGH						(@"HIGH")

/**
 * @brief Information that can be returned by the locale method.
 */
#define HSP_LOCALE_JP						(@"JP")
#define HSP_LOCALE_KR						(@"KR")
#define HSP_LOCALE_GLOBAL					(@"GLOBAL")

/**
 * @brief Information that can be returned by the marketCode method.
 */
#define HSP_MARKET_KA						(@"KA")
#define HSP_MARKET_JA						(@"JA")

/**
 * @brief Interface that configures HSPSDK.
 */
@interface HSPConfiguration : NSObject
{
	NSMutableDictionary*   _configuration;
	NSMutableDictionary*   _launchingZoneConfiguration;
}

/**
 * @brief Dictionary that contains HSP configuration information loaded from the configuration file.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)   NSDictionary*   configuration;

/**
 * @brief Dictionary that contains the launching zone information loaded from the configuration file.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)   NSDictionary*   launchingZoneConfiguration;

/**
 * @brief Returns the singleton instance of HSPConfiguration.
 * @return HSPConfiguration* Singleton instance of HSPConfiguration.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (HSPConfiguration*)sharedConfiguration;

/**
 * @brief Gets the object related to the key, from the information in HSPConfiguration.
 * @param key A key of the object to get.
 * @return id Id of the object related to the key.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (id)objectForKey:(id)key;

/**
 * @brief Stores the object related to the key, in the information in HSPConfiguration. 
 * @param object An object to set
 * @param key A key of an object to set
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setObject:(id)object forKey:(id)key;

/**
 * @brief Returns the zone to launch the HSP service.
 * @return NSString* Launching zone of the HSP service.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)launchingZone;

/**
 * @brief Returns the launching service URL.
 * @return NSString* Launching service URL.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)addressLaunching;

/**
 * @brief Returns the nelo service URL.
 * @return NSString* nelo service URL.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)neloServerAddress;

/**
 * @brief Returns the nelo debug level.
 * @return NSString* nelo debug level. ex) NONE, DEBUG, INFO, WARN, ERROR, FATAL
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)neloDebugLevel;

/**
 * @brief Returns whether the currnt launching zone is for real.
 * @return BOOL Whether the current launching zone is for real.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isRealLaunchingZone;

/**
 * @brief Sets refresh time interval of the launching service when re-logged in (unit: second).
 * Sets the time interval to refresh the information of launching service, and within this time interval the cached information is used. 
 * @return NSTimeInterval Time interval to refresh the launching service.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSTimeInterval)lncRefreshTimeInterval;

/**
 * @brief Returns the time interval of heartbeat exchanged with the HSP server (unit: second).
 * The setting value is used by default. If the heartbeat interval has been set by the launching service, it takes priority over the one set by HSP client.
 * @return NSTimeInterval Heartbeat interval.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSTimeInterval)heartbeatTimeInterval;

/**
 * @brief Returns the log level of HSPSDK.
 * @return NSString* The debug level.
 @"LOW" : Displays the logs using HSP_LOG_ERROR.
 @"MIDDLE" : Displays the logs using HSP_LOG_ERROR and HSP_LOG_DEBUG.
 @"HIGH" : Displays the logs using HSP_LOG_ERROR, HSP_LOG_DEBUG, and HSP_LOG_INFO.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)debugLevel;

/**
 * @brief When an assertion occurs in HSPSDK, returns whether to generate an exception or ignore it.
 * @return BOOL Returns YES to generate an exception, NO to ignore.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)enableAssertion;

/**
 * @brief Returns the version of HSPSDK.
 * @return NSString* HSPSDK version in the form of MajorVersion.MinorVersion.ReleaseSequence.RevisionNumber.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)version;

/**
 * @brief Returns the locale of the game application. 
 * HSP is implemented differently by this locale setting.
 * @return NSString* Locale setting. @"KR"(Korea)/@"JP"(Japan) supported.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)locale;

/**
 * @brief Returns the default market information for the game application.
 * KA(Korean App Store)/JA(Japanese App Store).
 * @return NSString* Market code setting. @"KA"(Korean App Store)/@"JA"(Japanese App Store) supported.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)marketCode;

/**
 * @brief Returns the encoding type of images to be uploaded in HSP (JPEG/PNG).
 * @return NSString* Image format setting. @"JPEG"/@"PNG" supported.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)saveImageFormat;

/**
 * @brief Returns the timeout interval for the requests to the HSP Server (unit: second).
 * @return NSTimeInterval Timeout interval.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSTimeInterval)timeoutTCP;

/**
 * @brief Returns the timeout interval for HTTP request (unit: second).
 * @return NSTimeInterval Timeout interval.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSTimeInterval)timeoutHTTP;

/**
 * @brief Returns the setting whether to show the Bridge view which is displayed when logged in. 
 * @return BOOL (YES: Show the HSP Bridge view, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isShowUiWelcomeMessage;

/**
 * @brief Returns whether HSP supports the UI flow which prompts users to download the new version of the game. 
 * @return BOOL (YES: Use HSP default UI flow, NO: Not use).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isShowUiVersionCheck;

/**
 * @brief Returns whether HSP supports the HSP default UI flow for maintenance or not.
 * @return BOOL (YES: Use HSP default UI flow, NO: Not use).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isShowUiMaintenance;

/**
 * @brief Returns whether to show recommended game page automatically when a user who gets a game recommendation logs in. 
 * @return BOOL (YES: Show the recommended game page automatically, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isShowUiRecommendGame;

/**
 * @brief Returns whether to support push service.
 * @return BOOL (YES: Use push service, NO: Not use).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isUsePush;

/**
 * @brief Returns whether to support nelo service
 * @return BOOL (YES: Use nelo service, NO: Not use).
 * @serviceDomain LINEGAME
 */
- (BOOL)isUseNelo;

/**
 * @brief Returns whether to support popover style on iPad (not supported on iPhone).<br>Default value is YES.
 * @return BOOL (YES: Use popover style, NO: Not use).
 * @serviceDomain 
 */
- (BOOL)isStylePopover;

/**
 * @brief Returns whether HSP automatically re-tries to report missions or ranking scores to the server after it fails. 
 * @return BOOL (YES: Auto re-try, NO: No re-try).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isResendFailedData;

/**
 * @brief Returns whether to suspend the app automatically when it is disabled.
 * @return BOOL (YES: Auto suspend, NO: No auto suspend).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isUseAutoSuspend;

/**
 * @brief Returns whether to support guest login only.
 * @return BOOL Whether to support guest login only (YES: Support guest login only, NO: Support welcome UI and ID based login as well)
 * @serviceDomain GLOBALGAME_SG
 */
- (BOOL)isEnforcedGuestLogin;

/**
 * @brief Returns whether to show punishment game page automatically when a user who punished logs in. 
 * @return BOOL (YES: Show the punishement page automatically, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isShowUiPunishment;

/**
 * @brief Returns whether to animate login UI launching and closing.
 * @return BOOL (YES : enable animation, NO : disable animation).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)enableLoginUIAnimation;

/**
 * @brief Returns whether to authorize line email account login.
 * @return BOOL (YES : enable email account, NO : disable email account).
 * @serviceDomain LINEGAME
 */
- (BOOL)enableLineEmailLogin;

/**
 * @brief Returns the login IDP.
 * @return NSString* The login IDP.
 * @serviceDomain GLOBALGAME_CN
 */
- (NSString*)loginIDP;

/**
 * @brief Returns whether use xcode5 later
 * @return BOOL (YES : xcode5 later, NO : xcode4 eariler).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isUseXcode5Later;

/**
 * @brief Returns whether use joga
 * @return BOOL (YES : Use joga, NO : Do not use joga).
 * @serviceDomain HANGAME
 */
- (BOOL)isUseJoga;

/**
 * @brief Returns How Time Retry Login
 * @return NSNumber*
 * @serviceDomain HANGAME_JP
 */
- (NSNumber*)loginRetryTimes;

/**
 * @brief Sets the HSP launching zone.<br>This function does not change HSPConfiguration.plist.
 * @param launchingZone HSP launching zone
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setLaunchingZone:(NSString*)launchingZone;

/**
 * @brief Sets the URL of launching service.<br>This function does not change HSPConfiguration.plist.
 * @param addressLaunching URL of launching service
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setAddressLaunching:(NSString*)addressLaunching;

/**
 * @brief Sets the URL of nelo service.<br>This function does not change HSPConfiguration.plist.
 * @param neloServerAddress URL of nelo service
 * @serviceDomain LINEGAME
 */
- (void)setNeloServerAddress:(NSString*)neloServerAddress;

/**
 * @brief Sets the debug level of nelo service.<br>This function does not change HSPConfiguration.plist.
 * @param neloDebugLevel debug level of nelo service
 * @serviceDomain LINEGAME
 */
- (void)setNeloDebugLevel:(NSString*)neloDebugLevel;

/**
 * @brief Sets the current launching zone as real release. <br>This function does not change HSPConfiguration.plist.
 * @param realZone Whether the current launching zone is for real release
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setRealZone:(BOOL)realZone;

/**
 * @brief Sets refresh time interval of the launching service when re-logged in (unit: second).<br>This function does not change HSPConfiguration.plist.
 * Sets the time interval to refresh the information of launching service, and within this time interval the cached information is used. 
 * @param lncRefreshTimeInterval Refresh time interval of the launching service to set
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setLncRefreshTimeInterval:(NSTimeInterval)lncRefreshTimeInterval;

/**
 * @brief Returns the time interval of heartbeat exchanged with the HSP server (unit: second).<br>This function does not change HSPConfiguration.plist.
 * The setting value is used by default. If the heartbeat interval has been set by the launching service, it takes priority over the one set by HSP client.
 * @param heartBeatTimeInterval Heartbeat interval
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setHeartBeatTimeInterval:(NSTimeInterval)heartBeatTimeInterval;

/**
 * @brief Sets the log level of HSPSDK.<br>This function does not change HSPConfiguration.plist.
 * @param debugLevel Debug level to set.
 @"LOW" : Displays the logs using HSP_LOG_ERROR.
 @"MIDDLE" : Displays the logs using HSP_LOG_ERROR and HSP_LOG_DEBUG.
 @"HIGH" : Displays the logs using HSP_LOG_ERROR, HSP_LOG_DEBUG, and HSP_LOG_INFO.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setDebugLevel:(NSString*)debugLevel;

/**
 * @brief Sets whether to enable assertion which occurs in HSPSDK.<br>This function does not change HSPConfiguration.plist.
 * @param enableAssertion Whether to enable assertion. Set YES to generate an exception, NO to ignore it.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setEnableAssertion:(BOOL)enableAssertion;

/**
 * @brief Sets the locale of the game application.<br>This function does not change HSPConfiguration.plist.
 * HSP is implemented differently by this locale setting.
 * @param locale Locale setting. @"KR"(Korea)/@"JP"(Japan) supported.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setLocale:(NSString*)locale;

/**
 * @brief Sets the default market information for the game application.<br>This function does not change HSPConfiguration.plist.
 * KA(Korean App Storer)/JA(Japanese App Storer).
 * @param marketCode Market code to set. @"KA"(Korean App Store)/@"JA"(Japanese App Store) supported.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setMarketCode:(NSString*)marketCode;

/**
 * @brief Sets the encoding type of images to be uploaded in HSP (JPEG/PNG).<br>This function does not change HSPConfiguration.plist.
 * @param saveImageFormat Image format to set. @"JPEG"/@"PNG" supported.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setSaveImageFormat:(NSString*)saveImageFormat;

/**
 * @brief Sets the timeout interval for the requests to the HSP server (unit: second).<br>This function does not change HSPConfiguration.plist.
 * @param timeoutTCP Timeout interval to set.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setTimeoutTCP:(NSTimeInterval)timeoutTCP;

/**
 * @brief Sets the timeout interval for HTTP requests (unit: second).<br>This function does not change HSPConfiguration.plist.
 * @param NSTimeInterval Timeout interval to set.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setTimeoutHTTP:(NSTimeInterval)timeoutHTTP;

/**
 * @brief Sets whether to show the Bridge view displayed when logged in. <br>This function does not change HSPConfiguration.plist.
 * @param show (YES: Show HSP Bridge view, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setShowUiWelcomeMessage:(BOOL)show;

/**
 * @brief Sets whether HSP supports the UI flow which prompts users to download the new version of the game.<br>This function does not change HSPConfiguration.plist.
 * @param show (YES: Show HSP default UI flow, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setShowUiVersionCheck:(BOOL)show;

/**
 * @brief Sets whether HSP supports the UI flow for maintenance.<br>This function does not change HSPConfiguration.plist.
 * @param show (YES: Show HSP default UI flow, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setShowUiMaintenance:(BOOL)show;

/**
 * @brief Sets whether to show recommended game page automatically when a user who gets a game recommendation logs in. <br>This function does not change HSPConfiguration.plist.
 * @param show (YES: Show the recommended game page automatically, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setShowUiRecommendGame:(BOOL)show;

/**
 * @brief Sets whether to support the push service.<br>This function does not change HSPConfiguration.plist.
 * @param use (YES: Use push service, NO: Not use).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setUsePush:(BOOL)use;

/**
 * @brief Sets whether to support the nelo service.<br>This function does not change HSPConfiguration.plist.
 * @param use (YES: Use nelo service, NO: Not use).
 * @serviceDomain LINEGAME
 */
- (void)setUseNelo:(BOOL)use;

/**
 * @brief Sets whether to support popover style on iPad (not supported on iPhone).<br>Default value is YES.<br>This function does not change HSPConfiguration.plist.
 * @param use (YES: Use popover style, NO: Not use).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setUseStylePopover:(BOOL)use;

/**
 * @brief Sets whether HSP automatically re-tries to report missions or ranking scores to the server after it fails.<br>This function does not change HSPConfiguration.plist.
 * @param use (YES: Auto re-try, NO: No re-try).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setUseResendFailedData:(BOOL)use;

/**
 * @brief Sets whether to suspend the app automatically whe it is disabled.
 * @param  use (YES: Auto suspend, NO: No auto suspend).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setUseAutoSuspend:(BOOL)use;

/**
 * @brief Sets whether to support guest login only. <br> When the parameter 'use' is set to YES, HSP uses guest login only and does not display welcome UI.
 * @param  use Whether to support guest login only (YES: Use guest login only, NO: Use welcome UI and ID based login as well)
 * @serviceDomain GLOBALGAME_SG
 */
- (void)setEnforcedGuestLogin:(BOOL)use;

/**
 * @brief Sets whether to show punishment page automatically when a user who punished logs in. <br>This function does not change HSPConfiguration.plist.
 * @param show (YES: Show the punishment page automatically, NO: Not show).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setShowUiPunishment:(BOOL)show;

/**
 * @brief Sets whether to animate login UI launching and closing. <br>This function does not change HSPConfiguration.plist.
 * @param show (YES: enable animation, NO: disable animation).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setEnableLoginUIAnimation:(BOOL)enable;

/**
 * @brief Sets whether to authorize line email account login. <br>This function does not change HSPConfiguration.plist.
 * @param show (YES: enable email account, NO: disable email account).
 * @serviceDomain LINEGAME
 */
- (void)setEnableLineEmailLogin:(BOOL)enable;

/**
 * @brief Sets the locale of the login IDP.<br>This function does not change HSPConfiguration.plist.
 * @param loginIDP Login IDP to set.
 * @serviceDomain GLOBALGAME_CN
 */
- (void)setLoginIDP:(NSString*)loginIDP;

/**
 * @brief Sets whether use xcode5 later or not <br>This function does not change HSPConfiguration.plist.
 * @param use (YES : xcode5 later, NO : xcode4 eariler).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)setUseXCode5Later:(BOOL)use;

/**
 * @brief Sets whether use JOGA agree view or not. <br>This function does not change HSPConfiguration.plist.
 * @param use (YES : Use JOGA agree view, NO : Do not use JOGA agree view).
 * @serviceDomain HANGAME
 */
- (void)setUseJoga:(BOOL)use;

@end
