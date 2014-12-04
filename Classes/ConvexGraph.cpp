//
//  ConvexGraph.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 8. 5..
//
//

#include "ConvexGraph.h"

ConvexGraph* ConvexGraph::create(string t_front_filename, CCRect t_front_9_out, CCRect t_front_9_in, CCSize t_full_size, ConvexGraphType t_type)
{
	ConvexGraph* t_cg = new ConvexGraph();
	t_cg->myInit(t_front_filename, t_front_9_out, t_front_9_in, t_full_size, t_type);
	t_cg->autorelease();
	return t_cg;
}

void ConvexGraph::setBack(string t_back_filename)
{
	if(back_img)
	{
		back_img->removeFromParent();
		back_img = NULL;
	}
	
	back_img = CCScale9Sprite::create(t_back_filename.c_str(), front_9_out, front_9_in);
	back_img->setContentSize(full_size);
	addChild(back_img, 0);
}

void ConvexGraph::setCover(string t_cover_filename, string t_mask_filename)
{
	if(cover_clipping_node)
	{
		cover_clipping_node->removeFromParent();
		cover_clipping_node = NULL;
	}
	
	cover_mask = CCScale9Sprite::create(t_mask_filename.c_str(), front_9_out, front_9_in);
	cover_mask->setContentSize(front_img->getContentSize());
	
	cover_clipping_node = CCClippingNode::create(cover_mask);
	cover_clipping_node->setAlphaThreshold(0.1f);
	cover_clipping_node->setPosition(front_img->getPosition());
	addChild(cover_clipping_node, 2);
	
	cover_img = CCSprite::create(t_cover_filename.c_str());
	cover_clipping_node->addChild(cover_img);
}

void ConvexGraph::setPercentage(float t_percentage)
{
	m_percentage = t_percentage;
	if(m_percentage > 100.f)
		m_percentage = 100.f;
	
	if(m_type == ConvexGraphType::width)
	{
		float t_width = zero_size.width + (full_size.width - zero_size.width)*(m_percentage/100.f);
		
		front_img->setContentSize(CCSizeMake(t_width, full_size.height));
		front_img->setPosition(ccp(-full_size.width/2.f + front_img->getContentSize().width/2.f, 0));
		
		if(cover_clipping_node)
		{
			cover_clipping_node->setPosition(front_img->getPosition());
			cover_mask->setContentSize(front_img->getContentSize());
		}
	}
}

float ConvexGraph::getPercentage()
{
	return m_percentage;
}

void ConvexGraph::myInit(string t_front_filename, CCRect t_front_9_out, CCRect t_front_9_in, CCSize t_full_size, ConvexGraphType t_type)
{
	ks_animator_node = CCNode::create();
	addChild(ks_animator_node);
	
	cover_clipping_node = NULL;
	back_img = NULL;
	
	front_filename = t_front_filename;
	front_9_out = t_front_9_out;
	front_9_in = t_front_9_in;
	full_size = t_full_size;
	m_type = t_type;
	zero_size = t_front_9_out.size - t_front_9_in.size;
	
	m_percentage = 0.f;
	
	front_img = CCScale9Sprite::create(front_filename.c_str(), front_9_out, front_9_in);
	if(m_type == ConvexGraphType::width)
	{
		front_img->setContentSize(CCSizeMake(zero_size.width, full_size.height));
		front_img->setPosition(ccp(-full_size.width/2.f + front_img->getContentSize().width/2.f, 0));
	}
	addChild(front_img, 1);
}