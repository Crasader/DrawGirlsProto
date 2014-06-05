#include "CommonAnimation.h"
#include "KSUtil.h"

CommonAnimation::CommonAnimation()
{
	
}
CommonAnimation::~CommonAnimation()
{
}

void CommonAnimation::openPopup(CCNode* node, CCNode* t_container, CCNodeRGBA* gray,
																std::function<void(void)> ingFunctor, std::function<void(void)> endFunctor)
{
	t_container->setScaleY(0.f);
	CCNode* t_popup = node;
	t_popup->addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
		t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.8f);
			t_popup->addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.f);}));}));}));
	
	t_popup->addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
	{
		if(gray)
			gray->setOpacity(t);
		KS::setOpacity(t_container, t);
		
		if(ingFunctor)
			ingFunctor();
	}, [=](int t)
	{
		if(gray)
			gray->setOpacity(255);
		KS::setOpacity(t_container, 255);
		
		if(endFunctor)
			endFunctor();
	}));
}