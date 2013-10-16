//
//  HSPAchievement.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 24..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Deals with the achievement information.
 *
 * {@link HSPAchievement} object is available only when logged in the HSP server.
 * <p>
 * Call loadAchievementsWithGameNo to get all the achievement information and the information whether a mission is achieved by players or not. Use the handler to get the {@link HSPAchievement} instance list, the achievement information for the game.
 * <p>
 * If a player achieves missions of the game, call reportAchievementWithAchieveID to report the achievement to the server. 
 * <p>
 * {@link HSPAchievement} class provides the information including achievement ID, title, detail, score, achieved icon URL, unachieved icon URL and whether it is achieved or not. 
 */
@interface HSPAchievement : NSObject {
    NSString*       _achievementID;
    NSString*       _title;
    NSString*       _detail;
    int32_t         _score;
    NSString*       _achievedIconURL;
    NSString*       _unachievedIconURL;    
    BOOL            _isAchieved;
}

/**
 * @brief ID of a mission to be achieved (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   achievementID;

/**
 * @brief Title of a mission to be achieved (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   title;

/**
 * @brief Details of a mission to be achieved (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   detail;

/**
 * @brief Scores when a mission is achieved (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int32_t     score;

/**
 * @brief URL for an achievement icon (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   achievedIconURL;

/**
 * @brief URL for an icon which means that a mission is not achieved (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   unachievedIconURL;

/**
 * @brief Whether a mission is achieved or not (read only).
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             BOOL        isAchieved;

/**
 * @brief Requests the achievement information list from the server. 
 * 
 * @param gameNo Game number.<br>Gets a list of achievement information with this game number. 
 * @param memberNo Member number.<br>Gets the information of achievement with this member number. 
 * @param completionHandler Is called when the achievement information list is received from the server. 
 *
 * This block needs the following parameters:
 * @param achievements An array of the achievement information.<br>This is an array of HSPAchievement object.
 * @param totalScore Total score of missions to be achieved.
 * @param totalCount Total number of missions to be achieved.
 * @param totalAchievedScore Total score of missions achieved.
 * @param totalAchievedCount Total number of missions achieved. 
 * @param error Error.<br>If the funcion succeeds, this value is 0.
 *
 * @code
 * [HSPAchievement loadAchievementsWithGameNo:101 memberNo:4105000000131917 completionHandler:^(NSArray* achievements, int32_t totalScore, int32_t totalCount, int32_t totalAchievedScore, int32_t totalAchievedCount, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Total score of missions to be achieved : %d", totalScore);
 * 		NSLog(@"Total number of missions to be achieved : %d", totalCount);
 * 		NSLog(@"Total score of missions achieved by the player : %d", totalAchievedScore);
 * 		NSLog(@"Total number of missions achieved by the player : %d", totalAchievedCount);
 *       
 * 		for ( HSPAchievement* achievement in achievements )
 * 		{
 * 			NSLog(@"ID : %@", achievement.achievementID);
 * 			NSLog(@"Name : %@", achievement.title);
 * 			NSLog(@"Detail : %@", achievement.detail);
 * 			NSLog(@"Score : %d", achievement.score);
 * 			NSLog(@"Achieved icon URL : %@", achievement.achievedIconURL);
 * 			NSLog(@"Unachieved icon URL : %@", achievement.unachievedIconURL);
 * 			NSLog(@"Whether achieved : %d", achievement.isAchieved);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)loadAchievementsWithGameNo:(int32_t)gameNo
						  memberNo:(int64_t)memberNo
				 completionHandler:(void (^)(NSArray* achievements, int32_t totalScore, int32_t totalCount, int32_t totalAchievedScore, int32_t totalAchievedCount, HSPError *error))completionHandler;

/**
 * @brief Requests the total score and the total number of the missions achieved from the server.
 * 
 * @param gameNo Game number.<br>Gets the total score and the total number of missions achieved for this game number. 
 * @param memberNo Member number.<br>Gets the achievement information for this member number. 
 * @param completionHandler Is called when the total score and the total number of missions achieved are received from the server. 
 *
 * This block needs the following parameters:
 * @param totalAchievedScore Total score of missions achieved.
 * @param totalAchievedCount Total number of missions achieved.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAchievement loadAchievedInfoWithGameNo:101 memberNo:4105000000131917 completionHandler:^(int32_t totalAchievedScore, int32_t totalAchievedCount, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Total number of missions achieved by the player : %d", totalAchievedCount);
 * 		NSLog(@"Total score of missions achieved by the player : %d", totalAchievedScore);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)loadAchievedInfoWithGameNo:(int32_t)gameNo
						  memberNo:(int64_t)memberNo
				 completionHandler:(void (^)(int32_t totalAchievedScore, int32_t totalAchievedCount, HSPError *error))completionHandler;

/**
 * @brief Reports the mission achievement to the server.
 * 
 * @param achievementID ID of a mission.
 * @param completionHandler Is called when the information whether the achievement is reported or not is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPAchievement reportAchievementWithAchieveID:101 completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The achievement is successfully reported.");
 * 	} else
 * 	{
 * 		NSLog(@"Reporting the achievement has failed.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)reportAchievementWithAchieveID:(NSString*)achieveID
					 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the achievement icon from the server.
 * 
 * @param completionHandler Is called when the achievement icon is received from the server.
 *
 * This block needs the following parameters:
 * @param image Image for the achievement icon.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * HSPAchievement* achievement = [achievements objectAtIndex:0];
 * [achievement downloadAchievedIconWithCompletionHandler:^(UIImage* image, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The image is successfully downloaded.");
 * 	} else
 * 	{
 * 		NSLog(@"Downloading the image has failed.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)downloadAchievedIconWithCompletionHandler:(void (^)(UIImage* image, HSPError *error))completionHandler;

/**
 * @brief Requests the unachievement icon from the server.
 * 
 * @param completionHandler Is called when the unachievement icon is received from the server.
 *
 * This block needs the following parameters:
 * @param image Image for the unachievement icon.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * HSPAchievement* achievement = [achievements objectAtIndex:0];
 * [achievement downloadUnachievedIconWithCompletionHandler:^(UIImage* image, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The image is successfully downloaded.");
 * 	} else
 * 	{
 * 		NSLog(@"Downloading the image has failed.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)downloadUnachievedIconWithCompletionHandler:(void (^)(UIImage* image, HSPError *error))completionHandler;

@end

