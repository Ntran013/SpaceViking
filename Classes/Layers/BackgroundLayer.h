#ifndef __B_LAYER_H__
#define __B_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
class BackgroundLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();
    LAYER_NODE_FUNC(BackgroundLayer);
	void setLayerScale();
};
#endif

