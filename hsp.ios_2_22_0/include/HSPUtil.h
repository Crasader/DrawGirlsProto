//
//  HSPUtil.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 24..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPCore.h"
#import "HSPError.h"

/**
 * @brief Utility interface.
 */
@interface HSPUtil : NSObject
{
	
}

/**
 * @brief Returns HSP version.
 * 
 * @return HSP version.
 *
 * @code
 * NSString* version = [HSPUtil hspVersion];
 * NSLog(@"HSP Version : %@", version);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (NSString*)hspVersion;

/**
 * @brief Returns a unique device ID.
 * 
 * @return Device ID.
 *
 * @code
 * NSString* udid = [HSPUtil uniqueDeviceID];
 * NSLog(@"Unique Device ID : %@", udid);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (NSString*)uniqueDeviceID;


/**
 * @brief Returns the country code.
 *
 * 1. Returns the country code of the SIM card.
 * 2. if no country code of the SIM card exist, returns the country code set as device locale.
 # 3. if no country code set as device locale exist, returns @"ZZ".
 *
 * @return country code. ex) "KR"
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG 
 */
+ (NSString*)countryCode;

/**
 * @brief Returns the name of the user's home cellular service provider.
 *
 * @return the name of the carrier. ex) "SKTelecom", "olleh", "LG U+"
 * 
 * @code
 * NSString* carrierName = [HSPUtil carrierName];
 * NSLog(@"Carrier Name : %@", carrierName);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG 
 */
+ (NSString*)carrierName;

/**
 * @brief Returns the MCC+MNC(mobile country code + mobile network code).
 *
 * @return the MCC+MNC. ex) "45011" => MCC(450), MNC(11)
 *
 * @code
 * NSString* carrierCode = [HSPUtil carrierCode];
 * NSLog(@"Carrier Code : %@", carrierCode);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (NSString*)carrierCode;

/**
 * @brief Requests downloading images from web.
 *
 * @param url Web URL.
 * @param completionHandler Is called when images are downloaded.
 *
 * This block needs the following parameters:  
 * @param image Image.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPUtil downloadImageFromURL:@"http://static.naver.net/www/u/2010/0611/nmms_215646753.gif" completionHandler:^(UIImage* image, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully downloaded.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to download.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)downloadImageFromURL:(NSString*)url
           completionHandler:(void (^)(UIImage* image, HSPError *error))completionHandler;

/**
 * @brief Get selected image from gallery.
 *
 * @param completionHandler Is called when image is selected.
 *
 * This block needs the following parameters:
 * @param image selected image object
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)getSelectedImageFromGalleryWithCompletionHandler:(void (^)(UIImage* image, HSPError* error))completionHandler;

/**
 * @brief Requests age requirement with alertView
 *
 * This block needs the following parameters:
 * @param ageRequirement
 *
 * @code
 * [HSPUtil alertViewWithAgeRequirementWithCompletionHandler:^(HSPAgeRequirement ageRequirement) {
 * 	if ( ageRequirement == HSP_KID_NOT_PERMITTED_BY_PARENTS )
 * 	{
 * 		NSLog(@"Age requirement is kid. But not permitted by parents");
 * 	}
 *	else if ( ageRequirement == HSP_KID_PERMITTED_BY_PARENTS )
 * 	{
 * 		NSLog(@"Age requirement is kid. Permitted by parents");
 * 	}
 *	else if ( ageRequirement == HSP_ADULT )
 * 	{
 * 		NSLog(@"Age requirement is adult.");
 * 	}
 *	else
 * 	{
 * 		NSLog(@"Already authenticated ad adult");
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GBGAME
 */
+ (void)alertViewWithAgeRequirementWithCompletionHandler:(void (^)(HSPAgeRequirement ageRequirement))completionHandler;

@end

#ifdef  __cplusplus
extern "C" {
#endif

#define IS_CRACKED( sec_result ) ( [[[HSPCore sharedHSPCore].serviceProperties valueFromKey:@"currentTime"] hash] != ( sec_result ) )
#define IS_CHEATED( sec_result ) ( [[[HSPCore sharedHSPCore].serviceProperties valueFromKey:@"currentTime"] hash] != ( sec_result ) )

/**
 * @brief Requests the server to inspect whether the device is jailbreak or not.
 *
 * @param isCrackedHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param secResult Whether the device is jailbreak or not.(You can check secResult using IS_CRACKED macro that is device cracked).
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * checkCrackedDeviceWithCompletionHandler( ^(NSUInteger secResult, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      if ( IS_CRACKED(isCracked_SecResult) == YES )
 *      {
 *          NSLog(@"The device is jailbreak.");
 *      } else {
 *          NSLog(@"The device is not jailbreak.");
 *      }
 * 	} else
 * 	{
 * 		NSLog(@"Failed to inspect the device.");
 * 	}
 * } );
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
typedef void(^isCrackedHandler)(NSUInteger secResult, HSPError* error);
void checkCrackedDeviceWithCompletionHandler( isCrackedHandler completionHandler );

/**
 * @brief Requests the server to inspect whether there is a cheating process.
 *
 * @param isCheatingHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param secResult Whether the device is rooted or not.(You can check secResult using IS_CHEATED macro that is device cracked).
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * checkCheatingWithCompletionHandler( ^(NSUInteger secResult, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      if ( IS_CHEATED(isCheated_SecResult) == YES )
 *      {
 *          NSLog(@"The device is cheated.");
 *      } else {
 *          NSLog(@"The device is not cheated.");
 *      }
 * 	} else
 * 	{
 * 		NSLog(@"Failed to inspect the device.");
 * 	}
 * } );
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
typedef void(^isCheatingHandler)(NSUInteger secResult, HSPError* error);
void checkCheatingWithCompletionHandler( isCheatingHandler completionHandler );

#ifdef  __cplusplus
}
#endif
