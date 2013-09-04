//
//  InApp.m
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 21..
//
//

#import "InApp.h"
#import "VerificationController.h"
//#include "ShopLayer.h"

static InApp* singleton;
@implementation InApp




+ (InApp *)sharedInstance
{
	if (singleton == nil)
    {
		singleton = [[InApp alloc] init];
	}
	return singleton;
}

-(void)setDelegate:(ShopLayer*)delegate
{
	shop_layer = delegate;
}
-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	for(SKProduct *product in response.products){
		
		/*
		 localizedDescription --> 아이템 설명
		 localizedTitle --> 아이템 이름
		 price  ->> 아이템 가격
		 priceLocale  -> 지역별 가격표시 ex) $,원 표시등..
		 productIdentifier -->제품식별코드
		 
		 아래와 같이 product.xxxxx 로 접근해서 화면에 뿌려주시면 되실것입니다.
		 product.localizedTitle
		 */
		NSLog(@"%@", [product productIdentifier]);
		SKPayment *payment = [SKPayment paymentWithProduct:product];
		[[SKPaymentQueue defaultQueue] addPayment:payment];
	}
	[request release];
}

-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	//[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
	//[buyButton setVisible:YES];
	for(SKPaymentTransaction* transaction in transactions)
	{
		switch(transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchasing:
				// 구매 진행중.
				break;
			case SKPaymentTransactionStatePurchased:
				// 구매됐다.
				// transaction.payment.productIdentifier = 구매한 항목 id
				//NSLog(@"%@ is purchased", transaction.payment.productIdentifier);

				if([[VerificationController sharedInstance] verifyPurchase:transaction])
				{
					if([transaction.payment.productIdentifier isEqualToString:@"ruby10"])
					{
//						shop_layer->finishedPurchase("ruby10", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"ruby20"])
					{
//						shop_layer->finishedPurchase("ruby20", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"ruby30"])
					{
//						shop_layer->finishedPurchase("ruby30", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"ruby50"])
					{
//						shop_layer->finishedPurchase("ruby50", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"ruby100"])
					{
//						shop_layer->finishedPurchase("ruby100", 0);
					}
					else
					{
//						shop_layer->finishedPurchase("", 0);
					}
					UIAlertView *alert =
					[[[UIAlertView alloc]
					  initWithTitle:@"Congratulation"
					  message:@"Purchase was completed successfully"
					  delegate:nil
					  cancelButtonTitle:@"CONFIRM"
					  otherButtonTitles:nil]autorelease];
					[alert show];
					NSLog(@"%@ is purchased", transaction.payment.productIdentifier);
				}
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				break;
			case SKPaymentTransactionStateRestored:
				// 이미 지불했음.
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//				shop_layer->finishedPurchase("", 0);
				break;
			case SKPaymentTransactionStateFailed:
				// 구매를 취소했거나 에러발생.
				//if(transaction.error.code != SKErrorPaymentCancelled)
			{
				// 구매 과정중 오류가 발생했으니 사용자에게 알린다.
				UIAlertView *alert =
									  [[[UIAlertView alloc]
									   initWithTitle:@"ERROR"
									   message:@"Try purchasing again"
									   delegate:nil
									   cancelButtonTitle:@"CONFIRM"
									   otherButtonTitles:nil]autorelease];
				[alert show];
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//				shop_layer->finishedPurchase("", 0);
				break;
			}
				
		}
	}
}



@end
