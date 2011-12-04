#ifndef __PHASER_BULLET_H__
#define __PHASER_BULLET_H__

#include "GameObjects\GameCharacter.h"

using namespace cocos2d;

class PhaserBullet : public GameCharacter 
{
	CC_SYNTHESIZE(CCAnimation *, firingAnim, FiringAnim);
	CC_SYNTHESIZE(CCAnimation *, travelingAnim, TravelingAnim);
	CC_SYNTHESIZE(PhaserDirection, myDirection, MyDirection);

	//Public methods
	~PhaserBullet();
	void changeState(CharacterStates newState);
	bool isOutsideOfScreen();
	void updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects);
	void initAnimations();
	bool init();
};
#endif