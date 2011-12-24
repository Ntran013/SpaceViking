#include "Rock.h"

using namespace cocos2d;

bool Rock::init()
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("rock.png"));
		gameObjectType = kRockType;

		pRet = true;
	}

	return pRet;
}

bool Rock::mouseJointBegan()
{
	PLAYSOUNDEFFECT(PUZZLE_ROCK1);
	return true;
}
