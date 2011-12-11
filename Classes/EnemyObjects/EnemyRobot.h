#ifndef __ENEMY_ROBOT_H__
#define __ENEMY_ROBOT_H__

#include "GameObjects\GameCharacter.h"
#include "Singletons\GameManager.h"
using namespace cocos2d;

class EnemyRobot : public GameCharacter
{
protected:
	bool isVikingWithinBoundingBox;
	bool isVikingWithinSight;
	GameCharacter *vikingCharacter;
	GameplayLayerDelegate *mDelegate;
	int walkingSound;

	CC_SYNTHESIZE(CCAnimation *, robotWalkingAnim, RobotWalkingAnim);
	CC_SYNTHESIZE(CCAnimation *, raisePhaserAnim, RaisePhaserAnim);
	CC_SYNTHESIZE(CCAnimation *, shootPhaserAnim, ShootPhaserAnim);
	CC_SYNTHESIZE(CCAnimation *, lowerPhaserAnim, LowerPhaserAnim);
	CC_SYNTHESIZE(CCAnimation *, torsoHitAnim, TorsoHitAnim);
	CC_SYNTHESIZE(CCAnimation *, headHitAnim, HeadHitAnim);
	CC_SYNTHESIZE(CCAnimation *, robotDeathAnim, RobotDeathAnim);

#if ENEMY_STATE_DEBUG == 1
	CC_SYNTHESIZE(CCLabelBMFont *, myDebugLabel, MyDebugLabel);
#endif

	//Public methods
	~EnemyRobot();
	void setDelegate(GameplayLayerDelegate *pDelegate);
	void initAnimations();
	void shootPhaser();
	CCRect eyesightBoundingBox();
	void changeState(CharacterStates newState);
	void updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects);
	CCRect adjustedBoundingBox();
	void playPhaserFireSound();
	void playWalkingSound();
	bool init();
#if ENEMY_STATE_DEBUG == 1
	void setDebugLabelTextAndPosition();
#endif
};
#endif