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

#define ThisClassType remove_pointer<decltype(this)>::type
using namespace cocos2d;
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




#endif
