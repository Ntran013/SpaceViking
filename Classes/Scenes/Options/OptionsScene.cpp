#include "OptionsScene.h"

using namespace cocos2d;

bool OptionsScene::init()
{
	bool pRet = false;
	if (CCScene::init())
	{
		OptionsLayer *myLayer = OptionsLayer::node();
		this->addChild(myLayer);
		pRet = true;
	}

	return pRet;
}