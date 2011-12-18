#include "MainMenuLayer.h"
#include "EPlibs\EPNotificationCenter.h"

using namespace cocos2d;

MainMenuLayer::MainMenuLayer()
{
	mainMenu = NULL;
	sceneSelectMenu = NULL;
}

void MainMenuLayer::buyBook(CCObject* pSender)
{
	GameManager::sharedGameManager()->openSiteWithLinkType(kLinkTypeBookSite);
}

void MainMenuLayer::showOptions(CCObject* pSender)
{
	CCLOG("Show the Options screen");
	GameManager::sharedGameManager()->runSceneWithID(kOptionsScene);
}

void MainMenuLayer::playScene(CCObject *pSender)
{
	if (GameManager::sharedGameManager()->getHasFinishedLoading() == true)
	{
		CCMenuItemFont * itemPassedIn = (CCMenuItemFont *) pSender;
		if (itemPassedIn->getTag() == 1) 
		{
			CCLOG("Tag 1 found, Scene 1");
			GameManager::sharedGameManager()->runSceneWithID(kIntroScene);
		} 
		else if (itemPassedIn->getTag() == 2) 
		{
			CCLOG("Tag 2 found, Scene 2");
			GameManager::sharedGameManager()->runSceneWithID(kCutSceneForLevel2);
		}
		else if (itemPassedIn->getTag() == 3) 
		{
			CCLOG("Tag 3 found, Scene 3");
			GameManager::sharedGameManager()->runSceneWithID(kGameLevel3);
		}
		else if (itemPassedIn->getTag() == 4) 
		{
			CCLOG("Tag 4 found, Scene 4");
			GameManager::sharedGameManager()->runSceneWithID(kGameLevel4);
		}
		else if (itemPassedIn->getTag() == 5) 
		{
			CCLOG("Tag 5 found, Scene 5");
			GameManager::sharedGameManager()->runSceneWithID(kGameLevel5);
		}
		else
		{
			CCLOG("Unexpected item. Tag was: %d", itemPassedIn->getTag());
		}

		// IMPORTANT: removeObserver when changing scene. What about when quitting without playing? Need to implement that?
		EPNotificationCenter::sharedNotifCenter()->removeObserver(this, kResourceLoadedNotif);
	}
}

void MainMenuLayer::displayMainMenu(CCObject* pSender)
{
	if (sceneSelectMenu != NULL)
	{
		sceneSelectMenu->removeFromParentAndCleanup(true);
	}
	/*
	// Main Menu with Buttons
	CCMenuItemImage *playGameButton = CCMenuItemImage::itemFromNormalImage("Menus/Main/PlayGameButtonNormal.png", "Menus/Main/PlayGameButtonSelected.png", NULL, this, menu_selector(MainMenuLayer::displaySceneSelection));
	CCMenuItemImage *buyBookButton = CCMenuItemImage::itemFromNormalImage("Menus/Main/BuyBookButtonNormal.png", "Menus/Main/BuyBookButtonSelected.png", NULL, this, menu_selector(MainMenuLayer::buyBook));
	CCMenuItemImage *optionsButton = CCMenuItemImage::itemFromNormalImage("Menus/Main/OptionsButtonNormal.png", "Menus/Main/OptionsButtonSelected.png", NULL, this, menu_selector(MainMenuLayer::showOptions));
	*/

	//Main Menu With Font
	CCLabelBMFont *playGameButtonLabel = CCLabelBMFont::labelWithString("PLAY!", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *playGameButton = CCMenuItemLabel::itemWithLabel(playGameButtonLabel, this, menu_selector(MainMenuLayer::displaySceneSelection));

	CCLabelBMFont *buyBookButtonLabel = CCLabelBMFont::labelWithString("       BUY\nTHE BOOK", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *buyBookButton = CCMenuItemLabel::itemWithLabel(buyBookButtonLabel, this, menu_selector(MainMenuLayer::buyBook));

	CCLabelBMFont *optionsButtonLabel = CCLabelBMFont::labelWithString("OPTIONS", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *optionsButton = CCMenuItemLabel::itemWithLabel(optionsButtonLabel, this, menu_selector(MainMenuLayer::showOptions));

	mainMenu = CCMenu::menuWithItems(playGameButton, buyBookButton, optionsButton, NULL);

	mainMenu->alignItemsVerticallyWithPadding(SCREEN_HEIGHT * 0.059f);
	mainMenu->setPosition(ccp(SCREEN_WIDTH * 2, SCREEN_HEIGHT/2));

	CCActionInterval *moveAction = CCMoveTo::actionWithDuration(0.5f, ccp(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT/2));
	CCEaseIn *moveEffect = CCEaseIn::actionWithAction(moveAction, 1.0f);

	mainMenu->runAction(moveEffect);
	this->addChild(mainMenu, 0, kMainMenuTagValue);
}

void MainMenuLayer::displaySceneSelection(CCObject* pSender)
{
	if (mainMenu != NULL)
	{
		mainMenu->removeFromParentAndCleanup(true);
	}

	CCLabelBMFont *playScene1Label = CCLabelBMFont::labelWithString("Level 1", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *playScene1 = CCMenuItemLabel::itemWithLabel(playScene1Label, this, menu_selector(MainMenuLayer::playScene));
	playScene1->setTag(1);

	CCLabelBMFont *playScene2Label = CCLabelBMFont::labelWithString("Level 2", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *playScene2 = CCMenuItemLabel::itemWithLabel(playScene2Label, this, menu_selector(MainMenuLayer::playScene));
	playScene2->setTag(2);

	CCLabelBMFont *playScene3Label = CCLabelBMFont::labelWithString("Level 3", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *playScene3 = CCMenuItemLabel::itemWithLabel(playScene3Label, this, menu_selector(MainMenuLayer::playScene));
	playScene3->setTag(3);

	CCLabelBMFont *playScene4Label = CCLabelBMFont::labelWithString("Level 4", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *playScene4 = CCMenuItemLabel::itemWithLabel(playScene4Label, this, menu_selector(MainMenuLayer::playScene));
	playScene4->setTag(4);

	CCLabelBMFont *playScene5Label = CCLabelBMFont::labelWithString("Level 5", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *playScene5 = CCMenuItemLabel::itemWithLabel(playScene5Label, this, menu_selector(MainMenuLayer::playScene));
	playScene5->setTag(5);

	CCLabelBMFont *backButtonLabel = CCLabelBMFont::labelWithString("Back", "Fonts/VikingSpeechFont64.fnt");
	CCMenuItemLabel *backButton = CCMenuItemLabel::itemWithLabel(backButtonLabel, this, menu_selector(MainMenuLayer::displayMainMenu));

	sceneSelectMenu = CCMenu::menuWithItems(playScene1, playScene2, playScene3, playScene4, playScene5, backButton, NULL);

	sceneSelectMenu->alignItemsVerticallyWithPadding(SCREEN_HEIGHT * 0.059f);
	sceneSelectMenu->setPosition(ccp(SCREEN_WIDTH * 2, SCREEN_HEIGHT/2));

	CCActionInterval *moveAction = CCMoveTo::actionWithDuration(0.5f, ccp(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT/2));
	CCEaseIn *moveEffect = CCEaseIn::actionWithAction(moveAction, 1.0f);

	sceneSelectMenu->runAction(moveEffect);
	this->addChild(sceneSelectMenu, 1, kSceneMenuTagValue);
}

// IMPORTANT: Fix up this function
void MainMenuLayer::loadResourcesAync()
{
	if (GameManager::sharedGameManager()->getHasFinishedLoading() == false)
	{
		/*
		CCTextureCache::sharedTextureCache()->addImageAsync("scene1atlasiPhone-hd.png", this, callfuncO_selector(MainMenuLayer::loadingCallBack));
		CCTextureCache::sharedTextureCache()->addImageAsync("ParallaxBackgrounds/ScrollingCloudsTextureAtlases/ScrollingCloudsTextureAtlasiPhone-hd.png", this, callfuncO_selector(MainMenuLayer::loadingCallBack));
		CCTextureCache::sharedTextureCache()->addImageAsync("ParallaxBackgrounds/chap9_scrolling4iPhone-hd.png", this, callfuncO_selector(MainMenuLayer::loadingCallBack));
		CCTextureCache::sharedTextureCache()->addImageAsync("ParallaxBackgrounds/chap9_scrolling2iPhone-hd.png", this, callfuncO_selector(MainMenuLayer::loadingCallBack));
		CCTextureCache::sharedTextureCache()->addImageAsync("ParallaxBackgrounds/chap9_scrolling3iPhone-hd.png", this, callfuncO_selector(MainMenuLayer::loadingCallBack));
			*/	
		
		EPResourceManager::sharedResourceManager()->addPngResourceAsync("ParallaxBackgrounds/ScrollingCloudsTextureAtlases/ScrollingCloudsTextureAtlasiPhone-hd",false);
		EPResourceManager::sharedResourceManager()->addPngResourceAsync("scene1atlasiPhone-hd",false);
		EPResourceManager::sharedResourceManager()->addPngResourceAsync("ParallaxBackgrounds/chap9_scrolling4iPhone-hd",false);
		EPResourceManager::sharedResourceManager()->addPngResourceAsync("ParallaxBackgrounds/chap9_scrolling2iPhone-hd",false);
		EPResourceManager::sharedResourceManager()->addPngResourceAsync("ParallaxBackgrounds/chap9_scrolling3iPhone-hd",false);
		EPResourceManager::sharedResourceManager()->loadResourceAsync();    
		
	}
}

void MainMenuLayer::loadingCallBack(CCObject *psender)
{
	GameManager::sharedGameManager()->getLoadingLabel()->setString("Loading Done!");
	GameManager::sharedGameManager()->setHasFinishedLoading(true);
}

bool MainMenuLayer::init()
{
	bool pRet = false;
	if (CCLayer::init())
	{
		CCSprite *background = CCSprite::spriteWithFile("Menus/Main/MainMenuBackground.png");
		background->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		this->addChild(background);
		this->displayMainMenu(NULL);

		// So that the Loading Done! label is only shown the first time the loading is finished
		if (GameManager::sharedGameManager()->getHasFinishedLoading() == false)
			GameManager::sharedGameManager()->getLoadingLabel()->setString("Loading...");
		else
			GameManager::sharedGameManager()->getLoadingLabel()->setString("");

		GameManager::sharedGameManager()->getLoadingLabel()->setPosition(ccp(SCREEN_WIDTH * 0.1, SCREEN_HEIGHT / 2));
		this->addChild(GameManager::sharedGameManager()->getLoadingLabel());

		EPNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(MainMenuLayer::loadingCallBack), kResourceLoadedNotif, NULL);

		this->loadResourcesAync();

		GameManager::sharedGameManager()->playBackgroundTrack(BACKGROUND_TRACK_MAIN_MENU);

		this->setIsKeypadEnabled(true);

		pRet = true;
	}

	return pRet;
}

void MainMenuLayer::keyBackClicked()
{
	EPResourceManager::purgeSharedResourceManager();
	EPNotificationCenter::purgeSharedNotifCenter();
	GameManager::purgeSharedGameManager();
	CCDirector::sharedDirector()->end();
}
