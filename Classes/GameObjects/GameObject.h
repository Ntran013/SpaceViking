#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "Constants\CommonProtocols.h"
#include "Constants\StringSplit.h"
#include "Singletons\GameManager.h"

class GameObject : public cocos2d::CCSprite 
{
	CC_SYNTHESIZE(bool, isActive, IsActive);
	CC_SYNTHESIZE(bool, reactsToScreenBoundaries, ReactsToScreenBoundaries);
	CC_SYNTHESIZE(cocos2d::CCSize, screenSize, ScreenSize);
	CC_SYNTHESIZE(GameObjectType, gameObjectType, GameObjectType);

	//public methods
	virtual bool init();
	virtual void changeState(CharacterStates newState);
	virtual void updateStateWithDeltaTime(cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects);
	virtual cocos2d::CCRect adjustedBoundingBox();
	cocos2d::CCAnimation *loadPlistForAnimationWithName(const char *animationName, const char *className);
	void setObjectScale();
};

#endif