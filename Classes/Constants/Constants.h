#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// Constants.h
// Constants used in SpaceViking

#define kVikingSpriteZValue 100
#define kVikingSpriteTagValue 0
#define kVikingIdleTimer 3.0f
#define kVikingFistDamage 10
#define kVikingMalletDamage 40
#define kRadarDishTagValue 10

// Screen size marcros
#define SCREEN_SIZE cocos2d::CCDirector::sharedDirector()->getWinSize()
#define SCREEN_WIDTH cocos2d::CCDirector::sharedDirector()->getWinSize().width
#define SCREEN_HEIGHT cocos2d::CCDirector::sharedDirector()->getWinSize().height

#define SCREEN_SIZE_PX cocos2d::CCDirector::sharedDirector()->getWinSizeInPixels()

// SCreen Resolutions for AppDelegate
#define WVGA 800,480
#define HVGA 480,320
#define RETINA 960,640
#define GALAXYNEXUS 1280,720
#define IPAD 1024,768

// Game Manager
#define kMainMenuTagValue 10
#define kSceneMenuTagValue 20

enum SceneTypes{
	kNoSceneUninitialized=0,
	kMainMenuScene=1,
	kOptionsScene=2,
	kCreditsScene=3,
	kIntroScene=4,
	kLevelCompleteScene=5,
	kGameLevel1=101,
	kGameLevel2=102,
	kGameLevel3=103,
	kGameLevel4=104,
	kGameLevel5=105,
	kCutSceneForLevel2=201
};

enum LinkTypes{
	kLinkTypeBookSite,
	kLinkTypeDeveloperSiteRod,
	kLinkTypeDeveloperSiteRay,
	kLinkTypeArtistSite,
	kLinkTypeMusicianSite
};

// Debug Enemy States with Labels
// 0 for OFF, 1 for ON
#define ENEMY_STATE_DEBUG 0

#define AUDIO_MAX_WAITTIME 150

enum GameManagerSoundState{
	kAudioManagerUninitialized=0,
	kAudioManagerFailed=1,
	kAudioManagerInitializing=2,
	kAudioManagerInitialized=100,
	kAudioManagerLoading=200,
	kAudioManagerReady=300
};

// Not sure??
#define SFX_NOTLOADED false
#define SFX_LOADED true

#define PLAYSOUNDEFFECT(...) \
	GameManager::sharedGameManager()->playSoundEffect(#__VA_ARGS__)

#define STOPSOUNDEFFECT(...) \
	GameManager::sharedGameManager()->stopSoundEffect(__VA_ARGS__)

// Background Music
// Menu Scenes
#define BACKGROUND_TRACK_MAIN_MENU "VikingPreludeV1.mp3"
// GameLevel1 (Ole Awakens)
#define BACKGROUND_TRACK_OLE_AWAKES "SpaceDesertV2.mp3"
// Physics Puzzle Level
#define BACKGROUND_TRACK_PUZZLE "VikingPreludeV1.mp3"
// Physics MineCart Level
#define BACKGROUND_TRACK_MINECART "DrillBitV2.mp3"
// Physics Escape Level
#define BACKGROUND_TRACK_ESCAPE "EscapeTheFutureV3.mp3"

// Defines for Cloud Scrolling Scene
#define kMaxCloudMoveDuration 10
#define kMinCloudMoveDuration 1

#define PTM_RATIO 50.0

#endif