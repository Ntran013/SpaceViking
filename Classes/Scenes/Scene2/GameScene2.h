#ifndef __GAME_SCENE_2_H__
#define __GAME_SCENE_2_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "GameControlLayer.h"
#include "GameplayScrollingLayer.h"
#include "StaticBackgroundLayer.h"

class GameScene2 : public cocos2d::CCScene
{
protected:
	GameControlLayer *controlLayer;

public:
	SCENE_NODE_FUNC(GameScene2);
	bool init();
};

#endif