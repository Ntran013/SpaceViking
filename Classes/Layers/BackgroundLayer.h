#ifndef __B_LAYER_H__
#define __B_LAYER_H__

#include "cocos2d.h"
class BackgroundLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();
    LAYER_NODE_FUNC(BackgroundLayer);
};
#endif

