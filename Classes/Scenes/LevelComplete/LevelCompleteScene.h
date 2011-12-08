#ifndef __LEVEL_COMPLETE_SCENE_H__
#define __LEVEL_COMPLETE_SCENE_H__

#include "cocos2d.h"
#include "LevelCompleteLayer.h"

class LevelCompleteScene : public cocos2d::CCScene
{
public:
	SCENE_NODE_FUNC(LevelCompleteScene);
	bool init();
};
#endif