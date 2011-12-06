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

// Screen Resolution Scales
#define W480Scale 0.75
#define W320Scale 0.5

// SCreen Resolutions for AppDelegate
#define WVGA 480,800
#define HVGA 320,480
#define RETINA 640,960
#define GALAXYNEXUS 720,1280
#endif