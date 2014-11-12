//
//  HSPCore.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"
#import "HSPServiceProperties.h"
#import "HSPDetailedProfile.h"

@interface NSNumber (HSP)

/**
 * Returns a NSNumber object including a member number. 
 * @param value Member number.
 */
+ (NSNumber*)numberWithMemberNo:(int64_t)value;

/**
 * Returns a NSNumber object including game numbers.
 * @param value Game number.
 */
+ (NSNumber*)numberWithGameNo:(int32_t)value;

/**
 * Returns a member number.
 * @return Member number.
 */
- (int64_t)memberNoValue;

/**
 * Returns a game number.
 * @return Game number.
 */
- (int32_t)gameNoValue;

@end

/**
 * @brief enum for OAuth Provider.
 */
typedef enum
{
	HSP_OAUTHPROVIDER_TWITTER = 0,  /**< Twitter. */
	HSP_OAUTHPROVIDER_MIXI,			/**< Mixi. */
	HSP_OAUTHPROVIDER_FACEBOOK,     /**< Facebook. */
	HSP_OAUTHPROVIDER_YAHOO,		/**< Yahoo. */
	HSP_OAUTHPROVIDER_GOOGLE,		/**< Google. */
	HSP_OAUTHPROVIDER_HANGAME,		/**< Hangame. */
	HSP_OAUTHPROVIDER_NAVER,		/**< Naver. */
	HSP_OAUTHPROVIDER_LINE,			/**< Line. */
	HSP_OAUTHPROVIDER_KAKAO,		/**< Kakao. */
	HSP_OAUTHPROVIDER_GREE,			/**< Gree. */
	HSP_OAUTHPROVIDER_HANGAME_EX,	/**< Hangame_ex. */
	HSP_OAUTHPROVIDER_GSQUARE,		/**< GSquare. */
	HSP_OAUTHPROVIDER_POKETROKET,	/**< pocketroket. */
	HSP_OAUTHPROVIDER_NEID,         /**< NEID. */
	HSP_OAUTHPROVIDER_GUEST,        /**< Guest. */
	HSP_OAUTHPROVIDER_WEIBO,		/**< Weibo. */
	HSP_OAUTHPROVIDER_GAMECENTER,	/**< Apple Game Center. */
	HSP_OAUTHPROVIDER_HANGAME_JP,	/**< Hangame jp. */
	HSP_OAUTHPROVIDER_HANGAME_JP_EMAIL,	/**< Hangame jp Email. */
	HSP_OAUTHPROVIDER_LASTLOGIN			/**< Only Japnan. */
} HSPOAuthProvider;

/**
 * @brief enum for Hangame Mapping Type
 */
typedef enum
{
	HSP_MAPPINGTYPE_HANGAME = 0,	/**< Hangame mapping type. */
	HSP_MAPPINGTYPE_EMAIL,			/**< Email mapping type. */
	HSP_MAPPINGTYPE_FACEBOOK,       /**< Facebook mapping type. */
	HSP_MAPPINGTYPE_TWITTER,        /**< Twitter mapping type. */
	HSP_MAPPINGTYPE_GAMECENTER,		/**< Game Center mapping type. */
} HSPMappingType;

/**
 * @brief Deals with Login Profile.
 */
@interface HSPLoginProfile : NSObject
{
	NSString*		_nickname;
	NSString*		_profileImageUrl;
}

/**
 * @brief nickname.
 */
@property(nonatomic, retain)		NSString*		nickname;

/**
 * @brief profile image url.
 */
@property(nonatomic, retain)		NSString*		profileImageUrl;

@end


/**
 * @brief Deals with OAuthData that needs for OAuth Login.
 */
@interface HSPOAuthData : NSObject
{
	int				_version;
	NSString*		_verifier;
	NSString*		_consumerKey;
	NSString*		_consumerSecret;
	NSString*		_nonce;
	NSString*		_signatureMethod;
	NSString*		_signature;
	NSString*		_accessToken;
	NSString*		_accessTokenSecret;
	NSString*		_timeStamp;
	NSString*		_refreshToken;
	long			_tokenExpireTime;
	NSString*		_extra;
}

/**
 * @brief version.
 */
@property(nonatomic, assign)		int				version;

/**
 * @brief verifier.
 */
@property(nonatomic, retain)		NSString*		verifier;

/**
 * @brief consumer key.
 */
@property(nonatomic, retain)		NSString*		consumerKey;

/**
 * @brief consumer secret.
 */
@property(nonatomic, retain)		NSString*		consumerSecret;

/**
 * @brief consumer nonce.
 */
@property(nonatomic, retain)		NSString*		nonce;

/**
 * @brief signature method.
 */
@property(nonatomic, retain)		NSString*		signatureMethod;

/**
 * @brief signature.
 */
@property(nonatomic, retain)		NSString*		signature;

/**
 * @brief acess token.
 */
@property(nonatomic, retain)		NSString*		accessToken;

/**
 * @brief acess token secret.
 */
@property(nonatomic, retain)		NSString*		accessTokenSecret;

/**
 * @brief time stamp.
 */
@property(nonatomic, retain)		NSString*		timeStamp;

/**
 * @brief refresh token.
 */
@property(nonatomic, retain)		NSString*		refreshToken;

/**
 * @brief token expired time.
 */
@property(nonatomic, assign)		long			tokenExpireTime;

/**
 * @brief extra data.
 */
@property(nonatomic, retain)		NSString*		extra;

- (id)initWithVersion:(int)version
			 verifier:(NSString*)verifier
		  consumerKey:(NSString*)consumerKey
	   consumerSecret:(NSString*)consumerSecret
				nonce:(NSString*)nonce
	  signatureMethod:(NSString*)signatureMethod
			signature:(NSString*)signature
		  accessToken:(NSString*)accessToken
	accessTokenSecret:(NSString*)accessTokenSecret
			timeStamp:(NSString*)timeStamp
		 refreshToken:(NSString*)refreshToken
	  tokenExpireTime:(long)tokenExpireTime
				extra:(NSString*)extra;

@end

/**
 * @brief Deals with connection with HSP server and related information. 
 * 
 * Calls createHSPCoreWithGameNo to create {@link HSPCore} instance. After that, use sharedHSPCore to get {@link HSPCore} instance, and use the instance functions of {@link HSPCore} to use {@link HSPCore}.
 * <p>
 * To use the classes other than {@link HSPCore}, {@link HSPBip}, {@link HSPUtil}, and {@link HSPWebClient}, you have to log in the HSP server. Call loginWithManualLogin to log in the HSP server, and logoutWithCompletionHandler to log out.
 */
@interface HSPCore : NSObject
{
	HSPServiceProperties*       _serviceProperties;

	id<UIApplicationDelegate>   _delegate;
	id<UIApplicationDelegate>   _hspDelegate;
}

/**
 * @brief Service property information. (read only)<br>
 * Gets the service property information received from the HSP server after login.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)		HSPServiceProperties*		serviceProperties;

/**
 * @brief Application delegate.
 */
@property(nonatomic, retain)				id<UIApplicationDelegate>	delegate;

/**
 * @brief Game number. (read only)<br>
 * A game number set by the createHSPCoreWithGameNo.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)				int32_t			gameNo;

/**
 * @brief Game ID. (read only)<br>
 * A game ID set by the createHSPCoreWithGameNo.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)		NSString*		gameID;

/**
 * @brief Game version. (read only)<br>
 * A game version set by the createHSPCoreWithGameNo.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)		NSString*		gameVersion;

/**
 * @brief Creates {@link HSPCore} object. This function needs to be called first to use HSP. <br>
 * If this function is called after the {@link HSPCore} object is created, it returns fail. Call sharedHSPCore to get the created {@link HSPCore} object.<br>
 * This function needs the information required to connect with ths HSP server, such as game number, game ID and game version. Once a {@link HSPCore} instance is created, you cannot create it again or change the settings.
 *
 * @param gameNo Game number.
 * @param gameID Game ID.
 * @param gameVersion Game version.
 * @return Result.
 * @see HSPLocale
 *
 * @code
 * BOOL result = [HSPCore createHSPCoreWithGameNo:101 gameID:@"SK101" gameVersion:@"1.0.0"];
 * @endcode 
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (BOOL)createHSPCoreWithGameNo:(int32_t)gameNo
						 gameID:(NSString*)gameID
					gameVersion:(NSString*)gameVersion;


/**
 * @brief Creates {@link HSPCore} object. This function needs to be called first to use HSP.<br>
 * If this function is called after the {@link HSPCore} object is created, it returns fail. Call sharedHSPCore to get the created {@link HSPCore} object.<br>
 * This function needs the information required to connect with ths HSP server, such as game number, game ID and game version. Once a {@link HSPCore} instance is created, you cannot create it again or change the settings.
 *
 * @param gameNo Game number.
 * @param gameID Game ID.
 * @param gameVersion Game version.
 * @param launchOptions Options to launch the game.
 * @return Result.
 * @see HSPLocale<br>UIApplicationDelegate<br>- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
 *
 * @code
 * BOOL result = [HSPCore createHSPCoreWithGameNo:101 gameID:@"SK101" gameVersion:@"1.0.0" launchOptions:launchOptions];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (BOOL)createHSPCoreWithGameNo:(int32_t)gameNo
						 gameID:(NSString*)gameID
					gameVersion:(NSString*)gameVersion
				  launchOptions:(NSDictionary*)launchOptions;

/**
 * @brief Gets a {@link HSPCore} object.
 * If the {@link HSPCore} object is not created by using the createHSPCoreWithGameNo, this function returns nil.
 * 
 * @return A {@link HSPCore} instance. If the object is not created, returns nil.
 * @see HSPCore
 *
 * @code
 * HSPCore* core = [HSPCore sharedHSPCore];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (HSPCore*)sharedHSPCore;

/**
 * @brief Adds a listener to be called before login.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addBeforeLoginListener:^() {
 *      NSLog(@"Before login.");
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)addBeforeLoginListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called after login.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addAfterLoginListener:^() {
 *      NSLog(@"After login.");
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)addAfterLoginListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called before logout.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addBeforeLogoutListener:^() {
 *      NSLog(@"Before logout.");
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)addBeforeLogoutListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called after logout.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addAfterLogoutListener:^() {
 *      NSLog(@"After logout.");
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)addAfterLogoutListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called before account reset. 
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addBeforeResetAccountListener:^() {
 *      NSLog(@"Before account reset.");
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)addBeforeResetAccountListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called after account reset.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addAfterResetAccountListener:^() {
 *      NSLog(@"After account reset.");
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)addAfterResetAccountListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called before account withdraw.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addBeforeWithdrawAccountListener:^() {
 *      NSLog(@"Before account withdraw.");
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)addBeforeWithdrawAccountListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called after account withdraw.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addAfterWithdrawAccountListener:^() {
 *      NSLog(@"After account withdraw.");
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)addAfterWithdrawAccountListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called before account mapping.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addBeforeMappingToAccountListener:^() {
 *      NSLog(@"Before account mapping.");
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
- (void)addBeforeMappingToAccountListener:(void (^)())listener;

/**
 * @brief Adds a listener to be called after account mapping.
 *
 * @param listener A listener.
 *
 * @code
 * [[HSPCore sharedHSPCore] addAfterMappingToAccountListener:^() {
 *      NSLog(@"After account mapping.");
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)addAfterMappingToAccountListener:(void (^)())listener;

/**
 * @brief set a handler to be called to set login profile on the way to login.
 * You can change the login profile data;
 *
 * @param handler A handler.
 *
 * @code
 * [[HSPCore sharedHSPCore] setLoginProfileHandler:^(HSPLoginProfile* loginProfile) {
 *      [loginProfile setNickname:@"PIG"];
 *		[loginProfile setProfileImageUrl:@""];
 * }];
 * @endcode
 * @serviceDomain KAKAOGAME
 */
- (void)setLoginProfileHandler:(void (^)(HSPLoginProfile* loginProfile))handler;

/**
 * @brief Set the gree delegate.
 *
 * @param delegate A delegate.
 * @endcode
 * @serviceDomain GREEGAME
 */
- (void)setGreeDelegate:(id)delegate;

/**
 * @brief Attempts to log in the HSP server.<br>
 * If logged in before, HSP attempts to log in the server with the same account automatically. If it succeeds, HSP gets the login result through the added handler. If it fails, HSP attempts to log in manually depending on the value of manualLogin; this process needs ID and password.<br>
 * After the login result is returned through the handler, you can use the 'state' function to analyze the current state and handle exceptions.
 *
 * @param manualLogin Whether to attempt to log in manually after auto login fails.
 * @param completionHandler Is called when a response to the request to log in is received from the server.
 *
 * This block needs the following parameters:
 * @param playable "playable" value of LaunchingService (LNC).
 * @param error Error.<br>If the funcion succeeds, this value is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] loginWithManualLogin:NO completionHandler:^(BOOL playable, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Logged in successfully");
 * 	} else
 * 	{
 * 		NSLog(@"Login failed (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)loginWithManualLogin:(BOOL)manualLogin
		   completionHandler:(void (^)(BOOL playable, HSPError *error))completionHandler;


/**
 * @brief Attempts to log with oAuth Data in the HSP server.<br>
 *
 * @param oAuthProvider OAuth Provider.
 * @param completionHandler Is called when a response to the request to log in is received from the server.
 *
 * This block needs the following parameters:
 * @param playable "playable" value of LaunchingService (LNC).
 * @param error Error.<br>If the funcion succeeds, this value is 0.
 *
 * @code
 *
 * [[HSPCore sharedHSPCore] loginWithOAuthProvider:HSP_OAUTHPROVIDER_GSQUARE
 *								 completionHandler:^(BOOL playable, HSPError *error) {
 *										NSLog(@"[LOGIN TEST] Login With GSquare Access Token : %@", error);
 * }];
 * @endcode
 * @serviceDomain ALL
 */
- (void)loginWithOAuthProvider:(HSPOAuthProvider)oAuthProvider
			 completionHandler:(void (^)(BOOL playable, HSPError *error))completionHandler;

/**
 * @brief Attempts to log with oAuth Data in the HSP server.<br>
 *
 * @param oAuthProvider OAuth Provider.
 * @param oAuthData data for OAuth login.
 * @param completionHandler Is called when a response to the request to log in is received from the server.
 *
 * This block needs the following parameters:
 * @param playable "playable" value of LaunchingService (LNC).
 * @param error Error.<br>If the funcion succeeds, this value is 0.
 *
 * @code
 * HSPOAuthData* oAuthData = [[HSPOAuthData alloc] init];
 * oAuthData.accessToken = @"Az";
 * oAuthData.accessTokenSecret = @"cMy";
 * oAuthData.tokenExpireTime = 1209600L;
 *
 * [[HSPCore sharedHSPCore] loginWithOAuthProvider:HSP_OAUTHPROVIDER_GSQUARE
 *										 oAuthData:oAuthData
 *								 completionHandler:^(BOOL playable, HSPError *error) {
 *										NSLog(@"[LOGIN TEST] Login With GSquare Access Token : %@", error);
 * }];
 * @endcode
 * @serviceDomain ALL
 */
- (void)loginWithOAuthProvider:(HSPOAuthProvider)oAuthProvider
					 oAuthData:(HSPOAuthData*)oAuthData
			 completionHandler:(void (^)(BOOL playalbe, HSPError *error))completionHandler;

/**
 * @brief Attempts to log by guest in the HSP server.<br>
 *
 * @param loginType HSP_LOGIN_TYPE_GUEST, HSP_LOGIN_TYPE_GSQUARE, HSP_LOGIN_TYPE_FACEBOOK, HSP_LOGIN_TYPE_CREATINGACCOUNT types are only available.
 * @param completionHandler Is called when a response to the request to log in is received from the server.
 *
 * This block needs the following parameters:
 * @param playable "playable" value of LaunchingService (LNC).
 * @param error Error.<br>If the funcion succeeds, this value is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] loginByLoginType:HSP_LOGIN_TYPE_GSQUARE completionHandler:^(BOOL playable, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Logged in successfully");
 * 	} else
 * 	{
 * 		NSLog(@"Login failed (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain GLOBALGAME_SG
 */
- (void)loginByLoginType:(HSPLoginType)loginType
	   completionHandler:(void (^)(BOOL playable, HSPError *error))completionHandler;

/**
 * @brief Attempts to log out the HSP server.
 * 
 * @param completionHandler Is called when a response to the request to log out is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] logoutWithCompletionHandler:^(HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Logged out successfully.");
 * 	} else
 * 	{	
 * 		NSLog(@"Logout failed (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG GREEGAME
 */
- (void)logoutWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests to reset accounts. Resets the accounts on the device. After the account reset, HSP cannot log in automatically and needs to log in manually.  
 * 
 * @param completionHandler Is called when a response to the request to reset the account is received from the server. 
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] resetAccountWithCompletionHandler:^(HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Account reset successfully.");
 * 	} else
 * 	{	
 * 		NSLog(@"Account reset failed (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)resetAccountWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests to withdraw accounts. Withdraws the accounts on the device. After the account withdraw, HSP cannot log in automatically and needs to log in manually.
 *
 * @param completionHandler Is called when a response to the request to withdraw the account is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] withdrawAccountWithCompletionHandler:^(HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Account withdraw successfully.");
 * 	} else
 * 	{
 * 		NSLog(@"Account withdraw failed (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)withdrawAccountWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Sets the account's status to OFFLINE. Reports to the server that the application has been disabled.
 * 
 * @param completionHandler Is called when a response is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] suspendWithCompletionHandler:^(HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The account status is set to OFFLINE successfully.");
 * 	} else
 * 	{	
 * 		NSLog(@"Failed to set the account status to OFFLINE (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)suspendWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to map the logged-in member numbers with other service accounts.<br>
 * In Korea, map with the Hangame accounts, and in Japan, map with the accounts of Google, Facebook, Twitter, Mixi and Hangame.
 * 
 * @param completionHandler Is called when a response to the request to map with other service accounts is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [[HSPCore sharedHSPCore] requestMappingToAccountWithCompletionHandler:^(HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully mapped with other service accounts.");
 * 	} else
 * 	{	
 * 		NSLog(@"Failed to map with other service accounts. (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)requestMappingToAccountWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to map the logged-in member numbers with other service accounts only for HANGAME domain.<br>
 * In Korea & Japan, map with the Hangame accounts or email account.
 *
 * @param completionHandler Is called when a response to the request to map with other service accounts is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] requestMappingToAccountWithMappingType:HSP_MAPPINGTYPE_HANGAME
												 completionHandler:^(HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully mapped with other service accounts.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to map with other service accounts. (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
- (void)requestMappingToAccountWithMappingType:(HSPMappingType)mappingType
							 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to re-map the logged-in member numbers with other service accounts only for TOASTGAME-US domain.<br>
 *
 * @param completionHandler Is called when a response to the request to re-map with other service accounts is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 * @param memberNo member number from previous mapping
 *
 * @serviceDomain TOASTGAME
 */
- (void)requestMappingToAccountWithMappingType:(HSPMappingType)mappingType
									 overwrite:(BOOL)overwrite
							 completionHandler:(void (^)(HSPError *error, int64_t memberNo))completionHandler;
/**
 * @brief Requests Universal Time Coordinated (UTC) of the HSP server currently connected.<br>
 * This function is used to know the time synchronized with the server, since the time on a device can be updated by a user.
 * 
 * @param completionHandler Is called when a response to the request for UTC is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [[HSPCore sharedHSPCore] requestServerUTCWithCompletionHandler:^(NSDate* date, HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Get current UTC time successfully from the server. - %@", date);
 * 	} else
 * 	{	
 * 		NSLog(@"Failed to get current UTC time from the server. (%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)requestServerUTCWithCompletionHandler:(void (^)(NSDate* date, HSPError *error))completionHandler;

/**
 * @brief Requests Change current guest memberNo in HANGAME_JP Domain.<br>
 * This function is used to change guest memberNo in Guest Login User. If you use this api in Hangame login User, the error will be returned.
 *
 * @param completionHandler Is called when a response to the request for UTC is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [[HSPCore sharedHSPCore] requestChangeGuestMemberNoWithCompletionHandler:^(HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Success to change memberNo.);
 * 	} else
 * 	{
 * 		NSLog(@"Failed to change memberNo(%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME_JP
 */
- (void)requestChangeGuestMemberNoWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Returns an authentication ticket. <br>
 * When logged in successfully, returns an authentication ticket rather than nil. This ticket is updated periodically, and used to check if a user logs in properly. 
 * 
 * @return An authentication ticket.<br>Invalid value if nil is returned.
 *
 * @code
 * NSString* ticket = [HSPCore sharedHSPCore].ticket;
 * NSLog(@"HSP authentication ticket = %@", ticket);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)ticket;

/**
 * @brief Returns the current HSP state.<br>
 * When the requests from the asynchronous functions provided by {@link HSPCore} have failed, use this to check the current HSP state.
 * 
 * @return State.
 * @see HSPState
 *
 * @code
 * HSPState state = [HSPCore sharedHSPCore].state;
 * switch ( state )
 * {
 *      case HSP_STATE_ONLINE:
 *          NSLog(@"HSP state = ONLINE");
 *          break;
 *      case HSP_STATE_INIT:
 *      case HSP_STATE_OFFLINE:
 *          NSLog(@"HSP state = OFFLINE");
 *          break;
 *      case HSP_STATE_UNAVAILABLE:
 *          NSLog(@"HSP state = UNAVAILABLE");
 *          break;
 *      default:
 *          NSLog(@"HSP state = UNKNOWN");
 *          break;
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (HSPState)state;

/**
 * @brief Returns the HSP Login Type.<br>
 * Provide internal login type infomation. for example, if user logined by guest, you may know about that.
 *
 * @return login type.
 * @see HSPLoginType
 * @serviceDomain KAKAOGAME GLOBALGAME_SG GLOBALGAME_CN
 */ 
- (HSPLoginType)loginType;

/**
 * @brief Returns my member number.
 * 
 * @return A member number.
 *
 * @code
 * int64_t memberNo = [HSPCore sharedHSPCore].memberNo;
 * NSLog(@"Member number = %lld", memberNo);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (int64_t)memberNo;

/**
 * @brief Returns my age.
 *
 * @return Member age. Returns -1 if not registered.
 *
 * @code
 * int8_t memberAge = [HSPCore sharedHSPCore].memberAge;
 * NSLog(@"Member age = %d", memberAge);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (int8_t)memberAge;

/**
 * @brief Returns my member ID.
 *
 * @return Member ID. Returns nil if not registered.
 *
 * @code
 * NSString* memberID = [HSPCore sharedHSPCore].memberID;
 * NSLog(@"Member ID = %@", memberID);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)memberID;

/**
 * @brief Returns facebook accessToken.
 *
 * @return facebook accessToken. Returns nil if not logged in with facebook.
 *
 * @code
 * NSString* accessToken = [[HSPCore sharedHSPCore] getOAuthAccessToken];
 * NSLog(@"accessToken = %@", accessToken);
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG TOASTGAME
 */
- (NSString*)getOAuthAccessToken;

@end

/**
 * @brief [Notifications] Gets posted when HSP Push Notification arrives. 
 */
extern NSString* const HSPPushNotificationName;

/**
 * @brief [Notifications] Gets posted when the account logs in.
 */
extern NSString* const HSPDidLoginNotificationName;

/**
 * @brief [Notifications] Gets posted when the account logs out.
 */
extern NSString* const HSPDidLogoutNotificationName;

/**
 * @brief [Notifications] Gets posted when the account is reset.
 */
extern NSString* const HSPDidResetAccountNotificationName;

/**
 * @brief [Notifications] Gets posted when the account is withdraw.
 */
extern NSString* const HSPDidWithdrawAccountNotificationName;

/**
 * @brief [Notifications] Gets posted when authentication expires because of external causes. 
 */
extern NSString* const HSPSessionExpiredNotificationName;
