#ifndef __FROZEN_OLE_H__
#define __FROZEN_OLE_H__

#include "Box2DSprite.h"

class FrozenOle : public Box2DSprite
{
public:
	bool init();
	virtual bool mouseJointBegan();
};

#endif