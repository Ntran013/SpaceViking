#ifndef __GAME_SCENE_4_H__
#define __GAME_SCENE_4_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "Scene4UILayer.h"
#include "scene4ActionLayer.h"

class Scene4 : public cocos2d::CCScene
{
public:
	SCENE_NODE_FUNC(Scene4);
	bool init();
};

#endif