#include "GameScene.h"

using namespace cocos2d;

bool GameScene::init()
{
	if (CCScene::init())
	{
		BackgroundLayer *backgroundLayer = BackgroundLayer::node();
		this->addChild(backgroundLayer, 0);

		GameplayLayer *gameplayLayer = GameplayLayer::node();
		this->addChild(gameplayLayer, 5);
	}

	return true;
}