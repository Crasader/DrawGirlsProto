//
//  Nelo2SDK for iOS
//
//  Created by NHN Business Platform.
//  Copyright (c) 2012 NHN Business Platform. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NeloHandle;

@interface NeloLog : NSObject {
    @private
    NeloHandle* handle;
}

+ (NeloLog *) init:(NSString *)server onPort:(int)port ofProjectName:(NSString*)projectName withProjectVersion:(NSString*)appVersion;
+ (NeloLog *) init:(NSString *)server onPort:(int)port ofProjectName:(NSString*)appName withProjectVersion:(NSString*)appVersion forUserId:(NSString*)userId;

+ (void) debug:(NSString*)errorCode withMessage:(NSString*)message;
+ (void) error:(NSString*)errorCode withMessage:(NSString*)message;
+ (void) fatal:(NSString*)errorCode withMessage:(NSString*)message;
+ (void) info:(NSString*)errorCode withMessage:(NSString*)message;
+ (void) warn:(NSString*)errorCode withMessage:(NSString*)message;

+ (void) debug:(NSString*)errorCode withMessage:(NSString*)message atLocation:(NSString*)location;
+ (void) error:(NSString*)errorCode withMessage:(NSString*)message atLocation:(NSString*)location;
+ (void) fatal:(NSString*)errorCode withMessage:(NSString*)message atLocation:(NSString*)location;
+ (void) info:(NSString*)errorCode withMessage:(NSString*)message atLocation:(NSString*)location;
+ (void) warn:(NSString*)errorCode withMessage:(NSString*)message atLocation:(NSString*)location;

+ (void) setUserId:(NSString*)userId;
+ (void) setCustomField:(NSString*)value forKey:(NSString*)key;
+ (void) removeAllCustomFields;
+ (void) removeCustomFieldForKey:(NSString*)key;
+ (void) setLogType:(NSString*)logType;
+ (void) setLogSource:(NSString*) logSource;

@end
