#include "PlatformScene.h"

using namespace cocos2d;

bool PlatformScene::init()
{
	bool pRet = false;
	if (CCScene::init())
	{
		PlatformScrollingLayer *scrollingLayer = PlatformScrollingLayer::node();
		this->addChild(scrollingLayer);
		pRet = true;
	}

	return pRet;
}