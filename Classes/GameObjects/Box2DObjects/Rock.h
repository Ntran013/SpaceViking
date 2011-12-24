#ifndef __ROCK_H__
#define __ROCK_H__

#include "Box2DSprite.h"

class Rock : public Box2DSprite
{
public:
	bool init();
	bool mouseJointBegan();
};

#endif