#include "FrozenOle.h"

using namespace cocos2d;


bool FrozenOle::init()
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("frozen_ole.png"));
		gameObjectType = kFrozenVikingType;

		pRet = true;
	}

	return pRet;
}

bool FrozenOle::mouseJointBegan()
{
	return false;
}
