#include "KsLocal.h"

#include <Foundation/Foundation.h>
std::string KsLocal::getLocalCode()
{
	string tempCode;
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
	NSString *currentLanguage = [languages objectAtIndex:0];
	
	// get the current language code.(such as English is "en", Chinese is "zh" and so on)
	NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
	NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
	
	tempCode = [languageCode cStringUsingEncoding:NSASCIIStringEncoding];
//    tempCode = "ko";
	return tempCode;
}