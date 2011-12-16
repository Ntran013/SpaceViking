#include "PhaserBullet.h"

PhaserBullet::~PhaserBullet()
{
	if (firingAnim)
	{
		firingAnim->release();
		firingAnim = NULL;
	}
	if (travelingAnim)
	{
		travelingAnim->release();
		travelingAnim = NULL;
	}
}

void PhaserBullet::changeState(CharacterStates newState)
{
	this->stopAllActions();
	CCFiniteTimeAction *action = NULL;
	characterState = newState;

	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

	switch (newState)
	{
	case kStateSpawning:
		//CCLOG("Phaser->Changed state to Spawning");
		action = CCAnimate::actionWithAnimation(firingAnim, false);
		break;
	case kStateTraveling:
	{
		//CCLOG("Phaser->Changed state to Traveling");
		CCPoint endLocation;
		if (myDirection == kDirectionLeft) 
		{
			//CCLOG("Phaser direction LEFT");
			endLocation = ccp(-10.0f, this->getPosition().y);
		} 
		else 
		{
			//CCLOG("Phaser direction RIGHT");
			endLocation = ccp(levelSize.width+24.0f, this->getPosition().y);
		}
		this->runAction(CCMoveTo::actionWithDuration(2.0f, endLocation));
		action = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(travelingAnim, false));
		
		break;
	}
	case kStateDead:
		//CCLOG("Phaser->Changed state to dead");
		// Remove from parent
		this->setIsVisible(false);
		this->removeFromParentAndCleanup(true);
	default:
		break;
	}
	if (action != NULL)
	{
		this->runAction(action);
	}
}

bool PhaserBullet::isOutsideOfScreen()
{
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

	CCPoint currentSpritePosition = this->getPosition();
	if ((currentSpritePosition.x < 0.0f) || (currentSpritePosition.x > levelSize.width)) 
	{
		this->changeState(kStateDead);
		return true;
	}
	return false;
}

void PhaserBullet::updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects)
{
	if (this->isOutsideOfScreen())
		return;
	if (this->numberOfRunningActions() == 0) 
	{
		if (characterState == kStateSpawning) 
		{
			this->changeState(kStateTraveling);
			return;
		} 
		else 
		{
			this->changeState(kStateDead);
			return;
			// Should not do anything else from traveling
		}
	}
}

void PhaserBullet::initAnimations()
{
	this->setFiringAnim(this->loadPlistForAnimationWithName("firingAnim", "Plists/PhaserBullet.plist"));
	firingAnim->retain();
	this->setTravelingAnim(this->loadPlistForAnimationWithName("travelingAnim", "Plists/PhaserBullet.plist"));
	travelingAnim->retain();
}

bool PhaserBullet::init()
{
	bool pRet = false;
	if (GameObject::init())
	{
		//CCLOG("### PhaserBullet initialized");
		this->initAnimations();
		gameObjectType = kEnemyTypePhaser;
		changeState(kStateSpawning); //Added to see the spawning animation, otherwise it won't appear
		pRet = true;
	}
	return pRet;
}