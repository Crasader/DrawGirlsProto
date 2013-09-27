#include "KSUtil.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <UIKit/UIKit.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include "jni.h"
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif
namespace KS
{
	//	CCSprite* spriteWithSpriteFrameName(const char *pszSpriteFrameName)
	//	{
	//		CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
	//
	//		if(pFrame)
	//			return CCSprite::createWithSpriteFrame(pFrame);
	//		else
	//			return 0;
	//
	//	}
	/** Compress a STL string using zlib with given compression level and return
	 * the binary data. */
	std::string compress_string(const std::string& str,
								int compressionlevel)
	{
		z_stream zs;                        // z_stream is zlib's control structure
		memset(&zs, 0, sizeof(zs));
		
		if (deflateInit(&zs, compressionlevel) != Z_OK)
			throw(std::runtime_error("deflateInit failed while compressing."));
		
		zs.next_in = (Bytef*)str.data();
		zs.avail_in = str.size();           // set the z_stream's input
		
		int ret;
		char outbuffer[32768];
		std::string outstring;
		
		// retrieve the compressed bytes blockwise
		do {
			zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
			zs.avail_out = sizeof(outbuffer);
			
			ret = deflate(&zs, Z_FINISH);
			
			if (outstring.size() < zs.total_out) {
				// append the block to the output string
				outstring.append(outbuffer,
								 zs.total_out - outstring.size());
			}
		} while (ret == Z_OK);
		
		deflateEnd(&zs);
		
		if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
			std::ostringstream oss;
			oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
			throw(std::runtime_error(oss.str()));
		}
		
		return outstring;
	}
	
	/** Decompress an STL string using zlib and return the original data. */
	std::string decompress_string(const std::string& str)
	{
		z_stream zs;                        // z_stream is zlib's control structure
		memset(&zs, 0, sizeof(zs));
		
		if (inflateInit(&zs) != Z_OK)
			throw(std::runtime_error("inflateInit failed while decompressing."));
		
		zs.next_in = (Bytef*)str.data();
		zs.avail_in = str.size();
		
		int ret;
		char outbuffer[32768];
		std::string outstring;
		
		// get the decompressed bytes blockwise using repeated calls to inflate
		do {
			zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
			zs.avail_out = sizeof(outbuffer);
			
			ret = inflate(&zs, 0);
			
			if (outstring.size() < zs.total_out) {
				outstring.append(outbuffer,
								 zs.total_out - outstring.size());
			}
			
		} while (ret == Z_OK);
		
		inflateEnd(&zs);
		
		if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
			std::ostringstream oss;
			oss << "Exception during zlib decompression: (" << ret << ") "
			<< zs.msg;
			throw(std::runtime_error(oss.str()));
		}
		
		return outstring;
	}
	string getLocalCode()
	{
		string tempCode;
		
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
		NSString *currentLanguage = [languages objectAtIndex:0];
		
		// get the current language code.(such as English is "en", Chinese is "zh" and so on)
		NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
		NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
		
		tempCode = [languageCode cStringUsingEncoding:NSASCIIStringEncoding];
		
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		tempCode = getCurrentLanguageJNI();
#endif
		return tempCode;
	}
	
	void setAllVisible(CCNode* n, bool b)
	{
		if(n)
		{
			n->setVisible(b);
			CCArray* ch = n->getChildren();
			if(ch)
			{
				for(int i=0; i<ch->count(); i++)
				{
					CCNode* chn = (CCNode*)ch->objectAtIndex(i);
					setAllVisible(chn, b);
				}
			}
			
		}
		else
		{
			
		}
	}
	
	bool isExistFile(const std::string& fileName)
	{
		return CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str()));
	}
}

