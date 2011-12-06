#include "GameCharacter.h"

using namespace cocos2d;

GameCharacter::~GameCharacter()
{
}

int GameCharacter::getWeaponDamage()
{
	//CCLOG("getWeaponDamage should be overridden");
	return 0;
}

void GameCharacter::checkAndClampSpritePosition()
{
	CCPoint	currentSpritePosition = this->getPosition();
	if (currentSpritePosition.x < 24.0f)
		this->setPosition(ccp(24.0f, currentSpritePosition.y));
	else if (currentSpritePosition.x > (SCREEN_WIDTH - 24.0f))
		this->setPosition(ccp((SCREEN_WIDTH - 24.0f), currentSpritePosition.y));
}
