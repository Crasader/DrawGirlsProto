//
//  HSPError.h
//  HSPCore
//
//  Created by  on 11. 10. 11..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPErrorCodes.h"

@class HSPHTTPResponse;

/**
 * @brief Deals with HSP error information.
 */
@interface HSPError : NSError
{
	NSMutableDictionary*    _handlerMap;
}

/**
 * @brief Requests the error page URL from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param errorPageURL The error page URL.
 * @param error Error.<br>If successful, the error code is 0.
 * 
 * @code
 * [error requestErrorPageURLWithCompletionHandler:^(NSString* errorPageURL, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Error page URL: %@", errorPageURL);		
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)requestErrorPageURLWithCompletionHandler:(void (^)(NSString* errorPageURL, HSPError *error))completionHandler;

/**
 * @brief Checks if the error code set in the error instance means success. 
 * 
 * @return Success or failure (YES: Success, NO: Failure).
 * 
 * @code
 * if ( [error isSuccess] == YES )
 *      NSLog(@"Success.");	
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isSuccess;

@end
