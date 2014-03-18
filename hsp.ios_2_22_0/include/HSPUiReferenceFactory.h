//
//  HSPUiReferenceFactory.h
//  HSPUIReference
//
//  Created by  on 11. 11. 28..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "HSPUiFactory.h"

/**
 * UI Factory interface which implements Korean version HSP UI using the class inherited from HSPUiFactory.
 */
@interface HSPUiReferenceFactory : HSPUiFactory

/**
 * Displays the view controller related to the URI scheme on the top of the container which contains top bar and bottom GNB. 
 * @param uri URI scheme of the root view controller [in]
 * @param delegate Delegate instance to handle events from the view controller [in]
 * @return UIViewController* type view container
 */
- (UIViewController*)HSPUiViewContainerWithUri:(HSPUri*)uri
									  delegate:(id<HSPUiViewControllerDelegate>)delegate;

@end
