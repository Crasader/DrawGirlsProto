//
//  HSPRanking.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 24..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPScore.h"

@interface NSNumber (HSPRankingCategory)

/**
 * @brief Creates an NSNumber object including a ranking factor.
 * @param value Ranking factor.
 * @return NSNumber type ranking factor.
 */
+(NSNumber*)numberWithFactor:(int32_t)value;

/**
 * @brief Creates an NSNumber objecting including ranking period.
 * @param value Ranking period.
 * @return NSNumber type ranking period.
 */
+(NSNumber*)numberWithPeriod:(int8_t)value;

/**
 * @brief Returns a ranking factor.
 * @return Ranking factor.
 */
-(int32_t)factorValue;

/**
 * @brief Returns a ranking period.
 * @return Ranking period.
 */
-(int8_t)periodValue;

@end

/**
 * @brief enum for ranking period.
 */
typedef enum
{
    HSP_RANKINGPERIOD_DAILY = 0,   /**< Daily. */
    HSP_RANKINGPERIOD_WEEKLY,      /**< Weekly. */
    HSP_RANKINGPERIOD_MONTHLY,     /**< Monthly. */
    HSP_RANKINGPERIOD_TOTAL        /**< Total. */
} HSPRankingPeriod;

/**
 * @brief enum for ranking scope.
 */
typedef enum
{
    HSP_RANKINGSCOPE_ALL = 0,    /**< Scope for all. */
    HSP_RANKINGSCOPE_FRIEND,      /**< Scope for friends. */
    HSP_RANKINGSCOPE_GAMEMATE    /**< Scope for game mates. */
} HSPRankingScope;

/**
 * @brief Ranking key interface.<br>
 * This interface is used to query score information.
 */
@interface HSPRankingKey : NSObject {
    int32_t             _factor;
    HSPRankingPeriod    _period;
}

/**
 * @brief Ranking factor.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic)        int32_t          factor;

/**
 * @brief Ranking period.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic)        HSPRankingPeriod  period;

/**
 * @brief Creates a ranking key object for which factor and period are set.
 *
 * @param factor Ranking factor.
 * @param period Ranking period.<br>HSPRankingPeriod type value.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(HSPRankingKey*) rankingKeyWithFactor:(int32_t)factor period:(HSPRankingPeriod)period;

@end

/**
 * @brief Deals with ranking information.
 */
@interface HSPRanking : NSObject {
    int32_t         _factor;
    NSString*       _name;
    NSArray*        _periods;
    NSString*       _unit;
    NSDictionary*   _resetDate;
    
    // private
    int32_t         _gameNo;
}

/**
 * @brief Ranking factor. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int32_t     factor;

/**
 * @brief Rank name. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   name;

/**
 * @brief List of ranking periods.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSArray*    periods;

/**
 * @brief Ranking unit. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   unit;

/**
 * @brief Date to reset the next ranking. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly, getter=resetDate)     NSDate*   resetDate;

/**
 * @brief Requests ranking information list of the specified game from the server.
 *
 * @param gameNo Game number.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param rankings An array of ranking information.<br>Array of HSPRanking objects.
 * @param repFactor Representative ranking factor.
 * @param repPeriod Representative ranking period.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPRanking loadRankingsWithGameNo:101 completionHandler:^(NSArray* rankings, int32_t repFactor, HSPRankingPeriod repPeriod, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Representative ranking factor : %d", repFactor);
 *      for ( HSPRanking* ranking in rankings )
 * 		{
 *          NSLog(@"Ranking information : %@", ranking);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loadRankingsWithGameNo:(int32_t)gameNo
                 completionHandler:(void (^)(NSArray* rankings, int32_t repFactor, HSPRankingPeriod repPeriod, HSPError *error))completionHandler;

/**
 * @brief Requests ranking of the specified game from the server.
 *
 * @param gameNo Game number.
 * @param factor Ranking factor.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 *
 * @param ranking Ranking information.<br>HSPRanking object.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPRanking loadRankingWithGameNo:101 factor: 1 completionHandler:^(HSPRanking* ranking, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Ranking information: %@", ranking);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loadRankingWithGameNo:(int32_t)gameNo
                           factor:(int32_t)factor
                completionHandler:(void (^)(HSPRanking* ranking, HSPError *error))completionHandler;

/**
 * @brief Requests the score information list related to the specified user's game number, ranking factor and ranking period.
 *
 * @param memberNo Member number.
 * @param gameNo Game number.
 * @param rankingKeys List of ranking keys.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param scores An array of score information.<br>Array of HSPScore objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryScoresWithMemberNo:(int64_t)memberNo
                             gameNo:(int32_t)gameNo
                        rankingKeys:(NSArray*)rankingKeys
                  completionHandler:(void (^)(NSArray* scores, HSPError *error))completionHandler;

/**
 * @brief Requests the server to report ranking score of the current game.
 *
 * @param score Score.
 * @param factor Ranking factor.
 * @param extraData Extra data. ( will be deprecated )
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 *
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPRanking reportRankingScore:100.0 factor:0 extraData:nil completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully reported ranking score.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to report ranking score.");
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */

+(void)     reportRankingScore:(double)score
                        factor:(int32_t)factor
                     extraData:(NSString*)data
             completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests a list of scores of the specified ranking type and period from the server.
 *
 * @param scope Whether it is for all or friends.
 * @param period Period.
 * @param index Start index.<br>It starts from 1.
 * @param count Count. If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param myScore My score.<br>HSPScore object.
 * @param scores An array of scores.<br>Array of HSPScore objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)     queryScoresByScope:(HSPRankingScope)scope
                        period:(HSPRankingPeriod)period
                         index:(int32_t)index
                         count:(int32_t)count
             completionHandler:(void (^)(HSPScore* myScore, NSArray* scores, HSPError *error))completionHandler;

/**
 * @brief Requests a list of scores within the specified scope before initialization, from the server.
 *
 * @param scope Whether it is for all or friends.
 * @param period Period.
 * @param index Start index.<br>It starts from 1.
 * @param count Count. If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param myScore My score.<br>HSPScore object.
 * @param scores An array of scores.<br>Array of HSPScore objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)queryPreviousScoresByScope:(HSPRankingScope)scope
                            period:(HSPRankingPeriod)period
                             index:(int32_t)index
                             count:(int32_t)count
                 completionHandler:(void (^)(HSPScore* myScore, NSArray* scores, HSPError *error))completionHandler;

/**
 * @brief Requests a list of scores around the ranking of the specified user.
 *
 * @param memberNo Member number.
 * @param period Period.<br>HSPRankingPeriod value.
 * @param upCount The number of rankings higher than the user.
 * @param downCount The number of rankings lower than the user.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param scores An array of scores.<br>Array of HSPScore objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)     queryScoresAroundMemberNo:(int64_t)memberNo
                               period:(HSPRankingPeriod)period
                              upCount:(int32_t)upCount
                            downCount:(int32_t)downCount
                    completionHandler:(void (^)(NSArray* scores, HSPError *error))completionHandler;

/**
 * @brief Requests a list of scores before initialization, around the ranking of the specified user, from the server.
 *
 * @param memberNo Member number.
 * @param period Period.<br>HSPRankingPeriod value.
 * @param upCount The number of rankings higher than the user.
 * @param downCount The number of rankings lower than the user.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param scores An array of scores.<br>Array of HSPScore objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)queryPreviousScoresAroundMemberNo:(int64_t)memberNo
                                   period:(HSPRankingPeriod)period
                                  upCount:(int32_t)upCount
                                downCount:(int32_t)downCount
                        completionHandler:(void (^)(NSArray* scores, HSPError *error))completionHandler;

/**
 * @brief Requests the score information list for users related to the specified member numbers and ranking period.
 *
 * @param memberNos List of member number.
 * @param period Period.<br>HSPRankingPeriod value.
 * @param index Start index.<br>It starts from 1.
 * @param count Count. If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param myScore My score.<br>HSPScore object. 
 * @param scores An array of score information.<br>Array of HSPScore objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)queryScoresWithMemberNos:(NSArray*)memberNos
                          period:(HSPRankingPeriod)period
                           index:(int32_t)index
                           count:(int32_t)count
               completionHandler:(void (^)(HSPScore* myScore, NSArray* scores, HSPError *error))completionHandler;

/**
 * @brief Requests the score information list for users related to the specified member numbers and ranking period before initialization.
 *
 * @param memberNos List of member number.
 * @param period Period.<br>HSPRankingPeriod value.
 * @param index Start index.<br>It starts from 1.
 * @param count Count. If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 *
 * @param myScore My score.<br>HSPScore object. 
 * @param scores An array of score information.<br>Array of HSPScore objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)queryPreviousScoresWithMemberNos:(NSArray*)memberNos
                                  period:(HSPRankingPeriod)period
                                   index:(int32_t)index
                                   count:(int32_t)count
                       completionHandler:(void (^)(HSPScore* myScore, NSArray* scores, HSPError *error))completionHandler;


/**
* @brief Gets the Date to reset the next ranking for period.(based on the current device's Timezone).<br>
* If the ranking is not reset, the method returns nil.
* 
* @param period Period.<br>HSPRankingPeriod value.
* @return NSDate object for the next time to reset rankings, based on the current Timezone. nil if the ranking is
*         not reset.
* @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
*/
- (NSDate*)resetDateForPeriod:(HSPRankingPeriod)period;

@end
