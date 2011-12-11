#include "RadarDish.h"

using namespace cocos2d;

RadarDish::~RadarDish()
{
	if (tiltingAnim)
	{
		tiltingAnim->release();
		tiltingAnim = NULL;
	}
	if (transmittingAnim)
	{
		transmittingAnim->release();
		transmittingAnim = NULL;
	}
	if (takingAHitAnim)
	{
		takingAHitAnim->release();
		takingAHitAnim = NULL;
	}
	if (blowingUpAnim)
	{
		blowingUpAnim->release();
		blowingUpAnim = NULL;
	}
}

void RadarDish::changeState(CharacterStates newState)
{
	this->stopAllActions();
	CCAnimate *action = NULL;
	this->setCharacterState(newState);

	switch (newState)
	{
	case kStateSpawning:
		//CCLOG("RadarDish->Starting the Spawning Animation");
		action = CCAnimate::actionWithAnimation(tiltingAnim, false);
		break;

	case kStateIdle:
		//CCLOG("RadarDish->Changing State to Idle");
		action = CCAnimate::actionWithAnimation(transmittingAnim, false);
		break;

	case kStateTakingDamage:
		//CCLOG("RadarDish->Changing State to TakingDamage");
		characterHealth = characterHealth - vikingCharacter->getWeaponDamage();
		if (characterHealth <= 0.0f)
			this->changeState(kStateDead);
		else 
		{
			PLAYSOUNDEFFECT(VIKING_HAMMERHIT1);
			action = CCAnimate::actionWithAnimation(takingAHitAnim, false);
		}
		break;

	case kStateDead:
		//CCLOG("RadarDish->Changing State to Dead");
		PLAYSOUNDEFFECT(VIKING_HAMMERHIT2);
		PLAYSOUNDEFFECT(ENEMYROBOT_DYING);
		action = CCAnimate::actionWithAnimation(blowingUpAnim, false);
		break;

	default:
		//CCLOG("Unhandled state %d in RadarDish", newState);
		break;
	}

	if(action != NULL)
		this->runAction(action);
}

void RadarDish::updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects)
{
	if (this->getCharacterState() == kStateDead)
		return;

	vikingCharacter = (GameCharacter *) this->getParent()->getChildByTag(kVikingSpriteTagValue);
	CCRect vikingBoundingBox = vikingCharacter->adjustedBoundingBox();
	CharacterStates vikingState = vikingCharacter->getCharacterState();

	if (vikingState == kStateAttacking && CCRect::CCRectIntersectsRect(this->adjustedBoundingBox(), vikingBoundingBox))
	{
		if (this->getCharacterState() != kStateTakingDamage)
		{
			this->changeState(kStateTakingDamage);
			return;
		}
	}

	if (this->numberOfRunningActions() == 0 && this->getCharacterState() != kStateDead)
	{
		//CCLOG("Going to Idle");
		this->changeState(kStateIdle);
		return;
	}
}

void RadarDish::initAnimations()
{
	this->setTiltingAnim(this->loadPlistForAnimationWithName("tiltingAnim", "Plists/RadarDish.plist"));
	tiltingAnim->retain();
	this->setTransmittingAnim(this->loadPlistForAnimationWithName("transmittingAnim", "Plists/RadarDish.plist"));
	transmittingAnim->retain();
	this->setTakingAHitAnim(this->loadPlistForAnimationWithName("takingAHitAnim", "Plists/RadarDish.plist"));
	takingAHitAnim->retain();
	this->setBlowingUpAnim(this->loadPlistForAnimationWithName("blowingUpAnim", "Plists/RadarDish.plist"));
	blowingUpAnim->retain();
}

bool RadarDish::init()
{
	bool pRet = false; 
	if (GameObject::init())
	{
		//CCLOG("### RadarDish initialized");
		this->initAnimations();
		characterHealth = 100.0f;
		this->setGameObjectType(kEnemyTypeRadarDish);
		this->changeState(kStateSpawning);
		pRet = true;
	}
	return pRet;
}