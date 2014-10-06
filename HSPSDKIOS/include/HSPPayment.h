//
//  HSPPayment.h
//  HSPPayment
//
//  Created by Wooyong Kim on 13. 3. 21..
//  Copyright 2013 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"
#import "HSPPaymentProductInfo.h"
#import "HSPPaymentItemInfo.h"

/**
 * @brief Deals with purchasing products in games.
 */
@interface HSPPayment : NSObject

+ (BOOL)handleOpenURL:(NSURL*)url;

/**
 * @brief Requests the server to purchase a specific product.
 * @param productId Product ID to purchase.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 */
+ (void)purchaseWithProductId:(NSString *)productId
			completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to purchase a specific product and deliver it to other user.
 * @param productId Product ID to purchase.
 * @param receiverMemberNo Member number of the user who will receive the purchased product.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 */
+ (void)purchaseWithProductId:(NSString *)productId
			 receiverMemberNo:(int64_t)receiverMemberNo
			completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests a list of product information related to the list of product IDs, from App Store.
 * @param productIds List of product IDs.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param products List of product information. Array of SKProduct objects.
 * @param error Error.<br>If successful, the error code is 0.
 */
+ (void)productsRequestWithIds:(NSSet*)productIds
			 completionHandler:(void (^)(NSArray* products, HSPError *error))completionHandler;


/**
 * @brief Requests a list of product infomation from payment server.
 *
 * This block needs the following parameter:
 * @param products List of product information. Array of HSPPaymentProductInfo objects.
 * @param error Error.<br>If successful, the error code is 0.
 * @see HSPPaymentProductInfo
 */
+ (void)requestProductInfosWithCompletionHandler:(void (^)(NSArray* productInfos, HSPError *error))completionHandler;

@end

