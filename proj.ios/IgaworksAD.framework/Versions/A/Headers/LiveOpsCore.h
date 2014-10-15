//
//  LiveOpsCore.h
//  LiveOps
//
//  Created by 강기태 on 2014. 7. 11..
//  Copyright (c) 2014년 IGAWorks. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AdBrixManager.h"

@interface LiveOpsCore : NSObject<AdBrixManagerDelegate>

+ (void)initLiveOps;

@end