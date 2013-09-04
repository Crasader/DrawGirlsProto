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

using namespace cocos2d;

typedef void (CCObject::*SEL_CallFuncF)(float);
#define callfuncF_selector(_SELECTOR) (SEL_CallFuncF)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncI)(int);
#define callfuncI_selector(_SELECTOR) (SEL_CallFuncI)(&_SELECTOR)

typedef void (CCObject::*SEL_CallFuncB)(bool);
#define callfuncB_selector(_SELECTOR) (SEL_CallFuncB)(&_SELECTOR)

#endif
