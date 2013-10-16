//
//  HSPUiLauncher.h
//  HSPUI
//
//  Created by  on 11. 9. 28..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPUiFramework.h"
#import "HSPUiViewController.h"

@class HSPUiLauncher;
@class HSPUiFactory;

@protocol HSPUiLauncherDelegate <NSObject>

- (void)didLaunch:(HSPUiLauncher*)launcher;
- (void)willClose:(HSPUiLauncher*)launcher;

@end

typedef enum
{
	HSP_LAUNCHER_USE_UIVIEWCONTROLLER = 0,
	HSP_LAUNCHER_USE_ANIMATION,
} HSPUiLaunchType;

/**
 * @brief Displays HSP UI objects on the screen.
 */
@interface HSPUiLauncher : NSObject
{
	// for game input
	UIWindow*                   _gameWindow;
	UIView*                     _gameView;
	UIViewController*			_gameViewController;
	NSMutableArray*             _windowContextStack;
	BOOL                        _lockOrientation;

	// for splash view control
	NSTimeInterval              _splashTimeInterval;
	UIViewController*           _splashViewController;

	// for HSP display
	HSPUiLaunchType				_launchType;
	BOOL						_isAnimating;
	HSPUiOrientationType        _supportsOrientation;
	HSPUiOrientationType        _preferredOrientation;

	// for Pop over style
	UIPopoverController*        _popoverController;
	CGSize                      _popoverContentSize;
	CGRect                      _popoverDisplayBaseRect;
	UIPopoverArrowDirection     _popoverArrowDirection;

	// UIFactory for view creation
	HSPUiFactory*               _uiFactory;

	id<HSPUiLauncherDelegate>   _delegate;
}

/**
 * @brief Game window to display HSP UI on the screen.
 */
@property (nonatomic, retain)   UIWindow*                   gameWindow;

/**
 * @brief Game view to display HSP UI on the screen.
 */
@property (nonatomic, retain)   UIView*                     gameView;

/**
 * @brief Game view controller to display HSP UI on the screen.
 */
@property (nonatomic, retain)   UIViewController*			gameViewController;

/**
 * @brief Interface to lock HSP UI orientation.
 */
@property (nonatomic)           BOOL                        lockInterfaceOrientation;

/**
 * @brief Interface to set supported game orientations.
 */
@property (nonatomic)           HSPUiOrientationType        supportsOrientation;

/**
 * @brief Interface to set preferred game orientations.
 */
@property (nonatomic)           HSPUiOrientationType        preferredOrientation;

/**
 * @brief Delegate to receive events that occur in HSP UI.
 */
@property (nonatomic, assign)   id<HSPUiLauncherDelegate>	delegate;

/**
 * @brief Time interval to show splash.
 */
@property (nonatomic)           NSTimeInterval              splashTimeInterval;

/**
 * @brief View controller currently splashed.
 */
@property (nonatomic, retain)   UIViewController*           splashViewController;

/**
 * @brief Animation effect to be applied when HSP view is displayed on the screen. 
 */
@property (nonatomic)			HSPUiLaunchType				launchType;

/**
 * @brief Whether the current view controller on the screen is pop-over type or not.
 */
@property (nonatomic, readonly) BOOL                        isPopover;

/**
 * @brief The view controller currently displayed on the screen.
 */
@property (nonatomic, readonly) UIViewController*           viewController;

/**
 * @brief Whether the current HSP view is appearing or disappearing.
 */
@property (nonatomic, readonly)	BOOL						isAnimating;

/**
 * @brief Size of view when using popover style in iPad.
 */
@property (nonatomic)           CGSize                      popoverContentSize;

/**
 * @brief Base rectangular area of view when using popover style in iPad.
 */
@property (nonatomic)           CGRect                      popoverDisplayBaseRect;

/**
 * @brief Arrow style of view when using popover style in iPad.
 */
@property (nonatomic)           UIPopoverArrowDirection     popoverArrowDirection;

/**
 * @brief HSPUiFactory instance which knows how to create a object. 
 */
@property (nonatomic, retain)   HSPUiFactory*               uiFactory;

/**
 * @brief Gets singleton instance of HSPUiLauncher.
 * @return HSPUiLauncher instance.
 */
+ (HSPUiLauncher*)sharedLauncher;

/**
 * @brief Displays the view container containing the view controller related to the specified URI. 
 * Since using grand centeral dispatch (GCD) causes collision with animation RunLoop, you need to use performSelector or directly call API.
 * @param uri URI Scheme of the view controller to display [in]
 * @param delegate Delegate to handle events of the view controller [in]
 * @param animated Whether to apply animation effect [in]
 * @return BOOL (YES : success, NO : fail).
 */
- (BOOL)launchWithUri:(HSPUri*)uri delegate:(id<HSPUiViewControllerDelegate>)delegate animated:(BOOL)animated;

/**
 * @brief Disaplays the specified view controller.
 * Since using grand centeral dispatch (GCD) causes collision with animation RunLoop, you need to use performSelector or directly call API.
 * @param viewController View controller to display [in]
 * @param animated Whether to apply animation effect [in]
 * @return BOOL Execution result (YES: success, NO: fail).
 */
- (BOOL)launch:(UIViewController*)viewController animated:(BOOL)animated;

/**
 * @brief Closes HSPUI displayed on the screen.
 * Since using grand centeral dispatch (GCD) causes collision with animation RunLoop, you need to use performSelector or directly call API.
 * @param animated Whether to apply animation effect [in]
 * @return BOOL Execution result (YES: success, NO: fail).
 */
- (BOOL)closeAnimated:(BOOL)animated;

/**
 * @brief Displays the view controller temporarily.
 * Since using grand centeral dispatch (GCD) causes collision with animation RunLoop, you need to use performSelector or directly call API.
 * @param viewController View controller to display [in]
 * @param timeInterval Time interval to display [in]
 * @return BOOL Execution result (YES: success, NO: fail).
 */
- (BOOL)showSplash:(UIViewController*)viewController timeInterval:(NSTimeInterval)timeInterval;

/**
 * @brief Hides the splash screen.
 * Since using grand centeral dispatch (GCD) causes collision with animation RunLoop, you need to use performSelector or directly call API.
 */
- (void)hideSplash;

/**
 * @brief Checks if the game is set to use popover style in iPad.
 * @return BOOL Setting (YES: Popover style, NO: Modal style).
 */
- (BOOL)isDiplayStylePopover;

/**
 * @brief Checks if the URI is supported by HSPUi.
 * @param uri URI to check
 * @return BOOL Whether it is supported by HSPUi or not (YES: supported, NO: not supported).
 * @see HSPUri
 */
- (BOOL)isSupportWithUri:(HSPUri*)uri;

/**
 * @brief Checks if the HSP related UI is displayed. 
 * @return BOOL Whether HSP related UI is displayed or not (YES: displayed, NO: not displayed).
 */
- (BOOL)hasViewController;

/**
 * @brief Returns URI of the view which is displayed on the top. 
 * @return HSPUri* URI of the view displayed on the top.
 * @see HSPUri
 */
- (HSPUri*)topViewURI;

/**
 * @brief Sets the interface orientation supported by the currently displayed view controller.
 * @param orientationType Supported orientation. [in]
 */
- (void)setUIOrientationForViewController:(HSPUiOrientationType)orientationType;

/**
 * @brief Gets the interface orientation supported by the currently displayed view controller.
 * @return HSPUiOrientationType. Supported orientation.
 */
- (HSPUiOrientationType)UIOrientationForViewController;

/**
 * @brief Checks if the current game supports specific orientation.
 * @param interfaceOrientation Orientation to check [in]
 * @return BOOL Whether to support (YES: support, NO: not support).
 */
- (BOOL)isApplicationSupportInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;

@end
