#ifndef __SKULL_H__
#define __SKULL_H__

#include "Box2DSprite.h"

class Skull : public Box2DSprite
{
public:
	bool init();
	void shrinkDone(cocos2d::CCNode *pSender);
	void changeState(CharacterStates newState);
	bool mouseJointBegan();
};

#endif