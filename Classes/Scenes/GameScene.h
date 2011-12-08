#ifndef __G_SCENE_H__
#define __G_SCENE_H__

#include "cocos2d.h"
#include "Scene1\Layers\BackgroundLayer.h"
#include "Scene1\Layers\GameplayLayer.h"
#include "HelloWorldScene.h"

class GameScene : public cocos2d::CCScene
{
public:
	virtual bool init();
	SCENE_NODE_FUNC(GameScene);
};
#endif