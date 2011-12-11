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
	// Change the scale factor to SCREEN_HEIGHT/2.5 to make the viking move at the same pace across all different resolutions
	CCPoint scaledVelocity = ccpMult(aJoystick->getVelocity(), SCREEN_HEIGHT/2.5f);
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
	STOPSOUNDEFFECT(walkingSound);
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
		PLAYSOUNDEFFECT(VIKING_WALKING_1);
		if (isCarryingMallet)
			action = CCAnimate::actionWithAnimation(walkingMalletAnim, false);
		else
			action = CCAnimate::actionWithAnimation(walkingAnim, false);
		break;

	case kStateCrouching:
		this->playCrouchingSound();
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
		this->playBreathingSound();
		if (isCarryingMallet)
			action = CCAnimate::actionWithAnimation(breathingMalletAnim, false);
		else
			action = CCAnimate::actionWithAnimation(breathingAnim, false);
		break;

	case kStateJumping:
		this->playJumpingSound();
		newPosition = ccp(screenSize.width * 0.2f, 0.0f);
		if (this->isFlipX() == true)
			newPosition = ccp(newPosition.x * -1.0f, 0.0f);
		// Change the jump height to be consistent across different resolutions
		movementAction = CCJumpBy::actionWithDuration(0.5f, newPosition, SCREEN_HEIGHT/2.0f, 1);
		if (isCarryingMallet)
			action = CCSequence::actions(CCAnimate::actionWithAnimation(crouchingMalletAnim, false), CCSpawn::actions(CCAnimate::actionWithAnimation(jumpingMalletAnim, true), movementAction, NULL), CCAnimate::actionWithAnimation(afterJumpingMalletAnim, false), NULL); 
		else
			action = CCSequence::actions(CCAnimate::actionWithAnimation(crouchingAnim, false), CCSpawn::actions(CCAnimate::actionWithAnimation(jumpingAnim, true), movementAction, NULL), CCAnimate::actionWithAnimation(afterJumpingAnim, false), NULL); 
		break;

	case kStateAttacking:
		if (isCarryingMallet)
		{
			action = CCAnimate::actionWithAnimation(malletPunchAnim, true);
			this->playSwingingSound();
		}
		else
		{
			PLAYSOUNDEFFECT(VIKING_PUNCHING);
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
		this->playTakingDamageSound();
		this->setCharacterHealth(this->getCharacterHealth() - 10.0f);
		action = CCAnimate::actionWithAnimation(phaserShockAnim, true);
		break;

	case kStateDead:
		this->playDyingSound();
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
	// This if statement is not in the book, but added to make the viking die when health reaches 0 and not just freeze in the takingDamage animation
	if ((characterState != kStateDead) && (characterHealth <= 0)) 
	{
		this->changeState(kStateDead);
		return;
	}

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

void Viking::playJumpingSound() 
{
	int soundToPlay = rand() % 4;
	if (soundToPlay == 0) 
		PLAYSOUNDEFFECT(VIKING_JUMPING_1);
	else if (soundToPlay == 1) 
		PLAYSOUNDEFFECT(VIKING_JUMPING_2);
	else if (soundToPlay == 2) 
		PLAYSOUNDEFFECT(VIKING_JUMPING_3);
	else 
		PLAYSOUNDEFFECT(VIKING_JUMPING_4);
}

void Viking::playSwingingSound() 
{
	int soundToPlay = rand() % 8;
	switch (soundToPlay) 
	{
	case 0:
		PLAYSOUNDEFFECT(VIKING_SWINGING_1);
		break;
	case 1:
		PLAYSOUNDEFFECT(VIKING_SWINGING_2);
		break;
	case 2:
		PLAYSOUNDEFFECT(VIKING_SWINGING_3);
		break;
	case 3:
		PLAYSOUNDEFFECT(VIKING_SWINGING_4);
		break;
	case 4:
		PLAYSOUNDEFFECT(VIKING_SWINGING_5);
		break;
	case 5:
		PLAYSOUNDEFFECT(VIKING_SWINGING_6);
		break;
	case 6:
		PLAYSOUNDEFFECT(VIKING_SWINGING_7);
		break;
	case 7:
		PLAYSOUNDEFFECT(VIKING_SWINGING_8);
		break;
	default:
		PLAYSOUNDEFFECT(VIKING_SWINGING_9);
		break;
	}
}

void Viking::playBreathingSound() 
{
	int soundToPlay = rand() % 4;
	if (soundToPlay == 0) 
		PLAYSOUNDEFFECT(VIKING_GRUMBLING_1);
	else if (soundToPlay == 1) 
		PLAYSOUNDEFFECT(VIKING_GRUMBLING_2);
	else if (soundToPlay == 2) 
		PLAYSOUNDEFFECT(VIKING_CURSING_1);
	else 
		PLAYSOUNDEFFECT(VIKING_CURSING_2);
}

void Viking::playTakingDamageSound() 
{
	int soundToPlay = rand() % 5;
	if (soundToPlay == 0) 
		PLAYSOUNDEFFECT(VIKING_HIT_1);
	else if (soundToPlay == 1) 
		PLAYSOUNDEFFECT(VIKING_HIT_2);
	else if (soundToPlay == 2) 
		PLAYSOUNDEFFECT(VIKING_HIT_3);
	else if (soundToPlay == 3) 
		PLAYSOUNDEFFECT(VIKING_HIT_4);
	else 
		PLAYSOUNDEFFECT(VIKING_HIT_5);
}

void Viking::playDyingSound() 
{
	int soundToPlay = rand() % 5;
	if (soundToPlay == 0) 
		PLAYSOUNDEFFECT(VIKING_DYING_1);
	else if (soundToPlay == 1) 
		PLAYSOUNDEFFECT(VIKING_DYING_2);
	else if (soundToPlay == 2) 
		PLAYSOUNDEFFECT(VIKING_DYING_3);
	else if (soundToPlay == 3) 
		PLAYSOUNDEFFECT(VIKING_DYING_4);
	else 
		PLAYSOUNDEFFECT(VIKING_DYING_5);
}

void Viking::playCrouchingSound() 
{
	int soundToPlay = rand() % 4;
	if (soundToPlay == 0) 
		PLAYSOUNDEFFECT(VIKING_CROUCHING_1);
	else if (soundToPlay == 1) 
		PLAYSOUNDEFFECT(VIKING_CROUCHING_2);
	else if (soundToPlay == 2) 
		PLAYSOUNDEFFECT(VIKING_CROUCHING_3);
	else 
		PLAYSOUNDEFFECT(VIKING_CROUCHING_4);
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
		pRet = true;
	}
	return pRet;
}