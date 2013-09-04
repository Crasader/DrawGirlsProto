//
//  InApp.h
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 21..
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "VerificationController.h"


class ShopLayer;

@interface InApp : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
	ShopLayer* shop_layer;
}
+ (InApp *) sharedInstance;
-(void)setDelegate:(ShopLayer*)delegate;
@end
