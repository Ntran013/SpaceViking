#ifndef __LONG_BLOCK_H__
#define __LONG_BLOCK_H__

#include "Box2DSprite.h"

class LongBlock : public Box2DSprite
{
public:
	bool init();
	virtual bool mouseJointBegan();
};

#endif