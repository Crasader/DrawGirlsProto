//
//  HSPFacebook.h
//  HSPInterface
//
//  Created by Kim Wooyong on 12. 5. 24..
//  Copyright (c) 2012년 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "HSPOAuth.h"

@class HSPError;

/**
 * @brief HSPFacebookProfile class
 *
 * HSPFacebookProfile includes facebookId, hspMemberNo and whether memberNo is my friend.
 */
@interface HSPFacebookProfile : NSObject
{
	NSString*		_facebookId;
	int64_t			_memberNo;
	BOOL			_isMyFriend;
}

/**
 * @brief facebookId of HSPFacebookProfile class (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)	NSString*		facebookId;

/**
 * @brief hsp memberNo mapped by facebookId (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly)	int64_t			memberNo;

/**
 * @brief whether memberNo is my friend (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly) BOOL			isMyFriend;

@end

/**
 * @brief Deals with authentication and requests for Facebook. 
 *
 * By abstracting APIs of Facebook, this interface enables you to use authenticate process to Facebook and functions supported by Facebook.
 */
@interface HSPFacebook : NSObject {
}

/**
 * @brief Returns the detailed information of OAuth specification authenticated to Facebook. <br>
 * @return id<HSPOAuth20> An instance of the class that implements HSPOAuth20 protocol.<br>
 * Returns nil if not logged in to Facebook.
 * @code
 * id<HSPOAuth20> oauthInfo = [HSPFacebook oauthInfo];
 * if ( oauthInfo != nil )
 * {
 *      NSLog(@"Access token authenticated to Facebook: %@", [oauthInfo accessToken]);
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (id<HSPOAuth20>)oauthInfo;

/**
 * @brief Requests the server to log in to Facebook.<br>
 * Logs in to Facebook automatically if there is a login authentication key, or prompts users to log in manually with login UI. 
 *
 * @param force Whether to force login. If YES, HSP displays UI prompting login regardless of the existing login authentication key.
 * @param completionHandler Is called when a response to the request that attempts to log in to Facebook is received from the server. 
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook loginByForce:NO completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully logged in to Facebook.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to log in to Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)loginByForce:(BOOL)force completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to log out of Facebook.
 *
 * @param completionHandler Is called when a response to the request that attempts to log out of Facebook is received from the server.
 *
 * @code
 * [HSPFacebook logoutWithCompletionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully logged out of Facebook.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to log out of Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)logoutWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to check whether to use the functions of Facebook.<br>
 * This function is used to check if the authentication key for Facebook is valid.  
 * The authentication key may expire in a specified time period after login.
 *
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param myData My data.<br>The data received from Facebook.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook verifyAuthenticationWithCompletionHandler:^(NSDictionary* myData, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Facebook authentication is valid.");		
 * 	} else
 * 	{
 * 		NSLog(@"Facebook authentication is not valid.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)verifyAuthenticationWithCompletionHandler:(void (^)(NSDictionary* myData, HSPError *error))completionHandler;

/**
 * @brief Requests the server to post a feed message on Facebook.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message A feed message to post.
 * @param imageURL Image URL.
 * @param name Name.
 * @param link Link.
 * @param captionLink Caption link.
 * @param descriptionLink Description link. 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook feedWithAuthentication:YES 
 *                             message:@"A feed message posted by the Smart Hangame app"
 *                            imageURL:@" http://images.hangame.co.kr/hangame/main/gnb_hangame.gif"
 *                                name:@"Hangame"
 *                                link:@" http://smart.hangame.com/"
 *                         captionLink:@"http://smart.hangame.com/"
 *                     descriptionLink:@"Smart Hangame"
 *                   completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully posted to Facebook.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to post to Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)feedWithAuthentication:(BOOL)doAuthentication
					   message:(NSString*)message
					  imageURL:(NSString*)imageURL
						  name:(NSString*)name
						  link:(NSString*)link
				   captionLink:(NSString*)captionLink
			   descriptionLink:(NSString*)descriptionLink
			 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to upload an image to Facebook album.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message Description about the image.
 * @param image Image.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook uploadImageWithAuthentication:YES
 *                                    message:@"handsome"
 *                                      image:[UIImage imageNamed:@"MyPhoto.png"]
 *                          completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully uploaded to Facebook.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to upload to Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */ 
+ (void)uploadImageWithAuthentication:(BOOL)doAuthentication
							  message:(NSString*)message
								image:(UIImage*)image
					completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to upload an image to Facebook album.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message Description about the image.
 * @param imagePath Absolute path of the image.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook uploadImageWithAuthentication:YES
 *                                    message:@"handsome"
 *                                  imagePath:imagePath // Absolute pathe of the image
 *                          completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully uploaded to Facebook.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to upload to Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */ 
+ (void)uploadImageWithAuthentication:(BOOL)doAuthentication
							  message:(NSString*)message
							imagePath:(NSString*)imagePath
					completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Displays the UI to prompt users to post a feed message to Facebook.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message A feed message to post.
 * @param imageURL Image URL.
 * @param name Name.
 * @param link Link.
 * @param captionLink Caption link.
 * @param descriptionLink Description link. 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook feedByUiWithAuthentication:YES 
 *                               message:@"Feed by Smart Hangame"
 *                              imageURL:@" http://images.hangame.co.kr/hangame/main/gnb_hangame.gif"
 *                                  name:@"Hangame"
 *                                  link:@" http://smart.hangame.com/"
 *                           captionLink:@"http://smart.hangame.com/"
 *                       descriptionLink:@"Smart Hangame"
 *                     completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully posted to Facebook.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to post to Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)feedByUiWithAuthentication:(BOOL)doAuthentication
						   message:(NSString*)message
						  imageURL:(NSString*)imageURL
							  name:(NSString*)name
							  link:(NSString*)link
					   captionLink:(NSString*)captionLink
				   descriptionLink:(NSString*)descriptionLink
				 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Displays the UI to prompt users to upload an image to Facebook album.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message Description about the image.
 * @param image Image.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook uploadImageByUiWithAuthentication:YES
 *                                        message:@"handsome"
 *                                          image:[UIImage imageNamed:@"MyPhoto.png"]
 *                              completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully uploaded to Facebook.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to upload to Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */ 
+ (void)uploadImageByUiWithAuthentication:(BOOL)doAuthentication
								  message:(NSString*)message
									image:(UIImage*)image
						completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Displays the UI to prompt users to upload an image to Facebook album..
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message Description about the image.
 * @param imagePath Absolute path of the image.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook uploadImageByUiWithAuthentication:YES
 *                                        message:@"handsome"
 *                                      imagePath:imagePath // absolute path of the image
 *                              completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully uploaded to Facebook.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to upload to Facebook.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */ 
+ (void)uploadImageByUiWithAuthentication:(BOOL)doAuthentication
								  message:(NSString*)message
								imagePath:(NSString*)imagePath
						completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests a user name that matches the specified Facebook ID. 
 *
 * @param idCode Facebook ID.
 * @param completionHandler Is called when a response to the request is received from the server. 
 *
 * This block needs the following parameter:
 * @param name Facebook user name. 
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook requestUserNameWithIdCode:@"123434" completionHandler:^(NSString* name, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      NSLog(@"Facebook user name: %@", name);
 * 		NSLog(@"Facebook user names successfully received.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to receive Facebook user names.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)requestUserNameWithIdCode:(NSString*)idCode
				completionHandler:(void (^)(NSString* name, HSPError *error))completionHandler;

/**
 * @brief Requests a list of HSP Member numbers of Facebbok users from the server. <br>
 * You can requests facebook IDs of up to 500 users; for facebook ID of a user who does not authenticated to HSP, the
 * callback returns 0.
 * @param facebookIds List of facebook IDs.
 * @param completionHandler Is called when a response to the request is received from the server.
 * @return void
 *
 * This block needs the following parameters:
 * @param facebookProfiles list of HSPFacebookProfile object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook queryHSPMemberNosWithFacebookIds:[NSArray arrayWithObject:facebookId] completionHandler:^(NSArray* facebookProfiles, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPFacebookProfile* facebookProfile in facebookProfiles )
 *			{
 *				NSLog(@"FacebookId : %@, hspMemberNo : %lld, isMyFriend = %@", facebookProfile.facebookId, facebookProfile.memberNo, facebookProfile.isMyFriend ? @"YES" : @"NO" );
 *			}
 *		}
 *      else
 *          NSLog(@"Failed to query hspmemberNo with facebookId(%@)", error);
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)queryHSPMemberNosWithFacebookIds:(NSArray*)facebookIds
					   completionHandler:(void (^)(NSArray* facebookProfiles, HSPError* error))completionHandler;

/**
 * @brief Requests facebook invite with specific friend's facebook ID. To see all of your facebook friends in webView, set facebookIds parameter to nil. 
 * @param facebookIds List of facebook friends IDs.
 * @param completionHandler Is called when a response to the request is received from the server.
 * @return void
 *
 * This block needs the following parameters:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPFacebook sendAppRequestWithFacebookIds:[NSArray arrayWithObject:facebookId] message:@"INVITE_MSG" completionHandler:^(HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *		{
 *			NSLog(@"Invite Success");
 *		}
 *      else
 *          NSLog(@"Invite Failed..(%@)", error);
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)sendAppRequestWithFacebookIds:(NSArray*)facebookIds
							  message:(NSString*)message
					completionHandler:(void (^)(HSPError* error))completionHandler;

@end
