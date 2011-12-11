#include "GameManager.h"
#include "Scenes\GameScene.h"
#include "Scenes\MainMenu\MainMenuScene.h"
#include "Scenes\Options\OptionsScene.h"
#include "Scenes\Intro\IntroScene.h"
#include "Scenes\LevelComplete\LevelCompleteScene.h"

using namespace cocos2d;
using namespace std;

static GameManager *_sharedGameManager = NULL;
static bool s_bFirstRun = true;

GameManager::~GameManager()
{
	// safe release for sharedGameManager
	CC_SAFE_RELEASE_NULL(_sharedGameManager);
	CC_SAFE_RELEASE_NULL(listOfSoundEffectFiles);
	CC_SAFE_DELETE(soundEffectsState);
}

GameManager::GameManager()
{
	listOfSoundEffectFiles = NULL;
	soundEffectsState = NULL;
}

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
	hasFinishedLoading = false;
	currentScene = kNoSceneUninitialized;
	hasAudioBeenInitialized = false;
	soundEngine = NULL;
	managerSoundState = kAudioManagerUninitialized;

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
		sceneToRun = GameScene::node();
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

	//Load audio
	this->loadAudioForSceneWithID(currentScene);

	if (CCDirector::sharedDirector()->getRunningScene() == NULL)
		CCDirector::sharedDirector()->runWithScene(sceneToRun);
	else
		CCDirector::sharedDirector()->replaceScene(sceneToRun);

	//Unload old scene's audio
	this->unloadAudioForSceneWithID(oldScene);

	currentScene = sceneID;
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

void GameManager::setUpAudioEngine()
{
	if (hasAudioBeenInitialized == true)
		return;

	else
	{
		hasAudioBeenInitialized = true;
	}
}

void GameManager::initAudioAsync()
{

}

std::string GameManager::formatSceneTypeToString(SceneTypes sceneID)
{
	std::string result = "";
	switch(sceneID)
	{
	case kNoSceneUninitialized:
		result = "kNoSceneUninitialized";
		break;
	case kMainMenuScene:
		result = "kMainMenuScene";
		break;
	case kOptionsScene:
		result = "kOptionsScene";
		break;
	case kCreditsScene:
		result = "kCreditsScene";
		break;
	case kIntroScene:
		result = "kIntroScene";
		break;
	case kLevelCompleteScene:
		result = "kLevelCompleteScene";
		break;
	case kGameLevel1:
		result = "kGameLevel1";
		break;
	case kGameLevel2:
		result = "kGameLevel2";
		break;
	case kGameLevel3:
		result = "kGameLevel3";
		break;
	case kGameLevel4:
		result = "kGameLevel4";
		break;
	case kGameLevel5:
		result = "kGameLevel5";
		break;
	case kCutSceneForLevel2:
		result = "kCutSceneForLevel2";
		break;
	default:
		CCLOG("Wrong SceneType");
		break;
	}

	return result;
}

CCDictionary<std::string, CCString*> * GameManager::getSoundEffectsListForSceneWithID(SceneTypes sceneID)
{
	const char *plistPath = CCFileUtils::fullPathFromRelativePath("Plists/SoundEffects.plist");

	CCDictionary<std::string, CCObject*> *plistDictionary = CCFileUtils::dictionaryWithContentsOfFile(plistPath);

	if (plistDictionary == NULL) {
		CCLOG("Error reading SoundEffects.plist");
		return NULL; // No Plist Dictionary or file found
	}

	if (listOfSoundEffectFiles == NULL || listOfSoundEffectFiles->count() < 1)
	{
		CCLOG("Before");
		listOfSoundEffectFiles = new cocos2d::CCDictionary<std::string, CCString*>();
		CCLOG("After");
		plistDictionary->begin();

		std::string key = "";
		CCDictionary<std::string, CCString*> *val = NULL;

		while( (val =  (CCDictionary<std::string, CCString*> *) plistDictionary->next(&key)) )
		{
			val->begin();

			std::string valKey = "";
			CCString * valVal = NULL;

			while ( (valVal = val->next(&valKey)) )
			{
				listOfSoundEffectFiles->setObject(valVal, valKey);
			}

			val->end();			

			listOfSoundEffectFiles->retain();
		}

		plistDictionary->end();

		CCLOG("Number of SFX filenames: %d", listOfSoundEffectFiles->count());
		
	}

	if (soundEffectsState == NULL || soundEffectsState->size() < 1)
	{
		soundEffectsState = new map<std::string, bool>();
		listOfSoundEffectFiles->begin();

		std::string key = "";
		CCString *val = NULL;

		while( (val =  listOfSoundEffectFiles->next(&key)) )
		{
			if (soundEffectsState->insert(pair<std::string, bool> (key, SFX_NOTLOADED)).second != true)
			{
				soundEffectsState->erase(key);
				soundEffectsState->insert(pair<std::string, bool> (key, SFX_NOTLOADED));
			}
		}

		listOfSoundEffectFiles->end();
	}

	std::string sceneIDName = formatSceneTypeToString(sceneID);
	CCDictionary<std::string, CCString*> *soundEffectsList = (CCDictionary<std::string, CCString*> *) plistDictionary->objectForKey(sceneIDName);

	return soundEffectsList;
}

void GameManager::loadAudioForSceneWithID(SceneTypes sceneID)
{
	CCDictionary<std::string, CCString*> *soundEffectsToLoad = this->getSoundEffectsListForSceneWithID(sceneID);
	
	if (soundEffectsToLoad == NULL) 
	{ 
		CCLOG("Error reading SoundEffects.plist");
		return;
	}

	soundEffectsToLoad->begin();

	std::string keyString = "";
	CCString *val = NULL;

	while ( (val = soundEffectsToLoad->next(&keyString)) )
	{
		CCLOG("\nLoading Audio Key: %s	File: %s", keyString.c_str(), val->toStdString().c_str());
		char effectName[100] = {0};
		sprintf(effectName, "Sounds/22k_SFX/%s", val->toStdString().c_str());
		soundEngine->preloadEffect(effectName);
		CCString *loaded = new CCString("1");
		if (soundEffectsState->insert(pair<std::string, bool> (keyString, SFX_LOADED)).second != true)
		{
			soundEffectsState->erase(keyString);
			soundEffectsState->insert(pair<std::string, bool> (keyString, SFX_LOADED));
		}
	}

	soundEffectsToLoad->end();
}

void GameManager::unloadAudioForSceneWithID(SceneTypes sceneID)
{
	CCDictionary<std::string, CCString*> *soundEffectsToUnload = this->getSoundEffectsListForSceneWithID(sceneID);

	if (soundEffectsToUnload == NULL) 
	{ 
		CCLOG("Error reading SoundEffects.plist");
		return;
	}

	soundEffectsToUnload->begin();

	std::string keyString = "";
	CCString *val = NULL;

	while ( (val = soundEffectsToUnload->next(&keyString)) )
	{
		if (soundEffectsState->insert(pair<std::string, bool> (keyString, SFX_NOTLOADED)).second != true)
		{
			soundEffectsState->erase(keyString);
			soundEffectsState->insert(pair<std::string, bool> (keyString, SFX_NOTLOADED));
		}
		CCLOG("\nUnloading Audio Key: %s	File: %s", keyString.c_str(), val->toStdString().c_str());
		char effectName[100] = {0};
		sprintf(effectName, "Sounds/22k_SFX/%s", val->toStdString().c_str());
		soundEngine->unloadEffect(effectName);	
	}

	soundEffectsToUnload->end();
}

void GameManager::playBackgroundTrack(const char *trackFileName) 
{
	if (soundEngine->isBackgroundMusicPlaying()) 
	{
		soundEngine->stopBackgroundMusic();
	}

	char filePath[100] = {0};
	sprintf(filePath, "Sounds/Music/%s", trackFileName);
	soundEngine->preloadBackgroundMusic(filePath);
	if (isMusicON == true)
		soundEngine->playBackgroundMusic(filePath, true);
}

void GameManager::stopSoundEffect(int soundEffectID) 
{
		soundEngine->stopEffect(soundEffectID);
}

int GameManager::playSoundEffect(const char *soundEffectKey) 
{
	int soundID = 0;
	bool isSFXLoaded = false;
	map<std::string, bool>::iterator it = soundEffectsState->find(std::string(soundEffectKey));

	if (it != soundEffectsState->end())
	{
		isSFXLoaded = it->second;
	}

	if (isSFXLoaded == SFX_LOADED) 
	{
		char filePath[100] = {0};
		sprintf(filePath, "Sounds/22k_SFX/%s", listOfSoundEffectFiles->objectForKey(std::string(soundEffectKey))->toStdString().c_str());
		if (isSoundEffectsON == true)
			soundID = soundEngine->playEffect(filePath);
	} else 
	{
		CCLOG("GameMgr: SoundEffect % is not loaded.", soundEffectKey);
	}
	return soundID;
}