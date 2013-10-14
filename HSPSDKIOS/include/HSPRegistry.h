//
//  HSPRegistry.h
//  HSPCore
//
//  Created by Hyunseung Jung on 11. 4. 26..
//  Copyright 2011 nhncorp. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * @brief A utility interface that stores the application data in the format of key-value in a local storage or memory. 
 * If the data is stored in a local storage, it is not deleted even when the application is terminated.
 */
@interface HSPRegistry : NSObject
{
@private
	NSMutableDictionary*	_registryNVDictionary;
	NSMutableDictionary*	_registryDictionary;
}

/**
 * @brief A dictionary object to store data in a local storage. The data stored in a local storage is not deleted even when the application is terminated.
 */
@property (nonatomic, retain)		NSDictionary*	registryNVDictionary;

/**
 * @brief A dictionary object to store data in a memory. The data stored in a memory is deleted when the application is terminated.
 */
@property (nonatomic, retain)		NSDictionary*	registryDictionary;

/**
 * @brief Stores data in the registry.
 * @param object Object to store. [in]
 * @param key Key of the object to store. [in]
 * @param toNV Specifies the repository. (YES : Not volatile, NO : volatile) [in]
 */
- (void)setObject:(id)object forKey:(id)key toNV:(BOOL)toNV;

/**
 * @brief Extracts data from the registry. 
 * @param key Key of the object to extract. [in]
 * @param toNV Specifies the repository. ( YES : Not volatile, NO : volatile ) [in]
 * @return id The stored object.
 */
- (id)objectForKey:(id)key fromNV:(BOOL)fromNV;

/**
 * @brief Deletes data from the registry.
 * @param key Key of the object to delete. [in]
 * @param toNV Specifies the repository. ( YES : Not volatile, NO : volatile ) [in]
 */
- (void)removeObjectForKey:(id)key fromNV:(BOOL)fromNV;

/**
 * @brief Deletes all data from the registry.
 * @param toNV Specifies the repository. ( YES : Not volatile, NO : volatile ) [in]
 */
- (void)removeAllObjectsFromNV:(BOOL)fromNV;

@end
