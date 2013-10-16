//
//  HSPDetailedProfile.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief enum for relations.
 */
typedef enum
{
    HSP_RELATIONTYPE_NONE = 0,  /**< None. */
    HSP_RELATIONTYPE_FOLLOW,    /**< Follow. */
    HSP_RELATIONTYPE_BLOCK,     /**< Block. */
} HSPRelationType;

/**
 * @brief enum for gender.
 */
typedef enum
{
    HSP_GENDER_UNKNOWN = 0, /**< Unknown. Not registered. */
    HSP_GENDER_MALE,        /**< Male. */
    HSP_GENDER_FEMALE       /**< Female. */
} HSPGender;

/**
 * @brief enum for services.
 */
typedef enum
{
    HSP_IDP_ME2DAY = 0,     /**< me2day. */
    HSP_IDP_TWITTER,        /**< Twitter. */
    HSP_IDP_FACEBOOK        /**< Facebook. */
} HSPIDPCode;

/**
 * @brief enum for punishment types.
 */
typedef enum
{
    HSP_PUNISHMENTTYPE_TODAYWORDS = 0,  /**< Status message. */
    HSP_PUNISHMENTTYPE_PROFILEIMAGE     /**< Profile image. */
} HSPPunishmentType;

/*
 * @brief Deals with ID information for external services.
 *
 * This interface deals with account information for external services including me2day and twitter.
 */
@interface HSPIDPInfo : NSObject {
    HSPIDPCode          _code;
    NSString*           _idpID;
    BOOL                _exposeID;
}

/*
 * @brief External service types. (read only)
 */
@property(nonatomic, readonly)              HSPIDPCode          code;

/*
 * @brief External service IDs. (read only)
 */
@property(nonatomic, readonly, retain)      NSString*           idpID;

/*
 * @brief Whether to expose the external service IDs. (read only)
 */
@property(nonatomic, readonly)              BOOL                exposeID;

@end

/*
 * @brief Deals with punishment information.
 */
@interface HSPPunishmentInfo : NSObject {
    HSPPunishmentType   _type;
    int32_t             _count;
    NSDate*             _releaseDate;
}

/*
 * @brief Punishment type.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              HSPPunishmentType   type;

/*
 * @brief The number of punishments.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              int32_t             count;

/*
 * @brief The date to stop punishment.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSDate*             releaseDate;

@end

/**
 * @brief Deals with the detailed information about users.
 *
 * {@link HSPDetailedProfile} object is available only when logged in the HSP server.
 * <p>
 * This class is used to deal with the detailed information about users which {@link HSPProfile} class does not support. You can get {@link HSPProfile} instances by list, but {@link HSPDetailedProfile} instances one by one.
 * <p>
 * {@link HSPDetailedProfile} class provides the information such as whether the user is admin or not, type of relations with me, whether the nickname is changed or not, age, gender, status message, whether to show age and gender, punishiment information, account information of external services, list of games recently played, and user data.
 */
@interface HSPDetailedProfile : NSObject {
    BOOL                _isAdmin;
    HSPRelationType     _relationTypeFromMe;
    HSPRelationType     _relationTypeToMe;
    BOOL                _isNicknameChanged;
    int8_t              _age;
    HSPGender           _gender;
    NSString*           _todayWords;
    BOOL                _exposeAge;
    BOOL                _exposeGender;
    NSDictionary*       _punishmentInfoMap;
    NSDictionary*       _idpInfoMap;   
    NSArray*            _playedGameNoList;
    NSString*           _gameUserData;
    
    // private
    NSString*           _phoneNo;
}

/**
 * @brief Whether the user is admin or not. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                isAdmin;

/**
 * @brief Relation type with other users in my position. (read only)<br>
 * For example, if I follow the user, relationTypeFromMe is HSP_RELATIONTYPE_FOLLOW. If I block the user, it is HSP_RELATIONTYPE_BLOCK.
 *
 * @see HSPRelationType
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              HSPRelationType     relationTypeFromMe;

/**
 * @brief Relation type with me in their position. (read only)<br>
 * For example, if the user follows me, relationTypeToMe is HSP_RELATIONTYPE_FOLLOW. However, if the user blocks me or has no relation with me, it is HSP_RELATIONTYPE_NONE because HSP does not allow the block information to be shown.  
 *
 * @see HSPRelationType
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              HSPRelationType     relationTypeToMe;

/**
 * @brief Whether the nickname is changed or not. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                isNicknameChanged;

/**
 * @brief Age. (read only)<br>
 * -1 if not added.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              int8_t              age;

/**
 * @brief Gender. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              HSPGender           gender;

/**
 * @brief Status message. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSString*           todayWords;

/**
 * @brief Whether to show age. (read only)<br>
 * The default value is YES.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                exposeAge;

/**
 * @brief Whether to show gender. (read only)<br>
 * The default value is YES.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                exposeGender;

/**
 * @brief List of game numbers recently played. (read only)<br>
 * An array of game numbers(long long type) which consist of NSNumbers.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSArray*            playedGameNoList;

/**
 * @brief User game data. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSString*           gameUserData;

/**
 * @brief Returns the number of punishments.
 *
 * @param type Type of punishment.
 * @return The number of punishments.<br>Returns 0 if no punishment.
 *
 * @code
 * int32_t count = [detailedProfile punishmentCountWithType:HSP_PUNISHMENTTYPE_TODAYWORDS];
 * NSLog(@"The number of punishments = %d", count);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (int32_t)punishmentCountWithType:(HSPPunishmentType)type;

/**
 * @brief Returns the date to stop punishment.
 *
 * @param type Type of punishment.
 * @return Date to stop punishment.<br>Returns nil if no punishment.
 *
 * @code
 * NSDate* date = [detailedProfile punishmentEndDateWithType:HSP_PUNISHMENTTYPE_TODAYWORDS];
 * NSLog(@"Date to stop punishment = %@", date);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSDate*)punishmentEndDateWithType:(HSPPunishmentType)type;

/**
 * @brief Returns whether to expose IDs related to the external services.
 * 
 * @param code Service type.
 * @return Whether to expose.
 *
 * @code
 * BOOL isExpose = [detailedProfile isExposeIdpIDWithIdpCode:HSP_IDP_FACEBOOK];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (BOOL)isExposeIdpIDWithIdpCode:(HSPIDPCode)code;

/**
 * @brief Returns the IDs related to the external service.
 * 
 * @param code Service type.
 * @return ID.<br>Returns nil if no ID related to the service.
 *
 * @code
 * NSString* facebookID = [detailedProfile idpIDWithIDPCode:HSP_IDP_FACEBOOK];
 * NSLog(@"Facebook ID: %@", facebookID);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)idpIDWithIDPCode:(HSPIDPCode)code;

/**
 * @brief Returns user names in the address book of the current device.<br>
 * If the user's phone number is the same as the number in the address book, the function returns the name of the number in the address book.<br>
 * If there is no matched number, it returns nil.
 * 
 * @return User name.<br>Returns nil if there is no matched number.
 *
 * @code
 * NSString* name = [detailedProfile nameInAddressBook];
 * NSLog(@"Name of address book : %@", name);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (NSString*)nameInAddressBook;

@end
