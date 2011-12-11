#include "MainMenuScene.h"

using namespace cocos2d;

bool MainMenuScene::init()
{
	bool pRet = false;
	if (CCScene::init())
	{
		mainMenuLayer = MainMenuLayer::node();
		this->addChild(mainMenuLayer);

		pRet = true;
	}

	return pRet;
}

