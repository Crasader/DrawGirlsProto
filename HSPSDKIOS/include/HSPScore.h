//
//  HSPScore.h
//  HSPInterface
//
//  Created by Wooyong Kim on 11. 10. 24..
//  Copyright 2011 nhn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HSPError.h"

/**
 * @brief Deals with score data.
 */
@interface HSPScore : NSObject {
    double      _score;
    int32_t     _grade;
    int32_t     _changeGrade;
    int64_t     _memberNo;
    NSString*   _extraData;
}

/**
 * @brief Score. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             double      score;

/**
 * @brief Ranking. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int32_t     grade;

/**
 * @brief Change compared to the previous ranking. (read only)<br>
 * If this value is 0XEFFFFFFF, the ranking is new.
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int32_t     changeGrade;

/**
 * @brief The member number who gets the score. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, readonly)             int64_t     memberNo;

/**
 * @deprecated
 * @brief Extra data. (read only)
 * @serviceDomain HANGAME LINEGAME GLOBALGAME_SG
 */
@property (nonatomic, retain, readonly)     NSString*   extraData;

@end
