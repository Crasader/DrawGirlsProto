//
//  HSPUiReference.h
//  HSPUIReference
//
//  Created by  on 11. 11. 28..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _HSPUiReference_h
#define _HSPUiReference_h

#import "HSPUiLauncher.h"
#import "HSPUiReferenceFactory.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HSPUI DEFAULT UI REFERENCE SETTING MACRO
//
/**
 * Re-defines HSPUiFactory to create HSP UI.
 */
#define USE_HSP_DEFAULT_UI_REFERENCE \
[HSPUiLauncher sharedLauncher].uiFactory = [[[HSPUiReferenceFactory alloc] init] autorelease]

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HSPUI URI PREDEFINES
//
// * CATEGORY SETTINGS
#define HSPUI_URI_SETTINGS                                      (@"HSPUI://settings")
#define HSPUI_URI_SETTINGS_SNS                                  (@"HSPUI://settings.sns")
#define HSPUI_URI_SETTINGS_ACCOUNT                              (@"HSPUI://settings.account")
#define HSPUI_URI_SETTINGS_ACCOUNT_MAPPING                      (@"HSPUI://settings.account.mapping")
#define HSPUI_URI_SETTINGS_ACCOUNT_RESET                        (@"HSPUI://settings.account.reset")

//
// * CATEGORY PROFILE
#define HSPUI_URI_PROFILE_MYPROFILE                             (@"HSPUI://profile.myprofile")
#define HSPUI_URI_PROFILE_USERPROFILE                           (@"HSPUI://profile.userprofile")
#define HSPUI_URI_PROFILE_MYPROFILE_DETAIL                      (@"HSPUI://profile.myprofile.detail")
#define HSPUI_URI_PROFILE_USERPROFILE_DETAIL                    (@"HSPUI://profile.userprofile.detail")
#define HSPUI_URI_PROFILE_MYPROFILE_SETTING_NICKNAME            (@"HSPUI://profile.myprofile.setting.nickname")
#define HSPUI_URI_PROFILE_MYPROFILE_SETTING_TODAYWORDS          (@"HSPUI://profile.myprofile.setting.todaywords")
#define HSPUI_URI_PROFILE_MYPROFILE_SETTING_GENDER              (@"HSPUI://profile.myprofile.setting.gender")
#define HSPUI_URI_PROFILE_MYPROFILE_SETTING_PRIVACY             HSPUI_URI_SETTINGS                              // will be deprecated
#define HSPUI_URI_PROFILE_MYPROFILE_ACCOUNTINFO                 HSPUI_URI_SETTINGS_ACCOUNT_MAPPING              // will be deprecated
#define HSPUI_URI_PROFILE_MYPROFILE_ACCOUNTINFO_RESET           HSPUI_URI_SETTINGS_ACCOUNT_RESET                // will be deprecated
#define HSPUI_URI_PROFILE_MYPROFILE_AGREEMENT_USECONTACTS       (@"HSPUI://profile.myprofile.agreement.usecontacts")

//
// * CATEGORY ACHIEVEMENT
#define HSPUI_URI_ACHIEVEMENT_LIST                              (@"HSPUI://achievement.list")
#define HSPUI_URI_ACHIEVEMENT_VS                                (@"HSPUI://achievement.vs")

//
// * CATEGORY RANKING
#define HSPUI_URI_RANKING_LIST                                  (@"HSPUI://ranking.list")
#define HSPUI_URI_RANKING_VS                                    (@"HSPUI://ranking.vs")
#define HSPUI_URI_RANKING_DETAIL                                (@"HSPUI://ranking.detail")

//
// * CATEGORY GAME
#define HSPUI_URI_GAME_LIST                                     (@"HSPUI://profile.recentplayedgamelist")

//
// * CATEGORY SOCIAL
#define HSPUI_URI_SOCIAL_FOLLOWINGLIST                          (@"HSPUI://social.followinglist")
#define HSPUI_URI_SOCIAL_BLOCKINGLIST                           (@"HSPUI://social.blockinglist")
#define HSPUI_URI_SOCIAL_RECOMMENDLIST                          (@"HSPUI://social.recommendlist")
#define HSPUI_URI_SOCIAL_FOLLOW                                 (@"HSPUI://social.follow")
#define HSPUI_URI_SOCIAL_BLOCK									(@"HSPUI://social.block")

//
// * CATEGORY NOTIFICATIONS
#define HSPUI_URI_MESSAGE_LIST                                  (@"HSPUI://message.list")

//
// * CATEGORY GAMEPLUS
#define HSPUI_URI_GAMEPLUS                                      (@"HSPUI://gameplus")

//
// * CATEGORY MORE
#define HSPUI_URI_MORE                                          (@"HSPUI://more")

//
// * CATEGORY SUPPORTS
#define HSPUI_URI_SUPPORT_CSCENTER                              (@"HSPUI://supports.cscenter")
#define HSPUI_URI_SUPPORT_NOTICE                                (@"HSPUI://supports.notice")
#define HSPUI_URI_SUPPORT_FAQ                                   (@"HSPUI://supports.faq")
#define HSPUI_URI_SUPPORT_TERMS_PRIVACY                         (@"HSPUI://supports.terms.privacy")
#define HSPUI_URI_SUPPORT_TERMS_USE                             (@"HSPUI://supports.terms.use")
#define HSPUI_URI_SUPPORT_TERMS_HANGAME                         (@"HSPUI://supports.terms.hangame")
#define HSPUI_URI_SUPPORT_TERMS_PUNISH                          (@"HSPUI://supports.terms.punish")

@interface HSPUiReference : NSObject

@end

#endif
