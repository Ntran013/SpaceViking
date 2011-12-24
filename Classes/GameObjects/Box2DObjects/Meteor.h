#ifndef __METEOR_H__
#define __METEOR_H__

#include "Box2DSprite.h"

class Meteor : public Box2DSprite
{
public:
	bool init();
	bool mouseJointBegan();
};

#endif