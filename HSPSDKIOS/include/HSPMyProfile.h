//
//  HSPMyProfile.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPProfile.h"
#import "HSPDetailedProfile.h"

/**
 * @brief Deals with my profile. 
 *
 * Inherits HSPProfile.
 * 
 * @see HSPProfile
 */
@interface HSPMyProfile : HSPProfile {
    NSString*       _hangameID;
    BOOL            _useAddressBook;
    NSString*       _phoneNo;
}

/**
 * @brief Hangame ID. (read only)
 * @serviceDomain HANGAME
 */
@property(nonatomic, readonly, retain)      NSString*       hangameID;

/**
 * @brief Whether to use addressbook. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL            useAddressBook;

/**
 * @brief Phone number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSString*       phoneNo;

/**
 * @brief Requests my profile from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:  
 * @param profile My profile<br>HSPMyProfile object.  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMyProfile loadMyProfileWithCompletionHandler:^(HSPMyProfile* profile, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"My profile: %@", profile);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)         loadMyProfileWithCompletionHandler:(void (^)(HSPMyProfile* profile, HSPError *error))completionHandler;

/**
 * @brief Requests the server to report a nickname. 
 * 
 * @param nickname Nickname.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [myProfile reportNickname:@"MyNickname" completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully reported a nickname.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to report a nickname.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         reportNickname:(NSString*)nickname
             completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to report a status message.
 * 
 * @param todayWords Status message.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [myProfile reportTodayWords:@"Nice day" completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully reported a status message.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to report a status message.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         reportTodayWords:(NSString*)todayWords
               completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to upload a profile image.
 * 
 * @param image Profile image.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [myProfile uploadProfileImage:[UIImage imageNamed:@"MyProfileImage.png"] completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully uploaded a profile image.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to uploaded a profile image.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         uploadProfileImage:(UIImage*)image
                 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to delete a profile image.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [myProfile deleteProfileImageWithCompletionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully deleted a profile image.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to delete a profile image.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         deleteProfileImageWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests a certification code message for adding phone numbers from the server. 
 * 
 * @param phoneNo Phone number.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [myProfile requestCertificationNoWithPhoneNo:@"01123456789" completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully requested a certification code.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to request a certification code.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         requestCertificationNoWithPhoneNo:(NSString*)phoneNo
                                completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to add a phone number.
 * If the certification code matches the entered code, the phoneNo used when requestCertificationNoWithPhoneNo is called is added to the server.
 * 
 * @param certificationNo Certification code.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param savedPhoneNo Saved phone number. 
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [myProfile reportPhoneNoWithCertificationNo:@"3458" completionHandler:^(NSString* savedPhoneNo, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully added a phone number.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to add a phone number.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         reportPhoneNoWithCertificationNo:(int32_t)certificationNo
                               completionHandler:(void (^)(NSString* savedPhoneNo, HSPError *error))completionHandler;

/**
 * @brief Requests the server to add a birth date.
 * 
 * @param date Birth date.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * NSDate* birthData = [NSDate dateWithString:@"1981-06-09 00:00:00: +0900"];
 * [myProfile reportBirthDate:birthData completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully added a birth date.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to add a birth date.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         reportBirthDate:(NSDate*)date
			  completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to add gender.
 * 
 * @param gender Gender.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [myProfile reportGender:HSP_GENDER_FEMALE completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully added gender.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to add gender.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         reportGender:(HSPGender)gender
           completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to add an external service ID.
 * 
 * @param idpCode External service type.
 * @param idpID External service ID.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0. 
 * @see HSPIDPCode
 *
 * @code
 * [myProfile reportIdpIDWithIdpCode:HSP_IDP_FACEBOOK idpID:@"ironaldo81" completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully added a Facebook ID.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to add a Facebook ID.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME
 */
-(void)         reportIdpIDWithIdpCode:(HSPIDPCode)idpCode
                                 idpID:(NSString*)idpID
                     completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to add user game data.
 * 
 * @param gameUserData User game data.<br>Enter JSON format character string; if the length of the string is over 3000 bytes, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * NSString* gameData = @"[ { "Grade" : "Intermediate level" }, { "Account" : "700,000 jjeon" }, { "Insurance" : "0 jjeon" } ]";
 * [myProfile reportGameUserData:gameData completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully add user game data.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to add user game data.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME
 */
-(void)         reportGameUserData:(NSString*)gameUserData
                 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to set whether to show if I am online.
 * 
 * @param confirm Whether to show.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [myProfile confirmToExposeOnline:YES completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully showed my connection status.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to show my connection status.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)         confirmToExposeOnline:(BOOL)confirm
                    completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to set whether to show age.
 * 
 * @param confirm Whether to show.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.   
 *
 * @code
 * [myProfile confirmToExposeAge:YES completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully showed age.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to show age.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)         confirmToExposeAge:(BOOL)confirm 
                 completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to set whether to show gender.
 * 
 * @param confirm Whether to show.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [myProfile confirmToExposeGender:YES completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully showed gender.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to show gender.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)         confirmToExposeGender:(BOOL)confirm
                    completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to set whether to show external IDs.
 * 
 * @param confirm Whether to show.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [myProfile confirmToExposeIdpIDWithIdpCode:HSP_IDP_TWITTER confirm:NO completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Twitter ID is successfully set to private.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to set Twitter ID to private.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
-(void)         confirmToExposeIdpIDWithIdpCode:(HSPIDPCode)idpCode
                                        confirm:(BOOL)confirm
                              completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests the server to set whether to use an address book.
 * 
 * @param confirm Whether to use address book.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:  
 * @param error Error.<br>If successful, the error code is 0.   
 *
 * @code
 * [myProfile confirmToUseAddressbook:YES completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully set to use address book.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to set to use address book.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
-(void)         confirmToUseAddressbook:(BOOL)confirm
                      completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Upload image from specified path.
 *
 * @param path specified image path
 *
 * This block needs the following parameters:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @serviceDomain HANGAME GLOBALGAME_SG
 */
- (void)uploadImageFileFromPath:(NSString*)path
			  completionHandler:(void(^)(HSPError* error))completionHandler;

@end

/**
 * @brief [Notifications] Posted when all properties of my profile are changed (in case of connecting external service accounts). 
 */
extern NSString* const HSPMyProfileAllPropertiesDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my nickname is changed.<br>
 * The changed nickname is passed as an NSString* object.
 */
extern NSString* const HSPMyProfileNicknameDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my status message is changed.<br>
 * The changed status message is passed as an NSString* object.
 */
extern NSString* const HSPMyProfileTodayWordsDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my profile image is changed.<br>
 * The changed image is passed as an UIImage* object.
 */
extern NSString* const HSPMyProfileImageDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my phone number is changed.<br>
 * The changed phone number is passed as an NSString* object.
 */
extern NSString* const HSPMyProfilePhoneNoDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my birth date is changed.<br>
 * The changed birth date is passed as an NSNumber*(int) object.
 */
extern NSString* const HSPMyProfileBirthDateDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my gender is changed.<br>
 * The changed gender is passed as an NSNumber*(HSPGender) object.
 */
extern NSString* const HSPMyProfileGenderDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my accounts of external services are changed.<br>
 */
extern NSString* const HSPMyProfileIdpIDDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when my game user data is changed.<br>
 * The change game user data is passed as an NSString* object.
 */
extern NSString* const HSPMyProfileGameUserDataDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when the status whether to show if I am online is changed.<br>
 * The changed status is passed as an NSNumber*(BOOL) object.
 */
extern NSString* const HSPMyProfileExposeOnlineDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when the status whether to show my age is changed.<br>
 * The changed status is passed as an NSNumber*(BOOL) object.
 */
extern NSString* const HSPMyProfileExposeAgeDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when the status whether to show my gender is changed.<br>
 * The changed status is passed as an NSNumber*(BOOL) object.
 */
extern NSString* const HSPMyProfileExposeGenderDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when the status whether to show my external service IDs is changed.<br>
 */
extern NSString* const HSPMyProfileExposeIdpIDDidChangeNotificationName;

/**
 * @brief [Notifications] Posted when the status whether to use my addressbook is changed.<br>
 * The changed status is passed as an NSNumber*(BOOL) object.
 */
extern NSString* const HSPMyProfileUseAddressBookDidChangeNotificationName;
