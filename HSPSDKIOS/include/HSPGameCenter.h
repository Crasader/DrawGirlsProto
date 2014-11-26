//
//  HSPGameCenter.h
//  HSPInterface
//
//  Created by Seungwoo on 2014. 11. 6..
//
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

@interface HSPGameCenter : NSObject
{
	NSString*	_playerId;
	NSString*	_nickname;
	UIImage*	_profileImageNormal;
	UIImage*	_profileImageSmall;
}

@property (nonatomic, retain)	NSString*	playerId;
@property (nonatomic, retain)	NSString*	nickname;
@property (nonatomic, retain)	UIImage*	profileImageNormal;
@property (nonatomic, retain)	UIImage*	profileImageSmall;

- (void)loadProfileImageDataWithCompletionHandler:(void (^)(NSData* imageData, HSPError* error))completionHandler;

@end
