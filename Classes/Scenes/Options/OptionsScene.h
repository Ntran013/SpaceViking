#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include "cocos2d.h"
#include "OptionsLayer.h"

class OptionsScene : public cocos2d::CCScene
{
public:
	SCENE_NODE_FUNC(OptionsScene);
	bool init();
};
#endif