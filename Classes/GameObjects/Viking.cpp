#include "Viking.h"

Viking::~Viking()
{
	joystick = NULL;
	jumpButton = NULL;
	attackButton = NULL;
	//
	if (breathingAnim)
	{
		breathingAnim->release();
		breathingAnim = NULL;
	}
	if (breathingMalletAnim)
	{
		breathingMalletAnim->release();
		breathingMalletAnim = NULL;
	}
	if (walkingAnim)
	{
		walkingAnim->release();
		walkingAnim = NULL;
	}
	if (walkingMalletAnim)
	{
		walkingMalletAnim->release();
		walkingMalletAnim = NULL;
	}
	//
	if (crouchingAnim)
	{
		crouchingAnim->release();
		crouchingAnim = NULL;
	}
	if (crouchingMalletAnim)
	{
		crouchingMalletAnim->release();
		crouchingMalletAnim = NULL;
	}
	if (standingUpAnim)
	{
		standingUpAnim->release();
		standingUpAnim = NULL;
	}
	if (standingUpMalletAnim)
	{
		standingUpMalletAnim->release();
		standingUpMalletAnim = NULL;
	}
	if (jumpingAnim)
	{
		jumpingAnim->release();
		jumpingAnim = NULL;
	}
	if (jumpingMalletAnim)
	{
		jumpingMalletAnim->release();
		jumpingMalletAnim = NULL;
	}
	if (afterJumpingAnim)
	{
		afterJumpingAnim->release();
		afterJumpingAnim = NULL;
	}
	if (afterJumpingMalletAnim)
	{
		afterJumpingMalletAnim->release();
		afterJumpingMalletAnim = NULL;
	}
	//
	if (rightPunchAnim)
	{
		rightPunchAnim->release();
		rightPunchAnim = NULL;
	}
	if (leftPunchAnim)
	{
		leftPunchAnim->release();
		leftPunchAnim = NULL;
	}
	if (malletPunchAnim)
	{
		malletPunchAnim->release();
		malletPunchAnim = NULL;
	}
	//
	if (phaserShockAnim)
	{
		phaserShockAnim->release();
		phaserShockAnim = NULL;
	}
	if (deathAnim)
	{
		deathAnim->release();
		deathAnim = NULL;
	}
}

bool Viking::isCarryingWeapon()
{
	return isCarryingMallet;
}

int Viking::getWeaponDamage()
{
	if (isCarryingMallet)
		return kVikingMalletDamage;
	return kVikingFistDamage;
}

void  Viking::applyJoystick(SneakyJoystick *aJoystick, float deltaTime)
{
	CCPoint scaledVelocity = ccpMult(aJoystick->getVelocity(), 128.0f);
	CCPoint oldPosition = this->getPosition();
	CCPoint newPosition = ccp(oldPosition.x + scaledVelocity.x * deltaTime, oldPosition.y);
	this->setPosition(newPosition);

	if (oldPosition.x > newPosition.x)
		this->setFlipX(true);
	else
		this->setFlipX(false);
}

void Viking::checkAndClampSpritePosition()
{
	if (this->getCharacterState() != kStateJumping)
	{
		if (this->getPosition().y > this->getScreenSize().height * 0.14f) //Original value is 110.0f, but this causes a sky walking bug when catching the mallet as it falls from the air
			this->setPosition(ccp(this->getPosition().x, this->getScreenSize().height * 0.14f));
	}
	//super
	GameCharacter::checkAndClampSpritePosition();
}

void Viking::changeState(CharacterStates newState)
{
	this->stopAllActions();
	CCFiniteTimeAction *action = NULL;
	CCFiniteTimeAction *movementAction = NULL;
	CCPoint newPosition;
	this->setCharacterState(newState);

	switch (newState)
	{
	case kStateIdle:
		if (isCarryingMallet)
			this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sv_mallet_1.png"));
		else
			this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sv_anim_1.png"));
		break;

	case kStateWalking:
		if (isCarryingMallet)
			action = CCAnimate::actionWithAnimation(walkingMalletAnim, false);
		else
			action = CCAnimate::actionWithAnimation(walkingAnim, false);
		break;

	case kStateCrouching:
		if (isCarryingMallet)
			action = CCAnimate::actionWithAnimation(crouchingMalletAnim, false);
		else
			action = CCAnimate::actionWithAnimation(crouchingAnim, false);
		break;

	case kStateStandingUp:
		if (isCarryingMallet)
			action = CCAnimate::actionWithAnimation(standingUpMalletAnim, false);
		else
			action = CCAnimate::actionWithAnimation(standingUpAnim, false);
		break;

	case kStateBreathing:
		if (isCarryingMallet)
			action = CCAnimate::actionWithAnimation(breathingMalletAnim, false);
		else
			action = CCAnimate::actionWithAnimation(breathingAnim, false);
		break;

	case kStateJumping:
		newPosition = ccp(screenSize.width * 0.2f, 0.0f);
		if (this->isFlipX() == true)
			newPosition = ccp(newPosition.x * -1.0f, 0.0f);
		movementAction = CCJumpBy::actionWithDuration(0.5f, newPosition, 160.0f, 1);
		if (isCarryingMallet)
			action = CCSequence::actions(CCAnimate::actionWithAnimation(crouchingMalletAnim, false), CCSpawn::actions(CCAnimate::actionWithAnimation(jumpingMalletAnim, true), movementAction, NULL), CCAnimate::actionWithAnimation(afterJumpingMalletAnim, false), NULL); 
		else
			action = CCSequence::actions(CCAnimate::actionWithAnimation(crouchingAnim, false), CCSpawn::actions(CCAnimate::actionWithAnimation(jumpingAnim, true), movementAction, NULL), CCAnimate::actionWithAnimation(afterJumpingAnim, false), NULL); 
		break;

	case kStateAttacking:
		if (isCarryingMallet)
			action = CCAnimate::actionWithAnimation(malletPunchAnim, true);
		else
		{
			if (kLeftHook == myLastPunch)
			{
				myLastPunch = kRightHook;
				action = CCAnimate::actionWithAnimation(rightPunchAnim, false);
			}
			else
			{
				myLastPunch = kLeftHook;
				action = CCAnimate::actionWithAnimation(leftPunchAnim, false);				
			}
		}
		break;

	case kStateTakingDamage:
		this->setCharacterHealth(this->getCharacterHealth() - 10.0f);
		action = CCAnimate::actionWithAnimation(phaserShockAnim, true);
		break;

	case kStateDead:
		action = CCAnimate::actionWithAnimation(deathAnim, false);
		break;

	default:
		break;
	}

	if (action != NULL)
		this->runAction(action);
}

void Viking::updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects)
{
	if (this->getCharacterState() == kStateDead)
		return;

	if (this->getCharacterState() == kStateTakingDamage && this->numberOfRunningActions() > 0)
		return;

	CCRect myBoundingBox = this->adjustedBoundingBox();
	CCObject *object = NULL;
	
	CCARRAY_FOREACH(listOfGameObjects, object)
	{
		GameCharacter *character = (GameCharacter *) object;
		if (character->getTag() == kVikingSpriteTagValue)
			continue;
		
		CCRect characterBox = character->adjustedBoundingBox();
		if (CCRect::CCRectIntersectsRect(myBoundingBox, characterBox))
		{
			if (character->getGameObjectType() == kEnemyTypePhaser)
			{	
				this->changeState(kStateTakingDamage);
				character->changeState(kStateDead);
			}

			else if (character->getGameObjectType() == kPowerUpTypeMallet)
			{
				isCarryingMallet = true;
				this->changeState(kStateIdle);
				character->changeState(kStateDead);
			}

			else if (character->getGameObjectType() == kPowerUpTypeHealth)
			{
				this->setCharacterHealth(100.0f);
				character->changeState(kStateDead);
			}
		}
	}

	this->checkAndClampSpritePosition();
	if (this->getCharacterState() == kStateIdle || this->getCharacterState() == kStateWalking ||
		this->getCharacterState() == kStateCrouching || this->getCharacterState() == kStateStandingUp ||
		this->getCharacterState() == kStateBreathing)
	{
		if (jumpButton->getIsActive())
			this->changeState(kStateJumping);
		else if (attackButton->getIsActive())
			this->changeState(kStateAttacking);
		else if (joystick->getVelocity().x == 0.0f && joystick->getVelocity().y == 0.0f)
		{
			if (this->getCharacterState() == kStateCrouching)
				this->changeState(kStateStandingUp);
		}
		else if (joystick->getVelocity().y < -0.45f)
		{
			if (this->getCharacterState() != kStateCrouching)
				this->changeState(kStateCrouching);
		}
		else if (joystick->getVelocity().x != 0.0f)
		{
			if (this->getCharacterState() != kStateWalking)
				this->changeState(kStateWalking);
			this->applyJoystick(joystick, deltaTime);
		}
	}

	if (this->numberOfRunningActions() == 0)
	{
		if (this->getCharacterHealth() <= 0.0f)
			this->changeState(kStateDead);

		else if (this->getCharacterState() == kStateIdle)
		{
			millisecondsStayingIdle = millisecondsStayingIdle + deltaTime;
			if (millisecondsStayingIdle > kVikingIdleTimer)
				this->changeState(kStateBreathing);
		}

		else if (this->characterState != kStateCrouching && this->characterState != kStateIdle)
		{
			millisecondsStayingIdle = 0.0f;
			this->changeState(kStateIdle);
		}
	}
}

CCRect Viking::adjustedBoundingBox()
{
	CCRect vikingBoundingBox = this->boundingBox();
	float xOffset;
	float xCropAmount = vikingBoundingBox.size.width * 0.5482f;
	float yCropAmount = vikingBoundingBox.size.width * 0.095f;

	if (this->isFlipX() == false)
		xOffset = vikingBoundingBox.size.width * 0.1566f;
	else
		xOffset = vikingBoundingBox.size.width * 0.4217f;

	vikingBoundingBox = CCRectMake(vikingBoundingBox.origin.x + xOffset, vikingBoundingBox.origin.y, vikingBoundingBox.size.width - xCropAmount, vikingBoundingBox.size.height - yCropAmount);
	if (characterState == kStateCrouching)
		vikingBoundingBox = CCRectMake(vikingBoundingBox.origin.x, vikingBoundingBox.origin.y, vikingBoundingBox.size.width, vikingBoundingBox.size.height * 0.56f);
	return vikingBoundingBox;
}

void Viking::initAnimations()
{
	this->setBreathingAnim(this->loadPlistForAnimationWithName("breathingAnim", "Plists/Viking.plist"));
	breathingAnim->retain();
	this->setBreathingMalletAnim(this->loadPlistForAnimationWithName("breathingMalletAnim", "Plists/Viking.plist"));
	breathingMalletAnim->retain();
	this->setWalkingAnim(this->loadPlistForAnimationWithName("walkingAnim", "Plists/Viking.plist"));
	walkingAnim->retain();
	this->setWalkingMalletAnim(this->loadPlistForAnimationWithName("walkingMalletAnim", "Plists/Viking.plist"));
	walkingMalletAnim->retain();	
	this->setCrouchingAnim(this->loadPlistForAnimationWithName("crouchingAnim", "Plists/Viking.plist"));
	crouchingAnim->retain();
	this->setCrouchingMalletAnim(this->loadPlistForAnimationWithName("crouchingMalletAnim", "Plists/Viking.plist"));
	crouchingMalletAnim->retain();	
	this->setStandingUpAnim(this->loadPlistForAnimationWithName("standingUpAnim", "Plists/Viking.plist"));
	standingUpAnim->retain();
	this->setStandingUpMalletAnim(this->loadPlistForAnimationWithName("standingUpMalletAnim", "Plists/Viking.plist"));
	standingUpMalletAnim->retain();
	this->setJumpingAnim(this->loadPlistForAnimationWithName("jumpingAnim", "Plists/Viking.plist"));
	jumpingAnim->retain();
	this->setJumpingMalletAnim(this->loadPlistForAnimationWithName("jumpingMalletAnim", "Plists/Viking.plist"));
	jumpingMalletAnim->retain();
	this->setAfterJumpingAnim(this->loadPlistForAnimationWithName("afterJumpingAnim", "Plists/Viking.plist"));
	afterJumpingAnim->retain();	
	this->setAfterJumpingMalletAnim(this->loadPlistForAnimationWithName("afterJumpingMalletAnim", "Plists/Viking.plist"));
	afterJumpingMalletAnim->retain();	
	this->setRightPunchAnim(this->loadPlistForAnimationWithName("rightPunchAnim", "Plists/Viking.plist"));
	rightPunchAnim->retain();
	this->setLeftPunchAnim(this->loadPlistForAnimationWithName("leftPunchAnim", "Plists/Viking.plist"));
	leftPunchAnim->retain();	
	this->setMalletPunchAnim(this->loadPlistForAnimationWithName("malletPunchAnim", "Plists/Viking.plist"));
	malletPunchAnim->retain();
	this->setPhaserShockAnim(this->loadPlistForAnimationWithName("phaserShockAnim", "Plists/Viking.plist"));
	phaserShockAnim->retain();
	this->setDeathAnim(this->loadPlistForAnimationWithName("vikingDeathAnim", "Plists/Viking.plist"));
	deathAnim->retain();
}

bool Viking::init()
{
	bool pRet = false; 
	if (GameObject::init())
	{
		joystick = NULL;
		jumpButton = NULL;
		attackButton = NULL;
		this->setGameObjectType(kVikingType);
		myLastPunch = kRightHook;
		millisecondsStayingIdle = 0.0f;
		isCarryingMallet = false;
		this->initAnimations();
		//this->setScreenSize(CCDirector::sharedDirector()->getWinSize()); //set screen size, not in the book but needed
		pRet = true;
	}
	return pRet;
}