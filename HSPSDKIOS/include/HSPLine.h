//
//  HSPLine.h
//  HSPInterface
//
//  Created by Hyunseung.Jung  on 12. 8. 28..
//  Copyright 2012 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Provides profile information of LINE members.
 */
@interface HSPLineProfile : NSObject {
 
    NSString*   _displayName;
    NSString*   _Mid;
    NSString*   _photoURL;
    NSString*   _thumbnailURL;
    int64_t     _memberNo;
}

/**
 * @brief Nickname of a LINE user. (read only)
 * @serviceDomain LINEGAME
 */
@property (nonatomic, readonly, retain)     NSString*   displayName;

/**
 * @brief Mid of the LINE user. (read only)
 * @serviceDomain LINEGAME
 */
@property (nonatomic, readonly, retain)     NSString*   Mid;

/**
 * @brief URL of profile image of the LINE user. (read only)
 * @serviceDomain LINEGAME
 */
@property (nonatomic, readonly, retain)     NSString*   photoURL;

/**
 * @brief URL of thumbnail image of the LINE user. (read only)
 * @serviceDomain LINEGAME
 */
@property (nonatomic, readonly, retain)     NSString*   thumbnailURL;

/**
 * @brief HSP member number of the LINE user. (read only)<br>
 * Returns -1 if the user is not a member of HSP.
 * @serviceDomain LINEGAME
 */
@property (nonatomic, readonly)             int64_t     memberNo;

@end

/**
 * @brief Provides the functions supported by LINE to the game.
 *
 * This interface provides an API that enables the game to easily use AppToApp protocol(URL scheme) supported by LINE.
 */
@interface HSPLine : NSObject

/**
 * @brief Checks if LINE is installed in the current device.<br>
 * It is available even if not logged in the HSP. 
 * @return BOOL YES if installed, or NO if not installed.
 *
 * @code
 * if ( [HSPLine isInstalled] == YES ) {
 * 		NSLog(@"LINE is already installed.");	
 * } else {
 * 		NSLog(@"LINE is not installed yet.");
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)isInstalled;

/**
 * @brief Launches the App Store to download LINE.<br>
 * It is available even if not logged in the HSP.
 * @return BOOL YES if Line app is installed, or NO if not installed. 
 *
 * @code
 * if ( [HSPLine isInstalled] == NO ) {
 * 		[HSPLine launchStore];
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)launchStore;

/**
 * @brief Launches LINE.<br>
 * It is available even if not logged in the HSP.
 * @return BOOL YES if Line app is installed, or NO if not installed.
 *
 * @code
 * if ( [HSPLine isInstalled] == YES ) {
 * 		[HSPLine launchLine];
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)launchLine;

/**
 * @brief Launches LINE shop for downloading mission sticker.<br>
 * It is available even if not logged in the HSP.
 * @return BOOL YES if Line app is installed, or NO if not installed.
 *
 * @code
 * if ( [HSPLine isInstalled] == YES ) {
 * 		[HSPLine launchLineShopWithPakcageId:@"Browny Sticker"];
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)launchLineShopWithPakcageId:(NSString*)packageId;

/**
 * @brief Checks if LINE is already installed in the current device and is available.<br>
 * It is available only when logged in the HSP.
 * @return BOOL YES if LINE is installed and available, NO if not installed or not available.
 *
 * @code
 * if ( [HSPLine canAuthorize] == YES ) {
 * 		NSLog(@"LINE is already installed and available.");	
 * } else {
 * 		NSLog(@"LINE is not installed yet or not available.");
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)canAuthorize;

/**
 * @brief Checks if the current login account has been authorized by LINE.<br>
 * It is available only when logged in the HSP.
 * @return BOOL YES if authorized by LINE, or NO otherwise.
 *
 * @code
 * if ( [HSPLine isAccountAuthorized] == YES ) {
 * 		NSLog(@"The user has been authorized by LINE.");	
 * } else {
 * 		NSLog(@"The user has not been authorized by LINE.");
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)isAccountAuthorized;

/**
 * @brief Launches LINE and sends a text passed as a parameter through LINE.<br>
 * A URL scheme is used to launch LINE.<br>
 * It is available even if not logged in the HSP.
 * @param text Text message to send.
 * @return BOOL YES if successful, or NO otherwise.
 *
 * @code
 * [HSPLine sendMessageWithText:@"Hello"];
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)sendMessageWithText:(NSString*)text;

/**
 * @brief Launches LINE and sends a text passed as a parameter through LINE.<br>
 * A URL scheme is used to launch LINE.<br>
 * It is available even if not logged in the HSP.
 * @param text Text message to send.
 * @return BOOL YES if successful, or NO otherwise.
 *
 * @code
 * [HSPLine sendMessageWithText:@"Hello"];
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)sendMessageWithText:(NSString*)text;

/**
 * @brief Launches LINE and sends an image passed as a parameter through LINE.<br>
 * A URL scheme is used to launch LINE.<br>
 * It is available even if not logged in the HSP.
 * @param image Image to send.
 * @return BOOL YES if successful, or NO otherwise.
 *
 * @code
 * [HSPLine sendMessageWithImage:image];
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)sendMessageWithImage:(UIImage*)image;

/**
 * @brief Launches LINE and sends the image passed as a parameter through LINE.<br>
 * A URL scheme is used to launch LINE.<br>
 * It is available even if not logged in the HSP.
 * @param imagePath Absolute path of an image to send.
 * @return BOOL YES if successful, or NO otherwise.
 *
 * @code
 * [HSPLine sendMessageWithImagePath:imagePath];
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)sendMessageWithImagePath:(NSString*)imagePath;

/**
 * @brief Launches LINE and sends an image passed as a parameter through LINE.<br>
 * A URL scheme is used to launch LINE.<br>
 * It is available even if not logged in the HSP.
 * @param imagePasteBoardName A key value of the image to send, stored in UIPasteBoard.
 * @return BOOL YES if successful, or NO otherwise.
 *
 * @code
 * [HSPLine sendMessageWithImagePasteBoardName:imagePasteBoardName];
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
+ (BOOL)sendMessageWithImagePasteBoardName:(NSString*)imagePasteBoardName;

/**
 * @brief Sends a text message containing the URL link of the current game.<br>
 * It is available only when logged in the HSP.
 * @param users List of users to whom the link message is sent.<br>Array of LINE IDs (MID, NSString*)
 * @param text Link message text to send.<br>\<b\>Supporting tags.
 * @param previewUrl URL of icon to be displayed on the link message to send.<br>Enter nil to display the game icon registerd in the HSP.
 * @param subText Supplementary message of the link message to send.<br>The font size of this message is smaller than that of the text message.
 * @param altText The message to be displayed on some versions of LINE which do not support the link message.<br>This is for backward compatibility.
 * @param linkText Text message to be displayed on the app link.<br>Enter nil and "Go To" will be automatically displayed in multiple languages.
 * @param iLinkUri URI to redirect when a user clicks the link on an iOS device.<br>Enter nil, and the URI will be set as the URLScheme of the game in HSP.
 * @param aLinkUri URI to redirect when a user clicks the link on an android device.<br>Enter nil, and the URI will be set as the URLScheme of the game in HSP.
 * @param completionHandler Is called when a response to the request is received from the server.
 * @return void
 *
 * @code
 * [HSPLine sendAppLinkMessageWithMids:[NSArray arrayWithObject:userMID]
 *                                text:@"Let's play Hangame!"
 *                          previewUrl:nil
 *                             subText:@"Come, join me!"
 *                             altText:@"Let's play Hangame! (http://m.smarthangame.com)"
 *                            linkText:@"Go To Hangame"
 *                            iLinkUri:@"SK101://"
 *                            aLinkUri:@"SK101://"
 *                   completionHandler:^(HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *          NSLog(@"Sent a link message successfully");
 *      else
 *          NSLog(@"Failed to send a link message");
 *  }];
 * @endcode
 * @serviceDomain LINEGAME
 */
+(void) sendAppLinkMessageWithMids:(NSArray*)users
							  text:(NSString*)text
						previewUrl:(NSString*)previewUrl
						   subText:(NSString*)subText
						   altText:(NSString*)altText
						  linkText:(NSString*)linkText
						  iLinkUri:(NSString*)iLinkUri
						  aLinkUri:(NSString*)aLinkUri
				 completionHandler:(void(^)(HSPError* error))completionHandler;

/**
 * @brief Sends an AppLink message to LINE of a HSP member.<br>
 * It is available only when logged in the HSP.
 * @param users List of users to whom a link message is sent.<br>Array of HSP memberNos (INT64)
 * @param text Link message text to send.<br>\<b\>Supporting tags.
 * @param previewUrl URL of icon to be displayed on the link message to send.<br>Enter nil to dipslay the game icon registered in the HSP.
 * @param subText Supplementary message of the link message to send.<br>The size of this message is smaller than that of the text message.
 * @param altText The message to be displayed on some versions of LINE which do not support the link message.<br>This is for backward compatibility.
 * @param linkText Text message to be displayed on the app link.<br>Enter nil, and "Go To" will be automatically displayed in multiple languages.
 * @param iLinkUri URI to redirect when a user clicks the link on an iOS device.<br>Enter nil, and the URI will be set as the URLScheme of the game in HSP.
 * @param aLinkUri URI to redirect when a user clicks the link on an android device.<br>Enter nil, and the URI will be set as the URLScheme of the game in HSP.
 * @param completionHandler Is called when a response to the request is received from the server.
 * @return void
 *
 * @code
 * [HSPLine sendAppLinkMessageWithMemberNos:[NSArray arrayWithObject:userMemberNos]
 *									   text:@"Let's play Hangame!"
 *								 previewUrl:nil
 *									subText:@"Come, join me!"
 *                                  altText:@"Let's play Hangame! (http://m.smarthangame.com)"
 *								   linkText:@"Go To Hangame"
 *								   iLinkUri:@"SK101://"
 *								   aLinkUri:@"SK101://"
 * 						  completionHandler:^(HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *          NSLog(@"Sent a link message successfully");
 *      else
 *          NSLog(@"Failed to send a link message");
 * }];
 * @endcode
 * @serviceDomain LINEGAME
 */
+ (void)sendAppLinkMessageWithMemberNos:(NSArray*)users
								   text:(NSString*)text
							 previewUrl:(NSString*)previewUrl
								subText:(NSString*)subText
								altText:(NSString*)altText
							   linkText:(NSString*)linkText
							   iLinkUri:(NSString*)iLinkUri
							   aLinkUri:(NSString*)aLinkUri
					  completionHandler:(void (^)(HSPError* error))completionHandler;

/**
 * @brief Requests a list of LINE friends.
 * 
 * @param index Start index.<br>It starts from 0.
 * @param count Count.<br>If the count is over 200, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * @code
 * [HSPLine queryLineFriendsFromIndex:0 
 *                              count:10
 *                  completionHandler:^(NSArray* lineProfileList, int32_t totalFriendCount, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *          NSLog(@"Successfully received a list of LINE friends");
 *      else
 *          NSLog(@"Failed to receive a list of LINE friends");
 * }];
 * @endcode
 * @serviceDomain LINEGAME
 */
+ (void)queryLineFriendsFromIndex:(int32_t)index
                            count:(int32_t)count
                completionHandler:(void(^)(NSArray* lineProfileList, int32_t totalFriendCount, HSPError* error))completionHandler;

/**
 * @brief Requests a list of LINE MIDs of HSP users from the server. 
 * You can requests LINE MIDs of up to 500 users; for MID of a user who does not authenticated to LINE, the function returns blank (@"").
 * @param memberNos List of member numbers to request<br>Array of NSNumber instances.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * @code
 * [HSPLine queryMidsWithMemberNos:memberNos
 *               completionHandler:^(NSDictionary* midDictionary, HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *      {
 *          NSLog(@"Successfully received MIDs");
 *          for ( NSNumber* memberNo in memberNos )
 *          {
 *              NSString* mid = [midDictionary objectForKey:memberNo];
 *              if ( mid != nil && [mid length] != 0 )
 *              {
 *                  NSLog(@"memberNo : %@, MID : %@", memberNo, mid);
 *              }
 *              else
 *              {
 *                  NSLog(@"%@ is not a LINE user.", memberNo);
 *              }
 *          }
 *      }
 *      else
 *      {
 *          NSLog(@"Failed to receive MIDs.");
 *      }
 * }];
 * @endcode
 * @serviceDomain LINEGAME
 */
+ (void)queryMidsWithMemberNos:(NSArray*)memberNos
             completionHandler:(void(^)(NSDictionary* midDictionary, HSPError* error))completionHandler;

/**
 * @brief Requests for registering Line timeline activity.
 *
 * @param templateId System message ID.
 * @param objIds Object IDs for system message.
 * @param dynamicObjs Dynamic object Map.
 * @param title Content title.
 * @param text Content message.
 * @param subText Content sub message.
 * @param aTargetUrl Link url for Android.(Url Scheme) if empty or null, it sets custom url on HSA.
 * @param iTargetUrl Link url for iOS.(Url Scheme) if empty or null, it sets custom url on HSA.
 * @param aMarketUrl Android Market url. if empty or null, it sets install url on HSA.
 * @param iMarketUrl iOS AppStore url. if empty or null, it sets install url on HSA.
 * @param thumbnailUrl Content thumbnail image url. if empty or null, it sets install image url on HSA.
 * @param thumbnailWidth Content thumbnail image width. if thumbnailUrl is empty or null, it sets install image width on HSA.
 * @param thumbnailHeight Content thumbnail image height. if thumbnailUrl is empty or null, it sets install image height on HSA.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * @code
 * [HSPLine registerLineTimelineActivityWithTemplateId:templateId
 *												objIds:objIds			// NSArray ( NSNumber(long long type)들의 array )
 *										   dynamicObjs:dynamicObjs		// NSDictionary ( key : NSString, value : NSString )
 *												 title:@"LINE POP!"
 *												  text:@"LINE 인기 캐릭터가 총출동!"
 *											   subText:@"LINE 친구들과 짜릿한 랭킹 승부를 겨루는 퍼즐게임"
 *											aTargetUrl:@"http://bit.ly/UfgVND"
 *											iTargetUrl:@"http://bit.ly/UfgVND"
 *											aMarketUrl:@""
 *											iMarketUrl:@""
 *										  thumbnailUrl:@"http://~~~.jpg"
 *										thumbnailWidth:100
 *									   thumbnailHeight:60
 *									 completionHandler:^(HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *      {
 *          NSLog(@"Successfully registered line timeline");
 *      }
 *      else
 *      {
 *          NSLog(@"Failed to register line timeline(%@)", error);
 *      }
 * }];
 * @endcode
 * @serviceDomain LINEGAME
 */
+ (void)registerLineTimelineActivityWithTemplateId:(int64_t)templateId
											objIds:(NSArray*)objIds
									   dynamicObjs:(NSDictionary*)dynamicObjs
											 title:(NSString*)title
											  text:(NSString*)text
										   subText:(NSString*)subText
										aTargetUrl:(NSString*)aTargetUrl
										iTargetUrl:(NSString*)iTargetUrl
										aMarketUrl:(NSString*)aMarketUrl
										iMarketUrl:(NSString*)iMarketUrl
									  thumbnailUrl:(NSString*)thumbnailUrl
									thumbnailWidth:(int32_t)thumbnailWidth
								   thumbnailHeight:(int32_t)thumbnailHeight
								 completionHandler:(void(^)(HSPError* error))completionHandler;

/**
 * @brief Requests approval for downloading mission sticker.
 *
 * @param packageId packageId.
 *
 * @code
 * [HSPLine requestMissionStickerWithPackageId:@"Browny Sticker"
 *							 completionHandler:^(HSPError* error) {
 *      if ( [error isSuccess] == YES )
 *      {
 *          NSLog(@"approved Successfully.");
 *      }
 *      else
 *      {
 *          NSLog(@"Failed to approve(%@)", error);
 *      }
 * }];
 * @endcode
 * @serviceDomain LINEGAME
 */
+ (void)requestMissionStickerWithPackageId:(NSString*)packageId
						 completionHandler:(void(^)(HSPError* error))completionHandler;

@end
