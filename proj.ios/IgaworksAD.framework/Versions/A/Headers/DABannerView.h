//
//  DABannerView.h
//  IgaworksAd
//
//  Created by wonje,song on 2014. 4. 8..
//  Copyright (c) 2014년 wonje,song. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "DAError.h"
#import "IgaworksAD.h"


@protocol DABannerViewDelegate;

typedef enum _DABannerViewSizeType
{
    DABannerViewSize320x50
} DABannerViewSizeType;

@interface DABannerView : UIView


@property (nonatomic, unsafe_unretained) id<DABannerViewDelegate> delegate;

// 최소 30초에서 최대 120초로 설정.
@property (nonatomic, unsafe_unretained) NSInteger adRefreshRate;


- (id)initWithBannerViewSize:(DABannerViewSizeType)size origin:(CGPoint)origin appKey:(NSString *)appKey spotKey:(NSString *)spotKey viewController:(UIViewController *)viewController;

- (void)loadRequest;


- (void)setLogLevel:(IgaworksADLogLevel)logLevel;

@end


@protocol DABannerViewDelegate <NSObject>

@optional
- (void)DABannerViewDidLoadAd:(DABannerView *)bannerView;

- (void)DABannerView:(DABannerView *)bannerView didFailToReceiveAdWithError:(DAError *)error;

- (void)DABannerViewWillLeaveApplication:(DABannerView *)bannerView;


@end