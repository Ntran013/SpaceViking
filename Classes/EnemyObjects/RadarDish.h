#ifndef __RADAR_DISH_H__
#define __RADAR_DISH_H__

#include "GameObjects\GameCharacter.h"

class RadarDish : public GameCharacter
{
	CC_SYNTHESIZE(cocos2d::CCAnimation *, tiltingAnim, TiltingAnim);
	CC_SYNTHESIZE(cocos2d::CCAnimation *, transmittingAnim, TransmittingAnim);
	CC_SYNTHESIZE(cocos2d::CCAnimation *, takingAHitAnim, TakingAHitAnim);
	CC_SYNTHESIZE(cocos2d::CCAnimation *, blowingUpAnim, BlowingUpAnim);
	GameCharacter *vikingCharacter;

	//PUblic methods
	~RadarDish();
	void changeState(CharacterStates newState);
	void updateStateWithDeltaTime(cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects);
	void initAnimations();
	bool init();

};
#endif