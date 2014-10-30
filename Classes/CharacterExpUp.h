//
//  CharacterExpUp.h
//  DGproto
//
//  Created by 사원3 on 2014. 10. 30..
//
//

#ifndef __DGproto__CharacterExpUp__
#define __DGproto__CharacterExpUp__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CharacterHistory;
class KSLabelTTF;
class ConvexGraph;
class CharacterExpUp : public CCNode
{
public:
	static CharacterExpUp* create(CharacterHistory t_before_history, CharacterHistory t_after_history, CCPoint t_base_position = ccp(240,320));
	
private:
	
	CCPoint base_position;
	KSLabelTTF* char_level;
	int level;
	int after_level;
	ConvexGraph* progress_timer;
	KSLabelTTF* exp_label;
	int exp_value;
	int after_exp_value;
	int after_max_exp_value;
	float before_percentage;
	float after_percentage;
	float dis_time;
	float exp_per_time;
	float before_exp;
	function<void()> end_func;
	void startUpAnimation();
	void myInit(CharacterHistory t_before_history, CharacterHistory t_after_history, CCPoint t_base_position);
};

#endif /* defined(__DGproto__CharacterExpUp__) */
