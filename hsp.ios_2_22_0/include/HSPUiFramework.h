//
//  HSPUiFramework.h
//  dualWindowTest
//
//  Created by  on 11. 12. 26..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPUiViewController.h"

/**
 * @brief Status mode type of HSP GNB.
 */
typedef enum
{
	/**
	 * @brief Draws horizontal HSP GNB.
	 */
	HSPUI_GNB_HORIZONTAL = 0,

	/**
	 * @brief Draws vertical HSP GNB.
	 */
	HSPUI_GNB_VERTICAL = 1,
} HSPUiGNBMode;

/**
 * @brief Functional definition protocol for each item of HSP GNB.
 */
@protocol HSPUiGNBItem <NSObject>

@required
/**
 * @brief Returns a UIControl instance where HSPGNBItem is drawn.
 * @return UIControl* UIControl instance
 */
- (UIControl*)getUIControl;

/**
 * @brief Returns a URI action of a view mapped to GNB. 
 * @return NSString* URI string.
 */
- (NSString*)getURI;

/**
 * @brief Returns a title of a view mapped to GNB.
 * @return NSString* Title string set for the view.
 */
- (NSString*)getTitle;

/**
 * @brief Gets a badge value set for GNB. 
 * @return NSString*
 */
- (NSString*)badge;

/**
 * @brief Sets a badge value for GNB.
 * @param badge Badge value to set. [in]
 */
- (void)setBadge:(NSString*)badge;

/**
 * @brief Checks if GNB is selected or not.
 * @return BOOL (YES: selected, NO: not selected.)
 */
- (BOOL)isGNBSelected;

/**
 * @brief Sets GNB to be selected.
 * @param selected Whether selected or not (YES: selected, NO: not selected). [in]
 */
- (void)setGNBSelected:(BOOL)selected;

/**
 * @brief Sets HSPUiGNBMode of GNB.
 * @param mode Mode to draw HSPUiGNBMode GNB on a view. [in]
 * @see HSPUiGNBMode
 */
- (void)setGnbMode:(HSPUiGNBMode)mode;

@end

@protocol HSPUiGNBView;

/**
 * @brief Delegate protocol to handle events of HSP GNB view.
 */
@protocol HSPUiGNBViewDelegate <NSObject>

/**
 * @brief Is called when the specified GNB item is selected.
 * @param gnbView HSPUiGNBView instance for which the event occurs [in]
 * @param selectedItem GNBItem for which the event occurs [in]
 * @see HSPUiGNBItem
 */
- (void)didItemSelected:(id<HSPUiGNBView>)gnbView
		   selectedItem:(id<HSPUiGNBItem>)selectedItem;

@end

/**
 * @brief Protocol that abstracts the HSP GNB view.
 */
@protocol HSPUiGNBView <NSObject>

@required
/**
 * @brief Sets GNB items in the GNB view.
 * @param gnbObjects Array of GNB items [in]
 * @see HSPUiGNBItem
 */
- (void)setGnbItems:(NSArray*)gnbObjects;

/**
 * @brief Sets the selected GNB item.
 * @param gnbItem Selected GNB item [in]
 * @see HSPUiGNBItem
 */
- (void)setSelectedGnb:(id<HSPUiGNBItem>)gnbItem;

/**
 * @brief Sets the mode to display the GNB view.
 * @param mode GnbMode to set [in]
 * @see HSPUiGNBMode
 */
- (void)setGnbMode:(HSPUiGNBMode)mode;

/**
 * @brief Sets a delegate to receive events from the GNB view. 
 * @param gnbViewDelegate Delegate to receive events [in]
 * @see HSPUiGNBViewDelegate
 */
- (void)setGnbViewDelegate:(id<HSPUiGNBViewDelegate>)gnbViewDelegate;

/**
 * @brief Gets GNB items stored in the GNB view.
 * @return HSPUiGNBItem Array of instances.
 * @see HSPUiGNBItem
 */
- (NSArray*)gnbItems;

/**
 * @brief Gets the selected GNB item.
 * @return HSPUiGNBItem Instance
 * @see HSPUiGNBItem
 */
- (id<HSPUiGNBItem>)selectedGnb;

/**
 * @brief Gets the current GNBMode.
 * @return HSPUiGNBMode Mode to display GNB items in the view.
 * @see HSPUiGNBMode
 */
- (HSPUiGNBMode)gnbMode;

/**
 * @brief Gets HSPUiGNBViewDelegate instance to receive events.
 * @return HSPUiGNBViewDelegate Instance to receive events of GNB items.
 * @see HSPUiGNBViewDelegate
 */
- (id<HSPUiGNBViewDelegate>)gnbViewDelegate;
  
@end

@protocol HSPUiTopBar;

/**
 * @brief Delegate to handle events from HSPUiTopBar.
 */
@protocol HSPUiTopBarDelegate <NSObject>

/**
 * @brief Notifies that the user clicks top bar.
 * @param topBar Top bar for which an event occurs [in]
 * @see HSPUiTopBar
 */
- (void)didTopBarClicked:(id<HSPUiTopBar>)topBar;

@end

/**
 * @brief Protocol that abstracts the object to be used as HSP top bar. 
 */
@protocol HSPUiTopBar <NSObject>

@required

/**
 * @brief Removes the items in the view controller stack until the entered view controller becomes the top of the stack. 
 * @param viewController View controller to be on the top of the stack [in]
 * @see HSPUiViewController
 */
- (void)popToHSPUiViewController:(HSPUiViewController*)viewController;

/**
 * @brief Adds a new view controller to the top bar.
 * @param viewController View controller to add [in]
 * @see HSPUiViewController
 */
- (void)pushHSPUiViewController:(HSPUiViewController*)viewController;

/**
 * @brief Removes a item from the top bar stack.
 */
- (void)popHSPUiViewController;

/**
 * @brief Removes all items from the top bar stack.
 */
- (void)popAll;

/**
 * @brief Sets the delegate to handle events of the top bar.
 * @param topBarDelegate Delegate to receive events [in]
 * @see HSPUiTopBarDelegate
 */
- (void)setTopBarDelegate:(id<HSPUiTopBarDelegate>)topBarDelegate;

/**
 * @brief Gets the delegate set for the top bar.
 * @return Delegate.
 * @see HSPUiTopBarDelegate
 */
- (id<HSPUiTopBarDelegate>)topBarDelegate;

@end

/**
 * @brief Protocol which abstracts the object to arrange HSP UI layout.
 */
@protocol HSPUiViewFrame <NSObject>

@required
/**
 * @brief Initializes HSPUiViewFrame with the GNB items and the GNB item instance to select. 
 * @param gnbItems Array of HSPUiGNBItems [in]
 * @param selected HSPUiGNBItem to select [in]
 * @return HSPUiViewFrame Instance.
 * @see HSPUiGNBItem
 */
- (id)initWithGNBItems:(NSArray*)gnbItems
	   selectedGnbItem:(id<HSPUiGNBItem>)selected;

/**
 * @brief Creates internal components of HSPUiViewFrame.
 */
- (void)createViewFrame;

/**
 * @brief Sets a new view controller for HSPUiViewFrame.
 * @param viewController View controller to be applied to the client area of view frame [in]
 * @param animate Flag to set whether to apply the animation effect when the view controller is applied to the client area of view frame [in]
 * @see HSPUiViewController
 */
- (void)attachViewFrameToViewController:(HSPUiViewController*)viewController
								animate:(BOOL)animate;

/**
 * @brief Detaches the view controller applied to HSPUiViewFrame.
 */
- (void)detachViewFrameFromViewController;

/**
 * @brief Re-arranges the layout of HSPUiViewFrame. [in]
 * @param interfaceOrientation Current orientation.
 */
- (void)layoutViewFrame:(UIInterfaceOrientation)interfaceOrientation;

/**
 * @brief Gets the client view applied to HSPUiViewFrame.
 * @return UIView* Client applied.
 */
- (UIView*)clientView;

/**
 * @brief Gets the GNB view applied to HSPUiViewFrame.
 * @return HSPUiGNBView GNB view applied.
 * @see HSPUiGNBView
 */
- (id<HSPUiGNBView>)gnbView;

/**
 * @brief Gets the top bar view applied to HSPUiViewFrame.
 * @return HSPUiTopBar Top bar view applied.
 * @see HSPUiTopBar
 */
- (id<HSPUiTopBar>)topBarView;

/**
 * @brief Checks if GNB is shown on the current view frame.
 * @return BOOL (YES: show, NO: not show).
 */
- (BOOL)isShowGNB;

/**
 * @brief Checks if the top bar is shown on the current view frame.
 * @return BOOL (YES: show, NO: not show).
 */
- (BOOL)isShowTopBar;

/**
 * @brief Sets whether to show GNB on the current view frame.
 * @param show Whether to show GNB (YES: show, NO: not show).
 * @param animate Whether to apply animation effect (YES: apply, NO: not apply).
 */
- (void)showGNB:(BOOL)show
		animate:(BOOL)animate;

/**
 * Sets whether to show top bar on the current view frame.
 * @param show Whether to show top bar (YES: show, NO: not show).
 * @param animate Whether to apply animation effect (YES: apply, NO: not apply).
 */
- (void)showTopBar:(BOOL)show
		   animate:(BOOL)animate;

@end

/**
 * @brief Protocol that abstracts the object to handle the stack of HSPUIViewController.
 */
@protocol HSPUiViewContainer <NSObject>

@required
/**
 * @brief Initializes the view container.
 * @param gnbItems Array of HSPUiGNBItems [in]
 * @param uri URI of the root view controller. [in]
 * @return delegate Delegate to receive events from HSPUiViewController.
 * @see HSPUiViewControllerDelegate
 */
- (id)initWithGNBItems:(NSArray*)gnbItems
			   rootUri:(HSPUri*)uri
			  delegate:(id<HSPUiViewControllerDelegate>)delegate;;

/**
 * @brief Checks if GNB is shown on the current view frame.
 * @return BOOL Whether to show GNB (YES: show, NO: not show).
 */
- (BOOL)isShowGNB;

/**
 * @brief Checks if top bar is shown on the current view frame.
 * @return BOOL Whether to show top bar (YES: show, NO: not show).
 */
- (BOOL)isShowTopBar;

/**
 * @brief Checks if GNB is shown on the current view frame.
 * @param show Whether to show GNB (YES: show, NO: not show).
 * @param animate Whether to apply animation effect (YES: apply, NO: not apply)
 */
- (void)showGNB:(BOOL)show
		animate:(BOOL)animate;

/**
 * @brief Sets whether to show top bar on the current view frame.
 * @param show whether to show top bar (YES: show, NO: not show).
 * @param animate Whether to apply animation effect (YES: apply, NO: not apply)
 */
- (void)showTopBar:(BOOL)show
		   animate:(BOOL)animate;

/**
 * @brief Creates a top bar button instance for the pentagonal button to be on the top bar. 
 * @param title Text for the top bar button.
 * @param target Target object to call a method when the top bar button is clicked. 
 * @param selector Selector to call when the top bar button is clicked.
 * @return UIBarButtonItem* Instance of the created top bar button
 */
- (UIBarButtonItem*)prevBarButtonItem:(NSString*)title
							   target:(id<NSObject>)target
							 selector:(SEL)selector;

/**
 * @brief Creates a top bar button instance for the quadrilateral button to be on the top bar.
 * @param title Text for the top bar button.
 * @param target Target object to call a method when the top bar button is clicked.
 * @param selector Selector to call when the top bar button is clicked.
 * @return UIBarButtonItem* Instance of the created top bar button
 */
- (UIBarButtonItem*)rectBarButtonItem:(NSString*)title
							   target:(id<NSObject>)target
							 selector:(SEL)selector;

/**
 * @brief Gets the view frame instance applied to the current container.
 * @return HSPUiViewFrame* View frame instance applied to the current container.
 * @see HSPUiViewFrame
 */
- (id<HSPUiViewFrame>)hspViewFrame;

/**
 * @brief Gets the root view controller.
 * @return HSPUiViewController* root view controller.
 * @see HSPUiViewController
 */
- (HSPUiViewController*)rootHSPViewController;

/**
 * @brief Gets the top view controller.
 * @return HSPUiViewController* top view controller.
 * @see HSPUiViewController
 */
- (HSPUiViewController*)topHSPViewController;

/**
 * @brief Selects a GNB item.
 * @param item GNB item instance to select [in]
 * @see HSPUiGNBItem
 */
- (void)selectGNBWithItem:(id<HSPUiGNBItem>)item;

/**
 * @brief Selects a GNB item.
 * @param uri URI action of GNB item to select [in]
 */
- (void)selectGNBWithUri:(NSString*)uri;

/**
 * @brief Gets a GNB item related to the URI.
 * @param uri URI action of GNB item to get [in]
 * @return HSPUiGNBItem instance.
 * @see HSPUiGNBItem
 */
- (id<HSPUiGNBItem>)gnbItemForUri:(NSString*)uri;

/**
 * @brief Adds a view controller to the stack of HSPUiViewContainer.
 * @param viewController View controller to add [in]
 * @param animate Whether to apply the animation effect [in]
 * @see HSPUiViewController
 */
- (void)pushHSPViewController:(HSPUiViewController*)viewController
					  animate:(BOOL)animate;

/**
 * @brief Removes a view controller from the stact of HSPUiViewContainer.
 * @param animate Whether to apply the animation effect [in]
 * @see HSPUiViewController
 */
- (void)popHSPViewControllerWithAnimate:(BOOL)animate;

/**
 * @brief Removes items from the stack of HSPUiViewContainer until the specified view controller becomes the top of the stack. 
 * @param viewController View controller to be the top of the stack [in]
 * @param animate Whether to apply the animation effect [in]
 * @see HSPUiViewController
 */
- (void)popToHSPViewController:(HSPUiViewController*)viewController
					   animate:(BOOL)animate;

/**
 * @brief Re-arranges the view frame of HSPUiViewContainer.
 * @param interfaceOrientation Current orientation [in]
 */
- (void)layoutViewFrame:(UIInterfaceOrientation)interfaceOrientation;

@end