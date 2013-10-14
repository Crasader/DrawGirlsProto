//
//  HSPGameLog.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Deals with the game play logs.
 * 
 * {@link HSPGameLog} object is available only when logged in the HSP server.
 * <p>
 * loadGameLogsWithGameNos is used to get the game play logs that match the game number. The function returns a list of {@link HSPGameLog} instances through the handler, each of which contains the game play log.
 * <p>
 * {@link HSPGameLog} class provides game number, first played date, and last played date.
 */
@interface HSPGameLog : NSObject {
    int32_t         _gameNo;
    NSDate*         _firstPlayedDate;
    NSDate*         _lastPlayedDate;
}

/**
 * @brief Game number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int32_t         gameNo;

/**
 * @brief The first played date. (read only)<br>
 * If the game is not played yet, the value is nil.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSDate*         firstPlayedDate;

/**
 * @brief The last played date. (read only)<br>
 * If the game is not played yet, the value is nil.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSDate*         lastPlayedDate;

/**
 * @brief Requests a list of game play logs from the server.<br>
 * The function is used to get a list of game play logs that matches the memberNo. 
 * 
 * @param gameNos List of game numbers.<br>Array of NSNumbers.
 * @param memberNo Member number.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:.
 * @param gameLogs List of game play logs.<br>Array of HSPGameLogs.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* gameNoArray = [NSMutableArray array];
 * [gameNoArray addObject:[NSNumber numberWithInt:101]];
 *
 * [HSPGameLog loadGameLogsWithGameNos:gameNoArray memberNo:4023012340L completionHandler:^(NSArray *gameLogs, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPGameLog* gameLog in gameLogs )
 * 		{
 * 			NSLog(@"Game number: %d", gameLog.gameNo);
 * 			NSLog(@"The first played date: %@", gameLog.firstPlayedDate);
 * 			NSLog(@"The last played date: %@", gameLog.lastPlayedDate);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loadGameLogsWithGameNos:(NSArray* )gameNos
                           memberNo:(int64_t)memberNo
                  completionHandler:(void (^)(NSArray *gameLogs, HSPError *error))completionHandler;

@end
