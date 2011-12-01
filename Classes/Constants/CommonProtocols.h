#ifndef __COMMON_H__
#define __COMMON_H__

// CommonProtocols.h
// SpaceViking
#include "cocos2d.h"

enum PhaserDirection {
kDirectionLeft,
kDirectionRight
};

enum CharacterStates {
kStateSpawning,
kStateIdle,
kStateCrouching,
kStateStandingUp,
kStateWalking,
kStateAttacking,
kStateJumping,
kStateBreathing,
kStateTakingDamage,
kStateDead,
kStateTraveling,
kStateRotating,
kStateDrilling,
kStateAfterJumping
}; // 1

enum GameObjectType {
kObjectTypeNone,
kPowerUpTypeHealth,
kPowerUpTypeMallet,
kEnemyTypeRadarDish,
kEnemyTypeSpaceCargoShip,
kEnemyTypeAlienRobot,
kEnemyTypePhaser,
kVikingType,
kSkullType,
kRockType,
kMeteorType,
kFrozenVikingType,
kIceType,
kLongBlockType,
kCartType,
kSpikesType,
kDiggerType,
kGroundType
};

class GameplayLayerDelegate 
{
public:
	virtual void createObjectOfType(GameObjectType objectType, int initialHealth, cocos2d::CCPoint spawnLocation, int zValue) = 0;
	virtual void createPhaserWithDirection(PhaserDirection phaserDirection, cocos2d::CCPoint spawnPosition) = 0;
};

#endif