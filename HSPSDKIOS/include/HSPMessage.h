//
//  HSPMessage.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 24..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

@interface NSNumber (HSPMessageCategory)

/**
 * @brief Returns an NSNumber object that is created including message numbers.
 * @param value A message number.
 */
+(NSNumber*)numberWithMessageNo:(int64_t)value;

/**
 * @brief Returns message numbers.
 * @return Message number values.
 */
-(int64_t)messageNoValue;

@end

/**
 * @brief enum for message types.
 */
typedef enum
{
    HSP_MESSAGECONTENTTYPE_TEXT = 1,    /**< Text. */
    HSP_MESSAGECONTENTTYPE_IMAGE,       /**< Image. */
} HSPMessageContentType;

/**
 * @brief Deals with message information.
 */
@interface HSPMessage : NSObject {
    int64_t                 _messageNo;
    int64_t                 _senderMemberNo;
    int64_t                 _receiverMemberNo;
    HSPMessageContentType   _contentType;
    NSString*               _content;
    NSDate*                 _sendedDate;
    BOOL                    _isSenderAdmin;
    BOOL                    _isReceiverAdmin;
}

/**
 * @brief Message number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              int64_t                 messageNo;

/**
 * @brief Member number of a sender. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              int64_t                 senderMemberNo;

/**
 * @brief Member number of a receiver. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              int64_t                 receiverMemberNo;

/**
 * @brief Message type. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              HSPMessageContentType   contentType;

/**
 * @brief Message content. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSString*               content;

/**
 * @brief Date of sending messages. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly, retain)      NSDate*                 sendedDate;

/**
 * @brief Whether the sender is admin or not. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                    isSenderAdmin;

/**
 * @brief Whether the receiver is admin or not. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property(nonatomic, readonly)              BOOL                    isReceiverAdmin;

/**
 * @brief Requests a list of received messages from the server.
 * 
 * @param messageNo Start message number. If you want to load recent messages, set the messageNo to -1.
 * @param count Count. If the count is over 500, the server returns failure.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param messages List of messages.<br>Array of HSPMessages.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMessage loadMessagesFromMessageNo:0xFFFFFF memberNo:4105000000134579 completionHandler:^(NSArray* messages, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		for ( HSPMessage* message in messages )
 * 		{
 * 			NSLog(@"Message : %@", message);
 * 		}
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     loadMessagesFromMessageNo:(int64_t)messageNo
                                count:(int32_t)count
                    completionHandler:(void (^)(NSArray* messages, HSPError *error))completionHandler;

/**
 * @brief Requests the number of new messages from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param messageCount The number of messages.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMessage queryNewMessageCountWithCompletionHandler:^(int32_t messageCount, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The number of new messages : %d", messageCount);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryNewMessageCountWithCompletionHandler:(void (^)(int32_t messageCount, HSPError *error))completionHandler;

/**
 * @brief Requests the number of new notices from the server.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param noticeCount The number of notices.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMessage queryNewNoticeCountWithCompletionHandler:^(int32_t noticeCount, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"The number of new notices : %d", noticeCount);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     queryNewNoticeCountWithCompletionHandler:(void (^)(int32_t noticeCount, HSPError *error))completionHandler;

/**
 * @brief Requests sending text messages from the server.
 * 
 * @param memberNo Member number.
 * @param text Contents of the text message.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param message Message.<br>HSPMessage object.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMessage sendTextMessageToReceiver:4105000000134579 text:@"hello" completionHandler:^(HSPMessage* message, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Sent : %@", message);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     sendTextMessageToReceiver:(int64_t)memberNo
                                 text:(NSString*)text
                    completionHandler:(void (^)(HSPMessage* message, HSPError *error))completionHandler;

/**
 * @brief Requests sending image messages from the server.
 * 
 * @param memberNo Member number.
 * @param image Imgae to send.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param message Message.<br>HSPMessage object.
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [HSPMessage sendImageMessageToReceiver:4105000000134579 image:[UIImage imageNamed:@"GameLog.png"] completionHandler:^(HSPMessage* message, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Sent : %@", message);
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     sendImageMessageToReceiver:(int64_t)memberNo
                                 image:(UIImage*)image
                     completionHandler:(void (^)(HSPMessage* message, HSPError *error))completionHandler;

/**
 * @brief Requests sending packets from the server.
 * 
 * @param memberNo Member number.
 * @param type Packet type.
 * @param data Packet data.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMessage sendPacketToReceiver:4105000000134579 type:0 packet:[NSData data] completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully sent.");
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     sendPacketToReceiver:(int64_t)memberNo
                            type:(int32_t)type
                          packet:(NSData*)data
               completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests sending push notification from the server.
 *
 * @param memberNo Member number.
 * @param message Alert message.
 * @param data Data to be sent to the app.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPMessage sendPushNotificationToReceiver:4105000000134579 message:@"Message" data:[NSData data] completionHandler:^(HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully sent.");
 * 	}
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+(void)     sendPushNotificationToReceiver:(int64_t)memberNo
                                   message:(NSString*)message
                                      data:(NSDictionary*)data
                         completionHandler:(void (^)(HSPError *error))completionHandler;

/**
 * @brief Requests an original image for the image message.
 * 
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param image Image.
 * @param error Error.<br>If successful, the error code is 0. 
 *
 * @code
 * [message downloadImageWithCompletionHandler:^(UIImage* image, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully downloaded.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to download.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */

-(void)     downloadImageWithCompletionHandler:(void (^)(UIImage* image, HSPError *error))completionHandler;

/**
 * @brief Requests a resized image for the image message. This function is used to get a thumbnail image.
 * 
 * @param width Width.
 * @param height Height.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param image Image.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [message downloadResizedImageWithWidth:10 height:10 completionHandler:^(UIImage* image, HSPError *error) {
 * 	if ( [error isSuccess] == YES )
 * 	{
 * 		NSLog(@"Successfully downloaded.");
 * 	} else
 * 	{
 * 		NSLog(@"Failed to download.");
 * 	} 
 * }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
-(void)     downloadResizedImageWithWidth:(int32_t)width
                                   height:(int32_t)height
                        completionHandler:(void (^)(UIImage* image, HSPError *error))completionHandler;

@end

/**
 * @brief [Notifications] Is posted when a packet is received from the HSP server.<br>
 * NSDictionary object is received.
 * key:@"gameNo" val:(NSNumber*), key:@"type" val:(NSNumber*), key:@"senderMemberNo" val:(NSNumber*), key:@"receiverMemberNo" val:(NSNumber*), key:@"data" val:(NSData*)
 */
extern NSString* const HSPDidReceivePacketNotificationName;

/**
 * @brief [Notifications] Is posted when a message is received from the HSP server.<br>
 * HSPMessage object is received.
 */
extern NSString* const HSPDidReceiveMessageNotificationName;

