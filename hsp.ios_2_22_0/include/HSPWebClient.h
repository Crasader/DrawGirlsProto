//
//  HSPWebClient.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 24..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Web client interface.
 */
@interface HSPWebClient : NSObject {
}
                                          
/**
 * @brief Checks if the URI is supported by HSP.
 *
 * @param uri URI to check.
 * @return Whether it is supported or not. If YES, the URI is supported by HSP.
 *
 * @code
 * BOOL isSupported = [HSPWebClient isSupportedUri:@"HSPUI://profile.myprofile"];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(BOOL)     isSupportedUri:(NSString*)uri;

/**
 * @brief Executes the protocol in HSP.<br>
 * If there is a protocol registered, the related handler is called.
 *
 * @param uri URI to process.
 * @param completionHandler Is called when the protocol is executed.
 *
 * This block needs the following parameters:
 * @param javaScript JavaScript.
 * @param error Error.<br>If successful, the error code is 0.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     processWithUri:(NSString*)uri
         completionHandler:(void (^)(NSString *javaScript, HSPError *error))completionHandler;

@end
