//
//  DAInterstitialView.h
//  IgaworksAd
//
//  Created by wonje,song on 2014. 4. 14..
//  Copyright (c) 2014년 wonje,song. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DAError.h"
#import "IgaworksAD.h"


@protocol DAInterstitialAdDelegate;

@interface DAInterstitialAd : NSObject


@property (nonatomic, unsafe_unretained) id<DAInterstitialAdDelegate> delegate;
@property (nonatomic, unsafe_unretained, getter = isInterstitialAdIsVisible) BOOL interstitialAdIsVisible;

- (id)initWithKey:(NSString *)appKey spotKey:(NSString *)spotKey viewController:(UIViewController *)viewController;


//- (BOOL)presentInView:(UIView *)view;
- (BOOL)presentFromViewController:(UIViewController *)viewController;

- (void)setLogLevel:(IgaworksADLogLevel)logLevel;


@end

@protocol DAInterstitialAdDelegate <NSObject>

- (void)DAInterstitialAd:(DAInterstitialAd *)interstitialAd didFailToReceiveAdWithError:(DAError *)error;

@optional
- (void)DAInterstitialAdDidLoad:(DAInterstitialAd *)interstitialAd;

- (void)DAInterstitialAdWillLeaveApplication:(DAInterstitialAd *)interstitialAd;

//- (void)daInterstitialAdDidLoad;


@end
