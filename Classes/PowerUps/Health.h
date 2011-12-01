#ifndef __HEALTH_H__
#define __HEALTH_H__

#include "GameObjects\GameObject.h"

class Health : public GameObject
{
	CC_SYNTHESIZE(cocos2d::CCAnimation *, healthAnim, HealthAnim);

	//Public methods
	~Health();
	void changeState(CharacterStates newState);
	void updateStateWithDeltaTime(cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects);
	void initAnimations();
	bool init();
};
#endif