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
	string insert_separator(const string& s, char separator, int width)
	{

		string ss = s;
		string::iterator i = ss.end() - width;
		while (i > ss.begin())
			i = ss.insert(i, separator) - width;
		
		return ss;
	}
	void setOpacity(CCObject* object, GLubyte opaque)
	{
		CCArray* childs = NULL;
		CCObject* child = NULL;
		CCNode* nodeObject = (CCNode*)object;
		CCRGBAProtocol* mine = dynamic_cast<CCRGBAProtocol*>(nodeObject);
		if(mine)
			mine->setOpacity(opaque);
		if (nodeObject) {
			childs = nodeObject->getChildren();
		}
		if (childs) {
			CCARRAY_FOREACH(childs, child){
				CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(child);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setOpacity(opaque);
					// pRGBAProtocol->setColor(ccc3(opaque, opaque, opaque));
				}
				CCNode* singleObject = (CCNode*)child;
				if (singleObject && singleObject->getChildrenCount()>0) {
					setOpacity(singleObject, opaque);
				}
			}
		}
	}
	
	void setColor(CCObject* object, const ccColor3B& color)
	{
		CCArray* childs = NULL;
		CCObject* child = NULL;
		CCNode* nodeObject = (CCNode*)object;
		CCNodeRGBA* mine = dynamic_cast<CCNodeRGBA*>(nodeObject);
		if(mine)
			mine->setColor(color);
		if (nodeObject) {
			childs = nodeObject->getChildren();
		}
		if (childs) {
			CCARRAY_FOREACH(childs, child){
				CCNodeRGBA *pRGBAProtocol = dynamic_cast<CCNodeRGBA*>(child);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setColor(color);
					// pRGBAProtocol->setColor(ccc3(opaque, opaque, opaque));
				}
				CCNode* singleObject = (CCNode*)child;
				if (singleObject && singleObject->getChildrenCount()>0) {
					setColor(singleObject, color);
				}
			}
		}
	}
	void setBlendFunc(CCObject* object, ccBlendFunc bf)
	{
		CCArray* childs = NULL;
		CCObject* child = NULL;
		CCNode* nodeObject = (CCNode*)object;
		CCBlendProtocol* mine = dynamic_cast<CCBlendProtocol*>(nodeObject);
		if(mine)
			mine->setBlendFunc(bf);
		if (nodeObject) {
			childs = nodeObject->getChildren();
		}
		if (childs) {
			CCARRAY_FOREACH(childs, child){
				CCBlendProtocol *pRGBAProtocol = dynamic_cast<CCBlendProtocol*>(child);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setBlendFunc(bf);
					// pRGBAProtocol->setColor(ccc3(opaque, opaque, opaque));
				}
				CCNode* singleObject = (CCNode*)child;
				if (singleObject && singleObject->getChildrenCount()>0) {
					setBlendFunc(singleObject, bf);
				}
			}
		}
	}
	void setPositionType(CCObject* object, tCCPositionType pt)
	{
		CCArray* childs = NULL;
		CCObject* child = NULL;
		CCNode* nodeObject = (CCNode*)object;
		CCParticleSystemQuad* mine = dynamic_cast<CCParticleSystemQuad*>(nodeObject);
		if(mine)
			mine->setPositionType(pt);
		if (nodeObject) {
			childs = nodeObject->getChildren();
		}
		if (childs) {
			CCARRAY_FOREACH(childs, child){
				CCParticleSystemQuad *pRGBAProtocol = dynamic_cast<CCParticleSystemQuad*>(child);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setPositionType(pt);
					// pRGBAProtocol->setColor(ccc3(opaque, opaque, opaque));
				}
				CCNode* singleObject = (CCNode*)child;
				if (singleObject && singleObject->getChildrenCount()>0) {
					setPositionType(singleObject, pt);
				}
			}
		}
	}
	void setFlipX(CCObject* object, bool flip)
	{
		CCArray* childs = NULL;
		CCObject* child = NULL;
		CCNode* nodeObject = (CCNode*)object;
		CCSprite* mine = dynamic_cast<CCSprite*>(nodeObject);
		if(mine)
			mine->setFlipX(flip);
		if (nodeObject) {
			childs = nodeObject->getChildren();
		}
		if (childs) {
			CCARRAY_FOREACH(childs, child){
				CCSprite *pRGBAProtocol = dynamic_cast<CCSprite*>(child);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setFlipX(flip);
					// pRGBAProtocol->setColor(ccc3(opaque, opaque, opaque));
				}
				CCNode* singleObject = (CCNode*)child;
				if (singleObject && singleObject->getChildrenCount()>0) {
					setFlipX(singleObject, flip);
				}
			}
		}
	}
	void setFlipToggleX(CCObject* object)
	{
		CCArray* childs = NULL;
		CCObject* child = NULL;
		CCNode* nodeObject = (CCNode*)object;
		CCSprite* mine = dynamic_cast<CCSprite*>(nodeObject);
		if(mine)
			mine->setFlipX(!mine->isFlipX());
		if (nodeObject) {
			childs = nodeObject->getChildren();
		}
		if (childs) {
			CCARRAY_FOREACH(childs, child){
				CCSprite *pRGBAProtocol = dynamic_cast<CCSprite*>(child);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setFlipX(pRGBAProtocol->isFlipX());
					// pRGBAProtocol->setColor(ccc3(opaque, opaque, opaque));
				}
				CCNode* singleObject = (CCNode*)child;
				if (singleObject && singleObject->getChildrenCount()>0) {
					setFlipToggleX(singleObject);
				}
			}
		}
	}
	string longLongToStr(long long t)
	{
		int radix = 26;
		int remainer;
		//                   0123456789...
		std::string alpha = "abcdefghijklmnopqrstuvwxyz"; // max 26
		std::string result;
		while(t)
		{
			remainer = t % radix;
			result.push_back(alpha[remainer]);
			t /= radix;
		}
		return std::string(result.rbegin(), result.rend());
	}
	long long strToLongLong(const std::string& t)
	{
		int radix = 26;
		auto converter = [](char c){
			std::string alpha = "abcdefghijklmnopqrstuvwxyz"; // max 26
			for(int i=0; i<alpha.size(); i++)
			{
				if(alpha[i] == c)
				{
					return i;
				}
			}
			
			return 0;
		};
		auto longLongPow = [](int n, int r)->long long
		{
			long long result = 1;
			for(int i=0; i<r; i++)
			{
				result *= n;
			}
			return result;
		};
		long long result = 0;
		int idx = 0;
		for(auto iter = t.rbegin(); iter != t.rend(); ++iter)
		{
			int n = converter(*iter);
			result += n * longLongPow(radix, idx);
			idx++;
		}
		return result;
//		char* endP;
//		return strtoll(t.c_str(), &endP, radix);
	}
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
	
	float getTimeSec();
}

