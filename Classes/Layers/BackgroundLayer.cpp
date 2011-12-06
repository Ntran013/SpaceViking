#include "BackgroundLayer.h"

using namespace cocos2d;

bool BackgroundLayer::init()
{
	if(CCLayer::init())
	{
		CCSprite *backgroundImage = CCSprite::spriteWithFile("backgroundiPhone-hd.png");
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		backgroundImage->setPosition(ccp(screenSize.width/2, screenSize.height/2));
		
		//Scaling
		this->setLayerScale();

		this->addChild(backgroundImage, 0, 0);
	}

	return true;
}

void BackgroundLayer::setLayerScale()
{
	if (SCREEN_HEIGHT == 480)
	{
		this->setScaleY(W480Scale);
		this->setScaleX(W480Scale);
	}

	else if (SCREEN_HEIGHT == 320)
	{
		this->setScaleY(W320Scale);
		this->setScaleX(W320Scale);
	}
}