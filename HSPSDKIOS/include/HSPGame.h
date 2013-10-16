//
//  HSPGame.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Deals with the game information.
 * 
 * {@link HSPGame} object is available only when logged in the HSP server.
 * <p>
 * All the games using HSP are assigned unique game numbers, which are used as the keys that represent the game. For example, if you need the information of game recently played, you need to get the game number to load the information.
 * <p>
 * To get the game information that matches the game number, call loadGamesWithGameNos to get a list of game numbers. This function uses the handler to get a list of {@link HSPGame} instances, each of which has the game information.
 * <p>
 * {@link HSPGame} class provides game ID, name, game icon URL, service status, admin's member number, custom URL and market URL.
 */
@interface HSPGame : NSObject {
    int32_t         _gameNo;
    NSString*       _gameID;
    NSString*       _gameName;
    NSString*       _gameIconURL;
    NSString*       _statusCode;
    int64_t         _adminMemberNo;
    NSString*       _redirectionURL;
    NSString*       _bundleID;
    NSString*       _customURL;
    NSString*       _marketURL;
}

/**
 * @brief Game's unique number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int32_t         gameNo;

/**
 * @brief Game's unique ID. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       gameID;

/**
 * @brief Game name. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       gameName;

/**
 * @brief Game icon URL. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       gameIconURL;

/**
 * @brief Game's service status code. (read only)<br>
 * Checks if the game is in service or out of service.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       statusCode;

/**
 * @brief Game admin's member number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int64_t         adminMemberNo;

/**
 * @brief Game installation URL. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       redirectionURL;

/**
 * @brief Bundle ID. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       bundleID;

/**
 * @brief Custom URL. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       customURL;

/**
 * @brief market URL. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*       marketURL;

/**
 * @brief Requests a list of game information from the server.<br>
 * The HSP server consistently maintains a list of games, and a new game is added to the end of the list. The function needs index and count to request a list of game information. If the function is used to request the information of 30 games from 0 but there are only 20 games in the list, then it returns the information of only 20 games through the handler. 
 * 
 * @param index Start number.<br>It starts from 0.
 * @param count Count.<br>If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param games Game information list.<br>Array of HSPGames.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPGame loadGamesFromIndex:0 count:5 completionHandler:^(NSArray *games, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPGame* game in games )
 * 		{
 * 			NSLog(@"Game number: %d", game.gameNo);
 * 			NSLog(@"Game ID: %@", game.gameID);
 * 			NSLog(@"Game name: %@", game.gameName);
 * 			NSLog(@"Game icon URL : %@", game.gameIconURL);
 * 			NSLog(@"Game status: %@", game.statusCode);
 * 			NSLog(@"Game admin number: %lld", game.adminMemberNo);
 * 			NSLog(@"Game redirection URL: %@", game.redirectionURL);
 * 			NSLog(@"Game bundle ID: %@", game.bundleID);
 * 			NSLog(@"Game custom URL : %@", game.customURL); 
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loadGamesFromIndex:(int32_t)index
                         count:(int32_t)count
             completionHandler:(void (^)(NSArray *games, HSPError *error))completionHandler;

/**
 * @brief Requests a list of game information from the server.<br>
 * Uses the game number list as a parameter, and gets the game information list through the handler.
 * 
 * @param gameNos Game number list.<br>Array of NSNumbers.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param games Game information list.<br>Array of HSPGames.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* gameNoArray = [NSMutableArray array];
 * [gameNoArray addObject:[NSNumber numberWithInt:101]];
 *
 * [HSPGame loadGamesWithGameNos:gameNoArray completionHandler:^(NSArray *games, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPGame* game in games )
 * 		{
 * 			NSLog(@"Game number: %d", game.gameNo);
 * 			NSLog(@"Game ID: %@", game.gameID);
 * 			NSLog(@"Game name: %@", game.gameName);
 * 			NSLog(@"Game icon URL : %@", game.gameIconURL);
 * 			NSLog(@"Game status: %@", game.statusCode);
 * 			NSLog(@"Game admin number: %lld", game.adminMemberNo);
 * 			NSLog(@"Game redirection URL : %@", game.redirectionURL);
 * 			NSLog(@"Game bundle ID: %@", game.bundleID);
 * 			NSLog(@"Game custom URL: %@", game.customURL); 
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loadGamesWithGameNos:(NSArray*)gameNos
               completionHandler:(void (^)(NSArray *games, HSPError *error))completionHandler;

/**
 * @brief Requests the game icon image from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param icon Game icon image.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * HSPGame* game = [games objectAtIndex:0];
 * [game downloadGameIconWithCompletionHandler:^(UIImage* icon, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Images successfully downloaded.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to download images.");
 * 	} 
 * }]; 
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */

-(void)     downloadGameIconWithCompletionHandler:(void (^)(UIImage* icon, HSPError *error))completionHandler;

/**
 * @brief Returns whether the game is supported by the current OS or not.<br>
 * For example, the game for android is not supported.
 *
 * @return Whether it is supported or not.(YES: supported, NO: not supported).
 *
 * @code 
 * HSPGame* game = [games objectAtIndex:0];
 * if ([game isSupported] == YES)
 * {
 *     NSLog(@"Supported by the OS."); 
 * } else {
 *     NSLog(@"Not supported by the OS.");
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(BOOL)     isSupported;

/**
 * @brief Returns whether the game is installed in this device.
 * 
 * @return Whether it is installed or not.(YES: installed, NO: not installed).
 *
 * @code 
 * HSPGame* game = [games objectAtIndex:0];
 * if ([game isInstalled] == YES)
 * {
 *     NSLog(@"Installed in this device."); 
 * } else {
 *     NSLog(@"Not installed in this device.");
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(BOOL)     isInstalled;

/**
 * @brief Launches the game app.
 *
 * @param requesterURI URI of the requester that calls the method.
 * @return Execution result.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(BOOL)     launchApp:(NSString*)requesterURI;

/**
 * @brief Opens the App Store page to download the game.
 *
 * @param requesterURI URI of the requester that calls the method.
 * @return Execution result.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(BOOL)     launchStore:(NSString*)requesterURI;

/**
 * @brief Launches the game app if installed.<br>
 * If not installed, launches the App Store. If there is no market app, opens the game introduction page.
 *
 * @param requesterURI URI of the requester that calls the method.
 * @return Execution result.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(BOOL)     launchAppOrStore:(NSString*)requesterURI;

@end
