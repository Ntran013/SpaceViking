#ifndef __VIKING_H__
#define __VIKING_H__

#include "GameObjects\GameCharacter.h"
#include "JoystickClasses\SneakyButton.h"
#include "JoystickClasses\SneakyJoystick.h"

using namespace cocos2d;

enum LastPunchType 
{
	kLeftHook,
	kRightHook
};

class Viking : public GameCharacter
{
	LastPunchType myLastPunch;
	bool isCarryingMallet;
	CCSpriteFrame *standingFrame;

	// Standing, breathing, and walking
	CC_SYNTHESIZE(CCAnimation *, breathingAnim, BreathingAnim);
	CC_SYNTHESIZE(CCAnimation *, breathingMalletAnim, BreathingMalletAnim);
	CC_SYNTHESIZE(CCAnimation *, walkingAnim, WalkingAnim);
	CC_SYNTHESIZE(CCAnimation *, walkingMalletAnim, WalkingMalletAnim);

	// Crouching, standing up, and Jumping
	CC_SYNTHESIZE(CCAnimation *, crouchingAnim, CrouchingAnim);
	CC_SYNTHESIZE(CCAnimation *, crouchingMalletAnim, CrouchingMalletAnim);
	CC_SYNTHESIZE(CCAnimation *, standingUpAnim, StandingUpAnim);
	CC_SYNTHESIZE(CCAnimation *, standingUpMalletAnim, StandingUpMalletAnim);
	CC_SYNTHESIZE(CCAnimation *, jumpingAnim, JumpingAnim);
	CC_SYNTHESIZE(CCAnimation *, jumpingMalletAnim, JumpingMalletAnim);
	CC_SYNTHESIZE(CCAnimation *, afterJumpingAnim, AfterJumpingAnim);
	CC_SYNTHESIZE(CCAnimation *, afterJumpingMalletAnim, AfterJumpingMalletAnim);

	// Punching
	CC_SYNTHESIZE(CCAnimation *, rightPunchAnim, RightPunchAnim);
	CC_SYNTHESIZE(CCAnimation *, leftPunchAnim, LeftPunchAnim);
	CC_SYNTHESIZE(CCAnimation *, malletPunchAnim, MalletPunchAnim);

	// Taking Damage and Death
	CC_SYNTHESIZE(CCAnimation *, phaserShockAnim, PhaserShockAnim);
	CC_SYNTHESIZE(CCAnimation *, deathAnim, DeathAnim);

	CC_SYNTHESIZE(SneakyJoystick *, joystick, Joystick);
	CC_SYNTHESIZE(SneakyButton *, jumpButton, JumpButton);
	CC_SYNTHESIZE(SneakyButton *, attackButton, AttackButton);

	float millisecondsStayingIdle;

	//Public methods
	~Viking();
	bool isCarryingWeapon();
	int getWeaponDamage();
	void applyJoystick(SneakyJoystick *aJoystick, float deltaTime);
	void checkAndClampSpritePosition();
	void changeState(CharacterStates newState);
	void updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects);
	CCRect adjustedBoundingBox();
	void initAnimations();
	bool init();
};
#endif 