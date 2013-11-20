// MissileDamageLabel.cpp
//

#include "MissileDamageLabel.h"
#define LZZ_INLINE inline
using namespace std;
MissileDamageLabel * MissileDamageLabel::create (int t_damage)
        {
		MissileDamageLabel* t_mdl = new MissileDamageLabel();
		t_mdl->myInit(t_damage);
		t_mdl->autorelease();
		return t_mdl;
	}
void MissileDamageLabel::startMyAction ()
        {
		float delay_time = 0.f;
		
		for(int i=digit_imgs.size()-1;i>=0;i--)
		{
			CCSprite* t_digit_img = digit_imgs[i];
			CCDelayTime* t_delay = CCDelayTime::create(delay_time);
			CCCallFuncO* t_call1 = CCCallFuncO::create(this, callfuncO_selector(MissileDamageLabel::visibleOn), t_digit_img);
			CCMoveBy* move1 = CCMoveBy::create(0.2f, ccp(0,10));
			CCMoveBy* move2 = CCMoveBy::create(0.1f, ccp(0,-5));
			if(i == 0)
			{
				CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(MissileDamageLabel::lastAnimation));
				CCSequence* t_seq = CCSequence::create(t_delay, t_call1, move1, move2, t_call2, NULL);
				t_digit_img->runAction(t_seq);
			}
			else
			{
				CCSequence* t_seq = CCSequence::create(t_delay, t_call1, move1, move2, NULL);
				t_digit_img->runAction(t_seq);
				delay_time += 0.1f;
			}
		}
	}
void MissileDamageLabel::lastAnimation ()
        {
		for(int i=digit_imgs.size()-1;i>=0;i--)
		{
			CCSprite* t_digit_img = digit_imgs[i];
			CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
			
			if(i == 0)
			{
				CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(MissileDamageLabel::removeFromParent));
				CCSequence* t_seq = CCSequence::create(t_fade, t_call1, NULL);
				t_digit_img->runAction(t_seq);
			}
			else
				t_digit_img->runAction(t_fade);
		}
	}
void MissileDamageLabel::visibleOn (CCObject * t_target)
        {
		((CCSprite*)t_target)->setVisible(true);
	}
void MissileDamageLabel::myInit (int t_damage)
        {
		m_batch_node = CCSpriteBatchNode::create("missile_damage_label.png");
		m_batch_node->setPosition(CCPointZero);
		addChild(m_batch_node);
		
		int calc_damage = t_damage;
		int digit_count = 0;
		int total_width = 0;
		CCRect digit_rect;
		int digit_number;
		
		do {
			digit_count++;
			digit_number = calc_damage%10;
			digit_rect = getNumberRect(digit_number);
			CCSprite* t_digit_img = CCSprite::createWithTexture(m_batch_node->getTexture(), digit_rect);
			m_batch_node->addChild(t_digit_img);
			digit_imgs.push_back(t_digit_img);
			total_width += digit_rect.size.width;
			calc_damage /= 10;
		} while (calc_damage > 0);
		
		float start_position_x = -total_width/2.f;
		for(int i=digit_imgs.size()-1;i>=0;i--)
		{
			CCSprite* t_digit_img = digit_imgs[i];
			t_digit_img->setPosition(ccp(start_position_x + t_digit_img->getContentSize().width/2.f,-5));
			t_digit_img->setVisible(false);
			start_position_x += t_digit_img->getContentSize().width;
		}
	}
CCRect MissileDamageLabel::getNumberRect (int t_number)
        {
		CCRect return_value;
		if(t_number == 0)
			return_value = CCRectMake(0, 0, 11, 17);
		else if(t_number == 1)
			return_value = CCRectMake(11, 0, 8, 17);
		else if(t_number == 2)
			return_value = CCRectMake(19, 0, 10, 17);
		else if(t_number == 3)
			return_value = CCRectMake(29, 0, 10, 17);
		else if(t_number == 4)
			return_value = CCRectMake(39, 0, 11, 17);
		else if(t_number == 5)
			return_value = CCRectMake(50, 0, 10, 17);
		else if(t_number == 6)
			return_value = CCRectMake(60, 0, 11, 17);
		else if(t_number == 7)
			return_value = CCRectMake(71, 0, 10, 17);
		else if(t_number == 8)
			return_value = CCRectMake(81, 0, 11, 17);
		else if(t_number == 9)
			return_value = CCRectMake(92, 0, 11, 17);
		return return_value;
	}
#undef LZZ_INLINE
