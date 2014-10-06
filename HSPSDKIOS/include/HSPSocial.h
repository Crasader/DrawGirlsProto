//
//  HSPSocial.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"
#import "HSPProfile.h"

typedef enum
{
	FRIEND_IDP_FACEBOOK,
	FRIEND_IDP_GOOGLEPLUS,
	FRIEND_IDP_APPLE_GAMECENTER
} GlobalIdp;

@interface SocialFriendProfile : NSObject
{
	NSString*	_socialId;
	NSString*	_socialNickname;
	NSString*	_socialPhotoUrl;
}

@property (nonatomic, retain, readonly)	NSString*	socialId;
@property (nonatomic, retain, readonly)	NSString*	socialNickname;
@property (nonatomic, retain, readonly)	NSString*	socialPhotoUrl;

@end

/**
 * @brief Deals with my relationship with other users.
 */
@interface HSPSocial : NSObject {
}

/**
 * @brief Requests the server to follow the specified users.
 * 
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSArray* memberNoArray = [NSArray arrayWithObjects:[NSNumber numberWithMemberNo:4105000000134579], nil];
 * [HSPSocial followMembersWithMemberNos:memberNoArray completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
+ (void)followMembersWithMemberNos:(NSArray*)memberNos
				 completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests the server to block the specified users.
 * 
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSArray* memberNoArray = [NSArray arrayWithObjects:[NSNumber numberWithMemberNo:4105000000134579], nil];
 * [HSPSocial blockMembersWithMemberNos:memberNoArray completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
+ (void)blockMembersWithMemberNos:(NSArray*)memberNos
				completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests the server to unfollow the specified users.
 * 
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.  
 *
 * @code
 * NSArray* memberNoArray = [NSArray arrayWithObjects:[NSNumber numberWithMemberNo:4105000000134579], nil];
 * [HSPSocial unfollowMembersWithMemberNos:memberNoArray completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
+ (void)unfollowMembersWithMemberNos:(NSArray*)memberNos
				   completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to unblock the specified users.
 * 
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * NSArray* memberNoArray = [NSArray arrayWithObjects:[NSNumber numberWithMemberNo:4105000000134579], nil];
 * [HSPSocial unblockMembersWithMemberNos:memberNoArray completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
+ (void)unblockMembersWithMemberNos:(NSArray*)memberNos
				  completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the number of users I followed from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param errormemberCount The number of users.
 * @param error Error.<br>If successful, the error code is 0.   
 *
 * @code
 * [HSPSocial queryFollowingMemberCountWithCompletionHandler:^(int32_t memberCount, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Number of users : %d", memberCount);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryFollowingMemberCountWithCompletionHandler:(void (^)(int32_t memberCount, HSPError *error))completionHandler;

/**
 * @brief Requests the number of users I blocked from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param errormemberCount The number of users.
 * @param error 에러.<br>If successful, the error code is 0.  
 *
 * @code
 * [HSPSocial queryBlockingMemberCountWithCompletionHandler:^(int32_t memberCount, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Number of users : %d", memberCount);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */

+(void)     queryBlockingMemberCountWithCompletionHandler:(void (^)(int32_t memberCount, HSPError *error))completionHandler;

/**
 * @brief Requests a list of users I followed from the server.
 * 
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPSocial queryFollowingMembersFromIndex:0 count:10 completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryFollowingMembersFromIndex:(int32_t)index
                                     count:(int32_t)count
                         completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests a list of users I blocked from the server.
 * 
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.   
 *
 * @code
 * [HSPSocial queryBlockingMembersFromIndex:0 count:10 completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)queryBlockingMembersFromIndex:(int32_t)index
                                   count:(int32_t)count
                       completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests a list of users who followed me from the server.
 * 
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [HSPSocial queryFollowersFromIndex:11 count:10 completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryFollowersFromIndex:(int32_t)index
                              count:(int32_t)count
                  completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests the recommended users who played the specified game from the server.
 * 
 * @param gameNo Game number.
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.   
 *
 * @code
 * [HSPSocial queryMembersRecommendedWithGameNo:101 index:0 count:10 completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryMembersRecommendedWithGameNo:(int32_t)gameNo
                                        index:(int32_t)index
                                        count:(int32_t)count
                            completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests a list of users whose nickname contains the entered nickname string from the server. 
 * 
 * @param nickname Nickname.
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.  
 *
 * @code
 * [HSPSocial queryMembersWithNickname:@"player" index:0 count:10 completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryMembersWithNickname:(NSString*)nickname
                               index:(int32_t)index
                               count:(int32_t)count
                   completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests a list of users who played the specified game from the server.
 * 
 * @param gameNo Game number.
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPSocial queryMembersPlayedGameWithGameNo:101 index:0 count:10 completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryMembersPlayedGameWithGameNo:(int32_t)gameNo
                                       index:(int32_t)index
                                       count:(int32_t)count
                           completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests a list of following users who played the specified game from the server.
 * 
 * @param gameNo Game number.
 * @param index Start index.<br>It starts from 0.
 * @param count Count. If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param error Error.<br>If successful, the error code is 0.   
 *
 * @code
 * [HSPSocial queryFollowingMembersPlayedGameWithGameNo:101 index:0 count:10 completionHandler:^(NSArray* memberNos, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryFollowingMembersPlayedGameWithGameNo:(int32_t)gameNo
                                                index:(int32_t)index
                                                count:(int32_t)count
                                    completionHandler:(void (^)(NSArray* memberNos, HSPError *error))completionHandler;

/**
 * @brief Requests a list of external service IDs of users I followed from the server.
 * 
 * @param idpCode External service code.
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNos List of member numbers.<br>Array of NSNumber objects.
 * @param idpIDs List of external service IDs.<br>Array of NSString objects. 
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPSocial queryFollowingMemberIDsWithIdpCode:HSP_IDP_FACEBOOK index:0 count:10 completionHandler:^(NSArray* memberNos, NSArray* idpIDs, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( NSNumber* memberNo in memberNos )
 * 		{
 * 			NSLog(@"Member number : %@", memberNo);
 * 		}
 * 		for ( NSString* idpID in idpIDs )
 * 		{
 * 			NSLog(@"ID : %@", idpID);
 * 		} 
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryFollowingMemberIDsWithIdpCode:(HSPIDPCode)idpCode
                                         index:(int32_t)index
                                         count:(int32_t)count
                             completionHandler:(void (^)(NSArray* memberNos, NSArray* idpIDs, HSPError *error))completionHandler;

/**
 * @brief Compares phone numbers (stored in the server) of the member number list with the phone number list entered, and returns the map of member number and phone number if they match.
 *
 * @param phoneNos List of phone numbers to compare with those stored in the server.<br>Array of NSString objects. 
 * @param memberNos List of member numbers.<br>Compared the phone numbers in this list with phoneNos.<br>Array of NSNumber objects.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param memberNoMap Dictionary of phone numbers as keys and member numbers as values.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryMembersWithPhoneNos:(NSArray*)phoneNos
                           memberNos:(NSArray*)memberNos
                   completionHandler:(void (^)(NSDictionary* memberNoMap, HSPError *error))completionHandler;

/**
 * @brief Requests the server to send a message recommending the current game to the specified users (used in Korea). 
 * 
 * @param senderPhoneNo Phone number of sender.
 * @param receiverPhoneNo Phone number of receiver.
 * @param receiverName Name of receiver.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPSocial sendRecommendingSMSWithSenderPhoneNo:@"01077776666" receiverPhoneNo:@"01066667777" receiverName:@"KIMCheolsoo" completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully sent an SMS to recommend a game.");
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
+(void)     sendRecommendingSMSWithSenderPhoneNo:(NSString*)senderPhoneNo
                                 receiverPhoneNo:(NSString*)receiverPhoneNo
                                    receiverName:(NSString*)receiverName
                               completionHandler:(void (^)(HSPError *error))completionHandler;


/**
 * @brief Requests the server to send an email recommending the current game to the specified users (used in Japan).
 * 
 * @param senderEmail Email address of sender.
 * @param receiverEmail Email address of receiver.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPSocial sendRecommendingEmailWithSenderEmail:@"testID@naver.com" receiverEmail:@"come1@naver.com" completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully sent an email to recommend a game.");
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
+(void)     sendRecommendingEmailWithSenderEmail:(NSString*)senderEmail
                                   receiverEmail:(NSString*)receiverEmail
                               completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests a list of HSP Member numbers of IDP(ID Provider) users from the server. <br>
 * You can requests IDP IDs of up to 500 users; for IDP ID of a user who does not authenticated to HSP, the callback returns 0.
 * @param idpIds List of IDP IDs.
 * @param completionHandler Is called when a response to the request is received from the server.
 * @return void
 *
 * This block needs the following parameters:
 * @param memberNoMap MemberNoMap key is IDP ID, value is HSP member number(if exist or 0). Nil if an error occurs.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPSocial queryHSPMemberNosWithIdpIds:[NSArray arrayWithObject:idpId] completionHandler:^(NSDictionary* memberNoMap, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *		{
 *			for ( NSString* idpId in memberNoMap.allKeys )
 *			{
 *				NSNumber* memberNo = [memberNoMap objectForKey:idpId];
 *				NSLog(@"HSPMemberNo : %lld with IDP ID : %@", [memberNo longLongValue], idpId);
 *			}
 *		}
 *      else
 *          NSLog(@"Failed to query hspmemberNo with IDP ID(%@)", error);
 * }];
 * @endcode
 * @serviceDomain GREEGAME LINEGAME KAKAOGAME
 */
+ (void)queryHSPMemberNosWithIdpIds:(NSArray*)idpIds
				  completionHandler:(void (^)(NSDictionary* memberNoMap, HSPError* error))completionHandler;

/**
 * @brief Requests a list of IDP IDs of HSP users from the server.
 * You can requests IDP IDs of up to 500 users; for member number of a user who does not authenticated to IDP, the callback returns blank (@"").
 * @param memberNos List of member numbers to request<br>Array of NSNumber instances.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * @code
 * [HSPSocial queryIdpIdsWithMemberNos:memberNos
 *	    	    	completionHandler:^(NSDictionary* idpIdDictionary, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *      {
 *          NSLog(@"Successfully received IDP IDs");
 *          for ( NSNumber* memberNo in memberNos )
 *          {
 *              NSString* idpId = [idpIdDictionary objectForKey:memberNo];
 *              if ( idpId != nil && [idpId length] != 0 )
 *              {
 *                  NSLog(@"memberNo : %@, IDP ID : %@", memberNo, idpId);
 *              }
 *              else
 *              {
 *                  NSLog(@"%@ is not an IDP user.", memberNo);
 *              }
 *          }
 *      }
 *      else
 *      {
 *          NSLog(@"Failed to receive IDP IDs.");
 *      }
 * }];
 * @endcode
 * @serviceDomain GREEGAME LINEGAME KAKAOGAME
 */
+ (void)queryIdpIdsWithMemberNos:(NSArray*)memberNos
			   completionHandler:(void(^)(NSDictionary* idpIdDictionary, HSPError* error))completionHandler;

/**
 * @brief Requests a list of IDP IDs of HSP users from the server.
 * You can requests IDP IDs of up to 500 users; for member number of a user who does not authenticated to IDP, the callback returns blank (@"").
 * @param memberNos List of member numbers to request<br>Array of NSNumber instances.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * @code
 * [HSPSocial queryIdpIdsWithMemberNos:memberNos
 *	    	    	completionHandler:^(NSDictionary* idpIdDictionary, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *      {
 *          NSLog(@"Successfully received IDP IDs");
 *          for ( NSNumber* memberNo in memberNos )
 *          {
 *              NSString* idpId = [idpIdDictionary objectForKey:memberNo];
 *              if ( idpId != nil && [idpId length] != 0 )
 *              {
 *                  NSLog(@"memberNo : %@, IDP ID : %@", memberNo, idpId);
 *              }
 *              else
 *              {
 *                  NSLog(@"%@ is not an IDP user.", memberNo);
 *              }
 *          }
 *      }
 *      else
 *      {
 *          NSLog(@"Failed to receive IDP IDs.");
 *      }
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)loadFriendsWithIdp:(GlobalIdp)globalIdp
					 index:(int32_t)index
					 count:(int32_t)count
		 completionHandler:(void (^)(NSArray* friendProfileList, HSPError* error))completionHandler;

/**
 * @brief Requests a list of HSP Member numbers of IDP(ID Provider) users from the server. <br>
 * You can requests IDP IDs of up to 500 users; for IDP ID of a user who does not authenticated to HSP, the callback returns 0.
 * @param globalIds List of globalIdp IDs.
 * @param completionHandler Is called when a response to the request is received from the server.
 * @return void
 *
 * This block needs the following parameters:
 * @param memberNos list of HSP memberNo
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPSocial queryHSPMemberNosWithGlobalIds:[NSArray arrayWithObject:idpId] completionHandler:^(NSDictionary* memberNoMap, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *		{
 *			for ( NSString* idpId in memberNoMap.allKeys )
 *			{
 *				NSLog(@"hsp memberNo : %ld", [[memberNoMap objectForKey:idpId] longLongValue]);
 *			}
 *		}
 *      else
 *          NSLog(@"Failed to query hspmemberNo with idpId(%@)", error);
 * }];
 * @endcode
 * @serviceDomain TOASTGAME
 */
+ (void)queryHSPMemberNosWithGlobalIds:(NSArray*)globalIds
					 completionHandler:(void (^)(NSDictionary* memberNoMap, HSPError* error))completionHandler;

@end

/**
 * @brief [Notifications] Is posted when a user is followed.<br>
 * NSArray object is received; it is an array of NSNumber objects containing followed member numbers.
 */
extern NSString* const HSPDidAddFollowingNotificationName;

/**
 * @brief [Notifications] Is posted when a user is unfollowed.<br>
 * NSArray object is received; it is an array of NSNumber objects containing unfollowed member numbers. 
 */
extern NSString* const HSPDidRemoveFollowingNotificationName;

/**
 * @brief [Notifications] Is posted when a user is blocked.<br>
 * NSArray object is received; it is an array of NSNumber objects containing blocked member numbers.
 */
extern NSString* const HSPDidAddBlockingNotificationName;

/**
 * @brief [Notifications] Is posted when a user is unblocked.<br>
 * NSArray object is received; it is an array of NSNumber objects containing unblocked member numbers.
 */
extern NSString* const HSPDidRemoveBlockingNotificationName;

