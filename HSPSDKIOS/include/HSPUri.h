//
//  HSPUri.h
//  HSPCore
//
//  Created by  on 11. 10. 19..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * @brief A URI of GamePlus WebView
 */
#define HSP_COMMON_URI_GAMEPLUS					(@"HSPUI://gameplus")

/**
 * @brief A URI of Hangame CSCenter WebView
 */
#define HSP_COMMON_URI_SUPPORT_CSCENTER			(@"HSPUI://supports.cscenter")

/**
 * @brief A URI of Hangame Notice Page WebView
 */
#define HSP_COMMON_URI_SUPPORT_NOTICE			(@"HSPUI://supports.notice")

/**
 * @brief A URI of Hangame FAQ Page WebView
 */
#define HSP_COMMON_URI_SUPPORT_FAQ				(@"HSPUI://supports.faq")

/**
 * @brief A URI of Hangame Privacy terms WebView
 */
#define HSP_COMMON_URI_SUPPORT_TERMS_PRIVACY	(@"HSPUI://supports.terms.privacy")

/**
 * @brief A URI of Hangame Use terms WebView
 */
#define HSP_COMMON_URI_SUPPORT_TERMS_USE		(@"HSPUI://supports.terms.use")

/**
 * @brief A URI of Hangame Hangame terms WebView
 */
#define HSP_COMMON_URI_SUPPORT_TERMS_HANGAME	(@"HSPUI://supports.terms.hangame")

/**
 * @brief A URI of Hangame Punish terms WebView
 */
#define HSP_COMMON_URI_SUPPORT_TERMS_PUNISH		(@"HSPUI://supports.terms.punish")

/**
 * @brief A URI of Toast Event WebView
 */
#define HSP_COMMON_URI_SUPPORT_TOAST_EVENT		(@"HSPUI://supports.toast.event")

/**
 * @brief Parses URI used in HSP.
 */
@interface HSPUri : NSObject<NSCopying>
{
	BOOL                   _autoURLEncoding;
	NSMutableDictionary*   _parsed;
}

/**
 * Flag to set whether to use parameter URLEncoding of HSPUri object.
 */
@property (nonatomic, assign)   BOOL autoURLEncoding;

/**
 * Creates an HSPUri instance by parsing string type URI.
 * @param string URI to create [in]
 * @return HSPUri* Created HSPUri instance.
 */
+ (HSPUri*)uriWithString:(NSString*)string;

/**
 * Creates an HSPUri instance by parsing string type URI.
 * @param string URI to create [in]
 * @param autoURLEncoding Sets whether to use parameter URLEncoding of HSPUri object. [in]
 * @return HSPUri* Created HSPUri instance.
 */
+ (HSPUri*)uriWithString:(NSString*)string autoURLEncoding:(BOOL)autoURLEncoding;

/**
 * Initializes the HSPUri instance by parsing string type URI.
 * @param string URI to create [in]
 * @param autoURLEncoding Sets whether to use parameter URLEncoding of HSPUri object. [in]
 * @return HSPUri* Created HSPUri instance.
 */
- (id)initWithString:(NSString*)string autoURLEncoding:(BOOL)autoURLEncoding;

/**
 * Gets a string type URI from HSPUri instance.
 * @return NSString* URI string.
 */
- (NSString*)uri;

/**
 * Gets a scheme from the parsed HSPUri instance.
 * @return NSString* scheme string (e.g.: HSPUI://, HSP://, HTTP://, ...)
 */
- (NSString*)scheme;

/**
 * Gets an action from the parsed HSPUri instance.
 * @return NSString* action string (e.g.: profile.myprofile, GetMemberNo, ...)
 */
- (NSString*)action;

/**
 * Gets parameters from the parsed HSPUri instance.
 * @return NSDictionary* Dictionary of parameters (you can get detailed parameters using key - value pairs).
 * @see - (NSString*) paramForKey:(NSString *)key
 */
- (NSDictionary*)params;

/**
 * Gets detailed parameters for the specified key from the parsed HSPUri instance.
 * @param key Key of the parameter to get [in]
 * @return NSString* Parameter value for the key.
 * @see - (NSDictionary*) params
 */
- (NSString*)paramForKey:(NSString *)key;

/**
 * Parses a string type URI and stores it in the HSPUri instance.
 * @param string URI to create [in]
 */
- (void)setUri:(NSString*)uri;

/**
 * Sets a scheme for the HSPUri instance. This setting overwrites any existing value. 
 * @param scheme Scheme to set [in]
 */
- (void)setScheme:(NSString*)scheme;

/**
 * Sets an action for the HSPUri instance. This setting overwrites any existing value.
 * @param action Action to set [in]
 */
- (void)setAction:(NSString*)action;

/**
 * Sets a parameter for the HSPUri instance. This setting overwrites any existing value.
 * @param param Parameter value to set [in]
 * @param key Parameter key to set [in]
 */
- (void)setParam:(NSString*)param forKey:(NSString*)key;

/**
 * Removes the parameter that maches the specified key from the HSPUri instance.
 * @param key Parameter key to set [in]
 */
- (void)removeParamForKey:(NSString*)key;

/**
 * Creates a key by combining scheme://action of the HSPUri instance.
 * @return NSString* scheme + "://" + action combined character string.
 */
- (NSString*)key;

/**
 * Compares HSPUri instances.
 * Compares scheme and action values and returns the result.
 * @param param Object to compare [in]
 * @return BOOL Result (YES: same, NO: different)
 */
- (BOOL)isEqual:(id)object;

/**
 * Compares HSPUri instances.
 * Compares scheme and action values and returns the result.
 * @param param Object to compare [in]
 * @return NSComparisonResult Instance comparison result
 */
- (NSComparisonResult)compare:(id)object;

@end
