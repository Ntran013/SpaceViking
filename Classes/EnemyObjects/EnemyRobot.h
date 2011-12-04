#ifndef __ENEMY_ROBOT_H__
#define __ENEMY_ROBOT_H__

#include "GameObjects\GameCharacter.h"

using namespace cocos2d;

class EnemyRobot : public GameCharacter, public GameplayLayerDelegate
{
protected:
	bool isVikingWithinBoundingBox;
	bool isVikingWithinSight;
	GameCharacter *vikingCharacter;
	GameplayLayerDelegate *mDelegate;

	CC_SYNTHESIZE(CCAnimation *, robotWalkingAnim, RobotWalkingAnim);
	CC_SYNTHESIZE(CCAnimation *, raisePhaserAnim, RaisePhaserAnim);
	CC_SYNTHESIZE(CCAnimation *, shootPhaserAnim, ShootPhaserAnim);
	CC_SYNTHESIZE(CCAnimation *, lowerPhaserAnim, LowerPhaserAnim);
	CC_SYNTHESIZE(CCAnimation *, torsoHitAnim, TorsoHitAnim);
	CC_SYNTHESIZE(CCAnimation *, headHitAnim, HeadHitAnim);
	CC_SYNTHESIZE(CCAnimation *, robotDeathAnim, RobotDeathAnim);


	//Public methods
	~EnemyRobot();
	void setDelegate(GameplayLayerDelegate *pDelegate);
	void initAnimations();
	void shootPhaser();
	virtual void createObjectOfType(GameObjectType objectType, int initialHealth, cocos2d::CCPoint spawnLocation, int zValue);
	virtual void createPhaserWithDirection(PhaserDirection phaserDirection, cocos2d::CCPoint spawnPosition);
	CCRect eyesightBoundingBox();
	void changeState(CharacterStates newState);
	void updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects);
	CCRect adjustedBoundingBox();
	bool init();
};
#endif