#include "GameScene2.h"

using namespace cocos2d;

bool GameScene2::init()
{
	bool pRet = false;

	if(CCScene::init())
	{
		CCLOG("LOADING");
		StaticBackgroundLayer *backgroundLayer = backgroundLayer->node();
		this->addChild(backgroundLayer, 0);

		controlLayer = controlLayer->node();
		this->addChild(controlLayer, 2);

		GameplayScrollingLayer *scrollingLayer = scrollingLayer->node();
		scrollingLayer->connectControlsWithJoystick(controlLayer->getLeftJoystick(), controlLayer->getJumpButton(), controlLayer->getAttackButton());
		this->addChild(scrollingLayer, 1);
		CCLOG("DONE");
		pRet = true;
	}

	return pRet;
}