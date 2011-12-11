#include "LevelCompleteLayer.h"

using namespace cocos2d;

void LevelCompleteLayer::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
	if (hasBeenSkipped == false)
	{
		CCLOG("Touches received, returning to the Main Menu");
		GameManager::sharedGameManager()->setHasPlayerDied(false); // Reset this for the next level
		GameManager::sharedGameManager()->runSceneWithID(kMainMenuScene);
		hasBeenSkipped == true;
	}

}

bool LevelCompleteLayer::init()
{
	bool pRet = false;
	if (CCLayer::init())
	{
		// Accept touch input
		this->setIsTouchEnabled(true);

		hasBeenSkipped = false;

		// If Viking is dead, reset him and show the tombstone,
		// Any touch gets you back to the main menu
		bool didPlayerDie = GameManager::sharedGameManager()->getHasPlayerDied();
		CCSprite *background = NULL;
		if (didPlayerDie) {
			background = CCSprite::spriteWithFile("Menus/LevelComplete/LevelCompleteDead.png");
		} else {
			background = CCSprite::spriteWithFile("Menus/LevelComplete/LevelCompleteAlive.png");
		}

		background->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		this->addChild(background);

		// Add the text for level complete.
		CCLabelBMFont *levelLabelText = CCLabelBMFont::labelWithString("Level Complete", "Fonts/VikingSpeechFont64.fnt");
		levelLabelText->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT * 0.9f));
		this->addChild(levelLabelText);

		char killNumber[100] = {0};
		sprintf(killNumber, "Kill Count: %d", GameManager::sharedGameManager()->getKillCount());
		CCLabelBMFont *killCountText = CCLabelBMFont::labelWithString(killNumber, "Fonts/VikingSpeechFont64.fnt");
		killCountText->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT * 0.6f));
		this->addChild(killCountText);
		GameManager::sharedGameManager()->setKillCount(0);
		pRet = true;
	}

	return pRet;
}