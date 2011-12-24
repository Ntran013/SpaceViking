#include "Meteor.h"

using namespace cocos2d;

bool Meteor::init()
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("meteor.png"));
		gameObjectType = kMeteorType;

		pRet = true;
	}

	return pRet;
}

bool Meteor::mouseJointBegan()
{
	PLAYSOUNDEFFECT(PUZZLE_METEOR);
	return true;
}
