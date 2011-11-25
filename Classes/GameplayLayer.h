#ifndef __G_LAYER_H__
#define __G_LAYER_H__

#include "cocos2d.h"
#include "JoystickClasses\SneakyButton.h"
#include "JoystickClasses\SneakyButtonSkinnedBase.h"
#include "JoystickClasses\SneakyJoystick.h"
#include "JoystickClasses\SneakyJoystickSkinnedBase.h"

class GameplayLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();
    LAYER_NODE_FUNC(GameplayLayer);
	void initJoystickAndButtons();
	void applyJoystick(SneakyJoystick *aJoystick, cocos2d::CCNode *tempNode, float deltaTime);
	void update(cocos2d::ccTime deltaTime);
protected:
	cocos2d::CCSprite *vikingSprite;
	SneakyJoystick *lefJoystick;
	SneakyButton *jumpButton;
	SneakyButton *attackButton;
};
#endif