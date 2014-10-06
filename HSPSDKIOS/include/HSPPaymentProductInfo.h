//
//  HSPPaymentProductInfo.h
//  HSPPayment
//
//  Created by Wooyong Kim on 13. 3. 22..
//  Copyright (c) 2013 NHN. All rights reserved.

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Deals with payment product info.
 */
@interface HSPPaymentProductInfo : NSObject
{
	NSString*	_productID;
	NSString*	_productName;
	NSString*	_productDescription;
	NSString*	_productInformation;
	NSString*	_currency;
	NSNumber*	_price;
	NSDate*		_validStartDate;
	NSDate*		_validEndDate;
	NSArray*	_ItemInfos;
}

/**
 * @brief Product ID. (read only)
 */
@property (nonatomic, retain, readonly)     NSString*   productID;

/**
 * @brief Product name. (read only)
 */
@property (nonatomic, retain, readonly)     NSString*   productName;

/**
 * @brief Product description. (read only)
 */
@property (nonatomic, retain, readonly)     NSString*   productDescription;

/**
 * @brief Product infomation. (read only)
 */
@property (nonatomic, retain, readonly)     NSString*   productInformation;

/**
 * @brief Product currency. (read only)
 */
@property (nonatomic, retain, readonly)     NSString*   currency;

/**
 * @brief Product price. (read only)
 */
@property (nonatomic, retain, readonly)		NSNumber*	price;

/**
 * @brief Product valid start date. (read only)
 */
@property (nonatomic, retain, readonly)     NSDate*		validStartDate;

/**
 * @brief Product valid end date. (read only)
 */
@property (nonatomic, retain, readonly)     NSDate*		validEndDate;

/**
 * @brief item infos. Array of HSPPaymentItemInfo objects. (read only)
 */
@property (nonatomic, retain, readonly)     NSArray*	ItemInfos;

@end

