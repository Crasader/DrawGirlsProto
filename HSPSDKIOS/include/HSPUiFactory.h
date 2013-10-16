//
//  HSPUiFactory.h
//  HSPUI
//
//  Created by  on 11. 10. 21..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPUri.h"

@protocol HSPUiViewControllerDelegate;
@class HSPUiViewController;

/**
 * @brief A factory interface that abstracts creation of HSP UI objects.
 */
@interface HSPUiFactory : NSObject
{
	NSMutableDictionary*   _uriDictionary;
}

/**
 * @brief Adds how to create a view controller related to the specified URI to the view controller creation map.
 * @param URIAction URI action of a view controller to create [in]
 * @param classType Class type of a view controller to create [in]
 * @param nibName Xib file name of a view controller to create [in]
 * @param defaultTitle Default title of a view controller to create [in]
 * @return BOOL (YES : success, NO : fail).
 */
- (BOOL)setUri:(NSString*)URIAction withClassType:(id)classType nibName:(NSString*)nibName defaultTitle:(NSString*)defaultTitle;

/**
 * @brief  Checks if the factory knows how to create a view controller related to the URI. 
 * @param uri URI of a view controller to create [in]
 * @return BOOL (YES : supported, NO : not supported).
 * @see HSPUri
 */
- (BOOL)isSupportWithUri:(HSPUri*)uri;

/**
 * @brief Creates a view container related to the URI. 
 * @param uri URI of a view container to create [in]
 * @param delegate Delegate instance to handle events for a view container to create [in]
 * @return UIViewController* UIViewController type instance of the created view container [in]
 * @see HSPUri, HSPUiViewControllerDelegate
 */
- (UIViewController*)HSPUiViewContainerWithUri:(HSPUri*)uri delegate:(id<HSPUiViewControllerDelegate>)delegate;

/**
 * @brief Creates a client view controller related to the URI. 
 * @param uri URI of a view controller to create [in]
 * @param delegate Delegate instance to handle events for a view controller to create [in]
 * @return HSPUiViewController* Instance of the created view controller [in]
 * @see HSPUri, HSPUiViewControllerDelegate
 */
- (HSPUiViewController*)HSPUiClientViewControllerWithUri:(HSPUri*)uri delegate:(id<HSPUiViewControllerDelegate>)delegate;

@end
