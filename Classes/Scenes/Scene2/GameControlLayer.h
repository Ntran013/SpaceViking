#ifndef __GAME_CONTROL_LAYER_H__
#define __GAME_CONTROL_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "JoystickClasses\SneakyJoystick.h"
#include "JoystickClasses\SneakyJoystickSkinnedBase.h"
#include "JoystickClasses\SneakyButton.h"
#include "JoystickClasses\SneakyButtonSkinnedBase.h"

class GameControlLayer : public cocos2d::CCLayer
{
protected:
	CC_SYNTHESIZE(SneakyJoystick *, leftJoystick, LeftJoystick);
	CC_SYNTHESIZE(SneakyButton *, jumpButton, JumpButton);
	CC_SYNTHESIZE(SneakyButton *, attackButton, AttackButton);

public:
	bool init();
	LAYER_NODE_FUNC(GameControlLayer);
	void initJoystickAndButtons();
};

#endif