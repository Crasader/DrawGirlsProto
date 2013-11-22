//
//  IntSeries.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 22..
//
//

#ifndef __DGproto__IntSeries__
#define __DGproto__IntSeries__

#include "cocos2d.h"
#include "EnumDefine.h"

USING_NS_CC;

#define pixelSize 2

class IntVector : public CCObject
{
public:
	int dx;
	int dy;
	
	IntVector()
	{
		dx = dy = 0;
	}
	
	IntVector(int t_dx, int t_dy)
	{
		dx = t_dx;
		dy = t_dy;
	}
	
	static IntDirection getLeftDirection(IntDirection direction)
	{
		IntDirection return_value;
		if(direction == directionLeft)			return_value = directionDown;
		else if(direction == directionUp)		return_value = directionLeft;
		else if(direction == directionRight)	return_value = directionUp;
		else if(direction == directionDown)		return_value = directionRight;
		else									return_value = directionStop;
		return return_value;
	}
	
	static IntDirection getRightDirection(IntDirection direction)
	{
		IntDirection return_value;
		if(direction == directionLeft)			return_value = directionUp;
		else if(direction == directionUp)		return_value = directionRight;
		else if(direction == directionRight)	return_value = directionDown;
		else if(direction == directionDown)		return_value = directionLeft;
		else									return_value = directionStop;
		return return_value;
	}
	
	static IntDirection getReverseDirection(IntDirection direction)
	{
		IntDirection return_value;
		if(direction == directionLeft)			return_value = directionRight;
		else if(direction == directionUp)		return_value = directionDown;
		else if(direction == directionRight)	return_value = directionLeft;
		else if(direction == directionDown)		return_value = directionUp;
		else									return_value = directionStop;
		return return_value;
	}
	
	static IntVector reverseDirectionVector(IntDirection direction)
	{
		IntVector r_v;
		if(direction == directionLeftUp)			r_v = IntVector(1,	-1);
		else if(direction == directionLeft)			r_v = IntVector(1,	0);
		else if(direction == directionLeftDown)		r_v = IntVector(1,	1);
		else if(direction == directionDown)			r_v = IntVector(0,	1);
		else if(direction == directionRightDown)	r_v = IntVector(-1,	1);
		else if(direction == directionRight)		r_v = IntVector(-1,	0);
		else if(direction == directionRightUp)		r_v = IntVector(-1,	-1);
		else if(direction == directionUp)			r_v = IntVector(0,	-1);
		else										r_v = IntVector(0, 0);
		return r_v;
	}
	
	static IntVector directionVector(IntDirection direction)
	{
		IntVector r_v;
		if(direction == directionLeftUp)			r_v = IntVector(-1,	1);
		else if(direction == directionLeft)			r_v = IntVector(-1,	0);
		else if(direction == directionLeftDown)		r_v = IntVector(-1,	-1);
		else if(direction == directionDown)			r_v = IntVector(0,	-1);
		else if(direction == directionRightDown)	r_v = IntVector(1,	-1);
		else if(direction == directionRight)		r_v = IntVector(1,	0);
		else if(direction == directionRightUp)		r_v = IntVector(1,	1);
		else if(direction == directionUp)			r_v = IntVector(0,	1);
		else										r_v = IntVector(0, 0);
		return r_v;
	}
	
	float getAngle()
	{
		float r_angle;
		if(dx == 0 && dy == 0)				r_angle = 0.f;
		else if(dx < 0 && dy > 0)			r_angle = 135.f;
		else if(dx < 0 && dy == 0)			r_angle = 180.f;
		else if(dx < 0 && dy < 0)			r_angle = -135.f;
		else if(dx == 0 && dy < 0)			r_angle = 90.f;
		else if(dx > 0 && dy < 0)			r_angle = -45.f;
		else if(dx > 0 && dy == 0)			r_angle = 0.f;
		else if(dx > 0 && dy > 0)			r_angle = 45.f;
		else if(dx == 0 && dy > 0)			r_angle = -90.f;
		
		return r_angle;
	}
	
	void multiply(int t_m)
	{
		dx *= t_m;
		dy *= t_m;
	}
};

#define IntPointValueIsNULL		-1

class IntPoint : public CCObject
{
public:
	int x;
	int y;
	
	IntPoint()
	{
		x = y = IntPointValueIsNULL;
	}
	IntPoint(const IntPoint& ip)
	{
		x = ip.x;
		y = ip.y;
	}
	IntPoint(int t_x, int t_y)
	{
		x = t_x;
		y = t_y;
	}
	int length() const
	{
		return sqrt(x*x + y*y);
	}
	IntPoint operator+(const IntPoint& right) const
	{
		return IntPoint(x + right.x, y + right.y);
	}
	IntPoint operator-(const IntPoint& right) const
	{
		return IntPoint(x - right.x, y - right.y);
	}
	IntPoint operator*(float a) const
	{
		return IntPoint(x * a, y * a);
	}
	IntPoint operator/(float a) const
	{
		return IntPoint(x / a, y / a);
	}
	IntPoint rotation(float rad) const
	{
		return IntPoint(round(cos(rad) * x - sin(rad) * y), round(cos(rad) * y + sin(rad) * x));
	}
	bool operator==(const IntPoint& right) const
	{
		return x == right.x && y == right.y;
	}
	bool operator<(const IntPoint& ip) const
	{
		if(x == ip.x)
			return y < ip.y;
		else
			return x < ip.x;
	}
	static IntPoint convertToIntPoint(CCPoint t_p)
	{
		return IntPoint(roundf((t_p.x-1)/pixelSize+1), roundf((t_p.y-1)/pixelSize+1));
	}
	
	CCPoint convertToCCP()
	{
		return ccp((x-1.f)*pixelSize+1.f, (y-1.f)*pixelSize+1.f);
	}
	
	bool isInnerMap()
	{
		if(x >= mapWidthInnerBegin && x < mapWidthInnerEnd && y >= mapHeightInnerBegin && y < mapHeightInnerEnd)
			return true;
		else
			return false;
	}
	
	bool isNull()
	{
		if(x == IntPointValueIsNULL && y == IntPointValueIsNULL)
			return true;
		else
			return false;
	}
};

CCPoint ip2ccp(const IntPoint& ip);
IntPoint ccp2ip(const CCPoint& cc);

class IntPointVector : public CCObject
{
public:
	IntPoint origin;
	IntVector distance;
	
	IntPointVector()
	{
		origin = IntPoint();
		distance = IntVector();
	}
	
	IntPointVector(int t_x, int t_y, int t_dx, int t_dy)
	{
		origin = IntPoint(t_x, t_y);
		distance = IntVector(t_dx, t_dy);
	}
};

class IntMoveState : public CCObject
{
public:
	IntPoint origin;
	IntDirection direction;
	
	IntMoveState()
	{
		origin = IntPoint();
		direction = directionStop;
	}
	
	IntMoveState(int t_x, int t_y, IntDirection t_d)
	{
		origin = IntPoint(t_x, t_y);
		direction = t_d;
	}
};

class IntSize : public CCObject
{
public:
	int width;
	int height;
	
	IntSize()
	{
		width = height = 0;
	}
	
	IntSize(int t_w, int t_h)
	{
		width = t_w;
		height = t_h;
	}
};

class IntRect : public CCObject
{
public:
	IntPoint origin;
	IntSize size;
	
	IntRect()
	{
		origin = IntPoint();
		size = IntSize();
	}
	
	IntRect(int t_x, int t_y, int t_w, int t_h)
	{
		origin = IntPoint(t_x, t_y);
		size = IntSize(t_w, t_h);
	}
};

#endif /* defined(__DGproto__IntSeries__) */
