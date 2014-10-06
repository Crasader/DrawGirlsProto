//
//  HSPMemberDataStorage.h
//  HSPInterface
//
//  Created by Wooyong Kim on 13. 07. 18..
//  Copyright 2013 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Deals with the data storage of the HSP Member.
 */
@interface HSPMemberDataStorage : NSObject {
}

/**
 * @brief Requests to save the data dictionary includes keys and values to HSP server.
 *
 * @param dataDic data dictionary includes keys(type:NSString) and values(type:NSString).
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableDictionary* dataDic = [NSMutableDictionary dictionary];
 * [dataDic setObject:@"5" forKey:@"win"];
 * [dataDic setObject:@"3" forKey:@"lose"];
 *
 * [HSPMemberDataStorage saveMemberDataWithDataDictionary:dataDic completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      NSLog(@"Save data successfully.");
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+(void)	saveMemberDataWithDataDictionary:(NSDictionary*)dataDic completionHandler:(void (^)(HSPError* error))completionHandler;

/**
 * @brief Requests to load the data dictionary that corresponds to the key list.
 *
 * @param dataKeyArray data key(type:NSString) array.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param dataDic data dictionary.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* dataKeyArray = [NSMutableArray array];
 * [dataKeyArray addObject:@"win"];
 * [dataKeyArray addObject:@"lose"];
 *
 * [HSPMemberDataStorage loadMemberDataWithKeyArray:dataKeyArray completionHandler:^(NSDictionary* dataDic, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      NSLog(@"Win Count : %@", [data objectForKey:@"win"]);
 *      NSLog(@"Lose Count : %@", [data objectForKey:@"lose"]); 
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+(void)	loadMemberDataWithKeyArray:(NSArray*)dataKeyArray completionHandler:(void (^)(NSDictionary* dataDic, HSPError* error))completionHandler;

/**
 * @brief Requests to load the specific user data dictionary that corresponds to the key list.
 *
 * @param memberNo member number.
 * @param dataKeyArray data key(type:NSString) array.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param dataDic data dictionary.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* dataKeyArray = [NSMutableArray array];
 * [dataKeyArray addObject:@"win"];
 * [dataKeyArray addObject:@"lose"];
 *
 * [HSPMemberDataStorage loadMemberDataWithMemberNo:799018257320175415 keyArray:dataKeyArray completionHandler:^(NSDictionary* dataDic, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      NSLog(@"Win Count : %@", [data objectForKey:@"win"]);
 *      NSLog(@"Lose Count : %@", [data objectForKey:@"lose"]);
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+(void) loadMemberDataWithMemberNo:(int64_t)memberNo keyArray:(NSArray*)dataKeyArray completionHandler:(void (^)(NSDictionary* dataDic, HSPError* error))completionHandler;

/**
 * @brief Requests to remove the some data that corresponds to the key list.
 *
 * @param dataKeyArray data key(type:NSString) array.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSMutableArray* dataKeyArray = [NSMutableArray array];
 * [dataKeyArray addObject:@"win"];
 * [dataKeyArray addObject:@"lose"];
 *
 * [HSPMemberDataStorage removeMemberDataWithKeyArray:dataKeyArray completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 *      NSLog(@"Remove data successfully.");
 * 	}
 * }];
 * @endcode
 * @serviceDomain ALL
 */
+(void) removeMemberDataWithKeyArray:(NSArray*)dataKeyArray completionHandler:(void (^)(HSPError* error))completionHandler;

@end
