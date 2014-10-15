//
//  DAPopupAd.h
//  IgaworksAd
//
//  Created by wonje,song on 2014. 4. 22..
//  Copyright (c) 2014년 wonje,song. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "DAError.h"
#import "IgaworksAD.h"


@protocol DAPopupAdDelegate;

@interface DAPopupAd : NSObject


@property (nonatomic, unsafe_unretained) id<DAPopupAdDelegate> delegate;


- (id)initWithKey:(NSString *)appKey spotKey:(NSString *)spotKey;
- (BOOL)presentFromViewController:(UIViewController *)viewController;
- (void)setLogLevel:(IgaworksADLogLevel)logLevel;

@end

@protocol DAPopupAdDelegate <NSObject>

@optional
- (void)DAPopupAdDidLoad:(DAPopupAd *)popupAd;

- (void)DAPopupAd:(DAPopupAd *)popupAd didFailToReceiveAdWithError:(DAError *)error;

- (void)DAPopupAdWillLeaveApplication:(DAPopupAd *)popupAd;

@end
