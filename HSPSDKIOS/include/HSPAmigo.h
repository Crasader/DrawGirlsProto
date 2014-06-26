//
//  HSPAmigo.h
//  HSPInterface
//
//  Created by Seungwoo on 14. 1. 27..
//
//

#import <Foundation/Foundation.h>
#import "HSPProfile.h"

typedef enum
{
	HSP_AMIGO_PROPOSED_FROM_ME,	/** query type for from me list */
	HSP_AMIGO_PROPOSED_TO_ME	/** query type for to me list */
} HSPAmigoProposedType;

typedef enum
{
	HSP_AMIGO_FOLLOWER,			/** query type for follower list */
	HSP_AMIGO_FOLLOWEE,			/** query type for followee list */
	HSP_AMIGO_MUTUAL_FOLLOW		/** query type for mutual following list */
} HSPAmigoFollowingType;

typedef enum
{
	HSP_AMIGO_SOCIAL_POINT_ADD,		/** add social point */
	HSP_AMIGO_SOCIAL_POINT_UPDATE	/** update social point*/
} HSPAmigoSocialPointType;

typedef enum
{
	HSP_AMIGO_RELATION_MATE,				/** HSPAmigo RelationCode for AMIGO_MATE */
	HSP_AMIGO_RELATION_MATE_PROPOSED,		/** HSPAmigo RelationCode for MATE_PROPOSED */
	HSP_AMIGO_RELATION_MATE_PROPOSAL_BLOCK,	/** HSPAmigo RelationCode for MATE_PROPOSAL_BLOCK */
	HSP_AMIGO_RELATION_MATE_BLOCK,			/** HSPAmigo RelationCode for MATE_BLOCK */
	HSP_AMIGO_RELATION_NONE					/** HSPAmigo RelationCode for NONE */
} HSPAmigoRelationCode;

typedef enum
{
	HSP_AMIGO_RELATION_RESULT_SUCCESS,				/** HSPAmigo RelationResult for SUCCESS */
	HSP_AMIGO_RELATION_RESULT_SYSTEM_ERROR,			/** HSPAmigo RelationResult for SYSTEM_ERROR */
	HSP_AMIGO_RELATION_RESULT_ALREADY_ACCEPTED,		/** HSPAmigo RelationResult for ALREADY_ACCEPTED */
	HSP_AMIGO_RELATION_RESULT_ALREADY_REQUESTED,	/** HSPAmigo RelationResult for ALREADY_REQUESTED */
	HSP_AMIGO_RELATION_RESULT_ALREADY_BLOCKED,		/** HSPAmigo RelationResult for ALREADY_BLOCKED */
	HSP_AMIGO_RELATION_RESULT_ALREADY_MATE,			/** HSPAmigo RelationResult for ALREADY_MATE */
	HSP_AMIGO_RELATION_RESULT_NO_MATE,				/** HSPAmigo RelationResult for NO_MATE */
} HSPAmigoRelationResultType;

/**
 * @brief Amigo Relation Info.
 */
@interface HSPAmigoRelationInfo : NSObject
{
	NSString*				_groupId;
	int64_t					_memberNo;
	int64_t					_opponentMemberNo;
	HSPAmigoRelationCode	_relationCode;
	int64_t					_socialPoint;
	NSString*				_extraData;
	NSString*				_regDate;
}

/**
 * @brief groupId of Mate(gameNo) (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, retain, readonly)	NSString*				groupId;

/**
 * @brief memberNo (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, assign, readonly)	int64_t					memberNo;

/**
 * @brief opponent's memberNo (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, assign, readonly)	int64_t					opponentMemberNo;

/**
 * @brief RelationCode value between the opponent and I (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, assign, readonly)	HSPAmigoRelationCode	relationCode;

/**
 * @brief Mate's socialPoint value (intimacy) (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, assign, readonly)	int64_t					socialPoint;

/**
 * @brief extra data (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, retain, readonly)	NSString*				extraData;

/**
 * @brief registration date (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, retain, readonly)	NSString*				regDate;

@end

/**
 * @brief When working with a friend about the return value.
 * Rather than a search add to friends, acceptance, and rejection, it is the return value for the operation.
 */
@interface HSPAmigoRelationResult : NSObject
{
	int64_t						_memberNo;
	int64_t						_opponentMemberNo;
	HSPAmigoRelationResultType	_result;
}

/**
 * @brief memberNo (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, assign, readonly)	int64_t						memberNo;

/**
 * @brief opponent's memberNo (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, assign, readonly)	int64_t						opponentMemberNo;

/**
 * @brief Result value (read only)
 * @serviceDomain ALL
 */
@property (nonatomic, assign, readonly)	HSPAmigoRelationResultType	result;

@end

@interface HSPAmigo : NSObject

/**
 * @brief Bidirectional friend request.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo proposeMateWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)proposeMateWithMemberNos:(NSArray*)memberNos
			   completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Bidirectional friend acceptance.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo acceptMateWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)acceptMateWithMemberNos:(NSArray*)memberNos
			  completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Bidirectional friend refusal.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo rejectMateWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)rejectMateWithMemberNos:(NSArray*)memberNos
			  completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Bidirectional friend dismiss request.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo dismissMateWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)dismissMateWithMemberNos:(NSArray*)memberNos
			   completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Bidirectional friend request cancel.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo deleteMateProposalWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)deleteMateProposalWithMemberNos:(NSArray*)memberNos
					  completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Bidirectional block friend request.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo blockMateWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)blockMateWithMemberNos:(NSArray*)memberNos
			 completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Requests the number of proposed mate count that from me or to me.<br>
 *
 * @param amigoProposedType Whether proposed type that from me or to me
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param count the number of proposed count that from me or to me.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryProposedMatesCountWithAmigoProposedType:HSP_AMIGO_PROPOSED_TO_ME completionHandler:^(int64_t count, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			NSLog(@"ProposedMatesCount : %lld", count);
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryProposedMatesCountWithAmigoProposedType:(HSPAmigoProposedType)amigoProposedType
								   completionHandler:(void (^)(int64_t count, HSPError* error))completionHandler;

/**
 * @brief Requests the list of proposed mate that from me or to me.<br>
 *
 * @param amigoProposedType Whether proposed type that from me or to me
 * @param index offset
 * @param count the number of list
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param memberNos The list of memberNo that proposed from me or to me.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryProposedMatesWithAmigoProposedType:HSP_AMIGO_PROPOSED_TO_ME
 *												index:0
 *												count:10
 *									completionHandler:^(NSArray *memberNos, HSPError *error) {
 *										if ( [error isSuccess] == YES )
 *										{
 *											for ( NSNumber* memberNo in memberNos )
 *											{
 *												NSLog(@"memberNo : %lld", [memberNo longLongValue]);
 *											}
 *										}
 *										else
 *										{
 *											NSLog(@"ERROR : %@", error);
 *										}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryProposedMatesWithAmigoProposedType:(HSPAmigoProposedType)amigoProposedType
										  index:(int64_t)index
										  count:(int64_t)count
							  completionHandler:(void (^)(NSArray* memberNos, HSPError* error))completionHandler;

/**
 * @brief Requests the number of Amigo mate count.<br>
 *
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param count The number of Amigo Mate.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryAmigoMatesCountWithCompletionHandler:^(int64_t count, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			NSLog(@"Mates Member Count : %lld", count);
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryAmigoMatesCountWithCompletionHandler:(void (^)(int64_t count, HSPError* error))completionHandler;

/**
 * @brief Requests the list of Amigo mate.<br>
 *
 * @param index offset
 * @param count the number of list
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param memberNos The list of Amigo Mate member's memberNo.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryAmigoMatesWithIndex:0
 *								count:10
 *					completionHandler:^(NSArray *memberNos, HSPError *error) {
 *						if ( [error isSuccess] == YES )
 *						{
 *							for ( NSNumber* memberNo in memberNos )
 *							{
 *								NSLog(@"memberNo : %lld", [memberNo longLongValue]);
 *							}
 *						}
 *						else
 *						{
 *							NSLog(@"ERROR : %@", error);
 *						}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryAmigoMatesWithIndex:(int64_t)index
						   count:(int64_t)count
			   completionHandler:(void (^)(NSArray* memberNos, HSPError* error))completionHandler;

/**
 * @brief Requests the number of Blocking members count.<br>
 *
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param count The number of Blocking Members.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryAmigoBlockingMembersCountWithCompletionHandler:^(int64_t count, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			NSLog(@"Blocking Members count : %lld", count);
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryAmigoBlockingMembersCountWithCompletionHandler:(void (^)(int64_t count, HSPError* error))completionHandler;

/**
 * @brief Requests the list of Blocking Members.<br>
 *
 * @param index offset
 * @param count the number of list
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param memberNos The list of Blocking member's memberNo.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryAmigoBlockingMembersWithIndex:0
 *										  count:10
 *							  completionHandler:^(NSArray *memberNos, HSPError *error) {
 *									if ( [error isSuccess] == YES )
 *									{
 *										for ( NSNumber* memberNo in memberNos )
 *										{
 *											NSLog(@"memberNo : %lld", [memberNo longLongValue]);
 *										}
 *									}
 *									else
 *									{
 *										NSLog(@"ERROR : %@", error);
 *									}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryAmigoBlockingMembersWithIndex:(int64_t)index
									 count:(int64_t)count
						 completionHandler:(void (^)(NSArray* memberNos, HSPError* error))completionHandler;

/**
 * @brief Requests the relation.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationInfos The list of HSPAmigoRelationInfo object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo queryRelationsWithMemberNos:memberNos completionHandler:^(NSArray* relationInfos, HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPAmigoRelationInfo* relationInfo in relationInfos )
 *		{
 *			NSLog(@"Relation Info : %@", relationInfo);
 *		}
 * 	}
 *	else
 * 	{
 * 		NSLog(@"ERROR : %@", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryRelationsWithMemberNos:(NSArray*)memberNos
				  completionHandler:(void (^)(NSArray* relationInfos, HSPError* error))completionHandler;

/**
 * @brief Request to save Social Point.<br>
 *
 * @param amigoSocialPointType Whether the social point add or update
 * @param targetMemberNo The target memberNo
 * @param socialPoint Social Point
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo requestSetSocialPointWithAmigoSocialPointType:HSP_AMIGO_SOCIAL_POINT_ADD
 *											targetMemberNo:targetMemberNo
 *												socialPoint:socialPoint
 *										completionHandler:^(HSPError *error) {
 *											if ( [error isSuccess] == YES )
 *											{
 *												NSLog(@"Successfully set social point.");
 *											}
 *											else
 *											{
 *												NSLog(@"ERROR : %@", error);
 *											}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)requestSetSocialPointWithAmigoSocialPointType:(HSPAmigoSocialPointType)amigoSocialPointType
									   targetMemberNo:(int64_t)targetMemberNo
										  socialPoint:(int64_t)socialPoint
									completionHandler:(void (^)(HSPError* error))completionHandler;

/**
 * @brief Request to query Social Point.<br>
 *
 * @param targetMemberNo The target memberNo
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param socialPoint targetMemberNo's social point.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo querySocialPointWithTargetMemberNo:targetMemberNo completionHandler:^(int64_t socialPoint, HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"TargetMemberNo's social point : %lld", socialPoint);
 * 	} 
 *	else
 * 	{
 * 		NSLog(@"ERROR : %@", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)querySocialPointWithTargetMemberNo:(int64_t)targetMemberNo
						 completionHandler:(void (^)(int64_t socialPoint, HSPError* error))completionHandler;

/**
 * @brief Request to save Extra Data.<br>
 *
 * @param targetMemberNo The target memberNo
 * @param extraData The Extra Data which will be saved.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo requestSetExtraDataWithTargetMemberNo:targetMemberNo
 *										 extraData:extraData
 *								 completionHandler:^(HSPError *error) {
 *									if ( [error isSuccess] == YES )
 *									{
 *										NSLog(@"Successfully set extra data.");
 *									}
 *									else
 *									{
 *										NSLog(@"ERROR : %@", error);
 *									}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)requestSetExtraDataWithTargetMemberNo:(int64_t)targetMemberNo
									extraData:(NSString*)extraData
							completionHandler:(void (^)(HSPError* error))completionHandler;

/**
 * @brief Request to query Extra Data.<br>
 *
 * @param targetMemberNo The target memberNo
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param socialPoint targetMemberNo's social point.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo queryExtraDataWithTargetMemberNo:targetMemberNo completionHandler:^(NSString* extraData, HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"TargetMemberNo's extra data : %@", extraData);
 * 	}
 *	else
 * 	{
 * 		NSLog(@"ERROR : %@", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryExtraDataWithTargetMemberNo:(int64_t)targetMemberNo
					   completionHandler:(void (^)(NSString* extraData, HSPError* error))completionHandler;

/**
 * @brief Requests the relation.<br>
 * All Infomation(Includes social point, extra data).<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationInfos The list of HSPAmigoRelationInfo object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo queryRelationsAllWithMemberNos:memberNos completionHandler:^(NSArray* relationInfos, HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPAmigoRelationInfo* relationInfo in relationInfos )
 *		{
 *			NSLog(@"RelationInfo : %@", relationInfo);
 *		}
 * 	}
 *	else
 * 	{
 * 		NSLog(@"ERROR : %@", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryRelationsAllWithMemberNos:(NSArray*)memberNos
					 completionHandler:(void (^)(NSArray* relationInfos, HSPError* error))completionHandler;

/**
 * @brief One-way friend request.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo requestSetFollowersWithMemberNos:memberNos completionHandler:^(NSArray* relationResults, HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPAmigoRelationResult* result in relationResults )
 *		{
 *			NSLog(@"Result : %@", result);
 *		}
 * 	} 
 *	else
 * 	{
 * 		NSLog(@"ERROR : %@", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)requestSetFollowersWithMemberNos:(NSArray*)memberNos
					   completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief One-way friend dismiss request.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo dismissFollowersWithMemberNos:memberNos completionHandler:^(NSArray* relationResults, HSPError* error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPAmigoRelationResult* result in relationResults )
 *		{
 *			NSLog(@"Result : %@", result);
 *		}
 * 	}
 *	else
 * 	{
 * 		NSLog(@"ERROR : %@", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)dismissFollowersWithMemberNos:(NSArray*)memberNos
					completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Requests the number of one-way friends count that from me or to me.<br>
 *
 * @param amigoFollowingType AmigoFollowingType that from me, to me or mutually friend.
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param count The number of one-way friends count that requested
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryFollowingCountWithFollowingType:HSP_AMIGO_FOLLOWER completionHandler:^(int64_t count, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			NSLog(@"Following Count : %lld", count);
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryFollowingCountWithFollowingType:(HSPAmigoFollowingType)amigoFollowingType
						   completionHandler:(void (^)(int64_t count, HSPError* error))completionHandler;

/**
 * @brief Requests the list of one-way friends that from me or to me.<br>
 *
 * @param amigoFollowingType AmigoFollowingType that from me, to me or mutually friend.
 * @param index offset.
 * @param count The number of list.
 * @param completionHandler Is called when a response to the request that received from the server.
 *
 * This block needs the following parameter:
 * @param memberNos The list of Following / Followee / Mutual Following member's memberNo.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo queryProposedMatesWithAmigoProposedType:HSP_AMIGO_PROPOSED_TO_ME
 *												index:0
 *												count:10
 *									completionHandler:^(NSArray *memberNos, HSPError *error) {
 *										if ( [error isSuccess] == YES )
 *										{
 *											for ( NSNumber* memberNo in memberNos )
 *											{
 *												NSLog(@"memberNo : %lld", [memberNo longLongValue]);
 *											}
 *										}
 *										else
 *										{
 *											NSLog(@"ERROR : %@", error);
 *										}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryFollowingListWithFollowingType:(HSPAmigoFollowingType)amigoFollowingType
									  index:(int64_t)index
									  count:(int64_t)count
						  completionHandler:(void (^)(NSArray* memberNos, HSPError* error))completionHandler;

/**
 * @brief Request the maximum number of friends to be registered.<br>
 *
 * @param completionHandler Is called when a response to the request that attempts to log in to Facebook is received from the server.
 *
 * This block needs the following parameter:
 * @param limitCount The maximum number of friends to be registered.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo queryMatesLimitCountWithCompletionHandler:^(int64_t limitCount, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The maximum number of friends to be registered : %lld", limitCount);
 * 	} 
 *	else
 * 	{
 * 		NSLog(@"Failed to log in to Facebook.(%@)", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)queryMatesLimitCountWithCompletionHandler:(void (^)(int64_t limitCount, HSPError* error))completionHandler;

/**
 * @brief Block the request of a bidirectional.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo blockProposalMatesWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)blockProposalMatesWithMemberNos:(NSArray*)memberNos
					  completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief Request to unblock a mate that blocked.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response that received from the server.
 *
 * This block needs the following parameter:
 * @param relationResults The list of HSPAmigoRelationResult object
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 *	[HSPAmigo cancelBlockMateWithMemberNos:memberNos completionHandler:^(NSArray *relationResults, HSPError *error) {
 *		if ( [error isSuccess] == YES )
 *		{
 *			for ( HSPAmigoRelationResult* result in relationResults )
 *			{
 *				NSLog(@"Result = %@", result);
 *			}
 *		}
 *		else
 *		{
 *			NSLog(@"ERROR : %@", error);
 *		}
 *	}];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)cancelBlockMateWithMemberNos:(NSArray*)memberNos
				   completionHandler:(void (^)(NSArray* relationResults, HSPError* error))completionHandler;

/**
 * @brief requests the auto-register in the HSP Amigo bidirectional friend IDP friend.<br>
 *
 * @param memberNos The list of memberNo which is NSNumber type.
 * @param completionHandler Is called when a response to the request that attempts to log in to Facebook is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAmigo requestAutoAddMateWithMemberNos:memberNos completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully auto add Amigo Mate");
 * 	} 
 *	else
 * 	{
 * 		NSLog(@"ERROR : %@", error);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+ (void)requestAutoAddMateWithMemberNos:(NSArray*)memberNos
					  completionHandler:(void (^)(HSPError* error))completionHandler;

@end
