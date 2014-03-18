//
//  Litmus.h
//  Litmus
//
//  Created by moonriver on 13. 2. 26..
//  Copyright (c) 2013년 moonriver. All rights reserved.
//

#import <Foundation/Foundation.h>

#define CDN_HSP 9999
#define CDN_LINE 9998

#define OPT_CHECK_JAILBREAK 0x01
#define OPT_CHECK_CHEAT     0x02

#define OPT_NONE   0x000
#define OPT_ALL_FUNC       OPT_CHECK_JAILBREAK|OPT_CHECK_CHEAT


#define OPT_APPNAME      0x002
#define OPT_OSVER       0x008
#define OPT_COUNTRYCODE 0x010
#define OPT_DEVICEMODEL  0x020


#define OPT_ALL_INFO     OPT_APPNAME|OPT_OSVER|OPT_COUNTRYCODE|OPT_DEVICEMODEL

@interface Litmus : NSObject

+ (int) getVersion;
+ (int) getPatternVersion;
+ (long) getCDN;

- (void) setFuncLogOpt:(long) opt;
- (void) setLogOpt:(long) opt;

- (int) LitmusInit;
- (int) LitmusUpdate:(long)cdn;
- (void)setGameVersion:(NSString* )ver;
- (void)setGameSno:(long )sno;
- (void)setGameUDID:(NSString *)udid;
//- (void)setLogDisable;

- (BOOL) CheckJailBreak;
- (BOOL) CheckGameCheat:(NSMutableArray*)result;
@end
