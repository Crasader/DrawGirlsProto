#ifndef COMMONANIMATION_H
#define COMMONANIMATION_H

#include "cocos2d.h"
USING_NS_CC;
#include <functional>

class CommonAnimation
{
public:
	CommonAnimation();
	virtual ~CommonAnimation();
	static void openPopup(CCNode* node, CCNode* t_container, CCNodeRGBA* gray,
												std::function<void(void)> ingFunctor = nullptr, std::function<void(void)> endFunctor = nullptr);
	static void closePopup(CCNode* node, CCNode* t_container, CCNodeRGBA* gray,
												std::function<void(void)> ingFunctor = nullptr, std::function<void(void)> endFunctor = nullptr);
	//virtual void registerWithTouchDispatcher();
};

#endif
