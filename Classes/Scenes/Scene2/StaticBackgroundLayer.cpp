#include "StaticBackgroundLayer.h"

using namespace cocos2d;

bool StaticBackgroundLayer::init() 
{
	bool pRet = false;
	if (CCLayer::init()) 
	{
		CCSprite *backgroundImage;
		backgroundImage = CCSprite::spriteWithFile("ParallaxBackgrounds/chap9_scrolling1iPhone-hd.png");
		backgroundImage->setPosition(ccp(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f));

		this->setLayerScale();

		this->addChild(backgroundImage);
		pRet = true;
	}
	return pRet;
}

void StaticBackgroundLayer::setLayerScale()
{
	this->setScaleY(SCREEN_SIZE_PX.height/640);
	this->setScaleX(SCREEN_SIZE_PX.width/960);
}