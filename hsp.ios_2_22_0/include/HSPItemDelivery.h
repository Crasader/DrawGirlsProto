//
//  HSPItemDelivery.h
//  HSPItemDelivery
//
//  Created by Jongsoo Yun on 12. 8. 27..
//  Copyright (c) 2012 NHN. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Deals with item information.
 */
@interface HSPItemInfo : NSObject 
{
    NSString *itemId;
    int64_t itemSequence;
    int quantity;
    NSString *storeId;
    int64_t sentMemberNo;
}

/**
 * @brief Item ID.<br>Identifies item types.
 */
@property (nonatomic, retain)   NSString *itemId;

/**
 * @brief Item number.<br>A unique number of item information object.
 */
@property (nonatomic)           int64_t itemSequence;

/**
 * @brief The number of items.
 */
@property (nonatomic)           int quantity;

/**
 * @brief The store ID.
 */
@property (nonatomic, retain)   NSString *storeId;

/**
 * @brief The memberNo who sent the item.
 */
@property (nonatomic)           int64_t sentMemberNo;

@end

/**
 * @brief Deals with sending game items.
 */
@interface HSPItemDelivery : NSObject

/**
 * @brief Requests a list of item information which is not delivered, and receipts from the server.
 *
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param transactionId Transaction ID.
 * @param itemInfoList Array of item information.<br>Array of HSPItemInfo objects.
 * @param receipt Receipt.
 * @param error Error.<br>If successful, the error code is 0.
 */
+ (void)requestItemDeliveryWithCompletionHandler:(void (^)(int64_t transactionId, NSArray* itemInfoList, NSString* receipt, HSPError *error))completionHandler;

/**
 * @brief Notifies the server that items are successfully delivered.
 *
 * @param transactionId Transaction ID.
 * @param itemSeqList Array of item sequences.<br>Array of NSNumber objects which have long long values.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 */
+ (void)finishItemDeliveryWithTransactionId:(int64_t)transactionId ItemSeqList:(NSArray *)itemSeqList CompletionHandler:(void (^)(HSPError *error))completionHandler;

@end

