#include "IceBlock.h"

using namespace cocos2d;

bool IceBlock::init()
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ice_block.png"));
		gameObjectType = kIceType;

		pRet = true;
	}

	return pRet;
}