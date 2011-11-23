#include "BackgroundLayer.h"

using namespace cocos2d;

bool BackgroundLayer::init()
{
	if(CCLayer::init())
	{
		CCSprite *backgroundImage = CCSprite::spriteWithFile("backgroundiPhone.png");
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		backgroundImage->setPosition(ccp(screenSize.width/2, screenSize.height/2));

		this->addChild(backgroundImage, 0, 0);
	}

	return true;
}