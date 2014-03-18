//
//  HSPUri.h
//  HSPCore
//
//  Created by  on 11. 10. 19..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * @brief Protocol providing the information to implement OAuth1.0a.
 */
@protocol HSPOAuth10a

/**
 * @brief Returns the name of service provider.
 * @return NSString* Service provider's name.<br> Returns a valid value only when logged in the HSP.
 */
- (NSString*)serviceProvider;

/**
 * @brief Returns a consumer key to differentiate applications provided by the service provider.
 * @return NSString* A key value to differentiate OAuth-authorized applications.
 */
- (NSString*)consumerKey;

/**
 * @brief Returns a consumer secret key of the application issued by the service provider.
 * @return NSString* A consumer secret key of the application issued by the service provider.<br> Returns a valid value only when logged in the HSP.
 */
- (NSString*)consumerSecret;

/**
 * @brief Checks if there is an OAuth-authorized access token.
 * @return BOOL Returns YES if there is an OAuth-authorized access token, or NO otherwise.
 */
- (BOOL)hasAccessToken;

/**
 * @brief Returns an OAuth-authorized access token if there is.
 * @return NSString* Returns an OAuth-authorized access token if there is, or nil otherwise. 
 */
- (NSString*)accessToken;

/**
 * @brief Returns an OAuth-authorized access token secret issued by the service provider. 
 * @return NSString* Returns an OAuth-authorized access token secret if there is, or nil otherwise.
 */
- (NSString*)accessTokenSecret;

@end

/**
 * @brief Protocol for providing the information to implement OAuth2.0.
 */
@protocol HSPOAuth20

/**
 * @brief Returns the name of service provider.
 * @return NSString* Service provider's name.<br> Returns a valid value only when logged in the HSP.
 */
- (NSString*)serviceProvider;

/**
 * @brief Returns a consumer key to differentiate applications provided by the service provider.
 * @return NSString* A consumer key to differentiate OAuth-authorized applications.<br> Returns a valid value only when logged in the HSP.
 */
- (NSString*)consumerKey;

/**
 * @brief Returns a consumer secret key of the application issued by the service provider. 
 * @return NSString* A consumer secret key of the application issued by the service provider.<br> Returns a valid value only when logged in the HSP.
 */
- (NSString*)consumerSecret;

/**
 * @brief Checks if there is an OAuth-authorized access token.
 * @return BOOL Returns YES if there is an OAuth-authorized access token, or NO otherwise.
 */
- (BOOL)hasAccessToken;

/**
 * @brief Returns an OAuth-authorized access token if there is.
 * @return NSString* Returns an OAuth-authorized access token if there is, or nil otherwise.
 */
- (NSString*)accessToken;

@end
