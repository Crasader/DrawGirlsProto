//
//  HSPPaymentItemInfo.h
//  HSPPayment
//
//  Created by Wooyong Kim on 13. 3. 22..
//  Copyright (c) 2013 NHN. All rights reserved.

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief enum for item provide type.
 */
typedef enum
{
	HSP_ITEMPROVIDETYPE_COIN = 0,	/**< Coin. */
	HSP_ITEMPROVIDETYPE_ITEM,		/**< Item. */
} HSPItemProvideType;

/**
 * @brief enum for item use type.
 */
typedef enum
{
	HSP_ITEMUSETYPE_ONCE = 0,	/**< Once. */
	HSP_ITEMUSETYPE_FOREVER,	/**< Forever. */
} HSPItemUseType;


/**
 * @brief Deals with payment item info.
 */
@interface HSPPaymentItemInfo : NSObject
{
	NSString*				_itemID;
	NSString*				_itemName;
	HSPItemProvideType		_itemProvideType;
	HSPItemUseType			_itemUseType;
	NSString*				_itemDescription;
	NSString*				_itemInfomation;
	NSNumber*				_itemQuantity;
}

/**
 * @brief Item ID. (read only)
 */
@property (nonatomic, retain, readonly) NSString*   itemID;

/**
 * @brief Item name. (read only)
 */
@property (nonatomic, retain, readonly) NSString*   itemName;

/**
 * @brief Item give type. (read only)
 */
@property (nonatomic, readonly)			HSPItemProvideType   itemProvideType;

/**
 * @brief Item use type. (read only)
 */
@property (nonatomic, readonly)			HSPItemUseType	itemUseType;

/**
 * @brief Item description. (read only)
 */
@property (nonatomic, retain, readonly) NSString*   itemDescription;

/**
 * @brief Item infomation. (read only)
 */
@property (nonatomic, retain, readonly) NSString*   itemInfomation;

/**
 * @brief Item quantity. (read only)
 */
@property (nonatomic, retain, readonly) NSNumber* 	itemQuantity;

@end
