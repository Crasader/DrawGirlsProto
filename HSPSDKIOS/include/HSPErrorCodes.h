//
//  HSPErrorCodes.h
//  HSPCore
//
//  Created by  on 11. 10. 13..
//  Copyright (c) 2011 NHN Corporation. All rights reserved.
//

#ifndef HSPErrorCodes_h
#define HSPErrorCodes_h

#include "HSPTypes.h"

#pragma mark - Result for HSP
#define HSP_ERR_SUCCESS							(0)

#pragma mark - Error domain
// domain for client side
#define HSP_ERR_DOMAIN_HTTPMANAGER				(@"HSP_ERR_DOMAIN_HTTPMANAGER")
#define HSP_ERR_DOMAIN_MFSCONNECTOR				(@"HSP_ERR_DOMAIN_MFSCONNECTOR")
#define HSP_ERR_DOMAIN_BIPSERVICE				(@"HSP_ERR_DOMAIN_BIPSERVICE")
#define HSP_ERR_DOMAIN_ICONSERVICE				(@"HSP_ERR_DOMAIN_ICONSERVICE")
#define HSP_ERR_DOMAIN_NOTICESERVICE			(@"HSP_ERR_DOMAIN_NOTICESERVICE")
#define HSP_ERR_DOMAIN_LAUNCHINGSERVICE			(@"HSP_ERR_DOMAIN_LAUNCHINGSERVICE")
#define HSP_ERR_DOMAIN_PHOTOSERVICE				(@"HSP_ERR_DOMAIN_PHOTOSERVICE")
#define HSP_ERR_DOMAIN_LOGINSERVICE				(@"HSP_ERR_DOMAIN_LOGINSERVICE")
#define HSP_ERR_DOMAIN_HSPSERVICE				(@"HSP_ERR_DOMAIN_HSPSERVICE")
#define HSP_ERR_DOMAIN_MASHUP					(@"HSP_ERR_DOMAIN_MASHUP")
#define HSP_ERR_DOMAIN_WEBCLIENTSERVICE			(@"HSP_ERR_DOMAIN_WEBCLIENTSERVICE")
#define HSP_ERR_DOMAIN_CGPSERVICE				(@"HSP_ERR_DOMAIN_CGPSERVICE")
#define HSP_ERR_DOMAIN_PAYMENTSERVICE			(@"HSP_ERR_DOMAIN_PAYMENTSERVICE")
#define HSP_ERR_DOMAIN_SNS						(@"HSP_ERR_DOMAIN_SNS")
#define HSP_ERR_DOMAIN_ETC	  					(@"HSP_ERR_DOMAIN_ETC")
#define HSP_ERR_DOMAIN_ITEMDELIVERY				(@"HSP_ERR_DOMAIN_ITEMDELIVERY")
#define HSP_ERR_DOMAIN_COIN						(@"HSP_ERR_DOMAIN_COIN")

#pragma mark - Error Codes
// error for client side
#define HSP_ERR_CONNECTION_FAIL					(0x00001001)
#define HSP_ERR_TIMEOUT							(0x00001002)
#define HSP_ERR_INVALID_PARAMETER				(0x00001003)
#define HSP_ERR_NOT_ENOUGH_MEMORY				(0x00001004)
#define HSP_ERR_NOT_INITIALIZED					(0x00001005)
#define HSP_ERR_IMAGE_ENCODING					(0x00001006)
#define HSP_ERR_JSON_PARSING					(0x00001007)
#define HSP_ERR_NOT_A_MAINTHREAD				(0x00001008)
#define HSP_ERR_NOT_LOGIN						(0x00001009)
#define HSP_ERR_NOT_SUPPORTED_FUNCTION			(0x0000100A)
#define HSP_ERR_DETECTED_CRACKING				(0x0000100B)
#define HSP_ERR_LITMUS_ERROR					(0x0000100B)
#define HSP_ERR_NETWORK_RESTORING				(0x0000100C)
#define	HSP_ERR_UNSUPPORTED_SERVICE_DOMAIN		(0x0000100D)
#define HSP_ERR_UNKNOWN_ERROR					(0x0000F000)
#define HSP_ERR_INTERNAL_ERROR					(0x0000F001)
//#define  HSP_ERR_INVALID_INTERNAL_STATE         (0x0000F002) - Deprecated
//#define  HSP_ERR_ALREADY_LOGIN_PROCESSING       (0x0000F003) - Deprecated
#define HSP_ERR_INVALID_PHONENUMBER				(0x0000F004)
#define HSP_ERR_INVALID_NOTICE_URL				(0x0000F005)
#define HSP_ERR_INVALID_LAUNCHING_INFO			(0x0000F006)
#define HSP_ERR_NOT_INSTALLED_APP				(0x0000F007)

////////////////////////////////////////////////////////////
// ERROR CODE LAUNCHING SERVICE
#define	HSP_ERR_GAMEINFO_FAIL					(0x00002001)
#define HSP_ERR_CLIENT_VERSION_FAIL				(0x00002002)	
#define HSP_ERR_CLIENT_LATEST_VERSION_EXIST		(0x00002003)
#define HSP_ERR_HANGAME_INSPECTION				(0x00002004)
#define HSP_ERR_GAME_INSPECTION					(0x00002005)	
#define HSP_ERR_PLATFORM_INSPECTION				(0x00002006)
#define HSP_ERR_BLOCKED_DEVICE					(0x00002007)
#define HSP_ERR_NOT_USABLE_DEVICE				(0x00002008)
#define HSP_ERR_RESOURCE_UPDATE_FAIL			(0x00002009)
#define HSP_ERR_ADMIN_NOT_LOGINABLE				(0x0000200A)
#define HSP_ERR_ADMIN_NOT_PLAYABLE				(0x0000200B)
#define HSP_ERR_SERVICE_CLOSED					(0x0000200C)
#define HSP_ERR_INSPECTION_DETAIL_VIEW			(0x0000200D)
#define HSP_ERR_INSPECTION_CLOSE				(0x0000200E)

////////////////////////////////////////////////////////////
// ERROR CODE HSP SERVICE
#define	HSP_ERR_WAITING_LOGIN					(0x00003001)
//#define  HSP_ERR_WAITING_LNC                    (0x00003002) - Deprecated
#define	HSP_ERR_ALREADY_LOGIN					(0x00003003)

////////////////////////////////////////////////////////////
// ERROR CODE LOGIN SERVICE
//#define  HSP_ERR_INITIALIZE_FAIL                (0x00004001) - Deprecated
//#define  HSP_ERR_LOGIN_FAIL                     (0x00004002) - Deprecated
//#define  HSP_ERR_MAPPING_TO_HANGAMEID_FAIL      (0x00004003) - Deprecated
//#define  HSP_ERR_RESETDEVICE_FAIL               (0x00004004) - Deprecated
#define HSP_ERR_USER_CANCELED					(0x00004005)

////////////////////////////////////////////////////////////
// ERROR CODE PHOTO SERVICE
#define HSP_ERR_NOT_FOUND_FILE					(0x00005001)

////////////////////////////////////////////////////////////
// ERROR CODE LITMUS SERVICE
#define HSP_ERR_LITMUS_NOT_INIT_BUT_LOGIN		(0x00006001)
#define HSP_ERR_DETECTED_ROOTING				(0x00006002)
#define HSP_ERR_DETECTED_CHEATING				(0x00006003)
#define HSP_ERR_REFUSE_LOGCOLLECT_AGREEMENT		(0x00006006)

////////////////////////////////////////////////////////////
// ERROR CODE LOGIN & PROFILE SERVER SIDE
#define HSP_ERR_EXIST_SNO						(0x81000002)
#define HSP_ERR_CANNOT_FOUND_ACCOUNT			(0x81000005)
#define HSP_ERR_ID_WAS_BANNED					(0x81000006)
#define HSP_ERR_MAPPED_BY_OTHER_DEVICE			(0x8100003E)
#define HSP_ERR_ALREADY_MAPPED_ACCOUNT			(0x00140067)
#define HSP_ERR_ALREADY_REMAPPED_ACCOUNT		(0x0014006D)
#define HSP_ERR_ALREADY_MAPPED_ACCOUNT_SG		(0x0014006D)
#define HSP_ERR_PUNISHED_USER					(0x0014006F)
#define HSP_ERR_DEACTIVATED_USER                (0x00140070)
#define HSP_ERR_BACKGROUND_APP_EXIST			(0x8100100D)
#define HSP_ERR_INVALID_OAUTH_TICKET			(0x81001010)
#define HSP_ERR_EXCEEDED_RESET_ACCOUNT			(0x81001011)
#define HSP_ERR_LINE_TALK_SERVER_UNAVAILABLE	(0x81001013)
#define	HSP_ERR_NOT_EXIST_AUTH_DATA				(0x8100A000)
#define	HSP_ERR_WRITE_AUTH_DATA_FAIL			(0x8100A001)
#define	HSP_ERR_INVALID_USING_API				(0x8100B000)
#define	HSP_ERR_INVALID_USING_LOGIN_API			(0x8100B001)
#define	HSP_ERR_INVALID_ARGUMENT				(0x8100C000)
#define HSP_ERR_LSTIMEOUT						(0x8100C001)
#define HSP_ERR_INVALID_SYSTEM_INFO				(0x8100D000)
#define HSP_ERR_INVALID_NETWORK					(0x8100D001)

// error for mashup server side
////////////////////////////////////////////////////////////
// ERROR CODE LOGIN
#define HSP_ERR_TICKETAUTH						(0x00010001)
#define HSP_ERR_UPDATEPROFILE					(0x00010002)	
#define HSP_ERR_GETLOGINNOTICE					(0x00010003)
#define HSP_ERR_PROCESSACHIEVE					(0x00010004)
#define HSP_ERR_CHECKFIRSTENTER					(0x00010005)	
#define HSP_ERR_GETPROFILEFROMSVC				(0x00010006)
#define HSP_ERR_UPDATEPRESENCE					(0x00010007)
#define HSP_ERR_UPDATEPROFILERECENTPLAYEDGAME	(0x00010008)
#define HSP_ERR_UPDATEPLAYEDGAME				(0x00010009)

////////////////////////////////////////////////////
// ERROR CODE PROFILE
#define HSP_ERR_LONGPHONENO						(0x00020001)
#define HSP_ERR_INVALIDTODAYWORD				(0x00020002)
#define HSP_ERR_GETSMSCERTNO					(0x00020003)
#define HSP_ERR_UPDATEPROFILEOPTION				(0x00020004)
#define HSP_ERR_UPSSETNICKNAME					(0x00020005)
#define HSP_ERR_UPSSETPHONENO					(0x00020006)
#define HSP_ERR_UPDATEPRESENCENICKNAME			(0x00020007)
#define HSP_ERR_UPDATEPROFILENICKNAME			(0x00020008)
//#define HSP_ERR_UPDATEPROFILEME2DAYID			(0x00020009)
#define HSP_ERR_UPDATEPROFILETODAYWORD			(0x0002000a)
#define HSP_ERR_UPDATEPROFILEPHONENO			(0x0002000c)
#define HSP_ERR_UPDATEPRESENCESERVICENO			(0x0002000d)
#define HSP_ERR_LONGNICKNAME					(0x0002000e)
#define HSP_ERR_LONGTODAYWORD					(0x0002000f)
#define HSP_ERR_GETPRESENCE						(0x00020010)
#define HSP_ERR_GETPROFILE						(0x00020011)
#define HSP_ERR_GETGAMEINFOFORPROFILE			(0x00020012)
#define HSP_ERR_GETRELATIONCODE					(0x00020013)
#define HSP_ERR_GETGAMEBRIEFLIST				(0x00020015)
#define HSP_ERR_GETGAMEDETAIL					(0x00020016)
#define HSP_ERR_GETRECENTPLAYEDGAMEINFO			(0x00020017)
#define HSP_ERR_GETGAMEUSERDATA					(0x00020018)
#define HSP_ERR_SETGAMEUSERDATA					(0x00020019)
#define HSP_ERR_UPDATEALLOWEDPHONEBOOK			(0x0002001a)
#define HSP_ERR_INVALIDNICKNAME					(0x0002001b)
#define HSP_ERR_GETPUNISHEDINFO					(0x0002001c)	
#define HSP_ERR_REQMODIFYTODAYWORD				(0x0002001d)
#define HSP_ERR_REQGETUSERPROFILEBYHNO			(0x0002001e)
#define HSP_ERR_GSGETUSERPRESENCELIST			(0x0002001f)
#define HSP_ERR_GSSETUSERPRESENCELIST			(0x00020020)	
#define HSP_ERR_CHECKPHONENO					(0x00020021)
#define HSP_ERR_REQMODIFYMOBILEPROFILE			(0x00020022)
#define HSP_ERR_SETBIRTHDATE					(0x00020023)	
#define HSP_ERR_SETGENDER						(0x00020024)	
#define HSP_ERR_NOTIFYSYNCMEMBERINFO			(0x00020025)
#define HSP_ERR_UPDATEPROFILEIDPID				(0x00020026)
#define HSP_ERR_GETPHONENOLIST					(0x00020027)
#define HSP_ERR_GETPLAYEDGAMELIST				(0x00020028)
#define HSP_ERR_REQFINDPROFILEINFOBYMULTIHID	(0x00020029)
#define HSP_ERR_REQFINDPROFILEINFOBYMULTISNO	(0x0002002a)
#define HSP_ERR_GETGAMEGENREINFO				(0x0002002b)
#define HSP_ERR_GETGAMENOTIFYADDRESS			(0x0002002c)	
#define HSP_ERR_GETGAMEAPPSTOREURLS				(0x0002002d)
#define HSP_ERR_GETGAMEUSERLIST					(0x0002002e)
#define HSP_ERR_GETPLAYEDGAMELOG				(0x0002002f)
#define HSP_ERR_GETIDPIDONWHITELIST				(0x00020030)
#define HSP_ERR_SETIDPID						(0x00020031)
#define HSP_ERR_INVALIDMEMBERNO					(0x00020032)	
#define HSP_ERR_GETMYPROFILE					(0x00020033)
#define HSP_ERR_GETCURRENTGAMENO				(0x00020034)
#define HSP_ERR_GETPROFILEDETAIL				(0x00020035)
#define HSP_ERR_GETPLAYEDGAMENOLIST				(0x00020036)
#define HSP_ERR_SETIDPAUTHTICKET				(0x00020037)
#define HSP_ERR_GETIDPAUTHTICKET				(0x00020038)
#define HSP_ERR_GETREGISTEREDGAMERECOMMENDER	(0x00020039)
#define HSP_ERR_GETREGISTERINGGAMERECOMMENDER	(0x00020040)
#define HSP_ERR_CHECKGAMENOVALIDATION			(0x00020041)
#define HSP_ERR_CHECKGAMERECOMMENDEREXISTENCE	(0x00020042)
#define HSP_ERR_ALREADYGAMERECOMMENDEREXISTING	(0x00020043)
#define HSP_ERR_SETGAMERECOMMENDER				(0x00020044)
//#define HSP_ERR_SENDREQADDFRIENDSFROMME2DAY		(0x00020045)
#define HSP_ERR_SENDREQADDFRIENDSFROMTWITTER	(0x00020046)
#define HSP_ERR_SENDREQADDFRIENDSFROMLINE		(0x00020047)
#define HSP_ERR_GETPHOTOURLLIST					(0x00020048)
#define HSP_ERR_GETLASTLOGINDEVICEKEYLIST		(0x00020049)
#define HSP_ERR_INVALID_GAMECENTER_PHOTO		(0x0002004A)

//////////////////////////////////////////////////
// ERROR CODE SMS
#define HSP_ERR_INSERTSMSRECOMMEND				(0x00030001)
#define HSP_ERR_SENDSMS							(0x00030002)
#define HSP_ERR_UPDATESMSCERTNO					(0x00030003)
#define HSP_ERR_MAXSMSRECOMMEND					(0x00030004)
#define HSP_ERR_MAKESMSRECOMMENDMSG				(0x00030005)
#define HSP_ERR_INVALIDPHONENO					(0x00030006)
#define HSP_ERR_SENDRECOMMENDEMAIL				(0x00030007)

//////////////////////////////////////////////////
// ERROR CODE SOCIAL
#define HSP_ERR_SEARCHMEMBERWITHTYPE			(0x00040001)
#define HSP_ERR_SEARCHFOLLOWERS					(0x00040002)
#define HSP_ERR_SEARCHID						(0x00040004)
#define HSP_ERR_SEARCHNICKNAME					(0x00040005)
#define HSP_ERR_SEARCHNICKINLIST				(0x00040006)
#define HSP_ERR_SEARCHNICKINFOLLOWERS			(0x00040007)
#define HSP_ERR_INVALIDRELATIONTYPE				(0x0004000a)
#define HSP_ERR_INVALIDUSERCOUNT				(0x0004000b)
#define HSP_ERR_GETPROFILELIST					(0x0004000c)
#define HSP_ERR_GETPRESENCELIST					(0x0004000e)
#define HSP_ERR_DELETEMEMBERLIST				(0x00040010)
#define HSP_ERR_CHECKIFISMEMBER					(0x00040011)
#define HSP_ERR_MAXINRELATION					(0x00040012)
#define HSP_ERR_NOADDINRELATION					(0x00040013)
#define HSP_ERR_GETBRIEFPROFILELIST				(0x00040014)
//#define HSP_ERR_GETME2DAYIDONWHITELIST			(0x00040015)
#define HSP_ERR_TOOMANYPARAMETERS				(0x00040016)
#define HSP_ERR_GETONLINEUSERLISTNOTINRELATION	(0x00040017)	
#define HSP_ERR_GETRANKINGLISTNOTINRELATION		(0x00040018)	
#define HSP_ERR_SEARCHTOOLONGWORD				(0x00040019)
#define HSP_ERR_GETRELATIONCOUNT				(0x0004001a)
#define HSP_ERR_CHECKUSERRELATION				(0x0004001b)
#define HSP_ERR_GETFRIENDINFOLIST				(0x0004001c)
#define HSP_ERR_GETGAMEWHITELIST				(0x0004001d)	
#define HSP_ERR_GETMEMBERLISTCOUNT				(0x0004001e)
#define HSP_ERR_SENDGAMEMATEEVENT				(0x0004001f)
#define HSP_ERR_GETFORWARDRELATIONLIST			(0x00040020)
#define HSP_ERR_GETBACKWARDRELATIONLIST			(0x00040021)
#define HSP_ERR_GETPLAYEDGAMEFOLLOWERS			(0x00040022)
#define HSP_ERR_GETLATESTLOGINUSERS				(0x00040023)
#define HSP_ERR_EXCLUDEREQUESTERINFO			(0x00040024)
#define HSP_ERR_EXCLUDEALREADYINRELATIONLIST	(0x00040025)
#define HSP_ERR_EXCLUDEBLOCKERLIST				(0x00040026)
#define HSP_ERR_EXCLUDEEXCEEDINGLIST			(0x00040027)
#define HSP_ERR_GETLINEACCESSTOKEN				(0x00040028)
#define HSP_ERR_GETLINEFRIENDLIST				(0x00040029)

//////////////////////////////////////////////////
// ERROR CODE IM
#define HSP_ERR_GETNORMALMSGLIST				(0x00050001)
#define HSP_ERR_GETNOTIMSGLIST					(0x00050002)
#define HSP_ERR_INVALIDMESSAGEKIND				(0x00050003)
#define HSP_ERR_DELETENORMALRECENTMSG			(0x00050004)
#define HSP_ERR_UPDATENORMALMSGDELETED			(0x00050005)
#define HSP_ERR_INSERTMSGINBOX					(0x00050006)
#define HSP_ERR_UPDATEMSGINRECENTBOX			(0x00050007)
#define HSP_ERR_SENDSERVERPUSH					(0x00050008)
#define HSP_ERR_GETNORMALMSGDETAILLIST			(0x00050009)
#define HSP_ERR_GETNOTIMSGDETAILLIST			(0x0005000a)
#define HSP_ERR_UPDATEMSGREADLOG				(0x0005000b)
#define HSP_ERR_GETMSGREADLOG					(0x0005000c)
#define HSP_ERR_GETNEWMSGCOUNT					(0x0005000d)
#define HSP_ERR_GETNEWNOTICECOUNT				(0x0005000e)
#define HSP_ERR_BLOCKEDUSER						(0x00050010)
#define HSP_ERR_BLACKLISTUSER					(0x00050011)
#define HSP_ERR_LONGMSG							(0x00050012)
#define HSP_ERR_GETSENTGAMEMAILS				(0x00050013)
#define HSP_ERR_GETSENTGAMEMAILSTOTALCOUNT		(0x00050014)
#define HSP_ERR_GETRECEIVEDGAMEMAILS			(0x00050015)
#define HSP_ERR_GETRECEIVEDGAMEMAILSTOTALCOUNT	(0x00050016)
#define HSP_ERR_DELETEGAMEMAIL					(0x00050017)
#define HSP_ERR_GETGAMEMAILLIST					(0x00050018)
#define HSP_ERR_GETGAMEMAILTOTALCOUNT			(0x00050019)
#define HSP_ERR_MODIFYGAMEMAIL					(0x00050020)
#define HSP_ERR_LONG_LINEAPPLINKMSG				(0x00050021)

//////////////////////////////////////////////////
// ERROR CODE ACHIEVE
#define HSP_ERR_GETACHIEVEINFOLIST				(0x00060001)
#define HSP_ERR_GETACHIEVETOTALPOINT			(0x00060002)
#define HSP_ERR_UPDATEACHIEVE					(0x00060003)
#define HSP_ERR_GETACHIEVETOTAL					(0x00060004)
#define HSP_ERR_GETACHIEVEITEMLIST				(0x00060005)
#define HSP_ERR_GETACHIEVETOTALOFALLGAME		(0x00060006)
#define HSP_ERR_GETACHIEVETOTALLIST				(0x00060007)
#define HSP_ERR_GETACHIEVETOTALLISTOFALLGAME	(0x00060008)

//////////////////////////////////////////////////
// ERROR CODE RANKING
#define HSP_ERR_SETRANKING						(0x00070001)
#define HSP_ERR_GETRANKINGSCHEMALIST			(0x00070002)
#define HSP_ERR_GETMAINRANKINGFORGAME			(0x00070003)
#define HSP_ERR_GETRANKINGBYIDLIST				(0x00070004)
#define HSP_ERR_GETRANKINGLIST					(0x00070005)
#define HSP_ERR_GETGROUPRANKINGLIST				(0x00070006)	
#define HSP_ERR_GETRANKINGBYID					(0x00070007)
#define HSP_ERR_GETRANKINGLISTONUSER			(0x00070008)
#define HSP_ERR_GETDEFAULTRANKING				(0x00070009)
#define HSP_ERR_GETFRIENDRANKINGLIST			(0x0007000A)
#define HSP_ERR_NORANKINGSCHEMA					(0x0007000B)
#define HSP_ERR_WRONGRANKINGINPUT				(0x0007000C)
#define HSP_ERR_GETRANKINGLISTNEARMEMBER		(0x0007000D)
#define HSP_ERR_GETRANKINGLISTBYRANKINGKEY		(0x0007000E)
#define HSP_ERR_GETFRIENDRANKINGLISTBYRANKINGKEY	(0x00070010)

////////////////////////////////////////////////////////////
// ERROR CODE Match Making
////////////////////////////////////////////////////////////

#define HSP_ERR_CREATEMATCH						(0x00080001)
#define HSP_ERR_GETMATCH						(0x00080002)
#define HSP_ERR_INSERTMATCHMEMBER				(0x00080003)
#define HSP_ERR_FINDMATCH						(0x00080004)
#define HSP_ERR_GETMATCHMEMBER					(0x00080005)
#define HSP_ERR_SETMATCHMEMBER					(0x00080006)

////////////////////////////////////////////////////////////
// ERROR CODE GameInfo
////////////////////////////////////////////////////////////

#define HSP_ERR_GETRELEASEDGAMECLIENTLIST		(0x00090001)
#define HSP_ERR_GETSVCGAMECLIENTLIST			(0x00090002)
#define HSP_ERR_GETGAMEINFOLIST					(0x00090003)
#define HSP_ERR_GETGAMEINFOLISTBYGAMENO			(0x00090004)
#define HSP_ERR_GETGAMECLIENTVERSION			(0x00090005)
#define HSP_ERR_GETRELEASEDGAMELIST				(0x00090006)
#define HSP_ERR_GETMARKETURLLIST				(0x00090007)
#define HSP_ERR_GETHSPCONNECTION				(0x00090008)
#define HSP_ERR_DELIVERITEM						(0x00090009)

////////////////////////////////////////////////////////////
// ERROR CODE Push
////////////////////////////////////////////////////////////

#define HSP_ERR_PUSHSYSTEM						(0x00100001)
#define HSP_ERR_PUSHLOADFAIL					(0x00100002)
#define HSP_ERR_PUSHREADFAIL					(0x00100003)
#define HSP_ERR_PUSHALLOCFAIL					(0x00100004)
#define HSP_ERR_PUSHDATABASE					(0x00100005)
#define HSP_ERR_PUSHSERVICEID					(0x00100006)
#define HSP_ERR_PUSHINVALIDVALUE				(0x00100007)
#define HSP_ERR_PUSHSENDINFONOTEXIST			(0x00100008)

////////////////////////////////////////////////////////////
// ERROR CODE New Social (http://wiki.nhncorp.com/display/gp/HSP_Social)
////////////////////////////////////////////////////////////

#define HSP_ERR_SOCIALDUPLICATED				(0x00110001)
#define HSP_ERR_SOCIALINVALIDPROPOSAL			(0x00110002)
#define HSP_ERR_SOCIALINVALIDMSGID				(0x0011fffd)
#define HSP_ERR_SOCIALDB						(0x0011fffe)
#define HSP_ERR_SOCIALINTERNAL					(0x0011ffff)

////////////////////////////////////////////////////////////
// ERROR CODE COIN
// AddCoin
#define HSP_ERR_COIN_ADDCOIN_INVALID_TARGETMEMBERNO		(0x00130011) // 유효하지 않은 USER 정보 - targetMemberNo 항목을 확인한다.
#define HSP_ERR_COIN_ADDCOIN_INVALID_ITEM				(0x00130012) // 유효하지 않은 ITEM 정보 - item항목이 유효하지 않다. cpid/itemid 항목을 확인한다.
#define HSP_ERR_COIN_ADDCOIN_NOT_ENOUGH_BALANCE			(0x00130013) // 잔액이 부족하다 - 잔여 금액과 상품금액을 확인한다.
#define HSP_ERR_COIN_ADDCOIN_INVALID_ITEMTYPECODE		(0x00130014) // 유효하지 않은 서비스 - itemTypeCode를 확인한다.
#define HSP_ERR_COIN_ADDCOIN_INVALID_ARGUMENT			(0x00130015) // 요청 인자 오류 - 인자를 다시 확인한다.
#define HSP_ERR_COIN_ADDCOIN_CHECKBLANCE				(0x00130016) // 코인 사용 실패 - 잔액 조회 후에 코인 사용을 다시 시도한다.
#define HSP_ERR_COIN_ADDCOIN_INVALID_CHECKSUM			(0x00130017) // Checksum 오류 - Checksum 생성 로직이 잘못되었거나, 데이터가 위/변조되었다.
#define HSP_ERR_COIN_ADDCOIN_DUPLICATED_REQUEST			(0x00130018) // 이미 처리된 요청 - randomKey가 중복되었다.
#define HSP_ERR_COIN_ADDCOIN_REMOVE_FREECOIN_FAILED		(0x00130019) // 무료 코인 사용 오류 - 기술 PM에게 문의한다.
#define HSP_ERR_COIN_ADDCOIN_NOT_ENOUGH_COIN			(0x0013001A) // 코인 부족
#define HSP_ERR_COIN_ADDCOIN_ADD_FREECOIN_FAILED		(0x0013001B) // 무료 코인 충전 오류 - 기술 PM에게 문의한다.
#define HSP_ERR_COIN_ADDCOIN_INVALID_COINTYPECODE		(0x0013001C) // 코인정보 오류 - coinTypeCode, itemTypeCode를 확인한다.
#define HSP_ERR_COIN_ADDCOIN_UNKNOWN					(0x00130010) // 기타
// GetCoinBalance
#define HSP_ERR_COIN_GETBALANCE_INVALID_TARGETMEMBERNO 	(0x00130031) // 유효하지 않은 USER 정보 - targetMemberNo 항목을 확인한다.
#define HSP_ERR_COIN_GETBALANCE_INVALID_ARGUMENT		(0x00130032) // 요청 인자 오류 - 인자를 다시 확인한다.
#define HSP_ERR_COIN_GETBALANCE_INVALID_COINTYPECODE	(0x00130033) // 코인정보 오류 - coinTypeCode, itemTypeCode를 확인한다.
#define HSP_ERR_COIN_GETBALANCE_UNKNOWN					(0x00130030) // 기타
// GetCoinBalance2
#define HSP_ERR_COIN_GETBALANCE2_INVALID_TARGETMEMBERNO	(0x00130041) // 유효하지 않은 USER 정보 - targetMemberNo 항목을 확인한다.
#define HSP_ERR_COIN_GETBALANCE2_INVALID_ARGUMENT		(0x00130042) // 요청 인자 오류 - 인자를 다시 확인한다.
#define HSP_ERR_COIN_GETBALANCE2_INVALID_COINTYPECODE	(0x00130043) // 코인정보 오류 - coinTypeCode, itemTypeCode를 확인한다.
#define HSP_ERR_COIN_GETBALANCE2_UNKNOWN				(0x00130040) // 기타
// GetAddCoinHistory
#define HSP_ERR_COIN_COINHISTROY_INVALID_TARGETMEMBERNO	(0x00130051) // 유효하지 않은 USER 정보 - targetMemberNo 항목을 확인한다.
#define HSP_ERR_COIN_COINHISTROY_INVALID_ARGUMENT		(0x00130052) // 요청 인자 오류 - 인자를 다시 확인한다.
#define HSP_ERR_COIN_COINHISTROY_INVALID_COINTYPECODE	(0x00130053) // 코인정보 오류 - coinTypeCode, itemTypeCode를 확인한다.
#define HSP_ERR_COIN_COINHISTROY_UNKNOWN				(0x00130050) // 기타

////////////////////////////////////////////////////////////
// ERROR CODE PAYMENT
#define HSP_ERR_PAYMENT_PRODUCT_REQUEST_INVALID_IDENTIFIER	(0x00080002)
#define HSP_ERR_PAYMENT_NOT_LOGGED_IN						(0x00080004)
#define HSP_ERR_PAYMENT_TRANSACTION_IS_NOT_FINISHED_YET		(0x00080005)
#define HSP_ERR_PAYMENT_SK_UNKNOWN							(0x00080006)
#define HSP_ERR_PAYMENT_SK_CLIENT_INVALID					(0x00080007)
#define HSP_ERR_PAYMENT_SK_PAYMENT_CANCELLED				(0x00080008)
#define HSP_ERR_PAYMENT_SK_INVALID							(0x00080009)
#define HSP_ERR_PAYMENT_SK_DEVICE_NOT_ALLOWED				(0x0008000A)
#define HSP_ERR_PAYMENT_CONFIRM_ORDER_FAIL					(0x0008000C)
#define HSP_ERR_PAYMENT_SK_TIMEOUT							(0x0008000D)
#define HSP_ERR_PAYMENT_USER_NOT_ALLOW_TO_PURCHASE			(0x0008000E)
#define HSP_ERR_PAYMENT_APPSTORE_PAYMENT_FAILED             (0x0008000F)
#define HSP_ERR_PAYMENT_APPSTORE_UNDER_MAINTENANCE          (0x00080010)
#define HSP_ERR_PAYMENT_GREE_PAYMENT						(0x00080011)
#define HSP_ERR_PAYMENT_GREE_PAYMENT_VERIFY					(0x00080012)

#define HSP_ERR_PAYMENT_COM_MANDATOR_PARAM					(0x000A0001)
#define HSP_ERR_PAYMENT_COM_CHECK_PRODUCT					(0x000A0002)
#define HSP_ERR_PAYMENT_COM_SELECT_TX						(0x000A0003)
#define HSP_ERR_PAYMENT_COM_UNEXPECTED						(0x000A0004)
#define HSP_ERR_PAYMENT_COM_DB								(0x000A0005)
#define HSP_ERR_PAYMENT_PRP_GATEWAY_ERROR					(0x000A0101)
#define HSP_ERR_PAYMENT_PRP_PARENT_AGREE					(0x000A0102)
#define HSP_ERR_PAYMENT_PRP_PAYMENT_AGREE					(0x000A0103)
#define HSP_ERR_PAYMENT_PRP_OVER_LIMIT						(0x000A0104)
#define HSP_ERR_PAYMENT_PRP_NETWORK							(0x000A0105)
#define HSP_ERR_PAYMENT_ITM_ABNOMAL_RECEIPT					(0x000A0201)
#define HSP_ERR_PAYMENT_ITM_NULLL_RECEIPT					(0x000AF202)
#define HSP_ERR_PAYMENT_ITM_VERIFY_PRODINFO	 				(0x000A0203)
#define HSP_ERR_PAYMENT_ITM_VERIFY_NETWORK	 				(0x000AF204)
#define HSP_ERR_PAYMENT_ITM_BILLING_NOTI	 				(0x000AF205)
#define HSP_ERR_PAYMENT_ITM_GIVEITEM	 					(0x000AF206)
#define HSP_ERR_PAYMENT_ITM_GIVEITEM_NETWORK				(0x000AF207)
#define HSP_ERR_PAYMENT_ITM_GIVEITEM_ETC					(0x000AF208)
#define HSP_ERR_PAYMENT_ITM_RETRY_LIMIT_END	 				(0x000AF2FF)

////////////////////////////////////////////////////////////
// ERROR CODE CROSS-GAME PROMOTION
#define HSP_ERR_CGP_LAUNCHING_FAILED						(0x00090001)
#define HSP_ERR_CGP_HSP_LOG_IN_REQUIRED						(0x00090002)
#define HSP_ERR_CGP_SERVER_RESPONSE_NOT_PROPERLY			(0x00090003)

#endif
