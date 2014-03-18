//
//  HSPUiViewController.h
//  HSPProjects
//
//  Created by  on 11. 11. 3..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#import "HSPUri.h"
#import "HSPUiFactory.h"

/**
 * Defines common key values used as parameters when HSP UI is launched through URI.
 */
#define HSPUI_URI_PARAM_TOPBAR_SHOW_KEY          (@"_topbarShow")		/**> Whether to show top bar or not. e.g.)topbarShow=FALSE */
#define HSPUI_URI_PARAM_GNB_SHOW_KEY             (@"_gnbShow")			/**> Whether to show gnb bar or not. e.g.)gnbShow=FALSE */
#define HSPUI_URI_PARAM_TITLE_KEY                (@"_title")			/**> Sets the title of view. e.g.)title=My info */
#define HSPUI_URI_PARAM_ORIENTATION_KEY          (@"_orientation")		/**> Sets whether to lock orientation of view. e.g.)orientation=[PORTRAIT | LANDSCAPE | BOTH] */
#define HSPUI_URI_PARAM_MEMBERNO_KEY             (@"memberNo")			/**> Passes memberno used for view. e.g.)memberNo=XXXXXXXXXX */
#define HSPUI_URI_PARAM_OWNERMEMBERNO_KEY        (@"ownermemberNo")		/**> Passes the memberno of owner for the view which needs membernos of two users. e.g.)ownermemberNo=XXXXXXXXXX */
#define HSPUI_URI_PARAM_OPPONENTMEMBERNO_KEY     (@"opponentmemberNo")	/**> Passes the memberno of opponent for the view which needs membernos of two users. e.g.)opponentmemberNo=XXXXXXXXXX */
#define HSPUI_URI_PARAM_GAMENO_KEY               (@"gameNo")			/**> Passes gameno used for the view. e.g.)gameNo=XXXXXXXXXX */
#define HSPUI_URI_PARAM_RANKINGFACTOR_KEY        (@"rankingFactor")		/**> Passes the ranking factor used for the ranking view. e.g.)rankingFactor=XXXXXXXXXX */
#define HSPUI_URI_PARAM_IMAGEURL_KEY             (@"imageUrl")			/**> Passes image URL used for the image view. e.g.)imageUrl=XXXXXXXXXX */
#define HSPUI_URI_PARAM_PHONENO_KEY              (@"phoneNo")			/**> Passes the phone number used for SMS certification view. e.g.)phoneNo=XXXXXXXXXX */
#define HSPUI_URI_PARAM_EMAIL_KEY                (@"email")				/**> Passes the email used for SMS certification view. e.g.)email=XXXXXXXXXX */

typedef NSUInteger HSPUiOrientationType;
enum
{
	HSPUI_ORIENTATION_PORTRAIT				= 0X01,
	HSPUI_ORIENTATION_PORTRAIT_UPSIDEDOWN	= 0X02,
	HSPUI_ORIENTATION_LANDSCAPE_LEFT		= 0X04,
	HSPUI_ORIENTATION_LANDSCAPE_RIGHT		= 0X08,
	HSPUI_ORIENTATION_PORTRAIT_ALL			= HSPUI_ORIENTATION_PORTRAIT | HSPUI_ORIENTATION_PORTRAIT_UPSIDEDOWN,
	HSPUI_ORIENTATION_LANDSCAPE_ALL			= HSPUI_ORIENTATION_LANDSCAPE_LEFT | HSPUI_ORIENTATION_LANDSCAPE_RIGHT,
	HSPUI_ORIENTATION_ALL					= HSPUI_ORIENTATION_PORTRAIT_ALL | HSPUI_ORIENTATION_LANDSCAPE_ALL,
};

@protocol HSPUiViewContainer;
@protocol HSPUiViewControllerDelegate;

/**
 * Abstracts behaviors of HSPUiViewController.
 */
@protocol HSPUiViewController<NSObject>
@required
/**
 * Initializes HSPUiViewController (created based on Xib).
 * @param nibNameOrNil Xib file name of view controller [in]
 * @param nibBundleOrNil Name of bundle where xib of view controller is located [in]
 * @param uiFactory HSPUiFactory inatance which knows how to create a view controller [in]
 * @param uri URI to be passed to the view controller [in]
 * @param delegate Delegate instance to handle events from the view controller [in]
 * @return HSPUiViewController instance.
 * @see HSPUiFactory, HSPUri, HSPUiViewControllerDelegate
 */
- (id)initWithNibName:(NSString*)nibNameOrNil
               bundle:(NSBundle *)nibBundleOrNil
            uiFactory:(HSPUiFactory*)uiFactory
				  uri:(HSPUri*)uri
             delegate:(id<HSPUiViewControllerDelegate>)delegate;

/**
 * Initializes HSPUiViewController (created without Xib).
 * @param uiFactory HSPUiFactory instance which knows how to create a view controller [in]
 * @param uri URI to be passed to the view controller [in]
 * @param delegate Delegate instance to handle events from the view controller [in]
 * @return HSPUiViewController instance.
 * @see HSPUiFactory, HSPUri, HSPUiViewControllerDelegate
 */
- (id)initWithFactory:(HSPUiFactory*)uiFactory
				  uri:(HSPUri*)uri
             delegate:(id<HSPUiViewControllerDelegate>)delegate;

/**
 * Gets the URI instance stored in HSPUiViewController.
 * @return HSPUri* URI instance.
 * @see HSPUri
 */
- (HSPUri*)uri;

/**
 * Gets HSPUiFactory instance stored in HSPUiViewController.
 * @return HSPUiFactory* UI Factory instance.
 * @see HSPUiFactory
 */
- (HSPUiFactory*)uiFactory;

/**
 * Gets HSPUiViewControllerDelegate inatance stored in HSPUiViewController.
 * @return id<HSPUiViewControllerDelegate> delegate instance.
 * @see HSPUiViewControllerDelegate
 */
- (id<HSPUiViewControllerDelegate>)hspUiDelegate;

/**
 * Sets interface orientation of the view controller. 
 * @param orientation Supported UIOrientationType.
 * @see HSPUiOrientationType
 */
- (void)setUIOrientation:(HSPUiOrientationType)orientation;

/**
 * Gets interface orientation of the view controller.
 * @return Supported UIOrientationType.
 * @see HSPUiOrientationType
 */
- (HSPUiOrientationType)UIOrientation;

@end

/**
 * Handles the events from HSPUiViewController.
 */
@protocol HSPUiViewControllerDelegate<NSObject>
@optional
/**
 * Notifies that an event occurs in HSPUiViewController.
 * @param event Event [in]
          <BR> NSDictionary type event in the form of key - value is received.
 * <TABLE border="1">
 * <TR><TH>  key </TH><TH> 	value </TH></TR>
 * <TR><TD>    uri </TD>       <TD>URI of the view where an event occur. </TD></TR>
 * <TR><TD>        control</TD>	<TD> closeAll: Close HSP UI. 
 *                    <BR> back: Close the current screen. 
 *                    <BR> refresh: Refresh the screen because the information is updated. </TD></TR>
 * <TR><TD>        action</TD><TD>     success: Successfully executed in the current screen. 
 *                    <BR> failed: Execution failed in the current screen.
 *                  <BR> canceled: Canceled by a user. </TD></TR>
 * </TABLE>				
 * @param viewController View controller where an event occurs [in]
 * @return id<HSPUiViewControllerDelegate> delegate instance.
 * @see HSPUiViewController
 */
- (void)receivedEvent:(NSDictionary*)event fromViewController:(UIViewController<HSPUiViewController>*)viewController;
@end


/**
 * @brief Default type of UIViewController used in HSP.
 *
 * To customize HSP UI, you need to inherit HSPUiViewController.
 */
@interface HSPUiViewController : UIViewController<HSPUiViewController, HSPUiViewControllerDelegate>
{
	HSPUri*                         _uri;
	HSPUiFactory*                   _uiFactory;
	HSPUiOrientationType            _UIOrientation;

	UIView*                         _activityIndicatorView;

	id<HSPUiViewContainer>          _hspViewContainer;
	id<HSPUiViewControllerDelegate> _hspUiDelegate;
}

/**
 * URI where the view controller is called.
 * @see HSPUri
 */
@property (nonatomic, retain)        HSPUri*                   uri;

/**
 * HSPUiFactory instance which knows how to create HSP UI.
 * @see HSPUiFactory
 */
@property (nonatomic, retain)        HSPUiFactory*                   uiFactory;

/**
 * Orientation setting supported by the view controller (default: HSPUI_ORIENTATION_BOTH)
 */
@property (nonatomic, assign)        HSPUiOrientationType            UIOrientation;

/**
 * Contains the layout of HSP UI and manages the view stack of HSPUiViewController.
 * @see HSPUiViewContainer
 */
@property (nonatomic, assign)        id<HSPUiViewContainer>          hspViewContainer;

/**
 * Handles the events received from the view controller.
 * @see HSPUiViewControllerDelegate
 */
@property (nonatomic, assign)        id<HSPUiViewControllerDelegate> hspUiDelegate;

/**
 * Initializes HSPUiViewController (created based on Xib).
 * @param nibNameOrNil Xib file name of view controller [in]
 * @param nibBundleOrNil Name of bundle where xib of view controller is located [in]
 * @param uiFactory HSPUiFactory instance which knows how to create a view controller [in]
 * @param uri URI to be passed to the view controller [in]
 * @param delegate Delegate instance to handle the events from the view controller [in]
 * @return HSPUiViewController instance.
 * @see HSPUiFactory, HSPUri, HSPUiViewControllerDelegate
 */
- (id)initWithNibName:(NSString*)nibNameOrNil
               bundle:(NSBundle *)nibBundleOrNil
            uiFactory:(HSPUiFactory*)uiFactory 
            uri:(HSPUri*)uri
             delegate:(id<HSPUiViewControllerDelegate>)delegate;

/**
 * Initializes HSPUiViewController (created without Xib).
 * @param uiFactory HSPUiFactory instance which knows how to create a view controller [in]
 * @param uri URI to be passed to the view controller [in]
 * @param delegate Delegate instance to handle the events from the veiw controller [in]
 * @return HSPUiViewController instance.
 * @see HSPUiFactory, HSPUri, HSPUiViewControllerDelegate
 */
- (id)initWithFactory:(HSPUiFactory*)uiFactory 
            uri:(HSPUri*)uri
             delegate:(id<HSPUiViewControllerDelegate>)delegate;

/**
 * Reloads HSPUiViewController.
 */
- (void)reloadView;

/**
 * Creates a control button (similar to Previous button) in the top bar of HSPUiViewContainer.
 * @param title Title of the button [in]
 * @param target Instance to handle when the button is clicked [in]
 * @param selector Selector to be handled when the button is clicked [in]
 * @return UIBarButtonItem* Bar button item of the upper navigation bar.
 */
- (UIBarButtonItem*)prevBarButtonItem:(NSString*)title target:(id<NSObject>)target selector:(SEL)selector;

/**
 * Creates a control button (similar to Close button) in the top bar of HSPUiViewContainer.
 * @param title Title of the button [in]
 * @param target Instance to handle when the button is clicked [in]
 * @param selector Selector to be handled when the button is clicked [in]
 * @return UIBarButtonItem* Bar button item of the upper navigation bar.
 */
- (UIBarButtonItem*)rectBarButtonItem:(NSString*)title target:(id<NSObject>)target selector:(SEL)selector;

/**
 * Updates the control button in the top bar of HSPUiViewContainer.
 */
- (void)updateBackButton;

/**
 * Displays a waiting view when the operation needs certain amount of time. 
 */
- (void)startAnimatingIndicator;

/**
 * Displays a waiting view when the operation needs certain amount of time.
 * @param viewController Parent UIViewController instance where the waiting view is displayed [in]
 */
- (void)startAnimatingIndicatorOnViewController:(UIViewController*)viewController;

/**
 * Closes the waiting view.
 */
- (void)stopAnimatingIndicator;

/**
 * Creates an event instance to be passed to HSPUiViewControllerDelegate.
 * @param control Control type of events that occur in the view controller; the type includes closeAll, back, and refresh. [in]
 * @param action Action type of events that occur in the view controller; the type includes success, failed, and canceled. [in]
 * @return NSDictionary* Event to be passed to the delegate.
 */
- (NSDictionary*)eventWithControl:(NSString*)control action:(NSString*)action;

/**
 * Sends the event to HSPUiViewControllerDelegate.
 * @param event Event instance to send [in]
 * @see -(NSDictionary*)eventWithControl:(NSString*)control action:(NSString*)action
 */
- (void)sendDelegateEvent:(NSDictionary*)event;


@end
