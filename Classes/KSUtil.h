//
//  KSUtil.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 13..
//
//

#ifndef DGproto_KSUtil_h
#define DGproto_KSUtil_h

#pragma once
#include <string>
#include <stdarg.h>
#include <algorithm>
#include <iterator>
#include <locale>
#include <functional>

#include "utf8.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "FromTo.h"

#define ThisClassType remove_pointer<decltype(this)>::type
using namespace cocos2d;
USING_NS_CC_EXT;
using namespace std;

#ifndef nil
#define nil (0)
#endif

#ifndef NULL
#define NULL (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define ENABLE_CODE TRUE
#include <sstream>
#include <iostream>

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <zlib.h>


namespace KS
{
	//	CCSprite* spriteWithSpriteFrameName(const char *pszSpriteFrameName);
	/** Compress a STL string using zlib with given compression level and return
	 * the binary data. */
	std::string compress_string(const std::string& str,
								int compressionlevel = Z_BEST_COMPRESSION);
	
	/** Decompress an STL string using zlib and return the original data. */
	std::string decompress_string(const std::string& str);
	
	string getLocalCode();
	
	string insert_separator(const string& s, char separator = ',', int width = 3);
	
	
	static void __KSLogCore(ostringstream& oss, const char* s) {
		
		while (*s) {
			if (*s == '%' && *++s != '%') {
				throw std::runtime_error(
										 "invalid format string: missing arguments");
			}
			oss << *s++;
		}
	}
	
	template<typename T, typename... Args>
	void __KSLogCore(ostringstream& oss, const char* s, T value, Args... args) {
		while (*s) {
			if (*s == '%' && *++s != '%') {
				oss << value;
				//				return;
				return __KSLogCore(oss, s, args...);
			}
			oss << *s++;
		}
		throw std::runtime_error(
								 "extra arguments provided to printf");
	}
	
	template<typename... Args>
	void KSLog(const char* s, Args... args) {
		ostringstream oss;
		__KSLogCore(oss, s, args...);
		CCLog("%s", oss.str().c_str());
	}
	
	void setAllVisible(CCNode* n, bool b);
	
	bool isExistFile(const std::string& fileName);
	void setOpacity(CCObject* object, GLubyte opaque);
	
	template <typename NodeT>
	pair<NodeT, CCBAnimationManager*> loadCCBI(CCObject* thiz, const std::string& fn)
	{
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		CCNode* p = reader->readNodeGraphFromFile(fn.c_str(), thiz);
		pair<NodeT, CCBAnimationManager*> retValue;
		retValue.first = dynamic_cast<NodeT>(p);
		retValue.second = reader->getAnimationManager();
		reader->release();
		
		return retValue;
	}
	/*
	 CCNode* CCBReader::readNodeGraphFromFile(const char* pCCBFileName, CCObject* pOwner)
	 {
	 return this->readNodeGraphFromFile(pCCBFileName, pOwner, CCDirector::sharedDirector()->getWinSize());
	 }
	 
	 CCNode* CCBReader::readNodeGraphFromFile(const char *pCCBFileName, CCObject *pOwner, const CCSize &parentSize)

	 */
	template <typename NodeT>
	pair<NodeT, CCBAnimationManager*> loadCCBIForFullPath(CCObject* thiz, const std::string fullPath)
	{
		CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
		CCBReader* reader = new CCBReader(nodeLoader);
		///////////////////////
		CCNode* p;
		{
			do
			{
				if (fullPath == "")
				{
					p = nullptr;
					break;
				}
				
				std::string strCCBFileName(fullPath);
				std::string strSuffix(".ccbi");
				// Add ccbi suffix
				if (!CCBReader::endsWith(strCCBFileName.c_str(), strSuffix.c_str()))
				{
					strCCBFileName += strSuffix;
				}
				
				std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(strCCBFileName.c_str());
				unsigned long size = 0;
				
				unsigned char * pBytes = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "rb", &size);
				CCData *data = new CCData(pBytes, size);
				CC_SAFE_DELETE_ARRAY(pBytes);
				
				CCNode *ret = reader->readNodeGraphFromData(data, thiz, CCDirector::sharedDirector()->getWinSize());
				
				data->release();
				
				p = ret;
			}while(0);
			
		}

		//////////////////////////
		pair<NodeT, CCBAnimationManager*> retValue;
		retValue.first = dynamic_cast<NodeT>(p);
		retValue.second = reader->getAnimationManager();
		reader->release();
		return retValue;
	}
}

class KS_Util
{
public:
	static string stringWithFormat(const char* _Format, ...)
	{
		va_list argptr;
		va_start(argptr, _Format);
		char buf[256];
		int ret = vsprintf(buf, _Format, argptr);
		string str = buf;
		
		va_end(argptr);
		return str;
	}
	static wstring ansi2utf16(std::string const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = std::mbstate_t();
		std::vector<wchar_t> buf(str.size() + 1);
		char const* in_next = str.c_str();
		wchar_t* out_next = &buf[0];
		std::codecvt_base::result r = codecvt.in(state,
												 str.c_str(), str.c_str() + str.size(), in_next,
												 &buf[0], &buf[0] + buf.size(), out_next);
		if (r == std::codecvt_base::error)
			throw std::runtime_error("can't convert string to wstring");
		return std::wstring(&buf[0]);
	}
	static string uni2ansi(std::wstring const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = std::mbstate_t();
		std::vector<char> buf((str.size() + 1) * codecvt.max_length());
		wchar_t const* in_next = str.c_str();
		char* out_next = &buf[0];
		std::codecvt_base::result r = codecvt.out(state,
												  str.c_str(), str.c_str() + str.size(), in_next,
												  &buf[0], &buf[0] + buf.size(), out_next);
		if (r == std::codecvt_base::error)
			throw std::runtime_error("can't convert wstring to string");
		return std::string(&buf[0]);
	}
	static string ansi2UTF8(const string& str)
	{
		//return string("daf");
		//CCLog(str.c_str());
		wstring str2 = ansi2utf16(str);
		//wstring str3(L"a???");
		string result;
		utf8::utf16to8(str2.begin(), str2.end(), back_inserter(result));
		return result;
	}
	static int getNumberOfChild(CCNode* node) {
		if (node->getChildren() != NULL) {
			if (node->getChildren()->count() == 0) {
				return 0;
			} else {
				int total = 0;
				CCObject* n;
				CCARRAY_FOREACH(node->getChildren(), n) {
					CCNode* node = dynamic_cast<CCNode*>(n);
					if (node) {
						int p = getNumberOfChild(node);
						//						CCLog("%d %x -- %d", node->getZOrder(),node, p);
						total += 1 + p;
					}
				}
				//				CCLog("%d..", total);
				return total;
			}
		} else {
			return 0;
		}
	}
};


class KSTimer : public CCNode
{
protected:
	std::function<void(void)> f;
	float m_s;
	float m_timer;
public:
	virtual ~KSTimer(){}
	void after()
	{
		unscheduleUpdate();
		removeFromParent();
		f();
	}
	static KSTimer* create(float s, std::function<void(void)> __f)
	{
		KSTimer* kt = new KSTimer;
		kt->init(s, __f);
		kt->autorelease();
		return kt;
	}
	bool init(float s, std::function<void(void)> __f)
	{
		f = __f;
		m_s = s;
		m_timer = 0;
		scheduleUpdate();
		return true;
	}
	void update(float dt)
	{
		m_timer += dt;
		if(m_timer >= m_s)
		{
			after();
		}
	}
};

template <typename T>
class KSGradualValue : public CCNode
{
protected:
	std::function<void(T)> m_f;
	std::function<void(T)> m_fFinish;
	T m_a;
	T m_b;
	T m_s;
	FromToWithDuration2<T> fromTo;
public:
	virtual ~KSGradualValue(){
	}
	static KSGradualValue* create(T a, T b, float s, std::function<void(T)> __f)
	{
		KSGradualValue* newO = new KSGradualValue;
		newO->init(a, b, s, __f);
		newO->autorelease();
		return newO;
	}
	static KSGradualValue* create(T a, T b, float s, std::function<void(T)> __f, std::function<void(T)> __finish)
	{
		KSGradualValue* newO = new KSGradualValue;
		newO->init(a, b, s, __f, __finish);
		newO->autorelease();
		return newO;
	}
	bool init(T a, T b, float s, std::function<void(T)> __f, std::function<void(T)> __finish)
	{
		
		m_a = a;
		m_b = b;
		m_f = __f;
		m_fFinish = __finish;
		
		fromTo.init(m_a, m_b, s);
		
		scheduleUpdate();
		return true;
	}
	bool init(T a, T b, float s, std::function<void(T)> __f)
	{
		return init(a, b, s, __f, []{});
	}
	void update(float dt)
	{
		dt = 1/60.f;
		T val = fromTo.getValue();
		m_f(val);
		bool result = fromTo.step(dt);
		if(result)
		{
		}
		else
		{
			m_fFinish(val);
			removeFromParent();
			unscheduleUpdate();
		}
	}
};

class KSNode : public CCNode
{
public:
	void alignItemsVerticallyWithPadding(float padding)
	{
		float height = -padding;
		if (m_pChildren && m_pChildren->count() > 0)
		{
			CCObject* pObject = NULL;
			CCARRAY_FOREACH(m_pChildren, pObject)
			{
				CCNode* pChild = dynamic_cast<CCNode*>(pObject);
				if (pChild)
				{
					height += pChild->getContentSize().height * pChild->getScaleY() + padding;
				}
			}
		}
		
		float y = height / 2.0f;
		if (m_pChildren && m_pChildren->count() > 0)
		{
			CCObject* pObject = NULL;
			CCARRAY_FOREACH(m_pChildren, pObject)
			{
				CCNode* pChild = dynamic_cast<CCNode*>(pObject);
				if (pChild)
				{
					pChild->setPosition(ccp(0, y - pChild->getContentSize().height * pChild->getScaleY() / 2.0f));
					y -= pChild->getContentSize().height * pChild->getScaleY() + padding;
				}
			}
		}
	}
};

#endif
