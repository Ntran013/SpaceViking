#ifndef __INTRO_LAYER_H__
#define __INTRO_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "Singletons\GameManager.h"

class IntroLayer : public cocos2d::CCLayer
{
public:
	LAYER_NODE_FUNC(IntroLayer);
	void startGamePlay();
	void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	bool init();
};

#endif