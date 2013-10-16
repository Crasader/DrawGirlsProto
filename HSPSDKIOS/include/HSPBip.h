//
//  HSPBip.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 24..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief enum for nelo log level.
 */
typedef enum
{
	HSP_NELOLOGLEVEL_DEBUG = 0,     /**< DEBUG. */
    HSP_NELOLOGLEVEL_INFO,			/**< INFO. */
    HSP_NELOLOGLEVEL_WARN,			/**< WARN. */
	HSP_NELOLOGLEVEL_ERROR,			/**< ERROR. */
	HSP_NELOLOGLEVEL_FATAL,			/**< FATAL. */
} HSPNeloLogLevel;

/**
 * @brief Sends the information to the Business Intelligence Platform (BIP) server.
 * 
 * {@link HSPBip} object provides the function to request the index information including game meta-information, inflow step and connection status from the BIP server.
 */
@interface HSPBip : NSObject {
}

/**
 * @brief Sends the game meta-information to the BIP server.
 * <br>
 * Developers and operators can choose which data to send to the server; they can also analyze and use the data received from the server if required.
 * 
 * @param type Type of data to send.<br>Enter the data type as agreed with the BIP server. 
 * @param data Data to send.
 * @param completionHandler Is called when the result whether the game meta-information is sent from the BIP server.
 *
 * This block contains the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSDictionary* data = [NSDictionary dictionary];
 * [HSPBip reportGameMetaInfoWithType:@"GameStart" data:data completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The game meta-information is successfully sent to the BIP server.");
 * 	}
 * }];
 * @endcode 
 */
+ (void)reportGameMetaInfoWithType:(NSString*)type
							  data:(NSDictionary*)data
				 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Sends the inflow step (stability index) to the BIP server.
 * <br>
 * The inflow step information is used to create the stability index which is used for analyzing the entire process. Let's suppose a game proceeds like a -> b -> c. This function is used to check how many users left the game during the process from a to b, and figure out the cause.   
 * 
 * @param step Inflow step.<br>This value is set up, as agreed between the game developer and the BIP server. For example, if you define the completion of a set of game as 21, you can use 21 as one of the inflow step values.  
 * @param completionHandler Is called when the result of sending the inflow step is received from the BIP server. 
 *
 * This block conatains the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPBip reportInflowStep:304 completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The inflow step is successfully sent to the BIP server.");
 * 	}
 * }];
 * @endcode  
 */
+ (void)reportInflowStep:(int32_t)step
	   completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Sends the connection status (concurrent user) to the BIP server.
 * <br>
 * The connection status is used to create the concurrent user index and check the concurrent user status.
 * 
 * @param completionHandler Is called when the result of sending connection status from the BIP server.
 *
 * This block contains the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPBip reportConnectedInfoWithCompletionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The connection status is successfully sent to the BIP server.");
 * 	}
 * }];
 * @endcode   
 */
+ (void)reportConnectedInfoWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Sends the log message to the NELO server.
 *
 * @param neloLogLevel log level.
 * @param stabilityCode stability code.
 * @param message log message.
 * @param errorCode error code.
 * @param location location of function called
 *
 * @code
 * [HSPBip reportNeloLogWithLevel:HSP_NELOLOGLEVEL_INFO
 *					stabilityCode:@"GAME_LOADING_START"
 *						  message:@"game loading started."
 *						errorCode:[error code]
 *						 location:[NSString stringWithFormat:@"%s", __FUNCTION__]];
 * @endcode
 */
+ (void)reportNeloLogWithLevel:(HSPNeloLogLevel)neloLogLevel
				 stabilityCode:(NSString*)stabilityCode
					   message:(NSString*)message
					 errorCode:(NSString*)errorCode
					  location:(NSString*)location;

@end
