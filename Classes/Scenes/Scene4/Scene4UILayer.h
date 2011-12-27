#ifndef __SCENE4_UI_LAYER_H__
#define __SCENE4_UI_LAYER_H__

#include "cocos2d.h"
#include "Constants/Constants.h"
#include "JoystickClasses\SneakyButton.h"
#include "JoystickClasses\SneakyButtonSkinnedBase.h"

class Scene4UILayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCLabelTTF *label;
	CC_SYNTHESIZE(SneakyButton *, jumpButton, JumpButton);
	CC_SYNTHESIZE(SneakyButton *, attackButton, AttackButton);
public:
	LAYER_NODE_FUNC(Scene4UILayer);
	bool init();
	bool displayText(const char *text, cocos2d::SelectorProtocol *target, cocos2d::SEL_CallFuncN selector);
	void initButtons();
};

#endif