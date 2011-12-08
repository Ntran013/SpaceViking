#include "LevelCompleteScene.h"

using namespace cocos2d;

bool LevelCompleteScene::init()
{
	bool pRet = false;
	if (CCScene::init())
	{
		LevelCompleteLayer *myLayer = LevelCompleteLayer::node();
		this->addChild(myLayer);
		pRet = true;
	}

	return pRet;
}