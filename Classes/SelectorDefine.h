//
//  SelectorDefine.h
//  DrawingJack
//
//  Created by 사원3 on 13. 6. 4..
//
//

#ifndef DrawingJack_SelectorDefine_h
#define DrawingJack_SelectorDefine_h

#include "cocos2d.h"
#include "IntSeries.h"

using namespace cocos2d;

typedef void (CCObject::*SEL_CallFuncF)(float);
#define callfuncF_selector(_SELECTOR) (SEL_CallFuncF)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncI)(int);
#define callfuncI_selector(_SELECTOR) (SEL_CallFuncI)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncB)(bool);
#define callfuncB_selector(_SELECTOR) (SEL_CallFuncB)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCp)(CCPoint);
#define callfuncCCp_selector(_SELECTOR) (SEL_CallFuncCCp)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCpColor)(CCPoint, ccColor4F);
#define callfuncCCpColor_selector(_SELECTOR) (SEL_CallFuncCCpColor)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCpI)(CCPoint, int);
#define callfuncCCpI_selector(_SELECTOR) (SEL_CallFuncCCpI)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCpB)(CCPoint, bool);
#define callfuncCCpB_selector(_SELECTOR) (SEL_CallFuncCCpB)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIIF)(int, int, float);
#define callfuncIIF_selector(_SELECTOR) (SEL_CallFuncIIF)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIIFCCp)(int, int, float, CCPoint);
#define callfuncIIFCCp_selector(_SELECTOR) (SEL_CallFuncIIFCCp)(&_SELECTOR)

typedef int (CCObject::*SEL_ICallFunc)();
#define icallfunc_selector(_SELECTOR) (SEL_ICallFunc)(&_SELECTOR)

typedef float (CCObject::*SEL_FCallFunc)();
#define fcallfunc_selector(_SELECTOR) (SEL_FCallFunc)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncCCpODv)(CCPoint, CCObject*, SEL_CallFunc);
#define callfuncCCpODv_selector(_SELECTOR) (SEL_CallFuncCCpODv)(&_SELECTOR)

typedef bool (CCObject::*SEL_BCallFunc)();
#define bcallfunc_selector(_SELECTOR) (SEL_BCallFunc)(&_SELECTOR)

typedef CCNode* (CCObject::*SEL_NCallFunc)();
#define ncallfunc_selector(_SELECTOR) (SEL_NCallFunc)(&_SELECTOR)

typedef CCArray* (CCObject::*SEL_ACallFunc)();
#define acallfunc_selector(_SELECTOR) (SEL_ACallFunc)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncOFF)(CCObject*, float, float);
#define callfuncOFF_selector(_SELECTOR) (SEL_CallFuncOFF)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIpOC)(IntPoint, CCObject*, SEL_CallFuncI);
#define callfuncIpOC_selector(_SELECTOR) (SEL_CallFuncIpOC)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIpIII)(IntPoint, int, int, int);
#define callfuncIpIII_selector(_SELECTOR) (SEL_CallFuncIpIII)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncFBCCp)(float, bool, CCPoint);
#define callfuncFBCCp_selector(_SELECTOR) (SEL_CallFuncFBCCp)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncTDTD)(CCObject*, SEL_CallFunc, CCObject*, SEL_CallFunc);
#define callfuncTDTD_selector(_SELECTOR) (SEL_CallFuncTDTD)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncII)(int, int);
#define callfuncII_selector(_SELECTOR) (SEL_CallFuncII)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIp)(IntPoint);
#define callfuncIp_selector(_SELECTOR) (SEL_CallFuncIp)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncIpv)(IntPointVector);
#define callfuncIpv_selector(_SELECTOR) (SEL_CallFuncIpv)(&_SELECTOR)

#endif
