//
//  HSPServiceProperties.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 21..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief enum for web name.
 */
typedef enum
{
    HSP_WEBNAME_PRIVACY_URL				= 0,	/**< privacyUrl web name. */
    HSP_WEBNAME_TERMS_URL				= 1,	/**< termsUrl web name. */
	HSP_WEBNAME_SMART_TERMS_URL			= 2,	/**< smartTermsURL web name. */
    HSP_WEBNAME_DOWNLOAD_URL			= 3,	/**< URL download web name.  */
    HSP_WEBNAME_SMS_CERT_HELP_URL		= 4,	/**< SMS certification web name. */
    HSP_WEBNAME_FAQ_URL					= 5,	/**< FAQ web name. */
    HSP_WEBNAME_PUNISH_INFO_URL			= 6,	/**< Punishment information web name. */
    HSP_WEBNAME_MINI_BANNER_URL			= 7,	/**< Mini banner web name. */
    HSP_WEBNAME_WEB_CS_URL				= 8,	/**< CS web name. */
    HSP_WEBNAME_GAME_REDIRECTION		= 9,	/**< Game Redirection server's web name */
    HSP_WEBNAME_WEBUIBASE				= 10,	/**< HSP Web UI server's web name */
    HSP_WEBNAME_MAINTENANCE_INFO_URL	= 11,	/**< maintenance info name */
	HSP_WEBNAME_PUNISH_REASON_URL		= 12,	/**< Punishment reason info web name */
	HSP_WEBNAME_CS_URL					= 13,	/**< CS web name */
} HSPWebName;

/**
 * @brief enum for server name.
 */
typedef enum
{
    HSP_SERVERNAME_NOTICE               = 0,    /**< Notice server name. */
    HSP_SERVERNAME_GAME_INDICATOR_BIP   = 1,    /**< GAME_INDICATOR_BIP server (a type of BIP server) name. */
    HSP_SERVERNAME_MFS                  = 2,    /**< MFS server name. */
    HSP_SERVERNAME_CS                   = 3,    /**< CS server name. */
    HSP_SERVERNAME_IMAGE                = 4,    /**< Image server name. */
    HSP_SERVERNAME_IMAGE_RESIZE         = 5,    /**< Image resize server name. */
    HSP_SERVERNAME_GAME_PLUS            = 6,    /**< Game plus server name. */
    HSP_SERVERNAME_STABILITY_BIP        = 7,    /**< STABILITY_BIP server (a type of BIP server) name. */
    HSP_SERVERNAME_SILOS                = 8,    /**< SILOS server name. */
    HSP_SERVERNAME_LOGIN                = 9,    /**< Login server name. */
    HSP_SERVERNAME_GAMESVR              = 10,   /**< Game server name. */
    HSP_SERVERNAME_NOMAD                = 11,   /**< Mashup server name. */
    HSP_SERVERNAME_RTA_BIP              = 12,   /**< RTA_BIP server (a type of BIP server) name. */
    HSP_SERVERNAME_PHOTO                = 13,   /**< Photo server name. */
    HSP_SERVERNAME_LCS                  = 14,   /**< LCS (location service) server name. */
    HSP_SERVERNAME_LIGHTHOUSE           = 15    /**< Light house server name. */
} HSPServerName;

/**
 * @brief enum for launching status.
 */
typedef enum
{
    HSP_LAUNCHINGSTATE_OK					= 0,            /**< Game launching state is OK. */
    HSP_LAUNCHINGSTATE_GAME_INFO_FAIL		= 1,            /**< Failed to load game information. */
    HSP_LAUNCHINGSTATE_CLIENT_VERSION_FAIL	= 2,            /**< Failed to check client version. */
    HSP_LAUNCHINGSTATE_CLIENT_LATESET_VERSION_EXIST	= 3,    /**< Service available, but the latest version exists. */
    HSP_LAUNCHINGSTATE_HANGAME_INSPECTION	= 4,            /**< Inspecting Hangame. */
    HSP_LAUNCHINGSTATE_GAME_INSPECTION		= 8,            /**< Inspecing game server. */
    HSP_LAUNCHINGSTATE_PLATFORM_INSPECTION	= 10,           /**< Inspecing platform. */
    HSP_LAUNCHINGSTATE_RESOURCE_UPDATE_FAIL	= 20,           /**< Failed to update resources. */
    HSP_LAUNCHINGSTATE_BLOCKED_DEVICE		= 91,           /**< Blocked device. */
    HSP_LAUNCHINGSTATE_NOT_USABLE_DEVICE	= 92,           /**< Not usable device. */
    HSP_LAUNCHINGSTATE_SERVICE_CLOSED		= 98,           /**< Service closed. */
    HSP_LAUNCHINGSTATE_INTERNAL_ERROR		= 99,           /**< Internal error. */
    HSP_LAUNCHINGSTATE_ACCESS_DENIED		= 0XFF,         /**< Access denied. */
} HSPLaunchingState;

/**
 * @brief Deals with service property information received from the HSP server.
 */
@interface HSPServiceProperties : NSObject {

}

/**
 * @brief Requests update of service property information. 
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.  
 *
 * @code
 * HSPServiceProperties* properties = [HSPCore sharedHSPCore].serviceProperties;
 * [serviceProperties loadServicePropertiesWithCompletionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully updated service property information.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to update service property information.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)                 loadServicePropertiesWithCompletionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Returns a dictionary that has service property information.
 *
 * @return Service property information dictionary.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(NSDictionary*)        servicePropertiesDictionary;

/**
 * @brief Returns a key value of service property information.
 * 
 * @param key Key.
 * @return Value.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(NSObject*)            valueFromKey:(NSString*)key;

/**
 * @brief Returns a web URL from the web name.
 * 
 * @param webName Web name.
 * @return Web URL.
 *
 * @code
 * HSPServiceProperties* properties = [HSPCore sharedHSPCore].serviceProperties;
 * NSString* url = [serviceProperties webURLFromName:HSP_WEBNAME_SMART_TERMS_URL];
 * NSLog(@"Web URL : %@", url);
 * @endcode  
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(NSString*)            webURLFromName:(HSPWebName)webName;

/**
 * @brief Returns a server address from the server name.
 * 
 * @param serverName Server name.
 * @return Server address.
 *
 * @code
 * HSPServiceProperties* properties = [HSPCore sharedHSPCore].serviceProperties;
 * NSString* address = [serviceProperties serverAddressFromName:HSP_SERVERNAME_PHOTO];
 * NSLog(@"Photo server address : %@", address);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(NSString*)            serverAddressFromName:(HSPServerName)serverName;

/**
 * @brief Returns launching state.
 * 
 * @return Launching Launching state.
 * @see HSPLaunchingState
 *
 * @code
 * HSPServiceProperties* properties = [HSPCore sharedHSPCore].serviceProperties;
 * HSPLaunchingState state = [properties launchingState];
 * switch ( state )
 * {
 * case HSP_LAUNCHINGSTATE_OK:
 * NSLog(@"HSPLaunchingState : OK");
 * break;
 * case HSP_LAUNCHINGSTATE_GAME_INFO_FAIL:
 * NSLog(@"HSPLaunchingState : Not registered game.");
 * break;
 * case HSP_LAUNCHINGSTATE_CLIENT_VERSION_FAIL:
 * NSLog(@"HSPLaunchingState : The latest version is found.");
 * break;
 * }
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(HSPLaunchingState)    launchingState;

/**
 * @brief Returns the maximum ranking size.<br>The list of rankings does not exceed the maximum ranking size. 
 * 
 * @return Ranking size.
 *
 * @code
 * HSPServiceProperties* properties = [HSPCore sharedHSPCore].serviceProperties;
 * int32_t size = [serviceProperties serverAddressFromName:HSP_SERVERNAME_PHOTO];
 * NSLog(@"Max ranking size : %d", size);
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(int32_t)              maxRankingSize;

@end
