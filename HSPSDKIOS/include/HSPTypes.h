//
//  HSPTypes.h
//  HSPCore
//
//  Created by  on 11. 10. 11..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#ifndef HSP_HSPTypes_h
#define HSP_HSPTypes_h

/**
 * Defines basic types used in HSP.
 */
typedef long long			INT64;
typedef int					INT32;
typedef short				INT16;
typedef signed char			INT8;

typedef unsigned long long	UINT64;
typedef unsigned int		UINT32;
typedef unsigned short		UINT16;
typedef unsigned char		UINT8;

/**
 * Defines device screen types.
 */
typedef enum
{
	/**
	 * iPhone.
	 */
	IOS_320X480		= 320 * 480,

	/**
	 * iPad.
	 */
	IOS_768X1024	= 768 * 1024,

	/**
	 * Unknown.
	 */
	IOS_UNKNOWN
} HSPScreenType;

/**
 * Defines debug levels of HSPSDK.
 */
typedef enum 
{
	/**
	 * NONE - Displays no log.
	 */
	HSP_DEBUGLEVEL_NONE,
	/**
	 * LOW - Displays the logs using HSP_LOG_ERROR.
	 */
	HSP_DEBUGLEVEL_LOW,
	/**
	 * MIDDLE - Displays the logs using HSP_LOG_ERROR and HSP_LOG_DEBUG.
	 */
	HSP_DEBUGLEVEL_MIDDLE,
	/**
	 * HIGH - Displays the logs using HSP_LOG_ERROR, HSP_LOG_DEBUG, and HSP_LOG_INFO.
	 */
	HSP_DEBUGLEVEL_HIGH
} HSPDebugLevel;

/**
 * Defines OS types where HSP is applied. 
 */
typedef enum 
{
	/**
	 * Unknown.
	 */
	HSP_PLATFORM_INVALID = 0,
	/**
	 * iOS.
	 */
	HSP_PLATFORM_IOS = 1,
	/**
	 * Android.
	 */
	HSP_PLATFORM_ANDROID = 2,
} HSPOSType;

/**
 * Defines network status types.
 */
typedef enum 
{
	/**
	 * Not connected.
	 */
	HSP_NOT_REACHABLE = 0,

	/**
	 * Connected with WIFI.
	 */
	HSP_REACHABLE_VIA_WIFI = 1,

	/**
	 * Connected with cellular network.
	 */
	HSP_REACHABLE_VIA_WWAN = 2,
} HSPNetworkType;

/**
 * Defines HSP status types.
 */
typedef enum 
{
	/**
	 * HSP Initial State - HSP not available.
	 */
	HSP_STATE_INIT = 0,

	/**
	 * Logged in successfully - HSP available.
	 */
	HSP_STATE_ONLINE,

	/**
	 * Not authorized - HSP not available.
	 */
	HSP_STATE_OFFLINE,

	/**
	 * HSP not available - Other issues including inspection or failure.
	 */
	HSP_STATE_UNAVAILABLE,
} HSPState;

/**
 * Defines HSP login types.
 */
typedef enum
{
	/**
	 * Unknown(Not loggined or unknown login type).
	 */
	HSP_LOGIN_TYPE_UNKNOWN = 0,

	/**
	 * Guest Login.
	 */
	HSP_LOGIN_TYPE_GUEST,

	/**
	 * G Square Login.
	 */
	HSP_LOGIN_TYPE_GSQUARE,
	
	/**
	 * Facebook Login.
	 */
	HSP_LOGIN_TYPE_FACEBOOK,
	
	/**
	 * Facebook Login.
	 */
	HSP_LOGIN_TYPE_KAKAO,
	
	/**
	 * Toast Login.
	 */
	HSP_LOGIN_TYPE_TOAST,
	
	/**
	 * Gamecenter Login.
	 */
	HSP_LOGIN_TYPE_GAMECENTER,
	
	/**
	 * Naver Login.
	 */
	HSP_LOGIN_TYPE_NAVER
} HSPLoginType;

/**
 * Defines HSP Age Requirement types.
 */
typedef enum
{
	/**
     * HSP Kid age - NOT Permitted by parents
     */
	HSP_KID_NOT_PERMITTED_BY_PARENTS = 0,
	
	/**
     * HSP Kid age - Permitted by parents
     */
	HSP_KID_PERMITTED_BY_PARENTS,
	
	/**
     * HSP Adult age
     */
	HSP_ADULT,
	
	/**
     * HSP Adult - Already authenticated as adult
     */
	HSP_ALREADY_AUTHENTICATED_AS_ADULT,
} HSPAgeRequirement;

/**
 * Defines JOGA Web View Result types.
 */
typedef enum
{
	/**
     * HSP JOGA Agreed by User
     */
	HSP_JOGA_AGREE = 0,
	
	/**
     * HSP JOGA Canceled by User
     */
	HSP_JOGA_CANCELED,
	
	/**
     * HSP JOGA Already agreed by user
     */
	HSP_JOGA_ALREADY_AGREED,
} HSPJogaResult;

#endif
