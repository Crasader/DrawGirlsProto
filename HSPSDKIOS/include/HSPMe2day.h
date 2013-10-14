//
//  HSPMe2day.h
//  HSPInterface
//
//  Created by Kim Wooyong on 12. 7. 4..
//  Copyright (c) 2012년 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class HSPError;

/**
 * @brief Deals with authentication and requests for me2day.
 *
 * By abstracting APIs of me2day, this interface enables you to use authentication process to me2day and functions supported by me2day.  
 */
@interface HSPMe2day : NSObject {
}

/**
 * @brief Attempts to log in to me2day.<br>
 * Logs in automatically if there is a login authentication key, or prompts users to log in manually with login UI.
 *
 * @param force Whether to force login or not.<br>If YES, HSP displays UI prompting login regardless of the existing login authentication key.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMe2day loginByForce:NO completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully logged in to me2day.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to log in to me2day.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loginByForce:(BOOL)force completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Attempts to log out of me2day.
 *
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMe2day logoutWithCompletionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully logged out of me2day.");	
 * 	} else
 * 	{
 * 		NSLog(@"Failed to log out of me2day.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     logoutWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Checks whether to use the functions of me2day.<br>
 * This function is used to check if the authentication key for me2day is valid.
 * The authentication key may expire after login.
 *
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param myData My data.<br>My data received from the me2day service.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMe2day verifyAuthenticationWithCompletionHandler:^(NSDictionary* myData, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"me2day authentication is valid.");		
 * 	} else
 * 	{
 * 		NSLog(@"me2day authentication is not valid.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     verifyAuthenticationWithCompletionHandler:(void (^)(NSDictionary* myData, HSPError *error))completionHandler;

/**
 * @brief Attemtps to post a feed message on me2day.
 * 
 * @param doAuthentication Whether to perform login process if not logged in.
 * @param message A feed message to post.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error 에러.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMe2day feedWithAuthentication:YES message:@"A feed message posted by the Smart Hangame app" completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully posted to me2day.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to post to me2day.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     feedWithAuthentication:(BOOL)doAuthentication
                           message:(NSString*)message
                 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests a user name that matches a me2day ID.
 *
 * @param idCode me2day ID.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param name User name.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMe2day requestUserNameWithIdCode:@"testID" completionHandler:^(NSString* name, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"me2day user names successfully received.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to receive me2day user names.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     requestUserNameWithIdCode:(NSString*)idCode
                    completionHandler:(void (^)(NSString* name, HSPError *error))completionHandler;

/**
 * @brief Requests names and profile images of me2day friends.
 *
 * @param range Range.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param idArray List of IDs.<br>Array of NSStrings.
 * @param nameArray List of names.<br>Array of NSStrings.
 * @param imageUrlArray List of image URLs.<br>Array of NSStrings.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMe2day requestFriendInfosWith:NSMakeRange(0, 10) completionHandler:^(NSArray *idArray, NSArray *nameArray, NSArray *imageUrlArray, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully received information of me2day friends.");		
 * 	} else
 * 	{
 * 		NSLog(@"Failed to receive information of me2day friends.(%@)", error);
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     requestFriendInfosWith:(NSRange)range
                 completionHandler:(void (^)(NSArray* idArray, NSArray* nameArray, NSArray* imageUrlArray, HSPError *error))completionHandler;

@end
