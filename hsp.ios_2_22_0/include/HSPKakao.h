//
//  HSPKakao.h
//  HSPInterface
//
//  Created by Seungwoo on 13. 1. 2..
//
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

@interface HSPKakao : NSObject

/**
 * @brief Requests a list of HSP Member numbers of Kakao Talk users from the server. <br>
 * You can requests Kakao IDs of up to 500 users; for Kakao ID of a user who does not authenticated to HSP, the
 * callback returns 0.
 * @param kakaoIds List of Kakao IDs.
 * @param completionHandler Is called when a response to the request is received from the server.
 * @return void
 *
 * This block needs the following parameters:
 * @param memberNoMap MemberNoMap key is Kakao ID, value is HSP member number(if exist or 0). Nil if an error occurs.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPKakao queryHSPMemberNosWithKakaoIds:[NSArray arrayWithObject:kakaoid] completionHandler:^(NSDictionary* memberNoMap, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *		{
 *			for ( NSString* kakaoId in memberNoMap.allKeys )
 *			{
 *				NSNumber* memberNo = [memberNoMap objectForKey:kakaoId];
 *				NSLog(@"HSPMemberNo : %lld with kakaoId : %@", [memberNo longLongValue], kakaoId);
 *			}
 *		}
 *      else
 *          NSLog(@"Failed to query hspmemberNo with kakaoId(%@)", error);
 * }];
 * @endcode
 * @serviceDomain KAKAOGAME
 */
+ (void)queryHSPMemberNosWithKakaoIds:(NSArray*)kakaoIds
					completionHandler:(void (^)(NSDictionary* memberNoMap, HSPError* error))completionHandler;

/**
 * @brief Requests a list of kakao IDs of HSP users from the server.
 * You can requests kakao Ids of up to 500 users; for kakao Id of a user who does not authenticated to kakao, the function returns blank (@"").
 * @param memberNos List of member numbers to request<br>Array of NSNumber instances.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * @code
 * [HSPKakao queryKakaoIdsWithMemberNos:memberNos
 *					  completionHandler:^(NSDictionary* kakaoIdDictionary, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *      {
 *          NSLog(@"Successfully received kakaoIds");
 *          for ( NSNumber* memberNo in memberNos )
 *          {
 *              NSString* kakaoId = [kakaoIdDictionary objectForKey:memberNo];
 *              if ( kakaoId != nil && [kakaoId length] != 0 )
 *              {
 *                  NSLog(@"memberNo : %@, kakaoId : %@", memberNo, kakaoId);
 *              }
 *              else
 *              {
 *                  NSLog(@"%@ is not a Kakao user.", memberNo);
 *              }
 *          }
 *      }
 *      else
 *      {
 *          NSLog(@"Failed to receive kakaoIds.");
 *      }
 * }];
 * @endcode
 * @serviceDomain KAKAOGAME
 */
+ (void)queryKakaoIdsWithMemberNos:(NSArray*)memberNos
				 completionHandler:(void(^)(NSDictionary* kakaoIdDictionary, HSPError* error))completionHandler;

@end
