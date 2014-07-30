#ifndef COMMONANIMATION_H
#define COMMONANIMATION_H

#include "cocos2d.h"
USING_NS_CC;
#include <functional>

class KSLabelTTF;
class CommonAnimation
{
public:
	CommonAnimation();
	virtual ~CommonAnimation();
	static void openPopup(CCNode* node, CCNode* t_container, CCNodeRGBA* gray,
												std::function<void(void)> ingFunctor = nullptr, std::function<void(void)> endFunctor = nullptr);
	static void closePopup(CCNode* node, CCNode* t_container, CCNodeRGBA* gray,
												std::function<void(void)> ingFunctor = nullptr, std::function<void(void)> endFunctor = nullptr);
	static void applyShadow(KSLabelTTF* parent);
	static void applyShadow(KSLabelTTF* parent, float shadowFontSize);
	static KSLabelTTF* applyBigShadow(KSLabelTTF* parent, float shadowFontSize);
	//virtual void registerWithTouchDispatcher();
};

#endif
