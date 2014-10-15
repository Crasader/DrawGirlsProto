//
//  AdBrix.h
//  IgaworksAd
//
//  Created by wonje,song on 2014. 3. 28..
//  Copyright (c) 2014년 wonje,song. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AdBrix : NSObject


/*!
 @abstract
 first time experience의 Activity에 해당할때 호출한다.
 
 @param activityName              activity name.
 */
+ (void)firstTimeExperience:(NSString *)activityName;


/*!
 @abstract
 first time experience의 Activity에 해당할때 호출한다.
 
 @param activityName              activity name.
 @param param                     parameter.
 */
+ (void)firstTimeExperience:(NSString *)activityName param:(NSString *)param;

/*!
 @abstract
 retension의 Activity에 해당할때 호출한다.
 
 @param activityName              activity name.
 */
+ (void)retention:(NSString *)activityName;

/*!
 @abstract
 retension의 Activity에 해당할때 호출한다.
 
 @param activityName              activity name.
 @param param                     parameter.
 */
+ (void)retention:(NSString *)activityName param:(NSString *)param;


/*!
 @abstract
 buy의 Activity에 해당할때 호출한다.
 
 @param activityName              activity name.
 */
+ (void)buy:(NSString *)activityName;

/*!
 @abstract
 buy의 Activity에 해당할때 호출한다.
 
 @param activityName              activity name.
 @param param                     parameter.
 */
+ (void)buy:(NSString *)activityName param:(NSString *)param;

/*!
 @abstract
 App.이 최초 실행될때 시작되었음을 서버로 전송하기 위해 호출한다.
 한번만 호출한다.
 
 @discussion
 AppDelegate의 - application:didFinishLaunchingWithOptions: 메소드에서 AdBrixManager - traceWithAppKey:andHashKey: 메소드를 호출하는 경우에는 start 메소드를 호출하지 않는다.
 Unity plugin의 경우 traceWithAppKey:andHashKey: 메소드 호출 후에, start 메소드를 호출한다.
 */
+ (void)start;

@end
