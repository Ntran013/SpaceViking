#include "GameManager.h"
#include "Scenes\GameScene.h"
#include "Scenes\MainMenu\MainMenuScene.h"
#include "Scenes\Options\OptionsScene.h"
#include "Scenes\CreditsScene.h"
#include "Scenes\Intro\IntroScene.h"
#include "Scenes\LevelComplete\LevelCompleteScene.h"

static GameManager *_sharedGameManager = NULL;
static bool s_bFirstRun = true;

GameManager * GameManager::sharedGameManager()
{
	if (s_bFirstRun)
	{
		if (!_sharedGameManager)
		{
			_sharedGameManager = new GameManager();
			_sharedGameManager->init();
		}
        s_bFirstRun = false;
	}
	return _sharedGameManager;
}

bool GameManager::init()
{
	CCLOG("Game Manager Singleton, init");
	isMusicON = true;
	isSoundEffectsON = true;
	hasPlayerDied = false;
	killCount = 0;
	currentScene = kNoSceneUninitialized;

	return true;
}

void GameManager::runSceneWithID(SceneTypes sceneID)
{
	SceneTypes oldScene = currentScene;
	currentScene = sceneID;
	CCScene *sceneToRun = NULL;

	switch (sceneID)
	{
	case kMainMenuScene:
		sceneToRun = MainMenuScene::node();
		break;
	case kOptionsScene:
		sceneToRun = OptionsScene::node();
		break;
	case kCreditsScene:
		//sceneToRun = CreditsScene::node();
		break;
	case kIntroScene:
		sceneToRun = IntroScene::node();
		break;
	case kLevelCompleteScene:
		sceneToRun = LevelCompleteScene::node();
		break;
	case kGameLevel1:
		sceneToRun = GameScene ::node();
		break;
	case kGameLevel2:
		// Placeholder for Level 2
		break;
	case kGameLevel3:
		// Placeholder for Level 3
		break;
	case kGameLevel4:
		// Placeholder for Level 4
		break;
	case kGameLevel5:
		// Placeholder for Level 5
		break;
	case kCutSceneForLevel2:
		// Placeholder for Platform Level
		break;
	default:
		CCLOG("Unknown ID, cannot switch scenes");
		return;
		break;
	}

	if (sceneToRun == NULL)
	{
		currentScene = oldScene;
		return;
	}

	if (sceneID < 100)
	{
		//If is not an iPad
		sceneToRun->setScaleX(SCREEN_SIZE_PX.width/1024);
		sceneToRun->setScaleY(SCREEN_SIZE_PX.height/768);
	}

	if (CCDirector::sharedDirector()->getRunningScene() == NULL)
		CCDirector::sharedDirector()->runWithScene(sceneToRun);
	else
		CCDirector::sharedDirector()->replaceScene(sceneToRun);
}

void GameManager::openSiteWithLinkType(LinkTypes linkTypeToOpen)
{
	if (linkTypeToOpen == kLinkTypeBookSite)
	{
		CCLOG("Opening Book Site");
		CCApplication::openURL("http://www.informit.com/title/9780321735621");
	}

	else if (linkTypeToOpen == kLinkTypeDeveloperSiteRod)
	{
		CCLOG("Opening Developer Site for Rod");
		CCApplication::openURL("http://www.prop.gr");
	}

	else if (linkTypeToOpen == kLinkTypeDeveloperSiteRay)
	{
		CCLOG("Opening Developer Site for Ray");
		CCApplication::openURL("http://www.raywenderlich.com/");
	}

	else if (linkTypeToOpen == kLinkTypeArtistSite)
	{
		CCLOG("Opening Artist Site");
		CCApplication::openURL("http://EricStevensArt.com");
	}

	else if (linkTypeToOpen == kLinkTypeMusicianSite)
	{
		CCLOG("Opening Musician Site");
		CCApplication::openURL("http://www.mikeweisermusic.com/");
	}

	else
	{
		CCLOG("Defaulting to Cocos2DBook.com Blog Site");
		CCApplication::openURL("http://www.cocos2dbook.com");
	}
}