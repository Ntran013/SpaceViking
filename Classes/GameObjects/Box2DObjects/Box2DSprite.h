#ifndef __BOX2D_SPRITE_H__ 
#define __BOX2D_SPRITE_H__

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "GameObjects\GameCharacter.h"

class Box2DSprite : public GameCharacter
{
public:
	CC_SYNTHESIZE(b2Body *, body, Body);

	// Public Methods
	// Return TRUE to accept the mouse joint
	// Return FALSE to reject the mouse joint
	virtual bool mouseJointBegan();
};

#endif