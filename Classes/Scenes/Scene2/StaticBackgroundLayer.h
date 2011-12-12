#ifndef __STATIC_BACKGROUND_LAYER_H__
#define __STATIC_BACKGROUND_LAYER_H__

#include "cocos2d.h"
#include "Constants/Constants.h"

class StaticBackgroundLayer : public cocos2d::CCLayer
{
public:
	bool init();
	LAYER_NODE_FUNC(StaticBackgroundLayer);
	void setLayerScale();
};

#endif