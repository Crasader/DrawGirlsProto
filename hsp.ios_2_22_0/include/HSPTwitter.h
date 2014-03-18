//
//  HSPTwitter.h
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
 * @brief Deals with authentication and requests for Twitter.
 *
 * By abstracting APIs of Twitter, this interface enables you to use authentication process to Twitter and functions supported by Twitter.
 */
@interface HSPTwitter : NSObject {
}

/**
 * @brief Returns the detailed information of OAuth specification authenticated to Twitter.<br>
 * @return id<HSPOAuth10a> Returns an instance of the class that implements HSPOAuth10a protocol.<br>
 * Returns nil if not logged in to Twitter.
 * @code
 * id<HSPOAuth20> oauthInfo = [HSPTwitter oauthInfo];
 * if ( oauthInfo != nil )
 * {
 *      NSLog(@"Access token authenticated to Twitter : %@", [oauthInfo accessToken]);
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(id<HSPOAuth10a>)   oauthInfo;

/**
 * @brief Requests the server to log in to Twitter.<br>
 * Logs in to Twitter automatically if there is a login authentication key, or prompts users to log in manually with login UI other.
 *
 * @param force Whether to force login. If YES, HSP displays UI prompting login regardless of the existing login authentication key. 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter loginByForce:NO completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully logged in to Twitter.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to log in to Twitter.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loginByForce:(BOOL)force completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to log out of Twitter.
 *
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * @code
 * [HSPTwitter logoutWithCompletionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully logged out of Twitter.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to log out of Twitter.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     logoutWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to check whether to use functions of Twitter.<br>
 * This function is used to check if the authentication key for Twitter is valid.
 * The authentication key may expire after login.
 *
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param myData My data.<br>Data received from Twitter service.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter verifyAuthenticationWithCompletionHandler:^(NSDictionary* myData, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Twitter authentication is valid.");		
 * 	} else
 * 	{
 * 		NSLog(@"Twitter authentication is not valid.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     verifyAuthenticationWithCompletionHandler:(void (^)(NSDictionary* myData, HSPError *error))completionHandler;

/**
 * @brief Requests the server to post a feed message on Twitter.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message A feed message to post.
 * @param image A feed image to post.<br>To post a message only, set this as nil.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter feedWithAuthentication:YES
 *                            message:@"handsome"
 *                              image:[UIImage imageNamed:@"MyPhoto.png"]
 *                   completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully posted to Twitter.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to post to Twitter.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     feedWithAuthentication:(BOOL)doAuthentication
                           message:(NSString*)message
                             image:(UIImage*)image
                 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to post a feed message on Twitter.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message A feed message to post.
 * @param imagePath Absolute path of a feed image to post.<br>To post a message only, set this as nil.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter feedWithAuthentication:YES
 *                            message:@"handsome"
 *                          imagePath:imagePath // absolute path of a image
 *                   completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully posted to Twitter.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to post to Twitter.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     feedWithAuthentication:(BOOL)doAuthentication
                           message:(NSString*)message
                         imagePath:(NSString*)imagePath
                 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Displays the UI to prompt users to post a feed message to Twitter.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message A feed message to post.
 * @param image A feed image to post.<br>To post a message only, set this as nil.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter feedByUiWithAuthentication:YES
 *                                message:@"handsome"
 *                                  image:[UIImage imageNamed:@"MyPhoto.png"]
 *                      completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully posted to Twitter.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to post to Twitter.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     feedByUiWithAuthentication:(BOOL)doAuthentication
                               message:(NSString*)message
                                 image:(UIImage*)image
                     completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Displays the UI to prompt users to post a feed message to Twitter.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message A feed message to post.
 * @param image Absolute path of a feed image to post.<br>To post a message only, set this as nil.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter feedByUiWithAuthentication:YES
 *                                message:@"handsome"
 *                                  image:imagePath // Absolute path of a image
 *                      completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully posted to Twitter.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to post to Twitter.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     feedByUiWithAuthentication:(BOOL)doAuthentication
                               message:(NSString*)message
                             imagePath:(NSString*)imagePath
                     completionHandler:(void (^)(HSPError *error))completionHandler;
/**
 * @brief Requests the name of user that matches the specified Twitter ID. 
 *
 * @param idCode Twitter ID.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param name Twitter user name.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter requestUserNameWithIdCode:@"195291180" completionHandler:^(NSString* name, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      NSLog(@"Twitter user name : %@", name);
 * 		NSLog(@"Twitter user names successfully received.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to receive Twitter user name.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     requestUserNameWithIdCode:(NSString*)idCode
                    completionHandler:(void (^)(NSString* name, HSPError *error))completionHandler;

/**
 * @brief Requests a list of friend IDs of user that matches the specified Twitter ID. 
 *
 * @param idCode Twitter ID.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param friendIDs List of friend IDs.<br>Array of NSStrings.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter requestFriendListWithIdCode:@"195291180" completionHandler:^(NSArray* friendIDs, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"List of Twitter friend IDs successfully received.");
 *      for (NSString* friendID in friendIDs) {
 *          NSLog(@"Friend ID : %@", friendID);
 *      }
 * 	} else
 * 	{
 * 		NSLog(@"Failed to receive Twitter friend IDs.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     requestFriendListWithIdCode:(NSString*)idCode
                      completionHandler:(void (^)(NSArray* friendIDs, HSPError *error))completionHandler;

/**
 * @brief Requests a list of user names and profile images that match the specified Twitter ID List from the server.
 *
 * @param idCodeArray List of Twitter IDs. Array of NSStrings.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param idArray List of IDs.<br>Array of NSStrings.
 * @param nameArray List of user names.<br>Array of NSStrings.
 * @param imageUrlArray List of profile image URLs.<br>Array of NSStrings. 
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPTwitter requestFriendInfosWithIdCode:idCodeArray completionHandler:^(NSArray* idArray, NSArray* nameArray, NSArray* imageUrlArray, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Twitter user data successfully received.");
 *      for (NSString* friendID in friendIDs) {
 *          NSLog(@"ID : %@", friendID);
 *      }  
 *      for (NSString* name in nameArray) {
 *          NSLog(@"User name : %@", name);
 *      }  
 *      for (NSString* imageUrl in imageUrlArray) {
 *          NSLog(@"Image URL : %@", imageUrl);
 *      }   
 * 	} else
 * 	{
 * 		NSLog(@"Failed to receive Twitter user data.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     requestFriendInfosWithIdCode:(NSArray*)idCodeArray
                       completionHandler:(void (^)(NSArray* idArray, NSArray* nameArray, NSArray* imageUrlArray, HSPError *error))completionHandler;

@end
