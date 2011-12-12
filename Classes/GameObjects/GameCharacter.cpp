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
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();
	float xOffset;
	xOffset = 24.0f;

	if (currentSpritePosition.x < xOffset)
		this->setPosition(ccp(xOffset, currentSpritePosition.y));
	else if (currentSpritePosition.x > (levelSize.width - xOffset))
		this->setPosition(ccp((levelSize.width - xOffset), currentSpritePosition.y));
}
