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
 * @deprecated
 * @brief Deals with authentication and requests for me2day.
 *
 * By abstracting APIs of me2day, this interface enables you to use authentication process to me2day and functions supported by me2day.
 */
@interface HSPMe2day : NSObject
{
}

/**
 * @deprecated
 * @brief Attempts to log in to me2day.<br>
 * Logs in automatically if there is a login authentication key, or prompts users to log in manually with login UI.
 */
+ (void)loginByForce:(BOOL)force completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @deprecated
 * @brief Attempts to log out of me2day.
 */
+ (void)logoutWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @deprecated
 * @brief Checks whether to use the functions of me2day.<br>
 * This function is used to check if the authentication key for me2day is valid.
 * The authentication key may expire after login.
 */
+ (void)verifyAuthenticationWithCompletionHandler:(void (^)(NSDictionary* myData, HSPError *error))completionHandler;

/**
 * @deprecated
 * @brief Attemtps to post a feed message on me2day.
 */
+ (void)feedWithAuthentication:(BOOL)doAuthentication
					   message:(NSString*)message
			 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @deprecated
 * @brief Requests a user name that matches a me2day ID.
 *
 */
+ (void)requestUserNameWithIdCode:(NSString*)idCode
				completionHandler:(void (^)(NSString* name, HSPError *error))completionHandler;

/**
 * @deprecated
 * @brief Requests names and profile images of me2day friends.
 */
+ (void)requestFriendInfosWith:(NSRange)range
			 completionHandler:(void (^)(NSArray* idArray, NSArray* nameArray, NSArray* imageUrlArray, HSPError *error))completionHandler;

@end
