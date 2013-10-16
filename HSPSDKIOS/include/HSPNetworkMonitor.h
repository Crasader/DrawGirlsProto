//
//  HSPNetworkMonitor.h
//  HSPCore
//
//  Created by  on 11. 10. 19..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import "HSPResHandler.h"
#import "HSPTypes.h"

/**
 * Delegate protocol to receive messages from HSPNetworkMonitor.
 */
@protocol HSPNetworkMonitorDelegate<NSObject>

/**
 * Is called when the network status is changed.
 * @param networkType Changed network type [in]
 * @param connectionRequired Whether network connection is required (YES: Network not available now, NO: Network available now) [in]
 */
- (void)didChangedNetworkStatus:(HSPNetworkType)networkType connectionRequired:(BOOL)connectionRequired;

@end

@interface HSPNetworkMonitor : NSObject
{
	SCNetworkReachabilityRef		_reachability;
	id<HSPNetworkMonitorDelegate>	_delegate;
}

/**
 * A delegate instance to receive a response when the network status is changed.
 */
@property (nonatomic, assign)   id<HSPNetworkMonitorDelegate>    delegate;

/**
 * Creates an HSPNetworkMonitor instance.
 * @return HSPNetworkMonitor instance.
 */
+(HSPNetworkMonitor*)networkMonitor;

/**
 * Creates an HSPNetworkMonitor instance.
 * @param delegate A delegate instance to receive a response when the network status is changed. [in]
 * @return HSPNetworkMonitor instance.
 */
+(HSPNetworkMonitor*)networkMonitorWithDelegate:(id<HSPNetworkMonitorDelegate>)delegate;

/**
 * Initializes an object.
 * @param delegate A delegate instance to receive a response when the network status is changed. [in]
 * @return HSPNetworkMonitor instance.
 */
- (id)initWithDelegate:(id<HSPNetworkMonitorDelegate>)delegate;

/**
 * Starts monitoring.
 * @return BOOL Result (YES: Success, NO: Fail).
 */
- (BOOL)start;

/**
 * Stops monitoring.
 */
- (void)stop;

/**
 * Checks the current network status.
 * @return HSPNetworkType Network status type.
 * @see HSPNetworkType
 */
- (HSPNetworkType)currentStatus;

/**
 * Checks if the connection is currently required or not.
 * @return BOOL Result (YES: Network not available now, NO: Network available now).
 */
- (BOOL)connectionRequired;

@end
