#ifndef __GAME_CHARACTER_H__
#define __GAME_CHARACTER_H__

#include "GameObject.h"

class GameCharacter : public GameObject
{
	CC_SYNTHESIZE(int, characterHealth, CharacterHealth);
	CC_SYNTHESIZE(CharacterStates, characterState, CharacterState);

	//public methods
	~GameCharacter();
	void checkAndClampSpritePosition();
	virtual int getWeaponDamage();
};

#endif