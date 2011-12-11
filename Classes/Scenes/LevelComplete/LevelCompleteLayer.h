#ifndef __LEVEL_COMPLETE_LAYER_H__
#define __LEVEL_COMPLETE_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "Singletons\GameManager.h"

class LevelCompleteLayer : public cocos2d::CCLayer
{
protected:
	bool hasBeenSkipped;
public:
	LAYER_NODE_FUNC(LevelCompleteLayer);
	void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	bool init();
};

#endif