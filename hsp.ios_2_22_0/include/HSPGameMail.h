//
//  HSPGameMail.h
//  HSPInterface
//
//  Created by Seungwoo on 12. 10. 8..
//
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

#define	HSP_GAMEMAIL_CONTENTTYPE_ALL	0

/**
 * @brief enum for game mailbox types.
 */
typedef enum
{
	HSP_GAMEMAIL_OUTGOING,	/** Sent */
	HSP_GAMEMAIL_INCOMING	/** Inbox */
} HSPGameMailBoxType;

/**
 * @brief Deals with game mails.
 *
 * You can use {@link HSPGameMail} object only when logged in the HSP server.
 * <p>
 * HSP provides {@link HSPGameMail} interface to send and receive game mails between users. Each user has Sent/Inbox and each mail is assigned a unique mailNo.
 * <p>
 * You can call loadGameMailsWithContentType with contentType and targetMailBox as parameters to get a list of {@link HSPGameMail} instances through the handler; each {@link HSPGameMail} instance has game mail information.
 * <p>
 * You can call sendGameMail with contentType and content as parameters to send a game mail to a specific user. You can call modifyGameMail to modify the game mail you already sent.
 * <p>
 * You can also call removeGameMail with an array of mailNos as a parameter to remeove game mails.
 * <p>
 * {@link HSPGameMail} class provides gamemail number, sender/receiver's member number, whether the sender or receiver is admin or not, game mail type, content, and sent/received time.
 */
@interface HSPGameMail : NSObject
{
	int64_t		_mailNo;
	
	int64_t		_senderMemberNo;
	int64_t		_receiverMemberNo;
	
	BOOL		_isSenderAdmin;
	BOOL		_isReceiverAdmin;
	
	int32_t		_contentType;
	NSString*	_content;
	
	NSDate*		_sentTimeStamp;
	NSDate*		_receiveTimeStamp;
	
	NSString*	_reserved;			// json 추가 필드 저장 변수
}

/**
 * @brief A unique number of game mail. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly)	int64_t		mailNo;

/**
 * @brief Sender's member number.  (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly) int64_t		senderMemberNo;

/**
 * @brief Receiver's member number. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly) int64_t		receiverMemberNo;

/**
 * @brief Whether the sender is admin or not. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly) BOOL		isSenderAdmin;

/**
 * @brief Whether the receiver is admin or not. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly) BOOL		isReceiverAdmin;

/**
 * @brief Game mail type. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, assign, readonly) int32_t		contentType;

/**
 * @brief Content of game mail. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly) NSString*	content;

/**
 * @brief Sent time. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly) NSDate*		sentTimeStamp;

/**
 * @brief Received time. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly) NSDate*		receiveTimeStamp;

/**
 * @brief Requests a list of game mails from the server.<br>
 * Gets game mails which are received after the specified time, in the form of paging.
 *
 * @param contentType Game mail type (0 is not available)
 * @param targetMailBox Mailbox type (Inbox, Sent)
 * @param baseDate Base time to query a list of game mails.
 * @param pageNo Page number to get (it starts from 1)
 * @param pageSize Paging size, up to 500.
 * @param updateReadenTime Whether to update the received time (valid only in Inbox)
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param mails List of game mails.<br>Array of HSPGameMail.
 * @param totalCount Total number of game mails (used for paging)
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPGameMail loadGameMailsWithContentType:1
 *							targetMailBox:HSP_GAMEMAIL_OUTGOING
 *							     baseDate:[[HSPUtility sharedUtility] convertStringToDate:@"20121008120000" dateFormat:@"yyyyMMddHHmmss"]
 *							   	   pageNo:1
 *								 pageSize:20
 *						 updateReadenTime:NO
 *						completionHandler:^(NSArray *mails, int32_t totalCount, HSPError *error) {
 *							if ( [error isSuccess] == YES )
 *							{
 *								if ( totalCount > 0 )
 *								{
 *									for ( HSPGameMail* gameMail in mails )
 *									{
 *										NSLog(@"Game mail number : %lld", gameMail.mailNo);
 *										NSLog(@"Sender's member number : %lld", gameMail.senderMemberNo);
 *										NSLog(@"Receiver's member number : %lld", gameMail.receiverMemberNo);
 *										NSLog(@"Whether sender is admin : %i", gameMail.isSenderAdmin);
 *										NSLog(@"Whether receiver is admin : %i", gameMail.isReceiverAdmin);
 *										NSLog(@"Game mail type : %i", gameMail.contentType);
 *										NSLog(@"Game mail content : %@", gameMail.content);
 *										NSLog(@"Sent time : %@", gameMail.sentTimeStamp);
 *										NSLog(@"Received time : %@", gameMail.receiveTimeStamp);
 *									}
 *								}
 *								else
 *									NSLog(@"No SentGameMail.");
 *							}
 *							else
 *								NSLog(@"Failed to check SentGameMails. (%@)", error);
 *						}];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)loadGameMailsWithContentType:(int32_t)contentType
					   targetMailBox:(HSPGameMailBoxType)targetMailBox
							baseDate:(NSDate*)baseDate
							  pageNo:(int32_t)pageNo
							pageSize:(int32_t)pageSize
					updateReadenTime:(BOOL)updateReadenTime
				   completionHandler:(void (^)(NSArray* mails, int32_t totalCount, HSPError* error))completionHandler;

/**
 * @brief Requests the number of new game mails from the server.<br>
 * After loading a list of game mails, requests the number of game mails sent to me.
 *
 * @param contentType Game mail type (0 is not available)
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param count The number of new game mails.
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPGameMail queryNewGameMailCountWithContentType:1
 *								   completionHandler:^(int32_t count, HSPError *error) {
 *								   if ( [error isSuccess] == YES )
 *								   {
 *								       NSLog(@"New mails : %d", count);
 *								   }
 *								   else
 *									   NSLog(@"Failed to check the number of new game mails. (%@)", error);
 *							   }];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)queryNewGameMailCountWithContentType:(int32_t)contentType
						   completionHandler:(void (^)(int32_t count, HSPError* error))completionHandler;

/**
 * @brief Requests the server to send a game mail.<br>
 *
 * @param receiverMemberNo Receiver's member number. 
 * @param contentType Game mail type (0 is not available)
 * @param content Game mail content
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameters:
 * @param sentMail Sent game mail (HSPGameMail object)
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [HSPGameMail sendGameMailToMember:4105000000134541L
 *						 contentType:1
 *							 content:@"Game mail content"
 *				   completionHandler:^(HSPGameMail* sentMail, HSPError *error) {
 *							if ( [error isSuccess] == YES )
 *							{
 *								NSLog(@"Sent game mail information");
 *								NSLog(@"Game mail number : %lld", sentMail.mailNo);
 *								NSLog(@"Sender's member number : %lld", sentMail.senderMemberNo);
 *								NSLog(@"Receiver's member number : %lld", sentMail.receiverMemberNo);
 *								NSLog(@"Whether sender is admin : %i", sentMail.isSenderAdmin);
 *								NSLog(@"Whether receiver is admin : %i", sentMail.isReceiverAdmin);
 *								NSLog(@"Game mail type : %i", sentMail.contentType);
 *								NSLog(@"Game mail content : %@", sentMail.content);
 *								NSLog(@"Sent time : %@", sentMail.sentTimeStamp);
 *								NSLog(@"Received time : %@", sentMail.receiveTimeStamp);
 *							}
 *							else
 *								NSLog(@"Failed to send a game mail. (%@)", error);
 *						}];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
+ (void)sendGameMailToMember:(int64_t)receiverMemberNo
				 contentType:(int32_t)contentType
					 content:(NSString*)content
		   completionHandler:(void (^)(HSPGameMail* sentMail, HSPError* error))completionHandler;

/**
 * @brief Requests the server to modify a game mail.<br>
 *
 * @param contentType Game mail type (0 is not available)
 * @param content Game mail content to modify.
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [gameMail modifyGameMailToContentType:1
 *                               content:@"Game mail content to modify"
 *                     completionHandler:^(HSPError *error) {
 *							if ( [error isSuccess] == YES )
 *							{
 *								NSLog(@"Successfully modified game mail contents.");
 *							}
 *							else
 *								NSLog(@"Failed to modify game mail contents. (%@)", error);
 *						}];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)modifyGameMailToContentType:(int32_t)contentType
                            content:(NSString*)content
                  completionHandler:(void (^)(HSPError* error))completionHandler;

/**
 * @brief Requests the server to remove a game mail.<br>
 *
 * @param targetMailBox Mailbox type (Inbox, Sent)
 * @param completionHandler Is called when a response to the request is received from the server.
 *
 * This block needs the following parameter:
 * @param error Error.<br>If successful, the error code is 0.
 *
 * @code
 * [gameMail removeGameMailForTargetMailBox:HSP_GAMEMAIL_OUTGOING
 *                        completionHandler:^(HSPError *error) {
 *							if ( [error isSuccess] == YES )
 *								NSLog(@"Successfully removed a game mail.");
 *							else
 *								NSLog(@"Failed to remove a game mail. (%@)", error);
 *						}];
 * @endcode
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
- (void)removeGameMailForTargetMailBox:(HSPGameMailBoxType)targetMailBox
                     completionHandler:(void (^)(HSPError* error))completionHandler;

@end
