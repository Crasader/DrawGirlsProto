//
//  LiveOpsUser.h
//  LiveOps
//
//  Created by 강기태 on 2014. 8. 5..
//  Copyright (c) 2014년 IGAWorks. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^LiveOpsUserLoginWorkCallback)();

@interface LiveOpsUser : NSObject

+ (void)login;

+ (void)setLoginCompleteCallback:(LiveOpsUserLoginWorkCallback)block;

+ (NSString*)getObjectId;

+ (void)setTargetingData:(id)obj withKey:(NSString*)key;
+ (id)getTargetingDataWithKey:(NSString*)key;
@end
