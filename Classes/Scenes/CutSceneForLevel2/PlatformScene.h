#ifndef __PLATFORM_SCENE_H__ 
#define __PLATFORM_SCENE_H__

#include "cocos2d.h"
#include "PlatformScrollingLayer.h"

class PlatformScene : public cocos2d::CCScene
{
public:
	SCENE_NODE_FUNC(PlatformScene);
	bool init();
};

#endif