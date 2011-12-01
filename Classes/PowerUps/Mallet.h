#ifndef __MALLET_H__
#define __MALLET_H__

#include "GameObjects\GameObject.h"

class Mallet : public GameObject
{
	CC_SYNTHESIZE(cocos2d::CCAnimation *, malletAnim, MalletAnim);

	//Public methods
	~Mallet();
	void changeState(CharacterStates newState);
	void updateStateWithDeltaTime(cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects);
	void initAnimations();
	bool init();
};
#endif