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
	if (SCREEN_SIZE_PX.height < 720)
	{
		this->setScaleY(SCREEN_SIZE_PX.height/640);
		this->setScaleX(SCREEN_SIZE_PX.width/960);
	}

	else if (SCREEN_SIZE_PX.height == 720)
	{
		this->setScaleY(SCREEN_SIZE_PX.height/640);
		this->setScaleX(SCREEN_SIZE_PX.width/960);
	}
}