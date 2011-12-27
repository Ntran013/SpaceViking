#include "Skull.h"

using namespace cocos2d;

bool Skull::init()
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("skull.png"));
		gameObjectType = kSkullType;

		pRet = true;
	}

	return pRet;
}

void Skull::shrinkDone( cocos2d::CCNode *pSender )
{
	this->removeFromParentAndCleanup(true);
}

void Skull::changeState( CharacterStates newState )
{
	this->setCharacterState(newState);

	switch (newState)
	{
	case kStateDead:
		{
			CCLOG("Skull->Changing State To Dead");

			PLAYSOUNDEFFECT(PUZZLE_SKULL);
			body->GetWorld()->DestroyBody(body);
			body = NULL;
			CCScaleTo *growAction = CCScaleBy::actionWithDuration(0.1, 1.5);
			CCScaleTo *shrinkAction = CCScaleBy::actionWithDuration(0.1, 0.1);
			CCCallFuncN *doneAction = CCCallFuncN::actionWithTarget(this, callfuncN_selector(Skull::shrinkDone));
			CCFiniteTimeAction *sequence = CCSequence::actions(growAction, shrinkAction, doneAction, NULL);
			this->runAction(sequence);
			break;
		}
	default:
		{
			CCLOG("Unhandled state %d in Skull", newState);
			break;
		}
	}
}

bool Skull::mouseJointBegan()
{
	if (this->getCharacterState() == kStateDead)
		return false;
	this->changeState(kStateDead);
	return false;
}