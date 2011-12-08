#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "MainMenuLayer.h"

class MainMenuScene : public cocos2d::CCScene
{
protected:
	MainMenuLayer *mainMenuLayer;
	//Public methods
public:
	SCENE_NODE_FUNC(MainMenuScene);
	bool init();
};
#endif