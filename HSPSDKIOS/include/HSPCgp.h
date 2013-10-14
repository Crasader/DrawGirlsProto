//
//  HSPCgp.h
//  HSPCgp
//
//  Created by  on 11. 11. 28..
//  Copyright (c) 2011 NHN Corp. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * enum for promotion status.
 */
typedef enum
{
	CGP_NONE,               /**< No promotions. */
	CGP_PROMOTION_EXISTS,   /**< Promotion exists. */
	CGP_REWARD_REQUIRED     /**< Reward required. */
} PromotionState;

#define MHG_URL_SIZE			512
#define RESPONSE_STRING_SIZE	100

/**
 @brief			Struct returned as a parameter of getPromotionInfo.
 */
typedef struct  
{
	/** Promotion ID */
	int promotionId;
	/** URL of promotion button image */
	char buttonUrl[MHG_URL_SIZE];
	/** URL of horizontal image for the promotion banner */
	char bannerLandUrl[MHG_URL_SIZE];
	/** URL of vertical image for the promotion banner */
	char bannerPortUrl[MHG_URL_SIZE];
	/** Promotion speech bubble text */
	char bubbleText[RESPONSE_STRING_SIZE];
	/** Promotion type 1. Button 2. Banner */
	int typeCode; // 1.button 2.banner
	/** Promotion button image */
	UIImage* buttonImg;
	/** Horizontal image of promotion banner */
	UIImage* bannerLandImg;
	/** Vertical image of promotion banner */
	UIImage* bannerPortImg;
	/** Promotion reward code */
	char rewardCode[RESPONSE_STRING_SIZE];
	/** The number of promotion reward items */
	int rewardValue;
	/** Event page URL */
	char eventUrl[MHG_URL_SIZE];
}PromotionInfo;

/**
 * @brief CGP interface.
 */
@interface HSPCGP : NSObject

/**
 * Checks the promotion information of the CGP server.
 * 
 * @param completionHandler Is called when the response is received from the server.
 */
+ (void)checkPromotionWithCompletionHandler:(void (^)(PromotionState promotionState,  HSPError *error))completionHandler;

/**
 * Gets the promotion information.
 */
+ (PromotionInfo *)getPromotionInfo;

/**
 * Displays the promotion page (or launches the App Store for the banner type promotion).
 *
 * Returns true if successful, or false otherwise.
 */
+ (bool)launchPromotion;

/**
 * Reports to the promotion server that the promotion is completed (single game only).
 *
 * Returns true if successful, or false otherwise.
 *
 */
+ (bool)completePromotion;

@end
