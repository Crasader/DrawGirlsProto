//
//  DANativeAd.h
//  IgaworksAd
//
//  Created by wonje,song on 2014. 7. 11..
//  Copyright (c) 2014년 wonje,song. All rights reserved.
//

#import <Foundation/Foundation.h>


#import "DAError.h"
#import "IgaworksAD.h"

#import "DAGetNativeAdvertisingResult.h"

@protocol DANativeAdDelegate;

@interface DANativeAd : NSObject

@property (nonatomic, unsafe_unretained) id<DANativeAdDelegate> delegate;

@property (nonatomic, strong) DAGetNativeAdvertisingResult *nativeAdvertisingResult;

@property (nonatomic, strong) id nativeAdvertisingResultJson;

- (id)initWithKey:(NSString *)appKey spotKey:(NSString *)spotKey;

- (void)loadAd;

- (void)setLogLevel:(IgaworksADLogLevel)logLevel;

- (void)callImpression:(NSString *)impressionUrl;
- (void)click:(NSString *)clickUrl;

@end


@protocol DANativeAdDelegate <NSObject>

@optional
- (void)DANativeAdDidFinishLoading:(DANativeAd *)nativeAd;

- (void)DANativeAd:(DANativeAd *)nativeAd didFailWithError:(DAError *)error;


@end