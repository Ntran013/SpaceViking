#include "LongBlock.h"

using namespace cocos2d;

bool LongBlock::init()
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("long_block.png"));
		gameObjectType = kLongBlockType;

		pRet = true;
	}

	return pRet;
}

bool LongBlock::mouseJointBegan()
{
	return false;
}
