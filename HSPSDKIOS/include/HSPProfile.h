//
//  HSPProfile.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"
#import "HSPDetailedProfile.h"

/**
 * @brief Deals with user profiles.
 */
@interface HSPProfile : NSObject {
    int64_t                 _memberNo;
    NSString*               _nickname;
    BOOL                    _isValid;
    BOOL                    _exposeOnline;
    NSDate*                 _lastLoginDate;
    int32_t                 _recentPlayedGameNo;
    id                      _reservedData;
    HSPDetailedProfile*     _detailedProfile;
}

/**
 * @brief Member number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              int64_t                 memberNo;

/**
 * @brief Nickname. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSString*               nickname;

/**
 * @brief Whether the user is valid or not. (read only)<br>The user who leaves the game service is not valid anymore.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                    isValid;

/**
 * @brief Whether to show if the user is online or not. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                    exposeOnline;

/**
 * @brief Last login date. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSDate*                 lastLoginDate;

/**
 * @brief Recently played game number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              int32_t                 recentPlayedGameNo;

/**
 * @brief Detailed user profile. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      HSPDetailedProfile*     detailedProfile;

/**
 * @brief Requests a list of user profiles from the server.
 * 
 * @param memberNos List of member numbers.<br>Array of NSNumbers.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:  
 * @param profiles List of user profiles.<br>Array of HSPProfile objects.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* memberNoArray = [NSArray arrayWithObjects:[NSNumber numberWithMemberNo:4105000000131917], nil];
 *
 * [HSPProfile loadProfilesWithMemberNos:memberNoArray completionHandler:^(NSArray* profiles, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPProfile* profile in profiles ) {
 *          NSLog(@"User profile : %@", profile);
 *      }
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loadProfilesWithMemberNos:(NSArray*)memberNos 
                    completionHandler:(void (^)(NSArray* profiles, HSPError *error))completionHandler;

/**
 * @brief Requests a list of game numbers currently connected, from the server.
 *
 * @param memberNos List of member numbers.<br>Array of NSNumbers.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param gameNos List of game numbers.<br>Array of int NSNumber objects.<br>It is 0 if there is no game currently connected.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* memberNoArray = [NSArray arrayWithObjects:[NSNumber numberWithMemberNo:4105000000131917], nil];
 *
 * [HSPProfile queryCurrentGameNoWithMemberNos:memberNoArray completionHandler:^(NSDictionary* gameNoDictionary, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNoArray ) {
 *          NSLog(@"Whether the user is online : %@", [[gameNoDictionary objectForKey:memberNo] intValue] == 0? @"NO" : @"YES");
 *      }
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)	queryCurrentGameNoWithMemberNos:(NSArray*)memberNos
                      completionHandler:(void (^)(NSDictionary* gameNoDictionary, HSPError *error))completionHandler;

/**
 * @brief Returns the URL address of profile image that matches the specified member number.<br><b>This method does not work for the games using LINE authorization.<b> For the games using LINE authorization, use requestProfileImageUrlsWithMemberNo:thumbnail:completionHandler:.
 *
 * @param memberNo Member number.
 * @param isThumbnail Whether it is thumbnail or not.
 * @return URL address of the profile image.
 *
 * @code
 * NSString* url = [HSPProfile profileImageURLWithMemberNo:4105000000131917 thumbnail:YES];
 * NSLog(@"Thumbnail profile image URL : %@", url);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(NSString*)    profileImageURLWithMemberNo:(int64_t)memberNo
                                  thumbnail:(BOOL)isThumbnail;

/**
 * @brief Requests the URL address list of the specified users' profile images from the server. 
 *
 * @param memberNos List of member numbers whose profile images are to be requested. 
 * @param isThumbnail Whether it is thumbnail or not.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param imageUrls URL list of user profile images.<br>Array of NSString objects.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* memberNoArray = [NSArray arrayWithObjects:[NSNumber numberWithMemberNo:4105000000131917], nil];
 *
 * [HSPProfile requestProfileImageUrlsWithMemberNo:memberNoArray completionHandler:^(NSArray* imageUrls, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( register unsigned int index = 0; index < [memberNos count]; index++ ) {
 *          NSLog(@"User(%@) profile image URL : %@", [memberNos objectAtIndex:index], [imageUrls objectAtIndex:index]);
 *      }
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)    requestProfileImageUrlsWithMemberNo:(NSArray*)memberNos
                                     thumbnail:(BOOL)isThumbnail
                             completionHandler:(void (^)(NSArray* imageUrls, HSPError *error))completionHandler;

/**
 * @brief Requests profile images from the server.
 * 
 * @param isThumbnail Whether it is thumbnail or not.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param image Image.
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [profile downloadProfileImageWithThumbnail:NO completionHandler:^(UIImage* image, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Images successfully downloaded.");
 * 	} else
 * 	{
 * 		NSLog(@"Filed to download images.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)     downloadProfileImageWithThumbnail:(BOOL)isThumbnail
                            completionHandler:(void (^)(UIImage* image, HSPError *error))completionHandler;

/**
 * @brief Requests the detailed user profile from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param detailedProfile Detailed user profile.<br>HSPDetailedProfile object.  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [profile loadDetailedProfileWithCompletionHandler:^(HSPDetailedProfile* detailedProfile, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Detailed user profiles successfully requested.");
 * 		NSLog(@"Detailed user profile: %@", detailedProfile);
 * 	} else
 * 	{
 * 		NSLog(@"Failed to request detailed user profiles.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)     loadDetailedProfileWithCompletionHandler:(void (^)(HSPDetailedProfile* detailedProfile, HSPError *error))completionHandler;

/**
 * @brief Returns whether the number is the same as the user's phone number.
 * 
 * @param phoneNumber Phone number to compare.
 * @return Whether it is the same (YES: it is the same, NO: it is not the same as the user's phone number or there is no phone number added).
 *
 * @code
 * BOOL has = [profile hasPhoneNo:@"01024242424"];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(BOOL)         hasPhoneNo:(NSString*)phoneNumber;

/**
 * @brief Returns the URL address of profile image.
 * 
 * @param isThumbnail Whether it is thumbnail or not.
 * @return URL. URL address of profile image.
 *
 * @code
 * NSString* url = [profile profileImageURLWithThumbnail:YES];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(NSString*)    profileImageURLWithThumbnail:(BOOL)isThumbnail;

@end
