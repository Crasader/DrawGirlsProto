//
//  AdPopcorn.h
//  IgaworksAd
//
//  Created by wonje,song on 2014. 3. 26..
//  Copyright (c) 2014년 wonje,song. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@protocol AdPopcornDelegate;


@interface AdPopcorn : NSObject
{

}


@property (nonatomic, unsafe_unretained) id<AdPopcornDelegate> delegate;

@property (nonatomic, unsafe_unretained) BOOL doNotShowContactUs;


/*!
 @abstract
 singleton AdPopcorn 객체를 반환한다.
 */
+ (AdPopcorn *)shared;

/*!
 @abstract
 Open offerwall.
 
 @discussion
 리스트 형태의 광고를 노출한다.
 
 @param vController          광고 리스트를 노출시킬 view controller
 @param delegate             AdPopcornDelegate
 @param userDataDictionaryForFilter    filtering(targeting)을 위한 user data
 */
+ (void)openOfferWallWithViewController:(UIViewController *)vController delegate:(id)delegate userDataDictionaryForFilter:(NSMutableDictionary *)userDataDictionaryForFilter;

+ (void)openPromotionEvent:(UIViewController *)vController delegate:(id)delegate;

@end


@protocol AdPopcornDelegate <NSObject>

@optional

/*!
 @abstract
 offerwall 리스트가 열리기 전에 호출된다.
 
 @discussion
 */
- (void)willOpenOfferWall;

/*!
 @abstract
 offerwall 리스트가 열린직 후 호출된다.
 
 @discussion
 */
- (void)didOpenOfferWall;


/*!
 @abstract
 offerwall 리스트가 닫히기 전에 호출된다.
 
 @discussion
 */
- (void)willCloseOfferWall;

/*!
 @abstract
 offerwall 리스트가 닫힌직 후 호출된다.
 
 @discussion
 */
- (void)didCloseOfferWall;

/*!
 @abstract
 promotion event가 열리기 전에 호출된다.
 
 @discussion
 */
- (void)willOpenPromotionEvent;

/*!
 @abstract
 promotion event가 열린직 후 호출된다.
 
 @discussion
 */
- (void)didOpenPromotionEvent;

/*!
 @abstract
 promotion event가 닫히기 전에 호출된다.
 
 @discussion
 */
- (void)willClosePromotionEvent;

/*!
 @abstract
 promotion event가 닫힌직 후 호출된다.
 
 @discussion
 */
- (void)didClosePromotionEvent;
   

@end
