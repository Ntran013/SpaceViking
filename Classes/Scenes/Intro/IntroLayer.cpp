#include "IntroLayer.h"

using namespace cocos2d;

void IntroLayer::startGamePlay()
{
	CCLOG("Intro complete, asking Game Manager to start the Game play");
	GameManager::sharedGameManager()->runSceneWithID(kGameLevel1);	
}

void IntroLayer::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
	CCLOG("Touches received, skipping intro");
	this->startGamePlay();
}

bool IntroLayer::init()
{
	bool pRet = false;
	if (CCLayer::init())
	{
		// Accept touch input
		this->setIsTouchEnabled(true);

		// Create the intro image
		CCSprite *introImage = CCSprite::spriteWithFile("Menus/Intro/intro1.png");
		introImage->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		this->addChild(introImage);

		// Create the intro animation, and load it from intro1 to intro7.png
		CCAnimation *introAnimation = CCAnimation::animation();
        introAnimation->setDelay(3.5f);

		char frameName[100] = {0};

		for (int frameNumber=1; frameNumber < 8; frameNumber++) 
		{
			CCLOG("Adding image intro%d.png to the introAnimation.",frameNumber);
			sprintf(frameName, "Menus/Intro/intro%d.png", frameNumber);
			introAnimation->addFrameWithFileName(frameName);
		}

		// Create the actions to play the intro
		CCFiniteTimeAction *animationAction = CCAnimate::actionWithAnimation(introAnimation, false);
		CCFiniteTimeAction *startGameAction = CCCallFunc::actionWithTarget(this, callfunc_selector(IntroLayer::startGamePlay));
		CCFiniteTimeAction *introSequence = CCSequence::actions(animationAction, startGameAction, NULL);
		
		introImage->runAction(introSequence);
		pRet = true;
	}

	return pRet;
}