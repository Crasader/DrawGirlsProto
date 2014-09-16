//
//  BackKeyBase.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 9. 15..
//
//

#include "BackKeyBase.h"
#include "BackKeyManager.h"

BackKeyBase::BackKeyBase() :
over_popup_count(0),
is_action_enabled(false),
back_action(nullptr)
{
	myBKM->addBase(this);
}

BackKeyBase::~BackKeyBase()
{
	myBKM->removeBase(this);
}

void BackKeyBase::setBackKeyEnabled(bool t_b)
{
	is_action_enabled = t_b;
}
bool BackKeyBase::isBackKeyEnabled()
{
	return is_action_enabled;
}
void BackKeyBase::setBackKeyFunc(function<void()> t_func)
{
	back_action = t_func;
}
void BackKeyBase::onBackKeyAction()
{
	if(is_action_enabled && over_popup_count == 0 && back_action != nullptr)
	{
		back_action();
		is_action_enabled = false;
		back_action = nullptr;
	}
}